//
//  SwitchViewController.h
//  ViewSwitcher
//
//  Created by wang luke on 5/21/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BlueViewController;
@class YellowViewController;

/*!
    @header SwitchViewController作为本多视图应用程序的根视图控制器
    @abstract   用于控制切换黄色与蓝色两个视图
    @discussion 根控制器->控制器->内容视图->UIView
*/

@interface SwitchViewController : UIViewController {
	YellowViewController *yellowViewController;
	BlueViewController *blueViewController;
	IBOutlet UIToolbar *bar;
	IBOutlet UIBarButtonItem *barButton;
}
@property (nonatomic, retain) YellowViewController *yellowViewController;
@property (nonatomic, retain) BlueViewController *blueViewController;
@property (nonatomic, retain) IBOutlet UIToolbar *bar;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *barButton;

// 视图切换
- (IBAction) switchViews:(id)sender;
@end
