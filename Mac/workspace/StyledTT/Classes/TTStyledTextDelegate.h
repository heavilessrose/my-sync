
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class TTStyledText;

@protocol TTStyledTextDelegate <NSObject>
@optional

- (void)styledTextNeedsDisplay:(TTStyledText*)text;

@end
