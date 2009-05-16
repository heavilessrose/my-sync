/*
File: GameServer.h
Abstract: This class implements the server side of a multiplayer game.

Version: 1.3
*/

#import <Foundation/Foundation.h>

#import "GamePeer.h"

//CLASSES:

@class GameServer, TCPServer, UDPSocket;

//PROTOCOLS:

@protocol GameServerDelegate <NSObject>
@optional
- (void) gameServerDidStartAdvertisingToClients:(GameServer*)server;
- (void) gameServerWillStopAdvertisingToClients:(GameServer*)server;

- (BOOL) gameServer:(GameServer*)server shouldAllowConnectionToClient:(GamePeer*)client;
- (void) gameServer:(GameServer*)server didFailConnectingToClient:(GamePeer*)client;
- (void) gameServer:(GameServer*)server didConnectToClient:(GamePeer*)client;
- (void) gameServer:(GameServer*)server didReceiveData:(NSData*)data fromClient:(GamePeer*)client immediate:(BOOL)immediate; //UDP delivery was used instead of TCP if "immediate" is YES
- (void) gameServer:(GameServer*)server didDisconnectFromClient:(GamePeer*)client;
@end

//CLASS INTERFACES:

/*
This class implements the server side of a multiplayer game.
It can be connected to one or more clients which are represented as instances of the GamePeer class.
You can then communicate with the clients using either TCP (for reliability) or UDP (for speed) protocols.
The server can be registered for automatic discovery by GameClients on the local network using Bonjour.
*/
@interface GameServer : NSObject
{
@private
	NSString*					_name;
	id							_plist;
	id<GameServerDelegate>		_delegate;
	NSUInteger					_delegateMethods;
	TCPServer*					_server;
	UDPSocket*					_socket;
	CFMutableDictionaryRef		_activeClients;
	NSMutableSet*				_connectedClients;
	
	BOOL						_advertising;
}
- (id) initWithName:(NSString*)name infoPlist:(id)plist port:(UInt16)port; //The "name" and "plist" can be "nil" - Pass 0 to have a port automatically be chosen
@property(readonly) NSString* name;
@property(readonly) UInt32 IPv4Address; //The returned address is in host-endian
@property(readonly) id infoPlist;
@property(readonly) UInt16 port;

@property(assign) id<GameServerDelegate> delegate;

- (BOOL) startAdvertisingToClientsWithIdentifier:(NSString*)identifier; //The "identifier" must be unique to your game
@property(readonly, getter=isAdvertising) BOOL advertising;
- (void) stopAdvertisingToClients;

@property(readonly) NSArray* connectedClients;
- (void) disconnectFromClient:(GamePeer*)client;

- (NSTimeInterval) measureRoundTripLatencyToClient:(GamePeer*)client; //Returns < 0.0 on error
- (BOOL) sendData:(NSData*)data toClient:(GamePeer*)client immediate:(BOOL)immediate; //UDP will be used instead of TCP if "immediate" is YES
- (BOOL) sendDataToAllClients:(NSData*)data immediate:(BOOL)immediate; //UDP will be used instead of TCP if "immediate" is YES
@end
