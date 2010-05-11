//
//  ReceivedFileObject.m
//  MSN
//
//  Created by  apple on 08-12-1.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ReceivedFileObject.h"
#import "ChatViewController.h"
#import "ChatViewData.h"
#import "ClientNetWorkController.h"
#import "HttpClient.h"
#import "NSData+base64.h"
#import "define.h"
#import "MSNAppDelegate.h"

#include <stdio.h>


@implementation ReceivedFileObject
@synthesize transferid, ftid,fileName, fileSize, fileUrl, acceptButton, rejectButton, playButton, chatViewData, status, cancelButton, progressBar, myAlertView;

- (void)buildTranserId{
	if ([ftid length] == 0){
		NSTimeInterval aSeconds = [[NSDate date] timeIntervalSince1970];
		NSString* aFid = [NSString stringWithFormat: @"%.0f_%@", aSeconds, chatViewData.sid];
		self.ftid = aFid;
	}
}

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		status = RECEIVE_ASK;
		cancelButton = nil;
		progressBar = nil;
		fileUrl = nil;
		isVoiceClip = NO;
		sysDismiss = YES;
		file = NULL;
		receivedFileSize = 0;
		NSArray *filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
		documentPath = [filePaths objectAtIndex: 0];
		[documentPath retain];
		self.acceptButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[acceptButton setTitle:NSLocalizedString(@"qtn_accept",nil) forState:UIControlStateNormal];
		[acceptButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchDown];
		
		self.rejectButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[rejectButton setTitle:NSLocalizedString(@"qtn_reject",nil) forState:UIControlStateNormal];
		[rejectButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchDown];
		
        seqids = [[NSMutableArray alloc] init];
	}
	return self;
}

- (void) dealloc
{
	if (file != NULL){
		fclose(file);	
    }
	file = NULL;
	NSString *filePath = [documentPath stringByAppendingPathComponent:fileName];
	if ([[NSFileManager defaultManager] fileExistsAtPath:filePath]){
		[[NSFileManager defaultManager] removeItemAtPath: [NSTemporaryDirectory() stringByAppendingPathComponent: fileName]
												   error: nil];
		[[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
	}
	if (myAlertView != nil)
	{
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}
	[fileName release];
	[fileSize release];
	[transferid release];
	[fileUrl release];
	[ftid release];
	[documentPath release];
	[progressBar release];
	[cancelButton release];
	[playButton release];
	[acceptButton release];
	[rejectButton release];
	httpClient.delegate = nil;
	[httpClient release];
    
    [seqids release];
	[super dealloc];
}

- (BOOL)isVoiceClip
{
	return isVoiceClip;
}

- (void)setIsVoiceClip:(BOOL)isVoice
{
	isVoiceClip = isVoice;
	progressBar = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
	[progressBar setProgress:0.0f];
	
	self.playButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[playButton setTitle:NSLocalizedString(@"qtn_vc_play",nil) forState:UIControlStateNormal];
	[playButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchDown];
	playButton.hidden = YES;
}


- (void)receivedData:(NSDictionary*)theInfo
{
	//int length = [[theInfo objectForKey:@"filesize"] intValue];
	//	int end = [[theInfo objectForKey:@"fileend"] intValue];	
	//int curent = [[theInfo objectForKey:@"filestart"] intValue];
	NSString *dataString = [theInfo  objectForKey:@"data"];
	if (dataString != nil)
	{
        NSString* aSeqid = [theInfo objectForKey: @"seqid"];
        if (![seqids containsObject: aSeqid]){
            [seqids addObject: aSeqid];
            NSData *data = [NSData dataWithBase64EncodedString:dataString];
            if (isVoiceClip)
                receivedFileSize += [dataString length];
            else
                receivedFileSize += [data length];
            [self writeData:data];
        }
        [[ClientNetWorkController getInstance] sendDataReceiveAck: transferid
                                                              sid: chatViewData.sid
                                                            seqid: aSeqid
                                                             ftid: ftid];
	}
}

- (void)writeData:(NSData*)theData
{
	if (theData != nil)
	{
		NSString *filePath = [documentPath stringByAppendingPathComponent:fileName];
		if (file == NULL && ![[NSFileManager defaultManager] fileExistsAtPath: filePath])
			file = fopen([filePath UTF8String], "wb+");
		if (file != NULL)
		{
			fseek(file, 0, SEEK_END);
            if (receivedFileSize <= [fileSize intValue]){
                fwrite([theData bytes], [theData length], 1, file);
            }
            else {
                if (receivedFileSize <= ([fileSize intValue] + [theData length])){
                    NSInteger aLen = receivedFileSize - [fileSize intValue];
                    fwrite([theData bytes], ([theData length] - aLen), 1, file);
                }
            }
		}
	}
	
	if ([fileSize intValue] > 0)
	{
		if (receivedFileSize/[fileSize floatValue] > progressBar.progress)
		{
			progressBar.progress = receivedFileSize/[fileSize floatValue];	
		}	
	}
	
	if (receivedFileSize >= [fileSize intValue])
	{
		[self receiveFinished];
	}
}

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
{
	BOOL saved = NO;
	if (error == nil)
		saved = YES;
	
	[chatViewData receiveFinished:self saved:saved];
}

- (void)receiveFinished
{
//    NSLog(@"seqids : %@", seqids);
    [seqids removeAllObjects];
	BOOL saved = NO;
	if (file != NULL)
	{
//        NSLog(@"(receiveFinished)file size is : %d", ftell(file));
		fclose(file);	
		file = NULL;
	}
	progressBar.hidden = YES;
	cancelButton.hidden = YES;
	NSString *filePath = [documentPath stringByAppendingPathComponent:fileName];
	if (isVoiceClip)
	{
		playButton.hidden = NO;
	}
	else
	{
		NSData *data = [NSData dataWithContentsOfFile:filePath];
		if (data != nil)
		{
			UIImage * image = [UIImage  imageWithData:data];
			if (image != nil)
			{
				saved = YES;
				UIImageWriteToSavedPhotosAlbum(image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);	
			}
		}
		if ([[NSFileManager defaultManager] fileExistsAtPath:filePath])
			[[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
	}
	
	if (isVoiceClip || saved){
		NSString *aLink = [NSTemporaryDirectory() stringByAppendingPathComponent: fileName];
		[[NSFileManager defaultManager] linkItemAtPath: filePath toPath: aLink error: nil];
	}
	
	if (!saved)
		[chatViewData receiveFinished:self saved:saved];
}

- (void)receivedUrl:(NSString*)theURL
{
	if (theURL != nil)
	{
		self.fileUrl = theURL;
		if (httpClient == nil)
		{
			httpClient = [[HttpClient alloc] init];
			httpClient.callbackFun = @selector(callBackData:dictionary:);
			httpClient.delegate = self;	
		}
		
		
		NSMutableDictionary *headers = [NSMutableDictionary dictionaryWithCapacity:0];
		[headers setObject:@"*/*" forKey:@"Accept"];
		
		NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"login"];
		if (dict != nil)
		{	
			NSString *userName = [dict valueForKey:@"id"];
			if (userName != nil)
			{
				NSRange range = [userName rangeOfString:@"@"];
				if (range.location != NSNotFound)
				{
					userName = [userName substringToIndex:range.location];
					NSString *password = [dict valueForKey:kPasswordKey];	
					if (password != nil)
					{
						[headers setObject:[NSString stringWithFormat:@"%@;%@",userName,password] forKey:@"Pica-Auth"];
					}
				}
			}
		}
		[headers setObject:NSLocalizedString(APPLICATION_VERSION,nil) forKey:@"Pica-Clientver"];
		if ([fileSize intValue] > 0)
		{
			if (receivedFileSize + 128*1024-1 > [fileSize intValue])
				[headers setObject:[NSString stringWithFormat:@"bytes=%d-%d",receivedFileSize,[fileSize intValue]] forKey:@"Pica-X-Range"];
			else
				[headers setObject:[NSString stringWithFormat:@"bytes=%d-%d",receivedFileSize,receivedFileSize + 128*1024-1] forKey:@"Pica-X-Range"];	
		}
		
		NSURL * urlPath = [[NSURL alloc] initWithString:theURL];
		[httpClient addHttpRequest:urlPath
							method:@"GET"
						   headers:headers
						bodyString:nil];	
		[urlPath release];
	}
}

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	if (theData != nil && [theData length] > 0 )
	{
		receivedFileSize += [theData length];
		[self writeData:theData];
		if (receivedFileSize < [fileSize intValue])
		{
			[self receivedUrl:fileUrl];
		}
	}
}

- (void)receivedAction:(NSString*)theAction
{
	if ([theAction compare:@"cancel"] == NSOrderedSame)
	{
		NSString *filePath = [documentPath stringByAppendingPathComponent:fileName];
		if ([[NSFileManager defaultManager] fileExistsAtPath:filePath])
			[[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
		progressBar.hidden = YES;
		cancelButton.hidden = YES;
		acceptButton.enabled = NO;
		[acceptButton setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
		rejectButton.enabled = NO;
		[rejectButton setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
		status = RECEIVE_CANCELED;
	}
	else if ([theAction compare:@"complete_ack"] == NSOrderedSame)
	{
	}
}

- (void)notifyTransferBegin: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	if ([note object] != self)
	{
		[acceptButton setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
	}
}

- (void)notifyTransferEnd: (NSNotification*)note
{
	if (status == RECEIVE_ASK && [note object] != self)
	{
		[acceptButton setTitleColor:[UIColor colorWithRed:0.3 green:0.313 blue:0.474 alpha:1.0f] forState:UIControlStateNormal];
		rejectButton.enabled = YES;	
	}
}

- (void)buttonClicked:(id)sender
{
	if (cancelButton != nil && cancelButton == sender)
	{
		[self buildTranserId];
		[[ClientNetWorkController getInstance] sendCancelTransferRequest:transferid sid:chatViewData.sid ftid:ftid];
		NSString *filePath = [documentPath stringByAppendingPathComponent:fileName];
		if ([[NSFileManager defaultManager] fileExistsAtPath:filePath])
			[[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
		status = RECEIVE_CANCELED;
		progressBar.hidden = YES;
		cancelButton.hidden = YES;
		[chatViewData receiveCanceled:self];
		[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:self];
	}
	else if (playButton != nil && playButton == sender)
	{
		[chatViewData.controller showPlayView:self];
	}
	else 
	{
		if (isSendReceive && acceptButton == sender)
		{
			NSString *message = @"";
			if (isSendReceive == SEND)
				message = NSLocalizedString(@"qtn_wait_sending_data",nil);
			else if (isSendReceive == RECEIVE)
				message = NSLocalizedString(@"qtn_wait_receiving_data",nil);
			
			self.myAlertView = [[[UIAlertView alloc] initWithTitle:@""
																   message:message 
																  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil) 
														 otherButtonTitles:nil] autorelease];
			[myAlertView setDelegate:self];
			sysDismiss = YES;
			[myAlertView show];
		}
		else if ([chatViewData.persons count] > 1)
		{
			NSString *message = NSLocalizedString(@"qtn_transfer_unallowed_file",nil);
			
			self.myAlertView = [[[UIAlertView alloc] initWithTitle:@""
														   message:message 
														  delegate:self cancelButtonTitle:NSLocalizedString(@"qtn_ok",nil) 
												 otherButtonTitles:nil] autorelease];
			[myAlertView setDelegate:self];
			sysDismiss = YES;
			[myAlertView show];
		}
		else
		{
			BOOL accept = NO;
			status = RECEIVE_ACTIONED;
			acceptButton.enabled = NO;
			[acceptButton setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
			rejectButton.enabled = NO;
			[rejectButton setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
			if (acceptButton == sender)
			{
				accept = YES;
				[[NSNotificationCenter defaultCenter] postNotificationName:TransferBeginNotification object:self];
				progressBar = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
				[progressBar setProgress:0.0f];
				self.cancelButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
				[cancelButton setTitle:NSLocalizedString(@"qtn_cancel",nil) forState:UIControlStateNormal];
				[cancelButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchDown];	
			}
			[chatViewData userAction:accept from:self];
			[self buildTranserId];
			[[ClientNetWorkController getInstance] sendTransferInviteResponse:transferid sid:chatViewData.sid 
																		 ftid:ftid accept:accept];
		}

	}
}


- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!sysDismiss)
	{
		alertView.delegate = nil;
		self.myAlertView = nil;
	}
}


@end
