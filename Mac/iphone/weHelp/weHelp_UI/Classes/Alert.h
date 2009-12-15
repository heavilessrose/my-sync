//
//  Alert.h
//  ColorSet
//
//  Created by wang luke on 5/7/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

void alertWithError(NSError *error);
void alertWithMessage(NSString *message);
void alertWithMessageAndTitle(NSString *title, NSString *message);
void alertWithOkAndDelegate(NSString *message, id delegate);
void alertWithMessageAndDelegate(NSString *message, id delegate);

