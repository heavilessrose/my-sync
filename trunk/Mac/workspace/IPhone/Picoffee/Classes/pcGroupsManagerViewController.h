//
//  pcGroupsManagerViewController.h
//  MSN
//
//  Created by  luke on 10-6-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "pcGroupsManagerTableCell.h"


@interface pcGroupsManagerViewController : UITableViewController <UITableViewDelegate, UIAlertViewDelegate, UIActionSheetDelegate>{
    pcGroupsManagerTableCell *tmpGroupCell;
}

@property (nonatomic, retain) pcGroupsManagerTableCell *tmpGroupCell;

@end
