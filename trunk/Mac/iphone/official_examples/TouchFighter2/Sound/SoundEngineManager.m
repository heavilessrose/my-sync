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

File: SoundEngineManager.m
Abstract: 3D sound objects and sound engine.

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

#if !TARGET_IPHONE_SIMULATOR
#import "SoundEngineManager.h"
#import "SoundEngine.h"
#import "Utils.h"

//CLASS INTERFACES:

@interface Sound(Private)
- (id) _initWithEffectID:(UInt32)eid;
@end

//CLASS IMPLEMENTATIONS:

@implementation Sound
@synthesize position=_position, pitch=_pitch, volume=_volume;

- (id) _initWithEffectID:(UInt32)eid looping:(BOOL)looping
{
	if ((self = [super init])) {
		_effectID = eid;
		_pitch = 1.;
		_volume = 1.;
		_looping = looping;
		_playing = NO;
	}
	
	return self;
}

- (void) play
{
	OSStatus					result = noErr;
	
	if (!_looping || (_looping && !_playing)) {
		result = SoundEngine_StartEffect(_effectID);
		if (result != noErr) {
			NSLog (@"ERROR: SoundEngine_StartEffect");
			return;
		}
		_playing = YES;
	}
	
	result = SoundEngine_SetEffectPosition(_effectID, -_position.y, -_position.x, _position.z);
	if (result != noErr) {
		NSLog (@"ERROR: SoundEngine_SetEffectPosition");
		return;
	}

	result = SoundEngine_SetEffectPitch(_effectID, _pitch);
	if (result != noErr) {
		NSLog (@"ERROR: SoundEngine_SetEffectPosition");
		return;
	}

	result = SoundEngine_SetEffectLevel(_effectID, _volume);
	if (result != noErr) {
		NSLog (@"ERROR: SoundEngine_SetEffectVolume");
		return;
	}
}

@end

@implementation SoundEngineManager
@synthesize effectsVolume=_effectsVolume, listenerPosition=_listenerPosition;

+ (SoundEngineManager*) sharedManager
{
	static SoundEngineManager*	manager = nil;
	
	if (!manager) {
		manager = [SoundEngineManager new];
	}
	
	return manager;
}

- (BOOL) isSoundInitialized:(NSString*)name 
{
	NSString*		filename;

	return ((filename = [_sounds valueForKey:name]) && [filename isKindOfClass:[Sound class]]);
}

- (id) init
{
	if ((self = [super init])) {
		if (SoundEngine_Initialize(22050.0) != noErr) {
			[self release];
			NSLog (@"Could not initialize");
			return nil;	
		}

		_effectsVolume = 1.;

		_sounds = [NSMutableDictionary new];
		[_sounds setValue:@"s_laser" forKey:@"laser"];
		[_sounds setValue:@"s_boom_laser" forKey:@"laserBoom"];
		[_sounds setValue:@"s_missle" forKey:@"missile"];
		[_sounds setValue:@"s_boom_missle" forKey:@"missileBoom"];
		[_sounds setValue:@"s_boom_mothership" forKey:@"mothership"];		
		[_sounds setValue:[NSArray arrayWithObjects:@"s_thrust_in", @"s_thrust", @"s_thrust_out", nil] forKey:@"thrusters"];		
		[_sounds setValue:@"s_initiate_missile" forKey:@"initiateMissile"];		
		[_sounds setValue:@"s_wind" forKey:@"wind"];		
	}
	
	return self;
}

- (void) dealloc
{
	UInt32 result = noErr;

	[_sounds release];
	
	result = SoundEngine_Teardown();
	if (result != noErr) {
		NSLog (@"ERROR: SoundEngine_Teardown");
	}

	[super dealloc];
}

- (void) fadeOutCallback:(NSTimer*)timer
{
	NSTimer*		newTimer;
	NSArray*		userInfo = [timer userInfo];
	NSTimeInterval	time = [[userInfo objectAtIndex:0] doubleValue], 
					length = [[userInfo objectAtIndex:1] doubleValue],
					nextTime;

	nextTime = MAX(time - 1./10., 0.);
	self.effectsVolume = self.effectsVolume * nextTime / length;
	
	if (nextTime > 0.) {
		newTimer = [NSTimer scheduledTimerWithTimeInterval:1./10. target:self selector:@selector(fadeOutCallback:) userInfo:[NSArray arrayWithObjects:[NSNumber numberWithDouble:nextTime], [userInfo objectAtIndex:1], nil] repeats:NO];
	}
}

- (void) fadeOut:(NSTimeInterval)seconds delay:(NSTimeInterval)delay
{
	NSTimer*		timer;
	
	timer = [NSTimer scheduledTimerWithTimeInterval:delay target:self selector:@selector(fadeOutCallback:) userInfo:[NSArray arrayWithObjects:[NSNumber numberWithDouble:seconds], [NSNumber numberWithDouble:seconds], nil]	repeats:NO];
}

- (Sound*) soundWithIdentifier:(NSString*)identifier
{
	Sound*			sound;
	NSString*		filename, *path1, *path2, *path3;
	BOOL			looping = NO;
	UInt32			effectID, result = noErr;
	
	if (![self isSoundInitialized:identifier]) {
		filename = [_sounds valueForKey:identifier];
		if ([filename isKindOfClass:[NSString class]]) {
			if (!(path1 = [[NSBundle mainBundle] pathForResource:filename ofType:@"wav"])) {
				NSLog (@"ERROR: Could not load file %@", filename);
				return nil;
			}
			result = SoundEngine_LoadEffect((const char*)[path1 UTF8String], &effectID);
			if (result != noErr) {
				NSLog (@"ERROR: Could not load effect %@", path1);
				return nil;
			}
		}
		else if ([filename isKindOfClass:[NSArray class]] && ([(NSArray*)filename count] == 3)) {
			if (!(path1 = [[NSBundle mainBundle] pathForResource:[(NSArray*)filename objectAtIndex:0] ofType:@"wav"])) {
				NSLog (@"ERROR: Could not load file %@", [(NSArray*)filename objectAtIndex:0]);
				return nil;
			}
			if (!(path2 = [[NSBundle mainBundle] pathForResource:[(NSArray*)filename objectAtIndex:1] ofType:@"wav"])) {
				NSLog (@"ERROR: Could not load file %@", [(NSArray*)filename objectAtIndex:1]);
				return nil;
			}
			if (!(path3 = [[NSBundle mainBundle] pathForResource:[(NSArray*)filename objectAtIndex:2] ofType:@"wav"])) {
				NSLog (@"ERROR: Could not load file %@", [(NSArray*)filename objectAtIndex:2]);
				return nil;
			}
			result = SoundEngine_LoadLoopingEffect((const char*)[path2 UTF8String], (const char*)[path1 UTF8String], (const char*)[path3 UTF8String], &effectID);
			if (result != noErr) {
				NSLog (@"ERROR: Could not load effect %@", path1);	
				return nil;
			}		
			looping = YES;
		}
		sound = [[Sound alloc] _initWithEffectID:effectID looping:looping];
		[_sounds setValue:sound forKey:identifier];
		[sound release];
	}
	else 
	sound = [_sounds valueForKey:identifier];
	
	return sound;
}

- (void) setEffectsVolume:(Float32)v
{
	UInt32 result = noErr;

	if (_effectsVolume != v) {
		result = SoundEngine_SetEffectsVolume(v);	
		if (result != noErr) {
			NSLog (@"ERROR: SoundEngine_SetEffectsVolume");
		} else {
			_effectsVolume = v;
		}
	}
}

- (void) setListenerPosition:(Point3)position
{
	UInt32 result = noErr;

	if (!Point3EqualsPoint3(position, _listenerPosition)) {
		result = SoundEngine_SetListenerPosition(-position.y, -position.x, position.z);	
		if (result != noErr) {
			NSLog (@"ERROR: SoundEngine_SetEffectsVolume");
		} else {
			_listenerPosition = position;
		}
	}
}

@end
#endif