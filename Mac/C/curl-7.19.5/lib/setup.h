#ifndef __LIB_CURL_SETUP_H
#define __LIB_CURL_SETUP_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2008, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: setup.h,v 1.164 2009-05-01 12:39:40 yangtse Exp $
 ***************************************************************************/

/*
 * Define WIN32 when build target is Win32 API
 */

#if (defined(_WIN32) || defined(__WIN32__)) && !defined(WIN32) && !defined(__SYMBIAN32__)
#define WIN32
#endif

/*
 * Include configuration script results or hand-crafted
 * configuration file for platforms which lack config tool.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#else

#ifdef _WIN32_WCE
#include "config-win32ce.h"
#else
#ifdef WIN32
#include "config-win32.h"
#endif
#endif

#if defined(macintosh) && defined(__MRC__)
#  include "config-mac.h"
#endif

#ifdef __AMIGA__
#include "amigaos.h"
#endif

#ifdef __SYMBIAN32__
#include "config-symbian.h"
#endif

#ifdef __OS400__
#  include "config-os400.h"
#endif

#ifdef TPF
#  include "config-tpf.h"
#endif

#endif /* HAVE_CONFIG_H */

/* ================================================================ */
/* Definition of preprocessor macros/symbols which modify compiler  */
/* behaviour or generated code characteristics must be done here,   */
/* as appropriate, before any system header file is included. It is */
/* also possible to have them defined in the config file included   */
/* before this point. As a result of all this we frown inclusion of */
/* system header files in our config files, avoid this at any cost. */
/* ================================================================ */

/*
 * Tru64 needs _REENTRANT set for a few function prototypes and
 * things to appear in the system header files. Unixware needs it
 * to build proper reentrant code. Others may also need it.
 */

#ifdef NEED_REENTRANT
#  ifndef _REENTRANT
#    define _REENTRANT
#  endif
#endif

/* ================================================================ */
/*  If you need to include a system header file for your platform,  */
/*  please, do it beyond the point further indicated in this file.  */
/* ================================================================ */

/*
 * libcurl's external interface definitions are also used internally,
 * and might also include required system header files to define them.
 */

#include <curl/curlbuild.h>

/*
 * Compile time sanity checks must also be done when building the library.
 */

#include <curl/curlrules.h>

/*
 * Ensure that no one is using the old SIZEOF_CURL_OFF_T macro
 */

#ifdef SIZEOF_CURL_OFF_T
#  error "SIZEOF_CURL_OFF_T shall not be defined!"
   Error Compilation_aborted_SIZEOF_CURL_OFF_T_shall_not_be_defined
#endif

/*
 * Set up internal curl_off_t formatting string directives for
 * exclusive use with libcurl's internal *printf functions.
 */

#ifdef FORMAT_OFF_T
#  error "FORMAT_OFF_T shall not be defined before this point!"
   Error Compilation_aborted_FORMAT_OFF_T_already_defined
#endif

#ifdef FORMAT_OFF_TU
#  error "FORMAT_OFF_TU shall not be defined before this point!"
   Error Compilation_aborted_FORMAT_OFF_TU_already_defined
#endif

#if (CURL_SIZEOF_CURL_OFF_T > CURL_SIZEOF_LONG)
#  define FORMAT_OFF_T  "lld"
#  define FORMAT_OFF_TU "llu"
#else
#  define FORMAT_OFF_T  "ld"
#  define FORMAT_OFF_TU "lu"
#endif

/*
 * Disable other protocols when http is the only one desired.
 */

#ifdef HTTP_ONLY
#  define CURL_DISABLE_TFTP
#  define CURL_DISABLE_FTP
#  define CURL_DISABLE_LDAP
#  define CURL_DISABLE_TELNET
#  define CURL_DISABLE_DICT
#  define CURL_DISABLE_FILE
#endif

/* ================================================================ */
/* No system header file shall be included in this file before this */
/* point. The only allowed ones are those included from curlbuild.h */
/* ================================================================ */

/*
 * OS/400 setup file includes some system headers.
 */

#ifdef __OS400__
#  include "setup-os400.h"
#endif

/*
 * Include header files for windows builds before redefining anything.
 * Use this preproessor block only to include or exclude windows.h,
 * winsock2.h, ws2tcpip.h or winsock.h. Any other windows thing belongs
 * to any other further and independant block.  Under Cygwin things work
 * just as under linux (e.g. <sys/socket.h>) and the winsock headers should
 * never be included when __CYGWIN__ is defined.  configure script takes
 * care of this, not defining HAVE_WINDOWS_H, HAVE_WINSOCK_H, HAVE_WINSOCK2_H,
 * neither HAVE_WS2TCPIP_H when __CYGWIN__ is defined.
 */

#ifdef HAVE_WINDOWS_H
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  ifdef HAVE_WINSOCK2_H
#    include <winsock2.h>
#    ifdef HAVE_WS2TCPIP_H
#       include <ws2tcpip.h>
#    endif
#  else
#    ifdef HAVE_WINSOCK_H
#      include <winsock.h>
#    endif
#  endif
#endif

/*
 * Define USE_WINSOCK to 2 if we have and use WINSOCK2 API, else
 * define USE_WINSOCK to 1 if we have and use WINSOCK  API, else
 * undefine USE_WINSOCK.
 */

#undef USE_WINSOCK

#ifdef HAVE_WINSOCK2_H
#  define USE_WINSOCK 2
#else
#  ifdef HAVE_WINSOCK_H
#    define USE_WINSOCK 1
#  endif
#endif

#ifdef HAVE_EXTRA_STRICMP_H
#  include <extra/stricmp.h>
#endif

#ifdef HAVE_EXTRA_STRDUP_H
#  include <extra/strdup.h>
#endif

#ifdef TPF
#  include <strings.h>    /* for bzero, strcasecmp, and strncasecmp */
#  include <string.h>     /* for strcpy and strlen */
#  include <stdlib.h>     /* for rand and srand */
#  include <sys/socket.h> /* for select and ioctl*/
#  include <netdb.h>      /* for in_addr_t definition */
#  include <tpf/sysapi.h> /* for tpf_process_signals */
   /* change which select is used for libcurl */
#  define select(a,b,c,d,e) tpf_select_libcurl(a,b,c,d,e)
#endif

#include <stdio.h>
#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif
#include <errno.h>

#ifdef __TANDEM /* for nsr-tandem-nsk systems */
#include <floss.h>
#endif

#ifndef STDC_HEADERS /* no standard C headers! */
#include <curl/stdcheaders.h>
#endif

/*
 * PellesC kludge section (yikes);
 *  - It has 'ssize_t', but it is in <unistd.h>. The way the headers
 *    on Win32 are included, forces me to include this header here.
 *  - sys_nerr, EINTR is missing in v4.0 or older.
 */
#ifdef __POCC__
  #include <sys/types.h>
  #include <unistd.h>
  #if (__POCC__ <= 400)
  #define sys_nerr EILSEQ  /* for strerror.c */
  #define EINTR    -1      /* for select.c */
  #endif
#endif

/*
 * Salford-C kludge section (mostly borrowed from wxWidgets).
 */
#ifdef __SALFORDC__
  #pragma suppress 353             /* Possible nested comments */
  #pragma suppress 593             /* Define not used */
  #pragma suppress 61              /* enum has no name */
  #pragma suppress 106             /* unnamed, unused parameter */
  #include <clib.h>
#endif

/*
 * Large file (>2Gb) support using WIN32 functions.
 */

#ifdef USE_WIN32_LARGE_FILES
#  include <io.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  define lseek(fdes,offset,whence)  _lseeki64(fdes, offset, whence)
#  define fstat(fdes,stp)            _fstati64(fdes, stp)
#  define stat(fname,stp)            _stati64(fname, stp)
#  define struct_stat                struct _stati64
#endif

/*
 * Small file (<2Gb) support using WIN32 functions.
 */

#ifdef USE_WIN32_SMALL_FILES
#  include <io.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  define lseek(fdes,offset,whence)  _lseek(fdes, (long)offset, whence)
#  define fstat(fdes,stp)            _fstat(fdes, stp)
#  define stat(fname,stp)            _stat(fname, stp)
#  define struct_stat                struct _stat
#endif

#ifndef struct_stat
#  define struct_stat struct stat
#endif

/*
 * Default sizeof(off_t) in case it hasn't been defined in config file.
 */

#ifndef SIZEOF_OFF_T
#  if defined(__VMS) && (defined(__alpha) || defined(__ia64))
#    if defined(_LARGEFILE)
#      define SIZEOF_OFF_T 8
#    endif
#  elif defined(__OS400__) && defined(__ILEC400__)
#    if defined(_LARGE_FILES)
#      define SIZEOF_OFF_T 8
#    endif
#  elif defined(__MVS__) && defined(__IBMC__)
#    if defined(_LP64) || defined(_LARGE_FILES)
#      define SIZEOF_OFF_T 8
#    endif
#  elif defined(__370__) && defined(__IBMC__)
#    if defined(_LP64) || defined(_LARGE_FILES)
#      define SIZEOF_OFF_T 8
#    endif
#  endif
#  ifndef SIZEOF_OFF_T
#    define SIZEOF_OFF_T 4
#  endif
#endif

/* Below we define some functions. They should
   1. close a socket

   4. set the SIGALRM signal timeout
   5. set dir/file naming defines
   */

#ifdef WIN32

#  if !defined(__CYGWIN__)
#    define sclose(x) closesocket(x)
#  else
#    define sclose(x) close(x)
#  endif

#  define DIR_CHAR      "\\"
#  define DOT_CHAR      "_"

#else /* WIN32 */

#  ifdef MSDOS  /* Watt-32 */

#    include <sys/ioctl.h>
#    define sclose(x)         close_s(x)
#    define select(n,r,w,x,t) select_s(n,r,w,x,t)
#    define ioctl(x,y,z) ioctlsocket(x,y,(char *)(z))
#    include <tcp.h>
#    ifdef word
#      undef word
#    endif
#    ifdef byte
#      undef byte
#    endif

#  else /* MSDOS */

#    ifdef __BEOS__
#      define sclose(x) closesocket(x)
#    else /* __BEOS__ */
#      define sclose(x) close(x)
#    endif /* __BEOS__ */

#  endif /* MSDOS */

#  ifdef _AMIGASF
#    undef sclose
#    define sclose(x) CloseSocket(x)
#  endif

#  ifdef __minix
     /* Minix 3 versions up to at least 3.1.3 are missing these prototypes */
     extern char * strtok_r(char *s, const char *delim, char **last);
     extern struct tm * gmtime_r(const time_t * const timep, struct tm *tmp);
#  endif

#  define DIR_CHAR      "/"
#  ifndef DOT_CHAR
#    define DOT_CHAR      "."
#  endif

#  ifdef MSDOS
#    undef DOT_CHAR
#    define DOT_CHAR      "_"
#  endif

#  ifndef fileno /* sunos 4 have this as a macro! */
     int fileno( FILE *stream);
#  endif

#endif /* WIN32 */

#if defined(WIN32) && !defined(__CYGWIN__) && !defined(USE_ARES) && \
    !defined(__LCC__)  /* lcc-win32 doesn't have _beginthreadex() */
#ifdef ENABLE_IPV6
#define USE_THREADING_GETADDRINFO
#else
#define USE_THREADING_GETHOSTBYNAME  /* Cygwin uses alarm() function */
#endif
#endif

/* "cl -ML" or "cl -MLd" implies a single-threaded runtime library where
   _beginthreadex() is not available */
#if (defined(_MSC_VER) && !defined(__POCC__)) && !defined(_MT) && !defined(USE_ARES)
#undef USE_THREADING_GETADDRINFO
#undef USE_THREADING_GETHOSTBYNAME
#define CURL_NO__BEGINTHREADEX
#endif

/*
 * msvc 6.0 does not have struct sockaddr_storage and
 * does not define IPPROTO_ESP in winsock2.h. But both
 * are available if PSDK is properly installed.
 */

#ifdef _MSC_VER
#if !defined(HAVE_WINSOCK2_H) || ((_MSC_VER < 1300) && !defined(IPPROTO_ESP))
#undef HAVE_STRUCT_SOCKADDR_STORAGE
#endif
#endif

#ifdef NETWARE
int netware_init(void);
#ifndef __NOVELL_LIBC__
#include <sys/bsdskt.h>
#include <sys/timeval.h>
#endif
#endif

#if defined(HAVE_LIBIDN) && defined(HAVE_TLD_H)
/* The lib was present and the tld.h header (which is missing in libidn 0.3.X
   but we only work with libidn 0.4.1 or later) */
#define USE_LIBIDN
#endif

#ifndef SIZEOF_TIME_T
/* assume default size of time_t to be 32 bit */
#define SIZEOF_TIME_T 4
#endif

#define LIBIDN_REQUIRED_VERSION "0.4.1"

#if defined(USE_GNUTLS) || defined(USE_SSLEAY) || defined(USE_NSS) || defined(USE_QSOSSL)
#define USE_SSL    /* SSL support has been enabled */
#endif

#if !defined(CURL_DISABLE_HTTP) && !defined(CURL_DISABLE_NTLM)
#if defined(USE_SSLEAY) || defined(USE_WINDOWS_SSPI) || defined(USE_GNUTLS)
#define USE_NTLM
#endif
#endif

/* non-configure builds may define CURL_WANTS_CA_BUNDLE_ENV */
#if defined(CURL_WANTS_CA_BUNDLE_ENV) && !defined(CURL_CA_BUNDLE)
#define CURL_CA_BUNDLE getenv("CURL_CA_BUNDLE")
#endif

/*
 * Include macros and defines that should only be processed once.
 */

#ifndef __SETUP_ONCE_H
#include "setup_once.h"
#endif

#endif /* __LIB_CURL_SETUP_H */
