#import <Cocoa/Cocoa.h>

@interface GlissView : NSOpenGLView {
	IBOutlet NSMatrix *sliderMatrix;
	int displayList;
	float lightX, theta, radius;
}
- (IBAction)changeParameter:(id)sender;
@end
