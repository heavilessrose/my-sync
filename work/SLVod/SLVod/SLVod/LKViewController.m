//
//  LKViewController.m
//  siluVod
//
//  Created by luke on 11-6-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKViewController.h"

@interface LKViewController ()
@end


@implementation LKViewController

@synthesize jsonData, movies;

- (void)dealloc
{
    [movies release];
    self.jsonData = nil;
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
    
    self.movies = [NSMutableArray array];
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

#pragma mark - JSON 
- (id)parse:(NSData *)theData
{
	NSString *jsonStr = [[[NSString alloc] initWithData:theData 
                                               encoding:NSUTF8StringEncoding] autorelease];
    
	if (jsonStr && [jsonStr length] > 0) {
        return jsonStr;
    }
    
    ALog(@"jsonStr invalid");
    // should implemented in subClasses
    return nil;
}

#pragma mark - BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

@end
