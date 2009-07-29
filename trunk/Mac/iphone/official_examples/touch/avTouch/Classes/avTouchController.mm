/*

    File: avTouchController.mm
Abstract: n/a
 Version: 1.0.1

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
Inc. ("Apple") in consideration of your agreement to the following
terms, and your use, installation, modification or redistribution of
this Apple software constitutes acceptance of these terms.  If you do
not agree with these terms, please do not use, install, modify or
redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and
subject to these terms, Apple grants you a personal, non-exclusive
license, under Apple's copyrights in this original Apple software (the
"Apple Software"), to use, reproduce, modify and redistribute the Apple
Software, with or without modifications, in source and/or binary forms;
provided that if you redistribute the Apple Software in its entirety and
without modifications, you must retain this notice and the following
text and disclaimers in all such redistributions of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may
be used to endorse or promote products derived from the Apple Software
without specific prior written permission from Apple.  Except as
expressly stated in this notice, no other rights or licenses, express or
implied, are granted by Apple herein, including but not limited to any
patent rights that may be infringed by your derivative works or by other
works in which the Apple Software may be incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2009 Apple Inc. All Rights Reserved.


*/

#import "avTouchController.h"
#include "CALevelMeter.h"

// amount to skip on rewind or fast forward
#define SKIP_TIME 1.0			
// amount to play between skips
#define SKIP_INTERVAL .2

@implementation avTouchController

@synthesize _volumeSlider;
@synthesize _progressBar;
@synthesize _currentTime;
@synthesize _duration;
@synthesize _updateTimer;
@synthesize _lvlMeter_in;
@synthesize _player;

-(void)updateCurrentTimeForPlayer:(AVAudioPlayer *)player
{
	self._currentTime.text = [NSString stringWithFormat:@"%d:%02d", (int)player.currentTime / 60, (int)player.currentTime % 60, nil];
	self._progressBar.value = player.currentTime;
}

- (void)updateCurrentTime
{
	[self updateCurrentTimeForPlayer:self._player];
}

- (void)updateViewForPlayerState:(AVAudioPlayer *)player
{
	[self updateCurrentTimeForPlayer:player];

	if (_updateTimer) 
		[_updateTimer invalidate];
		
	if (player.playing)
	{
		[_playButton setImage:((player.playing == YES) ? _pauseBtnBG : _playBtnBG) forState:UIControlStateNormal];
		[_lvlMeter_in setPlayer:player];
		_updateTimer = [NSTimer scheduledTimerWithTimeInterval:.01 target:self selector:@selector(updateCurrentTime) userInfo:player repeats:YES];
	}
	else
	{
		[_playButton setImage:((player.playing == YES) ? _pauseBtnBG : _playBtnBG) forState:UIControlStateNormal];
		[_lvlMeter_in setPlayer:nil];
		_updateTimer = nil;
	}
	
}

-(void)updateViewForPlayerInfo:(AVAudioPlayer*)player
{
	self._duration.text = [NSString stringWithFormat:@"%d:%02d", (int)player.duration / 60, (int)player.duration % 60, nil];
	self._progressBar.maximumValue = player.duration;
	self._volumeSlider.value = player.volume;
}

- (void)rewind
{
	AVAudioPlayer *player = _rewTimer.userInfo;
	player.currentTime-= SKIP_TIME;
	[self updateCurrentTimeForPlayer:player];
}

- (void)ffwd
{
	AVAudioPlayer *player = _ffwTimer.userInfo;
	player.currentTime+= SKIP_TIME;	
	[self updateCurrentTimeForPlayer:player];
}

- (void)awakeFromNib
{
	// Make the array to store our AVAudioPlayer objects
	_soundFiles = [[NSMutableArray alloc] initWithCapacity:3];

	_playBtnBG = [[[UIImage imageNamed:@"play.png"] stretchableImageWithLeftCapWidth:12.0 topCapHeight:0.0] retain];
	_pauseBtnBG = [[[UIImage imageNamed:@"pause.png"] stretchableImageWithLeftCapWidth:12.0 topCapHeight:0.0] retain];

	[_playButton setImage:_playBtnBG forState:UIControlStateNormal];
			
	_updateTimer = nil;
	_rewTimer = nil;
	_ffwTimer = nil;
	
	self._duration.adjustsFontSizeToFitWidth = YES;
	self._currentTime.adjustsFontSizeToFitWidth = YES;
	self._progressBar.minimumValue = 0.0;	
	
	// Load the array with the sample file
	NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"m4a"]];
	self._player = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:nil];	
	if (self._player)
	{
		_fileName.text = [NSString stringWithFormat: @"%@ (%d ch.)", [[self._player.url relativePath] lastPathComponent], self._player.numberOfChannels, nil];
		[self updateViewForPlayerInfo:self._player];
		[self updateViewForPlayerState:self._player];
	}
	
	OSStatus result = AudioSessionInitialize(NULL, NULL, NULL, NULL);
	if (result)
		printf("Error initializing audio session! %d\n", result);
	
	UInt32 category = kAudioSessionCategory_SoloAmbientSound;
	result = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
	if (result)
		printf("Error setting audio category! %d\n", result);
		
	[fileURL release];

}

-(void)pausePlaybackForPlayer:(AVAudioPlayer*)player
{
	[player pause];
	[self updateViewForPlayerState:player];
}

-(void)startPlaybackForPlayer:(AVAudioPlayer*)player
{
	if ([player play])
	{
		[self updateViewForPlayerState:player];
		player.delegate = self;
	}
	else
		NSLog(@"Could not play %@\n", player.url);
}

- (IBAction)playButtonPressed:(UIButton *)sender
{
	if (self._player.playing == YES)
		[self pausePlaybackForPlayer: self._player];
	else
		[self startPlaybackForPlayer: self._player];
}

- (IBAction)rewButtonPressed:(UIButton *)sender
{
	if (_rewTimer) [_rewTimer invalidate];
	_rewTimer = [NSTimer scheduledTimerWithTimeInterval:SKIP_INTERVAL target:self selector:@selector(rewind) userInfo:self._player repeats:YES];
}

- (IBAction)rewButtonReleased:(UIButton *)sender
{
	if (_rewTimer) [_rewTimer invalidate];
	_rewTimer = nil;
}

- (IBAction)ffwButtonPressed:(UIButton *)sender
{
	if (_ffwTimer) [_ffwTimer invalidate];
	_ffwTimer = [NSTimer scheduledTimerWithTimeInterval:SKIP_INTERVAL target:self selector:@selector(ffwd) userInfo:self._player repeats:YES];
}

- (IBAction)ffwButtonReleased:(UIButton *)sender
{
	if (_ffwTimer) [_ffwTimer invalidate];
	_ffwTimer = nil;
}

- (IBAction)volumeSliderMoved:(UISlider *)sender
{
	self._player.volume = [sender value];
}

- (IBAction)progressSliderMoved:(UISlider *)sender
{
	self._player.currentTime = sender.value;
	[self updateCurrentTimeForPlayer:self._player];
}

- (void)dealloc
{
	[super dealloc];
	[_updateTimer release];
	[_playBtnBG release];
	[_player release];
}

#pragma mark AVAudioPlayer delegate methods

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
	if (flag == NO)
		NSLog(@"Playback finished unsuccessfully");
		
	[player setCurrentTime:0.];
	[self updateViewForPlayerState:player];
}

- (void)playerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
	NSLog(@"ERROR IN DECODE: %@\n", error); 
}

// we will only get these notifications if playback was interrupted
- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player
{
	// the object has already been paused,	we just need to update UI
	[self updateViewForPlayerState:player];
}

- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player
{
	[self startPlaybackForPlayer:player];
}

@end