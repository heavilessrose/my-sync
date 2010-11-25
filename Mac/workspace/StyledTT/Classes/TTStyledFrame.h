
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class TTStyledElement;
@class TTStyledFrame;
@class TTStyledBoxFrame;

@interface TTStyledFrame : NSObject {
  TTStyledElement*  _element;
  TTStyledFrame*    _nextFrame;
  CGRect            _bounds;
}

/**
 * The element that contains the frame.
 */
@property (nonatomic, readonly) TTStyledElement* element;

/**
 * The next in the linked list of frames.
 */
@property (nonatomic, retain) TTStyledFrame* nextFrame;

/**
 * The bounds of the content that is displayed by this frame.
 */
@property (nonatomic) CGRect bounds;

@property (nonatomic) CGFloat x;
@property (nonatomic) CGFloat y;
@property (nonatomic) CGFloat width;
@property (nonatomic) CGFloat height;

- (UIFont*)font;

- (id)initWithElement:(TTStyledElement*)element;

/**
 * Draws the frame.
 */
- (void)drawInRect:(CGRect)rect;

- (TTStyledBoxFrame*)hitTest:(CGPoint)point;

@end
