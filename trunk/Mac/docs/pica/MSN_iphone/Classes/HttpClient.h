//
//  HttpRequest.h
//  msn_for_mac
//
//  Created by  apple on 08-9-10.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HttpClient : NSObject 
{
	NSDictionary *responseHeaders;
	NSOperationQueue *networkQueue;
	CFReadStreamRef readStream;
	NSMutableData *receivedData;
	NSInteger messageType;
	NSTimer *h3gTimer;

	NSLock *writeLock;
	BOOL requestComplete;
	id delegate;
	SEL callbackFun;
}

+ (HttpClient*)getInstance;

- (void)addHttpRequest:(NSURL*)requstURL 
				method:(NSString*)method
			   headers:(NSDictionary*)requestHeaders 
			bodyString:(NSData*)bodyString;

- (void)addHttpRequest:(NSURL*)requstURL 
				method:(NSString*)method 
			   headers:(NSDictionary*)requestHeaders 
			bodyString:(NSData*)bodyString 
				  type:(int)theType;

- (void)sendHttpRequest:(NSURL*)requstURL 
				 method:(NSString*)method 
				headers:(NSDictionary*)requestHeaders 
			 bodyString:(NSData*)bodyString 
				   type:(int)theType;

- (void)lockStream;
- (void)unlockStream;

- (void)handleBytesAvailable;
- (void)handleStreamComplete;
- (void)handleStreamError;
- (void)overTimeAlert;
- (void)overWIFITimeAlert;
- (void)handleNetworkEvent:(CFStreamEventType)type;

@property (nonatomic, assign) id delegate;
@property (nonatomic) SEL callbackFun;
@property (nonatomic, retain) NSLock *writeLock;
@property (nonatomic) NSInteger messageType;
@property (nonatomic, retain) NSTimer* h3gTimer;
@end
