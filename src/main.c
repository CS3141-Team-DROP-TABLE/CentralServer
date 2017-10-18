#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#include <gnutls/gnutls.h>


#include <clientHandler.h>

#define KEYFILE "NHM_server.key.pem" 
#define CERTFILE "NHM_server.cert.pem" 
#define CAFILE "intermediate.cert.pem" 
#define CRLFILE "intermediate.crl.pem"
#define OCSP_STATUS_FILE "ocsp-status.der"


void *handle_client(void *argsin);

int main(){
  int socket_fd;
  int connection_fd;
  int port_num = 5556;


  socklen_t client_addrlen;
  struct sockaddr_in server_socket, client_socket;



  /* =========================================================================
   * Configure Socket
   * ========================================================================= */  
  memset((char*)&server_socket, 0, sizeof(struct sockaddr_in));
  server_socket.sin_family = AF_INET;
  server_socket.sin_addr.s_addr = INADDR_ANY;
  server_socket.sin_port = htons(port_num);
  
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
  if(socket_fd < 0) {
    fprintf(stderr, "[CentralServer] [CRITICAL ERROR] Error creating socket. Dying  socket_fd: %d\n\n", socket_fd);
    exit(1);
  }
  
  int berror = bind(socket_fd, (struct sockaddr *)&server_socket, sizeof(struct sockaddr_in));
  if(berror < 0){
    
    fprintf(stderr, "[CentralServer] [CRITICAL ERROR] Failed to bind socket. error: %d  %s\n\n", berror, strerror(errno));
    exit(1);
    
  }


  
  gnutls_certificate_credentials_t x509_cred;
  gnutls_priority_t priority_cache;
  gnutls_session_t session;
  

  /* =========================================================================
   * Configure GNUtls
   * ========================================================================= */
  gnutls_global_init();
  if(gnutls_certificate_allocate_credentials(&x509_cred)<0)
    fprintf(stderr, "ERORR On Allocating credentials \n\n");
  if(gnutls_certificate_set_x509_trust_file(x509_cred, CAFILE, GNUTLS_X509_FMT_PEM)<0)
    fprintf(stderr, "ERORR Setting trust file\n\n");
  if(gnutls_certificate_set_x509_key_file(x509_cred, CERTFILE,KEYFILE, GNUTLS_X509_FMT_PEM)<0)
    fprintf(stderr, "ERORR Setting key file\n\n");
  if(gnutls_certificate_set_x509_crl_file(x509_cred, CRLFILE, GNUTLS_X509_FMT_PEM)<0) 
    fprintf(stderr, "ERORR Setting crl\n\n");   
  //if(gnutls_certificate_set_known_dh_params(x509_cred, GNUTLS_SEC_PARAM_MEDIUM)<0)
  //fprintf(stderr, "ERORR setting dh parameters\n");

  
  gnutls_priority_init(&priority_cache, NULL, NULL);
  


  
  /* =========================================================================
   * Start socket listening
   * ========================================================================= */
  listen(socket_fd, 100);

  struct connection_args *args;
  
  while(1){
    args = malloc(sizeof(struct connection_args));
    args->x509_cred = &x509_cred;
    args->priority_cache = &priority_cache;
    args->connection_fd = accept(socket_fd,
			   (struct sockaddr *)&(args->client_socket),
			   &(args->client_addrlen));

    pthread_create(&(args->thread_handle), NULL, handle_client, (void*) args);


  }

  gnutls_certificate_free_credentials(x509_cred);
  gnutls_priority_deinit(priority_cache);

  gnutls_global_deinit();


}
