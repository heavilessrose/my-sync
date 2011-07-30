//
//  SLDownMovie.h
//  SLVod
//
//  Created by luke on 11-7-30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

@class SLMovie, ASIHTTPRequest;
@interface SLDownMovie : NSObject
{
    int         downloadPercent;
    NSString    *hash;
    long long   *size;
    NSString    *path;
    SLMovie     *movie;
    ASIHTTPRequest *movieReq;
}


@property (nonatomic, assign) ASIHTTPRequest *movieReq;
@property (nonatomic, retain) SLMovie     *movie;
@property (nonatomic, assign) int         downloadPercent;
@property (nonatomic, retain) NSString    *hash;
@property (nonatomic, assign) long long   *size;
@property (nonatomic, retain) NSString    *path;

- (id)initWithMovie:(SLMovie *)mov req:(ASIHTTPRequest *)req;

@end
