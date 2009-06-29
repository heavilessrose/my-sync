//
//  MyTableViewController.h
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

// 嵌入tabController
@interface MyTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource>{
	NSArray *tableData;
}

@property (nonatomic, retain) NSArray *tableData;
@end
