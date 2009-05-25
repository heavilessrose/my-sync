//
//  GLFunViewController.h
//  GLFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Constants.h"
@interface GLFunViewController : UIViewController {
	IBOutlet UISegmentedControl *colorControl;
}
@property (nonatomic, retain) UISegmentedControl *colorControl;
- (IBAction)changeColor:(id)sender;
- (IBAction)changeShape:(id)sender;
@end

