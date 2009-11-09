#import "MyView.h"
@implementation MyView
- (void)TestThread
{
	int test = 0;
	while (true) 
	{
		[NSThread sleepForTimeInterval:10.0];
		CallLogInt(test++);
	}
}
@end