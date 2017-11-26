#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#include <gnutls/gnutls.h>

#include <mariadb/mysql.h>

#include <AVL.h>
#include <stringMap.h>
#include <configLoader.h>

struct connection_args{
  pthread_t thread_handle;
  int iret;

  MYSQL *mysql;
  
  struct str_map *targets;
  struct config *conf;
  
  int connection_fd;
  //int socket_fd;
  socklen_t client_addrlen;
  struct sockaddr_in client_socket;

  gnutls_certificate_credentials_t *x509_cred;
  gnutls_priority_t *priority_cache;
  gnutls_session_t session;

};



void *handle_client(void *argsin);

#endif
