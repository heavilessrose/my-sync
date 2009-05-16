/*
File: NetServiceBrowser.h
Abstract: Convenience class that wraps the CFNetServiceBrowser APIs from CFNetwork and allows to discover Bonjour services of a given type on the local network.

Version: 1.3
*/

#import <Foundation/Foundation.h>
#if !TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif

//CLASSES:

@class NetServiceBrowser;

//PROTOCOLS:

@protocol NetServiceBrowserDelegate <NSObject>
@optional
- (void) browserDidStart:(NetServiceBrowser*)browser;

- (void) browser:(NetServiceBrowser*)browser didAddService:(CFNetServiceRef)service;
- (void) browser:(NetServiceBrowser*)browser didRemoveService:(CFNetServiceRef)service;

- (void) browserWillStop:(NetServiceBrowser*)browser;
@end

//CLASS INTERFACES:

/*
This class wraps the CFNetServiceBrowser APIs from CFNetwork and allows to discover Bonjour services of a given type on the local network.
*/
@interface NetServiceBrowser : NSObject
{
@private
	NSString*							_domain;
	NSString*							_type;
	id<NetServiceBrowserDelegate>		_delegate;
	NSUInteger							_delegateMethods;
	
	CFRunLoopRef						_runLoop;
	CFNetServiceBrowserRef				_netBrowser;
	BOOL								_running;
}
- (id) initWithDomain:(NSString*)domain type:(NSString*)type; //Pass "nil" for the default local domain - For type, you must pass a fully-formed Bonjour type e.g. "_myApp._tcp."
- (id) initTCPBrowserWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol; //Assumes a TCP transport protocol - Pass "nil" for the default local domain - Pass only the application protocol for "protocol" e.g. "myApp"
- (id) initUDPBrowserWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol; //Assumes an UDP transport protocol - Pass "nil" for the default local domain - Pass only the application protocol for "protocol" e.g. "myApp"

@property(assign) id<NetServiceBrowserDelegate> delegate;

- (BOOL) startUsingRunLoop:(NSRunLoop*)runLoop;
@property(readonly, getter=isRunning) BOOL running;
- (void) stop;
@end
