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


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		super.backgroundColor = [UIColor colorWithWhite:1.0f alpha:0.0f];
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
	CGContextRef context = init_drawContext();
	//winks_ui_drawRect(<#int LeftTop_x#>, <#int LeftTop_y#>, <#int RightBottom_x#>, <#int RightBottom_y#>, <#unsigned int v_Color#>)
	
	draw_frame(rect, 0x00ff00ff);
	
	NSString *teststr = @"wie我我问那你";
	CGRect testRect = CGRectMake(self.bounds.origin.x, self.bounds.size.height - 20, self.bounds.size.width, 20.0f);
	[teststr drawInRect:testRect withFont:[UIFont systemFontOfSize:18.0f] 
		  lineBreakMode:UILineBreakModeClip alignment:UITextAlignmentCenter];
}


- (void)dealloc {
    [super dealloc];
}


@end
