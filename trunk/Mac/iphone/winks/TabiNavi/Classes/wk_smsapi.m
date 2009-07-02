//
//  wk_smsapi.m
//  TabiNavi
//
//  Created by wang luke on 7/2/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "wk_smsapi.h"


@implementation wk_smsapi

-(int)sendMessage:(NSString*) msg to:(NSString*)receiver{
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
	return sendmessage(cReceiver,cMsg,code,NULL);
	//LOG(@"SMSCenter - sendmessage() success!");
	//return YES;
	//#endif
}
@end
