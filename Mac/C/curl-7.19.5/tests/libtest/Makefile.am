#***************************************************************************
#                                  _   _ ____  _
#  Project                     ___| | | |  _ \| |
#                             / __| | | | |_) | |
#                            | (__| |_| |  _ <| |___
#                             \___|\___/|_| \_\_____|
#
# Copyright (C) 1998 - 2009, Daniel Stenberg, <daniel@haxx.se>, et al.
#
# This software is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at http://curl.haxx.se/docs/copyright.html.
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the COPYING file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#
# $Id: Makefile.am,v 1.80 2009-04-07 22:07:07 bagder Exp $
###########################################################################
AUTOMAKE_OPTIONS = foreign nostdinc

# Specify our include paths here, and do it relative to $(top_srcdir) and
# $(top_builddir), to ensure that these paths which belong to the library
# being currently built and tested are searched before the library which
# might possibly already be installed in the system.
#
# $(top_builddir)/include is for libcurl's generated curl/curlbuild.h file
# $(top_srcdir)/include is for libcurl's external include files
# $(top_builddir)/lib is for libcurl's generated lib/config.h file
# $(top_srcdir)/lib is for libcurl's lib/setup.h and other "borrowed" files

INCLUDES = -I$(top_builddir)/include \
           -I$(top_srcdir)/include   \
           -I$(top_builddir)/lib     \
           -I$(top_srcdir)/lib 

LIBDIR = $(top_builddir)/lib

EXTRA_DIST = test75.pl test307.pl test610.pl test613.pl test1013.pl	\
test1022.pl Makefile.inc

# Dependencies (may need to be overriden)
LDADD = $(LIBDIR)/libcurl.la
DEPENDENCIES = $(LIBDIR)/libcurl.la

# Makefile.inc provides the source defines (TESTUTIL, SUPPORTFILES, noinst_PROGRAMS, lib*_SOURCES, and lib*_CFLAGS)
include Makefile.inc
