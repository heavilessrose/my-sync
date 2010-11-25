
// Style
#import "TTStyledFrame.h"

@class TTStyledTextNode;

@interface TTStyledTextFrame : TTStyledFrame {
  TTStyledTextNode* _node;
  NSString*         _text;
  UIFont*           _font;
}

/**
 * The node represented by the frame.
 */
@property (nonatomic, readonly) TTStyledTextNode* node;

/**
 * The text that is displayed by this frame.
 */
@property (nonatomic, readonly) NSString* text;

/**
 * The font that is used to measure and display the text of this frame.
 */
@property (nonatomic, retain) UIFont* font;

- (id)initWithText:(NSString*)text element:(TTStyledElement*)element node:(TTStyledTextNode*)node;

@end
