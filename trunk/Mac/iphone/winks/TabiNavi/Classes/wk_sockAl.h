//
//  wk_sockAl.h
//  TabiNavi
//
//  Created by wang luke on 7/8/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>

// socket 抽象层
@interface wk_sockAl : NSObject {

}

#ifndef _SOCKAL_H_
#define _SOCKAL_H_

#ifndef _WK_PUBLIC_H_
#include "wk_public.h"
#endif

//#include "wk_osfnc.h"

typedef long int WK_FD;
typedef fd_set WK_FD_SET;

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

//关闭网络连接等待时间
#define WINKS_SO_CLEANTIME  (5 * 1000)

#define WINKS_SO_MAXSONUM	5	    //最大支持SOCKET句柄数
#define WINKS_SO_MAXGHNUM	5	    //最大支持域名查询数
#define WINKS_SO_MAXGHCACHENUM  5   //最大支持域名查询高速缓存数量
#define WINKS_SO_MAXPUSHNUM  5		//最大支持PUSH注册数量

#define WINKS_SO_THREADNAME	    ((char* )("WTHREAD"))	//全局线程名称
#define WINKS_SO_GHTHREADNAME	((char* )("WGTHREAD"))	//域名查询线程名称
#define WINKS_SO_EVENTNAME	    ((char* )("WEVENT"))	//全局事件名称
#define WINKS_SO_SOMUTEXNAME	((char* )("WSOMUTEX"))	//Socket句柄互斥锁名称
#define WINKS_SO_GHMUTEXNAME	((char* )("WGHMUTEX"))	//域名查询互斥锁名称
#define WINKS_SO_GHSYNCMUTEXNAME	((char* )("WGSMUTEX"))	//域名查询同步互斥锁名称

#define WINKS_SO_THREADPRI      150
#define WINKS_SO_THREADSIZE     2048

#define WINKS_SO_GHBFLEN	100	    //域名查询缓冲区建议长度

#define WINKS_SO_SUCCESS    0
#define WINKS_SO_FAILURE    -1

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

#define WINKS_SO_MAXCHANNELLEN  15
#define WINKS_SO_MAXNAMELEN     63
#define WINKS_SO_MAXAPPIDLEN    63

#define WINKS_SO_NAMELIVETIME   (7*24*60*60*1000)   //域名查询高速缓存生存时间，一周

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

struct winks_sockaddr
{ 
	unsigned short sa_family; 
	char sa_data[14]; 
}; 

struct winks_in_addr
{
    unsigned long wk_addr;
};

struct winks_sockaddr_in
{ 
	unsigned short sin_family; 
	unsigned short int sin_port; 
	struct winks_in_addr sin_addr; 
	unsigned char sin_zero[8]; 
};

typedef struct tag_Winks_Socket_s
	{
		int s;
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
		unsigned long ReqID;
		char pdata[WINKS_SO_MAXNAMELEN + 1];
	}Winks_GetHost_s;

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
@end
