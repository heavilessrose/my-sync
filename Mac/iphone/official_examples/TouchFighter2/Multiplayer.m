/*
File: Multiplayer.m
Abstract: Multiplayer class.

Version: 2.0
*/

#import "Multiplayer.h"
#import "GameServer.h"
#import "TouchFighterAppDelegate.h"
#import "NetUtilities.h"
#import "TCPService.h"
#import "NetServiceBrowser.h"
#import "Message.h"

#define kServerPort					51000
#define kGameIdentifier				@"TouchFighter2"
#define kResolveTimeOut				5.0

void ShowAlertWithTitle(NSString *title, NSString *format, ...)
{
	UIAlertView*		alertView;
	va_list				list;
	NSString*			message;
	
	//Create message
	if(format) {
		va_start(list, format);
		message = [[[NSString alloc] initWithFormat:format arguments:list] autorelease];
		va_end(list);
	}
	else
	message = nil;
	
	//Show alert on screen
	alertView = [[UIAlertView alloc] initWithTitle:title message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
	[alertView show];
	[alertView release];
}

@implementation Multiplayer
@synthesize type=_type;

- (id) initWithType:(MultiplayerType)type
{
	if ((self = [super init])) {
		if (type == kMultiplayerType_Host) {
			_server = [[GameServer alloc] initWithName:nil infoPlist:nil port:kServerPort];
			[_server setDelegate:self];
			if(_server == nil) {
				ShowAlertWithTitle(@"Failed creating server", @"Check your networking configuration.");
				[self release];
				return nil;
			}
			//Start looking for clients
			if(![_server startAdvertisingToClientsWithIdentifier:kGameIdentifier]) {
				ShowAlertWithTitle(@"Failed advertising server", @"Check your networking configuration.");
				[self release];
				return nil;
			}
		}
		else if (type == kMultiplayerType_Client) {
			_client = [[GameClient alloc] initWithName:nil infoPlist:nil];
			[_client setDelegate:self];

			if(_client == nil) {
				ShowAlertWithTitle(@"Failed creating client", @"Check your networking configuration.");
				[self release];
				return nil;
			}
			
			//Start looking for servers
			if(![_client startDiscoveringServersWithIdentifier:kGameIdentifier]) {
				ShowAlertWithTitle(@"Failed discovering servers", @"Check your networking configuration.");
				[self release];
				return nil;
			}
		}
	}	
	return self;
}

- (NSArray*) onlineServers
{
	NSArray*			peers = [_client onlineServers];
	NSMutableArray*		array = [NSMutableArray new];
	GamePeer*			peer;
	
	for (peer in peers) {
		if ([peer IPv4Address] != [(GameServer*)_server IPv4Address])
		[array addObject:peer];
	}
	return [array autorelease];
}

- (BOOL) connectTo:(GamePeer*)peer
{
	return [_client connectToServer:peer];
}

- (BOOL) sendMessage:(Message*)message immediate:(BOOL)b
{
	NSData*		data;
	
	data = [NSKeyedArchiver archivedDataWithRootObject:message];	
	
	if (_server)
	return [_server sendData:data toClient:_peer immediate:b];
	else
	return [_client sendData:data toServer:_peer immediate:b];
}

- (void) dealloc
{
	[_server release];
	[_client release];
	
	[super dealloc];
}

- (MultiplayerType) type
{
	return (_server ? kMultiplayerType_Host : kMultiplayerType_Client);
}

@end

@implementation Multiplayer (ServerDelegate)

- (BOOL) gameServer:(GameServer*)server shouldAllowConnectionToClient:(GamePeer*)client
{
	//Always allow connection unless we already have a client
	return (_peer ? NO : YES);
}

- (void) gameServer:(GameServer*)server didConnectToClient:(GamePeer*)client
{
	if(_peer == nil) {
		_peer = [client retain];
		
		//Stop looking for clients
		[_server stopAdvertisingToClients];
		
		[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] startMultiplayer:[NSArray arrayWithObject:_peer]];		
	}
}

- (void) gameServer:(GameServer*)server didReceiveData:(NSData*)data fromClient:(GamePeer*)client immediate:(BOOL)immediate
{
	Message*		message = [NSKeyedUnarchiver unarchiveObjectWithData:data];

	if (message) {
		if (message->type == kMessageType_GameInfo) {
			[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] receiveGameData:message->data fromPeer:client];
		}
		else if (message->type == kMessageType_SelectedShip) {
			[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] receiveShipType:[(NSNumber*)message->data unsignedIntValue] fromPeer:client];
		}
	}
}

- (void) gameServer:(GameServer*)server didDisconnectFromClient:(GamePeer*)client
{
	UIAlertView*			alertView;
	
	//Show a user alert
	if(client == _peer) {
		alertView = [[UIAlertView alloc] initWithTitle:@"Client Disconnected!" message:nil delegate:nil cancelButtonTitle:nil otherButtonTitles:@"Continue", nil];
		[alertView show];
		[alertView release];
		
		[_peer release];
		_peer = nil;

		[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] receiveShipType:kSpaceShipTypeUnknown fromPeer:client];
		
		//Start looking for clients again
		if(![_server startAdvertisingToClientsWithIdentifier:kGameIdentifier])
		ShowAlertWithTitle(@"Failed advertising server", @"Check your networking configuration.");
	}
}

@end

@implementation Multiplayer (ClientDelegate)

- (void) gameClientDidUpdateOnlineServers:(GameClient*)client
{
	[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] updatePeers];
}

- (void) gameClient:(GameClient*)client didFailConnectingToServer:(GamePeer*)server
{
	if(_peer == nil)
	[_client performSelector:@selector(gameClientDidUpdateOnlineServers:) withObject:client afterDelay:5.0];
}

- (void) gameClient:(GameClient*)client didConnectToServer:(GamePeer*)server
{	
	//A previously started connection to a server completed successfully
	if(_peer == nil) {
		_peer = [server retain];
		
		//Stop looking for servers
		[_client stopDiscoveringServers];

		[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] startMultiplayer:[NSArray arrayWithObject:_peer]];		
	}
}

- (void) gameClient:(GameClient*)client didReceiveData:(NSData*)data fromServer:(GamePeer*)server immediate:(BOOL)immediate
{
	Message*		message = [NSKeyedUnarchiver unarchiveObjectWithData:data];

	if (message) {
		if (message->type == kMessageType_GameInfo) {
			[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] receiveGameData:message->data fromPeer:server];
		}
		else if (message->type == kMessageType_SelectedShip) {
			[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] receiveShipType:[(NSNumber*)message->data unsignedIntValue] fromPeer:server];
		}
	}
}

- (void) gameClient:(GameClient*)client didDisconnectFromServer:(GamePeer*)server
{
	UIAlertView*			alertView;
	
	//Show a user alert
	if(server == _peer) {
		alertView = [[UIAlertView alloc] initWithTitle:@"Server Disconnected!" message:nil delegate:nil cancelButtonTitle:nil otherButtonTitles:@"Continue", nil];
		[alertView show];
		[alertView release];
		
		[_peer release];
		_peer = nil;
		
		//Start looking for servers again
		if(![_client startDiscoveringServersWithIdentifier:kGameIdentifier])
		ShowAlertWithTitle(@"Failed discovering servers", @"Check your networking configuration.");
	}
}

@end