
#import "TTStyledText.h"

// Style
#import "TTStyledTextDelegate.h"
#import "TTStyledNode.h"
#import "TTStyledFrame.h"
#import "TTStyledLayout.h"
#import "TTStyledTextParser.h"
#import "TTStyledImageNode.h"
#import "TTStyledTextNode.h"
#import "TTStyledBoxFrame.h"
#import "TTStyledTextFrame.h"
#import "TTStyledImageFrame.h"

// Network
#import "TTURLImageResponse.h"
#import "TTURLCache.h"
#import "TTURLRequest.h"

// Core
#import "TTGlobalCore.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
@interface TTStyledText()

- (void)stopLoadingImages;

@end


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
@implementation TTStyledText

@synthesize rootNode      = _rootNode;
@synthesize font          = _font;
@synthesize width         = _width;
@synthesize height        = _height;
@synthesize invalidImages = _invalidImages;
@synthesize delegate      = _delegate;


///////////////////////////////////////////////////////////////////////////////////////////////////
- (id)initWithNode:(TTStyledNode*)rootNode {
	if (self = [super init]) {
		_rootNode = [rootNode retain];
	}
	
	return self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)dealloc {
	[self stopLoadingImages];
	TT_RELEASE_SAFELY(_rootNode);
	TT_RELEASE_SAFELY(_rootFrame);
	TT_RELEASE_SAFELY(_font);
	TT_RELEASE_SAFELY(_invalidImages);
	TT_RELEASE_SAFELY(_imageRequests);
	
	[super dealloc];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (NSString*)description {
	return [self.rootNode outerText];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Class public


///////////////////////////////////////////////////////////////////////////////////////////////////
+ (TTStyledText*)textFromXHTML:(NSString*)source {
	return [self textFromXHTML:source lineBreaks:NO URLs:YES];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
+ (TTStyledText*)textFromXHTML:(NSString*)source lineBreaks:(BOOL)lineBreaks URLs:(BOOL)URLs {
	TTStyledTextParser* parser = [[[TTStyledTextParser alloc] init] autorelease];
	parser.parseLineBreaks = lineBreaks;
	parser.parseURLs = URLs;
	[parser parseXHTML:source];
	if (parser.rootNode) {
		NSLog(@"^^^^^^^^^^^^^^^^^^^^^^^^^^TTStyledText alloc start");
		NSLog(@"%@", [parser.rootNode outerHTML]);
		return [[[TTStyledText alloc] initWithNode:parser.rootNode] autorelease];
		NSLog(@"^^^^^^^^^^^^^^^^^^^^^^^^^^TTStyledText alloc end");
		
	} else {
		return nil;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
+ (TTStyledText*)textWithURLs:(NSString*)source {
	return [self textWithURLs:source lineBreaks:NO];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
+ (TTStyledText*)textWithURLs:(NSString*)source lineBreaks:(BOOL)lineBreaks {
	TTStyledTextParser* parser = [[[TTStyledTextParser alloc] init] autorelease];
	parser.parseLineBreaks = lineBreaks;
	parser.parseURLs = YES;
	[parser parseText:source];
	if (parser.rootNode) {
		return [[[TTStyledText alloc] initWithNode:parser.rootNode] autorelease];
		
	} else {
		return nil;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Private


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)stopLoadingImages {
	if (_imageRequests) {
		NSMutableArray* requests = [_imageRequests retain];
		TT_RELEASE_SAFELY(_imageRequests);
		
		if (!_invalidImages) {
			_invalidImages = [[NSMutableArray alloc] init];
		}
		
		for (TTURLRequest* request in requests) {
			[_invalidImages addObject:request.userInfo];
			[request cancel];
		}
		[requests release];
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)loadImages {
	[self stopLoadingImages];
	
	if (_delegate && _invalidImages) {
		BOOL loadedSome = NO;
		for (TTStyledImageNode* imageNode in _invalidImages) {
			if (imageNode.URL) {
				UIImage* image = [[TTURLCache sharedCache] imageForURL:imageNode.URL];
				if (image) {
					imageNode.image = image;
					loadedSome = YES;
					
				} else {
					TTURLRequest* request = [TTURLRequest requestWithURL:imageNode.URL delegate:self];
					request.userInfo = imageNode;
					request.response = [[[TTURLImageResponse alloc] init] autorelease];
					[request send];
				}
			}
		}
		
		TT_RELEASE_SAFELY(_invalidImages);
		
		if (loadedSome) {
			[_delegate styledTextNeedsDisplay:self];
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledFrame*)getFrameForNode:(TTStyledNode*)node inFrame:(TTStyledFrame*)frame {
	while (frame) {
		if ([frame isKindOfClass:[TTStyledBoxFrame class]]) {
			TTStyledBoxFrame* boxFrame = (TTStyledBoxFrame*)frame;
			if (boxFrame.element == node) {
				return boxFrame;
			}
			
			TTStyledFrame* found = [self getFrameForNode:node inFrame:boxFrame.firstChildFrame];
			if (found) {
				return found;
			}
			
		} else if ([frame isKindOfClass:[TTStyledTextFrame class]]) {
			TTStyledTextFrame* textFrame = (TTStyledTextFrame*)frame;
			if (textFrame.node == node) {
				return textFrame;
			}
			
		} else if ([frame isKindOfClass:[TTStyledImageFrame class]]) {
			TTStyledImageFrame* imageFrame = (TTStyledImageFrame*)frame;
			if (imageFrame.imageNode == node) {
				return imageFrame;
			}
		}
		
		frame = frame.nextFrame;
	}
	return nil;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark TTURLRequestDelegate


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)requestDidStartLoad:(TTURLRequest*)request {
	if (!_imageRequests) {
		_imageRequests = [[NSMutableArray alloc] init];
	}
	[_imageRequests addObject:request];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)requestDidFinishLoad:(TTURLRequest*)request {
	TTURLImageResponse* response = request.response;
	TTStyledImageNode* imageNode = request.userInfo;
	imageNode.image = response.image;
	
	[_imageRequests removeObject:request];
	
	[_delegate styledTextNeedsDisplay:self];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)request:(TTURLRequest*)request didFailLoadWithError:(NSError*)error {
	[_imageRequests removeObject:request];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)requestDidCancelLoad:(TTURLRequest*)request {
	[_imageRequests removeObject:request];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Public


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setDelegate:(id<TTStyledTextDelegate>)delegate {
	if (_delegate != delegate) {
		_delegate = delegate;
		[self loadImages];
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledFrame*)rootFrame {
	[self layoutIfNeeded];
	return _rootFrame;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setFont:(UIFont*)font {
	if (font != _font) {
		[_font release];
		_font = [font retain];
		[self setNeedsLayout];
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setWidth:(CGFloat)width {
	if (width != _width) {
		_width = width;
		[self setNeedsLayout];
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (CGFloat)height {
	[self layoutIfNeeded];
	return _height;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (BOOL)needsLayout {
	return !_rootFrame;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)layoutFrames {
	TTStyledLayout* layout = [[TTStyledLayout alloc] initWithRootNode:_rootNode];
	layout.width = _width;
	layout.font = _font;
	[layout layout:_rootNode];
	
	[_rootFrame release];
	_rootFrame = [layout.rootFrame retain];
	_height = ceil(layout.height);
	[_invalidImages release];
	_invalidImages = [layout.invalidImages retain];
	[layout release];
	
	[self loadImages];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)layoutIfNeeded {
	if (!_rootFrame) {
		[self layoutFrames];
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)setNeedsLayout {
	TT_RELEASE_SAFELY(_rootFrame);
	_height = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)drawAtPoint:(CGPoint)point {
	[self drawAtPoint:point highlighted:NO];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)drawAtPoint:(CGPoint)point highlighted:(BOOL)highlighted {
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSaveGState(ctx);
	CGContextTranslateCTM(ctx, point.x, point.y);
	
	TTStyledFrame* frame = self.rootFrame;
	static int frameCount = 0;
	NSLog(@"=================================start draw:\n");
	while (frame) {
		frameCount++;
		NSLog(@"draw frame: %@ in (%.0f,%.0f,%.0f,%.0f)", frame, frame.bounds.origin.x, frame.bounds.origin.y, frame.bounds.size.width, frame.bounds.size.height);
		[frame drawInRect:frame.bounds];
		frame = frame.nextFrame;
	}
	NSLog(@"-------frameCount: %d", frameCount);
	frameCount = 0;
	CGContextRestoreGState(ctx);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledBoxFrame*)hitTest:(CGPoint)point {
	return [self.rootFrame hitTest:point];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledFrame*)getFrameForNode:(TTStyledNode*)node {
	return [self getFrameForNode:node inFrame:_rootFrame];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)addChild:(TTStyledNode*)child {
	if (!_rootNode) {
		self.rootNode = child;
	} else {
		TTStyledNode* previousNode = _rootNode;
		TTStyledNode* node = _rootNode.nextSibling;
		while (node) {
			previousNode = node;
			node = node.nextSibling;
		}
		previousNode.nextSibling = child;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)addText:(NSString*)text {
	[self addChild:[[[TTStyledTextNode alloc] initWithText:text] autorelease]];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)insertChild:(TTStyledNode*)child atIndex:(NSInteger)insertIndex {
	if (!_rootNode) {
		self.rootNode = child;
	} else if (insertIndex == 0) {
		child.nextSibling = _rootNode;
		self.rootNode = child;
	} else {
		NSInteger i = 0;
		TTStyledNode* previousNode = _rootNode;
		TTStyledNode* node = _rootNode.nextSibling;
		while (node && i != insertIndex) {
			++i;
			previousNode = node;
			node = node.nextSibling;
		}
		child.nextSibling = node;
		previousNode.nextSibling = child;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (TTStyledNode*)getElementByClassName:(NSString*)className {
	TTStyledNode* node = _rootNode;
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
