//
//  OnlineListController.h
//  MSN
//
//  Created by  apple on 08-11-10.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@class ChatViewData;
@interface OnlineListController : UITableViewController
{
	NSMutableArray *persons;
	NSMutableArray *personsToAdd;
	ChatViewData *chatViewData;
}

- (void)addContactToConversation:(id)sender;
- (void)notifyPersonStateChanged: (NSNotification*)note;
- (void)participantChanged;

@property (nonatomic,assign)ChatViewData *chatViewData;
@end
