//
//  QuartzView.m
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "QuartzView.h"
#import "wk_osdraw.h"


@implementation QuartzView


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
	draw_frame(self.bounds, 0x00ff00ff);
}


- (void)dealloc {
    [super dealloc];
}


@end
