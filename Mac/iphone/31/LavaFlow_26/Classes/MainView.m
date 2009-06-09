//
//  MainView.m
//  LavaFlow
//
//  Created by apps on 8/26/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <MediaPlayer/MediaPlayer.h>
#import "MainView.h"

@implementation MainView

-(void)awakeFromNib {
	
	NSBundle *bundle = [NSBundle mainBundle];
	
	NSString *moviePath = [bundle pathForResource:@"lavaFlow" ofType:@"m4v"];

	NSURL *movieURL;
	
	if (moviePath)
	{
		movieURL = [NSURL fileURLWithPath:moviePath];
	}
	
	if (movieURL != nil) {
		moviePlayer = [[MPMoviePlayerController alloc] initWithContentURL:movieURL];
		
		// Register to receive a notification when the movie has finished playing. 
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(moviePlayBackDidFinish:) 
													 name:MPMoviePlayerPlaybackDidFinishNotification 
												   object:moviePlayer];
		
		moviePlayer.scalingMode = MPMovieScalingModeAspectFill; 
		moviePlayer.movieControlMode = MPMovieControlModeHidden;
		moviePlayer.backgroundColor = [UIColor blackColor];
		
		[moviePlayer play];
	}
	
}

-(void)moviePlayBackDidFinish: (NSNotification*)notification
{
	moviePlayer = [notification object];
	[moviePlayer play];
}

- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	// Drawing code
}

- (void)dealloc {
	[super dealloc];
}


@end
