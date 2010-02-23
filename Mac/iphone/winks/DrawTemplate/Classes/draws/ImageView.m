//
//  ImageView.m
//  DrawTemplate
//
//  Created by luke on 10-2-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ImageView.h"


@implementation ImageView


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}

/* UIImageView 不会调用drawRect
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)dealloc {
    [super dealloc];
}


@end
