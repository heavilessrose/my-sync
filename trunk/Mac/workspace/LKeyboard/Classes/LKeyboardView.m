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
		self.backgroundColor = [UIColor grayColor];
		self.board = [[UIScrollView alloc] initWithFrame:CGRectZero];
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
			x = (kPortrait_Emo_cube + kEmoGap) * col;
			y = (kPortrait_Emo_cube + kEmoGap) * row;
			w = kPortrait_Emo_cube;
			h = kPortrait_Emo_cube;
			break;
		case UIInterfaceOrientationLandscapeRight:
		case UIInterfaceOrientationLandscapeLeft:
			x = (kLandscap_Emo_cube + kEmoGap) * col;
			y = (kLandscap_Emo_cube + kEmoGap) * row;
			w = kLandscap_Emo_cube;
			h = kLandscap_Emo_cube;
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
	NSString	*aEmoImgName = nil;
	UIImage		*aEmoImg = nil;
	LEmoView	*aEmoView = nil;
	CGRect		aEmoViewFrame = CGRectZero;

	for (row = 0; row < kEmoCount_row; row++) {
		for (col = 0; col < kEmoCount_col; col++) {
			aEmoImgName = [NSString stringWithFormat:@"%d-png.png", row * kEmoCount_row + col + 1];
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
