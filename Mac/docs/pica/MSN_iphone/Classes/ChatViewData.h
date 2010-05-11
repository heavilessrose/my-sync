//
//  ChatViewData.h
//  MSN
//
//  Created by  apple on 09-3-18.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <AudioToolbox/AudioServices.h>

@class SendFileObject;
@class ReceivedFileObject;
@class ChatViewController;
@class PersonInfo;

@interface ChatViewData : NSObject <UITableViewDataSource, UITableViewDelegate>
{
	SystemSoundID messageSound;
	SystemSoundID nudgeSound;
	
	NSMutableArray *messageArray;
	NSMutableDictionary *sendingFileDicts;
	NSMutableDictionary *receivingFileDicts;
	
	PersonInfo *contact;
	NSMutableArray *persons;
	
	NSString *sendingText;
	NSString *myId;
	NSString *sid;
	UIImage *image;
	BOOL isSound;
	BOOL isConversationSaved;
	int hasMessage;
	int clearCount;
	
	ChatViewController *controller;
@private
	NSString*			voiceClipPath;
}

- (PersonInfo*)contact;
- (void)setContact:(PersonInfo*)theContact;

- (int)messageCount;

- (void)clear;
- (void)send:(NSString*)theMessage;
- (void)sendVoiceClip:(NSString*)theFile;
- (void)sendImage:(UIImage*)theImage;

- (BOOL)receivingFile;
- (BOOL)sendingFile;

- (void)receivedMessage:(NSDictionary*)theInfo;
- (void)receivedGroupMessage:(NSDictionary*)theInfo;
- (void)receivedAction:(NSDictionary*)theInfo;
- (void)receivedFile:(NSDictionary*)theInfo;
- (void)receivedVoiceClip:(NSDictionary*)theInfo;
- (void)receivedStatus:(NSDictionary*)theInfo;
- (void)receivedData:(NSDictionary*)theInfo;
- (void)participantChanged:(PersonInfo*)info type:(NSString*)type;

- (void)receiveFinished:(ReceivedFileObject*)sender saved:(BOOL)flag;
- (void)sendFinished:(SendFileObject*)sender;
- (void)receiveCanceled:(id)sender;
- (void)userAction:(BOOL)accept from:(id)sender;

- (void)notifyPersonAdded: (NSNotification*)note;
- (void)saveConversation:(NSString *)theName;

-(void)saveTranscriptsSessons;

/*!
 @method     
 @abstract   save transcripts
 @discussion save chat transcripts.
 */
- (void)saveTranscripts;

@property (nonatomic, assign) int hasMessage;
@property (nonatomic, retain) UIImage *image;
@property (nonatomic, retain) NSString *sid;
@property (nonatomic, retain) NSString *sendingText;
@property (nonatomic, retain) PersonInfo *contact;
@property (nonatomic, retain) NSMutableArray *persons;
@property (nonatomic, assign) ChatViewController *controller;

@property (nonatomic, retain) NSMutableArray* messageArray;
@property (nonatomic, assign) BOOL isSound;

- (void)sendAuioFile: (NSString*)aFilepath;

- (void)garbage;

- (void)cancelTransferingFile;
- (void)rejectReveicingFile;
@end
