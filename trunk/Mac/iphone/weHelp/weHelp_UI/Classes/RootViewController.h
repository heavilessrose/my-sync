//
//  RootViewController.h
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

// This protocol is used to tell the root view to flip between views
@protocol WeFlipControllerDelegate <NSObject>
@required
-(void)toggleView:(id)sender;
@end


@class weHelp_UIViewController;
@class InfoViewController;
@interface RootViewController : UIViewController <WeFlipControllerDelegate> {

	weHelp_UIViewController *mainViewControl;
	InfoViewController *infoViewControl;
}

@property (nonatomic, retain) weHelp_UIViewController *mainViewControl;
@property (nonatomic, retain) InfoViewController *infoViewControl;

- (void)loadInfoViewController;
- (void)loadMainViewController;
@end
