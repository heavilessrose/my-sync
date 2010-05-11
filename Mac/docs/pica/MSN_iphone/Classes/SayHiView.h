//
//  SayHiView.h
//  MSN
//
//  Created by Jun Du on 5/31/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SayHiView : UIView {
}

+ (id)SayHiInView:(UIView *)aSuperview rect:(CGRect)rect liveID:(NSString *)liveID;
- (void)removeSayHiView;
@end
