//
//  SLUserProfileCell.m
//  SLVod
//
//  Created by luke on 11-6-17.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLUserProfileCell.h"


@implementation SLUserProfileCell

@synthesize nameLabel, avatarView, signupButton, signinButton;
@synthesize signDelegate;

- (void)dealloc
{
    MLog(@"");
    [nameLabel release];
    [avatarView release];
    [signupButton release];
    [signinButton release];
    [super dealloc];
}

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

#pragma mark - button 

- (IBAction)signinPressed:(UIButton *)aButton
{
    if (signDelegate && [signDelegate respondsToSelector:@selector(signinButtonPressed:)]) {
        [signDelegate signinButtonPressed:aButton];
    }
}

- (IBAction)signupPressed:(UIButton *)aButton
{
    if (signDelegate && [signDelegate respondsToSelector:@selector(signupButtonPressed:)]) {
        [signDelegate signupButtonPressed:aButton];
    }
}

@end
