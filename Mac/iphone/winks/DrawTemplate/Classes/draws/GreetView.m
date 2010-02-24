//
//  GreetView.m
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "GreetView.h"
#import "wk_osdraw.h"

@implementation GreetView

@synthesize gText, greetingTimer, gNsGreet, gGreetWidth;

extern Winks_CCSW_Global_s gCCSW;
static int showtext_in_area(winks_scrolling_text_s * text);
static void handle_scrolling_text(winks_scrolling_text_s *s);

- (id)initWithSection:(Winks_CCDW_Text_s *)pSection
{
	gText = pSection;
	id instance = [self initWithFrame:CGRectMake(gText->base.Section.x, gText->base.Section.y, 
								   gText->base.Section.w, gText->base.Section.h)];
	
	[self getGreetingStyles];
	return instance;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		super.backgroundColor = [UIColor colorWithWhite:1.0f alpha:0.0f];
    }
    return self;
}

static int utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size)
{
	NSString *nsUtf8 = [NSString stringWithUTF8String:in_utf8];
	if ([nsUtf8 canBeConvertedToEncoding:NSUnicodeStringEncoding]) {
		memcpy(out_unicode, [nsUtf8 cStringUsingEncoding:NSUnicodeStringEncoding], size);
	} else {
		return -1;
	}
	
	return 0;
}

- (void)drawRect:(CGRect)rect {
    // Drawing code
	CGContextRef context = init_drawContext();

//	CGFloat strW = [self getStrWidth:nsText withFontSize:gText->txtstyle.font_feature.size];
//	if (strW > gText->txtstyle.width) {
//		// 需要滚动
//		
//	} else {
//		// 不滚动
//	}
	
	set_curColor(gText->txtstyle.font_feature.color);
	[self renderGreeting:gNsGreet width:gGreetWidth];

	
/*	
    if( gText->flags & WK_CCDW_FTNEEDSCROLL )
    {
        handle_scrolling_text( &(gText->txtstyle) );
        gText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
    }
    else
    {
        if( showtext_in_area( &(gText->txtstyle) ) == WINKS_NEEDSCROLLING_FLAG )
        {
            gText->flags |= WK_CCDW_FTNEEDSCROLL;
            gText->base.flags |= WK_CCDW_RGFLAG_REDRAW;
        }
    }
 */
}


- (void)dealloc {
	[gNsGreet release];
	[self disposeGreetTimer];
	
    [super dealloc];
}

#pragma mark draw

- (void)getGreetingStyles
{
    int dlen = 0;
    unsigned long fflag = 0;
    char* pdata = NULL;
    printf( "WK CDWW Draw Text %s\r\n", gText->base.pdata );
	
    if( (gText->base.flags & WK_CCDW_RGFLAG_SREF) && (gText->txtstyle.text == NULL) )
    {
        /* get data from server ref */
        dlen = WK_CCDW_TEXTBUFLEN;
        /* deal greeting sepreatedly */
		if( (Winks_GetShowElement( WINKS_DFT_CC_NO, WINKS_TRUE, 
								  gText->base.pdata, &fflag, gText->pbuf, (unsigned long* )(&dlen), NULL, 0 ) < 0) )
        {
            if( dlen > WK_CCDW_TEXTBUFLEN )
            {
                pdata = malloc( dlen + 2 );
                memset( pdata, 0, dlen + 2 );
                if( Winks_GetShowElement( WINKS_DFT_CC_NO, WINKS_TRUE, 
										 gText->base.pdata, &fflag, pdata, (unsigned long* )(&dlen), NULL, 0 ) < 0 )
                {
                    free( pdata );
                    return;
                }
                gText->flags |= WK_CCDW_FTALLOC;
                gText->txtstyle.text = pdata;
            }
            else
                return;
        }
        else
            gText->txtstyle.text = gText->pbuf;
	}
    printf( "WK CDWW Draw Text %s\r\n", gText->txtstyle.text );
    assert( gText->txtstyle.text  && "gText->txtstyle.text should not be NULL" );
	
    printf( "WK CCDW Draw Text %d, %d, %d, %d ref is %s\r\n", gText->base.Section.x, gText->base.Section.y, 
		   gText->base.Section.w, gText->base.Section.h, gText->base.pdata );
	
	self.gNsGreet = [NSString stringWithCString:gText->txtstyle.text];
	self.gGreetWidth = (int)[self getStrWidth:gNsGreet withFontSize:gText->txtstyle.font_feature.size];
}

- (CGFloat)getStrWidth:(NSString *)str withFontSize:(int)size
{
	CGSize strSize = [str sizeWithFont:[UIFont systemFontOfSize:(CGFloat)size]];
	return strSize.width;
}

#define CROLL_STEP 2
static int drawPoint_x = 0;
static int drawPoint_y = 0;
static int drawPoint_continue_x = 0;
static int drawPoint_continue_y = 0;

- (void)renderGreeting:(NSString *)greeting width:(int)greetingWidth
{
	// align
	UITextAlignment align = UITextAlignmentCenter;
	// 默认居中
	CGPoint rectPoint = CGPointMake((gText->txtstyle.width - greetingWidth) / 2, 
									(gText->txtstyle.height - gText->txtstyle.font_feature.size) / 2);
	// 滚动方向
	int direct = 0;
	
	if(gText->txtstyle.show_flags == WINKS_SHOW_CENTER){
		align = UITextAlignmentCenter;
		rectPoint.x = (gText->txtstyle.width - greetingWidth) / 2;
	} else if(gText->txtstyle.show_flags == WINKS_SHOW_RIGHT){
		align = UITextAlignmentRight;
		rectPoint.x = gText->txtstyle.width - greetingWidth;
	} else {
		align = UITextAlignmentLeft;
		rectPoint.x = 0;
	}
	
	
	// 默认单行
	CGSize rectSize = CGSizeMake(greetingWidth, gText->txtstyle.height);
	/*
	// multiLine
	if (gText->txtstyle.lineflags == 1) {
		// WK_WG_MULLINE = 1
		rectPoint.y = 0;
		rectSize.width = gText->txtstyle.width;
		direct = WK_WG_FONTUPSCROLL;
	}
	*/
	// wrap
	if (gText->txtstyle.wrapflags == WK_WG_WRAP) {
		rectSize.width = gText->txtstyle.width;
		rectPoint.y = 0;
		direct = WK_WG_FONTUPSCROLL;
	}

	// scroll
	// 默认向左滚动
//	if (gText->txtstyle.font_feature.way == WK_WG_FONTRIGHTSCROLL && greetingWidth > gText->txtstyle.width) {
//		direct = WK_WG_FONTRIGHTSCROLL;
//	}
	
	// 滚动
	CGRect dwRect = CGRectMake(rectPoint.x, rectPoint.y, rectSize.width, rectSize.height);
	switch (direct) {
		case WK_WG_FONTLEFTSCROLL:
		case WK_WG_FONTRIGHTSCROLL:
			[self drawCrollLeft:greeting withRect:dwRect withAlign:align];
			break;
		case WK_WG_FONTUPSCROLL:
			[self drawCrollUp:greeting withRect:dwRect withAlign:align];
			break;
		default:
		{// 默认不滚动
			self.greetingTimer = nil;
			//CGFloat x = (gText->txtstyle.width - gGreetWidth) / 2;
			/*
			 [greeting drawAtPoint:CGPointMake(x, (gText->txtstyle.height - gText->txtstyle.font_feature.size) / 2) 
			 withFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size]];
			 */
			
			[greeting drawInRect:CGRectMake(rectPoint.x, rectPoint.y, rectSize.width, rectSize.height) 
						withFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size] 
				   lineBreakMode:UILineBreakModeClip alignment:align];
		}
			break;
	}

}

- (void)drawCrollUp:(NSString *)greeting withRect:(CGRect)rect withAlign:(UITextAlignment)align
{
	// 计算所有行总高度
	CGSize boundingSize = CGSizeMake(rect.size.width, CGFLOAT_MAX);
	CGSize requiredSize = [greeting sizeWithFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size]
							   constrainedToSize:boundingSize
								   lineBreakMode:UILineBreakModeClip];
	CGFloat requiredHeight = requiredSize.height;
	
	rect.size.height = requiredHeight;
	if (gText->txtstyle.height < requiredHeight) 
	{
		// 控制上下移动
		if (drawPoint_y < requiredHeight + gText->txtstyle.height) {
			drawPoint_y += CROLL_STEP;
		} else {
			//drawPoint_continue_y = drawPoint_y;
			drawPoint_y = 0;
		}
		
		// 滚动
		if(greetingTimer == nil){
			NSTimeInterval animationInterval = 2.0 / 60.0;
			self.greetingTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval 
																  target:self 
																selector:@selector(timerFireMethod:) 
																userInfo:nil repeats:YES];
		}
		/*
		if ((drawPoint_continue_y != 0) && drawPoint_continue_y < (gText->txtstyle.width + gGreetWidth)) {
			drawPoint_continue_y += CROLL_STEP;
			
			[greeting drawInRect:CGRectMake(0, rect.origin.y - drawPoint_y, rect.size.width, rect.size.height) 
						withFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size] 
				   lineBreakMode:UILineBreakModeClip alignment:align];
		} else {
			drawPoint_continue_y = 0;
		}
		*/
		[greeting drawInRect:CGRectMake(/*gText->txtstyle.width*/0, rect.origin.y + gText->txtstyle.height - drawPoint_y, rect.size.width, rect.size.height) 
					withFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size] 
			   lineBreakMode:UILineBreakModeClip alignment:align];
	}
}

- (void)drawCrollLeft:(NSString *)greeting withRect:(CGRect)rect withAlign:(UITextAlignment)align
{
	if((gGreetWidth > gText->txtstyle.width && rect.size.width > gText->txtstyle.width)){
		// 控制左右移动
		if(drawPoint_x < (gText->txtstyle.width/2 + gGreetWidth) && (gGreetWidth > gText->txtstyle.width && rect.size.width > gText->txtstyle.width)) 
		{
			drawPoint_x += CROLL_STEP;
		} else {
			drawPoint_continue_x = drawPoint_x;
			drawPoint_x = 0;
		}
		
		// 向左滚动画
		if(greetingTimer == nil){
			NSTimeInterval animationInterval = 2.0 / 60.0;
			self.greetingTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval 
																  target:self 
																selector:@selector(timerFireMethod:) 
																userInfo:nil repeats:YES];
		}
		if ((drawPoint_continue_x != 0) && drawPoint_continue_x < (gText->txtstyle.width + gGreetWidth)) {
			drawPoint_continue_x += CROLL_STEP;
			
			[greeting drawInRect:CGRectMake(gText->txtstyle.width - drawPoint_continue_x, rect.origin.y, rect.size.width, rect.size.height) 
						withFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size] 
				   lineBreakMode:UILineBreakModeClip alignment:align];
			
		}else {
			drawPoint_continue_x = 0;
		}
		
		[greeting drawInRect:CGRectMake(gText->txtstyle.width - drawPoint_x, rect.origin.y, rect.size.width, rect.size.height) 
					withFont:[UIFont systemFontOfSize:gText->txtstyle.font_feature.size] 
			   lineBreakMode:UILineBreakModeClip alignment:align];
	}
}
#pragma mark timer
- (void)timerFireMethod:(NSTimer*)theTimer
{
	// repaint
	[self setNeedsDisplay];
}

- (void)disposeGreetTimer
{
	printf("disposeGreetTimer start\r\n");
	if(greetingTimer != nil){
		printf("disposeGreetTimer: timer != nil count = %u start\r\n", [greetingTimer retainCount]);
		[greetingTimer invalidate];
		self.greetingTimer = nil;
		drawPoint_x = 0;
		drawPoint_y = 0;
		drawPoint_continue_x = 0;
		drawPoint_continue_y = 0;
	}
	printf("disposeGreetTimer end\r\n");
}

@end
