//
//  SwipesViewController.h
//  Swipes
//
//  Created by Jeff LaMarche on 8/2/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//
#define kMinimumGestureLength	25
#define kMaximumVariance		5

typedef enum  {
	kNoSwipe = 0
	,kHorizontalSwipe 
	,kVerticalSwipe
} SwipeType;

#import <UIKit/UIKit.h>

@interface SwipesViewController : UIViewController {
	IBOutlet	UILabel	*label;
	
	CGPoint		gestureStartPoint;
}
@property (nonatomic, retain) UILabel *label;
@property CGPoint gestureStartPoint;
- (void)eraseText;
@end

