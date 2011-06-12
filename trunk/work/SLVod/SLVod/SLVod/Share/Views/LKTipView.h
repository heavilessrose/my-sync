//
//  LKTipView.h
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface LKTipView : UIView {
	
@private
	CGRect messageRect;
	NSString *text;
	UIImage *image;
	
	BOOL keepFit;
	BOOL roundedConner;
}

@property (nonatomic, retain) NSString *text;
@property (nonatomic, retain) UIImage *image;

- (id)init;
- (void)setText:(NSString *)str;
- (void)setImage:(UIImage *)image;
@end
