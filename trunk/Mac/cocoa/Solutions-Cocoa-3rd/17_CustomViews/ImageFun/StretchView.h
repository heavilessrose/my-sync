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
}
- (NSPoint)randomPoint;
@end
