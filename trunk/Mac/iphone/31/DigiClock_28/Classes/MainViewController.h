//
//  MainViewController.h
//  DigiClock
//
//  Created by amuck on 10/29/08.
//  Copyright Amuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainViewController : UIViewController {
	NSTimer *timer;
	UILabel *labels[7];
}
- (void)onTimer;
@end
