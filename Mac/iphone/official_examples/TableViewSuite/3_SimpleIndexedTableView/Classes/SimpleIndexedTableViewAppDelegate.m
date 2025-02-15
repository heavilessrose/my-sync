/*

File: SimpleIndexedTableViewAppDelegate.m
Abstract: Application delegate that configures the view controller.

Version: 1.8

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "SimpleIndexedTableViewAppDelegate.h"
#import "RootViewController.h"


@interface SimpleIndexedTableViewAppDelegate ()
@property (nonatomic, copy, readwrite) NSArray *list;
@end


@implementation SimpleIndexedTableViewAppDelegate

@synthesize window;
@synthesize navigationController;
@synthesize list;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
	self.list = [NSTimeZone knownTimeZoneNames];
	
	// Create the navigation and view controllers
	RootViewController *rootViewController = [[RootViewController alloc] initWithStyle:UITableViewStylePlain];
	UINavigationController *aNavigationController = [[UINavigationController alloc] initWithRootViewController:rootViewController];
	self.navigationController = aNavigationController;
	[aNavigationController release];
	[rootViewController release];
	
	// Configure and display the window
	[window addSubview:[navigationController view]];
	[window makeKeyAndVisible];
}


- (void)setList:(NSArray *)newList {
	if (list != newList) {
		[list release];
		list = [newList mutableCopy];
	}
}

- (NSUInteger)countOfList {
    return [list count];
}

- (id)objectInListAtIndex:(NSUInteger)theIndex {
    return [list objectAtIndex:theIndex];
}

- (void)getList:(id *)objsPtr range:(NSRange)range {
    [list getObjects:objsPtr range:range];
}

- (void)insertObject:(id)obj inListAtIndex:(NSUInteger)theIndex {
    [list insertObject:obj atIndex:theIndex];
}

- (void)removeObjectFromListAtIndex:(NSUInteger)theIndex {
    [list removeObjectAtIndex:theIndex];
}

- (void)replaceObjectInListAtIndex:(NSUInteger)theIndex withObject:(id)obj {
    [list replaceObjectAtIndex:theIndex withObject:obj];
}


- (void)dealloc {
	[navigationController release];
    [window release];
	[list release];
    [super dealloc];
}


@end
