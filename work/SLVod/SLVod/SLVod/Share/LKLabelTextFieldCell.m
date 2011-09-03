//
//  LKLabelTextfieldCell.m
//  Created by Luke on 7/1/09.
//

#import "LKLabelTextfieldCell.h"


@implementation LKLabelTextfieldCell
@synthesize field=_field;


- (id) initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier{
	if(!(self=[super initWithStyle:style reuseIdentifier:reuseIdentifier])) return nil;
    
    _field = [[UITextField alloc] initWithFrame:CGRectZero];
    _field.autocorrectionType = UITextAutocorrectionTypeYes;
    _field.font = [UIFont boldSystemFontOfSize:16.0];
    _field.delegate = self;
    [self.contentView addSubview:_field];
		
    
    return self;
}
- (id) initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier {
	self = [self initWithStyle:UITableViewCellStyleDefault reuseIdentifier:reuseIdentifier];
	return self;
}
- (void) dealloc {
	[_field release];
	[super dealloc];
}



- (void) layoutSubviews {
    [super layoutSubviews];
	
	CGRect r = CGRectInset(self.contentView.bounds, 8, 8);
	r.origin.x += self.label.frame.size.width + 6;
	r.size.width -= self.label.frame.size.width + 6;
	r.origin.y += 10;
	_field.frame = r;
	
	
}

- (BOOL) textFieldShouldReturn:(UITextField *)textField{
	[_field resignFirstResponder];
	return NO;
}

//- (void) setSelected:(BOOL)selected animated:(BOOL)animated {
//    [super setSelected:selected animated:animated];
//	_field.textColor = selected ? [UIColor whiteColor] : [UIColor blackColor];
//}
//- (void) setHighlighted:(BOOL)highlighted animated:(BOOL)animated{
//	[super setHighlighted:highlighted animated:animated];
//	_field.textColor = highlighted ? [UIColor whiteColor] : [UIColor blackColor];
//}


@end
