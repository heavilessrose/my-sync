//
//  numPadViewController.h
//  numPad
//
//  Created by wang luke on 8/5/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface numPadViewController : UIViewController <UITextFieldDelegate>{
	UIButton *doneButton;
	BOOL numberPadShowing;
	IBOutlet UITextField *selectedTextField;
}

@property (nonatomic, assign) BOOL numberPadShowing;
@property (nonatomic, retain) UIButton *doneButton;
@property (nonatomic, retain) UITextField *selectedTextField;
@end

