//
//  LKTabBar.h
//  siluVod
//
//  Created by luke on 11-5-15.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@class LKTabBar;
@protocol LKTabBarDelegate <NSObject>

- (UIImage*) imageFor:(LKTabBar *)tabBar atIndex:(NSUInteger)itemIndex;
- (UIImage*) backgroundImage;
- (UIImage*) selectedItemBackgroundImage;
- (UIImage*) glowImage;
- (UIImage*) selectedItemImage;
- (UIImage*) tabBarArrowImage;

@optional
- (void) touchUpInsideItemAtIndex:(NSUInteger)itemIndex;
- (void) touchDownAtItemAtIndex:(NSUInteger)itemIndex;
@end


@interface LKTabBar : UIView
{
    id <LKTabBarDelegate> delegate;
    NSMutableArray *buttons;
}

@property (nonatomic, assign) id <LKTabBarDelegate> delegate;
@property (nonatomic, retain) NSMutableArray *buttons;

- (id)initWithItemCount:(NSUInteger)itemCount itemSize:(CGSize)itemSize tag:(NSInteger)objectTag delegate:(id <LKTabBarDelegate>)customTabBarDelegate;

- (void)selectItemAtIndex:(NSInteger)index;
- (void)glowItemAtIndex:(NSInteger)index;
- (void)removeGlowAtIndex:(NSInteger)index;

@end