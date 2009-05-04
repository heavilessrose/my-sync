//
//  SQLiteViewController.h
//  SQLite
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "/usr/include/sqlite3.h"

#define kFilename @"data.sqlite3" // 数据库文件名

@interface SQLiteViewController : UIViewController {
	IBOutlet UITextField *field1;
	IBOutlet UITextField *field2;
	IBOutlet UITextField *field3;
	IBOutlet UITextField *field4;
	
	// 实例变量，指向数据库
	sqlite3 *database;
}

// ???: retain or asign 该如何选择
@property (nonatomic, retain) UITextField *field1;
@property (nonatomic, retain) UITextField *field2;
@property (nonatomic, retain) UITextField *field3;
@property (nonatomic, retain) UITextField *field4;

- (NSString *)dataFilePath;

// 程序退出时调用此方法将数据保存到属性列表文件
// !!!: 并非UIApplicationDelegate 的委托方法 
- (void)applicationWillTerminate:(NSNotification *)notification;


@end

