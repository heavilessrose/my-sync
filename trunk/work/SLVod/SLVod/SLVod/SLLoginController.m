//
//  SLLoginController.m
//  SLVod
//
//  Created by luke on 11-8-14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLLoginController.h"
#import "LKLabelTextFieldCell.h"
#import "LKButtonCell.h"
#import "FFSettings.h"

@implementation SLLoginController

@synthesize table, username, pass;

- (void)dealloc
{
    self.table = nil;
    [username release];
    [pass release];
    [super dealloc];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its 
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    self.table = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - network 

- (void)login
{
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:SL_LOGIN, self.username, self.pass] relativeToURL:SL_BASE_HOST];
    ASIHTTPRequest *loginReq = [ASIHTTPRequest requestWithURL:url];
    [loginReq setDelegate:self];
    [loginReq startAsynchronous];
}

- (void)requestFinished:(ASIHTTPRequest *)request
{
    [[FFSettings shareSettings] archiveSettings];
}

- (void)requestFailed:(ASIHTTPRequest *)request
{
    DLOG
}


#pragma mark - table 

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *LKLabelTextFieldCellID = @"LKLabelTextFieldCell";
    static NSString *LKButtonCellID = @"LKButtonCell";
    UITableViewCell *aCell = nil;
    if (indexPath.row == 0 || indexPath.row == 1) {
        LKLabelTextfieldCell *tCell = (LKLabelTextfieldCell *)[tableView dequeueReusableCellWithIdentifier:LKLabelTextFieldCellID];
        
        if (!tCell) {
            tCell = [[LKLabelTextfieldCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:LKLabelTextFieldCellID];
            tCell.selectionStyle = UITableViewCellSelectionStyleNone;
        }
        if (indexPath.row == 0) {
            [tCell.label setText:@"用户名:"];
            [tCell.field setPlaceholder:@"请输入用户名"];
            [tCell.field setText:self.username];
        }
        if (indexPath.row == 1) {
            [tCell.label setText:@"密码:"];
            [tCell.field setPlaceholder:@"请输入密码"];
            [tCell.field setText:self.pass];
        }
        
        aCell = tCell;
    }
    
    if (indexPath.row == 2) {
        LKButtonCell *bCell = (LKButtonCell *)[tableView dequeueReusableCellWithIdentifier:LKButtonCellID];
        if (!bCell) {
            bCell = [[LKButtonCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:LKButtonCellID];
        }
        bCell.textLabel.text = @"登  录";
        aCell = bCell;
    }
    return aCell;
}

@end
