//
//  CheckPleaseViewController.h
//  CheckPlease
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//
#define kMinimumCheckMarkAngle	50
#define kMaximumCheckMarkAngle	135
#define kMinimumCheckMarkLength 10
#import <UIKit/UIKit.h>

@interface CheckPleaseViewController : UIViewController {
	IBOutlet	UILabel *label;
	CGPoint		lastPreviousPoint;
	CGPoint		lastCurrentPoint;
	CGFloat		lineLengthSoFar;
}
@property (nonatomic, retain) UILabel *label;
- (void)eraseLabel;
@end

