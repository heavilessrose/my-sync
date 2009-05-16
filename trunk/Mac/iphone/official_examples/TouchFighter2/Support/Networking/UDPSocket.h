/*
File: UDPSocket.h
Abstract: Convenience class that acts as a controller for UDP based network connections.

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

@class UDPSocket;

//PROTOCOLS:

@protocol UDPSocketDelegate <NSObject>
@optional
- (void) socketDidEnableBonjour:(UDPSocket*)socket;
- (void) socketWillDisableBonjour:(UDPSocket*)socket;

- (void) socketDidInvalidate:(UDPSocket*)socket;
- (void) socket:(UDPSocket*)socket didReceiveData:(NSData*)data fromRemoteAddress:(const struct sockaddr*)address;
@end

//CLASS INTERFACES:

/*
This class acts as a controller for UDP based network connections.
The UDPSocket instance will use the current runloop at its time of creation.
You can also enable Bonjour advertising for the listening UDP port.
*/
@interface UDPSocket : NSObject
{
@private
	CFSocketRef					_socket;
	CFRunLoopRef				_runLoop;
	id<UDPSocketDelegate>		_delegate;
	NSUInteger					_delegateMethods;
	
	CFNetServiceRef				_netService;
	struct sockaddr*			_localAddress;
	BOOL						_invalidating;
}
- (id) initWithPort:(UInt16)port; //Pass 0 to have a port automatically be chosen

@property(assign) id<UDPSocketDelegate> delegate;

@property(readonly, getter=isValid) BOOL valid;
- (void) invalidate; //Close the connection

@property(readonly) UInt16 localPort;
@property(readonly) UInt32 localIPv4Address; //The returned address is in host-endian
@property(readonly) NSString* localAddress;

- (BOOL) enableBonjourWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol name:(NSString*)name; //Pass "nil" for the default local domain - Pass only the application protocol for "protocol" e.g. "myApp"
@property(readonly, getter=isBonjourEnabled) BOOL bonjourEnabled;
- (void) disableBonjour;

- (BOOL) sendData:(NSData*)data toRemoteAddress:(const struct sockaddr*)address; //Blocking - Must be called from same thread the connection was created on
- (BOOL) sendData:(NSData*)data toRemoteIPv4Address:(UInt32)address port:(UInt16)port; //Blocking - Must be called from same thread the connection was created on - The "address" is assumed to be in host-endian
#ifndef __CODEX__
#if !TARGET_OS_IPHONE
- (BOOL) sendData:(NSData*)data toRemoteIPv6Address:(const struct in6_addr*)address port:(UInt16)port; //Blocking - Must be called from same thread the connection was created on
#endif
#endif
@end

/*
This subclass of UDPSocket uses UDP broadcasting to send data.
*/
@interface UDPBroadcastSocket : UDPSocket
- (BOOL) sendData:(NSData*)data toPort:(UInt16)port;
@end
