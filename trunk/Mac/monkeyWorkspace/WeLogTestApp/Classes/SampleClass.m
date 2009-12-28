//
//  SampleClass.m
//  WeLogLib
//
//  Created by luke on 09-12-28.
//  Copyright 2009 WeTeam. All rights reserved.
//

#import "SampleClass.h"
#import "welog.h"

@implementation SampleClass

+ (void)debugExample;
{
	ERRLOG(@"A sample ERR message: %@", @"nserr");
	DEBUGLOG(@"A sample DEBUG message");
	INFOLOG(@"A sample INFO message");
	
	//int a = 1;
	//HEXLOG(&a, 4, "a:");
}

- (void)debugExample;
{
	ERRLOG(@"Another sample ERR message");
	DEBUGLOG(@"Another sample DEBUG message");
	INFOLOG(@"Another sample INFO message");
}
@end
