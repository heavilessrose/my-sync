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
#ifndef UTHREAD_H
  #include "uthread.h"
#endif

/****************************************************************************/
static void* ThreadFunction(void* object)
{
  THREAD* thread = (THREAD*)object;
  return thread->Run(thread->Param);
}

/****************************************************************************/
THREAD::THREAD()
{
  Started = FALSE;
  Detached = FALSE;
}

/****************************************************************************/
THREAD::~THREAD()
{
  Stop();
}

/****************************************************************************/
int THREAD::Start(void* param)
{
  if (!Started)
  {
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    if (Detached)
      pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);

    Param = param;
    ThreadID = 0;
    if (pthread_create(&ThreadHandle, &attributes, ThreadFunction, this) == 0)
      Started = TRUE;

    pthread_attr_destroy(&attributes);
  }

  return Started;
}

/****************************************************************************/
void THREAD::Detach()
{
  if (Started && !Detached)
    pthread_detach(ThreadHandle);

  Detached = TRUE;
}

/****************************************************************************/
void* THREAD::Wait()
{
  void* status = NULL;

  if (Started && !Detached)
  {
    pthread_join(ThreadHandle, &status);
    Detached = TRUE;
  }

  return status;
}

/****************************************************************************/
void THREAD::Stop()
{
  if (Started && !Detached)
  {
    pthread_cancel(ThreadHandle);
    pthread_detach(ThreadHandle);
    Detached = TRUE;
  }
}

/****************************************************************************/
void THREAD::Sleep(int delay)
{
  timeval timeout = {(delay / 1000), ((delay * 1000) % 1000000)};
  select(0, (fd_set*)NULL, (fd_set*)NULL, (fd_set*)NULL, &timeout);
}

/****************************************************************************/
void* THREAD::Run(void* Dum_)
{
  return Dum_;
}

/****************************************************************************/




