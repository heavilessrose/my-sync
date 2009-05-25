//
//  MyDocument.m
//  RaiseMan
//
//  Created by Aaron Hillegass on 9/24/07.
//  Copyright __MyCompanyName__ 2007 . All rights reserved.
//

#import "MyDocument.h"
#import "Person.h"
#import "PreferenceController.h"
#import "PeopleView.h"

@interface MyDocument ()

- (void)startObservingPerson:(Person *)p;
- (void)stopObservingPerson:(Person *)p;

@end

@implementation MyDocument

- (id)init
{
    if (![super init])
		return nil;
	
	[self setEmployees:[NSMutableArray array]];
	
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc addObserver:self
		   selector:@selector(handleColorChange:)
			   name:BNRColorChangedNotification
			 object:nil];
	NSLog(@"registered for notification");
	
    return self;
}

- (void)dealloc
{
	[self setEmployees:nil];
	[super dealloc];
}

#pragma mark Action methods

- (IBAction)createEmployee:(id)sender
{
	NSWindow *w = [tableView window];
	BOOL editingEnded = [w makeFirstResponder:w];
	if (!editingEnded) {
		return;
	}
	NSUndoManager *undo = [self undoManager];
	if ([undo groupingLevel]) {
		[undo endUndoGrouping];
		[undo beginUndoGrouping];
	}	
	
	Person *p = [employeeController newObject];
	[employeeController addObject:p];
	
	// In case the user has sorted the content array
	[employeeController rearrangeObjects];
	
	// Find the row of the new object
	NSArray *a = [employeeController arrangedObjects];
	int row = [a indexOfObjectIdenticalTo:p];
	
	// Start editing in the first column
	[tableView editColumn:0
					  row:row
				withEvent:nil
				   select:YES];
}

- (void)alertEnded:(NSAlert *)alert
			  code:(int)code
	   contextInfo:(void *)v
{
	if (code == NSAlertDefaultReturn) {
		[employeeController remove:nil];
	}
}

- (IBAction)removeEmployee:(id)sender
{
	NSArray *selectedPeople = [employeeController selectedObjects];
	
	NSAlert *alert = [NSAlert alertWithMessageText:NSLocalizedString(@"DELETE", @"Delete")
									 defaultButton:NSLocalizedString(@"DELETE", @"Delete")
								   alternateButton:NSLocalizedString(@"CANCEL", @"Cancel")
									   otherButton:nil
						 informativeTextWithFormat:NSLocalizedString(@"SURE_DELETE", @"Do you really want to delete %d people") ,
					  [selectedPeople count]];
	NSLog(@"started alert %@", alert);
	[alert beginSheetModalForWindow:[tableView window]
					  modalDelegate:self
					 didEndSelector:@selector(alertEnded:code:contextInfo:)
						contextInfo:nil];
}


#pragma mark Accessors - 'employees'
- (void)startObservingPerson:(Person *)person
{
	[person addObserver:self
			 forKeyPath:@"personName"
				options:NSKeyValueObservingOptionOld
				context:NULL];
	
	[person addObserver:self
			 forKeyPath:@"expectedRaise"
				options:NSKeyValueObservingOptionOld
				context:NULL];
}

- (void)stopObservingPerson:(Person *)person
{
	[person removeObserver:self
				forKeyPath:@"personName"];
	
	[person removeObserver:self
				forKeyPath:@"expectedRaise"];
}


- (void)insertObject:(Person *)p
  inEmployeesAtIndex:(int)r
{
	// Register the undo
	NSUndoManager *undo = [self undoManager];
	[[undo prepareWithInvocationTarget:self]
				removeObjectFromEmployeesAtIndex:r];
	if (![undo isUndoing]) {
		[undo setActionName:@"Insert Person"];
	}
	// Add the person to the array
	[self startObservingPerson:p];
	[employees insertObject:p atIndex:r];
}
- (void)removeObjectFromEmployeesAtIndex:(int)r
{
	Person *p = [employees objectAtIndex:r];
	
	NSUndoManager *undo = [self undoManager];
	[[undo prepareWithInvocationTarget:self]
	 insertObject:p inEmployeesAtIndex:r];
	if (![undo isUndoing]) {
		[undo setActionName:@"Delete Person"];
	}
	[self stopObservingPerson:p];
	[employees removeObjectAtIndex:r];
}

- (void)setEmployees:(NSMutableArray *)array
{
	if (array == employees) {
		return;
	}
	for (Person *p in employees) {
		[self stopObservingPerson:p];
	}
	[employees release];
	[array retain];
	employees = array;
	for (Person *p in employees) {
		[self startObservingPerson:p];
	}
}

- (void)changeKeyPath:(NSString *)keyPath
			 ofObject:(id)obj
			  toValue:(id)newValue
{
	[obj setValue:newValue forKeyPath:keyPath];
}

- (void)observeValueForKeyPath:(NSString *)keyPath
					  ofObject:(id)obj
						change:(NSDictionary *)change
					   context:(void *)context
{
	NSUndoManager *undo = [self undoManager];
	id oldValue = [change objectForKey:NSKeyValueChangeOldKey];
	if (oldValue == [NSNull null]) {
		oldValue = nil;
	}
	NSLog(@"oldValue = %@", oldValue);
	[[undo prepareWithInvocationTarget:self] 
	 changeKeyPath:keyPath
	 ofObject:obj
	 toValue:oldValue];
	[undo setActionName:@"Edit"];
}

#pragma mark NSDocument methods

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"MyDocument";
}

- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    [super windowControllerDidLoadNib:aController];
	
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSData *colorAsData = [defaults objectForKey:BNRTableBgColorKey];
	NSColor *color = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData];
	[tableView setBackgroundColor:color];
	
}

- (void)presentError:(NSError *)error modalForWindow:(NSWindow *)window delegate:(id)delegate didPresentSelector:(SEL)didPresentSelector contextInfo:(void *)contextInfo
{
	NSDictionary *ui = [error userInfo];
	NSError *underlying = [ui objectForKey:NSUnderlyingErrorKey];
	
	NSLog(@"error = %@, userInfo = %@, underlying info = %@", error, ui, [underlying userInfo]);
	[super presentError:error 
		 modalForWindow:window 
			   delegate:delegate 
	 didPresentSelector:didPresentSelector 
			contextInfo:contextInfo];
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
	[[tableView window] endEditingFor:nil];
	
	NSData *d = [NSKeyedArchiver archivedDataWithRootObject:employees];
	
	return d;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
	NSMutableArray *array = nil;
	@try {
		array = [NSKeyedUnarchiver unarchiveObjectWithData:data];
	}
	@catch (id obj) {
		if (outError) {
			NSDictionary *d = [NSDictionary dictionaryWithObject:@"This file is corrupted like Noriega."
														  forKey:NSLocalizedFailureReasonErrorKey];
			*outError = [NSError errorWithDomain:NSOSStatusErrorDomain 
											code:unimpErr 
										userInfo:d];
		}
		return NO;
	}
	
	[self setEmployees:array];

    return YES;
}

- (NSPrintOperation *)printOperationWithSettings:(NSDictionary *)ps 
                                           error:(NSError **)e; 
{ 
    PeopleView *view = [[PeopleView alloc] initWithPeople:employees]; 
    NSPrintInfo *printInfo = [self printInfo]; 
    NSPrintOperation *printOp = [NSPrintOperation printOperationWithView:view 
                                     printInfo:printInfo]; 
    [view release]; 
    return printOp; 
} 

#pragma Handling Notifications

- (void)handleColorChange:(NSNotification *)note
{
	NSLog(@"notification arrived:%@", note);
	NSColor *newColor = [[note userInfo] objectForKey:@"color"];
	[tableView setBackgroundColor:newColor];
}

@end
