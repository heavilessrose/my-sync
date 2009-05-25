//
//  PresidentsViewController.h
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SecondLevelViewController.h"

@class PresidentDetailController;

@interface PresidentsViewController : SecondLevelViewController
	<UITableViewDelegate, UITableViewDataSource> {
		NSMutableArray *list;
}
@property (nonatomic, retain) NSMutableArray *list;
@end
