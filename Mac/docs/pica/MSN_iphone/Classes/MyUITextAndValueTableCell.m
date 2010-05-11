//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 009. All rights reserved.
//

#import "MyUITextAndValueTableCell.h"
#import "Constants.h"

@implementation MyUITextAndValueTableCell

@synthesize titleLabel, valueLabel;

- (id)initWithFrame:(CGRect)aRect reuseIdentifier:(NSString *)identifier
{
	self = [super initWithFrame:aRect reuseIdentifier:identifier];
	if (self)
	{
		self.selectionStyle = UITableViewCellSelectionStyleBlue;		
		self.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
		doubleLine = NO;
		leftMargin = kTableCellLeftMargin;
		
		titleLabel = [[UILabel alloc] initWithFrame:CGRectZero];	// layoutSubViews will decide the final frame
		titleLabel.backgroundColor = [UIColor clearColor];
		titleLabel.textColor = [UIColor blackColor];
		titleLabel.highlightedTextColor = [UIColor whiteColor];
		titleLabel.font = [UIFont boldSystemFontOfSize:18];
		titleLabel.textAlignment = UITextAlignmentLeft;
		[self.contentView addSubview:titleLabel];
		
		valueLabel = [[UILabel alloc] initWithFrame:CGRectZero];	// layoutSubViews will decide the final frame
		valueLabel.backgroundColor = [UIColor clearColor];
		valueLabel.textColor = [UIColor blueColor];
		valueLabel.highlightedTextColor = [UIColor whiteColor];
		valueLabel.font = [UIFont systemFontOfSize:16];
		valueLabel.textAlignment = UITextAlignmentRight;
		[self.contentView addSubview:valueLabel];
	}
	
	return self;
}

- (void)setDoubleLine: (BOOL)isDoubleLine
{
	doubleLine = isDoubleLine;
	titleLabel.numberOfLines = (doubleLine ? 2 : 1);
	if(doubleLine)
	{
		valueLabel.textAlignment = UITextAlignmentLeft;
	}
}

- (void)layoutSubviews
{
	[super layoutSubviews];
    CGRect contentRect = [self.contentView bounds];

	if(doubleLine)
	{
		int height = kTableCellDoubleHeight/2 - 2.0;
	
		CGRect frame = CGRectMake(contentRect.origin.x + leftMargin, 0,  contentRect.size.width - (contentRect.origin.x + leftMargin + kTableCellLeftMargin),  height);
		titleLabel.frame = frame;
		
		frame = CGRectMake(contentRect.origin.x + leftMargin, height, contentRect.size.width - (contentRect.origin.x + leftMargin + kTableCellLeftMargin), height);
		valueLabel.frame = frame;		
	}
	else
	{
		BOOL hasValue = ([[valueLabel text] length] > 0);		
		int width = [titleLabel.text sizeWithFont: titleLabel.font].width + kTableCellLeftMargin;
		width = width > 80 ? width : 80;
		if(hasValue && width > 200)
			width = 200;
		CGRect frame = CGRectMake(contentRect.origin.x + leftMargin, 0,  hasValue ? width : (contentRect.size.width - (contentRect.origin.x + leftMargin + kTableCellLeftMargin)), kTableCellHeight);
		titleLabel.frame = frame;
		
		frame = hasValue ? CGRectMake(contentRect.origin.x + width + leftMargin, 0, contentRect.size.width - (contentRect.origin.x + width + leftMargin + kTableCellLeftMargin), kTableCellHeight) : CGRectZero;
		valueLabel.frame = frame;
	}
}

- (void)dealloc
{
	[titleLabel release];
	[valueLabel release];
    [super dealloc];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
	[super setSelected:selected animated:animated];
	titleLabel.highlighted = selected;
}


- (void)setTitle: (NSString*)title
{
    [titleLabel setText: title];
}

- (void)setValue: (NSString*)value
{
	[valueLabel setText: value];
}

- (void)setChecked: (BOOL)checked
{
	self.accessoryType = checked ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;	
}


@end
