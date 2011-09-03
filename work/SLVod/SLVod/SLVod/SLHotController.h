//
//  SLHotController.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"
#import "EGORefreshTableHeaderView.h"

@interface SLHotController : LKViewController <UITableViewDelegate, UITableViewDataSource, UISearchBarDelegate, UISearchDisplayDelegate, EGORefreshTableHeaderDelegate, LKMoreCellDelegate> {
    LKShadowTableView   *table;
    EGORefreshTableHeaderView *_refreshHeaderView;
    BOOL _reloading;
}

- (void)reloadTableViewDataSource;
- (void)doneLoadingTableViewData;

@property (nonatomic, retain) IBOutlet LKShadowTableView    *table;

@end
