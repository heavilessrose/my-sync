//
//  main.m
//  netTest
//
//  Created by wang luke on 09-10-23.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/types.h>
#import <sys/socket.h>
#import <errno.h>
#import <unistd.h>
#import <netinet/in.h>
#import <limits.h>
#import <netdb.h>
#import <arpa/inet.h>
#import <ctype.h>

#import "Utils.h"

int initGprs()
{
	CFSocketRef cfSock = CFSocketCreate( kCFAllocatorDefault, PF_INET, SOCK_STREAM, 
										IPPROTO_TCP, kCFSocketNoCallBack, NULL, NULL );
	
	if(CFSocketIsValid(cfSock)){
		struct sockaddr_in sin;
		sin.sin_family = PF_INET;
		sin.sin_addr.s_addr = inet_addr("66.249.89.147");
		sin.sin_port = htons(80);
		
		CFDataRef address = CFDataCreate ( kCFAllocatorDefault, (UInt8 *)&sin, sizeof(sin) );
		if(address == NULL){
			Winks_printf("create cfdata err");
			return -1;
		}
		CFSocketError err = CFSocketConnectToAddress (cfSock, address, 60);
		if (err == 0) {
			Winks_printf("CFSocketConnectToAddress success!! ");
			return 0;
		}else{
			Winks_printf("CFSocketConnectToAddress err = %d", err);
			return -1;
		}
	}else{
		Winks_printf("create cfSock fail");
		return -1;
	}
	return 0;
}

int initGprs2()
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSURLRequest *theRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.google.com/logos/missinglink.gif"]
												cachePolicy:NSURLRequestReloadIgnoringLocalCacheData 
											timeoutInterval:60];
	
	
	//NSURLConnection *nscon = [NSURLConnection connectionWithRequest:theRequest delegate:nil];
	NSURLConnection *nscon = [[NSURLConnection alloc] initWithRequest:theRequest 
										   delegate:nil startImmediately:YES];
	
	if(nscon == nil){
		Winks_printf("NSURLConnection fail");
		return -1;
	}
	Winks_printf("NSURLConnection success!!");
	[pool release];
	return 0;
}

// 从字符串中解析出服务器地址和端口及要下载的文件
void GetHost(char * src, char * host, char * file, int * port) {
	char * pA;
	char * pB;
	memset(host, 0, sizeof(host));
	memset(file, 0, sizeof(file));
	*port = 0;
	if(!(*src))
		return;
	pA = src;
	if(!strncmp(pA, "http://", strlen("http://")))
		pA = src+strlen("http://");
	else if(!strncmp(pA, "https://", strlen("https://")))
		pA = src+strlen("https://");
	pB = strchr(pA, '/');
	if(pB) {
		memcpy(host, pA, strlen(pA) - strlen(pB));
		if(pB+1) {
			memcpy(file, pB + 1, strlen(pB) - 1);
			file[strlen(pB) - 1] = 0;
		}
	}else{
		memcpy(host, pA, strlen(pA));
	}
	
	if(pB) 
		host[strlen(pA) - strlen(pB)] = 0;
	else 
		host[strlen(pA)] = 0;
	pA = strchr(host, ':');
	if(pA) 
		*port = atoi(pA + 1);
	else 
		*port = 80;
}

void sockTest()
{
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *host;
	char request[1024];
	char host_addr[256];
	char host_file[1024];
	int portnumber;
	GetHost("http://www.google.com/logos/missinglink.gif", host_addr, host_file, &portnumber);/*分析网址、端口、文件名等*/
	
	//	Winks_printf("\r\n\r\n----------------------------------\r\n");
	//	Winks_printf("host_addr:%s\r\n", host_addr);
	//	Winks_printf("host_file:%s\r\n", host_file);
	//	Winks_printf("portnumber:%d\r\n", portnumber);
	//	Winks_printf("--------------------------------------\r\n");
	
	
	sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
			User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
			Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber);
	
	//	Winks_printf("request: %s\r\n", request);
	
	// 取得主机IP地址
	if((host=gethostbyname(host_addr))==NULL){
		Winks_printf("Gethostname error, %s\r\n", strerror(errno));
		exit(1);
	}
	//	Winks_printf("host IP:hostent---------------\r\n");
	//	Winks_printf("host.h_name = %s\r\n", host->h_name);
	//	Winks_printf("host.h_aliases[0] = %s\r\n", host->h_aliases[0]);
	//	Winks_printf("host.h_addrtype = %d\r\n", host->h_addrtype);
	//	Winks_printf("host.h_length = %d\r\n", host->h_length);
	//	Winks_printf("host.h_addr = %s\r\n", host->h_addr);
	//	Winks_printf("-------------------------------\r\n");
	char s[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, host->h_addr, s, sizeof s);
	
	
	// 客户程序开始建立 sockfd描述符
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)/*建立SOCKET连接*/
	{
		Winks_printf("Socket Error:%s\r\n", strerror(errno));
		exit(1);
	}
	
	// 客户程序填充服务端的资料
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);	
	
	// 客户程序发起连接请求
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		Winks_printf("Connect Error:%s\r\n",strerror(errno));
		exit(1);
	}
	Winks_printf("connect to %s\r\n", s);
	
	// 发送请求
	int sent = write(sockfd, request, strlen(request));
	
	// 接收
	char buffer[256];
	int rcved = read(sockfd,buffer,256);
	Winks_printf("\r\nreceived :\r\n%s", buffer);
}

int main(int argc, char *argv[]) {
    
	if(initGprs() == -1){
		Winks_printf("init gprs 2 fail~!!");
	}
	if(initGprs2() == -1){
		Winks_printf("init gprs 2 fail~!!");
	}

    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
