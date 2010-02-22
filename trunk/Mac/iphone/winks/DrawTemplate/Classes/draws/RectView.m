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


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
//    Winks_CCDW_Rect_s* pRect = (Winks_CCDW_Rect_s* )pSection;
//    int rectoff = 0;
//	
//    Winks_printfInfo( "WK CDWW Draw Rect color %x\r\n", pRect->bcolor );
}


- (void)dealloc {
    [super dealloc];
}


@end
