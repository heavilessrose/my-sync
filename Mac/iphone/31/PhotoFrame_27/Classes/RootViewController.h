//
//  RootViewController.h
//  PhotoFrame
//
//  Created by amuck on 10/28/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;
@class FlipsideViewController;

@interface RootViewController : UIViewController {

	IBOutlet UIButton *infoButton;
	MainViewController *mainViewController;
	FlipsideViewController *flipsideViewController;
}

@property (nonatomic, retain) UIButton *infoButton;
@property (nonatomic, retain) MainViewController *mainViewController;
@property (nonatomic, retain) FlipsideViewController *flipsideViewController;

+ (RootViewController *)getInstance;

- (IBAction)toggleView;

@end
