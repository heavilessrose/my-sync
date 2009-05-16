/*
File: NetReachability.m
Abstract: Convenience class that wraps the SCNetworkReachability APIs from SystemConfiguration.

Version: 1.3
*/

#import <SystemConfiguration/SCNetworkReachability.h>
#import <netinet/in.h>

#import "NetReachability.h"
#import "Networking_Internal.h"

//MACROS:

#if TARGET_OS_IPHONE
#define IS_REACHABLE(__FLAGS__) (((__FLAGS__) & kSCNetworkReachabilityFlagsReachable) && !((__FLAGS__) & kSCNetworkReachabilityFlagsConnectionRequired))
#if TARGET_IPHONE_SIMULATOR
#define IS_CELL(__FLAGS__) (0)
#else
#define IS_CELL(__FLAGS__) (IS_REACHABLE(__FLAGS__) && ((__FLAGS__) & kSCNetworkReachabilityFlagsIsWWAN))
#endif
#else
#define IS_REACHABLE(__FLAGS__) (((__FLAGS__) & kSCNetworkFlagsReachable) && !((__FLAGS__) & kSCNetworkFlagsConnectionRequired))
#define IS_CELL(__FLAGS__) (0)
#endif

//CLASS IMPLEMENTATION:

@implementation NetReachability

static void _ReachabilityCallBack(SCNetworkReachabilityRef target, SCNetworkConnectionFlags flags, void* info)
{
	NSAutoreleasePool*		pool = [NSAutoreleasePool new];
	NetReachability*		self = (NetReachability*)info;
	
	[self->_delegate reachabilityDidUpdate:self reachable:(IS_REACHABLE(flags) ? YES : NO) usingCell:(IS_CELL(flags) ? YES : NO)];
	
	[pool release];
}

@synthesize delegate=_delegate;

/*
This will consume a reference of "reachability"
*/
- (id) _initWithNetworkReachability:(SCNetworkReachabilityRef)reachability
{
	if(reachability == NULL) {
		[self release];
		return nil;
	}
	
	if((self = [super init])) {
		_runLoop = (CFRunLoopRef)CFRetain(CFRunLoopGetCurrent());
		_netReachability = (void*)reachability;
	}
	
	return self;
}

- (id) initWithDefaultRoute:(BOOL)ignoresAdHocWiFi
{
	return [self initWithIPv4Address:(ignoresAdHocWiFi ? INADDR_ANY : IN_LINKLOCALNETNUM)];
}

- (id) initWithAddress:(const struct sockaddr*)address
{
	return [self _initWithNetworkReachability:(address ? SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, address) : NULL)];
}

- (id) initWithIPv4Address:(UInt32)address
{
	struct sockaddr_in				ipAddress;
	
	bzero(&ipAddress, sizeof(ipAddress));
	ipAddress.sin_len = sizeof(ipAddress);
	ipAddress.sin_family = AF_INET;
	ipAddress.sin_addr.s_addr = htonl(address);
	
	return [self initWithAddress:(struct sockaddr*)&ipAddress];
}

- (id) initWithHostName:(NSString*)name
{
	return [self _initWithNetworkReachability:([name length] ? SCNetworkReachabilityCreateWithName(kCFAllocatorDefault, [name UTF8String]) : NULL)];
}

- (void) dealloc
{
	[self setDelegate:nil];
	
	if(_runLoop)
	CFRelease(_runLoop);
	if(_netReachability)
	CFRelease(_netReachability);
	
	[super dealloc];
}

- (BOOL) isReachable
{
	SCNetworkConnectionFlags		flags;
	
	return (SCNetworkReachabilityGetFlags(_netReachability, &flags) && IS_REACHABLE(flags) ? YES : NO);
}

- (BOOL) isUsingCell
{
	SCNetworkConnectionFlags		flags;
	
	return (SCNetworkReachabilityGetFlags(_netReachability, &flags) && IS_CELL(flags) ? YES : NO);
}

- (void) setDelegate:(id<NetReachabilityDelegate>)delegate
{
	SCNetworkReachabilityContext	context = {0, self, NULL, NULL, NULL};
	
	if(delegate && !_delegate) {
		if(SCNetworkReachabilitySetCallback(_netReachability, _ReachabilityCallBack, &context)) {
			if(!SCNetworkReachabilityScheduleWithRunLoop(_netReachability, _runLoop, kCFRunLoopCommonModes)) {
				SCNetworkReachabilitySetCallback(_netReachability, NULL, NULL);
				delegate = nil;
			}
		}
		else
		delegate = nil;
		if(delegate == nil)
		REPORT_ERROR(@"Failed installing SCNetworkReachability callback on runloop %p", _runLoop);
	}
	else if(!delegate && _delegate) {
		SCNetworkReachabilityUnscheduleFromRunLoop(_netReachability, _runLoop, kCFRunLoopCommonModes);
		SCNetworkReachabilitySetCallback(_netReachability, NULL, NULL);
	}
	
	_delegate = delegate;
}

- (NSString*) description
{
	return [NSString stringWithFormat:@"<%@ = 0x%08X | reachable = %i>", [self class], (long)self, [self isReachable]];
}

@end
