//
// File:	   MainViewController.m
//
// Abstract:   The application's main view controller (front page).
//
// Version:    1.7
//
// Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc. ("Apple")
//             in consideration of your agreement to the following terms, and your use,
//             installation, modification or redistribution of this Apple software
//             constitutes acceptance of these terms.  If you do not agree with these
//             terms, please do not use, install, modify or redistribute this Apple
//             software.
//
//             In consideration of your agreement to abide by the following terms, and
//             subject to these terms, Apple grants you a personal, non - exclusive
//             license, under Apple's copyrights in this original Apple software ( the
//             "Apple Software" ), to use, reproduce, modify and redistribute the Apple
//             Software, with or without modifications, in source and / or binary forms;
//             provided that if you redistribute the Apple Software in its entirety and
//             without modifications, you must retain this notice and the following text
//             and disclaimers in all such redistributions of the Apple Software. Neither
//             the name, trademarks, service marks or logos of Apple Inc. may be used to
//             endorse or promote products derived from the Apple Software without specific
//             prior written permission from Apple.  Except as expressly stated in this
//             notice, no other rights or licenses, express or implied, are granted by
//             Apple herein, including but not limited to any patent rights that may be
//             infringed by your derivative works or by other works in which the Apple
//             Software may be incorporated.
//
//             The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
//             WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
//             WARRANTIES OF NON - INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
//             PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION
//             ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
//
//             IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
//             CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//             SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//             INTERRUPTION ) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
//             AND / OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER
//             UNDER THEORY OF CONTRACT, TORT ( INCLUDING NEGLIGENCE ), STRICT LIABILITY OR
//             OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (C) 2008 Apple Inc. All Rights Reserved.
//

#import "MainViewController.h"

#import "PageOneViewController.h"
#import "PageTwoViewController.h"
#import "PageThreeViewController.h"
#import "PageFourViewController.h"
#import "PageFiveViewController.h"
#import "PageSixViewController.h"

#import "MyCustomCell.h"
#import "Constants.h"	// contains the dictionary keys

enum PageIndices
{
	kPageOneIndex	= 0,
	kPageTwoIndex	= 1,
	kPageThreeIndex = 2,
	kPageFourIndex	= 3,
	kPageFiveIndex	= 4
};

@implementation MainViewController

@synthesize menuList, myTableView;

- (void)awakeFromNib
{
	// make the title of this page the same as the title of this app
	self.title = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
		
	self.menuList = [[NSMutableArray alloc] init];
	
	// We will lazily create our view controllers as the user requests them (at a later time),
	// but for now we will encase each title an explanation text into a NSDictionary and add it to a mutable array.
	// This dictionary will be used by our table view data source to populate the text in each cell.
	//
	// When it comes time to create the corresponding view controller we will replace each NSDictionary.
	//
	// If you want to add more pages, simply call "addObject" on "menuList"
	// with an additional NSDictionary.  Note we use NSLocalizedString to load a localized version of its title.
	//
	//
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"PageOneTitle", @""), kTitleKey,
						 NSLocalizedString(@"PageOneExplain", @""), kExplainKey,
						 nil]];
	
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"PageTwoTitle", @""), kTitleKey,
						 NSLocalizedString(@"PageTwoExplain", @""), kExplainKey,
						 nil]];
	
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"PageThreeTitle", @""), kTitleKey,
						 NSLocalizedString(@"PageThreeExplain", @""), kExplainKey,
						 nil]];
	
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"PageFourTitle", @""), kTitleKey,
						 NSLocalizedString(@"PageFourExplain", @""), kExplainKey,
						 nil]];
	
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"PageFiveTitle", @""), kTitleKey,
						 NSLocalizedString(@"PageFiveExplain", @""), kExplainKey,
						 nil]];
	
	modalViewController = [[PageSixViewController alloc] initWithNibName:@"PageSixViewController" bundle:nil];
	
	// add our custom button to show our modal view controller
	UIButton* modalViewButton = [UIButton buttonWithType:UIButtonTypeInfoLight];
	[modalViewButton addTarget:self action:@selector(modalViewAction:) forControlEvents:UIControlEventTouchUpInside];
	UIBarButtonItem *modalButton = [[UIBarButtonItem alloc] initWithCustomView:modalViewButton];
	self.navigationItem.rightBarButtonItem = modalButton;
	[modalViewButton release];
	
	[myTableView reloadData];
}

- (void)dealloc
{
    [myTableView release];
	[menuList release];
	[modalViewController release];
	
	[super dealloc];
}

- (IBAction)styleAction:(id)sender
{
	UIActionSheet *styleAlert =
		[[UIActionSheet alloc] initWithTitle:@"Choose a UIBarStyle:"
								delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:nil
								otherButtonTitles:@"Default", @"BlackOpaque", @"BlackTranslucent", nil, nil];
	
	// use the same style as the nav bar
	styleAlert.actionSheetStyle = self.navigationController.navigationBar.barStyle;
	
	[styleAlert showInView:self.view];
	[styleAlert release];
}

// user clicked the "i" button, present page six as modal UIViewController
- (IBAction)modalViewAction:(id)sender
{
	// present page six as a modal child or overlay view
	[[self navigationController] presentModalViewController:modalViewController animated:YES];
}

// change the navigation bar style, also make the status bar match with it
- (void)actionSheet:(UIActionSheet *)modalView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	switch (buttonIndex)
	{
		case 0:
		{
			[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleDefault;
			self.navigationController.navigationBar.barStyle = UIBarStyleDefault;
			break;
		}
		case 1:
		{
			[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleBlackOpaque;
			self.navigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
			break;
		}
		case 2:
		{
			[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleBlackTranslucent;
			self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
			break;
		}
	}
}

#pragma mark UIViewController delegates

- (void)viewWillAppear:(BOOL)animated
{
	NSIndexPath *tableSelection = [myTableView indexPathForSelectedRow];
	[myTableView deselectRowAtIndexPath:tableSelection animated:NO];
}

- (void)viewDidAppear:(BOOL)animated
{
	// do something here as our view re-appears
}


#pragma mark UITableView delegates

- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellAccessoryDisclosureIndicator;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	UIViewController *targetViewController = [[menuList objectAtIndex: indexPath.row] objectForKey:kViewControllerKey];
	if (targetViewController == nil)
	{
		// the view controller has not been created yet, create it and set it to our menuList array
		
		// create a new dictionary with the new view controller
		//
		NSMutableDictionary *newItemDict = [NSMutableDictionary dictionaryWithCapacity:3];
		[newItemDict addEntriesFromDictionary: [menuList objectAtIndex: indexPath.row]];	// copy the title and explain strings
		
		// which view controller do we create?
		switch (indexPath.row)
		{
			case kPageOneIndex:
			{
				targetViewController = [[PageOneViewController alloc] initWithNibName:@"PageOneViewController" bundle:nil];
				break;
			}
				
			case kPageTwoIndex:
			{
				targetViewController = [[PageTwoViewController alloc] initWithNibName:@"PageTwoViewController" bundle:nil];
				break;
			}
				
			case kPageThreeIndex:
			{
				targetViewController = [[PageThreeViewController alloc] initWithNibName:@"PageThreeViewController" bundle:nil];
				break;
			}
				
			case kPageFourIndex:
			{
				targetViewController = [[PageFourViewController alloc] initWithNibName:@"PageFourViewController" bundle:nil];
				break;
			}
				
			case kPageFiveIndex:
			{
				targetViewController = [[PageFiveViewController alloc] initWithNibName:@"PageFiveViewController" bundle:nil];
				break;
			}
		}

		// add the new view controller to the dictionary and then to the 'menuList' array
		[newItemDict setObject:targetViewController forKey:kViewControllerKey];
		[menuList replaceObjectAtIndex:	indexPath.row withObject: newItemDict];
		[targetViewController release];
		
		// load the view controll back in to push it
		targetViewController = [[menuList objectAtIndex: indexPath.row] objectForKey:kViewControllerKey];
	}
	
	// present the rest of the pages normally
	[[self navigationController] pushViewController:targetViewController animated:YES];
}


#pragma mark UITableView datasource methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [menuList count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	MyCustomCell *cell = (MyCustomCell*)[tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
	if (cell == nil)
	{
		cell = [[[MyCustomCell alloc] initWithFrame:CGRectZero reuseIdentifier:kCellIdentifier] autorelease];
	}

	// get the view controller's info dictionary based on the indexPath's row
	cell.dataDictionary = [menuList objectAtIndex:indexPath.row];
	
	return cell;
}

@end

