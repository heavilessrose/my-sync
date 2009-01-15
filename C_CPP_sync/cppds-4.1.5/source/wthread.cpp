#ifndef INCL_WINDOWS_H
  #define INCL_WINDOWS_H
  #include <windows.h>
#endif
#ifndef INCL_STDLIB_H
  #define INCL_STDLIB_H
  #include <stdlib.h>
#endif
#ifndef INCL_PROCESS_H
  #define INCL_PROCESS_H
  #include <process.h>
#endif
#ifndef WTHREAD_H
  #include "wthread.h"
#endif

/****************************************************************************/
static DWORD WINAPI ThreadFunction(LPVOID object)
{
  THREAD* thread = (THREAD*)object;
  return (DWORD)thread->Run(thread->Param);
}

/****************************************************************************/
THREAD::THREAD():
Started(FALSE),
Detached(FALSE)
{}

/****************************************************************************/
THREAD::~THREAD()
{
  Stop();
}

/****************************************************************************/
void* THREAD::Run(void*)
{
  return NULL;
}

/****************************************************************************/
int THREAD::Start(void* param)
{
  if (!Started)
  {
    Param = param;
    if (ThreadHandle = (HANDLE)CreateThread(NULL, 0, ThreadFunction, this, 0, &ThreadID))
    {	  
      if (Detached)
		CloseHandle(ThreadHandle);
      Started = TRUE;
    }
  }

  return Started;
}

/****************************************************************************/
void* THREAD::Wait(void)
{
  DWORD status = (DWORD)NULL;
  if (Started && !Detached)
  {
    WaitForSingleObject(ThreadHandle, INFINITE);
    GetExitCodeThread(ThreadHandle, &status);
    CloseHandle(ThreadHandle);
    Detached = TRUE;
  }

  return (void*)status;
}

/****************************************************************************/
void THREAD::Detach()
{
  if (Started && !Detached)
    CloseHandle(ThreadHandle);
  Detached = TRUE;
}

/****************************************************************************/
void THREAD::Stop()
{
  if (Started && !Detached)
  {
    TerminateThread(ThreadHandle, 0);
    CloseHandle(ThreadHandle);
    Detached = TRUE;
  }
}

/****************************************************************************/




