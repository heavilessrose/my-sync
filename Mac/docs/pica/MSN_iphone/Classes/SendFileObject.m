//
//  SendFileObject.m
//  MSN
//
//  Created by  apple on 08-11-28.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SendFileObject.h"
#import "ClientNetWorkController.h"
#import "ChatViewController.h"
#import "ChatViewData.h"
#import "NSData+base64.h"
#import "HttpClient.h"
#import "PersonInfo.h"
#import "define.h"

NSString *CancelSendingNotification = @"kCancelSendingNotification";

@implementation SendFileObject

@synthesize imageData, fileSize, ftid, chatViewData, transferid, status, etag, sendByServer,fileName, isVoiceClip,fileUrl;
@synthesize playButton, cancelButton, progressBar;
- (id)initWithImageData:(NSData*)theData voiceClip:(BOOL)flag
{
	self = [super init];
	if (self != nil)
	{
		index = 0;
		position = 0;
		status = 0;
		currentSize = 0;
		sendByServer = NO;
		etag = nil;
		fileUrl = nil;
		srandom(time(NULL));
		self.ftid = [NSString stringWithFormat:@"s-%d",random()];
		if (isVoiceClip)
			self.fileName = [NSString stringWithFormat:@"%0X.wav",random()];
		else
			self.fileName = [NSString stringWithFormat:@"%0X.jpg",random()];
		self.imageData = theData;
		fileSize = [imageData length];
		self.cancelButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[cancelButton setTitle:NSLocalizedString(@"qtn_cancel", nil) forState:UIControlStateNormal];
		[cancelButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchDown];	
		progressBar = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
		[progressBar setProgress:0.0f];
		isVoiceClip = flag;
		NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
		NSString *string = nil;
		if (dict != nil)
		{
			string = [dict objectForKey:@"data_method"];
		}
		if (!isVoiceClip)
		{
			if (string != nil)
				sendByServer = [string characterAtIndex:0] == '0';
			playButton = nil;
			[[NSNotificationCenter defaultCenter] postNotificationName:TransferBeginNotification object:self];
		}
		else
		{
			if (string != nil)
				sendByServer = [string characterAtIndex:2] == '0';
			self.playButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
			[playButton setTitle:NSLocalizedString(@"qtn_vc_play",nil) forState:UIControlStateNormal];
			[playButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchDown];
			playButton.hidden = YES;
		}
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(buttonClicked:)
				   name:CancelSendingNotification 
				 object:nil];
	}
	return self;
}


- (void)sendFile
{		
	char buffer[2049] = {0};
	currentSize = 2048;
	if (fileSize < position + 2047)
	{
		currentSize = fileSize - position;
	}
	
	NSRange range = NSMakeRange(position, currentSize);
	[imageData getBytes:buffer range:range];
	
	NSData *base64data = [NSData dataWithBytes:buffer length:currentSize];
	if (sendByServer)
	{
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
		[headers setObject:[NSString stringWithFormat:@"bytes %d-%d/%d",position,position + currentSize - 1,[imageData length]] forKey:@"Upload-Range"];
		if (etag != nil)
		{
			[headers setObject:[NSString stringWithFormat:@"%@",etag] forKey:@"Upload-ETag"];
		}
		dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
		NSURL *serverUrl = nil;
		if (dict != nil)
		{
			NSString *string = [dict objectForKey:@"data"];
			if (string != nil)
			{
				string = [string stringByAppendingFormat:@"/%@",fileName];
				serverUrl = [NSURL URLWithString:string];
			}
		}
		if (serverUrl != nil)
		{
			[httpClient addHttpRequest:serverUrl
								method:@"POST"
							   headers:headers
							bodyString:base64data];
		}
	}
	else
	{
		NSString *base64String = nil;
		
		if (isVoiceClip)
		{
			base64String = [imageData base64Encoding];
			[[ClientNetWorkController getInstance] sendVoiceClip:chatViewData.contact.jid
															 sid:chatViewData.sid
													 totalLength:[imageData length]
													base64Length:[base64String length]
															data:base64String
															 url:nil];
			status = SEND_FINISHED;	
			playButton.hidden = NO;
			progressBar.hidden = YES;
			[chatViewData sendFinished:self];
		}
		else
		{
			base64String = [base64data base64Encoding];
			[[ClientNetWorkController getInstance] sendTransferData:transferid 
																sid:chatViewData.sid 
															   ftid:ftid
															  seqid:[NSString stringWithFormat:@"%d",index] 
														totalLength:fileSize
														   startPos:position 
															 endPos:position + currentSize - 1
													  currentLength:[base64String length]
															   data:base64String];	
		}
	}
}

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	if (theData != nil && [theData length] > 0 && fileUrl == nil)
	{
		NSString *string = [[NSString alloc] initWithData:theData encoding:NSUTF8StringEncoding];
		self.fileUrl = string;
		[string release];
		if (fileUrl != nil && etag == nil)
		{
			self.etag = [fileUrl lastPathComponent];
		}
	}
	[self performSelectorOnMainThread:@selector(sendNextFile) withObject:nil waitUntilDone: NO];
}

- (void)sendNextFile
{	
	position += currentSize;
	if (position/(float)fileSize > progressBar.progress)
		progressBar.progress = position/(float)fileSize;
	
	if (position >= fileSize)
	{
		if (!isVoiceClip)
		{
			[[ClientNetWorkController getInstance] sendDataCompleteRequest:transferid
																	   sid:chatViewData.sid 
																	  ftid:ftid
																	   url:fileUrl];
			cancelButton.hidden = YES;	
			status = SEND_FINISHED;
			//[chatViewData sendFinished:self];
		}
		else 
		{
			[[ClientNetWorkController getInstance] sendVoiceClip:chatViewData.contact.jid
															 sid:chatViewData.sid
													 totalLength:0
													base64Length:0
															data:0
															 url:fileUrl];
			progressBar.hidden = YES;
			status = SEND_FINISHED;
			[chatViewData sendFinished:self];
		}
	}
	else
	{
		[self sendFile];	
		index++;
	}
}

- (void)receivedAction:(NSString*)theAction
{
	if ([theAction compare:@"cancel"] == NSOrderedSame)
	{
		progressBar.hidden = YES;
		cancelButton.hidden = YES;
		status = SEND_CANCELED;
		[imageData release];
		imageData = nil;
	}
	else if ([theAction compare:@"accept"] == NSOrderedSame)
	{
		status = SEND_ACCEPTED;
		[self sendFile];
	}
	else if ([theAction compare:@"reject"] == NSOrderedSame)
	{
		progressBar.hidden = YES;
		cancelButton.hidden = YES;
		status = SEND_CANCELED;
		[imageData release];
		imageData = nil;
	}
	else if ([theAction compare:@"complete_ack"] == NSOrderedSame)
	{
		progressBar.hidden = YES;
		cancelButton.hidden = YES;
		[imageData release];
		imageData = nil;
		[chatViewData sendFinished:self];
	}
}

- (void)buttonClicked:(id)sender
{
	if (playButton == sender)
	{
		[chatViewData.controller showPlayView:self];
	}else {
		[[ClientNetWorkController getInstance] sendCancelTransferRequest:transferid sid:chatViewData.sid ftid:ftid];
		progressBar.hidden = YES;
		cancelButton.hidden = YES;
		status = SEND_CANCELED;
		[imageData release];
		imageData = nil;
		[chatViewData receiveCanceled:self];
		[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:self];
	}

}

- (void) dealloc
{
	[etag release];
	[fileName release];
	[fileUrl release];
	httpClient.delegate = nil;
	[httpClient release];
	[cancelButton release];
	[progressBar release];
	[playButton release];
	[transferid release];
	[ftid  release];
	[imageData release];	
	[super dealloc];
}

@end
