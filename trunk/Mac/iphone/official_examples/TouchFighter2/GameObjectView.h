/*
File: GameObjectView.h
Abstract: A convenience view that visualize a GameObject with optional reflection and rotation.

Version: 2.0
*/

#import <UIKit/UIKit.h>
#import "EAGLView.h"

@class GameObject;

@interface GameObjectView : EAGLView {
	GameObject*				_gameObject;
	CGFloat					_rotation, _rotationSpeed;
	NSTimeInterval			_previousTime;
	BOOL					_reflection;
}

- (id)initWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained;
- (void) render;

@property (retain)	GameObject*			gameObject;
@property BOOL							hasReflection;
@property CGFloat						rotationSpeed;
@end
