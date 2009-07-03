//
//  wk_socket.h
//  TabiNavi
//
//  Created by wang luke on 7/3/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface wk_socket : NSObject {
	CFSocketContext _context;
	// Default run loop modes
	NSArray* _runLoopModes;
	
	CFReadStreamRef _readStream;
	CFWriteStreamRef _writeStream;
	int fuck;
}

/////////

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
