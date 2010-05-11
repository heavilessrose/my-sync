//
//  PresenceResponseParser.h
//  msn_for_mac
//
//  Created by  apple on 08-9-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface PresenceResponseParser : NSObject 
{
	NSData *parserData;
	NSString *character;
	NSString *resultString;
	NSString *sourceTypeString;
	NSError *parseError;
}

- (id)initWithData:(NSData *)theData;
- (NSString*)praseResponseData:(NSError**)theError;

@property (nonatomic, retain) NSData *parserData;
@property (nonatomic, retain) NSString *character;
@property (nonatomic, retain) NSString *resultString;
@property (nonatomic, retain) NSError *parseError;
@property (nonatomic, retain) NSString *sourceTypeString;
@end
