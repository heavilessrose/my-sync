//
//  Downloader.m
//  propertyList
//
//  Created by wang luke on 5/7/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "Downloader.h"


@implementation Downloader

// Creating a connection
- (NSData *)createConn
{
	// create the request
	NSURLRequest *theRequest=[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.apple.com/"]
											  cachePolicy:NSURLRequestUseProtocolCachePolicy
										  timeoutInterval:60.0];
	// create the connection with the request and start loading the data
	NSURLConnection *theConnection=[[NSURLConnection alloc] initWithRequest:theRequest delegate:self];
	if (theConnection) {
		// Create the NSMutableData that will hold the received data
		receivedData = [[NSMutableData data] retain];
	} else {
		// inform the user that the download could not be made
		NSLog(@"Error: the download could not be made");
	}
}

#pragma mark -
#pragma mark NSURLConnection basic delegate methods
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    // this method is called when the server has determined that it
    // has enough information to create the NSURLResponse
	
    // it can be called multiple times, for example in the case of a
    // redirect, so each time we reset the data.
    // receivedData is declared as a method instance elsewhere
    [receivedData setLength:0];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    // append the new data to the receivedData
    // receivedData is declared as a method instance elsewhere
    [receivedData appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    // release the connection, and the data object
    [connection release];
    // receivedData is declared as a method instance elsewhere
    [receivedData release];
	
    // inform the user
    NSLog(@"Connection failed! Error - %@ %@", [error localizedDescription], [[error userInfo] objectForKey:NSErrorFailingURLStringKey]);
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    // do something with the data
    // receivedData is declared as a method instance elsewhere
    NSLog(@"Succeeded! Received %d bytes of data",[receivedData length]);
	
    // release the connection, and the data object
    [connection release];
    [receivedData release];
}

#pragma mark -
#pragma mark 

@end
