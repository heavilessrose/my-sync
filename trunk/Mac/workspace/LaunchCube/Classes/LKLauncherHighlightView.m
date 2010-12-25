//
//  LKLauncherHighlightView.m
//  LaunchCube
//
//  Created by luke on 10-12-25.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKLauncherHighlightView.h"
#import "LKLauncherButton.h"


static const CGFloat kHighlightOverlayAlpha = 0.7f;
static const CGFloat kSpotlightScaleFactor = 2.5f;
static const CGFloat kSpotlightBlurInnerAlpha = 0.43f;
static const CGFloat kSpotlightBlurOuterAlpha = 0.7f;  // same as kHighlightOverlayAlpha; grr const
static const CGFloat kSpotlightBlurRadius = 120.0f;
static const CGFloat kHighlightTextPadding = 20.0f;


@implementation LKLauncherHighlightView

@synthesize highlightRect, parentView, text;

- (id)initWithFrame:(CGRect)frame {
	if ((self = [super initWithFrame:frame])) {
		self.alpha = 0.0;
		
		textLabel = [[UILabel alloc] init];
		textLabel.lineBreakMode = UILineBreakModeWordWrap;
		textLabel.numberOfLines = 0;
		textLabel.backgroundColor = [UIColor clearColor];
		textLabel.textAlignment = UITextAlignmentCenter;
		textLabel.textColor = [UIColor whiteColor];
		textLabel.font = [UIFont boldSystemFontOfSize:17.0f];
		textLabel.shadowColor = [UIColor blackColor];
		textLabel.shadowOffset = CGSizeMake(1, 1);
		
		[self addSubview:textLabel];
		
		CGRect coverFrame = [UIApplication sharedApplication].statusBarFrame;
		statusBarCover = [[UIWindow alloc] initWithFrame:coverFrame];
		statusBarCover.backgroundColor = [UIColor colorWithWhite:0.0 alpha:kHighlightOverlayAlpha];
		statusBarCover.windowLevel = UIWindowLevelStatusBar;
		statusBarCover.alpha = 0.0;
		statusBarCover.hidden = NO;
	}
	
	return self;
}

- (void)dealloc {
	
	[textLabel release];
	textLabel = nil;
	[statusBarCover release];
	statusBarCover = nil;
	self.parentView = nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Private

- (void)animateTextIn {
	textLabel.alpha = 0.0;
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.2];
	textLabel.alpha = 1.0;
	[UIView commitAnimations];
}

- (void)layoutLabel {
	CGSize superviewSize = self.superview.bounds.size;
	CGFloat width = superviewSize.width - 2 * kHighlightTextPadding;
	CGFloat height = [textLabel.text sizeWithFont:textLabel.font
								 constrainedToSize:CGSizeMake(width, superviewSize.height)].height;
	
	// If the highlighted rect is above center, put the text below it; otherwise, above it.
	CGFloat y = 0.0;
	if (highlightRect.origin.y + (highlightRect.size.height / 2) < superviewSize.height / 2) {
		y = highlightRect.origin.y + highlightRect.size.height + kHighlightTextPadding;
	} else {
		y = highlightRect.origin.y - height - kHighlightTextPadding;
	}
	
	textLabel.frame = CGRectMake(kHighlightTextPadding, y, width, height);
}

#pragma mark -
#pragma mark Public

- (void)setHighlightRect:(CGRect)rect {
	highlightRect = rect;
	[self layoutLabel];
}

- (NSString*)text {
	return textLabel.text;
}

- (void)setText:(NSString *)atext {
	textLabel.text = atext;
	[self layoutLabel];
}

- (void)setAlpha:(CGFloat)alpha {
	[super setAlpha:alpha];
	statusBarCover.alpha = alpha;
}

- (void)appear:(BOOL)animated {
	// The expanded frame needs to be 3.5 times the original size, and the expansion needs to emanate
	// from the same center as the highlighted rect
	CGRect expandedFrame = self.superview.bounds;
	expandedFrame.origin.x = -(highlightRect.origin.x + highlightRect.size.width / 2)
	* (kSpotlightScaleFactor - 1);
	expandedFrame.origin.y = -(highlightRect.origin.y + highlightRect.size.height / 2)
	* (kSpotlightScaleFactor - 1);
	expandedFrame.size.width *= kSpotlightScaleFactor;
	expandedFrame.size.height *= kSpotlightScaleFactor;
	self.frame = expandedFrame;
	self.alpha = 0.0;
	textLabel.alpha = 0.0;
	
	if (animated) {
		[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:0.3];
		[UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
		[UIView setAnimationDelegate:self];
		[UIView setAnimationDidStopSelector:@selector(animateTextIn)];
	}
	self.alpha = 1.0;
	self.frame = self.superview.bounds;
	statusBarCover.alpha = 1.0;
	
	if (animated) {
		[UIView commitAnimations];
	} else {
		textLabel.alpha = 1.0;
	}
}

- (BOOL)isOpaque {
	return NO;
}

- (void)drawRect:(CGRect)rect {
	[[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:kHighlightOverlayAlpha] set];
	UIRectFillUsingBlendMode(rect, kCGBlendModeOverlay);
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	
	// Draw the spotlight (more of a Fresnel lens effect but okay I'll stop now)
	const CGFloat components[] = {0, 0, 0, kSpotlightBlurInnerAlpha,
		0, 0, 0, kSpotlightBlurOuterAlpha};
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGGradientRef gradient = CGGradientCreateWithColorComponents(colorSpace, components, NULL, 2);
	CGPoint center = CGPointMake(floor(highlightRect.origin.x + highlightRect.size.width / 2),
								 floor(highlightRect.origin.y + highlightRect.size.height / 2));
	CGFloat startRadius = MIN(highlightRect.size.width, highlightRect.size.height) / 2;
	CGFloat endRadius = startRadius + kSpotlightBlurRadius;
	CGContextDrawRadialGradient(context,
								gradient,
								center, startRadius,
								center, endRadius,
								kCGGradientDrawsAfterEndLocation);
	CGColorSpaceRelease(colorSpace);
	CGGradientRelease(gradient);
	
	// Draw the clear circle
	CGContextSetRGBFillColor(context, 0, 0, 0, 0);
	CGContextFillEllipseInRect(context, highlightRect);
}

/**
 * Overridden so that -hitTest:withEvent: doesn't catch this subview. We can distinguish a user-
 * initiated tap from a programmatic hit test by event being nil in the programmatic case.
 */
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent*)event {
	if (event) {
		return [super pointInside:point withEvent:event];
	}
	
	return NO;
}


#pragma mark -
#pragma mark UIResponder

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent*)event {
	UITouch* touch = [touches anyObject];
	
	// If the user tapped within the highlighted rect, we want to pass the event through to the
	// launcher button.
	if (CGRectContainsPoint(highlightRect, [touch locationInView:self])) {
		UIView *targetButton = [self.superview hitTest:[touch locationInView:self] withEvent:nil];
		
		// This condition should always be true, but just to be safe
		if ([targetButton isKindOfClass:[LKLauncherButton class]]) {
			[(LKLauncherButton *)targetButton sendActionsForControlEvents:UIControlEventTouchUpInside];
		}
	}
	
	// This will take care of removing the view
	[parentView endHighlightItem:nil];
}

@end
