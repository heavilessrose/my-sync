//
//  Downloader.h
//  propertyList
//
//  Created by wang luke on 5/7/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>


static NSMutableData *receivedData = nil;
@interface Downloader : NSObject {

}

- (NSData *)createConn;

@end
