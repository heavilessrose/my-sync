#import "FlipsideViewController.h"

@implementation FlipsideViewController
- (void)viewDidLoad {
	NSString* urlString = @"http://www.appsamuck.com/day17.html";
	[webView loadRequest:[[NSURLRequest alloc] initWithURL:[[NSURL alloc] initWithString:urlString]]];
}
@end
