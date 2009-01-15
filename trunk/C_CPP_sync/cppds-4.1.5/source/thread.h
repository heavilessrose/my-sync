#ifndef THREAD_H
#define THREAD_H

#if defined(__linux__) | defined(__unix__)
  #include "uthread.h"
#else
  #include "wthread.h"
#endif

#endif




