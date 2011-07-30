//
//  LKTools.m
//  FFriends
//
//  Created by luke on 11-7-7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKTools.h"


NSString *docPath() {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *docDirPath = [paths objectAtIndex:0];
	return docDirPath;
}

@implementation LKTools


+ (NSString *)JsonStrFormat:(NSString *)oStr{
	oStr = [oStr stringByReplacingOccurrencesOfString:@"\r" withString:@""];
	oStr = [oStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
	oStr = [oStr stringByReplacingOccurrencesOfString:@"\t" withString:@""];
	oStr = [oStr stringByReplacingOccurrencesOfString:@",]" withString:@"]"];
	oStr = [oStr stringByReplacingOccurrencesOfString:@"[," withString:@"["];
	oStr = [oStr stringByReplacingOccurrencesOfString:@"\\" withString:@"#XG"];
	return oStr;
}

#pragma mark - url
// 适用于charset为utf-8的情况。
+ (NSString *)URLEncode:(NSString *)orgString
{
#if 1
	NSString *newString = NSMakeCollectable([(NSString *)CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault, (CFStringRef)orgString, NULL, CFSTR(":/?#[]@!$ &'()*+,;=\"<>%{}|\\^~`"), CFStringConvertNSStringEncodingToEncoding(NSUTF8StringEncoding)) autorelease]);
#else
	NSString *newString = [(NSString *)CFURLCreateStringByAddingPercentEscapes(kCFAllocatorNull, (CFStringRef)orgString, NULL, NULL, CFStringConvertNSStringEncodingToEncoding(NSUTF8StringEncoding)) autorelease];
#endif
	if (newString) {
		return newString;
	}
	return @"";
}

+ (NSString *)URLDecode:(NSString *)origStr
{
	NSString *filtered = [origStr stringByReplacingOccurrencesOfString:@"+" withString:@" "];
	NSString *res = [filtered stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	return res;
}

+ (NSString *)externUrlParams:(NSString *)urlPart withParams:(NSDictionary *)params
{
    NSMutableString *extParams = [NSMutableString string];
    BOOL plused = NO;
    if (params && [params count] > 0) {
        for (NSString *k in [params allKeys]) {
            if (plused) {
                [extParams appendFormat:@"&"];
            }
            [extParams appendFormat:@"%@=%@", k, [params objectForKey:k]];
            plused = YES;
        }
    }
    NSString *urlParam = [NSString stringWithFormat:@"%@?%@", urlPart, extParams];
    return urlParam;
}

+ (NSData *)postData:(NSDictionary *)params
{
    NSMutableString *extParams = [NSMutableString string];
    BOOL plused = NO;
    if (params && [params count] > 0) {
        for (NSString *k in [params allKeys]) {
            if (plused) {
                [extParams appendFormat:@"&"];
            }
            if ([k isEqualToString:@"name"]) {
                [extParams appendFormat:@"%@=%@", k, [LKTools URLEncode:[params objectForKey:k]]];
            } else {
                [extParams appendFormat:@"%@=%@", k, [params objectForKey:k]];
            }
            plused = YES;
        }
    }
    DLog(@"%@", extParams);
    NSData *postData = [extParams dataUsingEncoding:NSUTF8StringEncoding];
    return postData;
}

@end
