//
//  GroupInfo.h
//  msn_for_mac
//
//  Created by  apple on 08-9-19.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ListHeaderView;

@interface GroupInfo : NSObject 
{
	NSString *name;
	NSString *stringId;
	NSString *gbcode;
	NSString *sync;
	NSMutableArray *persons;
	ListHeaderView *cell;
}

- (void)updateInfo:(GroupInfo*)theInfo;
- (id)initWithDictionary:(NSDictionary*)dict;
- (NSDictionary*)dictionaryRepresentation;

@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *stringId;
@property (nonatomic, retain) NSString *gbcode;
@property (nonatomic, retain) NSString *sync;
@property (nonatomic, retain) NSMutableArray *persons;
@property (nonatomic, retain) ListHeaderView *cell;
@end
