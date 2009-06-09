//
//  MainView.h
//  iDrum
//
//  Created by apps on 10/22/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SoundEngine.h"

#define kListenerDistance			1.0  // Used for creating a realistic sound field

@interface MainView : UIView {

}
- (IBAction)drum;

- (void) startBackgroundMusic;

@end
