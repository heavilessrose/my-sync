//
//  NetStatusAppDelegate.m
//  NetStatus
//
//  Created by luke on 09-11-10.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "NetStatusAppDelegate.h"

#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>

#include <netdb.h>
#include <arpa/inet.h>

@implementation NetStatusAppDelegate

@synthesize window;
@synthesize textView;
///
@synthesize testObj;
@synthesize testObj1;
@synthesize testObj2;
@synthesize testObj3;
@synthesize testObj4;
@synthesize tmpObj;

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	self.textView = [[UITextView alloc] 
					 initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	
	struct sockaddr_in zeroAddr;
	bzero(&zeroAddr, sizeof(zeroAddr));
	zeroAddr.sin_len = sizeof(zeroAddr);
	zeroAddr.sin_family = AF_INET;
	
	SCNetworkReachabilityRef target = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (struct sockaddr *) &zeroAddr);
	
	SCNetworkReachabilityFlags flags;
	SCNetworkReachabilityGetFlags(target, &flags);
	
	NSString *sNetworkReachable;
	if (flags & kSCNetworkFlagsReachable)
		sNetworkReachable = @"YES";
	else
		sNetworkReachable = @"NO";
	
	NSString *sCellNetwork;
	if (flags & kSCNetworkReachabilityFlagsIsWWAN)
		sCellNetwork = @"YES";
	else
		sCellNetwork = @"NO";
	
	NSString *s;
	struct hostent *remoteHostEnt = gethostbyname("ip.winksi.com");
	
	if (remoteHostEnt != NULL) {
		struct in_addr *remoteInAddr = (struct in_addr *) remoteHostEnt->h_addr_list[0];
		
		char *sRemoteInAddr = inet_ntoa(*remoteInAddr);
		
		s = [[NSString alloc] initWithFormat:
			 @"Network Reachable: %@\n"
			 @"Cell Network: %@\n"
			 @"Remote IP: %s\n",
			 sNetworkReachable,
			 sCellNetwork,
			 sRemoteInAddr];
	}else {
		s = [[NSString alloc] initWithFormat:
			 @"Network Reachable: %@\n"
			 @"Cell Network: %@\n",
			 sNetworkReachable,
			 sCellNetwork];
	}

	self.textView.text = s;
	[sCellNetwork release];
	[sNetworkReachable release];
	[s release];
	
	self.textView.editable = NO;
	[window addSubview:self.textView];
	[self.textView release];
	
	////////
	[self retainTest];
	
	////////
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark retain test
- (void)retainTest
{
	self.tmpObj = [[NSObject alloc] init];
	[tmpObj release];
	// retain属性 无self
	NSLog(@"testObj count = %u, tmpObj count = %u", [testObj retainCount], [tmpObj retainCount]);
	testObj = [tmpObj retain];
	NSLog(@"testObj count = %u, tmpObj count = %u", [testObj retainCount], [tmpObj retainCount]);
	self.testObj = nil;
	NSLog(@"testObj count = %u, tmpObj count = %u", [testObj retainCount], [tmpObj retainCount]);
	[testObj release];
	NSLog(@"testObj count = %u, tmpObj count = %u", [testObj retainCount], [tmpObj retainCount]);
	[tmpObj release];
}


@end
