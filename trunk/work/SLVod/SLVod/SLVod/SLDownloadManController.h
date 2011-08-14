//
//  SLDownloadManController.h
//  SLVod
//
//  Created by luke on 11-6-17.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"
#import "SLHotCell.h"
#import "ASIHTTPRequest.h"
#import "ASIProgressDelegate.h"
#import "ASIHTTPRequestDelegate.h"
#import "ASICacheDelegate.h"
#import "ASINetworkQueue.h"

#define downingPlist    ([docPath() stringByAppendingPathComponent:@"movsInDownloading.plist"])
#define downedPlist     ([docPath() stringByAppendingPathComponent:@"movsDownloaded.plist"])

typedef enum {
    SLDownloadMovsType_downing = 0,
    SLDownloadMovsType_downed,
} SLDownloadMovsType;

@interface SLDownloadManController : LKViewController <SLMovDownloadDelegate, ASIProgressDelegate, ASIHTTPRequestDelegate, ASICacheDelegate> {
    LKShadowTableView *table;
    UISegmentedControl *seg;
    int segSelected;
    
    NSMutableArray *movsInDownloading;
    NSMutableArray *movsDownloaded;
    
    ASINetworkQueue *downingQueue;
}

@property (nonatomic, retain) NSMutableArray *movsInDownloading;
@property (nonatomic, retain) NSMutableArray *movsDownloaded;
@property (nonatomic, retain) IBOutlet LKShadowTableView *table;
@property (nonatomic, retain) UISegmentedControl *seg;
@property (nonatomic, retain) ASINetworkQueue *downingQueue;

@end
