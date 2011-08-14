//
//  SLMovDetailController.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"
#import "ASIHTTPRequest.h"
#import "SLMovie.h"

// Notification string used for touches to the overlay view
extern NSString * const OverlayViewTouchNotification;

@interface SLMovDetailController : LKViewController <UITableViewDelegate, UITableViewDataSource, SLHotCellDelegate> {
    LKShadowTableView   *table;
    
    SLMovie             *mov;
    ASIHTTPRequest      *downReq;
}

@property (nonatomic, retain) ASIHTTPRequest                *downReq;
@property (nonatomic, retain) SLMovie                       *mov;
@property (nonatomic, retain) IBOutlet LKShadowTableView    *table;
@end
