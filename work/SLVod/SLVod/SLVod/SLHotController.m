//
//  SLHotController.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLHotController.h"
#import "SLMovDetailController.h"

@interface SLHotController ()
- (void)fetchHotMovs;
@end


@implementation SLHotController

@synthesize table;

- (void)dealloc
{
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
    self.theTable = table;
    
    self.title = @"Hot";
    [self fetchHotMovs];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
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
//    return (UIInterfaceOrientationIsLandscape(interfaceOrientation) || interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - hot movs: http://i.siluhd.com/ipadgetnew.asp
// 
- (void)fetchHotMovs {
    
    [[LKTipCenter defaultCenter] postFallingTipWithMessage:@"加载中..." inContainer:(self.view) time:0];
    NSURL *hotsUrl = [NSURL URLWithString:SL_HOT relativeToURL:SL_BASE_HOST];
    NSURLRequest *hotsReq = [NSURLRequest requestWithURL:hotsUrl];
    NSURLConnection *hotsConn = [NSURLConnection connectionWithRequest:hotsReq delegate:self];
    [hotsConn start];
}

- (void)fetchImages {
    [self performSelector:@selector(loadImagesForOnscreenRows:) withObject:self.table];
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
    NSArray *movList = [self parse:jsonData];
    
    for (NSDictionary *aDic in movList) {
        SLMovie *aMov = [[SLMovie alloc] initWithDic:aDic];
        [movies addObject:aMov];
        [aMov release];
    }
    [self cancelListConn];
    [table reloadData];
    
    [NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(fetchImages) userInfo:nil repeats:NO];
    [[LKTipCenter defaultCenter] disposeFallingTip:self.view];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    DLOG
    [self cancelListConn];
    [[LKTipCenter defaultCenter] changeFallingTip:self.view withText:@"network err"];
}


#pragma mark json parse

- (id)parse:(NSData *)theData
{
    DLOG
    NSString *jsonStr = [super parse:theData];
    NSArray *hotList = [jsonStr JSONValue];
    if (!hotList) {
        ALog(@"no movie fetched");
        return nil;
    }
    return hotList;
}

#pragma mark - table view delegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 80;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{   
    SLMovDetailController *detailVC = [[SLMovDetailController alloc] initWithNibName:@"SLMovDetailController" bundle:nil];
    detailVC.mov = [movies objectAtIndex:indexPath.row];
    [self.navigationController pushViewController:detailVC animated:YES];
    [detailVC release];
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
{
    DLOG
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
        [[NSBundle mainBundle] loadNibNamed:@"SLHotCell" owner:self options:nil];
        if (tmpHotCell) {
            theCell = tmpHotCell;
            theCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
            self.tmpHotCell = nil;
        }
    }
    theCell.movie = [movies objectAtIndex:indexPath.row];
    return theCell;
}


#pragma mark - LKImageDownloadDelegate

- (void)imageDidLoad:(NSIndexPath *)indexPath
{
	LKImgDownload *imageDown = imageDown = [imageDownloadsInProgress objectForKey:indexPath];
	
    if (imageDown != nil)
    {
		SLHotCell *cell = (SLHotCell *)[self.table cellForRowAtIndexPath:imageDown.indexPathInTableView];
        
        [cell.imageView setImage:imageDown.imgRecord.img];
        [imageDownloadsInProgress removeObjectForKey:indexPath];
	}
}

- (void)imageLoadFailed:(NSIndexPath *)indexPath
{
	DLog(@"cell image loaded fail at %@", indexPath);
	LKImgDownload *imageDown = imageDown = [imageDownloadsInProgress objectForKey:indexPath];
	
    if (imageDown != nil)
    {
		SLHotCell *cell = (SLHotCell *)[self.table cellForRowAtIndexPath:imageDown.indexPathInTableView];
        [cell.imageView setImage:[UIImage imageNamed: @"nopic.png"]];
	}
	[imageDownloadsInProgress removeObjectForKey:indexPath];
}


#pragma mark - BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

@end
