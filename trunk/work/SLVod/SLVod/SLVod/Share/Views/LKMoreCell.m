//
//  LKMoreCell.m
//  SLVod
//
//  Created by luke on 11-7-24.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKMoreCell.h"

@implementation LKMoreCell

@synthesize anim, titleLabel, retryButton, delegate, state;
@synthesize moreInTable;

- (void)dealloc
{
    self.retryButton = nil;
    self.anim = nil;
    self.titleLabel = nil;
    
    [super dealloc];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.state = LKMoreCellState_Loaded;
    }
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)setState:(LKMoreCellState)aState
{
    if (aState == LKMoreCellState_Loading) {
        [self.anim setHidden:NO];
        [self.titleLabel setText:@"正在加载下一页..."];
        [self.retryButton setHidden:YES];
    }
    
    if (aState == LKMoreCellState_Loaded) {
        [self.anim setHidden:YES];
        [self.titleLabel setText:[NSString stringWithFormat:@"已加载"]];
        [self.retryButton setHidden:YES];
    }
    
    if (aState == LKMoreCellState_Failed) {
        [self.anim setHidden:YES];
        [self.titleLabel setText:@"加载失败请重试"];
        [self.retryButton setHidden:NO];
    }
    
    if (aState == LKMoreCellState_NoMore) {
        [self.anim setHidden:YES];
        [self.titleLabel setText:@"已到最后一页"];
        [self.retryButton setHidden:YES];
    }
    
    if (aState == LKMoreCellState_NoConn) {
        [self.anim setHidden:YES];
        [self.titleLabel setText:@"请检查网络连接"];
        [self.retryButton setHidden:NO];
    }
    state = aState;
}

- (void)startLoadMore
{
    self.state = LKMoreCellState_Loading;
}

- (void)loadSuccessed
{
    if (self.state == LKMoreCellState_Loading) {
        self.state = LKMoreCellState_Loaded;
    }
}

- (void)nomore
{
    if (self.state == LKMoreCellState_Loading) {
        self.state = LKMoreCellState_NoMore;
    }
}

- (void)loadFailed
{
    if (self.state == LKMoreCellState_Loading) {
        self.state = LKMoreCellState_Failed;
    }
}

- (IBAction)willRetry:(id)sender
{
    if (sender == retryButton) {
        if (delegate && [delegate respondsToSelector:@selector(retryLoad:)]) {
            self.state = LKMoreCellState_Loading;
            [delegate retryLoad:self];
        }
    }
}

@end
