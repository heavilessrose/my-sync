/*
File: TouchFighterView.m
Abstract: Subclass of EAGLView that handles game state update and rendering.

Version: 2.0
*/

#import "TouchFighterView.h"
#import "TouchFighterAppDelegate.h"
#import "AccelerometerSmoother.h"
#import "Gesture.h"
#import "GameObject_Private.h"
#import "PhysicalObject.h"
#import "SpaceShip.h"
#import "EnemyShip.h"
#import "Planet.h"
#import "StarField.h"
#import "Explosion.h"
#import "CubeMap.h"
#import "Bullets.h"
#import "Utils.h"
#import "GamePeer.h"
#import "Meteorite.h"
#if !TARGET_IPHONE_SIMULATOR
#import "SoundEngineManager.h"
#endif

//CONSTANTS:

#define kShowFPSDefaultKey					@"fps"
#define kInvertControlFPSDefaultKey			@"invert"
#define kSensitivityDefaultKey				@"sensitivity"
#define kGesturesDefaultKey					@"gestures"

#define kStarMaxCount						120
#define kStarDefaultZSpeed					25.
#define kMaxParticles						10

#define kEnemyMaxCount						10
#define kEnemyAttackSpeed					1.
#define kEnemyLaserDamage					30.
#define kEnemyMaxDamage						100.
#define kEnemyDamageSpeed					20.

#define kTitleFadeTime						20
#define kGameFadeTime						15

#define kMotherShipX						5.
#define kMotherShipY						-5.
#define kMotherShipZ						-50.
#define kPeerZ								-0.5

#define kMaxTouches							3

#define kEnemyBonus							50
#define kHitPenaltyRatio					1.2

#define kMultiplayerShipDistance			1.25

//CLASS INTERFACES:

@interface TouchFighterView(Internal)

/* Camera set-up */
- (void) setUpCamera;

/* That's where all the objects states are updated */
- (void) updateScene;

/* That's where all the objects are rendered */
- (void) renderScene;

/* Main Game Loop */
- (void) gameLoop;
@end

@interface TouchFighterView(Extras)

/* Title screen */
- (void) drawTitleScreen;

/* Peer choice screen */
- (void) drawMultiplayerScreen;

/* Fade form black at game start */
- (void) renderFade;

/* Render a FPS counter */
- (void) renderFPS;

/* Render score */
- (void) renderScore;

/* Internal function for fading */
- (void) fadeToR:(float)tr G:(float)tg B:(float)tb A:(float)ta fromR:(float)fr G:(float)fg B:(float)fb A:(float)fa withFrameCount:(int)count;
@end

@implementation TouchFighterView
@synthesize gameOver=_gameOver, pause=_paused, showFPS=_renderFPS, showGesture=_renderGestures, score=_score;

- (id) initWithFrame:(CGRect)rect pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained
{
	const GLfloat			lightAmbient[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			matAmbient[] = {0.3, 0.3, 0.35, 1.0};
	const GLfloat			matDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			matSpecular[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat			lightPosition[] = {3.5, -0.3, 1.0, 0.0};
	const GLfloat			lightShininess = 100.0;
	Texture2D*				texture;
	PhysicalObject*			enemy;
	Gesture*				gesture;
	Point3					gravityPoint;
	NSUInteger				i;

	if((self = [super initWithFrame:rect pixelFormat:format depthFormat:depth preserveBackbuffer:retained])) {
		//Initialize ivars.
		_titleFade = kTitleFadeTime;
		_fadeTime = 0;
		_fadeFrame = -1;
		_fadeTex = nil;
		_gameOver = NO;
		_atTitle = YES;
		_worldRotation = 0.;
		
		//Initiliazing GL states and ligth.
		glEnable(GL_LIGHT0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); 			
		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);
		glDisable(GL_DEPTH_TEST);
		
		//Initialize main title texture containing "Touch to Start message".
		_titleTexture = [[Texture2D alloc] initWithImagePath:@"Title.png"];
		CHECK_CREATION(_titleTexture);	
		[GameObject bindTexture2D:_titleTexture];
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		//Setting-up camera.
		[self setUpCamera];

		//Initiliaze starfield.
		gravityPoint.x = -340.0; gravityPoint.y = 0.0; gravityPoint.z = -100.0;
		_stars = [[StarField alloc] initWithGravityPoint:gravityPoint texture:[[[Texture2D alloc] initWithImagePath:@"startube.png"] autorelease] count:kStarMaxCount];
		CHECK_CREATION(_stars);
		[_stars setDirectionX:kStarDefaultZSpeed Y:0.0 Z:0.0];
		
		//Initiliaze explosion.
		_explosionTexture = [[Texture2D alloc] initWithImagePath:@"explosion.png"];

		//Initiliaze background cubemap.
#if !TARGET_IPHONE_SIMULATOR
		_cubeMap = [[CubeMap alloc] initWithTextures:[[[Texture2D alloc] initWithPVR4DataFile:@"posx.pvr4" pixelsWide:512 pixelsHigh:512] autorelease]
													:[[[Texture2D alloc] initWithImagePath:@"posy256.png"] autorelease] 
													:[[[Texture2D alloc] initWithPVR4DataFile:@"posz.pvr4" pixelsWide:512 pixelsHigh:512] autorelease]
													:[[[Texture2D alloc] initWithPVR4DataFile:@"negx.pvr4" pixelsWide:512 pixelsHigh:512] autorelease]
													:[[[Texture2D alloc] initWithImagePath:@"negy256.png"] autorelease]  
													:[[[Texture2D alloc] initWithPVR4DataFile:@"negz.pvr4" pixelsWide:512 pixelsHigh:512] autorelease]];
#else
		_cubeMap = [[CubeMap alloc] initWithTextures:[[[Texture2D alloc] initWithImagePath:@"posx.png"] autorelease] 
													:[[[Texture2D alloc] initWithImagePath:@"posy.png"] autorelease] 
													:[[[Texture2D alloc] initWithImagePath:@"posz.png"] autorelease]
													:[[[Texture2D alloc] initWithImagePath:@"negx.png"] autorelease]
													:[[[Texture2D alloc] initWithImagePath:@"negy.png"] autorelease] 
													:[[[Texture2D alloc] initWithImagePath:@"negz.png"] autorelease]];
#endif
		CHECK_CREATION(_cubeMap);
		
		//Initiliaze matrices
		MakeTransformationMatrix(_matrix, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		MakeTransformationMatrix(_transformationMatrix, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		_transformationRotation = Point3Zero;

		//Initiliaze planet
#if !TARGET_IPHONE_SIMULATOR
		texture = [[Texture2D alloc] initWithPVR4DataFile:@"venus-closeup.pvr4" pixelsWide:512 pixelsHigh:512];
#else
		texture = [[Texture2D alloc] initWithImagePath:@"venus-closeup.png"];
#endif
		CHECK_CREATION(texture);
		_planet = [[Planet alloc] initWithTexture:texture glowTexture:[[[Texture2D alloc] initWithImagePath:@"planet_glow.png"] autorelease] stacks:20 slices:30];
		CHECK_CREATION(_planet);
		_planet.position = Point3Make(gravityPoint.x, gravityPoint.y, gravityPoint.z);
		_planet.rotation = Point3Make(0., 90., 0.);
		_planet.scale = Point3Make(300., 300., 300.);
		[texture release];
		
		_meteorite = [[Meteorite alloc] initWithTexture:[[[Texture2D alloc] initWithImagePath:@"moon.png"] autorelease] stacks:10 slices:10];
		CHECK_CREATION(_meteorite);
		_meteorite.position = Point3Make(20., 30., -80.);
		_meteorite.scale = Point3Make(2., 2., 2.);
		_meteorite.velocity = Point3Make(-30.,-90., 150.);
		
		//Initiliaze mothership
		_motherShip = [MotherShip new];
		CHECK_CREATION(_motherShip);
		_motherShip.scale = Point3Make(30., 30., 20.);
		_motherShip.position = Point3Make(kMotherShipX, kMotherShipY, kMotherShipZ);
		_motherShip.rotation = Point3Make(125., -20., -90.);

		//Initiliaze spaceship
		_ship = [[SpaceShip alloc] initWithType:kSpaceShipTypeDart];
		CHECK_CREATION(_ship);
		_ship.mothership = _motherShip;
		
		//Initiliaze enemies
		_enemies = CFArrayCreateMutable(kCFAllocatorDefault, 10, &kCFTypeArrayCallBacks);
		_explosions = CFArrayCreateMutable(kCFAllocatorDefault, 10, &kCFTypeArrayCallBacks);
		for (i=0; i<kEnemyMaxCount; ++i) {
			enemy = [EnemyShip new];
			CHECK_CREATION(enemy);
			CFArrayAppendValue(_enemies, enemy);
			enemy.scale = Point3Make(3./8., 3./8., 3./8.);
			enemy.position = Point3Make(_motherShip.position.x, _motherShip.position.y, _motherShip.position.z);
			enemy.velocity = Point3Make(cos((float)i/kEnemyMaxCount*M_PI - M_PI/2.) * 40., sin((float)i/kEnemyMaxCount*M_PI - M_PI/2.) * 50., 0.0);
			enemy.acceleration = Point3Make(9.*kEnemyAttackSpeed*(sin(i)-1.), 0.0, 18./30. * kEnemyAttackSpeed*(cos(i)+2));
			enemy.rotation = Point3Make(0., 0., 270.);
			[enemy release];
		}
		
		//Accelerometer
		_accelerometer = [AccelerometerSmoother new];
		CHECK_CREATION(_accelerometer);
		_accelerometer.sensitivity = 1.;
		_accelerometer.smoothing = 10.;
#ifndef __INVERT_CONTROL__
		_accelerometer.invert = NO;
#endif

		_gestures = CFArrayCreateMutable(kCFAllocatorDefault, kMaxTouches, &kCFTypeArrayCallBacks);
		for (i=0; i<kMaxTouches; ++i) {
			gesture = [Gesture new];
			CFArrayAppendValue(_gestures, gesture);
			[gesture release];
		}
		
		//Initialize particle system cache
		[RenderingManager initPtCache];

		//Restore saved state.
		self.showFPS = [[NSUserDefaults standardUserDefaults] boolForKey:kShowFPSDefaultKey];
		self.invertControls = [[NSUserDefaults standardUserDefaults] boolForKey:kInvertControlFPSDefaultKey];
		self.showGesture = [[NSUserDefaults standardUserDefaults] boolForKey:kGesturesDefaultKey];
		if ([[NSUserDefaults standardUserDefaults] objectForKey:kSensitivityDefaultKey])
		self.accelerometerSensitivity = [[NSUserDefaults standardUserDefaults] floatForKey:kSensitivityDefaultKey];
		else
		self.accelerometerSensitivity = 1.;
	}
	
	return self;
}

void _PeerRenderFunction(const void *key, const void *value, void *context)
{
	SpaceShip*			peer = (SpaceShip*)value;
	NSTimeInterval		time = *(NSTimeInterval*)context;
	
	if (((TouchFighterAppDelegate*)[UIApplication sharedApplication].delegate).isHost) {
		glTranslatef(0., 0., kPeerZ);	
		[peer renderAtTime:time];
		[peer renderLasersAtTime:time];
		[peer renderMissilesAtTime:time];
		glTranslatef(0., 0., -kPeerZ);
	}
}

/* That's where we update our scene (object locations, collisions, ...). */
- (void) updateScene
{
	CFMutableArrayRef		lasers, missiles;
	Laser*					laser;
	Missile*				missile;
	EnemyLaser*				enemyLaser;
#if !TARGET_IPHONE_SIMULATOR
	Sound*					sound;
#endif
	EnemyShip*				enemy;
	Explosion*				explosion;
	Point3					bayPos, shipPosition;
	CGFloat					random,
							speed = 1.5, 
							smoothing = 0.1,
							angleMult = 200.;
	NSInteger				i, j, k;
	BOOL					regenerate = NO;
	
	//Adjust cubemap rotation.
	_worldRotation -= 5. * (_renderTime - _previousRenderTime);
	
	//Rotate mothership.
	_motherShip.rotation = Point3Make(125. + 20.*cos(_renderTime/5.), _motherShip.rotation.y, _motherShip.rotation.z);

	//Get position of mothership bay where enemy ships exit from.
	bayPos = _motherShip.bayOffset;
	
	//Update ship position and world tranformation from accelerometer.
	shipPosition = _accelerometer.position;
	
	//Compute camera rotation from ship position
	_transformationRotation = Point3Add(Point3Mult(shipPosition, smoothing), Point3Mult(_transformationRotation, (1. - smoothing)));
	MakeTransformationMatrix(_transformationMatrix, angleMult * -_transformationRotation.x, angleMult * _transformationRotation.y, _cameraRotation, 0., 0., 0.);
	
	//Update ship position
	_ship.position = Point3Make(-RadiansToDegrees(shipPosition.y * speed), -RadiansToDegrees(shipPosition.x * speed), -1.);

	//Collision detection between the ship lasers or missiles and each enemy.
	lasers = _ship.lasers;
	missiles = _ship.missiles;
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemy = (EnemyShip*)CFArrayGetValueAtIndex(_enemies, i);

		if (enemy.position.z >= 20.) {		//Regenerates eney ship when behind camera and main ship.
			regenerate = YES;
		}
		else {	//Check is a laser hit that enemy
			for (j=CFArrayGetCount(lasers)-1; j>=0; --j) {
				laser = (Laser*)CFArrayGetValueAtIndex(lasers, j);
				
				//Collision detection
				if ([laser collideWith:enemy]) {
					//If hit, increase enemy ship damage, play sound and destroy associated laser.
					enemy.damage += kEnemyLaserDamage;
#if !TARGET_IPHONE_SIMULATOR
					sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"laserBoom"];
					sound.position = Point3Make(enemy.position.x/12., enemy.position.y/12., enemy.position.z/12.);
					[sound play];
#endif
					CFArrayRemoveValueAtIndex(lasers, j);
					
					break;		
				}
			}

			if (_ship.mode < kSpaceShipModeMissileInitiated) {
				for (j=CFArrayGetCount(missiles)-1; j>=0; --j) {
					missile = (Missile*)CFArrayGetValueAtIndex(missiles, j);
					
					//Collision detection
					if ([missile collideWith:enemy]) {
						//If hit, increase enemy ship damage, play sound and destroy associated laser.
						enemy.damage = kEnemyMaxDamage;
#if !TARGET_IPHONE_SIMULATOR
						sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"missileBoom"];
						sound.position = Point3Make(enemy.position.x/12., enemy.position.y/12., enemy.position.z/12.);
						[sound play];
#endif
						CFArrayRemoveValueAtIndex(missiles, j);
						
						break;		
					}
				}
			}
				
			//If enemy got hit, increase its damages over time.
			if (enemy.damage) {
				enemy.damage += (_renderTime - _previousRenderTime) * kEnemyDamageSpeed;
			}

			//If enemy damages are over kEnemyMaxDamage threshold, create an explosion
			if (((enemy.damage > kEnemyMaxDamage) || (enemy.damage && (_ship.mode == kSpaceShipModeBombCreated)))) {
				_score += kEnemyBonus;
				
				//Initialize an explosion
				explosion = [[Explosion alloc] initWithTexture:_explosionTexture frameCount:16];
				explosion.scale = Point3Make(10., 10., 1.);
				
				//Set the explosion velocity and position to the destroyed enemy's
				explosion.velocity = enemy.velocity;
				explosion.position = enemy.position;
				
				//Inserts in a back-to-front manner for better blending
				k = CFArrayGetCount(_explosions)-1;
				while (k >= 0 && ((Explosion *)CFArrayGetValueAtIndex(_explosions, k)).position.z < explosion.position.z)
				k--;
				CFArrayInsertValueAtIndex(_explosions, k+1, explosion);
				[explosion release];
				
#if !TARGET_IPHONE_SIMULATOR
				//Play explosion sound
				sound = [[SoundEngineManager sharedManager] soundWithIdentifier:@"missileBoom"];
				sound.position = Point3Make(explosion.position.x/12., explosion.position.y/12., explosion.position.z/12.);
				[sound play];
#endif
				
				//Generate a new enemy if we're not in the final sequence
				regenerate = (_ship.mode == kSpaceShipModeNormal);
				if (!regenerate) {
					CFArrayRemoveValueAtIndex(_enemies, i);
				}
			}
		}
		//Generate a new ennemy
		if (regenerate) {
			random = fast_unit_rand();
			enemy.position = Point3Make(_motherShip.position.x + bayPos.x, _motherShip.position.y + bayPos.y, _motherShip.position.z + bayPos.z);
			enemy.velocity = Point3Make(sin(random*M_PI - M_PI/2.) * 20., sin(random*M_PI - M_PI/2.) * 30., 0.0);
			enemy.acceleration = Point3Make(9.*kEnemyAttackSpeed*(sin(random*kEnemyMaxCount)-1.), 0.0, 18./30. * kEnemyAttackSpeed*(cos(random*kEnemyMaxCount)+2.));
			enemy.damage = 0.;
			regenerate = NO;
		}
		//If not at title or in end sequence, is enemy is close enough and not damages, make the enemy ship fire lasers with a 0.998 probability
		else if ((!_multiplayer && !_isHost) && !_titleFade && (_ship.mode == kSpaceShipModeNormal) && (enemy.position.z < -30.) && (fast_unit_rand() > 0.998) && !enemy.damage) {
			[enemy fireAtPointX:_ship.position.x Y:_ship.position.y Z:_ship.position.z];
		}
	}	
	
	//Collision detection between lasers from enemy ships and main ship
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemyLaser = ((EnemyShip*)CFArrayGetValueAtIndex(_enemies, i)).laser;
		
		if (enemyLaser && [_ship collideWith:enemyLaser] && (_ship.mode == kSpaceShipModeNormal)) {
			_score /= kHitPenaltyRatio;
			if (enemyLaser.position.y > _ship.position.y) {
				[_ship hitRight];			//Ship hit at the right (jumps to the left)
			 } else {
				[_ship hitLeft];			//Ship hit at the left (jumps to the right)
			}
		}
	}
	
	if (!_meteorite.alive && (_renderTime- _lastMeteoriteTime > 10.)) {
		_lastMeteoriteTime = _renderTime;
		[_meteorite reset];
	}
}

/* That's where we render all objects. */
- (void) renderScene
{
	EnemyShip*						enemy;
	Explosion*						explosion;
	Matrix							m;
	NSInteger						i, j;
										
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT);			//Depth buffer is cleared by cube map so there's no need to do it here.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Store the camera matrix that we'll use for rendering billboards among other things.
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	//Apply camera transformation	.
	glMultMatrixf(_transformationMatrix);

/* From now on, we're in the WORLD referential */

	glEnableClientState(GL_VERTEX_ARRAY);

	//Render cubemap
	glPushMatrix(); {
		glRotatef(_worldRotation, 0., 1., 0.);
		glEnable(GL_DEPTH_TEST);
		[_cubeMap renderAtTime:_renderTime];	
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();

	//Render starfield.
	glDepthRangef(.5, 1.);
	[_stars renderAtTime:_renderTime];	
	
	//Render planet.
	[_planet renderAtTime:_renderTime];

	//Render mothership.
	glEnable(GL_DEPTH_TEST);
	[_motherShip renderAtTime:_renderTime];
	glDepthRangef(0., .5);

	//Render enemies.
	_ship.targetInFocus = NO;
	[_targetInFocus release];
	_targetInFocus = nil;
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemy = (EnemyShip*)CFArrayGetValueAtIndex(_enemies, i);
		if (_fadeFrame < kGameFadeTime) {
			[enemy renderAtTime:_renderTime];
		}
		enemy.highlight = NO;
		if ((_ship.mode == kSpaceShipModeNormal) && [_ship willHit:enemy]) {
			_ship.targetInFocus = YES;	
			enemy.highlight = YES;
			_targetInFocus = [enemy retain];
		}		
	}

	if (_multiplayer && _peers && CFDictionaryGetCount(_peers)) {
		CFDictionaryApplyFunction(_peers, _PeerRenderFunction, &_renderTime);
	}

	//Render meteorite.
	if (!_multiplayer && _meteorite.alive) {
		[_meteorite renderAtTime:_renderTime];
	}

	glDisable(GL_DEPTH_TEST);
			
	//Render Turning on particle caches.
	[RenderingManager enablePtCache];
					
	//Render Enemy lasers.
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemy = (EnemyShip*)CFArrayGetValueAtIndex(_enemies, i);
		[enemy renderLasersAtTime:_renderTime];
	}	
	[RenderingManager flushPtCache];		//Flush so that all particle are drawn right now (important for blending)
	[RenderingManager disablePtCache];
			
	//Render ship lasers
	[_ship renderLasersAtTime:_renderTime];
	
	//Render final missiles if needed
	if (_ship.mode < kSpaceShipModeFinal)
	[_ship renderMissilesAtTime:_renderTime];
	
	glLoadMatrixf(m);	

/* From now on, we're back the CAMERA referential */
	
	//Enemy highlights, damages, explosions
	[RenderingManager enablePtCache];
	//Render flames and fire for damaged enemies
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemy = (EnemyShip*)CFArrayGetValueAtIndex(_enemies, i);
		if (enemy.damage) {
			[enemy renderDamageFlamesAtTime:_renderTime transformPosition:_transformationMatrix];
		}
	}
	if (_meteorite.alive)
	[_meteorite renderFireTrailAtTime:_renderTime transformPosition:_transformationMatrix];

	[RenderingManager flushPtCache];		//Draw particles
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemy = (EnemyShip*)CFArrayGetValueAtIndex(_enemies, i);
		if (enemy.damage) {
			[enemy renderDamageFireAtTime:_renderTime transformPosition:_transformationMatrix];
		}
	}	
	[RenderingManager flushPtCache];

	for (j=CFArrayGetCount(_explosions)-1; j>=0; --j) {
		explosion = (Explosion*)CFArrayGetValueAtIndex(_explosions, j);
		if (!explosion.alive) {
			CFArrayRemoveValueAtIndex(_explosions, j);
		} else {
			[explosion renderAtTime:_renderTime transformPosition:_transformationMatrix];
		}
	}	
	[RenderingManager flushPtCache];		//Draw particles
	[RenderingManager disablePtCache];

	//Render highlight for enemies in focus.
	for (i=CFArrayGetCount(_enemies)-1; i>=0; --i) {
		enemy = (EnemyShip*)CFArrayGetValueAtIndex(_enemies, i);
		if (enemy.highlight) {
			[enemy renderHighlight];
		}
	}

	//Render crosshair.
	[_ship renderCrosshairAtTime:_renderTime transformPosition:_transformationMatrix];

	//Render main ship.
	glEnable(GL_DEPTH_TEST);
	[_ship renderAtTime:_renderTime];	
	glDisable(GL_DEPTH_TEST);
			
	if (_renderFPS)
	[self renderFPS];

	[self renderScore];
	
	if (_renderGestures) {
		for (i=0; i<kMaxTouches; ++i)
		[(Gesture*)CFArrayGetValueAtIndex(_gestures, i) render];
	}

	//Render bomb if needed
	if (_ship.mode == kSpaceShipModeBombCreated) {
		glMultMatrixf(_transformationMatrix);
		[_ship renderBombAtTime:_renderTime];	
	}

	glDepthRangef(0., 1.);

	// Handle end-of-game
	if (_ship.mode == kSpaceShipModeFinal) {
		// fade to white--at the end fadeFrame will be stuck at 0 to maintain the white
		if ((_fadeFrame < 0) && !_gameOver) {
			[self fadeToR:1. G:1. B:1. A:1. fromR:1. G:1. B:1. A:0. withFrameCount:0];
		}
		else if (_fadeFrame == 0) {
			_gameOver = YES;
		}
	}

	// Render fade form black when game starts
	if (_fadeFrame >= 0) {
		[self renderFade];
	}
	else
	_state = kState_Game;

	glDisableClientState(GL_VERTEX_ARRAY);
}

/* Main game loop: updates and renders scene*/
- (void) gameLoop
{
	//We calibrate accelerometer when title is fading out
	_accelerometer.calibrate = (_state == kState_TitleFadeOut);
	[_accelerometer update];
	
	if((_state == kState_Title) || (_state <= kState_TitleFadeOut)) { 		//_titleFade > 0 means we're still at the title screen or fading it out
		[self drawTitleScreen];
		if (_titleFade == 0) {
			if (_multiplayer) {
				_state = kState_MultiPlayer;
			}
			else {
				_state = kState_GameFadeIn;
			}
		}
		[self swapBuffers];	
	}
	else if (_state <= kState_MultiPlayer) {
		if (((TouchFighterAppDelegate*)[UIApplication sharedApplication].delegate).multiplayer) {
			[self drawMultiplayerScreen];
			[self swapBuffers];	
		}
	}
	else {
		if (_paused) {
			_previousRenderTime = _renderTime;
			_startTime += [NSDate timeIntervalSinceReferenceDate] - _startTime - _renderTime;
		}
		else {
			//We're updating current time.
			_previousRenderTime = _renderTime;
			if (_startTime == 0) {
				_startTime = [NSDate timeIntervalSinceReferenceDate];
			}
			_renderTime = [NSDate timeIntervalSinceReferenceDate] - _startTime;

			[self updateScene];
			[self renderScene];
			[self swapBuffers];		
		}
	}
}

/* We set-up the GL camera as perspective */
- (void) setUpCamera
{
	CGRect			rect = [[UIScreen mainScreen] bounds];
	const GLfloat	zNear = 0.5,
					zFar = 2000.0,
					fieldOfView = 60.0;
	GLfloat size = zNear * tanf(DegreesToRadians(fieldOfView) / 2.0);
	size *= 0.8;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumf(-size, size, -size / (rect.size.width / rect.size.height), size / (rect.size.width / rect.size.height), zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

- (void) dealloc
{
	[_stars release];
	[_ship release];
	if (_enemyShip) {
		[_enemyShip release];
	}
	[_explosionTexture release];
	[_planet release];
	[_meteorite release];
	[_motherShip release];
	[_accelerometer release];
	if (_tableView) {
		[_tableView release];
	}
	if (_titleBar) {
		[_titleBar release];
	}
	
	[_fpsTexture release];
	[_fadeTex release];
	
	if (_enemies)
	CFRelease(_enemies);
	if (_explosions)
	CFRelease(_explosions);
	if (_gestures)
	CFRelease(_gestures);
	if (_peers)
	CFRelease(_peers);
	
	ShipTextureRelease();
	EnemyShipTextureRelease();
	StarTextureRelease();
	FlameTextureRelease();	
	MissileTextureRelease();	
	
	[super dealloc];
}

- (CGFloat) accelerometerSensitivity
{
	return _accelerometer.sensitivity;
}

- (void) setAccelerometerSensitivity:(CGFloat)value
{
	if (_accelerometer.sensitivity != value) {
		[[NSUserDefaults standardUserDefaults] setFloat:value forKey:kSensitivityDefaultKey];
		_accelerometer.sensitivity = value;
	}
}

- (BOOL) invertControls
{
	return _accelerometer.invert;
}

- (void) setInvertControls:(BOOL)flag
{
	if (_accelerometer.invert != flag) {
		[[NSUserDefaults standardUserDefaults] setBool:flag forKey:kInvertControlFPSDefaultKey];
		_accelerometer.invert = flag;
	}
}

- (void) setShowFPS:(BOOL)flag
{
	if (_renderFPS != flag) {
		_renderFPS = flag;
		[[NSUserDefaults standardUserDefaults] setBool:flag forKey:kShowFPSDefaultKey];	
	}
}

- (void) setShowGesture:(BOOL)flag
{
	if (_renderGestures != flag) {
		_renderGestures = flag;
		[[NSUserDefaults standardUserDefaults] setBool:flag forKey:kGesturesDefaultKey];	
	}
}

- (void) updatePeers
{
	if (_tableView)
	[_tableView reloadData];
}

- (void) startMultiplayer:(GamePeer*)peer
{
	CGRect					rect = [[UIScreen mainScreen] bounds];	
	
	_state = kState_GameFadeIn;
	
	[UIView beginAnimations:nil context:NULL];
	self.center =  CGPointMake(rect.size.width/2., rect.size.height*1./2.);
	[_tableView removeFromSuperview];
	[_tableView release];
	_tableView = nil;
	[_titleBar removeFromSuperview];
	[_titleBar release];
	[UIView commitAnimations];
	
	if (_selectedShip%2) {
		[_ship release];
		_ship = [_enemyShip retain];
		[_enemyShip release];
		_enemyShip = nil;
	}
}

- (id) gameData
{
	NSMutableDictionary*		dic = [NSMutableDictionary new];
	NSMutableArray*				array = [NSMutableArray new];
	NSUInteger					i;
	
	[dic setObject:[_ship gameObjectInfo] forKey:@"ship"];
	
	for (i=0; i<CFArrayGetCount(_enemies); ++i) {
		[array addObject:[(EnemyShip*)CFArrayGetValueAtIndex(_enemies, i) gameObjectInfo]];
	}
	[dic setObject:array forKey:@"enemies"];
	[array release];
	
	//If server set game data.
	return [dic autorelease];
}

- (void) setGameData:(NSDictionary*)gameData forPeer:(GamePeer*)peer
{
	GameObject*		ship, *object;
	NSArray*		enemies;
	NSUInteger		i;
	
	if (peer.server) {
		enemies = [gameData objectForKey:@"enemies"];
		for (i=0; i<MIN([enemies count], CFArrayGetCount(_enemies)); ++i) {
			[(EnemyShip*)CFArrayGetValueAtIndex(_enemies, i) updateWithGameObjectInfo:[enemies objectAtIndex:i]];
		}
	}
	if (_peers && (ship = (GameObject*)CFDictionaryGetValue(_peers, peer))) {
		object = [gameData objectForKey:@"ship"];
		[(SpaceShip*)ship updateWithGameObjectInfo:object];
	}
}

- (void) setShipType:(SpaceShipType)type forPeer:(GamePeer*)peer
{
	SpaceShip*		enemy;
	
	if (!_peers)
	_peers = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	if ((enemy = (SpaceShip*)CFDictionaryGetValue(_peers, peer)) && (type == kSpaceShipTypeUnknown)) {
		CFDictionaryRemoveValue(_peers, peer);
	}
	else {
		enemy = [[SpaceShip alloc] initWithType:type];
		CFDictionarySetValue(_peers, peer, enemy);
		[enemy release];
	}
}

- (SpaceShipType) shipType
{
	return ((_selectedShip%2) ? kSpaceShipTypeEnemy : kSpaceShipTypeDart);
}

@end

@implementation TouchFighterView(Events)

- (void) _handleTouchAtLocation:(CGPoint)location tapCount:(NSUInteger)tapCount {
	CGRect				rect = [[UIScreen mainScreen] bounds];
	
	if (_titleFade <= 0.) {
		[_ship fireLasersAtPosition: (location.y < rect.size.height / 2) ? kSpaceShipLaserPosition_Left : kSpaceShipLaserPosition_Right];
	}
}

/* New touch events */
- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event {	
	CGRect			rect = [[UIScreen mainScreen] bounds];
	NSArray*		array = [touches allObjects];
	Gesture*		gesture;
	CGPoint			location;
	UITouch*		touch;
	NSUInteger		i, j;

	//Do not handle touch if we're at final sequence
	if (_ship.mode != kSpaceShipModeNormal) {
		return;
	}	
	
	//Start title fade-out at first tap.
	if(_state == kState_Title) {
		touch = [array objectAtIndex:0];
		location = [touch locationInView:self];
		if (location.y > rect.size.height / 2) {
			((TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate]).multiplayer = YES;
			_enemyShip = [[SpaceShip alloc] initWithType:kSpaceShipTypeEnemy];
			[_meteorite release];
			_meteorite = nil;
			_multiplayer = YES;
		}
		_titleFade = kTitleFadeTime-1;
		_state = kState_TitleFadeOut;
	}
	else {
		//Otherwise, launch missiles or lasers
		for (i=0; i<MIN([array count], kMaxTouches); ++i) {
			touch = [array objectAtIndex:i];
			location = [touch locationInView:self];
			if ([touch tapCount] == 1) {
				for (j=0; j<kMaxTouches; ++j) {
					gesture = (Gesture*)CFArrayGetValueAtIndex(_gestures, j);
					if (!gesture.touch) {
						gesture.touch = touch;
						[gesture addPoint:location];
						break;
					}
				}
			}
			else if (_state != kState_MultiPlayer) {
				[self _handleTouchAtLocation:location tapCount:[touch tapCount]];
			}
		}
	}
}

/* Updated touch event that we analyse for second of third fingers entry */
- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{	
	NSArray*		array = [touches allObjects];
	UITouch*		touch;
	Gesture*		gesture;
	CGPoint			location;
	NSUInteger		i, j;
	
	//Do not handle touch if we're at final sequence
	if (_ship.mode != kSpaceShipModeNormal) {
		return;
	}
	//Otherwise, launch missiles or lasers
	for (i=0; i<[array count]; ++i) {
		touch = [array objectAtIndex:i];
		location = [touch locationInView:self];
		if (touch.phase == UITouchPhaseBegan) {
			if ([touch tapCount] == 1) {
				for (j=0; j<kMaxTouches; ++j) {
					gesture = (Gesture*)CFArrayGetValueAtIndex(_gestures, j);
					if (!gesture.touch) {
						gesture.touch = touch;
						[gesture addPoint:location];
						break;
					}
				}
			}
			else
			[self _handleTouchAtLocation:location tapCount:[touch tapCount]];			
		}
		else {
			for (j=0; j<kMaxTouches; ++j) {
				gesture = (Gesture*)CFArrayGetValueAtIndex(_gestures, j);
				if (gesture.touch == touch) {
					if (touch.phase >= UITouchPhaseEnded) {
						[gesture reset];
					}
					else {
						[gesture addPoint:location];
						if (_state == kState_MultiPlayer) {
							_introShipTranslation = gesture.dPosition;
						}
					}
					break;
				}
			}
		}
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSArray*		array = [touches allObjects];
	UITouch*		touch;
	Gesture*		gesture;
	CGRect			rect = [[UIScreen mainScreen] bounds];
	NSUInteger		i, j;
	BOOL			missile = NO;
		
	for (i=0; i<[array count]; ++i) {
		touch = [array objectAtIndex:i];
		for (j=0; j<kMaxTouches; ++j) {
			gesture = (Gesture*)CFArrayGetValueAtIndex(_gestures, j);		
			if (gesture.touch == touch) {
				if (_state == kState_Game) {
					if (gesture.gestureType == kGestureType_Swipe) { 
						if ((ABS(gesture.gestureAngle) < 30.) || (ABS(gesture.gestureAngle - 360) < 30.)) {
							if ((_previousGesture == 0) && ([NSDate timeIntervalSinceReferenceDate] - _previousGestureTime < 0.05) && !_multiplayer) {
								_ship.mode = kSpaceShipModeMissileInitiated;
								_accelerometer.forceCalibration = YES;
								missile = NO;
							}
							else {
								missile = YES;
							}
							_previousGesture = 0;
							_previousGestureTime = [NSDate timeIntervalSinceReferenceDate];
						}
						else if (ABS(gesture.gestureAngle - 270.) < 30.) {
							if ((_previousGesture == 1) && ([NSDate timeIntervalSinceReferenceDate] - _previousGestureTime < 0.2))
							[(TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate] showParameters];
							_previousGesture = 1;
							_previousGestureTime = [NSDate timeIntervalSinceReferenceDate];
						}
						else {
							_previousGesture = 2;	
						}
						
					}
					else {
						[self _handleTouchAtLocation:gesture.gestureCenter tapCount:1];
						_previousGesture = 2;
					}
				}
				else if (_state == kState_MultiPlayer) {
					if (gesture.gestureType == kGestureType_Swipe) { 
						if (ABS(gesture.gestureAngle - 270.) < 70.) {
							_selectedShip ++;
						}
						else if (ABS(gesture.gestureAngle - 90) < 70.) {
							_selectedShip--;
						}
					}
					else if (gesture.gestureType == kGestureType_Unknown) {
						_state = kState_MultiPlayerChoosePeer;
					}
					_introShipOffset = CGPointMake(_introShipTranslation.x+_introShipOffset.x, _introShipTranslation.y+_introShipOffset.y);
					_introShipTranslation = CGPointZero;
				}
				
				printf ("%s", [[NSString stringWithFormat:@"Gesture: %@\n", gesture] UTF8String]);
				fflush(stdout);
				
				[gesture reset];
				break;
			}
		}
	}
	
	if (missile) {
		[_ship fireMissileAtPosition:([touch locationInView:self].y < rect.size.height / 2) ? kSpaceShipLaserPosition_Left : kSpaceShipLaserPosition_Right withTarget:_targetInFocus];
	}
}

@end

@implementation TouchFighterView(Extras)

/* Draw a fps counter. */
- (void) renderFPS
{
	NSString*				string;
	GLfloat					alpha = 0.05;
	
	if (_renderTime != _previousRenderTime) {
		_fps = alpha * 1./(_renderTime - _previousRenderTime) + (1.-alpha) * _fps;
	}
	if (_renderTime - _previousFPSRenderTime > .25) {
		string = [NSString stringWithFormat:@"%.1f", _fps];
		if (_fpsTexture) {
			[_fpsTexture release];
			_fpsTexture = nil;
		}
		_fpsTexture = [[Texture2D alloc] initWithString:string dimensions:CGSizeMake(32.,16.) fontSize:16.0];
		_previousFPSRenderTime = _renderTime;
	}

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);			

	glPushMatrix();
	glScalef(-1.0f,-1.0f,1.0f);
	glRotatef(90.,0.,0.,1.);	
	glTranslatef(260.,-180., 0.);	
	[GameObject bindTexture2D:_fpsTexture];
	[Texture2D drawTexture:_fpsTexture atPoint:CGPointMake(0.,0.) depth:-500];
	glPopMatrix();
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
	glDisable(GL_TEXTURE_2D);	
}

/* Draw a score. */
- (void) renderScore
{	
	GLfloat			color[4] = {0.6,0.6,0.6,0.6};

	if ((_previousScore != _score) || !_scoreTexture) {
		if (_scoreTexture)
		[_scoreTexture release];
		_scoreTexture = [[Texture2D alloc] initWithString:[NSString stringWithFormat:@"SCORE %i", _score] dimensions:CGSizeMake(32*4.,34.) fontSize:16.0];
		if (_previousScore < _score)
		color[0] = color[1] = color[2] = color[3] = 1.;
		else
		color[0] = color[3] = 1.0; color[1] = color[2] = 0.;		
		_previousScore = _score;
	}

	glColor4f(color[0], color[1], color[2], color[3]);
	glEnable(GL_TEXTURE_2D);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);			
	glEnable(GL_BLEND);

	glPushMatrix();
	glScalef(-1.0f,-1.0f,1.0f);
	glRotatef(90.,0.,0.,1.);	
	[GameObject bindTexture2D:_scoreTexture];
	glScalef(1./480., 1./480., 1.);
	[Texture2D drawTexture:_scoreTexture atPoint:CGPointMake(-480./2.,320./2.+20.) depth:-1.];
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
	glDisable(GL_TEXTURE_2D);	
}

- (void) drawTitleScreen
{
	GLfloat	vtxData[2*4] =
			{
				320., 0,
				0,    0,
				320., 480.,
				0,    480.,
			},
#define _320 (320.f/512.f)
#define _480 (480.f/512.f)
			texData[2*4] =
			{
				0,    0,
				0,    _320,
				_480,  0,
				_480, _320,
			};
#undef _320
#undef _480

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, 320, 480, 0, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(_titleFade < kTitleFadeTime) {
		GLfloat color = ((float)_titleFade) * (1.0f/(float)kTitleFadeTime);
		glColor4f(color, color, color, 1.0f);
		_titleFade--;
	}
	
	[GameObject bindTexture2D:_titleTexture];
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	[GameObject setVertexPointer:vtxData size:2 type:GL_FLOAT];
	[GameObject setTexcoordPointer:texData size:2 type:GL_FLOAT];
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//When fade-out of title screen is over
	if(_titleFade == 0) {
		[_titleTexture release];
		_titleTexture = nil;
		_titleFade = 0;
		[self fadeToR:0. G:0. B:0. A:0. fromR:0. G:0. B:0. A:1. withFrameCount:kGameFadeTime];
		[self setUpCamera];
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		_atTitle = NO;
	}
}

- (void) drawMultiplayerScreen
{
	static const GLfloat	rotationSpeed = 2.5;
	static const GLfloat	vtxData[3*4] = { 1.f, -1.f, -1.f,
										    -1.f, -1.f, -1.f,
										     1.f,  1.f, -1.f,
											-1.f,  1.f, -1.f };	
	static const GLfloat	radius = 0.75;	
	static const GLfloat	angleCoeff = 75.;	
	UINavigationItem*		item;
	CGAffineTransform		transform =	CGAffineTransformMakeRotation(M_PI/2.);
	CGRect					rect = [[UIScreen mainScreen] bounds];	
	
	_isHost = ((TouchFighterAppDelegate*)[UIApplication sharedApplication].delegate).isHost;
	if ([[[_titleBar topItem] title] isEqualToString:@"Choose Your Ship"] && (_state == kState_MultiPlayerChoosePeer)) {
		if (!_isHost) {
			_tableView = [[UITableView alloc] initWithFrame:CGRectMake(rect.size.height/2., 0., rect.size.height/2., rect.size.width-30.)];
			_tableView.backgroundColor = [UIColor colorWithWhite:1.0 alpha:0.1];
			_tableView.delegate = (TouchFighterAppDelegate*)[[UIApplication sharedApplication] delegate];
			_tableView.dataSource = (TouchFighterAppDelegate*) [[UIApplication sharedApplication] delegate];
			transform =	CGAffineTransformMakeRotation(M_PI/2.);
			_tableView.transform = transform;
			_tableView.center = CGPointMake(rect.size.width/2.-22., rect.size.height*3./2.);
			[self.superview addSubview:_tableView];
		}

		[UIView beginAnimations:nil context:NULL];
		self.center = CGPointMake(rect.size.width/2., rect.size.height*1./4.);
		if (_tableView) {
			_tableView.center = CGPointMake(rect.size.width/2. - 15., rect.size.height*3./4.);
		}
		[UIView commitAnimations];	
		if (!_tableView) {
			item = [[UINavigationItem alloc] initWithTitle:@"Waiting For Peers"];
		}
		else {
			[_tableView reloadData];		
			item = [[UINavigationItem alloc] initWithTitle:@"Choose Your Host"];
		}
		item.hidesBackButton = YES;
		[_titleBar pushNavigationItem:item animated:YES];
		[item release];
	}

	if (!_titleBar) {
		_titleBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0., 0., rect.size.height, 30.)];
		_titleBar.center = CGPointMake(rect.size.width - 15., rect.size.height/2.);
		_titleBar.transform = transform;
		_titleBar.barStyle = UIBarStyleBlackOpaque;
		_titleBar.backgroundColor = [UIColor clearColor];
		item = [[UINavigationItem alloc] initWithTitle:@"Choose Your Ship"];
		_titleBar.items = [NSArray arrayWithObject:item];
		[item release];
		[UIView beginAnimations:nil context:NULL];
		[self.superview addSubview:_titleBar];
		[UIView commitAnimations];	
	}
	
	_introShipRotation += rotationSpeed;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.15,0.,-1.);
	glTranslatef(0., 0., -radius);
	glRotatef(-20., 0., 1., 0.);
	if (_introShipOffset.y/rect.size.height != -_selectedShip*180./angleCoeff)
	_introShipOffset.y = -.05*_selectedShip*180./angleCoeff*rect.size.height + .95*_introShipOffset.y;
	glRotatef(_introShipOffset.y/rect.size.height*angleCoeff, 1., 0., 0.);
	glRotatef(_introShipTranslation.y/rect.size.height*angleCoeff, 1., 0., 0.);
	
	glEnableClientState(GL_VERTEX_ARRAY);	
	glEnable(GL_DEPTH_TEST);

	//Reflection
	glPushMatrix(); {
		glTranslatef(0., 0., radius);	
		glTranslatef(-0.1f, 0.f, 0.f);
		glPushMatrix(); {
			glRotatef(_introShipRotation, 1.f, 0.f, 0.f);
			glRotatef(180.f, 0.f, 0.f, 1.f);		
			[_ship renderAtTime:_introShipRotation/rotationSpeed/30.];
		}
		glPopMatrix();

		glTranslatef(0., 0., -2.*radius);	
		glPushMatrix(); {
			glRotatef(_introShipRotation, 1.f, 0.f, 0.f);
			glRotatef(180.f, 0.f, 0.f, 1.f);		
			[_enemyShip renderAtTime:_introShipRotation/rotationSpeed/30.];
		}
		glPopMatrix();
		
		//Draw black square with alpha
		glPushMatrix(); {
			glLoadIdentity();
			glEnable(GL_BLEND);
			glDepthFunc(GL_ALWAYS);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0., 0., 0., 0.7);
			[GameObject setVertexPointer:(void*)vtxData size:3 type:GL_FLOAT];
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glDepthFunc(GL_LEQUAL);
			glDisable(GL_BLEND);
		}
		glPopMatrix();
	}
	glPopMatrix();

	//Ships
	glPushMatrix(); {
		glTranslatef(0., 0., radius);	
		glRotatef(_introShipRotation, 1.f, 0.f, 0.f);
		[_ship renderAtTime:_introShipRotation/rotationSpeed/30.];
	}
	glPopMatrix();

	glPushMatrix(); {
		glTranslatef(0., 0., -radius);	
		glRotatef(_introShipRotation, 1.f, 0.f, 0.f);
		[_enemyShip renderAtTime:_introShipRotation/rotationSpeed/30.];
	}
	glPopMatrix();
		
	glDisable(GL_DEPTH_TEST);
	glDisableClientState(GL_VERTEX_ARRAY);	
	glPopMatrix();	
}

/* Render a quad of plain color with alpha covering the whole screen when fading into game from black and updates its alpha */
- (void) fadeToR:(float)tr G:(float)tg B:(float)tb A:(float)ta fromR:(float)fr G:(float)fg B:(float)fb A:(float)fa withFrameCount:(int)count{
	_fadeFrom[0] = fr;
	_fadeFrom[1] = fg;
	_fadeFrom[2] = fb;
	_fadeFrom[3] = fa;
	_fadeTo[0] = tr;
	_fadeTo[1] = tg;
	_fadeTo[2] = tb;
	_fadeTo[3] = ta;
	_fadeTime = count;
	_fadeFrame = count;
}

- (void) renderFade
{
	GLfloat fade;
	static const GLfloat vtxData[3*4] = {
		100., -100, -10.f,
		-100, -100, -10.f,
		100., 100., -10.f,
		-100, 100., -10.f,
	};


	fade = (float)_fadeFrame / (float)_fadeTime;
	if (_fadeFrame >= 0) {
		_fadeFrame--;
	}

	//If we have a non-zero alpha for the end color, we want to hold it indefinitely
	if (_fadeTo[3] != 0. && _fadeFrame < 0) {
		_fadeFrame = 0;
	}
	glLoadIdentity();

	glEnable(GL_BLEND);
	glDepthFunc(GL_ALWAYS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(_fadeFrom[0] * fade + _fadeTo[0] * (1. - fade),
			  _fadeFrom[1] * fade + _fadeTo[1] * (1. - fade), 
			  _fadeFrom[2] * fade + _fadeTo[2] * (1. - fade), 
			  _fadeFrom[3] * fade + _fadeTo[3] * (1. - fade));
	[GameObject setVertexPointer:(GLvoid *)vtxData size:3 type:GL_FLOAT];
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
}

@end
