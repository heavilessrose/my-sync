//
//  GprsInitor.h
//  netTest
//
//  Created by wang luke on 09-10-23.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GprsInitor : NSObject {
	NSURLConnection *connection;
}
@property(nonatomic, retain)NSURLConnection *connection;

- (void)gprsInit;
@end
