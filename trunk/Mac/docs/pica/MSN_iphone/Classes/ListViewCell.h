//
//  ListViewCell.h
//  msn_for_mac
//
//  Created by  apple on 08-9-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PersonInfo;
@class ImageLabel;

@interface ListViewCell : UITableViewCell 
{
	PersonInfo *personinfo;
	ImageLabel *nameLabel;
	ImageLabel *infoLabel;
	UIImageView *imageView; 
	UIImageView *spacesView;
	BOOL showBigImage;
	BOOL isSelected;
}

- (id)initWithFrame:(CGRect)frame personInfo:(PersonInfo *)info;
- (void)reloadText;
- (void)notifyGroupHerderClicked: (NSNotification*)note;
@property (nonatomic, assign) PersonInfo *personinfo;
@property (nonatomic, assign) BOOL isSelected;
@end
