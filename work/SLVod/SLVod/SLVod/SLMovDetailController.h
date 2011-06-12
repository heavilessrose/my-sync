//
//  SLMovDetailController.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"
#import "SLMovie.h"
#import "SLHotCell.h"
#import "SLMovInfoCell.h"


@interface SLMovDetailController : LKViewController {
    LKShadowTableView   *table;
    SLHotCell           *tmpHotCell;
    SLMovInfoCell       *tmpMovInfoCell;
    
    SLMovie             *mov;
}

@property (nonatomic, retain) SLMovie                       *mov;
@property (nonatomic, retain) IBOutlet SLMovInfoCell        *tmpMovInfoCell;
@property (nonatomic, retain) IBOutlet SLHotCell            *tmpHotCell;
@property (nonatomic, retain) IBOutlet LKShadowTableView    *table;
@end
