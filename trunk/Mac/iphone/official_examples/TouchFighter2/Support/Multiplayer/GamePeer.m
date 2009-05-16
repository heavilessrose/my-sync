/*
File: GamePeer.m
Abstract: This class represents a multiplayer game peer and its information.

Version: 1.3
*/

#import <netinet/in.h>

#import "GamePeer.h"
#import "GameClient.h"
#import "TCPConnection.h"
#import "UDPSocket.h"
#import "NetUtilities.h"
#import "Multiplayer_Internal.h"

//CONSTANTS:

#define kResolveTimeOut				5.0
#define kMagic						0xABCD1234

//CLASS INTERFACES:

@interface GamePeer (Internal) <UDPSocketDelegate, TCPConnectionDelegate>
@end

//FUNCTIONS:

/*
Game "identifier" is used as a Bonjour application protocol and is constrained by:
http://files.dns-sd.org/draft-cheshire-dnsext-dns-sd.txt
http://www.iana.org/assignments/port-numbers
Result string must be unique, no longer than 14 characters, and only contain the "a-z", "A-Z", "0-9", "." or "-" characters
*/
NSString* _ApplicationProtocolFromGameIdentifier(NSString* string)
{
	const char*					cString = [string UTF8String];
	unsigned long				hash = 0;
	int							c;
	
	while((c = *cString++))
	hash = c + (hash << 6) + (hash << 16) - hash;
	
	return [NSString stringWithFormat:@"GAME-%08X", hash];
}

//CLASS IMPLEMENTATIONS:

@implementation GamePeer

@synthesize server=_server, infoPlist=_plist, uniqueID=_uniqueID, name=_name, local=_local, delegate=_delegate, service=_service;

- (id) initWithCFNetService:(CFNetServiceRef)netService
{
	NSString*					name = (netService ? (NSString*)CFNetServiceGetName(netService) : nil);
	struct sockaddr*			address;
	
	if(netService && CFNetServiceResolveWithTimeout(netService, kResolveTimeOut, NULL))
	address = (struct sockaddr*)[[(NSArray*)CFNetServiceGetAddressing(netService) objectAtIndex:0] bytes];
	else
	address = NULL;
	
	if((self = [self initWithName:name address:address]))
	_service = YES;
	
	return self;
}

- (id) initWithName:(NSString*)name address:(const struct sockaddr*)address
{
	NSRange						range;
	
	if(address == NULL) {
		[self release];
		return nil;
	}
	
	if((self = [super init])) {
		_server = YES;
		_address = malloc(address->sa_len);
		bcopy(address, _address, address->sa_len);
		_local = IPAddressIsLocal(_address);
		
		if(name) {
			range = [name rangeOfString:@":" options:0 range:NSMakeRange(0, [name length])];
			if(range.location == NSNotFound) {
				[self release];
				return nil;
			}
			_uniqueID = [[name substringToIndex:range.location] copy];
			_name = [[name substringFromIndex:(range.location + 1)] copy];
		}
	}
	
	return self;
}

- (id) initWithConnection:(TCPConnection*)connection
{
	if((self = [super init])) {
		_connection = [connection retain];
		[_connection setDelegate:self];
	}
	
	return self;
}

- (void) dealloc
{
	[self disconnect];
	
	[_plist release];
	[_name release];
	[_uniqueID release];
	
	if(_address)
	free(_address);
	
	[super dealloc];
}

- (BOOL) connect
{
	if(_connection)
	return NO;
	
	_connection = [[TCPConnection alloc] initWithRemoteAddress:_address];
	if(_connection == nil)
	return NO;
	[_connection setDelegate:self];
	
	return YES;
}

- (BOOL) sendData:(NSData*)data immediate:(BOOL)immediate
{
	return (immediate ? (_socket != (id)kCFNull) && [_socket sendData:data toRemoteAddress:_address] : [_connection sendData:data]);
}

- (void) disconnect
{
	BOOL				wasValidating = (_connection ? YES : NO),
						wasConnected = (_socket ? YES : NO);
	
	if(_disconnecting == NO) {
		_disconnecting = YES;
		
		if(_socket != (id)kCFNull) {
			[_socket invalidate];
			[_socket setDelegate:nil];
			[_socket autorelease]; //NOTE: Ensure UDPSocket is not de-alloced immediately as -disconnect might be called from inside one of its delegate calls
		}
		_socket = nil;
		[_connection invalidate];
		[_connection setDelegate:nil];
		[_connection autorelease]; //NOTE: Ensure TCPConnection is not de-alloced immediately as -disconnect might be called from inside one of its delegate calls
		_connection = nil;
		
		if(wasConnected)
		[_delegate gamePeerDidDisconnect:self];
		else if(wasValidating)
		[_delegate gamePeerDidFailConnecting:self];
		
		_disconnecting = NO;
	}
}

- (const struct sockaddr*) socketAddress
{
	return _address;
}

- (UInt32) IPv4Address
{
	return (_address && (_address->sa_family == AF_INET) ? ntohl(((struct sockaddr_in*)_address)->sin_addr.s_addr) : 0);
}

- (NSString*) address
{
	return IPAddressToString(_address, NO, NO);
}

- (BOOL) isConnecting
{
	return (_connection && !_socket ? YES : NO);
}

- (BOOL) isConnected
{
	return (_socket ? YES : NO);
}

- (NSString*) description
{
	return [NSString stringWithFormat:@"<%@ = 0x%08X | ID = %@ | address = %@ | local = %i | name = \"%@\" | connected = %i>", [self class], (long)self, [self uniqueID], [self address], [self isLocal], [self name], [self isConnected]];
}

- (NSData*) _dataFromDictionary:(NSDictionary*)dictionary
{
	NSMutableData*				data = [NSMutableData data];
	int							magic = NSSwapHostIntToBig(kMagic);
	
	[data appendBytes:&magic length:sizeof(int)];
	[data appendData:[NSPropertyListSerialization dataFromPropertyList:dictionary format:NSPropertyListBinaryFormat_v1_0 errorDescription:NULL]];
	
	return data;
}

- (NSDictionary*) _dictionaryFromData:(NSData*)data
{
	NSDictionary*				dictionary = nil;
	NSString*					error;
	
	if(([data length] > sizeof(int)) && (NSSwapBigIntToHost(*((int*)[data bytes])) == kMagic)) {
		dictionary = [NSPropertyListSerialization propertyListFromData:[data subdataWithRange:NSMakeRange(sizeof(int), [data length] - sizeof(int))] mutabilityOption:NSPropertyListImmutable format:NULL errorDescription:&error];
		if(dictionary == nil)
		REPORT_ERROR(@"Failed de-serializing dictionary: \"%@\"", error);
	}
	
	return dictionary;
}

- (void) _finishConnecting:(NSDictionary*)dictionary
{
	[_uniqueID release];
	_uniqueID = [[dictionary objectForKey:@"id"] copy];
	[_name release];
	_name = [[dictionary objectForKey:@"name"] copy];
	_plist = [[dictionary objectForKey:@"info"] retain];
	if(![_uniqueID length] || ![_name length])
	[NSException raise:NSInternalInconsistencyException format:@""];
	
	if(_server == YES) {
		_socket = [[UDPSocket alloc] initWithPort:[_connection localPort]];
		if(_socket == nil) {
			REPORT_ERROR(@"Failed creating UDP socket on port %i", [_connection localPort]);
			[self disconnect];
			return;
		}
		[_socket setDelegate:self];
	}
	else
	_socket = (id)kCFNull;
	
	[_delegate gamePeerDidConnect:self];
}

- (NSTimeInterval) measureRoundTripLatency
{
	CFTimeInterval				time;
	NSData*						data;
	NSDictionary*				dictionary;
	
	data = [self _dataFromDictionary:[NSDictionary dictionary]];
	
	time = CFAbsoluteTimeGetCurrent();
	if(![_connection sendData:data])
	return -1.0;
	do {
		data = [_connection receiveData];
		if(data == nil)
		return -1.0;
		
		dictionary = [self _dictionaryFromData:data];
		if(dictionary) {
			if([dictionary count]) {
				REPORT_ERROR(@"Received unexpected dictionary: \"%@\"", dictionary);
				dictionary = nil;
			}
		}
		else
		[_delegate gamePeer:self didReceiveData:data immediate:NO];
	} while(dictionary == nil);
	time = CFAbsoluteTimeGetCurrent() - time;
	
	return time;
}

- (void) connectionDidFailOpening:(TCPConnection*)connection
{
	[self disconnect];
}

- (void) connectionDidOpen:(TCPConnection*)connection
{
	NSDictionary*				dictionary;
	NSData*						data;
	const struct sockaddr*		address;
	
	if(_address == NULL) {
		address = [_connection remoteSocketAddress];
		if(address == NULL) {
			REPORT_ERROR(@"Failed retrieving connection remote address from %@", _connection);
			[self disconnect];
			return;
		}
		_address = malloc(address->sa_len);
		bcopy(address, _address, address->sa_len);
		_local = IPAddressIsLocal(_address);
	}
	
	dictionary = [NSMutableDictionary new];
	[(NSMutableDictionary*)dictionary setObject:HostGetUniqueID() forKey:@"id"];
	[(NSMutableDictionary*)dictionary setObject:[_delegate gamePeerWillSendName:self] forKey:@"name"];
	[(NSMutableDictionary*)dictionary setValue:[_delegate gamePeerWillSendInfoPlist:self] forKey:@"info"];
	data = [self _dataFromDictionary:dictionary];
	[dictionary release];
	
	if(_server == NO) {
		dictionary = [self _dictionaryFromData:[_connection receiveData]];
		if(dictionary == nil) {
			REPORT_ERROR(@"Failed receiving dictionary from connection", NULL);
			[self disconnect];
			return;
		}
	}
	
	if(![_connection sendData:data]) {
		REPORT_ERROR(@"Failed sending dictionary to connection", NULL);
		[self disconnect];
	}
	
	if(_server == NO)
	[self _finishConnecting:dictionary];
}

- (void) connectionDidClose:(TCPConnection*)connection
{
	[self disconnect];
}

- (void) connection:(TCPConnection*)connection didReceiveData:(NSData*)data
{
	NSDictionary*				dictionary;
	
	if((dictionary = [self _dictionaryFromData:data])) {
		if(_socket == nil)
		[self _finishConnecting:dictionary];
		else if(![dictionary count]) {
			if(![_connection sendData:data])
			REPORT_ERROR(@"Failed sending dictionary: \"%@\"", dictionary);
		}
		else
		REPORT_ERROR(@"Received unexpected dictionary: \"%@\"", dictionary);
	}
	else
	[_delegate gamePeer:self didReceiveData:data immediate:NO];
}

- (void) socketDidInvalidate:(UDPSocket*)socket
{
	[self disconnect];
}

- (void) socket:(UDPSocket*)socket didReceiveData:(NSData*)data fromRemoteAddress:(const struct sockaddr*)address
{
	const struct sockaddr*				remoteAddress = [_connection remoteSocketAddress];
	
	if(address && remoteAddress && (address->sa_len == remoteAddress->sa_len) && !bcmp(address, remoteAddress, address->sa_len))
	[_delegate gamePeer:self didReceiveData:data immediate:YES];
	else
	REPORT_ERROR(@"Received UDP data from unknown sender at \"%@\"", IPAddressToString(address, NO, NO));
}

@end
