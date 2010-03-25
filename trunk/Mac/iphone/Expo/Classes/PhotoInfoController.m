    //
//  PhotoInfoController.m
//  Expo
//
//  Created by luke on 10-3-25.
//  Copyright 2010 Luke. All rights reserved.
//

#import "PhotoInfoController.h"


@implementation PhotoInfoController
@synthesize summary = _summary;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

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
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	TT_RELEASE_SAFELY(_summary);
    [super dealloc];
}

#pragma mark luke

- (id)init {
	if (self = [super init]) {
		_contentType = PhotoTypeNone;
		_index = -1;
		_summary = nil;
	}
	return self;
}

- (id)initWithPhotoInfo:(NSInteger)photoIndex {
	if (self = [super init]) {
		// 初始化
		_index = photoIndex;
		self.title = [NSString stringWithFormat:@"%d", _index];
//		summary = ;
	}
	return self;
}
@end
