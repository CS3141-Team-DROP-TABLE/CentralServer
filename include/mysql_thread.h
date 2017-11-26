#ifndef MYSQL_THREAD_H
#define MYSQL_THREAD_H

#include <mariadb/mysql.h>

#include <AVL.h>
#include <stringMap.h>
#include <configLoader.h>

struct mysql_th_args{
  int *run;
  pthread_t thread_handle;
  int iret;

  MYSQL *mysql;
  
  struct config *conf;
  struct str_map *targets;
};


#endif
