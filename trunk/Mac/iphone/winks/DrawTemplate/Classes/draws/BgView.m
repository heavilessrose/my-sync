//
//  BgView.m
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "BgView.h"
#import "wk_osdraw.h"


@implementation BgView


- (id)initWithFrame:(CGRect)frame {
	NSLog(@"initWithFrame");
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		// 默认填充背景色为黑色不透明
		self.backgroundColor = [UIColor blueColor];
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	NSLog(@"drawRect");
	CGContextRef context = init_drawContext();
	CGContextSaveGState(context);
	fill_frame(self.bounds, 0);
	CGContextRestoreGState(context);
	draw_frame(self.bounds, 0x00ff00ff);
	//winks_ui_drawRect(0, 30, 20, 50, 0x00ff00ff);
	CGContextRestoreGState(context);
}


- (void)dealloc {
    [super dealloc];
}


@end
