//
//  pcBalloonView.h
//  imageView
//
//  Created by  apple on 10-5-18.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define BALLOON_VIEW_MIN_HEIGHT	(34)

@interface pcBalloonView : UIView {
    UIImageView	*leftTopView;
    UIImageView	*topView;
    UIImageView	*rightTopView;
    UIImageView	*rightView;
    UIImageView	*rightBottomView;
    UIImageView	*bottomView;
    UIImageView	*leftBottomView;
    UIImageView	*leftView;
    UIImageView	*centerView;
    UILabel		*label;
	UILabel		*label2;
}

- (void)setLabel:(NSString *)aLabel via:(NSString *)viaRoute;

- (NSString *)getText;

- (NSInteger)testNeedHeight:(NSString *)aText;

@end
