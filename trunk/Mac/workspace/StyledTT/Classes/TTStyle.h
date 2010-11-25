
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class TTStyleContext;

@interface TTStyle : NSObject {
  TTStyle* _next;
}

@property (nonatomic, retain) TTStyle* next;

- (id)initWithNext:(TTStyle*)next;

- (TTStyle*)next:(TTStyle*)next;

- (void)draw:(TTStyleContext*)context;

- (UIEdgeInsets)addToInsets:(UIEdgeInsets)insets forSize:(CGSize)size;
- (CGSize)addToSize:(CGSize)size context:(TTStyleContext*)context;

- (void)addStyle:(TTStyle*)style;

- (id)firstStyleOfClass:(Class)cls;
- (id)styleForPart:(NSString*)name;

@end
