#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mariadb/mysql.h>

#include <pthread.h>

#include <gnutls/gnutls.h>

#include <linkedList.h>
#include <AVL.h>
#include <stringMap.h>
#include <configLoader.h>

#include <mysql_thread.h>
#include <clientHandler.h>

#define KEYFILE "NHM_server.key.pem" 
#define CERTFILE "NHM_server.cert.pem" 
#define CAFILE "intermediate.cert.pem" 
#define CRLFILE "intermediate.crl.pem"
#define OCSP_STATUS_FILE "ocsp-status.der"



void usage(){
  printf("Usage:\ncentral_server -c <config_file.cfg>\n\n");
  exit(1);
}

void config(struct config *conf, char *filename){
  config_loader_init(conf, 0, 0);
  load_config(conf, filename);
}

int initialize_socket(struct sockaddr_in *server_socket, struct sockaddr_in *client_socket, int port_num){
  int sockfd;  
  socklen_t client_addrlen;

  
  memset((char*)server_socket, 0, sizeof(struct sockaddr_in));
  server_socket->sin_family = AF_INET;
  server_socket->sin_addr.s_addr = INADDR_ANY;
  server_socket->sin_port = htons(port_num);

  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

  int opts = fcntl(sockfd, F_GETFL);
  opts = opts & (~O_NONBLOCK);
  fcntl(sockfd,F_SETFL,opts);

  
  if(sockfd < 0) {
    fprintf(stderr, "[CentralServer] [CRITICAL ERROR] Error creating socket. Dying  sockfd: %d\n\n", sockfd);
    exit(1);
  }
  
  int berror = bind(sockfd, (struct sockaddr *)server_socket, sizeof(struct sockaddr_in));
  if(berror < 0){
    
    fprintf(stderr, "[CentralServer] [CRITICAL ERROR] Failed to bind socket. error: %d  %s\n\n", berror, strerror(errno));
    exit(1);
    
  }

  return sockfd;
}

/* =========================================================================
 * Configure GNUtls
 * ========================================================================= */

void configure_gnutls(struct config *conf, gnutls_certificate_credentials_t *x509_cred, gnutls_priority_t *priority_cache){
  gnutls_global_init();
  if(gnutls_certificate_allocate_credentials(x509_cred)<0)
    fprintf(stderr, "ERORR On Allocating credentials \n\n");
  
  if(gnutls_certificate_set_x509_trust_file(
					    *x509_cred,
					    get_config(conf, "cafile"),
					    GNUTLS_X509_FMT_PEM)<0)
    fprintf(stderr, "ERORR Setting trust file\n\n");


  if(gnutls_certificate_set_x509_key_file(*x509_cred,
					  get_config(conf, "certfile"),
					  get_config(conf, "keyfile"),
					  GNUTLS_X509_FMT_PEM)<0)
    fprintf(stderr, "ERORR Setting key file\n\n");

  
  if(gnutls_certificate_set_x509_crl_file(*x509_cred,
					  get_config(conf, "crlfile"),
					  GNUTLS_X509_FMT_PEM)<0) 
    fprintf(stderr, "ERORR Setting crl\n\n");   


  //if(gnutls_certificate_set_known_dh_params(x509_cred, GNUTLS_SEC_PARAM_MEDIUM)<0)
  //fprintf(stderr, "ERORR setting dh parameters\n");

  
  gnutls_priority_init(priority_cache, NULL, NULL);
}


void *mysql_target_listener(void *argsin){
  struct mysql_th_args *args = (struct mysql_th_args*)argsin;
  MYSQL *mysql = NULL;
  mysql = mysql_init(mysql);
  if(mysql == NULL){
    fprintf(stderr, "Mysql init failure\n");
    return NULL;
  }

  if(get_config(args->conf, "mysql_port") == NULL)
    config_parse_line(args->conf, "mysql_port=0;\n");
  
  if(!mysql_real_connect(mysql,
			 get_config(args->conf, "mysql_host"),
			 get_config(args->conf, "mysql_username"),
			 get_config(args->conf, "mysql_password"),
			 get_config(args->conf, "mysql_db"),
			 atoi(get_config(args->conf, "mysql_port")),
			 NULL, // create your own socket,
			 CLIENT_FOUND_ROWS
			 )){
    fprintf(stderr, "Mysql connect failure\n");
    mysql_close(mysql);
    return NULL;
  }


  
  
  while(args->run){
    if (mysql_query(mysql, "SELECT * FROM target")) {
      printf("Query failed: %s\n", mysql_error(mysql));
    } else {
      MYSQL_RES *result = mysql_store_result(mysql);
      
      if (!result) {
	printf("Couldn't get results set: %s\n", mysql_error(mysql));
      } else {


	MYSQL_ROW row;
        int i;
        unsigned int num_fields = mysql_num_fields(result);
        
        while ((row = mysql_fetch_row(result))) {
	  str_map_insert_str(args->targets, row[0], row[1]);
	  printf("Inserted %s:%s to targets\n", row[0], row[1]);
        }
	
        mysql_free_result(result);
	sleep(60);

      }
    }
  
  }
  mysql_close(mysql);

}



int main(int argc, char **argv){
  int run = 1;
  
  struct config conf;
  
  char *filename = NULL;
  for(int i = 1; i < argc-1; i++){
    if(strncmp("-c", argv[i], 2) == 0 && strnlen(argv[i], 10) == 2)
      filename = argv[i+1];
  }
  if(filename == NULL)
    usage();

  config(&conf, filename);

  char *mip = get_config(&conf, "target_ip_max");
  size_t max_ip = (mip != NULL)? atoi(mip) : 25;
  struct str_map targets;
  str_map_init(&targets, NULL, max_ip);


  struct mysql_th_args my_th_ar;
  my_th_ar.run = &run;
  my_th_ar.conf = &conf;
  my_th_ar.targets = &targets;
  pthread_create(&(my_th_ar.thread_handle), NULL, mysql_target_listener, (void*) &my_th_ar);

  

  
  struct sockaddr_in server_socket, client_socket;
  int sockfd = initialize_socket(&server_socket, &client_socket, atoi(get_config(&conf, "tls_port_num")));
    
  int connection_fd;



  
  gnutls_certificate_credentials_t x509_cred;
  gnutls_priority_t priority_cache;
  
  
  configure_gnutls(&conf, &x509_cred, &priority_cache);
  

  
  /* =========================================================================
   * Start socket listening
   * ========================================================================= */
  listen(sockfd, 100);

  struct connection_args *args;
  
  while(1){
    args = malloc(sizeof(struct connection_args));
    args->x509_cred = &x509_cred;
    args->priority_cache = &priority_cache;
    args->connection_fd = accept(sockfd,
			   (struct sockaddr *)&(args->client_socket),
			   &(args->client_addrlen));

    args->conf = &conf;
    args->targets = &targets;
    
    if(args->connection_fd == EAGAIN || args->connection_fd == EWOULDBLOCK)
      fprintf(stderr, "[Central Server][Error] Socket is nonblocking\n");

    pthread_create(&(args->thread_handle), NULL, handle_client, (void*) args);


  }

  gnutls_certificate_free_credentials(x509_cred);
  gnutls_priority_deinit(priority_cache);

  gnutls_global_deinit();


}
