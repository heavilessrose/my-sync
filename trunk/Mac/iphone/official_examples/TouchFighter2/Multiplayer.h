/*
File: Multiplayer.h
Abstract: Multiplayer class.

Version: 2.0
*/

#import <UIKit/UIKit.h>
#import "GameServer.h"
#import "GameClient.h"

typedef enum {
	kMultiplayerType_Client,
	kMultiplayerType_Host
} MultiplayerType;

@class GamePeer, Message;

@interface Multiplayer : NSObject <GameServerDelegate, GameClientDelegate> {
	GameServer*			_server;
	GameClient*			_client;
	GamePeer*			_peer;	
}

- (NSArray*) onlineServers;
- (BOOL) connectTo:(GamePeer*)peer;
- (BOOL) sendMessage:(Message*)message immediate:(BOOL)b;

@property(readonly) MultiplayerType type;
@end