//
//  RectView.m
//  DrawTemplate
//
//  Created by luke on 10-2-22.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "RectView.h"
#import "wk_osdraw.h"

#import "wk_iphoneccshw.h"

@implementation RectView

@synthesize gRect;

- (id)initWithSection:(Winks_CCDW_Rect_s *)pSection
{
	gRect = pSection;
	id instance = [self initWithFrame:CGRectMake(gRect->base.Section.x, gRect->base.Section.y, 
								   gRect->base.Section.w, gRect->base.Section.h)];
	
	
	return instance;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	CGContextRef context = init_drawContext();
    int rectoff = 0;
	
    winks_ui_drawDashRect( gRect->base.Section.x, gRect->base.Section.y, 
						  (gRect->base.Section.x + gRect->base.Section.w), (gRect->base.Section.y + gRect->base.Section.h),
						  gRect->bcolor, gRect->bwidth, gRect->bstyle );
	
    rectoff = (gRect->bstyle == WINKS_LINE_DOUBLELINE) ? (3 * gRect->bwidth) : gRect->bwidth;
    if( gRect->base.flags & WK_CCDW_RGFLAG_BGCOLOR )
    {
        winks_ui_fillRect( gRect->base.Section.x + rectoff, gRect->base.Section.y + rectoff, 
						  (gRect->base.Section.x + gRect->base.Section.w - rectoff), (gRect->base.Section.y + gRect->base.Section.h - rectoff),
						  gRect->base.color );
    }
	
    printf( "Draw Rect %d, %d, %d, %d color is %x\r\n", gRect->base.Section.x, gRect->base.Section.y, 
        (gRect->base.Section.w), (gRect->base.Section.h), gRect->bcolor );
}


- (void)dealloc {
    [super dealloc];
}


@end
