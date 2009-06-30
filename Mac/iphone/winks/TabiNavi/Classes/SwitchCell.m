//
//  SwitchCell.m
//  TabiNavi
//
//  Created by wang luke on 6/30/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "SwitchCell.h"


@implementation SwitchCell
@synthesize turnOnSwitch;

- (id)initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithFrame:frame reuseIdentifier:reuseIdentifier]) {
        // Initialization code
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)dealloc {
    [super dealloc];
}


@end
