//
//  SearcherAppDelegate.h
//  Searcher
//
//  Created by luke on 10-9-20.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

//@class SearcherViewController;

@interface SearcherAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    UITabBarController *tabs;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *tabs;

@end

