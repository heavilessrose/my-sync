//
//  YahooJSONResponse.m
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "YahooJSONResponse.h"
#import "SearchResult.h"
#import "./Support/JSON/JSON.h"

@implementation YahooJSONResponse

- (NSError*)request:(TTURLRequest*)request processResponse:(NSHTTPURLResponse*)response data:(id)data
{
    NSString *responseBody = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    // 解析从服务器得到的JSON数据.
    NSDictionary *json = [responseBody JSONValue];
    [responseBody release];
    
    NSDictionary *resultSet = [json objectForKey:@"ResultSet"];
    totalObjectsAvailableOnServer = [[resultSet objectForKey:@"totalResultsAvailable"] integerValue];
	
    // 封装results为SearchResult.
    NSArray *results = [resultSet objectForKey:@"Result"];
    for (NSDictionary *rawResult in results) {
        SearchResult *result = [[[SearchResult alloc] init] autorelease];
        result.title = [rawResult objectForKey:@"Title"];
        result.bigImageURL = [rawResult objectForKey:@"Url"];
        result.thumbnailURL = [rawResult valueForKeyPath:@"Thumbnail.Url"];
        result.bigImageSize = CGSizeMake([[rawResult objectForKey:@"Width"] intValue], 
                                         [[rawResult objectForKey:@"Height"] intValue]);
        [self.objects addObject:result];
    }
    
    return nil;
}

- (NSString *)format { return @"json"; }

@end
