//
//  LKeyboardView.h
//  LKeyboard
//
//  Created by luke on 10-10-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define kEmoGap_Portrait_col (2)
#define kEmoGap_Portrait_row (2)
#define kEmoGap_Landscap_col (2)
#define kEmoGap_Landscap_row (2)

#define kEmoCount_Portrait_row (5)
#define kEmoCount_Portrait_col (8)
#define kEmoCount_Landscap_row (4)
#define kEmoCount_Landscap_col (10)

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
#define kPortraitRect_Frame CGRectMake(0, 244, 320, 216);
#define kLandscapRect_Frame CGRectMake(0, 160, 480, 140);

#define kPortrait_Emo_cube_w ((kBoard_Portrait_W - (kEmoCount_Portrait_col - 1) * kEmoGap_Portrait_col) / kEmoCount_Portrait_col)
#define kPortrait_Emo_cube_h ((kBoard_Portrait_H - (kEmoCount_Portrait_row - 1) * kEmoGap_Portrait_row) / kEmoCount_Portrait_row)
#define kLandscap_Emo_cube_w ((kBoard_Landscap_W - (kEmoCount_Landscap_col - 1) * kEmoGap_Landscap_col) / kEmoCount_Landscap_col)
#define kLandscap_Emo_cube_h ((kBoard_Landscap_H - (kEmoCount_Landscap_row - 1) * kEmoGap_Landscap_row) / kEmoCount_Landscap_row)

#elif defined TARGET_OS_IPAD

#endif



@interface LKeyboardView : UIView {
	IBOutlet UIScrollView *board;
}

@end
