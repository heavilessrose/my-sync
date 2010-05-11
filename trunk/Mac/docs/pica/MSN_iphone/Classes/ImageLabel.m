//
//  ImageLabel.m
//  MSN
//
//  Created by  apple on 08-10-31.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ImageLabel.h"
#import "MSNAppDelegate.h"


@implementation ImageLabel
@synthesize pressionImage, pressionStrings,currentString;

- (id)initWithFrame:(CGRect)frame 
{
	if (self = [super initWithFrame:frame])
	{
		[self setLabel];
		// Initialization code
	}
	return self;
}

- (void)setLabel
{
	findPressionString = NO;
	pressionIndex = -1;
	pressionImage = nil;
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	self.pressionStrings = appdelegate.pressionStrings;
}

- (void)drawRect:(CGRect)rect
{
	[super drawRect:rect];
	// Drawing code
}


- (void)findSring
{
	NSString *string = nil;
	NSArray *strings;
	NSUInteger index = NSNotFound;
	int imageIndex = 0;
	NSString *findedString;
	findPressionString = NO;
	int i = 0;
	for (string in pressionStrings)
	{
		strings = [string componentsSeparatedByString:@","];
		i++;
		for (string in strings)
		{
			NSRange range =  [self.text rangeOfString:string];
			if (range.location != NSNotFound)
			{
				int j = range.location + [string length];
				if (j < [self.text length])
				{
					NSString * subString = [self.text substringFromIndex:j];
					NSRange rangeAfter =  [subString rangeOfString:string];
					if (rangeAfter.location != NSNotFound)
						findPressionString = YES;
				}
				if (index != NSNotFound)
					findPressionString = YES;
				
				if (index > range.location)
				{
					index = range.location;	
					imageIndex = i;
					findedRange = range;
					findedString = string;
				}
			}
		}
	}
	if (imageIndex > 0)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%d-png", imageIndex]
															  ofType:@"png"];
		self.pressionImage = [UIImage imageWithContentsOfFile:imagePath];
		self.currentString = [self.text substringToIndex:findedRange.location];
		self.text = [self.text substringFromIndex:findedRange.location + findedRange.length];
	}
	else
		self.pressionImage = nil;
}


- (void)drawTextInRect:(CGRect)rect
{
	[self.textColor set];
	if (self.text != nil && [self.text length] > 0)
	{
		BOOL hasImage = NO;
		BOOL first = YES;
		findPressionString = YES;
		NSString *str = @".";
		CGSize  tailSize = [str sizeWithFont:self.font constrainedToSize:rect.size lineBreakMode:UILineBreakModeWordWrap];
		CGSize textSize = rect.size;
		textSize.height = 1000;
		NSString *subString = nil;
		NSString *lastString = nil;
		NSString *lineString = nil;
		NSArray *lineStrings = nil;
		CGPoint currentPoint = CGPointZero;
		
		while (findPressionString)
		{
			[self findSring];
			if (pressionImage != nil)
			{		
				hasImage = YES;
				if (currentString != nil && [currentString length] > 0)
				{
					lineStrings = [currentString componentsSeparatedByString:@"\n"];
					if ([lineStrings count] > 0)
					{
						int i = 0;
						first = YES;
						for (lineString in lineStrings)
						{
							if (!first)
							{
								currentPoint.x = 0;
								currentPoint.y += tailSize.height;
								textSize.width = rect.size.width;
							}
							first = NO;
							subString = nil;
							lastString = nil;
							for (i = 1; i <= [lineString length]; i++)
							{
								subString = [lineString substringToIndex:i];
								int index = [self indexOfString:subString forWidth:textSize.width lineHeight:tailSize.height];
								if (index > -1)
								{
									lastString = [lineString substringToIndex:index];
									if (lastString != nil && [lastString length] > 0)
									{
										if (currentPoint.y + tailSize.height*2 <= rect.size.height)
											[lastString drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:UILineBreakModeWordWrap];
										else //if(currentPoint.y + tailSize.height <= rect.size.height)
											[subString drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:self.lineBreakMode];
										
										textSize.width = rect.size.width;
										currentPoint.x = 0;
										currentPoint.y += tailSize.height;
									}
									lastString = [lineString substringFromIndex:index];
									if ([lastString isEqualToString: lineString]){
										break;
									}
									
									lineString = [lineString substringFromIndex:index];
									i = index;
								}
								else {
									lastString = subString;
								}
							}
							if (lastString != nil && [lastString length] > 0)
							{
								if (currentPoint.y + tailSize.height*2 <= rect.size.height)
									[lastString drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:UILineBreakModeWordWrap];
								else //if(currentPoint.y + tailSize.height <= rect.size.height)
									[lastString drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:self.lineBreakMode];
								
								currentPoint.x += [lastString sizeWithFont:self.font constrainedToSize:textSize lineBreakMode:UILineBreakModeWordWrap].width;
								textSize.width = rect.size.width - currentPoint.x;
							}
							
						}
					}
				}
				
				if (currentPoint.x + self.font.pointSize > rect.size.width
					&& currentPoint.y + tailSize.height*2 < rect.size.height)
				{
					currentPoint.x = 0;
					currentPoint.y += tailSize.height;
				}
	
				int l = (tailSize.height - self.font.pointSize)/2;
				CGRect rc = CGRectMake(currentPoint.x, currentPoint.y + l, self.font.pointSize, self.font.pointSize);
				
				if (CGRectContainsRect(rect,rc))
				{
					if ( self.lineBreakMode != UILineBreakModeTailTruncation 
						|| rc.origin.x + rc.size.width * 2 < rect.size.width)
					{
						[pressionImage drawInRect:rc];
						if (rc.origin.x + rc.size.width * 2 < rect.size.width)
						{
							currentPoint.x += self.font.pointSize;
						}
						else
						{
							currentPoint.x = 0;
							currentPoint.y += tailSize.height;
						}
					}
					else
					{
						[@"         " drawInRect:rc withFont:self.font lineBreakMode:UILineBreakModeTailTruncation];
						currentPoint.x = 0;
						currentPoint.y += tailSize.height;
					}

					textSize.width = rect.size.width - currentPoint.x;	
				}
				else
				{
					break;	
				}
			}
		}	
		
		if (!hasImage)
		{
			[self.text drawInRect:rect withFont:self.font lineBreakMode:self.lineBreakMode];	
		}
		else 
		{
			CGRect rc = CGRectZero;
			if (self.text != nil && [self.text length] > 0)
			{
				int index = [self indexOfString:self.text forWidth:textSize.width lineHeight:tailSize.height];
				if (index > -1)
				{
					lastString = [self.text substringToIndex:index];
					if (lastString != nil && [lastString length] > 0)
					{
						if (currentPoint.y + tailSize.height*2 <= rect.size.height)
						{
							[lastString drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:UILineBreakModeWordWrap];	
							rc = CGRectMake(0, currentPoint.y + tailSize.height, rect.size.width, rect.size.height - currentPoint.y - tailSize.height);
						}
						else //if(currentPoint.y + tailSize.height <= rect.size.height)
						{
							[self.text drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:self.lineBreakMode];
							rc.origin.y = rect.size.height;
						}
					}
					lastString = [self.text substringFromIndex:index];
					if (rc.origin.y + rc.size.height <= rect.size.height)
						[lastString drawInRect:rc withFont:self.font lineBreakMode:self.lineBreakMode];
				}
				else
				{
					if (currentPoint.y + tailSize.height <= rect.size.height)
						[self.text drawAtPoint:currentPoint forWidth:textSize.width withFont:self.font lineBreakMode:self.lineBreakMode];
				}
			}
		}
	}
}

- (int)indexOfString:(NSString*)theString forWidth:(float)theWidth lineHeight:(float)theLineHeight
{
	int findIndex = -1;
	NSString *findString = nil;
	CGSize textSize = CGSizeMake(theWidth, 1000);
	CGSize size = [theString sizeWithFont:self.font constrainedToSize:textSize lineBreakMode:UILineBreakModeWordWrap];
	if (size.height > theLineHeight)
	{
		findString = [theString stringByAppendingString:@"."];
		float width = [findString sizeWithFont:self.font constrainedToSize:textSize lineBreakMode:UILineBreakModeWordWrap].width;
		int j = 0;
		if (size.width == width)
		{
			for (j = 1; j < [theString length]; j++)
			{
				findString = [theString substringToIndex:j];
				width = [findString sizeWithFont:self.font forWidth:320 lineBreakMode:UILineBreakModeWordWrap].width;
				if (width >= size.width)
				{
					findIndex = j - 1;
					break;
				}
			}
		}
		else
		{
			for (j = [theString length] - 1; j > 0; j--)
			{
				findString = [theString substringFromIndex:j];
				if ([findString sizeWithFont:self.font forWidth:320 lineBreakMode:UILineBreakModeWordWrap].width <= size.width)
				{
					findIndex = j - 1;
					break;
				}
			}
		}	
	}
	return findIndex;
}
- (void)dealloc 
{
	[pressionStrings release];
	[pressionImage release];
	[currentString release];
	[super dealloc];
}


@end
