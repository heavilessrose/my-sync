//
//  LKLauncherItem.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKLauncherView;
@interface LKLauncherItem : NSObject <NSCoding> {

	LKLauncherView	*launcher;
	NSString		*title;
	NSString		*image;
	
	NSString		*style;
	NSInteger       badgeNumber;
	
	BOOL            canDelete;
}

@property (nonatomic, assign) LKLauncherView	*launcher;
@property (nonatomic, copy)   NSString			*title;
@property (nonatomic, copy)   NSString			*image;
@property (nonatomic, copy)   NSString			*style;
@property (nonatomic)         NSInteger			badgeNumber;
@property (nonatomic)         BOOL				canDelete;

@end
