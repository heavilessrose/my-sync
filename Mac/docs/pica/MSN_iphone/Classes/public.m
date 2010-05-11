//
//  public.m
//  MSN
//
//  Created by Jun Du on 4/13/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "public.h"
@implementation Public

+(void)alertMessage:(NSString *)title addMsg:(NSString *)msg {
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:msg
												   delegate:self cancelButtonTitle:NSLocalizedString(@"Dismiss",nil) otherButtonTitles: nil];
	[alert show];	
	[alert release];
}

+(BOOL) isENumberAlphabet:(unsigned char )c {
    return ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || (c == '-') || (c == '_') || (c == '.');
}

+(BOOL)isEnglishNumberAlphabet:(NSString *)string
{
	const char *theChar = [string UTF8String];
	long theLength = [string length];
	for (int i=0; i<theLength; i ++)
	{
		if (![self isENumberAlphabet:theChar[i]])
		{
			return NO;
		}
	}
	return YES;
}
@end
