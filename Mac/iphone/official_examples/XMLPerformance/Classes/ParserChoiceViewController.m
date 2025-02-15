/*
     File: ParserChoiceViewController.m
 Abstract: Provides an interface for choosing and running one of the two available parsers.
  Version: 1.0
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
*/

#import "ParserChoiceViewController.h"
#import "SongsViewController.h"
#import "LibXMLParser.h"
#import "CocoaXMLParser.h"

// Class extension for private properties and methods.
@interface ParserChoiceViewController ()

@property (nonatomic, retain) SongsViewController *songsViewController;
@property (nonatomic, retain) NSIndexPath *parserSelection;
@property (nonatomic, retain) UIButton *startButton;

@end

@implementation ParserChoiceViewController

@synthesize songsViewController, parserSelection, startButton;

- (void)dealloc {
    [songsViewController release];
    [parserSelection release];
    [startButton release];
	[super dealloc];
}

- (void)viewDidLoad {
    // Set the background for the main view to match the table view.
    self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
    // Set an initial selection.
    self.parserSelection = [NSIndexPath indexPathForRow:0 inSection:0];
}

- (SongsViewController *)songsViewController {
    if (songsViewController == nil) {
        self.songsViewController = [[[SongsViewController alloc] initWithNibName:@"SongsView" bundle:nil] autorelease];
    }
    return songsViewController;
}

- (IBAction)startParser {
    SongsViewController *controller = self.songsViewController;
    [self.navigationController pushViewController:controller animated:YES];
    [controller parseWithParserType:self.parserSelection.row];
}

- (NSUInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSUInteger)section {
    return 2;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *kCellIdentifier = @"MyCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:kCellIdentifier] autorelease];
    }
    cell.text = (indexPath.row == 0) ? [CocoaXMLParser parserName] : [LibXMLParser parserName];
    return cell;
}

- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath {
    return ([indexPath isEqual:parserSelection]) ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView cellForRowAtIndexPath:self.parserSelection].accessoryType = UITableViewCellAccessoryNone;
    self.parserSelection = indexPath;
    [tableView cellForRowAtIndexPath:indexPath].accessoryType = UITableViewCellAccessoryCheckmark;
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
