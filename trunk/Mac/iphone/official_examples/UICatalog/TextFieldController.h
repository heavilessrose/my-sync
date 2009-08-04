/*

File: TextFieldController.h
Abstract: The view controller for hosting the UITextField features of this
sample.

Version: 1.7

*/

#import <UIKit/UIKit.h>
#import "CellTextField.h"

@interface TextFieldController : UIViewController <UIScrollViewDelegate, UITextFieldDelegate,
												   UITableViewDelegate, UITableViewDataSource,
												   EditableTableViewCellDelegate>

{
	UITableView		*myTableView;
	UITextField		*savedFirstResponder;
	
	UITextField		*textField;
	CellTextField	*textFieldCell;			// kept track for editing
	
	UITextField		*textFieldRounded;
	CellTextField	*textFieldRoundedCell;	// kept track for editing
	
	UITextField		*textFieldSecure;
	CellTextField	*textFieldSecureCell;	// kept track for editing
}

@property (nonatomic, retain) UITableView *myTableView;

@end

