#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_PTHREAD_H
  #include <pthread.h>
  #define INCL_PTHREAD_H
#endif
#ifndef INCL_SYSTIME_H
  #include <sys/time.h>
  #define INCL_SYSTIME_H
#endif
#ifndef UMUTEX_H
  #include "umutex.h"
#endif

MUTEX::MUTEX()
{
  pthread_mutex_init(&Mutex, (pthread_mutexattr_t*)NULL);
}

MUTEX::~MUTEX()
{
  pthread_mutex_destroy(&Mutex);
}

void MUTEX::Acquire()
{
  pthread_mutex_lock(&Mutex);
}

int MUTEX::Acquired()
{
  return (pthread_mutex_trylock(&Mutex) == 0);
}

void MUTEX::Release()
{
  pthread_mutex_unlock(&Mutex);
}



