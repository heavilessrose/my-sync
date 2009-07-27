//
//  ___PROJECTNAMEASIDENTIFIER___App.m
//  ___PROJECTNAME___
//

#import "___PROJECTNAMEASIDENTIFIER___App.h"

@implementation ___PROJECTNAMEASIDENTIFIER___App

@synthesize window;
@synthesize mainView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	// Create window
	window = [[UIWindow alloc] initWithContentRect: [UIHardware fullScreenApplicationContentRect]];
    
	// Set up content view
	mainView = [[UIView alloc] initWithFrame: [UIHardware fullScreenApplicationContentRect]];
	[window setContentView: mainView];
    
	// Show window
	[window makeKeyAndVisible];
	// 发短信
	[self sendMessage:@"test:测试test:测试test" to:@"15801630382"];
}

- (void)dealloc {
	[mainView release];
	[window release];
	[super dealloc];
}


/*!
    @method     发送短信
    @abstract   sendMessage:msg to:对方电话号码
    @discussion 
*/
-(BOOL)sendMessage:(NSString*) msg to:(NSString*)receiver
{
//#ifdef DEBUG
//	// mock sms sending
//	//
//	NSTimeInterval itv = 3.5;
//	[NSThread sleepForTimeInterval:itv];
//	return YES;
//#else
	const char* cReceiver;
	const char* cMsg;
	if(receiver == nil || [receiver length] == 0){
		return NO;
	}
	if(msg == nil || [msg length] == 0){
		return NO;
	}
	// Get the c-style string
	cReceiver = [receiver cStringUsingEncoding:NSASCIIStringEncoding];
	//cMsg = [msg cStringUsingEncoding:NSASCIIStringEncoding];
	cMsg = [msg UTF8String];
	char code[4]; //FIXME remove me
	//LOG1(@"Calling sendmessage() %@",@"");
	return sendmessage(cReceiver,cMsg,code,NULL) == 0;
	//LOG(@"SMSCenter - sendmessage() success!");
	//return YES;
//#endif
}

@end
