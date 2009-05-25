//
//  FirstLetter.m
//  TypingTutor
//
//  Created by Aaron Hillegass on 10/22/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "FirstLetter.h"


@implementation NSString (FirstLetter)

- (NSString *)BNR_firstLetter
{
	if ([self length] < 2) {
		return self;
	}
	NSRange r;
	r.location = 0;
	r.length = 1;
	return [self substringWithRange:r];
}

@end
