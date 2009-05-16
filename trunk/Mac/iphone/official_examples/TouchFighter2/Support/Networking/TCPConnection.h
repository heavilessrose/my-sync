/*
File: TCPConnection.h
Abstract: Convenience class that acts as a controller for TCP based network connections.

Version: 1.3
*/

#import <Foundation/Foundation.h>
#if !TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif
#import <sys/socket.h>

//CLASSES:

@class TCPConnection;

//PROTOCOLS:

@protocol TCPConnectionDelegate <NSObject>
@optional
- (void) connectionDidFailOpening:(TCPConnection*)connection;
- (void) connectionDidOpen:(TCPConnection*)connection;
- (void) connectionDidClose:(TCPConnection*)connection;

- (void) connection:(TCPConnection*)connection didReceiveData:(NSData*)data;
@end

//CLASS INTERFACES:

/*
This class acts as a controller for TCP based network connections.
The TCPConnection instance will use the current runloop at its time of creation.
*/
@interface TCPConnection : NSObject
{
@private
	CFReadStreamRef				_inputStream;
	CFWriteStreamRef			_outputStream;
	CFRunLoopRef				_runLoop;
	id<TCPConnectionDelegate>	_delegate;
	NSUInteger					_delegateMethods;
	NSUInteger					_opened;
	struct sockaddr*			_localAddress;
	struct sockaddr*			_remoteAddress;
	BOOL						_invalidating;
}
- (id) initWithSocketHandle:(int)socket; //Acquires ownership of the socket
- (id) initWithRemoteAddress:(const struct sockaddr*)address;
- (id) initWithRemoteIPv4Address:(UInt32)address port:(UInt16)port; //The "address" is assumed to be in host-endian
#ifndef __CODEX__
#if !TARGET_OS_IPHONE
- (id) initWithRemoteIPv6Address:(const struct in6_addr*)address port:(UInt16)port;
#endif
#endif
- (id) initWithRemoteHostName:(NSString*)name port:(UInt16)port;
- (id) initWithCFNetService:(CFNetServiceRef)service timeOut:(NSTimeInterval)timeOut; //Pass a negative value if you don't want to attempt to resolve the CFNetService, or zero for an infinite timeout
- (id) initWithServiceDomain:(NSString*)domain type:(NSString*)type name:(NSString*)name timeOut:(NSTimeInterval)timeOut; //Pass a negative value if you don't want to attempt to resolve the CFNetService, or zero for an infinite timeout

@property(assign) id<TCPConnectionDelegate> delegate;

@property(readonly, getter=isValid) BOOL valid;
- (void) invalidate; //Close the connection

@property(readonly) UInt16 localPort;
@property(readonly) UInt32 localIPv4Address; //The returned address is in host-endian
@property(readonly) NSString* localAddress;
@property(readonly) UInt16 remotePort;
@property(readonly) UInt32 remoteIPv4Address; //The returned address is in host-endian
@property(readonly) NSString* remoteAddress;

@property(readonly) const struct sockaddr* remoteSocketAddress;

- (BOOL) sendData:(NSData*)data; //Blocking - Must be called from same thread the connection was created on
- (BOOL) hasDataAvailable; //Non-blocking - Must be called from same thread the connection was created on
- (NSData*) receiveData; //Blocking - Must be called from same thread the connection was created on
@end
