
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class TTStyledNode;
@class TTStyledElement;

#if __IPHONE_4_0 && __IPHONE_4_0 <= __IPHONE_OS_VERSION_MAX_ALLOWED
@interface TTStyledTextParser : NSObject <NSXMLParserDelegate> {
#else
@interface TTStyledTextParser : NSObject {
#endif
  TTStyledNode*     _rootNode;
  TTStyledElement*  _topElement;
  TTStyledNode*     _lastNode;

  NSError*          _parserError;

  NSMutableString*  _chars;
  NSMutableArray*   _stack;

  BOOL _parseLineBreaks;
  BOOL _parseURLs;
}

@property (nonatomic, retain) TTStyledNode* rootNode;
@property (nonatomic)         BOOL          parseLineBreaks;
@property (nonatomic)         BOOL          parseURLs;

- (void)parseXHTML:(NSString*)html;
- (void)parseText:(NSString*)string;

@end
