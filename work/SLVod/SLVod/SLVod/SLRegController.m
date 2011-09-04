//
//  SLRegController.m
//  SLVod
//
//  Created by luke on 11-8-14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLRegController.h"
#import "LKLabelTextFieldCell.h"
#import "LKButtonCell.h"
#import "FFSettings.h"
#import "LKAlert.h"

@implementation SLRegController

@synthesize table, loginReq;
@synthesize username, pass, pass2, email;

- (void)dealloc
{
    self.table = nil;
    [pass2 release];
    [username release];
    [pass release];
    [email release];
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

#pragma mark -

- (NSString *)username
{
    NSIndexPath *indexpath = [NSIndexPath indexPathForRow:0 inSection:0];
    LKLabelTextfieldCell *tCell = (LKLabelTextfieldCell *)[self.table cellForRowAtIndexPath:indexpath];
    if ([tCell isKindOfClass:[LKLabelTextfieldCell class]]) {
        return tCell.field.text;
    }
    return nil;
}

- (NSString *)pass
{
    NSIndexPath *indexpath = [NSIndexPath indexPathForRow:1 inSection:0];
    LKLabelTextfieldCell *tCell = (LKLabelTextfieldCell *)[self.table cellForRowAtIndexPath:indexpath];
    if ([tCell isKindOfClass:[LKLabelTextfieldCell class]]) {
        return tCell.field.text;
    }
    return nil;
}

- (NSString *)pass2
{
    NSIndexPath *indexpath = [NSIndexPath indexPathForRow:2 inSection:0];
    LKLabelTextfieldCell *tCell = (LKLabelTextfieldCell *)[self.table cellForRowAtIndexPath:indexpath];
    if ([tCell isKindOfClass:[LKLabelTextfieldCell class]]) {
        return tCell.field.text;
    }
    return nil;
}

- (NSString *)email
{
    NSIndexPath *indexpath = [NSIndexPath indexPathForRow:3 inSection:0];
    LKLabelTextfieldCell *tCell = (LKLabelTextfieldCell *)[self.table cellForRowAtIndexPath:indexpath];
    if ([tCell isKindOfClass:[LKLabelTextfieldCell class]]) {
        if (!tCell.field.text || [tCell.field.text length] == 0) {
            return nil;
        }
        return tCell.field.text;
    }
    return nil;
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
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

- (BOOL)verify
{
    if (!self.username || [[self.username stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] == 0) {
        alertWithMessageAndTitle(NSLocalizedString(@"Invalid username", nil), NSLocalizedString(@"Whitespace not allowed", nil));
        return NO;
    }
    if (!self.pass || [[self.pass stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] == 0) {
        alertWithMessageAndTitle(NSLocalizedString(@"Invalid pass", nil), NSLocalizedString(@"Whitespace not allowed", nil));
        return NO;
    }
    if (!self.pass2 || [[self.pass2 stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] == 0) {
        alertWithMessageAndTitle(NSLocalizedString(@"Invalid pass", nil), NSLocalizedString(@"Whitespace not allowed", nil));
        return NO;
    }
    if ((self.pass && self.pass2 && ![self.pass isEqualToString:self.pass2])) {
        alertWithMessageAndTitle(NSLocalizedString(@"Pass not equal", nil), nil);
        return NO;
    }
    if (!self.email || [[self.email stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] == 0) {
        alertWithMessageAndTitle(NSLocalizedString(@"Invalid email", nil), NSLocalizedString(@"Whitespace not allowed", nil));
        return NO;
    }
    return YES;
}

- (void)reg
{
    if ([self verify]) {
        
        [self HUDWithGradient:NSLocalizedString(@"Reging", nil)];
        NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:SL_REG, self.username, self.pass, self.email, [[UIDevice currentDevice] uniqueIdentifier]] relativeToURL:SL_BASE_HOST];
        ASIHTTPRequest *regReq = [ASIHTTPRequest requestWithURL:url];
        [regReq setDelegate:self];
        [regReq startAsynchronous];
        
    }
}

- (void)loginAuto
{
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:SL_LOGIN, self.username, self.pass] relativeToURL:SL_BASE_HOST];
    loginReq = [ASIHTTPRequest requestWithURL:url];
    [loginReq setDelegate:self];
    [loginReq startAsynchronous];
}

- (void)requestFinished:(ASIHTTPRequest *)request
{
    if (HUD) {
        [HUD hide:YES];
    }
    if (request == loginReq) {
        [FFSettings shareSettings].logined = YES;
        [[FFSettings shareSettings] archiveSettings];
        [self.navigationController popViewControllerAnimated:YES];
    } else {
        if (request.responseStatusCode == 200) {
            [FFSettings shareSettings].username = self.username;
            [FFSettings shareSettings].pass = self.pass;
            [FFSettings shareSettings].email = self.email;
            [[FFSettings shareSettings] archiveSettings];
            
            [self loginAuto];
        } else {
            alertWithMessageAndTitle(NSLocalizedString(@"User already existed", nil), nil);
            return;
        }
    }
}

- (void)requestFailed:(ASIHTTPRequest *)request
{
    DLOG
    if (HUD) {
        [HUD hide:YES];
    }
    alertWithMessageAndTitle(NSLocalizedString(@"Reg err", nil), nil);
}


#pragma mark - table 

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 4) {
        [self reg];
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 5;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *LKLabelTextFieldCellID = @"LKLabelTextFieldCell";
    static NSString *LKButtonCellID = @"LKButtonCell";
    UITableViewCell *aCell = nil;
    if (indexPath.row == 0 || indexPath.row == 1 || indexPath.row == 2 || indexPath.row == 3) {
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
        if (indexPath.row == 2) {
            [tCell.label setText:@"重复密码:"];
            [tCell.field setPlaceholder:@"请重复密码"];
            [tCell.field setText:self.pass2];
        }
        if (indexPath.row == 3) {
            [tCell.label setText:@"邮箱:"];
            [tCell.field setPlaceholder:@"请输入邮箱"];
            [tCell.field setText:self.email];
        }
        
        aCell = tCell;
    }
    
    if (indexPath.row == 4) {
        LKButtonCell *bCell = (LKButtonCell *)[tableView dequeueReusableCellWithIdentifier:LKButtonCellID];
        if (!bCell) {
            bCell = [[LKButtonCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:LKButtonCellID];
        }
        bCell.textLabel.text = @"注  册";
        aCell = bCell;
    }
    return aCell;
}

@end
