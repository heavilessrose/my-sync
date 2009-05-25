//
//  DeleteMeController.h
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SecondLevelViewController.h"

@interface DeleteMeController : SecondLevelViewController 
<UITableViewDelegate, UITableViewDataSource> {
	NSMutableArray *list;
}
@property (nonatomic, retain) NSMutableArray *list;
-(IBAction)toggleEdit:(id)sender;


@end
