//
//  Alert.m
//  ColorSet
//
//  Created by wang luke on 5/7/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "Alert.h"

void alertWithError(NSError *error)
{
    NSString *message = [NSString stringWithFormat:@"Error! %@ %@", [error localizedDescription], [error localizedFailureReason]];
	alertWithMessage (message);
}


void alertWithMessage(NSString *message)
{
	/* open an alert with an OK button */
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil 
													message:message delegate:nil 
										  cancelButtonTitle:@"确定" 
										  otherButtonTitles: nil];
	[alert show];
	[alert release];
}

void alertWithOkAndDelegate(NSString *message, id delegate)
{
	/* open an alert with an OK button */
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil 
													message:message delegate:delegate 
										  cancelButtonTitle:@"确定" 
										  otherButtonTitles: nil];
	[alert show];
	[alert release];
}

void alertWithMessageAndTitle(NSString *title, NSString *message)
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(title, nil) 
													message:NSLocalizedString(message, nil) delegate:nil 
										  cancelButtonTitle:NSLocalizedString(@"WinksAlertYes", nil) 
										  otherButtonTitles: nil];
	[alert show];
	[alert release];
}

void alertWithMessageAndDelegate(NSString *message, id delegate)
{
	/* open an alert with OK and Cancel buttons */
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"WinksAlertTitle", nil) 
													message:message delegate:delegate 
										  cancelButtonTitle:nil 
										  otherButtonTitles:NSLocalizedString(@"WinksAlertYes", nil), NSLocalizedString(@"WinksAlertNo", nil), nil];
	[alert show];
	[alert release];
}

//#pragma mark progressAlert
//static UIAlertView *progressAlert = NULL;
//void showProgressAlert(NSString *title, NSString *message, id delegate)
//{
//	if(progressAlert != nil){
//		//[progressAlert show:NO];
//		[progressAlert release];
//		progressAlert = NULL;
//	}
//	
//	progressAlert = [[UIAlertView alloc] initWithTitle:title
//															message:message
//														   delegate: delegate
//												  cancelButtonTitle: nil
//												  otherButtonTitles: nil];
//	
//	UIActivityIndicatorView *activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
//	activityView.frame = CGRectMake(139.0f-18.0f, 80.0f, 37.0f, 37.0f);
//	[progressAlert addSubview:activityView];
//	[activityView startAnimating];
//    [activityView release];
//	
//	[progressAlert show];
//	[progressAlert release];
//}
//
//void closeProgressAlert()
//{
//	[progressAlert dismissWithClickedButtonIndex:0 animated:YES];
//}
