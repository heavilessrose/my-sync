/*
File: NetServiceBrowser.m
Abstract: Convenience class that wraps the CFNetServiceBrowser APIs from CFNetwork and allows to discover Bonjour services of a given type on the local network.

Version: 1.3
*/

#import <netinet/in.h>

#import "NetServiceBrowser.h"
#import "Networking_Internal.h"

//CLASS IMPLEMENTATION:

@implementation NetServiceBrowser

static void _BrowserCallBack(CFNetServiceBrowserRef browser, CFOptionFlags flags, CFTypeRef domainOrService, CFStreamError* error, void* info)
{
	NSAutoreleasePool*		pool = [NSAutoreleasePool new];
	NetServiceBrowser*		self = (NetServiceBrowser*)info;
	
	if(!(flags & kCFNetServiceFlagIsDomain)) {
		if(flags & kCFNetServiceFlagRemove) {
			if(TEST_DELEGATE_METHOD_BIT(2))
			[self->_delegate browser:self didRemoveService:(CFNetServiceRef)domainOrService];
		}
		else {
			if(TEST_DELEGATE_METHOD_BIT(1))
			[self->_delegate browser:self didAddService:(CFNetServiceRef)domainOrService];
		}
	}
	
	[pool release];
}

@synthesize delegate=_delegate, running=_running;

- (id) initWithDomain:(NSString*)domain type:(NSString*)type
{
	if(![domain length])
	domain = @""; //NOTE: Equivalent to "local."
	
	if((self = [super init])) {
		_domain = [domain copy];
		_type = [type copy];
	}
	
	return self;
}

- (id) initTCPBrowserWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol
{
	return [self initWithDomain:domain type:_MakeServiceType(protocol, @"tcp")];
}

- (id) initUDPBrowserWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol
{
	return [self initWithDomain:domain type:_MakeServiceType(protocol, @"udp")];
}

- (void) dealloc
{
	[self stop];
	
	[_domain release];
	[_type release];

	[super dealloc];
}

- (void) setDelegate:(id<NetServiceBrowserDelegate>)delegate
{
	_delegate = delegate;
	
	SET_DELEGATE_METHOD_BIT(0, browserDidStart:);
	SET_DELEGATE_METHOD_BIT(1, browser:didAddService:);
	SET_DELEGATE_METHOD_BIT(2, browser:didRemoveService:);
	SET_DELEGATE_METHOD_BIT(3, browserWillStop:);
}

- (BOOL) startUsingRunLoop:(NSRunLoop*)runLoop
{
    CFNetServiceClientContext	context = {0, self, NULL, NULL, NULL};
	
	if(_runLoop)
	return NO;
	_runLoop = [runLoop getCFRunLoop];
	if(!_runLoop)
	return NO;
	CFRetain(_runLoop);
	
	_netBrowser = CFNetServiceBrowserCreate(kCFAllocatorDefault, _BrowserCallBack, &context);
	if(_netBrowser == NULL) {
		[self stop];
		return NO;
	}
	CFNetServiceBrowserScheduleWithRunLoop(_netBrowser, _runLoop, kCFRunLoopCommonModes);
	
	if(!CFNetServiceBrowserSearchForServices(_netBrowser, (CFStringRef)_domain, (CFStringRef)_type, NULL)) {
		[self stop];
		return NO;
	}
	
	_running = YES;
	if(TEST_DELEGATE_METHOD_BIT(0))
	[_delegate browserDidStart:self];
	
	return YES;
}

- (void) stop
{
	if(_running) {
		if(TEST_DELEGATE_METHOD_BIT(3))
		[_delegate browserWillStop:self];
		_running = NO;
	}
	
	if(_netBrowser) {
		CFNetServiceBrowserInvalidate(_netBrowser);
		CFNetServiceBrowserUnscheduleFromRunLoop(_netBrowser, _runLoop, kCFRunLoopCommonModes);
		CFRelease(_netBrowser);
		_netBrowser = NULL;
	}
	if(_runLoop) {
		CFRelease(_runLoop);
		_runLoop = NULL;
	}
}

- (NSString*) description
{
	return [NSString stringWithFormat:@"<%@ = 0x%08X | running = %i>", [self class], (long)self, [self isRunning]];
}

@end
