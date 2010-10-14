//
//  LKeyboardView.m
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKeyboardView.h"
#import "LEmoView.h"
#import "LKeyboardAppDelegate.h"

@interface LKeyboardView ()

@property (nonatomic, retain) UIScrollView *board;

- (CGRect)calcEachEmoRectWithRow:(NSInteger)row andCol:(NSInteger)col;
- (void)initEmos;
@end



@implementation LKeyboardView
@synthesize board;

#pragma mark -
#pragma mark lifecycle

- (id)initWithFrame:(CGRect)frame
{
    if ((self = [super initWithFrame:frame])) {
		self.board = [[UIScrollView alloc] initWithFrame:CGRectZero];
		self.board.backgroundColor = [UIColor grayColor];
		[self initEmos];
		[self addSubview:self.board];
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)dealloc
{
	[board release];
	
    [super dealloc];
}

#pragma mark -
#pragma mark Emo

void rectLog(CGRect aRect)
{
	NSLog(@"(%f, %f, %f, %f)", aRect.origin.x, aRect.origin.y, aRect.size.width, aRect.size.height);
}

- (CGRect)calcEachEmoRectWithRow:(NSInteger)row andCol:(NSInteger)col
{
	CGRect rect = CGRectZero;
	CGFloat x = 0;
	CGFloat y = 0;
	CGFloat w = 0;
	CGFloat h = 0;
	switch ([UIDevice currentDevice].orientation) {
		case UIInterfaceOrientationPortrait:
		case UIInterfaceOrientationPortraitUpsideDown:
			x = (kPortrait_Emo_cube_w + kEmoGap_Portrait_col) * col;
			y = (kPortrait_Emo_cube_h + kEmoGap_Portrait_row) * row;
			w = kPortrait_Emo_cube_w;
			h = kPortrait_Emo_cube_h;
			break;
		case UIInterfaceOrientationLandscapeRight:
		case UIInterfaceOrientationLandscapeLeft:
			x = (kLandscap_Emo_cube_w + kEmoGap_Landscap_col) * col;
			y = (kLandscap_Emo_cube_h + kEmoGap_Landscap_row) * row;
			w = kLandscap_Emo_cube_w;
			h = kLandscap_Emo_cube_h;
			break;
		default:
			NSLog(@"curOrientation invailide");
			break;
	}
	
	rect = CGRectMake(x, y, w, h);
	return rect;
}

- (void)initEmos
{
	NSInteger	row = 0;
	NSInteger	col = 0;
	NSInteger	rowCount = 0;
	NSInteger	colCount = 0;
	NSString	*aEmoImgName = nil;
	UIImage		*aEmoImg = nil;
	LEmoView	*aEmoView = nil;
	CGRect		aEmoViewFrame = CGRectZero;
	
	switch ([UIDevice currentDevice].orientation) {
		case UIInterfaceOrientationPortrait:
		case UIInterfaceOrientationPortraitUpsideDown:
			rowCount = kEmoCount_Portrait_row;
			colCount = kEmoCount_Portrait_col;
			break;
		case UIInterfaceOrientationLandscapeRight:
		case UIInterfaceOrientationLandscapeLeft:
			rowCount = kEmoCount_Landscap_row;
			colCount = kEmoCount_Landscap_col;
			break;
		default:
			NSLog(@"curOrientation invailide");
			break;
	}
	for (row = 0; row < rowCount; row++) {
		for (col = 0; col < colCount; col++) {
			aEmoImgName = [NSString stringWithFormat:@"%d-png.png", row * colCount + col + 1];
			aEmoImg = [UIImage imageNamed:aEmoImgName];
			aEmoViewFrame = [self calcEachEmoRectWithRow:row andCol:col];
			//rectLog(aEmoViewFrame);
			aEmoView = [[LEmoView alloc] initWithFrame:aEmoViewFrame emoImg:aEmoImg];
			aEmoView.emoRow = row;
			aEmoView.emoCol = col;
			[self.board addSubview:aEmoView];
			[aEmoView release];
		}
	}
}

- (void)layoutSubviews
{
	[super layoutSubviews];
	switch ([UIDevice currentDevice].orientation) {
		case UIInterfaceOrientationPortrait:
		case UIInterfaceOrientationPortraitUpsideDown:
			[self.board setFrame:kPortraitRect];
			break;
		case UIInterfaceOrientationLandscapeRight:
		case UIInterfaceOrientationLandscapeLeft:
			[self.board setFrame:kLandscapRect];
			break;
		default:
			NSLog(@"curOrientation invailide");
			break;
	}
}

@end
