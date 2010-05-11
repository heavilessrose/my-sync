//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 009. All rights reserved.
//

#import "MyUITextSwitchTableCell.h"
#import "Constants.h"

@implementation MyUITextSwitchTableCell
@synthesize uiSwitch;

- (id)initWithFrame:(CGRect)aRect reuseIdentifier:(NSString *)anIdentifier delegate: (id)aSwitchDelegate
{
	self = [super initWithFrame: aRect reuseIdentifier: anIdentifier];
	if (self)
	{
		uiSwitch = [[UISwitch alloc] initWithFrame: CGRectMake(0, 0, kSwitchButtonWidth, kSwitchButtonHeight)];
		uiSwitch.backgroundColor = [UIColor clearColor];
		[self.contentView addSubview:uiSwitch];
		control = uiSwitch;
		switchDelegate = aSwitchDelegate;
		
		[uiSwitch addTarget: self
					 action: @selector(switchAction:)
		   forControlEvents: UIControlEventValueChanged];

		label.font = [UIFont systemFontOfSize: 16];
	}
	return self;
}

- (void)setEnableSwitch: (BOOL)anEnable
{
	uiSwitch.enabled		= anEnable;
}

- (void)setValue: (BOOL)value
{
	uiSwitch.on = value;
}

- (BOOL)getValue
{
	return uiSwitch.on;
}

- (IBAction)switchAction: (id)sender{
	if ([switchDelegate respondsToSelector: @selector(switchAction:)]){
		[switchDelegate switchAction: self];
	}
}

@end
