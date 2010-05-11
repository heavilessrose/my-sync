//
//  ContactList.m
//  msn_for_mac
//
//  Created by  apple on 08-9-25.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ContactList.h"
#import "GroupInfo.h"
#import "PersonInfo.h"
#import "ListViewCell.h"
#import "ListHeaderView.h"
#import "ListViewController.h"
#import "MSNAppDelegate.h"
#import "ClientNetWorkController.h"
#import "Advertisement.h"
#import "AdvertisementLabel.h"
#import "AdvertisementView.h"
#import "define.h"
#import "API.h"
#import "pcIndicatorView.h"

#import "ContactInfoViewController.h"

NSString		*PersonAddedNotification = @"PersonAdded";

#define FRESH_INTERFACE_INTERVAL	(0.100)

//static NSInteger s_Count = 0;
//static NSInteger s_Count2 = 0;

@interface ContactList (Private)
- (void)stopObservingDelContact;
@end

static ContactList *s_contactList = nil;
@implementation ContactList

@synthesize listView, contactList, groupList, customedGroups, sortByStatus,myAlertView,selectedIndexPath,listViewAdvertise, stateGroupList;
@synthesize indicator, delContactTimer;


- (pcIndicatorView*)createIndicator
{
	if (indicator == nil){
		indicator = [[pcIndicatorView alloc] initWithFrame: CGRectMake(0, 0, 140, 140)
													 title: @""
												background: nil];
	}
	return indicator;
}

- (void)observingDelContact: (NSTimer*)aTimer{
	if (stopDelContactTimer){
		[self stopObservingDelContact];
	}
}

- (void)startObservingDelContact{
	pcIndicatorView* anIndicator = [self createIndicator];
	[listView addSubview: anIndicator];
	CGPoint ptOffset = listView.contentOffset;
	anIndicator.center = CGPointMake(160, ptOffset.y + 200);
	[anIndicator startAnimation];
	
	stopDelContactTimer		= NO;
	
	self.delContactTimer = [NSTimer scheduledTimerWithTimeInterval: FRESH_INTERFACE_INTERVAL
															target: self
														  selector: @selector(observingDelContact:)
														  userInfo: nil
														   repeats: YES];
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
}

- (void)stopObservingDelContact{
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];
	[indicator stopAnimating];
	[indicator removeFromSuperview];
	[delContactTimer invalidate];
	self.delContactTimer = nil;
	self.selectedIndexPath = nil;
}



- (id) init
{
	self = [super init];
	if (self != nil)
	{
		sysDismiss = YES;
		contactList = [[NSMutableArray alloc] init];
		groupList = [[NSMutableArray alloc] init];
		nameGroupList = [[NSMutableArray alloc] init];
		stateGroupList = [[NSMutableArray alloc] init];
		[self newCustomedGroup];
		
		MSNAppDelegate *appdelegate = [MSNAppDelegate globalAppDelegate];
		infoDictionary = appdelegate.infoDict;
		disPlayOnlines = [[infoDictionary objectForKey:kDisplayOnLinesKey] boolValue];
		sortByStatus = [[infoDictionary objectForKey:@"sorttype"] boolValue];
		
		stopDelContactTimer		= YES;
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];

		[nc addObserver: self
			   selector: @selector(notifyUpdateOriginGroup:) 
				   name: UpdateGroupNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyUpdatePerson:)
				   name: UpdatePersonNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyDeleteOriginGroup:) 
				   name: DeleteGroupNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(notifyDeletePerson:)
				   name: DeletePersonNotification 
				 object: nil];
		
		[nc addObserver: self
			   selector: @selector(notifyReloadList:) 
				   name: ReloadListNotification 
				 object: nil];
		[nc addObserver: self
			   selector: @selector(notifyReceiveRosterFooterAd:) 
				   name: RosterFooterAdNotification 
				 object: nil];
	}
	return self;
}

- (void)newCustomedGroup
{
	self.customedGroups = [NSMutableArray arrayWithCapacity:0];
	GroupInfo *personGroup = [[GroupInfo alloc] init]; 
	personGroup.name = NSLocalizedString(@"qtn_msn_group_online",nil);
	[customedGroups insertObject:personGroup atIndex:0];
	[personGroup release];
	personGroup = [[GroupInfo alloc] init];
	personGroup.name = NSLocalizedString(@"qtn_msn_group_offline",nil);
	[customedGroups insertObject:personGroup atIndex:1];
	[personGroup release];
}

- (void) dealloc
{
	LogMethod();
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver: self
				  name: UpdateGroupNotification 
				object: nil];
	[nc removeObserver: self
				  name: UpdatePersonNotification 
				object: nil];
	[nc removeObserver: self
				  name: DeleteGroupNotification 
				object: nil];
	[nc removeObserver: self
				  name: DeletePersonNotification 
				object: nil];
	[nc removeObserver: self
			   name: ReloadListNotification 
			 object: nil];
	[nc removeObserver: self
				  name: RosterFooterAdNotification 
				object: nil];
	[nc removeObserver: self
				  name: AdImageChangedNotification 
				object: nil];
	
	if (myAlertView != nil)
	{
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}
	
	[nameGroupList release];
	[stateGroupList release];
	[listViewAdvertise release];
	[contactList release];
	[groupList release];
	[customedGroups release];
	[selectedIndexPath release];
	if (s_contactList == self)
		s_contactList = nil;
	
	[self stopObservingDelContact];
	[indicator release];
	[super dealloc];
}

+ (id)getInstance
{
	if (s_contactList == nil)
	{
		s_contactList = [[[ContactList alloc] init] autorelease];
	}
	return s_contactList;
}

- (void)resetGroup
{
	[infoDictionary removeObjectForKey:@"groupList"];
	[infoDictionary removeObjectForKey:@"contactList"];
	contactList = [[NSMutableArray alloc] init];
	groupList = [[NSMutableArray alloc] init];
	[self newCustomedGroup];
	[nameGroupList release];
    nameGroupList = nil;
	[stateGroupList release];
    stateGroupList = nil;
    [self showGroup: nil];
	nameGroupList = [[NSMutableArray alloc] init];
	stateGroupList = [[NSMutableArray alloc] init];
}

- (void)resetPersons
{
	[infoDictionary removeObjectForKey:@"contactList"];
	GroupInfo *group;
	for (group in groupList)
	{
		group.persons = [NSMutableArray arrayWithCapacity:0];
	}
	self.contactList = [NSMutableArray arrayWithCapacity:0];
}


- (IBAction)showGroup:(id)sender
{ 
	[listView reloadData];
}

- (void)setGroupInfo:(GroupInfo*)theGroup
{
	BOOL find = NO;
	if (theGroup != nil)
	{
		if (!sortByStatus)
		{
			GroupInfo *group;
			for (group in nameGroupList){
				if ([group.stringId compare: theGroup.stringId] == NSOrderedSame){
					find = YES;
					if (theGroup.sync == nil
						|| [theGroup.sync compare:@"deleted"] != NSOrderedSame){
						break;
					}
					else {
						[nameGroupList removeObject:group];
						break;
					}
				}
			}
			if (!find)
			{
				if (theGroup.sync == nil
					|| [theGroup.sync compare:@"deleted"] != NSOrderedSame)
				{
					int i = 0;
					for (; i < [nameGroupList count]; i++)
					{
						group = [nameGroupList objectAtIndex:i];
						NSComparisonResult ret = compareStrings(group.gbcode, theGroup.gbcode);
						if (ret == NSOrderedDescending
							||ret == NSOrderedSame)
						{
							break;
						}
					}
					[nameGroupList insertObject:theGroup atIndex:i];
				}
			}
			[listView reloadData];
		}
	}
}


- (void)setPersonInfo:(PersonInfo*)thePersonInfo withGroup:(NSString*)thePersonGroup
{
	GroupInfo *personGroup = nil;
	PersonInfo *person = nil;
	GroupInfo *group = nil;
	BOOL find = NO;

	NSInteger aSection = 0;
	NSInteger anIndex = 0;
	NSIndexPath* anIndexPath = nil;
	NSMutableArray* anIndexPaths = [[NSMutableArray alloc] init];
	
	for (group in nameGroupList) 
	{
		if (personGroup == nil)
		{
			if (thePersonGroup != nil)
			{
				if ([thePersonGroup compare:group.stringId] == NSOrderedSame)
				{
					personGroup = group;
				}
			}
			else
			{
				if ([group.stringId compare:@"0"] == NSOrderedSame)
				{
					personGroup = group;
				}
			}
		}
		
		for (person in personGroup.persons)
		{
			if (thePersonInfo.sync == nil
				|| [thePersonInfo.sync compare:@"deleted"] != NSOrderedSame)
			{
				if ([person.jid compare:thePersonInfo.jid] == NSOrderedSame)
				{
					find = YES;
					if (disPlayOnlines)
					{
						if ( thePersonInfo.state == nil
							||[thePersonInfo.state compare: @"offline"] == NSOrderedSame)
						{
							[personGroup.persons removeObject:person];
							anIndexPath = [NSIndexPath indexPathForRow: anIndex inSection: aSection];
							[anIndexPaths addObject: anIndexPath];
						}
					}
					break;
				}
			}
			else
			{
				if ([person.jid compare:thePersonInfo.jid] == NSOrderedSame)
				{
					find = YES;
					[group.persons removeObject:person];
					anIndexPath = [NSIndexPath indexPathForRow: anIndex inSection: aSection];
					[anIndexPaths addObject: anIndexPath];
					break;
				}
			}
			anIndex++;
		}
		
		aSection++;
		anIndex = 0;
	}
    
    while ([anIndexPaths count] > 0) {
        anIndexPath = [anIndexPaths lastObject];
        UITableViewCell* aCell = [listView cellForRowAtIndexPath: anIndex];
        if (aCell == nil){
            [anIndexPaths removeObject: anIndexPath];
        }
    }
	
	if ([anIndexPaths count] > 0){
		[listView beginUpdates];
		[listView deleteRowsAtIndexPaths: anIndexPaths withRowAnimation: UITableViewRowAnimationFade];
		[listView endUpdates];
	}
	
	if (!find)	
	{
		if (thePersonInfo.sync == nil
			|| [thePersonInfo.sync compare:@"deleted"] != NSOrderedSame)
		{
			if (personGroup == nil)
			{
				if (thePersonGroup != nil)
				{
					for (group in nameGroupList) 
					{
						if ([group.stringId compare:@"0"] == NSOrderedSame)
						{
							personGroup = group;
							break;
						}
					}
				}
				if (personGroup == nil)
				{
					personGroup = [[GroupInfo alloc] init];
					personGroup.stringId = @"0";
					personGroup.name = NSLocalizedString(@"Other contacts", nil);
					[nameGroupList addObject:personGroup];
					[personGroup release];
				}
				if (personGroup != nil){
					[self addPerson:thePersonInfo toGroup:personGroup];
					[listView reloadData];
				}
			}
			else
			{
				[self addPerson:thePersonInfo toGroup:personGroup];
				[listView reloadData];
			}	
		}
	}
	
	[anIndexPaths release];
}


- (void)setPersonInfo:(PersonInfo*)thePersonInfo
{ 
	if (!sortByStatus)
	{
		NSString *splitGroup;
		NSArray *persongroups;
		if (thePersonInfo.group != nil)
		{
			persongroups = [thePersonInfo.group componentsSeparatedByString:@","];
			for (splitGroup in persongroups)
			{
				[self setPersonInfo:thePersonInfo withGroup:splitGroup];
			}
		}
		else
		{
			[self setPersonInfo:thePersonInfo withGroup:nil];
		}
	}
	else
	{
		GroupInfo *personGroup = nil;
		PersonInfo *person = nil;
		GroupInfo *group = nil;
		BOOL find = NO;
		
		NSInteger anIndex = 0;
		NSInteger aSection = 0;
		NSIndexPath* anIndexPath = nil;
#if 0
		NSMutableArray* anIndexPaths = [[NSMutableArray alloc] init];
#else
		NSMutableArray* anIndexPaths = nil;
#endif
		for (group in stateGroupList)
		{
			if (personGroup == nil)
			{
				if ( thePersonInfo.state == nil
					|| [thePersonInfo.state compare: @"offline"] == NSOrderedSame)
				{
					if ([group.name isEqualToString: NSLocalizedString(@"qtn_msn_group_offline",nil)])
					{
						personGroup = group;
						aSection = 1;
					}
				}
				else
				{
					if ([group.name compare:NSLocalizedString(@"qtn_msn_group_online",nil)] == NSOrderedSame)
					{
						personGroup = group;
						aSection = 0;
					}
				}
			}
			for (person in group.persons)
			{
				if (thePersonInfo.sync == nil
					|| [thePersonInfo.sync compare:@"deleted"] != NSOrderedSame)
				{
					if ([person.jid compare:thePersonInfo.jid] == NSOrderedSame)
					{
						if ( thePersonInfo.state == nil
							||[thePersonInfo.state compare: @"offline"] == NSOrderedSame)
						{
							if ([group.name compare:NSLocalizedString(@"qtn_msn_group_online",nil)] == NSOrderedSame)
							{
								[group.persons removeObject:person];
							}
							else
							{
								find = YES;
							}
						}
						else
						{
							if ([group.name compare:NSLocalizedString(@"qtn_msn_group_online",nil)] == NSOrderedSame)
							{
								find = YES;
							}
							else
							{
								[group.persons removeObject:person];
#if 0
								if (aSection < [stateGroupList count] && ([listView numberOfRowsInSection: aSection] > anIndex)){
									anIndexPath = [NSIndexPath indexPathForRow: anIndex inSection: aSection];
									[anIndexPaths addObject: anIndexPath];
								}
#endif
							}
						}
						break;
					}
				}
				else
				{
					if ([person.jid compare:thePersonInfo.jid] == NSOrderedSame){
						find = YES;
						[group.persons removeObject:person];
#if 0					
						if (aSection < [stateGroupList count] && [listView numberOfRowsInSection: aSection] > anIndex){
							anIndexPath = [NSIndexPath indexPathForRow: anIndex inSection: aSection];
							[anIndexPaths addObject: anIndexPath];
						}
#endif
						break;
					}
				}
				anIndex++;
			}
			anIndex = 0;
		}
#if 0
		if ([anIndexPaths count] > 0){
			[listView beginUpdates];
			[listView deleteRowsAtIndexPaths: anIndexPaths withRowAnimation: UITableViewRowAnimationFade];
			[listView endUpdates];
		}
		[anIndexPaths release];
#endif
		if (!find)
		{
			if (thePersonInfo.sync == nil
				|| [thePersonInfo.sync compare:@"deleted"] != NSOrderedSame)
			{
				if (personGroup == nil)
				{
					personGroup = [[GroupInfo alloc] init];
					if (thePersonInfo.state == nil
						|| [thePersonInfo.state compare:@"offline"] == NSOrderedSame)
					{
						personGroup.name = NSLocalizedString(@"qtn_msn_group_offline",nil);
						[stateGroupList insertObject:personGroup atIndex:[stateGroupList count]];
					}
					else 
					{
						personGroup.name = NSLocalizedString(@"qtn_msn_group_online",nil);
						[stateGroupList insertObject:personGroup atIndex:0];
					}
					
					[self addPerson:thePersonInfo toGroup:personGroup];
					[personGroup release];
				}
				else
				{
					[self addPerson:thePersonInfo toGroup:personGroup];
				}	
			}
		}
//		[listView reloadData];
	}
	[listView reloadData];
}

- (void)sort:(BOOL)byStatus
{
	PersonInfo *person = nil;
	GroupInfo *group = nil;

	if (byStatus)
	{
		[nameGroupList release];
		nameGroupList = nil;
		sortByStatus = YES;
		stateGroupList = [[NSMutableArray alloc] initWithCapacity:0];
		for (group in customedGroups)
		{
			group.persons = [NSMutableArray arrayWithCapacity:0];
			[stateGroupList addObject:group];
		}
	}
	else
	{
		[stateGroupList release];
		stateGroupList = nil;
		nameGroupList = [[NSMutableArray alloc] initWithCapacity:0];
		sortByStatus = NO;
		for (group in groupList)
		{
			group.persons = [NSMutableArray arrayWithCapacity:0];
			[self setGroupInfo:group];
		}
	}
	[infoDictionary setObject:[NSNumber numberWithBool:byStatus] forKey:@"sorttype"];
	for (person in contactList)
	{
		[self setPersonInfo:person];
	}
}


- (void)find:(NSString*)theName
{
	if (theName != nil)
	{
		PersonInfo *person = nil;
		NSIndexPath *path = nil;
		NSArray *array = nil;
		if (sortByStatus)
			array = stateGroupList;
		else
			array = nameGroupList;

		BOOL find = NO;
		int i = 0, j = 0;
		for (; i < [array count]; i++)
		{
			GroupInfo *group = [array objectAtIndex:i];
			if (group != nil)
			{
				for (j = 0; j < [group.persons count]; j++)
				{
					person = [group.persons objectAtIndex:j];

					if (person.imid != nil)
					{
						if ([person.imid compare:theName] == NSOrderedSame)
						{
							find = YES;
							break;
						}
					}
				}
				if (find)
				{
					path = [NSIndexPath indexPathForRow:j inSection:i];
					if ([[self groupAtIndex:path].persons count] > j)
						[listView selectRowAtIndexPath:path animated:YES scrollPosition:UITableViewScrollPositionBottom];
					break;
				}	
			}
		}
		if (!find)
		{
			self.myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"qtn_main_buddy_search_buddy",nil) 
																  message:NSLocalizedString(@"not_find",nil)  
																 delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil) 
														otherButtonTitles:nil] autorelease];
			
			[myAlertView setDelegate:self];
			sysDismiss = YES;
			[myAlertView show];
		}
	}
}


- (void)addPerson:(PersonInfo*)thePerson
{
	[contactList addObject:thePerson];
	[self setPersonInfo:thePerson];
}

- (void)addGroup:(GroupInfo*)theGroup
{
	[groupList addObject:theGroup];
	[self setGroupInfo:theGroup];
}

- (PersonInfo*)mergelocalPerson: (PersonInfo*)aPersonInfo{
    PersonInfo* thePersonInfo = nil;
    if (aPersonInfo == nil || [aPersonInfo.jid length] == 0 || [aPersonInfo.imid length] == 0) return thePersonInfo;
    NSMutableDictionary* aDictPerson = nil;
    NSDictionary * anAccounts = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
    NSString *anAccountName = [[NSUserDefaults standardUserDefaults] objectForKey: kLastAccountKey];
    NSDictionary *anAccount = [anAccounts objectForKey: anAccountName];
    NSMutableArray* aPersons = [anAccount objectForKey: @"contactList"];
    for (aDictPerson in aPersons)
    {
        if ([[aDictPerson objectForKey: @"imid"] isEqualToString: aPersonInfo.imid] ||
            [[aDictPerson objectForKey: @"jid"] isEqualToString: aPersonInfo.jid]){
            thePersonInfo = [[PersonInfo alloc] initWithDictionary:aDictPerson];
            if ([aPersonInfo.nickname length] == 0){
                aPersonInfo.nickname = thePersonInfo.nickname;
            }
            
            if ([aPersonInfo.gbcode length] == 0){
                aPersonInfo.gbcode = thePersonInfo.gbcode;
            }
            
            if ([aPersonInfo.sync length] == 0){
                aPersonInfo.sync = thePersonInfo.sync;
            }
            
            if ([aPersonInfo.block length] == 0){
                aPersonInfo.block = thePersonInfo.block;
            }
            
            if ([aPersonInfo.impresa length] == 0){
                aPersonInfo.impresa = thePersonInfo.impresa;
            }
            
            if ([aPersonInfo.group length] == 0){
                aPersonInfo.group = thePersonInfo.group;
            }
            
            if ([aPersonInfo.portraithash length] == 0){
                aPersonInfo.portraithash = thePersonInfo.portraithash;
            }
            
            if ([aPersonInfo.spaces length] == 0){
                aPersonInfo.spaces = thePersonInfo.spaces;
            }
            
            if ([aPersonInfo.phone length] == 0){
                aPersonInfo.phone = thePersonInfo.phone;
            }
            
            if ([aPersonInfo.portraitType length] == 0){
                aPersonInfo.portraitType = thePersonInfo.portraitType;
            }
            
            if ([aPersonInfo.portraitData length] == 0){
                aPersonInfo.portraitData = thePersonInfo.portraitData;
            }

            if ([aPersonInfo.portraitURL length] == 0){
                aPersonInfo.portraitURL = thePersonInfo.portraitURL;
            }
            
            [thePersonInfo release];
            thePersonInfo = aPersonInfo;
            return thePersonInfo;
        }
    }
    return nil;
}

- (void)updatePersonInfo:(PersonInfo*)thePersonInfo
{
	BOOL find = NO;
	BOOL sort  = NO;
	GroupInfo *group = nil;
	NSArray *groups = nil;
	BOOL stateChanged = NO;
	PersonInfo * person = nil;
	
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	if (thePersonInfo.imid != nil && [thePersonInfo.imid compare:appdelegate.myAccount] == NSOrderedSame)
	{
		find = YES;
	}
	else if (thePersonInfo.jid != nil)
	{
		person = [self personWithJid:thePersonInfo.jid];
		if (person != nil)
		{
			if (person.imid != nil && [person.imid compare:appdelegate.myAccount] == NSOrderedSame)
			{
				find = YES;
			}
		}
	}
	
	if (find)
	{
		thePersonInfo.nickname = [infoDictionary objectForKey:@"nickname"];
		thePersonInfo.impresa = [infoDictionary objectForKey:@"impresa"];
		find = NO;
	}
		
	if (thePersonInfo.jid != nil)
	{
		person = nil;
		for (person in contactList)
		{
			if ([person.jid compare:thePersonInfo.jid] == NSOrderedSame)
			{
				if (thePersonInfo.sync == nil
					|| [thePersonInfo.sync compare:@"deleted"] != NSOrderedSame)
				{
					if (thePersonInfo.state != nil)	//sort by state ang gbcode
					{
						if (compareStrings(thePersonInfo.state, person.state) != NSOrderedSame)
						{
							sort = YES;
							stateChanged = YES;
						}
					}
					else if (thePersonInfo.gbcode != nil)
					{
						if (compareStrings(thePersonInfo.gbcode, person.gbcode) != NSOrderedSame)
							sort = YES;
					}
					
					if ([listView.delegate isKindOfClass: [ListViewController class]]){
						NSString* aNickName = nil;
						ListViewController* aController = (ListViewController*)listView.delegate;
						UIViewController* aTopController = [aController.navigationController visibleViewController];
						if ([aTopController isKindOfClass: [ContactInfoViewController class]]){
							PersonInfo* aPerson = ((ContactInfoViewController*)aTopController).personInfo;
							if ([aPerson.jid isEqualToString: thePersonInfo.jid] &&
								![person.nickname isEqualToString: thePersonInfo.nickname]){
								if ([thePersonInfo.nickname length] > 0){
									aNickName = thePersonInfo.nickname;
								}
								else {
									aNickName = (([person.nickname length] > 0) ? person.nickname : person.imid);
								}
								[aTopController updateNickName: aNickName];
							}
						}
					}
					[person updateInfo:thePersonInfo];
					
					if (disPlayOnlines || (stateChanged && sortByStatus))
					{
						[self setPersonInfo:person];
					}
					else
					{
						if (sort)
						{
							groups = [self groupWithPerson:person];
							for (group in groups)
							{
								[self updatePerson:person inGroup:group];	
							}
						}
						[listView reloadData];
					}
				}
				else if ([thePersonInfo.sync compare:@"deleted"] == NSOrderedSame)
				{
					[person updateInfo:thePersonInfo];
					[self setPersonInfo:person];
					[contactList removeObject:person];
				}
				find = YES;
				break;
			}
		}
		if (!find)
		{
			if (![thePersonInfo.sync isEqualToString: @"deleted"])
			{
				if (thePersonInfo.imid != nil){
                    PersonInfo* aMergePerson = [self mergelocalPerson: thePersonInfo];
                    if (aMergePerson == nil)
                        aMergePerson = thePersonInfo;
					[contactList addObject: aMergePerson];
                }
				
				if (thePersonInfo.sync != nil
					&& [thePersonInfo.sync compare:@"new"] == NSOrderedSame )
				{
					[[NSNotificationCenter defaultCenter] postNotificationName:PersonAddedNotification object:thePersonInfo];
				}
				
				[self setPersonInfo:thePersonInfo];	
			}
		}	
	}
}

- (void)updatGroupInfo:(GroupInfo*)theGroup
{
	BOOL find = NO;
	if (theGroup.stringId != nil)
	{
		GroupInfo * group = nil;
		for (group in groupList)
		{
			if ([group.stringId compare:theGroup.stringId] == NSOrderedSame)
			{
				find = YES;
				if (theGroup.sync == nil
					|| [theGroup.sync compare:@"deleted"] != NSOrderedSame)
				{
					[group updateInfo:theGroup];
				}
				else
				{
					[groupList removeObject:group];
				}
				break;
			}
		}
	}
	if (!find)
	{
		if (theGroup.sync == nil
			|| ([theGroup.sync compare:@"deleted"] != NSOrderedSame)
			)
		{
			[groupList addObject:theGroup];
			[self setGroupInfo:theGroup];		
		}
	}
	else
		[listView reloadData];
}

- (NSArray*)groupWithPerson:(PersonInfo*)thePerson
{
	GroupInfo * group = nil;
	NSArray *array = nil;

	if (sortByStatus)
		array = stateGroupList;
	else
		array = nameGroupList;

	NSMutableArray *groups = [NSMutableArray arrayWithArray:0];

	for (group in array)
	{
		if ([group.persons indexOfObject:thePerson] != NSNotFound)
			[groups addObject:group];
	}
	return groups;
}

- (GroupInfo*)groupAtIndex:(NSIndexPath*)theIndex
{
	GroupInfo * group = nil;
	
	NSArray *array = nil;
	if (sortByStatus)
		array = stateGroupList;
	else
		array = nameGroupList;

	if ([array count] > theIndex.section){
		group = [array objectAtIndex:theIndex.section];
    }
	
	return group;		
}

- (PersonInfo*)personAtIndex:(NSIndexPath*)theIndex
{
	PersonInfo *person = nil;
	GroupInfo *group = [self groupAtIndex:theIndex];
	
	if (group != nil)
	{
		if ([group.persons count] > theIndex.row){
			person = [group.persons objectAtIndex:theIndex.row];
        }
	}
	
	return person;
}

- (PersonInfo*)personWithJid:(NSString*)theJid
{
	BOOL aFound = NO;
	PersonInfo *person = nil;
	for (person in contactList)
	{
		if (person != nil && person.jid != nil)
		{
			if ([person.jid compare:theJid] == NSOrderedSame)
			{
				aFound = YES;
				break;
			}
		}
	}
	return (aFound ? person : nil);
}

- (PersonInfo*)personWithImid:(NSString*)theImid
{
	BOOL aFound = NO;
	PersonInfo *person = nil;
	for (person in contactList)
	{
		if (person != nil && person.imid != nil)
		{
			if ([person.imid compare:theImid] == NSOrderedSame)
			{
				aFound = YES;
				break;
			}
		}
	}
	return (aFound ? person : nil);
}


- (void)deletePersonAtIndexPath:(NSIndexPath*)thePath
{
	GroupInfo *group = [self groupAtIndex:thePath];
	
	if (group != nil && [group.persons count] > thePath.row)
	{
		if ([group.persons count] > thePath.row)
		{
			[group.persons removeObjectAtIndex:thePath.row];
			[listView deleteRowsAtIndexPaths:[NSArray arrayWithObject:thePath] withRowAnimation:UITableViewRowAnimationTop];
		}
	}
}

/////////////table view delegate and datasource method

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	NSArray *array = nil;
	if (sortByStatus)
		array = stateGroupList;
	else
		array = nameGroupList;
	int sections = [array count];
	if (listViewAdvertise.imageView != nil
		|| listViewAdvertise.textLabel != nil)
	{
		sections += 1;
	}

	if (sections == 0)
		sections = 1;
	return sections;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	// Number of rows is the number of names in the region dictionary for the specified section
	int count = 0;
	GroupInfo *group = [self groupAtIndex:[NSIndexPath indexPathForRow:0 inSection:section]];
	if (group.cell != nil)
	{
		if (!group.cell.closed)
		{
			count = [group.persons count];
		}
	}
	else
		count = [group.persons count];
	return count;
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return nil;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
	float height = 0;
	GroupInfo *group = [self groupAtIndex:[NSIndexPath indexPathForRow:0 inSection:section]];
	if (group != nil)
	{
		height = 27;
	}
	else
	{
		if (listViewAdvertise.imageView != nil)
		{
			height = listViewAdvertise.imageView.frame.size.height;
		}
		else if (listViewAdvertise.textLabel != nil)
		{
			height = listViewAdvertise.textLabel.frame.size.height;
		}
	}
	return height;
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
	UIView *view = nil;
	ListHeaderView *headerView = nil;
	GroupInfo *group = [self groupAtIndex:[NSIndexPath indexPathForRow:0 inSection:section]];
	if (group != nil)
	{
		if (group.cell == nil)
		{
			headerView = [[[ListHeaderView alloc] initWithFrame:CGRectZero groupInfo:group type:sortByStatus] autorelease];
			headerView.target = self;
			headerView.action = @selector(showGroup:);
		}
		else
		{
			headerView = group.cell;
			[headerView setGroup:group type:sortByStatus];
		}
		view = headerView;
	}
	else
	{
		if (listViewAdvertise.imageView != nil)
		{
			view = listViewAdvertise.imageView;
		}
		else if (listViewAdvertise.textLabel != nil)
		{
			view = listViewAdvertise.textLabel;	
		}
	}
	return view;
}

- (NSInteger)rowFromIndexPath: (NSIndexPath*)anIndexPath{
	NSInteger aRow = 0;
    NSInteger aSection = 0;
    GroupInfo* aGropInfo = nil;
	NSArray *aGroupsInfo = sortByStatus ? stateGroupList : nameGroupList;
	if ([aGroupsInfo count] > anIndexPath.section){
        for (; aSection < anIndexPath.section; aSection++){
            aGropInfo = [aGroupsInfo objectAtIndex: anIndexPath.section];
            aRow += [aGropInfo.persons count];
        }
        aRow += anIndexPath.row;
    }
	return aRow;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
#if 0
	ListViewCell *cell = nil;

	PersonInfo *person = [self personAtIndex:indexPath]; 
	
	if (person != nil)
	{
		if (person.cell == nil)
		{
			cell = [[[ListViewCell alloc] initWithFrame:CGRectZero personInfo:person] autorelease];			
		}
		else
		{
			cell = person.cell;
			[cell reloadText];
		}
	}
	else
	{
		cell = [[[ListViewCell alloc] initWithFrame:CGRectZero personInfo:person] autorelease];
	}
	return cell;
#else
    PersonInfo* aPerson = nil;
    NSInteger aRow = [self rowFromIndexPath: indexPath];
    NSString* aCellId = [NSString stringWithFormat: @"%d", aRow];
	UITableViewCell *aCell = [tableView dequeueReusableCellWithIdentifier: aCellId];
    if (aCell == nil){
        aPerson = [self personAtIndex:indexPath];
        aCell = [[[ListViewCell alloc] initWithFrame: CGRectZero personInfo: aPerson] autorelease];			
    }
    else {
        [aCell reloadText];
    }
    return aCell;
#endif
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle 
										forRowAtIndexPath:(NSIndexPath *)indexPath 
{
	NSString *aName = nil;
	UIAlertView* anAlertView = nil;
	self.selectedIndexPath = indexPath;
	PersonInfo *aPerson = [self personAtIndex: indexPath];
	if (aPerson != nil){
		aName = aPerson.nickname;
		if ([aName length] == 0)
			aName = aPerson.imid;
		if ([aPerson.block boolValue]){
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_delete_attache", nil)
													 message: [NSString stringWithFormat:NSLocalizedString(@"qtn_msn_delete_account_confirm", nil), aName] 
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_cancel", nil) 
										   otherButtonTitles: NSLocalizedString(@"qtn_ok", nil), nil];
		}
		else
		{
			anAlertView = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"qtn_delete_attache", nil)
													 message: [NSString stringWithFormat: NSLocalizedString(@"qtn_confirm_delete_buddy",nil), aName]
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_cancel", nil)
										   otherButtonTitles: NSLocalizedString(@"qtn_ok",nil), NSLocalizedString(@"qtn_msn_block_delete", nil), nil];
			
		}
		self.myAlertView = anAlertView;
		sysDismiss = YES;
		[myAlertView show];	
	}
	[anAlertView release];
}

- (void)updatePerson:(PersonInfo*)thePerson inGroup:(GroupInfo*)theGroup
{
	if (thePerson != nil && theGroup != nil)
	{
		thePerson = [thePerson retain];
		[theGroup.persons removeObject:thePerson];
		[self addPerson:thePerson toGroup:theGroup];
		[thePerson release];
	}
}


- (void)addPerson:(PersonInfo*)thePerson toGroup:(GroupInfo*)theGroup
{
	PersonInfo *info = nil;
	int i = 0;
	if (thePerson.imid != nil)
	{
		if (thePerson != nil && theGroup != nil)
		{
			int count = [theGroup.persons count];
			if (thePerson.state == nil || [[thePerson.state lowercaseString] compare:@"offline"] == NSOrderedSame)
			{
				if (!disPlayOnlines)
				{
					for (i = count - 1; i >= 0; i--)
					{		
						info  = [theGroup.persons objectAtIndex:i];
						if ([[info.state lowercaseString] compare:@"offline"] == NSOrderedSame)
						{
							NSComparisonResult ret = compareStrings(info.gbcode, thePerson.gbcode);
							if (ret == NSOrderedAscending
								||ret == NSOrderedSame)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
					[theGroup.persons insertObject:thePerson atIndex:i + 1];
				}
			}
			else
			{
				for (i = 0; i < count; i++)
				{		
					info  = [theGroup.persons objectAtIndex:i];
					if ([[info.state lowercaseString] compare:@"offline"] == NSOrderedSame)
					{
						break;
					}
					else
					{
						NSComparisonResult ret = compareStrings(thePerson.gbcode, info.gbcode);
						if (ret == NSOrderedAscending
							||ret == NSOrderedSame)
						{
							break;
						}
					}
				}
				[theGroup.persons insertObject:thePerson atIndex:i];
			}
			
		}		
	}
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!sysDismiss)
	{
		if ([alertView.title compare:NSLocalizedString(@"qtn_delete_attache",nil)] == NSOrderedSame)
		{
			PersonInfo *person = [self personAtIndex:selectedIndexPath]; 
			if (person != nil)
			{
#if 0
				if (buttonIndex == 1 || buttonIndex == 2){
					[[ClientNetWorkController getInstance] sendDeleteContactRequest: person.jid block: (buttonIndex == 2)];
					[self startObservingDelContact];
				}
				else {
					self.selectedIndexPath = nil;
				}
#else
				if (buttonIndex == 1)
				{
					[listView beginUpdates];
					[[ClientNetWorkController getInstance] sendDeleteContactRequest: person.jid block: NO];
					[self deletePersonAtIndexPath:selectedIndexPath];
					[listView endUpdates];
					self.selectedIndexPath = nil;
				}
				else if (buttonIndex == 2)
				{
					[listView beginUpdates];
					[[ClientNetWorkController getInstance] sendDeleteContactRequest:person.jid block: YES];
					[self deletePersonAtIndexPath:selectedIndexPath];
					[listView endUpdates];
					self.selectedIndexPath = nil;
				}
				else {
				}
#endif
			}
		}	
		self.myAlertView = nil;
	}
}

- (void)notifyReloadList: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	[listView reloadData];
}

- (void)notifyUpdateOriginGroup: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	GroupInfo *groupToUpdate = [[note userInfo] objectForKey:GroupInfoKey];
	[self updatGroupInfo:groupToUpdate];
}


- (void)notifyUpdatePerson: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	if (!stopDelContactTimer){
		stopDelContactTimer = YES;
		
	}
	PersonInfo *personToUpdate = [[note userInfo] objectForKey:PersonInfoKey];
	[self updatePersonInfo:personToUpdate];
}

- (void)notifyReceiveRosterFooterAd: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	NSDictionary * info = [note userInfo];
	if (info != nil)
	{
		id advt = [info objectForKey:@"advt"];
		if (advt != nil && [advt isKindOfClass:[Advertisement class]])
		{
			self.listViewAdvertise = advt;
			
			[[NSNotificationCenter defaultCenter] addObserver: self
													 selector: @selector(notifyAdImageChanged:) 
														 name: AdImageChangedNotification 
													   object: listViewAdvertise];
			
			[listView reloadData];
		}
	}
}


- (void)notifyAdImageChanged: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	[listView reloadData];
}

- (void)notifyDeleteOriginGroup: (NSNotification*)note
{
	self.contactList = nil;
	self.groupList = nil;
	[self resetGroup];
}

- (void)notifyDeletePerson: (NSNotification*)note{
	stopDelContactTimer = YES;
	[listView beginUpdates];
	[self deletePersonAtIndexPath: selectedIndexPath];
	[listView endUpdates];
	self.selectedIndexPath = nil;
	[self resetPersons];
}

- (void)notifyReceivedError: (NSNotification*)note{
	if (!stopDelContactTimer){
		stopDelContactTimer = YES;
	}
}

@end
