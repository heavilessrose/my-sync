#ifndef WMUTEX_H
#define WMUTEX_H

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

class MUTEX
{
  private:
    HANDLE Mutex;

  public:
    MUTEX();
    virtual ~MUTEX();
    virtual void Acquire();
    virtual int Acquired();
    virtual void Release();
};

#endif



