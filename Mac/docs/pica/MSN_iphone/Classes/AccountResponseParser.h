//
//  LoginResponseParaser.h
//  msn_for_mac
//
//  Created by  apple on 08-9-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface AccountResponseParser : NSObject
{
	NSString *character;
	NSString *resultString;
	NSString *errorString;
	NSError *parseError;
	NSData *parserData;
	int errorCode;
}
- (id)initWithData:(NSData *)theData;
- (NSString*)praseResponseData:(NSError**)theError;

@property (nonatomic, retain) NSString *character;
@property (nonatomic, retain) NSData *parserData;
@property (nonatomic, retain) NSString *resultString;
@property (nonatomic, retain) NSString *errorString;
@property (nonatomic, retain) NSError *parseError;
@property (nonatomic, assign) int errorCode;
@end
