//
//  SLDownloadManController.h
//  SLVod
//
//  Created by luke on 11-6-17.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"


@interface SLDownloadManController : LKViewController {
    LKShadowTableView *table;
    UISegmentedControl *seg;
    
    NSMutableArray *movsInDownloading;
    NSMutableArray *movsDownloaded;
}

@property (nonatomic, retain) NSMutableArray *movsInDownloading;
@property (nonatomic, retain) NSMutableArray *movsDownloaded;
@property (nonatomic, retain) IBOutlet LKShadowTableView *table;
@property (nonatomic, retain) UISegmentedControl *seg;

@end
