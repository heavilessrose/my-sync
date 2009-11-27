//
//  WebGifAppDelegate.h
//  WebGif
//
//  Created by luke on 09-11-27.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GifWebPlayer.h"

@interface WebGifAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

