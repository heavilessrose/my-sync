//
//  ContactInfoViewController.h
//  MSN
//
//  Created by  apple on 08-10-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PersonInfo;

@interface ContactInfoViewController: UIViewController <UITextFieldDelegate> 
{
	PersonInfo *personInfo;
	UIImageView *imageView;
	UILabel *statusLabel;
	UIImageView *statusView;
	UIImageView *spaceView;
	UIScrollView *nickNameView;
	UITextField *nickNameField;
	UIScrollView *impresaView;
	UIActivityIndicatorView *spinner;
	
	UIScrollView*	scrollViewNickName;
	UIScrollView*	scrollViewEmailAddress;
}
@property (nonatomic, retain) PersonInfo *personInfo;
@property (nonatomic, retain) UITextField *nickNameField;
@property (nonatomic, retain) UIActivityIndicatorView *spinner;

- (void)updateNickName: (NSString*)aNickName;
- (void)notifynetworkDisabled: (NSNotification*)note;

@end
