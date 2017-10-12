
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include <gnutls/gnutls.h>

#define KEYFILE "NHM_server.key.pem" 
#define CERTFILE "NHM_server.cert.pem" 
#define CAFILE "intermediate.cert.pem" 
#define CRLFILE "intermediate.crl.pem"
#define OCSP_STATUS_FILE "ocsp-status.der"

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
  if(gnutls_certificate_set_known_dh_params(x509_cred, GNUTLS_SEC_PARAM_MEDIUM)<0)
    fprintf(stderr, "ERORR setting dh parameters\n");

  
  gnutls_priority_init(&priority_cache, NULL, NULL);
  


  
  /* =========================================================================
   * Start socket listening
   * ========================================================================= */
  listen(socket_fd, 100);

  char buffer[256];
  int n = 0;
  char quit[6];
  memcpy(quit, "quit\n\0", 7);
  while(1){
    connection_fd = accept(socket_fd,
				 (struct sockaddr *)&client_socket,
				 &client_addrlen);

    
    /* =========================================================================
     * Attach GNUtls session to socket
     * ========================================================================= */
    gnutls_init(&session, GNUTLS_SERVER);
    gnutls_priority_set(session, priority_cache);
    gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, x509_cred);
    gnutls_certificate_server_set_request(session, GNUTLS_CERT_IGNORE);
    gnutls_handshake_set_timeout(session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
    gnutls_transport_set_int(session, connection_fd);


    // Handshake
    int ret;
    do {
      ret = gnutls_handshake(session);
      printf("Handshake failed -- retrying\n\n");
    } while (ret < 0 && gnutls_error_is_fatal(ret) == 0);
    if (ret < 0) {
      fprintf(stderr,
	      "[Central Server] TLS handshake failed (%s)\n\n",
	      gnutls_strerror(ret));
      break;
    }
    printf("Handshake completed\n\n");


    

    memset(buffer, 0, 256);
    ret = 1;
    while(memcmp(buffer, quit, 4) != 0  && ret != 0){
      
      printf("%d\n\n", memcmp(buffer, quit, 5) );
      memset(buffer, 0, 256);
      
      do { //recv is non-blocking
	usleep(100);
	ret = gnutls_record_recv(session, buffer, 255);
      }while(ret == GNUTLS_E_AGAIN);

      
      printf("\"%s\"\n", buffer);
      //write(connection_fd, buffer, n);
      gnutls_record_send(session, buffer, ret);
      
    }

    printf("Connection terminated\n\n");
    gnutls_bye(session, GNUTLS_SHUT_WR);
    close(connection_fd);
    gnutls_deinit(session);

  }

  gnutls_certificate_free_credentials(x509_cred);
  gnutls_priority_deinit(priority_cache);

  gnutls_global_deinit();


}
