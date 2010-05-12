//
//  AdreessBookToolsAppDelegate.h
//  AdreessBookTools
//
//  Created by luke on 5/12/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

@interface AdreessBookToolsAppDelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    UINavigationController *navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end

