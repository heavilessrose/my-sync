//
//  MyDocument.h
//  Departments
//
//  Created by Aaron Hillegass on 10/15/07.
//  Copyright __MyCompanyName__ 2007 . All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class ManagingViewController;

@interface MyDocument : NSPersistentDocument {
	IBOutlet NSBox *box;
	IBOutlet NSPopUpButton *popUp;
	NSMutableArray *viewControllers;
}
- (void)displayViewController:(ManagingViewController *)vc;

- (IBAction)changeViewController:(id)sender;

@end
