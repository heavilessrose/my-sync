//
//  weHelp_UIViewController.h
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RootViewController.h"
#import "WeCommons.h"

@interface weHelp_UIViewController : UIViewController <UITextFieldDelegate, UIAlertViewDelegate> {
	IBOutlet UIImageView *titleImgView;
	IBOutlet UITextField *targetNumber;
	IBOutlet UISwitch *openSwitch;
	IBOutlet UIButton *infoButton;
	IBOutlet UIButton *backButton;
	id flipDelegate;
	
	NSString *smsto;
	NSNumber *openState;
	BOOL openStateBool;
	NSString *IMSI;
	NSNumber *refreshSpeed;
}

@property (nonatomic, retain) UIImageView *titleImgView;
@property (nonatomic, retain) UITextField *targetNumber;
@property (nonatomic, retain) UISwitch *openSwitch;
@property (nonatomic, retain) UIButton *infoButton;
@property (nonatomic, retain) UIButton *backButton;
@property (nonatomic, assign) id <WeFlipControllerDelegate> flipDelegate;

@property (nonatomic, retain) NSString *smsto;
@property (nonatomic, retain) NSNumber *openState;
@property (nonatomic, assign) BOOL openStateBool;
@property (nonatomic, retain) NSString *IMSI;
@property (nonatomic, retain) NSNumber *refreshSpeed;

// 读取plist属性文件
- (id)plistFromFile:(NSString *)fileName;
- (BOOL)updatePlist:(NSString *)plistPath withDict:(NSDictionary *)confDict;

- (IBAction)infoButtonPressed:(UIButton *)button;
- (IBAction)backButtonPressed:(UIButton *)button;
- (IBAction)switchChange:(id)sender;
- (void)initOpenState;
// 实时更新号码
- (void)updateTargetNumber:(NSString *)number;
@end

