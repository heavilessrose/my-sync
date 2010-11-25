
#import "TTStyledElement.h"

// Style
#import "TTStyledTextNode.h"

// Style (private)
#import "TTStyledNodeInternal.h"

// Core
#import "TTCorePreprocessorMacros.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
@implementation TTStyledElement

@synthesize firstChild  = _firstChild;
@synthesize lastChild   = _lastChild;
@synthesize className   = _className;


///////////////////////////////////////////////////////////////////////////////////////////////////
- (id)initWithText:(NSString*)text {
  if (self = [self initWithText:text next:nil]) {
  }

  return self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (id)initWithText:(NSString*)text next:(TTStyledNode*)nextSibling {
  if (self = [super initWithNextSibling:nextSibling]) {
    if (nil != text) {
      [self addChild:[[[TTStyledTextNode alloc] initWithText:text] autorelease]];
    }
  }

  return self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (id)init {
  if (self = [self initWithText:nil next:nil]) {
  }

  return self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)dealloc {
  TT_RELEASE_SAFELY(_firstChild);
  TT_RELEASE_SAFELY(_className);

  [super dealloc];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (NSString*)description {
	NSString *desc = nil;
	if (_firstChild) {
		desc = [NSString stringWithFormat:@"+element: %@", _firstChild];
	} else {
		desc = [NSString stringWithFormat:@"+element: %@", [self className]];
	}

	return desc;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark TTStyledNode


///////////////////////////////////////////////////////////////////////////////////////////////////
- (NSString*)outerText {
  if (_firstChild) {
    NSMutableArray* strings = [NSMutableArray array];
    for (TTStyledNode* node = _firstChild; node; node = node.nextSibling) {
      [strings addObject:node.outerText];
    }
    return [strings componentsJoinedByString:@""];
  } else {
    return [super outerText];
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (NSString*)outerHTML {
  NSString* html = nil;
  if (_firstChild) {
    html = [NSString stringWithFormat:@"<div>%@</div>", _firstChild.outerHTML];
  } else {
    html = @"<div/>";
  }
  if (_nextSibling) {
    return [NSString stringWithFormat:@"%@%@", html, _nextSibling.outerHTML];
  } else {
    return html;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Public


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)addChild:(TTStyledNode*)child {
	NSLog(@"addChild: %@", child);
  if (!_firstChild) {
    _firstChild = [child retain];
    _lastChild = [self findLastSibling:child];
  } else {
    _lastChild.nextSibling = child;
    _lastChild = [self findLastSibling:child];
  }
  child.parentNode = self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)addText:(NSString*)text {
  [self addChild:[[[TTStyledTextNode alloc] initWithText:text] autorelease]];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)replaceChild:(TTStyledNode*)oldChild withChild:(TTStyledNode*)newChild {
  if (oldChild == _firstChild) {
    newChild.nextSibling = oldChild.nextSibling;
    oldChild.nextSibling = nil;
    newChild.parentNode = self;
    if (oldChild == _lastChild) {
      _lastChild = newChild;
    }
    [_firstChild release];
    _firstChild = [newChild retain];

  } else {
    TTStyledNode* node = _firstChild;
    while (node) {
      if (node.nextSibling == oldChild) {
        [oldChild retain];
        if (newChild) {
          newChild.nextSibling = oldChild.nextSibling;
          node.nextSibling = newChild;
        } else {
          node.nextSibling = oldChild.nextSibling;
        }
        oldChild.nextSibling = nil;
        newChild.parentNode = self;
        if (oldChild == _lastChild) {
          _lastChild = newChild ? newChild : node;
        }
        [oldChild release];
        break;
      }
      node = node.nextSibling;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledNode*)getElementByClassName:(NSString*)className {
  TTStyledNode* node = _firstChild;
  while (node) {
    if ([node isKindOfClass:[TTStyledElement class]]) {
      TTStyledElement* element = (TTStyledElement*)node;
      if ([element.className isEqualToString:className]) {
        return element;
      }

      TTStyledNode* found = [element getElementByClassName:className];
      if (found) {
        return found;
      }
    }
    node = node.nextSibling;
  }
  return nil;
}


@end
