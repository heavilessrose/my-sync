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
	NSInteger columnCount;
	NSArray *pages;
	NSMutableArray *buttons;
}

@property (nonatomic, retain) LKLauncherScrollView *scrollView;
@property (nonatomic, assign) NSInteger columnCount;
@property (nonatomic, retain) NSArray *pages;
@property (nonatomic, retain) NSMutableArray *buttons;

@end
