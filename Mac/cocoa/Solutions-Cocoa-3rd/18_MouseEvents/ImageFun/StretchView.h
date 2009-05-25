//
//  StretchView.h
//  ImageFun
//
//  Created by Aaron Hillegass on 10/17/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface StretchView : NSView {
	NSBezierPath *path;
	NSImage *image;
	float opacity;
	NSPoint downPoint;
	NSPoint currentPoint;
}
@property (readwrite) float opacity;
- (NSRect)currentRect;
- (NSPoint)randomPoint;
- (void)setImage:(NSImage *)i;
@end
