//
//  GifView.h
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PlayerView.h"


@interface GifView : UIView {
	PlayerView *plView;
}

@property(nonatomic, retain)PlayerView *plView;

- (int)prepareGifLayer:(PlayerView *)mainLayer gifPath:(NSString *)pgifpath;

@end
