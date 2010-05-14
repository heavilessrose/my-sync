//
//  conversation.m
//  MSN
//
//  Created by  apple on 09-1-12.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Conversation.h"
#import "MSNAppDelegate.h"
#import "ChatViewController.h"
#import "ImageLabel.h"
#import "define.h"
#import "API.h"

@implementation Conversation
@synthesize datasource;

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) 
	{
	}
	return self;
}

// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
}

- (void)viewDidAppear:(BOOL)animated
{
	self.navigationController.navigationBar.tintColor = [UIColor colorWithRed:32.0/255.0 green:75.0/255.0 blue:118.0/255.0 alpha:1.0];
	[self.navigationController setNavigationBarHidden:NO animated:YES];
	[super viewDidAppear:animated];
}

- (void)setDatasourceFile:(NSString*)theFile
{
	if (theFile != nil)
	{
		NSArray *filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
		NSString *directory = [filePaths objectAtIndex: 0];
		NSString *fileString = [directory stringByAppendingPathComponent:theFile];
		char buffer[1024];
		int index ;
		NSMutableData *data = [NSMutableData data];
		FILE *file = fopen([fileString UTF8String], "a+");
		if (file != NULL)
		{
			fseek(file, 0, SEEK_SET);
			while (!feof(file))
			{
				memset(buffer, 0, sizeof(buffer));
				index = fread(buffer, 1, sizeof(buffer) - 1, file);
				if (index > 0)
				{
					[data appendBytes:buffer length:index];
				}
			}
			fclose(file);
			if ([data length] > 0)
			{
				NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
				self.datasource = [string componentsSeparatedByString:@"[***##]"];
				[string release];
			}
		}
	}
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	return [datasource count];
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	int height = 0;
	NSString *string = nil;
	NSString *name = nil;
	NSString *message = nil;
	NSString *reason = nil;
	NSArray *array = nil;
	
	string = [datasource objectAtIndex:indexPath.row];
	array = [string componentsSeparatedByString:@"[#####]"];
	name = [array objectAtIndex:0];
	if ([array count] == 2)
	{
		string = [array objectAtIndex:1];
		if (string != nil)
		{
			array = [string componentsSeparatedByString:@"[*****]"];	
			message = [array objectAtIndex:0];
			if ([array count] == 2)
			{
				reason = [array objectAtIndex:1];
			}
		}
	}
	
	UILabel *label = [[UILabel alloc] initWithFrame:CGRectZero];
	label.lineBreakMode = UILineBreakModeWordWrap;
	label.font = [UIFont systemFontOfSize:16];
	label.numberOfLines = 100;
	if (name != nil)
	{
		label.text = name;
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 240, 500) limitedToNumberOfLines:0];
		height += rc.size.height;	
	}
	
	if (message != nil)
	{
		label.text = message;
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 240, 500) limitedToNumberOfLines:0];
		height += rc.size.height;	
	}
	if (reason != nil)
	{
		label.text = reason;
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 240, 500) limitedToNumberOfLines:0];
		height += rc.size.height;
	}
	[label release];
	if (height == 0)
		height = 5;
	return height;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSString *string = [NSString stringWithFormat:@"%d",indexPath.row];
	NSString *name = nil;
	NSString *message = nil;
	NSString *reason = nil;
	NSArray *array = nil;
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:string];
	if (cell == nil) 
	{
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectMake(0, 0, 320, 5) reuseIdentifier:string] autorelease];

		string = [datasource objectAtIndex:indexPath.row];
		array = [string componentsSeparatedByString:@"[#####]"];
		name = [array objectAtIndex:0];
		if ([array count] == 2)
		{
			string = [array objectAtIndex:1];
			if (string != nil)
			{
				array = [string componentsSeparatedByString:@"[*****]"];	
				message = [array objectAtIndex:0];
				if ([array count] == 2)
				{
					reason = [array objectAtIndex:1];
				}
			}
		}
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		UILabel *label = nil;
		int height = 0;
		if (name != nil && [name length] > 0)
		{
			label = [[ImageLabel alloc] initWithFrame:CGRectMake(0, 0, 310, 500)];
			label.lineBreakMode = UILineBreakModeWordWrap;
			label.font = [UIFont systemFontOfSize:16];
			label.text = name;
			
			CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 300, 500) limitedToNumberOfLines:0];
			rc.origin.y += height;
			rc.size.width = 300;
			label.frame = rc;
			height += rc.size.height;
			label.numberOfLines = 100;
			label.textColor = [UIColor grayColor];	
			if (reason != nil && [reason length] > 0)
				label.font = [UIFont systemFontOfSize:12];
			
			[cell.contentView addSubview:label];
			[label release];
		}

		if (message != nil && [message length] > 0)
		{
			label = [[ImageLabel alloc] initWithFrame:CGRectZero];
			label.lineBreakMode = UILineBreakModeWordWrap;
			if (reason == nil || [reason length] == 0)
			{
				label.textColor = [UIColor blackColor];
				label.font = [UIFont systemFontOfSize:16];
			}
			else
			{
				label.textColor = [UIColor grayColor];
			}
			label.numberOfLines = 100;
			label.text = message;
			CGRect rc = [label textRectForBounds:CGRectMake(10, 0, 300, 500) limitedToNumberOfLines:0];
			rc.origin.y += height;
			rc.size.width = 300;
			label.frame = rc;
			height += rc.size.height;
			[cell.contentView addSubview:label];
			[label release];
		}

		if (reason != nil && [reason length] > 0)
		{
			label = [[UILabel alloc] initWithFrame:CGRectZero];
			label.lineBreakMode = UILineBreakModeWordWrap;
			label.font = [UIFont systemFontOfSize:16];
			label.textColor = [UIColor grayColor];
			label.numberOfLines = 100;
			label.text = reason;
			CGRect rc = [label textRectForBounds:CGRectMake(10, 0, 300, 500) limitedToNumberOfLines:0];
			rc.origin.y += height;
			label.frame = rc;
			[cell.contentView addSubview:label];
			[label release];
		}
	}
	// Configure the cell
	return cell;
}


- (void)dealloc 
{
//	MYLog(@"----------%@ dealloc------------------------",self);
	[datasource release];
	[super dealloc];
}


- (void)viewDidLoad 
{
	((UITableView*)self.view).separatorColor = [UIColor whiteColor];
	[super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
}


@end

