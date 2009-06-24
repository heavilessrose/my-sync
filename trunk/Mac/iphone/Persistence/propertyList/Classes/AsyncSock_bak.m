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
#import "propertyListViewController.h"

//@class propertyListViewController;

@implementation AsyncSock

- (id)init{
	if((self = [super init])){
		_runLoopModes = [[NSArray arrayWithObject:NSDefaultRunLoopMode] retain];
		
		_context.version = 0;
		_context.info = self;
		_context.retain = nil;
		_context.release = nil;
		_context.copyDescription = nil;
	}
	return self;
}

- (void)dealloc{
	[_runLoopModes release];
	[NSObject cancelPreviousPerformRequestsWithTarget:self];
	[super dealloc];
}

- (void)test{
	_sock = [self createSocket:kCFSocketConnectCallBack|kCFSocketDataCallBack|kCFSocketReadCallBack|kCFSocketWriteCallBack|kCFSocketNoCallBack];
	[self attachSockToRunloop];
	CFDataRef remoteAddr = (CFDataRef)[self getRemoteAddr:@"online.sccnn.com" port:80];
	//CFDataRef remoteAddr = (CFDataRef)[self getRemoteAddr:@"127.0.0.1" port:9034];
	CFSocketConnectToAddress(_sock, remoteAddr, -1);
}

#pragma mark -
#pragma mark utils


- (NSError *)errorFromCFStreamError:(CFStreamError)err
{
	if (err.domain == 0 && err.error == 0) return nil;
	
	// Can't use switch; these constants aren't int literals.
	NSString *domain = @"CFStreamError (unlisted domain)";
	NSString *message = nil;
	
	if(err.domain == kCFStreamErrorDomainPOSIX) {
		domain = NSPOSIXErrorDomain;
	}
	else if(err.domain == kCFStreamErrorDomainMacOSStatus) {
		domain = NSOSStatusErrorDomain;
	}
	else if(err.domain == kCFStreamErrorDomainMach) {
		domain = NSMachErrorDomain;
	}
	else if(err.domain == kCFStreamErrorDomainNetDB)
	{
		domain = @"kCFStreamErrorDomainNetDB";
		message = [NSString stringWithCString:gai_strerror(err.error) encoding:NSASCIIStringEncoding];
	}
	else if(err.domain == kCFStreamErrorDomainNetServices) {
		domain = @"kCFStreamErrorDomainNetServices";
	}
	else if(err.domain == kCFStreamErrorDomainSOCKS) {
		domain = @"kCFStreamErrorDomainSOCKS";
	}
	else if(err.domain == kCFStreamErrorDomainSystemConfiguration) {
		domain = @"kCFStreamErrorDomainSystemConfiguration";
	}
	else if(err.domain == kCFStreamErrorDomainSSL) {
		domain = @"kCFStreamErrorDomainSSL";
	}
	
	NSDictionary *info = nil;
	if(message != nil)
	{
		info = [NSDictionary dictionaryWithObject:message forKey:NSLocalizedDescriptionKey];
	}
	return [NSError errorWithDomain:domain code:err.error userInfo:info];
}

- (NSError *)getStreamError
{
	CFStreamError err;
	if (_readStream != NULL)
	{
		err = CFReadStreamGetError (_readStream);
		if (err.error != 0) return [self errorFromCFStreamError: err];
	}
	
	if (_writeStream != NULL)
	{
		err = CFWriteStreamGetError (_writeStream);
		if (err.error != 0) return [self errorFromCFStreamError: err];
	}
	
	return nil;
}

/**
 * Creates the CFReadStream and CFWriteStream from the given hostname and port number.
 * The CFSocket may be extracted from either stream after the streams have been opened.
 **/
//- (BOOL)createStreamsToHost:(NSString *)hostname onPort:(UInt16)port error:(NSError **)errPtr
//{
//	// Create the socket & streams.
//	CFStreamCreatePairWithSocketToHost(kCFAllocatorDefault, (CFStringRef)hostname, port, &_readStream, &_writeStream);
//	if (_readStream == NULL || _writeStream == NULL)
//	{
//		if (errPtr) *errPtr = [self getStreamError];
//		return NO;
//	}
//	
//	// Ensure the CF & BSD socket is closed when the streams are closed.
//	CFReadStreamSetProperty(_readStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue);
//	CFWriteStreamSetProperty(_writeStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue);
//	
//	return YES;
//}

- (NSData*)getRemoteAddr:(NSString *)hostname port:(UInt16)port{
	struct addrinfo hints, *servinfo;
	int rv;
	char HOST[256];
	sprintf(HOST, "%s", [hostname UTF8String]);
	char PORT[5];
	sprintf(PORT, "%d", port);
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	// 取得服务器地址信息（struct addrinfo）
	if ((rv = getaddrinfo(/* argv[1] */ HOST, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and connect to the first we can
	NSData *addr = [NSData dataWithBytes:servinfo->ai_addr length:servinfo->ai_addrlen];
	
	freeaddrinfo(servinfo);
	return addr;
}

#pragma mark -
#pragma mark 回调
//- (void)doCFReadStreamCallback:(CFStreamEventType)type forStream:(CFReadStreamRef)stream
//{
//	NSParameterAssert(theReadStream != NULL);
//	
//	CFStreamError err;
//	switch (type)
//	{
//		case kCFStreamEventOpenCompleted:
//			theFlags |= kDidCompleteOpenForRead;
//			[self doStreamOpen];
//			break;
//		case kCFStreamEventHasBytesAvailable:
//			if(theFlags & kStartingTLS)
//				[self onTLSStarted:YES];
//			else
//				[self doBytesAvailable];
//			break;
//		case kCFStreamEventErrorOccurred:
//		case kCFStreamEventEndEncountered:
//			err = CFReadStreamGetError (theReadStream);
//			[self closeWithError: [self errorFromCFStreamError:err]];
//			break;
//		default:
//			NSLog (@"AsyncSocket %p received unexpected CFReadStream callback, CFStreamEventType %d.", self, type);
//	}
//}
//
//- (void)doCFWriteStreamCallback:(CFStreamEventType)type forStream:(CFWriteStreamRef)stream
//{
//	NSParameterAssert(theWriteStream != NULL);
//	
//	CFStreamError err;
//	switch (type)
//	{
//		case kCFStreamEventOpenCompleted:
//			theFlags |= kDidCompleteOpenForWrite;
//			[self doStreamOpen];
//			break;
//		case kCFStreamEventCanAcceptBytes:
//			if(theFlags & kStartingTLS)
//				[self onTLSStarted:YES];
//			else
//				[self doSendBytes];
//			break;
//		case kCFStreamEventErrorOccurred:
//		case kCFStreamEventEndEncountered:
//			err = CFWriteStreamGetError (theWriteStream);
//			[self closeWithError: [self errorFromCFStreamError:err]];
//			break;
//		default:
//			NSLog (@"AsyncSocket %p received unexpected CFWriteStream callback, CFStreamEventType %d.", self, type);
//	}
//}
//
///**
// * This is the callback we setup for CFReadStream.
// * This method does nothing but forward the call to it's Objective-C counterpart
// **/
//static void MyCFReadStreamCallback (CFReadStreamRef stream, CFStreamEventType type, void *pInfo)
//{
//	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//	
//	AsyncSock *theSocket = [[(AsyncSock *)pInfo retain] autorelease];
//	[theSocket doCFReadStreamCallback:type forStream:stream];
//	
//	[pool release];
//}
//
///**
// * This is the callback we setup for CFWriteStream.
// * This method does nothing but forward the call to it's Objective-C counterpart
// **/
//static void MyCFWriteStreamCallback (CFWriteStreamRef stream, CFStreamEventType type, void *pInfo)
//{
//	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//	
//	AsyncSock *theSocket = [[(AsyncSock *)pInfo retain] autorelease];
//	[theSocket doCFWriteStreamCallback:type forStream:stream];
//	
//	[pool release];
//}
//
///**
// * Creates the CFReadStream and CFWriteStream from the given native socket.
// * The CFSocket may be extracted from either stream after the streams have been opened.
// * 
// * Note: The given native socket must already be connected!
// **/
//- (BOOL)createStreamsFromNative:(CFSocketNativeHandle)native error:(NSError **)errPtr
//{
//	// Create the socket & streams.
//	CFStreamCreatePairWithSocket(kCFAllocatorDefault, native, &_readStream, &_writeStream);
//	if (_readStream == NULL || _writeStream == NULL)
//	{
//		//NSError *err = [self getStreamError];
//		
//		//NSLog (@"AsyncSocket %p couldn't create streams from accepted socket: %@", self, err);
//		
//		//if (errPtr) *errPtr = err;
//		return NO;
//	}
//	
//	// Ensure the CF & BSD socket is closed when the streams are closed.
//	CFReadStreamSetProperty(_readStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue);
//	CFWriteStreamSetProperty(_writeStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue);
//	
//	return YES;
//}
//
//- (BOOL)attachStreamsToRunLoop:(NSRunLoop *)runLoop error:(NSError **)errPtr
//{
//	// Get the CFRunLoop to which the socket should be attached.
//	_runLoop = (runLoop == nil) ? CFRunLoopGetCurrent() : [runLoop getCFRunLoop];
//	
//	// Setup read stream callbacks
//	
//	CFOptionFlags readStreamEvents = kCFStreamEventHasBytesAvailable | 
//	kCFStreamEventErrorOccurred     |
//	kCFStreamEventEndEncountered    |
//	kCFStreamEventOpenCompleted;
//	
//	if (!CFReadStreamSetClient(_readStream,
//							   readStreamEvents,
//							   (CFReadStreamClientCallBack)&MyCFReadStreamCallback,
//							   (CFStreamClientContext *)(&_context)))
//	{
//		//NSError *err = [self getStreamError];
//		
//		NSLog (@"AsyncSocket %p couldn't attach read stream to run-loop,", self);
//		//NSLog (@"Error: %@", err);
//		
//		//if (errPtr) *errPtr = err;
//		return NO;
//	}
//	
//	// Setup write stream callbacks
//	
//	CFOptionFlags writeStreamEvents = kCFStreamEventCanAcceptBytes |
//	kCFStreamEventErrorOccurred  |
//	kCFStreamEventEndEncountered |
//	kCFStreamEventOpenCompleted;
//	
//	if (!CFWriteStreamSetClient (_writeStream,
//								 writeStreamEvents,
//								 (CFWriteStreamClientCallBack)&MyCFWriteStreamCallback,
//								 (CFStreamClientContext *)(&_context)))
//	{
//		//NSError *err = [self getStreamError];
//		
//		NSLog (@"AsyncSocket %p couldn't attach write stream to run-loop,", self);
//		//NSLog (@"Error: %@", err);
//		
//		//if (errPtr) *errPtr = err;
//		return NO;
//	}
//	
//	// Add read and write streams to run loop
//	
//	unsigned i, count = [_runLoopModes count];
//	for(i = 0; i < count; i++)
//	{
//		CFStringRef runLoopMode = (CFStringRef)[_runLoopModes objectAtIndex:i];
//		CFReadStreamScheduleWithRunLoop(_readStream, _runLoop, runLoopMode);
//		CFWriteStreamScheduleWithRunLoop(_writeStream, _runLoop, runLoopMode);
//	}
//	
//	return YES;
//}
//
//- (BOOL)openStreamsAndReturnError:(NSError **)errPtr
//{
//	BOOL pass = YES;
//	
//	if(pass && !CFReadStreamOpen (_readStream))
//	{
//		NSLog (@"AsyncSocket %p couldn't open read stream,", self);
//		pass = NO;
//	}
//	
//	if(pass && !CFWriteStreamOpen (_writeStream))
//	{
//		NSLog (@"AsyncSocket %p couldn't open write stream,", self);
//		pass = NO;
//	}
//	
//	if(!pass)
//	{
//		if (errPtr) *errPtr = [self getStreamError];
//	}
//	
//	return pass;
//}
//
//- (void)doSocketOpen:(CFSocketRef)sock withCFSocketError:(CFSocketError)socketError
//{
//	//NSParameterAssert ((sock == theSocket4) || (sock == theSocket6));
//	
//	if(socketError == kCFSocketTimeout || socketError == kCFSocketError)
//	{
//		NSLog(@"doSocketOpen error: %d", socketError);
//		//[self closeWithError:[self getSocketError]];
//		return;
//	}
//	
//	// Get the underlying native (BSD) socket
//	CFSocketNativeHandle nativeSocket = CFSocketGetNative(sock);
//	
//	// Setup the socket so that invalidating the socket will not close the native socket
//	CFSocketSetSocketFlags(sock, 0);
//	
//	// Invalidate and release the CFSocket - All we need from here on out is the nativeSocket
//	// Note: If we don't invalidate the socket (leaving the native socket open)
//	// then theReadStream and theWriteStream won't function properly.
//	// Specifically, their callbacks won't work, with the exception of kCFStreamEventOpenCompleted.
//	// I'm not entirely sure why this is, but I'm guessing that events on the socket fire to the CFSocket we created,
//	// as opposed to the CFReadStream/CFWriteStream.
//	
//	CFSocketInvalidate(sock);
//	CFRelease(sock);
//	
//	NSError *err;
//	BOOL pass = YES;
//	
//	if(pass && ![self createStreamsFromNative:nativeSocket error:&err]) pass = NO;
//	if(pass && ![self attachStreamsToRunLoop:nil error:&err])           pass = NO;
//	if(pass && ![self openStreamsAndReturnError:&err])                  pass = NO;
//	
//	if(!pass)
//	{
//		//[self closeWithError:err];
//	}
//}

// 测试： 打开流
- (BOOL)openStreams{
	BOOL pass = YES;
	if(pass && !CFReadStreamOpen (_readStream)){
		NSLog (@"AsyncSock %p couldn't open read stream,", self);
		pass = NO;
	}		
	if(pass && !CFWriteStreamOpen (_writeStream)){
		NSLog (@"AsyncSocket %p couldn't open write stream,", self);
		pass = NO;
	}
	
//	if(!pass){
//		if (errPtr) *errPtr = [self getStreamError];
//	}
	
	return pass;
}

static int i = 0;
- (void)doCFSocketCallback:(CFSocketCallBackType)type forSocket:(CFSocketRef)sock
			   withAddress:(NSData *)address withData:(const void *)data
{
	//NSParameterAssert ((sock == theSocket4) || (sock == theSocket6));
	switch (type)
	{
		// the connect finishes
		case kCFSocketConnectCallBack: 
			NSLog(@"doCFSocketCallback: connected, ready to open streams");
			
			//char dd[1024] = "lalala";
			char dd[1024];
			//???: 请求或响应以必须以两个\n结束？
			sprintf(dd, "GET /%s HTTP/1.1\r\nAccept: */*\r\nHost: %s:%d\r\nConnection: Close\r\n\r\n","gif8/021/gif0405_001.gif","online.sccnn.com",80);
			
			NSData* send = [NSData dataWithBytes:dd length:strlen(dd)];
			
			//???: 这个方法是UDP还是TCP？ 用它不用打开流？
			if(CFSocketSendData(_sock, (CFDataRef)address, (CFDataRef)send, 10) != kCFSocketSuccess){
				NSLog(@"send error");
			}else{
				NSLog(@"send success");
			}
			// The data argument is either NULL or a pointer to an SInt32 error code, if the connect failed.
			//if(data)
//				[self doSocketOpen:sock withCFSocketError:kCFSocketError];
//			else
//				[self doSocketOpen:sock withCFSocketError:kCFSocketSuccess];
			break;
		//???: 服务器写到客户端, 会截断数据？还是说 缓冲很小？
		case kCFSocketDataCallBack:
			NSLog(@"doCFSocketCallback: Incoming data ready");
			NSLog(@"incoming data: %s", [(NSData*)data bytes]);
			if(i == 0){
				i++;
				//???: 此方法不是以append方式打开文件
				[(NSData*)data writeToFile:[[propertyListViewController appDocumentsDir] stringByAppendingPathComponent:@"dd.gif"] atomically:YES];
			}
			break;
		//called when the socket is writable
		case kCFSocketWriteCallBack:
			NSLog(@"doCFSocketCallback: socket is writable");
			
			//if(CFSocketSendData(_sock, (CFDataRef)address, (CFDataRef)send, 10) != kCFSocketSuccess){
//				NSLog(@"send error");
//			}else{
//				NSLog(@"send success");
//			}
			break;
		//called when data is available to be read or a new connection is waiting to be accepted
		case kCFSocketReadCallBack:
			NSLog(@"doCFSocketCallback: kCFSocketReadCallBack");
			
			break;
		case kCFSocketNoCallBack:
			NSLog(@"doCFSocketCallback: kCFSocketNoCallBack");
			
			break;
//		case kCFSocketAcceptCallBack:
//			[self doAcceptWithSocket: *((CFSocketNativeHandle *)pData)];
//			break;
		default:
			NSLog (@"AsyncSocket %p received unexpected CFSocketCallBackType %d.", self, type);
			break;
	}
}

/**
 * This is the callback we setup for CFSocket.
 * This method does nothing but forward the call to it's Objective-C counterpart
 **/
static void MyCFSocketCallback (CFSocketRef sref, CFSocketCallBackType callbackType, CFDataRef address, 
								const void *data, void *info)
{
	NSLog(@"MyCFSocketCallback: %d", (int)callbackType);
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	AsyncSock *theSocket = [[(AsyncSock *)info retain] autorelease];
	[theSocket doCFSocketCallback:callbackType forSocket:sref withAddress:(NSData *)address withData:data];
	
	[pool release];
}

#pragma mark -
#pragma mark 创建socket并加入runloop

- (CFSocketRef)createSocket:(CFOptionFlags)callbackTypes{
	CFSocketRef sock = CFSocketCreate(kCFAllocatorDefault, PF_INET, SOCK_STREAM, IPPROTO_TCP, 
									  callbackTypes, (CFSocketCallBack)&MyCFSocketCallback, &_context);
	return sock;
}

- (void)attachSockToRunloop{
	CFRunLoopSourceRef source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, _sock, 0);
	_runLoop = (_runLoop == nil) ? CFRunLoopGetCurrent() : _runLoop;
	CFRunLoopAddSource (_runLoop, source, kCFRunLoopCommonModes);
}

@end
