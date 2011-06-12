//
//  SLHotController.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLHotController.h"

@interface SLHotController ()
- (void)fetchHotMovs;
@end


@implementation SLHotController

- (void)dealloc
{
    [super dealloc];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self fetchHotMovs];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - hot movs: http://i.siluhd.com/ipadgetnew.asp
// 
- (void)fetchHotMovs {
    
    NSURL *hotsUrl = [NSURL URLWithString:SL_HOT relativeToURL:SL_BASE_HOST];
    NSURLRequest *hotsReq = [NSURLRequest requestWithURL:hotsUrl];
    NSURLConnection *hotsConn = [NSURLConnection connectionWithRequest:hotsReq delegate:self];
    [hotsConn start];
}

#pragma mark connection handle 

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    DLOG
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    DLOG
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    DLOG
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    DLOG
}

#pragma mark -
#pragma BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

@end
