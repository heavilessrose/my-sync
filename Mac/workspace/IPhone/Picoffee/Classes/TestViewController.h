//
//  TestViewController.h
//  Picoffee
//
//  Created by luke on 10-7-16.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ScrollShowView.h"

@interface TestViewController : UIViewController <ScrollShowViewPageDelegate> {

    NSArray *imgNameArr_test;
    ScrollShowView *scrollShow;
}

@property (nonatomic, retain) ScrollShowView *scrollShow;
@property (nonatomic, retain) NSArray *imgNameArr_test;
@end
