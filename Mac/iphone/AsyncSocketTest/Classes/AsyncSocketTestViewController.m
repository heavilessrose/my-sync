//
//  AsyncSocketTestViewController.m
//  AsyncSocketTest
//
//  Created by wang luke on 6/18/09.
//  Copyright luke 2009. All rights reserved.
//

#import "AsyncSocketTestViewController.h"

@implementation AsyncSocketTestViewController

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark -
- (id)init
{
	if(self = [super init])
	{
		asyncSocket = [[AsyncSocket alloc] initWithDelegate:self];
	}
	return self;
}

- (void)viewDidLoad{
	// 主线程持续不断画图 。。
	// ...
	
	// 联网
	NSLog(@"Ready");
	
	NSError *err = nil;
	if(![asyncSocket connectToHost:@"online.sccnn.com" onPort:80 error:&err])
	{
		NSLog(@"connectToHost Error: %@", err);
	}
}

+ (NSString *)appDocumentsDir
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	//documentsDirectory = [[NSString alloc] initWithCString:getenv("HOME")];
	//return [documentsDirectory autorelease];
	return documentsDirectory;
}
#pragma mark delegate
//- (void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err{
//	NSLog(@"willDisconnectWithError: %@");
//}
//
//- (void)onSocketDidDisconnect:(AsyncSocket *)sock{
//	NSLog(@"onSocketDidDisconnect");
//}
//
//- (void)onSocket:(AsyncSocket *)sock didAcceptNewSocket:(AsyncSocket *)newSocket{
//	NSLog(@"didAcceptNewSocket");
//}
//
//- (NSRunLoop *)onSocket:(AsyncSocket *)sock wantsRunLoopForNewSocket:(AsyncSocket *)newSocket{
//	NSLog(@"wantsRunLoopForNewSocket");
//}
//
//- (BOOL)onSocketWillConnect:(AsyncSocket *)sock{
//	NSLog(@"onSocketWillConnect");
//}
//
//- (void)onSocket:(AsyncSocket *)sock didReadPartialDataOfLength:(CFIndex)partialLength tag:(long)tag{
//	NSLog(@"didReadPartialDataOfLength");
//}
//
//- (NSTimeInterval)onSocket:(AsyncSocket *)sock
//  shouldTimeoutReadWithTag:(long)tag
//				   elapsed:(NSTimeInterval)elapsed
//				 bytesDone:(CFIndex)length{
//	NSLog(@"shouldTimeoutReadWithTag");
//}
//
//- (NSTimeInterval)onSocket:(AsyncSocket *)sock
// shouldTimeoutWriteWithTag:(long)tag
//				   elapsed:(NSTimeInterval)elapsed
//				 bytesDone:(CFIndex)length{
//	NSLog(@"shouldTimeoutWriteWithTag");
//}
// 准备开始联网
- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port{
	NSLog(@"didConnectToHost");
	long tag;
	
	char request[1024];
	sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nHost: %s:%d\r\nConnection: Close\r\n\r\n", "gif8/021/gif0405_001.gif", "online.sccnn.com", 80);
	NSString *nsRequest = [[NSString alloc] initWithCString:request];
	const char *utfString = [nsRequest UTF8String];
	NSData *requestData = [NSData dataWithBytes:utfString length:strlen(utfString)];
	[sock writeData:requestData withTimeout:-1 tag:tag];
}

// 读取数据完成
- (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{
	NSLog(@">>didReadData");
	NSString *doc = [AsyncSocketTestViewController appDocumentsDir];
	[data writeToFile:[doc stringByAppendingPathComponent:@"gif0405_001.gif"] atomically:YES];
}

// 发送数据完成
- (void)onSocket:(AsyncSocket *)sock didWriteDataWithTag:(long)tag{
	NSLog(@">>didWriteDataWithTag");
	NSData *response = [NSData data];
	//- (void)readDataToData:(NSData *)data withTimeout:(NSTimeInterval)timeout maxLength:(CFIndex)length tag:(long)tag;
	[sock readDataToData:response withTimeout:-1 tag:tag];
}
@end
