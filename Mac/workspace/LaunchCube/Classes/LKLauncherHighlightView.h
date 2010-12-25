//
//  LKLauncherHighlightView.h
//  LaunchCube
//
//  Created by luke on 10-12-25.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKLauncherView.h"


@interface LKLauncherHighlightView : UIView {
	
	LKLauncherView *parentView;
	CGRect highlightRect;
	CGFloat highlightScale;
	UIWindow *statusBarCover;
	UILabel *textLabel;
}

@property (nonatomic, assign) CGRect         highlightRect;
@property (nonatomic, retain) LKLauncherView *parentView;
@property (nonatomic, copy)   NSString		 *text;

- (void)appear:(BOOL)animated;

@end
