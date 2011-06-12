//
//  SLHotController.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SLHotCell.h"

@class LKShadowTableView;
@interface SLHotController : LKViewController <UITableViewDelegate, UITableViewDataSource> {
    LKShadowTableView   *table;
    SLHotCell           *tmpCell;
}

@property (nonatomic, retain) IBOutlet SLHotCell            *tmpCell;
@property (nonatomic, retain) IBOutlet LKShadowTableView    *table;
@end
