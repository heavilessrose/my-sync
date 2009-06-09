//
//  MainView.h
//  DeathCalc
//
//  Created by apps on 11/1/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainView : UIView {
    IBOutlet UIWebView *deathInfo;
    IBOutlet UITextField *ageField;
    IBOutlet UISegmentedControl *genderField;
}
- (void)deathUpdate;
- (BOOL)textFieldShouldReturn:(UITextField *)textField;
- (IBAction)ageUpdate;
- (IBAction)genderUpdate;
- (IBAction) resignResponder;
@end
