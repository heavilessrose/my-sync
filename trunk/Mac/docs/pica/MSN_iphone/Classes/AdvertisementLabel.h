//
//  AdvertisementLabel.h
//  MSN
//
//  Created by  apple on 09-3-11.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface AdvertisementLabel : UIToolbar
{
	NSString *text;
	UITextAlignment textAlignment;
	UIColor *textColor;
	UILabel *label;
	NSString *sLink;
	id target;
	SEL callbackFun;
}

- (void)addTarget:(id)theTarget action:(SEL)theFun;
- (CGRect)textRectForBounds:(CGRect)bounds limitedToNumberOfLines:(NSInteger)numberOfLines;
@property (nonatomic, retain) NSString *text;
@property (nonatomic, retain) UIColor *textColor;
@property (nonatomic) UITextAlignment textAlignment;
@property (nonatomic, assign) id target;
@property (nonatomic) SEL callbackFun;
@property (nonatomic, retain) NSString *sLink;
@property (nonatomic, retain) UILabel *label;
@end
