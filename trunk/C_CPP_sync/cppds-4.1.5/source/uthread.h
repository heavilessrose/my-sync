#ifndef UTHREAD_H
#define UTHREAD_H

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

static void* ThreadFunction(void*);

class THREAD
{
  friend void* ThreadFunction(void*);

  public:
    THREAD();
    virtual ~THREAD();
    int Start(void* = NULL);
    void Detach();
    void* Wait();
    void Stop();
    unsigned int GetThreadID()
	{ return ThreadID; }
    static unsigned int GetCurrentID()
	{ return 0; }
    static void Sleep(int);

  protected:
    virtual void* Run(void*);

  private:
    pthread_t ThreadHandle;
    unsigned int ThreadID;
    int Started;
    int Detached;
    void* Param;
};

#endif



