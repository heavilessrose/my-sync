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
#if 1
    NSURL *hotsUrl = [NSURL URLWithString:[NSString stringWithFormat:SL_HOT, page] relativeToURL:SL_BASE_HOST];
#else
    
    NSURL *urlaa = [NSURL URLWithString:@"http://211.151.64.33:8081/jyapi"];
    NSString *urlPart = @"info/jylist/1/20?city=bj";
    NSURL *hotsUrl = [NSURL URLWithString:urlPart relativeToURL:urlaa];
#endif
    DLog(@"%@", hotsUrl);
    NSURLRequest *hotsReq = [NSURLRequest requestWithURL:hotsUrl];
//    DLog(@"%@")
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
    DLog(@"%@", [response allHeaderFields]);
    NSLog(@"response: URL: %@", [response URL]);
    NSLog(@"response: MIME: %@", [response MIMEType]);
    NSLog(@"response: enc: %@", [response textEncodingName]);
    if (connection == listConn) {
        if (jsonData) {
            self.jsonData = nil;
        }
        self.jsonData = [NSMutableData data];
    }
    
    
    if (connection == searchConn) {
        if (searchJsonData) {
            self.searchJsonData = nil;
        }
        self.searchJsonData = [NSMutableData data];
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    DLOG
    if (connection == listConn) {
        if (jsonData) {
            [jsonData appendData:data];
        } else {
            ALog(@"jsonData invaliad!");
        }
    }
    
    if (connection == searchConn) {
        if (searchJsonData) {
            [searchJsonData appendData:data];
        } else {
            ALog(@"searchJsonData invaliad!");
        }
    }
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    DLOG
    if (connection == listConn) {
        DLog(@"%@", [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]);
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
    
    if (connection == searchConn) {
        NSArray *aSearchList = [self parse:searchJsonData];
        
        for (NSDictionary *aDic in aSearchList) {
            SLMovie *aMov = [[SLMovie alloc] initWithDic:aDic];
            [searchList addObject:aMov];
            [aMov release];
        }
        [self cancelSearchConn];
        [self.searchDisplayController.searchResultsTableView reloadData];
        
        [NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(fetchImages) userInfo:nil repeats:NO];
        
        //    [[LKTipCenter defaultCenter] disposeFallingTip:self.view];
        if (HUD) {
            [HUD hide:YES];
        }
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    DLOG
    if (connection == listConn) {
        [self cancelListConn];
    }
    if (connection == searchConn) {
        [self cancelSearchConn];
    }
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
    CGFloat h = 0;
    if (tableView == table) {
        h = 80;
    }
    if (tableView == self.searchDisplayController.searchResultsTableView) {
        h = 44;
    }
    return h;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (tableView == table) {
        SLMovDetailController *detailVC = [[SLMovDetailController alloc] initWithNibName:@"SLMovDetailController" bundle:nil];
        detailVC.mov = [movies objectAtIndex:indexPath.row];
        [self.navigationController pushViewController:detailVC animated:YES];
        [detailVC release];
    }
    if (tableView == self.searchDisplayController.searchResultsTableView) {
        SLMovDetailController *detailVC = [[SLMovDetailController alloc] initWithNibName:@"SLMovDetailController" bundle:nil];
        detailVC.mov = [searchList objectAtIndex:indexPath.row];
        [self.navigationController pushViewController:detailVC animated:YES];
        [detailVC release];
    }
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
    int count = 0;
    if (tableView == table) {
        count = [movies count];
        if (count >= SL_PAGESIZE && shouldLoadNextPage) {
            count++;
        }
    }
    if (tableView == self.searchDisplayController.searchResultsTableView) {
        count = [searchList count];
    }
    return count;
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (tableView == table && indexPath.row == [movies count]) {
        
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *theCell;
    if (tableView == table) {
        if (indexPath.row == [movies count]) {
            static NSString *nextPageCellID = @"LKMoreCell";
        } else {
            static NSString *cellID = @"SLHotCell";
            SLHotCell *hCell = (SLHotCell *)[tableView dequeueReusableCellWithIdentifier:cellID];
            if (!hCell) {
                [[NSBundle mainBundle] loadNibNamed:@"SLHotCell" owner:self options:nil];
                if (tmpHotCell) {
                    hCell = tmpHotCell;
                    hCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                    self.tmpHotCell = nil;
                }
            }
            hCell.movie = [movies objectAtIndex:indexPath.row];
            theCell = hCell;
        }
    }
    
    if (tableView == self.searchDisplayController.searchResultsTableView) {
        
        static NSString *sCellID = @"UITableViewCell";
        theCell = [tableView dequeueReusableCellWithIdentifier:sCellID];
        if (!theCell) {
            theCell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:sCellID] autorelease];
            theCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        }
        SLMovie *s = [searchList objectAtIndex:indexPath.row];
        if (s) {
            theCell.textLabel.text = s.title;
            theCell.detailTextLabel.text = s.actor;
        }
    }
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

#pragma mark - search 

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchString:(NSString *)searchString
{
    return YES;
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)aSearchBar {
    if (searchList) {
        [searchList removeAllObjects];
    } else {
        self.searchList = [NSMutableArray array];
    }
	NSString *key = [aSearchBar.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
	if (key && [key length] > 0) {
        [self searchWithKeyword:key];
	}
}


@end
