#define kAccelerationThreshold		2.2
#define kUpdateInterval			(1.0f/10.0f)

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>

@interface ShakeAndBreakViewController : UIViewController <UIAccelerometerDelegate> {
	IBOutlet	UIImageView	*imageView;
	
	BOOL		brokenScreenShowing;
	SystemSoundID soundID;
	UIImage		*fixed;
	UIImage		*broken;
}
@property (nonatomic, retain) UIImageView *imageView;
@property (nonatomic, retain) UIImage *fixed;
@property (nonatomic, retain) UIImage *broken;
@end
