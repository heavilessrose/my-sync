//
//  URLCacheConnection.m
//  propertyList
//
//  Created by wang luke on 5/8/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "URLCacheConnection.h"
#import "Alert.h"


@implementation URLCacheConnection

@synthesize delegate;
@synthesize receivedData;
@synthesize lastModified;
@synthesize conn;

// 中止联网
- (void) cancel
{
	NSLog(@"%d",[conn retainCount]);
	if(self.conn != nil){
		[self.conn cancel];
		[self.conn release];
	}else{
		NSLog(@"conn is null");
	}
}

// 利用request初始化连接。异步方式，实现NSURLConnection的一些委托方法用于当不同事件发生是回调。
- (id) initWithURL:(NSURL *)theURL delegate:(id<URLCacheConnectionDelegate>)theDelegate
{
	if (self = [super init]) {
		
		self.delegate = theDelegate;
		NSURLRequest *theRequest = [NSURLRequest requestWithURL:theURL
													cachePolicy:NSURLRequestReloadIgnoringLocalCacheData 
												timeoutInterval:60];
		
		receivedData = [[NSMutableData alloc] initWithLength:0];
		conn = [[NSURLConnection alloc] initWithRequest:theRequest 
																	  delegate:self 
															  startImmediately:YES];
		if (conn == nil) {
			NSString *message = NSLocalizedString (@"无法初始化request.", 
												   @"NSURLConnection initialization method failed.");
			alertWithMessage(message);
		}
	}
	
	return self;
}

- (NSString *) description
{
	return @"URLCacheConnection";
}

- (void)dealloc
{
	[receivedData release];
	[lastModified release];
//	[conn release];
	[super dealloc];
}

#pragma mark -
#pragma mark NSURLConnection delegate methods
- (void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    /* This method is called when the server has determined that it has
	 enough information to create the NSURLResponse. It can be called
	 multiple times, for example in the case of a redirect, so each time
	 we reset the data. */
    [self.receivedData setLength:0];
	
	/* Try to retrieve last modified date from HTTP header. If found, format  
	 date so it matches format of cached image file modification date. */
	if ([response isKindOfClass:[NSHTTPURLResponse self]]) {
		NSDictionary *headers = [(NSHTTPURLResponse *)response allHeaderFields];
		NSString *modified = [headers objectForKey:@"Last-Modified"];
		if (modified) {
			NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
			[dateFormatter setDateFormat:@"EEE, dd MMM yyyy HH:mm:ss zzz"];
			self.lastModified = [dateFormatter dateFromString:modified];
			[dateFormatter release];
		}
		else {
			/* default if last modified date doesn't exist (not an error) */
			self.lastModified = [NSDate dateWithTimeIntervalSinceReferenceDate:0];
		}
	}
}


- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    /* Append the new data to the received data. */
    [self.receivedData appendData:data];
}


- (void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	alertWithError(error);
	[self.delegate connectionDidFail:self];
	[connection release];
}


- (NSCachedURLResponse *) connection:(NSURLConnection *)connection 
				   willCacheResponse:(NSCachedURLResponse *)cachedResponse
{
	/* this application does not use a NSURLCache disk or memory cache */
    return nil;
}


- (void) connectionDidFinishLoading:(NSURLConnection *)connection
{
	[self.delegate connectionDidFinish:self];
	[connection release];
}


@end
