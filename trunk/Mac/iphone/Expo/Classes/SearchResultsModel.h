//
//  SearchResultsModel.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"

// 基类, 处理一组SearchCountryResult的抽象.
// 在这里确定是哪个web service 和 joson or xml

typedef enum {
    SearchServiceYahoo,
    SearchServiceFlickr,
    SearchServiceDefault = SearchServiceYahoo
} SearchService;
extern SearchService CurrentSearchService;

typedef enum {
    SearchResponseFormatJSON,
    SearchResponseFormatXML,
    SearchResponseFormatDefault = SearchResponseFormatXML
} SearchResponseFormat;
extern SearchResponseFormat CurrentSearchResponseFormat;


#pragma mark -
// a remote search service.
@protocol SearchResultsModel <TTModel>

// A list of domain objects constructed by the model after parsing the web service's HTTP response. 
// In this case, it is a list of SearchResult objects.
@property (nonatomic, readonly) NSArray *results;
// The total number of results available on the server (but not necessarily downloaded) 
// for the current model configuration's search query.
@property (nonatomic, readonly) NSUInteger totalResultsAvailableOnServer;
// The keywords that will be submitted to the web service in order to do the actual image search (e.g. "green apple")
@property (nonatomic, retain) NSString *searchTerms;

// The designated initializer
- (id)initWithResponseFormat:(SearchResponseFormat)responseFormat;

@end

#pragma mark -
// 工厂方法, 用来初始化一个实际的SearchCountryResultsModel.
id<SearchResultsModel> CreateSearchModelWithCurrentSettings(void);
id<SearchResultsModel> CreateSearchModel(SearchService service, SearchResponseFormat responseFormat);
