//
//  LKLabelFieldCell.m
//  Created by Luke on 7/2/09.
//

#import "LKLabelFieldCell.h"


@implementation LKLabelFieldCell
@synthesize field=_field;


- (id) initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if(!(self=[super initWithStyle:style reuseIdentifier:reuseIdentifier])) return nil;
	
	_field = [[UILabel alloc] initWithFrame:CGRectZero];
    _field.font = [UIFont boldSystemFontOfSize:16.0];
	[self.contentView addSubview:_field];

    return self;
}
- (id) initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier {
	self=[self initWithStyle:UITableViewCellStyleDefault reuseIdentifier:reuseIdentifier];
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
	_field.frame = r;
	
}
- (void) setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];
	_field.textColor = selected ? [UIColor whiteColor] : [UIColor blackColor];
}
- (void) setHighlighted:(BOOL)highlighted animated:(BOOL)animated{
	[super setHighlighted:highlighted animated:animated];
	_field.textColor = highlighted ? [UIColor whiteColor] : [UIColor blackColor];
}





@end
