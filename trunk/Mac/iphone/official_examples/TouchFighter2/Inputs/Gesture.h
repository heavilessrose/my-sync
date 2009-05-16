/*
File: Gesture.h
Abstract: Analyse gesture.

Version: 2.0
*/

#import "TouchFighter_Types.h"
#import "GameObject.h"

typedef enum _GestureType {
	kGestureType_Swipe = 0,
	kGestureType_Rotate = 1,
	kGestureType_Unknown = 2,
	kGesture_Count
} GestureType;

@interface Gesture : NSObject {
	UITouch*		_touch;
	CGPoint*		_points,
					*_velocity,
					*_acceleration,
					_direction,
					_center;
	NSUInteger		_count, _capacity,
					_currentVertexIndex, _currentIndex;
	CGFloat			_alpha, 
					_curvature,
					_curvatureMean,
					_curvatureVar;
	BOOL			_needsUpdate;

	CGFloat			_gestureAngle;
	GestureType		_gestureType;
	
	Point3*			_vertices;
	Point4*			_colors;
	CGPoint*		_texCoords;
	CGFloat			_verticesCount;
	
}

- (void) addPoint:(CGPoint)point;
- (void) reset;

- (void) render;

@property(readonly) GestureType gestureType;
@property(readonly)	CGFloat		gestureAngle;
@property(readonly)	CGPoint		gestureCenter;
@property(readonly)	CGPoint		dPosition;
@property(retain) UITouch*		touch;
@end
