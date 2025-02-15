//
//  LKViewController.h
//  siluVod
//
//  Created by luke on 11-6-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIViewController+iconImage.h"
#import "SBJson.h"
#import "LKTipCenter.h"
#import "UIView+LKAddition.h"
#import "LKImgDownload.h"
#import "LKImageRecord.h"
#import "LKShadowTableView.h"
#import "NSString+URL.h"

#import "SLHotCell.h"
#import "SLMovInfoCell.h"
#import "SLUserProfileCell.h"
#import "LKMoreCell.h"

#import "SLVodAppDelegate.h"
#import "MBProgressHUD.h"

@interface LKViewController : UIViewController <LKImgDownloadDelegate, SLHotCellDelegate, MBProgressHUDDelegate> {
    
    NSMutableData   *jsonData;
    NSMutableArray  *movies;
    BOOL            shouldLoadNextPage;
    NSURLConnection *listConn;
    NSMutableData   *searchJsonData;
    NSURLConnection *searchConn;
    NSMutableArray  *searchList;
    int        page;
    int        pageSearch;
    NSInteger nextingPn;
    NSInteger nextingPnSearch;
    NSString *key;
    
    BOOL            allRequestShouldCancel;
    NSMutableDictionary	*imageDownloadsInProgress;
    
    UITableView     *theTable;
    
    // cells
    LKMoreCell          *tmpMoreCell;
    SLHotCell           *tmpHotCell;
    SLMovInfoCell       *tmpMovInfoCell;
    SLUserProfileCell   *tmpUProfileCell;
    MBProgressHUD       *HUD;
}

@property (nonatomic, assign) int               page;
@property (nonatomic, assign) BOOL              shouldLoadNextPage;
@property (nonatomic, assign) UITableView       *theTable;
@property (nonatomic, retain) NSString *key;

@property (nonatomic, assign) IBOutlet LKMoreCell          *tmpMoreCell;
@property (nonatomic, assign) IBOutlet SLHotCell *tmpHotCell;
@property (nonatomic, assign) IBOutlet SLMovInfoCell *tmpMovInfoCell;
@property (nonatomic, assign) IBOutlet SLUserProfileCell *tmpUProfileCell;

@property (nonatomic, assign) BOOL              allRequestShouldCancel;
@property (nonatomic, retain) NSMutableDictionary	*imageDownloadsInProgress;
@property (nonatomic, retain) NSURLConnection   *listConn;
@property (nonatomic, retain) NSURLConnection   *searchConn;
@property (nonatomic, retain) NSMutableArray    *movies;
@property (nonatomic, retain) NSMutableData     *jsonData;
@property (nonatomic, retain) NSMutableData     *searchJsonData;
@property (nonatomic, retain) NSMutableArray    *searchList;


- (void)hideTabbar;
- (void)showTabbar;

- (void)searchWithKeyword:(NSString *)key pn:(NSInteger)aPn;

#pragma mark - HUD
- (void)HUDWithLabel:(NSString *)tip;
- (void)HUDWithGradient:(NSString *)tip;

- (void)cancelAllImgLoading;
@end



#pragma mark - ListAndImageLoad()
@interface LKViewController (ListAndImageLoad)
- (id)parse:(NSData *)theData;
- (void)cancelListConn;
- (void)cancelSearchConn;
- (void)loadImagesForOnscreenRows:(UITableView *)theTable;
@end

#pragma mark - Http Live Stream()
@interface LKViewController (HLS)
- (void)initAndPlayMovie:(NSURL *)movieURL;
@end

