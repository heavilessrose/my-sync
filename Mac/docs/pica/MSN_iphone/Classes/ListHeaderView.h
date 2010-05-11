//
//  ListHeaderView.h
//  MSN
//
//  Created by  apple on 08-10-20.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GroupInfo;
@interface ListHeaderView : UIToolbar
{
	BOOL closed;
	UIImageView *imageView;
	UILabel * label;
	NSString *title;
	BOOL moved;
	SEL action;
	id target;
}

- (id)initWithFrame:(CGRect)frame groupInfo:(GroupInfo*)group type:(BOOL)sortByStatus;
- (void)notifyGroupHerderClicked: (NSNotification*)note;
- (void)setGroup:(GroupInfo*)group type:(BOOL)sortByStatus;
- (void)forceUnfoldGroup;

@property (nonatomic, assign)NSString *title;
@property (nonatomic, assign)SEL action;
@property (nonatomic, assign)id target;
@property (nonatomic, readonly)BOOL closed;
@end
