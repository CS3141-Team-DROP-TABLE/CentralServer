#ifndef MYSQL_THREAD_H
#define MYSQL_THREAD_H

#include <AVL.h>
#include <stringMap.h>
#include <configLoader.h>

struct mysql_th_args{
  int *run;
  pthread_t thread_handle;
  int iret;

  struct config *conf;
  struct str_map *targets;
};


#endif
