//
//  SLHotCell.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLHotCell.h"


@implementation SLHotCell

@synthesize imageView, playButton, playDelegate;
@synthesize titleLabel;
@synthesize actorLabel;
@synthesize movie;


- (void)dealloc
{
    MLog(@"");
    [movie release];
    [playButton release];
    [imageView release];
    [titleLabel release];
    [actorLabel release];
    [super dealloc];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

#pragma mark -

- (void)setMovie:(SLMovie *)theMov
{
    movie = [theMov retain];
    self.titleLabel.text = theMov.title;
    self.actorLabel.text = theMov.actor;
    
    if (theMov.imgRecord && theMov.imgRecord.img) {
        self.imageView.image = theMov.imgRecord.img;
    }
}

- (IBAction)playButtonPressed:(UIButton *)pButton
{
    if (playDelegate && [playDelegate respondsToSelector:@selector(play:)]) {
        [playDelegate play:movie];
    }
}

@end
