//
//  SLLoginController.h
//  SLVod
//
//  Created by luke on 11-8-14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKViewController.h"
#import "LKShadowTableView.h"

@interface SLLoginController : LKViewController {
    LKShadowTableView *table;
}

@property (nonatomic, retain) IBOutlet LKShadowTableView *table;

@end
