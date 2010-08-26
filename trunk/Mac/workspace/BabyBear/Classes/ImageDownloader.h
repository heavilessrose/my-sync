//
//  ImageDownloader.h
//  BabyBear
//
//  Created by luke on 10-8-20.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BaseProduct.h"
#import "Product.h"


typedef enum _DownloadType {
	DT_PRODUCT_ICON = 0,
	DT_PRODUCT_IMG,
}DownlaodType;

typedef int DownloadIndex;

@protocol ImageDownloaderDelegate
@optional
- (void)imageDidLoadWithIndexPath:(NSIndexPath *)indexPath;
- (void)imageDidLoadWithIndex:(int)aIndex;
@end


@interface ImageDownloader : NSObject {
    id <ImageDownloaderDelegate>	delegate;
	
	BaseProduct						*product;
    NSIndexPath						*indexPathInTableView;
    
    NSMutableData					*activeDownload;
    NSURLConnection					*imageConnection;
	DownlaodType					dt;
	DownloadIndex					index;
}

@property (nonatomic, assign) id <ImageDownloaderDelegate> delegate;

@property (nonatomic, retain) BaseProduct					*product;
@property (nonatomic, retain) NSIndexPath					*indexPathInTableView;

@property (nonatomic, retain) NSMutableData					*activeDownload;
@property (nonatomic, retain) NSURLConnection				*imageConnection;
@property (nonatomic, assign) DownloadIndex					index;

//- (void)startDownload;
- (void)startDownload:(DownlaodType)dt imgIndex:(int)aIndex;
- (void)cancelDownload;

@end
