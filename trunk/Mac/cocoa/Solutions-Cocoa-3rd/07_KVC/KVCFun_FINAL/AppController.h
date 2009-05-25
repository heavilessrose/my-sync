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
@property (readwrite, assign) int fido;

- (IBAction)incrementFido:(id)sender;

@end
