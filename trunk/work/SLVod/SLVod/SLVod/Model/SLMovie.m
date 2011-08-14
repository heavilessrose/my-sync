//
//  SLMovie.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLMovie.h"


@implementation SLMovie

@synthesize title;
//@synthesize imageUrl;
@synthesize url;
@synthesize content;
@synthesize actor;
@synthesize cate;
@synthesize imgRecord, downProgress, size, recvedBytes;

- (void)dealloc
{
    self.title = nil;
    self.imgRecord = nil;
    self.url = nil;
    self.content = nil;
    self.actor = nil;
    self.cate = nil;
    [super dealloc];
}

- (id)initWithDic:(NSDictionary *)aDic
{
    if ((self = [super init])) {
        if (aDic && [aDic count] > 0) {
            self.title = [aDic objectForKey:@"title"];
            self.url = [NSURL URLWithString:[aDic objectForKey:@"url"]];
            self.content = [aDic objectForKey:@"content"];
            self.actor = [aDic objectForKey:@"actor"];
            self.cate = [aDic objectForKey:@"typename"];
            NSString *ssize = [aDic objectForKey:@"size"];
            self.size = [ssize longLongValue]; // 198352;
            
            imgRecord = [[LKImageRecord alloc] initWithUrl:[aDic objectForKey:@"image"]];
            return self;
        } else {
            self = nil;
            return nil;
        }
    }
    return nil;
}

#pragma mark - NSCoding

- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeInt64:size forKey:@"size"];
    [aCoder encodeInt64:recvedBytes forKey:@"recvedBytes"];
    [aCoder encodeFloat:downProgress forKey:@"downProgress"];
    [aCoder encodeObject:title forKey:@"title"];
    [aCoder encodeObject:url forKey:@"url"];
    [aCoder encodeObject:content forKey:@"content"];
    [aCoder encodeObject:actor forKey:@"actor"];
    [aCoder encodeObject:cate forKey:@"cate"];
    [aCoder encodeObject:imgRecord forKey:@"imgRecord"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if ((self = [super init])) {
        size = [aDecoder decodeInt64ForKey:@"size"];
        recvedBytes = [aDecoder decodeInt64ForKey:@"recvedBytes"];
        downProgress = [aDecoder decodeFloatForKey:@"downProgress"];
        title = [[aDecoder decodeObjectForKey:@"title"] retain];
        url = [[aDecoder decodeObjectForKey:@"url"] retain];
        content = [[aDecoder decodeObjectForKey:@"content"] retain];
        actor = [[aDecoder decodeObjectForKey:@"actor"] retain];
        cate = [[aDecoder decodeObjectForKey:@"cate"] retain];
        imgRecord = [[aDecoder decodeObjectForKey:@"imgRecord"] retain];
    }
    return self;
}
@end
