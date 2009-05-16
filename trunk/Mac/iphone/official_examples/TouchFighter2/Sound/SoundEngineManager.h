/*
File: SoundEngineManager.h
Abstract: 3D sound objects and sound engine.

Version: 2.0
*/

#import "TouchFighter_Types.h"

@interface Sound : NSObject {
@private
	BOOL					_looping, _playing;
	UInt32					_effectID;

	Point3					_position;
	Float32					_pitch, _volume;
}

/* Play the sound */
- (void) play;

/* Sound position. */
@property Point3 position;

/* Sound pitch. */
@property Float32 pitch;

/* Sound volume. */
@property Float32 volume;
@end

/* The sound engine manager manages sounds, listener, and overal volumes
 * Sound files paths are hardcoded in SoundEngineManager.m */

@interface SoundEngineManager : NSObject {
@private
	NSMutableDictionary*		_sounds;
	
	
	Point3						_listenerPosition;
	Float32						_effectsVolume;
}

+ (SoundEngineManager*) sharedManager;

/* Returns the sound object associated with the identifier */
- (Sound*) soundWithIdentifier:(NSString*)identifier;

/* Smoothly shuts down all sounds */
- (void) fadeOut:(NSTimeInterval)time delay:(NSTimeInterval)delay;

/* Listener position. */
@property Point3		listenerPosition;

/* All effects volume. */
@property Float32		effectsVolume;
@end