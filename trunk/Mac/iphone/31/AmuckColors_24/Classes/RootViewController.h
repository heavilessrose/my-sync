//
//  RootViewController.h
//  AmuckColors
//
//  Created by AppsAmuck on 10/25/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;
@class FlipsideViewController;

@interface RootViewController : UIViewController {
	MainViewController *mainViewController;
	FlipsideViewController *flipsideViewController;
	UINavigationBar *flipsideNavigationBar;
}

@property (nonatomic, retain) MainViewController *mainViewController;
@property (nonatomic, retain) UINavigationBar *flipsideNavigationBar;
@property (nonatomic, retain) FlipsideViewController *flipsideViewController;

- (IBAction)toggleView;

+ (RootViewController *)getInstance;

@end
