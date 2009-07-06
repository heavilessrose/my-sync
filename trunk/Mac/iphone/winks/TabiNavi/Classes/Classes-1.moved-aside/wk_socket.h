//
//  wk_socket.h
//  TabiNavi
//
//  Created by wang luke on 7/3/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface wk_socket : NSObject {
	//CFSocketContext _context;
//	// Default run loop modes
//	NSArray* _runLoopModes;
//	
//	CFReadStreamRef _readStream;
//	CFWriteStreamRef _writeStream;
//	int fuck;
}

/////////
/*************************************************
 Sochet接口
 **************************************************/
#ifndef AF_INET

#define SOCKET_ERROR    (-1)
#define SOCK_STREAM     1               /* stream socket */
#define SOCK_DGRAM      2               /* datagram socket */
#define SOCK_RAW        3               /* raw-protocol interface */

#define SOCK_RDM    4
#define SOCK_SEQPACKET    5
#define TCP_NODELAY    0x0001
#define TCP_KEEPIDLE    0x0004          /* Start keeplives after this period */

/*
 * Address families.
 */
#define AF_UNSPEC    0
#define AF_UNIX      1
#define AF_INET      2
#define AF_IMPLINK   3
#define AF_PUP       4
#define AF_CHAOS     5
#define AF_IPX       6
#define AF_NS        6
#define AF_ISO       7
#define AF_OSI       AF_ISO
#define AF_ECMA      8
#define AF_DATAKIT   9
#define AF_CCITT     10
#define AF_SNA       11
#define AF_DECnet    12
#define AF_DLI       13
#define AF_LAT       14
#define AF_HYLINK    15
#define AF_APPLETALK 16
#define AF_NETBIOS   17
#define AF_VOICEVIEW 18
#define AF_FIREFOX   19
#define AF_UNKNOWN1  20
#define AF_BAN       21
#define AF_ATM       22
#define AF_INET6     23
#define AF_CLUSTER   24
#define AF_12844     25
#define AF_IRDA      26
#define AF_NETDES    28                     
#define AF_MAX       29

#endif
typedef struct tag_Winks_Socketmsg_s
	{
		unsigned long wParam;
		unsigned long lParam;
	}Winks_Socketmsg_s;

typedef struct tag_Winks_hostent 
	{
		char* h_name;           /* official name of host */
		char** h_aliases;          /* alias list */
		short h_addrtype;       /* host address type */
		short h_length;         /* length of address */
		char** h_addr_list;     /* list of addresses */
#define h_addr h_addr_list[0]   /* address, for backward compat */
	}Winks_hostent;

struct winks_sockaddr
{ 
    unsigned short sa_family; 
    char sa_data[14]; 
}; 

struct winks_in_addr
{
    unsigned int wk_addr;
};

struct winks_sockaddr_in
{ 
    unsigned short sin_family; 
    unsigned short int sin_port; 
    struct winks_in_addr sin_addr; 
    unsigned char sin_zero[8]; 
};



int Winks_getlasterror( void );


//socket抽象层初始化函数。
int Winks_SoStartup( void );
//socket抽象层关闭函数
int Winks_SoCleanup( void );

//socket传输句柄创建函数
int Winks_socket( int family ,int type ,int protocol );

//socket传输句柄消息设置函数
int Winks_asyncselect( int socket, int opcode, WINKS_CHN_ID channel, int msg );

//socket连接函数
int Winks_connect(int socket, struct winks_sockaddr* serv_addr, int addrlen );

//socket数据发送函数
int Winks_send( int socket, void *buf, int len, int flags );

//socket数据接收函数
int Winks_recv( int socket, void *buf, int len, int flags );

//socket句柄关闭函数
int Winks_closesocket( int socket );

//异步域名查询函数
int Winks_AsyncGetHostByName( char* name, char* pHost, int hostlen, WINKS_CHN_ID channel, int msg );

//异步域名查询放弃函数
int Winks_CancelGetHostByName( int handle );

//本地字节序转换为网络字节序
unsigned short Winks_htons( unsigned short port );
@end
