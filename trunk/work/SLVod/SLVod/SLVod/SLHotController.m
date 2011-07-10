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
- (void)fetchHotMovs:(BOOL)showHUD;
@end


@implementation SLHotController

@synthesize table;

- (void)dealloc
{
    MLog(@"");
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
    [self fetchHotMovs:YES];
    
    
	if (_refreshHeaderView == nil) {
		
		EGORefreshTableHeaderView *view = [[EGORefreshTableHeaderView alloc] initWithFrame:CGRectMake(0.0f, 0.0f - self.table.bounds.size.height, self.view.frame.size.width, self.table.bounds.size.height)];
		view.delegate = self;
		[self.table addSubview:view];
		_refreshHeaderView = view;
		[view release];
		
	}
	
	//  update the last update date
	[_refreshHeaderView refreshLastUpdatedDate];
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
- (void)fetchHotMovs:(BOOL)showHUD {
    
//    [[LKTipCenter defaultCenter] postFallingTipWithMessage:@"加载中..." inContainer:(self.view) time:0];
    if (showHUD) {
        [self HUDWithGradient:@"加载中..."];
    }
    NSURL *hotsUrl = [NSURL URLWithString:[NSString stringWithFormat:SL_HOT, page] relativeToURL:SL_BASE_HOST];
    NSURLRequest *hotsReq = [NSURLRequest requestWithURL:hotsUrl];
    self.listConn = [NSURLConnection connectionWithRequest:hotsReq delegate:self];
    [self.listConn start];
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
    if (movies && [movies count] > 0) {
        [movies removeAllObjects];
    }
    for (NSDictionary *aDic in movList) {
        SLMovie *aMov = [[SLMovie alloc] initWithDic:aDic];
        [movies addObject:aMov];
        [aMov release];
    }
    [self cancelListConn];
    [table reloadData];
    
    [NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(fetchImages) userInfo:nil repeats:NO];
//    [[LKTipCenter defaultCenter] disposeFallingTip:self.view];
    if (HUD) {
        [HUD hide:YES];
    }
    
    if (_reloading) {
        [self doneLoadingTableViewData];
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    DLOG
    [self cancelListConn];
//    [[LKTipCenter defaultCenter] changeFallingTip:self.view withText:@"network err"];
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

- (void)imageDidLoad:(NSIndexPath *)indexPath theImgRecord:(LKImageRecord *)aRec
{
    [super imageDidLoad:indexPath theImgRecord:aRec];
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


#pragma mark - ego refresh table delegate 

- (void)egoRefreshTableHeaderDidTriggerRefresh:(EGORefreshTableHeaderView*)view
{
    [self reloadTableViewDataSource];
}

- (BOOL)egoRefreshTableHeaderDataSourceIsLoading:(EGORefreshTableHeaderView*)view
{
    return _reloading;
}

- (NSDate*)egoRefreshTableHeaderDataSourceLastUpdated:(EGORefreshTableHeaderView*)view{
	
	return [NSDate date]; // should return date data source was last changed
	
}

#pragma mark UIScrollViewDelegate Methods

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{	
	
	[_refreshHeaderView egoRefreshScrollViewDidScroll:scrollView];
    
}

- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate{
	
	[_refreshHeaderView egoRefreshScrollViewDidEndDragging:scrollView];
	
}

#pragma mark Data Source Loading / Reloading Methods

- (void)reloadTableViewDataSource
{
	//  should be calling your tableviews data source model to reload
	//  put here just for demo
	_reloading = YES;
    
    [self cancelAllImgLoading];
    [self fetchHotMovs:NO];
}

- (void)doneLoadingTableViewData
{
	//  model should call this when its done loading
	_reloading = NO;
    
	[_refreshHeaderView egoRefreshScrollViewDataSourceDidFinishedLoading:self.table];
	
}

@end
