//
//  WBImgDownload.h
//  WBNetwork
//
//  Created by luke on 11-2-22.
//  Copyright 2011 58.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class LKImageRecord;

@protocol LKImgDownloadDelegate <NSObject>

@required
- (void)imageDidLoad:(NSIndexPath *)indexPath;
- (void)imageLoadFailed:(NSIndexPath *)indexPath;

@end


#define kTableIconHeight 50

@interface LKImgDownload : NSObject {
	
	LKImageRecord				*imgRecord;
    NSIndexPath					*indexPathInTableView;
	
    id <LKImgDownloadDelegate>	delegate;
    
    NSMutableData				*activeDownload;
    NSURLConnection				*imageConnection;
}

@property (nonatomic, retain) LKImageRecord					*imgRecord;
@property (nonatomic, retain) NSIndexPath					*indexPathInTableView;
@property (nonatomic, assign) id <LKImgDownloadDelegate>	delegate;
@property (nonatomic, retain) NSMutableData					*activeDownload;
@property (nonatomic, retain) NSURLConnection				*imageConnection;

- (void)startDownload;
- (void)cancelDownload;

@end
