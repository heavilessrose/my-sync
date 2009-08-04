//
//  greetAppDelegate.h
//  greet
//
//  Created by wang luke on 8/3/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GreetView.h"

@interface greetAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	GreetView *greetView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) GreetView *greetView;
@end

