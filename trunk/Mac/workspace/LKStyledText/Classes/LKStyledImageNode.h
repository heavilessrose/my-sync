//
//  LKStyledImageNode.h
//  LKStyledText
//
//  Created by luke on 10-12-1.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKStyledNode.h"

@interface LKStyledImageNode : LKStyledNode {
	
	UIImage *image;
}

@property (nonatomic, retain) UIImage *image;

- (id)initWithImage:(UIImage *)aImg;
@end
