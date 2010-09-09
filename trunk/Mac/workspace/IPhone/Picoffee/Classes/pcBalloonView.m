//
//  pcBalloonView.m
//  imageView
//
//  Created by  apple on 10-5-18.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "pcBalloonView.h"

#define TEXT_TOP_INTERVAL           6
#define TEXT_LEFT_INTERVAL          18
#define LABEL_MAX_LINES             20
#define DEFAULT_BALLOON_MIN_WIDTH   200

@interface pcBalloonView ()

@property (nonatomic, retain) UIImageView	*leftTopView;
@property (nonatomic, retain) UIImageView	*topView;
@property (nonatomic, retain) UIImageView	*rightTopView;
@property (nonatomic, retain) UIImageView	*rightView;
@property (nonatomic, retain) UIImageView	*rightBottomView;
@property (nonatomic, retain) UIImageView	*bottomView;
@property (nonatomic, retain) UIImageView	*leftBottomView;
@property (nonatomic, retain) UIImageView	*leftView;
@property (nonatomic, retain) UIImageView	*centerView;
@property (nonatomic, retain) UILabel		*label;
@property (nonatomic, retain) UILabel		*label2;


- (void)relayout;
@end




@implementation pcBalloonView

@synthesize leftTopView, topView, rightTopView, rightView, rightBottomView, bottomView, 
leftBottomView, leftView, centerView, label, label2;



- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self.backgroundColor	= [UIColor clearColor];
        self.leftTopView		= [[UIImageView alloc] initWithFrame: CGRectZero];
        leftTopView.image       = [UIImage imageNamed: @"Balloon_S_01.png"];
        [self addSubview:leftTopView];
		[leftTopView release];
		
        self.topView			= [[UIImageView alloc] initWithFrame: CGRectZero];
        topView.contentMode     = UIViewContentModeScaleToFill;
        topView.image           = [UIImage imageNamed: @"Balloon_S_02.png"];
        [self addSubview:topView];
		[topView release];
		
        self.rightTopView		= [[UIImageView alloc] initWithFrame: CGRectZero];
        rightTopView.image      = [UIImage imageNamed: @"Balloon_S_03.png"];
        [self addSubview:rightTopView];
		[rightTopView release];
        
		self.rightView			= [[UIImageView alloc] initWithFrame: CGRectZero];
        rightView.image         = [UIImage imageNamed: @"Balloon_S_06.png"];
        rightView.contentMode   = UIViewContentModeScaleToFill;
        [self addSubview:rightView];
		[rightView release];
		
        self.rightBottomView	= [[UIImageView alloc] initWithFrame: CGRectZero];
        rightBottomView.image   = [UIImage imageNamed: @"Balloon_S_09.png"];
        [self addSubview: rightBottomView];
		[rightBottomView release];
		
        self.bottomView			= [[UIImageView alloc] initWithFrame: CGRectZero];
        bottomView.image        = [UIImage imageNamed: @"Balloon_S_08.png"];
        bottomView.contentMode  = UIViewContentModeScaleToFill;
        [self addSubview: bottomView];
		[bottomView release];
		
        self.leftBottomView		= [[UIImageView alloc] initWithFrame: CGRectZero];
        leftBottomView.image    = [UIImage imageNamed: @"Balloon_S_07.png"];
        [self addSubview: leftBottomView];
		[leftBottomView release];
		
        self.centerView			= [[UIImageView alloc] initWithFrame: CGRectZero];
        centerView.image        = [UIImage imageNamed: @"Balloon_S_05.png"];
        centerView.contentMode  = UIViewContentModeScaleToFill;
        [self addSubview: centerView];
		[centerView release];
		
        self.leftView			= [[UIImageView alloc] initWithFrame: CGRectZero];
        leftView.image          = [UIImage imageNamed: @"Balloon_S_04.png"];
        leftView.contentMode    = UIViewContentModeScaleToFill;
        [self addSubview: leftView];
		[leftView release];
        
        self.label				= [[UILabel alloc] initWithFrame: CGRectZero];
        label.backgroundColor   = [UIColor clearColor];
        label.numberOfLines     = LABEL_MAX_LINES;
        [self addSubview: label];
		[label release];
		
		self.label2				= [[UILabel alloc] initWithFrame: CGRectZero];
        label2.backgroundColor	= [UIColor clearColor];
		label2.font	= [UIFont systemFontOfSize: 10];
        label2.numberOfLines	= LABEL_MAX_LINES;
        [self addSubview: label2];
		[label2 release];
    }
    return self;
}

- (void)relayout
{
	CGRect rect = self.bounds;
    CGRect rcLeftTop = CGRectZero;
    rcLeftTop.size = [leftTopView.image size];
    leftTopView.frame = rcLeftTop;
    CGRect rcRightTop= CGRectZero;
    rcRightTop.size = [rightTopView.image size];
    rcRightTop.origin.x = rect.size.width - rcRightTop.size.width;
    rightTopView.frame = rcRightTop;
    CGRect rcTop = CGRectZero;
    rcTop.origin.x = rcLeftTop.size.width;
    rcTop.size.width = rcRightTop.origin.x - rcTop.origin.x;
    rcTop.size.height = [topView.image size].height;
    topView.frame = rcTop;
    CGRect rcRightBottom = CGRectZero;
    rcRightBottom.size = [rightBottomView.image size];
    rcRightBottom.origin.x = rcRightTop.origin.x;
    rcRightBottom.origin.y = rect.size.height - rcRightBottom.size.height;
    rightBottomView.frame = rcRightBottom;
    CGRect rcRight = CGRectZero;
    rcRight.size.width = [rightView.image size].width;
    rcRight.size.height= rcRightBottom.origin.y - rcRightTop.size.height;
    rcRight.origin.x   = rcRightTop.origin.x;
    rcRight.origin.y   = rcRightTop.size.height;
    rightView.frame = rcRight;
    CGRect rcLeftBottom = CGRectZero;
    rcLeftBottom.size = [leftBottomView.image size];
    rcLeftBottom.origin.y = rcRightBottom.origin.y;
    leftBottomView.frame = rcLeftBottom;
    CGRect rcBottom = CGRectZero;
    rcBottom.size.height = [bottomView.image size].height;
    rcBottom.size.width  = rcRightBottom.origin.x - rcLeftBottom.size.width;
    rcBottom.origin.x    = rcLeftBottom.size.width;
    rcBottom.origin.y    = rcLeftBottom.origin.y;
    bottomView.frame     = rcBottom;
    CGRect rcLeft = CGRectZero;
    rcLeft.size.width    = [leftView.image size].width;
    rcLeft.size.height   = rcLeftBottom.origin.y - rcLeftTop.size.height;
    rcLeft.origin.y      = rcLeftTop.size.height;
    leftView.frame       = rcLeft;
    CGRect rcCenter      = CGRectZero;
    rcCenter.origin.x    = rcLeftTop.size.width;
    rcCenter.origin.y    = rcLeftTop.size.height;
    rcCenter.size.width  = rcRightTop.origin.x - rcCenter.origin.x;
    rcCenter.size.height = rcRightBottom.origin.y - rcCenter.origin.y;
    centerView.frame     = rcCenter;
    
    CGRect rcLabel       = rcCenter;
    rcLabel.origin.y     = TEXT_TOP_INTERVAL;
	
	rcLabel.size.width   += TEXT_LEFT_INTERVAL;
	rcLabel.size.height  = rect.size.height - 2*TEXT_TOP_INTERVAL -18 ;
	
	if (rcLabel.size.height < 5) {
		rcLabel.size.height  = 0;
	}
	
	label.frame          = rcLabel;
 
	rcLabel.origin.y    += rcLabel.size.height;
	rcLabel.size.height  = 18;
	label2.frame		 = rcLabel;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
   	[self relayout];
	//label2.frame		= rcLabel;
}

- (void)dealloc
{
    [leftTopView release];
    [topView release];
    [rightTopView release];
    [rightView release];
    [rightBottomView release];
    [bottomView release];
    [leftBottomView release];
    [centerView release];
    [leftView release];
    [label release];
	[label2 release];
	
    [super dealloc];
}

- (void)setLabel:(NSString *)aLabel via:(NSString *)viaRoute
{
    label.text	= aLabel;
	label2.text	= viaRoute;
	//NSString* aText = [NSString stringWithFormat:@"%@\n%@", label.text,label2.text];
    CGRect rect = self.frame;
    CGSize constrainedSize = CGSizeMake(rect.size.width, 9999);
    CGSize sizeFont = [aLabel sizeWithFont: label.font
                         constrainedToSize: constrainedSize];
   // sizeFont.height += 12;
    rect.size.height = sizeFont.height;
	sizeFont = [viaRoute sizeWithFont: label.font constrainedToSize: constrainedSize];
	rect.size.height += sizeFont.height;
    self.frame = rect;
	[self relayout];
}

- (NSString *)getText
{
	return [NSString stringWithFormat:@"%@%@", label.text, label2.text];
}

- (NSInteger)testNeedHeight:(NSString *)aText
{
    CGRect rect = self.frame;
    if (rect.size.width < DEFAULT_BALLOON_MIN_WIDTH)
        rect.size.width = DEFAULT_BALLOON_MIN_WIDTH;
    CGSize constrainedSize = CGSizeMake(rect.size.width, 9999);
    CGSize sizeFont = [aText sizeWithFont: label.font
                         constrainedToSize: constrainedSize];
	/*
	if ( sizeFont.height > 0 )
	{
		sizeFont.height += 30.0f;
	}
    sizeFont.height += (TEXT_TOP_INTERVAL*2);
	return (sizeFont.height < 33.0) ? 33 : sizeFont.height;*/
	return sizeFont.height;
}

@end
