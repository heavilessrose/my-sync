//
//  MessageParser.m
//  msn_for_mac
//
//  Created by  apple on 08-9-25.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "MessageParser.h"
#import "ClientNetWorkController.h"
#import "MSNAppDelegate.h"
#import "GroupInfo.h"
#import "PersonInfo.h"
#import "StrangerInfo.h"
#import "define.h"

//static NSInteger s_Count4 = 0;

NSString *BeAddedRequestNotification = @"BeAddedRequest";
NSString *ReceiveMessageNotification = @"ReceiveMessage";
NSString *LocalNameResultNotification = @"LocalNameResultNotification";
NSString *LogoutNotification = @"Logout";
NSString *UpdateAuthInfoNotification = @"UpdateAuthInfo";
NSString *ParticipantChangedNotification = @"ParticipantChanged";
NSString *ReceiveVoiceNotification = @"ReceiveVoice";
NSString *ReceiveFileNotification = @"ReceiveFile";
NSString *ReceiveActionNotification = @"ReceiveAction";
NSString *ReceiveStatusNotification = @"ReceiveStatus";
NSString *ReceiveDataNotification = @"ReceiveData";

@implementation MessageParser

@synthesize  messageType, receiveJid, receiveImid, receiveTransferid, receiveSid, receiveFtid, receiveStatus, receiveSeqid,itemInfo;
@synthesize resultString, errorString, errorCode, character, parserData, actionType, fileName, fileSize,messageBody, messageReason;
@synthesize currentSize, fileStart, fileEnd, base64data, fileURL, parseError;
@synthesize receiveMsgTime;

- (id)initWithData:(NSData *)theData
{
	self = [super init];
	if (self != nil) 
	{
		errorCode = 0;
		nudge = NO;
		urlChanged = NO;
		resultString = nil;
		errorString = nil;
		itemInfo = nil;
		self.receiveSid = nil;
		self.receiveJid = nil;
		self.receiveImid = nil;
		self.receiveFtid = nil;
		self.receiveTransferid = nil;
		self.receiveSeqid = nil;
		self.fileName = nil;
		self.fileSize = nil;
		self.fileURL = nil;
		self.currentSize = nil;
		self.fileStart = nil;
		self.fileEnd = nil;
		self.base64data = nil;
		receiveMsgTime = nil;
		nodeType = EM_UNKNOW_MESSAGE;
		self.parserData = theData;
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		infoDictionary = appdelegate.infoDict;
	}
	return self;
	
}

- (void) dealloc
{
//	MYLog(@"-----------------%@ dealloc----------------", self);
	[itemInfo release];
	[fileName release];
	[fileSize release];
	[fileURL release];
	[receiveImid release];
	[receiveJid release];
	[receiveSid release];
	[receiveFtid release];
	[receiveTransferid release];
	[receiveSeqid release];
	[messageType release];
	[messageBody release];
	[messageReason release];
	[actionType release];
	[parserData release];
	[character release];
	[resultString release];
	[errorString release];
	[currentSize release];
	[fileStart release];
	[fileEnd release];
	[base64data release];
	[parseError release];
	[receiveStatus release];
	[receiveMsgTime release];
	[super dealloc];
}

- (NSString*)praseResponseData:(NSError**)theError
{
	self.resultString = @"BAD";
	NSXMLParser *parser = [[NSXMLParser alloc] initWithData:parserData];
	[parser setDelegate:self];
	[parser setShouldProcessNamespaces:NO];
	[parser setShouldReportNamespacePrefixes:NO];
	[parser setShouldResolveExternalEntities:NO];
	[parser parse];
    
	self.parseError = [parser parserError];
	if (parseError != nil)
		theError = &parseError;
	[parser setDelegate: nil];
	[parser release];
	return resultString;
}

- (void)updateProfileVersion:(NSString*)theVersion
{
	NSString *version = [infoDictionary objectForKey:kProfileVersionKey];
	if (version == nil)
		[infoDictionary setObject:theVersion forKey:kProfileVersionKey];
	else
	{
		NSArray *news = [theVersion componentsSeparatedByString:@"|"];
		NSArray *olds = [version componentsSeparatedByString:@"|"];
		version = @"";
		int i = 0;
		for (i = 0;i < 3; i++)
		{
			if ([news objectAtIndex:i] != nil)
			{
				version = [version stringByAppendingString:[news objectAtIndex:i]];
			}
			else if ([olds objectAtIndex:i] != nil)
			{
				version = [version stringByAppendingString:[olds objectAtIndex:i]];
			}
			if (i < 2)
				version = [version stringByAppendingString:@"|"];
		}
		[infoDictionary setObject:version forKey:kProfileVersionKey];
	}
	[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
}

- (void)updateRosterVersion:(NSString*)theVersion
{
	NSString *version = [infoDictionary objectForKey:kRosterVersionKey];
	if (version == nil)
		[infoDictionary setObject:theVersion forKey:kRosterVersionKey];
	else
	{
		NSArray *news = [theVersion componentsSeparatedByString:@"|"];
		NSArray *olds = [version componentsSeparatedByString:@"|"];
		version = @"";
		int i = 0;
		for (i = 0;i < 4; i++)
		{
			if ([news objectAtIndex:i] != nil && [[news objectAtIndex:i] length] > 0)
			{
				version = [version stringByAppendingString:[news objectAtIndex:i]];
			}
			else if ([olds objectAtIndex:i] != nil)
			{
				version = [version stringByAppendingString:[olds objectAtIndex:i]];
			}
			if (i < 3)
				version = [version stringByAppendingString:@"|"];
		}
		[infoDictionary setObject:version forKey:kRosterVersionKey];
	}
	[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
}


- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI 
									   qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
{
	
	if (elementName != nil)
	{
		NSString *value = nil;
		int index = 0;
		if ([elementName compare:@"iq"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"type"];
				
				if (index != NSNotFound)
					self.resultString = [attributeDict objectForKey:@"type"];
			}
			else{
				MYLog(@">>>message parser bad response!");
				MYLog(@">>>[element]:",elementName);
			}
		}
		else if ([elementName compare:@"error"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"code"];
				
				if (index != NSNotFound)
					self.errorCode = [[attributeDict objectForKey:@"code"] intValue];
			}
		}
		else if ([elementName compare:@"message"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"type"];
				
				if (index != NSNotFound)
				{
					nudge = NO;
					self.messageBody = nil;
					self.messageReason = nil;
					self.receiveJid = nil;
					self.receiveImid = nil;
					self.receiveSid = nil;
					self.receiveTransferid = nil;
					self.receiveStatus = nil;
					self.receiveSeqid = nil;
					self.messageType = [attributeDict objectForKey:@"type"];
					
					if ([messageType compare:@"chat"] == NSOrderedSame)	
					{
						nodeType = EM_MESSAGE_CHAT;
					}
					else if ([messageType compare:@"groupchat"] == NSOrderedSame)
					{
						nodeType = EM_MESSAGE_GROUPCHAT;	
					}
					else if ([messageType compare:@"voiceclip"] == NSOrderedSame)
					{
						nodeType = EM_MESSAGE_VOICE;
					}
					else if ([messageType compare:@"ft"] == NSOrderedSame)
					{
						nodeType = EM_MESSAGE_File;
						index = [[attributeDict allKeys] indexOfObject:@"transferid"];
						if (index != NSNotFound)
						{
							self.receiveTransferid = [attributeDict objectForKey:@"transferid"];	
						}
					}
					
					index = [[attributeDict allKeys] indexOfObject:@"jid"];
					
					if (index != NSNotFound)
					{
						self.receiveJid = [attributeDict objectForKey:@"jid"];	
					}
					else
					{
						index = [[attributeDict allKeys] indexOfObject:@"imid"];
						if (index != NSNotFound)
						{
							self.receiveImid = [attributeDict objectForKey:@"imid"];	
						}
					}
					index = [[attributeDict allKeys] indexOfObject:@"sid"];
					
					if (index != NSNotFound)
					{
						self.receiveSid = [attributeDict objectForKey:@"sid"];	
					}
				}
			}
		}
		else if ([elementName compare:@"control"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"ftid"];
				if (index != NSNotFound)
				{
					self.receiveFtid = [attributeDict objectForKey:@"ftid"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"action"];
				if (index != NSNotFound)
				{
					self.actionType = [attributeDict objectForKey:@"action"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"status"];
				if (index != NSNotFound)
				{
					self.receiveStatus = [attributeDict objectForKey:@"status"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"seqid"];
				if (index != NSNotFound)
				{
					self.receiveSeqid = [attributeDict objectForKey:@"seqid"];
				}
			}
		}
		else if ([elementName compare:@"query"] == NSOrderedSame)
		{
			self.actionType = nil;
			
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"xmlns"];
				
				if (index != NSNotFound)
				{
					value = [attributeDict objectForKey:@"xmlns"];
					
					if ([value compare:@"jabber:iq:roster"] == NSOrderedSame)	
					{
						self.actionType = nil;
						index = [[attributeDict allKeys] indexOfObject:@"action"];
						
						if (index != NSNotFound)
						{
							self.actionType = [attributeDict objectForKey:@"action"];
						}
						
						index = [[attributeDict allKeys] indexOfObject:@"version"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"version"];
							if (value != nil)
								[self updateRosterVersion:value];
								//[infoDictionary setObject:value forKey:kRosterVersionKey];
						}
					}
					else if ([value compare:@"vcard-temp"] == NSOrderedSame)
					{
						index = [[attributeDict allKeys] indexOfObject:@"version"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"version"];	
							if (value != nil)
								[self updateProfileVersion:value];
								//[infoDictionary setObject:value forKey:kProfileVersionKey];
						}
						
						index = [[attributeDict allKeys] indexOfObject:@"resourceid"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"resourceid"];	
							if (value != nil)
								[infoDictionary setObject:value forKey:@"resourceid"];
						}
						nodeType = EM_MESSAGE_QUERY;
					}
					else if ([value compare:@"jabber:x:dialog"] == NSOrderedSame)
					{
						self.receiveSid = nil;
						self.actionType = nil;
						index = [[attributeDict allKeys] indexOfObject:@"sid"];
						if (index != NSNotFound)
						{
							self.receiveSid = [attributeDict objectForKey:@"sid"];
						}
						
						index = [[attributeDict allKeys] indexOfObject:@"action"];
						
						if (index != NSNotFound)
						{
							self.actionType = [attributeDict objectForKey:@"action"];
						}
					}
				}
			}
		}
		else if ([elementName compare:@"participant"] == NSOrderedSame)
		{
			self.itemInfo = [[[PersonInfo alloc] init] autorelease];
			index = [[attributeDict allKeys] indexOfObject:@"jid"];
			
			if (index != NSNotFound)
			{
				itemInfo.jid = [attributeDict objectForKey:@"jid"];	
			}
			else
			{
				index = [[attributeDict allKeys] indexOfObject:@"imid"];
				if (index != NSNotFound)
				{
					itemInfo.imid = [attributeDict objectForKey:@"imid"];	
				}
				index = [[attributeDict allKeys] indexOfObject:@"nickname"];
				if (index != NSNotFound)
				{
					itemInfo.nickname = [attributeDict objectForKey:@"nickname"];
				}
			}
		}
		else if ([elementName compare:@"portrait"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"mime"];
				
				if (index != NSNotFound)
				{
					if (itemInfo != nil)
					{
						itemInfo.portraitType = [attributeDict objectForKey:@"xmlns"];
					}
				}
			}
		}
		else if ([elementName compare:@"presence"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"type"];
				if (index != NSNotFound && resultString == nil)
				{
					self.resultString = [attributeDict objectForKey:@"type"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"from"];
				if (index != NSNotFound)
				{
					NSString *from = [attributeDict objectForKey:@"from"];
					if ([from compare:@"msn.pica"] == NSOrderedSame)
					{
						nodeType = EM_MESSAGE_PRESENCE_SELF;
					}
					else
					{
						self.itemInfo = [[[PersonInfo alloc] init] autorelease];
						itemInfo.jid = from;
						nodeType = EM_MESSAGE_PRESENCE_OTHERS;
					}
				}
				index = [[attributeDict allKeys] indexOfObject:@"version"];
				if (index != NSNotFound)
				{
					value = [attributeDict objectForKey:@"version"];
					if (value != nil)
						[self updateRosterVersion:value];
				}
			}
		}
		else if ([elementName compare:@"url"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_VOICE)
			{
				self.fileURL = nil;
				index = [[attributeDict allKeys] indexOfObject:@"mime"];
				if (index != NSNotFound)
				{
					self.fileName = [attributeDict objectForKey:@"mime"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"bytes"];
				if (index != NSNotFound)
				{
					self.fileSize = [attributeDict objectForKey:@"bytes"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"length"];
				if (index != NSNotFound)
				{
					self.currentSize = [attributeDict objectForKey:@"length"];
				}
			}
		}
		else if ([elementName compare:@"data"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_File)
			{
				index = [[attributeDict allKeys] indexOfObject:@"bytes"];
				if (index != NSNotFound)
				{
					self.fileSize = [attributeDict objectForKey:@"bytes"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"start"];
				if (index != NSNotFound)
				{
					self.fileStart = [attributeDict objectForKey:@"start"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"end"];
				if (index != NSNotFound)
				{
					self.fileEnd = [attributeDict objectForKey:@"end"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"length"];
				if (index != NSNotFound )
				{
					self.currentSize = [attributeDict objectForKey:@"length"];
				}
			}
			else if (nodeType == EM_MESSAGE_VOICE)
			{
				self.fileURL = nil;
				index = [[attributeDict allKeys] indexOfObject:@"mime"];
				if (index != NSNotFound)
				{
					self.fileName = [attributeDict objectForKey:@"mime"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"bytes"];
				if (index != NSNotFound)
				{
					self.fileSize = [attributeDict objectForKey:@"bytes"];
				}
				
				index = [[attributeDict allKeys] indexOfObject:@"length"];
				if (index != NSNotFound)
				{
					self.currentSize = [attributeDict objectForKey:@"length"];
				}
				
			}
		}
		else if ([elementName compare:@"item"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				if (actionType != nil)
				{
					if ([actionType compare:@"getportrait"] == NSOrderedSame)
					{
						index = [[attributeDict allKeys] indexOfObject:@"jid"];
						
						if (index != NSNotFound)
						{
							NSString *jid = [attributeDict objectForKey:@"jid"];
							if ([jid compare:@"msn.pica"] == NSOrderedSame)
							{
								nodeType = EM_MESSAGE_PRESENCE_SELF;
							}
							else
							{
								self.itemInfo = [[[PersonInfo alloc] init] autorelease];
								itemInfo.jid = jid;
								nodeType = EM_MESSAGE_PRESENCE_OTHERS;
							}
						}
					}
					else if ([actionType compare:@"add"] == NSOrderedSame
							 || [actionType compare:@"subscriptionanswer"] == NSOrderedSame)
					{
						index = [[attributeDict allKeys] indexOfObject:@"jid"];
						if (index != NSNotFound)
						{
							NSString *jid = [attributeDict objectForKey:@"jid"];
							if (jid != nil)
							{
								self.itemInfo = [[[PersonInfo alloc] init] autorelease];
								itemInfo.jid = jid;
								index = [[attributeDict allKeys] indexOfObject:@"imid"];
								if (index != NSNotFound)
								{
									itemInfo.imid = [attributeDict objectForKey:@"imid"];	
								}
								index = [[attributeDict allKeys] indexOfObject:@"nickname"];
								if (index != NSNotFound)
								{
									itemInfo.nickname = [attributeDict objectForKey:@"nickname"];	
								}
								index = [[attributeDict allKeys] indexOfObject:@"gbcode"];
								if (index != NSNotFound)
								{
									itemInfo.gbcode = [attributeDict objectForKey:@"gbcode"];	
								}
								index = [[attributeDict allKeys] indexOfObject:@"sync"];
								if (index != NSNotFound)
								{
									itemInfo.sync = [attributeDict objectForKey:@"sync"];	
								}
								index = [[attributeDict allKeys] indexOfObject:@"block"];
								if (index != NSNotFound)
								{
									itemInfo.block = [attributeDict objectForKey:@"block"];	
								}
								nodeType = EM_MESSAGE_PRESENCE_OTHERS;
							}
						}
					}
					else if ([actionType compare:@"delete"] == NSOrderedSame
							 || [actionType compare:@"rmblk"] == NSOrderedSame)
					{
						index = [[attributeDict allKeys] indexOfObject:@"jid"];
						if (index != NSNotFound)
						{
							NSString *jid = [attributeDict objectForKey:@"jid"];
							if (jid != nil)
							{
								self.itemInfo = [[[PersonInfo alloc] init] autorelease];
								itemInfo.jid = jid;
								itemInfo.sync = [attributeDict objectForKey:@"sync"];
								if ([actionType compare:@"rmblk"] == NSOrderedSame)
									itemInfo.block = @"1";
								nodeType = EM_MESSAGE_PRESENCE_OTHERS;
							}
						}
					}
					else if ([actionType compare:@"block"] == NSOrderedSame
							 || [actionType compare:@"unblock"] == NSOrderedSame)
					{
						index = [[attributeDict allKeys] indexOfObject:@"jid"];
						if (index != NSNotFound)
						{
							NSString *jid = [attributeDict objectForKey:@"jid"];
							if (jid != nil)
							{
								self.itemInfo = [[[PersonInfo alloc] init] autorelease];
								itemInfo.jid = jid;
								itemInfo.block = [attributeDict objectForKey:@"block"];
								itemInfo.sync = [attributeDict objectForKey:@"sync"];
								nodeType = EM_MESSAGE_PRESENCE_OTHERS;
							}
						}
					}
					else if ([actionType compare:@"subscriptionask"] == NSOrderedSame)
					{
						index = [[attributeDict allKeys] indexOfObject:@"imid"];
						if (index != NSNotFound)
						{
							strangerInfo = [[StrangerInfo alloc] init];
							strangerInfo.imid = [attributeDict objectForKey:@"imid"];
							nodeType = EM_MESSAGE_PRESENCE_OTHERS;
						}
					}
					else if ([actionType compare:@"modify"] == NSOrderedSame)
					{
//						MYLog(@">>> modification of localName");
						NSString *sync = [attributeDict objectForKey:@"sync"];
						NSMutableDictionary *info = nil;
						if ([sync compare:@"updated"] == NSOrderedSame)
						{
							// Post success notification.
							info = [NSMutableDictionary dictionaryWithObject:@"success" forKey:@"updateLocalNameResult"];
						}else{
							// Post failed notificaition.
							info = [NSMutableDictionary dictionaryWithObject:@"fail" forKey:@"updateLocalNameResult"];
						}
						[info setObject:[attributeDict objectForKey:@"jid"] forKey:@"jid"];
						[info setObject:[attributeDict objectForKey:@"localname"] forKey:@"localname"];
						[info setObject:[attributeDict objectForKey:@"localgbcode"] forKey:@"localgbcode"];
						
						[ClientNetWorkController postNotification:LocalNameResultNotification info:info];
						nodeType = EM_MESSAGE_IQ;
					}
					else
					{
						MYLog(@"unknown item");
					}					
				}// end if(actionType != nil)
				else  
				{
					index = [[attributeDict allKeys] indexOfObject:@"jid"];
					if (index != NSNotFound)
					{
						NSString *jid = [attributeDict objectForKey:@"jid"];
						if (jid != nil)
						{
							self.itemInfo = [[[PersonInfo alloc] init] autorelease];
							itemInfo.jid = jid;
							index = [[attributeDict allKeys] indexOfObject:@"imid"];
							if (index != NSNotFound)
							{
								itemInfo.imid = [attributeDict objectForKey:@"imid"];	
							}
							index = [[attributeDict allKeys] indexOfObject:@"nickname"];
							if (index != NSNotFound)
							{
								itemInfo.nickname = [attributeDict objectForKey:@"nickname"];	
							}
							index = [[attributeDict allKeys] indexOfObject:@"gbcode"];
							if (index != NSNotFound)
							{
								itemInfo.gbcode = [attributeDict objectForKey:@"gbcode"];	
							}
							index = [[attributeDict allKeys] indexOfObject:@"sync"];
							if (index != NSNotFound)
							{
								itemInfo.sync = [attributeDict objectForKey:@"sync"];	
							}
							index = [[attributeDict allKeys] indexOfObject:@"block"];
							if (index != NSNotFound)
							{
								itemInfo.block = [attributeDict objectForKey:@"block"];	
							}
							nodeType = EM_MESSAGE_PRESENCE_OTHERS;
						}
					}
					
				}
			}
		}		
	}
		
	self.character = nil;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	NSString *key = nil;
	if (elementName != nil)
	{
		if ([elementName compare:@"error"] == NSOrderedSame)
		{
			self.errorString = character;
			MYLog(@"errorstring:%@",character);
		}
		else if ([elementName compare:@"message"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_CHAT
				|| nodeType == EM_MESSAGE_GROUPCHAT)
			{
				
				NSMutableDictionary * info = [NSMutableDictionary dictionaryWithCapacity:0];
				if (messageType != nil)
					[info setObject:messageType forKey:@"type"];
				if (receiveJid != nil)
					[info setObject:receiveJid forKey:@"jid"];
				else if (receiveImid != nil)
					[info setObject:receiveImid forKey:@"imid"];	
				if (receiveSid != nil)
					[info setObject:receiveSid forKey:@"sid"];	
				if (messageBody != nil)
					[info setObject:messageBody forKey:@"message"];
				if (nudge)
					[info setObject:[NSNumber numberWithBool:nudge] forKey:@"nudge"];
				if (messageReason != nil)
					[info setObject:messageReason forKey:@"reason"];
				if (receiveMsgTime != nil)
					[info setObject: receiveMsgTime forKey:@"recvtime"];
				[ClientNetWorkController postNotification:ReceiveMessageNotification info:info];
			}
			else if (nodeType == EM_MESSAGE_VOICE)
			{
				NSMutableDictionary * info = [NSMutableDictionary dictionaryWithCapacity:0];
				if (receiveJid != nil)
					[info setObject:receiveJid forKey:@"jid"];
				
				if (receiveSid != nil)
					[info setObject:receiveSid forKey:@"sid"];
				
				if (receiveImid != nil)
					[info setObject:receiveImid forKey:@"imid"];
				
				if (fileName != nil)
					[info setObject:fileName forKey:@"type"];
				
				if (fileSize != nil)
					[info setObject:fileSize forKey:@"bytes"];
				
				if (currentSize != nil)
					[info setObject:currentSize forKey:@"size"];
				
				if (base64data != nil)
					[info setObject:base64data forKey:@"data"];
				
				if (fileURL != nil)
					[info setObject:fileURL forKey:@"url"];
				
				[ClientNetWorkController postNotification:ReceiveVoiceNotification info:info];
			}
			else if (nodeType == EM_MESSAGE_File)
			{
				NSMutableDictionary * info = [NSMutableDictionary dictionaryWithCapacity:0];
				
				if (receiveJid != nil)
					[info setObject:receiveJid forKey:@"jid"];
				else if (receiveImid != nil)
					[info setObject:receiveImid forKey:@"imid"];
				
				if (receiveSid != nil)
					[info setObject:receiveSid forKey:@"sid"];
				
				if (receiveTransferid != nil)
					[info setObject:receiveTransferid forKey:@"transferid"];
				
				if (receiveFtid != nil)
					[info setObject:receiveFtid forKey:@"ftid"];
				
				if ([actionType compare:@"push"] == NSOrderedSame)
				{
					if (fileName != nil)
						[info setObject:fileName forKey:@"name"];
					if (fileSize != nil)
						[info setObject:fileSize forKey:@"size"];
					
					[ClientNetWorkController postNotification:ReceiveFileNotification info:info];
				}
				else if ([actionType compare:@"accept"] == NSOrderedSame
						 || [actionType compare:@"reject"] == NSOrderedSame
						 || [actionType compare:@"cancel"] == NSOrderedSame
						 || [actionType compare:@"complete_ack"] == NSOrderedSame)
				{
					if (actionType != nil)
					{
						[info setObject:actionType forKey:@"action"];
						if (fileURL != nil)
							[info setObject:fileURL forKey:@"url"];		
						[ClientNetWorkController postNotification:ReceiveActionNotification info:info];
					}
				}			
				else if ([actionType compare:@"ack"] == NSOrderedSame)
				{
					if (receiveSeqid != nil)
						[info setObject:receiveSeqid forKey:@"seqid"];
					if (receiveStatus != nil)
						[info setObject:receiveStatus forKey:@"status"];
					[ClientNetWorkController postNotification:ReceiveStatusNotification info:info];
				}
				else if ([actionType compare:@"data"] == NSOrderedSame)
				{
					if (receiveSeqid != nil)
						[info setObject:receiveSeqid forKey:@"seqid"];
					
					if (fileSize != nil)
						[info setObject:fileSize forKey:@"filesize"];
					
					if (fileStart != nil)
						[info setObject:fileStart forKey:@"filestart"];
					
					if (fileEnd != nil)
						[info setObject:fileEnd forKey:@"fileend"];
					
					if (currentSize != nil)
						[info setObject:currentSize forKey:@"currentsize"];
                    
					if (base64data != nil){
						[info setObject:base64data forKey:@"data"];
                    }
					[ClientNetWorkController postNotification:ReceiveDataNotification info:info];
				}
				
			}
			nodeType = EM_UNKNOW_MESSAGE;
		}
		else if ([elementName compare:@"participant"] == NSOrderedSame)
		{
			if (itemInfo != nil && receiveSid != nil)
			{
				NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
				if (itemInfo.jid != nil || itemInfo.imid != nil)
				{
					[dict setObject:itemInfo forKey:@"info"];
					if (receiveSid != nil)
						[dict setObject:receiveSid forKey:@"sid"];
					if (actionType != nil)
						[dict setObject:actionType forKey:@"type"];
				}
				[ClientNetWorkController postNotification:ParticipantChangedNotification info:dict];
				self.itemInfo = nil;
			}
		}
		else if ([elementName compare:@"body"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_CHAT
				|| nodeType == EM_MESSAGE_GROUPCHAT)
			{
				self.messageBody = character;
			}
		}
		else if ([elementName isEqualToString: @"recvtime"])
		{
			if (nodeType == EM_MESSAGE_CHAT
				|| nodeType == EM_MESSAGE_GROUPCHAT)
			{
				self.receiveMsgTime = character;
			}
		}
		else if ([elementName compare:@"nudge"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_CHAT
				|| nodeType == EM_MESSAGE_GROUPCHAT)
			{
				nudge = YES;
			}
		}
		else if ([elementName compare:@"reason"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_CHAT
				|| nodeType == EM_MESSAGE_GROUPCHAT)
			{
				self.messageReason = character;
			}
		}
		else if ([elementName compare:@"query"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_QUERY)
			{
				NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:urlChanged] forKey:@"urlChanged"];
				[ClientNetWorkController postNotification:UpdateAuthInfoNotification info:dict];
			}
			self.actionType = nil;
			nodeType = EM_UNKNOW_MESSAGE;
		}
		else if ([elementName compare:@"presence"] == NSOrderedSame)
		{
			if (	nodeType == EM_MESSAGE_PRESENCE_SELF)
			{
				NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:urlChanged] forKey:@"urlChanged"];
				[ClientNetWorkController postNotification:UpdateAuthInfoNotification info:dict];
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					NSDictionary *info = [NSDictionary dictionaryWithObject:itemInfo forKey:PersonInfoKey];
					[ClientNetWorkController postNotification:UpdatePersonNotification info:info];
					self.itemInfo = nil;
				}
			}
			else
				MYLog(@" ------------unknown nodeType---------------- ");
			
			nodeType = EM_UNKNOW_MESSAGE;
		}
		else if ([elementName compare:@"item"] == NSOrderedSame)
		{
			if ( nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					if (actionType != nil)
					{
						if ([actionType compare:@"getportrait"] == NSOrderedSame
							|| [actionType compare:@"unblock"] == NSOrderedSame
							|| [actionType compare:@"block"] == NSOrderedSame
							|| [actionType compare:@"add"] == NSOrderedSame
							|| [actionType compare:@"delete"] == NSOrderedSame
							|| [actionType compare:@"rmblk"] == NSOrderedSame
							|| [actionType compare:@"subscriptionanswer"] == NSOrderedSame)
						{
//                            NSLog(@"(messeger parser)person count from server : %d", ++s_Count4);
							NSDictionary *info = [NSDictionary dictionaryWithObject:itemInfo forKey:PersonInfoKey];
							[ClientNetWorkController postNotification:UpdatePersonNotification info:info];	
							self.itemInfo = nil;
						}
						else
							MYLog(@" ------------unknown actionType:%@---------------- ",actionType);
					}
					else
					{
						NSDictionary *info = [NSDictionary dictionaryWithObject:itemInfo forKey:PersonInfoKey];
						[ClientNetWorkController postNotification:UpdatePersonNotification info:info];	
						self.itemInfo = nil;
					}
						
				}
				else if (strangerInfo != nil)
				{
					if (actionType != nil && [actionType compare:@"subscriptionask"] == NSOrderedSame)
					{
						NSDictionary *info = [NSDictionary dictionaryWithObject:strangerInfo forKey:PersonInfoKey];
						[ClientNetWorkController postNotification:BeAddedRequestNotification info:info];
					}
                    [strangerInfo release];
                    strangerInfo = nil;
				}
				else
					MYLog(@"itemInfo == nil");
			}
			else
				MYLog(@" nodeType != EM_MESSAGE_PRESENCE_OTHERS");
		}
		else if ([elementName compare:@"show"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_PRESENCE_SELF)
			{
				key = @"show";	
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.state = character;
				}		
			}
			else
			{
				MYLog(@"-----unknown state-------");
			}
		}
		else if ([elementName compare:@"name"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_File)
				self.fileName = character;
		}
		else if ([elementName compare:@"size"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_File)
				self.fileSize = character;
		}
		else if ([elementName compare:@"nickname"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_PRESENCE_SELF
				|| nodeType == EM_MESSAGE_QUERY)
			{
				key = @"nickname";	
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.nickname = character;
				}
				else if (strangerInfo != nil)
				{
					strangerInfo.nickname = character;
				}
			}
			else
			{
				MYLog(@"-----unknown nickname-------");
			}
		}
		else if ([elementName compare:@"impresa"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_PRESENCE_SELF
				|| nodeType == EM_MESSAGE_QUERY)
			{
				key = @"impresa";	
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.impresa = character;
				}
			}
			else
			{
				MYLog(@"-----unknown impresa-------");
			}
		}
		else if ([elementName compare:@"portraithash"] == NSOrderedSame)
		{		
			if (nodeType == EM_MESSAGE_PRESENCE_SELF
				|| nodeType == EM_MESSAGE_QUERY)
			{
				key = @"portraithash";	
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.portraithash = character;
				}
				NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:@"urlChanged"];
				[ClientNetWorkController postNotification:UpdateAuthInfoNotification info:dict];
			}
			else
			{
				MYLog(@"-----unknown portraithash-------");
			}			
		}
		else if ([elementName compare:@"data"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_PRESENCE_SELF
				|| nodeType == EM_MESSAGE_QUERY)
			{
				key = @"portraithashData";	
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.portraitData = character;
				}
			}
			else if (nodeType == EM_MESSAGE_File
					 || nodeType == EM_MESSAGE_VOICE)
			{
				self.base64data = character;
			}
			else
				MYLog(@"-----unknown data-------");
		}
		else if ([elementName compare:@"url"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_PRESENCE_SELF
				|| nodeType == EM_MESSAGE_QUERY)
			{
				urlChanged = YES;
				key = @"portraithashUrl";	
			}
			else if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.portraitURL = character;
				}
			}
			else if (nodeType == EM_MESSAGE_File
					 || nodeType == EM_MESSAGE_VOICE)
			{
				self.fileURL = character;
			}
			else
				MYLog(@"-----unknown url-------");
		}
		else if ([elementName compare:@"realname"] == NSOrderedSame)
		{
			if (strangerInfo != nil)
			{
				strangerInfo.realname = character;
			}
		}
		else if ([elementName compare:@"group"] == NSOrderedSame)
		{
			if (nodeType == EM_MESSAGE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					itemInfo.group = character;
				}
				else if (strangerInfo != nil)
				{
					strangerInfo.group = character;
				}
			}
		}
		else if ([elementName compare:@"sex"] == NSOrderedSame)
		{
			if (strangerInfo != nil)
			{
				strangerInfo.sex = character;
			}
		}
		else if ([elementName compare:@"age"] == NSOrderedSame)
		{
			if (strangerInfo != nil)
			{
				strangerInfo.age = character;
			}
		}
		else if ([elementName compare:@"province"] == NSOrderedSame)
		{
			if (strangerInfo != nil)
			{
				strangerInfo.province = character;
			}
		}
		else if ([elementName compare:@"desc"] == NSOrderedSame)
		{
			if (strangerInfo != nil)
			{
				strangerInfo.desc = character;
			}
		}
		if (key != nil)	
		{
			if (character == nil)				
				[infoDictionary removeObjectForKey:key];
			else
				[infoDictionary setObject:character forKey:key];
			
			NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:@"urlChanged"];
			[ClientNetWorkController postNotification:UpdateAuthInfoNotification info:dict];	
		}
	}
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
{
	if (string != nil)
	{
		if (character == nil)
			self.character = string;
		else
			self.character = [character stringByAppendingFormat:string];
	}
	else
		self.character = nil;
}

@end
