//
//  contactSampleAppDelegate.h
//  contactSample
//
//  Created by wang luke on 5/21/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class contactSampleViewController;

@interface contactSampleAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    contactSampleViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet contactSampleViewController *viewController;

@end

