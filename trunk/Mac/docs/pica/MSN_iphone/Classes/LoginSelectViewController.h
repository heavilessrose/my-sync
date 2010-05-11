//
//  LoginSelectViewController.h
//  MSN
//
//  Created by  apple on 08-10-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum EM_ViewType
{
	AccountView = 0,
	DomainView,
	StatusView,
}ViewType;
	

@class LoginViewController;

@interface LoginSelectViewController : UITableViewController
{
	ViewType type;
	LoginViewController *loginController;
	NSObject <UITableViewDataSource> *datasource;
}
- (void)notifyDeleteAccount: (NSNotification*)note;

- (void)setStatus: (NSString*)aStatus;
- (NSString*)status;

@property (nonatomic,assign) ViewType type;
@property (nonatomic,assign) LoginViewController *loginController;
@end
