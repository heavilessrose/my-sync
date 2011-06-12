//
//  LKTipCenter.h
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class LKTip, LKTipView, LKStatusBar;

@interface LKTipCenter : NSObject {
	
	BOOL active;
	//id<LKTipDelegate> delegate;
@private
	NSMutableDictionary *tips;
	LKTipView *curTipView;
	LKTip *curTip;
	
	LKStatusBar *statusBar;
    
    NSMutableDictionary  *fallingContainers;
}

+ (LKTipCenter *)defaultCenter;

- (void)postFallingTipWithMessage:(NSString *)message inContainer:(UIView *)container time:(NSTimeInterval)dTime;
- (void)changeFallingTip:(UIView *)container withText:(NSString *)errStr;
- (void)disposeFallingTip:(UIView *)container;


@end
