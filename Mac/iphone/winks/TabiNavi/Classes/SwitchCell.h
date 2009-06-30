//
//  SwitchCell.h
//  TabiNavi
//
//  Created by wang luke on 6/30/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SwitchCell : UITableViewCell {
	IBOutlet UISwitch *turnOnSwitch;
}

@property (nonatomic, retain) IBOutlet UISwitch *turnOnSwitch;
@end
