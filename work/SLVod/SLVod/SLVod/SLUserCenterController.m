//
//  SLUserCenterController.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLUserCenterController.h"
#import "SLLoginController.h"
#import "SLRegController.h"

@implementation SLUserCenterController

@synthesize table;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)dealloc
{
    MLog(@"");
    [table release];
    [super dealloc];
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
    self.theTable = table;
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
//    return (UIInterfaceOrientationIsLandscape(interfaceOrientation) || interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - table view delegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0) {
        return 86.0f;
    }
    return 44.0f;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{   
//    SLMovDetailController *detailVC = [[SLMovDetailController alloc] initWithNibName:@"SLMovDetailController" bundle:nil];
//    detailVC.mov = [movies objectAtIndex:indexPath.row];
//    [self.navigationController pushViewController:detailVC animated:YES];
//    [detailVC release];
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
{
    DLOG
}

- (BOOL)tableView:(UITableView *)tableView shouldIndentWhileEditingRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}

#pragma mark table data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 2;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *theCell = nil;
    if (indexPath.row == 0) {
        static NSString *PCellID = @"SLUserProfileCell";
        SLUserProfileCell *pCell = (SLUserProfileCell *)[tableView dequeueReusableCellWithIdentifier:PCellID];
        if (!pCell) {
            [[NSBundle mainBundle] loadNibNamed:@"SLUserProfileCell" owner:self options:nil];
            if (tmpUProfileCell) {
                pCell = tmpUProfileCell;
                pCell.selectionStyle = UITableViewCellSelectionStyleNone;
                pCell.avatarView.image = [UIImage imageNamed:@"noimg.png"];
                pCell.signDelegate = self;
                pCell.accessoryType = UITableViewCellAccessoryNone;
                self.tmpUProfileCell = nil;
            }
        }
        theCell = pCell;
        //theCell.user = [movies objectAtIndex:indexPath.row];
    } else {
        static NSString *usettingCell = @"SLUSettingCell";
        theCell = [tableView dequeueReusableCellWithIdentifier:usettingCell];
        if (!theCell) {
            theCell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:usettingCell] autorelease];
        }
        /*
        if (indexPath.row == 1) {
            theCell.imageView.image = [UIImage imageNamed:@"vip.png"];
            theCell.textLabel.text = @"加入VIP";
        }
        if (indexPath.row == 2) {
            theCell.imageView.image = [UIImage imageNamed:@"setting.png"];
            theCell.textLabel.text = @"个人设置";
        }
         */
        if (indexPath.row == 1) {
            theCell.imageView.image = [UIImage imageNamed:@"about.png"];
            theCell.textLabel.text = @"关于";
        }
        theCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
    return theCell;
}

#pragma mark - sign delegate
- (void)signinButtonPressed:(UIButton *)aButton
{
    DLOG
    SLLoginController *l = [[SLLoginController alloc] initWithNibName:@"SLLoginController" bundle:nil];
    [self.navigationController pushViewController:l animated:YES];
    [l release];
}

- (void)signupButtonPressed:(UIButton *)aButton
{
    DLOG
    SLRegController *l = [[SLRegController alloc] initWithNibName:@"SLRegController" bundle:nil];
    [self.navigationController pushViewController:l animated:YES];
    [l release];
}

#pragma mark -
#pragma BCTabbar 

- (NSString *)iconImageName {
	return @"magnifying-glass.png";
}

@end
