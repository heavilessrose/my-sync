#ifndef THREAD_CPP
#define THREAD_CPP

#ifndef THREAD_H
  #include "thread.h"
#endif

#if defined(__linux__) | defined(__unix__)
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
  #include "uthread.cpp"
#else
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
  #include "wthread.cpp"
#endif

#endif




