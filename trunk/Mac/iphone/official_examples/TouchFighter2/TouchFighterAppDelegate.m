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

File: TouchFighterAppDelegate.m
Abstract: UIApplication's delegate class i.e. the central controller of the application.

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

#import "TouchFighterAppDelegate.h"
#import "TouchFighterView.h"
#import "HighScoresView.h"
#import "SpaceShip.h"
#import "EnemyShip.h"
#import "Utils.h"
#import "Message.h"

#import <MediaPlayer/MediaPlayer.h>

//CONSTANTS:

#define kFPS								60.0
#define kNetworkFrequency					10.0

@interface TouchFighterAppDelegate(Internal)
- (void) playMovie;
@end


//CLASS IMPLEMENTATIONS:

@implementation TouchFighterAppDelegate

/* Game loop called by timer which direct the game view update */
- (void) gameLoop
{
	if (_gameView && !_gameView.gameOver) {		//If game is not over, simply invoke gameLoop on the game view.
		[_gameView gameLoop];
	} 
	else {										//If game over, play movie and show high scores.
		[self playMovie];
	}
}

/* That's where we initialize the game objects */
- (void) applicationDidFinishLaunching:(UIApplication*)application
{
	CGRect					rect = [[UIScreen mainScreen] bounds];
	NSAutoreleasePool*		pool = [NSAutoreleasePool new];
	
	//Initialize random function
	fast_rand_init();
	
	[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
	[UIApplication sharedApplication].isIdleTimerDisabled = YES;
	[UIApplication sharedApplication].delegate = self;
	
	//Initialize the application window
	_window = [[UIWindow alloc] initWithFrame:rect];
	if (!_window) {
		[self release];
		return;
	}
	
	//Set the window background color as black color
	_window.backgroundColor = [UIColor blackColor];
	
	//Initialize the game view
	_gameView = [[TouchFighterView alloc] initWithFrame:rect pixelFormat:kEAGLColorFormatRGB565 depthFormat:GL_DEPTH_COMPONENT24_OES preserveBackbuffer:NO];
	if (!_gameView) {
		[self release];
		return;
	}
	[_window addSubview:_gameView];
	
	//Activate multitouch support
	[_gameView setMultipleTouchEnabled:YES];

	//Initializing game timer
	_timer = [NSTimer scheduledTimerWithTimeInterval:(1.0 / kFPS) target:self selector:@selector(gameLoop) userInfo:nil repeats:YES];
	
	//Starting the game
	[_gameView gameLoop];	
	[_window makeKeyAndVisible];
	
	[pool release];

	return;
}

/* When getting a call, pause the game */
- (void)applicationWillResignActive:(UIApplication *)application
{
	if (_gameView)
	_gameView.pause = YES;
}

/* Call got rejected*/
- (void)applicationDidBecomeActive:(UIApplication *)application
{
	if (_gameView)
	_gameView.pause = NO;
}

/* Play movie */
- (void) playMovie
{
	CGRect							rect = [[UIScreen mainScreen] bounds];
	CGAffineTransform				transform;

	//Release the timer
	[_timer invalidate];
	_timer = nil;
	if (_networkTimer) {
		[_networkTimer invalidate];
		_networkTimer = nil;
	}
	
	//Set the window background color as white color.
	_window.backgroundColor = [UIColor whiteColor];
	
	//Release and remove the game view for optimal movie playback performance
	[_gameView removeFromSuperview];
	[_gameView release];
	_gameView = nil;
		
	//Create high scores view and save score
	_highScoresView = [[HighScoresView alloc] initWithFrame:CGRectMake(0., 0., rect.size.height, rect.size.width)];
	[_highScoresView addName:nil score:_gameView.score];
	transform = CGAffineTransformMakeRotation(M_PI/2.);
	transform = CGAffineTransformTranslate(transform, (rect.size.height - rect.size.width)/2., (rect.size.height - rect.size.width)/2.);
	_highScoresView.transform = transform;
	
	//Setting -up end movie
	_endMovie = [[MPMoviePlayerController alloc] initWithContentURL:[NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"End" ofType:@"mp4"]]];
	if (!_endMovie)
	return;
	
	//Set the background color to clearColor, that way the background will be the window background (white) when the movie loads.
	_endMovie.backgroundColor=[UIColor clearColor];
	//Add callback for when the movie is over.
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(movieEnded:) name:MPMoviePlayerPlaybackDidFinishNotification object:nil];		
	
	//Start the final movie
	[_endMovie play];
	
	//Once the movie has started, we set the background color to black for proper transition with final "Winner don't use drugs" screen.
	[NSTimer scheduledTimerWithTimeInterval:3. target:self selector:@selector(movieStarted:) userInfo:nil repeats:NO];
}

/* Called once the movie has started */
- (void) movieStarted:(NSTimer*)timer
{
	_window.backgroundColor = [UIColor blackColor];
}

/* Called once the final movie is over */
- (void) movieEnded:(NSNotification *)note
{
	CGRect					rect = [[UIScreen mainScreen] bounds];
	UIImageView*			imageView;
	UIImage*				image;
	CGAffineTransform		transform;
		
	//Once movie is over, show the "Winners don't use drugs" screen by adding a UIImage view whose content is set to Winner.png
	image = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle]  pathForResource:@"Winner" ofType:@"png"]];
	imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0., 0., rect.size.height, rect.size.width)];
	[_window addSubview:imageView];
	imageView.image = image;

	//Rotate to portrait mode
	transform = CGAffineTransformMakeRotation(M_PI/2.);
	transform = CGAffineTransformTranslate(transform, (rect.size.height - rect.size.width)/2., (rect.size.height - rect.size.width)/2.);
	imageView.transform = transform;

	//Release temporary resources
	[image release];
	[imageView release];
	
	[_window addSubview:_highScoresView];
	[_highScoresView release];
	_highScoresView = nil;
}

- (void) showParameters
{
	UIView*					view;
	NSArray*				nibContents;
	CGRect					rect = [[UIScreen mainScreen] bounds];	
	CGAffineTransform		transform;
	
	if (!_settingsView) {
		nibContents = [[NSBundle mainBundle] loadNibNamed:@"Settings" owner:self options:nil];
		if ([nibContents count]>1) {
			_settingsView = [[UIView alloc] initWithFrame:CGRectMake(0., 0., rect.size.height, rect.size.width)];
			_settingsView.backgroundColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.5];
			view = [nibContents objectAtIndex:1];
			[_settingsView addSubview:view];
			transform = CGAffineTransformMakeRotation(M_PI/2.);
			_settingsView.transform = transform;
			_settingsView.center = CGPointMake(rect.size.width/2., rect.size.height*3./2.);
		}
	}

	if (!_settingsView) {
		NSLog (@"Error loading Settings.xib");
		return;
	}
	
	_sensitivitySlider.value = _gameView.accelerometerSensitivity;
	_invertControls.on = _gameView.invertControls;
	_renderFPS.on = _gameView.showFPS;
	_renderGestures.on = _gameView.showGesture;
	
	[_window addSubview:_settingsView];
	
	[UIView beginAnimations:nil context:NULL];
	_settingsView.center = CGPointMake(rect.size.width/2., rect.size.height/2.);
	[UIView commitAnimations];

	_gameView.pause = YES;
}

- (IBAction) hideParameters:(id)sender
{	
	CGRect					rect = [[UIScreen mainScreen] bounds];	

	_gameView.accelerometerSensitivity = _sensitivitySlider.value;
	_gameView.invertControls = _invertControls.on;
	_gameView.showFPS = _renderFPS.on;
	_gameView.showGesture = _renderGestures.on;
	_gameView.pause = NO;

	[UIView beginAnimations:@"Settings" context:NULL];
	_gameView.center = CGPointMake(rect.size.width/2., rect.size.height/2.);
	_settingsView.center = CGPointMake(rect.size.width/2., rect.size.height*3./2.);
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(animationDidStop:finished:context:)];
	[UIView commitAnimations];	
}

- (void) dealloc
{
	[_window release];
	if (_gameView) {
		[_gameView release];
	}
	if (_server) {
		[_server release];
	}
	if (_highScoresView) {
		[_highScoresView release];
	}
	if (_timer) {
		[_timer invalidate];
	}
	if (_networkTimer) {
		[_networkTimer invalidate];
	}
	if (_endMovie) {
		[_endMovie release];
	}
	if (_settingsView) {
		[_settingsView release];
	}	
	if (_networkType) {
		[_networkType release];
	}
	
	[super dealloc];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [[_server onlineServers] count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath  {
	
	UITableViewCell *cell = nil;

	cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero] autorelease];
	cell.text = [[[_server onlineServers] objectAtIndex:indexPath.row] name];

	return cell;
}

- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	GamePeer*		server;
	
	server = [[_server onlineServers] objectAtIndex:indexPath.row];
	
	if(![_server connectTo:server])
	return [tableView indexPathForSelectedRow];
	else {
		//[self startMultiplayer:[NSArray arrayWithObject:server]];
		return indexPath;
	}
}

- (IBAction) startAsHost:(id)sender
{
	_server = [[Multiplayer alloc] initWithType:kMultiplayerType_Host];
	[UIView beginAnimations:@"NetworkType" context:NULL];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(animationDidStop:finished:context:)];
	_networkType.alpha = 0.;
	[UIView commitAnimations];
	_multiplayer = YES;
}

- (IBAction) startAsClient:(id)sender
{
	_server = [[Multiplayer alloc] initWithType:kMultiplayerType_Client];
	[UIView beginAnimations:@"NetworkType" context:NULL];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(animationDidStop:finished:context:)];
	_networkType.alpha = 0.;
	[UIView commitAnimations];
	_multiplayer = YES;
}

- (void) animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	if ([animationID isEqualToString:@"NetworkType"]) {
		[_networkType removeFromSuperview];
		[_networkType release];
		_networkType = nil;
	}
	else if ([animationID isEqualToString:@"Settings"]) {
		[_settingsView removeFromSuperview];
		[_settingsView release];
		_settingsView = nil;
	}
}

@end

@implementation TouchFighterAppDelegate(Networking)

- (void) updatePeers
{
	[_gameView updatePeers];
}

- (void) sendGameData
{
	id		data = [_gameView gameData];
	
	if (data) {
		if (![_server sendMessage:[Message messageWithType:kMessageType_GameInfo data:data] immediate:YES]) {
			NSLog(@"Error Sending Game Data");
		}
	}
}

- (void) receiveGameData:(id)data fromPeer:(GamePeer*)peer
{
	if (_gameView)
	[_gameView setGameData:data forPeer:peer];
}

- (void) receiveShipType:(SpaceShipType)type fromPeer:(GamePeer*)peer
{
	[_gameView setShipType:type forPeer:peer];
	
	//In case we're a client, send ship type to host.
	if (_server.type == kMultiplayerType_Client) {
		if (![_server sendMessage:[Message messageWithType:kMessageType_SelectedShip data:[NSNumber numberWithUnsignedInt:[_gameView shipType]]] immediate:NO]) {
			NSLog(@"Error sending ship info");
		}
	}
}

- (void) startMultiplayer:(NSArray*)peers
{
	[_gameView startMultiplayer:[peers objectAtIndex:0]];
	
	//In case we're a host and just connected to host, send ship info to client
	if (_server.type == kMultiplayerType_Host) {
		if (![_server sendMessage:[Message messageWithType:kMessageType_SelectedShip data:[NSNumber numberWithUnsignedInt:[_gameView shipType]]] immediate:NO]) {
			NSLog(@"Error sending ship info");
		}
	}
	_networkTimer = [NSTimer scheduledTimerWithTimeInterval:(1.0 / kNetworkFrequency) target:self selector:@selector(sendGameData) userInfo:nil repeats:YES];
}

- (BOOL) multiplayer
{
	return _multiplayer;
}

- (void) setMultiplayer:(BOOL)flag
{
	NSArray*				nibContents;
	CGRect					rect = [[UIScreen mainScreen] bounds];	
	CGAffineTransform		transform;
	
	nibContents = [[NSBundle mainBundle] loadNibNamed:@"MultiplayerType" owner:self options:nil];
	if ([nibContents count]>1) {
		_networkType = [[nibContents objectAtIndex:1] retain];
		transform = CGAffineTransformMakeRotation(M_PI/2.);
		transform = CGAffineTransformTranslate(transform, (rect.size.height - rect.size.width)/2., (rect.size.height - rect.size.width)/2.);
		_networkType.transform = transform;
		_networkType.center = CGPointMake(rect.size.height/2., rect.size.width/2.);
		[_window addSubview:_networkType];
		_networkType.alpha = 0.;
		[UIView beginAnimations:nil context:NULL];
		_networkType.alpha = 1.;
		[UIView commitAnimations];
	}
}

- (BOOL) isHost
{
	return ((_server.type == kMultiplayerType_Host) || (!_multiplayer));
}

@end