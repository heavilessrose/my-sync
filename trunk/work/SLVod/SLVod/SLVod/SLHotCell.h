//
//  SLHotCell.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SLMovie.h"

@protocol SLPlayDelegate <NSObject>

- (void)play:(SLMovie *)theMov;

@end

@interface SLHotCell : UITableViewCell {
    UIImageView     *imageView;
    UILabel         *titleLabel;
    UILabel         *actorLabel;
    UIButton        *playButton;
    UILabel         *cateLabel;
    UIProgressView  *progressView;
    id<SLPlayDelegate> playDelegate;
    
    SLMovie         *movie;
}

@property (nonatomic, retain) IBOutlet UIProgressView  *progressView;
@property (nonatomic, assign) id<SLPlayDelegate> playDelegate;
@property (nonatomic, retain) SLMovie *movie;
@property (nonatomic, retain) IBOutlet UIButton        *playButton;
@property (nonatomic, retain) IBOutlet UIImageView     *imageView;
@property (nonatomic, retain) IBOutlet UILabel         *titleLabel;
@property (nonatomic, retain) IBOutlet UILabel         *actorLabel;
@property (nonatomic, retain) IBOutlet UILabel         *cateLabel;

- (IBAction)playButtonPressed:(UIButton *)pButton;

@end
