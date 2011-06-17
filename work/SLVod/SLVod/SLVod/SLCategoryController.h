//
//  SLCategoryController.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"


@interface SLCategoryController : LKViewController <UITableViewDelegate, UITableViewDataSource> {
    LKShadowTableView   *table;
}

@property (nonatomic, retain) IBOutlet LKShadowTableView   *table;

@end
