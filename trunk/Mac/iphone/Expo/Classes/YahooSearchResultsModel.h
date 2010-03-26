//
//  YahooSearchResultsModel.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"
#import "SearchResultsModel.h"

@class URLModelResponse;

/* 用来表示查询的 "最终" 结果(还是会最后封装为一个SearchResultsPhotoSource给视图控制器), 分离查询结果与如何显示
     - 发送 HTTP query 到 Yahoo Image Search web service.
     - 记录recordset offset,从而可取得同一个query的更多结果.
     - 根据response类型(XML or JSON)分发到一个URLModelResponse具体实现来解析HTTP response.
	 - 得到解析后的一组URLModelResponse::Searchesult. 
 */
@interface YahooSearchResultsModel : TTURLRequestModel <SearchResultsModel>
{
    URLModelResponse *responseProcessor;
    NSString *searchTerms;
    NSUInteger recordOffset;
}

// 初始化方法定义在SearchResultsModel protocol.

@end
