//
//  QuartzFunView.m
//  QuartzFun
//
//  Created by Jeff LaMarche on 7/31/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "QuartzFunView.h"
#import "UIColor-Random.h"

@implementation QuartzFunView
@synthesize firstTouch;
@synthesize lastTouch;
@synthesize currentColor;
@synthesize shapeType;
@synthesize drawImage;
@synthesize redrawRect;
@dynamic currentRect;
@dynamic redrawRect;
@synthesize useRandomColor;

// This version is a little different that the code in the book. This
// version is more efficient, but the change was made too late to get
// it into the book. In the book, we loaded a UIImage every time we 
// needed to draw it, which is inefficient. In this version, we use 
// an instance variable to store the UIImage while the program is
// running, and use that image to draw
- (id)initWithCoder:(NSCoder*)coder
{
	if((self = [super initWithCoder:coder])) {
		self.currentColor = [UIColor redColor];
		self.useRandomColor = NO;
		if (drawImage == nil)
			self.drawImage = [UIImage imageNamed:@"iphone.png"];
	}
	return self;
}

- (CGRect)currentRect {
	return CGRectMake (
						(firstTouch.x > lastTouch.x) ? lastTouch.x : firstTouch.x,
						(firstTouch.y > lastTouch.y) ? lastTouch.y : firstTouch.y,
						fabsf(firstTouch.x - lastTouch.x),
						fabsf(firstTouch.y - lastTouch.y));
}

- (void)drawRect:(CGRect)rect {
	
	
	CGContextRef context = UIGraphicsGetCurrentContext();

	CGContextSetLineWidth(context, 2.0);
	CGContextSetStrokeColorWithColor(context, currentColor.CGColor);

	CGContextSetFillColorWithColor(context, currentColor.CGColor);
		
	switch (shapeType) {
		case kLineShape:
			CGContextMoveToPoint(context, firstTouch.x, firstTouch.y);
			CGContextAddLineToPoint(context, lastTouch.x, lastTouch.y);
			CGContextStrokePath(context);
			break;
		case kRectShape:
			CGContextAddRect(context, self.currentRect);
			CGContextDrawPath(context, kCGPathFillStroke);
			break;
		case kEllipseShape:
			CGContextAddEllipseInRect(context, self.currentRect);
			CGContextDrawPath(context, kCGPathFillStroke);
			break;
		case kImageShape: {
			CGFloat horizontalOffset = drawImage.size.width / 2;
			CGFloat verticalOffset = drawImage.size.height / 2;
			CGPoint drawPoint = CGPointMake(lastTouch.x - horizontalOffset,
											lastTouch.y - verticalOffset);
			[drawImage drawAtPoint:drawPoint];
			break;			
		}
		default:
			break;
	}

}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	if (useRandomColor)
		self.currentColor = [UIColor randomColor];
	
	UITouch *touch = [touches anyObject];
	firstTouch = [touch locationInView:self];
	lastTouch = [touch locationInView:self];
	
	if (shapeType == kImageShape) {
		CGFloat horizontalOffset = drawImage.size.width / 2;
		CGFloat verticalOffset = drawImage.size.height / 2;
		redrawRect = CGRectMake(firstTouch.x - horizontalOffset, firstTouch.y - verticalOffset, drawImage.size.width, drawImage.size.height);
	}
	else
		redrawRect = CGRectMake(firstTouch.x, firstTouch.y, 0, 0);
	[self setNeedsDisplay];
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject];
	lastTouch = [touch locationInView:self];
	
	if (shapeType == kImageShape) {
		CGFloat horizontalOffset = drawImage.size.width / 2;
		CGFloat verticalOffset = drawImage.size.height / 2;
		redrawRect = CGRectUnion(redrawRect, CGRectMake(lastTouch.x - horizontalOffset, lastTouch.y - verticalOffset, drawImage.size.width, drawImage.size.height));
	}
	else
		redrawRect = CGRectUnion(redrawRect, self.currentRect);

	[self setNeedsDisplayInRect:redrawRect];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject];
	lastTouch = [touch locationInView:self];
	
	if (shapeType == kImageShape) {
		CGFloat horizontalOffset = drawImage.size.width / 2;
		CGFloat verticalOffset = drawImage.size.height / 2;
		redrawRect = CGRectUnion(redrawRect, CGRectMake(lastTouch.x - horizontalOffset, lastTouch.y - verticalOffset, drawImage.size.width, drawImage.size.height));
	}
	redrawRect = CGRectUnion(redrawRect, self.currentRect);

	[self setNeedsDisplayInRect:redrawRect];
	
}
- (void)dealloc {
	[currentColor release];
	[drawImage release];
	[super dealloc];
}

@end
