/*
     File: MyViewController.h
 Abstract: The main view controller of this app
  Version: 1.2
 */

#import <UIKit/UIKit.h>

@interface MyViewController : UIViewController <UINavigationBarDelegate, UITableViewDelegate, UITableViewDataSource>
{
	UIView			*containerView;
	
	UITableView		*myTableView;
	IBOutlet UIView	*instructionsView;
	
	NSString		*firstNameStr;
	NSString		*lastNameStr;
	
	UIBarButtonItem *flipButton;
	UIBarButtonItem *doneButton;
}

@property (nonatomic, retain) UIView *containerView;
@property (nonatomic, retain) UITableView *myTableView;
@property (nonatomic, retain) UIView *instructionsView;
@property (nonatomic, retain) NSString *firstNameStr;
@property (nonatomic, retain) NSString *lastNameStr;
@property (nonatomic, retain) UIBarButtonItem *flipButton;
@property (nonatomic, retain) UIBarButtonItem *doneButton;

@end