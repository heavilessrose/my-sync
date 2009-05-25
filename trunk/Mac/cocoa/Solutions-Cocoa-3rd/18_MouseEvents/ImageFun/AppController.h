//
//  AppController.h
//  ImageFun
//
//  Created by Aaron Hillegass on 10/18/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class StretchView;

@interface AppController : NSObject {
	IBOutlet StretchView *stretchView;
}
- (IBAction)showOpenPanel:(id)sender;
@end
