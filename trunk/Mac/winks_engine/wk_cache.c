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

#include "wk_public.h"
#include "wk_osal.h"
#include "wk_cache.h"


/*#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>*/


#define  TOTALSPACE_CACHE      20000
#ifndef  FS_O_RDWR
#define  FS_O_RDWR             "r"
#define  FS_O_WRONLY           "w"
#endif


static int expire_cache_time(Winks_CacheItem_s * node);
static int detele_cache(Winks_CacheItem_s * prior, Winks_CacheItem_s * node);
static Winks_CacheItem_s * create_cache(void);
//static int strnicmp( const char * dst, const char * src, int count);
static int detele_cache_tail(void);
static int free_cache(Winks_CacheItem_s * node);
static int require_availablespace(unsigned long date_lenth);
static int put_cache(Winks_CacheItem_s * node);
static int destroy_cache(void);
extern void stringtogmt(char* gmt, Winks_DateTime_s * time);
static Winks_DataList_s * creat_data_node(int lenth);
static int expire_cache_time(Winks_CacheItem_s * node);
static int get_value(char * name, char * cache_control, long int *value);
extern void stringtogmt_rfc850(char* gmt, Winks_DateTime_s * time);

static int free_datanode_list(Winks_DataList_s * node_head);
extern char * wk_stristr(const char * Src,const char *Dest);
extern void stringtogmt_asctime(char* gmt, Winks_DateTime_s * time);


//static Winks_CacheList_s  cachelist;
#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
static Winks_CacheList_s  cachelist;
#endif

#if 1
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_UI_CONTEXT_DEF()  \
    Winks_CacheList_s  *Winks_CacheContext = NULL;
#define WINKS_UI_CONTEXT_INIT() \
    Winks_CacheContext = (Winks_CacheList_s *)get_module_glob(WK_MODULE_CACHE); 

#else
#define WINKS_UI_CONTEXT_DEF()  \
    Winks_CacheList_s *Winks_CacheContext = NULL; 
#define WINKS_UI_CONTEXT_INIT() \
    Winks_CacheContext = &cachelist; 
#endif
#endif

/*================================================================
*   winks_getlenth_cache
*       return the lenth of a cache.
*       
*   Parameters:
*       url: url by http request 
*       lenth:the lenth of a cache
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int  winks_getlenth_cache(char * url, int url_lenth, int method, unsigned int * cache_lenth, char** last_modified_time, int *last_modified_time_lenth)
{
	Winks_CacheItem_s * node;

	WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    
	Winks_assert(url != NULL || cache_lenth != NULL || last_modified_time != NULL ||last_modified_time_lenth != NULL);
	
	for(node = (*Winks_CacheContext).head; node != NULL; node = node->pNext)
	{
		if (0 == strnicmp(url,((int )node->url + node->data_head->data), url_lenth) && method == node->method )
		{
			*cache_lenth = node->data_lenth;/*计算长度*/
			if ( node->last_modified_time == NULL)
			{
				*last_modified_time = node->last_modified_time;
			}
			else
			{
				*last_modified_time = node->last_modified_time + (int)node->data_head->data;
			}
			*last_modified_time_lenth = node->last_modified_time_lenth;
			return 0;
			
		}
		
	}
	*cache_lenth = 0;
    return 0;
}


/*================================================================
*   winks_get_cache
*       return the centent of a cache.
*       
*   Parameters:
*       url: url by http request 
*       data:the centent of a cache
*       lenth:the lenth of a cache data
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int  winks_find_cache(char* url, int url_lenth, int method)
{
	
	Winks_CacheItem_s * p;
	Winks_CacheItem_s * prior;

	WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
	
	//unsigned int lenth_remained;
	
	Winks_assert(url != NULL );
	
	/*过期扫描濉-将列表中过期的结点删除*/
	while (((*Winks_CacheContext).head !=NULL)  && (1 == expire_cache_time((*Winks_CacheContext).head)))
    {

        p = (*Winks_CacheContext).head ;
        (*Winks_CacheContext).head = (*Winks_CacheContext).head->pNext;	
        free_cache( p);
    }
	if ((*Winks_CacheContext).head == NULL)
		return 0;
	prior= (*Winks_CacheContext).head;
	for ( p = prior->pNext; p != NULL; )
	{
		
		if (1 == expire_cache_time(p))
		{
			prior->pNext = p->pNext;
			free_cache( p);
			p = prior->pNext;
			continue;
		}
		prior = p;
		p = p->pNext;
	}
	
	for ( p = (*Winks_CacheContext).head; p != NULL; p = p->pNext)
	{
		if ((url_lenth== p->url_lenth) && (0 == strnicmp(url,((int )p->url + p->data_head->data), url_lenth)) && (method == p->method ))
		{
			return ((int)p);
			
		}
		
	}
	return 0;
}
int winks_get_cache_centent_type(int handle, char * centent_type, int * centent_type_lenth)
{
	Winks_CacheItem_s * p = NULL;

	if (handle == 0)
		return 0;
	//if(centent_type == NULL)
	Winks_assert(centent_type != NULL);
	//if(centent_type == NULL)

	//Winks_mem_set(centent_type, 0,centent_type_lenth);
	p = ((Winks_CacheItem_s *)handle);
    
	if (* centent_type_lenth < p->content_type_lenth)
	{
		* centent_type_lenth = p->content_type_lenth;
		return -1;
	}

	Winks_mem_set(centent_type, 0, *centent_type_lenth);
	strncpy(centent_type, (char *)((int)p->content_type + p->data_head->data),p->content_type_lenth);
	* centent_type_lenth = p->content_type_lenth;
	return 0;
}
int winks_get_cache_append(int handle, int offset, char * data, int* data_lenth)
{
	char * tempdata;
	int  len;
	int templen;
    Winks_DataList_s * currentdatap = NULL;
    int currentoffset = 0;
	
	
	int lenth_remained = 0;
	int datalen_remained = 0;
	if (handle == 0)
		return 0;
    /*	if (currentdatap == NULL)
	{
	currentdatap  = ((Winks_CacheItem_s *)handle)->data_head->pNext;		
	
	}*/ 
	lenth_remained = ((Winks_CacheItem_s *)handle)->data_lenth - offset; 
	currentdatap  = ((Winks_CacheItem_s *)handle)->data_head->pNext;
    templen = currentdatap->data_lenth;
	while (templen <= offset)
	{
		offset = offset - templen;
		currentdatap = currentdatap->pNext;
		templen = currentdatap->data_lenth;
	}

    currentoffset = offset;
	tempdata = (char*)data;
	if (lenth_remained > *data_lenth)
	{
		datalen_remained = *data_lenth;/*用当前的数据块填不满适*/
		len = (currentdatap->data_lenth - currentoffset);
		while (datalen_remained >= len)
		{
			Winks_mem_cpy(tempdata, currentdatap->data + currentoffset,currentdatap->data_lenth - currentoffset);

			datalen_remained = datalen_remained - (currentdatap->data_lenth - currentoffset);
			tempdata = tempdata + (currentdatap->data_lenth - currentoffset);
			currentdatap = currentdatap->pNext;
			currentoffset = 0;
			
		}
		if (datalen_remained != 0)
		{
			Winks_mem_cpy(tempdata, currentdatap->data + currentoffset, datalen_remained);
			//currentdatap = currentdatap->pNext;
			currentoffset = currentoffset + datalen_remained;
		}	   
		return *data_lenth;
	}  
	else
	{
		datalen_remained = lenth_remained;
		len = currentdatap->data_lenth - currentoffset;
		while (datalen_remained >= len)
		{
			Winks_mem_cpy(tempdata, currentdatap->data + currentoffset,currentdatap->data_lenth - currentoffset);

			datalen_remained = datalen_remained - (currentdatap->data_lenth - currentoffset);
			tempdata = tempdata + (currentdatap->data_lenth - currentoffset);
			currentdatap = currentdatap->pNext;
			currentoffset = 0;
		
		}
		if (datalen_remained != 0)
		{
			Winks_mem_cpy(tempdata, currentdatap->data + currentoffset, datalen_remained);
			//currentdatap = currentdatap->pNext;
			currentoffset = currentoffset + datalen_remained;
		}
		currentdatap = NULL;
		return lenth_remained;
	}/*将对应的数据返回*/
	return 1;
}


/*================================================================
*   winks_put_cache
*       put a  cache in list.
*       
*   Parameters:
*       url: url by http request 
*       data:the centent of a cache
*       lenth:the lenth of a cache data
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_create_cache(char *url, int url_lenth, int method, unsigned int data_lenth, Winks_CacheTimeItem_s * tempnode)
{

    Winks_CacheItem_s * node;
    //  char * p;
    char * currentcp;
    char tempcontrol[100] = {0};

    struct tag_Winks_DateTime_s  time;

    int templenth  = 0;
    long int max_age;


    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    if (data_lenth > (*Winks_CacheContext).totalspace_cache)
    {
        Winks_printf("%s", "can't get require_availablespace( > totalspace_cache)##############!\n");
        return 0;
    }

    strncpy(tempcontrol, tempnode->cache_control, tempnode->cache_control_lenth);
    get_value("max-age", tempcontrol,&max_age);
    if (strstr(tempcontrol,"no-cache") ||strstr(tempcontrol,"no-store") ||0 == max_age)
    {
        return 0;
    }
    templenth = url_lenth + tempnode->expire_time_lenth + tempnode->last_modified_time_lenth + tempnode->content_type_lenth;  

    node = create_cache();
    node->data_head = creat_data_node(templenth);

    currentcp = node->data_head->data;
    Winks_mem_cpy( currentcp, url, url_lenth);
    currentcp = currentcp + url_lenth;
    Winks_mem_cpy( currentcp, tempnode->expire_time, tempnode->expire_time_lenth);
    currentcp = currentcp + tempnode->expire_time_lenth;
    Winks_mem_cpy( currentcp, tempnode->last_modified_time, tempnode->last_modified_time_lenth);
    //currentcp = currentcp + tempnode->last_modified_time_lenth;

    node->url = (char *)0;
    node->url_lenth = url_lenth ;

    if (tempnode->expire_time_lenth == 0)
    {
        node->expire_time = NULL;
        node->expire_time_lenth = tempnode->expire_time_lenth;
    }
    else
    {
        node->expire_time = (char *)url_lenth;
        node->expire_time_lenth = tempnode->expire_time_lenth;
    }
    if (tempnode->last_modified_time_lenth == 0)
    {
        node->last_modified_time = NULL;
        node->last_modified_time_lenth = tempnode->last_modified_time_lenth;
    }
    else
    {
        node->last_modified_time = (char *)(url_lenth + tempnode->expire_time_lenth);
        node->last_modified_time_lenth = tempnode->last_modified_time_lenth;
    }
    //node->max_age = tempnode->max_age;
    get_value("max-age", tempcontrol,&node->max_age);
    get_value("max-stale", tempcontrol,(long int*)&node->max_stale);
    //node->max_stale = tempnode->max_stale;
    get_value("min-fresh", tempcontrol,(long int*)&node->min_fresh);
    //node->min_fresh = tempnode->min_fresh;
    //node->data = p;
    node->data_lenth = data_lenth;
    node->method = method;
    //node->tal_data_lenth = templenth;
    stringtogmt(tempnode->date_value,&time );
    if (tempnode->respose_time > Winks_mktime(&time)) 
    {
        node->corrected_initial_age =  MAX((tempnode->respose_time - Winks_mktime(&time)), tempnode->age_value) + tempnode->respose_time - tempnode->request_time;
    }
    else
    {
        node->corrected_initial_age = tempnode->age_value + tempnode->respose_time - tempnode->request_time;
    }
    node->respose_time = tempnode->respose_time;
    currentcp = node->data_head->data + (url_lenth + tempnode->expire_time_lenth + tempnode->last_modified_time_lenth);
    Winks_mem_cpy( currentcp, tempnode->content_type, tempnode->content_type_lenth);
    node->content_type = (char *)(url_lenth + tempnode->expire_time_lenth + tempnode->last_modified_time_lenth);
    node->content_type_lenth= tempnode->content_type_lenth;
    if (put_cache(node) == 0)
    {
        return ((int)node->data_head); 
    }
    return 0;/* 放一个cache在列表中*/

}
		 
/*================================================================
*   winks_put_cache_append
*       put the cache append .
*       
*   Parameters:
*       int handle
*       int lenth
*       char * data
*
*   Return Value:
*       int  1 deputy success.
*
*=================================================================*/
		  		  
int winks_put_cache_append(int handle, int lenth, char * data)
{

    Winks_DataList_s * currentp;
    currentp = (Winks_DataList_s*)handle;
    for(; currentp->pNext !=NULL; currentp = currentp->pNext)
    {
    }
    currentp->pNext = creat_data_node(lenth);
    Winks_mem_cpy((void*)(currentp->pNext->data), (void*)data, (unsigned long)lenth);
    return 1;
}
/*================================================================
*   winks_setupcapability_cache
*       setup the capability of list .
*       
*   Parameters:
*       coo_value:capability value.
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_setupcapability_cache(int opcode, unsigned long coo_value)
{

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    opcode = 0;
    if (coo_value < (*Winks_CacheContext).totalspace_cache)
    {   
        if ( 0 == coo_value)
        {
            destroy_cache( );
        }
        else
        { 
            if ((*Winks_CacheContext).totalspace_cache - (*Winks_CacheContext).availablespace  > (*Winks_CacheContext).totalspace_cache - coo_value)
            {
                require_availablespace(((*Winks_CacheContext).totalspace_cache - (*Winks_CacheContext).availablespace ) - coo_value);
            }
            else
            {
                (*Winks_CacheContext).availablespace = (*Winks_CacheContext).availablespace  - ((*Winks_CacheContext).totalspace_cache - coo_value);
            }

        }

    }
    else
    {
        (*Winks_CacheContext).availablespace = (*Winks_CacheContext).availablespace  + (coo_value - (*Winks_CacheContext).totalspace_cache);

    }
    (*Winks_CacheContext).totalspace_cache = coo_value;
    return 0;
}

/*================================================================
*   detele_cache
*       detele a  cache from the cache list.
*       
*   Parameters:
*       prior: prior node 
*       node:the cache node to detele
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/
static int detele_cache(Winks_CacheItem_s * prior, Winks_CacheItem_s * node)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    if (node == (*Winks_CacheContext).head)
    {
        (*Winks_CacheContext).head = node->pNext;
    } 

    prior->pNext = node->pNext;
    free_cache( node);
    return 0;
}

/*================================================================
*   detele_cache_tail
*       detele a  cache that is in the list of tail.
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int detele_cache_tail(void)
{
    Winks_CacheItem_s * p;
    Winks_CacheItem_s * prior;
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    if ((*Winks_CacheContext).head != NULL)
    {
        Winks_printf("%s", "(*Winks_CacheContext).head == NULL##############!\n");
        return 0;
    }
    prior = p = (*Winks_CacheContext).head;
    while (p->pNext != NULL)
    {   
        prior = p;
        p = p->pNext;
    }
    detele_cache(prior, p);
    return 0;
}

/*================================================================
*   destroy_cache
*       destroy a  cache list.
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int destroy_cache(void)
{
    Winks_CacheItem_s * p;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    p = (*Winks_CacheContext).head;
    while (p != NULL)
    {   
        (*Winks_CacheContext).head = p->pNext;
        free_datanode_list(p->data_head);
        Winks_salFree(p);
        p = (*Winks_CacheContext).head;
    }
    (*Winks_CacheContext).availablespace = 0;
    return 0;
}

/*================================================================
*   free_datanode_list
*       free a   list node.
*       
*   Parameters:
*       node_head:the cache head  node。
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/
int free_datanode_list(Winks_DataList_s * node_head)
{
    Winks_DataList_s * temp;
    Winks_DataList_s * head;
    head = node_head;
    while (head != NULL)
    {  
        temp = head;
        head = head->pNext;
        Winks_salFree(temp->data);
        Winks_salFree(temp);
    }
    return 0;
}
/*================================================================
*   free_cache
*       free a  cache's space.
*       
*   Parameters:
*       node:the cache node to detele
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int free_cache(Winks_CacheItem_s * node)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    (*Winks_CacheContext).availablespace = (*Winks_CacheContext).availablespace + node->data_lenth + node->data_head->data_lenth + sizeof(Winks_CacheItem_s);//结点长度不计+ sizeof(Winks_CacheItem_s );
    free_datanode_list(node->data_head);
    Winks_salFree(node);
    node = NULL;
    return 0;
}



/*================================================================
*   require_availablespace
*       insure get adequate and available space  for any purpose.
*       
*   Parameters:
*       date_lenth: lenth of the space
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int require_availablespace (unsigned long date_lenth)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    if (date_lenth > (*Winks_CacheContext).totalspace_cache)
    {
        Winks_printf("%s", "can't get require_availablespace( > totalspace_cache)##############!\n");
    }
    while ( date_lenth > (*Winks_CacheContext).availablespace)
    {
        detele_cache_tail();
    }
    return 0;
}

/*================================================================
*   add_cache_head
*       add a cache node in the list head.
*       
*   Parameters:
*       node:the cache node to add
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int add_cache_head(Winks_CacheItem_s * node)
{
    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    if ((*Winks_CacheContext).head == NULL)
    {
        (*Winks_CacheContext).head = node;
        node->pNext = NULL;
    }
    else
    {
        node->pNext = (*Winks_CacheContext).head;
        (*Winks_CacheContext).head = node;
    }
    (*Winks_CacheContext).availablespace = (*Winks_CacheContext).availablespace - (node->data_lenth + node->data_head->data_lenth + sizeof(Winks_CacheItem_s));
    return 0;
}

/*================================================================
*   put_cache
*       put a cache node in the list .
*       
*   Parameters:
*       node:the cache node to add
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int put_cache(Winks_CacheItem_s * node)
{
    /* 判断在列表中有没有相同的cache*/

    Winks_CacheItem_s * p;
    Winks_CacheItem_s * prior;

    WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()

    while (((*Winks_CacheContext).head !=NULL) && (((*Winks_CacheContext).head->url_lenth == node->url_lenth) && (0 == strnicmp((int)(*Winks_CacheContext).head->url + (*Winks_CacheContext).head->data_head->data, (int)node->url + node->data_head->data, (*Winks_CacheContext).head->url_lenth) && ((*Winks_CacheContext).head->method == node->method))
            ||expire_cache_time((*Winks_CacheContext).head)))
    {

        p = (*Winks_CacheContext).head;

        (*Winks_CacheContext).head = (*Winks_CacheContext).head->pNext;
        free_cache( p);
    }
    if ((*Winks_CacheContext).head == NULL)
    {
        if (require_availablespace(node->data_lenth) == 0)
        {
            add_cache_head(node); /*添加到列表的第一位*/
            return 0;
        }
        return -1;
    }
    prior= (*Winks_CacheContext).head;
    for ( p = prior->pNext; p != NULL; )
    {

        if ((node->url_lenth == p->url_lenth) && (0 == strnicmp((int)node->url + node->data_head->data, (int)p->url + p->data_head->data, node->url_lenth))
            ||expire_cache_time(node))
        {
            prior->pNext = p->pNext;
            free_cache( p);
            p = prior->pNext;
            continue;
        }
        prior = p;
        p = p->pNext;
    }

    if (require_availablespace(node->data_lenth) == 0)
    {
        add_cache_head(node); /*添加到列表的第一位*/
        return 0;
    }
    return -1;
}


/*================================================================
*   create_cache
*       create a cache node .
*       
*   Parameters:
*      
*   Return Value:
*       int  return the cache address.
*
*=================================================================*/

Winks_CacheItem_s * create_cache(void)
{
    Winks_CacheItem_s *temp = NULL;

    temp = (Winks_CacheItem_s * )Winks_salAlloc(sizeof(Winks_CacheItem_s));
    if (temp == NULL)
    {
        Winks_assert( temp != NULL) ;
        Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
    }

    temp->url = (char *)NULL;
    temp->url_lenth = 0; 
    temp->expire_time = NULL;
    temp->expire_time_lenth = 0;
    temp->corrected_initial_age = 0;
    temp->respose_time = 0;
    temp->expire_time = NULL;
    temp->expire_time_lenth = 0;
    temp->last_modified_time = NULL; 
    temp->last_modified_time_lenth = 0;
    temp->corrected_initial_age = 0;
    temp->method = 0;
    temp->respose_time = 0;
    temp->max_age = -1;
    temp->max_stale = 0;
    temp->min_fresh = 0;
    temp->data_lenth = 0;
    temp->data_head = NULL;	
    temp->pNext = NULL;
    return temp;
}

/*================================================================
*   creat_data_node
*       create a data node .
*       
*   Parameters:
*      
*   Return Value:
*       int  return the data node's address.
*
*=================================================================*/

Winks_DataList_s * creat_data_node(int lenth)
{
    Winks_DataList_s *temp = NULL;
    char * tempp;
    temp = (Winks_DataList_s * )Winks_salAlloc(sizeof(Winks_DataList_s));
    if (temp == NULL)
    {
        Winks_assert( temp != NULL) ;
        Winks_printf("%s", "Winks_salAlloc  data node  error##############!\n");
    }
    tempp = (char * )Winks_salAlloc(lenth);
    if (tempp == NULL)
    {
        Winks_assert( temp != NULL) ;
        Winks_printf("%s", "Winks_salAlloc  data node  error##############!\n");
    }
    temp->pNext= NULL;
    temp->data_lenth = lenth; 
    temp->data = tempp;
    return temp;
}

/*================================================================
*   cache_init
*       initialize the cache  list .
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_cache_init(void)
{
    FILE * fp;
    /*FILE * currentfp;
    FILE *  priorfp;
    char temp[10]= {0};*/
    Winks_CacheItem_s * node;
    Winks_DataList_s * currentp;
    Winks_DataList_s * tempp;
    char * p;

    unsigned int value = 0;
    unsigned long  buchang = 0;
    unsigned long remained_lenth = 0;
    int lenth = 0;

    Winks_CacheList_s *Winks_CacheContext = NULL;

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    Winks_CacheContext = (Winks_CacheList_s *)Winks_salAlloc(sizeof(Winks_CacheList_s));
    Winks_assert(Winks_CacheContext != NULL);
    set_module_glob(WK_MODULE_CACHE, (void*)Winks_CacheContext);
#else
    Winks_CacheContext = &cachelist;
#endif

    Winks_mem_set(Winks_CacheContext, 0, sizeof(Winks_CacheList_s));

//  WINKS_UI_CONTEXT_DEF()
//  WINKS_UI_CONTEXT_INIT()

    if (0 != Winks_fexist(WINKS_FS_CACHE_PATH))
    {
        (*Winks_CacheContext).head = NULL;
        (*Winks_CacheContext).totalspace_cache = TOTALSPACE_CACHE;
        (*Winks_CacheContext).availablespace = TOTALSPACE_CACHE;
    }
    else
    {
        fp = (FILE *)Winks_fopen((WINKS_FS_CACHE_PATH), FS_O_RDWR);


        if (NULL == fp)
        {
            Winks_assert( fp != NULL);
            Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
        }
        Winks_fseek(fp, -(int)(sizeof(Winks_CacheList_s) ), WINKS_SEEK_END);//SEEK_END
        value =Winks_ftell(fp);
        value = Winks_fread(&(*Winks_CacheContext), sizeof(Winks_CacheList_s), 1, fp);
        Winks_fseek(fp, -(int)(sizeof(Winks_CacheList_s)), WINKS_SEEK_CUR);
        value =Winks_ftell(fp);
        /*if (value == 0)
        {
        Winks_assert( value != 0 );
        Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
        }*/


        while (0 != Winks_ftell(fp))
        {

            node = (Winks_CacheItem_s * )create_cache();
            Winks_fseek(fp, -( int)sizeof(Winks_CacheItem_s), WINKS_SEEK_CUR);
            value = Winks_ftell(fp);
            value = Winks_fread(node, sizeof(Winks_CacheItem_s), 1, fp);
            Winks_fseek(fp, -( int)sizeof(Winks_CacheItem_s), WINKS_SEEK_CUR);
            value =Winks_ftell(fp);
            /*if (value == 0)
            {
            Winks_assert( value != 0 );
            Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
            }*/
            buchang = node->data_lenth;
            /*tempp = Winks_salAlloc(sizeof(Winks_DataList_s));
            if (tempp == NULL)
            {
            Winks_assert( tempp != NULL);
            Winks_printf("%s", "Winks_salAlloc Winks_DataList_s  error##############!\n");
            }*/
            lenth = node->expire_time_lenth + node->last_modified_time_lenth + node->url_lenth + node->content_type_lenth;

            tempp = creat_data_node(lenth);

            node->data_head = tempp;

            Winks_fseek(fp, -lenth, WINKS_SEEK_CUR);
            value = Winks_fread(node->data_head->data, lenth, 1, fp);
            Winks_fseek(fp, -lenth, WINKS_SEEK_CUR);
            currentp = node->data_head;
            remained_lenth = node->data_lenth;
            while(remained_lenth > 0)
            {

                p =  (char * )Winks_salAlloc(buchang);
                if (NULL == p)
                {
                    if(buchang < 512) 
                    {
                        free_cache(node);
                        return -1;
                    }
                    if( buchang % 2)
                    {
                        buchang = buchang >>1;
                    }
                    else
                    {
                        buchang = buchang >>1 + 1;
                    }
                    Winks_printf("%s", "Winks_salAlloc    error##############!\n");
                }
                else

                {
                	Winks_fseek(fp, -(long int)MIN(remained_lenth, buchang ), WINKS_SEEK_CUR);
                	value = Winks_fread(p, MIN(remained_lenth, buchang ), 1, fp);
                	Winks_fseek(fp, -(long int)MIN(remained_lenth, buchang ), WINKS_SEEK_CUR);

                	currentp->pNext =  (Winks_DataList_s * )Winks_salAlloc(sizeof(Winks_DataList_s));
                	currentp->pNext->data = p;
                	currentp->pNext->data_lenth = MIN(remained_lenth, buchang );
                	currentp->pNext->pNext = NULL;
                	remained_lenth = remained_lenth - MIN(remained_lenth, buchang );
                }
            }
            /*node->data =  (char *)Winks_salAlloc(node->tal_data_lenth);
            Winks_fseek(fp, -(long int)(node->tal_data_lenth ), SEEK_CUR);
            value =Winks_ftell(fp);
            value = Winks_fread(node->data, node->tal_data_lenth , 1, fp);
            Winks_fseek(fp, -(long int)(node->tal_data_lenth ), SEEK_CUR);
            value =Winks_ftell(fp);*/
            /* if (0 == value)
            {
            Winks_assert( value != 0 );
            Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
            }*/
            add_cache_head(node);

        }
        Winks_fclose(fp); 
    }
    return 0;

}

/*================================================================
*   cache_close
*        dispose the cache  list before close.
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_cache_close(void)
{
	//int templenth ;
	FILE * fp;
	   
	Winks_CacheItem_s * currentnode;
	Winks_CacheItem_s * priornode;
	Winks_DataList_s * currentdatap;
	
	int value = 0;

	WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()
    
	if (0 == Winks_fexist(WINKS_FS_CACHE_PATH) &&  NULL == (*Winks_CacheContext).head)
	{
		Winks_fdelete(WINKS_FS_CACHE_PATH);
	}
	else
	{ 
		priornode = currentnode = (*Winks_CacheContext).head; 
		
		/*整个列表的过期检查*/
		while  (  NULL != currentnode)
		{
			if (expire_cache_time(currentnode))
			{
				
				if (currentnode == (*Winks_CacheContext).head)
				{
					(*Winks_CacheContext).head = currentnode->pNext;
					free_cache( currentnode);
					currentnode = (*Winks_CacheContext).head;
				} 
				else
				{
					priornode->pNext = currentnode->pNext;
					free_cache( currentnode);
					currentnode = priornode->pNext;
				}
				
				
            }
			else
			{
				priornode = currentnode;
				currentnode = currentnode->pNext;
			}
		}
		if (NULL == (*Winks_CacheContext).head) 
		{
			Winks_fdelete(WINKS_FS_CACHE_PATH);
		#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
	
			if (Winks_CacheContext != NULL)
			{
				Winks_salFree(Winks_CacheContext);
				Winks_CacheContext = NULL;
			}
		#endif
			return 0;
		}
		fp = (FILE *)Winks_fopen(WINKS_FS_CACHE_PATH, FS_O_WRONLY);
		currentnode = (*Winks_CacheContext).head;
		while (NULL != currentnode)
		{
			
			for(currentdatap = currentnode->data_head->pNext; currentdatap != NULL; currentdatap = currentdatap->pNext)
			{
				value = Winks_fwrite(currentdatap->data, currentdatap->data_lenth, 1, fp);
			}
			value = Winks_fwrite(currentnode->data_head->data, currentnode->data_head->data_lenth, 1, fp);
			/*if (0 == value)
			{
			Winks_assert( value != 0);
			Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
    		}*/
			value = Winks_fwrite(currentnode, sizeof(Winks_CacheItem_s), 1, fp);
			/* if (0 == value)
			{
			Winks_assert( value != 0);
			Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
    		}*/
			(*Winks_CacheContext).head = currentnode->pNext;
			free_cache( currentnode);
			currentnode = (*Winks_CacheContext).head;
			
		}
		value = Winks_fwrite(&(*Winks_CacheContext), sizeof(Winks_CacheList_s), 1, fp);
		/*if (value == 0)
		{
		Winks_assert( value != 0);
		Winks_printf("%s", "Winks_fopen cache file  error##############!\n");
				}*/
		//Winks_salFree(&(*Winks_CacheContext));
		Winks_fclose(fp);
	}
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
	
	if (Winks_CacheContext != NULL)
	{
		Winks_salFree(Winks_CacheContext);
		Winks_CacheContext = NULL;
	}
#endif
   	return 0;
}



/*================================================================
*   stringtogmt
*      transformation form GMT'S string  to struct tm.
*       
*   Parameters:
*       gmt:GMT'S string.
*	    time:transformed struct tm.
*
*   Return Value:
*       int  1 deputy .
*
*=================================================================*/

void stringtogmt(char* gmt, Winks_DateTime_s * time)
{ 
	char wday[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 
	char mon[12][4] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun" ,"Jul" ,"Aug","Sep" ,"Oct" ,"Nov" ,"Dec"};
	char * p = gmt;
	char temp[10] = {0};
	
	int i = 0;
	
	strncpy(temp, p, 3);
	temp[3] = '\0';
	for (i = 0; i < 7; i++)
	{
		if (strcmp(wday[i], temp))
		{
			continue;
		}
		time->week = (unsigned char)i;
		break;
	}
	p = p + 5;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->day = (unsigned char)atoi(temp);
	p = p + 3;
	strncpy(temp, p, 3);
	temp[3] = '\0';
	for (i = 0; i < 12; i++)
	{
		if (strcmp(mon[i], temp))
		{
			continue;
		}
		time->month = (unsigned char)i;
		break;
	}
	p = p + 4;
	strncpy(temp, p, 4);
	temp[4] = '\0';
	time->year = atoi(temp);
	
	p = p + 5;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->hour = (unsigned char)atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->min = (unsigned char)atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->sec = (unsigned char)atoi(temp);
	
} 

/*================================================================
*   expire_cache_time
*       judge a cache node while is expire or not .
*       
*   Parameters:
*       node:the cache node.
*
*   Return Value:
*       int  1 deputy expired or expiring.
*
*=================================================================*/

int expire_cache_time(Winks_CacheItem_s * node)
{
	//struct tm currettime;
	Winks_DateTime_s temp_time;
	//time_t ti;
	Winks_DateTime_s dateTime_p;
	
	Winks_TimeZone_e time_zone;
	unsigned long  gtime;
	unsigned long current_age;
	
	//time(&ti);
	Winks_GetSysDateTime(&dateTime_p);
	current_age = node->corrected_initial_age + Winks_mktime(&dateTime_p) - node->respose_time;
	if (node->max_age != -1)
	{
	    if (node->min_fresh != 0)
        {
            if (current_age >= (unsigned long)node->max_age - node->min_fresh)
            {
                return 1;
            } 
            else
            {
                return 0;
            }
        }
        else
        {
            if (current_age >= (unsigned long)node->max_age + node->max_stale)
            {
                return 1;
            } 
            else
            {
                return 0;
            }
        }
	}
	if (NULL != node->expire_time)
	{
	    if (NULL == wk_stristr((int)node->expire_time + node->data_head->data, "GMT"))
        {   
    		if (NULL == strstr((int)node->expire_time + node->data_head->data, "-"))
    		{
    			stringtogmt_rfc850((int)node->expire_time + node->data_head->data,&temp_time);
    		}
    		else
    		{
    			stringtogmt((int)node->expire_time + node->data_head->data,&temp_time);//根据ＧＭＴ字符串填写时间的结构体
    		}/*根据ＧＭＴ字符串填写时间的结构体*/
        }
        else
        {
           stringtogmt_asctime((int)node->expire_time + node->data_head->data,&temp_time);
        }
		Winks_GetSysTimeZone(&time_zone);
		gtime = Winks_mktime(&dateTime_p) - (time_zone -12)*60*60;
		if (gtime >= Winks_mktime(&temp_time))
		{
            return 1;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}

/*================================================================
*   get_value
*       get a cache_control value for the element given.
*       
*   Parameters:
*       name:name of the element.
* 	  cache_control:the string of the cache-control head in the http respose head.
*       value:value of the element given.
*
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int get_value(char * name, char * cache_control, long int *value)
{
	/*char * p;*/
	char * currentcp;
	char tempstr[20] = {0};
	char * tempp;
	int value_lenth;
	currentcp = wk_stristr(cache_control, name);
	if (NULL != currentcp)
	{
		tempp = currentcp + strlen(name);
		while(*tempp ==' ' ||*tempp == '=')
		{
			tempp++;
		}
		for ( currentcp =  tempp; (*currentcp != ',' && *currentcp != ';' && *currentcp != '\0'); currentcp++)
		{	
			
		}
		//	tempp = tempp - (int)cache_control;
		value_lenth= currentcp - tempp;
		strncpy(tempstr, tempp, value_lenth);
		tempstr[value_lenth] = '\0';
		*value = atol(tempstr);
		return 0;
	}
	/*for (p = set_cookie2; *p !='\0'; p++)
	{
			 if (0 == strncmp(p, name, sizeof(name)))
			 {
			 
			   value = p + sizeof(name) + 1 ;//处理找到的想要的值,p + 3;
			   for ( currentcp =  value; *currentcp != (';'|| 'CR'); currentcp++)
			   {	
			   
				 }
				 value = value - (int)set_cookie2;
				 *value_lenth= currentcp - value -1;
				 return 0;
				 }
    }*/
	if (stricmp(name,"max-age") == 0)
		{
			*value = -1;
		}
	else
		{
			*value = 0;   /*//没找到时*/
		}
	return 0;
}


/*================================================================
*   stringtogmt_rfc850
*      transformation form GMT'S string  to struct tm.
*       
*   Parameters:
*       gmt:GMT'S string.
*	  time:transformed struct tm.
*   Return Value:
*       int  1 deputy expired or expiring.
*
*=================================================================*/
void stringtogmt_rfc850(char* gmt, Winks_DateTime_s * time)
{ 
	char wday[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
	char mon[12][4] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun" ,"Jul" ,"Aug","Sep" ,"Oct" ,"Nov" ,"Dec"};
	char * p = gmt;
	char temp[10] = {0};
	
	int i = 0;
	int lenth = 0;
	//for ( ; *p != ','; p++);
	p = strstr(gmt, ",");
	lenth = (int)p - (int)gmt;
	strncpy(temp, gmt, lenth);
	temp[lenth] = '\0';
	for (i = 0; i < 7; i++)
	{
		if (strcmp(wday[i], temp))
		{
			continue;
		}
		time->week = (unsigned char)i;
		break;
	}
	p = p + 2;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->day = (unsigned char)atoi(temp);
	p = p + 3;
	strncpy(temp, p, 3);
	temp[3] = '\0';
	for (i = 0; i < 12; i++)
	{
		if (strcmp(mon[i], temp))
		{
			continue;
		}
		time->month = (unsigned char)(i + 1);
		break;
	}
	p = p + 4;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->year = atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->hour = (unsigned char)atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->min = (unsigned char)atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->sec = (unsigned char)atoi(temp);
} 
/*================================================================
*   stringtogmt_asctime
*      transformation form GMT'S string  to struct tm.
*       
*   Parameters:
*       gmt:GMT'S string.
*	  time:transformed struct tm.
*   Return Value:
*       int  1 deputy expired or expiring.
*
*=================================================================*/
extern void stringtogmt_asctime(char* gmt, Winks_DateTime_s * time)
{ 
	char wday[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 
	char mon[12][4] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun" ,"Jul" ,"Aug","Sep" ,"Oct" ,"Nov" ,"Dec"};
	char * p = gmt;
	char temp[10] = {0};
	
	int i = 0;
	int lenth = 0;
	//for ( ; *p != ','; p++);
	p = strstr(gmt, " ");
	lenth = (int)p - (int)gmt;
	strncpy(temp, gmt, lenth);
	temp[lenth] = '\0';
	for (i = 0; i < 7; i++)
	{
		if (strcmp(wday[i], temp))
		{
			continue;
		}
		time->week = (unsigned char)i;
		break;
	}
	p = p + 1;
	
	strncpy(temp, p, 3);
	temp[3] = '\0';
	for (i = 0; i < 12; i++)
	{
		if (strcmp(mon[i], temp))
		{
			continue;
		}
		time->month = (unsigned char)(i + 1);
		break;
	}
    p = p + 4;
    for (i = 0; *p !=' ';i++,p++) 
	{
		strncpy(&temp[i], p, 1);
	}
	temp[i] = '\0';
	time->day = (unsigned char)atoi(temp);
	p = p + 1;
	//
	
	
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->hour = (unsigned char)atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->min = (unsigned char)atoi(temp);
	
	p = p + 3;
	strncpy(temp, p, 2);
	temp[2] = '\0';
	time->sec = (unsigned char)atoi(temp);
    p = p + 3;
	strncpy(temp, p, 4);
	temp[4] = '\0';
	time->year = atoi(temp);
}

  
