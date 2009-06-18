//
//  httpClient.m
//  propertyList
//
//  Created by wang luke on 5/20/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "httpClient.h"


@implementation httpClient
@synthesize delegate;

- (id) initWithDelegate:(id<httpClientDelegate>)theDelegate
{
	[self init];
	self.delegate = theDelegate;
	return self;
}

- (void) setRequest
{
	
}

- (void) connectAndDown
{
	//http://l.yimg.com/a/i/ww/beta/y3.gif
	//http://www.google.com/logos/missinglink.gif
	//http://www.apple.com.cn/macpro/images/overview_hero1_20090303.png
	//http://www.gifanimations.com/GA/image/animations/aliens/alien-01.gif
	//http://online.sccnn.com/gif8/021/gif0405_001.gif

	NSString *filepath = [self download:@"http://online.sccnn.com/gif8/021/gif0405_001.gif"];
	[self.delegate didFinishDownload:filepath];
}

- (void) getResponseHeader
{
	
}

#pragma mark -
#pragma mark c utils

// 搜索字符串右边起的第一个匹配字符
char * Rstrchr(char * s, char x) {
	int i = strlen(s);
	if(!(*s)) 
		return 0;
	while(s[i-1]) 
		if(strchr(s + (i - 1), x)) 
			return (s + (i - 1)); 
		else i--;
	return 0;
}

// 把字符串转换为全小写
void ToLowerCase(char * s) {
	while(s && *s) {
		*s=tolower(*s);
		s++;
	}
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

// 得到HOME目录
static char home[1024];
char* gethome()
{
	bzero(home, sizeof(home));
	strcpy(home, getenv("HOME"));
	return home;
}

#pragma mark -
#pragma mark BSD Socket Method
// 取得主机IP地址
struct hostent * getHost(NSString *name)
{
	struct hostent *host;
	const char* host_addr = [name UTF8String];
	if((host=gethostbyname(host_addr))==NULL){
		fprintf(stderr,"Gethostname error, %s\n", strerror(errno));
		exit(1);
	}
	printf("----host IP:hostent---------------\n");
	printf("host.h_name = %s\n", host->h_name);
	printf("host.h_aliases[0] = %s\n", host->h_aliases[0]);
	printf("host.h_addrtype = %d\n", host->h_addrtype);
	printf("host.h_length = %d\n", host->h_length);
	printf("host.h_addr = %s\n", host->h_addr);
	printf("-------------------------------\n");
	
	return host;
}


- (NSString *) download:(NSString *)url
{
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber,nbytes;
	char host_addr[256];
	char host_file[1024];
	char local_file[256];
	FILE * fp;
	char request[1024];
	int send, totalsend;
	int i;
	char * pt;
	
	char *url_c = [url UTF8String];
	
	if(strlen(url_c) <= 0) {
		NSLog(@"url can not be nil");
		//exit(1);
	}
	// 将参数转换为全小写
	//ToLowerCase(url_c);
	printf("lowercase url is: %s\n", url_c);
	
	GetHost(url_c, host_addr, host_file, &portnumber);/*分析网址、端口、文件名等*/
	printf("\n\n----------------------------------\n");
	printf("host_addr:%s\n", host_addr);
	printf("host_file:%s\n", host_file);
	printf("portnumber:%d\n", portnumber);
	printf("----------------------------------\n\n");
	
	// 取得主机IP地址
	if((host=gethostbyname(host_addr))==NULL){
		fprintf(stderr,"Gethostname error, %s\n", strerror(errno));
		//exit(1);
	}
	printf("----host IP:hostent---------------\n");
	printf("host.h_name = %s\n", host->h_name);
	printf("host.h_aliases[0] = %s\n", host->h_aliases[0]);
	printf("host.h_addrtype = %d\n", host->h_addrtype);
	printf("host.h_length = %d\n", host->h_length);
	printf("host.h_addr = %s\n", host->h_addr);
	printf("-------------------------------\n");
	
	// 客户程序开始建立 sockfd描述符
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)/*建立SOCKET连接*/
	{
		fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		//exit(1);
	}
	
	// 客户程序填充服务端的资料
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);
	
	// 客户程序发起连接请求
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)/*连接网站*/
	{
		fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		//exit(1);
	}
	
	//sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
//			User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
//			Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber);
	sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nHost: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber);
	
	printf("----request---------------\n");
	printf("%s", request);// 准备request，将要发送给服务器
	
	char *local_request = strcat(gethome(), "/request.txt");
	FILE *requestfp = fopen(local_request, "a");
	fwrite(request, 1, strlen(request), requestfp);
	fclose(requestfp);
	printf("-------------------------------\n");
	
	// 取得真实的文件名
	if(host_file && *host_file)
		pt = Rstrchr(host_file, '/');
	else 
		pt = 0;
	
	memset(local_file, 0, sizeof(local_file));
	if(pt && *pt) {
		if((pt + 1) && *(pt+1))
			strcpy(local_file, pt + 1);
		else
			memcpy(local_file, host_file, strlen(host_file) - 1);
	}
	else if(host_file && *host_file) 
		strcpy(local_file, host_file);
	else 
		strcpy(local_file, "index.html");
//	char home[1024];
//	strcpy(home,getenv("HOME"));
	char tt[1024] = "/";
	char local_file_path[1024];
	strcpy(local_file_path,strcat(gethome(), strcat(tt, local_file)));
	printf("local filename to write:%s\n\n", local_file);
	
	// 发送http request
	send = 0;totalsend = 0;
	nbytes=strlen(request);
	while(totalsend < nbytes) {
		send = write(sockfd, request + totalsend, nbytes - totalsend);
		if(send==-1) {
			printf("send error!%s\n", strerror(errno));exit(0);
		}
		totalsend+=send;
		printf("%d bytes send OK!\n", totalsend);
	}
	
	fp = fopen(local_file_path, "a");
	
		
	char *local_responseHeader = strcat(gethome(), "/responseHeader.txt");
	FILE *responseHeader = fopen(local_responseHeader, "a");
	if(!fp) {
		printf("create file error! %s\n", strerror(errno));
		//return 0;
	}
	printf("\n----------response header------------\n");
	i=0;
	// 接收http response
	while((nbytes=read(sockfd,buffer,1))==1)
	{
		if(i < 4) {
			if(buffer[0] == '\r' || buffer[0] == '\n') 
				i++;
			else 
				i = 0;
			printf("%c", buffer[0]);//打印http头
			fwrite(buffer, 1, 1, responseHeader);
		}
		else {
			fwrite(buffer, 1, 1, fp);//将正文数据写入文件
			i++;
			if(i%1024 == 0) 
				fflush(fp); // 每1K时存盘一次
		}
	}
	fclose(responseHeader);
	printf("----------------------------------\n");
	fclose(fp);
	// 结束通讯
	close(sockfd);
	//exit(0);
	//return 0;
	
	NSString* downloadedFilePath = [[NSString alloc] initWithCString:local_file_path];
	//[downloadedFilePath release];
	NSLog(@">>>>>>>>>> %@",downloadedFilePath);
	return downloadedFilePath;
}

#pragma mark -
#pragma mark operation object 在另一线程下载图片
//- (void)launchTaskWithData:(id)url
//{
//    NSInvocationOperation* theOp = [[NSInvocationOperation alloc] initWithTarget:self
//																		selector:@selector(download:) object:url];
//	
//	// Get the custom queue object from the app delegate.
////    NSOperationQueue* myQueue = [[[UIApplication sharedApplication] delegate] myOperationQueue];
////    [myQueue addOperation:theOp];
//    // Add the operation to the internal operation queue managed by the application delegate.
//    [[propertyListAppDelegate sharedOperationQueue] addOperation:theOp];
//}


@end
