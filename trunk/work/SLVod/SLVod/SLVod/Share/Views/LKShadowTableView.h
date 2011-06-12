//
//  LKShadowTableView.h
//  I58OS
//
//  Created by luke on 11-5-12.
//  Copyright 2011年 58.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

@class UITableView;
@interface LKShadowTableView : UITableView {
    
	// shadow
	CAGradientLayer *originShadow;
	CAGradientLayer *topShadow;
	CAGradientLayer *bottomShadow;
	
	BOOL			customHitTest;
	BOOL			showShadow;
}


@property (nonatomic, assign) BOOL			showShadow;
@property (nonatomic, assign) BOOL			customHitTest;
@end
