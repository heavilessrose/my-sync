//
//  SendFileObject.h
//  MSN
//
//  Created by  apple on 08-11-28.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ChatViewData;
@class HttpClient;

typedef enum _SendingStatus
{
	SEND_WAIT= 0,
	SEND_ACCEPTED,
	SEND_FINISHED,
	SEND_CANCELED,
} SendingStatus;


@interface SendFileObject : NSObject
{
	SendingStatus status;
	UIButton *cancelButton;
	UIButton *playButton;
	UIProgressView *progressBar;
	ChatViewData *chatViewData;
	HttpClient *httpClient;
	BOOL isVoiceClip;
	BOOL sendByServer;
	NSString *etag;
	NSString *fileUrl;
	NSString *fileName;
	NSData *imageData;
	NSString *transferid;
	NSString *ftid;
	NSInteger index;
	NSUInteger position;
	NSUInteger currentSize;
	NSUInteger fileSize;
}

- (id)initWithImageData:(NSData*)theData voiceClip:(BOOL)flag;
- (void)receivedAction:(NSString*)theAction;
- (void)buttonClicked:(id)sender;
- (void)sendFile;
- (void)sendNextFile;

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict;


@property (nonatomic, retain)NSData *imageData;
@property (nonatomic, retain)NSString *ftid;
@property (nonatomic, retain)NSString *transferid;
@property (nonatomic, retain)NSString *etag;
@property (nonatomic, retain)NSString *fileName;
@property (nonatomic, retain)NSString *fileUrl;
@property (nonatomic, assign)NSUInteger fileSize;
@property (nonatomic, assign)ChatViewData *chatViewData;
@property (nonatomic, retain)UIProgressView *progressBar;
@property (nonatomic, retain)UIButton *cancelButton;
@property (nonatomic, retain)UIButton *playButton;
@property (nonatomic, assign)SendingStatus status;
@property (nonatomic, assign)BOOL isVoiceClip;
@property (nonatomic, assign)BOOL sendByServer;
@end
