//
//  propertyListViewController.h
//  propertyList
//
//  Created by wang luke on 4/30/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "URLCacheConnection.h"
#import "httpClient.h"

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#define kFilename @"data.plist"

@class NSData;


@interface propertyListViewController : UIViewController <URLCacheConnectionDelegate, UIAlertViewDelegate, httpClientDelegate> {
	IBOutlet UILabel* localLable;
	
	IBOutlet UITextField *field1;
	IBOutlet UITextField *field2;
	// 是否成功写入
	IBOutlet UILabel *home;
	IBOutlet UILabel *doc;
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
	IBOutlet UIButton *download;
	
	IBOutlet UIButton *cancel;
	IBOutlet URLCacheConnection *conn;
	
	UIButton *sms;
}
@property (nonatomic, retain) UILabel *localLable;
@property (nonatomic, assign) UIButton *sms;
- (IBAction) sendSMS:(id)sender;

- (void)setTip:(NSString *)isSuccess;
- (NSString *) getDocumentsDir;
// ???: retain or asign 该如何选择
@property (nonatomic, retain) UILabel *home;
@property (nonatomic, retain) UILabel *doc;
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
@property (nonatomic, retain) UIButton *cancel;
@property (nonatomic, retain) UIButton *download;

@property (nonatomic, retain) URLCacheConnection *conn;
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
+ (void)write:(const char *)buffer toFile:(NSString *)fileName;

- (BOOL)writePlist:(id)plist toFile:(NSString *)fileName;
- (id)plistFromFile:(NSString *)fileName;
- (BOOL)writeData:(NSData *)data toFile:(NSString *)fileName;
- (NSData *)dataFromFile:(NSString *)fileName;

///////////////
- (IBAction) onCancel:(id)sender;
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

- (NSData *) getNSDataFromFile:(NSString *)filepath;
- (NSData *) getNSDataFromURL:(NSString *)url;
- (NSData *) getNSDataFromBytes;
- (BOOL) write:(NSData *)data toDir:(NSString *)dir asFile:(NSString *)fileName;
- (BOOL) write:(NSData *)data toURL:(NSString *)url;
// 下载并显示
- (IBAction) startDownload:(id)sender;
/////////流
- (void) streamTests:(NSString *)fileURL;

// 线程
void LaunchThread();
void* PosixThreadMainRoutine(void* data);

@end

