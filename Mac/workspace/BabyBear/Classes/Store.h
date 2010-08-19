//
//  Store.h
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Store : NSObject {
	
	NSString	*sid;
	NSString	*sname;
	NSString	*saddr;
	NSString	*sreserves;
	NSString	*slongitude;
	NSString	*slatitude;
}

@property (nonatomic, retain) NSString	*sid;
@property (nonatomic, retain) NSString	*sname;
@property (nonatomic, retain) NSString	*saddr;
@property (nonatomic, retain) NSString	*sreserves;
@property (nonatomic, retain) NSString	*slongitude;
@property (nonatomic, retain) NSString	*slatitude;
@end
