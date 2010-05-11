//
//  NetWorkReachability.h.h
//  MSN
//
//  Created by  apple on 08-11-25.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <SystemConfiguration/SystemConfiguration.h>

typedef enum {
	NotReachable = 0,
	ReachableViaCarrierDataNetwork,
	ReachableViaWiFiNetwork
} NetworkStatus;

@interface NetWorkReachability : NSObject 
{
	NetworkStatus networkStatus;	
	SCNetworkReachabilityRef reachability;
	SCNetworkReachabilityFlags addressReachabilityFlags;
}

- (NetworkStatus)updateReachabilityChanges;
- (void)beginListeningForReachabilityChanges;
- (void)stopListeningForReachabilityChanges;

static void ReachabilityCallback(SCNetworkReachabilityRef target, SCNetworkReachabilityFlags flags, void *info);
@end
