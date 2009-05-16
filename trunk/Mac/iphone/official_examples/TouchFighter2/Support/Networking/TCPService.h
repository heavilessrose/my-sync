/*
File: TCPService.h
Abstract: Convenience class that acts as a controller for a listening TCP port that accepts incoming connections.

Version: 1.3
*/

#import <Foundation/Foundation.h>
#if !TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif
#import <sys/socket.h>

//CLASS INTERFACES:

/*
This class acts a controller for a listening TCP port that accepts incoming connections.
You must subclass this class and override the -handleNewConnectionWithSocket:fromRemoteAddress: method.
You can also enable Bonjour advertising for the listening TCP port.
*/
@interface TCPService : NSObject
{
@private
	UInt16				_port;
	
	CFRunLoopRef		_runLoop;
	CFSocketRef			_ipv4Socket;
#ifndef __CODEX__
#if !TARGET_OS_IPHONE
	CFSocketRef			_ipv6Socket;
#endif
#endif
	CFNetServiceRef		_netService;
	BOOL				_running;
	struct sockaddr*	_localAddress;
}
- (id) initWithPort:(UInt16)port; //Pass 0 to have a port automatically be chosen

- (BOOL) startUsingRunLoop:(NSRunLoop*)runLoop;
@property(readonly, getter=isRunning) BOOL running;
- (void) stop;

@property(readonly) UInt16 localPort; //Only valid when running
@property(readonly) UInt32 localIPv4Address; //Only valid when running - The returned address is in host-endian
@property(readonly) NSString* localAddress; //Only valid when running

- (BOOL) enableBonjourWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol name:(NSString*)name; //Pass "nil" for the default local domain - Pass only the application protocol for "protocol" e.g. "myApp"
@property(readonly, getter=isBonjourEnabled) BOOL bonjourEnabled;
- (void) disableBonjour;

- (void) handleNewConnectionWithSocket:(NSSocketNativeHandle)socket fromRemoteAddress:(const struct sockaddr*)address; //To be implemented by subclasses
@end
