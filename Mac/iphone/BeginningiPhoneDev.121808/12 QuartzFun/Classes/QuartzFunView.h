//
//  QuartzFunView.h
//  QuartzFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Constants.h"

@interface QuartzFunView : UIView {
	CGPoint		firstTouch;
	CGPoint		lastTouch;
	UIColor		*currentColor;
	ShapeType	shapeType;
	UIImage		*drawImage;
	CGRect		redrawRect;
	BOOL		useRandomColor;
}
@property CGPoint firstTouch;
@property CGPoint lastTouch;
@property (nonatomic, retain) UIColor *currentColor;
@property ShapeType shapeType;
@property (nonatomic, retain) UIImage *drawImage;
@property (readonly) CGRect currentRect;
@property CGRect redrawRect;
@property BOOL useRandomColor;
@end
