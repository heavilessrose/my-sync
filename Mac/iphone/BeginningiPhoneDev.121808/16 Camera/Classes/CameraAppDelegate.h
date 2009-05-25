//
//  CameraAppDelegate.h
//  Camera
//
//  Created by Jeff LaMarche on 8/8/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CameraViewController;

@interface CameraAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet CameraViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) CameraViewController *viewController;

@end

