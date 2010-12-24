//
//  LKLauncherItem.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKLauncherItem.h"


@implementation LKLauncherItem

@synthesize launcher, title, image, style, badgeNumber, canDelete;





- (void)setBadgeNumber:(NSInteger)aBadgeNumber {
	badgeNumber = aBadgeNumber;
	
	[launcher performSelector:@selector(updateItemBadge:) withObject:self];
}

#pragma mark -
#pragma mark NSCoding

- (id)initWithCoder:(NSCoder*)decoder {
	if (self = [super init]) {
		self.title = [decoder decodeObjectForKey:@"title"];
		self.image = [decoder decodeObjectForKey:@"image"];
		self.style = [decoder decodeObjectForKey:@"style"];
		self.canDelete = [decoder decodeBoolForKey:@"canDelete"];
	}
	
	return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
	[encoder encodeObject:title forKey:@"title"];
	[encoder encodeObject:image forKey:@"image"];
	[encoder encodeObject:style forKey:@"style"];
	[encoder encodeBool:canDelete forKey:@"canDelete"];
}


@end
