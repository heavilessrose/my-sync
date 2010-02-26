//
//  GifView.h
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PlayerView.h"
#import "wk_iphoneccshw.h"


@interface GifView : UIView {
	PlayerView *plView;
	Winks_CCDW_Media_s *gMedia;
}

@property (nonatomic, retain) PlayerView *plView;
@property (nonatomic, assign) Winks_CCDW_Media_s *gMedia;

- (int)prepareGifLayer:(PlayerView *)mainLayer gifPath:(char *)pgifpath;
- (id)initWithSection:(Winks_CCDW_Media_s *)pSection;

@end
