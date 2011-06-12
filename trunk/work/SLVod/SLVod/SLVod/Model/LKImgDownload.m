//
//  WBImgDownload.m
//  WBNetwork
//
//  Created by luke on 11-2-22.
//  Copyright 2011 58.com. All rights reserved.
//

#import "LKImgDownload.h"
#import "LKImageRecord.h"


@implementation LKImgDownload

@synthesize imageConnection, activeDownload, delegate, indexPathInTableView, imgRecord;

- (void)dealloc {
	
	MLog(@"");
	[imgRecord release];
	[indexPathInTableView release];
	self.imageConnection = nil;
	self.activeDownload = nil;
	[super dealloc];
}

- (void)startDownload {
	
    self.activeDownload = [NSMutableData data];
    NSURLConnection *conn = [NSURLConnection connectionWithRequest:[NSURLRequest requestWithURL:
																	imgRecord.url]
							 delegate:self];
	DLog(@"%@", imgRecord.url);
	
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    self.imageConnection = conn;
}

- (void)cancelDownload {
	
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    [self.imageConnection cancel];
    self.imageConnection = nil;
    self.activeDownload = nil;
}

#pragma mark -
#pragma mark NSURLConnectionDelegate

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    [self.activeDownload appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
	
    self.activeDownload = nil;
    self.imageConnection = nil;
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	if (delegate && [delegate respondsToSelector:@selector(imageLoadFailed:)]) {
		[delegate imageLoadFailed:self.indexPathInTableView];
	}
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
	
    UIImage *image = [[UIImage alloc] initWithData:self.activeDownload];
#if 0
	if (image.size.width != kTableIconHeight && image.size.height != kTableIconHeight ) {
		CGSize itemSize = CGSizeMake(kTableIconHeight, kTableIconHeight);
		UIGraphicsBeginImageContext(itemSize);
		CGRect imageRect = CGRectMake(0.0, 0.0, itemSize.width, itemSize.height);
		[image drawInRect:imageRect];
		self.tableRecord.picImage = UIGraphicsGetImageFromCurrentImageContext();
		UIGraphicsEndImageContext();
	} else {
		self.tableRecord.picImage = image;
	}
#else
	self.imgRecord.img = image;
#endif

    self.activeDownload = nil;
    [image release];
    
    self.imageConnection = nil;
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	if (delegate && [delegate respondsToSelector:@selector(imageDidLoad:)]) {
		[delegate imageDidLoad:self.indexPathInTableView];
	}
}

@end
