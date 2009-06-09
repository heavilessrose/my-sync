#import "MainView.h"

@implementation MainView

int count = 0;

-(void)awakeFromNib {
	
	counter.text = @"0";

}

- (IBAction)addUnit {
	
	if(count >= 999) return;
	
	NSString *numValue = [[NSString alloc] initWithFormat:@"%d", count++];
	counter.text = numValue;
	[numValue release];
}

- (IBAction)subtractUnit {
	
	if(count <= 0) return;

	NSString *numValue = [[NSString alloc] initWithFormat:@"%d", count--];
	counter.text = numValue;
	[numValue release];	
}
@end
