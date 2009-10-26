//
//  GprsInitor_2.m
//  netTest
//
//  Created by luke on 09-10-25.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GprsInitor_2.h"
#import <stdio.h>
#import <stdlib.h>
#import <sys/types.h>
#import <sys/socket.h>
#import <netdb.h>

#define HOSTNAME "www.google.com"
#define PORT "80"


@implementation GprsInitor_2

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

- (NSInteger) gprsInit
{
	int sockfd;
	int numRecv, numSent;
	struct addrinfo hints, *servInfo, *p;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	int rv;
	if ((rv = getaddrinfo(HOSTNAME, PORT, &hints, &servInfo)) != 0) {
		Winks_printf("getaddrinfo: %s", gai_strerror(rv));
		return -1;
	}
	
	for (p = servInfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			Winks_printf("socket: %s", strerror(errno));
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			Winks_printf("connect: %s", strerror(errno));
			continue;
		}
		break;
	}
	
	if (p == NULL) {
		Winks_printf("fail to connect");
		return -1;
	}
	
	//debug
	char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof(s));
	Winks_printf("connected to %s", s);
	
	
	
//	CFHostRef 
//	CFStreamCreatePairWithSocketToCFHost(kCFAllocatorDefault,
//										 CFHostRef host,
//										 SInt32 port,
//										 CFReadStreamRef *readStream,
//										 CFWriteStreamRef *writeStream
//										 );
	
	return 0;
}


@end
