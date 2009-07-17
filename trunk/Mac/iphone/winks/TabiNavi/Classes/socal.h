/*
 *  socal.h
 *  TabiNavi
 *
 *  Created by wang luke on 7/15/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#ifndef _SOCKAL_H_
#define _SOCKAL_H_

#ifndef _WK_PUBLIC_H_
#include "wk_public.h"
#endif

#include "wk_osfnc.h"

#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <pthread.h>
//#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
//#include <stdio.h>
#include <stdlib.h>

typedef void *(*WINKS_THREAD_ENTRY)( void* param ); // 线程执行函数

///////////////////////////////////////////////////////////////////////////////////////////
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
#define WINKS_SO_MAXPUSHNUM  5		//最大支持PUSH注册数量

#define WINKS_SO_THREADNAME	    ((char* )("WTHREAD"))	//全局线程名称
#define WINKS_SO_GHTHREADNAME	((char* )("WGTHREAD"))	//域名查询线程名称
#define WINKS_SO_EVENTNAME	    ((char* )("WEVENT"))	//全局事件名称
#define WINKS_SO_SOMUTEXNAME	((char* )("WSOMUTEX"))	//Socket句柄互斥锁名称
#define WINKS_SO_GHMUTEXNAME	((char* )("WGHMUTEX"))	//域名查询互斥锁名称
#define WINKS_SO_GHSYNCMUTEXNAME	((char* )("WGSMUTEX"))	//域名查询同步互斥锁名称

#define WINKS_SO_THREADPRI      150
#define WINKS_SO_THREADSIZE     (4 * 1024)

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

///* Push recv error code */
//#define WINKS_SO_PUSHEDEST  -2
//#define WINKS_SO_PUSHEAPPID -3

#define WINKS_SO_MAXCHANNELLEN  15
#define WINKS_SO_MAXNAMELEN     63
#define WINKS_SO_MAXAPPIDLEN    63

//#define WINKS_SO_MAXPUSHDATALEN	140	//最大PUSH数据长度

#define WINKS_SO_NAMELIVETIME   (7*24*60*60*1000)   //域名查询高速缓存生存时间，一周
#define WINKS_SO_SYSSECPTICK    1 /*16*/                 //系统每一次心跳折合毫秒数

///* Push TPDU num define */
///* Message Type */
//#define WINKS_P_MTIMASK         0x03    /* 00000011 */
//#define WINKS_P_MTIDELIVER      0x00
//#define WINKS_P_MTISTATUSREP    0x02
//#define WINKS_P_MTISUBMIT       0x01
//#define WINKS_P_MTIRESERVED     0x03
///* More Message to Send */
//#define WINKS_P_MMSMASK         0x04    /* 00000100 */
///* Reply Path */
//#define WINKS_P_RPMASK          0x80    /* 10000000 */
///*User Data Header Indicator*/
//#define WINKS_P_UDHIMASK        0x40    /* 01000000 */
///* Status Report Indication */
//#define WINKS_P_SRIMASK         0x20    /* 00100000 */
///* Data Coding Scheme */
///* Data Coding Scheme General Data Code indication */
//#define WINKS_P_DCSHGCMASK      0xc0    /* 11000000 */
//#define WINKS_P_DCSLGCMASK      0x0c    /* 00001100 */
//#define WINKS_P_DCSGCGSM7       0x00
//#define WINKS_P_DCSGC8BIT       0x04
//#define WINKS_P_DCSGCUCS2       0x08
//#define WINKS_P_DCSGCRESERVED   0x0c
//
///* Data Coding Scheme Data coding/message class */
//#define WINKS_P_DCSHDCMASK      0xf0    /* 11110000 */
//#define WINKS_P_DCSLDCMASK      0x04    /* 00000100 */
//#define WINKS_P_DCSDCGSM7       0x00
//#define WINKS_P_DCSDC8BIT       0x04
//
//#define WINKS_P_SCTSLEN         7
//#define WINKS_P_MAXDATALEN      140
//#define WINKS_P_MAXOALEN        12
//
///* Push UDH Indication code */
//#define WINKS_P_UDHREFER8B      0x00    /* Concatenated short message, 8-bit reference number (ref 1 + totalid 1 + nowid 1) */
//#define WINKS_P_UDHPORT8B       0x04    /* Application port addressing scheme, 8 bit address (Dest port 1 + Orig port 1) */
//#define WINKS_P_UDHPORT16B      0x05    /* Application port addressing scheme, 16 bit address (ref 2 + totalid 1 + nowid 1) */
//#define WINKS_P_UDHREFER16B     0x08    /* Concatenated short message, 16-bit reference number (Dest port 2 + Orig port 2) */

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

// Socket控制块，内部使用。
typedef struct tag_Winks_Socket_s
	{
		int s;
		unsigned long Opcode; // socket操作码
		WINKS_CHN_ID channel;
		int MsgNum;
	} Winks_Socket_s;

typedef struct tag_Winks_GH_s
	{
		int GHID;
		char name[1];
	}Winks_GH_s;

// 域名查询控制块
typedef struct tag_Winks_GetHost_s
	{
		char* pHost;
		int Hostlen;
		WINKS_CHN_ID channel;
		int MsgNum;
		void* ThreadID;
	}Winks_GetHost_s;


// Socket全局控制结构
typedef struct tag_Winks_SocketALGB_s
	{
		int ifInit;
		unsigned short sockhd; // 全局Socket句柄申请控制器
		unsigned short ghhd; // 全局域名查询句柄申请控制器
		unsigned short pushhd;
		unsigned short unused;
		Winks_Socket_s sockcb[WINKS_SO_MAXSONUM]; // 全局Socket控制块池
		Winks_GetHost_s GHcb[WINKS_SO_MAXGHNUM]; // 全局域名查询控制块池
		Winks_GHCache_s GHCache[WINKS_SO_MAXGHCACHENUM];
//		Winks_Pushcb_s Pushcb[WINKS_SO_MAXPUSHNUM];
//		Winks_Pushdata_s* ptmpdata;
		void* Global_Thread; // 全局轮询线程
		int ifWait; // 线程等待事件(条件变量)
		//void* Global_Event; // 线程等待事件(条件变量)
		void* Socket_Mutex; // Socket控制块互斥锁
		void* GH_Mutex; // 域名查询控制块互斥锁
		void* GH_SyncMutex;
		
		int winks_errcode;
	}Winks_SocketALGB_s;

/* socket message structure */
typedef struct tag_Winks_Socketmsg_s
	{
		unsigned long wParam;
		unsigned long lParam;
	}Winks_Socketmsg_s;

/* socket brief message, for thread poll socket status */
typedef struct tag_Winks_Sockbri_s
	{
		int sockhd;
		unsigned long Opcode;
		int index;// sockhd
	}Winks_Sockbri_s;
//有事件发生的socket集合结构
typedef struct tag_Winks_EventSock_s
	{
		int num;
		Winks_Sockbri_s s[WINKS_SO_MAXSONUM];
	}Winks_EventSock_s;

//typedef struct tag_Winks_Push_s
//	{
//		char* Appid;
//		WINKS_CHN_ID channel;
//		int msg;
//		unsigned short port;
//	}Winks_Push_s;
//
//typedef struct tag_Winks_PushSMSHead_s
//	{
//		unsigned char* pOA;
//		unsigned short OAlen;
//		unsigned char PID;
//		unsigned char DCS;
//		unsigned char*pScts;
//		int Sctslen;
//		int udlen;
//		unsigned char* pUd;
//	}Winks_PushSMSHead_s;
//
//typedef struct tag_Winks_PushUDH_s
//	{
//		int udhl;
//		unsigned short refer;
//		unsigned char totalid;
//		unsigned char nowid;
//		unsigned short dport;
//		unsigned short oport;
//		unsigned char* pUd;
//	}Winks_PushUDH_s;
//
//typedef struct tag_Winks_Pushbuff_s
//	{
//		int len;
//		unsigned char pScts[WINKS_P_SCTSLEN + 1];            //Service Centre Time Stamp
//		unsigned char* pdata;
//	}Winks_Pushbuff_s;
//
//typedef struct tag_Winks_Pushdata_s
//	{
//		struct tag_Winks_Pushdata_s* pnext;
//		unsigned short num;
//		unsigned short destlen;
//		unsigned short refer;
//		unsigned short unused;
//		unsigned char pDest[WINKS_P_MAXOALEN];
//		unsigned short dport;
//		unsigned short oport;
//		char* pAppid;
//		Winks_Pushbuff_s pbuff[1];
//	}Winks_Pushdata_s;
//
//typedef struct tag_Winks_Pushcb_s
//	{
//		char Appid[WINKS_SO_MAXAPPIDLEN + 1];
//		WINKS_CHN_ID channel;
//		unsigned short port;
//		int msg;
//		Winks_Pushdata_s* ppushdata;
//	}Winks_Pushcb_s;

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
 内部函数接口
 \********************************************************************************/
#pragma mark -
#pragma mark 内部函数接口

unsigned short Winks_htons( unsigned short port );
// 系统心跳(开机运行了多长时间 WINKS_SO_SYSSECPTICK最后换算成毫秒)
static unsigned long Winks_GetTick();
static unsigned long Winks_GHGetfromCache( char* name );
static int Winks_GHPutinCache( char* name, unsigned long addr );
static Winks_GetHost_s* winks_lockGHhandle( int GHid );
static void* winks_GHThread( void* param );
static void* Winks_CreateThread(WINKS_THREAD_ENTRY entry, void* param);
static int Winks_DeleteThread( void* thread );
static int Winks_CreateEvent();
//static void Winks_DeleteEvent();
static int Winks_GetEvent(struct timespec* timeout );
static int Winks_SetEvent(int event);
static void* Winks_CreateMutex();
static int Winks_DeleteMutex( void* mutex );
static int Winks_GetMutex( void* mutex);
static int Winks_PutMutex( void* mutex );
static Winks_Socket_s* winks_lockhandle( int sockhd );
static int winks_ifthreadwait();
static int winks_getsockstatus( Winks_EventSock_s* pSevent );
static void* winks_SocketThread( void* param );

#pragma mark -
#pragma mark PUSH
//static int Winks_PushCheckSMSHead( unsigned char* pdata, int size, Winks_PushSMSHead_s* pSMSHead );
//static int Winks_PushCheckUDHHead( unsigned char* pdata, int size, Winks_PushUDH_s* pUDH );
//static Winks_Pushcb_s* Winks_PushFindCb( unsigned short dport );
//static Winks_Pushdata_s* Winks_PushCreateSeg( Winks_PushSMSHead_s* pSMSHead, Winks_PushUDH_s* pUDH );
//static int Winks_PushAddDataToCb( Winks_Pushcb_s* pPCb, Winks_Pushdata_s* pPData );
//static Winks_Pushdata_s* Winks_PushFindSeg( Winks_PushSMSHead_s* pSMSHead, Winks_PushUDH_s* pUDH );
//static int Winks_PushAppendData( Winks_Pushdata_s* pPData, Winks_PushSMSHead_s* pSMSHead, Winks_PushUDH_s* pUDH );
//
//static int Winks_PushAddDataToTmp( Winks_Pushdata_s* pPData );
//
//static int Winks_PushDumpData( Winks_Pushdata_s* pPData );
//int Winks_PushParse( unsigned char* pdata, int size );
//int Winks_pushcreate( Winks_Push_s* pPush );
//int Winks_pushrecv(int push, char* pDest, int* destlen, 
//				   char* pAppid, int* appidlen, char* pdata, int* datalen );
//int Winks_pushclose( int push );
//int Winks_setdialid( unsigned long id );



#endif
