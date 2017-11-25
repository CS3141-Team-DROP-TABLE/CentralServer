#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#include <gnutls/gnutls.h>

#include <clientHandler.h>

void *handle_client(void *argsin){

  gnutls_session_t session;
  struct connection_args *args = (struct connection_args*)argsin;
  
  char buffer[256];
  int n = 0;
  char quit[6];
  memcpy(quit, "quit\n\0", 7);
    
  /* =========================================================================
   * Attach GNUtls session to socket
   * ========================================================================= 
   */
  

  
  gnutls_init(&session, GNUTLS_SERVER);
  gnutls_priority_set(session, *args->priority_cache);
  gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, *args->x509_cred);
  gnutls_certificate_server_set_request(session, GNUTLS_CERT_IGNORE);
  gnutls_handshake_set_timeout(session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
  gnutls_transport_set_int(session, args->connection_fd);
  

  // Handshake
  int ret;
  do {
    ret = gnutls_handshake(session);
    if( ret < 0) printf("Handshake failed -- retrying\n\n");
  } while (ret < 0 && gnutls_error_is_fatal(ret) == 0);
  if (ret < 0) {
    fprintf(stderr,
	    "[Central Server] TLS handshake failed (%s)\n\n",
	    gnutls_strerror(ret));
    return NULL;
  }
  printf("Handshake completed\n\n");
  
  
  
  
  memset(buffer, 0, 256);
  ret = 1;
  while(memcmp(buffer, quit, 4) != 0  && ret > 0){
    memset(buffer, 0, 256);
    
    do { //recv is non-blocking
      usleep(100);
      ret = gnutls_record_recv(session, buffer, 255);
    }while(ret == GNUTLS_E_AGAIN);
    
    printf("Message received\n");
    if(memcmp(buffer, "hello", 5) == 0){
      memset(buffer, 0, 256);
      buffer[0] = 1;
      in_addr_t addr = inet_addr("127.0.0.1");
      //memcpy(&buffer[1], &addr, sizeof(in_addr_t));
      //printf("\"%s\"\n", buffer);
      //write(connection_fd, buffer, n);
      //gnutls_record_send(session, buffer, sizeof(in_addr_t)+1);
      
      addr = inet_addr("8.8.8.8");
      memcpy(&buffer[1], &addr, sizeof(in_addr_t));
      gnutls_record_send(session, buffer, sizeof(in_addr_t)+1);
    } else {
      printf("Message Recived: %d: %s\n ", ret, buffer);
      gnutls_record_send(session, "recv\n\0", 5);
      
    }
    
  }
  
  printf("Connection terminated\n\n");
  gnutls_bye(session, GNUTLS_SHUT_WR);
  close(args->connection_fd);
  gnutls_deinit(session);
  
}
