//
//  Alert.m
//  propertyList
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
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"URLCache" message:message delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
	[alert show];
	[alert release];
}


void alertWithMessageAndDelegate(NSString *message, id delegate)
{
	/* open an alert with OK and Cancel buttons */
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"URLCache" message:message delegate:delegate cancelButtonTitle:@"Cancel" otherButtonTitles: @"OK", nil];
	[alert show];
	[alert release];
}
