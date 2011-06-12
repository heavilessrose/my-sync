//
//  SLHotController.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLHotController.h"
#import "LKShadowTableView.h"

@interface SLHotController ()
- (void)fetchHotMovs;
@end


@implementation SLHotController

@synthesize table, tmpCell;

- (void)dealloc
{
    [tmpCell release];
    [table release];
    [super dealloc];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self fetchHotMovs];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - hot movs: http://i.siluhd.com/ipadgetnew.asp
// 
- (void)fetchHotMovs {
    
    NSURL *hotsUrl = [NSURL URLWithString:SL_HOT relativeToURL:SL_BASE_HOST];
    NSURLRequest *hotsReq = [NSURLRequest requestWithURL:hotsUrl];
    NSURLConnection *hotsConn = [NSURLConnection connectionWithRequest:hotsReq delegate:self];
    [hotsConn start];
}

#pragma mark connection handle 

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    DLOG
    
	if (jsonData) {
		self.jsonData = nil;
	}
	NSLog(@"response: URL: %@", [response URL]);
	NSLog(@"response: MIME: %@", [response MIMEType]);
	NSLog(@"response: enc: %@", [response textEncodingName]);
	self.jsonData = [NSMutableData data];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    DLOG
	if (jsonData) {
		[jsonData appendData:data];
	} else {
		ALog(@"jsonData invaliad!");
	}
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    DLOG
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    DLOG
}



#pragma mark json parse

- (id)parse:(NSData *)theData
{
    DLOG
    NSString *jsonStr = [super parse:theData];
    NSArray *hotList = [jsonStr JSONValue];
    return hotList;
}

#pragma mark - table view delegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 80;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
}

- (BOOL)tableView:(UITableView *)tableView shouldIndentWhileEditingRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}

#pragma mark table data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [movies count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"SLHotCell";
    SLHotCell *theCell = (SLHotCell *)[tableView dequeueReusableCellWithIdentifier:cellID];
    if (!theCell) {
        theCell = [[[SLHotCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID] autorelease];
    }
    theCell.movie = [movies objectAtIndex:indexPath.row];
    return theCell;
}

#pragma - BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

@end
