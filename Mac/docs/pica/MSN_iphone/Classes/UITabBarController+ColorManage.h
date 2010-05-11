//
//  UITabBarController+ColorManage.h
//  MSN
//
//  Created by Jun Du on 6/16/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface UITabBarController_ColorManage : UITabBarController {
	UISegmentedControl *segmentedControl;
	UIImageView* chatImageView;
    
    UIToolbar*      myToolBar;
}

@property (nonatomic, retain) UISegmentedControl *segmentedControl;
@property (nonatomic, retain) UIImageView *chatImageView;

@property (nonatomic, retain) UIToolbar* myToolBar;
@end
