//
//  URLCacheConnection.h
//  propertyList
//
//  Created by wang luke on 5/8/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol URLCacheConnectionDelegate;

@interface URLCacheConnection : NSObject {
	id <URLCacheConnectionDelegate> delegate;
	NSURLConnection *conn; // url连接对象
	NSMutableData *receivedData; // 服务器返回的数据
	NSDate *lastModified; // 最后更新时间
}

@property (nonatomic, retain) NSURLConnection *conn;
@property (nonatomic, assign) id delegate;
@property (nonatomic, retain) NSMutableData *receivedData;
@property (nonatomic, retain) NSDate *lastModified;

- (id) initWithURL:(NSURL *)theURL delegate:(id<URLCacheConnectionDelegate>)theDelegate;
- (void) cancel;
@end


@protocol URLCacheConnectionDelegate<NSObject>

- (void) connectionDidFail:(URLCacheConnection *)theConnection;
- (void) connectionDidFinish:(URLCacheConnection *)theConnection;
@end
