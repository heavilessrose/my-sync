//
//  ListHeaderView.m
//  MSN
//
//  Created by  apple on 08-10-20.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ListHeaderView.h"
#import "GroupInfo.h"
#import "PersonInfo.h"
#import "define.h"

NSString		*GroupHerderClickedNotification = @"GroupHerderClicked";
NSString		*ForceUnfoldGroupNotification	= @"forceUnfoldGroup";

@implementation ListHeaderView
@synthesize title,action,target,closed;

- (id)initWithFrame:(CGRect)frame groupInfo:(GroupInfo*)group type:(BOOL)sortByStatus
{
	if (self = [super initWithFrame:frame]) 
	{
		closed = YES;
		group.cell = self;
		self.tintColor = [UIColor whiteColor];
		self.alpha = 1.0;
		self.title = group.name;
		imageView = [[UIImageView alloc] initWithFrame:CGRectMake(5, 1, 23, 23)];
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"group_close-png"
															  ofType:@"png"];
		imageView.image = [UIImage imageWithContentsOfFile:imagePath];
		label = [[UILabel alloc] initWithFrame:CGRectMake(40, 1, 250, 23)];
		if (sortByStatus)
		{
			label.text = [title stringByAppendingFormat:@" ( %d )",[group.persons count]];	
		}
		else
		{
			PersonInfo *info = nil;
			int i = 0;
			for (info in group.persons)
			{
				if ([[info.state lowercaseString] compare:@"offline"] != NSOrderedSame)
					i++;
			}
			label.text = [title stringByAppendingFormat:@" ( %d / %d )",i,[group.persons count]];
		}
		label.backgroundColor = [UIColor clearColor];
		[self addSubview:imageView];
		[self addSubview:label];
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(notifyGroupHerderClicked:)
				   name:GroupHerderClickedNotification
				 object:nil];
		[nc addObserver:self selector:@selector(notifyForceUnfoldGroup:)
				   name:ForceUnfoldGroupNotification
				 object:nil];
		
		// Initialization code
	}
	return self;
}

- (void)forceUnfoldGroup
{
	NSString *imagePath = nil;
	
	imagePath = [[NSBundle mainBundle] pathForResource:@"group_open-png"
												ofType:@"png"];	
	closed = NO;
	imageView.image = [UIImage imageWithContentsOfFile:imagePath];
	[target performSelector:action withObject:self];
}

- (void)notifyForceUnfoldGroup: (NSNotification *)note
{
	[self forceUnfoldGroup];
}

- (void)notifyGroupHerderClicked: (NSNotification*)note
{
	id view = [note object];
	if (view == self)
		self.tintColor = [UIColor colorWithRed:210.0/255 green:234.0/255 blue:246.0/255 alpha:1.0];
	else 
		self.tintColor = [UIColor whiteColor];
}

- (void)setGroup:(GroupInfo*)group type:(BOOL)sortByStatus
{
	if (sortByStatus)
	{
		label.text = [title stringByAppendingFormat:@" ( %d )",[group.persons count]];	
	}
	else
	{
		PersonInfo *info = nil;
		int i = 0;
		for (info in group.persons)
		{
			if ([[info.state lowercaseString] compare:@"offline"] != NSOrderedSame)
				i++;
		}
		label.text = [title stringByAppendingFormat:@" ( %d / %d )",i,[group.persons count]];
	}
}

- (void)drawRect:(CGRect)rect 
{
	[super drawRect:rect];
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextTranslateCTM(context, 0, rect.size.height);
	CGContextSetGrayStrokeColor(context, 0.5, 1);
	CGContextStrokeRect(context,rect);
}

- (void)dealloc 
{
	LogMethod();
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver:self name:GroupHerderClickedNotification object:nil];
	[imageView release];
	[title release];
	[label release];
	target = nil;
	[super dealloc];
}

#pragma mark -
#pragma mark UIResponder methods.

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[[NSNotificationCenter defaultCenter] postNotificationName: GroupHerderClickedNotification object:self];

	moved = NO;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	moved = YES;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	LogMethod();
	if (!moved)
	{
		NSString *imagePath = nil;
		
		if (!closed)
		{
			imagePath = [[NSBundle mainBundle] pathForResource:@"group_close-png"
														ofType:@"png"];	
			closed = YES;
		}
		else
		{
			imagePath = [[NSBundle mainBundle] pathForResource:@"group_open-png"
														ofType:@"png"];	
			closed = NO;
		}
		imageView.image = [UIImage imageWithContentsOfFile:imagePath];
		[target performSelector:action withObject:self];
	}
}

@end
