//
//  AppController.h
//  KVCFun
//
//  Created by Aaron Hillegass on 9/12/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface AppController : NSObject {
	int fido;
}
- (IBAction)incrementFido:(id)sender;
- (int)fido;
- (void)setFido:(int)x;

@end
