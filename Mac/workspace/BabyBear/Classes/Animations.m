//
//  Animations.m
//  BabyBear
//
//  Created by luke on 10-9-1.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Animations.h"
#import <QuartzCore/QuartzCore.h>


@implementation Animations

+ (void)pulsatingEff:(UIView *)pview
{
	//Create an animation with pulsating effect
	CABasicAnimation *theAnimation;
	//within the animation we will adjust the "opacity" 
	//value of the layer
	theAnimation=[CABasicAnimation animationWithKeyPath:@"opacity"];
	//animation lasts 0.4 seconds
	theAnimation.duration=0.6;	
	//and it repeats forever
	//theAnimation.repeatCount= 1e100f;
	//we want a reverse animation
	theAnimation.autoreverses=NO;	
	//justify the opacity as you like (1=fully visible, 0=unvisible)
	theAnimation.fromValue=[NSNumber numberWithFloat:0.1]; 
	theAnimation.toValue=[NSNumber numberWithFloat:1.0];
	//Assign the animation to your UIImage layer and the 
	//animation will start immediately
	[pview.layer addAnimation:theAnimation 
								 forKey:@"animateOpacity"];
}

@end
