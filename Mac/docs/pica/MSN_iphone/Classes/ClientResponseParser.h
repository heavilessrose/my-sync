//
//  ClientResponseParser.h
//  msn_for_mac
//
//  Created by  apple on 08-9-17.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

enum ClientNodeType
{
	EM_UNKNOWN = 0,
	EM_SSO, 
	EM_SYSTEM,
	EM_UPGRADE,
	EM_LOGIN,
	EM_CONFIG,
	EM_TRANSPARENT,
};

@interface ClientResponseParser : NSObject 
{	
	NSInteger resultType;
	NSData *parserData;
	NSString *character;
	NSError *parseError;
	enum ClientNodeType nodeType;
	
	NSMutableArray *systemProperties;
	NSMutableDictionary *upgradeProperties;
	NSMutableDictionary *loginProperties;
	NSMutableDictionary *configProperties;	
	NSMutableDictionary *transparentProperties;	
}

- (id)initWithData:(NSData *)theData;
- (NSInteger)praseResponseData:(NSError**)theError;

@property (nonatomic, retain) NSData *parserData;
@property (nonatomic, retain) NSString *character;
@property (nonatomic, retain) NSError *parseError;
@property (nonatomic, retain) NSMutableDictionary *upgradeProperties;
@property (nonatomic, retain) NSMutableDictionary *configProperties;
@property (nonatomic, retain) NSMutableDictionary *loginProperties;
@property (nonatomic, retain) NSMutableDictionary *transparentProperties;

@property (nonatomic, retain) NSMutableArray *systemProperties;

@end
