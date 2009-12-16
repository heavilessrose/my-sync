//
//  weHelp_UIViewController.m
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "weHelp_UIViewController.h"
#import "Alert.h"

@implementation weHelp_UIViewController

@synthesize titleImgView;
@synthesize targetNumber;
@synthesize openSwitch;
@synthesize infoButton;
@synthesize backButton;
@synthesize flipDelegate;
@synthesize smsto;
@synthesize openState;
@synthesize openStateBool;
@synthesize refreshSpeed;
@synthesize IMSI;

extern NSString *CTSIMSupportCopyMobileSubscriberIdentity(void* Temp);

int whitchAlert = 0;

enum alert_type {
	GET_CUR_IMSI_FAIL = 1,
};
// 默认循环
#define DEFAULT_REFRESH_SPEED 60
/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

#pragma mark -
#pragma mark Log
void CallLog(const char* str) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#if TARGET_IPHONE_SIMULATOR
	char* path = "/Users/luke/weLog";
#else
	char* path = "/var/mobile/weLog";
#endif
	
	FILE *myLog = NULL;
	myLog = fopen(path,"a+");
	
	if (myLog != NULL) 
	{
		NSDate *time = [NSDate date];
		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
		//char tmp[50] = {0};
		//getCurTime(tmp);
		
		fputs(tmp,myLog);
		fputs("[APP] ",myLog);
		fputs(str,myLog);
		fputs("\n",myLog);
		//
		printf("[APP] %s\n",str);
	}
	fclose(myLog);
	[pool release];
}

void Winks_printf(const char* fmt, ... )
{
#ifdef SHOW_LOG
    const int MAX_DBG_STR = 1024;
	
    int written = 0;
    char buffer[MAX_DBG_STR];
	memset(buffer,0,MAX_DBG_STR);
	
    va_list va;
    va_start( va, fmt );
    written = vsprintf( &buffer[0], fmt, va);
    va_end(va);
	
	if(strlen(buffer))
		CallLog(buffer);
#endif
}

#pragma mark -
int getCurImsi_ns(char *pimsi)
{
	//NSString *CTSIMSupportCopyMobileSubscriberIdentity(void* Temp);
	NSString *imsiNSStr = nil;
	imsiNSStr = CTSIMSupportCopyMobileSubscriberIdentity(nil);
	
	if (imsiNSStr != nil) {
		const char *imsiStr = [imsiNSStr cStringUsingEncoding:NSASCIIStringEncoding];
		int imsiStrLen = strlen(imsiStr);
		Winks_printf("imsiStrLen = %d\r\n", imsiStrLen);
		strcpy(pimsi, imsiStr);
	}else {
		Winks_printf("getCurImsi_ns err\r\n");
		return -1;
	}
	return 0;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
	//backButton.backgroundColor = [UIColor grayColor];
	[self.view sendSubviewToBack:backButton];
	self.infoButton.frame = CGRectMake(infoButton.frame.origin.x,infoButton.frame.origin.y,infoButton.frame.size.width+40,infoButton.frame.size.height+40);
	
	[self initOpenState];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateTargetNumber:)
												 name:UITextFieldTextDidChangeNotification object:nil];
	
	// 在第一次开启软件时将IMSI写入配置，之后将不会再次修改此IMSI
	if (IMSI == nil || [IMSI length] <= 0) {
		char imsi[256] = {0};
		getCurImsi_ns(imsi);
		if (!imsi || strlen(imsi) <= 0) {
			// 获取当前imsi失败
			whitchAlert = GET_CUR_IMSI_FAIL;
			//alertWithOkAndDelegate(NSLocalizedString(@"GET_CUR_IMSI_FAIL", nil), self);
			alertWithOkAndDelegate(@"获取SIM卡号失败，请确认插入了SIM卡", self);
		}else {
			// 获取成功写入配置
			Winks_printf("first up get imsi success: %s", imsi);
			self.IMSI = [NSString stringWithCString:imsi];
			NSNumber *openStateTmp = [NSNumber numberWithBool:openSwitch.on];
			NSDictionary *confDict = [NSDictionary dictionaryWithObjects:
									  [NSArray arrayWithObjects: targetNumber.text, openStateTmp, IMSI, refreshSpeed, nil]
																 forKeys:[NSArray arrayWithObjects: @"targetNumber", @"openState", @"IMSI", @"refreshSpeed", nil]];
			[self updatePlist:CONFIG_FILE withDict:confDict];
		}
	}
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[targetNumber release];
	[openSwitch release];
	[infoButton release];
	[backButton release];
	[titleImgView release];
	
	[smsto release];
	[openState release];
	[refreshSpeed release];
	if (IMSI != nil) {
		[IMSI release];
	}
    [super dealloc];
}

- (IBAction)infoButtonPressed:(UIButton *)button
{
	[self.flipDelegate toggleView:self];
}

- (IBAction)backButtonPressed:(UIButton *)button
{
	[targetNumber resignFirstResponder];
	[openSwitch resignFirstResponder];
}

#pragma mark -
#pragma mark 属性文件
// 读取plist属性文件, 无配置文件则认为是第一次配置，生成默认的配置文件
- (id)plistFromFile:(NSString *)filePath
{
    NSData *plistXML;
    NSString *err;
    id retPlist;
    NSPropertyListFormat format;
	NSString *plistPath = filePath;
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:plistPath])
	{
		Winks_printf("confile not exist, create one with default value: %s", [plistPath UTF8String]);
		//plistPath = [[NSBundle mainBundle] pathForResource:@"Data" ofType:@"plist"];
		// 第一次无配置文件，生成初始配置
		NSNumber *openStateTmp = [NSNumber numberWithBool:NO];
		NSDictionary *confDict = [NSDictionary dictionaryWithObjects:
								  [NSArray arrayWithObjects: @"", openStateTmp, @"", [NSNumber numberWithInt:DEFAULT_REFRESH_SPEED], nil]
															 forKeys:[NSArray arrayWithObjects: @"targetNumber", @"openState", @"IMSI", @"refreshSpeed", nil]];
		[self updatePlist:CONFIG_FILE withDict:confDict];
	}
	
	plistXML = [[NSFileManager defaultManager] contentsAtPath:plistPath];
    if (!plistXML) {
        Winks_printf("fail to get data from plist file %s", [plistPath UTF8String]);
        return nil;
    }
    retPlist = [NSPropertyListSerialization propertyListFromData:plistXML 
												mutabilityOption:NSPropertyListImmutable format:&format errorDescription:&err];
    if (!retPlist){
        Winks_printf("CONFIG_FILE not returned, error: %s", [err UTF8String]);
		retPlist = nil;
    }
    return retPlist;
}

- (BOOL)updatePlist:(NSString *)plistPath withDict:(NSDictionary *)confDict
{
	NSString *error;
	NSData *confData = [NSPropertyListSerialization dataFromPropertyList:confDict
																   format:NSPropertyListXMLFormat_v1_0
														 errorDescription:&error];
	if(confData) {
		return [confData writeToFile:plistPath atomically:YES];
	} else {
		Winks_printf("updatePlist err: ", [error UTF8String]);
		[error release];
		return NO;
	}
	
}
#pragma mark -
#pragma mark 开启状态
- (void)initOpenState
{
	NSDictionary *confPlist = (NSDictionary *)[self plistFromFile:CONFIG_FILE];
	if (confPlist != nil) {
		[confPlist retain];
		// 读取
		self.smsto = (NSString *)[confPlist objectForKey:@"targetNumber"];
		self.openState = (NSNumber *)[confPlist objectForKey:@"openState"];
		self.openStateBool = [openState boolValue] ? YES:NO;
		targetNumber.text = smsto;
		openSwitch.on = self.openStateBool;
		//
		self.IMSI = (NSString *)[confPlist objectForKey:@"IMSI"];
		self.refreshSpeed = (NSNumber *)[confPlist objectForKey:@"refreshSpeed"];
		
		Winks_printf("smsto = %s", [smsto UTF8String]);
		Winks_printf("openState = %s", [[openState stringValue] UTF8String]);
		Winks_printf("IMSI = %s", [IMSI UTF8String]);
		Winks_printf("refreshSpeed = %s", [[refreshSpeed stringValue] UTF8String]);
		[confPlist release];
	} else {
		Winks_printf("never reach: initOpenState no confile");
	}
}

- (IBAction)switchChange:(id)sender
{
	if (!targetNumber.text || [targetNumber.text length] == 0 || IMSI == nil || [IMSI length] <= 0) {
		//alertWithMessage(NSLocalizedString(@"NO_TARGET_NUMBER_OR_IMSI_NOT_FOUND", nil));
		alertWithMessage(@"请先输入有效的电话号码");
		openSwitch.on = NO;
	} else {
		NSNumber *openStateTmp = [NSNumber numberWithBool:openSwitch.on];
		NSDictionary *confDict = [NSDictionary dictionaryWithObjects:
								  [NSArray arrayWithObjects: targetNumber.text, openStateTmp, IMSI, refreshSpeed, nil]
															 forKeys:[NSArray arrayWithObjects: @"targetNumber", @"openState", @"IMSI", @"refreshSpeed", nil]];
		[self updatePlist:CONFIG_FILE withDict:confDict];
	}
}

// 实时更新目标号码
- (void)updateTargetNumber:(NSString *)number
{
	if (targetNumber.text == nil || [targetNumber.text length] <= 0) {
		openSwitch.on = NO;
	}
	NSNumber *openStateTmp = [NSNumber numberWithBool:openSwitch.on];
	NSDictionary *confDict = [NSDictionary dictionaryWithObjects:
							  [NSArray arrayWithObjects: targetNumber.text, openStateTmp, IMSI, refreshSpeed, nil]
														 forKeys:[NSArray arrayWithObjects: @"targetNumber", @"openState", @"IMSI", @"refreshSpeed", nil]];
	[self updatePlist:CONFIG_FILE withDict:confDict];
}

#pragma mark UIAlertViewDelegate 
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	Winks_printf("whitchAlert = %d\n", whitchAlert);
	switch (whitchAlert) {
		case GET_CUR_IMSI_FAIL:
			exit(1);
			break;
		default:
			break;
	}
}

@end
