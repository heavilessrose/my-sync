//
//  SLHotCell.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLHotCell.h"


@implementation SLHotCell

@synthesize imageView;
@synthesize titleLabel;
@synthesize actorLabel;
@synthesize movie;


- (void)dealloc
{
    MLog(@"");
    [movie release];
    [imageView release];
    [titleLabel release];
    [actorLabel release];
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

#pragma mark -

- (void)setMovie:(SLMovie *)theMov
{
    movie = [theMov retain];
    self.titleLabel.text = theMov.title;
    self.actorLabel.text = theMov.actor;
}

@end
