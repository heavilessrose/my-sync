//
//  AdvertisementView.h
//  MSN
//
//  Created by  apple on 09-3-11.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface AdvertisementView : UIToolbar 
{
	UIImage *image;
	UIImageView *imageView;
	id target;
	SEL callbackFun;
}

- (void)addTarget:(id)theTarget action:(SEL)theFun;

@property (nonatomic, retain) UIImage *image;
@property (nonatomic, assign) id target;
@property (nonatomic) SEL callbackFun;
@end
