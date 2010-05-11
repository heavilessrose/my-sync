//
//  AdviceResponseParser.h
//  MSN
//
//  Created by  apple on 09-3-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

enum AdvtNodeType
{
	EM_UNKNOW_ADVT = 0,
	EM_ADVT_AD, 
	EM_ADVT_ITEM, 
	EM_ADVT_MEDIA, 
	EM_ADVT_TEXT,
};

@class Advertisement;

@interface AdvtResponseParser : NSObject 
{
	NSData *parserData;
	NSString *character;
	NSString *resultString;
	NSError *parseError;
	Advertisement *advt;
	enum AdvtNodeType nodeType;
}

- (id)initWithData:(NSData *)theData;
- (NSString*)praseResponseData:(NSError**)theError;

@property (nonatomic, retain) NSData *parserData;
@property (nonatomic, retain) NSError *parseError;
@property (nonatomic, retain) NSString *character;
@property (nonatomic, retain) NSString *resultString;
@property (nonatomic, retain) Advertisement *advt;
@end
