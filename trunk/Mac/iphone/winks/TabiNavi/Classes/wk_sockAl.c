//
//  wk_sockAl.c
//  TabiNavi
//
//  Created by wang luke on 7/8/09.
//  Copyright 2009 luke. All rights reserved.
//
#include "wk_sockAl.h"

static Winks_SocketALGB_s Winks_SocketALGB;
static WK_FD maxfd;

//声明外部数据
//extern Winks_sposal_context  g_sposal_context;
//extern MMICMSBRW_SETTING_T g_cmsbrw_setting_info;
/********************************************************************************\
 对外提供的函数接口
 \********************************************************************************/
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
#if TARGET_IPHONE_SIMULATOR
	char* pdsvr = "192.168.10.2";
#endif
	
    if( Winks_SocketALGB.ifInit )
    {
        Winks_printf( "WINKS socket startup have initialed\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( (Winks_SocketALGB.Socket_Mutex = Winks_CreateMutex( WINKS_SO_SOMUTEXNAME )) == NULL )
    {
        Winks_printf( "WINKS socket startup create socket mutex failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( (Winks_SocketALGB.GH_Mutex = Winks_CreateMutex( WINKS_SO_GHMUTEXNAME)) == NULL )
    {
        Winks_printf( "WINKS socket startup create GH mutex failure\r\n" );
        return WINKS_SO_FAILURE;
    }
    if( (Winks_SocketALGB.GH_SyncMutex = Winks_CreateMutex( WINKS_SO_GHSYNCMUTEXNAME)) == NULL )
    {
        Winks_printf( "WINKS socket startup create GH sync mutex failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    if( (Winks_SocketALGB.Global_Event = Winks_CreateEvent( WINKS_SO_EVENTNAME)) == NULL )
    {
        Winks_printf( "WINKS socket startup create event failure, \r\n");
        return WINKS_SO_FAILURE;
    }
	
//	if ((Winks_SocketALGB.gprs_event = Winks_CreateEvent( WINKS_SO_GPRSEVENT)) == NULL )
//	{
//		Winks_printf( "WINKS socket startup create gprs event failure, \r\n");
//        return WINKS_SO_FAILURE;
//	}
	
    
    for( i = 0; i < WINKS_SO_MAXSONUM; i++ )
    {
        Winks_SocketALGB.sockcb[i].s = -1;
    }
	
    for( i = 0; i < WINKS_SO_MAXGHNUM; i++ )
    {
        Winks_SocketALGB.GHcb[i].ThreadID = NULL;
    }
	
    if( (Winks_SocketALGB.Global_Thread = Winks_CreateThread( WINKS_SO_THREADNAME, winks_SocketThread, 
															 NULL )) == NULL )
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
	
    if( Winks_SocketALGB.Global_Event != NULL )
        Winks_DeleteEvent( Winks_SocketALGB.Global_Event );
	
    if( Winks_SocketALGB.GH_Mutex != NULL )
        Winks_DeleteMutex( Winks_SocketALGB.GH_Mutex );
	
    if( Winks_SocketALGB.Socket_Mutex != NULL )
        Winks_DeleteMutex( Winks_SocketALGB.Socket_Mutex );
	
    for( i = 0; i < WINKS_SO_MAXSONUM; i++ )
    {
        if( Winks_SocketALGB.sockcb[i].s != -1 )
			osal_sock_close(Winks_SocketALGB.sockcb[i].s);
    }
	
    for( i = 0; i < WINKS_SO_MAXGHNUM; i++ )
    {
        if( Winks_SocketALGB.GHcb[i].ThreadID != NULL )
            Winks_DeleteThread( Winks_SocketALGB.GHcb[i].ThreadID );
    }
	
    memset( &Winks_SocketALGB, 0, sizeof(Winks_SocketALGB) );

    return WINKS_SO_SUCCESS;
}

int Winks_getlasterror()
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
	
    Winks_GetMutex( Winks_SocketALGB.Socket_Mutex, -1 );
    while( i++ < WINKS_SO_MAXSONUM - 1 )
    {
        /* We keep handle 0 as a debug handle, will not alloc 0 to user */
        if( (++Winks_SocketALGB.sockhd) >= WINKS_SO_MAXSONUM )
            Winks_SocketALGB.sockhd = 1;
        if( Winks_SocketALGB.sockcb[Winks_SocketALGB.sockhd].s == -1 )
        {
            /* Get a vaild socket CB */
            if( (Winks_SocketALGB.sockcb[Winks_SocketALGB.sockhd].s = osal_socket( family, type, protocol )) 
			   == WINKS_SO_FAILURE )
            {
                Winks_SocketALGB.sockhd --;
                Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
                Winks_printf( "WINKS socket create socket failure\r\n" );
                return WINKS_SO_FAILURE;
            }
            Winks_printf( "WINKS socket create socket hd %d, socket %d\r\n", Winks_SocketALGB.sockhd, 
						 Winks_SocketALGB.sockcb[Winks_SocketALGB.sockhd].s );
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            return Winks_SocketALGB.sockhd;
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
    int ret = 0;
	
    if( (pSock = winks_lockhandle(sockhd)) == NULL )
    {
        Winks_printf( "WINKS socket asyncselect gethandle failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    pSock->Opcode = opcode;
    pSock->MsgNum = msg;
    pSock->channel = channel;
	ret = osal_sock_setnonblock(pSock->s);
    //Winks_printf( "WINKS socket hd %d, num %d asyncselect fcntl return %d\r\n", sockhd, pSock->s, ret );
	
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
    
    int ret = 0;
	
    Winks_printf("winks connect sockhd:%d,addrlen:%d\r\n",sockhd,addrlen);

	memset(&Winks_SocketALGB.sock_connect_info[sockhd],0,sizeof(Winks_SocketALGB.sock_connect_info[sockhd]));
	Winks_Socket_s* pSock = NULL;
	int ret;
	if( (pSock = winks_lockhandle(sockhd)) == NULL )
	{
		Winks_printf( "WINKS socket connect gethandle failure\r\n" );
		return WINKS_SO_FAILURE;
	}
	ret = osal_sock_connect( pSock->s, serv_addr, addrlen );
	if( ret < 0 && Winks_getlasterror()!= WINKS_SO_EWOULDBLOCK) 
	{
		Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
		Winks_printf( "WINKS socket connect call system error %d\r\n", Winks_get_platform_error() );
		return ret;
	}
	
	if( ret == 0 && pSock->MsgNum )
	{
		ret = -1;
		winks_set_lasterror(WINKS_SO_EWOULDBLOCK);
	}
	
	winks_set_lasterror(WINKS_SO_EWOULDBLOCK);
	pSock->Opcode |= WINKS_SO_ONCONNECT;
	
	Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
	
	if( winks_ifthreadwait() )
		Winks_SetEvent( Winks_SocketALGB.Global_Event );
	
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
	
    ret = osal_sock_send(pSock->s, buf, len);
    Winks_printf("##### osal_sock %d send bytes:%d\n",sockhd,ret);
	if(ret < 0)
	{
		Winks_printf("winks osal_sock send err:%d\n",Winks_get_platform_error());
	}
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
        if( Winks_getlasterror() != WINKS_SO_EWOULDBLOCK )
        {
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            Winks_printf( "WINKS socket send call system error %d\r\n", Winks_get_platform_error() );
        }
        else
        {
            pSock->Opcode |= WINKS_SO_ONWRITE;
            winks_set_lasterror(WINKS_SO_EWOULDBLOCK);
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            if( winks_ifthreadwait() )
                Winks_SetEvent( Winks_SocketALGB.Global_Event );
        }
		
    }
    else //在这里发送可读消息
	{
		//		msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
		//		msg.lParam = WINKS_SO_READ;
		//      Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
		Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
	}
	
	
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
    ret = osal_sock_recv( pSock->s, buf, len );
    //Winks_printf( "WINKS socket %d recv after recv\r\n", pSock->s );
    Winks_printf( "######  recv  bytes:%d \r\n", ret);
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
        if( Winks_getlasterror() != WINKS_SO_EWOULDBLOCK )
        {
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            Winks_printf( "WINKS socket recv call system error %d\r\n", Winks_get_platform_error() );
        }
        else
        {
            pSock->Opcode |= WINKS_SO_ONREAD;
            winks_set_lasterror(WINKS_SO_EWOULDBLOCK);
            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
            if( winks_ifthreadwait() )
                Winks_SetEvent( Winks_SocketALGB.Global_Event );
        }
		
    }
    else
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
	
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
	//取消正在等待
    Winks_SocketALGB.sock_connect_info[sockhd].is_waiting = 0;
    ret = osal_sock_close( pSock->s );
	
    if( ret < 0 )
    {
        Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
        Winks_printf( "WINKS socket closesocket system failure\r\n" );
        return ret;
    }
    else
    {
        memset( pSock, 0, sizeof(Winks_Socket_s) );
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
    Winks_hostent* phost = (Winks_hostent* )pHost;
    int len = 0;
    Winks_GH_s* pGH = NULL;
    static int Tname = 0;
#define WINKS_SO_TNAMELEN   16
    char pTname[WINKS_SO_TNAMELEN];
	
    if( name == NULL || pHost == NULL || hostlen < (sizeof(Winks_hostent) + 4 + 4 + 4) )
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
    memset( pGH, 0, (sizeof(int) + len + 1) );
    pGH->GHID = pHostcb - Winks_SocketALGB.GHcb;
    memcpy( pGH->name, name, len );
	
    if( Tname ++ >= 9999 )
        Tname = 0;
    sprintf( pTname, "%s%d", WINKS_SO_GHTHREADNAME, Tname );
	
    if( (pHostcb->ThreadID = Winks_CreateThread( (void* )pTname, winks_GHThread, 
												pGH )) == NULL )
    {
        memset( pHostcb, 0, sizeof(Winks_GetHost_s) );
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
	
    if( Winks_DeleteThread( pHostcb->ThreadID ) != WINKS_SO_SUCCESS )
    {
        Winks_printf( "WINKS socket gethostbyname cancel delete thread failure\r\n" );
        return WINKS_SO_FAILURE;
    }
	
    memset( pHostcb, 0, sizeof(Winks_GetHost_s) );
    pHostcb->ThreadID = (void* )NULL;
	
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
	
    return WINKS_SO_SUCCESS;
}


/********************************************************************************\
 不依赖具体平台的内部函数接口
 \********************************************************************************/
// 得到sockhd对应的Winks_Socket_s
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
	
    Winks_GetMutex( Winks_SocketALGB.Socket_Mutex, -1 );
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

static int winks_getsockstatus( Winks_EventSock_s* pSevent )
{
    int ret = -1, i = 0;
	
    memset( pSevent, 0, sizeof(Winks_EventSock_s) );
	
    Winks_GetMutex( Winks_SocketALGB.Socket_Mutex, -1 );
    for( i = 0; i < WINKS_SO_MAXSONUM; i ++ )
    {
        if( (Winks_SocketALGB.sockcb[i].s != -1) && (Winks_SocketALGB.sockcb[i].Opcode & WINKS_SO_ONMASK) )
        {
            if( ret < (int)Winks_SocketALGB.sockcb[i].s )
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
static void winks_SocketThread( void* param )
{
    int nfds = 0, ret = 0, i = 0, lret = 0;
    Winks_EventSock_s SEvent;
    Winks_Socketmsg_s msg;
    
    Winks_Socket_s* pSock = NULL;
    WK_FD_SET rdfds;
    WK_FD_SET wrfds;
    WK_FD_SET expfds;
    long times;
	
    
    times = WINKS_SO_TIME;   //200 microsecond
	
    /* firstly we wait in global event for any event */
    Winks_SocketALGB.ifWait = 1;
    Winks_printf( "WINKS socket thread initial wait\r\n" );
    Winks_GetEvent( Winks_SocketALGB.Global_Event, -1 );
    Winks_printf( "WINKS socket thread initial wakeup\r\n" );
    Winks_SocketALGB.ifWait = 0;
    
    /* when get event, set flags first */
	
    while( 1 )
    {
        if( (nfds = winks_getsockstatus( &SEvent )) > 0 )
        {
            //Winks_printf( "WINKS socket thread get live socket, num = %d\r\n", SEvent.num );
            WK_FD_ZERO( &rdfds );
            WK_FD_ZERO( &wrfds );
            WK_FD_ZERO( &expfds );
            for( i = 0; i < SEvent.num; i++ )
            {
                if( (SEvent.s[i].Opcode & WINKS_SO_ONCONNECT) )
                {
                    //Winks_printf( "WINKS socket thread get onconnect socket %d\r\n", SEvent.s[i].sockhd );
                    WK_FD_SET_ADD( SEvent.s[i].sockhd, &wrfds );
                    WK_FD_SET_ADD( SEvent.s[i].sockhd, &expfds );
                }
                if( (SEvent.s[i].Opcode & WINKS_SO_ONWRITE) )
                {
                    //Winks_printf( "WINKS socket thread get onwrite socket %d\r\n", SEvent.s[i].sockhd );
                    WK_FD_SET_ADD( SEvent.s[i].sockhd, &wrfds );
                }
                if( (SEvent.s[i].Opcode & WINKS_SO_ONREAD) )
                {
                    //Winks_printf( "WINKS socket thread get onread socket %d\r\n", SEvent.s[i].sockhd );
                    WK_FD_SET_ADD( SEvent.s[i].sockhd, &rdfds );
                }
            }
            //ret = osal_sock_select(&rdfds, &wrfds, &expfds, 0 );
			ret = osal_sock_select(&rdfds, &wrfds, &expfds, 200);
			
            if( ret > 0 )
            {
                /* some event occured */
                for( i = 0; i < SEvent.num; i++ )
                {
                    if( WK_FD_ISSET(SEvent.s[i].sockhd, &wrfds) )
                    {
                        if( (pSock = winks_lockhandle( SEvent.s[i].index )) != NULL )
                        {
                            if( SEvent.s[i].Opcode & WINKS_SO_ONCONNECT )
                            {
								
                                Winks_printf( "WINKS socket thread poll onconnect event %d, %d\r\n", SEvent.s[i].index,SEvent.s[i].sockhd );
								
                                pSock->Opcode &= ~WINKS_SO_ONCONNECT;
                                msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                                msg.lParam = WINKS_SO_CONNECT;
                                lret = Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                                Winks_printf( "WINKS socket thread post connect msg ret %d\r\n", lret );
                                //why need this statment 
								pSock->Opcode |= WINKS_SO_ONREAD | WINKS_SO_ONWRITE;
                            }
                            else if( SEvent.s[i].Opcode & WINKS_SO_ONWRITE )
                            {
                                Winks_printf( "WINKS socket thread poll onwrite event %d, %d\r\n", SEvent.s[i].index,SEvent.s[i].sockhd );
                                pSock->Opcode &= ~WINKS_SO_ONWRITE;
                                msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                                msg.lParam = WINKS_SO_WRITE;
                                lret = Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                                Winks_printf( "WINKS socket thread post write msg ret %d\r\n", lret );
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
                    if( WK_FD_ISSET(SEvent.s[i].sockhd, &rdfds) )
                    {
                        if( (pSock = winks_lockhandle( SEvent.s[i].index )) != NULL )
                        {
                            Winks_printf( "WINKS socket thread poll onreceive event %d, %d\r\n", SEvent.s[i].index,SEvent.s[i].sockhd );
                            pSock->Opcode &= ~WINKS_SO_ONREAD;
                            msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                            msg.lParam = WINKS_SO_READ;
                            lret = Winks_PostMsg( pSock->channel, (unsigned long )(pSock->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
                            Winks_printf( "WINKS socket thread post read msg ret %d\r\n", lret );
                            Winks_PutMutex( Winks_SocketALGB.Socket_Mutex );
                        }
                        else
                        {
                            Winks_printf( "WINKS socket thread poll read event %d, but socket invalid\r\n", SEvent.s[i].sockhd );
                        }
                    }
#if 0
                    if( WK_FD_ISSET(SEvent.s[i].sockhd, &expfds) )
                    {
                        if( (pSock = winks_lockhandle( SEvent.s[i].index )) != NULL )
                        {
                            Winks_printf( "WINKS socket thread poll onconnect failure event %d\r\n", SEvent.s[i].sockhd );
                            pSock->Opcode &= ~WINKS_SO_ONCONNECT;
                            msg.wParam = (unsigned long )(pSock - Winks_SocketALGB.sockcb);
                            msg.lParam = WINKS_SO_CONNECT | (tp_os_net_get_errno() << 16);
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
            else if(ret<0) 
            {
                Winks_printf( "WINKS socket thread select return error %d\r\n", Winks_get_platform_error() );
            }
			//adde sleep thread
			osal_thread_sleep(200);
        }
        else
        {
            Winks_SocketALGB.ifWait = 1;
            Winks_printf( "WINKS socket thread wait\r\n" );
            Winks_GetEvent( Winks_SocketALGB.Global_Event, -1 );
            Winks_printf( "WINKS socket thread wakeup\r\n" );
            Winks_SocketALGB.ifWait = 0;
        }
    }
	
}



static unsigned long Winks_GHGetfromCache( char* name )
{
    int i = 0;
	
    Winks_GetMutex( Winks_SocketALGB.GH_Mutex, -1 );
    for( i = 0; i < WINKS_SO_MAXGHCACHENUM; i++ )
    {
        if( Winks_SocketALGB.GHCache[i].name != NULL )
        {
            if( osal_get_tick() > Winks_SocketALGB.GHCache[i].live )
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
	
    Winks_GetMutex( Winks_SocketALGB.GH_Mutex, -1 );
    for( i = 0; i < WINKS_SO_MAXGHCACHENUM; i++ )
    {
        if( (osal_get_tick() > Winks_SocketALGB.GHCache[i].live) || (Winks_SocketALGB.GHCache[i].name == NULL) )
        {
            if( (Winks_SocketALGB.GHCache[i].name != NULL) &&
			   (osal_get_tick() > Winks_SocketALGB.GHCache[i].live) )
            {
                /* timeout, drop */
                if( Winks_SocketALGB.GHCache[i].name != Winks_SocketALGB.GHCache[i].pdata )
                    Winks_salFree(Winks_SocketALGB.GHCache[i].name);
                Winks_SocketALGB.GHCache[i].name = NULL;
            }
            Winks_SocketALGB.GHCache[i].live = osal_get_tick() + 
			(WINKS_SO_NAMELIVETIME / WINKS_SO_SYSSECPTICK);
            if( (len = strlen(name)) > WINKS_SO_MAXNAMELEN )
                Winks_SocketALGB.GHCache[i].name = Winks_salAlloc( len + 1 );
            else
                Winks_SocketALGB.GHCache[i].name = Winks_SocketALGB.GHCache[i].pdata;
            memcpy( Winks_SocketALGB.GHCache[i].name, name, len );
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
	
    Winks_GetMutex( Winks_SocketALGB.GH_Mutex, -1 );
	
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
        if( Winks_SocketALGB.GHcb[GHid].ThreadID == (void* )NULL )
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

static void winks_GHThread( void* param )
{
    Winks_GH_s* pGH = param;
    unsigned long addr;	
    Winks_hostent* phost = NULL;
    Winks_GetHost_s* pHostcb = NULL;
    Winks_Socketmsg_s msg;
	
    if( pGH == NULL )
    {
        Winks_printf( "WINKS socket get GH thread param error\r\n" );
        return;
    }
	
    /* We user a mutex to sync gh thread */
    Winks_GetMutex( Winks_SocketALGB.GH_SyncMutex, -1 );
    Winks_printf( "WINKS socket GH thread call %s gethostbyname\r\n", pGH->name );
    addr = osal_gethostbyname( (char* )pGH->name );
    Winks_printf( "WINKS socket GH thread call %s gethostbyname return %d\r\n", pGH->name, addr );
	
    if( (pHostcb = winks_lockGHhandle( pGH->GHID )) == NULL )
    {
        Winks_printf( "WINKS socket GH thread lock handle error\r\n" );
        Winks_PutMutex( Winks_SocketALGB.GH_SyncMutex );
        return;
    }
	
    if( addr == 0 )
    {
        msg.wParam = (unsigned long )pGH->GHID;
        msg.lParam = (unsigned long )WINKS_SO_FAILURE;
        Winks_PostMsg( pHostcb->channel, (unsigned long )(pHostcb->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
        memset( pHostcb, 0, sizeof(Winks_GetHost_s) );
        pHostcb->ThreadID = (void* )NULL;
		
        Winks_printf( "WINKS socket GH thread gethostbyname return failure\r\n" );
        Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
        Winks_PutMutex( Winks_SocketALGB.GH_SyncMutex );
		
        Winks_salFree(pGH);
        return;
    }
	
    phost = (Winks_hostent* )(pHostcb->pHost);
    phost->h_name = NULL;
    phost->h_aliases = NULL;
    phost->h_addrtype = AF_INET;
    phost->h_length = 4;
    phost->h_addr_list = (char** )(phost + 1);
    phost->h_addr_list[0] = (char* )(phost + 1) + 2 * sizeof(char* );
    phost->h_addr_list[1] = NULL;
    *((unsigned long* )(phost->h_addr_list[0])) = addr;
	
    msg.wParam = (unsigned long )pGH->GHID;
    msg.lParam = WINKS_SO_SUCCESS;
    Winks_PostMsg( pHostcb->channel, (unsigned long )(pHostcb->MsgNum), &msg, sizeof(Winks_Socketmsg_s) );
    memset( pHostcb, 0, sizeof(Winks_GetHost_s) );
    pHostcb->ThreadID = (void* )NULL;
    
    Winks_PutMutex( Winks_SocketALGB.GH_Mutex );
    Winks_PutMutex( Winks_SocketALGB.GH_SyncMutex );
	
    Winks_GHPutinCache( pGH->name, addr );
    Winks_salFree(pGH);
    return;
	
} 

static int Winks_get_platform_error(){
	return Winks_SocketALGB.platform_errcode;
}

static void winks_set_lasterror(int errCode){
	Winks_SocketALGB.winks_errcode = errCode;
	return;
}

/********************************************************************************\
 依赖具体平台的内部函数接口
 \********************************************************************************/
//static void  osal_thread_entry(unsigned int argc,void *param){
//    OsalThreadParam *threadParam = (OsalThreadParam *)param;
//	(*(threadParam->pEntry))(threadParam->param);
//	SCI_Free(threadParam);
//}

static void osal_thread_sleep(unsigned int ms){
	ms = 200;
	sleep(ms);
}

static void* Winks_CreateThread(const char* name, WK_THREAD_ENTRY entry,void* param ){
	pthread_attr_t attr;
	pthread_t posixThreadID;
	
	assert(!pthread_attr_init(&attr));
	// 线程以分离状态启动，在线程退出时收回占用资源
	assert(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
	
	int threadError = pthread_create(&posixThreadID, &attr, &entry, param);
	assert(!pthread_attr_destroy(&attr));
	if (threadError != 0){
		return NULL;
	}
	return (void*)posixThreadID;
}
static int Winks_DeleteThread( void* thread ){
	return pthread_cancel((pthread_t)thread);
}

static void* Winks_CreateEvent( const char* name)
{
//    SCI_EVENT_GROUP_PTR eventHandle = NULL;
//	eventHandle = SCI_CreateEvent(name);
//	if (SCI_NULL == eventHandle)
//	{
//		return NULL;
//	}
//	return (void*)eventHandle;
	
	return (void*)sem_create(0);
}

static int Winks_DeleteEvent( void* event )
{
//    SCI_EVENT_GROUP_PTR eventHandle = (SCI_EVENT_GROUP_PTR)event;
//	if (SCI_SUCCESS == SCI_DeleteEvent(eventHandle))
//	{
//		return WINKS_SO_SUCCESS;
//	}
	
	if((sem_rm((int *)event)) == -1)
		return WINKS_SO_FAILURE;
	else {
		return WINKS_SO_SUCCESS;
	}
}

static int Winks_GetEvent( void* event, int timeout )
{
//	SCI_EVENT_GROUP_PTR eventHandle = (SCI_EVENT_GROUP_PTR)event;
//    uint32 requestFlag = 0x1;
//	uint32 getOption = SCI_AND_CLEAR;
//	uint32 actual_flag;
//	uint32 wait_option;
//	if(0 == timeout)
//		wait_option = SCI_NO_WAIT;
//	else if (-1 == timeout)
//	{
//		wait_option = SCI_WAIT_FOREVER;
//	}else
//		wait_option = timeout;
//	if (SCI_SUCCESS == SCI_GetEvent(eventHandle,requestFlag,getOption,&actual_flag,wait_option))
//	{
//		return WINKS_SO_SUCCESS;
//	}
	if((sem_get((int *)event)) == -1)
		return WINKS_SO_FAILURE;
	else {
		return WINKS_SO_SUCCESS;
	}
}

static int Winks_SetEvent( void* event )
{
//	SCI_EVENT_GROUP_PTR eventHandle = (SCI_EVENT_GROUP_PTR)event;
//    if (SCI_SUCCESS == SCI_SetEvent(eventHandle,0x01,SCI_OR))
//    {
//		return WINKS_SO_SUCCESS;
//    }
	
	if((sem_set((int *)event)) == -1)
		return WINKS_SO_FAILURE;
	else {
		return WINKS_SO_SUCCESS;
	}
}


static void* Winks_CreateMutex( const char* name){
	// name没有用到
	pthread_mutex_t *mutex = NULL;
	pthread_mutex_init(&mutex, NULL);
	return (void*)mutex;
}
static int Winks_DeleteMutex( void* mutex ){
	return pthread_mutex_destroy((pthread_mutex_t*)mutex);
}
static int Winks_GetMutex( void* mutex, int timeout ){
	return pthread_mutex_lock((pthread_mutex_t*)mutex);
}
static int Winks_PutMutex( void* mutex ){
	return pthread_mutex_unlock((pthread_mutex_t*)mutex);
}

static WK_FD osal_socket(int family , int type , int protocol){
	WK_FD sock_handle;
	if (AF_INET != family || SOCK_STREAM != type) {
		return -1;
	}
	sock_handle = socket(AF_INET,SOCK_STREAM,/*IPPROTO_TCP*/0);
	if (sock_handle == -1) {
		//osal_set_last_error(WSAGetLastError());
		perror("socket create");
		return -1;
	}
	return sock_handle;
}
static int osal_sock_close(WK_FD socket){
	if(close(socket) == -1){
		perror("close socket");
		return -1;
	}
	return 0;
}
static int osal_sock_setnonblock(WK_FD socket){
	if(fcntl(socket, F_SETFL, O_NONBLOCK) == -1){
		perror("set nonblocking");
		return -1;
	}
	return 0;
}
static int osal_sock_connect(WK_FD socket, struct winks_sockaddr* serv_addr, int addrlen){
	if(-1 == connect(socket, (struct sockaddr*)serv_addr, addrlen)){
		perror("sock connect");
		return -1;
	}
	return 0;
}
static int osal_sock_send(WK_FD socket, char *buf, int len){
	int byteSent = send(socket, buf, len, 0);
	if(-1 == byteSent){
		perror("send");
		return -1;
	}
	return 0;
}
static int osal_sock_recv(WK_FD socket,char *buf,int len){
	int byteRecved = recv(socket, buf, len, 0);
	if(-1 == byteRecved){
		perror("recv");
		return -1;
	}
	return 0;
}

static int osal_sock_select(WK_FD_SET *readfds, WK_FD_SET *writefds, WK_FD_SET *excptfds,long timeout){
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout * 1000;
	int readyFDnum = select(maxfd + 1, readfds, writefds, excptfds, &tv);
	if(-1 == readyFDnum){
		perror("select");
		return -1;
	}
	return readyFDnum;
}

static void WK_FD_ZERO(WK_FD_SET *set){
	FD_ZERO(set);
}

static void WK_FD_SET_ADD(WK_FD fd, WK_FD_SET *set){
	FD_SET(fd,set);
}

static int WK_FD_ISSET(WK_FD fd,WK_FD_SET *set){
	FD_ISSET(fd, set);
}

static void osal_set_last_error(int platform_errcode){
	Winks_SocketALGB.platform_errcode = platform_errcode;
	if (EWOULDBLOCK == platform_errcode || EINPROGRESS == platform_errcode){
		Winks_SocketALGB.winks_errcode = WINKS_SO_EWOULDBLOCK;
	}else{
		Winks_SocketALGB.winks_errcode = WINKS_SO_FAILURE;
	}
}  

static unsigned long osal_gethostbyname(char *name){
	struct hostent *pHostent = gethostbyname(name);
	if (NULL == pHostent) {
		perror("gethostbyname");
		return 0;
	}
	return *((unsigned long*)pHostent->h_addr_list[0]);
}

static unsigned int osal_get_tick(){
	return SCI_GetTickCount();
}

static long Winks_SocErrConvert(long error)
{
	long result = 0;
	
    //result = error;
	
	switch (error)
	{
//		case SOC_SUCCESS:
//			break;
//		case SOC_ERROR:
//			result = WINKS_SO_FAILURE;
//			break;
		case EWOULDBLOCK:
			result = WINKS_SO_EWOULDBLOCK;
			break;
		case EMFILE:
			result = WINKS_SO_ELOWSYSRESOURCE;
			break;
		case ENOTSOCK:
			result = WINKS_SO_EINVALID_SOCKET;
			break;
//		case EINVAL:
//			result = WINKS_SO_EINVALIDPARA;
//			break;
		case EINPROGRESS:
			result = WINKS_SO_EINPROGRESS;
			break;
		case EOPNOTSUPP:
			result = WINKS_SO_EOPNOTSUPP;
			break;
		case ECONNABORTED:
			result = WINKS_SO_ECONNABORTED;
			break;
		case EINVAL:
			result = WINKS_SO_EINVAL;
			break;
		case EPIPE:
			break;
		case ENOTCONN:
			result = WINKS_SO_ENOTCONN;
			break;
		case EMSGSIZE:
			result = WINKS_SO_EMSGSIZE;
			break;
		case ENETDOWN:
			result = WINKS_SO_ENETDOWN;
			break;
		case ECONNRESET:
			result = WINKS_SO_ERESET;
			break;
		default:
			result = WINKS_SO_EUNKNOWERROR;
			break;
	}
	
	return result;
}
