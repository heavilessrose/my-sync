//
//  pcSearchContactsViewController.m
//  iSecret
//
//  Created by ancle on 10/6/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "pcSearchContactsViewController.h"
#import "ContactList.h"
#import "ListViewCell.h"
#import "PersonInfo.h"
#import "ContactInfoViewController.h"
#import "MSNAppDelegate.h"
#import "define.h"

@implementation pcSearchContactsViewController
@synthesize contacts;

- (void)showSessionController: (NSIndexPath*)anIndexPath{
	if (anIndexPath == nil) return;
	PersonInfo *person = [filterContacts objectAtIndex: anIndexPath.row];
	NSDictionary *info = [NSDictionary dictionaryWithObject:person forKey:@"key"];
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
    [self.navigationController dismissModalViewControllerAnimated: YES];
}

- (void)showContactInfoController: (NSIndexPath*)anIndexPath{
	if (anIndexPath == nil) return;
	ContactInfoViewController *aController = [[ContactInfoViewController alloc] init];
	aController.personInfo = [contacts objectAtIndex: anIndexPath.row];
	[self.navigationController pushViewController: aController animated: YES];
	[aController release];
}

- (NSMutableArray*)sortContacts: (NSMutableArray*)aContacts isOnLine: (BOOL)isOnLine{
    PersonInfo* aPerson = nil;
    NSMutableArray* theContacts = [[NSMutableArray alloc] init];
    if (isOnLine){
        for (aPerson in aContacts){
            if (!(([aPerson.state length] == 0) || ([aPerson.state  isEqualToString: @"offline"]))){
                [theContacts addObject: aPerson];
            }
        }
        for (aPerson in aContacts){
            if ((([aPerson.state length] == 0) || ([aPerson.state  isEqualToString: @"offline"]))){
                [theContacts addObject: aPerson];
            }
        }
    }
    else {
        for (aPerson in aContacts){
            if ((([aPerson.state length] == 0) || ([aPerson.state  isEqualToString: @"offline"]))){
                [theContacts addObject: aPerson];
            }
        }
        for (aPerson in aContacts){
            if (!(([aPerson.state length] == 0) || ([aPerson.state  isEqualToString: @"offline"]))){
                [theContacts addObject: aPerson];
            }
        }
    }
    return theContacts;
}

- (void)viewDidLoad{
    [super viewDidLoad];
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(notifynetworkDisabled:) 
                                                 name: NetWorkDisabledNotification 
                                               object: nil];
    
	filterContacts          = [[NSMutableArray alloc] init];
	saveContacts			= [[NSMutableArray alloc] init];
	searchBar.autocorrectionType = UITextAutocorrectionTypeNo;
	searchBar.autocapitalizationType = UITextAutocapitalizationTypeNone;
//	searchBar.keyboardType = UIKeyboardTypeEmailAddress;
    
	ContactList* aContactList= [ContactList getInstance];
    self.contacts           = [self sortContacts: aContactList.contactList isOnLine: YES];

	UIBarButtonItem *button = [[UIBarButtonItem alloc] initWithBarButtonSystemItem: UIBarButtonSystemItemCancel
                                                                            target: self
                                                                            action: @selector(cancel:)];
	
	self.navigationItem.leftBarButtonItem	= button;
	[button release];
    [filterContacts addObjectsFromArray: contacts];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
	return [filterContacts count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    PersonInfo *aPerson = nil;
	ListViewCell *aListCell = nil;
    NSString* aCellId = [NSString stringWithFormat: @"%d", indexPath.row];
	UITableViewCell *aCell = [tableView dequeueReusableCellWithIdentifier: aCellId];
	if (aCell == nil){
        aPerson = [filterContacts objectAtIndex: indexPath.row];
        aListCell = [[[ListViewCell alloc] initWithFrame:CGRectZero personInfo: aPerson] autorelease];
        aCell = aListCell;
    }
    else {
        aListCell = (ListViewCell*)aCell;
        [aListCell reloadText];
    }
    return aCell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	[contactsView deselectRowAtIndexPath:indexPath animated:NO];
    [searchBar resignFirstResponder];
	MSNAppDelegate *anAppDelegate = (MSNAppDelegate*)[MSNAppDelegate globalAppDelegate];
	PersonInfo* aPerson = [filterContacts objectAtIndex: indexPath.row];
	if (![aPerson.imid isEqualToString: anAppDelegate.myAccount]){
		[self showSessionController: indexPath];
	}
}

- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath{
	return UITableViewCellAccessoryDetailDisclosureButton;
}

- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)anIndexPath{
    [searchBar resignFirstResponder];
	[self showContactInfoController: anIndexPath];
}

- (void)searchBarTextDidBeginEditing:(UISearchBar *)aSearchBar{
	searchBar.showsCancelButton = YES;
	[saveContacts removeAllObjects];
	[saveContacts addObjectsFromArray: filterContacts];
}

- (void)searchBarTextDidEndEditing:(UISearchBar *)aSearchBar{
	searchBar.showsCancelButton = NO;
}

- (void)searchBar:(UISearchBar *)aSearchBar textDidChange:(NSString *)aSearchText{
    NSRange aRange;
	[filterContacts removeAllObjects];
    if ([aSearchText length] == 0){
        [filterContacts addObjectsFromArray: contacts];
    }
    else {
        PersonInfo* aPerson = nil;
        for (aPerson in contacts)
        {
            aRange = [[aPerson ex_nickname] rangeOfString: aSearchText options: NSCaseInsensitiveSearch];
            if (aRange.location != NSNotFound){
                [filterContacts addObject: aPerson];
            }
        }
    }
	[contactsView reloadData];
}

- (void)searchBarCancelButtonClicked:(UISearchBar *)aSearchBar{
	if (aSearchBar.text.length > 0){
		[filterContacts removeAllObjects];
		[filterContacts addObjectsFromArray: saveContacts];
	}
	[contactsView reloadData];
	[aSearchBar resignFirstResponder];
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)aSearchBar{
	[aSearchBar resignFirstResponder];
}

- (void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver: self
                                                    name: NetWorkDisabledNotification
                                                  object: nil];
	[contactsView release];
	[searchBar release];
    [contacts release];
	[filterContacts release];
	[saveContacts release];
	[super dealloc];
}

- (IBAction)cancel: (id)sender{
    [self.navigationController dismissModalViewControllerAnimated: YES];
}

- (void)notifynetworkDisabled: (NSNotification*)note{
    [self.navigationController dismissModalViewControllerAnimated: YES];
}

@end
