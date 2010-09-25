//
//  RootViewController.h
//  TableView
//
//  Created by iPhone SDK Articles on 1/17/09.
//  Copyright www.iPhoneSDKArticles.com 2009. 
//

#import <UIKit/UIKit.h>

@class OverlayViewController;

@interface RootViewController : UITableViewController {
	
	NSMutableArray *listOfItems;
	NSMutableArray *copyListOfItems;
	IBOutlet UISearchBar *searchBar;
	BOOL searching;
	BOOL letUserSelectRow;
	
	OverlayViewController *ovController;
}

- (UITableViewCell *) getCellContentView:(NSString *)cellIdentifier;
- (void) searchTableView;
- (void) doneSearching_Clicked:(id)sender;


@end
