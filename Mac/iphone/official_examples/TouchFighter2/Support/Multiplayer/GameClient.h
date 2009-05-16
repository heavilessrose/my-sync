/*
File: GameClient.h
Abstract: This class implements the client side of a multiplayer game.

Version: 1.3
*/

#import <Foundation/Foundation.h>

#import "GamePeer.h"

//CLASSES:

@class GameClient, NetServiceBrowser;

//PROTOCOLS:

@protocol GameClientDelegate <NSObject>
@optional
- (void) gameClientDidStartDiscoveringServers:(GameClient*)client;
- (void) gameClientDidUpdateOnlineServers:(GameClient*)client;
- (void) gameClientWillStopDiscoveringServers:(GameClient*)client;

- (void) gameClient:(GameClient*)client didFailConnectingToServer:(GamePeer*)server;
- (void) gameClient:(GameClient*)client didConnectToServer:(GamePeer*)server;
- (void) gameClient:(GameClient*)client didReceiveData:(NSData*)data fromServer:(GamePeer*)server immediate:(BOOL)immediate; //UDP delivery was used instead of TCP if "immediate" is YES
- (void) gameClient:(GameClient*)client didDisconnectFromServer:(GamePeer*)server;
@end

//CLASS INTERFACES:

/*
This class implements the client side of a multiplayer game.
It can be connected to one or more servers which are represented as instances of the GamePeer class.
You can then communicate with the servers using either TCP (for reliability) or UDP (for speed) protocols.
Servers can be automatically found on the local network using Bonjour.
*/
@interface GameClient : NSObject
{
@private
	NSString*					_name;
	id							_plist;
	id<GameClientDelegate>		_delegate;
	NSUInteger					_delegateMethods;
	NSMutableSet*				_connectingServers;
	NSMutableSet*				_connectedServers;
	
	CFMutableDictionaryRef		_onlineServers;
	NetServiceBrowser*			_browser;
}
+ (GamePeer*) serverWithAddress:(NSString*)address; //For instance: @"games.apple.com:123"
+ (GamePeer*) serverWithIPv4Address:(UInt32)address port:(UInt16)port; //The "address" is assumed to be in host-endian

- (id) initWithName:(NSString*)name infoPlist:(id)plist; //The "name" and "plist" can be "nil"
@property(readonly) NSString* name;
@property(readonly) id infoPlist;

@property(assign) id<GameClientDelegate> delegate;

- (BOOL) startDiscoveringServersWithIdentifier:(NSString*)identifier; //The "identifier" must be unique to your game
@property(readonly, getter=isDiscoveringServers) BOOL discoveringServers;
@property(readonly) NSArray* onlineServers; //Returns nil when not discovering servers
- (void) stopDiscoveringServers;

- (BOOL) connectToServer:(GamePeer*)server;
@property(readonly) NSArray* connectedServers;
- (void) disconnectFromServer:(GamePeer*)server;

- (NSTimeInterval) measureRoundTripLatencyToServer:(GamePeer*)server; //Returns < 0.0 on error
- (BOOL) sendData:(NSData*)data toServer:(GamePeer*)server immediate:(BOOL)immediate; //UDP will be used instead of TCP if "immediate" is YES
@end
