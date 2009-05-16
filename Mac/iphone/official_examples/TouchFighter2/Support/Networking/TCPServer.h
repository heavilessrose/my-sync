/*
File: TCPServer.h
Abstract: Subclass of TCPService that implements a full TCP server.

Version: 1.3
*/

#import "TCPService.h"
#import "TCPConnection.h"

//CLASSES:

@class TCPServer, TCPServerConnection;

//PROTOCOLS:

@protocol TCPServerDelegate <NSObject>
@optional
- (void) serverDidStart:(TCPServer*)server;
- (void) serverDidEnableBonjour:(TCPServer*)server;

- (BOOL) server:(TCPServer*)server shouldAcceptConnectionFromAddress:(const struct sockaddr*)address;
- (void) server:(TCPServer*)server didOpenConnection:(TCPServerConnection*)connection; //From this method, you typically set the delegate of the connection to be able to send & receive data through it
- (void) server:(TCPServer*)server didCloseConnection:(TCPServerConnection*)connection;

- (void) serverWillDisableBonjour:(TCPServer*)server;
- (void) serverWillStop:(TCPServer*)server;
@end

//CLASS INTERFACES:

/*
This subclass of TCPService implements a full TCP server which automatically maintains the list of active connections.
See TCPService.h for other methods.
*/
@interface TCPServer : TCPService
{
@private
	pthread_mutex_t				_connectionsMutex;
	NSMutableSet*				_connections;
	id<TCPServerDelegate>		_delegate;
	NSUInteger					_delegateMethods;
}
+ (BOOL) useConnectionThreads; //Use a separate thread for each connection - NO by default
+ (Class) connectionClass; //Must be a subclass of "TCPServerConnection"

@property(readonly) NSArray* allConnections;

@property(assign) id<TCPServerDelegate> delegate;
@end

/*
Subclass of TCPConnection used by TCPServer for its connections.
*/
@interface TCPServerConnection : TCPConnection
{
@private
	TCPServer*			_server; //Not retained
}
@property(readonly) TCPServer* server;
@end
