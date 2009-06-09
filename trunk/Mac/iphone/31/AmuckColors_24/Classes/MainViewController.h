//
//  MainViewController.h
//  AmuckColors
//
//  Created by AppsAmuck on 10/25/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainViewController : UIViewController {
	IBOutlet UIToolbar *toolBar;
}
@property (nonatomic, retain) UIToolbar *toolBar;
- (IBAction)toggleView;
- (IBAction)visiBone;
@end
