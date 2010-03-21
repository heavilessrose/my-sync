//
//  URLModelResponse.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"

// HTTP response parsers的抽象基类, 用来从response创建一个具体实现(yahoo json or yahoo xml)的对象.
// 具体子类需实现相关方法.
@interface URLModelResponse : NSObject <TTURLResponse> {
    NSMutableArray *objects;
    NSUInteger totalObjectsAvailableOnServer;
}

@property (nonatomic, retain) NSMutableArray *objects; // read-only for clients, read-write for sub-classes
@property (nonatomic, readonly) NSUInteger totalObjectsAvailableOnServer;

+ (id)response;
- (NSString *)format;

@end
