//
//  SQLToolsAppDelegate.h
//  SQLTools
//
//  Created by luke on 5/17/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

@interface SQLToolsAppDelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    UINavigationController *navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end

