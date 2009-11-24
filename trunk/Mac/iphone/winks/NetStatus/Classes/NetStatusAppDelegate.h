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
	
	//////
	NSObject *testObj;
	NSObject *testObj1;
	NSObject *testObj2;
	NSObject *testObj3;
	NSObject *testObj4;
	
	NSObject *tmpObj;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) UITextView *textView;
////
@property (nonatomic, retain) NSObject *testObj;
@property (nonatomic, retain) NSObject *testObj1;
@property (nonatomic, assign) NSObject *testObj2;
@property (nonatomic, assign) NSObject *testObj3;
@property (nonatomic, retain) NSObject *testObj4;
@property (nonatomic, retain) NSObject *tmpObj;

- (void)retainTest;
@end

