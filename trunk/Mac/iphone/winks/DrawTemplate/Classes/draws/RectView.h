//
//  RectView.h
//  DrawTemplate
//
//  Created by luke on 10-2-22.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "wk_iphoneccshw.h"


@interface RectView : UIView {
	Winks_CCDW_Rect_s *gRect;
}

@property (nonatomic, assign) Winks_CCDW_Rect_s *gRect;

- (id)initWithSection:(Winks_CCDW_Rect_s *)pSection;

@end
