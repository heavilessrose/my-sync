//
//  SettingsViewController.h
//  TabiNavi
//
//  Created by wang luke on 6/30/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TextFieldCell.h"
#import "SwitchCell.h"


@interface SettingsViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource>{
	IBOutlet TextFieldCell *textFieldCell;
	IBOutlet SwitchCell *switchCell;
	NSArray *TableData;
}

@property (nonatomic, retain) TextFieldCell *textFieldCell;
@property (nonatomic, retain) SwitchCell *switchCell;
@property (nonatomic, retain) NSArray *TableData;

// 键盘Done事件处理
- (IBAction)fieldsDoneEditing:(id)sender;
// 点击背景关闭键盘
- (IBAction)backgroundClicked:(id)sender;
@end
