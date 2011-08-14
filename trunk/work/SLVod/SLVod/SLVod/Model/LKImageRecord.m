//
//  WBImageRecord.m
//  WBNetwork
//
//  Created by luke on 11-2-23.
//  Copyright 2011 58.com. All rights reserved.
//

#import "LKImageRecord.h"
#import "UIImage+NSCoding.h"


@implementation LKImageRecord

@synthesize img, show, downloaded;
@synthesize url;

- (void)dealloc
{
	MLog(@"");
	[img release];
	[url release];
	[super dealloc];
}

- (id)initWithUrl:(NSString *)urlStr
{
    if ((self = [super init])) {
        self.show = YES;
        self.url = [NSURL URLWithString:urlStr];
        self.img = [UIImage imageNamed:@"noimg.png"];
    }
    return self;
}

#pragma mark - NSCoding

- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeBool:show forKey:@"show"];
    [aCoder encodeBool:downloaded forKey:@"downloaded"];
    [aCoder encodeObject:img forKey:@"img"];
    [aCoder encodeObject:url forKey:@"url"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if ((self = [super init])) {
        downloaded = [aDecoder decodeBoolForKey:@"downloaded"];
        show = [aDecoder decodeBoolForKey:@"show"];
        img = [[aDecoder decodeObjectForKey:@"img"] retain];
        url = [[aDecoder decodeObjectForKey:@"url"] retain];
    }
    return self;
}
@end
