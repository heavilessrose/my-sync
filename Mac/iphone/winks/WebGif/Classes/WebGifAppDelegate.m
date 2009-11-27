//
//  WebGifAppDelegate.m
//  WebGif
//
//  Created by luke on 09-11-27.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "WebGifAppDelegate.h"

@implementation WebGifAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	
	GifWebPlayer *gifplayer = [[GifWebPlayer alloc] init];
	gifplayer.localHtml = @"playgif";
	
	[window addSubview:gifplayer.view];
	[gifplayer release];
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
