//
//  TextFieldCell.h
//  TabiNavi
//
//  Created by wang luke on 6/30/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

// the cell holding a UITextField
@interface TextFieldCell : UITableViewCell {
	IBOutlet UITextField *textField;
}

@property (nonatomic, retain) IBOutlet UITextField *textField;
@end
