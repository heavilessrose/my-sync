//
//  SLUser.h
//  SLVod
//
//  Created by luke on 11-6-17.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKImageRecord.h"

@interface SLUser : NSObject {
    
    NSString *username;
    LKImageRecord *avatar;
}

@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) LKImageRecord *avatar;

- (id)initWithDic:(NSDictionary *)usrInfo;

@end
