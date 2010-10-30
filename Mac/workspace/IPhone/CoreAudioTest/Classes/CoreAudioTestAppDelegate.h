//
//  CoreAudioTestAppDelegate.h
//  CoreAudioTest
//
//  Created by luke on 10-10-31.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CoreAudioTestViewController;

@interface CoreAudioTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    CoreAudioTestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet CoreAudioTestViewController *viewController;

@end

