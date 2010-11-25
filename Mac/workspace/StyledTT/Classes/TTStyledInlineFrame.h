
// Style
#import "TTStyledBoxFrame.h"

@interface TTStyledInlineFrame : TTStyledBoxFrame {
  TTStyledInlineFrame* _inlinePreviousFrame;
  TTStyledInlineFrame* _inlineNextFrame;
}

@property (nonatomic, readonly) TTStyledInlineFrame* inlineParentFrame;
@property (nonatomic, assign)   TTStyledInlineFrame* inlinePreviousFrame;
@property (nonatomic, assign)   TTStyledInlineFrame* inlineNextFrame;

@end
