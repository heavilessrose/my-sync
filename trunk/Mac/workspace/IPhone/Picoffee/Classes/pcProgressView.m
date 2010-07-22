//
//  pcProgressView.m
//  Picoffee
//
//  Created by luke on 10-7-16.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "pcProgressView.h"
#import <QuartzCore/QuartzCore.h>

#define ind_w 30
#define ind_h ind_w
#define ind_margin 10
#define msg_ind_margin 5
#define msg_x_margin 5

#define self_x 60
#define self_y 200
#define self_w 200
#define self_h 80

@implementation pcProgressView
@synthesize msgLabel;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
        self.backgroundColor = [UIColor blackColor];
        self.alpha = 0.8;
        
        UIActivityIndicatorView *ind = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
        ind.frame = CGRectMake((frame.size.width - ind_w) / 2, ind_margin, ind_w, ind_h);
        [ind startAnimating];
        [self addSubview:ind];
        [ind release];
        
        self.msgLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        msgLabel.backgroundColor = [UIColor clearColor];
        msgLabel.textColor = [UIColor whiteColor];
        [self addSubview:msgLabel];
        [msgLabel release];
    }
    return self;
}

- (void)layoutSubviews {
    if(msgLabel && msgLabel.text && [msgLabel.text length] > 0){
        msgLabel.lineBreakMode = UILineBreakModeTailTruncation;
        msgLabel.font = [UIFont systemFontOfSize:[UIFont buttonFontSize]];
        CGSize strSize = [msgLabel.text sizeWithFont:msgLabel.font];
        if(strSize.width < self.frame.size.width){
            msgLabel.frame = CGRectMake((self.frame.size.width - strSize.width) / 2, self.frame.size.height - strSize.height - 2, strSize.width, strSize.height);   
        } else {
            msgLabel.frame = CGRectMake(msg_x_margin, self.frame.size.height - strSize.height - 2, self.frame.size.width - (msg_x_margin * 2), strSize.height);
        }
    }
    [[UIApplication sharedApplication] beginIgnoringInteractionEvents];
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)dealloc {
    [[UIApplication sharedApplication] endIgnoringInteractionEvents];
    [msgLabel release];
    [super dealloc];
}

@end
