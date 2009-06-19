//
//  AsyncSock.m
//  propertyList
//
//  Created by wang luke on 6/19/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "AsyncSock.h"
#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <netdb.h>


@implementation AsyncSock

- (void)doCFSocketCallback:(CFSocketCallBackType)type
				 forSocket:(CFSocketRef)sock
			   withAddress:(NSData *)address
				  withData:(const void *)pData
{
	//NSParameterAssert ((sock == theSocket4) || (sock == theSocket6));
	
	switch (type)
	{
		case kCFSocketConnectCallBack:
			// The data argument is either NULL or a pointer to an SInt32 error code, if the connect failed.
			if(pData)
				[self doSocketOpen:sock withCFSocketError:kCFSocketError];
			else
				[self doSocketOpen:sock withCFSocketError:kCFSocketSuccess];
			break;
		default:
			NSLog (@"AsyncSocket %p received unexpected CFSocketCallBackType %d.", self, type);
			break;
	}
}

/**
 * This is the callback we setup for CFSocket.
 * This method does nothing but forward the call to it's Objective-C counterpart
 **/
static void MyCFSocketCallback (CFSocketRef sref, CFSocketCallBackType callbackType, CFDataRef address, const void *pData, void *pInfo)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	AsyncSock *theSocket = [[(AsyncSock *)pInfo retain] autorelease];
	[theSocket doCFSocketCallback:callbackType forSocket:sref withAddress:(NSData *)address withData:pData];
	
	[pool release];
}

- (CFSocketRef)createSocket{
	CFSocketContext context = {
		context.version = 0,
		context.info = self,
		context.retain = nil,
		context.release = nil,
		context.copyDescription = nil
	};
	CFSocketRef sock1 = CFSocketCreate(kCFAllocatorDefault, PF_INET, SOCK_STREAM, IPPROTO_TCP, kCFSocketConnectCallBack, (CFSocketCallBack)&MyCFSocketCallback, &context);
	return sock1;
}


@end
