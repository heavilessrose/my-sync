//
//  LoginViewController.m
//  msn_for_mac
//
//  Created by  apple on 08-9-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "LoginViewController.h"
#import "LoginSelectViewController.h"
#import "define.h"
#import "API.h"
#import "public.h"
#import "MyUITextAndValueTableCell.h"
#import "MSNAppDelegate.h"

#import <QuartzCore/CALayer.h>

enum {
	ALERTREASON_FOR_NONE			= 0,
	ALERTREASON_FOR_SAVE_PASSWORD,
	ALERTREASON_FOR_AUTO_LOGIN,
};

NSString * LoginActionNotification = @"LoginAction";

#define MIN_VALID_PWD_CHAR			33
#define MAX_VALID_PWD_CHAR			(MIN_VALID_PWD_CHAR + 93)

@implementation LoginViewController
@synthesize emailTextField;
@synthesize status;
@synthesize domain;

- (BOOL)isValidPwdString: (NSString*)aString{
	BOOL ret = YES;
	const char* p = [aString UTF8String];
	for (NSInteger i = 0; i < strlen(p); i++){
		if (p[i] < MIN_VALID_PWD_CHAR && p[i] > MAX_VALID_PWD_CHAR){
			ret = NO;
			break;
		}
	}
	return ret;
}

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		NSString * string = nil;
		NSDictionary * accounts = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
		if ([accounts count] > 0){
			NSString *fullAccount = [[NSUserDefaults standardUserDefaults] objectForKey: kLastAccountKey];
			if ([fullAccount length] == 0)
				fullAccount = [[accounts allKeys] objectAtIndex: 0];
			if ([fullAccount length] > 0){
				NSDictionary *account = [accounts objectForKey:fullAccount];
				if (account != nil){
					string = [account objectForKey: kLoginStatusKey];
				}
			}
		}
		if (string == nil)
			string = @"online";
		self.status = getStatusString(string, NO);
		
		NSString * sDomain = [[NSUserDefaults standardUserDefaults] objectForKey:@"loginDomain"];
		if (sDomain == nil)
			sDomain = @"@hotmail.com";
		self.domain = sDomain;
	}
	return self;
}

// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
	NSString *string = nil;
	int xPoint = 0;
	CGRect rect = CGRectMake(0, 0, 320, 51);
	UIImageView *imageview = [[UIImageView alloc] initWithFrame:rect];
	string = [[NSBundle mainBundle] pathForResource:@"LoginTitle" ofType:@"png"];
	imageview.image = [UIImage imageWithContentsOfFile:string];
	[self.view addSubview:imageview];
	[imageview release];
	
	rect = CGRectMake(17, 56, 290, 26);
	UILabel *label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"qtn_msn_full_name",nil);
	label.text = [string stringByAppendingString:@":"];
	[self.view addSubview:label];
	[label release];
	rect = CGRectMake(17, 85, 286, 36);
	emailTextField = [[UITextField alloc] initWithFrame:rect];
	emailTextField.borderStyle = UITextBorderStyleRoundedRect;
	emailTextField.placeholder = @"xxx@msn.com";
	emailTextField.autocorrectionType = UITextAutocorrectionTypeNo;	// no auto correction support
	emailTextField.autocapitalizationType = UITextAutocapitalizationTypeNone;
	emailTextField.returnKeyType = UIReturnKeyDone;
	emailTextField.keyboardType = UIKeyboardTypeEmailAddress;
	emailTextField.clearButtonMode = UITextFieldViewModeWhileEditing;	// has a clear 'x' button to the right
	[self.view addSubview:emailTextField];
	[emailTextField release];
	
	domainsButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	domainsButton.frame = CGRectMake(154, 131, 150, 32);
	[domainsButton setTitle:NSLocalizedString(@"qtn_msn_domain_type",nil) forState:UIControlStateNormal];
	[domainsButton addTarget:self action:@selector(domainButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:domainsButton];	
	
	rect = CGRectMake(17, 167, 280, 26);
	label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"qtn_msn_pwd",nil);
	label.text = [string stringByAppendingString:@":"];
	[self.view addSubview:label];
	[label release];
	
	rect = CGRectMake(17, 196, 286, 36);
	passwordTextField = [[UITextField alloc] initWithFrame:rect];
	passwordTextField.borderStyle = UITextBorderStyleRoundedRect;
	passwordTextField.secureTextEntry = YES;
	passwordTextField.autocorrectionType = UITextAutocorrectionTypeNo;
	passwordTextField.clearButtonMode = UITextFieldViewModeWhileEditing;
	passwordTextField.returnKeyType = UIReturnKeyDone;
	[self.view addSubview:passwordTextField];
	[passwordTextField release];
	
	rect = CGRectMake(17, 246, 196, 26);
	label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"pica_str_label_login_insivibly",nil);
	label.text = [string stringByAppendingString:@":"];
	label.frame = rect;
	[self.view addSubview:label];
	[label release];
	rect.origin.x = 207;
	rect.size.width = 90;
	rect.size.height = 27;
	xPoint = rect.origin.x;
	saveButton = [[UISwitch alloc] initWithFrame:rect];
	[saveButton addTarget:self action:@selector(saveButtonPressed:) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview:saveButton];
	[saveButton release];
	
	rect = CGRectMake(17, 285, 189, 27);
	label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"pica_str_label_auto_login",nil);
	label.text = [string stringByAppendingString:@":"];
	label.font = [UIFont systemFontOfSize: 17];
	label.frame = rect;
	[self.view addSubview:label];
	[label release];
	rect.origin.x = 207;
	rect.size.width = 90;
	rect.size.height = 27;
	autoLoadButton = [[UISwitch alloc] initWithFrame: rect];
	[autoLoadButton addTarget: self action:@selector(autoLoadButtonPressed:) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview: autoLoadButton];
	[autoLoadButton release];

	rect = CGRectMake(8, 320, 304, 44);
	stateTable = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
	[self.view addSubview:stateTable];
	[stateTable release];
	
	rect = CGRectMake(0, 364, 320, 116);
	UIImageView* anImageView = [[UIImageView alloc] initWithFrame: rect];
	anImageView.image = [UIImage imageNamed: @"Login_bg.png"];
	[self.view addSubview: anImageView];
	[anImageView release];
	
	loginButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	loginButton.font = [UIFont systemFontOfSize: 17];
	loginButton.frame = CGRectMake(76, 390, 166, 44);
	UIImage* anImage = [UIImage imageNamed: @"button_Login.png"];
	[loginButton setBackgroundImage: anImage forState: UIControlStateNormal];
	anImage = [UIImage imageNamed: @"button_Login_Press.png"];
	[loginButton setBackgroundImage: anImage forState: UIControlStateHighlighted];
	
	[loginButton setTitle:NSLocalizedString(@"pica_str_cmd_login",nil) forState:UIControlStateNormal];
	[loginButton addTarget:self action:@selector(signinButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:loginButton];
}


- (void)viewDidLoad 
{
	[super viewDidLoad];
	
	emailTextField.delegate = self;
	passwordTextField.delegate = self;
	stateTable.dataSource = self;
	stateTable.delegate = self;
	alertReason = ALERTREASON_FOR_NONE;
	
	NSDictionary * accounts = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
	if ([accounts count] == 0)
		return;	
	NSString *fullAccount = [[NSUserDefaults standardUserDefaults] objectForKey:kLastAccountKey];

	if (fullAccount == nil)
		fullAccount = [[accounts allKeys] objectAtIndex: 0];

	BOOL autoSingin = YES;
	BOOL aSavePwd = NO;
	
	if ([fullAccount length] > 0){
		NSRange range = [fullAccount rangeOfString:@"@"];
		if (range.location != NSNotFound){
			NSString *theAccount = [fullAccount substringToIndex:range.location];
			NSString *theDomain = [fullAccount substringFromIndex:range.location];
#ifndef FOR_PAYMENT_APP
			NSSet *theSet = [NSSet setWithObjects:@"@hotmail.com",@"@msn.com",@"@live.com",@"@live.hk",nil];
#else
			NSSet *theSet = [NSSet setWithObjects:@"@hotmail.com",@"@msn.com",@"@live.com",nil];
#endif
			if ([theSet containsObject:theDomain])
			{
				emailTextField.text = theAccount;
				[self selectedDomain:theDomain];
			}else {
				emailTextField.text = fullAccount;
				[self selectedDomain:NSLocalizedString(@"qtn_msn_account_other",nil)];
			}
		}
		else {
			emailTextField.text = fullAccount;
		}
		
		if ([accounts count] > 0){
			NSDictionary *account = [accounts objectForKey:fullAccount];
			if (account != nil){
				passwordTextField.text = [account objectForKey:kPasswordKey];
				autoSingin = [[account objectForKey:kAutoLoginKey] boolValue];
				aSavePwd = [[account objectForKey: kSavePwdFlagKey] boolValue];
			}
		}
	}
	
	[saveButton setOn: aSavePwd];
	[autoLoadButton setOn: autoSingin];
	autoLoadButton.enabled = aSavePwd;
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	[stateTable reloadData];
	[domainsButton setTitle:NSLocalizedString(self.domain,nil) forState:UIControlStateNormal];
}

- (void)dealloc 
{
	[status release];
	[domain release];
	[super dealloc];
}

- (BOOL)validPassword
{
#if 1
	if ([passwordTextField.text length] == 0)
		return NO;
	
	BOOL ret = YES;
	NSRange wordRange = NSMakeRange(33, 93);
	NSMutableCharacterSet *separatorSet = [NSMutableCharacterSet characterSetWithRange:wordRange];
	NSCharacterSet *tokenSet = [separatorSet invertedSet];
	if ([passwordTextField.text rangeOfCharacterFromSet:tokenSet].location != NSNotFound)
		ret = NO;
	
	return ret;
#else
	return ([passwordTextField.text length] > 0 && [self isValidPwdString: passwordTextField.text]);
#endif
}

- (IBAction)signinButtonPressed:(id)sender
{
	UIAlertView *anAlert = nil;
	NSString *message = nil;
	
	
	if ([emailTextField.text length] == 0)
		return;
	
	if ([self.domain compare:NSLocalizedString(@"qtn_msn_account_other",nil)] == NSOrderedSame)
	{
		if (!isEmail(emailTextField.text)){
			message = NSLocalizedString(@"qtn_msn_account_error", nil);
			anAlert = [[UIAlertView alloc] initWithTitle: @"" 
												 message: message 
												delegate: nil 
									   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)  
									   otherButtonTitles: nil];
			[anAlert show];
			[anAlert release];
			return;
		}
	}
	
	if (![self validPassword])
	{
		message = NSLocalizedString(@"qtn_msn_password_error", nil);
		anAlert = [[UIAlertView alloc] initWithTitle: @"" 
											 message: message 
											delegate: nil 
								   cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)  
								   otherButtonTitles: nil];
		[anAlert show];
		[anAlert release];
	}
	else
		[[NSNotificationCenter defaultCenter] postNotificationName:LoginActionNotification object:self];
}

- (IBAction)domainButtonPressed:(id)sender
{
	{
		LoginSelectViewController * controller = [[LoginSelectViewController alloc] initWithStyle:UITableViewStyleGrouped];
		controller.loginController = self;
		controller.type = DomainView;
		[self.navigationController pushViewController:controller animated:YES];
		[controller release];
	}
}

- (IBAction)saveButtonPressed:(id)sender
{	
	UISwitch *theSwitch = (UISwitch *)sender;
	if (theSwitch.on == YES)
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:NSLocalizedString(@"msg_safe_prompt",nil)
													   delegate:self
											  cancelButtonTitle:nil
											  otherButtonTitles:NSLocalizedString(@"qtn_yes",nil),nil];
		alertReason = ALERTREASON_FOR_SAVE_PASSWORD;
		[alert show];
		[alert release];
	}
	else {
		if ([self autoLoad]) [autoLoadButton setOn: NO animated: YES];
	}
}

- (IBAction)autoLoadButtonPressed:(id)sender
{
	UISwitch *theSwitch = (UISwitch *)sender;
	if (theSwitch.on == YES){
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:NSLocalizedString(@"qtn_msn_auto_login_confirm",nil)
													   delegate:self
											  cancelButtonTitle:nil
											  otherButtonTitles:NSLocalizedString(@"qtn_yes",nil),nil];
		alertReason = ALERTREASON_FOR_AUTO_LOGIN;
		[alert show];
		[alert release];
	}
}


- (BOOL)savePassword
{
	return saveButton.on;
}

- (BOOL)autoLoad
{
	return autoLoadButton.on;
}


- (NSString*)email
{
	return emailTextField.text;
}

- (NSString*)password
{
	return passwordTextField.text;
}

- (void)selectedAccount:(NSString*)theAccount Domain:(NSString *)theDomain
{
#ifndef FOR_PAYMENT_APP
	NSSet *theSet = [NSSet setWithObjects:@"@hotmail.com",@"@msn.com",@"@live.com",@"@live.hk",nil];
#else
	NSSet *theSet = [NSSet setWithObjects:@"@hotmail.com",@"@msn.com",@"@live.com",nil];
#endif
	
	if ([theSet containsObject:theDomain] == YES)
	{
		emailTextField.text = theAccount;
		self.domain = theDomain;
	}else {
		emailTextField.text = [theAccount stringByAppendingString:theDomain];
		self.domain = NSLocalizedString(@"qtn_msn_account_other",nil);
	}
	
	
	NSDictionary * accounts = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
	if (accounts != nil)
	{
		passwordTextField.text = [[accounts objectForKey:[NSString stringWithFormat:@"%@%@",theAccount,theDomain]] objectForKey:kPasswordKey];
		if (passwordTextField.text != nil)
		{
			saveButton.enabled = YES;
			[saveButton setOn:YES];
			autoLoadButton.enabled = YES;
			BOOL autoSingin = [[[accounts objectForKey:theAccount] objectForKey:kAutoLoginKey] boolValue];
			if (autoSingin)
				[saveButton setOn:YES];
			[autoLoadButton setOn:autoSingin];
		}	
		else
		{
			saveButton.enabled = NO;
			[saveButton setOn:NO];
			autoLoadButton.enabled = NO;
			[autoLoadButton setOn:NO];
		}
	}
	else
	{
		saveButton.enabled = NO;
		[saveButton setOn:NO];
		autoLoadButton.enabled = NO;
		[autoLoadButton setOn:NO];
	}
}

- (void)selectedStatus:(NSString*)theStatus;
{
	self.status = theStatus;
	[[MSNAppDelegate globalAppDelegate].infoDict setObject:getStatus(theStatus) forKey:kLoginStatusKey];
	[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
	[stateTable reloadData];
}

- (void)selectedDomain:(NSString*)theDomain
{
	self.domain = theDomain;
	[[NSUserDefaults standardUserDefaults] setObject:theDomain forKey:@"loginDomain"];
	[stateTable reloadData];
}



- (void)changeToOtherDomain
{
	[self selectedDomain:NSLocalizedString(@"qtn_msn_account_other",nil)];
}

#pragma mark -
#pragma mark alert view delegate methods

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (alertReason == ALERTREASON_FOR_SAVE_PASSWORD){
		alertReason = ALERTREASON_FOR_NONE;
		if ([alertView numberOfButtons] == 2){
			[saveButton setOn: (buttonIndex == 1) animated:YES];
			autoLoadButton.enabled = (buttonIndex == 1);
		}
		else {
			[saveButton setOn: YES animated: YES];
			autoLoadButton.enabled = YES;
		}
	}
	else if (alertReason == ALERTREASON_FOR_AUTO_LOGIN){
		if ([self autoLoad]){
			if (buttonIndex == 0){
				if (![self savePassword]){
					UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: @""
																	  message:NSLocalizedString(@"msg_safe_prompt",nil)
																	 delegate: self
															cancelButtonTitle: NSLocalizedString(@"qtn_yes",nil)
															otherButtonTitles: nil];
					alertReason = ALERTREASON_FOR_SAVE_PASSWORD;
					[anAlert show];
					[anAlert release];
				}
			}
			else {
				[autoLoadButton setOn: NO animated:NO];
			}
		}
	}
	else {
	}
}

#pragma mark -
#pragma mark textField delegate methods

- (BOOL)textFieldShouldClear:(UITextField *)textField
{
	if (textField == emailTextField){
		loginButton.enabled = NO;
	}
	return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	BOOL ret = YES;
	int length = [textField.text length] - range.length + [string length];

	if (textField == emailTextField){
		passwordTextField.text = @"";
	}
	
	if ([textField.text length] >= 113)
	{
		if ([string length] == 0)
		{
			return YES;
		}else {
			return NO;
		}
	}
	
	if (textField == emailTextField)
	{
		if ([string compare:@"@"] == NSOrderedSame)
		{
			[self changeToOtherDomain];
			if (textField == emailTextField && [emailTextField.text length] > 0){
			NSRange range = [string rangeOfString: @"@"];
			if (range.location != NSNotFound){
				NSString *theDomain = [string substringFromIndex: range.location];
				theDomain = [theDomain lowercaseString];
#ifndef FOR_PAYMENT_APP
				NSSet *theSet = [NSSet setWithObjects:@"@hotmail.com",@"@msn.com",@"@live.com",@"@live.hk",nil];
#else
				NSSet *theSet = [NSSet setWithObjects:@"@hotmail.com",@"@msn.com",@"@live.com",nil];
#endif
				if ([theSet containsObject: theDomain]){
					[self selectedDomain:theDomain];
				}else {
					[self selectedDomain:NSLocalizedString(@"qtn_msn_account_other",nil)];
				}
				[domainsButton setTitle:NSLocalizedString(self.domain,nil) forState:UIControlStateNormal];
			}
			}
		}
	}
	
	if (textField == passwordTextField)
	{
		if ([emailTextField.text length] > 0)
		{
			if (length > 0)
			{
				saveButton.enabled = YES;
				autoLoadButton.enabled = YES;
			}
			else
			{
				saveButton.enabled = NO;
				[saveButton setOn:NO animated:NO];
				autoLoadButton.enabled = NO;
				[autoLoadButton setOn:NO animated:NO];
			}
		}
		if (length > 16)
			ret = NO;
	}	
	else
	{
		if (!isValidEmailCharacter(string))
			ret = NO;
		if (length > 113)
			ret = NO;
	}
	return ret;
}

// called when 'return' key pressed. return NO to ignore.
- (BOOL)textFieldShouldReturn:(UITextField *)textField{
	[textField resignFirstResponder];
	return (textField == emailTextField) || (textField == passwordTextField);
}

- (void)textFieldDidEndEditing:(UITextField *)textField{
	if (textField == emailTextField){
		loginButton.enabled = ([emailTextField.text length] > 0);
	}
}

#pragma mark -
#pragma mark tableView delegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	[tableView deselectRowAtIndexPath: indexPath animated: NO];
	
	LoginSelectViewController * controller = [[LoginSelectViewController alloc] initWithStyle:UITableViewStyleGrouped];
	controller.loginController = self;
	switch (indexPath.row)
	{
		case 0:
			controller.type = StatusView;
			break;
			
		case 2:
			controller.type = DomainView;
			break;
		case 1:
			controller.type = AccountView;
			break;
			
		default:
			break;
	}
	[self.navigationController pushViewController:controller animated:YES];
	[controller release];
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
	return 0;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
	return 44;
}

/////datasource method
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	// Number of sections is the number of region dictionaries
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	int rows = 1;
	NSDictionary * accounts = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
	if ([accounts count] > 0)
	{
		rows = 1;
	}
	return rows;
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return nil;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
	
	NSDictionary * accounts = nil;
	NSString* anId = [NSString stringWithFormat: @"%d", indexPath.row];
	UITableViewCell* aCell = [tableView dequeueReusableCellWithIdentifier: anId];
	
	if (aCell == nil){
		aCell = [[[UITableViewCell alloc] initWithFrame: CGRectZero reuseIdentifier: anId] autorelease];
		aCell.font = [UIFont systemFontOfSize: 16];
		aCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
	}
	switch (indexPath.row){
		case 0:
			if (statusCell == nil){
				statusCell = [[MyUITextAndValueTableCell alloc] initWithFrame: CGRectZero
															  reuseIdentifier: anId];
				[statusCell setTitle: NSLocalizedString(@"qtn_msn_status_type",nil)];
				
				statusCell.titleLabel.font = [UIFont systemFontOfSize: 16];
				statusCell.valueLabel.font = [UIFont systemFontOfSize: 16];
				statusCell.valueLabel.textColor = [UIColor grayColor];
			}
			[statusCell setValue: status];
			aCell = statusCell;
			break;
		case 1:
			accounts = [[NSUserDefaults standardUserDefaults] objectForKey:kAccountsKey];
			if ([accounts count] > 0){
				if (accountCell == nil){
					accountCell = [[MyUITextAndValueTableCell alloc] initWithFrame: CGRectZero
																  reuseIdentifier: anId];
					[accountCell setTitle: NSLocalizedString(@"Previous e-mail address",nil)];
					accountCell.titleLabel.font = [UIFont systemFontOfSize: 16];
					accountCell.valueLabel.font = [UIFont systemFontOfSize: 16];
				}
			}
			aCell = accountCell;
			break;
	}

	return aCell;
}

@end
