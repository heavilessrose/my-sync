//
//  LKLabelCell.m
//  Created by Luke on 7/1/09.
//

#import "LKLabelCell.h"

@implementation LKLabelCell
@synthesize label=_label;


- (id) initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if(!(self=[super initWithStyle:style reuseIdentifier:reuseIdentifier])) return nil;

    _label = [[UILabel alloc] initWithFrame:CGRectZero];
    _label.textAlignment = UITextAlignmentRight;
    _label.textColor = [UIColor grayColor];
    _label.font = [UIFont boldSystemFontOfSize:16.0];
    _label.adjustsFontSizeToFitWidth = YES;
    _label.baselineAdjustment = UIBaselineAdjustmentNone;
    _label.numberOfLines = 20;
	
	[self.contentView addSubview:_label];

    
    return self;
}
- (id) initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier {
	self = [self initWithStyle:UITableViewCellStyleDefault reuseIdentifier:reuseIdentifier];
	return self;
}
- (void) dealloc {
	[_label release];
    [super dealloc];
}


- (void) layoutSubviews {
    [super layoutSubviews];
	
	CGRect r = CGRectInset(self.contentView.bounds, 8, 8);
	r.size.width = 72;
	r.size.height = 30;
	_label.frame = r;
	
	
}



- (void) setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];
	_label.textColor = selected ? [UIColor whiteColor] : [UIColor grayColor];
}
- (void) setHighlighted:(BOOL)highlighted animated:(BOOL)animated{
	[super setHighlighted:highlighted animated:animated];
	_label.textColor = highlighted ? [UIColor whiteColor] : [UIColor grayColor];
}





@end
