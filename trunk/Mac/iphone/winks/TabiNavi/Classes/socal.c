/*
 *  socal.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/15/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_public.h"
#include "wk_osfnc.h"
#include "socal.h"

static Winks_SocketALGB_s Winks_SocketALGB;
// 条件变量
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t condLock = PTHREAD_MUTEX_INITIALIZER;
static struct timespec timeout;
#define WAIT 1
#define RESUME 0

#pragma mark -
#pragma mark 向外提供的接口
/*************************************************************************************\
 函数原型：  int Winks_SoStartup( void )
 函数介绍：
 socket抽象层初始化函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 函数返回值：
 成功返回0，失败返回-1。
 实现描述：
 任何socket函数在被调用以前都需要确定本函数被调用。
 本函数初始化socket抽象层的所有资源，创建轮训查询线程，等待用户随后的调用动作。
 \*************************************************************************************/
int Winks_SoStartup( void )
{
    int i = 0;
	
//#if TARGET_IPHONE_SIMULATOR
//	char * dns_svr = "192.168.10.2";
//	
//	tp_os_dns_add_server( dns_svr, DNS_ADD_TO_END );
//#endif
	
    if( Winks_SocketALGB.ifInit )
    {
        Winks_printf( "WINKS socket startup have initialed\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( (Winks_SocketALGB.Socket_Mutex = Winks_CreateMutex( )) == NULL)
    {
        Winks_printf( "WINKS socket startup create socket mutex failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( (Winks_SocketALGB.GH_Mutex = Winks_CreateMutex( )) == NULL)
    {
        Winks_printf( "WINKS socket startup create GH mutex failure\r\n" );
        return WINKS_SO_FAILURE;
    }
    if( (Winks_SocketALGB.GH_SyncMutex = Winks_CreateMutex( )) == NULL)
    {
        Winks_printf( "WINKS socket startup create GH sync mutex failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	Winks_CreateEvent();
//    if( (Winks_SocketALGB.Global_Event = Winks_CreateEvent( (void* )WINKS_SO_EVENTNAME, OS_TRUE )) == (void* )OS_FAILURE )
//    {
//        Winks_printf( "WINKS socket startup create event failure, error %d\r\n", TP_OS_GET_ERRNO() );
//        return WINKS_SO_FAILURE;
//    }
	
    
    for( i = 0; i < WINKS_SO_MAXSONUM; i++ )
    {
        Winks_SocketALGB.sockcb[i].s = -1;
    }
	
    for( i = 0; i < WINKS_SO_MAXGHNUM; i++ )
    {
        Winks_SocketALGB.GHcb[i].ThreadID = NULL;
    }
	
    if( (Winks_SocketALGB.Global_Thread = Winks_CreateThread(winks_SocketThread, NULL )) == NULL )
    {
        Winks_printf( "WINKS socket startup create thread failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    Winks_SocketALGB.ifInit = 1;
	
    return WINKS_SO_SUCCESS;
}

/*************************************************************************************\
 函数原型：  int Winks_SoCleanup( void )
 函数介绍：
 socket抽象层关闭函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 函数返回值：
 成功返回0，失败返回-1。
 实现描述：
 本函数释放所有socket抽象层资源，关闭线程。第一期开发本函数不实现，以存根函数存在。
 \*************************************************************************************/
int Winks_SoCleanup( void )
{
    int i = 0;
	
    if( !Winks_SocketALGB.ifInit )
    {
        Winks_printf( "WINKS socket cleanup have not initialed\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( Winks_SocketALGB.Global_Thread != NULL )
        Winks_DeleteThread( Winks_SocketALGB.Global_Thread );
	
//    if( Winks_SocketALGB.Global_Event != NULL )
//        Winks_DeleteEvent( Winks_SocketALGB.Global_Event );
	
    if( Winks_SocketALGB.GH_Mutex != NULL )
        Winks_DeleteMutex( Winks_SocketALGB.GH_Mutex );
	
    if( Winks_SocketALGB.Socket_Mutex != NULL )
        Winks_DeleteMutex( Winks_SocketALGB.Socket_Mutex );
	
    for( i = 0; i < WINKS_SO_MAXSONUM; i++ )
    {
        if( Winks_SocketALGB.sockcb[i].s != -1 )
            close( Winks_SocketALGB.sockcb[i].s );
    }
	
    for( i = 0; i < WINKS_SO_MAXGHNUM; i++ )
    {
        if( Winks_SocketALGB.GHcb[i].ThreadID != NULL )
            Winks_DeleteThread( Winks_SocketALGB.GHcb[i].ThreadID );
    }
	
    Winks_mem_set( &Winks_SocketALGB, 0, sizeof(Winks_SocketALGB) );
	
    return WINKS_SO_SUCCESS;
}

static void Winks_setlasterror(int err)
{
    Winks_SocketALGB.winks_errcode = err;
}

extern int Winks_getlasterror(void)
{
    return Winks_SocketALGB.winks_errcode;
}

/*************************************************************************************\
 函数原型：  int Winks_socket( int family, int type, int protocol )
 函数介绍：
 socket传输句柄创建函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 family	    int	        socket家族	
 type	    int	        socket类型	
 protocol	int	        socket协议	
 函数返回值：
 成功返回创建出来的socket句柄，失败返回-1。
 实现描述：
 本函数创建socket传输句柄，如果成功会返回创建出来的socket句柄，失败返回-1。
 一旦被调用，首先从全局资源中查找可用的socket句柄资源，如果没找到，返回失败。
 如果找到，则使用用户提供的参数调用平台socket函数，返回成功则告知用户返回成
 功，返回失败则释放申请的资源，同时返回用户失败。
 \*************************************************************************************/
int Winks_socket( int family, int type, int protocol )
{
    int i = 0;
	
    if( !Winks_SocketALGB.ifInit )
    {
        Winks_printf( "WINKS socket creat not initial\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    Winks_GetMutex( Winks_SocketALGB.Socket_Mutex );
    while( i++ < WINKS_SO_MAXSONUM - 1 )
    {
        /* We keep handle 0 as a debug handle, will not alloc 0 to user */
        if( (++Winks_SocketALGB.sockhd) >= WINKS_SO_MAXSONUM )
            Winks_SocketALGB.sockhd = 1;
        if( Winks_SocketALGB.sockcb[Winks_SocketALGB.sockhd].s == -1 )
        {
            /* Get a vaild socket CB */
            if( (Winks_SocketALGB.sockcb[Winks_SocketALGB.sockhd].s = socket( family, type, protocol )) == -1 )
            {
                Winks_SocketALGB.sockhd --;
                Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
                Winks_printf( "WINKS socket create socket failure\r\n" );
                return WINKS_SO_FAILURE;
            }
            Winks_printf( "WINKS socket create socket hd %d, socket %d\r\n", Winks_SocketALGB.sockhd, 
						 Winks_SocketALGB.sockcb[Winks_SocketALGB.sockhd].s );
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            return Winks_SocketALGB.sockhd;// 新建的socket句柄的下标
        }
    }
    Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
	
    Winks_printf( "WINKS socket create socket control block full\r\n" );
    return WINKS_SO_FAILURE;
}

/*************************************************************************************\
 函数原型：  int Winks_asyncselect( int socket, int opcode, void* channel, int channellen, int msg )
 函数介绍：
 socket传输句柄消息设置函数。
 参数说明：
 参数名称	参数类型	    参数介绍	备注
 socket	    int	            socket句柄	
 opcode	    unsigned long	操作码	    目前未用
 channel	    void*	        消息通道名称	
 channellen	int	            消息通道参数长度	
 msg	        int	            消息数值	
 函数返回值：
 成功返回0，失败返回-1。
 实现描述：
 本函数用来设置已经创建好的socket句柄的消息发送内容。任何socket句柄经过本函数设置
 以后默认会变成异步socket，并且设置以后，该socket的任何连接、读取、接收和关闭事件
 发生以后，用户指定的消息通道都会接收到指定的消息，其中消息数据为两个长整形数值，
 如上述结构Winks_Socketmsg_s。在该结构中，wParam指定发生事件的socket句柄数值，lParam
 高16位代表一个错误码，低16位代表发生的事件类型，事件类型定义见上述预定义常量。
 
 一旦被调用，首先检查参数有效值，错误返回失败；如果正确，记录用户设置，将该socket
 设置成非阻塞模式并返回成功。
 \*************************************************************************************/
int Winks_asyncselect( int sockhd, int opcode, WINKS_CHN_ID channel, int msg )
{
    Winks_Socket_s* pSock = NULL;
    int flags = 0;
    int ret = 0;
	
    if( (pSock = winks_lockhandle(sockhd)) == NULL )
    {
        Winks_printf( "WINKS socket asyncselect gethandle failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    pSock->Opcode = opcode;
    pSock->MsgNum = msg;
    pSock->channel = channel;
    ret = fcntl( pSock->s, F_SETFL, O_NONBLOCK );
    flags = fcntl( pSock->s, F_GETFL, 0 );
	
    Winks_printf( "WINKS socket hd %d, num %d asyncselect fcntl return %d flag %d\r\n", sockhd, pSock->s, ret, flags );
	
    Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
    return WINKS_SO_SUCCESS;
}

/*************************************************************************************\
 函数原型：  int Winks_connect(int socket, struct winks_sockaddr* serv_addr, int addrlen )
 函数介绍：
 socket连接函数。
 参数说明：
 参数名称	参数类型	        参数介绍	备注
 socket	    int	                socket句柄	
 serv_addr	struct sockaddr*	目的地址	
 addrlen	    int	                目的地址长度	
 函数返回值：
 成功返回0，失败返回-1。
 实现描述：
 本函数用来与目的服务器进行连接。如果该socket句柄被设置成异步消息模式的话，
 本函数被调用以后，系统会进行后续的连接操作，通常情况下函数会同步返回EINPROGRESS
 错误，提醒用户等待连接结果。一旦连接成功或者失败，用户指定的消息通道里面
 会接收到连接成功或者失败的消息通知。
 一旦被调用，首先检查参数有效性，错误返回失败；如果正确，需要将本地全局socket
 资源里面的操作码设置上连接操作掩码，使用用户参数调用平台连接函数，同时按
 照情况释放全局轮训查询线程等待事件，启动轮训查询工作。向用户返回平台函数
 的返回值。
 连接函数被调用以后，轮训查询函数就需要启动查询该传输句柄的状态，而一旦连
 接成功，则该句柄的数据接收和数据发送消息都需要同时开始监视。
 \*************************************************************************************/
int Winks_connect(int sockhd, struct winks_sockaddr* serv_addr, int addrlen )
{
    Winks_Socket_s* pSock = NULL;
    int ret = 0;
	
    if( (pSock = winks_lockhandle(sockhd)) == NULL )
    {
        Winks_printf( "WINKS socket connect gethandle failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    ret = connect( pSock->s, (struct sockaddr* )serv_addr, addrlen );
	
//    if( ret < 0 && tp_os_net_get_errno() && (tp_os_net_get_errno() != EINPROGRESS) )
	if( ret < 0 && errno && (errno != EINPROGRESS) )
    {
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
        Winks_printf( "WINKS socket connect call system error %d\r\n", strerror(errno) );
        return ret;
    }
	//???:
    if( ret == 0 && pSock->MsgNum )
    {
        /* 屏蔽平台差异 */
        ret = -1;
        Winks_setlasterror(WINKS_SO_EWOULDBLOCK);
    }
	
    Winks_setlasterror(WINKS_SO_EWOULDBLOCK);
    pSock->Opcode |= WINKS_SO_ONCONNECT;
	
    Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
    
    if( winks_ifthreadwait() )
        Winks_SetEvent(RESUME);
	
    return ret;
    
}

/*************************************************************************************\
 函数原型：int Winks_send( int socket, void *buf, int len, int flags )
 函数介绍：
 socket数据发送函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 socket	    int	        socket句柄	
 buf	        void*	    发送数据	
 len	        int	        发送数据长度	
 flags	    int	        发送标记	
 函数返回值：
 成功返回发送的数据长度，失败返回-1。
 实现描述：
 本函数用来向目的服务器发送数据。通常情况下会返回发送数据的长度，当用户
 将该句柄设置成异步消息通知模式的情况下，如果用户提供的数据过长，底层缓
 冲区无法完全保留用户数据的话，函数会返回实际发送的数据长度，极端情况下，
 当底层缓冲区无法装入数据的话，函数会返回EAGAIN错误，通知用户需要稍后发
 送。一旦底层缓冲区允许继续发送数据了，用户指定的消息通道会接收到继续发
 送的消息通知。每次发送数据要求用户需要确保数据发送完成或循环调用本函数
 直到返回EAGAIN错误，否则系统不会上发继续发送消息。如果连接已经被平稳断
 开，函数会返回0。
 一旦被调用，首先检查参数有效性，错误返回失败；如果正确，需要将本地全局
 socket资源里面的操作码删除发送操作掩码，使用用户参数调用平台发送函数。
 向用户返回平台函数的返回值。
 特别的，由于模块本身可能会发送大量的数据，因此本函数的调用有可能因为底
 层缓冲区不够而返回失败，本API要求用户必须循环调用本函数直到数据完全发送
 出去或者因EAGAIN返回失败，否则不会上发继续发送消息，此功能在本函数内部
 处理的时候需要注意。另外，鉴于Linux Socket的关闭处理特点，平台发送函数
 一旦返回0，本函数在告知用户平台返回值的同时，还需补发关闭消息通知应用连
 接已经平稳断连。
 \*************************************************************************************/
int Winks_send( int sockhd, void *buf, int len, int flags )
{
    Winks_Socket_s* pSock = NULL;
    int ret = 0;
    Winks_Socketmsg_s msg;
	
    if( (pSock = winks_lockhandle(sockhd)) == NULL )
    {
        Winks_printf( "WINKS socket send gethandle failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    ret = send( pSock->s, buf, len, flags );
    
    if( ret == 0 )
    {
        /* gracefully disconnect */
        msg.wParam = (unsigned long)(pSock - Winks_SocketALGB.sockcb);
        msg.lParam = WINKS_SO_CLOSE;
        Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
    }
    else if( ret < 0 )
    {
        if( errno != EAGAIN )
        {
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
//            Winks_printf( "WINKS socket send call system error %d\r\n", tp_os_net_get_errno() );
			Winks_printf( "WINKS socket send call system error %d\r\n", strerror(errno) );
        }
        else
        {
            pSock->Opcode |= WINKS_SO_ONWRITE;
            Winks_setlasterror(WINKS_SO_EWOULDBLOCK);
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            if( winks_ifthreadwait() )
                Winks_SetEvent(RESUME);
        }
		
    }
    else
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
	
    return ret;
}

/*************************************************************************************\
 函数原型：  int Winks_recv( int socket, void *buf, int len, int flags )
 函数介绍：
 socket数据发送函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 socket	    int	        socket句柄	
 buf	        void*	    接收数据缓冲区	
 len	        int	        接收数据缓冲区长度	
 flags	    int	        接收标记	
 函数返回值：
 成功返回接收到的数据长度，失败返回-1。
 实现描述：
 本函数用来从目的服务器接收数据，如果用户提供的缓冲区足够长，函数会返回
 实际接收到的数据长度，如果用户缓冲区不足以装下所有数据，函数会返回实际
 接收到的数据（一般情况下会是用户缓冲区的长度）。如果用户将该句柄设置成
 异步消息通知模式的话，一旦该句柄有数据到达，用户指定的消息通道会接收到
 数据到达的消息通知。每次调用都需要用户循环调用本函数知道返回EAGAIN错误，
 否则系统将不会再次发送数据到达消息。如果连接已经被平稳断开，函数会返回0。
 
 一旦被调用，首先检查参数有效性，错误返回失败；如果正确，需要将本地全局
 socket资源里面的操作码删除接收操作掩码，使用用户参数调用平台接收函数。
 向用户返回平台函数的返回值。
 特别的，由于调用本函数提供的缓冲区的不确定性，因此本函数的调用有可能因
 为用户缓冲区不够而返回失败，本API要求用户必须循环调用本函数直到数据完全
 接收完毕并且因EAGAIN返回失败，否则不会再次上发数据到来消息，此功能在本
 函数内部处理的时候需要注意。另外，鉴于Linux Socket的关闭处理特点，平台
 发送函数一旦返回0，本函数在告知用户平台返回值的同时，还需补发关闭消息
 通知应用连接已经平稳断连。
 \*************************************************************************************/
int Winks_recv( int sockhd, void *buf, int len, int flags )
{
    Winks_Socket_s* pSock = NULL;
    int ret = 0;
    Winks_Socketmsg_s msg;
	
    if( (pSock = winks_lockhandle(sockhd)) == NULL )
    {
        Winks_printf( "WINKS socket recv gethandle failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	/*
	 Winks_printf( "WINKS socket %d recv before recv\r\n", pSock->s );
	 {
	 int ret = 0;
	 ret = fcntl( pSock->s, F_GETFL, 0 );
	 Winks_printf( "WINKS socket hd %d, num %d get fcntl return %d\r\n", sockhd, pSock->s, ret );
	 }*/
    ret = recv( pSock->s, buf, len, flags );
    //Winks_printf( "WINKS socket %d recv after recv\r\n", pSock->s );
    
    if( ret == 0 )
    {
        /* gracefully disconnect */
        msg.wParam = (unsigned long)(pSock - Winks_SocketALGB.sockcb);
        msg.lParam = WINKS_SO_CLOSE;
		
        Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
    }
    else if( ret < 0 )
    {
        if( errno != EAGAIN )
        {
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            Winks_printf( "WINKS socket recv call system error %d\r\n", strerror(errno) );
        }
        else
        {
            pSock->Opcode |= WINKS_SO_ONREAD;
            Winks_setlasterror(WINKS_SO_EWOULDBLOCK);
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            if( winks_ifthreadwait() )
                Winks_SetEvent(RESUME);
        }
		
    }
    else
    {
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
        //Winks_printHex((unsigned char *)buf, MIN(ret, 128), (unsigned char *)"Winks_recv");
    }
	
    return ret;
}

/*************************************************************************************\
 函数原型：  int Winks_closesocket( int socket )
 函数介绍：
 socket句柄关闭函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 socket	    int	        socket句柄	
 函数返回值：
 成功返回接收到的数据长度，失败返回-1。
 实现描述：
 本函数用来关闭socket句柄，本函数被调用以后，抽象层本地全局socket资源即会被释放。
 一旦被调用，首先检查参数有效性，错误返回失败；如果正确，需要将本地全局socket
 资源删除，使用用户参数调用平台关闭函数。向用户返回平台函数的返回值。
 特别的，我们不关注该socket句柄是否真的被删除了，考虑到Linux资源足够丰富，我们
 仅仅需要调用平台的关闭函数即可，下层资源的释放由系统处理。
 \*************************************************************************************/
int Winks_closesocket( int sockhd )
{
    Winks_Socket_s* pSock = NULL;
    int ret = 0;
	
    if( (pSock = winks_lockhandle(sockhd)) == NULL )
    {
        Winks_printf( "WINKS socket closesocket gethandle failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    Winks_printf( "WINKS socket closesocket hd %d, socket %d\r\n", sockhd, pSock->s );
	
    ret = close( pSock->s );
	
    if( ret < 0 )
    {
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
        Winks_printf( "WINKS socket closesocket system failure\r\n" );
        return ret;
    }
    else
    {
        Winks_mem_set( pSock, 0, sizeof(Winks_Socket_s) );
        pSock->s = -1;
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
    }
	
    return ret;
}

/*************************************************************************************\
 函数原型：  int Winks_AsyncGetHostByName( char* name, char* pHost, int hostlen, void* channel, 
 int channellen, int msg )
 函数介绍：
 异步域名查询函数。
 参数说明：
 参数名称	参数类型	参数介绍	        备注
 name	    char*	    需要查询的域名	
 pHost	    char*	    域名查询结果缓冲区	本缓冲区要有足够的长度填写查询结果，
 建议长度不要低于预定义常量WINKS_SO_GHBFLEN
 hostlen	    int	        域名查询结果缓冲区长度	
 channel	    void*	    消息通道名称	
 channellen	int	        消息通道名称长度	
 msg	        int	        消息数值	
 函数返回值：
 成功返回域名查询句柄，失败返回-1。如果返回0，代表本函数同步成功返回，用户可以直
 接从提供的缓冲区里面获得域名查询结果。 
 实现描述：
 本函数用来进行异步域名查询，本函数会在资源许可的情况下返回域名查询句柄，系统会
 自动进行随后的域名查询工作，一旦域名查询工作结束，系统会将查询结构填写入用户提
 供的缓冲区并且用户指定的消息通道会接收到指定消息，其中消息数据为两个长整形数值，
 如上述结构Winks_Socketmsg_s。在该结构中，wParam指定发生事件的域名查询句柄数值，
 lParam代表一个错误码。用户在查询成功以后可以通过读取指定的缓冲区内的数据获得域
 名查询结果。
 一旦被调用，首先检查参数有效性，错误返回失败；如果正确，需要在本地全局域名查询
 资源里面申请资源，申请失败返回-1，申请成功的话，通过建立一个独立线程进行域名查
 询，无论查询成功与否，都要向用户发送消息通知查询结果。
 \*************************************************************************************/
int Winks_AsyncGetHostByName( char* name, char* pHost, int hostlen, WINKS_CHN_ID channel, int msg )
{
    Winks_GetHost_s* pHostcb = NULL;
    unsigned long addr = 0;
    struct hostent* phost = (struct hostent* )pHost;
    int len = 0;
    Winks_GH_s* pGH = NULL;
    static int Tname = 0;
#define WINKS_SO_TNAMELEN   16
    char pTname[WINKS_SO_TNAMELEN];
	
    if( name == NULL || pHost == NULL || hostlen < (sizeof(struct hostent) + 4 + 4 + 4) )
    {
        Winks_printf( "WINKS socket gethostbyname parameter error\r\n" );
        return -1;
    }
    
    if( ((addr = inet_addr(name)) != (unsigned long)-1) || (((addr = Winks_GHGetfromCache(name)) != 0)) )
    {
        /* We get it from cache */
        phost->h_name = NULL;
        phost->h_aliases = NULL;
        phost->h_addrtype = AF_INET;
        phost->h_length = 4;
        phost->h_addr_list = (char** )(phost + 1);
        phost->h_addr_list[0] = (char* )(phost->h_addr_list) + (2 * sizeof(char* ));
        phost->h_addr_list[1] = NULL;
        *((unsigned long* )(phost->h_addr_list[0])) = addr;
		
        return 0;
    }
    /* we find a free handle */
    if( (pHostcb = winks_lockGHhandle( WINKS_SO_MAXGHNUM )) == NULL )
    {
        Winks_printf( "WINKS socket gethostbyname cb full\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    /* we save user parameters */
    len = strlen(name);
    pHostcb->channel = channel;
    pHostcb->pHost = pHost;
    pHostcb->Hostlen = hostlen;
    pHostcb->MsgNum = msg;
	
    pGH = Winks_salAlloc( sizeof(int) + len + 1 );
    Winks_mem_set( pGH, 0, (sizeof(int) + len + 1) );
    pGH->GHID = pHostcb - Winks_SocketALGB.GHcb;
    Winks_mem_cpy( pGH->name, name, len );
	
    if( Tname ++ >= 9999 )
        Tname = 0;
    sprintf( pTname, "%s%d", WINKS_SO_GHTHREADNAME, Tname );
	
    if( (pHostcb->ThreadID = Winks_CreateThread( winks_GHThread, pGH )) == NULL )
    {
        Winks_mem_set( pHostcb, 0, sizeof(Winks_GetHost_s) );
        pHostcb->ThreadID = NULL;
        Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
        Winks_printf( "WINKS socket gethostbyname create thread failure\r\n" );
        return WINKS_SO_FAILURE;
    }
    
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
	
    return (pHostcb - Winks_SocketALGB.GHcb);
}

/*************************************************************************************\
 函数原型：  int Winks_CancleGetHostByName( int handle )
 函数介绍：
 异步域名查询放弃函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 handle	    int	        域名查询句柄	
 函数返回值：
 成功返回0，失败返回-1。
 实现描述：
 本函数用来放弃一个正在进行的域名查询请求。用户可以使用本函数放弃一个正在进行
 的域名查询，需要注意的是，系统不能保证放弃过程中是否已经成功的获得了结果，因
 此即使放弃了本次操作，也需要准备在消息通道中接收并处理该域名查询的消息。
 一旦被调用，首先检查参数有效性，错误返回失败；如果正确，会直接关闭对应的域名
 查询线程，返回成功。
 特别的，我们不关注本次操作中是否已经发送过消息了，因此用户仍然需要处理可能到
 来的消息。
 \*************************************************************************************/
int Winks_CancelGetHostByName( int handle )
{
    Winks_GetHost_s* pHostcb = NULL;
	
    if( handle >= WINKS_SO_MAXGHNUM )
    {
        Winks_printf( "WINKS socket gethostbyname cancle param error\r\n" );
        return WINKS_SO_FAILURE;
    }
    /* we find a free handle */
    if( (pHostcb = winks_lockGHhandle( handle )) == NULL )
    {
        Winks_printf( "WINKS socket gethostbyname cancel handle not availble\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( Winks_DeleteThread( pHostcb->ThreadID ) != 0 )
    {
        Winks_printf( "WINKS socket gethostbyname cancel delete thread failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    Winks_mem_set( pHostcb, 0, sizeof(Winks_GetHost_s) );
    pHostcb->ThreadID = NULL;
	
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
	
    return WINKS_SO_SUCCESS;
	
}


/********************************************************************************\
 内部函数接口
 \********************************************************************************/
#pragma mark -
#pragma mark 内部函数接口

unsigned short Winks_htons( unsigned short port )
{
    return (unsigned short)htons(port);
}

// 系统心跳(开机运行了多长时间 WINKS_SO_SYSSECPTICK最后换算成毫秒)
static unsigned long Winks_GetTick(){
	
}

static unsigned long Winks_GHGetfromCache( char* name )
{
    int i = 0;
	
    Winks_GetMutex( Winks_SocketALGB.GH_Mutex );
    for( i = 0; i < WINKS_SO_MAXGHCACHENUM; i++ )
    {
        if( Winks_SocketALGB.GHCache[i].name != NULL )
        {
            if( Winks_GetTick() > Winks_SocketALGB.GHCache[i].live )
            {
                /* timeout, drop */
                if( Winks_SocketALGB.GHCache[i].name != Winks_SocketALGB.GHCache[i].pdata )
                    Winks_salFree(Winks_SocketALGB.GHCache[i].name);
                Winks_SocketALGB.GHCache[i].name = NULL;
            }
            else if( stricmp(Winks_SocketALGB.GHCache[i].name, name) == 0 )
            {
                Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
                return Winks_SocketALGB.GHCache[i].addr;
            }
        }   
    }
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
	
    return 0;
}

static int Winks_GHPutinCache( char* name, unsigned long addr )
{
    int i = 0, len = 0;
	
    if( name == NULL || addr == 0 )
    {
        Winks_printf( "WINKS socket put in gh cache param error\r\n" );
        return -1;
    }
	
    Winks_GetMutex( Winks_SocketALGB.GH_Mutex );
    for( i = 0; i < WINKS_SO_MAXGHCACHENUM; i++ )
    {
        if( (Winks_GetTick() > Winks_SocketALGB.GHCache[i].live) || (Winks_SocketALGB.GHCache[i].name == NULL) )
        {
            if( (Winks_SocketALGB.GHCache[i].name != NULL) &&
			   (Winks_GetTick() > Winks_SocketALGB.GHCache[i].live) )
            {
                /* timeout, drop */
                if( Winks_SocketALGB.GHCache[i].name != Winks_SocketALGB.GHCache[i].pdata )
                    Winks_salFree(Winks_SocketALGB.GHCache[i].name);
                Winks_SocketALGB.GHCache[i].name = NULL;
            }
            Winks_SocketALGB.GHCache[i].live = Winks_GetTick() + 
			(WINKS_SO_NAMELIVETIME / WINKS_SO_SYSSECPTICK);
            if( (len = strlen(name)) > WINKS_SO_MAXNAMELEN )
                Winks_SocketALGB.GHCache[i].name = Winks_salAlloc( len + 1 );
            else
                Winks_SocketALGB.GHCache[i].name = Winks_SocketALGB.GHCache[i].pdata;
            Winks_mem_cpy( Winks_SocketALGB.GHCache[i].name, name, len );
            Winks_SocketALGB.GHCache[i].name[len] = 0;
            Winks_SocketALGB.GHCache[i].addr = addr;
            Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
            
            return i;
        }   
    }
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
	
    return -1;
}

static Winks_GetHost_s* winks_lockGHhandle( int GHid )
{
    int i = 0;
	
    Winks_GetMutex( Winks_SocketALGB.GH_Mutex );
	
    if( GHid >= WINKS_SO_MAXGHNUM )
    {
        /* User want to find a free block */
        while( i++ < WINKS_SO_MAXGHNUM - 1 )
        {
            /* We keep handle 0 as a debug handle, will not alloc 0 to user */
            if( (++Winks_SocketALGB.ghhd) >= WINKS_SO_MAXGHNUM )
                Winks_SocketALGB.ghhd = 1;
            if( Winks_SocketALGB.GHcb[Winks_SocketALGB.ghhd].ThreadID == NULL )
            {
                return &(Winks_SocketALGB.GHcb[Winks_SocketALGB.ghhd]);
            }
        }
        Winks_printf( "WINKS socket get GH handle failure\r\n" );
        Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
        return NULL;
    }
    else
    {
        /* User want to find a vaild block */
        if( GHid < 0 )
        {
            Winks_printf( "WINKS socket get GH handle ID error\r\n" );
            Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
            return NULL;
        }
        if( Winks_SocketALGB.GHcb[GHid].ThreadID == NULL )
        {
            Winks_printf( "WINKS socket get GH handle invalid\r\n" );
            Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
            return NULL;
        }
		
        return &(Winks_SocketALGB.GHcb[GHid]);
    }
	
    /* will not come here */
    Winks_fail( "gethost lock handle unknow error\r\n" );
    return NULL;
	
}

static void* winks_GHThread( void* param )
{
    Winks_GH_s* pGH = param;
    struct hostent* pHost = NULL;
    Winks_hostent* phost = NULL;
    Winks_GetHost_s* pHostcb = NULL;
    Winks_Socketmsg_s msg;
	
    if( pGH == NULL )
    {
        Winks_printf( "WINKS socket get GH thread param error\r\n" );
        return NULL;
    }
	
    /* We user a mutex to sync gh thread */
    Winks_GetMutex( Winks_SocketALGB.GH_SyncMutex );
    Winks_printf( "WINKS socket GH thread call %s gethostbyname\r\n", pGH->name );
    pHost = gethostbyname( (const char* )pGH->name );
    Winks_printf( "WINKS socket GH thread call %s gethostbyname return %x\r\n", pGH->name, pHost );
	
    if( (pHostcb = winks_lockGHhandle( pGH->GHID )) == NULL )
    {
        Winks_printf( "WINKS socket GH thread lock handle error\r\n" );
        Winks_PutMutex( Winks_SocketALGB.GH_SyncMutex );
        return NULL;
    }
	
    if( pHost == NULL )
    {
        msg.wParam = (unsigned long )pGH->GHID;
        msg.lParam = (unsigned long )WINKS_SO_FAILURE;
        Winks_PostMsg( pHostcb->channel, (unsigned long )(pHostcb->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
        Winks_mem_set( pHostcb, 0, sizeof(Winks_GetHost_s) );
        pHostcb->ThreadID = NULL;
		
        Winks_printf( "WINKS socket GH thread gethostbyname return failure\r\n" );
        Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
        Winks_PutMutex( Winks_SocketALGB.GH_SyncMutex );
		
        Winks_salFree(pGH);
        return NULL;
    }
	
    phost = (Winks_hostent* )(pHostcb->pHost);
    phost->h_name = NULL;
    phost->h_aliases = NULL;
    phost->h_addrtype = AF_INET;
    phost->h_length = 4;
    phost->h_addr_list = (char** )(phost + 1);
    phost->h_addr_list[0] = (char* )(phost + 1) + 2 * sizeof(char* );
    phost->h_addr_list[1] = NULL;
    *((unsigned long* )(phost->h_addr_list[0])) = *((unsigned long* )(pHost->h_addr_list[0]));
	
    msg.wParam = (unsigned long )pGH->GHID;
    msg.lParam = WINKS_SO_SUCCESS;
    Winks_PostMsg( pHostcb->channel, (unsigned long )(pHostcb->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
    Winks_mem_set( pHostcb, 0, sizeof(Winks_GetHost_s) );
    pHostcb->ThreadID = NULL;
    
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
    Winks_PutMutex( Winks_SocketALGB.GH_SyncMutex );
	
    Winks_GHPutinCache( pGH->name, *((unsigned long* )(pHost->h_addr_list[0])) );
    Winks_salFree(pGH);
    return NULL;
}


static void* Winks_CreateThread(WINKS_THREAD_ENTRY entry, void* param)
{
	pthread_attr_t attr;
	//???:是否需要定义为全局的？ 
	pthread_t posixThreadID;
	
	assert(!pthread_attr_init(&attr));
	// 线程以分离状态启动，在线程退出时立即回收资源
	assert(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
	
	int threadError = pthread_create(&posixThreadID, &attr, entry, param);
	assert(!pthread_attr_destroy(&attr));
	if (threadError != 0){
		printf("Winks_CreateThread: %s\n", strerror(threadError));
		return NULL;
	}
	//???:pthread_t本身就是一个指针？
	return (void*)&posixThreadID;
}

// 
static int Winks_DeleteThread( void* thread )
{
	int ret;
	//???: 异步过程?
	if((ret = pthread_cancel((pthread_t)thread)) != 0){
		printf("Winks_DeleteThread: %s\n", strerror(ret));
	}
    return ret;
}

static int Winks_CreateEvent()
{
	Winks_SocketALGB.ifWait = 1;
//	condition = PTHREAD_COND_INITIALIZER;
//	condLock = PTHREAD_MUTEX_INITIALIZER;
    return 0; // 返回条件变量
}

//static void Winks_DeleteEvent()
//{
//    Winks_SocketALGB.Global_Event ＝ 0;
//}

static int Winks_GetEvent(struct timespec* timeout )
{
	pthread_mutex_lock(&condLock);
	while(WAIT == Winks_SocketALGB.ifWait){
		printf("Winks_GetEvent: poll thread starting wait\n");
		pthread_cond_wait(&condition, &condLock);
		//pthread_cond_timedwait(&condition, &condLock, &timeout);
	}
	pthread_mutex_unlock(&condLock);
    return 0;
}

static int Winks_SetEvent(int event)
{
	pthread_mutex_lock(&condLock);
	Winks_SocketALGB.ifWait = event;
	pthread_mutex_unlock(&condLock);
	if(event == RESUME)
		pthread_cond_signal(&condition);
	
    return 0;
}


static void* Winks_CreateMutex()
{
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	return (void*)&mutex;
}

static int Winks_DeleteMutex( void* mutex )
{
    return pthread_mutex_destroy((pthread_mutex_t*)mutex);
}

static int Winks_GetMutex( void* mutex)
{
    return pthread_mutex_lock((pthread_mutex_t*)mutex);
}

static int Winks_PutMutex( void* mutex )
{
    return pthread_mutex_unlock((pthread_mutex_t*)mutex);
}

/*static int Winks_PostMessage( void* channel, int msg, void* msgdata, int msgsize )
 {
 #ifdef WINKS_DEBUG
 if( FALSE == tp_comm_channel_query((const char* )channel) )
 {
 Winks_printf( "WINKS postmessage channel %s not exist\r\n", (char* )channel );
 return WINKS_SO_FAILURE;
 }
 #endif
 return (int)tp_comm_channel_post_msg( (const char* )channel, (unsigned long)msg, 
 msgdata, (unsigned long)msgsize );
 }*/


static Winks_Socket_s* winks_lockhandle( int sockhd )
{
    Winks_Socket_s* pSock = NULL;
	
    if( !Winks_SocketALGB.ifInit )
    {
        Winks_printf( "WINKS socket get handle not initial\r\n" );
        return NULL;
    }
    if( sockhd < 0 && sockhd >= WINKS_SO_MAXSONUM )
    {
        Winks_printf( "WINKS socket get handle socket invalid in range\r\n" );
        return NULL;
    }
	
    pSock = &(Winks_SocketALGB.sockcb[sockhd]);
	
    Winks_GetMutex( Winks_SocketALGB.Socket_Mutex );
    if( pSock->s == -1 )
    {
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
        Winks_printf( "WINKS socket get handle socket invalid\r\n" );
        return NULL;
    }
	
    return pSock;
}

static int winks_ifthreadwait()
{
    return Winks_SocketALGB.ifWait;
}

// 返回已有事件发生的socket数＋1，并加入到Winks_EventSock_s
static int winks_getsockstatus( Winks_EventSock_s* pSevent )
{
    int ret = -1, i = 0;
	
    Winks_mem_set( pSevent, 0, sizeof(Winks_EventSock_s) );
	
    Winks_GetMutex( Winks_SocketALGB.Socket_Mutex );
    for( i = 0; i < WINKS_SO_MAXSONUM; i ++ )
    {
        if( (Winks_SocketALGB.sockcb[i].s != -1) && (Winks_SocketALGB.sockcb[i].Opcode & WINKS_SO_ONMASK) )
        {
            if( ret < Winks_SocketALGB.sockcb[i].s )
                ret = Winks_SocketALGB.sockcb[i].s;
            pSevent->s[pSevent->num].sockhd = Winks_SocketALGB.sockcb[i].s;
            pSevent->s[pSevent->num].Opcode = Winks_SocketALGB.sockcb[i].Opcode;
            pSevent->s[pSevent->num].index = i;
            pSevent->num ++;
        }
    }
    Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
	
    return ret + 1;
}
/*************************************************************************************\
 函数原型：  static void* winks_SocketThread( void* param )
 函数介绍：
 socket轮训事件查询线程函数。
 参数说明：
 参数名称	参数类型	参数介绍	备注
 param	    void* 	    socket全局控制块	
 函数返回值：
 无。
 实现描述：
 本函数为socket轮询事件查询函数，用来查询socket事件并发送消息。本函数为
 内部关键函数，不对外公开。
 本函数按照用户调用情况，使用一个固定时间和select函数轮询查询所有活动socket
 的状态，通过检查各个socket的状态，发送事件通知消息。
 \*************************************************************************************/
static void* winks_SocketThread( void* param )
{
    int nfds = 0, ret = 0, i = 0, lret = 0;
    Winks_EventSock_s SEvent;
    Winks_Socketmsg_s msg;
    
    Winks_Socket_s* pSock = NULL;
    fd_set rdfds;
    fd_set wrfds;
    fd_set expfds;
    struct timeval times;
	
    times.tv_sec = 0;
    times.tv_usec = WINKS_SO_TIME * 1000;   //200 microsecond
	
    /* firstly we wait in global event for any event */
    //Winks_SocketALGB.ifWait = 1;
	Winks_SetEvent(WAIT);
    Winks_printf( "WINKS socket thread initial wait\r\n" );
    Winks_GetEvent( &timeout );
    Winks_printf( "WINKS socket thread initial wakeup\r\n" );
    //Winks_SocketALGB.ifWait = 0;
	Winks_SetEvent(RESUME);
	
    
    /* when get event, set flags first */
	
    while( 1 )
    {
        times.tv_sec = 0;
        times.tv_usec = WINKS_SO_TIME * 1000;   // = 200 ms
		
        if( (nfds = winks_getsockstatus( &SEvent )) > 0 )
        {
            Winks_printf( "WINKS socket thread get live socket, num = %d\r\n", SEvent.num );
            FD_ZERO( &rdfds );
            FD_ZERO( &wrfds );
            FD_ZERO( &expfds );
            for( i = 0; i < SEvent.num; i++ )
            {
                if( (SEvent.s[i].Opcode & WINKS_SO_ONCONNECT) )
                {
                    Winks_printf( "WINKS socket thread get onconnect socket %d\r\n", SEvent.s[i].sockhd );
                    FD_SET( SEvent.s[i].sockhd, &wrfds );
                    FD_SET( SEvent.s[i].sockhd, &expfds );
                }
                if( (SEvent.s[i].Opcode & WINKS_SO_ONWRITE) )
                {
                    Winks_printf( "WINKS socket thread get onwrite socket %d\r\n", SEvent.s[i].sockhd );
                    FD_SET( SEvent.s[i].sockhd, &wrfds );
                }
                if( (SEvent.s[i].Opcode & WINKS_SO_ONREAD) )
                {
                    Winks_printf( "WINKS socket thread get onread socket %d\r\n", SEvent.s[i].sockhd );
                    FD_SET( SEvent.s[i].sockhd, &rdfds );
                }
            }
            ret = select( nfds, &rdfds, &wrfds, &expfds, &times );
			
            if( ret > 0 )
            {
                /* some event occured */
                for( i = 0; i < SEvent.num; i++ )
                {
                    if( FD_ISSET(SEvent.s[i].sockhd, &wrfds) )
                    {
                        if( (pSock = winks_lockhandle( SEvent.s[i].index )) != NULL )
                        {
                            if( SEvent.s[i].Opcode & WINKS_SO_ONCONNECT )
                            {
                                Winks_printf( "WINKS socket thread poll onconnect event %d\r\n", SEvent.s[i].sockhd );
								
                                pSock->Opcode &= ~WINKS_SO_ONCONNECT;
                                msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                                msg.lParam = WINKS_SO_CONNECT;
                                lret = Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                                Winks_printf( "WINKS socket thread postmsg ret %d\r\n", lret );
                                pSock->Opcode |= WINKS_SO_ONREAD | WINKS_SO_ONWRITE;
                            }
                            else if( SEvent.s[i].Opcode & WINKS_SO_ONWRITE )
                            {
                                Winks_printf( "WINKS socket thread poll onwrite event %d\r\n", SEvent.s[i].sockhd );
                                pSock->Opcode &= ~WINKS_SO_ONWRITE;
                                msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                                msg.lParam = WINKS_SO_WRITE;
                                lret = Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                                Winks_printf( "WINKS socket thread postmsg ret %d\r\n", lret );
                            }
                            else
                            {
                                Winks_printf( "WINKS socket thread poll write event %d, but socket no flags\r\n", SEvent.s[i].sockhd );
                            }
                            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
                        }
                        else
                        {
                            Winks_printf( "WINKS socket thread poll write event %d, but socket invalid\r\n", SEvent.s[i].sockhd );
                        }
                    }
                    if( FD_ISSET(SEvent.s[i].sockhd, &rdfds) )
                    {
                        if( (pSock = winks_lockhandle( SEvent.s[i].index )) != NULL )
                        {
                            Winks_printf( "WINKS socket thread poll onreceive event %d\r\n", SEvent.s[i].sockhd );
                            pSock->Opcode &= ~WINKS_SO_ONREAD;
                            msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                            msg.lParam = WINKS_SO_READ;
                            lret = Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                            Winks_printf( "WINKS socket thread postmsg ret %d\r\n", lret );
                            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
                        }
                        else
                        {
                            Winks_printf( "WINKS socket thread poll read event %d, but socket invalid\r\n", SEvent.s[i].sockhd );
                        }
                    }
#if 0
                    if( FD_ISSET(SEvent.s[i].sockhd, &expfds) )
                    {
                        if( (pSock = winks_lockhandle( SEvent.s[i].index )) != NULL )
                        {
                            Winks_printf( "WINKS socket thread poll onconnect failure event %d\r\n", SEvent.s[i].sockhd );
                            pSock->Opcode &= ~WINKS_SO_ONCONNECT;
                            msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
//                            msg.lParam = WINKS_SO_CONNECT | (tp_os_net_get_errno() << 16);
                            msg.lParam = WINKS_SO_CONNECT | errno << 16);
                            lret = Winks_PostMsg( pSock->channel, (void* )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                            Winks_printf( "WINKS socket thread postmsg ret %d\r\n", lret );
                            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
                        }
                        else
                        {
                            Winks_printf( "WINKS socket thread poll except event %d, but socket invalid\r\n", SEvent.s[i].sockhd );
                        }
                        
                    }
#endif
                }
                
            }
            else
            {
//                Winks_printf( "WINKS socket thread select return error %d\r\n", tp_os_net_get_errno() );
				Winks_printf( "WINKS socket thread select return error %d\r\n", strerror(errno) );
            }
        }
        else
        {
//            Winks_SocketALGB.ifWait = 1;
			Winks_SetEvent(WAIT);
            Winks_printf( "WINKS socket thread wait\r\n" );
            Winks_GetEvent( &timeout );
            Winks_printf( "WINKS socket thread wakeup\r\n" );
//            Winks_SocketALGB.ifWait = 0;
			Winks_SetEvent(RESUME);
        }
    }
	
}


/*************************************************************************************\
 PUSH
\*************************************************************************************/
#pragma mark -
#pragma mark PUSH
//static int Winks_PushCheckSMSHead( unsigned char* pdata, int size, Winks_PushSMSHead_s* pSMSHead )
//{
//
//    return off;
//}
//
//static int Winks_PushCheckUDHHead( unsigned char* pdata, int size, Winks_PushUDH_s* pUDH )
//{
//
//    return off;
//}
//
//static Winks_Pushcb_s* Winks_PushFindCb( unsigned short dport )
//{
//
//    return NULL;
//}
//
//static Winks_Pushdata_s* Winks_PushCreateSeg( Winks_PushSMSHead_s* pSMSHead, Winks_PushUDH_s* pUDH )
//{
//
//    return pPData;
//	
//}
//
//static int Winks_PushAddDataToCb( Winks_Pushcb_s* pPCb, Winks_Pushdata_s* pPData )
//{
//
//    return 0;
//	
//}
//
//static Winks_Pushdata_s* Winks_PushFindSeg( Winks_PushSMSHead_s* pSMSHead, Winks_PushUDH_s* pUDH )
//{
//
//    return NULL;
//}
//
//static int Winks_PushAppendData( Winks_Pushdata_s* pPData, Winks_PushSMSHead_s* pSMSHead, Winks_PushUDH_s* pUDH )
//{
//    return 1;
//}
//
//static int Winks_PushAddDataToTmp( Winks_Pushdata_s* pPData )
//{
//
//    return 0;
//	
//}
//
//static int Winks_PushDumpData( Winks_Pushdata_s* pPData )
//{
//    return -1;
//	
//}
///*************************************************************************************\
// 函数原型：  int Winks_pushparse( char* pdata, int size )
// 
// 函数介绍：
// Push数据解析函数。
// 参数说明：
// 参数名称	参数类型		参数介绍		备注
// pdata		char*			PUSH数据
// size		int				PUSH数据长度
// 
// 函数返回值：
// 成功返回0，失败返回-1。
// 实现描述：
// 本函数用来解析PUSH数据，抽象层初始化的时候需要向消息通道注册PUSH接收消息，
// 一旦有PUSH数据到来，抽象层应该调用本函数将PUSH数据送到PUSH处理模块进行处理。
// \*************************************************************************************/
//int Winks_PushParse( unsigned char* pdata, int size )
//{
//    return WINKS_SO_SUCCESS;
//}
//
///*************************************************************************************\
// 函数原型：  int Winks_pushcreate( Winks_Push_s* pPush )
// 
// 函数介绍：
// Push创建函数。
// 参数说明：
// 参数名称	参数类型		参数介绍	备注
// pPush		Winks_Push_s	创建参数	
// 
// 函数返回值：
// 成功返回创建句柄，失败返回-1。
// 实现描述：
// 本函数用来创建Push侦听控制块，一旦返回成功，抽象层将在用户指定的端口上侦听Push数据，
// 同时还可对指定APPID进行侦听（第一版暂不支持APPID）如果有符合要求的Push数据到达，抽象
// 层会往用户指定的消息通道发送指定消息，用户可以通过调用函数Winks_pushrecv得到数据。
// 消息数值为一个Winks_Socketmsg_s结构，其中wParam代表Push侦听句柄，lParam代表Push数据的长度。
// \*************************************************************************************/
//int Winks_pushcreate( Winks_Push_s* pPush )
//{
//    return WINKS_SO_FAILURE;
//}
///*************************************************************************************\
// 函数原型：  int Winks_pushrecv(int push, unsigned short* pPort, char* pDest, int* destlen, 
// char* pAppid, int* appidlen, void* pdata, int* datalen )
// 
// 函数介绍：
// Push读取数据函数。
// 参数说明：
// 参数名称	参数类型		参数介绍			备注
// push		int				Push侦听句柄	
// pPort		unsigned short*	Push数据到达端口	为空代表用户不关心
// pDest		char* 			目的地址			为空代表用户不关心
// destlen		int*			目的地址缓冲区长度	
// pAppid		char*			Appid内容			为空代表用户不关心
// appidlen	int*			appid缓冲区长度	
// pdata		char*			数据缓冲区	
// datalen		int*			数据缓冲区长度	
// 函数返回值：
// 成功返回读取数据长度，
// 失败返回
// -1                          general error
// WINKS_SO_PUSHEDEST          Dest parameter error
// WINKS_SO_PUSHEAPPID         Appid parameter error
// 实现描述：
// 本函数用来读取Push数据，抽象层会按照用户要求将数据和相应参数填入用户提供的缓冲区内，
// 如果用户提供的缓冲区长度不够，抽象层会在用户提供的缓冲区长度指针里面填写需要的长度，
// 以便用户申请更多的缓冲区来获取数据。本函数返回的数据为实际的push数据。
// \*************************************************************************************/
//int Winks_pushrecv(int push, char* pDest, int* destlen, 
//				   char* pAppid, int* appidlen, char* pdata, int* datalen )
//{
//    return len;
//}
//
///*************************************************************************************\
// 函数原型：  int Winks_pushclose( int push )
// 函数介绍：
// Push关闭函数。
// 参数说明：
// 参数名称	参数类型	参数介绍	备注
// push		int			Push侦听句柄	
// 函数返回值：
// 成功返回0，失败返回-1。
// 实现描述：
// 本函数关闭用户指定的Push侦听句柄。
// \*************************************************************************************/
//int Winks_pushclose( int push )
//{
//    return 0;
//}
//int Winks_setdialid( unsigned long id )
//{
//    return 0;
//}
