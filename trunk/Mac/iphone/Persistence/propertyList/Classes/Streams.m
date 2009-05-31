//
//  Streams.m
//  propertyList
//
//  Created by wang luke on 5/26/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "Streams.h"


@implementation Streams

#pragma mark -
#pragma mark 输入流
//- (void)setUpStreamForFile:(NSString *)path {
//	// iStream = [[NSInputStream alloc] initWithData:data;
//    iStream = [[NSInputStream alloc] initWithFileAtPath:path];
//	
//    [iStream setDelegate:self];
//	
//	// 避免阻塞的方式之一run-loop：schedule inputstream object to receive stream events on a run loop.
//	// If streaming is taking place on another thread, be sure to schedule the stream object on that thread’s run loop. You should never attempt to access a scheduled stream from a thread different than the one owning the stream’s run loop.
//    [iStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
//	
//    [iStream open];
///* 
// After a stream object is sent open, you can find out about its status, whether it has bytes available to read, and the nature of any error with the following messages:
//	streamStatus
//	hasBytesAvailable
//	streamError
// The returned status is an NSStreamStatus constant indicating that the stream is opening, reading, at the end of the stream, and so on. The returned error is an NSError object encapsulating information about any error that took place. 
// */
//}
//
//
//#pragma mark -
//#pragma mark 输出流
//// Creating and initializing an NSOutputStream object for memory
//- (void)createOutputStream {
//    NSLog(@"Creating and opening NSOutputStream...");
//	//oStream = [[NSOutputStream alloc] initToFileAtPath:<#(NSString *)path#> append:<#(BOOL)shouldAppend#>
//    oStream = [[NSOutputStream alloc] initToMemory];
//    [oStream setDelegate:self];
//    [oStream scheduleInRunLoop:[NSRunLoop currentRunLoop]
//					   forMode:NSDefaultRunLoopMode];
//    [oStream open];
//}
//
//#pragma mark -
//#pragma mark 流事件处理
//- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode 
//{
//    switch(eventCode) {
//		// Although a stream object that has reported an error can be queried for state before it is closed, it cannot be reused for read or write operations.
//		case NSStreamEventErrorOccurred:{
//			NSError *theError = [stream streamError];
//            NSAlert *theAlert = [[NSAlert alloc] init]; // modal delegate releases
//            [theAlert setMessageText:@"Error reading stream!"];
//            [theAlert setInformativeText:[NSString stringWithFormat:@"Error %i: %@",
//										  [theError code], [theError localizedDescription]]];
//            [theAlert addButtonWithTitle:@"OK"];
//            [theAlert beginSheetModalForWindow:[NSApp mainWindow]
//								 modalDelegate:self
//								didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:)
//								   contextInfo:nil];
//            [stream close];
//            [stream release];
//			break;
//		}
//		case NSStreamEventOpenCompleted:{
//			
//			break;
//		}
//			//  reaches the end of a stream
//		case NSStreamEventEndEncountered:{
//			// 取得写到内存的数据
//			NSData *newData = [oStream propertyForKey:NSStreamDataWrittenToMemoryStreamKey];
//            if (!newData) {
//                NSLog(@"No data written to memory!");
//            } else {
//                [self processData:newData];
//            }
//			/////////////////
//			[stream close];
//            [stream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
//            [stream release];
//            stream = nil; // stream is ivar, so reinit it
//			break;
//		}
//		//=======================inputStream
//		// bytes-available event
//        case NSStreamEventHasBytesAvailable:{
//            if(!_data) {
//                _data = [[NSMutableData data] retain];
//            }
//            uint8_t buf[1024];
//            unsigned int len = 0;
//            len = [(NSInputStream *)stream read:buf maxLength:1024];
//            if(len) {
//                [_data appendBytes:(const void *)buf length:len];
//                // bytesRead is an instance variable of type NSNumber.
//                [bytesRead setIntValue:[bytesRead intValue]+len];
//            } else {
//                NSLog(@"no buffer!");
//            }
//            break;
//        }
//		// =======================outputStream
//		// space-available event
//		// If the delegate receives an NSStreamEventHasSpaceAvailable event and does not write anything to the stream, it does not receive further space-available events from the run loop until the NSOutputStream object receives more bytes. When this happens, the run loop is restarted for space-available events. If this scenario is likely in your implementation, you can have the delegate set a flag when it doesn’t write to the stream upon receiving an NSStreamEventHasSpaceAvailable event. Later, when your program has more bytes to write, it can check this flag and, if set, write to the output-stream instance directly.
//		case NSStreamEventHasSpaceAvailable:{
//			uint8_t *readBytes = (uint8_t *)[_data mutableBytes];
//            readBytes += byteIndex; // instance variable to move pointer
//            int data_len = [_data length];
//            unsigned int len = ((data_len - byteIndex >= 1024) ?
//								1024 : (data_len-byteIndex));
//            uint8_t buf[len];
//            (void)memcpy(buf, readBytes, len);
//            len = [stream write:(const uint8_t *)buf maxLength:len];
//            byteIndex += len;
//            break;
//			
//			// 发送一个http GET request 
////			if (stream == oStream) {
////                NSString * str = [NSString stringWithFormat:@"GET / HTTP/1.0\r\n\r\n"];
////                const uint8_t * rawstring = (const uint8_t *)[str UTF8String];
////                [oStream write:rawstring maxLength:strlen(rawstring)];
////                [oStream close];
////            }
////			break;
//		}
//	}
//}
//
//#pragma mark -
//#pragma mark Polling方式处理事件
//// Writing to an output stream using polling
//- (void)createNewFile {
//    oStream = [[NSOutputStream alloc] initToMemory];
//    [oStream open];
//    uint8_t *readBytes = (uint8_t *)[data mutableBytes];
//    uint8_t buf[1024];
//    int len = 1024;
//	
//    while (1) {
//        if (len == 0) break;
//        if ([oStream hasSpaceAvailable]) {
//			(void)strncpy(buf, readBytes, len);
//			readBytes += len;
//			if ([oStream write:(const uint8_t *)buf maxLength:len] == -1) {
//				[self handleError:[oStream streamError]];
//				break;
//			}
//			[bytesWritten setIntValue:[bytesWritten intValue]+len];
//			len = (([data length] - [bytesWritten intValue] >= 1024) ? 1024 :
//				   [data length] - [bytesWritten intValue]);
//        }
//    }
//    NSData *newData = [oStream propertyForKey:NSStreamDataWrittenToMemoryStreamKey];
//    if (!newData) {
//        NSLog(@"No data written to memory!");
//    } else {
//        [self processData:newData];
//    }
//    [oStream close];
//    [oStream release];
//    oStream = nil;
//}
//
//#pragma mark -
//#pragma mark socket streams 适用于cocoa？
//// Setting up a network socket stream
//// Because the stream objects you receive back from getStreamsToHost:port:inputStream:outputStream: are autoreleased, be sure to retain them right away. If the socket connection fails, then one or both of the requested NSInputStream and NSOutputStream objects are nil. 
//- (IBAction)searchForSite:(id)sender
//{
//    NSString *urlStr = [sender stringValue];
//    if (![urlStr isEqualToString:@""]) {
//        [searchField setEnabled:NO];
//        NSURL *website = [NSURL URLWithString:urlStr];
//        if (!website) {
//            NSLog(@"%@ is not a valid URL");
//            return;
//        }
//        NSHost *host = [NSHost hostWithName:[website host]];
//        // iStream and oStream are instance variables
//        [NSStream getStreamsToHost:host port:80 inputStream:&iStream outputStream:&oStream];
//        [iStream retain];
//        [oStream retain];
//        [iStream setDelegate:self];
//        [oStream setDelegate:self];
//        [iStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
//        [oStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
//        [iStream open];
//        [oStream open];
//    }
//}
//
//// Making an HTTP GET request
//- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
//    NSLog(@"stream:handleEvent: is invoked...");
//	
//    switch(eventCode) {
//        case NSStreamEventHasSpaceAvailable:
//        {
//
//            break;
//        }
//			// continued ...
//    }
//}
@end
