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
#include "wk_cookie.h"



#define  TOTALSPACE_COOKIE      2000
#define  WINKIS_DOMAIN          "domain"
#define  WINKIS_PATH            "path"
#define  WINKIS_VERSION         "version"
#define  WINKIS_PORT            "port"
#define  WINKIS_COMMENT         "comment"
#define  WINKIS_MAX_AGE         "max-age"
#define  WINKIS_SECURE          "secure"
#define  WINKIS_COMMENTURL      "commentURL"
#define  WINKIS_DISCARD         "discard"
#define  CR                     0X0d

#ifndef  FS_O_RDWR
	#define  FS_O_RDWR          "r"
	#define  FS_O_WRITE         "w"
#endif



static int if_match(char*url,struct tagwkCookieItem * node, int port);
static int get_value2(char * name, char * start_pointer, char* end_pointer, int * value);
static int get_value(char * name, char * start_pointer, char* end_pointer, char ** value, int* value_lenth,struct tagwkCookieItem * node);
static int cmp_cookie(Winks_CookieItem_s * cookie, Winks_CookieItem_s *cookie2);
static int detele_cookie(Winks_CookieItem_s * prior, Winks_CookieItem_s * node);
static int detele_cookie_tail(void);
static int destroy_cookie(void);
static int free_cookie(Winks_CookieItem_s * node);
static int require_availablespace(unsigned long date_lenth); 
static int add_cookie_head(Winks_CookieItem_s * node);
static int put_cookie(struct tagwkCookieItem * node);
static Winks_CookieItem_s * create_cookie(void);
static int mache_domain(char * hostname, char * domain, int lenth);
static int expire_cookie_time(Winks_CookieItem_s * node);
extern void stringtogmt(char* gmt, Winks_DateTime_s * time);
extern void stringtogmt_rfc850(char* gmt, Winks_DateTime_s * time);
extern void stringtogmt_asctime(char* gmt, Winks_DateTime_s * time);
static int full_in_one_cookie(char* url, char * pprior, char * currentcp, char *  date_value, unsigned long  age_value,  unsigned long  request_time, unsigned long respose_time, int port);
static int get_defaultepath(char * url, struct tagwkCookieItem * node , char ** hostname);
//static int get_value2(char * name, char * set_cookie2, int data_lenth, int * value);
static int if_port_in_portlist(int port, char *set_cookie2);
static int get_hostname(char * url, struct tagwkCookieItem * node ,char ** hostname);
static unsigned long tal_node_data_lenth(Winks_CookieItem_s * node);
extern char * wk_stristr(const char * Src,const char *Dest);

//static Winks_CookieList_s  cookielist;
#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
static Winks_CookieList_s  cookielist;
#endif

#if 1
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_UI_CONTEXT_DEF()  \
    Winks_CookieList_s  *Winks_CookieContext = NULL;
#define WINKS_UI_CONTEXT_INIT() \
    Winks_CookieContext = (Winks_CookieList_s *)get_module_glob(WK_MODULE_COOKIE); 

#else
#define WINKS_UI_CONTEXT_DEF()  \
    Winks_CookieList_s *Winks_CookieContext = NULL; 
#define WINKS_UI_CONTEXT_INIT() \
    Winks_CookieContext = &cookielist; 
#endif
#endif
/*================================================================
*   winks_getlenth_cookie
*       return the lenth of a cookie.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       lenth:the lenth of a cookie
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int  winks_getlenth_cookie(char * url, int * lenth, int  port)
{
	Winks_CookieItem_s * node;

	int len = 0;
	int num = 0;

	WINKS_UI_CONTEXT_DEF()
    WINKS_UI_CONTEXT_INIT()


	for(node = (*Winks_CookieContext).head; node != NULL; node = node->pNext)
		{
			if ( 0 == if_match(url, node , port))
			{
				num++;
				len+= node->data_lenth;//计算长度
			}
    	}
	*lenth = len + num - 1;
	return 0;
}


/*================================================================
*   winks_get_cookie
*       return the centent of a cookie.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
         data:the centent of a cookie
*       lenth:the lenth of a cookie data
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_get_cookie(char * url, char * cookie, int  * lenth, int port)
{
	int len = 0;
	int flag = 0;
	char * temp = NULL;
	Winks_CookieItem_s * currentnode;
	Winks_CookieItem_s * prior;
	
	WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()

	Winks_assert(url != NULL || cookie != NULL ||lenth != NULL);

	/*过期扫描濉-将列表中过期的结点删除*/
	while (((*Winks_CookieContext).head !=NULL)  && (1 == expire_cookie_time((*Winks_CookieContext).head)))
	{			
		currentnode = (*Winks_CookieContext).head ;
		(*Winks_CookieContext).head = (*Winks_CookieContext).head->pNext;	
		free_cookie( currentnode);
	}
	if ((*Winks_CookieContext).head == NULL)
		return 0;
	prior= (*Winks_CookieContext).head;
	for ( currentnode = prior->pNext; currentnode != NULL; )
	{
	
		if (1 == expire_cookie_time(currentnode))
		{
			prior->pNext = currentnode->pNext;
			free_cookie( currentnode);
			currentnode = prior->pNext;
			continue;
		}
		prior = currentnode;
		currentnode = currentnode->pNext;
	}
	temp = cookie;
	for ( currentnode = (*Winks_CookieContext).head; currentnode != NULL; currentnode = currentnode->pNext)
	{
	
		if (0 == if_match(url, currentnode,port))
		{	
				/*将对应的数据添加到cookie*/
		    if(flag == 0)
			{
				Winks_mem_cpy(temp, currentnode->data, currentnode->data_lenth);
				temp = temp + currentnode->data_lenth;
				len = len + currentnode->data_lenth;
				flag = 1;
			}
			else
			{
				*temp++ = ',';
				//len++;
				Winks_mem_cpy(temp, currentnode->data, currentnode->data_lenth);
				temp = temp + currentnode->data_lenth;
				len = len + currentnode->data_lenth + 1;// + 1 代替len++;

			}
		
	  	}
		
	}

	*lenth = len;
	
	return len;
}




/*================================================================
*   winks_put_cookie
*       put a  cookie in list.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       set_cookie2:pointer of cookie
*	  set_cookie2_lenth:lenth of cookie
*	  date_value:value of the date
*	  age_value:value of the age
*	  request_time:time of request 
*       respose_time:time that user agent received the respose of request
*	  port:user agent received the respose from the port
*       data:the centent of a cookie
*       lenth:the lenth of a cookie data
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_put_cookie(char* url, char * set_cookie2, long int set_cookie2_lenth, char *  date_value, unsigned long  age_value,  unsigned long  request_time, unsigned long respose_time, int port)
{
	char * pprior;
	char * currentcp;
	//	char * tempp[2] = {0};
	
	//struct tm time;
	//struct tagwkCookieItem * node; 
	
	//	int lenth = 0;
	
	WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()



    if ((unsigned long)set_cookie2_lenth > (*Winks_CookieContext).totalspace_cookie)
    {
      Winks_printf("%s", "can't get require_availablespace( > totalspace_cache)##############!\n");
      return 0;
    }
	pprior = set_cookie2; 	
	/*for (currentcp = pprior; (currentcp = strstr(pprior, ",")) != NULL; currentcp++) 
		{
            full_in_one_cookie(url,pprior,currentcp,date_value,age_value,request_time,respose_time,port);
			pprior = currentcp + 1;
	 	}*/
	currentcp = set_cookie2 + set_cookie2_lenth -1;
	full_in_one_cookie(url,pprior,currentcp,date_value,age_value,request_time,respose_time,port);
    return 0;
              
}

/*================================================================
*   full_in_one_cookie
*      full in a cookie with all element .
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       pprior:pointer started of cookie
*       currentcp:pointer ended of cookie
*	  date_value:value of the date 
*       age_value:value of the age
*	  request_time:time of request 
*       respose_time:time that user agent received the respose of request
*	  port:user agent received the respose from the port
*       data:the centent of a cookie
*       lenth:the lenth of a cookie data
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/
int full_in_one_cookie(char* url, char * pprior, char* currentcp, char *  date_value, unsigned long  age_value,  unsigned long  request_time, unsigned long respose_time, int port)
{
	char * p;
	char * temp;
	char * cp;
	//char * ptemp;
	char * tempp[2] = {0};
	
	Winks_DateTime_s time;
	struct tagwkCookieItem * node; 
	
	int lenth = 0;
	
	WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()

	node = create_cookie();
		
			 //node->data_lenth = sizeof(set_cookie2);
	node->data_lenth = currentcp - pprior + 1;
	stringtogmt(date_value, &time);
        if (respose_time > Winks_mktime(&time)) 
        {
            node->corrected_initial_age =  MAX((respose_time - Winks_mktime(&time)), age_value) + respose_time - request_time;
        }
        else
        {
            node->corrected_initial_age = age_value + respose_time - request_time;
        }
	node->respose_time = respose_time;
	for (temp = pprior; *temp != ';'; temp++ )
	{
	}
	node->name_lenth = temp - pprior;
		
	get_value(WINKIS_DOMAIN, temp + 1, currentcp, &node->domain,  &node->domain_lenth, node);
	if (0 == node->domain_lenth)
	{
		get_hostname(url,  node , &tempp[0]);/*domain defalut*/
				
	}
	get_value(WINKIS_PATH, temp + 1, currentcp, &node->path,  &node->path_lenth, node);
	if (0 == node->path_lenth)
	{
		get_defaultepath( url, node , &tempp[1]);
	}
	get_value(WINKIS_COMMENT, temp + 1, currentcp, &node->comment,  &node->comment_lenth, node);
	get_value(WINKIS_COMMENTURL, temp + 1, currentcp, &node->commentURL,  &node->commentURL_lenth, node);
	get_value2(WINKIS_VERSION, temp + 1,currentcp, &node->version);
	get_value2(WINKIS_SECURE, temp + 1,currentcp, &node->version);
	get_value2(WINKIS_DISCARD, temp + 1,currentcp, &node->version);
	cp = wk_stristr(pprior, WINKIS_PORT);
	if (NULL == cp && cp < currentcp)
	{
		node->port = -1;
	}
	else
	{
		if (NULL != cp && *cp == ';')
	 	{
    		node->port = port;
 		}
		else
		{
	 		node->port = -2;
	 	}
	}
	if (tempp[0] != NULL)
	{
		lenth = lenth + node->domain_lenth;
	}
	if (tempp[1] != NULL)
	{
		lenth = lenth + node->path_lenth;
	}
	lenth = lenth + currentcp - pprior  + 1;
	p = (char *)Winks_salAlloc(lenth +1);
	if (NULL == p)
	{
		Winks_assert( p != NULL) ;
        Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
		}
	strncpy(p, pprior, currentcp - pprior + 1);
	p[ currentcp - pprior + 1] = '\0';
	if (tempp[0] != NULL)
	{
		strncat(p,tempp[0],node->domain_lenth);
	}
	if (tempp[1] != NULL)
	{
		strncat(p,tempp[1],node->path_lenth);
	}
	node->data = p;
	node->name = 0;
	put_cookie(node);
	return 0;
}


/*================================================================
*   if_match
*     judge whether  url and cookie match or not.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       node:node of cookie
*	  port:user agent received the respose from the port
*   Return Value:
*       int  0 deputy match.
*
*=================================================================*/

int if_match(char*url,struct tagwkCookieItem * node , int port)
{
//	char *p;
	char * currentcp;
	char * priorcp;
	
	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	int hostlenth = 0;
	
	if ((unsigned long)node->domain == node->data_lenth)
	{
		priorcp =  url + strlen("http://");
		for (currentcp = priorcp; (*currentcp != ':')&&(*currentcp != '/'); currentcp++)
		{
		}
		hostlenth = currentcp - priorcp;
		if (node->domain_lenth != hostlenth)
		{
			value1 = 1;
		}
		else
		{
			value1 = strnicmp(priorcp, node->domain + (int)node->data, node->domain_lenth);
		}	
	}
	else
	{
		for (currentcp = url + strlen("http://"); (*currentcp != ':') && (*currentcp !='/'); currentcp++)
		{
		}
		value1 = mache_domain(currentcp - 1, ((int)node->domain + node->data + node->domain_lenth -1), node->domain_lenth);
	}
	for ( ; *currentcp != '/'; currentcp++)
	{
	}
	value2 = strncmp(currentcp, (int)node->path + node->data,node->path_lenth);
	if ( node->port == -2)
	{
		value3 = if_port_in_portlist(port, node->data);
	}
	
	if (0 == value1 && 0 == value2 && 0 == value3)
	{
		return 0;
	}
	return 1;
}


/*================================================================
*   if_match
*       judge whether  port  is in the portlist or not.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       node:node of cookie
*	  port:user agent received the respose from the port
*   Return Value:
*       int  0 deputy port in the portlist.
*
*=================================================================*/

int if_port_in_portlist(int port, char *set_cookie2)
{
	char * currentcp;
	char * priorcp;
	char  tempstr[10] = {0};
	
	int len = 0;
	
	currentcp = wk_stristr(set_cookie2, "port");
	if (NULL != currentcp)
	{
		priorcp = currentcp + strlen("port") + 1; 
		for ( currentcp =  priorcp; (*currentcp != ';')&&(*currentcp != CR); currentcp++)
		{	
			 if (*currentcp == ',' ||*currentcp == '<')
		 	{
		 		len = currentcp -priorcp;
		 		strncpy(tempstr, priorcp,len);
				tempstr[len] = '\0';
				if (port == atoi(tempstr))
				{
					return 0;
				}
				priorcp =  currentcp + 1;
		 	}
		} 
	}
	return 1;
}

/*================================================================
*   get_value2
*       get the value  for element.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       node:node of cookie
*	  port:user agent received the respose from the port
*   Return Value:
*       int  0 deputy port in the portlist.
*
*=================================================================*/

int get_value2(char * name, char * start_pointer, char* end_pointer, int * value)
{
	char * currentcp;
	char * priorcp;
	
	char  tempstr[10] = {0};

	int len = 0;
	
	currentcp = wk_stristr(start_pointer, name);
	if (NULL != currentcp && currentcp <= end_pointer)
    {
        if (0 == strnicmp(WINKIS_VERSION, name,strlen(WINKIS_VERSION)))
    	{
    		priorcp = currentcp + strlen(name);
    		while(*priorcp ==' ' ||*priorcp == '=')
			{
				priorcp++;
			}
    		for ( currentcp =  priorcp; (*currentcp != ';') && (currentcp >= end_pointer); currentcp++)
		{
		}
    		len = currentcp -priorcp;
    	 	strncpy(tempstr, priorcp,len);
    		tempstr[len] = '\0';
    		*value = atoi(tempstr);
    		return 1;
    	}
        *value = 1;
    	return 1;

    }
	*value = 0;
	return 0;
	
}

/*================================================================
*   get_hostname
*       get the hostname  form  the url.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       node:node of cookie
*	  hostname:store pointer of the hostname finded
*   Return Value:
*       int  0 deputy port in the portlist.
*
*=================================================================*/

int get_hostname(char * url, struct tagwkCookieItem * node ,char ** hostname)
{
	char * currentcp;
	char * priorcp;
	
	priorcp =  url + strlen("http://");
	for (currentcp = priorcp; (*currentcp != ':')&&(*currentcp != '/'); currentcp++)
	{
	}
    
	node->domain_lenth = currentcp - priorcp;
	*hostname = priorcp;
	node->domain = (char *)node->data_lenth;									
  	return 0;
}

/*================================================================
*   get_hostname
*       get the defaultepath  form  the url.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       node:node of cookie
*	  defaultepath:store pointer of the defaultepath finded
*   Return Value:
*       int  0 deputy port in the portlist.
*
*=================================================================*/

int get_defaultepath(char * url, struct tagwkCookieItem * node , char ** defaultepath)
{
	char * currentcp;
	char * priorcp;
	char * lastcp = NULL;

	priorcp =  url + strlen("http://");
	for (currentcp = priorcp; *currentcp != '/'; currentcp++)
	{
	}
	priorcp = currentcp;
	for (currentcp = priorcp; (*currentcp != '?') && (*currentcp != ' '); currentcp++)
	{
		if (*currentcp == '/')
		{
			lastcp = currentcp;
		}
	}
	* defaultepath = priorcp;
	node->path_lenth = lastcp - priorcp + 1;
	if ((unsigned long)node->domain ==  node->data_lenth)
	{
		//strncpy(node->data + node->data_lenth + node->domain_lenth, priorcp, node->path_lenth);
		node->path = (char *)node->data_lenth + node->domain_lenth;		
	}
	else
	{
		//strncpy(node->data + node->data_lenth, priorcp, node->path_lenth);
		node->path = (char *)node->data_lenth;
	}
  	return 0;
}

/*================================================================
*   get_value
*       get the value  for the element given.
*       
*   Parameters:
*       url: url by http recurrentcpuest 
*       start_pointer:pointer started of cookie
*       end_pointer:pointer ended of cookie
*	  value:value of the element.
*       value_lenth:lenth of the element value 
*	  node:node of a cookie 
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int get_value(char * name, char * start_pointer, char* end_pointer, char ** value, int* value_lenth, struct tagwkCookieItem * node )
{
	/*char * p;*/
	char * currentcp;
	char * priorcp;
	char * temp = NULL;
	currentcp = wk_stristr(start_pointer, name);
	if (NULL != currentcp && currentcp <= end_pointer)
	{
		priorcp = currentcp + strlen(name);
		while(*priorcp == ' ' ||*priorcp == '=')
		{
			priorcp++;
		}
		for ( currentcp = priorcp; (*currentcp != ';')&& (currentcp <= end_pointer); currentcp++)
		{	

		} 
		*value_lenth = currentcp - priorcp;
		temp = priorcp; 
		*value = temp  + node->name_lenth +1 - (int)start_pointer;
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
	*value = NULL;   /*//没找到时*/
	*value_lenth = 0;										
  	return 0;
 }


/*================================================================
*   cmp_cookie
*       judge whether two cookies are same or not.
*       
*   Parameters:
*       cookie:pointer of cookie node
*	  cookie2:pointer of cookie node
*   Return Value:
*       int  0 deputy same.
*
*=================================================================*/

int cmp_cookie(Winks_CookieItem_s * cookie, Winks_CookieItem_s *cookie2)
{
	if ((cookie->name_lenth == cookie->name_lenth) && (0 == strncmp(cookie->name + (int)cookie->data, cookie2->name + (int)cookie2->data, cookie2->name_lenth))
		&&(cookie->domain_lenth == cookie2->domain_lenth) && (0 == strnicmp(cookie->domain + (int)cookie->data, cookie2->domain + (int)cookie2->data,cookie2->domain_lenth))
		&&(cookie->path_lenth==cookie2->path_lenth) &&(0 == strncmp(cookie->path + (int)cookie->data, cookie2->path + (int)cookie2->data,cookie2->path_lenth))
		&&(cookie->port == cookie2->port))
		{   
			return  0;
		}
 	else
		{
			return 1;
		}
}

/*================================================================
*   winks_setupcapability_cookie
*       setup the capability of list .
*       
*   Parameters:
*       coo_value:capability value.
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_setupcapability_cookie (int opcode, unsigned long coo_value)
{

    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	opcode = 0;
 	 if (coo_value < (*Winks_CookieContext).totalspace_cookie)
		{   
			 if ( 0 == coo_value)
				{
					destroy_cookie( );
				}
			else
				{ 
					if ((*Winks_CookieContext).totalspace_cookie - (*Winks_CookieContext).availablespace  > (*Winks_CookieContext).totalspace_cookie - coo_value)
						{
 							require_availablespace(((*Winks_CookieContext).totalspace_cookie - (*Winks_CookieContext).availablespace ) - coo_value);
						}
					else
						{
							(*Winks_CookieContext).availablespace = (*Winks_CookieContext).availablespace  - ((*Winks_CookieContext).totalspace_cookie - coo_value);
						}
					
				}
			
		}
 	 else
		{
			(*Winks_CookieContext).availablespace = (*Winks_CookieContext).availablespace  + (coo_value - (*Winks_CookieContext).totalspace_cookie);
		
		}
	(*Winks_CookieContext).totalspace_cookie = coo_value;
	return 0;
}

/*================================================================
*   detele_cookie
*       detele a  cookie from the cookie list.
*       
*   Parameters:
*       prior: prior node 
*       node:the cookie node to detele
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/
static int detele_cookie(Winks_CookieItem_s * prior, Winks_CookieItem_s * node)
{
    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	if (node == (*Winks_CookieContext).head)
		{
			(*Winks_CookieContext).head = node->pNext;
		} 
	
	prior->pNext = node->pNext;
	free_cookie( node);
	return 0;
}

/*================================================================
*   detele_cookie_tail
*       detele a  cookie that is in the list of tail.
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int detele_cookie_tail(void)
{
	Winks_CookieItem_s * p;
	Winks_CookieItem_s * prior;

    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()

	if ((*Winks_CookieContext).head == NULL)
	{
	
		Winks_printf("%s", "(*Winks_CookieContext).head == NULL##############!\n");
		return 0;
	}
		
	prior = p = (*Winks_CookieContext).head;
	while (p->pNext != NULL)
	{   
		prior = p;
		p = p->pNext;
	}
	detele_cookie(prior, p);
	return 0;
}

/*================================================================
*   destroy_cookie
*       destroy a  cookie list.
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int destroy_cookie(void)
{
	Winks_CookieItem_s * p;
    
	WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	p = (*Winks_CookieContext).head;
	while (p != NULL)
	{   
		(*Winks_CookieContext).head = p->pNext;
		free_cookie(p);
		p = (*Winks_CookieContext).head;
	}
	(*Winks_CookieContext).availablespace = 0;
	return 0;
}

/*================================================================
*   free_cookie
*       free a  cookie's space.
*       
*   Parameters:
*       node:the cookie node to detele
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int free_cookie(Winks_CookieItem_s * node)
{
	WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	//(*Winks_CookieContext).availablespace = (*Winks_CookieContext).availablespace + node->data_lenth + sizeof(Winks_CookieItem_s) + len;
	(*Winks_CookieContext).availablespace = (*Winks_CookieContext).availablespace + sizeof(Winks_CookieItem_s) + tal_node_data_lenth(node);
	Winks_salFree(node->data);
	Winks_salFree(node);
	node = NULL;
	return 0;
}
unsigned long tal_node_data_lenth(Winks_CookieItem_s * node)
{
	unsigned int len = 0;
	
	if ((unsigned long)node->domain == node->data_lenth)
	{
		len = len +  node->domain_lenth;		
	}
	if ((unsigned long)node->path >= node->data_lenth)
	{
		len = len +  node->path_lenth;
	}
	len = len + node->data_lenth;
	return len;
}
/*================================================================
*   require_availablespace
*       insure get  require and available space  for any purpose.
*       
*   Parameters:
*       date_lenth: lenth of the space
*       
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int require_availablespace(unsigned long date_lenth) 

{

    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	while ( date_lenth > (*Winks_CookieContext).availablespace)
	{
		detele_cookie_tail();
	}
	return 0;
}

/*================================================================
*   add_cookie_head
*       add a cookie node in the list head.
*       
*   Parameters:
*       node:the cookie node to add
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int add_cookie_head(Winks_CookieItem_s * node)
{
    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	(*Winks_CookieContext).availablespace = (*Winks_CookieContext).availablespace - sizeof(Winks_CookieList_s) - tal_node_data_lenth(node);
	if ((*Winks_CookieContext).head == NULL)
	{
		(*Winks_CookieContext).head = node;
		node->pNext = NULL;
	}
	else
	{
		node->pNext = (*Winks_CookieContext).head;
		(*Winks_CookieContext).head = node;
	}
 	return 0;
}

/*================================================================
*   put_cookie
*       put a cookie node in the list .
*       
*   Parameters:
*       node:the cookie node to add
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/
//int put_cookie(char*url,struct tagwkCookieItem * node, int port)
int put_cookie(struct tagwkCookieItem * node)
{
  	 /* 判断在列表中有没有相同的cookie*/
   
	Winks_CookieItem_s * currentnode;
	Winks_CookieItem_s * prior;

    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	while (((*Winks_CookieContext).head !=NULL) && (( cmp_cookie (node , (*Winks_CookieContext).head) == 0) ||expire_cookie_time((*Winks_CookieContext).head)))
	{
		
		currentnode = (*Winks_CookieContext).head;
		
		(*Winks_CookieContext).head = (*Winks_CookieContext).head->pNext;
		free_cookie( currentnode);
	}
	if ((*Winks_CookieContext).head == NULL)
	{
		require_availablespace(node->data_lenth);
		add_cookie_head(node); /*添加到列表的第一位*/
		return 0;
	}
	prior= (*Winks_CookieContext).head;
	for ( currentnode = prior->pNext; currentnode != NULL; )
	{
	
		if (( cmp_cookie(node , currentnode) == 0)||expire_cookie_time(node))
		{
			prior->pNext = currentnode->pNext;
			free_cookie( currentnode);
			currentnode = prior->pNext;
			continue;
		}
		prior = currentnode;
		currentnode = currentnode->pNext;
	}
		
	require_availablespace(node->data_lenth);
	add_cookie_head(node); /*添加到列表的第一位*/
	return 0;
}



/*================================================================
*   create_cookie
*       create a cookie node .
*       
*   Parameters:
*      
*   Return Value:
*       int  return the cookie address.
*
*=================================================================*/
Winks_CookieItem_s * create_cookie(void)
{
	Winks_CookieItem_s *temp = NULL;
    
	temp = (Winks_CookieItem_s * )Winks_salAlloc(sizeof(Winks_CookieItem_s));
	if (temp == NULL)
	{
		Winks_assert( temp != NULL ) ;
		Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
	}
	temp->name = NULL ;
	temp->name_lenth = 0; 
 	temp->path = NULL;
  	temp->path_lenth = 0;
 	temp->max_age = -1;
  	temp->corrected_initial_age = 0;
 	temp->respose_time = 0;
 	temp->expire_time = NULL;
 	temp->expire_time_lenth = 0;
 	temp->domain = NULL; 
	temp->domain_lenth = 0;
 	temp->comment = NULL;
	temp->comment_lenth = 0;
	temp->commentURL = NULL;
	temp->commentURL_lenth = 0;
	temp->discard = 0;
	temp->secure = 0;
	temp->version = 1;
	temp->port = 0;
	temp->data_lenth = 0;
	temp->data = NULL;
	temp->pNext = NULL;	
	return temp;
}

/*================================================================
*   mache_domain
*    judge whether domain and hostname are mache or not.
*       
*   Parameters:
*     hostname:
*     domain:
*     lenth:lenth of the domain string.
* 
*   Return Value:
*       int  0 deputy mache.
*
*=================================================================*/

int mache_domain(char * hostname, char * domain, int lenth)
{
	int ch1, ch2;
	
 	 do 
	 {
		 if ( ((ch1 = (unsigned char)(*( hostname --))) >= 'A') &&(ch1 <= 'Z') )
				 ch1 += 0x20;
		 if ( ((ch2 = (unsigned char)(*( domain --))) >= 'A') &&(ch2 <= 'Z') )
				 ch2 += 0x20;
	} while ( -- lenth && ch1 && (ch1 == ch2) );
   	return (ch1 - ch2);

}




/*================================================================
*   cookie_init
*       initialize the cookie  list .
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_cookie_init(void)
{
	FILE * fp;
	/*FILE * currentfp;
	FILE *  priorfp;
      char temp[10]= {0};*/
	Winks_CookieItem_s * node;

	unsigned int value = 0;

    Winks_CookieList_s *Winks_CookieContext = NULL;

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    Winks_CookieContext = (Winks_CookieList_s *)Winks_salAlloc(sizeof(Winks_CookieList_s));
    Winks_assert(Winks_CookieContext != NULL);
    set_module_glob(WK_MODULE_COOKIE, (void*)Winks_CookieContext);
#else
    Winks_CookieContext = &cookielist;
#endif

    Winks_mem_set(Winks_CookieContext, 0, sizeof(Winks_CookieList_s));

//  WINKS_UI_CONTEXT_DEF()
//	WINKS_UI_CONTEXT_INIT()

	if (0 != Winks_fexist(WINKS_FS_COOKIE_PATH))
	{
		(*Winks_CookieContext).head = NULL;
		(*Winks_CookieContext).totalspace_cookie = TOTALSPACE_COOKIE;
		(*Winks_CookieContext).availablespace = TOTALSPACE_COOKIE;
	}
	else
    {
        fp = (FILE *)Winks_fopen((WINKS_FS_COOKIE_PATH), FS_O_RDWR);


        if (NULL == fp)
        {
            Winks_assert( fp != NULL);
            Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
        }
        Winks_fseek(fp, 0, WINKS_SEEK_END);
        value =Winks_ftell(fp);
        Winks_fseek(fp, -(int)(sizeof(Winks_CookieList_s)), WINKS_SEEK_END);//SEEK_END
        value =Winks_ftell(fp);
        value = Winks_fread(&(*Winks_CookieContext), sizeof(Winks_CookieList_s), 1, fp);
        Winks_fseek(fp, -(int)(sizeof(Winks_CookieList_s)), WINKS_SEEK_CUR);
        value =Winks_ftell(fp);
        /*if (value == 0)
        {
        Winks_assert( value != 0 );
        Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
        }*/


        while (0 != Winks_ftell(fp))
        {
             
            node = (Winks_CookieItem_s * )create_cookie();
            Winks_fseek(fp, -( int)sizeof(Winks_CookieItem_s), WINKS_SEEK_CUR);
            value =Winks_ftell(fp);
            value = Winks_fread(node, sizeof(Winks_CookieItem_s), 1, fp);
            Winks_fseek(fp, -( int)sizeof(Winks_CookieItem_s), WINKS_SEEK_CUR);
            value =Winks_ftell(fp);
            /*if (value == 0)
            	{
            		Winks_assert( value != 0 );
            		 Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
            	}*/
            node->data =  (char *)Winks_salAlloc(tal_node_data_lenth(node));
            Winks_fseek(fp, -(long int)(tal_node_data_lenth(node) ), WINKS_SEEK_CUR);
            value =Winks_ftell(fp);
            value = Winks_fread(node->data, tal_node_data_lenth(node) , 1, fp);
            Winks_fseek(fp, -(long int)(tal_node_data_lenth(node)), WINKS_SEEK_CUR);
            value =Winks_ftell(fp);
            /* if (0 == value)
            	{
            		Winks_assert( value != 0 );
            		 Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
            	}*/
            add_cookie_head(node);

        }
        Winks_fclose(fp); 
    }
    return 0;

}


/*================================================================
*   cookie_close
*        dispose the cookie  list before close.
*       
*   Parameters:
*      
*   Return Value:
*       int  0 deputy success.
*
*=================================================================*/

int winks_cookie_close(void)
{
	//int templenth ;
	FILE * fp;
	   
	Winks_CookieItem_s * currentnode;
	Winks_CookieItem_s * priornode;
	
	int value = 0;

    WINKS_UI_CONTEXT_DEF()
	WINKS_UI_CONTEXT_INIT()
	
	if (0 == Winks_fexist(WINKS_FS_COOKIE_PATH) &&  NULL == (*Winks_CookieContext).head)
	{
		Winks_fdelete(WINKS_FS_COOKIE_PATH);
	}
	else
	{ 
		priornode = currentnode = (*Winks_CookieContext).head; 

		/*整个列表的过期检查*/
		while (NULL != currentnode)
        {
            if (expire_cookie_time(currentnode))
            {
            	
            	if (currentnode == (*Winks_CookieContext).head)
            	{
            		(*Winks_CookieContext).head = currentnode->pNext;
            		free_cookie(currentnode);
            		currentnode = (*Winks_CookieContext).head;
            	} 
  	 	        else
            	{
            		priornode->pNext = currentnode->pNext;
            		free_cookie(currentnode);
            		currentnode = priornode->pNext;
      		   	}


              }
            else
            {
            	priornode = currentnode;
            	currentnode = currentnode->pNext;
            }
            }
		if (NULL == (*Winks_CookieContext).head) 
			{
				Winks_fdelete(WINKS_FS_COOKIE_PATH);
				#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
				if (Winks_CookieContext != NULL)
				{
					Winks_salFree(Winks_CookieContext);
					Winks_CookieContext = NULL;
				}
				#endif
				return 0;
			}
		fp = (FILE *)Winks_fopen(WINKS_FS_COOKIE_PATH, FS_O_WRITE);
		currentnode = (*Winks_CookieContext).head;
		while (NULL != currentnode)
		{
			 value = Winks_fwrite(currentnode->data, tal_node_data_lenth(currentnode), 1, fp);
			 /*if (0 == value)
				{
					 Winks_assert( value != 0);
					 Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
				}*/
			 value = Winks_fwrite(currentnode, sizeof(Winks_CookieItem_s), 1, fp);
			/* if (0 == value)
				{
					 Winks_assert( value != 0);
					 Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
				}*/
			(*Winks_CookieContext).head = currentnode->pNext;
			free_cookie( currentnode);
			currentnode = (*Winks_CookieContext).head;
	
		}
        value = Winks_fwrite(&(*Winks_CookieContext), sizeof(Winks_CookieList_s), 1, fp);
		/*if (value == 0)
			{
				Winks_assert( value != 0);
				Winks_printf("%s", "Winks_fopen cookie file  error##############!\n");
			}*/
		//Winks_salFree(&(*Winks_CookieContext));
		Winks_fclose(fp);
	}
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
	
	if (Winks_CookieContext != NULL)
	{
		Winks_salFree(Winks_CookieContext);
		Winks_CookieContext = NULL;
	}
#endif
   	return 0;

}
#if 0
/*================================================================
*   stringtogmt
*      transformation form GMT'S string  to struct tm.
*       
*   Parameters:
*       gmt:GMT'S string.
*	  time:transformed struct tm.
*   Return Value:
*       int  1 deputy expired or expiring.
*
*=================================================================*/

void stringtogmt(char* gmt, Winks_DateTime_s * time)
{ 
	char wday[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 
	char mon[][4] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun" ,"Jul" ,"Aug","Sep" ,"Oct" ,"Nov" ,"Dec"};
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
		time->month = (unsigned char)(i + 1);
		break;
	}
	p = p + 4;
	strncpy(temp, p, 4);
	temp[4] = '\0';
	time->year = (unsigned char)atoi(temp);

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
	char wday[][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
	char mon[][4] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun" ,"Jul" ,"Aug","Sep" ,"Oct" ,"Nov" ,"Dec"};
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
	time->year = (unsigned long)atoi(temp);
    if (time->year >= 70)
	{
		time->year = time->year + 1900;
	}
	else
	{
		time->year = time->year + 2000;
	}
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
#endif
/*================================================================
*   expire_cookie_time
*       judge whether a cookie node  is expire or not .
*       
*   Parameters:
*       node:the cookie node.
*   Return Value:
*       int  1 deputy expired or expiring.
*
*=================================================================*/

int expire_cookie_time(Winks_CookieItem_s * node)
{
	//struct tm time;
	Winks_DateTime_s dateTime_p;
	Winks_DateTime_s temp_time;
	Winks_TimeZone_e time_zone;
	
	unsigned long  gtime;
	unsigned long current_age;

	//time(&ti);
	Winks_GetSysDateTime(&dateTime_p);
	current_age = node->corrected_initial_age + Winks_mktime(&dateTime_p) - node->respose_time;
	if (node->max_age != -1)
	{
		if (current_age >= (unsigned long)node->max_age)
		{
			return 1;
		} 
		else
		{
			return 0;
		}
	}
	if (NULL  !=  node->expire_time)
	{
        if (NULL == wk_stristr((int)node->expire_time + node->data, "GMT"))
        {
    		if (NULL != strstr((int)node->expire_time + node->data, "-"))
    		{
    			stringtogmt_rfc850((int)node->expire_time + node->data,&temp_time);
    		}
    		else
    		{
    			stringtogmt((int)node->expire_time + node->data,&temp_time);//根据ＧＭＴ字符串填写时间的结构体
    		}
        }
        else
        {
            stringtogmt_asctime((int)node->expire_time + node->data,&temp_time);
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
		node->discard = 1;
		return 0;
	}
	return 0;
}

char * wk_stristr(const char * Src,const char *Dest)
{
    char * s1 = (char *)Src;
    char * d1 = (char *)Dest;
    char * d2 = d1;
    char * s2 = s1;

    int i = 1;
    if (s1 == NULL || d1 == NULL)
    {
        return NULL;
    }

    while (i)
    {
        while (('\0' != *s1)&&('\0' != *d1)&&(((*s1)|32) == ((*d1)|32)))
        {
            s1++;
            d1++;
        }

        if (*d1 == '\0')
        {
            return (s2);
        }

        if (*s1 == '\0')
        {
            return NULL;
        }

        d1 = d2;
        s2++;
        s1 = s2;
    }
    return 0;
}

