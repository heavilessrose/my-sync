//
//  MainView.m
//  HomelandSecurity
//
//  Created by apps on 10/18/08.
//  Copyright AppsAmuck.com 2008. All rights reserved.
//

#import "MainView.h"

@implementation MainView

-(void)awakeFromNib {
	
	[self onTimer];
		
	[NSTimer scheduledTimerWithTimeInterval:(300.0) target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
	
}

- (void)onTimer
{
	
	NSString* advisoryCondition = [self getAdvisoryCondition];
	
	Boolean isSevere = [advisoryCondition hasSuffix:@"=\"SEVERE\" /> "];
	Boolean isHigh = [advisoryCondition hasSuffix:@"=\"HIGH\" /> "];
	Boolean isElevated = [advisoryCondition hasSuffix:@"=\"ELEVATED\" /> "];
	Boolean isGuarded = [advisoryCondition hasSuffix:@"=\"GUARDED\" /> "];
	Boolean isLow = [advisoryCondition hasSuffix:@"=\"LOW\" /> "];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1];

	if(isSevere == true)
		severe.alpha = 0.0;
	
	if(isHigh == true)
		high.alpha = 0.0;
	
	if(isElevated == true)
		elevated.alpha = 0.0;
	
	if(isGuarded == true)
		guarded.alpha = 0.0;
	
	if(isLow == true)
		low.alpha = 0.0;
	
	[UIView commitAnimations];

}

- (NSString*)getAdvisoryCondition {
	
	NSError *error;
	NSURLResponse *response;
	NSData *dataReply;
	NSString *stringReply;

	NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL: [NSURL URLWithString: @"http://www.dhs.gov/dhspublic/getAdvisoryCondition"]];
	[request setHTTPMethod: @"GET"];
	dataReply = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
	stringReply = [[NSString alloc] initWithData:dataReply encoding:NSUTF8StringEncoding];
	NSLog(stringReply);

	return stringReply;
}


- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	// Drawing code
}


- (void)dealloc {
	[super dealloc];
}


@end
