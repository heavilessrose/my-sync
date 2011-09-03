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


+ (CGFloat)contentHeight:(NSString *)info
{
    CGSize csize = [info sizeWithFont:[UIFont systemFontOfSize:17.0f] constrainedToSize:CGSizeMake(280.0f, 5000.0f) lineBreakMode:UILineBreakModeWordWrap];
    return csize.height;
}

- (void)fixFrame
{
    CGRect cellOldFrame = self.frame;
    CGRect cframe = self.contentTextView.frame;
    cframe.size.height = [SLMovInfoCell contentHeight:movie.content]+50;
    [self.contentTextView setFrame:cframe];
    
    cellOldFrame.size.height += cframe.size.height;
    self.frame = cellOldFrame;
}

- (void)setMovie:(SLMovie *)theMov
{
    if (theMov) {
        [movie release];
        movie = nil;
        movie = [theMov retain];
        
        if (theMov.content && [theMov.content length] > 0) {
            self.contentTextView.text = theMov.content;
            [self fixFrame];
        } else {
            self.contentTextView.text = @"暂无";
        }
    }
}


@end
