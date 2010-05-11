//
//  MessageParser.h
//  msn_for_mac
//
//  Created by  apple on 08-9-25.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

enum MessageNodeType
{
	EM_UNKNOW_MESSAGE = 0,
	EM_MESSAGE_IQ, 
	EM_MESSAGE_QUERY, 
	EM_MESSAGE_PRESENCE_SELF,
	EM_MESSAGE_PRESENCE_OTHERS,
	EM_MESSAGE_ITEM,
	EM_MESSAGE_GROUP,
	EM_MESSAGE_CHAT,
	EM_MESSAGE_GROUPCHAT,
	EM_MESSAGE_File,
	EM_MESSAGE_VOICE,
};


@class PersonInfo;
@class StrangerInfo;
@interface MessageParser : NSObject 
{
	int errorCode;
	BOOL nudge;
	BOOL urlChanged;
	NSData *parserData;
	NSString *character;
	NSString *resultString;
	NSString *errorString;
	NSString *actionType;
	NSString *messageType;
	NSString *messageBody;
	NSString *messageReason;
	NSString *receiveJid;
	NSString *receiveImid;	
	NSString *receiveSid;
	NSString *receiveTransferid;
	NSString *receiveFtid;
	NSString *receiveSeqid;
	NSString *receiveStatus;
	NSString *receiveMsgTime;
	NSString *base64data;
	NSString *fileURL;
	NSString *fileName;
	NSString *fileSize;
	NSString *currentSize;
	NSString *fileStart;
	NSString *fileEnd;
	NSError *parseError;
	PersonInfo *itemInfo;
	StrangerInfo *strangerInfo;
	NSMutableDictionary *infoDictionary;
	enum MessageNodeType nodeType;
}

- (id)initWithData:(NSData *)theData;
- (NSString*)praseResponseData:(NSError**)theError;

@property (nonatomic, retain) NSData *parserData;
@property (nonatomic, retain) NSString *character;
@property (nonatomic, retain) NSString *resultString;
@property (nonatomic, retain) NSString *errorString;
@property (nonatomic, retain) NSString *actionType;
@property (nonatomic, retain) NSString *messageType;
@property (nonatomic, retain) NSString *messageBody;
@property (nonatomic, retain) NSString *messageReason;
@property (nonatomic, retain) NSString *receiveJid;
@property (nonatomic, retain) NSString *receiveImid;
@property (nonatomic, retain) NSString *receiveSid;
@property (nonatomic, retain) NSString *receiveTransferid;
@property (nonatomic, retain) NSString *receiveFtid;
@property (nonatomic, retain) NSString *receiveSeqid;
@property (nonatomic, retain) NSString *receiveStatus;
@property (nonatomic, retain) NSString *receiveMsgTime;
@property (nonatomic, retain) NSString *fileName;
@property (nonatomic, retain) NSString *fileSize;
@property (nonatomic, retain) NSString *currentSize;
@property (nonatomic, retain) NSString *fileStart;
@property (nonatomic, retain) NSString *fileEnd;
@property (nonatomic, retain) NSString *fileURL;
@property (nonatomic, retain) NSString *base64data;
@property (nonatomic, retain) NSError *parseError;
@property (nonatomic, retain) PersonInfo *itemInfo;
@property (nonatomic, assign) int errorCode;

@end
