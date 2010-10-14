//
//  LKeyboardView.h
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define kEmoGap (2)
#define kEmoCount_row (5)
#define kEmoCount_col (8)

#if defined TARGET_OS_IPHONE || defined TARGET_IPHONE_SIMULATOR

#define kBoard_Portrait_X (0.0f)
//#define kBoard_Portrait_Y (244.0f)
#define kBoard_Portrait_Y (0.0f)
#define kBoard_Portrait_W (320.0f)
#define kBoard_Portrait_H (216.0f)
#define kBoard_Landscap_X (0.0f)
//#define kBoard_Landscap_Y (160.0f)
#define kBoard_Landscap_Y (0.0f)
#define kBoard_Landscap_W (480.0f)
#define kBoard_Landscap_H (140.0f)

#define kPortraitRect (CGRectMake(kBoard_Portrait_X, kBoard_Portrait_Y, kBoard_Portrait_W, kBoard_Portrait_H))
#define kLandscapRect (CGRectMake(kBoard_Landscap_X, kBoard_Landscap_Y, kBoard_Landscap_W, kBoard_Landscap_H))

#define kPortrait_Emo_cube ((kBoard_Portrait_W - (kEmoCount_col - 1) * kEmoGap) / kEmoCount_col)
#define kLandscap_Emo_cube ((kBoard_Landscap_W - (kEmoCount_col - 1) * kEmoGap) / kEmoCount_col)

#elif defined TARGET_OS_IPAD

#endif



@interface LKeyboardView : UIView {
	IBOutlet UIScrollView *board;
}

@end
