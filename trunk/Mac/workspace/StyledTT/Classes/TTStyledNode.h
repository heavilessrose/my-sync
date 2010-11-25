
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface TTStyledNode : NSObject {
  TTStyledNode* _nextSibling;
  TTStyledNode* _parentNode;
}

@property (nonatomic, retain)   TTStyledNode* nextSibling;
@property (nonatomic, assign)   TTStyledNode* parentNode;
@property (nonatomic, readonly) NSString*     outerText;
@property (nonatomic, readonly) NSString*     outerHTML;

// Designated initializer
- (id)initWithNextSibling:(TTStyledNode*)nextSibling;

- (id)ancestorOrSelfWithClass:(Class)cls;

- (void) performDefaultAction;

@end
