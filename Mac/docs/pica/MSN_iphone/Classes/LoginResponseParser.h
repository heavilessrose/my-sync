//
//  LoginResponseParser.h
//  msn_for_mac
//
//  Created by  apple on 08-9-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

enum LoginNodeType
{
	EM_UNKNOW_NODE = 0,
	EM_IQ, 
	EM_PRESENCE,
	EM_ITEM,
	EM_GROUP,
};
enum LoginNoteType
{
	EM_UNKNOW_NOTE = 0,
	EM_NOTE_VCARD, 
	EM_NOTE_GROUP,
	EM_NOTE_ROSER,
	EM_NOTE_PRESENCE_SELF,
	EM_NOTE_PRESENCE_OTHERS,
};

@class GroupInfo;
@class PersonInfo;
@class StrangerInfo;

@interface LoginResponseParser : NSObject 
{
	int errorCode;
	NSData *parserData;
	NSString *character;
	NSString *resultString;
	NSString *errorString;
	NSError *parseError;
	NSString *messageOrigin;
	NSString *actionType;
	
	NSMutableDictionary *infoDictionary;
	NSInteger additionValue;
	NSInteger lastValue;
	enum LoginNodeType nodeType;
	enum LoginNoteType noteType;
	GroupInfo *gropuInfo;
	PersonInfo *itemInfo;
	StrangerInfo *strangerInfo;
	
	BOOL isGetRoster;
	NSInteger tempLastValue;
}

- (id)initWithData:(NSData *)theData;
- (NSString*)praseResponseData:(NSError**)theError;

@property (nonatomic, retain) NSData *parserData;
@property (nonatomic, retain) NSError *parseError;
@property (nonatomic, retain) NSString *character;
@property (nonatomic, retain) NSString *resultString;
@property (nonatomic, retain) NSString *errorString;
@property (nonatomic, retain) NSString *messageOrigin;
@property (nonatomic, retain) NSString *actionType;
@property (nonatomic, retain) PersonInfo *itemInfo;
@property (nonatomic, assign) int errorCode;
@property (nonatomic, assign) NSInteger lastValue;
@end
