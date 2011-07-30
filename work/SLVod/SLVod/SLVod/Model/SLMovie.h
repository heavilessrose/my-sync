//
//  SLMovie.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKImageRecord.h"

@interface SLMovie : NSObject {
    NSString    *title;
//    NSString    *imageUrl;
    NSURL       *url;
    NSString    *content;
    NSString    *actor;
    NSString    *cate;
    long long   size;
    
    LKImageRecord   *imgRecord;
    
    long long   recvedBytes;
    float       downProgress;
}

@property (nonatomic, assign) long long   recvedBytes;
@property (nonatomic, assign) long long     size;
@property (nonatomic, assign) float       downProgress;
@property (nonatomic, retain) LKImageRecord   *imgRecord;
@property (nonatomic, retain) NSString    *title;
//@property (nonatomic, retain) NSString    *imageUrl;
@property (nonatomic, retain) NSURL       *url;
@property (nonatomic, retain) NSString    *content;
@property (nonatomic, retain) NSString    *actor;
@property (nonatomic, retain) NSString    *cate;

- (id)initWithDic:(NSDictionary *)aDic;

@end
