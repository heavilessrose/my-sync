//
//  LKLauncherView.h
//  LaunchCube
//
//  Created by luke on 10-12-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LKLauncherScrollView;

@interface LKLauncherView : UIView <UIScrollViewDelegate> {

	LKLauncherScrollView *scrollView;
}

@property (nonatomic, retain) LKLauncherScrollView *scrollView;
@end
