//
//  pcIndicatorView.h
//  mBackground
//
//  Created by ancle on 4/28/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface pcIndicatorView : UIImageView {
	UILabel*						label;
	UIActivityIndicatorView*		indicator;
}
- (id)initWithFrame: (CGRect)frame title: (NSString*)aTitle background: (NSString*)aFilename;

- (void)setTitle: (NSString*)aTitle;
- (void)startAnimation;
- (void)stopAnimating;
@end
