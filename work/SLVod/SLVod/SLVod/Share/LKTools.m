//
//  LKTools.m
//  FFriends
//
//  Created by luke on 11-7-7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKTools.h"
#import "WBCityModel.h"
#import "WBXML.h"


NSString *docPath() {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *docDirPath = [paths objectAtIndex:0];
	return docDirPath;
}

@implementation LKTools

+ (NSArray *)xmlListForModel:(WBModelID)modelID xmlfile:(NSString *)filename 
                  withXPath:(NSString *)xpath withInfo:(id)additionInfo
{
	NSData *xmlData = [[NSMutableData alloc] initWithContentsOfFile:filename];
	NSArray *modelArray = XMLXPathQueryForModel(modelID, xmlData, xpath, additionInfo);
	[xmlData release];
	return modelArray;
}

+ (NSArray *)loadCitysAndHotCitys:(NSMutableArray *)hotCitys
{
    NSMutableArray *provinces = [NSMutableArray array];
    
	NSString *provinceXmlPath = [[NSBundle mainBundle] pathForResource:@"pro" ofType:@"xml"];
	NSData *provinceData = [NSData dataWithContentsOfFile:provinceXmlPath];
	NSArray *proArray = XMLXPathQueryForModel(WBModelID_Province, provinceData, @"/root/pro", nil);
    
    if (proArray && [proArray count] > 0) {
        NSMutableArray *tmpHotCitys = [NSMutableArray array];
        NSString *cityXmlPath = [[NSBundle mainBundle] pathForResource:@"city" ofType:@"xml"];
        for (WBProvinceModel *aProModel in proArray) {            
//            NSString *dirname = aProModel.name;
            NSString *proid = aProModel.ID;
            
            NSString *xpath =[NSString stringWithFormat:@"//root//city[@proid=\"%@\"]", proid];
            NSArray *aCityArr = [LKTools xmlListForModel:WBModelID_City xmlfile:cityXmlPath withXPath:xpath withInfo:aProModel];
            aProModel.cityArr = aCityArr;
            for (WBCityModel *aCityModel in aCityArr) {
                if (aCityModel.hot && ![aCityModel.hot isEqualToString:@"0"]) {
                    [tmpHotCitys addObject:aCityModel];
                }
            }
            
            [provinces addObject:aProModel];
        }
        
        // hot sort
        NSSortDescriptor *alphaDesc = [[NSSortDescriptor alloc] initWithKey:@"hot" ascending:YES selector:@selector(localizedCaseInsensitiveCompare:)]; 
        
        NSArray *sortedHotCitys = [tmpHotCitys sortedArrayUsingDescriptors:[NSArray arrayWithObjects:alphaDesc, nil]]; 
        [hotCitys removeAllObjects];
        [hotCitys addObjectsFromArray:sortedHotCitys];
        [alphaDesc release];
        alphaDesc = nil;
    }
    
    return provinces;
}

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
