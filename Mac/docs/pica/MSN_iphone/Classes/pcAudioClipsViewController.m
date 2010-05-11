//
//  pcAudioClipsViewController.m
//  MSN
//
//  Created by  apple on 09-12-4.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "pcAudioClipsViewController.h"
#import "API.h"

@implementation pcAudioClipsViewController
@synthesize audioClips;
@synthesize selectedIndexPath;

- (void)viewDidLoad {
    [super viewDidLoad];
	selectedIndexPath				= nil;
	
	UIBarButtonItem* aButton		= [[UIBarButtonItem alloc] initWithBarButtonSystemItem: UIBarButtonSystemItemCancel
																			  target: self
																			  action: @selector(cancel:)];
	self.navigationItem.leftBarButtonItem	= aButton;
	[aButton release];
	aButton							= [[UIBarButtonItem alloc] initWithBarButtonSystemItem: UIBarButtonSystemItemDone
																  target: self
																  action: @selector(ok:)];
	self.navigationItem.rightBarButtonItem	= aButton;
	[aButton release];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
	if (audioClips == nil){
		NSString* aFilePath = audioClipPath(nil);
		NSArray* anItems = [[NSFileManager defaultManager] contentsOfDirectoryAtPath: aFilePath
																			   error: nil];
		audioClips = [[NSMutableArray alloc] initWithArray: anItems];
	}
	self.navigationItem.rightBarButtonItem.enabled = ([audioClips count] > 0);
}

#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return ([audioClips count] <= 0) ? 1 : [audioClips count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    NSString* aCellId = [NSString stringWithFormat: @"%d", indexPath.row];
    UITableViewCell *aCell = [tableView dequeueReusableCellWithIdentifier: aCellId];
    if (aCell == nil) {
        aCell = [[[UITableViewCell alloc] initWithFrame: CGRectZero reuseIdentifier: aCellId] autorelease];
    }
    
    // Set up the cell...
	if ([audioClips count] == 0)
		aCell.text		= NSLocalizedString(@"No Audio Clips", nil);
	else{
		aCell.text			= [audioClips objectAtIndex: indexPath.row];
    }
    return aCell;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	UITableViewCell* aCell = nil;
	[tableView deselectRowAtIndexPath: indexPath animated: NO];
	if ([audioClips count] == 0 || selectedIndexPath == indexPath) return;
	aCell = [tableView cellForRowAtIndexPath: indexPath];
	aCell.accessoryType = UITableViewCellAccessoryCheckmark;
	if (selectedIndexPath){
		aCell = [tableView cellForRowAtIndexPath: selectedIndexPath];
		aCell.accessoryType = UITableViewCellAccessoryNone;
	}
	self.selectedIndexPath = indexPath;
}


- (void)dealloc {
	[selectedIndexPath release];
	[audioClips release];
    [super dealloc];
}

- (IBAction)ok: (id)sender{
	[self.navigationController dismissModalViewControllerAnimated: YES];
}

- (IBAction)cancel: (id)sender{
	self.selectedIndexPath = nil;
	[self.navigationController dismissModalViewControllerAnimated: YES];
}

@end

