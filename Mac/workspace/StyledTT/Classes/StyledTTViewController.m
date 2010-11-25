//
//  StyledTTViewController.m
//  StyledTT
//
//  Created by luke on 10-11-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "StyledTTViewController.h"
#import "TTStyledTextLabel.h"

@implementation StyledTTViewController



/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
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


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	NSString* kText = @"This is a test of styled labels.  Styled labels support <b>bold text</b>, <i>italic text</i>, <span class=\"blueText\">colored text</span>, <span class=\"largeText\">font sizes</span>, <span class=\"blueBox\">spans with backgrounds</span>, inline images <img src=\"bundle://smiley.png\"/>, and <a href=\"http://www.google.com\">hyperlinks</a> you can actually touch. URLs are automatically converted into links, like this: http://www.foo.comaanan asdf <div>You can enclose blocks within an HTML div.</div> Both line break characters\n\nand HTML line breaks<br/>are respected.";
	
	TTStyledTextLabel* label1 = [[[TTStyledTextLabel alloc] initWithFrame:self.view.bounds] autorelease];
	label1.font = [UIFont systemFontOfSize:17];
	label1.text = [TTStyledText textFromXHTML:kText lineBreaks:YES URLs:YES];
	label1.contentInset = UIEdgeInsetsMake(10, 10, 10, 10);
	//label1.backgroundColor = [UIColor grayColor];
	[label1 sizeToFit];
	[self.view addSubview:label1];
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
    [super dealloc];
}

@end
