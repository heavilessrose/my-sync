/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: Multiplayer.m
Abstract: Multiplayer class.

Version: 2.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

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