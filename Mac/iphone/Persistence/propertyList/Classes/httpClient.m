//
//  httpClient.m
//  propertyList
//
//  Created by wang luke on 5/20/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "httpClient.h"


@implementation httpClient

- (void) setRequest
{
	
}

- (void) connectAndDown
{
	//http://l.yimg.com/a/i/ww/beta/y3.gif
	//http://www.google.com/logos/missinglink.gif
	[self download:@"http://www.apple.com.cn/macpro/images/overview_hero1_20090303.png"];
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

// 把字符串转换为全小写/
void ToLowerCase(char * s) {
	while(s && *s) {
		*s=tolower(*s);
		s++;
	}
}

// 从字符串src中解析出网站地址和端口，并得到用户要下载的文件
void GetHost(char * src, char * web, char * file, int * port) {
	char * pA;
	char * pB;
	memset(web, 0, sizeof(web));
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
		memcpy(web, pA, strlen(pA) - strlen(pB));
		if(pB+1) {
			memcpy(file, pB + 1, strlen(pB) - 1);
			file[strlen(pB) - 1] = 0;
		}
	}else{
		memcpy(web, pA, strlen(pA));
	}
	
	if(pB) 
		web[strlen(pA) - strlen(pB)] = 0;
	else 
		web[strlen(pA)] = 0;
	pA = strchr(web, ':');
	if(pA) 
		*port = atoi(pA + 1);
	else 
		*port = 80;
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

- (int) download:(NSString *)url
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
	//ToLowerCase(url_c);/*将参数转换为全小写*/
	printf("lowercase url is: %s\n", url_c);
	
	GetHost(url_c, host_addr, host_file, &portnumber);/*分析网址、端口、文件名等*/
	printf("\n\n----------------------------------\n");
	printf("host_addr:%s\n", host_addr);
	printf("host_file:%s\n", host_file);
	printf("portnumber:%d\n", portnumber);
	printf("----------------------------------\n\n");
	
	/*取得主机IP地址*/
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
	
	/* 客户程序开始建立 sockfd描述符 */
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
	sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
			User-Agent: Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543 Safari/419.3\r\n\
			Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber);
	
	printf("----request---------------\n");
	printf("%s", request);// 准备request，将要发送给服务器
	
	//char *local_request = strcat(getenv("HOME"), strcat("/", "request.txt"));
	//FILE *requestfp = fopen(local_request, "a");
	//fwrite(request, 1, strlen(request), requestfp);
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
	char home[1024];
	strcpy(home,getenv("HOME"));
	char tt[1024] = "/";
	char local_file_path[1024];
	strcpy(local_file_path,strcat(home, strcat(tt, local_file)));
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
	//printf("local_file = %s\n", local_file_path);
	//char *local_responseHeader = strcat(getenv("HOME"), strcat("/", "responseHeader.txt"));
	//FILE *responseHeader = fopen(local_responseHeader, "a");
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
			//fwrite(buffer, 1, 1, responseHeader);
		}
		else {
			fwrite(buffer, 1, 1, fp);//将正文数据写入文件
			i++;
			if(i%1024 == 0) 
				fflush(fp); // 每1K时存盘一次
		}
	}
//	fclose(responseHeader);
//	fclose(fp);
	printf("----------------------------------\n");
	fclose(fp);
	// 结束通讯
	close(sockfd);
	//exit(0);
}
@end
