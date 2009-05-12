//
//  SimpleTableViewController.h
//  SimpleTable
//
//  Created by wang luke on 5/11/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SimpleTableViewController : UIViewController <UITableViewDelegate, UITableViewDataSource> {
	NSArray *listData;
}

@property (nonatomic, retain)NSArray *listData;

@end

