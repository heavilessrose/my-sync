//
//  LKTextStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKTextStyle.h"
#import "LKStyleContext.h"
#import "UIFontAddition.h"


@implementation LKTextStyle

@synthesize font, color, shadowColor, shadowOffset, minimumFontSize;
@synthesize numberOfLines, textAlignment, verticalAlignment, lineBreakMode;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		shadowOffset = CGSizeZero;
		numberOfLines = 1;
		textAlignment = UITextAlignmentCenter;
		verticalAlignment = UIControlContentVerticalAlignmentCenter;
		lineBreakMode = UILineBreakModeTailTruncation;
	}
	
	return self;
}

- (void)dealloc {
	self.font = nil;
	self.color = nil;
	self.shadowColor = nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Class public

+ (LKTextStyle *)styleWithFont:(UIFont *)font next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.font = font;
	return style;
}

+ (LKTextStyle *)styleWithColor:(UIColor *)color next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.color = color;
	return style;
}

+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.font = font;
	style.color = color;
	return style;
}

+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
                textAlignment:(UITextAlignment)textAlignment next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.font = font;
	style.color = color;
	style.textAlignment = textAlignment;
	return style;
}

+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
                  shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset
                         next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.font = font;
	style.color = color;
	style.shadowColor = shadowColor;
	style.shadowOffset = shadowOffset;
	return style;
}

+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
              minimumFontSize:(CGFloat)minimumFontSize
                  shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset
                         next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.font = font;
	style.color = color;
	style.minimumFontSize = minimumFontSize;
	style.shadowColor = shadowColor;
	style.shadowOffset = shadowOffset;
	return style;
}

+ (LKTextStyle *)styleWithFont:(UIFont *)font color:(UIColor *)color
              minimumFontSize:(CGFloat)minimumFontSize
                  shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset
                textAlignment:(UITextAlignment)textAlignment
            verticalAlignment:(UIControlContentVerticalAlignment)verticalAlignment
                lineBreakMode:(UILineBreakMode)lineBreakMode numberOfLines:(NSInteger)numberOfLines
                         next:(LKStyle *)next {
	LKTextStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.font = font;
	style.color = color;
	style.minimumFontSize = minimumFontSize;
	style.shadowColor = shadowColor;
	style.shadowOffset = shadowOffset;
	style.textAlignment = textAlignment;
	style.verticalAlignment = verticalAlignment;
	style.lineBreakMode = lineBreakMode;
	style.numberOfLines = numberOfLines;
	return style;
}


#pragma mark -
#pragma mark Private

- (CGSize)sizeOfText:(NSString *)text withFont:(UIFont *)afont size:(CGSize)size {
	if (numberOfLines == 1) {
		return [text sizeWithFont:afont];
		
	} else {
		CGSize maxSize = CGSizeMake(size.width, CGFLOAT_MAX);
		CGSize textSize = [text sizeWithFont:afont constrainedToSize:maxSize
							   lineBreakMode:lineBreakMode];
		if (numberOfLines) {
			CGFloat maxHeight = afont.ttLineHeight*numberOfLines;
			if (textSize.height > maxHeight) {
				textSize.height = maxHeight;
			}
		}
		return textSize;
	}
}

- (CGRect)rectForText:(NSString *)text forSize:(CGSize)size withFont:(UIFont *)afont {
	CGRect rect = CGRectZero;
	if (textAlignment == UITextAlignmentLeft
		&& verticalAlignment == UIControlContentVerticalAlignmentTop) {
		rect.size = size;
	} else {
		CGSize textSize = [self sizeOfText:text withFont:afont size:size];
		
		if (size.width < textSize.width) {
			size.width = textSize.width;
		}
		
		rect.size = textSize;
		
		if (textAlignment == UITextAlignmentCenter) {
			rect.origin.x = round(size.width/2 - textSize.width/2);
		} else if (textAlignment == UITextAlignmentRight) {
			rect.origin.x = size.width - textSize.width;
		}
		
		if (verticalAlignment == UIControlContentVerticalAlignmentCenter) {
			rect.origin.y = round(size.height/2 - textSize.height/2);
		} else if (verticalAlignment == UIControlContentVerticalAlignmentBottom) {
			rect.origin.y = size.height - textSize.height;
		}
	}
	return rect;
}

- (void)drawText:(NSString *)text context:(LKStyleContext *)context {
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSaveGState(ctx);
	
	UIFont *aFont = font ? font : context.font;
	
	if (nil == aFont) {
		aFont = [UIFont systemFontOfSize:[UIFont systemFontSize]];
	}
	
	if (shadowColor) {
		CGSize offset = CGSizeMake(shadowOffset.width, -shadowOffset.height);
		CGContextSetShadowWithColor(ctx, offset, 0, shadowColor.CGColor);
	}
	
	if (color) {
		[color setFill];
	}
	
	CGRect rect = context.contentFrame;
	
	if (numberOfLines == 1) {
		CGRect titleRect = [self rectForText:text forSize:rect.size withFont:aFont];
		titleRect.size = [text drawAtPoint:
						  CGPointMake(titleRect.origin.x+rect.origin.x, titleRect.origin.y+rect.origin.y)
								  forWidth:rect.size.width withFont:aFont
							   minFontSize:minimumFontSize ? minimumFontSize : aFont.pointSize
							actualFontSize:nil lineBreakMode:lineBreakMode
						baselineAdjustment:UIBaselineAdjustmentAlignCenters];
		context.contentFrame = titleRect;
	} else {
		CGRect titleRect = [self rectForText:text forSize:rect.size withFont:aFont];
		titleRect = CGRectOffset(titleRect, rect.origin.x, rect.origin.y);
		rect.size = [text drawInRect:titleRect withFont:aFont lineBreakMode:lineBreakMode
						   alignment:textAlignment];
		context.contentFrame = rect;
	}
	
	CGContextRestoreGState(ctx);
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	if ([context.delegate respondsToSelector:@selector(textForLayerWithStyle:)]) {
		NSString *text = [context.delegate textForLayerWithStyle:self];
		if (text) {
			context.didDrawContent = YES;
			[self drawText:text context:context];
		}
	}
	
	if (!context.didDrawContent
		&& [context.delegate respondsToSelector:@selector(drawLayer:withStyle:)]) {
		[context.delegate drawLayer:context withStyle:self];
		context.didDrawContent = YES;
	}
	
	[self.next draw:context];
}

- (CGSize)addToSize:(CGSize)size context:(LKStyleContext *)context {
	if ([context.delegate respondsToSelector:@selector(textForLayerWithStyle:)]) {
		NSString *text = [context.delegate textForLayerWithStyle:self];
		UIFont *aFont = font ? font : context.font;
		
		CGFloat maxWidth = context.contentFrame.size.width;
		if (!maxWidth) {
			maxWidth = CGFLOAT_MAX;
		}
		CGFloat maxHeight = numberOfLines ? numberOfLines * aFont.ttLineHeight : CGFLOAT_MAX;
		CGSize maxSize = CGSizeMake(maxWidth, maxHeight);
		CGSize textSize = [self sizeOfText:text withFont:aFont size:maxSize];
		
		size.width += textSize.width;
		size.height += textSize.height;
	}
	
	if (next) {
		return [self.next addToSize:size context:context];
	} else {
		return size;
	}
}

@end
