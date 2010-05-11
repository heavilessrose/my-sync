//
//  pcSearchContactsViewController.h
//  iSecret
//
//  Created by ancle on 10/6/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface pcSearchContactsViewController : UIViewController<UITableViewDelegate, 
												 		     UITableViewDataSource, 
															 UISearchBarDelegate,
															 UIActionSheetDelegate>
{
	IBOutlet UITableView*           contactsView;
	IBOutlet UISearchBar*           searchBar;

	NSMutableArray*                 contacts;
	NSMutableArray*                 filterContacts;
	NSMutableArray*                 saveContacts;
}
@property(nonatomic, retain) NSMutableArray* contacts;

- (IBAction)cancel: (id)sender;
- (void)notifynetworkDisabled: (NSNotification*)note;

@end
