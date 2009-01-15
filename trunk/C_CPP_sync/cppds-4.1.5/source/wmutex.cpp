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
#ifndef WMUTEX_H
  #include "wmutex.h"
#endif

MUTEX::MUTEX()
{
  Mutex = CreateMutex(NULL, FALSE, NULL);
}

MUTEX::~MUTEX()
{
  CloseHandle(Mutex);
}

void MUTEX::Acquire()
{
  WaitForSingleObject(Mutex, INFINITE);
}

int MUTEX::Acquired()
{
  return (WaitForSingleObject(Mutex, 0) != WAIT_TIMEOUT);
}

void MUTEX::Release()
{
  ReleaseMutex(Mutex);
}




