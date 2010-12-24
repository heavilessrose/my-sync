//
//  LKShadowStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKShadowStyle.h"
#import "LKStyleContext.h"
#import "LKRects.h"
#import "NSStringAddition.h"
#import "LKShape.h"


@implementation LKShadowStyle

@synthesize color, blur, offset;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		offset = CGSizeZero;
	}
	
	return self;
}

- (void)dealloc {
	self.color = nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Class public

+ (LKShadowStyle *)styleWithColor:(UIColor*)color blur:(CGFloat)blur offset:(CGSize)offset
                            next:(LKStyle *)next {
	LKShadowStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.color = color;
	style.blur = blur;
	style.offset = offset;
	return style;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	CGFloat blurSize = round(blur / 2);
	UIEdgeInsets inset = UIEdgeInsetsMake(blurSize, blurSize, blurSize, blurSize);
	if (offset.width < 0) {
		inset.left += fabs(offset.width) + blurSize*2;
		inset.right -= blurSize;
	} else if (offset.width > 0) {
		inset.right += fabs(offset.width) + blurSize*2;
		inset.left -= blurSize;
	}
	if (offset.height < 0) {
		inset.top += fabs(offset.height) + blurSize*2;
		inset.bottom -= blurSize;
	} else if (offset.height > 0) {
		inset.bottom += fabs(offset.height) + blurSize*2;
		inset.top -= blurSize;
	}
	
	context.frame = LKRectInset(context.frame, inset);
	context.contentFrame = LKRectInset(context.contentFrame, inset);
	
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSaveGState(ctx);
	
	// Due to a bug in OS versions 3.2 and 4.0, the shadow appears upside-down. It pains me to
	// write this, but a lot of research has failed to turn up a way to detect the flipped shadow
	// programmatically
	float shadowYOffset = -offset.height;
	NSString *osVersion = [UIDevice currentDevice].systemVersion;
	if ([osVersion versionStringCompare:@"3.2"] != NSOrderedAscending) {
		shadowYOffset = offset.height;
	}
	
	[context.shape addToPath:context.frame];
	CGContextSetShadowWithColor(ctx, CGSizeMake(offset.width, shadowYOffset), blur,
								color.CGColor);
	CGContextBeginTransparencyLayer(ctx, nil);
	[self.next draw:context];
	CGContextEndTransparencyLayer(ctx);
	
	CGContextRestoreGState(ctx);
}

- (CGSize)addToSize:(CGSize)size context:(LKStyleContext *)context {
	CGFloat blurSize = round(blur / 2);
	size.width += offset.width + (offset.width ? blurSize : 0) + blurSize*2;
	size.height += offset.height + (offset.height ? blurSize : 0) + blurSize*2;
	
	if (next) {
		return [self.next addToSize:size context:context];
	} else {
		return size;
	}
}

@end
