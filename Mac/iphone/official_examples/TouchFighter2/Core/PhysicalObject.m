/*
File: PhysicalObject.m
Abstract: A subclass of GameObject that describes an object with physical properties.

Version: 2.0
*/

#import "PhysicalObject.h"
#import "GameObject_Private.h"
#import "Utils.h"

//CLASS IMPLEMENTATIONS:

@implementation PhysicalObject

@synthesize	velocity = _velocity, acceleration = _acceleration, friction = _friction;

- (id) init
{
	if ((self = [super init])) {
		//Initializes ivars.
		_velocity = _acceleration = Point3Zero;
	}
	
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder
{
	NSData*		data;
	
	[super encodeWithCoder:aCoder];

	data = [NSData dataWithBytes:&_velocity length:sizeof(Point3)];
	[aCoder encodeObject:data forKey:@"velocity"];
	data = [NSData dataWithBytes:&_acceleration length:sizeof(Point3)];
	[aCoder encodeObject:data forKey:@"acceleration"];
	data = [NSData dataWithBytes:&_friction length:sizeof(Point3)];
	[aCoder encodeObject:data forKey:@"friction"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	const void*		bytes;

	if ((self = [super initWithCoder:aDecoder])) {
		bytes = [(NSData*)[aDecoder decodeObjectForKey:@"velocity"] bytes];
		_velocity = *(Point3*)bytes;
		bytes = [(NSData*)[aDecoder decodeObjectForKey:@"acceleration"] bytes];
		_acceleration = *(Point3*)bytes;
		bytes = [(NSData*)[aDecoder decodeObjectForKey:@"friction"] bytes];
		_friction = *(Point3*)bytes;
	}
	return self;
}

- (void) updateWithGameObjectInfo:(PhysicalObject*)info
{
	[super updateWithGameObjectInfo:info];
	
	self.velocity = info.velocity;
	self.acceleration = info.acceleration;
	self.friction = info.friction;	
}

/* We update the object position and velocity using basic physics: dv = a*dt-v*friction + C, dp = v*dt */
- (void) updatePositionAtTime:(NSTimeInterval)time
{
	unsigned			i;
	
	if (_previousRenderTime < 0.) {
		_previousRenderTime = time;
	}
	_velocity = Point3Add(_velocity, Point3Mult(_acceleration, time - _previousRenderTime));
	for (i=0; i<3; ++i)
	POINT3_COORD_AT_INDEX(_velocity, i) = Sign(POINT3_COORD_AT_INDEX(_velocity, i)) * MAX(ABS(POINT3_COORD_AT_INDEX(_velocity, i)) - ABS(POINT3_COORD_AT_INDEX(_velocity, i))*POINT3_COORD_AT_INDEX(_friction, i), 0.f);	
	_position = Point3Add(_position, Point3Mult(_velocity, time - _previousRenderTime));
	
	_previousRenderTime = time;
}

/* To render the object, we first update its position, and then render it at that position */
- (void) renderAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix
{
	[self updatePositionAtTime:time];
	[super renderAtTime:time transformPosition:matrix];
}

@end