//
//  MSNAppDelegate.m
//  MSN
//
//  Created by  apple on 08-10-7.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "MSNAppDelegate.h"
#import "ClientNetWorkController.h"
#import "NetWorkReachability.h"
#import "LoginViewController.h"
#import "LoginFlashViewController.h"
#import "ListViewController.h"
#import "ChatSelectController.h"
#import "ChatViewController.h"
#import "ChatViewData.h"
#import "ImageLabel.h"
#import "ContactList.h"
#import "GroupInfo.h"
#import "PersonInfo.h"
#import "StrangerInfo.h"
#import "Advertisement.h"
#import "ZwpController.h"
#import "define.h"
#import "API.h"
#import "SayHiView.h"
#import "OnlineListController.h"
#import "pcExtSettingsViewController.h"
#import <mach/mach.h>
#import <mach/mach_host.h>

NSString *kMessageAlertKey			= @"MessageAlert";
NSString *kOnLineAlertKey			= @"OnLineAlert";
NSString *kNotifyNudgeKey			= @"NotifyNudge";
NSString *kShowBigImageKey			= @"ShowBigImage";
NSString *kDisplayOnLinesKey		= @"DisplayOnlines";
NSString *kSaveConversationKey		= @"SaveConversation";
NSString *DismissAlertViewNotification = @"DismissAlertView";
NSString *MClubOnlineNotification = @"MClubOnlineNotification";

enum {
	ALERT_REASON_FOR_NONE,
	ALERT_REASON_FOR_REFRESH_BY_IF_SHOW_BIG_IAMGE,
};

@interface pcAlertAndStrangerInfoObject : NSObject{
    UIAlertView*            alertObject;
    StrangerInfo*           strangerObject;
}
@property (nonatomic, assign)   UIAlertView*    alertObject;
@property (nonatomic, retain)   StrangerInfo*   strangerObject;

@end

@implementation pcAlertAndStrangerInfoObject
@synthesize alertObject;
@synthesize strangerObject;

- (id) init{
    self = [super init];
    if (self != nil) {
        alertObject = nil;
        strangerObject = nil;
    }
    return self;
}


- (void) dealloc{
    alertObject = nil;
    [strangerObject release];
    [super dealloc];
}

@end

@implementation MSNAppDelegate

@synthesize window,navigationController, netWorkController, infoDict, myAccount, myPassword, myStatus, stranger, logoutAlertView,alerts;
@synthesize pressionStrings, chatViewAdvertise, loginViewAdvertise;
@synthesize tabController;
@synthesize currentViewIndexArray;
@synthesize zwpController;
@synthesize standbyZwpController;
@synthesize chatSelectController;
@synthesize chatViewController;
@synthesize listViewController;
@synthesize errCode;
@synthesize systemNotice;
@synthesize updateURL;
@synthesize h3g_time, h3g_phone_number;
@synthesize strangers;

+ (MSNAppDelegate*)globalAppDelegate{
	return (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
}

+ (natural_t)systemFreeMemory{
	mach_port_t host_port;
	mach_msg_type_number_t host_size;
	vm_size_t pagesize;
	host_port = mach_host_self();
	host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
	host_page_size(host_port, &pagesize);
	vm_statistics_data_t vm_stat;
	if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
		return 0;
	}
	return (vm_stat.free_count * pagesize);
}

- (UINavigationController*)chatViewNavigation{
	NSArray* aControllers = [tabController viewControllers];
	UIViewController* aChatViewNavigation = [aControllers objectAtIndex: 2];
	return [aChatViewNavigation isKindOfClass: [UINavigationController class]] ? (UINavigationController*)aChatViewNavigation : nil;
}

- (BOOL)containsTabControllerViewinView: (UIView*)aView{
    BOOL aFound = NO;
	NSArray* aSubViews = [aView subviews];
	for (UIView* aSubview in aSubViews){
        if (aSubview == tabController.view){
            aFound = YES;
            break;
        }
        else {
            aFound = [self containsTabControllerViewinView: aSubview];
            if (aFound) break;
        }
	}
    return aFound;
}

- (BOOL)findListViewTabSelectPageView: (UIView*)aView{
	BOOL aFound = NO;
	NSArray* aSubViews = [aView subviews];
	for (UIView* aSubview in aSubViews){
		if ([aSubview isKindOfClass: [UITableView class]] &&
			aSubview == ((ContactList*)[ContactList getInstance]).listView){
			aFound = YES;
            break;
		}
		else {
			aFound = [self findListViewTabSelectPageView: aSubview];
			if (aFound) break;
		}
	}
	return aFound;
}

- (void)garbage{
	// Ugly begin. I don't know how to resolve this wird! odd bug, 
	// so, ugly code vs. odd bug.
	UIDevice *theDevice = [UIDevice currentDevice];
	if ([[theDevice systemVersion] compare:@"2.0"] > NSOrderedSame)
	{
		int MAGIC_NO = 4;
		
		for (UIViewController *theVC in tabController.viewControllers)
		{
			while ([theVC retainCount] > MAGIC_NO)
			{
				[theVC release];
			}
			while ([theVC retainCount] < MAGIC_NO) {
				[theVC retain];
			}
		}
	}
	// Ugly end..
	[self saveAccountInfo];
	
	[Advertisement saveImage];
	
	if ([chatViewController retainCount] > 1){
		[chatViewController garbage];
		[chatViewController release];
	}
	else {
		self.chatViewController = nil;
	}
	
	BOOL aFound = [self findListViewTabSelectPageView: tabController.view];
    if ([self containsTabControllerViewinView: window] && !aFound){
        while ([listViewController retainCount] > 1) {
            [listViewController release];
        }
    }
	[listViewController release];
	listViewController = nil;
	
	chatSelectController.chatViewDatas = nil;
	[chatSelectController release];
	chatSelectController = nil;
	
	[settingController release];
	settingController = nil;
	
	[zwpController release];
	zwpController = nil;
	
	[tabController.view removeFromSuperview];
	self.tabController = nil;
	
	[viewControllers release];
	viewControllers = nil;
	
	self.infoDict = nil;
}

- (void)dismissSettingsAlertView{
	UIViewController* aController = tabController.selectedViewController;
	if ([aController isKindOfClass: [UINavigationController class]]){
		UIViewController* theController = [(UINavigationController*)aController topViewController];
		if ([theController isKindOfClass: [pcExtSettingsViewController class]]){
			[(pcExtSettingsViewController*)theController forseDismissAlertview];
		}
	}
}

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		hasLogin = NO;
		sysDismiss = YES;
		firstLogin = YES;
		needLogin = NO;
		rememberPassword = NO;
		autoLoad = YES;
		currentChatIndex = 0;
		navigationController = nil;
		messageAlertLabel = nil;
		messageAlertTimer = nil;
		tabController = nil;
		currentViewIndexArray = [[NSMutableArray alloc] init];
		self.alerts = [NSMutableArray arrayWithCapacity:0];
		deviceToken = NULL;
		pressionStrings = [[NSArray alloc] initWithObjects:
							 @":),:-)",@":d,:D,:-d,:-D",@";),;-)",@":o,:O,:-o,:-O",@":p,:P,:-p,:-P",@"(h),(H)",@":@,:-@",@":s,:S,:-s,:-S",@":$,:-$",@":(,:-(",@":'(",
							 @":|,:-|",@"(a),(A)",@"8o|",@"8-|",@"+o(",@"<:o)",@"|-)",@"*-)",@":-#",@":-*",@"^o)",@"8-)",@"(l),(L)",
							 @"(u),(U)",@"(m),(M),*9mil",/*,*red+u,*unicef,*wwf,*oxfam,*one,*care,*mssoc,*help,*hsus,*bgca,*sierra,*acs,*mod,*naf*/
							 @"(@)",@"(&)",@"(sn)",@"(bah)",@"(S)",@"(*)",@"(#)",@"(r),(R)",@"({)",@"(})",@"(k),(K)",@"(f),(F)",@"(w),(W)",@"(o),(O),(0)",nil];
        
        strangers = [[NSMutableArray alloc] init];
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		[nc addObserver: self
			   selector: @selector(notifyNetworkReachabilityChanged:) 
				   name: NetworkReachabilityChangedNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyLoginFailed:) 
				   name: LoginFaildNotification 
				 object: nil];

		[nc addObserver: self
			   selector: @selector(notifyH3GLoginFailed:) 
				   name: H3GLoginFaildNotification 
				 object: nil];
		
		[nc addObserver: self 
			   selector: @selector(notifyH3GWIFIFailed:) 
				   name: H3GWIFIFaildNotification 
				 object: nil];

		[nc addObserver: self 
			   selector: @selector(notifyH3GTime:) 
				   name: H3GWIFITimeNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifynetworkDisabled:) 
				   name: NetWorkDisabledNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyGetSystemInfo:) 
				   name: GetSystemInfoNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyGetUpgradeInfo:) 
				   name: GetUpgradeInfoNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyNeedUpdateVersion:) 
				   name: NeedUpdateNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyLogout:) 
				   name: LogoutNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyLogin:) 
				   name: LoginNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyBeAddedToPerson:) 
				   name: BeAddedRequestNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyLoginButtonPressed:) 
				   name: LoginActionNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyShowZwpView:) 
				   name: ShowZwpViewNotification 
				 object: nil];

		[nc addObserver: self
			   selector: @selector(notifyShowStandbyZwpView:) 
				   name: ShowStandbyZwpViewNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyShowContactList:) 
				   name: ShowContactListNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyShowChatView:) 
				   name: ShowChatViewNotification 
				 object: nil];
				
		[nc addObserver: self
			   selector: @selector(notifyShowChatViews:) 
				   name: ShowChatViewsNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifySayHiView:) 
				   name: SayHiViewNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceivedMessage:) 
				   name: ReceiveMessageNotification 
				 object: nil];
	
		[nc addObserver: self
			   selector: @selector(notifyParticipantChanged:) 
				   name: ParticipantChangedNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(notifyChatviewContactChanged:) 
				   name: ChangeChatviewContactNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(notifyReceivedVoiceClip:) 
				   name: ReceiveVoiceNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceivedFile:) 
				   name: ReceiveFileNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceiveAction:) 
				   name: ReceiveActionNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceiveStatus:) 
				   name: ReceiveStatusNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceiveData:) 
				   name: ReceiveDataNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceiveChatHeaderAd:) 
				   name: ChatHeaderAdNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReceiveLoginProgAd:) 
				   name: LoginProgAdNotification 
				 object: nil];

		[nc addObserver: self
			   selector: @selector(notifyReceivedError:) 
				   name: ReceivedErrorNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyUnregisterUser:) 
				   name: kLoginFaildOfUnregisterUserNotify
				 object: nil];
		
		[nc addObserver: self 
			   selector:@selector(notifyUpdateMessage)
				   name:ShowUpdateMessageNotification 
				 object:nil];
	}
	return self;
}


- (void)dealloc 
{
	[alerts release];
	[myPassword release];
	[myStatus release];
	[messageAlertLabel release];
	[infoDict release];
	[stranger release];
	[logoutAlertView release];
	[navigationController release];
	[listViewController release];
	[chatViewController release];
	[zwpController release];
	[standbyZwpController release];
	[tabController release];
	[viewControllers release];
	[pressionStrings release];
	pressionPanel.delegate = nil;
	[pressionPanel release];
	[chatViewAdvertise release];
	[loginViewAdvertise release];
	[netWorkController release];
	[PersonInfo releaseImages];
	[Advertisement saveImage];
	[self clearTmpDirectory];
	[currentViewIndexArray release];
	[chatSelectController release];
	[window release];
	[myAccount release];
	[errCode release];
	[systemNotice release];
	[h3g_time release];
	[h3g_phone_number release];
    
    [strangers release];

	if (deviceToken != NULL)
		free(deviceToken);
	deviceToken = NULL;
	[super dealloc];
}

#pragma mark -
#pragma mark utilites.

- (int) getTheIndex:(NSString *)VCName
{
	return [currentViewIndexArray indexOfObject:VCName];
}

- (unsigned char*)deviceToken{
	return deviceToken;
}

- (void)clearChatViewController{
    chatViewController = nil;
}

- (void)clearTmpDirectory
{
	NSString *path = NSTemporaryDirectory();
	NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path error:nil];
	NSString *file = nil;
	for (file in files)
	{
		[[NSFileManager defaultManager] removeItemAtPath:[path stringByAppendingPathComponent:file] error:nil];
	}
}

- (NSDictionary*)viewControllers
{
	return viewControllers;
}

+ (void)initialize
{
	NSMutableDictionary *defaultValues = [NSMutableDictionary dictionary];
	[defaultValues setObject: [NSMutableDictionary dictionaryWithCapacity:0] forKey: kAccountsKey];
	[defaultValues setObject: [NSMutableDictionary dictionaryWithCapacity:0] forKey: @"login"];
	NSUserDefaults  *defaults = [NSUserDefaults standardUserDefaults];
	[defaults registerDefaults: defaultValues];
}

- (void)initPressionPanel
{
	int i = 1,j = 1;
	UIButton *button = nil;
	UIImage *imagea = nil;
	NSString *imageName = nil;
	NSString *imagePath = nil;
	
	pressionPanel = [[UIAlertView alloc] initWithTitle:nil 
												 message:nil 
												delegate:self 
									   cancelButtonTitle:nil 
									   otherButtonTitles:nil];
	UIView *pressionView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 318, 200)];
	pressionView.backgroundColor = [UIColor blueColor];
	for (i = 1; i < 6; i++)
	{
		for (j = 1; j < 9; j++)
		{
			button = [UIButton buttonWithType:UIButtonTypeCustom];
			button.frame = CGRectMake((j-1)*40, (i-1)*40, 39, 39);
			button.backgroundColor = [UIColor whiteColor];
			imageName = [NSString stringWithFormat:@"%d-png",(i-1)*8 + j];
			imagePath = [[NSBundle mainBundle] pathForResource:imageName ofType:@"png"];
			imagea = [UIImage imageWithContentsOfFile:imagePath];
			if (imagea != nil)
				[button setImage:imagea forState:UIControlStateNormal];
			[button addTarget:self action:@selector(pressionSelected:) forControlEvents:UIControlEventTouchUpInside];
			[pressionView addSubview:button];
		}
	}
	[pressionPanel addSubview:pressionView];
	[pressionView release];
}

- (UIAlertView*)pressionPanel
{
	if (pressionPanel == nil)
	{
		[self initPressionPanel];
	}
	return pressionPanel;
}

- (void)pressionSelected:(id)sender
{
	if (pressionPanel.delegate != nil)
		[pressionPanel.delegate performSelector:@selector(pressionSelected:) withObject:sender];
}

- (void)checkSettingValue
{
	NSNumber *showBigImage;
	NSNumber *messageAlert;
	NSNumber *onlineAlert;
	NSNumber *displayOnlines;
	NSNumber *notifyNudge;
	onlineAlert = (NSNumber*)[[NSUserDefaults standardUserDefaults] objectForKey:kOnLineAlertKey];
	if (onlineAlert == nil)
	{
		NSString *pathStr = [[NSBundle mainBundle] bundlePath];
		NSString *settingsBundlePath = [pathStr stringByAppendingPathComponent:@"Settings.bundle"];
		NSString *finalPath = [settingsBundlePath stringByAppendingPathComponent:@"Root.plist"];
		
		NSDictionary *settingsDict = [NSDictionary dictionaryWithContentsOfFile:finalPath];
		NSArray *prefSpecifierArray = [settingsDict objectForKey:@"PreferenceSpecifiers"];
		
		NSDictionary *prefItem;
		NSString *str = nil;
		id value  = nil;
		for (prefItem in prefSpecifierArray)
		{
			str = [prefItem objectForKey:@"Key"];
			value = [prefItem objectForKey:@"DefaultValue"];
			
			if ([str isEqualToString:kShowBigImageKey])
			{
				showBigImage = value;
			}
			if ([str isEqualToString:kDisplayOnLinesKey])
			{
				displayOnlines = value;
			}
			if ([str isEqualToString:kNotifyNudgeKey])
			{
				notifyNudge = value;
			}
			else if ([str isEqualToString:kMessageAlertKey])
			{
				messageAlert = value;
			}
			else if ([str isEqualToString:kOnLineAlertKey])
			{
				onlineAlert = value;
			}
//			else if ([str isEqualToString:kSaveConversationKey])
//			{
//				saveConversation = value;
//			}
			
		}
		NSDictionary *appDefaults =  [NSDictionary dictionaryWithObjectsAndKeys:
									  showBigImage, kShowBigImageKey,
  									  displayOnlines,kDisplayOnLinesKey,
									  notifyNudge,kNotifyNudgeKey,
									  messageAlert, kMessageAlertKey,
									  onlineAlert, kOnLineAlertKey,
//									  saveConversation,kSaveConversationKey,
									  nil];

		[[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
		[[NSUserDefaults standardUserDefaults] synchronize];
	}
}


- (void)login
{
	if (!needLogin)
	{
		if (navigationController != nil
			&& [navigationController.topViewController isKindOfClass:[LoginViewController class]])
		{
			LoginViewController *loginViewController = (LoginViewController *)navigationController.topViewController;
			
			if ([loginViewController.domain isEqualToString: NSLocalizedString(@"qtn_msn_account_other",nil)])
			{
				//NSString *anEmail = [loginViewController email];
				//NSRange aRange = [anEmail rangeOfString:@"@"];
				//if (aRange.location == NSNotFound){
				//	anEmail = [[anEmail stringByAppendingString: [loginViewController domain]] lowercaseString];
				//}
				//self.myAccount = anEmail;
				self.myAccount = [[loginViewController email] lowercaseString];
			}else {
				self.myAccount = [[[loginViewController email] stringByAppendingString:[loginViewController domain]] lowercaseString];
			}

			self.myPassword = [loginViewController password];
			self.myStatus = getStatus(loginViewController.status);
			autoLoad = [loginViewController autoLoad];
			rememberPassword = [loginViewController savePassword];
		}
		else {
			MYLog(@"-------unknown controller-------");
		}
	}
	else
	{
		needLogin = NO;
	}
	NSMutableDictionary *accountsDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
	NSMutableDictionary *dict = [accountsDictionary objectForKey:myAccount];
	
	self.infoDict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	if (dict != nil)
	{
		[infoDict addEntriesFromDictionary:dict];
	}
	
	if (navigationController != nil)
	{
		if ([navigationController.topViewController isKindOfClass:[LoginViewController class]])
		{
			[infoDict setObject:myStatus forKey:kLoginStatusKey];
			[[NSUserDefaults standardUserDefaults] setObject:myAccount forKey:kLastAccountKey];
			
			if (rememberPassword)
			{
				[infoDict setObject:[NSNumber numberWithBool:autoLoad] forKey:kAutoLoginKey];
				[infoDict setObject:[NSNumber numberWithBool:rememberPassword] forKey:kSavePwdFlagKey];
				if (myPassword != nil)
					[infoDict setObject:myPassword forKey:kPasswordKey];
			}
			else
			{
				[infoDict setObject:[NSNumber numberWithBool:NO] forKey:kAutoLoginKey];
				[infoDict setObject:[NSNumber numberWithBool:rememberPassword] forKey:kSavePwdFlagKey];
				NSString *password = [infoDict objectForKey:kPasswordKey];
				if (password != nil)
				{
					[infoDict removeObjectForKey:kPasswordKey];
				}
			}
			
			if ([infoDict objectForKey: kProfileVersionKey] == nil){
				[infoDict setObject: @"||" forKey: kProfileVersionKey];
			}
			if ([infoDict objectForKey: kRosterVersionKey] == nil){
				[infoDict setObject: @"|||" forKey: kRosterVersionKey];
			}
			
			[infoDict setObject: [NSNumber numberWithBool: YES] forKey: kOnLineAlertKey];
			[infoDict setObject: [NSNumber numberWithBool: YES] forKey: kMessageAlertKey];
			[infoDict setObject: [NSNumber numberWithBool: YES] forKey: kShowBigImageKey];
			[self saveAccountInfo];			
		}

		[navigationController.view removeFromSuperview];
	}
	else// (navigationController == nil)
	{
		self.myPassword = [infoDict objectForKey:kPasswordKey];
		self.myStatus = [infoDict objectForKey:kLoginStatusKey];
	}
	
	LoginFlashViewController *loginFlashViewController = [[LoginFlashViewController alloc] init];
	UINavigationController *tmpController = [[UINavigationController alloc] initWithRootViewController:loginFlashViewController];
	self.navigationController = tmpController;
	[tmpController release];
	[navigationController setNavigationBarHidden:YES animated:NO];
	[loginFlashViewController release];
	
	[window addSubview: navigationController.view];
	if (firstLogin)
	{
		[self showLoginFlashView];
		firstLogin = NO;
	}
	else
	{
		[self showLoginFlashView];
	}
}

- (void)showMyAlertView:(UIAlertView*)theAlert model:(BOOL)flag
{
	if (theAlert)
	{
		id sender = nil;
		if (flag){
			sender = self;
		}
		[[NSNotificationCenter defaultCenter] postNotificationName:DismissAlertViewNotification object:sender];
		sysDismiss = YES;
		[theAlert show];
	}
}

- (void)showLoginFlashView
{
    self.netWorkController = [ClientNetWorkController getInstance];
    [netWorkController sendClientRequest:nil];
    
	UIApplication *application = [UIApplication sharedApplication];
	application.networkActivityIndicatorVisible = YES;	
}

- (void)logoutWithNetworkConnected:(BOOL)isconnected
{
	shouldHideAlertMsg = YES;
	
	LogMethod();
	UIApplication *application = [UIApplication sharedApplication];
	application.networkActivityIndicatorVisible = NO;
	
	if (hasLogin)
	{
		[self garbage];
		
		if (isconnected)
		{
			UIAlertView *alert;
			for (alert in alerts)
			{
				alert.delegate = nil;
				[alert dismissWithClickedButtonIndex:0 animated:NO];
			}
			[alerts removeAllObjects];
			
			self.logoutAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"qtn_main_buddy_view_logout",nil)
															message:NSLocalizedString(@"pica_str_tip_logging_out",nil)
														   delegate:self 
												  cancelButtonTitle:nil
												  otherButtonTitles:nil] autorelease];
			[logoutAlertView show];
			[netWorkController sendLogOutRequest];
		}
		else
		{
			self.netWorkController = nil;
			if (!needLogin)
				[self showLoginWindow];
		}
		
		hasLogin = NO;
	}
	else
	{
		[netWorkController release];
		netWorkController = nil;
		if (!needLogin)
			[self showLoginWindow];
	}
}

- (void)logout
{
	[self logoutWithNetworkConnected:YES];
}

- (void)refreshList
{
	UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"qtn_buddy_refresh",nil) 
														   message:NSLocalizedString(@"qtn_msn_update_buddy_list_confirm",nil)  
														  delegate:self 
												 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil)
												 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil),nil] autorelease];
	[alerts addObject:myAlertView];
	[self showMyAlertView: myAlertView model: NO];
}

- (void)restart{
	needLogin = YES;
	if (!hasLogin){
		[listViewController release];
		listViewController = nil;
		[infoDict removeObjectForKey:kProfileVersionKey];
		[infoDict removeObjectForKey:kRosterVersionKey];
		[infoDict removeObjectForKey:@"groupList"];
		[infoDict removeObjectForKey:@"contactList"];
	}
	[self logoutWithNetworkConnected:NO];
	[self login];
}

- (void)saveAccountInfo
{
	if (listViewController != nil)
	{
		ContactList * list = [ContactList getInstance];
		NSMutableArray *dictionaryArray = [NSMutableArray arrayWithCapacity:0];
		GroupInfo *groupInfo = nil;
		for (groupInfo in list.groupList)
		{
			if (groupInfo != nil && ![groupInfo.name isEqualToString: NSLocalizedString(@"Other contacts", nil)]){
				[dictionaryArray addObject:[groupInfo dictionaryRepresentation]];
            }
		}
		if ([dictionaryArray count] > 0)
		{
			[infoDict setObject:dictionaryArray forKey:@"groupList"];	
		}
		
		dictionaryArray = [NSMutableArray arrayWithCapacity:0];
		PersonInfo *personInfo = nil;
		for (personInfo in list.contactList)
		{
			if (personInfo != nil)
				[dictionaryArray addObject:[personInfo dictionaryRepresentation]];
		}
		if ([dictionaryArray count] > 0)
		{
			[infoDict setObject:dictionaryArray forKey:@"contactList"];	
		}	
	}
	if ([infoDict count] > 0)
	{
		NSDictionary * dict = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
		NSMutableDictionary *accountsDictionary = [NSMutableDictionary dictionaryWithCapacity:0];
		[accountsDictionary addEntriesFromDictionary:dict];
		[accountsDictionary setObject:infoDict forKey:myAccount];
		[[NSUserDefaults standardUserDefaults] setObject:accountsDictionary forKey:kAccountsKey];
	}
}

- (void)showLoginWindow
{	
	LoginViewController *loginViewController = [[LoginViewController alloc] init];
	loginViewController.navigationItem.title=@"Messenger";
	[navigationController.view removeFromSuperview];
	UINavigationController *tmpController = [[UINavigationController alloc] initWithRootViewController:loginViewController];
	self.navigationController = tmpController;
	[tmpController release];
	[navigationController setNavigationBarHidden:YES animated:NO];
	[loginViewController release];
	[window addSubview: navigationController.view];	
}


- (void)showReceivedMessageAlert:(NSString*)theName 
						 message:(NSString*)theMessage
{
	if (theMessage == nil)
		theMessage = @"...";
	
	if (messageAlertTimer != nil)
	{
		[messageAlertTimer invalidate];
		messageAlertTimer = nil;
	}
	
	if (messageAlertLabel == nil)
	{
		messageAlertLabel = [[ImageLabel alloc] initWithFrame:CGRectZero];
		messageAlertLabel.backgroundColor = [UIColor colorWithWhite:1.0 alpha:0.5];
		messageAlertLabel.numberOfLines = 2;
	}
	

	if (theName != nil && theMessage != nil)
	{
		messageAlertLabel.text = [NSString stringWithFormat:@"%@ %@\n%@",theName,NSLocalizedString(@"qtn_msn_say_name",nil),theMessage];
	}
	
	if (shouldHideAlertMsg){
		messageAlertLabel.text = @"";
	}
	
	[window addSubview: messageAlertLabel];

	CGContextRef context = UIGraphicsGetCurrentContext();
	[UIView beginAnimations:nil context:context];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	[UIView setAnimationDuration:0.5];
	
	CGRect rect = CGRectMake(1, 21, 318, 42);
	[messageAlertLabel setFrame:rect];
	[UIView commitAnimations];

	messageAlertTimer = [NSTimer scheduledTimerWithTimeInterval: 1.0f
														 target: self
													   selector: @selector(performDismiss:)
													   userInfo: nil repeats:NO];
}

- (void) performDismiss: (NSTimer *)timer
{
	if (messageAlertLabel != nil)
	{
		CGContextRef context = UIGraphicsGetCurrentContext();
		[UIView beginAnimations:nil context:context];
		[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
		[UIView setAnimationDuration:0.5];
		[messageAlertLabel setFrame:CGRectZero];
		[UIView commitAnimations];
		[messageAlertLabel removeFromSuperview];
		[messageAlertLabel release];
		messageAlertLabel = nil;
	}
	messageAlertTimer = nil;
}


- (void)notifyLoginFailed: (NSNotification*)note
{
	NSString *message = nil;
	if ([note userInfo] != nil)
		message = [[note userInfo] objectForKey:@"message"];
	
	UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle: NSLocalizedString(@"Login failed",nil) 
														   message: message 
														  delegate: self
												 cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil) 
												 otherButtonTitles: nil] autorelease];
	[alerts addObject:myAlertView];
	[self showMyAlertView:myAlertView model:NO];
}

- (void)notifyH3GLoginFailed: (NSNotification*)note
{
	NSString *message = nil;
	if ([note userInfo] != nil)
		message = [[note userInfo] objectForKey:@"message"];
	
	UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle: NSLocalizedString(@"Login failed",nil) 
														   message: message 
														  delegate: self
												 cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil) 
												 otherButtonTitles: nil] autorelease];
	[alerts addObject:myAlertView];
	[self showMyAlertView:myAlertView model:NO];
}

//20100311 add check wifi function by liubing

- (void)notifyH3GWIFIFailed: (NSNotification*)note
{
	self.netWorkController = [ClientNetWorkController getInstance];
	[netWorkController sendH3GClientRequest:X_H3G_MSISDN];
}

- (void)notifyH3GTime: (NSNotification*)note
{
	NSString* str = (NSString*)[note object];
	self.h3g_time = str;
	NSData* data = [self.infoDict objectForKey:kH3GpHomeTime];
	if(data == nil)
	{
		self.netWorkController = [ClientNetWorkController getInstance];
		[netWorkController sendH3GClientRequest:X_H3G_MSISDN];
	}
	else
	{
		NSString* tmp = [data base64Encoding];
		if([str isEqualToString:tmp])
		{
			NetworkStatus status = [netWorkController.wifiStatus updateReachabilityChanges];
			switch (status) 
			{
				case 0:
					self.netWorkController = [ClientNetWorkController getInstance];
					[netWorkController sendH3GClientRequest:X_H3G_MSISDN];
					break;
				case 1:
					self.netWorkController = [ClientNetWorkController getInstance];
					[netWorkController sendH3GClientRequest:X_H3G_MSISDN];
					break;
				case 2:
					[self checkPhoneNumber];
					break;
				default:
					break;
			}
		}
		else
		{
			self.netWorkController = [ClientNetWorkController getInstance];
			[netWorkController sendH3GClientRequest:X_H3G_MSISDN];
		}
	}	
}

- (void)checkPhoneNumber
{
	NSString* data = [self.infoDict objectForKey:kH3GPhoneNumber];
	if(data == nil)
	{		
		self.netWorkController = [ClientNetWorkController getInstance];
		[netWorkController sendH3GClientRequest:X_H3G_MSISDN];	
	}
	else
	{
		self.netWorkController = [ClientNetWorkController getInstance];
		[netWorkController sendClientRequest: [netWorkController numericString: data]];
	}
}

- (void)checkH3GTime
{
	NSData* data = [self.infoDict objectForKey:kH3GpHomeTime];
	if(data == nil)
	{		
		if((self.h3g_time == nil) || (self.h3g_phone_number == nil))
		{
			return;
		}
		NSData *time = [NSData dataWithBase64EncodedString:self.h3g_time];
		[self.infoDict setObject:time forKey:kH3GpHomeTime];
		[self.infoDict setObject:self.h3g_phone_number forKey:kH3GPhoneNumber];
	}
	else
	{
		if((self.h3g_time == nil) || (self.h3g_phone_number == nil))
		{
			return;
		}
		NSString* tmp = [data base64Encoding];
		if([tmp isEqualToString:self.h3g_time])
		{
			[self.infoDict setObject:self.h3g_phone_number forKey:kH3GPhoneNumber];
		}
		else
		{
			NSData *time = [NSData dataWithBase64EncodedString:self.h3g_time];
			[self.infoDict setObject:time forKey:kH3GpHomeTime];
			[self.infoDict setObject:self.h3g_phone_number forKey:kH3GPhoneNumber];
		}
	}
}
//end

- (void)setChatviewData:(ChatViewData*)theChatViewData forKey:(NSString*)theKey
{
	if (theChatViewData != nil && theKey != nil)
	{
		[viewControllers setObject:theChatViewData forKey:theKey];
		if (navigationController != nil && [navigationController.topViewController isKindOfClass: [ChatSelectController class]])
		{
			[(ChatSelectController*)navigationController.topViewController refreshVC: nil];
		}
	}
}

- (void)notifynetworkDisabled: (NSNotification*)note;
{
	NSString *message = nil;
	if ([note userInfo] != nil)
		message = [[note userInfo] objectForKey:@"message"];
	
	UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Network error",nil) 
														   message:message 
														  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_cancel", nil) 
												 otherButtonTitles:NSLocalizedString(@"qtn_ok", nil),nil] autorelease];
	[alerts addObject:myAlertView];
	[myAlertView setDelegate:self];
	[self showMyAlertView:myAlertView model:YES];
}

- (void)notifyGetSystemInfo: (NSNotification*)note
{
	UIAlertView *anAlertView = nil;
	NSDictionary* aDict = [note userInfo];
	self.systemNotice = [aDict objectForKey:@"system"];
	self.errCode = [aDict objectForKey: @"myErrCode"];
	if ([systemNotice count] > 0)
	{
		id anObject = [systemNotice objectAtIndex: 0];
		if ([anObject isKindOfClass: [NSMutableDictionary class]]){
			self.updateURL = [anObject objectForKey: @"url"];
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"text_is_upgrade_version",nil) 
													 message: NSLocalizedString(@"text_is_upgrade_now",nil)
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"text_upgrade_cancel",nil)
										   otherButtonTitles: NSLocalizedString(@"text_upgrade_ok",nil), nil];
			[alerts addObject: anAlertView];
			[self showMyAlertView: anAlertView model:NO];
			[systemNotice removeObjectAtIndex: 0];
			[anAlertView release];
		}
		else {
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
													 message: anObject
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
										   otherButtonTitles: nil];
			[alerts addObject: anAlertView];
			[self showMyAlertView: anAlertView model:NO];
			[systemNotice removeObjectAtIndex: 0];
			[anAlertView release];
		}
	}
}


- (void)notifyGetUpgradeInfo: (NSNotification*)note
{
	UIAlertView *anAlertView = nil;
	NSDictionary* aDict = [note userInfo];
	self.errCode = [aDict objectForKey: @"myErrCode"];
	NSDictionary *infoes = [aDict objectForKey:@"upgrade"];
	if (infoes != nil && [[infoes allKeys] count] > 0)
	{
		NSString *message = nil;
		NSString *string = [infoes objectForKey:@"version"];
		if (string != nil)
			message = [NSString stringWithFormat:@"version:%@",string];

		self.updateURL = [infoes objectForKey: @"url"];
		
		if ([updateURL length] > 0){
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"text_is_upgrade_version",nil) 
													 message: NSLocalizedString(@"text_is_upgrade_now",nil)
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"text_upgrade_cancel",nil)
										   otherButtonTitles: NSLocalizedString(@"text_upgrade_ok",nil), nil];
			[alerts addObject: anAlertView];
			[self showMyAlertView: anAlertView model:NO];
			[anAlertView release];
		}
		else {
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_msn_need_update_confirm",nil) 
													 message: message 
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
										   otherButtonTitles: nil];
			[alerts addObject: anAlertView];
			[self showMyAlertView: anAlertView model:NO];
		}
	}
}


- (void)notifyNeedUpdateVersion: (NSNotification*)note
{
	NSString *version = nil;
	UIAlertView *anAlertView = nil;
	NSMutableDictionary* aMessage = nil;
	self.errCode = [[note userInfo] objectForKey: @"myErrCode"];
	if ([note userInfo] != nil)
	{
		self.systemNotice = [[note userInfo] objectForKey:@"message"];
		
		if ([systemNotice count] == 1){
			aMessage = [systemNotice objectAtIndex: 0];
			version = [aMessage objectForKey:@"version"];
			self.updateURL = [aMessage objectForKey: @"url"];
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"Upgrade",nil) 
													 message: [NSString stringWithFormat:NSLocalizedString(@"UpgradeVersion",nil), version] 
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
										   otherButtonTitles: nil];	
			self.systemNotice = nil;
		}
		else if ([systemNotice count] > 1){
			version = [aMessage objectForKey:@"version"];
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
													 message: [systemNotice objectAtIndex: 0]
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
										   otherButtonTitles: nil];	
			[systemNotice removeObjectAtIndex: 0];
		}
		if (anAlertView != nil){
			[alerts addObject: anAlertView];
			[self showMyAlertView: anAlertView model:NO];
		}
		[anAlertView release];
	}
}

- (void)notifyReceivedError: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	if (listViewController != nil){
		[listViewController.contactList notifyReceivedError: note];
	}
	
	NSString *message = nil;
	if ([note userInfo] != nil)
	{
		message = [[note userInfo] objectForKey:@"message"];
		errorCode = [[[note userInfo] objectForKey:@"errorcode"] intValue];
	}

	if (message != nil)
	{
		[self dismissSettingsAlertView];
		
		UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error",nil)
															   message:message 
															  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil)  
													 otherButtonTitles:nil] autorelease];	
		[alerts addObject:myAlertView];
		[myAlertView setDelegate:self];
		[self showMyAlertView:myAlertView model:YES];
	}
}

- (void)notifyBeAddedToPerson: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	self.stranger = [[note userInfo] objectForKey:PersonInfoKey];
	if (stranger != nil)
	{
		NSString *string = [NSString stringWithFormat: @"%@\n%@", NSLocalizedString(@"qtn_msn_detail_nickname",nil), (([stranger.nickname length] == 0) ? @"" : stranger.nickname)];
		if (string == nil)
			string = [NSString stringWithFormat:@"%@\n%@",NSLocalizedString(@"qtn_msn_detail_email_address",nil), stranger.imid];
		else
			string = [string stringByAppendingFormat:@"\n%@\n%@",NSLocalizedString(@"qtn_msn_detail_email_address",nil), stranger.imid];

		if (string != nil)
		{
			string = [string stringByAppendingFormat:@"\n%@",NSLocalizedString(@"pica_contact_addinfo",nil)];
			string = [string stringByAppendingFormat:@"\n%@",NSLocalizedString(@"pica_contact_details",nil)];
			string = [string stringByAppendingFormat:@"\n%@",NSLocalizedString(@"pica_contact_passinfo",nil)];
			
			if (stranger.realname != nil)
				string = [string stringByAppendingFormat:@"\nRealName: %@",stranger.realname];
			
			if (stranger.group != nil)
				string = [string stringByAppendingFormat:@"\nGroup: %@",stranger.group];
			
			if (stranger.sex != nil)
				string = [string stringByAppendingFormat:@"\n%@: %@",NSLocalizedString(@"pica_contact_sex",nil),stranger.sex];
			
			if (stranger.age != nil)
				string = [string stringByAppendingFormat:@"\n%@: %@",NSLocalizedString(@"pica_contact_age",nil),stranger.age];
			
			if (stranger.province != nil)
				string = [string stringByAppendingFormat:@"\n%@: %@",NSLocalizedString(@"pica_contact_area",nil),stranger.province];
			
			if (stranger.desc != nil)
				string = [string stringByAppendingFormat:@"\n%@: %@",NSLocalizedString(@"pica_contact_desc",nil),stranger.desc];
			UIAlertView *myAlertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"qtn_msn_member_addtobuddy",nil) 
																  message:string 
																 delegate:self 
														cancelButtonTitle:NSLocalizedString(@"qtn_accept",nil)
														otherButtonTitles:NSLocalizedString(@"qtn_reject",nil),nil];
            
            pcAlertAndStrangerInfoObject* anObject = [[pcAlertAndStrangerInfoObject alloc] init];
            anObject.alertObject = myAlertView;
            anObject.strangerObject = [[note userInfo] objectForKey:PersonInfoKey];
            [strangers addObject: anObject];
            [anObject release];
            
			[alerts addObject: myAlertView];			
			[myAlertView setDelegate:self];
			[self showMyAlertView:myAlertView model:NO];
			[myAlertView release];
		}
	}
}

- (void)notifyNetworkReachabilityChanged: (NSNotification*)note
{
	NetworkStatus status = [netWorkController.wifiStatus updateReachabilityChanges];
	MYLog(@"wifiStatus:%d",status);
}


- (void)notifyLogin: (NSNotification*)note
{
	LogMethod();
	NSString *str = nil;
	NSString *imagePath = nil;
	NSMutableArray *array = nil;
	UIApplication *application = nil;
	NSDictionary *dict = nil;
	GroupInfo *groupInfo = nil;
	PersonInfo *personInfo = nil;
	NSArray * savedList = nil;
	NSString *url = nil;
	NSUInteger theIndex = NSNotFound;
	UINavigationController *tmpController = nil;
	MSNAppDelegate *appdelegate = nil;
	NSArray *filePaths = nil; 
	NSString *directory = nil;
	NSString *filePath = nil;
	NSDictionary * conversations = nil;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	str = [[note userInfo] objectForKey:@"presence"];
	
	if ([str isEqualToString: @"available"])
	{
		if (!hasLogin)
		{
			application = [UIApplication sharedApplication];
			application.networkActivityIndicatorVisible = NO;
			
			hasLogin = YES;
			viewControllers = [[NSMutableDictionary alloc] initWithCapacity:0];
			array = [NSMutableArray arrayWithCapacity:0];
			dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
			
			if ([dict objectForKey:@"second_zwp"] != nil)
			{
				if (standbyZwpController == nil){
					standbyZwpController = [[ZwpController alloc] initWithUrl:[dict objectForKey:@"second_zwp"] type:STANDBY_ZWP_REQUEST];					
				}
				
				tmpController = [[UINavigationController alloc] initWithRootViewController:standbyZwpController];
				tmpController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
				[tmpController setNavigationBarHidden:YES animated:NO];
				imagePath = [[NSBundle mainBundle] pathForResource:@"a10"
															ofType:@"png"];
				tmpController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
				[array addObject:tmpController];
				[currentViewIndexArray addObject:kStandbyLiveWapVC];
				[tmpController release];
			}			
			
			if (settingController == nil)
			{
				settingController = [[pcExtSettingsViewController alloc] initWithNibName: @"ExtSettings"
																				  bundle: nil];
			}
			
			tmpController = [[UINavigationController alloc] initWithRootViewController:settingController];
			settingController.title = NSLocalizedString(@"Settings", nil);

			tmpController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
			imagePath = [[NSBundle mainBundle] pathForResource:@"myzone"
														ofType:@"png"];
			tmpController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
			[array addObject:tmpController];
			[currentViewIndexArray addObject:kSettingsVC];
			[tmpController release];
			
			if (listViewController == nil)
			{
				listViewController = [[ListViewController alloc] init];	
			}
			listViewController.contactList = [ContactList getInstance];
			listViewController.showViewsButton.enabled = NO;
			listViewController.toolbarTitle.text = NSLocalizedString(@"qtn_title_contact", nil);
			tmpController = [[UINavigationController alloc] initWithRootViewController:listViewController];
			[tmpController setNavigationBarHidden:YES animated:NO];
			imagePath = [[NSBundle mainBundle] pathForResource:@"roster-png"
														ofType:@"png"];
			tmpController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
			[array addObject:tmpController];
			[currentViewIndexArray addObject:kListChatVC];
			[tmpController release];
			
			chatSelectController = [[ChatSelectController alloc] initWithStyle:UITableViewStyleGrouped];
			chatSelectController.navigationItem.title = NSLocalizedString(@"qtn_msn_chatlist_title",nil);

			if (chatViewController == nil)
			{
				chatViewController = [[ChatViewController alloc] init];				
			}			
			
			tmpController = [[UINavigationController alloc] initWithRootViewController:chatSelectController];
			tmpController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
			imagePath = [[NSBundle mainBundle] pathForResource:@"dialog-png"
														ofType:@"png"];
			tmpController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
			[array addObject:tmpController];
			[currentViewIndexArray addObject:kChatVC];
			[tmpController release];
					
			if ([dict objectForKey:@"zwp"] != nil)
			{
				if (zwpController == nil){
					zwpController = [[ZwpController alloc] initWithUrl:[dict objectForKey:@"fpage"] type:ZWP_REQUEST];					
				}
				
				tmpController = [[UINavigationController alloc] initWithRootViewController:zwpController];
				tmpController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
				[tmpController setNavigationBarHidden:YES animated:NO];
				imagePath = [[NSBundle mainBundle] pathForResource:@"zwp-png"
															ofType:@"png"];
				tmpController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
				[array addObject:tmpController];
				[currentViewIndexArray addObject:kLiveWapVC];
				[tmpController release];
				
			}
			
			tabController = [[UITabBarController_ColorManage alloc] init];				
			tabController.delegate = self;
			[tabController setViewControllers:array animated:NO];

//			MYLog(@">>>[currentViewIndexArray]:%@",currentViewIndexArray);
//			MYLog(@">>>[kListChatVC]:%d",[self getTheIndex:kListChatVC]);
			theIndex = [self getTheIndex:kListChatVC];
			tabController.selectedIndex = theIndex;
			tabController.segmentedControl.selectedSegmentIndex = theIndex;


			if ([infoDict objectForKey:@"groupList"] != nil)
			{
				dict = nil;
				savedList = [infoDict objectForKey:@"groupList"];
				for (dict in savedList)
				{
					if (dict != nil)
					{
						groupInfo = [[GroupInfo alloc] initWithDictionary:dict];
						[[ContactList getInstance] addGroup:groupInfo];
						[groupInfo release];
					}
				}
				
				savedList = [infoDict objectForKey:@"contactList"];
				for (dict in savedList)
				{
					if (dict != nil)
					{
						personInfo = [[PersonInfo alloc] initWithDictionary:dict];
						[[ContactList getInstance] addPerson:personInfo];
						[personInfo release];
					}
				}
			}
			dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
			if (dict != nil)
			{
				url = [dict objectForKey:@"adurl"];
				if (url != nil)
				{
					[netWorkController sendAdviceRequest:url];	
				}
			}
			//[self notifyShowContactList:nil];
		}
	}
	else
		MYLog(@"present result != available");
	
	
	// Load conversation history.
	appdelegate = [MSNAppDelegate globalAppDelegate];
	filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
	directory = [filePaths objectAtIndex: 0];
	filePath = [directory stringByAppendingPathComponent:@"Sessions.plist"];
	conversations = [[NSDictionary alloc] initWithContentsOfFile:filePath];
	
	if (conversations != nil)
	{
		[appdelegate.infoDict setObject:conversations forKey:@"conversation"];		
		[conversations release];
	}
	
	[self checkH3GTime];

	[pool drain];
}




- (void)notifyLoginButtonPressed: (NSNotification*)note;
{	
	[self login];
}

- (void)notifyLogout: (NSNotification*)note
{
	shouldHideAlertMsg = YES;
	LogMethod();
	if (hasLogin)
	{
		[self garbage];
		hasLogin = NO;
	}
	if (logoutAlertView)
	{
		[logoutAlertView dismissWithClickedButtonIndex:0 animated:YES];	
		self.logoutAlertView = nil;
	}
	
	[self showLoginWindow];
	self.netWorkController = nil;	
	UIApplication *application = [UIApplication sharedApplication];
	application.networkActivityIndicatorVisible = NO;
}

- (void)notifyShowZwpView: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	LogMethod();
	NSDictionary * info = [note userInfo];
	if (info != nil)
	{
		NSString *url = [info objectForKey:@"url"];
		if (url != nil)
		{
			if (navigationController != nil)
			{
				[navigationController.view removeFromSuperview];
				self.navigationController = nil;
				[window addSubview:tabController.view];
			}
			zwpController.url = url;
			tabController.segmentedControl.selectedSegmentIndex = [self getTheIndex:kLiveWapVC];
			tabController.selectedIndex = [self getTheIndex:kLiveWapVC];
		}
	}
}

- (void)notifyShowStandbyZwpView: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	NSDictionary * info = [note userInfo];
	if (info != nil)
	{
		NSString *url = [info objectForKey:@"url"];
		if (url != nil)
		{
			if (navigationController != nil)
			{
				[navigationController.view removeFromSuperview];
				self.navigationController = nil;
				[window addSubview:tabController.view];
			}
			standbyZwpController.url = url;
			tabController.segmentedControl.selectedSegmentIndex = [self getTheIndex:kStandbyLiveWapVC];
			tabController.selectedIndex = [self getTheIndex:kStandbyLiveWapVC];
		}
	}
}

- (void)notifyShowContactList: (NSNotification*)note
{
	shouldHideAlertMsg = NO;
	LogMethod();
	if (navigationController != nil)
	{
		[navigationController.view removeFromSuperview];
		self.navigationController = nil;
		[window addSubview:tabController.view];
	}
}


- (void)notifyShowChatViews: (NSNotification*)note;
{
	if ([viewControllers count] > 0)
	{
		[tabController.view removeFromSuperview];
		
		ChatSelectController * controller = [[ChatSelectController alloc] initWithStyle:UITableViewStyleGrouped];
		controller.navigationItem.title = NSLocalizedString(@"qtn_msn_chatlist_title",nil);
		controller.chatViewDatas = viewControllers;
		if (currentChatIndex >= [viewControllers count])
			currentChatIndex = [viewControllers count] - 1;
		controller.currentIndex = currentChatIndex;
		
		UINavigationController *tmpController = [[UINavigationController alloc] initWithRootViewController:controller];
		self.navigationController = tmpController;
		[tmpController release];
		navigationController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
		[controller release];	
		[window addSubview: navigationController.view];
	}
	 
}

- (void)notifySayHiView:(NSNotification *)note
{
	LogMethod();
	PersonInfo *thePerson = [[note userInfo] objectForKey:@"key"];
	
	UIViewController *theController = [note object];
	[SayHiView SayHiInView:[theController view] rect:CGRectMake(25, 50, 270, 330) liveID:thePerson.imid];
	return;
}

- (void)notifyShowChatView: (NSNotification*)note
{
	LogMethod();
	id info;
	id obj = [note object];
	if ([note userInfo] != nil)
	{
		info = [[note userInfo] objectForKey:@"key"];
		ChatViewData * theChatViewData = nil;
		if ([info isKindOfClass:[PersonInfo class]])
		{
			PersonInfo* thePersonInfo = (PersonInfo*)info;
			if (thePersonInfo.jid != nil || thePersonInfo.imid != nil)
			{
				NSString *personid = thePersonInfo.jid;
				if (personid == nil)
				{
					personid = thePersonInfo.imid;
				}
				NSString *string = nil;
				NSArray *keys = [viewControllers allKeys];

				for (string in keys)
				{
					if ([string length] > [personid length] 
						&& [[string substringToIndex:[personid length]] compare:personid] == NSOrderedSame)
					{
						theChatViewData = [[viewControllers objectForKey: string] retain];
						if ([theChatViewData.persons count] == 1)
							break;
						else {
							[theChatViewData release];
							theChatViewData = nil;
						}
					}
					
				}
				
				if(theChatViewData == nil)
				{
					ChatViewData * tmpChatViewData = nil; 
					for (tmpChatViewData in [viewControllers allValues])
					{
						if([tmpChatViewData.contact.imid isEqualToString: thePersonInfo.imid])
						{
#if 1    
							int index = 0;
							index = [[viewControllers allValues] indexOfObject:tmpChatViewData];
							theChatViewData = [[[viewControllers allValues] objectAtIndex:index] retain];
#else
							theChatViewData = [tmpChatViewData retain];
#endif
							if ([theChatViewData.persons count] == 1)
								break;
							else {
								[theChatViewData release];
								theChatViewData = nil;
							}
						}
						
					}
				}
				
				
				if (theChatViewData != nil && [theChatViewData isKindOfClass:[ChatViewData class]])
				{
					currentChatIndex = [[viewControllers allValues] indexOfObject: theChatViewData];
				}
				else
				{
					listViewController.showViewsButton.enabled = YES;
					
					NSUInteger theIndex = [self getTheIndex:kChatVC];
					UITabBarController_ColorManage * theTabBarController = [[tabController viewControllers] objectAtIndex:theIndex];
					[theTabBarController tabBarItem].enabled = YES;
					[tabController.segmentedControl setEnabled:YES forSegmentAtIndex:theIndex];
					tabController.segmentedControl.selectedSegmentIndex = theIndex;
					
					
					theChatViewData = [[ChatViewData alloc] init];
					theChatViewData.contact = info;
					
					NSString *key = nil;
					if (theChatViewData.contact.jid != nil)
						key = [theChatViewData.contact.jid stringByAppendingFormat:@"_%@",theChatViewData.sid];
					else if(theChatViewData.contact.imid != nil)
						key = [theChatViewData.contact.imid stringByAppendingFormat:@"_%@",theChatViewData.sid];
					
					if (key != nil){
						[self setChatviewData: theChatViewData forKey: key];
						currentChatIndex = [[viewControllers allValues] indexOfObject: theChatViewData];
					}
				}
			}
		}
		else if ([info isKindOfClass:[ChatViewData class]])
		{
			theChatViewData = [((ChatViewData*)info) retain];
			currentChatIndex = [[viewControllers allValues] indexOfObject:theChatViewData];
		}
		if (theChatViewData != nil)
		{
			chatSelectController.chatViewDatas = viewControllers;
			if (currentChatIndex >= [viewControllers count])
				currentChatIndex = [viewControllers count] - 1;
			chatSelectController.currentIndex = currentChatIndex;
			
			if ([obj isMemberOfClass:[ListViewController class]])
			{
				tabController.segmentedControl.selectedSegmentIndex = [self getTheIndex:kChatVC];
				tabController.selectedIndex = [self getTheIndex:kChatVC];
                
                UINavigationController* aNavigation = (UINavigationController *)tabController.selectedViewController;
                if ([aNavigation topViewController] == chatViewController){
                    [aNavigation popViewControllerAnimated: NO];
                }
				
				chatViewController.chatViewData = theChatViewData;
				[(UINavigationController *)tabController.selectedViewController pushViewController:chatViewController animated:YES];
			}
		}
		[theChatViewData release];
	}
}

- (void)notifyChatviewContactChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	LogMethod();
	NSString *key = nil;
	id obj = [note object];
	if ([obj isKindOfClass:[ChatViewData class]])
	{
		ChatViewData *theChatViewData = (ChatViewData*)obj;
		int index = [[viewControllers allValues] indexOfObject:theChatViewData];
		if (index != NSNotFound)
		{
			theChatViewData = [theChatViewData retain];
			[viewControllers removeObjectForKey:[[viewControllers allKeys] objectAtIndex:index]];
			if (theChatViewData.contact.jid != nil)
			{
				key = [theChatViewData.contact.jid stringByAppendingFormat:@"_%@",theChatViewData.sid];

				[self setChatviewData:theChatViewData forKey:key];	
			}
			else if (theChatViewData.contact.imid != nil)
			{
				key = [theChatViewData.contact.imid stringByAppendingFormat:@"_%@",theChatViewData.sid];
				[self setChatviewData:theChatViewData forKey:key];	
			}
			if (chatViewController.chatViewData == theChatViewData)
				[chatViewController setTitleText];
			[theChatViewData release];
		}
	}
}

- (void)notifyParticipantChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	PersonInfo *info = [[note userInfo] objectForKey:@"info"];
	NSString *sid = [[note userInfo] objectForKey:@"sid"];
	NSString *type = [[note userInfo] objectForKey:@"type"];
	if (sid != nil)
	{
		BOOL find = NO;
		ChatViewData *theChatViewData = nil;
		NSArray * array = [viewControllers allValues];
		for (theChatViewData in array)
		{
			if ([theChatViewData.sid compare:sid] == NSOrderedSame)
			{
				find = YES;
				[theChatViewData participantChanged:info type:type];
				break;
			}
		}
		
		if (!find && info != nil && [type compare:@"enter"] == NSOrderedSame)
		{
			NSString *key = nil;
			theChatViewData = [[ChatViewData alloc] init];
			PersonInfo *person = nil;
			if (info.jid != nil)
				person = [[ContactList getInstance] personWithJid:info.jid];
			
			if (person != nil)
			{
				theChatViewData.contact = person;
				theChatViewData.sid = sid;
				key = [theChatViewData.contact.jid stringByAppendingFormat:@"_%@",theChatViewData.sid];
			}
			else
			{
				theChatViewData.contact = info;
				theChatViewData.sid = sid;
				if (info.jid != nil)
				{
					key = [theChatViewData.contact.jid stringByAppendingFormat:@"_%@",theChatViewData.sid];
				}
				else if (info.imid != nil)
				{
					key = [theChatViewData.contact.imid stringByAppendingFormat:@"_%@",theChatViewData.sid];
				}
			}

			[theChatViewData participantChanged:info type:type];
			[self setChatviewData:theChatViewData forKey:key];
			[theChatViewData release]; //count - 1
			if([viewControllers count] > 0)
			{
				listViewController.showViewsButton.enabled = YES;
			}
		}
	}
}


- (void)notifyReceivedVoiceClip: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	NSDictionary * info = [note userInfo];
	NSString *jid = [info objectForKey:@"jid"];
	NSString *imid = [info objectForKey:@"imid"];
	NSString *sid = [info objectForKey:@"sid"];
	if (jid != nil)
	{
		NSString *key = [jid stringByAppendingFormat:@"_%@",sid];
		ChatViewData *theChatViewData = [self findChatViewData:jid sid:sid];
		if (theChatViewData == nil)
		{
			PersonInfo *person = [[ContactList getInstance] personWithJid:jid];
			if (person != nil)
			{
				if (person.imid != nil)
				{
					theChatViewData = [self findChatViewData:person.imid sid:sid];
				}
				if (theChatViewData == nil)
				{
					theChatViewData = [[ChatViewData alloc] init];
					theChatViewData.contact = person;
					theChatViewData.sid = sid;
					[self setChatviewData:theChatViewData forKey:key];
					[theChatViewData release]; //count - 1	
				}
				else
				{
					theChatViewData.contact = person;
					[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:theChatViewData];
				}
			}
		}
		if (theChatViewData != nil)
			[theChatViewData receivedVoiceClip:info];
	}
	if (imid != nil)
	{
		NSString *key = [imid stringByAppendingFormat:@"_%@",sid];
		ChatViewData *theChatViewData = [self findChatViewData:imid sid:sid];
		if (theChatViewData == nil)
		{
			theChatViewData = [self findChatViewDatabyImid:imid sid:sid];
			if (theChatViewData == nil)
			{
				PersonInfo *person = [[PersonInfo alloc] init];
				if (person != nil)
				{
					person.imid = imid;
					theChatViewData = [[ChatViewData alloc] init];
					theChatViewData.contact = person;
					theChatViewData.sid = sid;
					[self setChatviewData:theChatViewData forKey:key];
					[theChatViewData release]; //count - 1
				}
				[person release];
			}
			else
			{
				PersonInfo *unknown = [[PersonInfo alloc] init];
				unknown.imid = imid;
				theChatViewData.contact = unknown;
				[unknown release];
				[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:theChatViewData];
			}
		}
		if (theChatViewData != nil)
			[theChatViewData receivedVoiceClip:info];
	}
	if([viewControllers count] > 0)
	{
		listViewController.showViewsButton.enabled = YES;
		NSUInteger theIndex = [self getTheIndex:kChatVC];
		UITabBarController_ColorManage * theTabBarController = [[tabController viewControllers] objectAtIndex:theIndex];
		[theTabBarController tabBarItem].enabled = YES;
		[tabController.segmentedControl setEnabled:YES forSegmentAtIndex:theIndex];
		tabController.segmentedControl.selectedSegmentIndex = theIndex;
	}
}

- (void)notifyReceivedFile: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	NSString* aNewSid = nil;
	NSString* aNewKey = nil;
	NSDictionary * info = [note userInfo];
	NSString *jid = [info objectForKey:@"jid"];
	NSString *imid = [info objectForKey:@"imid"];
	NSString *sid = [info objectForKey:@"sid"];
	ChatViewData *theChatViewData = nil;
	
	if (jid != nil)
	{
		PersonInfo *person = nil;
		NSString *key = [jid stringByAppendingFormat:@"_%@",sid];
		theChatViewData = [self findChatViewData:jid sid:sid];
		if (theChatViewData == nil)
		{
			person = [[ContactList getInstance] personWithJid:jid];
			if (person != nil)
			{
				if (person.imid != nil)
				{
					theChatViewData = [self findChatViewData:person.imid sid:sid];
				}
				if (theChatViewData == nil)
				{
					theChatViewData = [[ChatViewData alloc] init];
					theChatViewData.contact = person;
					theChatViewData.sid = sid;
					[self setChatviewData:theChatViewData forKey:key];
					[theChatViewData release]; //count - 1	
				}
				else
				{
					theChatViewData.contact = person;
					[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:theChatViewData];
				}
			}
		}
		else {
			//if the session is multi-persons' session, then find another session by jid
			if ([theChatViewData.persons count] > 1){
				theChatViewData = [self findP2PSessionByJid: jid currentChatViewData: theChatViewData];
				
				if (theChatViewData == nil){
					if ([self isExistSessionKeyOfJid: jid sid: sid]){
						aNewSid = [self buildSidByJid: jid sid: sid];
						aNewKey = [jid stringByAppendingFormat: @"_%@", aNewSid];
					}
					else {
						aNewSid = sid;
						aNewKey = key;
					}
					person = [[ContactList getInstance] personWithJid: jid];
					theChatViewData = [[ChatViewData alloc] init];
					theChatViewData.contact = person;
					theChatViewData.sid = aNewSid;
					[self setChatviewData:theChatViewData forKey: aNewKey];
					[theChatViewData release]; //count - 1	
				}
			}
		}
		if (theChatViewData != nil)
			[theChatViewData receivedFile:info];
	}
	if (imid != nil)
	{
		NSString *key = [imid stringByAppendingFormat:@"_%@",sid];
		theChatViewData = [self findChatViewData:imid sid:sid];
		if (theChatViewData == nil)
		{
			theChatViewData = [self findChatViewDatabyImid:imid sid:sid];
			if (theChatViewData == nil)
			{
				
				PersonInfo *person = [[PersonInfo alloc] init];
				if (person != nil)
				{
					person.imid = imid;
					theChatViewData = [[ChatViewData alloc] init];
					theChatViewData.contact = person;
					theChatViewData.sid = sid;
					[self setChatviewData:theChatViewData forKey:key];
					[theChatViewData release]; //count - 1
				}
				[person release];
			}
			else
			{
				PersonInfo *unknown = [[PersonInfo alloc] init];
				unknown.imid = imid;
				theChatViewData.contact = unknown;
				[unknown release];
				[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:theChatViewData];
			}
		}
		if (theChatViewData != nil)
			[theChatViewData receivedFile:info];
	}
	if (theChatViewData != nil)
	{
		if (navigationController != nil 
			|| tabController.selectedIndex != [self getTheIndex:kChatVC]
			|| chatViewController.chatViewData != theChatViewData)
		{
			if ([navigationController.topViewController isKindOfClass: [ChatSelectController class]])
			{
				[(ChatSelectController*)navigationController.topViewController refreshVC: nil];
			}
			
			theChatViewData.hasMessage += 1;
			NSString *name = theChatViewData.contact.nickname;
			if (name == nil)
				name = theChatViewData.contact.imid;
			
			[self showReceivedMessageAlert:name message:@"..."];
			NSDictionary *info = [NSDictionary dictionaryWithObject:theChatViewData.contact forKey:@"key"];
			[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
		}
	}
	
	if([viewControllers count] > 0)
	{
		listViewController.showViewsButton.enabled = YES;
		NSUInteger theIndex = [self getTheIndex:kChatVC];
		UITabBarController_ColorManage * theTabBarController = [[tabController viewControllers] objectAtIndex:theIndex];
		[theTabBarController tabBarItem].enabled = YES;
		[tabController.segmentedControl setEnabled:YES forSegmentAtIndex:theIndex];
		tabController.segmentedControl.selectedSegmentIndex = theIndex;
	}
}

- (void)notifyReceiveAction: (NSNotification*)note
{
	NSDictionary * info = [note userInfo];
	NSString *sid = [info objectForKey:@"sid"];
	
	ChatViewData *theChatViewData = nil;
	NSArray * array = [viewControllers allValues];
	for (theChatViewData in array)
	{
		if ([theChatViewData.sid compare:sid] == NSOrderedSame)
		{
			[theChatViewData receivedAction:info];
			break;
		}
	}
}

- (void)notifyReceiveStatus: (NSNotification*)note
{
	LogMethod();
	NSDictionary * info = [note userInfo];
	NSString *sid = [info objectForKey:@"sid"];
	
	ChatViewData *theChatViewData = nil;
	NSArray * array = [viewControllers allValues];
	for (theChatViewData in array)
	{
		if ([theChatViewData.sid compare:sid] == NSOrderedSame)
		{
			[theChatViewData receivedStatus:info];
			break;
		}
	}
}

- (void)notifyReceiveData: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	NSDictionary * info = [note userInfo];
	NSString *sid = [info objectForKey:@"sid"];
	
	ChatViewData *theChatViewData = nil;
	NSArray * array = [viewControllers allValues];
	for (theChatViewData in array)
	{
		if ([theChatViewData.sid compare:sid] == NSOrderedSame)
		{
			[theChatViewData receivedData:info];
			break;
		}
	}
}

- (ChatViewData*)findChatViewDatabyImid:(NSString *)theImid sid:(NSString*)theSid
{
	NSString *key = nil;
	ChatViewData *theChatViewData = nil;

	NSArray * controllers = [viewControllers allValues];
	int i = 0;
	for (; i < [controllers count]; i++)
	{
		theChatViewData = [controllers objectAtIndex:i];
		if ([theChatViewData.contact.imid compare:theImid] == NSOrderedSame)
		{
			if ([theChatViewData.persons count] == 1)
			{
				if ([theChatViewData.sid compare:theSid] != NSOrderedSame)
				{
					theChatViewData = [theChatViewData retain];
					theChatViewData.sid = theSid;
					key = [[viewControllers allKeys] objectAtIndex:i];
					[viewControllers removeObjectForKey:key];
					key = [theImid stringByAppendingFormat:@"_%@",theSid];
					[self setChatviewData:theChatViewData forKey:key];
					[theChatViewData release];
				}
				break;
			}
			else
				theChatViewData = nil;
		}
		else
			theChatViewData = nil;
	}
	return theChatViewData;
}

- (ChatViewData*)findChatViewData:(NSString *)theJid sid:(NSString*)theSid
{
	NSString *key = nil;
	ChatViewData *theChatViewData = nil;
	key = [theJid stringByAppendingFormat:@"_%@",theSid];
	theChatViewData = [viewControllers objectForKey:key];
	
	if (theChatViewData == nil)
	{
		key = nil;
		NSArray * keys = [viewControllers allKeys];
		int i = 0;
		for (; i < [keys count]; i++)
		{
			key = [keys objectAtIndex:i];
			
			if (key != nil
				&& [theJid length] < [key length])
			{
				if ([[key substringToIndex:[theJid length]] compare:theJid] == NSOrderedSame)
				{
					theChatViewData = [viewControllers objectForKey:key];
					if ([theChatViewData.persons count] == 1)
					{
						if ([theChatViewData.sid compare:theSid] != NSOrderedSame)
						{
							theChatViewData = [theChatViewData retain];
							theChatViewData.sid = theSid;
							[viewControllers removeObjectForKey:key];
							key = [theJid stringByAppendingFormat:@"_%@",theSid];
							[self setChatviewData:theChatViewData forKey:key];
							[theChatViewData release];
						}
						break;
					}
					else
					{
						theChatViewData = nil;
					}
				}
			}
		}	
	}
	return theChatViewData;
}

- (ChatViewData*)findNextChatViewDataByJid: (NSString*)aJid
					   currentChatViewData: (ChatViewData*)aChatViewData{
	if ([aJid length] == 0) return nil;
	NSRange aRange;
	NSInteger i = 0;
	NSString* aKey = nil;
	BOOL aFound = NO;
	ChatViewData* theChatViewData = nil;
	
	NSArray* aKeys = [viewControllers allKeys];
	if (aChatViewData == nil){
		for (; i < [aKeys count]; i++){
			aKey = [aKeys objectAtIndex: i];
			aRange = [aKey rangeOfString: aJid];
			if (aRange.location != NSNotFound){
				theChatViewData = [viewControllers objectForKey: aKey];
				aFound = YES;
				break;
			}
		}	
	}
	else {
		for (; i < [aKeys count]; i++){			
			aKey = [aKeys objectAtIndex: i];
			aRange = [aKey rangeOfString: aJid];
			if (aRange.location != NSNotFound){
				theChatViewData = [viewControllers objectForKey: aKey];
				if (theChatViewData == aChatViewData){
					i++;
					break;
				}
			}
		}
		
		for (; i < [aKeys count]; i++){
			aKey = [aKeys objectAtIndex: i];
			aRange = [aKey rangeOfString: aJid];
			if (aRange.location != NSNotFound){
				theChatViewData = [viewControllers objectForKey: aKey];
				aFound = YES;
				break;
			}
		}
		
		if (!aFound){
			for (i = 0; i < [aKeys count]; i++){			
				aKey = [aKeys objectAtIndex: i];
				aRange = [aKey rangeOfString: aJid];
				if (aRange.location != NSNotFound){
					theChatViewData = [viewControllers objectForKey: aKey];
					if (theChatViewData != aChatViewData){
						aFound = YES;
						break;
					}
				}
			}
		}
	}
	return aFound ? theChatViewData : nil;
}

- (ChatViewData*)findP2PSessionByJid: (NSString*)aJid
				 currentChatViewData: (ChatViewData*)aChatViewData{
	ChatViewData* theChatViewData = nil;
	if ([aJid length] == 0) return nil;
	if (aChatViewData != nil && 
		[aChatViewData.contact.jid isEqualToString: aJid] &&
		[aChatViewData.persons count] == 1){
		return aChatViewData;
	}
	theChatViewData = aChatViewData;
	do {
		theChatViewData = [self findNextChatViewDataByJid: aJid
									  currentChatViewData: theChatViewData];
	}
	while (((theChatViewData != nil) && ([theChatViewData.persons count] > 1)));
	
	return theChatViewData;
}

- (ChatViewData*)findAnyChatViewDataByJid: (NSString*)aJid keyWords: (NSString*)aKeyWords{
    ChatViewData* aChatViewData = nil;
    if ([aJid length] == 0) return aChatViewData;
    NSString* anUndetermineKey = [NSString stringWithFormat: @"%@_%@", aJid, aKeyWords];
    NSString* aKey = nil;
    NSArray* anAllKeys = [viewControllers allKeys];
    for (aKey in anAllKeys){
        if ([aKey rangeOfString: anUndetermineKey].location != NSNotFound){
            aChatViewData = [viewControllers objectForKey: aKey];
            break;
        }
    }
    return aChatViewData;
}

- (ChatViewData*)findAnyChatViewDataByImid: (NSString*)anImid keyWords: (NSString*)aKeyWords{
    ChatViewData* aChatViewData = nil;
    if ([anImid length] == 0) return aChatViewData;
    NSString* anUndetermineKey = [NSString stringWithFormat: @"%@_%@", anImid, aKeyWords];
    NSString* aKey = nil;
    NSArray* anAllKeys = [viewControllers allKeys];
    for (aKey in anAllKeys){
        if ([aKey rangeOfString: anUndetermineKey].location != NSNotFound){
            aChatViewData = [viewControllers objectForKey: aKey];
            break;
        }
    }
    return aChatViewData;
}

- (NSString*)buildSidByJid: (NSString*)aJid sid: (NSString*)aSid{
	if ([aJid length] == 0 || [aSid length] == 0) return nil;
	
	NSRange aRange;
	NSString* aKey = nil;
	NSString* theSid = nil;
	NSInteger aValue = 0;
	NSString* aString = nil;
	NSString* aLeftString = nil;
	NSInteger anIndex = NSNotFound;
	NSArray* aKeys = [viewControllers allKeys];
	
	aRange = [aSid rangeOfCharacterFromSet: [NSCharacterSet decimalDigitCharacterSet]];
	if (aRange.location != NSNotFound){
		aString = [aSid substringFromIndex: aRange.location];
		aLeftString = [aSid substringToIndex: aRange.location];
		aValue = [aString intValue];
	}
	assert([aLeftString length] > 0);
	
	for (aKey in aKeys){
		aRange = [aKey rangeOfString: aJid];
		if (aRange.location != NSNotFound){
			anIndex = aRange.location + aRange.length + 1;
			theSid = [aKey substringFromIndex: anIndex];
			aRange = [theSid rangeOfCharacterFromSet: [NSCharacterSet decimalDigitCharacterSet]];
			if (aRange.location != NSNotFound){
				aString = [theSid substringFromIndex: aRange.location];
				if ([aString intValue] >= aValue){
					aValue = [aString intValue];
				}
			}
		}
	}
	return [NSString stringWithFormat: @"%@%d", aLeftString, ++aValue];
}

- (BOOL)isExistSessionKeyOfJid: (NSString*)aJid sid: (NSString*)aSid{
	assert([aJid length] > 0 && [aSid length] > 0);
	NSString* aKey = [aJid stringByAppendingFormat: @"_%@", aSid];
	NSArray* aKeys = [viewControllers allKeys];
	return [aKeys containsObject: aKey];
}


- (void)notifyReceivedMessage: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	LogMethod();
    NSRange aRange;
    BOOL aFound = NO;
    NSString *aKey = nil;
    NSString* anotherKey = nil;
    NSArray* anAllKeys = nil;
	NSDictionary * info = [note userInfo];
	NSString *type = [info objectForKey:@"type"];
	NSString *jid = [info objectForKey:@"jid"];
	NSString *imid = [info objectForKey:@"imid"];
	NSString *message = [info objectForKey:@"message"];
	NSString *sid = [info objectForKey:@"sid"];
	ChatViewData *theChatViewData = nil;
	NSString * name = nil;
    
//    NSLog(@"type : %@ jid : %@ imid : %@ message : %@ sid : %@", type, jid, imid, message, sid);
	if ([type isEqualToString: @"chat"]){
		if ([jid length] > 0){
			if ([sid length] == 0){
                theChatViewData = [[self findAnyChatViewDataByJid: jid keyWords: @"js"] retain];
                if (theChatViewData == nil){
                    srandom(time(NULL));
                    sid = [NSString stringWithFormat: @"js-%d",random()];
                    aKey = [jid stringByAppendingFormat: @"_%@",sid];
                }
			}
            else {
                aKey = [jid stringByAppendingFormat:@"_%@",sid];
                theChatViewData = [[self findChatViewData:jid sid:sid] retain];
                if (theChatViewData == nil){
                    anAllKeys = [viewControllers allKeys];
                    for (anotherKey in anAllKeys){
                        aRange = [anotherKey rangeOfString: jid];
                        if (aRange.location != NSNotFound){
                            aFound = YES;
                            break;
                        }
                    }
                    if (aFound && ![anotherKey isEqualToString: aKey]){
                        theChatViewData = [[viewControllers objectForKey: anotherKey] retain];
                        [viewControllers removeObjectForKey: anotherKey];
                        [viewControllers setObject: theChatViewData forKey: aKey];
                    }
                }
            }
			if (theChatViewData == nil){
				PersonInfo *person = [[ContactList getInstance] personWithJid:jid];
				if (person != nil){
					if (person.imid != nil){
						theChatViewData = [[self findChatViewData:person.imid sid:sid] retain];
					}
					if (theChatViewData == nil){
						theChatViewData = [[ChatViewData alloc] init];
						theChatViewData.contact = person;
						theChatViewData.sid = sid;
						[self setChatviewData: theChatViewData forKey: aKey];
					}
					else{
						theChatViewData.contact = person;
						[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:theChatViewData];
					}
				}
			}
			if (theChatViewData != nil){
				[theChatViewData receivedMessage:info];
                [theChatViewData release];
            }
		}
		else if ([imid length] > 0){
			if ([sid length] == 0){
                theChatViewData = [[self findAnyChatViewDataByImid: imid keyWords: @"jsm"] retain];
                if (theChatViewData == nil){
                    srandom(time(NULL));
                    sid = [NSString stringWithFormat:@"jsm-%d",random()];
                    aKey = [imid stringByAppendingFormat:@"_%@",sid];
                }
			}
            else {
                aKey = [imid stringByAppendingFormat:@"_%@",sid];
                theChatViewData = [[self findChatViewData:imid sid:sid] retain];
                if (theChatViewData == nil){
                    anAllKeys = [viewControllers allKeys];
                    for (anotherKey in anAllKeys){
                        aRange = [anotherKey rangeOfString: imid];
                        if (aRange.location != NSNotFound){
                            aFound = YES;
                            break;
                        }
                    }
                    if (aFound && ![anotherKey isEqualToString: aKey]){
                        theChatViewData = [[viewControllers objectForKey: anotherKey] retain];
                        [viewControllers removeObjectForKey: anotherKey];
                        [viewControllers setObject: theChatViewData forKey: aKey];
                    }
                }
                
                if (theChatViewData == nil){
                    theChatViewData = [[self findChatViewDatabyImid:imid sid:sid] retain];
                    if (theChatViewData == nil){
                        PersonInfo *person = [[PersonInfo alloc] init];
                        if (person != nil){
                            person.imid = imid;
                            theChatViewData = [[ChatViewData alloc] init];
                            theChatViewData.contact = person;
                            theChatViewData.sid = sid;
                            [self setChatviewData:theChatViewData forKey: aKey];
                        }
                        [person release];
                        person = nil;
                    }
                    else{
                        PersonInfo *unknown = [[PersonInfo alloc] init];
                        unknown.imid = imid;
                        theChatViewData.contact = unknown;
                        [unknown release];
                        [[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:theChatViewData];
                    }
                }
            }
			if (theChatViewData != nil){
				[theChatViewData receivedMessage:info];
			}
		}	
		name = theChatViewData.contact.nickname;
		if (name == nil)
			name = theChatViewData.contact.imid;
	}
	else if ([type isEqualToString: @"groupchat"]){
		theChatViewData = nil;
		NSArray *array = [viewControllers allValues];
		for (theChatViewData in array){
			if ([theChatViewData.sid isEqualToString: sid] ||
                [theChatViewData.contact.jid isEqualToString: jid]){
                if (![theChatViewData.sid isEqualToString: sid]){
                    theChatViewData.sid = sid;
                }
				if ([jid length] > 0){
					[theChatViewData receivedGroupMessage:info];
					PersonInfo *thePerson = [[ContactList getInstance] personWithJid:jid];
					name = thePerson.nickname;
					if (name == nil)
						name = thePerson.imid;
				}
				else if ([imid length] > 0){
					[theChatViewData receivedGroupMessage:info];
					name = imid;
				}
				break;
			}
		}
		if (theChatViewData == nil){
			if ([sid length] == 0){
                if ([jid length] > 0){
                    theChatViewData = [[self findAnyChatViewDataByJid: jid keyWords: @"gs"] retain];
                }
                else {
                    theChatViewData = [[self findAnyChatViewDataByImid: imid keyWords: @"gs"] retain];
                }
                if (theChatViewData == nil){
                    srandom(time(NULL));
                    if ([jid length] > 0){
                        sid = [NSString stringWithFormat:@"gs-%d",random()];
                        aKey = [jid stringByAppendingFormat: @"_%@", sid];
                    }
                    else{
                        sid = [NSString stringWithFormat:@"gsm-%d",random()];
                        aKey = [imid stringByAppendingFormat: @"_%@", sid];
                    }
                    
                    theChatViewData = [[ChatViewData alloc] init];
                    PersonInfo *person = [[ContactList getInstance] personWithJid:jid];
                    if (person == nil){
                        person = [[PersonInfo alloc] init];
                        person.jid = jid;
                        person.imid = imid;
                        theChatViewData.contact = person;
                        [person release];
                    }
                    else {
                        theChatViewData.contact = person;
                    }
                    person = nil;
                    theChatViewData.sid = sid;
                }
                [theChatViewData receivedGroupMessage:info];
                [self setChatviewData:theChatViewData forKey: aKey];
                [theChatViewData release];
			}
		}
		
		if (chatSelectController.chatViewDatas != viewControllers)
			chatSelectController.chatViewDatas = viewControllers;
	}
	if (theChatViewData != nil){
		UINavigationController* nvg = (UINavigationController*)[tabController selectedViewController];
		if (navigationController != nil 
			|| tabController.selectedIndex != [self getTheIndex:kChatVC]
			|| [nvg.topViewController isKindOfClass: [ChatSelectController class]]
			|| chatViewController.chatViewData != theChatViewData)
		{
			if ([navigationController.topViewController isKindOfClass: [ChatSelectController class]])
			{
				[(ChatSelectController*)navigationController.topViewController refreshVC: nil];
			}
			[self showReceivedMessageAlert:name message:message];
			theChatViewData.hasMessage += 1;
		}
	}
	
	int num = 0;
	for(ChatViewData* vData in [viewControllers allValues]){
		num = num + vData.hasMessage;
	}
	[self changeChatPicture:num];
}

- (void)notifyReceiveChatHeaderAd: (NSNotification*)note
{
	NSDictionary * info = [note userInfo];
	if (info != nil)
	{
		id advt = [info objectForKey:@"advt"];
		if (advt != nil && [advt isKindOfClass:[Advertisement class]])
		{
			self.chatViewAdvertise = advt;
		}
	}
}

- (void)notifyReceiveLoginProgAd: (NSNotification*)note
{
	NSDictionary * info = [note userInfo];
	if (info != nil)
	{
		id advt = [info objectForKey:@"advt"];
		if (advt != nil && [advt isKindOfClass:[Advertisement class]])
		{
			self.loginViewAdvertise = advt;
		}
	}
}

- (void)notifyUnregisterUser: (NSNotification*)aNotify{
	UIAlertView *anAlertView = nil;
	NSDictionary* aDict = [aNotify userInfo];
	self.systemNotice = [aDict objectForKey:@"system"];
	self.errCode = [aDict objectForKey: @"myErrCode"];
	if ([systemNotice count] > 0)
	{
		id anObject = [systemNotice objectAtIndex: 0];
		if ([anObject isKindOfClass: [NSMutableDictionary class]]){
			self.updateURL = [anObject objectForKey: @"url"];
			NSURL* aURL = [NSURL URLWithString: self.updateURL];
			[[UIApplication sharedApplication] openURL: aURL];
		}
		else {
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
													 message: [systemNotice objectAtIndex: 0]
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
										   otherButtonTitles: nil];
			[alerts addObject: anAlertView];
			[self showMyAlertView: anAlertView model:NO];
			[systemNotice removeObjectAtIndex: 0];
			[anAlertView release];
		}
	}
}

#pragma mark -
#pragma mark ApplicationDelegate methods.

- (void)applicationWillTerminate:(UIApplication *)application
{
	//[netWorkController sendLogOutRequest];
	application.networkActivityIndicatorVisible = NO;
	[[NSUserDefaults standardUserDefaults] setObject: myPassword forKey: kAPSPasswordKey];
	[[NSUserDefaults standardUserDefaults] synchronize];
	[self saveAccountInfo];
	[self garbage];
}


- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
	MYLog(@"%@  applicationDidReceiveMemoryWarning !!",application);
	UIAlertView *myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"MemoryNote",nil) 
														   message:NSLocalizedString(@"MemoryWarning",nil)  
														  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil)
												 otherButtonTitles:nil] autorelease];
	
	[alerts addObject:myAlertView];
	[myAlertView setDelegate:self];
	[self showMyAlertView:myAlertView model:YES];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    /*
    //TODO: test parse code
    const char *str = "<iq type='result' from='msn.pica' to='52724222@pica/pica' addition='1' last='1'><query xmlns='jabber:iq:group' version='1|2010-05-13T05:27:33.3230000Z|2010-05-13T05:13:22.9400000Z|2010-05-13T06:33:50.7000000Z'><group groupid='0' name='Other contacts' gbcode='6f746865' sync='updated'/></query></iq><iq type='result' from='msn.pica' to='52724222@pica/pica' addition='1' last='1'><query xmlns='jabber:iq:roster' version='1|2010-05-13T05:27:33.3230000Z|2010-05-13T05:13:22.9400000Z|2010-05-13T06:33:50.7000000Z'><item jid='a5a55ac2-e68e-4496-9b1a-26697e73537d@msn.pica' block='0' sync='updated'/></query></iq><presence type='available' from='a5a55ac2-e68e-4496-9b1a-26697e73537d@msn.pica' to='52724222@pica/pica'><show>online</show><impresa/><portraithash>LlAZ+lp6VAYvrczPryecymfnJNc=</portraithash></presence><presence type='available' from='a5a55ac2-e68e-4496-9b1a-26697e73537d@msn.pica' to='52724222@pica/pica'><show>online</show><portraithash>LlAZ+lp6VAYvrczPryecymfnJNc=</portraithash></presence><presence type='available' from='a5a55ac2-e68e-4496-9b1a-26697e73537d@msn.pica' to='52724222@pica/pica'><show/><impresa/></presence>";
    ClientNetWorkController *net = [[ClientNetWorkController alloc] init];
    NSData *data = [NSData dataWithBytes:(const void *)str length:strlen(str)];
    if(data)
        [net praseLoginResponseData:data];
     */
    
	shouldHideAlertMsg = NO;
	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	[window setBackgroundColor:[UIColor whiteColor]];
    [window makeKeyAndVisible];
	if ([[NSUserDefaults standardUserDefaults] objectForKey: kAPSDurationKey] == nil){
        [[NSUserDefaults standardUserDefaults] setInteger: 6 forKey: kAPSDurationKey];
    }
    
#if 0
    unsigned char* ptr = "610eacea6f93a628ac618cc8e3212ba908210fbc5edfe0f5e0867535110165ff";
    deviceToken = (unsigned char*)malloc(65);
    strcpy(deviceToken, ptr);
    deviceToken[64] = 0;
#endif
    
	if ([application respondsToSelector: @selector(registerForRemoteNotificationTypes:)]){
		[application registerForRemoteNotificationTypes: 7];
	}
	if (application.applicationIconBadgeNumber > 0){
		NSUserDefaults* anUserDefaults = [NSUserDefaults standardUserDefaults];
		self.myAccount = [anUserDefaults objectForKey: kLastAccountKey];
		NSMutableDictionary *anAccounts = [anUserDefaults objectForKey: kAccountsKey];
		self.infoDict = [anAccounts objectForKey: myAccount];
		self.myStatus = [infoDict objectForKey: kLoginStatusKey];
		self.myPassword = [anUserDefaults objectForKey: kAPSPasswordKey];
		sleep(5);
		LoginFlashViewController *loginFlashViewController = [[LoginFlashViewController alloc] init];
		UINavigationController *tmpController = [[UINavigationController alloc] initWithRootViewController:loginFlashViewController];
		self.navigationController = tmpController;
		[tmpController release];
		[navigationController setNavigationBarHidden:YES animated:NO];
		[loginFlashViewController release];
		
		[window addSubview: navigationController.view];
	
		[self showLoginFlashView];
	}
	else {
		NSDictionary *accountsDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
		if (accountsDictionary != nil)
		{
			self.myAccount = [[NSUserDefaults standardUserDefaults] objectForKey:kLastAccountKey];
			if (myAccount != nil)
			{
				NSDictionary *dict = [accountsDictionary objectForKey:myAccount];
				if (dict != nil)
				{
					if ([[dict objectForKey:kAutoLoginKey] boolValue])
					{
						sleep(5);
						[self login];
						return;
					}
				}
			}		
		}
		[self showLoginWindow];
	}
	
	application.applicationIconBadgeNumber = 0;
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)aDeviceToken{
//    NSLog(@"aDeviceToken : %@", aDeviceToken);
	NSInteger aLength = [aDeviceToken length];
	if (aLength == 0) return;
	unsigned char buf[32];
	memset(buf, 0, 32);
	[aDeviceToken getBytes: buf length: 32];
	if (deviceToken != NULL) free(deviceToken);
	deviceToken = malloc(aLength * 2 + 1);
	unsigned char* p = deviceToken;
	NSInteger i = 0;
	for (; i < aLength; i++){
		NSString* aValue = [NSString stringWithFormat: @"%02x", buf[i]];
		memcpy(p, [aValue UTF8String], [aValue length]);
		p += [aValue length];
	}
	deviceToken[aLength * 2] = 0;
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error{
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
	application.applicationIconBadgeNumber = 0;
}

#pragma mark -
#pragma mark alertViewController delegate methods.
- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}


- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    pcAlertAndStrangerInfoObject* anObject = nil;
	UIAlertView *anAlertView = nil;
	if (!sysDismiss)
	{
		if ([alertView.title compare:NSLocalizedString(@"pica_str_cmd_login",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1)
				[self showLoginFlashView];
			else
				[self showLoginWindow];
		}
		else if ([alertView.title compare:NSLocalizedString(@"Login failed",nil)] == NSOrderedSame)
		{
			[self logout];
		}
		else if ([alertView.title compare:NSLocalizedString(@"Network error",nil)] == NSOrderedSame)
		{
			[self logoutWithNetworkConnected:NO];
			if (buttonIndex == 1)
			{
				needLogin = YES;
				[self login];
			}
		}
		else if ([alertView.title isEqualToString: NSLocalizedString(@"qtn_buddy_refresh",nil)])
		{
			if (buttonIndex == 1){
				[self restart];
			}
		}
		else if ([alertView.title compare:NSLocalizedString(@"Error",nil)] == NSOrderedSame)
		{
			if (errorCode == 600
				|| errorCode == 401
				|| errorCode == 1401
				|| errorCode == 1402
				|| errorCode == 1600)
			{
				[self logout];
			}
			else if (errorCode == 1403
					 || errorCode == 1601)
			{
				[self logoutWithNetworkConnected:NO];
				[self login];	
			}
			else if (errorCode != 1400
					 && !(errorCode > 1404 && errorCode < 1412)
					 && !(errorCode > 1500 && errorCode < 1505)
					 )
			{
				//[self logout];
			}
			
			errorCode = 0;
		}
		else if ([alertView.title isEqualToString: NSLocalizedString(@"MemoryNote",nil)])
		{
            [self logout];
		}
		else if ([alertView.title isEqualToString: NSLocalizedString(@"qtn_msn_member_addtobuddy",nil)])
		{
            for (anObject in strangers){
                if (anObject.alertObject == alertView){
                    [netWorkController sendAnswerStrangerRequest: anObject.strangerObject.imid add: (buttonIndex == 0)];
                    [strangers removeObject: anObject];
                    break;
                }
            }
		}
		else if (errCode != nil){
			NSURL* aURL = nil;
			NSString* version = nil;
			switch ([errCode intValue]) {
				case 0:
					if ([systemNotice count] > 0){
						anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
																 message: [systemNotice objectAtIndex: 0]
																delegate: self
													   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
													   otherButtonTitles: nil];
						[alerts addObject: anAlertView];
						[self showMyAlertView: anAlertView model:NO];
						[systemNotice removeObjectAtIndex: 0];
						[anAlertView release];
					}
					else {
						[self logout];
						self.errCode = nil;
						self.systemNotice = nil;
					}
					break;
				case 1:
					//: TODO
					if (systemNotice == nil){
						if ([updateURL length] > 0){
							aURL = [NSURL URLWithString: updateURL];
							if (aURL == nil) exit(1);
							[[UIApplication sharedApplication] openURL: aURL];
						}
						[self logout];
					}
					else {
						if ([systemNotice count] > 0){
							id anObject = [systemNotice objectAtIndex: 0];
							if ([anObject isKindOfClass: [NSMutableDictionary class]]){
								version = [anObject objectForKey:@"version"];
								self.updateURL = [anObject objectForKey: @"url"];
								anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"Upgrade",nil) 
																		 message: [NSString stringWithFormat:NSLocalizedString(@"UpgradeVersion",nil), version] 
																		delegate: self
															   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
															   otherButtonTitles: nil];	
								[alerts addObject: anAlertView];
								[self showMyAlertView: anAlertView model:NO];
								[systemNotice removeObjectAtIndex: 0];
								[anAlertView release];
							}
							else {
								anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
																		 message: anObject
																		delegate: self
															   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
															   otherButtonTitles: nil];
								[alerts addObject: anAlertView];
								[self showMyAlertView: anAlertView model:NO];
								[systemNotice removeObjectAtIndex: 0];
								[anAlertView release];
							}
							if ([systemNotice count] == 0)
								self.systemNotice = nil;
						}
						else {
							if ([self.updateURL length] > 0){
								if (buttonIndex == 1){
									aURL = [NSURL URLWithString: updateURL];
									[[UIApplication sharedApplication] openURL: aURL];
								}
								self.updateURL = nil;
							}
							self.errCode = nil;
							self.systemNotice = nil;
							[[ClientNetWorkController getInstance] performSelectorOnMainThread:@selector(sendDisPatchRequest) withObject:nil waitUntilDone:YES];
						}
					}
					break;
				case 2:
					if (systemNotice == nil){
						if ([updateURL length] > 0){
							if (buttonIndex == 1){
								aURL = [NSURL URLWithString: updateURL];
								if (aURL == nil) exit(1);
								[[UIApplication sharedApplication] openURL: aURL];
							}
							else {
								self.updateURL = nil;
								self.errCode = nil;
								self.systemNotice = nil;
								[[ClientNetWorkController getInstance] performSelectorOnMainThread:@selector(sendDisPatchRequest) withObject:nil waitUntilDone:YES];
							}
						}
						else {
							[[ClientNetWorkController getInstance] performSelectorOnMainThread:@selector(sendDisPatchRequest) withObject:nil waitUntilDone:YES];
						}
					}
					else {
						if ([systemNotice count] > 0){
							id anObject = [systemNotice objectAtIndex: 0];
							if ([anObject isKindOfClass: [NSMutableDictionary class]]){
								self.updateURL = [anObject objectForKey: @"url"];
								anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"text_is_upgrade_version",nil) 
																		 message: NSLocalizedString(@"text_is_upgrade_now",nil)
																		delegate: self
															   cancelButtonTitle: NSLocalizedString(@"text_upgrade_cancel",nil)
															   otherButtonTitles: NSLocalizedString(@"text_upgrade_ok",nil), nil];
								[alerts addObject: anAlertView];
								[self showMyAlertView: anAlertView model:NO];
								[systemNotice removeObjectAtIndex: 0];
								[anAlertView release];
							}
							else {
								anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
																		 message: anObject
																		delegate: self
															   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
															   otherButtonTitles: nil];
								[alerts addObject: anAlertView];
								[self showMyAlertView: anAlertView model:NO];
								[systemNotice removeObjectAtIndex: 0];
								[anAlertView release];
							}
						}
						else {
							if ([self.updateURL length] > 0){
								if (buttonIndex == 1){
									aURL = [NSURL URLWithString: updateURL];
									[[UIApplication sharedApplication] openURL: aURL];
								}
								self.updateURL = nil;
							}
							[[ClientNetWorkController getInstance] performSelectorOnMainThread:@selector(sendDisPatchRequest) withObject:nil waitUntilDone:YES];
							self.errCode = nil;
							self.systemNotice = nil;
						}
					}
					break;
				case 4:
					if ([systemNotice count] > 0){
						id anObject = [systemNotice objectAtIndex: 0];
						if ([anObject isKindOfClass: [NSMutableDictionary class]]){
							version = [anObject objectForKey:@"version"];
							self.updateURL = [anObject objectForKey: @"url"];
							NSURL* aURL = [NSURL URLWithString: self.updateURL];
							[[UIApplication sharedApplication] openURL: aURL];
						}
						else {
							anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_system_msg_title",nil) 
																	 message: anObject
																	delegate: self
														   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil) 
														   otherButtonTitles: nil];
							[alerts addObject: anAlertView];
							[self showMyAlertView: anAlertView model:NO];
							[systemNotice removeObjectAtIndex: 0];
							[anAlertView release];
						}
						if ([systemNotice count] == 0)
							self.systemNotice = nil;
					}
					break;
			}
		}

		
		if ([alerts indexOfObject:alertView] != NSNotFound)
		{
			alertView.delegate = nil;
			[alerts removeObject:alertView];
		}
		sysDismiss = YES;
	}
}

#pragma mark -
#pragma mark tab bar controller delegate methods.
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController
{
	NSArray* aControllers = [tabBarController viewControllers];
	UIViewController* aChatViewNavigation = [aControllers objectAtIndex: 2];
	if ((aChatViewNavigation != viewController) &&
		([aChatViewNavigation isKindOfClass: [UINavigationController class]])){
			UIViewController* aController = [aChatViewNavigation topViewController];
			if ([aController isKindOfClass: [OnlineListController class]]){
				[aController.navigationController popViewControllerAnimated: NO];
		}
	}
}
//增加 未读消息提示功能 20100310
- (void)showViews: (UIView*)aView level: (NSInteger) aLevel anum:(NSInteger*)num{
	NSArray* aSubViews = [aView subviews];
	for (UIView* aSubview in aSubViews){
		//		NSLo.g(@"aSubview : %@ aLevel : %d", aSubview, aLevel + 1);
		//[self showViews: aSubview level: aLevel + 1];
		Class aClass = [aSubview class];
		NSString *aDescription = [aClass description];
		if ([aDescription isEqualToString: @"UIImageView"]){			
			for (UIView* redSubView in aSubview.subviews)
			{
				if(num == 0)
				{
					redSubView.hidden = TRUE;
					return;
				}
				else
				{
					redSubView.hidden = NO;
					if(num >= 10)
					{
						UIImageView * iw = (UIImageView*)redSubView;
						iw.frame = CGRectMake(20, -5, 32, 24);
						iw.image = [UIImage imageNamed:@"info_warrning_big.png"];
					}
					else
					{
						UIImageView * iw = (UIImageView*)redSubView;
						iw.frame = CGRectMake(20, -5, 24, 24);
						iw.image = [UIImage imageNamed:@"info_warrning.png"];
					}
					
					for(UIView* labView in redSubView.subviews)
					{
						UILabel* lv = (UILabel*)labView;		
						if(num >=10)
						{
							lv.frame = CGRectMake(4, -2, 24, 24);
						}
						else
						{
							lv.frame = CGRectMake(0, -2, 24, 24);
						}
						NSString* str = [NSString stringWithFormat:@"%d",num];
						lv.text = str;
					}
					return;
				}
			}
			
			if(num == 0)
			{
				return;
			}
			
			UIImageView* imgView = [[UIImageView alloc] initWithFrame: CGRectMake(20, -5, 24, 24)];
			UIImage* img = [UIImage imageNamed:@"info_warrning.png"];
			[imgView setImage:img];
			
			UILabel* aLabel = [[UILabel alloc] initWithFrame: CGRectMake(0, -2, 24, 24)];
			aLabel.font = [UIFont systemFontOfSize: 12];
			aLabel.textAlignment = UITextAlignmentCenter;
			NSString* str = [NSString stringWithFormat:@"%d",num];
			aLabel.text = str;
			aLabel.font = [UIFont boldSystemFontOfSize: 12];
			aLabel.textColor=[UIColor whiteColor];
			aLabel.backgroundColor=[UIColor clearColor];
			[imgView addSubview:aLabel];

			[aSubview addSubview: imgView];
			[imgView release];
			[aLabel release];
			return;
		}
	}
}

- (IBAction)changeChatPicture: (NSInteger)num{
	
	UISegmentedControl* segCtl = [tabController segmentedControl];
	int i = 0;
	for (UIImageView* view in segCtl.subviews)
	{
		if([tabController chatImageView] == view)
		{
			[self showViews: view level: 0 anum:num];
			return;
		}
		i++;
	}	
}


- (void)notifyUpdateMessage
{
	int num = 0;
	for(ChatViewData* vData in [viewControllers allValues])
	{
		num = num + vData.hasMessage;
	}
	[self changeChatPicture:num];
}
//end
@end
