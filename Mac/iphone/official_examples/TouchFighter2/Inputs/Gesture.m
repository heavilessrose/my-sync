/*
File: Gesture.m
Abstract: Analyse gesture.

Version: 2.0
*/

#import "Gesture.h"
#import "Maths.h"
#import "Texture2D.h"
#import "Utils.h"

#define RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) / M_PI * 180.0)
#define	INDEX_BORDER(I) ((I<3) ? 3 : I)
#define	Norm2(X, Y) (sqrt((X)*(X) + (Y)*(Y)))
#define	Dot2(X, Y) (X.x*Y.x+X.y*Y.y)
#define	Cross2(X, Y) (X.x*Y.y-X.y*Y.x)

#define kMinPointCount			1
#define	kDrawDepth				-0.75
#define	kDrawWidth				0.1
#define	kDrawStep				2
#define	kColorCoeff				1./5.
#define	kRenderColorR			1.
#define	kRenderColorG			0.2
#define	kRenderColorB			1.

//CONSTANTS:

static Texture2D*	_arrowTexture = nil;

//CLASS IMPLEMENTATIONS:

@implementation Gesture
@synthesize touch=_touch;

+ (id) allocWithZone:(NSZone*)zone
{
	_arrowTexture = [[Texture2D alloc] initWithImagePath:@"arrow.png"];
	return [super allocWithZone:zone];
}

- (id) init
{
	if ((self = [super init])) {
		_points = (CGPoint*)malloc(20*sizeof(CGPoint));
		_velocity = (CGPoint*)malloc(20*sizeof(CGPoint));
		_acceleration = (CGPoint*)malloc(20*sizeof(CGPoint));
		_capacity = 20;
		_count = 0;
		_gestureType = kGestureType_Unknown;
		_direction = CGPointZero;
		_center = CGPointZero;
	}
	
	return self;
}

- (void) dealloc
{
	free(_points);
	free(_velocity);
	free(_acceleration);
	if (_vertices)
	free(_vertices);
	if (_texCoords)
	free(_texCoords);
	if (_colors)
	free(_colors);
	
	[super dealloc];
}

- (void) addPoint:(CGPoint)p
{
	CGFloat			curvature, velocityNorm, alpha = 0.5;

	if (_count == _capacity - 1) {
		_capacity *= 2.;
		_points = (CGPoint*)realloc(_points, _capacity*sizeof(CGPoint));
		_velocity = (CGPoint*)realloc(_velocity, _capacity*sizeof(CGPoint));
		_acceleration = (CGPoint*)realloc(_acceleration, _capacity*sizeof(CGPoint));
	}
	
	_points[_count] = p;
	if (!_count)
	_velocity[_count] = CGPointZero;
	else {	
		if (_count > 0)
		_velocity[_count] = CGPointMake((p.x - _points[_count -1].x) * alpha + (1.-alpha) * _velocity[_count-1].x, (p.y - _points[_count -1].y) * alpha + (1.-alpha) * _velocity[_count-1].y);		
		else
		_velocity[_count] = CGPointMake(p.x - _points[_count -1].x , p.y - _points[_count -1].y);

		if (_count > 1)
		_acceleration[_count] = CGPointMake(_velocity[_count].x - _acceleration[_count -1].x , _velocity[_count].y - _acceleration[_count -1].y);
		else
		_acceleration[_count] = CGPointZero;		
	}
	_direction.x += _acceleration[_count].x;
	_direction.y += _acceleration[_count].y;
	_center.x += p.x;
	_center.y += p.y;
	
	if (_count > 1) {
		velocityNorm = Norm2(_velocity[_count].x, _velocity[_count].y);
		if (velocityNorm > 0.5) {
			curvature = ABS(_velocity[_count].x * _acceleration[_count].y - _velocity[_count].y * _acceleration[_count].x) / (velocityNorm*velocityNorm*velocityNorm);
			_curvatureMean += curvature;
			_curvatureVar += (curvature - _curvatureMean/_count) * (curvature - _curvatureMean/_count);
		}
	}
	
	_count++;
	_needsUpdate = YES;
}

- (void) updateValues
{
	CGPoint			vect1, vect2;
	CGFloat			norm1, norm2;

	if (_curvatureMean/_count + sqrt(_curvatureVar/_count) >= 0.1)
	_gestureType = kGestureType_Rotate;
	else
	_gestureType = kGestureType_Swipe;		
	
	switch (_gestureType) {
		case (kGestureType_Swipe):
			if (_direction.x) {
				_gestureAngle = RADIANS_TO_DEGREES(atan(_direction.y/_direction.x));
				_gestureAngle = (_direction.x >= 0) ? _gestureAngle : _gestureAngle + 180.;
			}
			else {
				_gestureAngle = ((_direction.y>=0) ? 90. : 270.);
			}
			break;
		case (kGestureType_Rotate):
			vect1 = CGPointMake(_points[0].x - _center.x, _points[0].y- _center.y);
			norm1 = Norm2(vect1.x, vect1.y);
			vect2 = CGPointMake(_points[_count].x - _center.x, _points[_count].y- _center.y);
			norm2 = Norm2(vect2.x, vect2.y);
			if (norm1 && norm2) {
				_gestureAngle = acos((vect1.x*vect2.x + vect1.y*vect2.y)/(norm1*norm2));
				if (vect1.x * vect2.y - vect1.y * vect2.x < 0)
				_gestureAngle = _gestureAngle + 270.;
			}
			break;
		default:break;
	}
	
	_gestureAngle = (_gestureAngle < 0.) ? _gestureAngle+360. : _gestureAngle;
	
	_needsUpdate = NO;
}

- (GestureType) gestureType
{
	if (_needsUpdate && (_count > kMinPointCount))
	[self updateValues];
	
	return _gestureType;
}

- (CGFloat) gestureAngle
{
	if (_needsUpdate && (_count > kMinPointCount))
	[self updateValues];
	
	return _gestureAngle;
}

- (CGPoint) gestureCenter
{
	return CGPointMake(_center.x/_count, _center.y/_count);
}

- (CGPoint) dPosition
{
	return CGPointMake(_points[_count-1].x - _points[0].x, _points[_count-1].y - _points[0].y);
}

- (void) reset
{
	_count = 0;
	_center = CGPointZero;
	_direction = CGPointZero;
	_gestureAngle = 0.;
	_gestureType = kGestureType_Unknown;
	_curvatureMean = 0.;
	_curvatureVar = 0.;	
	_currentVertexIndex = 0;
	_currentIndex = 0;
	_touch = nil;
}

- (void) _startTrail
{
	CGRect			rect = [[UIScreen mainScreen] bounds];
	CGFloat			norm, alpha;
	CGPoint			normal;
	
	norm = Norm2(_velocity[INDEX_BORDER(_currentIndex)].x, _velocity[INDEX_BORDER(_currentIndex)].y);
	normal = CGPointMake(_velocity[INDEX_BORDER(_currentIndex)].y/norm, _velocity[INDEX_BORDER(_currentIndex)].x/norm);
	
	alpha = Norm2(_acceleration[INDEX_BORDER(_currentIndex)].x, _acceleration[INDEX_BORDER(_currentIndex)].y)*kColorCoeff;
	_vertices[_currentVertexIndex] = Point3Make((_points[_currentIndex].x-rect.size.width/2.)/rect.size.height-kDrawWidth*(_velocity[INDEX_BORDER(_currentIndex)].x/norm + normal.x), -(_points[_currentIndex].y-rect.size.height/2.)/rect.size.height-kDrawWidth*(-_velocity[INDEX_BORDER(_currentIndex)].y/norm + normal.y), kDrawDepth);
	_texCoords[_currentVertexIndex] = CGPointMake(0., 1.);
	_colors[_currentVertexIndex] = Point4Make(kRenderColorR*alpha, kRenderColorG*alpha, kRenderColorB*alpha, alpha);
	_currentVertexIndex++;
	_vertices[_currentVertexIndex] = _vertices[_currentVertexIndex-1];
	_texCoords[_currentVertexIndex] = CGPointMake(0., 1.);
	_colors[_currentVertexIndex] = _colors[_currentVertexIndex-1];
	_currentVertexIndex++;
	_vertices[_currentVertexIndex] = Point3Make((_points[_currentIndex].x-rect.size.width/2.)/rect.size.height+kDrawWidth*(-_velocity[INDEX_BORDER(_currentIndex)].x/norm + normal.x), -(_points[_currentIndex].y-rect.size.height/2.)/rect.size.height+kDrawWidth*(_velocity[INDEX_BORDER(_currentIndex)].y/norm + normal.y), kDrawDepth);
	_texCoords[_currentVertexIndex] = CGPointMake(0., 0.);
	_colors[_currentVertexIndex] = _colors[_currentVertexIndex-1];
	_currentVertexIndex++;
}

- (void) _closeTrail
{
	CGRect			rect = [[UIScreen mainScreen] bounds];
	CGFloat			norm;
	CGPoint			normal;
	
	norm = Norm2(_velocity[_currentIndex].x, _velocity[_currentIndex].y);
	normal = CGPointMake(_velocity[_currentIndex].y/norm, _velocity[_currentIndex].x/norm);
	_vertices[_currentVertexIndex] = Point3Make((_points[_currentIndex].x-rect.size.width/2.)/rect.size.height-kDrawWidth*(-_velocity[_currentIndex].x/norm + normal.x), -(_points[_currentIndex].y-rect.size.height/2.)/rect.size.height-kDrawWidth*(_velocity[_currentIndex].y/norm + normal.y), kDrawDepth);
	_texCoords[_currentVertexIndex] = CGPointMake(1., 1.);
	_colors[_currentVertexIndex] = _colors[_currentVertexIndex-1];
	_currentVertexIndex++;
	_vertices[_currentVertexIndex] = Point3Make((_points[_currentIndex].x-rect.size.width/2.)/rect.size.height+kDrawWidth*(_velocity[_currentIndex].x/norm + normal.x), -(_points[_currentIndex].y-rect.size.height/2.)/rect.size.height+kDrawWidth*(-_velocity[_currentIndex].y/norm + normal.y), kDrawDepth);
	_texCoords[_currentVertexIndex] = CGPointMake(1., 0.);
	_colors[_currentVertexIndex] = _colors[_currentVertexIndex-1];
	_currentVertexIndex++;
	_vertices[_currentVertexIndex] = _vertices[_currentVertexIndex-1];
	_texCoords[_currentVertexIndex] = CGPointMake(0., 1.);
	_colors[_currentVertexIndex] = _colors[_currentVertexIndex-1];
	_currentVertexIndex++;
}

- (void) render
{	
	CGRect			rect = [[UIScreen mainScreen] bounds];
	CGFloat			norm;
	CGPoint			v1, v2, v3;
	CGPoint			normal;
	NSUInteger		savIndex, newCount;

	if (_count < kDrawStep)
	return;

	if (_count > _currentIndex) {
		if (_currentVertexIndex + 14*(_count-_currentIndex+1) >= _verticesCount) {
			newCount = _currentVertexIndex + 14*((_count-_currentIndex)/kDrawStep+1);
			if (!_vertices) {
				_vertices = malloc(newCount*sizeof(Point3));
				_texCoords = malloc(newCount*sizeof(CGPoint));
				_colors = malloc(newCount*sizeof(Point4));
				_verticesCount = newCount;
			}
			else {
				_vertices = realloc(_vertices, newCount*sizeof(Point3));
				_texCoords = realloc(_texCoords, newCount*sizeof(CGPoint));
				_colors = realloc(_colors, newCount*sizeof(Point4));
				_verticesCount = newCount;
			}
		}
		if (_count > 10)
		NSLog(@"");

		if(_currentIndex == 0)
		[self _startTrail];

		while (_currentIndex<_count) {
			norm = Norm2(_velocity[INDEX_BORDER(_currentIndex)].x, _velocity[INDEX_BORDER(_currentIndex)].y);
			normal = CGPointMake(_velocity[INDEX_BORDER(_currentIndex)].y/norm, _velocity[INDEX_BORDER(_currentIndex)].x/norm);
			
			norm = Norm2(_acceleration[INDEX_BORDER(_currentIndex)].x, _acceleration[INDEX_BORDER(_currentIndex)].y)*kColorCoeff;
			_vertices[_currentVertexIndex] = Point3Make((_points[_currentIndex].x-rect.size.width/2.)/rect.size.height-kDrawWidth*normal.x, -(_points[_currentIndex].y-rect.size.height/2.)/rect.size.height-kDrawWidth*normal.y, kDrawDepth);
			_texCoords[_currentVertexIndex] = CGPointMake(0.5, 1.);
			_colors[_currentVertexIndex] = Point4Make(kRenderColorR*norm, kRenderColorG*norm, kRenderColorB*norm, norm);
			_currentVertexIndex++;
			_vertices[_currentVertexIndex] = Point3Make((_points[_currentIndex].x-rect.size.width/2.)/rect.size.height+kDrawWidth*normal.x, -(_points[_currentIndex].y-rect.size.height/2.)/rect.size.height+kDrawWidth*normal.y, kDrawDepth);
			_texCoords[_currentVertexIndex] = CGPointMake(0.5, 0.);
			_colors[_currentVertexIndex] = Point4Make(kRenderColorR*norm, kRenderColorG*norm, kRenderColorB*norm, norm);
			_currentVertexIndex++;
					
			//Make sure that 2 new vertices do not "cross" the previous ones.
			v1 = CGPointMake(_vertices[_currentVertexIndex-3].x - _vertices[_currentVertexIndex-4].x, _vertices[_currentVertexIndex-3].y - _vertices[_currentVertexIndex-4].y);
			v2 = CGPointMake(_vertices[_currentVertexIndex-2].x - _vertices[_currentVertexIndex-4].x, _vertices[_currentVertexIndex-2].y - _vertices[_currentVertexIndex-4].y);
			v3 = CGPointMake(_vertices[_currentVertexIndex-1].x - _vertices[_currentVertexIndex-4].x, _vertices[_currentVertexIndex-1].y - _vertices[_currentVertexIndex-4].y);
			if (Cross2(v1, v2)*Cross2(v1, v3) < 0.) {			//Cross product: if opposed means the new vertices crosses the previous ones.
				_currentVertexIndex-=2;
				_currentIndex -= kDrawStep;
				[self _closeTrail];
				_currentIndex += kDrawStep;
				[self _startTrail];
			}
			else
			_currentIndex+=kDrawStep;
		}
	}	

	savIndex = _currentIndex;
	_currentIndex = _count-1;
	[self _closeTrail];
	_currentVertexIndex-=3;
	_currentIndex=savIndex;
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);			
	glEnableClientState(GL_COLOR_ARRAY);			

	[GameObject setVertexPointer:_vertices size:3 type:GL_FLOAT];
	[GameObject setTexcoordPointer:_texCoords size:2 type:GL_FLOAT];
	[GameObject setColorPointer:_colors size:4 type:GL_FLOAT];
	[GameObject bindTexture2D:StarTexture()];
	glDrawArrays(GL_TRIANGLE_STRIP, 0, _currentVertexIndex+3);	
	
	glDisable(GL_TEXTURE_2D);	
	glDisableClientState(GL_COLOR_ARRAY);			
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);			
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
/*
//	if (_gestureType != kGestureType_Unknown)
	_alpha = 0.2 * 1. + 0.8 * _alpha;
//	else
//	_alpha = 0.2 * 0. + 0.8 * _alpha;
	
	if (_alpha < 0.01)
	_alpha = 0.;
	
	if (_alpha) {
		p.x = (_center.x/_count-rect.size.width/2.)/rect.size.height;
		p.y = -(_center.y/_count-rect.size.height/2.)/rect.size.height;
		diag.x = ABS(p.x - (_points[0].x-rect.size.width/2.)/rect.size.height);
		diag.y = ABS(p.y + (_points[0].y-rect.size.height/2.)/rect.size.height);
		rect.origin.x = p.x - diag.x;
		rect.origin.y = p.y - diag.y;	
		rect.size.width = 2.* diag.x;
		rect.size.height = 2.* diag.y;		

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_TEXTURE_2D);	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		[_arrowTexture drawInRect:rect depth:kDrawDepth];
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);			
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}	*/
}

- (NSString*) description
{
	if (_needsUpdate && (_count > kMinPointCount))
	[self updateValues];
	
	return [NSString stringWithFormat:@"<%@ = 0x%08X | type = %@ | count = %i | center = (%f, %f) | direction = (%f, %f) | angle = %f | Curvature Mean = %f | Curvature Var = %f>", [self class], self, ((_gestureType == 1) ? @"Rotate" : ((_gestureType == 0) ? @"Swipe" : @"Unknown")), _count, _center.x/_count, _center.y/_count, _direction.x/_count, _direction.y/_count, _gestureAngle, _curvatureMean/_count, sqrt(_curvatureVar/_count)];
}

@end
