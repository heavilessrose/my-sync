//
//  AdjustKeyboardTest.h
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright 2010 WeTeam. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AdjustKeyboardTest : UIViewController <UIScrollViewDelegate, UITextFieldDelegate>
{
	BOOL shouldKeyboardAct;
	BOOL keyboardVisible;
	CGPoint offset;
	UIScrollView *scrollview;
	UITextField *textfield;
	UITextField *textfield2;
}

@property (nonatomic, retain) UIScrollView *scrollview;
@property (nonatomic, retain) UITextField *textfield;
@property (nonatomic, retain) UITextField *textfield2;

-(void) keyboardWillShow: (NSNotification *)notif;
-(void) keyboardWillHide: (NSNotification *)notif;
@end
