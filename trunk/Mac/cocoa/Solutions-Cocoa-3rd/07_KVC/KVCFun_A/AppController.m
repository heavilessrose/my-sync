//
//  AppController.m
//  KVCFun
//
//  Created by Aaron Hillegass on 9/12/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"


@implementation AppController

- (id)init
{
	[super init];
	
	// Set fido to 5
	[self setValue:[NSNumber numberWithInt:5]
			forKey:@"fido"];
	
	// Read fido
	NSNumber *n = [self valueForKey:@"fido"];
	NSLog(@"fido = %@", n);
	return self;
}

- (IBAction)incrementFido:(id)sender
{
	[self willChangeValueForKey:@"fido"];
	[self setFido:[self fido] + 1];
	NSLog(@"fido is now %d", fido);
	[self didChangeValueForKey:@"fido"];
}

- (int)fido
{
	NSLog(@"-fido is returning %d", fido);
	return fido;
}

- (void)setFido:(int)x
{
	NSLog(@"-setFido: is called with %d", x);
	fido = x;
}

@end
