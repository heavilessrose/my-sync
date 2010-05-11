//
//  LoginSelectViewController.m
//  MSN
//
//  Created by  apple on 08-10-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "LoginSelectViewController.h"
#import "LoginViewController.h"
#import "StatusViewDatasource.h"
#import "AccountViewDatasource.h"
#import "DomainSelectViewDatasource.h"
#import "define.h"
#import "API.h"

@implementation LoginSelectViewController

@synthesize type, loginController;

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) 
	{
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(notifyDeleteAccount:)
				   name:DeleteAccountNotification 
				 object:nil];	
	}
	return self;
}

// If you need to do additional setup after loading the view, override viewDidLoad.
- (void)viewDidLoad 
{
	switch (type) {
		case AccountView:
			self.title = NSLocalizedString(@"Previous e-mail address",nil);
			datasource = [[AccountViewDatasource alloc] init];
			if ([loginController.emailTextField.text rangeOfString:@"@"].location == NSNotFound)
			{
				((AccountViewDatasource*)datasource).selectedAccount = [[loginController email] stringByAppendingString:loginController.domain];
			}else {
				((AccountViewDatasource*)datasource).selectedAccount = [loginController email];
			}

			break;
		case StatusView:
			self.title = NSLocalizedString(@"qtn_msn_status_type",nil);
			datasource = [[StatusViewDatasource alloc] init];
			((StatusViewDatasource*)datasource).selectedStatus = loginController.status;
			break;
		case DomainView:
			self.title = NSLocalizedString(@"qtn_msn_domain_select",nil);
			datasource = [[DomainSelectViewDatasource alloc] init];
			((DomainSelectViewDatasource*)datasource).selectedDomain = loginController.domain;
			break;

		default:
			break;
	}
	

	UITableView *view = (UITableView*)self.view;
	view.delegate = self;
	view.dataSource = datasource;
}

- (void)viewWillAppear: (BOOL)animated{
	[super viewWillAppear: animated];
	[self.navigationController setNavigationBarHidden: NO animated: NO];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[self.navigationController setNavigationBarHidden:YES animated:NO];
	[super viewWillDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc 
{
	UITableView *view = (UITableView*)self.view;
	view.delegate = nil;
	view.dataSource = nil;
	[datasource release];
	[super dealloc];
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
	switch (type) {
		case AccountView:
		{
			NSRange range = [cell.text rangeOfString:@"@"];
			NSString *theDomain = [cell.text substringFromIndex:range.location];
			NSString *theAccount = [cell.text substringToIndex:range.location];

			[loginController selectedAccount:theAccount Domain:theDomain]; 
			((AccountViewDatasource*)datasource).selectedAccount = cell.text;

			break;			
		}
		case StatusView:
			[loginController selectedStatus:cell.text];	
			((StatusViewDatasource*)datasource).selectedStatus = cell.text;
			break;
		case DomainView:
		{
			NSString * sDomain = ((DomainSelectViewDatasource*)datasource).selectedDomain;
		
			if ([sDomain compare:NSLocalizedString(@"qtn_msn_account_other",nil)] == NSOrderedSame)
			{
				// Remove content from @ to the end.
				NSString *sText = loginController.emailTextField.text;
				NSRange range = [sText rangeOfString:@"@"];
				if (range.location != NSNotFound)
				{
					loginController.emailTextField.text = [sText substringToIndex:range.location];
				}
			}
			[loginController selectedDomain:cell.text];	
			break;
		}
		default:
			break;
	}
	
	[tableView reloadData];
	[self.navigationController popViewControllerAnimated:YES];
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
	return 0;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return 40;
}

- (void)notifyDeleteAccount: (NSNotification*)note
{
	if (	[[((AccountViewDatasource*)datasource).source allKeys] count] > 0)
	{
		NSIndexPath *indexPath = [[note userInfo] objectForKey:@"indexPath"];
		if (indexPath.row > 0)
			indexPath = [NSIndexPath indexPathForRow:indexPath.row-1  inSection:0];
		else
			indexPath = [NSIndexPath indexPathForRow:indexPath.row+1  inSection:0];
		
		[(UITableView*)self.view selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionNone];	
		[self tableView:(UITableView*)self.view didSelectRowAtIndexPath:indexPath];
	}
	else
	{
		[loginController selectedAccount:@"" Domain:nil]; 
	}
}

- (void)setStatus: (NSString*)aStatus{
	((StatusViewDatasource*)datasource).selectedStatus = aStatus;
}

- (NSString*)status{
	return ((StatusViewDatasource*)datasource).selectedStatus;
}

@end
