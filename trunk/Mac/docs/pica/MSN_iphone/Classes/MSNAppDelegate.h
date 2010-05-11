//
//  MSNAppDelegate.h
//  MSN
//
//  Created by  apple on 08-10-7.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UITabBarController+ColorManage.h"

#if 1
#define RETURN_WHEN_LESS_MEMORY {									\
natural_t awailMem = [MSNAppDelegate systemFreeMemory];			\
if ((awailMem - MIN_FREE_MEMORY_SIZE) <= 0)	return;				\
}
#else
#define RETURN_WHEN_LESS_MEMORY
#endif

//@class MSNViewController;
@class ListViewController;
@class ContactList;
@class ClientNetWorkController;
@class StrangerInfo;
@class ChatViewController;
@class ZwpController;
@class ChatViewData;
@class ImageLabel;
@class Advertisement;
@class ChatSelectController;
//@class UITabBarController_ColorManage;
@class pcExtSettingsViewController;

@interface MSNAppDelegate : NSObject <UIApplicationDelegate, UIAlertViewDelegate,UITabBarControllerDelegate> 
{
	UIWindow *window;

	BOOL sysDismiss;
	BOOL hasLogin;
	BOOL autoLoad;
	BOOL rememberPassword;
	BOOL firstLogin;
	BOOL needLogin;
	int errorCode;
	int currentChatIndex;
	NSString *myAccount;
	NSString *myPassword;
	NSString *myStatus;
	NSString* h3g_time;
	NSString* h3g_phone_number;
	StrangerInfo *stranger;
	NSArray *pressionStrings;
	UIAlertView *pressionPanel;
	NSMutableArray *alerts;
	NSMutableDictionary *infoDict;
	UIAlertView *logoutAlertView;
	UINavigationController *navigationController;
	UITabBarController_ColorManage *tabController;
	
	ZwpController *zwpController;
	ZwpController *standbyZwpController;
	ListViewController *listViewController;
	ChatViewController *chatViewController;
	ChatSelectController *chatSelectController;
	pcExtSettingsViewController *settingController;
	
	NSMutableDictionary *viewControllers;
	ClientNetWorkController *netWorkController;
	Advertisement *chatViewAdvertise;
	Advertisement *loginViewAdvertise;	
	NSTimer *messageAlertTimer;
	ImageLabel *messageAlertLabel;
	
	NSMutableArray *currentViewIndexArray;
	
	BOOL		shouldHideAlertMsg;
	NSNumber*	errCode;
	NSMutableArray* systemNotice;
	NSString*	updateURL;
	unsigned char*	deviceToken;
    
    NSMutableArray*         strangers;
}

+ (MSNAppDelegate*)globalAppDelegate;
+ (natural_t)systemFreeMemory;

- (UINavigationController*)chatViewNavigation;

- (void)initPressionPanel;
- (void)clearTmpDirectory;

- (NSDictionary*)viewControllers;
- (void)login;
- (void)logout;
- (void)refreshList;

- (void)restart;

- (void)showMyAlertView:(UIAlertView*)theAlert model:(BOOL)flag;
- (void)showLoginWindow;
- (void)saveAccountInfo;
- (void)checkSettingValue;
- (void)showLoginFlashView;
- (void)logoutWithNetworkConnected:(BOOL)isconnected;

- (void)performDismiss: (NSTimer *)timer;
- (void)showReceivedMessageAlert:(NSString*)theName  
						 message:(NSString*)theMessage;
- (ChatViewData*)findChatViewData:(NSString *)theJid 
										  sid:(NSString*)theSid;
- (ChatViewData*)findChatViewDatabyImid:(NSString *)theImid 
												sid:(NSString*)theSid;
- (ChatViewData*)findNextChatViewDataByJid: (NSString*)aJid
					   currentChatViewData: (ChatViewData*)aChatViewData;
- (ChatViewData*)findP2PSessionByJid: (NSString*)aJid
				 currentChatViewData: (ChatViewData*)aChatViewData;
- (ChatViewData*)findAnyChatViewDataByJid: (NSString*)aJid keyWords: (NSString*)aKeyWords;
- (ChatViewData*)findAnyChatViewDataByImid: (NSString*)anImid keyWords: (NSString*)aKeyWords;
- (NSString*)buildSidByJid: (NSString*)aJid sid: (NSString*)aSid;
- (BOOL)isExistSessionKeyOfJid: (NSString*)aJid sid: (NSString*)aSid;
- (void)setChatviewData:(ChatViewData*)theChatViewData forKey:(NSString*)theKey;
- (void)notifyNetworkReachabilityChanged: (NSNotification*)note;
- (void)notifyLoginButtonPressed: (NSNotification*)note;
- (void)notifyShowZwpView: (NSNotification*)note;
- (void)notifyShowContactList: (NSNotification*)note;
- (void)notifyShowChatView: (NSNotification*)note;
- (void)notifyShowChatViews: (NSNotification*)note;
- (void)notifyReceivedMessage: (NSNotification*)note;
- (void)notifyReceivedVoiceClip: (NSNotification*)note;
- (void)notifyReceivedFile: (NSNotification*)note;
- (void)notifyReceiveAction: (NSNotification*)note;
- (void)notifyReceiveStatus: (NSNotification*)note;
- (void)notifyReceiveData: (NSNotification*)note;

- (void)notifyGetSystemInfo: (NSNotification*)note;
- (void)notifyGetUpgradeInfo: (NSNotification*)note;

- (void)notifyNeedUpdateVersion: (NSNotification*)note;
- (void)notifyReceivedError: (NSNotification*)note;
- (void)notifyBeAddedToPerson: (NSNotification*)note;
- (void)notifyParticipantChanged: (NSNotification*)note;
- (void)notifyChatviewContactChanged: (NSNotification*)note;
- (void)notifyLogout: (NSNotification*)note;
- (void)notifyLogin: (NSNotification*)note;
- (void)notifyLoginFailed: (NSNotification*)note;
- (void)notifyH3GLoginFailed: (NSNotification*)note;
- (void)notifyH3GTime: (NSNotification*)note;
- (void)notifyH3GWIFIFailed: (NSNotification*)note;
- (void)notifynetworkDisabled: (NSNotification*)note;
- (void)notifyReceiveChatHeaderAd: (NSNotification*)note;
- (void)notifyReceiveLoginProgAd: (NSNotification*)note;
- (void)notifyUnregisterUser: (NSNotification*)aNotify;

- (int) getTheIndex:(NSString *)VCName;

- (unsigned char*)deviceToken;

- (void)clearChatViewController;

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) NSString *myAccount;
@property (nonatomic, retain) NSString *myPassword;
@property (nonatomic, retain) NSString *myStatus;
@property (nonatomic, retain) NSString *h3g_time;
@property (nonatomic, retain) NSString *h3g_phone_number;
@property (nonatomic, retain) UIAlertView *logoutAlertView;
@property (nonatomic, retain) NSMutableDictionary *infoDict;

@property (nonatomic, retain) StrangerInfo *stranger;
@property (nonatomic, retain) NSMutableArray *alerts;
@property (nonatomic, retain) ClientNetWorkController *netWorkController;
@property (nonatomic, retain) UINavigationController *navigationController;
@property (nonatomic, retain) UITabBarController_ColorManage *tabController;
@property (nonatomic, readonly) UIAlertView *pressionPanel;
@property (nonatomic, readonly) NSArray *pressionStrings;
@property (nonatomic, retain) Advertisement *chatViewAdvertise;
@property (nonatomic, retain) Advertisement *loginViewAdvertise;	
@property (nonatomic, retain) NSMutableArray *currentViewIndexArray;
@property (nonatomic, retain) ZwpController *standbyZwpController;
@property (nonatomic, retain) ZwpController *zwpController;
@property (nonatomic, retain) ChatSelectController *chatSelectController;
@property (nonatomic, retain) ChatViewController *chatViewController;
@property (nonatomic, retain) ListViewController* listViewController;
@property (nonatomic, retain) NSNumber*	errCode;
@property (nonatomic, retain) NSMutableArray* systemNotice;
@property (nonatomic, retain) NSString*	updateURL;
@property (nonatomic, retain) NSMutableArray* strangers;


@end

