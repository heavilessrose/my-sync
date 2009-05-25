//
//  SwipesAppDelegate.h
//  Swipes
//
//  Created by Jeff LaMarche on 8/2/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SwipesViewController;

@interface SwipesAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet SwipesViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) SwipesViewController *viewController;

@end

