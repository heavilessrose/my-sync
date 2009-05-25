//
//  FlipsideViewController.h
//  AppSettings
//
//  Created by Jeff LaMarche on 7/24/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FlipsideViewController : UIViewController {
	IBOutlet	UISwitch *engineSwitch;
	IBOutlet	UISlider *warpFactorSlider;
	
}
@property (nonatomic, retain) UISwitch *engineSwitch;
@property (nonatomic, retain) UISlider *warpFactorSlider;
@end

