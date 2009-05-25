//
//  PeopleView.h
//  RaiseMan
//
//  Created by Aaron Hillegass on 10/23/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface PeopleView : NSView {
	NSArray *people;
	NSMutableDictionary *attributes;
	float lineHeight;
	NSRect pageRect;
	int linesPerPage;
	int currentPage;
}

- (id)initWithPeople:(NSArray *)persons;


@end
