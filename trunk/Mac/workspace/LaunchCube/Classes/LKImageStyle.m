//
//  LKImageStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKImageStyle.h"
#import "LKStyleContext.h"
#import "UIImageAddition.h"
#import "LKShape.h"


@implementation LKImageStyle

@synthesize imageURL, image, defaultImage, size, contentMode;

- (id)initWithNext:(LKStyle *)aNext {
	if (self = [super initWithNext:aNext]) {
		contentMode = UIViewContentModeScaleToFill;
		size = CGSizeZero;
	}
	
	return self;
}

- (void)dealloc {
	self.imageURL = nil;
	self.image = nil;
	self.defaultImage = nil;
	
	[super dealloc];
}


#pragma mark -
#pragma mark Class public

+ (LKImageStyle *)styleWithImageURL:(NSString*)imageURL next:(LKStyle *)next {
	LKImageStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.imageURL = imageURL;
	return style;
}

+ (LKImageStyle *)styleWithImageURL:(NSString *)imageURL defaultImage:(UIImage *)defaultImage
                              next:(LKStyle *)next {
	LKImageStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.imageURL = imageURL;
	style.defaultImage = defaultImage;
	return style;
}

+ (LKImageStyle *)styleWithImageURL:(NSString *)imageURL defaultImage:(UIImage *)defaultImage
                       contentMode:(UIViewContentMode)contentMode size:(CGSize)size next:(LKStyle *)next {
	LKImageStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.imageURL = imageURL;
	style.defaultImage = defaultImage;
	style.contentMode = contentMode;
	style.size = size;
	return style;
}

+ (LKImageStyle *)styleWithImage:(UIImage *)image next:(LKStyle *)next {
	LKImageStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.image = image;
	return style;
}

+ (LKImageStyle *)styleWithImage:(UIImage *)image defaultImage:(UIImage *)defaultImage
                           next:(LKStyle *)next {
	LKImageStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.image = image;
	style.defaultImage = defaultImage;
	return style;
}

+ (LKImageStyle *)styleWithImage:(UIImage *)image defaultImage:(UIImage *)defaultImage
                    contentMode:(UIViewContentMode)contentMode size:(CGSize)size next:(LKStyle *)next {
	LKImageStyle *style = [[[self alloc] initWithNext:next] autorelease];
	style.image = image;
	style.defaultImage = defaultImage;
	style.contentMode = contentMode;
	style.size = size;
	return style;
}


#pragma mark -
#pragma mark Private

- (UIImage *)imageForContext:(LKStyleContext *)context {
	UIImage *aimage = self.image;
	if (!aimage && [context.delegate respondsToSelector:@selector(imageForLayerWithStyle:)]) {
		aimage = [context.delegate imageForLayerWithStyle:self];
	}
	return aimage;
}


#pragma mark -
#pragma mark TTStyle

- (void)draw:(LKStyleContext *)context {
	UIImage *aimage = [self imageForContext:context];
	if (aimage) {
		CGContextRef ctx = UIGraphicsGetCurrentContext();
		CGContextSaveGState(ctx);
		CGRect rect = [aimage convertRect:context.contentFrame withContentMode:contentMode];
		[context.shape addToPath:rect];
		CGContextClip(ctx);
		
		[aimage drawInRect:context.contentFrame contentMode:contentMode];
		
		CGContextRestoreGState(ctx);
	}
	return [self.next draw:context];
}

- (CGSize)addToSize:(CGSize)aSize context:(LKStyleContext *)context {
	if (size.width || size.height) {
		aSize.width += size.width;
		aSize.height += size.height;
	} else if (contentMode != UIViewContentModeScaleToFill
			   && contentMode != UIViewContentModeScaleAspectFill
			   && contentMode != UIViewContentModeScaleAspectFit) {
		UIImage *aimage = [self imageForContext:context];
		if (aimage) {
			aSize.width += aimage.size.width;
			aSize.height += aimage.size.height;
		}
	}
	
	if (next) {
		return [self.next addToSize:aSize context:context];
		
	} else {
		return aSize;
	}
}


#pragma mark -
#pragma mark Public

- (UIImage *)image {
	if (!image && imageURL) {
		//image = [[[TTURLCache sharedCache] imageForURL:imageURL] retain];
	}
	
	return image;
}

@end
