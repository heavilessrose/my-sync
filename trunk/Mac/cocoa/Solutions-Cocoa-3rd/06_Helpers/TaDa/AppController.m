//
//  AppController.m
//  TaDa
//
//  Created by Aaron Hillegass on 12/10/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"


@implementation AppController

- (id)init
{
    [super init];
    items = [[NSMutableArray alloc] init];
    return self;
}

- (IBAction)createNewItem:(id)sender
{
    NSString *newItem = [newItemField stringValue];
    [items addObject:newItem];
    [newItemField setStringValue:@""];
    [toDoTableView reloadData];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return [items count];
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    return [items objectAtIndex:row];
}

//- (void)tableView:(NSTableView *)tableView setObjectValue:(id)newValue forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
//{
//    [items replaceObjectAtIndex:row
//                     withObject:newValue];
//}

@end
