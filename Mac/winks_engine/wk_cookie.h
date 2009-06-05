///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007-2008 Mobile Winks Corporation 
// All rights reserved. 
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met: 
//
// * Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer. 
// * Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution. 
// * Neither name of Intel Corporation nor the names of its contributors 
// may be used to endorse or promote products derived from this software 
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef _COOKIE_H
#define _COOKIE_H 



typedef struct tagwkCookieItem
{
  char *                           name ;
  int                              name_lenth; 
  char *                           path;
  int                              path_lenth;
  long int                         max_age;
  unsigned long                    corrected_initial_age;
  unsigned long                    respose_time;
  char*                            expire_time;
  int                              expire_time_lenth;
  char *                           domain;
  int                              domain_lenth;
  char *                           comment;
  int                              comment_lenth;
  char *                           commentURL ;
  int                              commentURL_lenth ;
  int                              discard;
  int                              secure;
  int                              version;
  int                              port;
  unsigned long                    data_lenth;
  char*                            data;   /*指向消息数据的指针的值*/  
  struct     tagwkCookieItem *     pNext;      /*单元的后向指针*/
}Winks_CookieItem_s;

typedef struct tagwkcookieList
{
  struct tagwkCookieItem *			head; /*head, first item is stored.*/ 
  unsigned long						totalspace_cookie;
  unsigned long						availablespace;
} Winks_CookieList_s;


extern int winks_cookie_init(void);
extern int winks_cookie_close(void);
extern int winks_getlenth_cookie(char * url, int * lenth, int  port);
extern int winks_get_cookie(char * url, char * cookie, int  * lenth, int port);
extern int winks_put_cookie(char* url, char * set_cookie2, long int set_cookie2_lenth, char *  date_value, unsigned long  age_value,  unsigned long  request_time, unsigned long respose_time, int port);
extern int winks_setupcapability_cookie (int opcode, unsigned long coo_value);

#ifndef WINKS_FS_COOKIE_PATH
	#define WINKS_FS_COOKIE_PATH  WINKS_APP_PATH"cookie.txt"
#endif
#endif


