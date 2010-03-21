//
//  YahooSearchResultsModel.m
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "YahooSearchResultsModel.h"
#import "YahooJSONResponse.h"
#import "YahooXMLResponse.h"
#import "./Support/GTMNSDictionary+URLArguments.h"

// 每次请求拿下的result数
const static NSUInteger kYahooBatchSize = 16;

@implementation YahooSearchResultsModel

@synthesize searchTerms;

- (id)initWithResponseFormat:(SearchResponseFormat)responseFormat;
{
    if ((self = [super init])) {
        switch ( responseFormat ) {
            case SearchResponseFormatJSON:
                responseProcessor = [[YahooJSONResponse alloc] init];
                break;
            case SearchResponseFormatXML:
                responseProcessor = [[YahooXMLResponse alloc] init];
                break;
            default:
                [NSException raise:@"SearchResponseFormat unknown!" format:nil];
        }
        recordOffset = 1; // Yahoo API offset从1开始.
    }
    return self;
}

- (id)init
{
    return [self initWithResponseFormat:CurrentSearchResponseFormat];
}

- (void)load:(TTURLRequestCachePolicy)cachePolicy more:(BOOL)more
{
    if (!searchTerms) {
        TTDWARNING(@"No search terms specified. Cannot load the model resource.");
        return;
    }
    
    if (more)
        recordOffset += kYahooBatchSize;
    else
        [responseProcessor.objects removeAllObjects]; // 清除上一次请求的数据.
    
    NSString *offset = [NSString stringWithFormat:@"%lu", (unsigned long)recordOffset];
    NSString *batchSize = [NSString stringWithFormat:@"%lu", (unsigned long)kYahooBatchSize];
    
    // request.
    NSString *host = @"http://search.yahooapis.com";
    NSString *path = @"/ImageSearchService/V1/imageSearch";
    NSDictionary *parameters = [NSDictionary dictionaryWithObjectsAndKeys:
                                searchTerms, @"query",
                                @"YahooDemo", @"appid",
                                [responseProcessor format], @"output",
                                offset, @"start",
                                batchSize, @"results",
                                nil];
	
    NSString *url = [host stringByAppendingFormat:@"%@?%@", path, [parameters gtm_httpArgumentsString]];
    TTURLRequest *request = [TTURLRequest requestWithURL:url delegate:self];
    request.cachePolicy = cachePolicy;
    request.response = responseProcessor;
    request.httpMethod = @"GET";
    
    // 分发request.
    [request send];
}

- (void)reset
{
    [super reset];
    [searchTerms release];
    searchTerms = nil;
    recordOffset = 1;
    [[responseProcessor objects] removeAllObjects];
}

- (void)setSearchTerms:(NSString *)theSearchTerms
{
    if (![theSearchTerms isEqualToString:searchTerms]) {
        [searchTerms release];
        searchTerms = [theSearchTerms retain];
        recordOffset = 1;
    }
}

- (NSArray *)results
{
    return [[[responseProcessor objects] copy] autorelease];
}

- (NSUInteger)totalResultsAvailableOnServer
{
    return [responseProcessor totalObjectsAvailableOnServer];
}

- (void)dealloc
{
    [searchTerms release];
    [responseProcessor release];
    [super dealloc];
}


@end
