//
//  NetStatusAppDelegate.h
//  NetStatus
//
//  Created by luke on 09-11-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NetStatusAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	UITextView *textView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) UITextView *textView;

@end

