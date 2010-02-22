//
//  DrawTemplateViewController.h
//  DrawTemplate
//
//  Created by luke on 10-1-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "wk_iphoneccshw.h"

@interface DrawTemplateViewController : UIViewController {

}

- (int)parseTemplate;
- (int)ccsw_draw:(Winks_CCDW_Global_s *)pCCDW;
- (int)ccdw_draw:(Winks_CCDW_RgBase_s *)pBase;

- (int)ccdw_drawRect:(Winks_CCDW_RgBase_s *)pSection;
- (int)ccdw_drawText:(Winks_CCDW_RgBase_s *)pSection;
- (int)ccdw_drawMedia:(Winks_CCDW_RgBase_s *)pSection;
@end

