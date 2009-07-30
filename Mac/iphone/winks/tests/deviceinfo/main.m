//
//  main.m
//  deviceinfo
//
//  Created by wang luke on 7/24/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <UIKit/UIDevice.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
	
	
	NSString *_name = [[UIDevice currentDevice] name];
	NSLog(@"name = %@", _name);
	NSString *model = [[UIDevice currentDevice] model];
	NSLog(@"model = %@", model);
	NSString *localizedModel = [[UIDevice currentDevice] localizedModel];
	NSLog(@"localizedModel = %@", localizedModel);
	NSString *systemName = [[UIDevice currentDevice] systemName];
	NSLog(@"systemName = %@", systemName);
	NSString *systemVersion = [[UIDevice currentDevice] systemVersion];
	NSLog(@"systemVersion = %@", systemVersion);
	NSString *UDID = [[UIDevice currentDevice] uniqueIdentifier];
	NSLog(@"UDID = %@", UDID);
	
    [pool release];
	return retVal;
    //return 0;
}
