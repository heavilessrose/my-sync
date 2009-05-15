/*

===== IMPORTANT =====

This is sample code demonstrating API, technology or techniques in development.
Although this sample code has been reviewed for technical accuracy, it is not
final. Apple is supplying this information to help you plan for the adoption of
the technologies and programming interfaces described herein. This information
is subject to change, and software implemented based on this sample code should
be tested with final operating system software and final documentation. Newer
versions of this sample code may be provided with future seeds of the API or
technology. For information about updates to this and other developer
documentation, view the New & Updated sidebars in subsequent documentation
seeds.

=====================

File: PhysicalObject.m
Abstract: A subclass of GameObject that describes an object with physical properties.

Version: 2.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

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