//
//  ZwpController.h
//  MSN
//
//  Created by  apple on 09-3-17.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LoadingView.h"

@interface ZwpController : UIViewController <UIWebViewDelegate,UIAccelerometerDelegate>
{
	UIWebView *zwpView;
	NSString *url;
	NSURL *documentUrl;
	uint linkType;
	NSString *advisitString;
	NSString *targetString;
	NSString *targetType;
	NSString *baseUrlString;
	NSString *baseUrlPath;
	NSString *htmlString;
	NSString *session;
	UIAlertView *myAlertView;
	UISegmentedControl *segmentedControl;
	NSTimer *timer;
	NSMutableArray *webUrls;
	BOOL cacheHtml;
	BOOL back;
	BOOL sysDismiss;
	
	UIAcceleration *lastAcceleration;
	int shakeCount;
	
	int requestFrom;
	LoadingView *loadingView;
}

- (id)initWithUrl:(NSString*)theUrl type:(int)theType;

- (void)backAction:(id)sender;
- (void)homeAction:(id)sender;
- (void)refreshAction:(id)sender;
- (void)segmentAction:(id)sender;
- (void)zwpTimeOut:(NSTimer*)theTimer;

- (NSString*)linkString:(NSString*)theUrl;
- (void)setUrlString:(NSString*)theString;
- (void)setUrl:(NSString*)theUrl headers:(NSDictionary*)headers 
		  body:(NSData*)body method:(NSString*)method;
- (void)notifyZwpTitleChanged: (NSNotification*)note;
- (void)notifyZwpDataChanged: (NSNotification*)note;

@property (nonatomic,retain)NSString *baseUrlString;
@property (nonatomic,retain)NSString *baseUrlPath;
@property (nonatomic,retain)NSString *targetString;
@property (nonatomic,retain)NSString *targetType;
@property (nonatomic,retain)NSString *advisitString;
@property (nonatomic,retain)NSString *htmlString;
@property (nonatomic,retain)NSString *url;
@property (nonatomic,retain)NSString *session;
@property (nonatomic,retain)UIAlertView *myAlertView;
@property (nonatomic,retain)NSTimer *timer;
@property (nonatomic, retain) LoadingView *loadingView;


@property (nonatomic, retain) UIAcceleration *lastAcceleration;

- (void)setUrl:(NSString*)theUrl;

@end
