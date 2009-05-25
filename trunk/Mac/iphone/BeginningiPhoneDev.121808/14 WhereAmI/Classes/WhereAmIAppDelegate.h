//
//  WhereAmIAppDelegate.h
//  WhereAmI
//
//  Created by Jeff LaMarche on 8/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WhereAmIViewController;

@interface WhereAmIAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet WhereAmIViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) WhereAmIViewController *viewController;

@end

