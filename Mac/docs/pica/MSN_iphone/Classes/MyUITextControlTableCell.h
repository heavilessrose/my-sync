//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MyUITextControlTableCell : UITableViewCell
{
	UILabel     *label;
	UIControl   *control;
	BOOL         titleFirst;
}

@property (nonatomic, retain) UILabel     *label;
@property (nonatomic, retain) UIControl   *control;

- (void)setTitle: (NSString*)title;

@end
