//
//  InfoViewController.h
//  weHelp_UI
//
//  Created by luke on 09-12-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RootViewController.h"

@interface InfoViewController : UIViewController {
	IBOutlet UIToolbar *toolbar;
	IBOutlet UIBarButtonItem *backItem;
	id flipDelegate;
	
	//IBOutlet UIImageView *background;
}

@property (nonatomic, retain) UIToolbar *toolbar;
@property (nonatomic, retain) UIBarButtonItem *backItem;
@property (nonatomic, assign) id <WeFlipControllerDelegate> flipDelegate;
//@property (nonatomic, retain) UIImageView *background;

- (IBAction)back;
@end
