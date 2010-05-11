//
//  tcpClient.m
//  msn_for_mac
//
//  Created by  apple on 08-9-11.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "TcpClient.h"
#import "ClientNetWorkController.h"
#import "define.h"
#import "API.h"
#import "MSNAppDelegate.h"
#import <CFNetwork/CFHost.h>
#import <CFNetwork/CFSocketStream.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

static TCPClient *tcpClient = nil;
@interface TCPTask : NSOperation
{	
	NSInteger messageType;
	NSString *requestString;
}

- (id) initwithString:(NSString*)theString;

@property (nonatomic,retain) NSString *requestString;
@property (nonatomic) NSInteger messageType;

@end

@implementation TCPTask
@synthesize requestString, messageType;

- (id) initwithString:(NSString*)theString
{
	self = [super init];
	if (self != nil) 
	{
		self.requestString = theString;
	}
	return self;
}

- (void) dealloc
{
	[requestString release];
	[super dealloc];
}

- (void)main
{
	if (requestString != nil)
		[tcpClient writeRequestIfValid:requestString messageType:messageType];
}
@end

static const CFOptionFlags kNetworkEvents = kCFStreamEventOpenCompleted |
										  kCFStreamEventCanAcceptBytes |
									   kCFStreamEventHasBytesAvailable |
										  kCFStreamEventEndEncountered |
											kCFStreamEventErrorOccurred;

static void ReadStreamClientCallBack(CFReadStreamRef readStream, CFStreamEventType type, void *clientCallBackInfo)
{
	RETURN_WHEN_LESS_MEMORY;
	[(TCPClient*)clientCallBackInfo handleNetworkEvent:type];
}

static void WriteStreamClientCallBack(CFWriteStreamRef readStream, CFStreamEventType type, void *clientCallBackInfo)
{
	RETURN_WHEN_LESS_MEMORY;
	[(TCPClient*)clientCallBackInfo handleWriteEvent:type];
}


//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


static NSLock *sWriteLock = nil;

@implementation TCPClient

@synthesize delegate, callbackFun, messageType, writeLock, receivedData, unParsedData, brakeTimer;

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		delegate = nil;
		callbackFun = nil;
		receivedData = nil;
		self.brakeTimer = nil;
		if (sWriteLock == nil)
			sWriteLock = [[NSLock alloc] init];
		self.writeLock = sWriteLock;
		self.unParsedData = [NSMutableData dataWithCapacity:0];
		networkQueue = [[NSOperationQueue alloc] init];
		[networkQueue setMaxConcurrentOperationCount:1];
		receivedQueue = [[NSOperationQueue alloc] init];
		[receivedQueue setMaxConcurrentOperationCount:1];
	}
	return self;
}

- (void) dealloc
{
	[networkQueue cancelAllOperations];
	[networkQueue release];
	[receivedQueue cancelAllOperations];
	[receivedQueue release];
	
	[self reset];
	
	[self unlockStream];
	[writeLock release];
	
	if ([sWriteLock retainCount] == 1)
	{
		[sWriteLock release];
		sWriteLock = nil;
	}
	[receivedData release];
	[unParsedData release];
	if (tcpClient == self)
		tcpClient = nil;
	[super dealloc];
}

+ (TCPClient*)getInstance
{
	if (tcpClient == nil)
	{
		tcpClient = [[[TCPClient alloc] init] autorelease];
	}
	return tcpClient;
}

- (BOOL)connectSocket
{
	BOOL ret = NO;
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"login"];
	if (dict != nil && [dict count] > 0)
	{
		NSString *ip = [dict valueForKey:@"ip"];
		int index = [ip rangeOfString:@":"].location;
		NSString *address = [ip substringToIndex:index];
		NSString *port = [ip substringFromIndex:index+1];
		struct sockaddr_in addr4;
		memset(&addr4, 0, sizeof(addr4));
		addr4.sin_len = sizeof(addr4);
		addr4.sin_family = AF_INET;
		addr4.sin_port = htons([port intValue]);
		addr4.sin_addr.s_addr = inet_addr([address UTF8String]); 
		
		NSData *address4 = [NSData dataWithBytes:&addr4 length:sizeof(addr4)];
		
		CFHostRef host = CFHostCreateWithAddress(kCFAllocatorDefault,(CFDataRef)address4);
		if (host)
		{
			CFStreamCreatePairWithSocketToCFHost (kCFAllocatorDefault,
												  host,
												  [port intValue],
												  &readStream,
												  &writeStream);
			CFStreamClientContext ctxt = {0, self, NULL, NULL, NULL};
			if (!CFReadStreamSetClient(readStream, kNetworkEvents, ReadStreamClientCallBack, &ctxt)) {
				CFRelease(readStream);
				readStream = NULL;
				MYLog(@"--------------CFReadStreamSetClient 2 fail----------");	
				return ret;
			}
			
			CFReadStreamScheduleWithRunLoop(readStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
			
			if (!CFReadStreamOpen(readStream)) 
			{
				CFReadStreamSetClient(readStream, 0, NULL, NULL);
				CFReadStreamUnscheduleFromRunLoop(readStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
				CFRelease(readStream);
				readStream = NULL;
				MYLog(@"--------------CFReadStreamOpen 2 fail----------");	
				return ret;
			}	
			
			CFStreamClientContext ctxt2 = {0, self, NULL, NULL, NULL};
			if (!CFWriteStreamSetClient(writeStream, kNetworkEvents, WriteStreamClientCallBack, &ctxt2))
			{
				CFRelease(writeStream);
				writeStream = NULL;
				MYLog(@"--------------CFReadStreamSetClient 2 fail----------");	
				return ret;
			}
			
			CFWriteStreamScheduleWithRunLoop(writeStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
			if (!CFWriteStreamOpen(writeStream)) 
			{
				CFWriteStreamSetClient(writeStream, 0, NULL, NULL);
				CFWriteStreamUnscheduleFromRunLoop(writeStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
				CFRelease(writeStream);
				writeStream = NULL;
				MYLog(@"--------------CFReadStreamOpen 2 fail----------");	
				return ret;
			}
			[self lockStream];	
			ret = YES;
			CFRelease(host);
			//[ self performSelectorOnMainThread:@selector(lockWriteStream) withObject:nil waitUntilDone:YES];		
		}   
	}
	return ret;
}

- (void)writeString:(NSString*)theString messageType:(NSInteger)theType
{
	RETURN_WHEN_LESS_MEMORY;
	MYLog(@"send string: %@", theString);
	if (brakeTimer != nil){
		[brakeTimer invalidate];
		self.brakeTimer = nil;
	}
	self.brakeTimer  = [NSTimer scheduledTimerWithTimeInterval:150.0 target:self 
											selector:@selector(sendBrakeMessage:) 
											userInfo:nil 
											 repeats:YES];	

	TCPTask * request = [[TCPTask alloc] initwithString:theString];
	request.messageType = theType;
	[networkQueue addOperation:request];
	[request release];
}


- (void)sendBrakeMessage:(NSTimer*)theTimer
{
	TCPTask * request = [[TCPTask alloc] initwithString:@" "];
	request.messageType = messageType;
	[networkQueue addOperation:request];
	[request release];
}

- (void)reset{
	[networkQueue cancelAllOperations];
	[networkQueue waitUntilAllOperationsAreFinished];
	[receivedQueue cancelAllOperations];
	[receivedQueue waitUntilAllOperationsAreFinished];
	
	delegate = nil;
	callbackFun = nil;
	
	if (readStream)
	{
        CFReadStreamSetClient(readStream, kCFStreamEventNone, nil, nil);
        CFReadStreamUnscheduleFromRunLoop(readStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
        CFReadStreamClose(readStream);
        CFRelease(readStream);
        readStream = nil;
    }
	if (writeStream)
	{
    	CFWriteStreamSetClient(writeStream, 0, NULL, NULL);
		CFWriteStreamUnscheduleFromRunLoop(writeStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
	    CFWriteStreamClose(writeStream);
		CFRelease(writeStream);
		writeStream = nil;
    }
	
	[brakeTimer invalidate];
	self.brakeTimer = nil;
	
}

- (void)writeRequestIfValid:(NSString*)theString messageType:(NSInteger)theType
{
	messageType = theType;
	[self lockStream];
	[self unlockStream];
	[self performSelectorOnMainThread:@selector(lockStream) withObject:nil waitUntilDone:YES];
	CFIndex index = 0;

	if (theString != nil && writeStream != nil)
	{
		NSData *data = [theString dataUsingEncoding:NSUTF8StringEncoding];
		index = CFWriteStreamWrite (writeStream, (UInt8*)[data bytes], [data length]);
		
		if (index != [data length])
		{
			MYLog(@"write failed");
			[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
		}
	}
}


- (void)lockStream
{
	if (sWriteLock != nil)
		[sWriteLock lock];
}

- (void)unlockStream
{
	if (sWriteLock != nil)
	{
		[sWriteLock tryLock];
		[sWriteLock unlock];	
	}
}

- (void)handleWriteEvent:(CFStreamEventType)type
{	
	switch (type) {
		case kCFStreamEventOpenCompleted:
            break;
			
        case kCFStreamEventCanAcceptBytes:
			[self unlockStream];
            break;
            
        case kCFStreamEventEndEncountered:
            break;
            
        case kCFStreamEventErrorOccurred:
            [self handleStreamError];
            break;
            
        default:
            break;
	} 
}

- (void)handleNetworkEvent:(CFStreamEventType)type
{
    switch (type) {
		case kCFStreamEventOpenCompleted:
            break;
			
        case kCFStreamEventHasBytesAvailable:
			[self handleStreamAvailable];	
            break;
            
        case kCFStreamEventEndEncountered:
			MYLog(@"---------kCFStreamEventEndEncountered----");
            [self handleStreamComplete];
            break;
            
        case kCFStreamEventErrorOccurred:
            //[self handleStreamError];
            break;
            
        default:
            break;
    }
}

- (void)handleStreamAvailable
{
	UInt8 buffer[256]={0};
    CFIndex bytesRead = CFReadStreamRead(readStream, buffer, sizeof(buffer) - 1);
    NSLog(@"tcp client buffer : %s", buffer);
	if (bytesRead < 0)
	{
        [self handleStreamError];
    }
	else if (bytesRead > 0) 
	{
		if (bytesRead == sizeof(buffer) - 1)
		{
			if (receivedData == nil)
			{
				self.receivedData = [NSMutableData dataWithLength:0];
			}
			[receivedData appendBytes:buffer length:bytesRead];
		}
		else
		{
			NSData *data = nil;
			if (receivedData != nil)
			{
				[receivedData appendBytes:buffer length:bytesRead];
				
				data = [NSData dataWithBytes:[receivedData bytes] length:[receivedData length]];
				self.receivedData = nil;	
			}
			else
			{
				data = [NSData dataWithBytes:buffer length:bytesRead];
			}
			
			if (data != nil)
			{
				NSInvocationOperation *option = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(parserValidData:) object:data];
				[receivedQueue addOperation:option];
				[option release];
			}
		}
    }
	else
		MYLog(@"---------bytesRead:%d------",bytesRead);
}

- (void)parserValidData:(NSData*)theData
{
	if (messageType == DISPATCH_REQUEST)
	{

		//xmlparser can not parse data
		[delegate performSelector:callbackFun withObject:theData withObject:[NSNumber numberWithInt:messageType]];
	}
	else
	{
		NSString * tmpString = [[[NSString alloc] initWithData:theData encoding:NSUTF8StringEncoding] autorelease];

//		MYLog(@"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
//		MYLog(@"Data string : \t%@",tmpString);
//		MYLog(@"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		
		if ([tmpString rangeOfString:@"mclub.pica"].location != NSNotFound)
		{
			// mClub process.
			[delegate praseMClubResponseData:tmpString];
			return;
		}
		
		[unParsedData appendData:theData];
		NSMutableData *testData = [NSMutableData data];
		[testData appendData:[@"<queue>" dataUsingEncoding:NSUTF8StringEncoding]];
		[testData appendData:unParsedData];
		[testData appendData:[@"</queue>" dataUsingEncoding:NSUTF8StringEncoding]];
		
		
		NSXMLParser *parser = [[NSXMLParser alloc] initWithData:testData];
		[parser parse];
		NSError *parseError = [parser parserError];
		if (parseError == nil)
		{
			[parser release];
			
			[delegate performSelector:callbackFun withObject:unParsedData withObject:[NSNumber numberWithInt:messageType]];
			self.unParsedData = [NSMutableData dataWithCapacity:0];
		}
		else
		{
			[parser release];
			NSData *data = nil;
			int count = 0;
			int length = 0;
			int add = 0;
			NSRange range;
			
			NSString * tmpString = [[NSString alloc] initWithData:unParsedData encoding:NSUTF8StringEncoding];
//			MYLog(@">>> STH error, and IN Stream:%@",tmpString);
			
			if (tmpString != nil && [tmpString length] > 0)
			{
				NSRange subRange =  NSMakeRange(0, [tmpString length]);
				
				do{
					length = NSIntegerMax;
					range = [tmpString rangeOfString:@"</" options:NSCaseInsensitiveSearch range:subRange];
					if (range.location != NSNotFound)
					{
						add = -1;
						length = range.location;
					}
					
					range = [tmpString rangeOfString:@"<" options:NSCaseInsensitiveSearch range:subRange];
					if (range.location != NSNotFound)
					{
						if (range.location < length)
						{
							add = 1;
							length = range.location;
						}
					}
					
					if (length != NSIntegerMax)
					{
						length = NSIntegerMax;
						range = [tmpString rangeOfString:@">" options:NSCaseInsensitiveSearch range:subRange];
						if (range.location != NSNotFound)
						{
							length = range.location + range.length;
						}
						
						range = [tmpString rangeOfString:@"/>" options:NSCaseInsensitiveSearch range:subRange];
						if (range.location != NSNotFound)
						{
							if (range.location + range.length <= length)
							{
								add = 0;
								length = range.location + range.length;
							}
						}
						
						if (length != NSIntegerMax)
						{
							subRange.location = length;
							subRange.length = [tmpString length] - length;
							count += add;
							
							if (count == 0)
							{				
								if (subRange.location >= [tmpString length])
								{
									data = [NSData dataWithData: [tmpString dataUsingEncoding:NSUTF8StringEncoding]];
									[delegate performSelector:callbackFun withObject:data withObject:[NSNumber numberWithInt:messageType]];
									[tmpString release];
									tmpString = nil;
									self.unParsedData = [NSMutableData dataWithCapacity:0];
									break;
								}
								else
								{
									NSString *subString = nil;
									subString = [tmpString substringToIndex:subRange.location];
									data = [NSData dataWithData: [subString dataUsingEncoding:NSUTF8StringEncoding]];
									[delegate performSelector:callbackFun withObject:data withObject:[NSNumber numberWithInt:messageType]];
									
									subString = [tmpString substringFromIndex:subRange.location];
									[tmpString release];
									tmpString = [subString retain];
									subRange =  NSMakeRange(0, [tmpString length]);
								}
							}
							
							if (subRange.location >= [tmpString length])
								break;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
					
				}while(1);
				
				if (tmpString != nil)
				{
					self.unParsedData = [NSMutableData dataWithData:[tmpString dataUsingEncoding:NSUTF8StringEncoding]];
					[tmpString release];
				}		
			}	
		}
	}

}

- (void)threadHandleBytesAvailable:(id)param
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[delegate performSelector:callbackFun withObject:(NSData*)param withObject:[NSNumber numberWithInt:messageType]];
	[pool drain];
}

- (void)handleStreamError
{
	NSError *err = (NSError *)CFReadStreamCopyError(readStream);
	if (err) 
	{ 
		NSDictionary *info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"pica_str_tip_disconnected_unknown_reason",nil) forKey:@"message"];
		[ClientNetWorkController postNotification:NetWorkDisabledNotification info:info];
		MYLog(@"error : %@",[err localizedDescription]);
		[err release];
	}
	[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
}

- (void)handleStreamComplete
{
	[ClientNetWorkController postNotification:LogoutNotification info:nil];
}
@end
