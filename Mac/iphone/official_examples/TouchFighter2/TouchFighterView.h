/*
File: TouchFighterView.h
Abstract: Subclass of EAGLView that handles game state update and rendering.

Version: 2.0
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