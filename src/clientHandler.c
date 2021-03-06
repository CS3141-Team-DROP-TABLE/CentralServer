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

#include <AVL.h>
#include <stringMap.h>
#include <configLoader.h>


#include <clientHandler.h>


struct sender_args{
  gnutls_session_t *session;

};


void send_all(void *a, void *b, void *argsin){
  struct sender_args *args = (struct sender_args*)argsin;
  char buffer[512];
  buffer[511] = '\0';
  snprintf(buffer, 511, "Host:%s\n", (char*)a);
  gnutls_record_send(*args->session, buffer, strnlen(buffer, 512));
}

void send_all_hosts(gnutls_session_t *session, struct str_map *s){
  struct sender_args sa;
  sa.session = session;
  str_apply_to_all(s, &send_all, &sa);

}

void *handle_client(void *argsin){

  gnutls_session_t session;
  struct connection_args *args = (struct connection_args*)argsin;
  
  char buffer[255];
  char insquery[255];
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
    
    printf("Message received: %s\n", buffer);
    if(memcmp(buffer, "hello", 5) == 0){
      send_all_hosts(&session, args->targets);
    } else if(strncmp(buffer, "Recv:", 5) == 0){
      char host[100];
      char time[100];
      sscanf(&buffer[5], "%20[^=]%*c%20[^;]", host, time); 
      snprintf(buffer, 250, "UPDATE stats SET timestamp=CURRENT_TIMESTAMP, ping=%d WHERE ip='%s';", atoi(time), host);
      snprintf(insquery, 250, "INSERT INTO record (time, ip) VALUES (%d, '%s');", atoi(time), host);
      send_all_hosts(&session, args->targets);
     
      if ( mysql_real_query(args->mysql, buffer, strnlen(buffer, 250))) {
	printf("Query failed: %s\n", mysql_error(args->mysql));
      }
      
      if ( mysql_real_query(args->mysql, insquery, strnlen(buffer, 250))) {
	printf("Query failed: %s\n", mysql_error(args->mysql));
      } 
      
    } else if( memcmp(buffer, "A", 1)) {
      int id;
      int code;
      char msg[1024];

      sscanf(&buffer[1], "%d:%d%1023[\\n]", &id, &code, msg);

      printf("Agent: %d, Code: %d, msg: %s\n", id, code, msg);

      if(code == 0)
	snprintf(buffer, 1023, "UPDATE agent SET ip='%s' WHERE id=%d;", msg, id);
      if(code == 1)
	snprintf(buffer, 1023, "UPDATE agent SET lastCheckIn=NOW() WHERE id=%d;", id);
      if(code == 3)
	snprintf(buffer, 1023, "UPDATE agent SET speed='%s' WHERE id=%d;", msg, id);

       if ( mysql_real_query(args->mysql, insquery, strnlen(buffer, 250))) {
	printf("Query failed: %s\n", mysql_error(args->mysql));
      }
       
    }else {
      printf("Message Recived: %d: %s\n ", ret, buffer);
      
      
    }
    
  }
  
  printf("Connection terminated\n\n");
  gnutls_bye(session, GNUTLS_SHUT_WR);
  close(args->connection_fd);
  gnutls_deinit(session);
  
}
