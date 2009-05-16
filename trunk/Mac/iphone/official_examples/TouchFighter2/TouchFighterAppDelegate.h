/*
File: TouchFighterAppDelegate.h
Abstract: UIApplication's delegate class i.e. the central controller of the application.

Version: 2.0
*/

#import "Multiplayer.h"
#import "SpaceShip.h"

@class TouchFighterView, GameObjectView, HighScoresView, MPMoviePlayerController;

@interface TouchFighterAppDelegate: NSObject <UIApplicationDelegate, UITableViewDelegate, UITableViewDataSource>
{
@private
	UIWindow*					_window;
	TouchFighterView*			_gameView;
	HighScoresView*				_highScoresView;
	NSTimer*					_timer, *_networkTimer;
	MPMoviePlayerController*	_endMovie;
	Multiplayer*				_server;
	BOOL						_multiplayer;

	UIView*						_settingsView, *_networkType;	
	IBOutlet UISlider*			_sensitivitySlider;
	IBOutlet UISwitch*			_invertControls;
	IBOutlet UISwitch*			_renderFPS;
	IBOutlet UISwitch*			_renderGestures;
	
	UITextField*				_textField;
	
}

- (void) showParameters;
- (IBAction) hideParameters:(id)sender;
- (IBAction) startAsHost:(id)sender;
- (IBAction) startAsClient:(id)sender;
@end

@interface TouchFighterAppDelegate(Networking)
@property BOOL				multiplayer;
@property(readonly) BOOL	isHost;
- (void) startMultiplayer:(NSArray*)peer;
- (void) updatePeers;
- (void) receiveGameData:(id)data fromPeer:(GamePeer*)peer;
- (void) receiveShipType:(SpaceShipType)type fromPeer:(GamePeer*)peer;
@end