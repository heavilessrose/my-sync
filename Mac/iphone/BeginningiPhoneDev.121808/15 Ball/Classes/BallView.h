#define kVelocityMultiplier		500

#import <UIKit/UIKit.h>


@interface BallView : UIView {
	UIImage *image;
	
	CGPoint	currentPoint;
	CGPoint	previousPoint;
	
	UIAcceleration *acceleration;
	CGFloat	ballXVelocity;
	CGFloat ballYVelocity;
}
@property (nonatomic, retain) UIImage *image;
@property CGPoint currentPoint;
@property CGPoint previousPoint;
@property (nonatomic, retain) UIAcceleration *acceleration;
@property CGFloat ballXVelocity;
@property CGFloat ballYVelocity;
- (void)draw;
@end

