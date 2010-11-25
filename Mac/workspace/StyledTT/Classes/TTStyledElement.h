
// Style
#import "TTStyledNode.h"

@interface TTStyledElement : TTStyledNode {
  TTStyledNode* _firstChild;
  TTStyledNode* _lastChild;
  NSString*     _className;
}

@property (nonatomic, readonly) TTStyledNode* firstChild;
@property (nonatomic, readonly) TTStyledNode* lastChild;
@property (nonatomic, retain)   NSString*     className;

- (id)initWithText:(NSString*)text;

// Designated initializer
- (id)initWithText:(NSString*)text next:(TTStyledNode*)nextSibling;

- (void)addChild:(TTStyledNode*)child;
- (void)addText:(NSString*)text;
- (void)replaceChild:(TTStyledNode*)oldChild withChild:(TTStyledNode*)newChild;

- (TTStyledNode*)getElementByClassName:(NSString*)className;

@end
