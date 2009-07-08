/*
 *  wk_socket.h
 *  TabiNavi
 *
 *  Created by wang luke on 7/6/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */
#ifndef _SOCKAL_H_
#define _SOCKAL_H_

#ifndef _WK_PUBLIC_H_
#include "wk_public.h"
#endif

#include "wk_osfnc.h"

#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

#include <assert.h>
#include <pthread.h>

/*************************************************************************\
 平台相关的数据类型
 \*************************************************************************/

typedef long int WK_FD;
typedef fd_set WK_FD_SET;

#define WINKS_SO_THREADSTACKSIZE     1024*4
#define WINKS_SO_SYSSECPTICK    1                  //系统每一次心跳折合毫秒数

//平台的错误代码
#define     ENOBUFS        1
#define     ETIMEDOUT      2
#define     EISCONN        3
#define     EOPNOTSUPP     4
#define     ECONNABORTED   5
#define     EWOULDBLOCK    6
#define     ECONNREFUSED   7
#define     ECONNRESET     8
#define     ENOTCONN       9
#define     EALREADY       10
#define     EINVAL         11
#define     EMSGSIZE       12
#define     EPIPE          13
#define     EDESTADDRREQ   14
#define     ESHUTDOWN      15
#define     ENOPROTOOPT    16
#define     EHAVEOOB       17
#define     ENOMEM         18
#define     EADDRNOTAVAIL  19
#define     EADDRINUSE     20
#define     EAFNOSUPPORT   21
#define     EINPROGRESS    22
#define     ELOWER         23    /* lower layer (IP) error */
#define     ENOTSOCK	   24    /* Includes sockets which closed while blocked */
#define     EIEIO		   27    /* bad input/output on Old Macdonald's farm :-) */

/*************************************************************************\
 socket 相关常量和数据类型
 \**************************************************************************/

//Socket 操作码及通知消息事件类型数值
#define WINKS_SO_CONNECT	0x01	//连接
#define WINKS_SO_READ		0x02	//读取
#define WINKS_SO_WRITE		0x04	//写入
#define WINKS_SO_CLOSE	    0x08	//关闭

/* Socket 监视事件掩码 */
#define WINKS_SO_ONCONNECT	0x0100	//连接
#define WINKS_SO_ONREAD		0x0200	//读取
#define WINKS_SO_ONWRITE	0x0400	//写入
#define WINKS_SO_ONCLOSE    0x0800	//关闭

#define WINKS_SO_ONMASK     0x0f00  //监视事件掩码

//socket状态查询轮训时间
#define WINKS_SO_TIME	    200	    //轮训时间（毫秒）

#define WINKS_SO_MAXSONUM	5	    //最大支持SOCKET句柄数
#define WINKS_SO_MAXGHNUM	5	    //最大支持域名查询数
#define WINKS_SO_MAXGHCACHENUM  5   //最大支持域名查询高速缓存数量


#define WINKS_SO_THREADNAME	    ((char* )("WTHREAD"))	//全局线程名称
#define WINKS_SO_GHTHREADNAME	((char* )("WGTHREAD"))	//域名查询线程名称
#define WINKS_SO_EVENTNAME	    ((char* )("WEVENT"))	//全局事件名称
#define WINKS_SO_SOMUTEXNAME	((char* )("WSOMUTEX"))	//Socket句柄互斥锁名称
#define WINKS_SO_GHMUTEXNAME	((char* )("WGHMUTEX"))	//域名查询cache互斥锁名称
#define WINKS_SO_GHSYNCMUTEXNAME	((char* )("WGSMUTEX"))	//域名查询同步互斥锁名称

#define WINKS_SO_NAMELIVETIME   (7*24*60*60*1000)   //域名查询高速缓存生存时间，一周

#define WINKS_SO_GHBFLEN	100	    //域名查询缓冲区建议长度
#define WINKS_SO_MAXNAMELEN     63

//#define WINKS_SO_THREADPRI      150


typedef void (*WK_THREAD_ENTRY)( void* param );

typedef struct tag_OsalThreadParam{
	WK_THREAD_ENTRY  pEntry;
	void* param;
}OsalThreadParam;

#define WINKS_SO_EWOULDBLOCK        -10
#define WINKS_SO_ELOWSYSRESOURCE    -11
#define WINKS_SO_EINVALID_SOCKET    -12
#define WINKS_SO_EINVALIDPARA       -14
#define WINKS_SO_EINPROGRESS        -15
#define WINKS_SO_EOPNOTSUPP         -16
#define WINKS_SO_ECONNABORTED       -17
#define WINKS_SO_EINVAL             -18
#define WINKS_SO_ENOTCONN           -19
#define WINKS_SO_EMSGSIZE           -20
#define WINKS_SO_ENETDOWN           -21
#define WINKS_SO_ERESET             -22
#define WINKS_SO_EUNKNOWERROR       -23


typedef struct tag_Winks_GHCache_s
	{
		char* name;
		unsigned long addr;
		unsigned long live;
		char pdata[WINKS_SO_MAXNAMELEN + 1];
	}Winks_GHCache_s;

typedef struct tag_Winks_hostent 
	{
		char* h_name;           /* official name of host */
		char** h_aliases;  	    /* alias list */
		short h_addrtype;       /* host address type */
		short h_length;         /* length of address */
		char** h_addr_list; 	/* list of addresses */
#define h_addr h_addr_list[0]   /* address, for backward compat */
	}Winks_hostent;

struct winks_sockaddr{ 
	unsigned short sa_family; 
	char sa_data[14]; 
}; 

struct winks_in_addr
{
	unsigned int wk_addr;
};

struct winks_sockaddr_in{ 
	unsigned short sin_family; 
	unsigned short int sin_port; 
	struct winks_in_addr sin_addr; 
	unsigned char sin_zero[8];
};

typedef struct tag_Winks_Socket_s
	{
		WK_FD s;
		unsigned long Opcode;
		WINKS_CHN_ID channel;
		int MsgNum;
	} Winks_Socket_s;

typedef struct tag_Winks_GH_s
	{
		int GHID;
		char name[1];
	}Winks_GH_s;

typedef struct tag_Winks_GetHost_s
	{
		char* pHost;
		int Hostlen;
		WINKS_CHN_ID channel;
		int MsgNum;
		void* ThreadID;
	}Winks_GetHost_s;


/* socket message structure */
typedef struct tag_Winks_Socketmsg_s
	{
		unsigned long wParam;
		unsigned long lParam;
	}Winks_Socketmsg_s;

/* socket brief message, for thread poll socket status */
typedef struct tag_Winks_Sockbri_s
	{
		WK_FD sockhd;
		unsigned long Opcode;
		int index;
	}Winks_Sockbri_s;

typedef struct tag_Winks_EventSock_s
	{
		int num;
		Winks_Sockbri_s s[WINKS_SO_MAXSONUM];
	}Winks_EventSock_s;

//

typedef struct tag_Winks_SockConnectInfo{
	int is_waiting;
	int sockhd;
	struct winks_sockaddr addr_info;
	int addr_len;
}Winks_SockConnectInfo;

typedef struct tag_Winks_SocketALGB_s
	{
		int ifInit;
		unsigned short sockhd;
		unsigned short ghhd;
		unsigned short pushhd;
		unsigned short unused;
		Winks_Socket_s sockcb[WINKS_SO_MAXSONUM];
		Winks_GetHost_s GHcb[WINKS_SO_MAXGHNUM];
		Winks_GHCache_s GHCache[WINKS_SO_MAXGHCACHENUM];
		void* Global_Thread;
		int ifWait;
		void* Global_Event;
		void* Socket_Mutex;
		void* GH_Mutex;
		void* GH_SyncMutex;
		
		int winks_errcode;   //存储的是Winks平台统一规定的错误码
		int platform_errcode; //存储的是各平台的错误码，为方便调试而保存。
		Winks_SockConnectInfo sock_connect_info[WINKS_SO_MAXSONUM]; //记录等待连接的socket
		
	}Winks_SocketALGB_s;

/********************************************************************************\
 对外提供的函数接口
 \********************************************************************************/
int Winks_SoStartup( void );
int Winks_SoCleanup( void );
int Winks_socket( int family, int type, int protocol );
int Winks_asyncselect( int sockhd, int opcode, WINKS_CHN_ID channel, int msg );
int Winks_connect(int sockhd, struct winks_sockaddr* serv_addr, int addrlen );
int Winks_send( int sockhd, void *buf, int len, int flags );
int Winks_recv( int sockhd, void *buf, int len, int flags );
int Winks_closesocket( int sockhd );
int Winks_getlasterror();
int Winks_AsyncGetHostByName( char* name, char* pHost, int hostlen, WINKS_CHN_ID channel, int msg );
int Winks_CancelGetHostByName( int handle );

/********************************************************************************\
 不依赖具体平台的内部函数接口
 \********************************************************************************/

static void winks_SocketThread( void* param );
static void winks_GHThread( void* param );
static Winks_Socket_s* winks_lockhandle( int sockhd );
static Winks_GetHost_s* winks_lockGHhandle( int GHid );
static int winks_ifthreadwait();
static int winks_getsockstatus( Winks_EventSock_s* pSevent );
static unsigned long Winks_GHGetfromCache( char* name );
static int Winks_GHPutinCache( char* name, unsigned long addr );
static int Winks_get_platform_error();
static void winks_set_lasterror(int errCode);

/********************************************************************************\
 依赖具体平台的内部函数接口
 \********************************************************************************/

static void* Winks_CreateMutex( const char* name);
static int Winks_DeleteMutex( void* mutex );
static int Winks_GetMutex( void* mutex, int timeout );
static int Winks_PutMutex( void* mutex );

static int winks_real_connect_socket(int sockhd, struct winks_sockaddr* serv_addr, int addrlen);
static void* Winks_CreateEvent( const char* name);
static int Winks_DeleteEvent( void* event );
static int Winks_GetEvent( void* event, int timeout );
static int Winks_SetEvent( void* event );

static void* Winks_CreateThread(const char* name, WK_THREAD_ENTRY entry,void* param );
static int Winks_DeleteThread( void* thread );

static WK_FD osal_socket(int family , int type , int protocol);
static int osal_sock_close(WK_FD socket);
static int osal_sock_setnonblock(WK_FD socket);
static int osal_sock_connect(WK_FD socket, struct winks_sockaddr* serv_addr, int addrlen);
static int osal_sock_send(WK_FD socket, char *buf, int len);
static int osal_sock_recv(WK_FD socket,char *buf,int len);

static int osal_sock_select(WK_FD_SET *readfds, WK_FD_SET *writefds, WK_FD_SET *excptfds,long timeout);
static void WK_FD_ZERO(WK_FD_SET *set);
static void WK_FD_SET_ADD(WK_FD fd, WK_FD_SET *set);
static int WK_FD_ISSET(WK_FD fd,WK_FD_SET *set);
static void osal_set_last_error(int platform_errcode);

static unsigned long osal_gethostbyname(char *name);
static unsigned int osal_get_tick();

static void osal_thread_sleep(uint32 ms);


#endif
