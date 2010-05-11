//
//  ReceivedFileObject.h
//  MSN
//
//  Created by  apple on 08-12-1.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ChatViewData;
@class HttpClient;

typedef enum _ReceivingStatus
{
	RECEIVE_ASK = 0,
	RECEIVE_ACTIONED,
	RECEIVE_FINISHED,
	RECEIVE_CANCELED,
} ReceivingStatus;

@interface ReceivedFileObject : NSObject <UIAlertViewDelegate>
{
	ReceivingStatus status;
	int receivedFileSize;
	BOOL sysDismiss;
	BOOL isVoiceClip;
	FILE *file;
	NSString *fileName;
	NSString *fileSize;
	NSString *transferid;
	NSString *fileUrl;
	NSString *ftid;
	NSString *documentPath;
	UIButton *acceptButton;
	UIButton *rejectButton;
	UIButton *cancelButton;
	UIButton *playButton;
	UIProgressView *progressBar;
	ChatViewData *chatViewData;
	HttpClient *httpClient;
	UIAlertView *myAlertView;
    
    NSMutableArray*         seqids;
}

- (void)receivedData:(NSDictionary*)theInfo;
- (void)receivedAction:(NSString*)theAction;
- (void)receivedUrl:(NSString*)theURL;
- (void)buttonClicked:(id)sender;
- (void)receiveFinished;
- (void)writeData:(NSData*)theData;
- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict;


@property (nonatomic, retain) NSString *transferid;
@property (nonatomic, retain) NSString *ftid;
@property (nonatomic, retain) NSString *fileName;
@property (nonatomic, retain) NSString *fileSize;
@property (nonatomic, retain) NSString *fileUrl;
@property (nonatomic, retain) UIButton *acceptButton;
@property (nonatomic, retain) UIButton *rejectButton;
@property (nonatomic, retain) UIButton *cancelButton;
@property (nonatomic, retain) UIButton *playButton;
@property (nonatomic, retain) UIProgressView *progressBar;
@property (nonatomic, assign) BOOL isVoiceClip;
@property (nonatomic,assign) ReceivingStatus status;
@property (nonatomic,assign) ChatViewData *chatViewData;
@property (nonatomic, retain) UIAlertView *myAlertView;
@end
