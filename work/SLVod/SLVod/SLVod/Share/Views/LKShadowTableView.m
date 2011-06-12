//
//  LKShadowTableView.m
//  I58OS
//
//  Created by luke on 11-5-12.
//  Copyright 2011年 58.com. All rights reserved.
//

#import "LKShadowTableView.h"


@implementation LKShadowTableView
@synthesize customHitTest, showShadow;

#pragma makr -
#pragma mark shadow

#define SHADOW_HEIGHT 10.0
#define SHADOW_INVERSE_HEIGHT 8.0
#define SHADOW_RATIO (SHADOW_INVERSE_HEIGHT / SHADOW_HEIGHT+20)

- (BOOL)isLastSection {
	return NO;
}

- (id)init {
	if ((self = [super init])) {
		self.showShadow = YES;
	}
	return self;
}

- (id)initWithFrame:(CGRect)frame {
	if ((self = [super initWithFrame:frame])) {
		self.showShadow = YES;
	}
	return self;
}

- (id)initWithFrame:(CGRect)frame style:(UITableViewStyle)style {
	if ((self = [super initWithFrame:frame style:style])) {
		self.showShadow = YES;
	}
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
	if ((self = [super initWithCoder:aDecoder])) {
		self.showShadow = YES;
	}
	return self;
}
//
// shadowAsInverse:
//
// Create a shadow layer
//
// Parameters:
//    inverse - if YES then shadow fades upwards, otherwise shadow fades downwards
//
// returns the constructed shadow layer
//
- (CAGradientLayer *)shadowAsInverse:(BOOL)inverse
{
	CAGradientLayer *newShadow = [[[CAGradientLayer alloc] init] autorelease];
	CGRect newShadowFrame = CGRectMake(0, 0, self.frame.size.width,
									   inverse ? SHADOW_INVERSE_HEIGHT : SHADOW_HEIGHT);
	newShadow.frame = newShadowFrame;
	CGColorRef darkColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:
							inverse ? (SHADOW_INVERSE_HEIGHT / SHADOW_HEIGHT) * 0.5 : 0.5].CGColor;
	CGColorRef lightColor = [self.backgroundColor colorWithAlphaComponent:0.0].CGColor;
	newShadow.colors = [NSArray arrayWithObjects:
						(id)(inverse ? lightColor : darkColor),
						(id)(inverse ? darkColor : lightColor),
						nil];
	return newShadow;
}

//
// layoutSubviews
//
// Override to layout the shadows when cells are laid out.
//
- (void)layoutSubviews
{
	[super layoutSubviews];
	if (!showShadow) {
		return;
	}
	//
	// Construct the origin shadow if needed
	//
	if (!originShadow)
	{
		originShadow = [self shadowAsInverse:NO];
		[self.layer insertSublayer:originShadow atIndex:0];
	}
	else if (![[self.layer.sublayers objectAtIndex:0] isEqual:originShadow])
	{
		[self.layer insertSublayer:originShadow atIndex:0];
	}
	
	[CATransaction begin];
	[CATransaction setValue:(id)kCFBooleanTrue forKey:kCATransactionDisableActions];
	
	//
	// Stretch and place the origin shadow
	//
	CGRect originShadowFrame = originShadow.frame;
	originShadowFrame.size.width = self.frame.size.width;
	originShadowFrame.origin.y = self.contentOffset.y;
	originShadow.frame = originShadowFrame;
	
	[CATransaction commit];
	
	NSArray *indexPathsForVisibleRows = [self indexPathsForVisibleRows];
	if ([indexPathsForVisibleRows count] == 0)
	{
		[topShadow removeFromSuperlayer];
		[topShadow release];
		topShadow = nil;
		[bottomShadow removeFromSuperlayer];
		[bottomShadow release];
		bottomShadow = nil;
		return;
	}
	
	NSIndexPath *firstRow = [indexPathsForVisibleRows objectAtIndex:0];
	if ([firstRow section] == 0 && [firstRow row] == 0)
	{
		UIView *cell = [self cellForRowAtIndexPath:firstRow];
		if (!topShadow)
		{
			topShadow = [[self shadowAsInverse:YES] retain];
			[cell.layer insertSublayer:topShadow atIndex:0];
		}
		else if ([cell.layer.sublayers indexOfObjectIdenticalTo:topShadow] != 0)
		{
			[cell.layer insertSublayer:topShadow atIndex:0];
		}
		
		CGRect shadowFrame = topShadow.frame;
		shadowFrame.size.width = cell.frame.size.width;
		shadowFrame.origin.y = -SHADOW_INVERSE_HEIGHT;
		topShadow.frame = shadowFrame;
	}
	else
	{
		[topShadow removeFromSuperlayer];
		[topShadow release];
		topShadow = nil;
	}
	
	//	NSIndexPath *lastSection = [self inde
	NSIndexPath *lastRow = [indexPathsForVisibleRows lastObject];
//	DLog(@"lastRow: (%d,%d)", lastRow.section, lastRow.row);
//	DLog(@"numberofsections: %d", [self numberOfSections]);
//	DLog(@"%d rows in section %d", [self numberOfRowsInSection:[lastRow section]], [lastRow section]);
	if ([lastRow section] == [self numberOfSections] - 1
		&& [lastRow row] == [self numberOfRowsInSection:[lastRow section]] - 1)
	{
		UIView *cell = [self cellForRowAtIndexPath:lastRow];
		if (!bottomShadow)
		{
			bottomShadow = [[self shadowAsInverse:NO] retain];
			[cell.layer insertSublayer:bottomShadow atIndex:0];
		}
		else if ([cell.layer.sublayers indexOfObjectIdenticalTo:bottomShadow] != 0)
		{
			[cell.layer insertSublayer:bottomShadow atIndex:0];
		}
		
		CGRect shadowFrame = bottomShadow.frame;
		shadowFrame.size.width = cell.frame.size.width;
		shadowFrame.origin.y = cell.frame.size.height;
		bottomShadow.frame = shadowFrame;
	}
	//	else if ()
	//	{
	//		
	//	}
	else
	{
		[bottomShadow removeFromSuperlayer];
		[bottomShadow release];
		bottomShadow = nil;
	}
}


#pragma mark -
#pragma mark hit test
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event {
	if (customHitTest) {
		if (point.y < 0) {
			return YES;
		}
	}
	return YES;
}

@end
