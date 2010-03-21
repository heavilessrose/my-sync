//
//  SearchResult.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"

// 表示一条国家搜索记录.
// 当执行搜索之后, 包含一组该对象的TTModel被载入.
@interface SearchResult : NSObject {
    NSString *title;
    NSString *bigImageURL;
    NSString *thumbnailURL;
	NSString *txtInfo; // 搜索出的国家相关文字信息
    CGSize bigImageSize;
}

@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *bigImageURL;
@property (nonatomic, retain) NSString *thumbnailURL;
@property (nonatomic, retain) NSString *txtInfo;
@property (nonatomic, assign) CGSize bigImageSize;

@end
