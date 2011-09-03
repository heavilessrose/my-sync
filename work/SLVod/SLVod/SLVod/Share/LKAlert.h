//
//  LKAlert.h
//  SK
//
//  Created by luke on 10-11-15.
//  Copyright 2010 58.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKAlert;

@interface LKAlert : NSObject {
	NSString *title;
	NSString *message;
	NSString *yesStr;
	NSString *noStr;
	
	id<UIAlertViewDelegate> delegate;
}


@property (nonatomic, assign) id<UIAlertViewDelegate> delegate;
@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *message;
@property (nonatomic, retain) NSString *yesStr;
@property (nonatomic, retain) NSString *noStr;

@end


void alertWithMsgAndTitleWithDelegare(NSString *title, NSString *message, id delegate);
void alertWithError(NSError *error, NSString *additionInfo);
void alertWithMessage(NSString *message, NSString *yesStr, NSString *noStr);
void alertWithMessageAndTitle(NSString *title, NSString *message);
void alertWithOkAndDelegate(NSString *message, id delegate);
void alertWithMessageAndDelegate(NSString *message, id delegate);

void alertWithTwoButton(LKAlert *alert);
void alertWithRetry(NSString *message, id delegate, NSString *yes, NSString *no);
