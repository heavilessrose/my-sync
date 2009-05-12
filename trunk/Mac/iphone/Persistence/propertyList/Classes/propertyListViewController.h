//
//  propertyListViewController.h
//  propertyList
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "URLCacheConnection.h"

#define kFilename @"data.plist"

@class NSData;


@interface propertyListViewController : UIViewController <URLCacheConnectionDelegate, UIAlertViewDelegate> {
	IBOutlet UITextField *field1;
	IBOutlet UITextField *field2;
	// 是否成功写入
	IBOutlet UILabel *written;
	IBOutlet UIButton *saveButton;
	IBOutlet UIButton *copyButton;
	IBOutlet UITextField *copyto;
	/////////////
	NSString *cacheDir; // 自己的cache路径
	NSString *cachedFilePath; // path to the cached image
	NSDate *fileDate; // cached image的文件最后修改时间
	NSMutableArray *urlArray; // URLCache.plist文件中所有的url
	NSError *error; // 指向所有的错误（始终指向最新的错误）
	
	IBOutlet UIImageView *imageView;
	IBOutlet UIActivityIndicatorView *activityIndicator;
	IBOutlet UIButton *display;
	IBOutlet UIButton *clear;
	IBOutlet UILabel *statusField;
	IBOutlet UILabel *dateField;
	IBOutlet UILabel *infoField;
}

- (void)setTip:(NSString *)isSuccess;

// ???: retain or asign 该如何选择
@property (nonatomic, retain) UITextField *field1;
@property (nonatomic, retain) UITextField *field2;

@property (nonatomic, retain) UIButton *saveButton;
@property (nonatomic, retain) UIButton *copyButton;
@property (nonatomic, retain) UILabel *written;
@property (nonatomic, retain) UITextField *copyto;

/////
@property (nonatomic, copy) NSString *cacheDir;
@property (nonatomic, copy) NSString *cachedFilePath;
@property (nonatomic, retain) NSDate *fileDate;
@property (nonatomic, retain) NSMutableArray *urlArray;

@property (nonatomic, retain) UIImageView *imageView;
@property (nonatomic, retain) UIActivityIndicatorView *activityIndicator;
@property (nonatomic, retain) UILabel *statusField;
@property (nonatomic, retain) UILabel *dateField;
@property (nonatomic, retain) UILabel *infoField;
@property (nonatomic, retain) UIButton *display;
@property (nonatomic, retain) UIButton *clear;

+ (NSString *)appDocumentsDir;
- (NSString *)dataFilePath;

// 程序退出时调用此方法将数据保存到属性列表文件
// !!!: 并非UIApplicationDelegate 的委托方法 
- (void)applicationWillTerminate:(NSNotification *)notification;
- (IBAction)save:(id)sender;
// 键盘Done事件处理
- (IBAction)fieldsDoneEditing:(id)sender;
// 点击背景关闭键盘
- (IBAction)backgroundClicked:(id)sender;

//test
- (IBAction)copy:(id)sender;
// 文件字节数
- (unsigned)fileLength:(FILE *)fp;
- (void)write:(const char *)buffer toFile:(NSString *)fileName;

- (BOOL)writePlist:(id)plist toFile:(NSString *)fileName;
- (id)plistFromFile:(NSString *)fileName;
- (BOOL)writeData:(NSData *)data toFile:(NSString *)fileName;
- (NSData *)dataFromFile:(NSString *)fileName;

///////////////

- (IBAction) onDisplayImage:(id)sender;
- (IBAction) onClearCache:(id)sender;

- (void) initImageView;
- (void) startAnimation;
- (void) stopAnimation;
- (void) buttonsEnabled:(BOOL)flag;
- (void) displayImageWithURL:(NSURL *)theURL;
- (void) displayCachedImage;
- (void) turnOffSharedCache;
- (void) initCache;
- (void) clearCache;
- (void) getFileModificationDate;

- (void)loadUrlRes;
@end

