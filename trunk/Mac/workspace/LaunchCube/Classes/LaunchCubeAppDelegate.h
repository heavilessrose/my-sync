//
//  LaunchCubeAppDelegate.h
//  LaunchCube
//
//  Created by luke on 10-12-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LaunchCubeViewController.h"

@interface LaunchCubeAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	LaunchCubeViewController *launcher;
}

@property (nonatomic, retain) LaunchCubeViewController *launcher;
@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

