//
//  ChatSelectController.h
//  MSN
//
//  Created by  apple on 09-4-2.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ChatSelectController : UITableViewController
{
	BOOL sysDismiss;
	int currentIndex;
	UIAlertView *myAlertView;
	NSMutableDictionary * chatViewDatas;
}

-(void) refreshVC:(id)sender;
- (void)closeChatAtIndex:(int)index;
- (void)notifyContactStatusChanged: (NSNotification*)aNotify;

@property (nonatomic, assign) NSMutableDictionary *chatViewDatas;
@property (nonatomic, assign) NSInteger currentIndex;
@property (nonatomic, retain) UIAlertView *myAlertView;
@end
