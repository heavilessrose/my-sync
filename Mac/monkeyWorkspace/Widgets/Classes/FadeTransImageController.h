//
//  FadeTransImageController.h
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright 2010 WeTeam. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface FadeTransImageController : UIViewController
{
	UIImageView *layer1;
	UIImageView *layer2;
	NSTimer *transTimer;
	
	UIView *psuperView;
}

@property (nonatomic, retain) UIImageView *layer1;
@property (nonatomic, retain) UIImageView *layer2;
@property (nonatomic, assign) NSTimer *transTimer;
@property (nonatomic, retain) UIView *psuperView;

- (void)initTrans;
- (void)startTransAnimation;
- (void)stopTransAnimation;
- (void)transTimerEvent;

@end
