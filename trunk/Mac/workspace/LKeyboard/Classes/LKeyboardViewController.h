//
//  LKeyboardViewController.h
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LKeyboardView;
@interface LKeyboardViewController : UIViewController {
	LKeyboardView *emoKeyBoardView;
	IBOutlet UITextField *txtField;
	IBOutlet UIButton *showOrHideButton;
}

@property (nonatomic, retain) LKeyboardView *emoKeyBoardView;

- (IBAction)showOrHideEmoKeyboard;
@end

