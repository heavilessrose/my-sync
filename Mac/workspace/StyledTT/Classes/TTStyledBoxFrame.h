
// Style
#import "TTStyledFrame.h"
#import "TTStyleDelegate.h"

@interface TTStyledBoxFrame : TTStyledFrame <TTStyleDelegate> {
  TTStyledBoxFrame* _parentFrame;
  TTStyledFrame*    _firstChildFrame;
  TTStyle*          _style;
}

@property (nonatomic, assign) TTStyledBoxFrame* parentFrame;
@property (nonatomic, retain) TTStyledFrame*    firstChildFrame;

/**
 * The style used to render the frame;
 */
@property (nonatomic, retain) TTStyle* style;

@end
