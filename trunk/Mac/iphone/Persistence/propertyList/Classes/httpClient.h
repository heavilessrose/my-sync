//
//  httpClient.h
//  propertyList
//
//  Created by wang luke on 5/20/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sys/types.h>
#import <sys/socket.h>
#import <errno.h>
#import <unistd.h>
#import <netinet/in.h>
#import <limits.h>
#import <netdb.h>
#import <arpa/inet.h>
#import <ctype.h>

@protocol httpClientDelegate;

@interface httpClient : NSObject {
	id<httpClientDelegate> delegate;
}
@property (nonatomic, assign) id delegate;

- (id) initWithDelegate:(id<httpClientDelegate>)theDelegate;
- (void) setRequest;
- (void) connectAndDown;
- (void) getResponseHeader;

- (int) download:(NSString *)url;
@end

@protocol httpClientDelegate<NSObject>
- (void) didFinishDownload:(NSString *)filepath;
@end
