//
//  MainView.h
//  DrawTest
//
//  Created by AppsAmuck on 8/26/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface MainView : UIView <UIAccelerometerDelegate> {
	UIImageView* helpView;
	int billCount;
	NSMutableArray* billList;
	CABasicAnimation* flipAnimation;
	CGRect emitterRect;
	BOOL useEmitterRect;
	CGPoint deviceTilt;
	int billIndex;
}

+(UIImage *)rotateImage:(UIImage *)image;

@end
