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

File: TouchFighterView.h
Abstract: Subclass of EAGLView that handles game state update and rendering.

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

#import "EAGLView.h"
#import "SpaceShip.h"
#import "Maths.h"

typedef enum _State {
	kState_Title = 0,
	kState_TitleFadeOut,
	kState_MultiPlayerFadeIn,
	kState_MultiPlayerChoosePeer,
	kState_MultiPlayer,
	kState_GameFadeIn,
	kState_Game
} State;

@class AccelerometerSmoother, CubeMap, EAGLView, MotherShip, MPMoviePlayerController, SpaceShip, ParticleSystemObject, Planet, StarField, Texture2D, Gesture, GameObject, GamePeer, Meteorite;

@interface TouchFighterView : EAGLView {
@private
	//Game Objects
	Texture2D*				_explosionTexture;
	StarField*				_stars;
	SpaceShip*				_ship, *_enemyShip;
	CubeMap*				_cubeMap;
	Planet*					_planet;
	Meteorite*				_meteorite;
	MotherShip*				_motherShip;

	CFMutableArrayRef		_enemies,
							_explosions;
	CFMutableDictionaryRef	_peers;

	//General state ivars
	State					_state;
	Texture2D*				_titleTexture;
	Matrix					_matrix,
							_transformationMatrix;
	Point3					_transformationRotation;
	CGPoint					_introShipTranslation,
							_introShipOffset;
	NSInteger				_selectedShip;
	CGFloat					_worldRotation,
							_introShipRotation,
							_titleFade,
							_cameraRotation, _prevAngle;
	NSTimeInterval			_lastTouchTime, _lastMeteoriteTime;
	BOOL					_atTitle, _multiplayer, _isHost;
	UITableView*			_tableView;
	UINavigationBar*		_titleBar;
	NSUInteger				_previousGesture;
	NSTimeInterval			_previousGestureTime;

	//FPS and Rendering:	
	NSTimeInterval			_renderTime,
							_startTime,
							_previousRenderTime,
							_previousFPSRenderTime;
	CGFloat					_fps;
	Texture2D*				_fpsTexture;

	// Fade
	Texture2D*				_fadeTex;
	CGFloat					_fadeFrom[4],
							_fadeTo[4];
	NSInteger				_fadeTime,
							_fadeFrame;
	
	// Game management
	BOOL					_paused,
							_gameOver,
							_renderFPS,
							_renderGestures;
	NSUInteger				_score, _previousScore;
	Texture2D*				_scoreTexture;
	
	//Inputs
	AccelerometerSmoother*	_accelerometer;
	CFMutableArrayRef		_gestures;
	GameObject*				_targetInFocus;
}

- (void) gameLoop;

- (void) updatePeers;
- (void) startMultiplayer:(GamePeer*)peer;
- (id) gameData;
- (void) setGameData:(NSDictionary*)gameData forPeer:(GamePeer*)peer;
- (SpaceShipType) shipType;
- (void) setShipType:(SpaceShipType)type forPeer:(GamePeer*)peer;

@property(assign) BOOL pause, showFPS, showGesture;
@property(assign) BOOL invertControls;
@property(assign) CGFloat accelerometerSensitivity;

@property(readonly) BOOL gameOver;
@property(readonly) NSUInteger score;
@end