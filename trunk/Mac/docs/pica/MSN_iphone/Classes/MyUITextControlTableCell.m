//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 009. All rights reserved.
//

#import "MyUITextControlTableCell.h"
#import "Constants.h"

@implementation MyUITextControlTableCell

@synthesize label, control;


- (id)initWithFrame:(CGRect)aRect reuseIdentifier:(NSString *)identifier
{
	self = [super initWithFrame:aRect reuseIdentifier:identifier];
	if (self)
	{
		self.selectionStyle = UITableViewCellSelectionStyleNone;
		self.accessoryType = UITableViewCellAccessoryNone;
		
		titleFirst = YES;
		
		label = [[UILabel alloc] initWithFrame:CGRectZero];	// layoutSubViews will decide the final frame
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor blackColor];
		label.highlightedTextColor = [UIColor whiteColor];
		label.font = [UIFont boldSystemFontOfSize:18];
		label.textAlignment = UITextAlignmentLeft;
		[self.contentView addSubview:label];
	}
	return self;
}

- (void)layoutSubviews
{
	[super layoutSubviews];
	
	CGRect contentRect = [self.contentView bounds];
	
	if(titleFirst)
	{
		int width = [label.text sizeWithFont: label.font].width + kTableCellLeftMargin;
		width = width > 80 ? width : 80;
		if(control.bounds.size.width && width > contentRect.size.width - control.bounds.size.width)
			width = contentRect.size.width - control.bounds.size.width - kTableCellLeftMargin; 
		int width2 = control.bounds.size.width;
		if(!width2)
			width2 = contentRect.size.width - width - 2 * kTableCellLeftMargin;
		
		CGRect frame = CGRectMake(	contentRect.origin.x + kTableCellLeftMargin,
								  contentRect.origin.y,
								  width,
								  kTableCellHeight);
		label.frame  = frame;
		
		frame = CGRectMake(contentRect.size.width - width2 - kTableCellLeftMargin,
						   round((contentRect.size.height - control.bounds.size.height) / 2.0),
						   width2,
						   control.bounds.size.height);
		
		control.frame  = frame;		
	}
	else
	{
		CGRect frame = CGRectMake(contentRect.origin.x,
								  round((contentRect.size.height - control.bounds.size.height) / 2.0),
								  control.bounds.size.width,
								  control.bounds.size.height);
		control.frame  = frame;
		
		frame = CGRectMake(control.bounds.size.width + contentRect.origin.x,
						   contentRect.origin.y,						   
						   contentRect.size.width - control.bounds.size.width - contentRect.origin.x - kTableCellLeftMargin,
						   contentRect.size.height);
		
		label.frame  = frame;				
	}
}

- (void)dealloc
{
	[label release];
	[control release];
    [super dealloc];
}

- (void)setTitle: (NSString*)title
{
    [label setText: title];
}


@end
