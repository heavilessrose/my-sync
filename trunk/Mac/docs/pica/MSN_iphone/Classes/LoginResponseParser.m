//
//  LoginResponseParser.m
//  msn_for_mac
//
//  Created by  apple on 08-9-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "LoginResponseParser.h"
#import "MSNAppDelegate.h"
#import "ClientNetWorkController.h"
#import "StrangerInfo.h"
#import "GroupInfo.h"
#import "PersonInfo.h"
#import "define.h"

//#import "MessageParser.h"
//static NSInteger s_Count3 = 0;

NSString		*GroupInfoKey = @"groupItem";
NSString		*PersonInfoKey = @"PersonItem";

NSString		*DeleteGroupNotification = @"DeleteGroup";
NSString		*DeletePersonNotification = @"DeletePerson";
NSString		*UpdateGroupNotification = @"UpdateGroup";
NSString		*UpdatePersonNotification = @"UpdatePerson";
NSString		*ShowContactListNotification = @"ShowContactList";
NSString		*LoginNotification = @"Login";
@implementation LoginResponseParser
@synthesize parserData, character, actionType, resultString, errorString, messageOrigin, errorCode,parseError,itemInfo;
@synthesize lastValue;

- (id)initWithData:(NSData *)theData
{
	self = [super init];
	if (self != nil) 
	{
		nodeType = EM_UNKNOW_NODE;
		noteType = EM_UNKNOW_NOTE;
		errorCode = 0;
		lastValue = -1;
		additionValue = -1;
		tempLastValue = -1;
		isGetRoster = NO;
		resultString = nil;		
		messageOrigin = nil;
		self.parserData = theData;
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		infoDictionary = appdelegate.infoDict;
	}
	return self;
	
}

- (void) dealloc
{
	MYLog(@"-----------------%@ dealloc----------------", self);
	[itemInfo release];
	[parserData release];
	[character release];
	[resultString release];
	[errorString release];
	[messageOrigin release];
	[parseError release];
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
	{
		theError = &parseError;	
	}
	[parser setDelegate: nil];
	[parser release];
	if (tempLastValue == 1 && isGetRoster){
		ClientNetWorkController* aNetwork = [ClientNetWorkController getInstance];
		[aNetwork sendContactsNickNameToServer];
		[aNetwork sendAPSInfoToServer];
		[aNetwork sendOfflineMsgRequest];
        //[aNetwork sendUpdatePortrait];
#if 0
        NSArray *filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
        NSString *directory = [filePaths objectAtIndex: 0];
        NSString *aPath = [directory stringByAppendingPathComponent:@"test.txt"];
        NSData* aData = [[NSFileManager defaultManager] contentsAtPath: aPath];
        MessageParser* aParser = [[MessageParser alloc] initWithData: aData];
        [aParser praseResponseData: nil];
        [aParser release];
#endif
	}
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
	NSString *value = nil;
	int index = -1;	
//    NSLog(@"parser delegate with attribute -- element : %@, namespaceURI : %@, qualifiedName : %@",
//    elementName, namespaceURI, qName);
	if (elementName != nil)
	{
		if ([elementName compare:@"iq"] == NSOrderedSame)
		{
            NSLog(@"<iq>");
			nodeType = EM_IQ;
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"type"];
				if (index != NSNotFound)
				{
					self.resultString = [attributeDict objectForKey:@"type"];
					if ([resultString compare:@"result"] == NSOrderedSame)			//vcard information
					{
						index = [[attributeDict allKeys] indexOfObject:@"addition"];
						if (index != NSNotFound)
						{
							additionValue = [[attributeDict objectForKey:@"addition"] intValue];	
                            NSLog(@"    addition = %d", additionValue);
						}
						
						index = [[attributeDict allKeys] indexOfObject:@"last"];
						if (index != NSNotFound)
						{
							lastValue = [[attributeDict objectForKey:@"last"] intValue];	
							if (tempLastValue != 1 && lastValue == 1){
								tempLastValue = lastValue;
							}
                            NSLog(@"    last = %d", tempLastValue);
						}
					}					
				}
			}
		}
		else if([elementName compare:@"presence"] == NSOrderedSame)
		{
            NSLog(@"<presence>");
			nodeType = EM_PRESENCE;
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"type"];
				if (index != NSNotFound )
				{
					self.resultString = [attributeDict objectForKey:@"type"];	
                    NSLog(@"    presence::type = %@", resultString);
					index = [[attributeDict allKeys] indexOfObject:@"from"];
					if (index != NSNotFound)
					{
						self.messageOrigin = [attributeDict objectForKey:@"from"];	
                        NSLog(@"    presence::from = %@", messageOrigin);
						if ([messageOrigin compare:@"msn.pica"] == NSOrderedSame)
						{
							noteType = EM_NOTE_PRESENCE_SELF;
						}
						else
						{
							self.itemInfo = [[[PersonInfo alloc] init] autorelease];
							itemInfo.jid = messageOrigin;
							noteType = EM_NOTE_PRESENCE_OTHERS;
						}
					}

					index = [[attributeDict allKeys] indexOfObject:@"version"];
					if (index != NSNotFound)
					{
						value = [attributeDict objectForKey:@"version"];	
                        NSLog(@"    presence::version = %@", value);
						if (value != nil)
							[self updateRosterVersion:value];
					}
				}			
			}
		}
		else if ([elementName compare:@"item"] == NSOrderedSame)
		{
            NSLog(@"<item>");
			if (actionType == nil)
			{
				if (attributeDict && [attributeDict count] > 0)
				{
					self.itemInfo = [[[PersonInfo alloc] init] autorelease];
					index = [[attributeDict allKeys] indexOfObject:@"jid"];
					if (index != NSNotFound)
					{
						itemInfo.jid = [attributeDict objectForKey:@"jid"];	
                        NSLog(@"    item::jid = %@", itemInfo.jid);
					}
					index = [[attributeDict allKeys] indexOfObject:@"imid"];
					if (index != NSNotFound)
					{
						itemInfo.imid = [attributeDict objectForKey:@"imid"];	
                        NSLog(@"    item::imid = %@", itemInfo.imid);
					}
					index = [[attributeDict allKeys] indexOfObject:@"nickname"];
					if (index != NSNotFound)
					{
						itemInfo.nickname = [attributeDict objectForKey:@"nickname"];	
                        NSLog(@"    item::nickname = %@", itemInfo.nickname);
					}
					index = [[attributeDict allKeys] indexOfObject:@"gbcode"];
					if (index != NSNotFound)
					{
						itemInfo.gbcode = [attributeDict objectForKey:@"gbcode"];	
                        NSLog(@"    item::gbcode = %@", itemInfo.gbcode);
					}
					index = [[attributeDict allKeys] indexOfObject:@"localname"];
					if (index != NSNotFound)
					{
						itemInfo.localname = [attributeDict objectForKey:@"localname"];		
                        NSLog(@"    item::localname = %@", itemInfo.localname);
					}
					index = [[attributeDict allKeys] indexOfObject:@"localgbcode"];
					if (index != NSNotFound)
					{
						itemInfo.localgbcode = [attributeDict objectForKey:@"localgbcode"];		
                        NSLog(@"    item::localgbcode = %@", itemInfo.localgbcode);
					}
					index = [[attributeDict allKeys] indexOfObject:@"sync"];
					if (index != NSNotFound)
					{
						itemInfo.sync = [attributeDict objectForKey:@"sync"];		
                        NSLog(@"    item::sync = %@", itemInfo.sync);
					}
					index = [[attributeDict allKeys] indexOfObject:@"block"];
					if (index != NSNotFound)
					{
						itemInfo.block = [attributeDict objectForKey:@"block"];		
                        NSLog(@"    item::block = %@", itemInfo.block);
					}
				}
				nodeType = EM_ITEM;
			}
			else if ([actionType compare:@"subscriptionask"] == NSOrderedSame)
			{
                NSLog(@"<subscriptionask>");
				index = [[attributeDict allKeys] indexOfObject:@"imid"];
				if (index != NSNotFound)
				{
					strangerInfo = [[StrangerInfo alloc] init];
					strangerInfo.imid = [attributeDict objectForKey:@"imid"];
					nodeType = EM_NOTE_PRESENCE_OTHERS;
				}
			}
				
		}
		else if ([elementName compare:@"error"] == NSOrderedSame)
		{
            NSLog(@"<error>");
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"code"];
				
				if (index != NSNotFound)
					self.errorCode = [[attributeDict objectForKey:@"code"] intValue];
                NSLog(@"    error::errorCode = %d", errorCode);
			}
		}
		else if ([elementName compare:@"spaces"] == NSOrderedSame)
		{
            NSLog(@"<spaces>");
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"gleam"];
				if (index != NSNotFound)
				{
					itemInfo.spacesGleam = [[attributeDict objectForKey:@"gleam"] boolValue];	
				}
			}
		}
		else if ([elementName compare:@"group"] == NSOrderedSame)
		{
            NSLog(@"<group>");
			if (nodeType != EM_ITEM)
			{
				nodeType = EM_GROUP;
				if (attributeDict && [attributeDict count] > 0)
				{
					gropuInfo = [[GroupInfo alloc] init];
					index = [[attributeDict allKeys] indexOfObject:@"groupid"];
					if (index != NSNotFound)
					{
						gropuInfo.stringId = [attributeDict objectForKey:@"groupid"];	
                        NSLog(@"    groupid = %@", gropuInfo.stringId);
					}
					index = [[attributeDict allKeys] indexOfObject:@"name"];
					if (index != NSNotFound)
					{
						gropuInfo.name = [attributeDict objectForKey:@"name"];	
                        NSLog(@"    name = %@", gropuInfo.name);
					}
					index = [[attributeDict allKeys] indexOfObject:@"gbcode"];
					if (index != NSNotFound)
					{
						gropuInfo.gbcode = [attributeDict objectForKey:@"gbcode"];
                        NSLog(@"    gbcode = %@", gropuInfo.gbcode);	
					}
					index = [[attributeDict allKeys] indexOfObject:@"sync"];
					if (index != NSNotFound)
					{
						gropuInfo.sync = [attributeDict objectForKey:@"sync"];
                        NSLog(@"    sync = %@", gropuInfo.sync);		
					}
				}
			}
			
		}
		else if ([elementName compare:@"query"] == NSOrderedSame)
		{
            NSLog(@"<query>");
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"xmlns"];
				if (index != NSNotFound)
				{
					value = [attributeDict objectForKey:@"xmlns"];
                    NSLog(@"    xmlns = %@", value);
					if ([value compare:@"vcard-temp"] == NSOrderedSame)			//vcard information
					{
						index = [[attributeDict allKeys] indexOfObject:@"version"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"version"];	
							if (value != nil)
								[self updateProfileVersion:value];
								//[infoDictionary setObject:value forKey:kProfileVersionKey];
						} else {
                            NSLog(@"    version = NULL");
                        }
						
						index = [[attributeDict allKeys] indexOfObject:@"resourceid"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"resourceid"];
							if (value != nil)
								[infoDictionary setObject:value forKey:@"resourceid"];
						}
						noteType = EM_NOTE_VCARD;
					}
					else if ([value compare:@"jabber:iq:group"] == NSOrderedSame)		//group information
					{
						index = [[attributeDict allKeys] indexOfObject:@"version"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"version"];	
                            NSLog(@"    version = %@", value);
							if (lastValue == 1 && value != nil)
								[infoDictionary setObject:value forKey:@"group_version"];
						} else {
                            NSLog(@"    version = NULL");
                        }
						if (additionValue == 0)
						{
							[ClientNetWorkController postNotification:DeleteGroupNotification info:nil];
						}
						noteType = EM_NOTE_GROUP;
					}
					else if ([value compare:@"jabber:iq:roster"] == NSOrderedSame)		//roster information
					{
						isGetRoster = YES;
						index = [[attributeDict allKeys] indexOfObject:@"action"];
						
						if (index != NSNotFound)
						{
							self.actionType = [attributeDict objectForKey:@"action"];
						}
						else
						{
							noteType = EM_NOTE_ROSER;
						}
						index = [[attributeDict allKeys] indexOfObject:@"version"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"version"];	
                            NSLog(@"    version = %@", value);
							if (lastValue == 1 && value != nil)
								[self updateRosterVersion:value];
								//[infoDictionary setObject:value forKey:kRosterVersionKey];
						}
						index = [[attributeDict allKeys] indexOfObject:@"deviceid"];
						if (index != NSNotFound)
						{
							value = [attributeDict objectForKey:@"deviceid"];	
							if (lastValue == 1 && value != nil)
								[infoDictionary setObject:value forKey:@"deviceid"];
						}
						if (additionValue == 0)
						{
							[ClientNetWorkController postNotification:DeletePersonNotification info:nil];
						}
					}
				}
			}
		}
		else if ([elementName compare:@"portrait"] == NSOrderedSame)
		{
            NSLog(@"<portrait>");
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"mime"];
				if (index != NSNotFound)
				{
					value = [attributeDict objectForKey:@"mime"];	
                    NSLog(@"    mime = %@", value);
				}
			}
			if (value != nil)
				[infoDictionary setObject:value forKey:@"mime"];
		}
		else if ([elementName compare:@"data"] == NSOrderedSame)
		{
            NSLog(@"<data>");
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"length"];
				if (index != NSNotFound)
				{
					value = [attributeDict objectForKey:@"length"];		
                    NSLog(@"    length = %@", value);
					if (value != nil)
						[infoDictionary setObject:value forKey:@"length"];
				}
			}
		}	
	}
	
	self.character = nil;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	NSString *key = nil;
//    NSLog(@"parser delegate no attribute -- element : %@, namespaceURI : %@, qualifiedName : %@",
//          elementName, namespaceURI, qName);
	if (elementName != nil)
	{
		if ([elementName compare:@"iq"] == NSOrderedSame)
		{
			lastValue = -1;
			additionValue = -1;
			nodeType = EM_UNKNOW_NODE;
		}
		else if ([elementName compare:@"query"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD)
			{
				[ClientNetWorkController postNotification:ShowContactListNotification info:nil];	
			}
			else if (noteType == EM_NOTE_ROSER)
			{
				if (lastValue == 1)
				{
					[[ClientNetWorkController getInstance] setMessageType:MESSAGE_REQUEST];
				}
			}
			noteType = EM_UNKNOW_NOTE;
		}
		else if ([elementName compare:@"presence"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_PRESENCE_SELF)
			{
				NSDictionary *info = [NSDictionary dictionaryWithObject:resultString forKey:@"presence"];
				[ClientNetWorkController postNotification:LoginNotification info:info];
			}
			if (noteType == EM_NOTE_PRESENCE_OTHERS)
			{
				if (itemInfo != nil)
				{
					NSDictionary *info = [NSDictionary dictionaryWithObject:itemInfo forKey:PersonInfoKey];
					[ClientNetWorkController postNotification:UpdatePersonNotification info:info];		
					self.itemInfo = nil;
				}
			}
		}
		else if([elementName compare:@"item"] == NSOrderedSame)
		{
			if (itemInfo != nil)
			{
//                NSLog(@"(login parser)person count from server : %d", ++s_Count3);
				NSDictionary *info = [NSDictionary dictionaryWithObject:itemInfo forKey:PersonInfoKey];
				[ClientNetWorkController postNotification:UpdatePersonNotification info:info];
				self.itemInfo = nil;	
			}
			else if (strangerInfo != nil)
			{
				NSDictionary *info = [NSDictionary dictionaryWithObject:strangerInfo forKey:PersonInfoKey];
				[ClientNetWorkController postNotification:BeAddedRequestNotification info:info];
				[strangerInfo release];
				strangerInfo = nil;
			}
		}
		else if ([elementName compare:@"group"] == NSOrderedSame)
		{		
			if (nodeType == EM_ITEM
				|| noteType == EM_NOTE_PRESENCE_OTHERS)
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
			else
			{
				if (gropuInfo != nil)
				{
					NSDictionary *info = [NSDictionary dictionaryWithObject:gropuInfo forKey:GroupInfoKey];
					[ClientNetWorkController postNotification:UpdateGroupNotification info:info];
					[gropuInfo release];
					gropuInfo = nil;	
				}
				nodeType = EM_UNKNOW_NODE;
			}
		}
		else if ([elementName compare:@"show"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"show";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
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
		else if ([elementName compare:@"nickname"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"nickname";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
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
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"impresa";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
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
		else if ([elementName compare:@"data"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"portraithashData";
			}
		}
		else if ([elementName compare:@"url"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				[infoDictionary setObject:[NSNumber numberWithBool:1] forKey:@"urlChanged"];
				key = @"portraithashUrl";
			}
		}
		else if ([elementName compare:@"portraithash"] == NSOrderedSame)
		{		
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"portraithash";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
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
		else if ([elementName compare:@"realname"] == NSOrderedSame)
		{
			if (strangerInfo != nil)
			{
				strangerInfo.realname = character;
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
		else if ([elementName compare:@"spaces"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"spaces";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
			{
				if (itemInfo != nil)
				{
					itemInfo.spaces = character;
				}
			}
			else
			{
				MYLog(@"-----sunknown paces-------");
			}	
		}
		else if ([elementName compare:@"phone"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"phone";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
			{
				if (itemInfo != nil)
				{
					itemInfo.phone = character;
				}
			}
			else
			{
				MYLog(@"-----unknown phone-------");
			}
		}
		else if ([elementName compare:@"gbcode"] == NSOrderedSame)
		{
			if (noteType == EM_NOTE_VCARD
				|| noteType == EM_NOTE_PRESENCE_SELF)
			{
				key = @"gbcode";	
			}
			else if (noteType == EM_NOTE_PRESENCE_OTHERS
					 || nodeType == EM_ITEM)
			{
				if (itemInfo != nil)
				{
					itemInfo.gbcode = character;
				}
			}
			else
			{
				MYLog(@"-----unknown gbcode-------");
			}
		}
		else if ([elementName compare:@"error"] == NSOrderedSame)
		{
			self.errorString = character;
		}
		if (key != nil)	
		{
			if (character == nil)
				[infoDictionary removeObjectForKey:key];
			else
				[infoDictionary setObject:character forKey:key];
		}
	}
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
{
	if (string != nil)
	{
		if (character != nil)
		{
			self.character = [character stringByAppendingFormat:string];
		}
		else
		{
			self.character = string;
		}
	}
	else
		self.character = nil;
}

@end
