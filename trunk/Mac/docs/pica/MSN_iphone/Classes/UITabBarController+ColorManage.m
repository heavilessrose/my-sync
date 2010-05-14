//
//  UITabBarController+ColorManage.m
//  MSN
//
//  Created by Jun Du on 6/16/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "UITabBarController+ColorManage.h"

@interface UITabBarController (private)
- (UITabBar *)tabBar;
@end
@implementation UITabBarController_ColorManage
@synthesize segmentedControl;
@synthesize chatImageView;
@synthesize myToolBar;

- (void)segmentAction:(id)sender
{
	self.selectedIndex = [sender selectedSegmentIndex];
}

- (id)init {
    [super init];
	
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	
	NSArray *listArray = nil;
	
	if ([dict objectForKey:@"second_zwp"] != nil)
	{
		listArray = [NSArray arrayWithObjects:
					 [UIImage imageNamed:@"a10.png"],
					 [UIImage imageNamed:@"myzone.png"],
					 [UIImage imageNamed:@"roster-png.png"],
					 [UIImage imageNamed:@"dialog-png.png"],
					 [UIImage imageNamed:@"zwp-png.png"],
					 nil];
	}else{
		
		if ([dict objectForKey:@"zwp"] != nil)
		{
			listArray = [NSArray arrayWithObjects:
						 [UIImage imageNamed:@"myzone.png"],
						 [UIImage imageNamed:@"roster-png.png"],
						 [UIImage imageNamed:@"dialog-png.png"],
						 [UIImage imageNamed:@"zwp-png.png"],
						 nil];
		}
		else
		{
			listArray = [NSArray arrayWithObjects:
						 [UIImage imageNamed:@"myzone.png"],
						 [UIImage imageNamed:@"roster-png.png"],
						 [UIImage imageNamed:@"dialog-png.png"],
						 nil];
		}
	}
	
	segmentedControl = [[UISegmentedControl alloc] initWithItems:listArray];
	segmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;	
	segmentedControl.tintColor = [UIColor colorWithRed:28.0/255.0 green:79.0/255.0 blue:130.0/255.0 alpha:1.0];
	[segmentedControl addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview:segmentedControl];
	CGRect aFrame = [self tabBar].frame;
	aFrame.origin.y -= 2;
	aFrame.size.height += 2;
	aFrame.origin.x -= 4;
	aFrame.size.width += 8;
	segmentedControl.frame = aFrame;
    
    myToolBar = [[UIToolbar alloc] initWithFrame: aFrame];
    [self.view addSubview: myToolBar];
    [myToolBar setHidden: YES];
	
	int i = 0;
	chatImageView = [UIImageView alloc];
	for(UIImageView* iv in segmentedControl.subviews)
	{
		if(i == 2)
		{
			chatImageView = iv;
		}
		i++;
	}
	
	return self;
}

- (void)dealloc {
	[segmentedControl release];
    [myToolBar release];
    [super dealloc];
}
@end