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


@protocol ImageDownloaderDelegate
@required
- (void)imageDidLoad:(NSIndexPath *)indexPath;

@end


@interface ImageDownloader : NSObject {
    id <ImageDownloaderDelegate>	delegate;
	
	BaseProduct						*product;
    NSIndexPath						*indexPathInTableView;
    
    NSMutableData					*activeDownload;
    NSURLConnection					*imageConnection;
}

@property (nonatomic, assign) id <ImageDownloaderDelegate> delegate;

@property (nonatomic, retain) BaseProduct					*product;
@property (nonatomic, retain) NSIndexPath					*indexPathInTableView;

@property (nonatomic, retain) NSMutableData					*activeDownload;
@property (nonatomic, retain) NSURLConnection				*imageConnection;

- (void)startDownload;
- (void)cancelDownload;

@end
