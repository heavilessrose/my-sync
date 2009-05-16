/*
File: TCPServer.m
Abstract: Subclass of TCPService that implements a full TCP server.

Version: 1.3
*/

#import <pthread.h>
#import <unistd.h>

#import "TCPServer.h"
#import "Networking_Internal.h"

//STRUCTURES:

typedef struct {
	NSUInteger					depth;
	NSAutoreleasePool*			pool;
} ObserverData;

//FUNCTIONS:

static void _ObserverCallBack(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void* info)
{
	ObserverData*				data = (ObserverData*)info;
	
	if(activity & kCFRunLoopEntry)
	data->depth += 1;
	
	if((activity & kCFRunLoopAfterWaiting) && (data->depth == 1)) {
		if(data->pool == nil)
		data->pool = [NSAutoreleasePool new];
	}
	
	if((activity & kCFRunLoopBeforeWaiting) && (data->depth == 1)) {
		[data->pool drain];
		data->pool = nil;
	}
	
	if(activity & kCFRunLoopExit)
	data->depth -= 1;
}

//CLASS INTERFACES:

@interface TCPServerConnection (Private)
- (void) _setServer:(TCPServer*)server;
@end

@interface TCPServer (Internal)
- (void) _removeConnection:(TCPServerConnection*)connection;
@end

//CLASS IMPLEMENTATIONS:

@implementation TCPServerConnection

@synthesize server=_server;

- (void) _setServer:(TCPServer*)server
{
	_server = server;
}

- (void) _invalidate
{
	CFRunLoopRef		runLoop = [self CFRunLoop]; //NOTE: We don't need to retain it as we know it will still be valid
	TCPServer*			server;
	
	server = [_server retain];
	
	[super _invalidate]; //NOTE: The server delegate may destroy the server when notified this connection was invalidated
	
	if([[server class] useConnectionThreads])
	CFRunLoopStop(runLoop);
	else
	[server _removeConnection:self];
	
	[server release];
}

@end

@implementation TCPServer

@synthesize delegate=_delegate;

+ (BOOL) useConnectionThreads
{
	return NO;
}

+ (Class) connectionClass
{
	return [TCPServerConnection class];
}

- (id) initWithPort:(UInt16)port
{
	if((self = [super initWithPort:port])) {
		_connections = [NSMutableSet new];
		pthread_mutex_init(&_connectionsMutex, NULL);
	}
	
	return self;
}

- (void) dealloc
{
	[self stop]; //NOTE: Make sure our -stop is executed immediately
	
	pthread_mutex_destroy(&_connectionsMutex);
	
	[_connections release];
	
	[super dealloc];
}

- (void) setDelegate:(id<TCPServerDelegate>)delegate
{
	_delegate = delegate;
	
	SET_DELEGATE_METHOD_BIT(0, serverDidStart:);
	SET_DELEGATE_METHOD_BIT(1, serverDidEnableBonjour:);
	SET_DELEGATE_METHOD_BIT(2, server:shouldAcceptConnectionFromAddress:);
	SET_DELEGATE_METHOD_BIT(3, server:didOpenConnection:);
	SET_DELEGATE_METHOD_BIT(4, server:didCloseConnection:);
	SET_DELEGATE_METHOD_BIT(5, serverWillDisableBonjour:);
	SET_DELEGATE_METHOD_BIT(6, serverWillStop:);
}

- (BOOL) startUsingRunLoop:(NSRunLoop*)runLoop
{
	if(![super startUsingRunLoop:runLoop])
	return NO;
	
	if(TEST_DELEGATE_METHOD_BIT(0))
	[_delegate serverDidStart:self];
	
	return YES;
}

- (BOOL) enableBonjourWithDomain:(NSString*)domain applicationProtocol:(NSString*)protocol name:(NSString*)name
{
	if(![super enableBonjourWithDomain:domain applicationProtocol:protocol name:name])
	return NO;
	
	if(TEST_DELEGATE_METHOD_BIT(1))
	[_delegate serverDidEnableBonjour:self];
	
	return YES;
}

- (void) disableBonjour
{
	if([self isBonjourEnabled] && TEST_DELEGATE_METHOD_BIT(5))
	[_delegate serverWillDisableBonjour:self];
	
	[super disableBonjour];
}

- (void) stop
{
	NSArray*			connections;
	TCPConnection*		connection;
	
	if([self isRunning] && TEST_DELEGATE_METHOD_BIT(6))
	[_delegate serverWillStop:self];
	
	[super stop];
	
	//NOTE: To avoid dead-locks in the connection threads, we need to work on a copy
	connections = [self allConnections];
	for(connection in connections)
	[connection invalidate];
}

- (NSArray*) allConnections
{
	NSArray*				connections;
	
	pthread_mutex_lock(&_connectionsMutex);
	connections = [_connections allObjects];
	pthread_mutex_unlock(&_connectionsMutex);
	
	return connections;
}

- (void) _addConnection:(TCPServerConnection*)connection
{
	pthread_mutex_lock(&_connectionsMutex);
	[_connections addObject:connection];
	[connection _setServer:self];
	pthread_mutex_unlock(&_connectionsMutex);
	
	if(TEST_DELEGATE_METHOD_BIT(3))
	[_delegate server:self didOpenConnection:connection];
}

- (void) _removeConnection:(TCPServerConnection*)connection
{
	if(TEST_DELEGATE_METHOD_BIT(4))
	[_delegate server:self didCloseConnection:connection];
	
	pthread_mutex_lock(&_connectionsMutex);
	[connection _setServer:nil];
	[_connections removeObject:connection];
	pthread_mutex_unlock(&_connectionsMutex);
}

- (void) _connectionThread:(NSNumber*)socketNumber
{
	NSAutoreleasePool*			pool = [NSAutoreleasePool new];
	ObserverData				data = {0, nil};
	CFRunLoopObserverContext	context = {0, &data, NULL, NULL, NULL};
	TCPServerConnection*		connection;
	CFRunLoopObserverRef		observerRef;
	
	connection = [[[[self class] connectionClass] alloc] initWithSocketHandle:[socketNumber intValue]];
	if(connection) {
		[self _addConnection:connection];
		
		observerRef = CFRunLoopObserverCreate(kCFAllocatorDefault, kCFRunLoopEntry | kCFRunLoopBeforeWaiting | kCFRunLoopAfterWaiting | kCFRunLoopExit, true, 0, _ObserverCallBack, &context);
		CFRunLoopAddObserver(CFRunLoopGetCurrent(), observerRef, kCFRunLoopCommonModes);
		CFRunLoopRun();
		CFRunLoopObserverInvalidate(observerRef);
		CFRelease(observerRef);
		[data.pool drain];
		
		[self _removeConnection:connection];
		[connection release];
	}
	else
	REPORT_ERROR(@"Failed creating TCPServerConnection for socket #%i", [socketNumber intValue]);
	
	[pool drain];
}

- (void) handleNewConnectionWithSocket:(NSSocketNativeHandle)socket fromRemoteAddress:(const struct sockaddr*)address
{
	TCPServerConnection*		connection;
	
	if(!TEST_DELEGATE_METHOD_BIT(2) || [_delegate server:self shouldAcceptConnectionFromAddress:address]) {
		if([[self class] useConnectionThreads])
		[NSThread detachNewThreadSelector:@selector(_connectionThread:) toTarget:self withObject:[NSNumber numberWithInt:socket]];
		else {
			connection = [[[[self class] connectionClass] alloc] initWithSocketHandle:socket];
			if(connection) {
				[self _addConnection:connection];
				[connection release];
			}
			else
			REPORT_ERROR(@"Failed creating TCPServerConnection for socket #%i", socket);
		}
	}
	else
	close(socket);
}

- (NSString*) description
{
	return [NSString stringWithFormat:@"<%@ = 0x%08X | running = %i | local address = %@ | %i connections>", [self class], (long)self, [self isRunning], [self localAddress], [_connections count]];
}

@end
