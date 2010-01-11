//
//  AdjustKeyboardTest.m
//  Widgets
//
//  Created by luke on 10-1-11.
//  Copyright 2010 WeTeam. All rights reserved.
//

#import "AdjustKeyboardTest.h"


@implementation AdjustKeyboardTest
@synthesize scrollview, textfield, textfield2;


#define SCROLLVIEW_HEIGHT 460
#define SCROLLVIEW_WIDTH  320

#define SCROLLVIEW_CONTENT_HEIGHT 720
#define SCROLLVIEW_CONTENT_WIDTH  320

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
	if (self = [super init]) 
	{
		self.view = [[[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
		self.view.backgroundColor = [UIColor whiteColor];
		
		// Create scrollview
		scrollview = [[UIScrollView alloc] init];
		scrollview.contentSize = CGSizeMake(SCROLLVIEW_CONTENT_WIDTH, SCROLLVIEW_CONTENT_HEIGHT);
		scrollview.frame = CGRectMake(0, 0, SCROLLVIEW_WIDTH, SCROLLVIEW_HEIGHT);
		scrollview.scrollsToTop = NO;
		scrollview.delegate = self;
		[self.view addSubview:scrollview];
		
		// Create textfield 1
		textfield = [[UITextField alloc] initWithFrame:CGRectMake(20, 240, 280, 30)];
		textfield.placeholder = @"Textfield 1";
		textfield.delegate = self;
		textfield.returnKeyType = UIReturnKeyDone;
		textfield.borderStyle = UITextBorderStyleBezel;
		textfield.enablesReturnKeyAutomatically = TRUE;
		[scrollview addSubview:textfield];
		
		// Create textfield 2
		textfield2 = [[UITextField alloc] initWithFrame:CGRectMake(20, 290, 280, 30)];
		textfield2.placeholder = @"Textfield 2";
		textfield2.delegate = self;
		textfield2.returnKeyType = UIReturnKeyDone;
		textfield2.borderStyle = UITextBorderStyleBezel;
		textfield2.enablesReturnKeyAutomatically = TRUE;
		[scrollview addSubview:textfield2];
	}
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector (keyboardWillShow:)
												 name: UIKeyboardWillShowNotification object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector (keyboardWillHide:)
												 name: UIKeyboardWillHideNotification object:nil];
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
	[scrollview release];
	[textfield release];
	[textfield2 release];
	
    [super dealloc];
}

#pragma mark -
#pragma mark keyboard notify handle

-(void) keyboardWillShow: (NSNotification *)notif
{
	NSLog(@"keyboardWillShow");
	// If keyboard is visible, return
	if (keyboardVisible) 
	{
		NSLog(@"Keyboard is already visible. Ignoring notification.");
		return;
	}
	
	// Get the size of the keyboard.
	NSDictionary* info = [notif userInfo];
	NSValue* aValue = [info objectForKey:UIKeyboardBoundsUserInfoKey];
	CGSize keyboardSize = [aValue CGRectValue].size;
	
	// Save the current location so we can restore
	// when keyboard is dismissed
	offset = scrollview.contentOffset;
	
	// Resize the scroll view to make room for the keyboard
	CGRect viewFrame = scrollview.frame;
	viewFrame.size.height -= keyboardSize.height;
	scrollview.frame = viewFrame;
	
	// Keyboard is now visible
	keyboardVisible = YES;
	shouldKeyboardAct = NO;
}

-(void) keyboardWillHide: (NSNotification *)notif
{
	NSLog(@"keyboardWillHide");
	// Is the keyboard already shown
	if (!keyboardVisible)
	{
		NSLog(@"Keyboard is already hidden. Ignoring notification.");
		return;
	}
	
	if (shouldKeyboardAct) {
		// Reset the height of the scroll view to its original value
		scrollview.frame = CGRectMake(0, 0, SCROLLVIEW_WIDTH, SCROLLVIEW_HEIGHT);
		
		// Reset the scrollview to previous location
		scrollview.contentOffset = offset;
		
		// Keyboard is no longer visible
		keyboardVisible = NO;
	}
}

#pragma mark -
#pragma mark textfield
// this helps dismiss the keyboard when the "Done" button is clicked
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	NSLog(@"textFieldShouldReturn");
	[textfield resignFirstResponder];
	[textfield2 resignFirstResponder];
	shouldKeyboardAct = YES;
	return YES;
}
@end
