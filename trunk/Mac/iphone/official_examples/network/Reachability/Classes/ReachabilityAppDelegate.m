/*

File: ReachabilityAppDelegate.m
Abstract: The application's controller.

Version: 1.5

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "ReachabilityAppDelegate.h"

@interface ReachabilityAppDelegate()
- (void)updateCarrierDataNetworkWarning;
- (void)addTable;
- (NSString *)hostName;
@end

@implementation ReachabilityAppDelegate

typedef enum {
	RemoteHostReachableSection = 0,
	InternetConnectionSection,
	ComputerToComputerSection
} TableViewSections;

@synthesize window;
@synthesize contentView;
@synthesize summaryLabel;
@synthesize carrierDataNetworkImage;
@synthesize wiFiImage;
@synthesize stopImage;
@synthesize tableView;
@synthesize remoteHostStatus;
@synthesize internetConnectionStatus;
@synthesize localWiFiConnectionStatus;

- (void)dealloc {
    [contentView release];
    [summaryLabel release];
    [window release];
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{	
	
	/*
     You can use the Reachability class to check the reachability of a remote host
     by specifying either the host's DNS name (www.apple.com) or by IP address.
     */
	[[Reachability sharedReachability] setHostName:[self hostName]];
	//[[Reachability sharedReachability] setAddress:@"0.0.0.0"];
	
	// The Reachability class is capable of notifying your application when the network
	// status changes. By default, those notifications are not enabled.
	// Uncomment the following line to enable them:
	//[[Reachability sharedReachability] setNetworkStatusNotificationsEnabled:YES];
    
	self.carrierDataNetworkImage = [UIImage imageNamed:@"WWAN5.png"];
	self.wiFiImage = [UIImage imageNamed:@"Airport.png"];
	self.stopImage = [UIImage imageNamed:@"stop-32.png"];
	
	[self addTable];
	[self updateStatus];
	[self updateCarrierDataNetworkWarning];
	
	[self.window makeKeyAndVisible];
    
    // Observe the kNetworkReachabilityChangedNotification. When that notification is posted, the
    // method "reachabilityChanged" will be called. 
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:@"kNetworkReachabilityChangedNotification" object:nil];
}

- (void)reachabilityChanged:(NSNotification *)note
{
    [self updateStatus];
}

- (void)updateStatus
{
	// Query the SystemConfiguration framework for the state of the device's network connections.
	self.remoteHostStatus           = [[Reachability sharedReachability] remoteHostStatus];
	self.internetConnectionStatus	= [[Reachability sharedReachability] internetConnectionStatus];
	self.localWiFiConnectionStatus	= [[Reachability sharedReachability] localWiFiConnectionStatus];
	
	[tableView reloadData];
}

- (BOOL)isCarrierDataNetworkActive
{
	return (self.remoteHostStatus == ReachableViaCarrierDataNetwork);
}

- (NSString *)hostName
{
	// Don't include a scheme. 'http://' will break the reachability checking.
	// Change this value to test the reachability of a different host.
	return @"www.apple.com";
}

- (NSString *)hostNameLabel
{
	return [NSString stringWithFormat:@"Remote Host: %@", [self hostName]];
}

- (void)configureTableCell:(UITableViewCell *)cell forIndexPath:(NSIndexPath *)path inTable:(UITableView *)table
{
	NSUInteger row = path.row;
	
	cell.font = [UIFont systemFontOfSize:12.0];
	cell.textColor = [UIColor darkGrayColor];
	cell.textAlignment = UITextAlignmentLeft;
	
	/*
     Table 1:
     Remote Host Reachable
     Not reachable | Reachable via EDGE | Reachable via WiFi
     
     Table 2:
     Connection to Internet
     Not available | Available via EDGE | Available via WiFi
     
     Table 3:
     Connection to Local Network.
     Not available | Available via WiFi
     */
	
	if (path.section == RemoteHostReachableSection) {
		
		if (self.remoteHostStatus == NotReachable) {
			cell.text = @"Cannot Connect To Remote Host.";
			cell.image = self.stopImage;
		} else if (self.remoteHostStatus == ReachableViaCarrierDataNetwork) {
			cell.text = @"Reachable Via Carrier Data Network.";
			cell.image = self.carrierDataNetworkImage;
		} else if (self.remoteHostStatus == ReachableViaWiFiNetwork) {
			cell.text = @"Reachable Via WiFi Network.";
			cell.image = self.wiFiImage;
		}
	}
	
	if (path.section == InternetConnectionSection) {
		if (self.internetConnectionStatus == NotReachable) {
			cell.text = @"Access Not Available.";
			cell.image = self.stopImage;
		} else if (self.internetConnectionStatus == ReachableViaCarrierDataNetwork) {
			cell.text = @"Available Via Carrier Data Network.";
			cell.image = self.carrierDataNetworkImage;
		} else if (self.internetConnectionStatus == ReachableViaWiFiNetwork) {
			cell.text = @"Available Via WiFi Network.";
			cell.image = self.wiFiImage;
		}
	}
	
	if (path.section == ComputerToComputerSection) {
		
		if (self.localWiFiConnectionStatus == NotReachable) {
			cell.text = @"Access Not Available.";
			cell.image = self.stopImage;
		} else if (self.localWiFiConnectionStatus == ReachableViaWiFiNetwork) {
			cell.text = @"Available Via WiFi Network.";
			cell.image = self.wiFiImage;
		}
	}
}

- (void)addTable
{
	CGRect tableFrame = [[UIScreen mainScreen] applicationFrame];
	
	tableView = [[UITableView alloc] initWithFrame:tableFrame style:UITableViewStyleGrouped];
	[tableView setAutoresizingMask:(UIViewAutoresizingFlexibleWidth |UIViewAutoresizingFlexibleHeight)];
	tableView.delegate = self;
	tableView.dataSource = self;
	tableView.rowHeight = 44.0;
	tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
	tableView.sectionHeaderHeight = 28.0;
	tableView.scrollEnabled = NO;
	
	[self.contentView insertSubview:tableView belowSubview:summaryLabel];
    [self.contentView bringSubviewToFront:summaryLabel];
	[tableView reloadData];
	[tableView autorelease];	
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 3;
}

- (NSInteger)tableView:(UITableView *)table numberOfRowsInSection:(NSInteger)section
{
	return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	if (section == 0) {
		return [self hostNameLabel];
	}
	if (section == 1) {
		return @"Access To Internet Hosts";
	}
	if (section == 2) {
		return @"Access To Local Bonjour Hosts";
	}
	return @"Unknown";
}

- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	// Disable row selection.
	return nil;
}

- (UITableViewCell *)tableView:(UITableView *)theTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *ReachabilityTableCellIdentifier = @"ReachabilityTableCell";
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:ReachabilityTableCellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:ReachabilityTableCellIdentifier] autorelease];
	}
    
	[self configureTableCell:cell forIndexPath:indexPath inTable:theTableView];
	
	return cell;
}

- (UIFont *)statusKeyLabelFont
{
	return [UIFont systemFontOfSize:12.0];
}

- (void)updateCarrierDataNetworkWarning
{
	if ([self isCarrierDataNetworkActive]) {
		summaryLabel.hidden = NO;
	} else {
		summaryLabel.hidden = YES;        
    }
}

@end
