//
//  ContactMapperAppDelegate.h
//  ContactMapper
//
//  Created by Bill Dudney on 5/15/09.
//  Copyright Gala Factory Software LLC 2009. All rights reserved.
//
//
//  Licensed with the Apache 2.0 License
//  http://apache.org/licenses/LICENSE-2.0
//

#import <UIKit/UIKit.h>

@class ContactMapperViewController;

@interface ContactMapperAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    ContactMapperViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet ContactMapperViewController *viewController;

@end

