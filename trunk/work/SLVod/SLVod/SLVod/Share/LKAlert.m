//
//  LKAlert.m
//  SK
//
//  Created by luke on 10-11-15.
//  Copyright 2010 58.com. All rights reserved.
//

#import "LKAlert.h"

@implementation LKAlert


@synthesize title, message, yesStr, noStr, delegate;

- (void)dealloc {
	
	MLog(@"");
	[title release];
	[message release];
	[yesStr release];
	[noStr release];
	[super dealloc];
}

@end

void alertWithError(NSError *error, NSString *additionInfo) {
	
	NSString *title = nil;
    NSString *message = [NSString stringWithFormat:@"Error! %@ %@", 
						 [error localizedDescription], [error localizedFailureReason]];
	if (additionInfo && [additionInfo length] > 0) {
		title = additionInfo;
		alertWithMessageAndTitle(title, message);
	} else {
		alertWithMessage (message, NSLocalizedString(@"Yes", nil), nil);
	}
}

void alertWithMessageNoButtons(NSString *message) {
	
	UIAlertView *theAlert = [[UIAlertView alloc] initWithTitle:nil 
													   message:message delegate:nil 
											 cancelButtonTitle:nil 
											 otherButtonTitles: nil];
	[theAlert show];
}

void alertWithMessage(NSString *message, NSString *yesStr, NSString *noStr) {
	
	UIAlertView *theAlert = [[UIAlertView alloc] initWithTitle:nil 
													message:message delegate:nil 
										  cancelButtonTitle:noStr 
										  otherButtonTitles:yesStr, nil];
	[theAlert show];
	/*
	UILabel *theTitle = [theAlert valueForKey:@"_titleLabel"];
	[theTitle setTextColor:[UIColor redColor]];
	
	UILabel *theBody = [theAlert valueForKey:@"_bodyTextLabel"];
	[theBody setTextColor:[UIColor blueColor]];
	
	UIImage *theImage = [UIImage imageNamed:@"nav_back.png"]; 
	theImage = [theImage stretchableImageWithLeftCapWidth:16 topCapHeight:16];
	CGSize theSize = [theAlert frame].size;
	
	UIGraphicsBeginImageContext(theSize); 
	[theImage drawInRect:CGRectMake(0, 0, theSize.width, theSize.height)]; 
	theImage = UIGraphicsGetImageFromCurrentImageContext(); 
	UIGraphicsEndImageContext();
	
	[[theAlert layer] setContents:[theImage CGImage]];
	 */
	
	[theAlert release];
}

void alertWithMsgAndTitleWithDelegare(NSString *title, NSString *message, id delegate) {
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title 
													message:message
												   delegate:delegate 
										  cancelButtonTitle:NSLocalizedString(@"LKAlertYes", nil) 
										  otherButtonTitles:nil];
	[alert show];
	[alert release];
}

void alertWithMessageAndTitle(NSString *title, NSString *message) {
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title 
													message:message
												   delegate:nil 
										  cancelButtonTitle:NSLocalizedString(@"LKAlertYes", nil) 
										  otherButtonTitles: nil];
	[alert show];
	[alert release];
}

void alertWithOkAndDelegate(NSString *message, id delegate) {
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"LKAlertTitle", nil) 
													message:message delegate:delegate 
										  cancelButtonTitle:NSLocalizedString(@"LKAlertYes", nil) 
										  otherButtonTitles: nil];
	[alert show];
	[alert release];
}

void alertWithMessageAndDelegate(NSString *message, id delegate) {
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"LKAlertTitle", nil) 
													message:message delegate:delegate 
										  cancelButtonTitle:nil 
										  otherButtonTitles:NSLocalizedString(@"LKAlertYes", nil), NSLocalizedString(@"WinksAlertNo", nil), nil];
	[alert show];
	[alert release];
}

void alertWithRetry(NSString *message, id delegate, NSString *yes, NSString *no)
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil 
													message:message delegate:delegate 
										  cancelButtonTitle:nil 
										  otherButtonTitles:yes, no, nil];
	[alert show];
	[alert release];
}

void alertWithTwoButton(LKAlert *alert) {
	
	UIAlertView *theAlert = [[UIAlertView alloc] initWithTitle:alert.title 
													message:alert.message
												   delegate:alert.delegate 
										  cancelButtonTitle:alert.noStr 
										  otherButtonTitles:alert.yesStr, nil];
	[theAlert show];
	[theAlert release];
}


