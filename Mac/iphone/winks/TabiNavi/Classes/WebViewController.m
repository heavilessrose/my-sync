//
//  WebViewController.m
//  TabiNavi
//
//  Created by luke on 7/5/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "WebViewController.h"

@implementation WebViewController

#define kLeftMargin				20.0
#define kTopMargin				20.0
#define kRightMargin			20.0
#define kBottomMargin			20.0
#define kTweenMargin			10.0
#define kTextFieldHeight		30.0
- (id)init
{
	self = [super init];
	if (self)
	{
		// navigation bar标题
		self.title = NSLocalizedString(@"WebTitle", @"");
	}
	return self;
}

- (void)dealloc
{
	[myWebView release];
	[urlField release];
	
	[super dealloc];
}

- (void)loadView
{
	// the base view for this view controller
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	contentView.backgroundColor = [UIColor whiteColor];
	
	// important for view orientation rotation
	contentView.autoresizesSubviews = YES;
	contentView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);	
	
	self.view = contentView;
	
	[contentView release];
	
	CGRect webFrame = [[UIScreen mainScreen] applicationFrame];
	webFrame.origin.y += kTopMargin + 5.0;
	webFrame.size.height -= 40.0;
	myWebView = [[UIWebView alloc] initWithFrame:webFrame];
	myWebView.backgroundColor = [UIColor whiteColor];
	myWebView.scalesPageToFit = YES;
	myWebView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
	myWebView.delegate = self;
	[self.view addSubview: myWebView];
	
	CGRect textFieldFrame = CGRectMake(kLeftMargin, kTweenMargin, self.view.bounds.size.width - (kLeftMargin * 2.0), kTextFieldHeight);
	urlField = [[UITextField alloc] initWithFrame:textFieldFrame];
    urlField.borderStyle = UITextBorderStyleBezel;
    urlField.textColor = [UIColor blackColor];
    urlField.delegate = self;
    urlField.placeholder = @"<enter a URL>";
    urlField.text = @"http://www.google.com";
	urlField.backgroundColor = [UIColor whiteColor];
	urlField.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	urlField.returnKeyType = UIReturnKeyGo;
	urlField.keyboardType = UIKeyboardTypeURL;
	urlField.autocorrectionType = UITextAutocorrectionTypeNo;
	urlField.clearButtonMode = UITextFieldViewModeAlways;
	[self.view addSubview:urlField];
	
	[myWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.google.com"]]];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
}

// 取消键盘
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	[textField resignFirstResponder];
	[myWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:[textField text]]]];
	
	return YES;
}


#pragma mark UIWebView delegate methods

- (void)webViewDidStartLoad:(UIWebView *)webView
{
	// 状态栏显示loading图标
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
	// 取消状态栏显示loading图标
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = NO;
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = NO;
	
	NSString* errorString = [NSString stringWithFormat:
							 @"<html><center><font size=+5 color='red'>An error occurred:<br>%@</font></center></html>",
							 error.localizedDescription];
	[myWebView loadHTMLString:errorString baseURL:nil];
}

@end
