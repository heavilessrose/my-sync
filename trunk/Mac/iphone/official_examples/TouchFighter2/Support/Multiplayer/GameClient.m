/*
File: GameClient.m
Abstract: This class implements the client side of a multiplayer game.

Version: 1.3
*/

#import <netinet/in.h>

#import "GameClient.h"
#import "NetServiceBrowser.h"
#import "NetUtilities.h"
#import "Multiplayer_Internal.h"

//CLASS INTERFACE:

@interface GameClient (Internal) <GamePeerDelegate, NetServiceBrowserDelegate>
@end

//FUNCTIONS:

static void _DictionaryApplierFunction(const void* key, const void* value, void* context)
{
	GamePeer*				server = (GamePeer*)value;
	NSMutableArray*			array = (NSMutableArray*)context;
	GamePeer*				peer;
	
	for(peer in array) {
		if(![[peer uniqueID] isEqualToString:[server uniqueID]])
		continue;
		if([peer service] == [server service])
		return;
		
		if(([peer isConnecting] || [peer isConnected]) && ([server isConnecting] || [server isConnected]))
		[NSException raise:NSInternalInconsistencyException format:@""];
		
		if([server isConnecting] || [server isConnected] || [server service]) {
			[array removeObject:peer];
			break;
		}
		else
		return;
	}
	
	[array addObject:server];
}

//CLASS IMPLEMENTATIONS:

@implementation GameClient

@synthesize delegate=_delegate, name=_name, infoPlist=_plist;

+ (GamePeer*) serverWithAddress:(NSString*)address
{
	return [[[GamePeer alloc] initWithName:nil address:IPAddressFromString(address)] autorelease];
}

+ (GamePeer*) serverWithIPv4Address:(UInt32)address port:(UInt16)port
{
	struct sockaddr_in		ipAddress;
	
	bzero(&ipAddress, sizeof(ipAddress));
	ipAddress.sin_len = sizeof(ipAddress);
	ipAddress.sin_family = AF_INET;
	ipAddress.sin_port = htons(port);
	ipAddress.sin_addr.s_addr = htonl(address);
	
	return [[[GamePeer alloc] initWithName:nil address:(struct sockaddr*)&ipAddress] autorelease];
}

- (NSArray*) connectedServers
{
	return [_connectedServers allObjects];
}

- (id) init
{
	return [self initWithName:nil infoPlist:nil];
}

- (id) initWithName:(NSString*)name infoPlist:(id)plist
{
	if(![name length])
	name = HostGetName();
	
	if((self = [super init])) {
		_name = [name copy];
		_plist = [plist retain];
		_connectingServers = [NSMutableSet new];
		_connectedServers = [NSMutableSet new];
	}
	
	return self;
}

- (void) dealloc
{
	GamePeer*			peer;
	
	[self stopDiscoveringServers];
	
	for(peer in _connectingServers) {
		[peer setDelegate:nil];
		[peer disconnect];
	}
	[_connectingServers release];
	
	for(peer in _connectedServers) {
		[peer setDelegate:nil];
		[peer disconnect];
	}
	[_connectedServers release];
	
	[_plist release];
	
	[super dealloc];
}

- (void) setDelegate:(id<GameClientDelegate>)delegate
{
	_delegate = delegate;
	
	SET_DELEGATE_METHOD_BIT(0, gameClientDidStartDiscoveringServers:);
	SET_DELEGATE_METHOD_BIT(1, gameClientDidUpdateOnlineServers:);
	SET_DELEGATE_METHOD_BIT(2, gameClientWillStopDiscoveringServers:);
	SET_DELEGATE_METHOD_BIT(3, gameClient:didFailConnectingToServer:);
	SET_DELEGATE_METHOD_BIT(4, gameClient:didConnectToServer:);
	SET_DELEGATE_METHOD_BIT(5, gameClient:didReceiveData:fromServer:immediate:);
	SET_DELEGATE_METHOD_BIT(6, gameClient:didDisconnectFromServer:);
}

- (BOOL) startDiscoveringServersWithIdentifier:(NSString*)identifier
{
	if(_onlineServers || ![identifier length])
	return NO;
	
	_onlineServers = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	_browser = [[NetServiceBrowser alloc] initTCPBrowserWithDomain:nil applicationProtocol:_ApplicationProtocolFromGameIdentifier(identifier)];
	[_browser setDelegate:self];
	if(![_browser startUsingRunLoop:[NSRunLoop currentRunLoop]]) {
		[_browser release];
		_browser = nil;
		CFRelease(_onlineServers);
		_onlineServers = NULL;
		return NO;
	}
	
	if(TEST_DELEGATE_METHOD_BIT(0))
	[_delegate gameClientDidStartDiscoveringServers:self];
	
	return YES;
}

- (BOOL) isDiscoveringServers
{
	return (_onlineServers ? YES : NO);
}

- (NSArray*) onlineServers
{
	NSMutableArray*			array = nil;
	
	if(_onlineServers) {
		array = [NSMutableArray array];
		CFDictionaryApplyFunction(_onlineServers, _DictionaryApplierFunction, array);
	}
	
	return array;
}

- (void) stopDiscoveringServers
{
	if(_onlineServers) {
		if(TEST_DELEGATE_METHOD_BIT(2))
		[_delegate gameClientWillStopDiscoveringServers:self];
		
		[_browser stop];
		[_browser release];
		_browser = nil;
		
		CFRelease(_onlineServers);
		_onlineServers = NULL;
	}
}

- (BOOL) connectToServer:(GamePeer*)server
{
	if(!server || [server isConnected] || [server isConnecting])
	return NO;
	
	if(![server connect])
	return NO;
	
	[server setDelegate:self];
	[_connectingServers addObject:server];
	
	return YES;
}

- (void) disconnectFromServer:(GamePeer*)server
{
	if(server && ([_connectingServers containsObject:server] || [_connectedServers containsObject:server]))
	[server disconnect];
}

- (NSTimeInterval) measureRoundTripLatencyToServer:(GamePeer*)server
{
	return (server && [_connectedServers containsObject:server] ? [server measureRoundTripLatency] : -1.0);
}

- (BOOL) sendData:(NSData*)data toServer:(GamePeer*)server immediate:(BOOL)immediate
{
	if(!server || ![_connectedServers containsObject:server])
	return NO;
	
	return [server sendData:data immediate:immediate];
}

- (NSString*) description
{
	return [NSString stringWithFormat:@"<%@ = 0x%08X | discovering = %i | connected servers = %i>", [self class], (long)self, [self isDiscoveringServers], [[self connectedServers] count]];
}

@end

@implementation GameClient (GamePeerDelegate)

- (NSString*) gamePeerWillSendName:(GamePeer*)peer
{
	return _name;
}

- (id) gamePeerWillSendInfoPlist:(GamePeer*)peer
{
	return _plist;
}

- (void) gamePeerDidFailConnecting:(GamePeer*)peer
{
	[peer retain];
	
	[_connectingServers removeObject:peer];
	
	if(TEST_DELEGATE_METHOD_BIT(3))
	[_delegate gameClient:self didFailConnectingToServer:peer];
	
	[peer setDelegate:nil];
	[peer release];
}

- (void) gamePeerDidConnect:(GamePeer*)peer
{
	[_connectingServers removeObject:peer];
	[_connectedServers addObject:peer];
	
	if(TEST_DELEGATE_METHOD_BIT(4))
	[_delegate gameClient:self didConnectToServer:peer];
}

- (void) gamePeerDidDisconnect:(GamePeer*)peer
{
	[peer retain];
	
	if([_connectedServers containsObject:peer]) {
		[_connectedServers removeObject:peer];
		
		if(TEST_DELEGATE_METHOD_BIT(6))
		[_delegate gameClient:self didDisconnectFromServer:peer];
	}
	
	[peer setDelegate:nil];
	[peer release];
}

- (void) gamePeer:(GamePeer*)peer didReceiveData:(NSData*)data immediate:(BOOL)immediate
{
	if(TEST_DELEGATE_METHOD_BIT(5))
	[_delegate gameClient:self didReceiveData:data fromServer:peer immediate:immediate];
}

@end

@implementation GameClient (NetServiceBrowserDelegate)

- (void) browser:(NetServiceBrowser*)server didAddService:(CFNetServiceRef)service
{
	GamePeer*						peer;
	
	peer = [[GamePeer alloc] initWithCFNetService:service];
	if(peer) {
		CFDictionarySetValue(_onlineServers, service, peer);
		[peer release];
		
		if(TEST_DELEGATE_METHOD_BIT(1))
		[_delegate gameClientDidUpdateOnlineServers:self];
	}
	else {
		if (peer)
		[peer release];
		else
		REPORT_ERROR(@"Failed creating GamePeer from %@", service);
	}
}

- (void) browser:(NetServiceBrowser*)server didRemoveService:(CFNetServiceRef)service
{
	CFDictionaryRemoveValue(_onlineServers, service);
	
	if(TEST_DELEGATE_METHOD_BIT(1))
	[_delegate gameClientDidUpdateOnlineServers:self];
}

@end
