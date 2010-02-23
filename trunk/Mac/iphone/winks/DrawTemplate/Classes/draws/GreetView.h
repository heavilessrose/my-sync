//
//  GreetView.h
//  TemplateShow
//
//  Created by luke on 10-2-7.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "wk_iphoneccshw.h"

@interface GreetView : UIView {
	BOOL multiline;
	int align;
	
	Winks_CCDW_Text_s *gText;
}

@property (nonatomic, assign) Winks_CCDW_Text_s *gText;

- (id)initWithSection:(Winks_CCDW_Text_s *)pSection;

@end
