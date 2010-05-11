//
//  ChatViewsController.m
//  MSN
//
//  Created by  apple on 08-10-8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ChatViewsController.h"
#import "ChatViewController.h"
#import "ChatViewData.h"
#import "MSNAppDelegate.h"
#import "TouchScrollView.h"
#import "PersonInfo.h"
#import "define.h"


@interface ChatViewsController (PrivateMethods)

- (void)scrollViewDidScroll:(UIScrollView *)sender;

@end


@implementation ChatViewsController

@synthesize chatViewDatas, currentPage, myAlertView;


- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		currentPage = 0;
		sysDismiss = YES;
		pageControlUsed = NO;
		chatViewDatas = nil;
		closeButton = [[UIButton buttonWithType:UIButtonTypeCustom] retain];
		closeButton.frame = CGRectMake(0, 0, 20, 20);
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"close"
															  ofType:@"png"];
		UIImage *image = [UIImage imageWithContentsOfFile:imagePath];
		[closeButton setImage:image
					 forState:UIControlStateNormal];
		
		[closeButton addTarget:self action:@selector(closeCurrentPage:) forControlEvents:UIControlEventTouchUpInside];
	}
	return self;
}


// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
	self.view.backgroundColor = [UIColor blueColor];
	CGRect rect = CGRectMake(0, 30, 320, 40);
	label = [[UILabel alloc] initWithFrame:rect];
	label.backgroundColor = [UIColor clearColor];
	label.font = [UIFont systemFontOfSize:18.0];
	label.textAlignment = UITextAlignmentLeft;
	label.textColor = [UIColor whiteColor];
	label.textAlignment = UITextAlignmentCenter;
	label.numberOfLines = 2;
	[self.view addSubview:label];
	[label release];
	
	rect = CGRectMake(50, 90, 220, 300);
	picturesView = [[TouchScrollView alloc] initWithFrame:rect];
	picturesView.autoresizingMask = UIViewAutoresizingNone;
	picturesView.opaque = NO;
	picturesView.clipsToBounds = NO;
	picturesView.pagingEnabled = YES;
    picturesView.showsHorizontalScrollIndicator = NO;
    picturesView.showsVerticalScrollIndicator = NO;
    picturesView.scrollsToTop = NO;
    picturesView.delegate = self;
	[self.view addSubview:picturesView];
	[picturesView release];
	
	rect = CGRectMake(90, 384, 140, 30);
	pageControl = [[UIPageControl alloc] initWithFrame:rect];
	[pageControl addTarget:self action:@selector(changePage:) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview:pageControl];
	[pageControl release];
}

- (void)refreshView
{
	picturesView.contentSize = CGSizeMake(picturesView.frame.size.width * [chatViewDatas count], picturesView.frame.size.height);
	
	int i = 0;
	CGRect frame;
	[closeButton removeFromSuperview];

	for (; i < [chatViewDatas count]; i++)
	{
		ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:i];

		if (chatViewData = nil) 
		{
			ChatViewController *controller = nil;
			if (chatViewData.controller == nil)
			{
				controller = [[ChatViewController alloc] init];
				controller.chatViewData = chatViewData;				
			}
			else
			{
				controller = chatViewData.controller;				
			}
			NSArray *views = [picturesView subviews];
			if (i < [views count])    //closebutton view
			{
				
				UIImageView * view = [views objectAtIndex:i];
				view.image = [controller getImage];
				/*if (chatViewData.image != nil)
				{
					if (view.image != chatViewData.image)
					{					
						view.image = chatViewData.image;
					}
				}
				else
				{
					view.image = chatViewData.image;//[chatViewData getImage];
				}*/
			}
			else
			{
				frame = picturesView.frame;
				frame.origin.x = frame.size.width * i + 5;
				frame.origin.y = 10;
				frame.size.width -= 10;
				frame.size.height -= 10;
				UIImageView * view = [[UIImageView alloc] initWithFrame:frame];
				view.image = [controller getImage];
				//view.image = chatViewData.image;//[chatViewData getImage];	
				[picturesView addSubview:view];
				[view release];
			}
			if (chatViewData.controller == nil)
				[controller release];
		}
	}

	[picturesView addSubview:closeButton];
	pageControl.numberOfPages = [chatViewDatas count];
	[self scrollViewToPage:currentPage];
	pageControlUsed = NO;
}

- (void)viewDidLoad 
{	
	picturesView.contentSize = CGSizeMake(picturesView.frame.size.width * [chatViewDatas count], picturesView.frame.size.height);
	
	int i = 0;
	CGRect frame;
	
	for (; i < [chatViewDatas count]; i++)
	{
		ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:i];
		if (chatViewData != nil) 
		{
			ChatViewController *controller = nil;
			if (chatViewData.controller == nil)
			{
				controller = [[ChatViewController alloc] init];
				controller.chatViewData = chatViewData;
			}
			else
			{
				controller = chatViewData.controller;
			}
			frame = picturesView.frame;
			frame.origin.x = frame.size.width * i + 5;
			frame.origin.y = 10;
			frame.size.width -= 10;
			frame.size.height -= 10;
			UIImageView * view = [[UIImageView alloc] initWithFrame:frame];
			//view.image = chatViewData.image; //[chatViewData getImage];
			view.image = [controller getImage];
			[picturesView addSubview:view];
			[view release];
			if (chatViewData.controller == nil)
				[controller release];
		}
	}
	if (currentPage >= [chatViewDatas count] || currentPage < 0)
	{
		currentPage = 0;
	}

	[picturesView addSubview:closeButton];
	
	pageControl.numberOfPages = [chatViewDatas count];
	[self scrollViewToPage:currentPage];
	pageControlUsed = NO;
	[super viewDidLoad];
}

- (void)closeCurrentPage:(id)sender
{
	ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:currentPage];
	if ([chatViewData sendingFile]
		|| [chatViewData receivingFile])
	{
		self.myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert", nil)
												   message:NSLocalizedString(@"close_alert",nil)
												  delegate:self 
										 cancelButtonTitle:NSLocalizedString(@"qtn_cancel",nil) 
										 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil),nil] autorelease];
		[myAlertView setDelegate:self];
		
		[myAlertView show];
	}
	else
	{
		[self closeChatAtIndex:currentPage];
	}
			
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!sysDismiss)
	{
		alertView.delegate = nil;
		self.myAlertView = nil;
		
		if (buttonIndex == 1)
			[self closeChatAtIndex:currentPage];
	}
}



- (void)closeChatAtIndex:(int)index
{
	if (index > -1 && index < [chatViewDatas count])
	{
		if ([chatViewDatas count] == 1)
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:ShowContactListNotification 
																object:[[chatViewDatas allValues] objectAtIndex:0]];
			currentPage = 0;
		}
		else if (index == [chatViewDatas count] - 1)
		{
			UIImageView	*imageView = [[picturesView subviews] objectAtIndex:index];
			[imageView removeFromSuperview];
			currentPage = index -1;	
			[self scrollViewToPage:currentPage];
			
		}
		else
		{
			CGRect frameRect = closeButton.frame;
			frameRect.origin.x += picturesView.frame.size.width;
			closeButton.frame = frameRect;
			
			CGContextRef context = UIGraphicsGetCurrentContext();
			[UIView beginAnimations:nil context:context];
			[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
			[UIView setAnimationDuration:1.0];
			
			UIImageView	*imageView = [[picturesView subviews] objectAtIndex:index];
			[imageView removeFromSuperview];
			
			frameRect.origin.x -= picturesView.frame.size.width;
			closeButton.frame = frameRect;
			int i = 0;
			for (i = index; i < [[picturesView subviews] count] - 1; i++)
			{
				imageView = [[picturesView subviews] objectAtIndex:i];
				frameRect = imageView.frame;
				frameRect.origin.x -= picturesView.frame.size.width;
				imageView.frame = frameRect;
			}
			
			ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:index + 1];
			if (chatViewData != nil)
				[self setLabelText:chatViewData];
			
			[UIView commitAnimations];
		}
		
		NSString *key = [[chatViewDatas allKeys] objectAtIndex:index];
		if (key != nil)
			[chatViewDatas removeObjectForKey:key];
		
		pageControl.numberOfPages = [chatViewDatas count];
		picturesView.contentSize = CGSizeMake(picturesView.frame.size.width * [chatViewDatas count], picturesView.frame.size.height);
	}	
}

- (void)setLabelText:(ChatViewData*)theChatViewData
{
	PersonInfo *info = [theChatViewData.persons objectAtIndex:0];
	NSString *text = nil;
	if (info != nil)
	{
		if (theChatViewData.hasMessage)
			label.textColor = [UIColor orangeColor];
		else
			label.textColor = [UIColor whiteColor];
		
		if ([theChatViewData.persons count] > 1)
		{
			PersonInfo *other = [theChatViewData.persons objectAtIndex:1];
			if (other != nil)
			{
				if (info.nickname != nil)
				{
					if (other.nickname != nil)
						text = [info.nickname stringByAppendingFormat:@"; %@; ...",other.nickname];
					else
						text = [info.nickname stringByAppendingFormat:@"; %@; ...",other.imid];
				}
				else
				{
					if (other.nickname != nil)
						text = [info.imid stringByAppendingFormat:@"; %@; ...",other.nickname];
					else
						text = [info.imid stringByAppendingFormat:@"; %@; ...",other.imid];
				}	
			}
			else
			{
				if (info.nickname != nil)
						text = [info.nickname stringByAppendingString:@"; ..."];
				else
						text = [info.imid stringByAppendingString:@"; ..."];
			}
		}
		else
		{
			if (info.nickname != nil)
			{
				text = [info.nickname stringByAppendingFormat:@"\n%@",info.imid];
			}
			else
			{
				text = info.imid;
			}
		}
		label.text = text;
	}
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	MYLog(@"---------%@ didReceiveMemoryWarning---------",self);
}


- (void)dealloc 
{
	MYLog(@"-----------------%@ dealloc----------------", self);
	if (myAlertView != nil)
	{
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}
	picturesView.delegate = nil;
	[closeButton release];
	[super dealloc];
}

- (ChatViewData*)getControllerByTouchPoint:(NSNumber*)theXLocation
{
	ChatViewData *chatViewData = nil;
	if ([chatViewDatas count] > 0)
	{
		CGFloat pageWidth = picturesView.frame.size.width;
		int page = floor([theXLocation intValue] / pageWidth);
		chatViewData = [[chatViewDatas allValues] objectAtIndex:page];
	}
	return chatViewData;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (!pageControlUsed) 
	{
		CGFloat pageWidth = scrollView.frame.size.width;
		int page = floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
		if (page < [chatViewDatas count] && page >= 0)
		{
			ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:page];
			if (chatViewData != nil)
			{
				[self setLabelText:chatViewData];
				currentPage = page;
				pageControl.currentPage = currentPage;
				CGRect frame = picturesView.frame;
				frame.origin.x = frame.size.width * page;
				frame.origin.y = 0;
				CGRect rc = closeButton.frame;
				rc.origin = frame.origin;
				closeButton.frame = rc;
			}	
		}
    }
}

- (void)scrollViewToPage:(int)page
{
	ChatViewData *chatViewData = [[chatViewDatas allValues] objectAtIndex:page];
	if (chatViewData != nil)
	{
		[self setLabelText:chatViewData];
		CGRect frame = picturesView.frame;
		frame.origin.x = frame.size.width * page;
		frame.origin.y = 0;

		[picturesView scrollRectToVisible:frame animated:YES];
		CGRect rc = closeButton.frame;
		rc.origin = frame.origin;
		closeButton.frame = rc;
		pageControlUsed = YES;
		pageControl.currentPage = currentPage;
	}
	else
	{
		currentPage = pageControl.currentPage;
	}
}


- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView 
{
    pageControlUsed = NO;
}

- (IBAction)changePage:(id)sender 
{
	currentPage = pageControl.currentPage;
	[self scrollViewToPage:currentPage];
}

@end
