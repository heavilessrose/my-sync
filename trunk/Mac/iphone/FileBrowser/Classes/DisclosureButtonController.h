//
//  DisclosureButtonController.h
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SecondLevelViewController.h"
@class DisclosureDetailController;

@interface DisclosureButtonController : SecondLevelViewController <UITableViewDelegate, UITableViewDataSource> {
	NSArray *list;
	DisclosureDetailController *childController;
}
@property (nonatomic, retain) NSArray *list;
@end
