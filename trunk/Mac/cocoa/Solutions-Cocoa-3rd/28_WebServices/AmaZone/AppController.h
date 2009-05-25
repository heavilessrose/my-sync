#import <Cocoa/Cocoa.h>

// Developed by Aaron Hillegass of Big Nerd Ranch, Inc.
// Use this code anyway you want, but neither Aaron nor BNR is
// responsible for anything you do with it or anything
// bad that results from its use.

@interface AppController : NSObject
{
    IBOutlet NSProgressIndicator *progress;
    IBOutlet NSTextField *searchField;
    IBOutlet NSTableView *tableView;
	NSXMLDocument *doc;
	NSArray *itemNodes;

}
- (void)fetchBooks:(id)sender;
@end
