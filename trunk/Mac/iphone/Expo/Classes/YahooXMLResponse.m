//
//  YahooXMLResponse.m
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "YahooXMLResponse.h"
#import "SearchResult.h"
#import "./Support/DDXMLDocument.h"

@implementation YahooXMLResponse

#pragma mark TTURLResponse

- (NSError*)request:(TTURLRequest*)request processResponse:(NSHTTPURLResponse*)response data:(id)data
{
    NSError *error = nil;
    
    // 解析XML文档.
    DDXMLDocument *doc = [[[DDXMLDocument alloc] initWithData:data options:0 error:&error] autorelease];
    NSAssert(doc, @"Failed to parse XML. The document is nil.");
    
	// 手动的设置namespace. (只有这样XPath queries才工作,不是很了解KissXML)
    DDXMLElement *root = [doc rootElement];
    [root addNamespace:[DDXMLNode namespaceWithName:@"foo" stringValue:@"urn:yahoo:srchmi"]];
	
    // 根据Yahoo Image Search API 查询XML树.
    NSArray *titles = [root nodesForXPath:@"//foo:Title" error:&error];
    NSArray *bigImageURLs = [root nodesForXPath:@"//foo:Result/foo:Url" error:&error];
    NSArray *bigImageWidths = [root nodesForXPath:@"//foo:Result/foo:Width" error:&error];
    NSArray *bigImageHeights = [root nodesForXPath:@"//foo:Result/foo:Height" error:&error];
    NSArray *thumbnailURLs = [root nodesForXPath:@"//foo:Result/foo:Thumbnail/foo:Url" error:&error];
    totalObjectsAvailableOnServer = [[[[root nodesForXPath:@"//foo:ResultSet/@totalResultsAvailable" error:&error] lastObject] stringValue] integerValue];
    
    NSAssert1(!error, @"XML Parse error: %@", error);
    NSAssert([titles count] == [bigImageURLs count] && [titles count] == [thumbnailURLs count], 
             @"XPath error: the quantity of the data retrieved does not match.");
    
    // 将results封装为一组SearchResult.
    for (NSUInteger i = 0; i < [titles count]; i++) {
        SearchResult *result = [[[SearchResult alloc] init] autorelease];
        result.title = [[titles objectAtIndex:i] stringValue];
        result.bigImageURL = [[bigImageURLs objectAtIndex:i] stringValue];
        result.thumbnailURL = [[thumbnailURLs objectAtIndex:i] stringValue];
        result.bigImageSize = CGSizeMake([[[bigImageWidths objectAtIndex:i] stringValue] intValue], 
                                         [[[bigImageHeights objectAtIndex:i] stringValue] intValue]);
        [self.objects addObject:result];
    }
    
    return nil;
}

- (NSString *)format { return @"xml"; }

@end
