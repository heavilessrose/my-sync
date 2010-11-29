
// Style
#import "TTStyledNode.h"

@interface TTStyledTextNode : TTStyledNode {
  NSString* _text;
}

@property (nonatomic, retain) NSString* text;

- (id)initWithText:(NSString*)text;

// Designated initializer
- (id)initWithText:(NSString*)text next:(TTStyledNode*)nextSibling;

@end
