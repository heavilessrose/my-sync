/*
     File: MyViewController.m
 Abstract: The main view controller of this app
  Version: 1.2
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
 */

#import "MyViewController.h"
#import "AppDelegate.h"

NSString *kCellIdentifier = @"MyIdentifier";

#define kTransitionDuration	0.75

@implementation MyViewController

@synthesize containerView, myTableView, instructionsView, firstNameStr, lastNameStr, flipButton, doneButton;

- (void)dealloc
{
	[containerView release];
	[myTableView release];
	[instructionsView release];
	[firstNameStr release];
	[lastNameStr release];
	[flipButton release];
	[doneButton release];
	
	// unregister for this notification
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidFinishLaunchingNotification object:nil]; 

	[super dealloc];
}

- (void)awakeFromNib
{
	[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
	
	// add the top-most parent view
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	contentView.backgroundColor = [UIColor blackColor];
	self.view = contentView;
	[contentView release];
	
	// create the container view which we will use for flip animation (centered horizontally)
	containerView = [[UIView alloc] initWithFrame:self.view.bounds];
	[self.view addSubview:containerView];

	// watch when the app has finished launching so we can update our preference settings and apply them to the UI
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateSettings:) 
										name:UIApplicationDidFinishLaunchingNotification object:nil];
	
	// create and configure the table view
	myTableView = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStyleGrouped];	
	myTableView.delegate = self;
	myTableView.dataSource = self;
	myTableView.scrollEnabled = NO;
	
	[containerView addSubview: myTableView];
	
	// make sure the table cell data is populated up front so the cell is allocated when 'updateSettings' is called
	[myTableView reloadData];
	
	// add our custom flip button as the nav bar's custom right view
	UIButton* infoButton = [UIButton buttonWithType:UIButtonTypeInfoLight];
	[infoButton addTarget:self action:@selector(flipAction:) forControlEvents:UIControlEventTouchUpInside];
	flipButton = [[UIBarButtonItem alloc] initWithCustomView:infoButton];
	self.navigationItem.rightBarButtonItem = flipButton;
	
	// create our done button as the nav bar's custom right view for the flipped view (used later)
	doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
						target:self action:@selector(flipAction:)];
}

// this is called when the app finishes launching (i.e. UIApplicationDidFinishLaunchingNotification)
//
- (void)updateSettings:(NSNotification *)notif
{
	AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
	
	// ask our app delegate for the preferred string values
	firstNameStr = appDelegate.firstName;
	lastNameStr = appDelegate.lastName;
	
	// now change the cell's text and color
	UITableViewCell *cell = [myTableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:0]];
	switch ([appDelegate textColor])
	{
		case blue:
			cell.textColor = [UIColor blueColor];
			break;
		
		case green:
			cell.textColor = [UIColor greenColor];
			break;
			
		case red:
			cell.textColor = [UIColor redColor];
			break;
	}
	
	// now change the app view's background color
	switch ([appDelegate backgroundColor])
	{
		case blackBackColor:
			myTableView.backgroundColor = [UIColor blackColor];
			break;
			
		case whiteBackColor:
			myTableView.backgroundColor = [UIColor whiteColor];
			break;
		
		case blueBackColor:
			myTableView.backgroundColor = [UIColor blueColor];
			break;
			
		case patternBackColor:
			myTableView.backgroundColor = [UIColor groupTableViewBackgroundColor];
			break;
	}
						 
	[myTableView reloadData];	// this should update the cell's text
}

- (void)didReceiveMemoryWarning
{
	// Invoke super's implementation to do the Right Thing, but also release the input controller since we can do that	
	// In practice this is unlikely to be used in this application, and it would be of little benefit,
	// but the principle is the important thing.
	//
	[super didReceiveMemoryWarning];
}

- (void)flipAction:(id)sender
{
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(animationDidStop:animationIDfinished:finished:context:)];
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:kTransitionDuration];
	
	[UIView setAnimationTransition:([myTableView superview] ?
									UIViewAnimationTransitionFlipFromLeft : UIViewAnimationTransitionFlipFromRight)
							forView:containerView cache:YES];
	if ([instructionsView superview])
	{
		[instructionsView removeFromSuperview];
		[containerView addSubview:myTableView];
	}
	else
	{
		[myTableView removeFromSuperview];
		[containerView addSubview:instructionsView];
	}
	
	[UIView commitAnimations];
	
	// adjust our done/info buttons accordingly
	if ([instructionsView superview])
		self.navigationItem.rightBarButtonItem = doneButton;
	else
		self.navigationItem.rightBarButtonItem = flipButton;
}

#pragma mark - UITableView delegates

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellEditingStyleNone;  // no editing style in our case
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return 1;
}

// to determine which UITableViewCell to be used on a given row.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
	if (cell == nil)
	{
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:kCellIdentifier] autorelease];
	}
	
	cell.selectionStyle = UITableViewCellSelectionStyleNone;
	cell.text = [NSString stringWithFormat:@"%@ %@", firstNameStr, lastNameStr];
	
	return cell;
}

@end

