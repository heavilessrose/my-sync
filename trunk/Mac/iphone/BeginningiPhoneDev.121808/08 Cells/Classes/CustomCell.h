//
//  CustomCell.h
//  Cells
//
//  Created by Jeff LaMarche on 7/12/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CustomCell : UITableViewCell {
	IBOutlet	UILabel	*nameLabel;
	IBOutlet	UILabel *colorLabel;
}
@property (nonatomic, retain) UILabel *nameLabel;
@property (nonatomic, retain) UILabel *colorLabel;
@end
