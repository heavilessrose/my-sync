//
//  LaunchCubeViewController.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LKLauncherView.h"

@interface LaunchCubeViewController : UIViewController {

	LKLauncherView *launcherView;
}

@property (nonatomic, retain) LKLauncherView *launcherView;
@end
