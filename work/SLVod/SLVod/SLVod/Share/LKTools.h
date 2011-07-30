//
//  LKTools.h
//  FFriends
//
//  Created by luke on 11-7-7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

NSString *docPath();
NSString *keyForURL(NSURL *url);

@interface LKTools : NSObject {
    
}

+ (NSString *)JsonStrFormat:(NSString *)oStr;

+ (NSString *)URLEncode:(NSString *)orgString;
+ (NSString *)URLDecode:(NSString *)origStr;
+ (NSString *)externUrlParams:(NSString *)urlPart withParams:(NSDictionary *)params;
+ (NSData *)postData:(NSDictionary *)params;

@end
