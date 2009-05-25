//
//  RowControlsController.h
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#define kSwitchTag 100

#import <UIKit/UIKit.h>
#import "SecondLevelViewController.h"

@interface RowControlsController : SecondLevelViewController
		<UITableViewDelegate, UITableViewDataSource> {
	
	NSArray *list;
	
}
@property (nonatomic, retain) NSArray *list;
@end
