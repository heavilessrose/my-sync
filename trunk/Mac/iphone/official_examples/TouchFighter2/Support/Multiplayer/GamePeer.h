/*
File: GamePeer.h
Abstract: This class represents a multiplayer game peer and its information.

Version: 1.3
*/

#import <Foundation/Foundation.h>
#import <sys/socket.h>

//CLASSES:

@class UDPSocket, TCPConnection;

//CLASS INTERFACES:

/*
This class represents a multiplayer game peer and its information.
If you obtainted a GamePeer instance from a GameServer, then it represents a remote client, otherwise, it represents a remote server.
*/
@interface GamePeer : NSObject
{
@private
	BOOL					_service,
							_server,
							_local;
	NSString*				_uniqueID;
	NSString*				_name;
	struct sockaddr*		_address;
	UDPSocket*				_socket;
	TCPConnection*			_connection;
	id						_plist;
	id						_delegate;
	BOOL					_disconnecting;
}
@property(readonly, getter=isServer) BOOL server;
@property(readonly) NSString* uniqueID;
@property(readonly) NSString* name;
@property(readonly) UInt32 IPv4Address; //The returned address is in host-endian
@property(readonly) NSString* address;
@property(readonly, getter=isLocal) BOOL local; //Yes if on the same subnet as the host

@property(readonly, getter=isConnecting) BOOL connecting;
@property(readonly, getter=isConnected) BOOL connected;
@property(readonly) id infoPlist; //Only valid if connected
@end
