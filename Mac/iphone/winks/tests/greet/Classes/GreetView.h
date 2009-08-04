//
//  GreetView.h
//  greet
//
//  Created by wang luke on 8/3/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface GreetView : UIView {
	NSTimer *greetingTimer;
}
@property(nonatomic, assign) NSTimer *greetingTimer;

//- (void)timerFireMethod:(NSTimer*)theTimer;
//- (void)initGreeting:(NSString *)greeting;
//- (void)drawGreeting:(CGFloat)x:(CGFloat)y;

#pragma mark -
- (CGFloat)getStrWidth:(NSString *)str;
- (void)renderGreeting:(NSString *)greeting;
- (void)disposeTimer;
- (void)timerFireMethod:(NSTimer*)theTimer;
@end
