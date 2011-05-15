//
//  LKSegmenteControl.h
//  LKSegmenteControl
//
//  Created by LukeW on 12/10/10.
//
// Copyright (c) 2011 LukeW
// 
//

@class LKSegmenteControl;
@protocol LKSegmenteControlDelegate <NSObject>

- (UIButton *)buttonFor:(LKSegmenteControl *)segmentedControl atIndex:(NSUInteger)segmentIndex;

@optional
- (void)touchUpInsideSegmentIndex:(NSUInteger)segmentIndex;
- (void)touchDownAtSegmentIndex:(NSUInteger)segmentIndex;
@end

@interface LKSegmenteControl : UIView
{
  id <LKSegmenteControlDelegate> delegate;
  NSMutableArray *buttons;
}

@property (nonatomic, retain) NSMutableArray *buttons;

- (id)initWithSegmentCount:(NSUInteger)segmentCount segmentsize:(CGSize)segmentsize dividerImage:(UIImage *)dividerImage tag:(NSInteger)objectTag delegate:(id <LKSegmenteControlDelegate>)customSegmentedControlDelegate;

@end
