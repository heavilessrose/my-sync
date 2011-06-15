//
//  SLMovInfoCell.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLMovInfoCell.h"


@implementation SLMovInfoCell

@synthesize label, contentTextView, movie;

- (void)dealloc
{
    MLog(@"");
    [movie release];
    [label release];
    [contentTextView release];
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

- (void)setMovie:(SLMovie *)theMov
{
    if (theMov) {
        if (theMov.content && [theMov.content length] > 0) {
            self.contentTextView.text = theMov.content;
        } else {
            self.contentTextView.text = @"暂无";
        }
    }
}

@end
