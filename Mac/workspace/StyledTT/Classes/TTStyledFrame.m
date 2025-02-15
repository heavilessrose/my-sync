
#import "TTStyledFrame.h"

// Core
#import "TTCorePreprocessorMacros.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
@implementation TTStyledFrame

@synthesize element   = _element;
@synthesize nextFrame = _nextFrame;
@synthesize bounds    = _bounds;


///////////////////////////////////////////////////////////////////////////////////////////////////
- (id)initWithElement:(TTStyledElement*)element {
  if (self = [super init]) {
    _element = element;
    _bounds = CGRectZero;
  }

  return self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)dealloc {
  TT_RELEASE_SAFELY(_nextFrame);

  [super dealloc];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Public


///////////////////////////////////////////////////////////////////////////////////////////////////
- (CGFloat)x {
  return _bounds.origin.x;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setX:(CGFloat)x {
  _bounds.origin.x = x;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (CGFloat)y {
  return _bounds.origin.y;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setY:(CGFloat)y {
  _bounds.origin.y = y;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (CGFloat)width {
  return _bounds.size.width;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setWidth:(CGFloat)width {
  _bounds.size.width = width;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (CGFloat)height {
  return _bounds.size.height;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setHeight:(CGFloat)height {
  _bounds.size.height = height;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (UIFont*)font {
  return nil;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)drawInRect:(CGRect)rect {
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledBoxFrame*)hitTest:(CGPoint)point {
  return [_nextFrame hitTest:point];
}


@end
