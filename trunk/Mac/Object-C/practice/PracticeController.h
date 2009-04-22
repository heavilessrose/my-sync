//
//  PracticeController.h
//  practice
//
//  Created by wang luke on 4/22/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface PracticeController : NSObject {
	IBOutlet NSTextField *result;
	IBOutlet NSTextField *inputField;
	
	NSString *resultStr;
}

- (IBAction)showResult:(id)sender;

@end
