//
//  DisclosureButtonController.h
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SecondLevelViewController.h"
@class DisclosureDetailController;

@interface DisclosureButtonController : SecondLevelViewController 
		<UITableViewDelegate, UITableViewDataSource> {
			NSArray	*list;
			DisclosureDetailController *childController;

}
@property (nonatomic, retain) NSArray *list;
@end
