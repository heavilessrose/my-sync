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

#import "SLVodAppDelegate.h"

@interface LKViewController : UIViewController <LKImgDownloadDelegate> {
    
    NSMutableData   *jsonData;
    NSMutableArray  *movies;
    NSURLConnection *listConn;
    
    BOOL            allRequestShouldCancel;
    NSMutableDictionary	*imageDownloadsInProgress;
}

@property (nonatomic, assign) BOOL              allRequestShouldCancel;
@property (nonatomic, retain) NSMutableDictionary	*imageDownloadsInProgress;
@property (nonatomic, retain) NSURLConnection   *listConn;
@property (nonatomic, retain) NSMutableArray    *movies;
@property (nonatomic, retain) NSMutableData     *jsonData;

- (id)parse:(NSData *)theData;
- (void)cancelListConn;

- (void)loadImagesForOnscreenRows:(UITableView *)theTable;

@end
