/*
<codex>
<version>1.1</version>
</codex>
*/

#if !TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif

#import "GamePeer.h"

//MACROS:

/* Generic error reporting */
#define REPORT_ERROR(__FORMAT__, ...) printf("%s: %s\n", __FUNCTION__, [[NSString stringWithFormat:__FORMAT__, __VA_ARGS__] UTF8String])

/* Optional delegate methods support */
#ifndef __DELEGATE_IVAR__
#define __DELEGATE_IVAR__ _delegate
#endif
#ifndef __DELEGATE_METHODS_IVAR__
#define __DELEGATE_METHODS_IVAR__ _delegateMethods
#endif
#define TEST_DELEGATE_METHOD_BIT(__BIT__) (self->__DELEGATE_METHODS_IVAR__ & (1 << __BIT__))
#define SET_DELEGATE_METHOD_BIT(__BIT__, __NAME__) { if([self->__DELEGATE_IVAR__ respondsToSelector:@selector(__NAME__)]) self->__DELEGATE_METHODS_IVAR__ |= (1 << __BIT__); else self->__DELEGATE_METHODS_IVAR__ &= ~(1 << __BIT__); }

//PROTOCOLS:

@protocol GamePeerDelegate <NSObject>
- (NSString*) gamePeerWillSendName:(GamePeer*)peer;
- (id) gamePeerWillSendInfoPlist:(GamePeer*)peer;
- (void) gamePeerDidFailConnecting:(GamePeer*)peer;
- (void) gamePeerDidConnect:(GamePeer*)peer;
- (void) gamePeerDidDisconnect:(GamePeer*)peer;
- (void) gamePeer:(GamePeer*)peer didReceiveData:(NSData*)data immediate:(BOOL)immediate;
@end

//PROTOTYPES:

extern NSString* _ApplicationProtocolFromGameIdentifier(NSString* string);

//CLASS INTERFACES:

@interface GamePeer ()
@property(assign) id<GamePeerDelegate> delegate;
@property(readonly) BOOL service;
@property(readonly) const struct sockaddr* socketAddress;

- (id) initWithCFNetService:(CFNetServiceRef)netService;
- (id) initWithName:(NSString*)name address:(const struct sockaddr*)address;
- (id) initWithConnection:(TCPConnection*)connection;
- (BOOL) connect;
- (BOOL) sendData:(NSData*)data immediate:(BOOL)immediate;
- (void) disconnect;
- (NSTimeInterval) measureRoundTripLatency;
@end
