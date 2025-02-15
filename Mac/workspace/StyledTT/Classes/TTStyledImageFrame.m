//
// Copyright 2009-2010 Facebook
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#import "TTStyledImageFrame.h"

// Style
#import "TTStyledImageNode.h"
#import "TTStyleContext.h"
#import "TTShape.h"
#import "TTImageStyle.h"
#import "UIImageAdditions.h"

// Core
#import "TTCorePreprocessorMacros.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
@implementation TTStyledImageFrame

@synthesize imageNode = _imageNode;
@synthesize style     = _style;


///////////////////////////////////////////////////////////////////////////////////////////////////
- (id)initWithElement:(TTStyledElement*)element node:(TTStyledImageNode*)node {
  if (self = [super initWithElement:element]) {
	  _imageNode = node;
	  NSLog(@"创建了TTStyledImageFrame: element= [%@], node= [%@]", element, _imageNode);
  }

  return self;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)dealloc {
  TT_RELEASE_SAFELY(_style);

  [super dealloc];
}


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)drawImage:(CGRect)rect {
  CGContextRef ctx = UIGraphicsGetCurrentContext();
  CGContextSaveGState(ctx);
  CGContextAddRect(ctx, rect);
  CGContextClip(ctx);

  UIImage* image = _imageNode.image ? _imageNode.image : _imageNode.defaultImage;
  [image drawInRect:rect contentMode:UIViewContentModeScaleAspectFit];
  CGContextRestoreGState(ctx);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark TTStyleDelegate


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)drawLayer:(TTStyleContext*)context withStyle:(TTStyle*)style {
  CGContextRef ctx = UIGraphicsGetCurrentContext();
  CGContextSaveGState(ctx);
  [context.shape addToPath:context.frame];
  CGContextClip(ctx);

  UIViewContentMode contentMode = UIViewContentModeScaleAspectFit;
  if ([style isMemberOfClass:[TTImageStyle class]]) {
    TTImageStyle* imageStyle = (TTImageStyle*)style;
    contentMode = imageStyle.contentMode;
  }

  UIImage* image = _imageNode.image ? _imageNode.image : _imageNode.defaultImage;
  [image drawInRect:context.contentFrame contentMode:contentMode];

  CGContextRestoreGState(ctx);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Public


///////////////////////////////////////////////////////////////////////////////////////////////////
- (void)drawInRect:(CGRect)rect {
  if (_style) {
    TTStyleContext* context = [[[TTStyleContext alloc] init] autorelease];
    context.delegate = self;
    context.frame = rect;
    context.contentFrame = rect;

    [_style draw:context];
    if (!context.didDrawContent) {
      [self drawImage:rect];
    }

  } else {
    [self drawImage:rect];
  }
}


@end
