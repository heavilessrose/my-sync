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

#ifndef _CACHE_H
#define _CACHE_H

typedef struct tagwkDataList
{
	struct tagwkDataList *          pNext;
	int                             data_lenth;
	char *                          data;
} Winks_DataList_s;
 
typedef struct tagwkCacheItem
{                                                                
	char *						    url;                       
	int                             url_lenth;  
	
	char *                          content_type;  
	int                             content_type_lenth; 
	
	char *                          expire_time;               
	int                             expire_time_lenth;         
	char *                          last_modified_time;        
	int							    last_modified_time_lenth;  
	int                             corrected_initial_age;     
	int                             method;                    
	unsigned long					respose_time;              
	long   int                      max_age;                   
	unsigned long					max_stale;                 
	unsigned long					min_fresh;                 
	unsigned long					data_lenth;                
	struct tagwkDataList *			data_head;                 
	struct  tagwkCacheItem *	    pNext;                     
}                                                                
 Winks_CacheItem_s;
typedef struct tagwkCacheList
{
	struct tagwkCacheItem *			head;
	unsigned long                   totalspace_cache;
	unsigned long                   availablespace;
} Winks_CacheList_s;

typedef struct tagwkCacheTimeItem
{
	char *							date_value;
	int                             date_value_lenth;
	char *                          expire_time;
	int                             expire_time_lenth;
	char *                          last_modified_time;
	int                             last_modified_time_lenth;
	unsigned long					request_time;
	unsigned long					respose_time;
	char *                          cache_control;
	int                             cache_control_lenth;
	unsigned long					age_value;
	char *                          content_type; 
	int                             content_type_lenth; 
} Winks_CacheTimeItem_s;

extern int winks_cache_init(void);
extern int winks_cache_close(void);
extern int winks_getlenth_cache(char * url, int url_lenth, int method, unsigned int * cache_lenth, char** last_modified_time, int *last_modified_time_lenth);
extern int winks_find_cache(char * url, int url_lenth, int method);
extern int winks_create_cache(char *url, int url_lenth, int method, unsigned int data_lenth, Winks_CacheTimeItem_s * tempnode);
extern int winks_setupcapability_cache(int opcode, unsigned long coo_value);
extern int winks_get_cache_append(int handle, int offset, char * data, int* data_lenth);
extern int winks_put_cache_append(int handle, int lenth, char * data);
extern int winks_get_cache_centent_type(int handle, char * centent_type, int * centent_type_lenth);

#define WINKS_FS_CACHE_PATH  WINKS_APP_PATH"cache.txt"
#endif
