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

File: SpaceShip.h
Abstract: Main spaceship.

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

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "PhysicalObject.h"
#import "Maths.h"
#import "ParticleSystemObject.h"
#import <pthread.h>

#define kMissileWingAngle -40.

@class Texture2D, Missile, AnimationObject, Fire, SmokeTrail, Bomb, SpriteObject, ShipInfo;

// TYPEDEFS:

typedef enum _SpaceShipType {
	kSpaceShipTypeDart = 0,
	kSpaceShipTypeEnemy = 1,
	kSpaceShipTypeUnknown
} SpaceShipType;

typedef enum _SpaceShipLaserPosition {
	kSpaceShipLaserPosition_Left = 0,
	kSpaceShipLaserPosition_Right,
} SpaceShipLaserPosition;

typedef enum _SpaceShipMode {
	kSpaceShipModeNormal = 0,
	kSpaceShipModeMissileInitiated,
	kSpaceShipModeMissileFired,
	kSpaceShipModeBombCreated,
	kSpaceShipModeFinal
} SpaceShipMode;

//CLASS INTERFACES:

@interface SpaceShip : PhysicalObject {
@private
	SpaceShipType		_type;
	
	Texture2D*			_texture;
	
	//SpaceShip model and lasers
	Point3				_shipRotation,
						_shipPosition,
						_offset;
	Fire*				_propulsionFire;
	Fire*				_damages;
	CGFloat				_damage;

	//Crosshair and targetting
	SpriteObject*		_crosshair;
	Point3				_fireDirection,
						_firePosition;
	BOOL				_targetInFocus,
						_isFiring,
						_nextLasersAreTopLasers;
	GameObject*			_mothership;			//Not retained
	
	//Bullets
	CFMutableArrayRef	_lasers,
						_missiles;
	Bomb*				_bomb[2];

	//Wing effects
	CGFloat				_wingAngle,
						_wingDefaultAngle,
						_shipFallback;

	//Current mode of the spaceship (normal, final missile, ...)
	SpaceShipMode		_mode;

	//ModelView of the spaceship, used for converting lasers coordinates to the world referential
	Matrix				_modelview;
}

- (id) initWithType:(SpaceShipType)type;

/* Control */

- (void) fireLasersAtPosition:(SpaceShipLaserPosition)position;
- (void) fireMissileAtPosition:(SpaceShipLaserPosition)position withTarget:(GameObject*)target;
- (void) fireMissiles;
- (BOOL) willHit:(PhysicalObject*)object;

/* When being hit */

- (void) hitLeft;
- (void) hitRight;

/* Drawing */

- (void) renderCrosshairAtTime:(NSTimeInterval)time transformPosition:(Matrix)matrix;					//modelview needs to be CAMERA modelview for billboard rendering 
- (void) renderLasersAtTime:(NSTimeInterval)time;		
- (void) renderMissilesAtTime:(NSTimeInterval)time;
- (void) renderBombAtTime:(NSTimeInterval)time;

@property SpaceShipMode						mode;
@property BOOL								targetInFocus;
@property(readonly) CFMutableArrayRef		lasers;
@property(readonly) CFMutableArrayRef		missiles;
@property(retain) GameObject*				mothership;
@end

@interface ShipInfo	: PhysicalObject <NSCoding>
{
@public
	Point3			_shipPosition;
	NSArray*		_lasers;
	NSArray*		_missiles;
}
@property(retain)NSArray* lasers;
@property(retain)NSArray* missiles;
@property Point3 shipPosition;
@end
