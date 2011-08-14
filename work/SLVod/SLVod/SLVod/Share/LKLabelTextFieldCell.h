//
//  LKLabelTextfieldCell.h
//  Created by Luke on 7/1/09.
//


#import "LKLabelCell.h"

@interface LKLabelTextfieldCell : LKLabelCell<UITextFieldDelegate> {
	UITextField *_field;
}
@property (retain, nonatomic) UITextField *field;
@end
