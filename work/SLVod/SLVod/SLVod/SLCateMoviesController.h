//
//  SLCateMoviesController.h
//  SLVod
//
//  Created by luke on 11-6-15.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKViewController.h"


@interface SLCateMoviesController : LKViewController {
    
    LKShadowTableView   *table;
    SLMovie             *theCate;
}

@property (nonatomic, retain) SLMovie *theCate;
@property (nonatomic, retain) IBOutlet LKShadowTableView    *table;
@end
