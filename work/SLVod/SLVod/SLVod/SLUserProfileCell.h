//
//  SLUserProfileCell.h
//  SLVod
//
//  Created by luke on 11-6-17.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol SLUserProfileCellDelegare <NSObject>

- (void)signinButtonPressed:(UIButton *)aButton;
- (void)signupButtonPressed:(UIButton *)aButton;

@end

@interface SLUserProfileCell : UITableViewCell {
    UILabel *nameLabel;
    UIImageView *avatarView;
    UIButton *signupButton;
    UIButton *signinButton;
    
    id <SLUserProfileCellDelegare> signDelegate;
}

@property (nonatomic, assign) id <SLUserProfileCellDelegare> signDelegate;
@property (nonatomic, retain) IBOutlet UILabel *nameLabel;
@property (nonatomic, retain) IBOutlet UIImageView *avatarView;
@property (nonatomic, retain) IBOutlet UIButton *signupButton;
@property (nonatomic, retain) IBOutlet UIButton *signinButton;

- (IBAction)signinPressed:(UIButton *)aButton;
- (IBAction)signupPressed:(UIButton *)aButton;
@end
