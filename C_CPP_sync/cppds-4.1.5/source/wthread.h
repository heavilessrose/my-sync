#ifndef WTHREAD_H
#define WTHREAD_H

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
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif

static DWORD WINAPI ThreadFunction(LPVOID);

class THREAD
{
  friend DWORD WINAPI ThreadFunction(LPVOID);

  public:
    THREAD();
    virtual ~THREAD();

    int Start(void* = NULL);
    void Detach();
    void* Wait();
    void Stop();
    unsigned int GetThreadID()
	{ return ThreadID; }
    static unsigned int GetCurrentThreadID()
	{ return GetCurrentThreadId(); }
    static void Sleep(int delay)
	{ ::Sleep(delay); }

  protected:
    virtual void* Run(void*);
    void* Param;

  private:
    HANDLE ThreadHandle;
    DWORD ThreadID;
    BOOL Started;
    BOOL Detached;
};

#endif



