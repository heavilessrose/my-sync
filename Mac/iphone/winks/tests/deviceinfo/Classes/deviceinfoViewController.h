//
//  deviceinfoViewController.h
//  deviceinfo
//
//  Created by wang luke on 7/24/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import <Message/NetworkController.h>
#import <UIKit/UIDevice.h>

@interface deviceinfoViewController : UIViewController {
	IBOutlet UITextView* show;
}

@property (nonatomic, retain) IBOutlet UITextView* show;
@end

