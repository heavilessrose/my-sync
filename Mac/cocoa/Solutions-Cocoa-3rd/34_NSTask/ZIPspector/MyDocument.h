//
//  MyDocument.h
//  ZIPspector
//
//  Created by Aaron Hillegass on 11/5/07.
//  Copyright Big Nerd Ranch 2007 . All rights reserved.
//


#import <Cocoa/Cocoa.h>

@interface MyDocument : NSDocument
{
    IBOutlet NSTableView *tableView;
    NSArray *filenames;
}
@end
