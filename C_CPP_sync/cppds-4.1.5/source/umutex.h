#ifndef UMUTEX_H
#define UMUTEX_H

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
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif

class MUTEX
{
  public:
    MUTEX();
    virtual ~MUTEX();
    virtual void Acquire();
    virtual int Acquired();
    virtual void Release();

  private:
    pthread_mutex_t Mutex;
};

#endif



