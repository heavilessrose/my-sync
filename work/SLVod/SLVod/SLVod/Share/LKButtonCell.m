//
//  LKButtonCell.m
//  Created by Luke on 7/2/09.
//

#import "LKButtonCell.h"


@implementation LKButtonCell

- (id) initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
	if(!(self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) return nil;
	self.textLabel.textAlignment = UITextAlignmentCenter;
	self.textLabel.font = [UIFont boldSystemFontOfSize:18.0];
    return self;
}

- (id) initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier {
	return [self initWithStyle:UITableViewCellStyleDefault reuseIdentifier:reuseIdentifier];
}


- (void) layoutSubviews {
	[super layoutSubviews];
	CGRect r = CGRectInset(self.contentView.bounds , 20, 8);
	self.textLabel.frame = r;
}




- (void) setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];
	
	if(selected)
		self.textLabel.textColor = [UIColor whiteColor];
	else
		self.textLabel.textColor = [UIColor colorWithRed:74/255.0 green:110/255.0 blue:165/255.0 alpha:1.0];

}
- (void) setHighlighted:(BOOL)highlight animated:(BOOL)animated {
	
    [super setHighlighted:highlight animated:animated];
	
	if(highlight)
		self.textLabel.textColor = [UIColor whiteColor];
	else
		self.textLabel.textColor = [UIColor colorWithRed:74/255.0 green:110/255.0 blue:165/255.0 alpha:1.0];
	
}





@end
