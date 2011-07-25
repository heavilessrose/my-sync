//
//  LKViewController.m
//  siluVod
//
//  Created by luke on 11-6-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKViewController.h"
#import "BCTabBarController.h"
#import "SLVodAppDelegate.h"
#import <MediaPlayer/MediaPlayer.h>
#import "SLMovie.h"
#import "LKVideoController.h"
#import "NSString+URL.h"


#define LK_Frame_Portrait       CGRectMake(0, 0, 320, 372)
#define LK_Frame_Landscape      CGRectMake(0, 0, 480, 239)


@interface LKViewController ()
- (void)disselectCurrentRow;
@end

#pragma mark -
@implementation LKViewController

@synthesize page;
@synthesize allRequestShouldCancel;
@synthesize jsonData, movies, listConn, imageDownloadsInProgress, searchConn, searchJsonData, searchList;
@synthesize tmpMoreCell, tmpHotCell, tmpMovInfoCell, tmpUProfileCell, theTable, shouldLoadNextPage;

- (void)dealloc
{
    MLog(@"");
    HUD.delegate = nil;
    [self cancelAllImgLoading];
    self.imageDownloadsInProgress = nil;
    [self.listConn cancel];
    self.listConn = nil;
    [self.searchConn cancel];
    self.searchConn = nil;
    self.jsonData = nil;
    [movies release];
    self.searchJsonData = nil;
    [searchList release];
    
    [super dealloc];
}

- (void)cancelAllImgLoading
{
    LKImgDownload *imageDown;
    for (NSIndexPath *key in imageDownloadsInProgress) {
        if ((imageDown = [imageDownloadsInProgress objectForKey:key])) {
            [imageDown cancelDownload];
        }
    }
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.page = 1;
        self.shouldLoadNextPage = YES;
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
    
    self.movies = [NSMutableArray array];
    self.imageDownloadsInProgress = [NSMutableDictionary dictionary];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self disselectCurrentRow];
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
#pragma mark Orientation
- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    DLOG
    if (toInterfaceOrientation == UIDeviceOrientationUnknown) {
        DLog(@"UIDeviceOrientationUnknown");
    }
    if (toInterfaceOrientation == UIDeviceOrientationPortrait) {
        DLog(@"UIDeviceOrientationPortrait: %@", self.view);
        self.view.frame = LK_Frame_Portrait;
    }
    if (toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) {
        DLog(@"UIDeviceOrientationLandscapeLeft: %@", self.view);
        self.view.frame = LK_Frame_Landscape;
    }
    if (toInterfaceOrientation == UIDeviceOrientationLandscapeRight) {
        DLog(@"UIDeviceOrientationLandscapeRight: %@", self.view);
        self.view.frame = LK_Frame_Landscape;
    }
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    DLOG
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    DLOG
}

#pragma mark - table 
- (void)disselectCurrentRow
{
    NSIndexPath *curSelectedIndex = [self.theTable indexPathForSelectedRow];
    if (curSelectedIndex) {
        [self.theTable deselectRowAtIndexPath:curSelectedIndex animated:YES];
    }
}

#pragma mark - JSON 
- (id)parse:(NSData *)theData
{
	NSString *jsonStr = [[[NSString alloc] initWithData:theData 
                                               encoding:NSUTF8StringEncoding] autorelease];
    
	if (jsonStr && [jsonStr length] > 0) {
        return jsonStr;
    }
    
    ALog(@"jsonStr invalid");
    // should implemented in subClasses
    return nil;
}

- (void)cancelListConn
{
    self.jsonData = nil;
    [self.listConn cancel];
    self.listConn = nil;
}

- (void)cancelSearchConn
{
    self.searchJsonData = nil;
    [self.searchConn cancel];
    self.searchConn = nil;
}

#pragma mark - image download handle

- (void)startImageDown:(LKImageRecord *)brecord forIndexPath:(NSIndexPath *)indexPath
{
	@synchronized (imageDownloadsInProgress) {
		if (!allRequestShouldCancel) {
			LKImgDownload *imageDown = [imageDownloadsInProgress objectForKey:indexPath];
			if (imageDown == nil) 
			{
				imageDown = [[LKImgDownload alloc] init];
				imageDown.imgRecord = brecord;
				imageDown.indexPathInTableView = indexPath;
				imageDown.delegate = self;
				[imageDownloadsInProgress setObject:imageDown forKey:indexPath];
				[imageDown startDownload];
				[imageDown release];   
			}
		}
	}
}

- (void)loadImagesForOnscreenRows:(UITableView *)aTable
{
    if ([movies count] > 0)
    {
        NSArray *visiblePaths = [aTable indexPathsForVisibleRows];
        for (NSIndexPath *indexPath in visiblePaths)
        {
            UITableViewCell *mcell = [aTable cellForRowAtIndexPath:indexPath];
            if (![mcell isKindOfClass:[LKMoreCell class]]) {
                SLMovie *aMov = [movies objectAtIndex:indexPath.row];
                if (aMov.imgRecord && aMov.imgRecord.show && !aMov.imgRecord.downloaded) {
                    LKImageRecord *cRecord = aMov.imgRecord;
                    if (cRecord.url) {
                        [self startImageDown:cRecord forIndexPath:indexPath];
                    }
                }
            }
        }
    }
}

#pragma mark LKImageDownloadDelegate

- (void)imageDidLoad:(NSIndexPath *)indexPath theImgRecord:(LKImageRecord *)aRec
{
    // implement in subClasses
    aRec.downloaded = YES;
}

- (void)imageLoadFailed:(NSIndexPath *)indexPath theImgRecord:(LKImageRecord *)aRec
{
    // implement in subClasses
}

#pragma mark - play 

- (void)play:(SLMovie *)theMov
{
    // implement in subClasses
}

- (void)download:(SLMovie *)theMov
{
    // implement in subClasses
}

#pragma mark - media player

- (void)initAndPlayMovie:(NSURL *)movieURL
{
    NSLog(@"%@", movieURL);
    if (NSClassFromString(@"UISplitViewController") != nil) {
        LKVideoController *mp = [[LKVideoController alloc] initWithContentURL:movieURL];
        [[mp moviePlayer] prepareToPlay];
        [[mp moviePlayer] setFullscreen:YES animated:YES];
        [[mp moviePlayer] setMovieSourceType:MPMovieSourceTypeStreaming];
        [[mp moviePlayer] setShouldAutoplay:YES];
        [[mp moviePlayer] setControlStyle:MPMovieControlStyleFullscreen];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(moviePlayBackDidFinish:) name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
        [self presentMoviePlayerViewControllerAnimated:mp];
        [mp release];
        [self hideTabbar];
    }
}

//  Notification called when the movie finished preloading.
- (void)moviePreloadDidFinish:(NSNotification *)notification
{
    DLOG
    /* 
     < add your code here >
     
     MPMoviePlayerController* moviePlayerObj=[notification object];
     etc.
     */
}

//  Notification called when the movie finished playing.
- (void)moviePlayBackDidFinish:(NSNotification *)notification
{
    DLOG
    DLog(@"%@", self.view);
    [self showTabbar];
    [self dismissMoviePlayerViewControllerAnimated];
    /*
     < add your code here >
     
     MPMoviePlayerController* moviePlayerObj=[notification object];
     etc.
     */
}

//  Notification called when the movie scaling mode has changed.
- (void)movieScalingModeDidChange:(NSNotification*)notification
{
    DLOG
    /* 
     < add your code here >
     
     MPMoviePlayerController* moviePlayerObj=[notification object];
     etc.
     */
}

#pragma mark - image down

- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    if (!decelerate && scrollView == theTable)
	{
        [self loadImagesForOnscreenRows:(UITableView *)scrollView];
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    if (scrollView == theTable) {
        [self loadImagesForOnscreenRows:(UITableView *)scrollView];
    }
}

#pragma mark - Tip

- (void)showTipFromTopWith:(NSString *)tipStr theAnimation:(BOOL)showAnimation
{
    
}

#pragma mark - BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

- (void)hideTabbar
{
    globalApp.tabBarController.tabBar.hidden = YES;
}

- (void)showTabbar
{
    globalApp.tabBarController.tabBar.hidden = NO;
}

#pragma mark - HUD

- (void)disposeHUD
{
    
}

- (void)HUDWithLabel:(NSString *)tip {
    
    HUD = [[MBProgressHUD alloc] initWithView:self.navigationController.view];
	[self.navigationController.view addSubview:HUD];
	
    HUD.delegate = self;
    HUD.labelText = tip;
	
//    [HUD showWhileExecuting:@selector(myTask) onTarget:self withObject:nil animated:YES];
    [HUD show:YES];
}

- (void)HUDWithGradient:(NSString *)tip {
	UIView *tmp = self.navigationController.view;
    HUD = [[MBProgressHUD alloc] initWithView:tmp];
	[tmp addSubview:HUD];
	
	HUD.dimBackground = YES;
	
	// Regiser for HUD callbacks so we can remove it from the window at the right time
    HUD.delegate = self;
    HUD.labelText = tip;
    [HUD show:YES];
}

#pragma mark - search 

- (void)searchWithKeyword:(NSString *)key
{
    DLog(@"key= %@", key);
    NSURL *searchUrl = [NSURL URLWithString:[NSString stringWithFormat:SL_SEARCH, [key URLEncodedString]] relativeToURL:SL_BASE_HOST];
    NSURLRequest *searchReq = [NSURLRequest requestWithURL:searchUrl];
    self.searchConn = [NSURLConnection connectionWithRequest:searchReq delegate:self];
}

@end
