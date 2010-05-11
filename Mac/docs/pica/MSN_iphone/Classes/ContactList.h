//
//  ContactList.h
//  msn_for_mac
//
//  Created by  apple on 08-9-25.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@class GroupInfo;
@class PersonInfo;
@class Advertisement;
@class pcIndicatorView;

@interface ContactList : NSObject <UITableViewDataSource,UIAlertViewDelegate>
{
	UITableView *listView;
	NSMutableArray *contactList;
	NSMutableArray *customedGroups;
	NSMutableArray *groupList;
	NSMutableArray *nameGroupList;
	NSMutableArray *stateGroupList;
	NSIndexPath *selectedIndexPath;
	NSMutableDictionary *infoDictionary;
	UIAlertView *myAlertView;
	Advertisement *listViewAdvertise;
	BOOL sysDismiss;
	BOOL sortByStatus;
	BOOL disPlayOnlines;

	pcIndicatorView*	indicator;
	NSTimer*			delContactTimer;
	BOOL				stopDelContactTimer;
}

+ (id)getInstance;

- (void)resetGroup;
- (void)resetPersons;

- (void)sort:(BOOL)byStatus;
- (void)find:(NSString*)theName;
- (IBAction)showGroup:(id)sender;

- (void)deletePersonAtIndexPath:(NSIndexPath*)thePath;
- (void)newCustomedGroup;

- (NSArray*)groupWithPerson:(PersonInfo*)thePerson;
- (GroupInfo*)groupAtIndex:(NSIndexPath*)theIndex;
- (PersonInfo*)personAtIndex:(NSIndexPath*)theIndex;
- (PersonInfo*)personWithJid:(NSString*)theJid;
- (PersonInfo*)personWithImid:(NSString*)theImid;
- (void)addPerson:(PersonInfo*)thePerson;
- (void)addGroup:(GroupInfo*)theGroup;
- (void)addPerson:(PersonInfo*)thePerson toGroup:(GroupInfo*)theGroup;
- (void)updatePerson:(PersonInfo*)thePerson inGroup:(GroupInfo*)theGroup;
- (void)notifyUpdateOriginGroup: (NSNotification*)note;
- (void)notifyUpdatePerson: (NSNotification*)note;
- (void)notifyDeleteOriginGroup: (NSNotification*)note;
- (void)notifyDeletePerson: (NSNotification*)note;
- (void)notifyReloadList: (NSNotification*)note;
- (void)notifyReceiveRosterFooterAd: (NSNotification*)note;
- (void)notifyAdImageChanged: (NSNotification*)note;
- (void)notifyReceivedError: (NSNotification*)note;

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section;
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section;


- (void)observingDelContact: (NSTimer*)aTimer;

@property (nonatomic) BOOL sortByStatus;
@property (nonatomic, retain) UIAlertView *myAlertView;
@property (nonatomic, assign) UITableView *listView;
@property (nonatomic, retain) NSMutableArray *contactList;
@property (nonatomic, retain) NSMutableArray *groupList;
@property (nonatomic, retain) NSMutableArray *stateGroupList;
@property (nonatomic, retain) NSMutableArray *customedGroups;
@property (nonatomic, retain) NSIndexPath *selectedIndexPath;
@property (nonatomic, retain) Advertisement *listViewAdvertise;

@property (nonatomic, retain) pcIndicatorView* indicator;
@property (nonatomic, retain) NSTimer* delContactTimer;

@end
