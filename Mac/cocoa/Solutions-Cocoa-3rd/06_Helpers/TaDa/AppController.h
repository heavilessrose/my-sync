#import <Cocoa/Cocoa.h>


@interface AppController : NSObject 
{
    IBOutlet NSTableView *toDoTableView;
    IBOutlet NSTextField *newItemField;
    NSMutableArray *items;
}
- (IBAction)createNewItem:(id)sender;
@end
