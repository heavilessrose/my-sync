//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MyUITextControlTableCell.h"

@interface MyUITextSwitchTableCell : MyUITextControlTableCell
{
    UISwitch *uiSwitch;
	id		switchDelegate;
}
- (id)initWithFrame:(CGRect)aRect reuseIdentifier:(NSString *)anIdentifier delegate: (id)aSwitchDelegate;


- (void)setEnableSwitch: (BOOL)anEnable;
- (void)setValue: (BOOL)value;
- (BOOL)getValue;

@property (nonatomic, retain) UISwitch* uiSwitch;

@end
