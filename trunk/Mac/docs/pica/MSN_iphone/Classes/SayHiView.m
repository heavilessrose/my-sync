//
//  SayHiView.m
//  MSN
//
//  Created by Jun Du on 5/31/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "SayHiView.h"
#import <QuartzCore/QuartzCore.h>
#import "define.h"
#import "ClientNetWorkController.h"
#import "MSNAppDelegate.h"

static NSString *toLiveID = nil;
//static NSString *fromLiveID = nil;

@implementation SayHiView

#pragma mark -
#pragma mark utilities

-(void)buttonClicked:(id)sender
{
	UIButton *theButton = (UIButton *)sender;
	NSInteger theTag = theButton.tag;
	MSNAppDelegate *theApp = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	
	NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:theTag],@"tag",toLiveID,@"liveID",theApp.myAccount,@"fromLiveID",nil];

	[[ClientNetWorkController getInstance] sendHiPackage:infoDict];
	[self removeSayHiView];
}

#pragma mark -

//
// NewPathWithRoundRect
//
// Creates a CGPathRect with a round rect of the given radius.
//

CGPathRef NewPathWithRoundRect(CGRect rect, CGFloat cornerRadius)
{
	//
	// Create the boundary path
	//
	CGMutablePathRef path = CGPathCreateMutable();
	CGPathMoveToPoint(path, NULL,
					  rect.origin.x,
					  rect.origin.y + rect.size.height - cornerRadius);
	
	// Top left corner
	CGPathAddArcToPoint(path, NULL,
						rect.origin.x,
						rect.origin.y,
						rect.origin.x + rect.size.width,
						rect.origin.y,
						cornerRadius);
	
	// Top right corner
	CGPathAddArcToPoint(path, NULL,
						rect.origin.x + rect.size.width,
						rect.origin.y,
						rect.origin.x + rect.size.width,
						rect.origin.y + rect.size.height,
						cornerRadius);
	
	// Bottom right corner
	CGPathAddArcToPoint(path, NULL,
						rect.origin.x + rect.size.width,
						rect.origin.y + rect.size.height,
						rect.origin.x,
						rect.origin.y + rect.size.height,
						cornerRadius);
	
	// Bottom left corner
	CGPathAddArcToPoint(path, NULL,
						rect.origin.x,
						rect.origin.y + rect.size.height,
						rect.origin.x,
						rect.origin.y,
						cornerRadius);
	
	// Close the path at the rounded rect
	CGPathCloseSubpath(path);
	
	return path;
}

- (void)removeSayHiView
{
//	UIView *aSuperview = [self superview];
	[super removeFromSuperview];
	
/*	CATransition *animation = [CATransition animation];
	[animation setType:kCATransitionFade];
	
	[[aSuperview layer] addAnimation:animation forKey:@"layerAnimation"];
 */
}

+ (id)SayHiInView:(UIView *)aSuperview rect:(CGRect)rect liveID:(NSString *)liveID
{
	if (CGRectEqualToRect(CGRectZero, rect))
	{
		rect = [aSuperview bounds];
	}
	
	SayHiView *hiView = [[[SayHiView alloc] initWithFrame:rect] autorelease];
	if(!hiView)
	{
		return nil;
	}
	
	toLiveID = liveID;
//	fromLiveID = ;
	
	hiView.opaque = NO;
	hiView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	[aSuperview addSubview:hiView];
	
	UIButton *theButton = nil;
	NSString *theName = nil;
	const CGFloat DEFAULT_IMAGE_WIDTH = 40;
	const CGFloat DEFAULT_IMAGE_HEIGHT = 40;
	const CGFloat DEFAULT_GAP_LENGTH = 60;
	for (int i=1; i<=16; i++)
	{
		if (i<10)
		{
			theName = [NSString stringWithFormat:@"m0%d.png",i];
		}else {
			theName = [NSString stringWithFormat:@"m%d.png",i];
		}
		
		UIImage *theImage = [UIImage imageNamed:theName];
		theButton = [UIButton buttonWithType:UIButtonTypeCustom];
		[theButton setBackgroundImage:theImage forState:UIControlStateNormal];
		
		NSInteger iCol = i%4 -1;
		if (iCol == -1)
			iCol = 3;
		NSInteger iRow = (i-1)/4;
		
		theButton.frame = CGRectMake(20+(iCol*DEFAULT_GAP_LENGTH), 40+(iRow*DEFAULT_GAP_LENGTH), DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT);
		// For distinguish buttons.
		theButton.tag = i;
		[theButton addTarget:hiView action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
		[hiView addSubview:theButton];
	}
	
	theButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	theButton.frame = CGRectMake(90, 280, 100, 35);
	[theButton setTitle:NSLocalizedString(@"qtn_cancel",nil) forState:UIControlStateNormal];
	[theButton addTarget:hiView action:@selector(removeSayHiView) forControlEvents:UIControlEventTouchUpInside];
	[hiView addSubview:theButton];
	theButton = nil;

	// Set up the fade-in animation
/*	CATransition *animation = [CATransition animation];
	[animation setType:kCATransitionFade];
	[[aSuperview layer] addAnimation:animation forKey:@"layerAnimation"];
*/	
	return hiView;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	rect.size.height -= 1;
	rect.size.width -= 1;
	
	const CGFloat RECT_PADDING = 8.0;
	rect = CGRectInset(rect, RECT_PADDING, RECT_PADDING);
	
	const CGFloat ROUND_RECT_CORNER_RADIUS = 5.0;
	CGPathRef roundRectPath = NewPathWithRoundRect(rect, ROUND_RECT_CORNER_RADIUS);
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	const CGFloat BACKGROUND_OPACITY = 0.85;
	CGContextSetRGBFillColor(context, 0, 0, 0, BACKGROUND_OPACITY);
	CGContextAddPath(context, roundRectPath);
	CGContextFillPath(context);
	
	const CGFloat STROKE_OPACITY = 0.25;
	CGContextSetRGBStrokeColor(context, 1, 1, 1, STROKE_OPACITY);
	CGContextAddPath(context, roundRectPath);
	CGContextStrokePath(context);
	
	CGPathRelease(roundRectPath);
}


- (void)dealloc {
    [super dealloc];
}


@end
