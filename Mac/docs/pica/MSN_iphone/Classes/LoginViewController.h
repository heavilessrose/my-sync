//
//  LoginViewController.h
//  msn_for_mac
//
//  Created by  apple on 08-9-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MyUITextAndValueTableCell;

@interface LoginViewController : UIViewController<UITextFieldDelegate,UIAlertViewDelegate,UITableViewDelegate,UITableViewDataSource>
{
	UITextField *emailTextField;
	UITextField *passwordTextField;
	UITableView *stateTable;
	
	UIButton *loginButton;
	UIButton *domainsButton;
	UISwitch *saveButton;
	UISwitch *autoLoadButton;
	
	NSString *status;
	NSString *domain;
	
	NSInteger alertReason;
	
	MyUITextAndValueTableCell*		statusCell;
	MyUITextAndValueTableCell*		accountCell;
}

- (IBAction)signinButtonPressed:(id)sender;
- (IBAction)saveButtonPressed:(id)sender;
- (IBAction)autoLoadButtonPressed:(id)sender;
- (BOOL)savePassword;
- (BOOL)autoLoad;
- (NSString*)email;
- (NSString*)password;

- (void)selectedAccount:(NSString*)theAccount Domain:(NSString *)theDomain;
- (void)selectedStatus:(NSString*)theStatus;
- (void)selectedDomain:(NSString*)theDomain;

@property (nonatomic, retain) UITextField *emailTextField;

@property (nonatomic, retain) NSString *status;
@property (nonatomic, retain) NSString *domain;
@end
