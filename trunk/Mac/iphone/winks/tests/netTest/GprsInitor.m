//
//  GprsInitor.m
//  netTest
//
//  Created by wang luke on 09-10-23.
//  Copyright 2009 luke. All rights reserved.
//

#import "GprsInitor.h"
#import "Utils.h"

extern void sockTest();

@implementation GprsInitor
@synthesize connection;

- (void)gprsInit
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSURLRequest *theRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.google.com"]
												cachePolicy:NSURLRequestReloadIgnoringLocalCacheData 
											timeoutInterval:60];
	
	
	//NSURLConnection *nscon = [NSURLConnection connectionWithRequest:theRequest delegate:nil];
	connection = [[NSURLConnection alloc] initWithRequest:theRequest 
															 delegate:self startImmediately:YES];
	
	if(connection == nil){
		Winks_printf("NSURLConnection fail");
		return;
	}
	Winks_printf("NSURLConnection success!!");
	[pool release];
}

#pragma mark -
#pragma mark NSURLConnectionDelegate
- (void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
	Winks_printf("didReceiveResponse");
}


- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	Winks_printf("didReceiveData");
}


- (void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	Winks_printf("didReceiveData");
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
	Winks_printf("connectionDidFinishLoading");
	[connection release];
	
	sockTest();
}



@end
