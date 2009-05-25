//
//  PinchMeViewController.h
//  PinchMe
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#define kMinimumPinchDelta 100
#import <UIKit/UIKit.h>

@interface PinchMeViewController : UIViewController {
	IBOutlet	UILabel *label;
	CGFloat		initialDistance;
}
@property (nonatomic, retain) UILabel *label;
@property CGFloat initialDistance;
- (void)eraseLabel;
@end

