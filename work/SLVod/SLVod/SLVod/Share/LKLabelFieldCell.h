//
//  LKLabelFieldCell.h
//  Created by Luke on 7/2/09.
//


#import "LKLabelCell.h"

@interface LKLabelFieldCell : LKLabelCell {
	UILabel *_field;
}
@property (retain, nonatomic) UILabel *field;
@end

