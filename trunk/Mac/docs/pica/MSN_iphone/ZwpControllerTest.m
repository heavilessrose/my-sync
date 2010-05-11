//
//  ZwpControllerTest.m
//  MSN
//
//  Created by Jun Du on 5/11/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ZwpController.h"

@interface ZwpControllerTest : GHTestCase
{
	ZwpController *zwpController;
}
@end

@implementation ZwpControllerTest

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		zwpController = [[ZwpController alloc] init];
	}
	return self;
}

- (void)dealloc 
{
	[zwpController release];
	[super dealloc];
}

#pragma mark GHTestCase protocols.
- (void)setUp 
{
	
}

- (void)tearDown 
{
	
}

#pragma mark -
#pragma mark new test

- (void)testAddbuddy
{
	zwpController.htmlString = @"<html><body><a href=\"addbuddy:9025@pica.com\">addbuddy:9025@pica.com</a></body></html>";
	
	NSDictionary *infoDict = [NSDictionary dictionaryWithObject:zwpController.htmlString forKey:@"html"];
	NSNotification *note = [NSNotification notificationWithName:@"test" object:nil userInfo:infoDict];

	// Just for console output msg.
	[zwpController notifyZwpDataChanged:note];
	return;
}

@end
