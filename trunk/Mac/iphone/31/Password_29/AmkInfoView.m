//
//  AmkInfoView.m
//  Password
//
//  Created by apple on 8/31/08.
//  Copyright 2008 ACS Technologies. All rights reserved.
//

#import "AmkInfoView.h"


@implementation AmkInfoView
- (id)initWithMessage:(NSString *)message delegate:(id)delegate {
	if(self = [[UIAlertView alloc] initWithTitle:@"Enter your E-Mail address" message:@"" delegate:delegate cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil]) {
	   self.frame = CGRectMake(0.0, 20.0, 200.0, 450.0);
	}
}
@end
