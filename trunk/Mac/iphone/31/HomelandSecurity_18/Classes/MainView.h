//
//  MainView.h
//  HomelandSecurity
//
//  Created by apps on 10/18/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainView : UIView {
    IBOutlet UIImageView *elevated;
    IBOutlet UIImageView *guarded;
    IBOutlet UIImageView *high;
    IBOutlet UIImageView *low;
    IBOutlet UIImageView *severe;
}

- (void)onTimer;
- (NSString*)getAdvisoryCondition;

@end
