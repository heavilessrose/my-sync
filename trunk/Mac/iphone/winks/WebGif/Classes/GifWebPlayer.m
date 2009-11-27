//
//  GifWebPlayer.m
//  WebGif
//
//  Created by luke on 09-11-27.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GifWebPlayer.h"

@implementation GifWebPlayer
@synthesize localHtml, webview;


/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	contentView.backgroundColor = [UIColor whiteColor];
	
	contentView.autoresizesSubviews = YES;
	contentView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);	
	self.view = contentView;
	[contentView release];
	
	CGRect webFrame = [[UIScreen mainScreen] applicationFrame];
	//webFrame.origin.y -= 20;
	//webFrame.size.height -= [toolbar frame].size.height;
	printf("webframe y=%f, x=%f, width=%f, height=%f\n",webFrame.origin.y,webFrame.origin.x,webFrame.size.width,webFrame.size.height);
	printf("self.view.bounds.size.width=%f, self.view.bounds.size.height=%f\n",self.view.bounds.size.width,self.view.bounds.size.height);
	
	
	self.webview = [[UIWebView alloc] initWithFrame:webFrame];
	printf("webview.width=%f, webview.height=%f\n", webview.bounds.size.width, webview.bounds.size.height);
	
	//self.webview.backgroundColor = [UIColor whiteColor];
	self.webview.scalesPageToFit = NO;
	self.webview.opaque = NO;
	self.webview.backgroundColor = [UIColor clearColor];
	self.webview.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
	self.webview.delegate = self;
	[self.view addSubview: webview];
	
	webview.scalesPageToFit = NO;
	webview.opaque = NO;
	webview.backgroundColor = [UIColor clearColor];
	[webview loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:[[NSBundle mainBundle] 
																			pathForResource:localHtml ofType:@"html"]isDirectory:NO]]];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[localHtml release];
	[webview release];
    [super dealloc];
}


@end
