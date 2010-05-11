//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MyUITextAndValueTableCell : UITableViewCell
{
	UILabel			*titleLabel;
	UILabel			*valueLabel;
	BOOL			doubleLine;	
	int             leftMargin;
}

@property (nonatomic, retain) UILabel *titleLabel;
@property (nonatomic, retain) UILabel *valueLabel;

- (void)setTitle: (NSString*)title;
- (void)setValue: (NSString*)value;
- (void)setDoubleLine: (BOOL)isDoubleLine;
- (void)setChecked: (BOOL)checked;
@end
