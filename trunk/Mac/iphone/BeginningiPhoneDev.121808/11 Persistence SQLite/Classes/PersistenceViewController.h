//
//  PersistenceViewController.h
//  Persistence
//
//  Created by Jeff LaMarche on 7/29/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "/usr/include/sqlite3.h"

#define kFilename		@"data.sqlite3"
#define kDataKey		@"Data"

@interface PersistenceViewController : UIViewController {
	IBOutlet UITextField *field1;
	IBOutlet UITextField *field2;
	IBOutlet UITextField *field3;
	IBOutlet UITextField *field4;
	
	sqlite3	*database;
}
@property (nonatomic, retain) UITextField *field1;
@property (nonatomic, retain) UITextField *field2;
@property (nonatomic, retain) UITextField *field3;
@property (nonatomic, retain) UITextField *field4;

- (NSString *)dataFilePath;
- (void)applicationWillTerminate:(NSNotification *)notification;
@end

