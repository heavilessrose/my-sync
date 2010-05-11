//
//  NetWorkReachability.h.m
//  MSN
//
//  Created by  apple on 08-11-25.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "NetWorkReachability.h"
#import "ClientNetWorkController.h"

#import <SystemConfiguration/SCNetworkReachability.h>
#import <sys/socket.h>
#import <netinet/in.h>
#import <netinet6/in6.h>
#import <arpa/inet.h>
#import <ifaddrs.h>
#include <netdb.h>

NSString *NetworkReachabilityChangedNotification = @"NetworkReachabilityChanged";
@implementation NetWorkReachability

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		reachability = nil;
		networkStatus = 0;
		addressReachabilityFlags = 0;
		[self beginListeningForReachabilityChanges];
	}
	return self;
}

- (void) dealloc
{
	[self stopListeningForReachabilityChanges];
	CFRelease(reachability);
	[super dealloc];
}

static void reachabilityCallback(SCNetworkReachabilityRef target, SCNetworkReachabilityFlags flags, void *info)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    // Post a notification to notify the client that the network reachability changed.
    [ClientNetWorkController postNotification:NetworkReachabilityChangedNotification info:nil];
	
	[pool drain];
}

- (void)beginListeningForReachabilityChanges
{
	struct sockaddr_in sin;
	
	bzero(&sin, sizeof(sin));
	sin.sin_len = sizeof(sin);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(IN_LINKLOCALNETNUM);
	
	if (reachability == nil)
	{
		reachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&sin);
		
		SCNetworkReachabilityContext	context = {0, self, NULL, NULL, NULL};
		SCNetworkReachabilitySetCallback(reachability, reachabilityCallback, &context);
		SCNetworkReachabilityScheduleWithRunLoop(reachability, [[NSRunLoop currentRunLoop] getCFRunLoop], kCFRunLoopDefaultMode);
	}
	[self updateReachabilityChanges];
}

- (NetworkStatus)updateReachabilityChanges
{
	BOOL isReachable = NO;
	BOOL gotFlags = SCNetworkReachabilityGetFlags(reachability, &addressReachabilityFlags);
    if (gotFlags) 
	{
        // There was an error getting the reachability flags.
		isReachable = addressReachabilityFlags & kSCNetworkReachabilityFlagsReachable;
		BOOL noConnectionRequired = !(addressReachabilityFlags & kSCNetworkReachabilityFlagsConnectionRequired);
		if ((addressReachabilityFlags & kSCNetworkReachabilityFlagsIsWWAN)) 
		{
			noConnectionRequired = YES;
		}
		isReachable =  (isReachable && noConnectionRequired) ? YES : NO;
    }
    
	if (isReachable && (addressReachabilityFlags & kSCNetworkReachabilityFlagsIsDirect)) 
		networkStatus = ReachableViaWiFiNetwork;
	else
		networkStatus = NotReachable;
	return networkStatus;
}

- (void)stopListeningForReachabilityChanges
{
	SCNetworkReachabilityUnscheduleFromRunLoop(reachability, [[NSRunLoop currentRunLoop] getCFRunLoop], kCFRunLoopDefaultMode);
}
@end
