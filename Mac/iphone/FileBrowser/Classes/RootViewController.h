//
//  RootViewController.h
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

// 应用程序的根视图控制器为导航控制器，导航控制器还有自己的根视图控制器－－栈底的视图控制器
// 导航控制器的根视图控制器: 一个表视图
@interface RootViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource> {
	// 存放二级视图控制器的实例，并为表视图提供数据
	NSArray *controllers;
}
@property (nonatomic, retain) NSArray *controllers;
@end
