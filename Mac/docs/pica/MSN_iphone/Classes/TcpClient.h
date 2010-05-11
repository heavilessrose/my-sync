//
//  tcpClient.h
//  msn_for_mac
//
//  Created by  apple on 08-9-11.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TCPClient : NSObject 
{
	CFReadStreamRef readStream;
	CFWriteStreamRef writeStream;
	NSMutableData *receivedData;
	NSMutableData *unParsedData;
	NSOperationQueue *networkQueue;
	NSOperationQueue *receivedQueue;
	NSInteger messageType;
	NSLock *writeLock;
	NSTimer *brakeTimer;
	BOOL complete;	
	
	id delegate;
	SEL callbackFun;
}

+ (TCPClient*)getInstance;

- (BOOL)connectSocket;
- (void)writeString:(NSString*)theString messageType:(NSInteger)theType;
- (void)writeRequestIfValid:(NSString*)theString messageType:(NSInteger)theType;
- (void)lockStream;
- (void)unlockStream;

- (void)parserValidData:(NSData*)theData;

- (void)handleStreamComplete;
- (void)handleStreamError;
- (void)handleNetworkEvent:(CFStreamEventType)type;
- (void)handleWriteEvent:(CFStreamEventType)type;
- (void)handleStreamAvailable;
- (void)threadHandleBytesAvailable:(id)param;
- (void)sendBrakeMessage:(NSTimer*)theTimer;

- (void)reset;

@property (nonatomic, assign) id delegate;
@property (nonatomic) SEL callbackFun;
@property (nonatomic) NSInteger messageType;
@property (nonatomic, retain) NSLock *writeLock;
@property (nonatomic, retain) NSTimer *brakeTimer;
@property (nonatomic, retain) NSMutableData *receivedData;
@property (nonatomic, retain) NSMutableData *unParsedData;
@end
