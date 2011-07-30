//
//  SLDownMovie.h
//  SLVod
//
//  Created by luke on 11-7-30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLMovie.h"

@interface SLDownMovie : SLMovie
{
    int         downloadPercent;
    NSString    *hash;
    long long   *size;
    NSString    *path;
}


@property (nonatomic, assign) int         downloadPercent;
@property (nonatomic, retain) NSString    *hash;
@property (nonatomic, assign) long long   *size;
@property (nonatomic, retain) NSString    *path;

@end
