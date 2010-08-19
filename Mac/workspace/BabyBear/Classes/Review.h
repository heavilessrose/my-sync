//
//  Review.h
//  BabyBear
//
//  Created by luke on 10-8-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Review : NSObject {
	
	NSString	*rname;
	NSString	*rrating;
	NSString	*rdate;
	NSString	*rtitle;
	NSString	*rcatagory;
	NSString	*rcomment;
}

@property (nonatomic, retain) NSString	*rname;
@property (nonatomic, retain) NSString	*rrating;
@property (nonatomic, retain) NSString	*rdate;
@property (nonatomic, retain) NSString	*rtitle;
@property (nonatomic, retain) NSString	*rcatagory;
@property (nonatomic, retain) NSString	*rcomment;

@end
