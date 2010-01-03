#import "SmartScreenWidgetProtocol.h"

@interface ClockWidget : UIViewController <SmartScreenWidgetProtocol>
{
    NSMutableDictionary* _preferences;

    UIImageView* ivBackground;
    UILabel* lTime;
    UILabel* lDate;

    NSTimer* _timer;
    NSDateFormatter* _timeFormatter;
    NSDateFormatter* _dateFormatter;
}

-(void) loadPreferences;
-(void) savePreferences;

-(void) updateControls;

@end

@interface UIColor (RGBA)

+(UIColor*) colorWithRGB:(unsigned int) rgb;

@end