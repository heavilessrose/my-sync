//
//  propertyListViewController.h
//  propertyList
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

#define kFilename @"data.plist"
@interface propertyListViewController : UIViewController {
	IBOutlet UITextField *field1;
	IBOutlet UITextField *field2;
	IBOutlet UITextField *field3;
	IBOutlet UITextField *field4;
	// 是否成功写入
	IBOutlet UILabel *written;
	IBOutlet UIButton *saveButton;
}
- (void)setTip:(NSString *)isSuccess;

// ???: retain or asign 该如何选择
@property (nonatomic, retain) UITextField *field1;
@property (nonatomic, retain) UITextField *field2;
@property (nonatomic, retain) UITextField *field3;
@property (nonatomic, retain) UITextField *field4;

@property (nonatomic, retain) UIButton *saveButton;
@property (nonatomic, retain) UILabel *written;

- (NSString *)dataFilePath;

// 程序退出时调用此方法将数据保存到属性列表文件
// !!!: 并非UIApplicationDelegate 的委托方法 
- (void)applicationWillTerminate:(NSNotification *)notification;
- (IBAction)save:(id)sender;

//test
- (void)copy:(NSString *)source dest:(NSString *)dest;
- (void)write;
@end

