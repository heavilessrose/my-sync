//
//  FFSettings.h
//  FFriends
//
//  Created by luke on 11-7-7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#define FFSettings_Key          (@"FFSettings")
#define FFSettings_Path         ([docPath() stringByAppendingPathComponent:FFSettings_Key])


@interface FFSettings : NSObject <NSCoding> {
    BOOL        FirstRun;
    NSString    *ClientVersion;
    NSString    *username;
    NSString    *pass;
    NSString    *email;
    BOOL        logined;
}

@property (nonatomic, assign) BOOL        logined;
@property (nonatomic, retain) NSString    *email;
@property (nonatomic, assign) BOOL        FirstRun;
@property (nonatomic, retain) NSString    *ClientVersion;
@property (nonatomic, retain) NSString    *username;
@property (nonatomic, retain) NSString    *pass;

+ (FFSettings *)shareSettings;
+ (BOOL)isFirstRun;
- (BOOL)archiveSettings;

@end
