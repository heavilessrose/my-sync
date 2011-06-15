//
//  SLMovDetailController.m
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLMovDetailController.h"

@implementation SLMovDetailController

@synthesize table, mov;

- (void)dealloc
{
    [mov release];
    [table release];
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
    // Do any additional setup after loading the view from its nib.
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
//    return (interfaceOrientation == UIInterfaceOrientationPortrait);
    return (UIInterfaceOrientationIsLandscape(interfaceOrientation) || interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - table view delegate

- (CGFloat)contentHeight
{
    CGSize csize = [mov.content sizeWithFont:[UIFont systemFontOfSize:17.0f] constrainedToSize:CGSizeMake(280.0f, 1000.0f) lineBreakMode:UILineBreakModeWordWrap];
    return csize.height;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CGFloat rowHight = .0f;
    if (indexPath.row == 0) {
        rowHight = 80.0f;
    }
    if (indexPath.row == 1) {
        rowHight = [self contentHeight]+50.0f;
    }
    return rowHight;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
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
        static NSString *hotCellID = @"SLHotCell";
        SLHotCell *hotCell = (SLHotCell *)[tableView dequeueReusableCellWithIdentifier:hotCellID];
        if (!theCell) {
            [[NSBundle mainBundle] loadNibNamed:@"SLHotCell" owner:self options:nil];
            if (tmpHotCell) {
                hotCell = tmpHotCell;
                hotCell.playDelegate = self;
                hotCell.playButton.hidden = NO;
            }
        }
        hotCell.movie = mov;
        theCell = hotCell;
    }
    if (indexPath.row == 1) {
        static NSString *infoCellID = @"SLMovInfoCell";
        SLMovInfoCell *infoCell = (SLMovInfoCell *)[tableView dequeueReusableCellWithIdentifier:infoCellID];
        if (!theCell) {
            [[NSBundle mainBundle] loadNibNamed:@"SLMovInfoCell" owner:self options:nil];
            if (tmpMovInfoCell) {
                infoCell = tmpMovInfoCell;
                CGRect cframe = infoCell.contentTextView.frame;
                cframe.size.height = [self contentHeight];
                [infoCell.contentTextView setFrame:cframe];
            }
        }
        infoCell.movie = mov;
        theCell = infoCell;
    }
    return theCell;
}


#pragma mark - play 

- (void)play:(SLMovie *)theMov
{
    DLOG
	
    NSURL *movieURL = [NSURL URLWithString:@"http://127.0.0.1/~luke/html5/video/res/Movie.m4v"]; //self.mov.url;
    if (movieURL)
    {
        if ([movieURL scheme])	// sanity check on the URL
        {
            [self initAndPlayMovie:movieURL];
        }
    }
}

@end
