/*
File: GameObject_Private.h
Abstract: Private functions used in GameObject and PhysicalObject.

Version: 2.0
*/

#import "Maths.h"
#import "PhysicalObject.h"

@interface GameObject(Private)
- (BOOL) collideWith:(id<CollisionDetection>)object tolerance:(float)tolerance;
- (void) renderAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix;
@end

@interface PhysicalObject(Private)
- (void) updatePositionAtTime:(NSTimeInterval)time;
@end