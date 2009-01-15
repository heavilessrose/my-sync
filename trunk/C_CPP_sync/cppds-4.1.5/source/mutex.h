#ifndef MUTEX_H
#define MUTEX_H

#if defined(__linux__) | defined(__unix__)
  #include "umutex.h"
#else
  #include "wmutex.h"
#endif

#endif




