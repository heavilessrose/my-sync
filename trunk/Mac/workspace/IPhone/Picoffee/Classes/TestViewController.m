//
//  TestViewController.m
//  Picoffee
//
//  Created by luke on 10-7-16.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TestViewController.h"
#import "pcTwoLineAlertView.h"
#import <QuartzCore/QuartzCore.h>
#import "pcProgressView.h"
#import "UIImage+roundCorner.h"
#import "TapImage.h"
#import <QuartzCore/QuartzCore.h>

@implementation TestViewController
@synthesize scrollShow, imgNameArr_test;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    [super loadView];
	self.view.backgroundColor = [UIColor whiteColor];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 处理locale变化
    NSNotificationCenter *globalNc = [NSNotificationCenter defaultCenter];
    [globalNc addObserver:self selector:@selector(localeChanged) name:NSCurrentLocaleDidChangeNotification object:nil];
    
#if 0
	pcTwoLineAlertView *alert = [[pcTwoLineAlertView alloc] initWithTitle:@"title" 
																  message:@"message" 
																 delegate:self 
														cancelButtonTitle:@"cancel" 
														otherButtonTitles:@"ok", nil];
	[alert show];
	[alert release];
#endif
	
#if 0
	CGRect viewFrame = CGRectMake(40, 200, 80, 50);
	UIWebView *progressView = [[UIWebView alloc] initWithFrame:viewFrame];
	progressView.backgroundColor = [UIColor blackColor];
	//[progressView setCornerRadius:10];
	[progressView setClipsToBounds:YES];
	
	[progressView.layer setBorderColor:
	 [[UIColor colorWithRed:0.52 green:0.09 blue:0.07 alpha:1] CGColor]];
	[progressView.layer setBorderWidth:2.75];
	
	[self.view addSubview:progressView];
	[progressView release];
#elif 0
	CGRect viewFrame = CGRectMake(120, 200, 80, 50);
	UIView *progressView = [[UIView alloc] initWithFrame:viewFrame];
	progressView.backgroundColor = [UIColor blackColor];
	[progressView setClipsToBounds:YES];
    
    CGRect indFram = CGRectMake(25, 2, 30, 30);
    UIActivityIndicatorView *ind = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    ind.frame = indFram;
    [ind startAnimating];
	[progressView addSubview:ind];
	[self.view addSubview:progressView];
	[progressView release];
#elif 0
    CGRect progressFrame = CGRectMake(60, 200, 200, 80);
    pcProgressView *progressView = [[pcProgressView alloc] initWithFrame:progressFrame];
    progressView.msgLabel.text = @"asdkjflasjdflaalsjdflajsldfja;sdlfja;dslfkja;dslfkjasd;flajdsf;aljsdlfjalsdjkf";
    [self.view addSubview:progressView];
    [progressView release];
#endif
	
	// 圆角图片
	UIImage *testImg = [UIImage imageNamed:@"test.png"];
	CGRect targetRect = CGRectMake(10, 50, 100, 100);
	UIImageView *imgView = [[UIImageView alloc] initWithFrame:targetRect];
#if 0			
	UIImage *rounded = [UIImage makeRoundCornerImageWithWidth:100 withHeight:200 cornerWidth:20 cornerHeight:20];
	UIImageView *imgView = [[UIImageView alloc] initWithImage:rounded];
#elif 0
	imgView.image = testImg;
	imgView.layer.cornerRadius = 5.0;
	imgView.layer.masksToBounds = YES;
	imgView.layer.borderColor = [UIColor lightGrayColor].CGColor;
	imgView.layer.borderWidth = 1.0;
#elif 0
	UIImage *rounded = [UIImage roundedImage:testImg targetRect:targetRect];
	imgView.image = rounded;
#elif 0
	UIImage *rounded = [UIImage makeRoundCornerImage:testImg cornerWidth:20 cornerHeight:20];
	imgView.image = rounded;
#endif
	[self.view addSubview:imgView];
	[imgView release];
    
#if 1 // scroll show
    self.imgNameArr_test = [NSArray arrayWithObjects:
                       @"ballmer1.jpg",
                       @"hoff2.jpg",
                       @"brolin.jpg",
                       @"hoff1.jpg",
                       @"ballmer2.jpg",
                       nil];
    CGRect scrollRect = CGRectMake(10, 50, 300, 100);
    CGSize pageContentSize = CGSizeMake(80, 80);
    
    self.scrollShow = [[ScrollShowView alloc] initWithFrame:scrollRect pageContentSize:pageContentSize];
    scrollShow.backgroundColor = [UIColor darkGrayColor];
    scrollShow.backShadow = YES;
    scrollShow.pageDelegate = self;
    scrollShow.pageStyle = PAGESTYLE_PADDING;
    scrollShow.x_padding = 10.0f;
    scrollShow.y_padding = 10.0f;
    [self.view addSubview:scrollShow];
    [scrollShow release];
    self.view.userInteractionEnabled = YES;
#endif
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
	[scrollShow didReceiveMemoryWarning];
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [imgNameArr_test release];
    [scrollShow release];
    [super dealloc];
}

#pragma mark -
#pragma mark ScrollShowViewPageDelegate
- (UIView *)viewForPageAtIndex:(ScrollShowView *)scrollView pageIndex:(int)index
{
    CGRect ImgViewAtPageRect = CGRectMake(0, 0, 80, 80.0f);
    TapImage *ImgViewAtPage = [[[TapImage alloc] initWithFrame:ImgViewAtPageRect] autorelease];
	ImgViewAtPage.userInteractionEnabled = YES;
	ImgViewAtPage.image = [UIImage imageNamed:[imgNameArr_test objectAtIndex:index]];
    return ImgViewAtPage;
}

- (int)itemCount:(ScrollShowView *)scrollView
{
    return [imgNameArr_test count];
}

#pragma mark -
#pragma mark notifications
- (void)localeChanged
{
    NSLog(@"[%s] ", _cmd);
}

#pragma mark alert

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 1) {
		NSLog(@"press ok");
	} else {
		NSLog(@"press cancel");
	}
}

@end
