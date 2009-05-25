//
//  RootViewController.h
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface RootViewController : UITableViewController 
	<UITableViewDelegate, UITableViewDataSource> {
		NSArray	*controllers;
}
@property (nonatomic, retain) NSArray *controllers;
@end
