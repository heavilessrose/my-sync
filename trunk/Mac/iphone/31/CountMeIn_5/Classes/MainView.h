#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface MainView : UIView {
    IBOutlet UILabel *counter;
}
- (IBAction)addUnit;
- (IBAction)subtractUnit;
@end
