//
//  ImageDownloader.m
//  BabyBear
//
//  Created by luke on 10-8-20.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ImageDownloader.h"
#import "TapImage.h"

#define kProductIconHeight 48

@implementation ImageDownloader

@synthesize delegate, product, indexPathInTableView, activeDownload, imageConnection, index;


#pragma mark Mark

- (void)dealloc
{
    [product release];
    [indexPathInTableView release];
    
    [activeDownload release];
    
    [imageConnection cancel];
    [imageConnection release];
    
    [super dealloc];
}

- (void)startDownload:(DownlaodType)aDt imgIndex:(int)aIndex
{
    self.activeDownload = [NSMutableData data];
	dt = aDt;
	NSString *urlStr = nil;
	switch (aDt) {
		case DT_PRODUCT_ICON:
			//index = -1;
			urlStr = product.pUrlIcon;
			break;
		case DT_PRODUCT_IMG:
			assert(self.index == aIndex);
			index = aIndex;
			urlStr = [product.pgallary objectAtIndex:index];
			break;
		default:
			NSLog(@"unkown dt");
			break;
	}
	
    // alloc+init and start an NSURLConnection; release on completion/failure
    NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:
                             [NSURLRequest requestWithURL:
                              [NSURL URLWithString:urlStr]] delegate:self];
    self.imageConnection = conn;
    [conn release];
}

- (void)cancelDownload
{
    [self.imageConnection cancel];
    self.imageConnection = nil;
    self.activeDownload = nil;
}


#pragma mark -
#pragma mark Download support (NSURLConnectionDelegate)

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [self.activeDownload appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	// Clear the activeDownload property to allow later attempts
    self.activeDownload = nil;
    
    // Release the connection now that it's finished
    self.imageConnection = nil;
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    // Set appIcon and clear temporary data/image
    UIImage *image = [[UIImage alloc] initWithData:self.activeDownload];
    if (dt == DT_PRODUCT_ICON) {
		if (image.size.width != kProductIconHeight && image.size.height != kProductIconHeight) {
			CGSize itemSize = CGSizeMake(kProductIconHeight, kProductIconHeight);
			UIGraphicsBeginImageContext(itemSize);
			CGRect imageRect = CGRectMake(0.0f, 0.0f, itemSize.width, itemSize.height);
			[image drawInRect:imageRect];
			self.product.productIcon = UIGraphicsGetImageFromCurrentImageContext();
			UIGraphicsEndImageContext();
		}
		else {
			self.product.productIcon = image;
		}
	}
	else if (dt == DT_PRODUCT_IMG) {
		TapImage *theTapImageView = (TapImage *)[self.product.productImgs objectAtIndex:index];
		theTapImageView.image = image;
	} else {
		assert(0);
	}
    
    self.activeDownload = nil;
    [image release];
    
    // Release the connection now that it's finished
    self.imageConnection = nil;
	
    // call our delegate and tell it that our icon is ready for display
	if (dt == DT_PRODUCT_ICON) {
		[delegate imageDidLoadWithIndexPath:self.indexPathInTableView];
	} else if (dt == DT_PRODUCT_IMG) {
		[delegate imageDidLoadWithIndex:index];
	}
}


@end
