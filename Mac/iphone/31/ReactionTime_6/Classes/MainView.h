#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface MainView : UIView {
	NSDate *startDate;
	
    IBOutlet UIImageView *stopLight;
}

@property (nonatomic, copy) NSDate *startDate;

- (IBAction)gasPedalPressed;
@end
