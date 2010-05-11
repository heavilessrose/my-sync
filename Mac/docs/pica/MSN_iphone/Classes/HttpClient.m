//
//  HttpRequest.m
//  msn_for_mac
//
//  Created by  apple on 08-9-10.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//


#import "HttpClient.h"
#import "ClientNetWorkController.h"
#import "define.h"

#import <CFNetwork/CFHTTPMessage.h>
#import <CFNetwork/CFHTTPStream.h>

static HttpClient *httpClient = nil;

@interface HttpRequest : NSOperation
{	
	HttpClient *client;
	NSDictionary *requestHeaders;
	NSData *bodyString;
	NSString *method;
	NSURL *requstURL;
	NSInteger messageType;
}

@property (nonatomic, retain) NSURL *requstURL;
@property (nonatomic, retain) NSString *method;
@property (nonatomic, retain) NSData *bodyString;
@property (nonatomic, retain) NSDictionary *requestHeaders;
@property (nonatomic, retain) HttpClient *client;
@property (nonatomic) NSInteger messageType;
@end


@implementation HttpRequest

@synthesize method, bodyString, requstURL,requestHeaders, client, messageType;

- (id)init
{
	self = [super init];
	if (self)
	{
		bodyString = nil;
		method = nil;
		requestHeaders = nil;
		client = nil;
	}
	
	return self;
}

- (void)dealloc
{
	[client release];
	[requstURL release];
	[bodyString release];
	[method release];
	[requestHeaders release];
	[super dealloc];
}

#if 0

+ (HttpRequest*)requestObject{
	return [[[[self class] alloc] init] autorelease];
}

#endif

- (void)main
{
 	[client sendHttpRequest:requstURL method:method headers:requestHeaders bodyString:bodyString type:messageType];
}
@end

/////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////


static const CFOptionFlags kNetworkEvents = kCFStreamEventOpenCompleted | kCFStreamEventHasBytesAvailable
											| kCFStreamEventEndEncountered | kCFStreamEventErrorOccurred;

static void ReadStreamClientCallBack(CFReadStreamRef readStream, CFStreamEventType type, void *clientCallBackInfo)
{
	[(HttpClient*)clientCallBackInfo handleNetworkEvent:type];
}

NSString		*NetWorkDisabledNotification = @"NetWorkDisabled";

@implementation HttpClient

@synthesize delegate,callbackFun, writeLock,messageType,h3gTimer;

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		responseHeaders = nil;
        readStream = nil;
		delegate = nil;
		callbackFun = nil;
		h3gTimer = nil;
		requestComplete = NO;
		receivedData = nil;
		writeLock = [[NSLock alloc] init];
		
		networkQueue = [[NSOperationQueue alloc] init];
		[networkQueue setMaxConcurrentOperationCount:1];
	}
	return self;
}

- (void) dealloc
{
	callbackFun = nil;
	if (readStream)
	{
        CFReadStreamClose(readStream);
        CFReadStreamSetClient(readStream, kCFStreamEventNone, nil, nil);
		CFReadStreamUnscheduleFromRunLoop(readStream, CFRunLoopGetMain(), kCFRunLoopCommonModes);

        CFRelease(readStream);
        readStream = nil;
    }
	
	if (h3gTimer != nil)
		[h3gTimer invalidate];	
	self.h3gTimer = nil;
	[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
	[writeLock release];
	writeLock = nil;
	[receivedData release];
	receivedData = nil;
	[networkQueue cancelAllOperations];
	[networkQueue release];
	networkQueue = nil;
	if (httpClient == self)
		httpClient = nil;
//	MYLog(@"-----------------%@ dealloc----------------", self);
	[super dealloc];
}

+ (HttpClient*)getInstance
{
	if (httpClient == nil)
	{
		httpClient = [[[HttpClient alloc] init] autorelease];
	}
	return httpClient;
}

- (void)lockStream
{
	if (writeLock != nil)
		[writeLock lock];
}

- (void)unlockStream
{
	if (writeLock != nil)
	{
		[writeLock tryLock];
		[writeLock unlock];	
	}
}

- (void)addHttpRequest:(NSURL*)requstURL method:(NSString*)method headers:(NSDictionary*)requestHeaders bodyString:(NSData*)bodyString type:(int)theType
{
	HttpRequest *request = nil;
	if (theType == ZWP_REQUEST || theType == STANDBY_ZWP_REQUEST)
	{
		for (request in [networkQueue operations])
		{
			if (request.messageType == ZWP_REQUEST ||request.messageType == STANDBY_ZWP_REQUEST)
				[request cancel];
		}
	}

	if(theType == X_H3G_MSISDN)
	{
		if (h3gTimer != nil)
			[h3gTimer invalidate];	
		self.h3gTimer  = [NSTimer scheduledTimerWithTimeInterval:20.0 target:self 
														  selector:@selector(overTimeAlert) 
														  userInfo:nil 
														  repeats:NO];	
	}
	
	if(theType == H3G_WIFI)
	{
		if (h3gTimer != nil)
			[h3gTimer invalidate];	
		self.h3gTimer  = [NSTimer scheduledTimerWithTimeInterval:10.0 target:self 
														selector:@selector(overWIFITimeAlert) 
														userInfo:nil 
														 repeats:NO];	
	}

	requestComplete = NO;
#if 1
	request = [[HttpRequest alloc] init];
	request.requestHeaders = requestHeaders;
	request.bodyString = bodyString;
	request.method = method;
	request.requstURL = requstURL;
	request.client = self;
	request.messageType = theType;
	[networkQueue addOperation:request];
	[request release];
#else
	request	= [HttpRequest requestObject];
	request.requestHeaders = requestHeaders;
	request.bodyString = bodyString;
	request.method = method;
	request.requstURL = requstURL;
	request.client = self;
	request.messageType = theType;
	[networkQueue addOperation:request];
#endif
}

- (void)addHttpRequest:(NSURL*)requstURL method:(NSString*)method headers:(NSDictionary*)requestHeaders bodyString:(NSData*)bodyString
{
	[self addHttpRequest:requstURL method:method headers:requestHeaders bodyString:bodyString type:0];
}

- (void)sendHttpRequest:(NSURL*)requstURL method:(NSString*)method headers:(NSDictionary*)requestHeaders bodyString:(NSData*)bodyString type:(int)theType
{
	[self lockStream];
	[self unlockStream];
	[self performSelectorOnMainThread:@selector(lockStream) withObject:nil waitUntilDone:YES];
	messageType = theType;
	
	if (requstURL != nil && method != nil)
	{
	    CFHTTPMessageRef request = CFHTTPMessageCreateRequest(kCFAllocatorDefault, (CFStringRef)method, (CFURLRef)requstURL, kCFHTTPVersion1_1);
		if (request)
		{
			NSString *header;
			for (header in [requestHeaders allKeys]) 
			{
				CFHTTPMessageSetHeaderFieldValue(request, (CFStringRef)header, (CFStringRef)[requestHeaders objectForKey:header]);
			}
			if (bodyString != nil)
			{
				if ([bodyString length] > 0)
				{
					CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-length"), (CFStringRef)[NSString stringWithFormat:@"%d",[bodyString length]]);
					CFHTTPMessageSetBody(request, (CFDataRef)bodyString);	
				}	
			}
			readStream = CFReadStreamCreateForStreamedHTTPRequest(kCFAllocatorDefault, request,readStream);
			
			if (readStream)
			{
				CFStreamClientContext ctxt = {0, self, nil, nil, nil};
				
				if (CFReadStreamSetClient(readStream, kNetworkEvents, ReadStreamClientCallBack, &ctxt))
				{
					CFReadStreamScheduleWithRunLoop(readStream, CFRunLoopGetMain(), kCFRunLoopCommonModes);	
					if (!CFReadStreamOpen(readStream))
					{
						CFReadStreamSetClient(readStream, 0, nil, nil);
						CFReadStreamUnscheduleFromRunLoop(readStream, CFRunLoopGetMain(), kCFRunLoopCommonModes);
						CFRelease(readStream);
						readStream = nil;
						[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
						MYLog(@"--------------CFReadStreamOpen fail----------");
					}
				}
				else
				{
					CFRelease(readStream);
					readStream = nil;
					[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
					MYLog(@"--------------CFReadStreamSetClient fail----------");	
				}
			}
			else
			{
				MYLog(@"--------------readStream fail----------");	
				[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
			}
			CFRelease(request);
		}
		else
		{
			[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
			MYLog(@"--------------fail----------");				
		}
	}
	else
	{
		[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
	}
}

#pragma mark -
#pragma mark network event handlers.

- (void)handleNetworkEvent:(CFStreamEventType)theType
{
	switch (theType) 
	{
		case kCFStreamEventOpenCompleted:
//			MYLog(@"kCFStreamEventOpenCompleted＋＋＋＋");
            break;
			
        case kCFStreamEventHasBytesAvailable:
            [self handleBytesAvailable];
            break;
            
        case kCFStreamEventEndEncountered:
            [self handleStreamComplete];
            break;
            
        case kCFStreamEventErrorOccurred:
            [self handleStreamError];
            break;
            
        default:
            break;
    }
}

- (void)handleBytesAvailable
{
	UInt8 buffer[2048]={0};
    CFIndex bytesRead = CFReadStreamRead(readStream, buffer, sizeof(buffer));
//    NSLog(@"http client buffer : %s", buffer);
    
    if (bytesRead > 0)
	{
		if (messageType == ZWP_REQUEST || messageType == STANDBY_ZWP_REQUEST)
		{
			NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
			[dict setObject:[NSNumber numberWithInt:messageType] forKey:@"messageType"];
			NSData *data = [NSData dataWithBytes:buffer length:bytesRead];
			[delegate performSelector:callbackFun withObject:data withObject:dict]; 
		}
		else
		{
			if (receivedData == nil)
				receivedData = [[NSMutableData alloc] init];
			[receivedData appendBytes:buffer length:bytesRead];
		}
	}
	else if (bytesRead < 0)
	{
        //[self handleStreamError];
    }
	//else If zero bytes were read, wait for the EOF to come.
}

- (void)handleStreamComplete
{
	if (responseHeaders == nil)
	{
		CFHTTPMessageRef headers = (CFHTTPMessageRef)CFReadStreamCopyProperty(readStream, kCFStreamPropertyHTTPResponseHeader);
		if (headers != nil) 
		{
			if (CFHTTPMessageIsHeaderComplete(headers))
			{
				responseHeaders = (NSDictionary *)CFHTTPMessageCopyAllHeaderFields(headers);
				/*NSString *string = nil;
				 for (string in [responseHeaders allKeys])
				 {
				 MYLog(@"string:%@ :%@",string,[responseHeaders objectForKey:string]);
				 }*/
			}
			CFRelease(headers);
		}
	}
	requestComplete = YES;
	if (readStream)
	{
        CFReadStreamClose(readStream);
        CFReadStreamSetClient(readStream, kCFStreamEventNone, NULL, NULL);
		CFReadStreamUnscheduleFromRunLoop(readStream, CFRunLoopGetMain(), kCFRunLoopCommonModes);
        CFRelease(readStream);
        readStream = nil;
    }
	
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	[dict setObject:[NSNumber numberWithInt:messageType] forKey:@"messageType"];
	[dict setObject:responseHeaders forKey:@"headers"];
	
	[delegate performSelector:callbackFun withObject:receivedData withObject:dict]; 
	[receivedData release];
	receivedData = nil;

	[responseHeaders release];
	responseHeaders = nil;
	if ((messageType == X_H3G_MSISDN) || (messageType == H3G_WIFI))
	{
		[h3gTimer invalidate];
		self.h3gTimer =nil;
	}
	[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
}

- (void)handleStreamError
{	
	NSError *err = [(NSError *)CFReadStreamCopyError(readStream) autorelease];
	if (err && messageType == CLIENT_REQUEST) 
	{
		//NSDictionary *info = [NSDictionary dictionaryWithObject:[err description] forKey:@"message"];
		NSDictionary *info = nil;
		if ([delegate isKindOfClass:[ClientNetWorkController class]])
		{
			info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"pica_str_tip_iap_connect_failed",nil) forKey:@"message"];
			[ClientNetWorkController postNotification: H3GLoginFaildNotification info:info];
		}
		else
		{
			info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"pica_str_tip_disconnected_unknown_reason",nil) forKey:@"message"];	
			[ClientNetWorkController postNotification:NetWorkDisabledNotification info:info];
		}
	}
	else if (err && messageType == X_H3G_MSISDN) 
	{
		[h3gTimer invalidate];
		self.h3gTimer =nil;
		NSDictionary *info = nil;
		if ([delegate isKindOfClass:[ClientNetWorkController class]])
		{
			info = [NSDictionary dictionaryWithObject: NSLocalizedString(@"This service is applicable to 3-3G customers only. If you are connecting by WIFI, please login by 3-3G first on the same day.",nil)
                                               forKey: @"message"];
			[ClientNetWorkController postNotification: H3GLoginFaildNotification info:info];
		}
		else
		{
			info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"pica_str_tip_disconnected_unknown_reason",nil) forKey:@"message"];	
			[ClientNetWorkController postNotification:NetWorkDisabledNotification info:info];
		}
	}
	else if ((err && messageType == H3G_WIFI))
	{
		[h3gTimer invalidate];
		NSDictionary *info = nil;
		info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"Connection timeout.",nil) forKey:@"message"];
		[ClientNetWorkController postNotification: H3GWIFIFaildNotification info: info];
		self.h3gTimer = nil;
		
	}
	MYLog(@"err:%@",[err localizedDescription]);
	if (readStream)
	{
        CFReadStreamClose(readStream);
        CFReadStreamSetClient(readStream, kCFStreamEventNone, NULL, NULL);
		CFReadStreamUnscheduleFromRunLoop(readStream, CFRunLoopGetMain(), kCFRunLoopCommonModes);
        CFRelease(readStream);
        readStream = nil;
    }
	[self performSelectorOnMainThread:@selector(unlockStream) withObject:nil waitUntilDone:YES];
}

- (void)overTimeAlert
{
	NSDictionary *info = nil;
	info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"Connection timeout.",nil) forKey:@"message"];
	[ClientNetWorkController postNotification: H3GLoginFaildNotification info: info];
	self.h3gTimer = nil;
}

- (void)overWIFITimeAlert
{
	NSDictionary *info = nil;
	info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"Connection timeout.",nil) forKey:@"message"];
	[ClientNetWorkController postNotification: H3GWIFIFaildNotification info: info];
	self.h3gTimer = nil;
}
@end

