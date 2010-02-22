//
//  wk_socketEngin.m
//  ColorSet
//
//  Created by wwg on 09-8-5.
//  Copyright 2009 www. All rights reserved.
//
#import "wk_engin.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/ipc.h>;
#include "MyLog.h";
#include <dlfcn.h>
#import  <arpa/inet.h>

//#include <OpenTransport.h>
#include                 <sys/stat.h>   
#include                 <string.h>   
#include                 <fcntl.h>   
#include                 <errno.h>   

#define   FIFO         "/tmp/fifo.temp1"   
#define   MAXLINE   1024  


/*是否使用socket写缓冲区*/
//#define WINKS_SOCKET_WRITE_CATCH
/*是否使用读socket缓冲*/
#define WINKS_SOCKET_READ_CATCH
/*缓冲区步长*/
#define WINKS_SOCKET_SPACE 50

int client_sockfd = -1;//socket句柄

//存放socket缓冲数据
typedef struct tag_Winks_Catch_s
{
	int     datalen;/*数据长度*/
	int     catchlen;/*缓冲长度*/
	void*	readptr;//读缓冲时指向当前的数据位置
	void*   msg;/*数据 */
}Winks_Catch_s;

#ifdef WINKS_SOCKET_WRITE_CATCH
static Winks_Catch_s socket_write_catch;

//static Winks_Catch_s socket_read_catch;
#endif

#ifdef WINKS_SOCKET_READ_CATCH
static Winks_Catch_s socket_read_catch;
#endif

//flush缓冲区，实际从socket读数据
ssize_t Winks_Read_flush() {
#ifdef WINKS_SOCKET_READ_CATCH
	
	Winks_printf("into Winks_Read_flush");
	//初始化socket数据缓冲区
	if (socket_read_catch.msg) {
		free(socket_read_catch.msg);
		socket_read_catch.msg = NULL;
	}
	memset(&socket_read_catch,0,sizeof(Winks_Catch_s));
//Winks_printf("Winks_Read_flush 1");
	
	//如果返回的错误
	int funid = 0;//函数号
	int reslen = 0;//返回值长度
	Winks_printf("Winks_Read_flush:recive data start@@@");
	int tmp = recv(client_sockfd,&funid,(size_t)sizeof(int),0);
	Winks_printf("Winks_Read_flush:recive data end@@@");
	if (tmp == 0) {
		Winks_printfErr("out Winks_Read_flush:error=%s",strerror(errno));		
		return WINKS_RETURN_FAILURE;
	}
	
//Winks_printf("Winks_Read_flush 2");
	if (funid >= 0) {
		
		tmp = recv(client_sockfd,&reslen,(size_t)sizeof(int),0);
				if ((tmp == 0)||(reslen<0||reslen>3000)) {
			Winks_printfErr("out Winks_Read_flush:1 error=%s",strerror(errno));
			return WINKS_RETURN_FAILURE;
		}
		//Winks_printf("Winks_Read_flush 3");
		socket_read_catch.msg = malloc(reslen+sizeof(int) * 2);
		if(socket_read_catch.msg == NULL)
		{
			Winks_printfErr("Winks_Read_flush:malloc failt");
			return WINKS_RETURN_FAILURE;
		}

		socket_read_catch.datalen = sizeof(int) * 2 + reslen;
		socket_read_catch.catchlen = sizeof(int) * 2 + reslen;
		socket_read_catch.readptr = socket_read_catch.msg;
		
		memcpy(socket_read_catch.msg,&funid,sizeof(int));
		memcpy(socket_read_catch.msg+sizeof(int),&reslen,sizeof(int));
		
		//Winks_printf("out Winks_Read_flush");
		if (reslen > 0) {
			Winks_printf("Winks_Read_flush:recive data start");
			int len = recv(client_sockfd,socket_read_catch.msg+sizeof(int) * 2,(size_t)reslen,0);
			Winks_printf("Winks_Read_flush:recive data end");
			if(len==0)
			{
				Winks_printf("Winks_Read_flush:recive data failt error=%s",strerror(errno));
				return WINKS_RETURN_FAILURE;
			}
			Winks_HEX_DataOut(socket_read_catch.msg, socket_read_catch.datalen, "[READ FLUSH DATA]");
			socket_read_catch.datalen; 
		}
			
	} 
	else 
	{
		Winks_printf("Winks_Read_flush 4");
		reslen = sizeof(int);
		socket_read_catch.msg = malloc(reslen);
		memcpy(socket_read_catch.msg,&funid,reslen);

		socket_read_catch.datalen = reslen;
		socket_read_catch.catchlen = reslen;
		socket_read_catch.readptr = socket_read_catch.msg;
	}

	//Winks_printf("out Winks_Read_flush");
	//return WINKS_RETURN_SUCCESS;	
#endif
	return WINKS_RETURN_SUCCESS;
}

//关闭读socket的缓冲区
void Winks_Close_Read_flush() {
#ifdef WINKS_SOCKET_READ_CATCH
	if (socket_read_catch.msg) {
		free(socket_read_catch.msg);
		memset(&socket_read_catch,0,sizeof(Winks_Catch_s));
	}
#endif
}


//flush缓冲区，实际往socket写数据
ssize_t Winks_Write_flush() {
#ifdef WINKS_SOCKET_WRITE_CATCH
	//Winks_printf("into Winks_Write_flush");
	if (socket_write_catch.msg) {
//		Winks_printf("Winks_Write_flush:client_sockfd = %d",client_sockfd);
//		Winks_printf("Winks_Write_flush:socket_write_catch.datalen = %d",socket_write_catch.datalen);
//		Winks_printf("Winks_Write_flush:socket_write_catch.msg = %x",socket_write_catch.msg);
		int res = write(client_sockfd,socket_write_catch.msg,socket_write_catch.datalen);
		if (res <= 0) {
			Winks_printfErr("Winks_Write_flush:error=%s",strerror(errno));
		}
		Winks_HEX_DataOut(socket_write_catch.msg, socket_write_catch.datalen, "[WRITE FLUSH DATA]");
		
		//释放原有的缓冲，并初始化新的socket数据缓冲区
		free(socket_write_catch.msg);
		socket_write_catch.msg= NULL;
		
		//初始化为零
		socket_write_catch.datalen = 0;
		socket_write_catch.catchlen = 0;
		
		//Winks_printf("out Winks_Write_flush the res = %d",res);
		return res;
	} else {
		Winks_printfErr("Winks_Write_flush:error the msg is null");
		return WINKS_RETURN_FAILURE;
	}
#else
	return 1;
#endif
}

/*向客户端socket写数据，实际写到缓冲区*/
ssize_t Winks_Write(const void *buf, size_t nbyte) {
#ifdef WINKS_SOCKET_WRITE_CATCH
	//如果现有的缓冲区不够，创建新的
	if (socket_write_catch.datalen+nbyte > socket_write_catch.catchlen) {
		//Winks_printf("Winks_Write: the catch is not enough");
		int newlen = 0;
		if (nbyte <= WINKS_SOCKET_SPACE) {
			newlen = socket_write_catch.catchlen + WINKS_SOCKET_SPACE;
		} else {
			newlen = socket_write_catch.catchlen + nbyte;
		}
		void* tmp = malloc(newlen);
		memset(tmp,0,newlen);		
		
		if (socket_write_catch.msg) 
		{
			memcpy(tmp,socket_write_catch.msg,socket_write_catch.datalen);
			free(socket_write_catch.msg);
		}
		socket_write_catch.msg = tmp;
		socket_write_catch.catchlen = newlen;
	}
	//写数据到缓冲区
	void* ptr = socket_write_catch.msg;
	ptr += socket_write_catch.datalen;
	memcpy(ptr,buf,nbyte);
	socket_write_catch.datalen += nbyte;

	return nbyte;
#else
	int res = write(client_sockfd,buf,nbyte);
	if (res <= 0) {
		Winks_printfErr("Winks_Write:error=%s  the res=%d",strerror(errno),res);
	}
	return res;
#endif
}
//从客户端socket读数据
ssize_t Winks_Read(void *buf, size_t nbyte) {
#ifdef WINKS_SOCKET_READ_CATCH
	//如果已经到数据末尾
	if (socket_read_catch.readptr == (socket_read_catch.msg + socket_read_catch.datalen)) {
		return 0;
	} else if ((socket_read_catch.readptr + nbyte) > (socket_read_catch.msg + socket_read_catch.datalen)) {
		//如果取的数据已经不够
		int reallylen = socket_read_catch.msg + socket_read_catch.datalen - socket_read_catch.readptr;
		memcpy(buf,socket_read_catch.readptr,reallylen);
		socket_read_catch.readptr += reallylen;
		return reallylen;
	} else {
		//正常情况
		memcpy(buf,socket_read_catch.readptr,nbyte);
		socket_read_catch.readptr += nbyte;
		return nbyte;
	}
#else
	int res = read(client_sockfd,buf,nbyte);
	if (res <= 0) {
		Winks_printfErr("Winks_Read:error=%s the res=%d",strerror(errno),res);
	}
	return res;
#endif
}

off_t Winks_Lseek(off_t offset, int whence) {
#ifdef WINKS_SOCKET_READ_CATCH
	//如果已经到数据末尾
	if (socket_read_catch.readptr == (socket_read_catch.msg + socket_read_catch.datalen)) {
		return 0;
	} else if ((socket_read_catch.readptr + offset) > (socket_read_catch.msg + socket_read_catch.datalen)) {
		//如果取的数据已经不够
		int reallylen = socket_read_catch.msg + socket_read_catch.datalen - socket_read_catch.readptr;
		socket_read_catch.readptr += reallylen;
		return reallylen;
	} else {
		//正常情况
		socket_read_catch.readptr += offset;
		return offset;
	}
#else 
	int res = -1;
	if (whence = SEEK_CUR) {
		char *tmp = malloc(offset);
		res =  read(client_sockfd,tmp,offset);
		//Winks_HEX_DataOut(tmp,offset,"Winks_Lseek:");
		free(tmp);
	}
	
	//int res =  lseek(client_sockfd,offset,whence);
	if (res <= 0) {
		Winks_printfErr("Winks_Lseek:error=%s the res=%d",strerror(errno),res);
	}
	return res;	
#endif
}

static int   testClientFIFO(void)   
{   
	
	int           fifo;   
	char         buf[MAXLINE];   
	int           len;   
	int           i   =   0;   
    
	strcpy(buf,   "10");   
	if   ((fifo   =   open(FIFO,   O_RDWR))   <   0)                   //读写打开有名管道   
	{   
		Winks_printfErr("mkfifo   error:   %s\n",   strerror(errno));   
		return(0);   
	}   
	while   (i   <   5)   
	{   
		sprintf(buf,   "%d",   i   +   1);   
		len   =   write(fifo,   buf,   strlen(buf));       //写入信息到管道中   
		Winks_printf("send   len   =   %d\n",   len);   
		sleep(i);   
		i++;   
	}   
    
	return(0);   
}   

//初始化客户端socket
int Winks_InitSocket() {
	//testClientFIFO();
	
	/*
	Winks_printf("start load the dylib");
	void* lib_handle = dlopen("/var/mobile/MyTest.dylib", RTLD_LOCAL);
	Winks_printf("load the dylib=%s",lib_handle);
 	const char* open_err = dlerror();
	Winks_printf("open the dylib dlerror=%s",open_err);
	
	if(lib_handle != NULL) 
	{
	    typedef void (*myfunc_t)();
	    myfunc_t myfunc = dlsym(lib_handle, "Winks_InitSocket"); 
		Winks_printf("get the func=%s",lib_handle);
	    if(myfunc != NULL)
	    {
	        (*myfunc)();
	    }
	}
	 */

	//Winks_printf("start Winks_InitSocket\n");
	//Winks_printf("into Winks_InitSocket sfd=%d",client_sockfd);
#ifdef WINKS_LONG_SOCKET	
	//关闭socket
	Winks_CloseSocket();
#endif	
	
#ifdef WINKS_SOCKET_WRITE_CATCH	
	//初始化socket数据缓冲区
	memset(&socket_write_catch,0,sizeof(Winks_Catch_s));
	//Winks_printf("start Winks_InitSocket memset\n");
	//socket_write_catch.datalen = 0;
	//socket_write_catch.catchlen = 0;	
#endif
	
	//int client_sockfd;
	int len;
	//struct sockaddr_un server_sockaddr,cli_sockaddr;
	struct sockaddr_in server_sockaddr,cli_sockaddr;

	int result;
#if 0
	//int ret = system("rm -f /var/mobile/Winks_Dylib");
	int ret = unlink("/var/mobile/Winks_Dylib");
	if (ret < 0) {
		Winks_printfErr("error unlick /var/mobile/Winks_Dylib!ret =%d erro=%s\n",ret,strerror(errno));
	}
	
	//创建socket
	client_sockfd= socket(AF_UNIX,SOCK_STREAM,0);
	
	//绑定地址
	//memset( &cli_sockaddr, 0, sizeof( cli_sockaddr) ) ;
	cli_sockaddr.sun_family = AF_UNIX ;
	strcpy( cli_sockaddr.sun_path, WINKS_SOCKET_CLIENT_ADDRESS ) ;
	int rcd = bind( client_sockfd, ( struct sockaddr * )&cli_sockaddr, sizeof( cli_sockaddr ) ) ;
	if (rcd < 0) {
		Winks_printfErr("error on bind:%s\n",strerror(errno));
		return WINKS_RETURN_FAILURE;
	}
	
	//连接server
	server_sockaddr.sun_family=AF_UNIX;
	//	server_sockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//	server_sockaddr.sin_port=htons(9734);
	strcpy( server_sockaddr.sun_path, WINKS_SOCKET_SERVER_ADDRESS) ;
	//strcpy( server_sockaddr.sun_path, "/var/mobile/server_socket_wwg" ) ; 
	len=sizeof(server_sockaddr);
#endif
	
	
	
#if 1
	//创建socket
	client_sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(client_sockfd<0)
	{  
		Winks_printfErr("error creat socket =\n",strerror(errno));
        return WINKS_RETURN_FAILURE;  
    }
	
	server_sockaddr.sin_family = AF_INET;
#define SERVERPORT 9734
	server_sockaddr.sin_port = htons(SERVERPORT);
	server_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(server_sockaddr.sin_zero), 8);
	len=sizeof(server_sockaddr);	
#endif

	//连接server
	result = connect(client_sockfd,( struct sockaddr * )&server_sockaddr,len);
	if (result <0){
		Winks_printfErr("error on connecting:%s\n",strerror(errno));
		//exit(1);
		return WINKS_RETURN_FAILURE;
	}
	
	int portnum = 0;
	
    portnum = getsockname(client_sockfd,(struct sockaddr *)&server_sockaddr,&len);
	
	Winks_printf("end Winks_InitSocket port=%d sfd=%d",server_sockaddr.sin_port,client_sockfd);
	return WINKS_RETURN_SUCCESS;
}

//关闭客户端socket
void Winks_CloseSocket() {
	Winks_printf("into Winks_CloseSocket sfd=%d",client_sockfd);
	
	//关闭客户端socket
	if (client_sockfd != -1) {
		close (client_sockfd);
		client_sockfd = -1;
		Winks_printf("close Winks_CloseSocket");
	}
	Winks_printf("out Winks_CloseSocket");
}





