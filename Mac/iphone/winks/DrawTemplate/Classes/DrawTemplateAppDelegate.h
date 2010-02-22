//
//  DrawTemplateAppDelegate.h
//  DrawTemplate
//
//  Created by luke on 10-1-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DrawTemplateViewController;

@interface DrawTemplateAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    DrawTemplateViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet DrawTemplateViewController *viewController;

@end

