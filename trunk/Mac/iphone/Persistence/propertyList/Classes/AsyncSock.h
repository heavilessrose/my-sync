//
//  AsyncSock.h
//  propertyList
//
//  Created by wang luke on 6/19/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>
//#import <CoreFoundation/CFStream.h>

@interface NSObject (AsyncSockDelegate)

@end


@interface AsyncSock : NSObject {
	CFSocketRef _sock;
	CFSocketContext _context;
	CFRunLoopRef _runLoop;
	// Default run loop modes
	NSArray* _runLoopModes;
	
	CFReadStreamRef _readStream;
	CFWriteStreamRef _writeStream;
}
- (CFSocketRef)createSocket:(CFOptionFlags)callbackTypes;
- (void)attachSockToRunloop;
- (void)doCFSocketCallback:(CFSocketCallBackType)type forSocket:(CFSocketRef)sock
			   withAddress:(NSData *)address withData:(const void *)data;


- (NSData*)getRemoteAddr:(NSString *)hostname port:(UInt16)port;
- (BOOL)createStreamsToHost:(NSString *)hostname onPort:(UInt16)port error:(NSError **)errPtr;
- (void)test;

// not for AsyncSock_bak.m
- (BOOL)doCreateStreamsFromNative:(CFSocketNativeHandle)native error:(NSError **)errPtr;
- (BOOL)createStreamsToHost:(NSString *)hostname onPort:(UInt16)port error:(NSError **)errPtr;
- (BOOL)openStreamsAndReturnError:(NSError **)errPtr;
- (void)doCFReadStreamCallback:(CFStreamEventType)type forStream:(CFReadStreamRef)stream;
- (void)doCFWriteStreamCallback:(CFStreamEventType)type forStream:(CFWriteStreamRef)stream;
- (void)closeWithError:(NSError *)err;
- (BOOL)attachStreamsToRunLoop:(NSRunLoop *)runLoop error:(NSError **)errPtr;
- (void)doSendBytes;
- (void)doBytesAvailable;
@end
