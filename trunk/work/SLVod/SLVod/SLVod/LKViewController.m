//
//  LKViewController.m
//  siluVod
//
//  Created by luke on 11-6-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKViewController.h"
#import "SLMovie.h"

@interface LKViewController ()
@end


@implementation LKViewController

@synthesize allRequestShouldCancel;
@synthesize jsonData, movies, listConn, imageDownloadsInProgress;
@synthesize tmpHotCell, tmpMovInfoCell;

- (void)dealloc
{
    MLog(@"");
    self.imageDownloadsInProgress = nil;
    [self.listConn cancel];
    self.listConn = nil;
    self.jsonData = nil;
    [movies release];
    self.jsonData = nil;
    
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
    
    self.movies = [NSMutableArray array];
    self.imageDownloadsInProgress = [NSMutableDictionary dictionary];
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

- (void)loadImagesForOnscreenRows:(UITableView *)theTable
{
    if ([movies count] > 0)
    {
        NSArray *visiblePaths = [theTable indexPathsForVisibleRows];
        for (NSIndexPath *indexPath in visiblePaths)
        {
            SLMovie *aMov = [movies objectAtIndex:indexPath.row];
			LKImageRecord *cRecord = aMov.imgRecord;
			if (cRecord.url) {
				[self startImageDown:cRecord forIndexPath:indexPath];
			}
        }
    }
}

#pragma mark LKImageDownloadDelegate

- (void)imageDidLoad:(NSIndexPath *)indexPath
{
    // implement in subClasses
}

- (void)imageLoadFailed:(NSIndexPath *)indexPath
{
    // implement in subClasses
}

#pragma mark - play 

- (void)play:(SLMovie *)theMov
{
    // implement in subClasses
}



#pragma mark - media player

-(void)initAndPlayMovie:(NSURL *)movieURL
{
    if (NSClassFromString(@"UISplitViewController") != nil) {
        MPMoviePlayerViewController *mp = [[MPMoviePlayerViewController alloc] initWithContentURL:movieURL];
        [[mp moviePlayer] prepareToPlay];
        [[mp moviePlayer] setShouldAutoplay:YES];
        [[mp moviePlayer] setControlStyle:MPMovieControlStyleFullscreen];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(moviePlayBackDidFinish:) name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
        
        [self presentMoviePlayerViewControllerAnimated:mp];
        
        [globalApp hideTabbar];
    }
}

//  Notification called when the movie finished preloading.
- (void)moviePreloadDidFinish:(NSNotification*)notification
{
    DLOG
    /* 
     < add your code here >
     
     MPMoviePlayerController* moviePlayerObj=[notification object];
     etc.
     */
}

//  Notification called when the movie finished playing.
- (void)moviePlayBackDidFinish:(NSNotification*)notification
{
    DLOG
    [globalApp showTabbar];
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



#pragma mark - BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

@end
