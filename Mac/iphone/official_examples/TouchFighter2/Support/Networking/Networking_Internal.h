/*
File: Networking_Internal.h

Version: 1.3
*/

#import "TCPConnection.h"

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

//PROTOTYPES:

extern NSString* _MakeServiceType(NSString* applicationProtocol, NSString* transportProtocol);

//CLASS INTERFACES:

@interface TCPConnection ()
@property(readonly) CFRunLoopRef CFRunLoop;
- (void) _invalidate; //For subclasses
@end
