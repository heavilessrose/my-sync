//
//  AdviceResponseParser.m
//  MSN
//
//  Created by  apple on 09-3-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "AdvtResponseParser.h"
#import "ClientNetWorkController.h"
#import "Advertisement.h"
#import "define.h"

NSString		*LoginProgAdNotification = @"LoginProgAd";
NSString		*RosterFooterAdNotification = @"RosterFooterAd";
NSString		*ChatHeaderAdNotification = @"ChatHeaderAd";

@implementation AdvtResponseParser
@synthesize parserData, parseError, character, resultString, advt;
 
- (id)initWithData:(NSData *)theData
{
	self = [super init];
	if (self != nil) 
	{
		nodeType = EM_UNKNOW_ADVT;
		self.parserData = theData;
	}
	return self;
}

- (void) dealloc
{
	[parserData release];
	[character release];
	[resultString release];
	[parseError release];
	[advt release];
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

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI 
 qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
{
	if (elementName != nil)
	{
		int index = NSNotFound;

		if ([elementName compare:@"ad"] == NSOrderedSame)
		{
			self.resultString = nil;
			nodeType = EM_ADVT_AD;			
		}
		else if ([elementName compare:@"item"] == NSOrderedSame)
		{
			if (nodeType == EM_ADVT_AD)
			{
				nodeType = EM_ADVT_ITEM;
				if (attributeDict && [attributeDict count] > 0)
				{
					advt = [[Advertisement alloc] init];
					
					index = [[attributeDict allKeys] indexOfObject:@"id"];
					if (index != NSNotFound)
						advt.adId = [[attributeDict objectForKey:@"id"] intValue];
					
					index = [[attributeDict allKeys] indexOfObject:@"dbid"];
					if (index != NSNotFound)
						advt.dbid = [[attributeDict objectForKey:@"dbid"] intValue];
					
					index = [[attributeDict allKeys] indexOfObject:@"type"];
					if (index != NSNotFound)
						advt.type = [attributeDict objectForKey:@"type"];
					
					index = [[attributeDict allKeys] indexOfObject:@"target"];
					if (index != NSNotFound)
						advt.target = [attributeDict objectForKey:@"target"];
					
					index = [[attributeDict allKeys] indexOfObject:@"action"];
					if (index != NSNotFound)
						advt.action = [attributeDict objectForKey:@"action"];
					
					index = [[attributeDict allKeys] indexOfObject:@"param0"];
					if (index != NSNotFound)
						advt.param = [attributeDict objectForKey:@"param0"];
					
				}
			}
			else{
				MYLog(@">>> Advt bad response!");				
			}

		}
		else if ([elementName compare:@"media"] == NSOrderedSame)
		{
			if (nodeType == EM_ADVT_ITEM)
			{
				nodeType = EM_ADVT_MEDIA;
				if (attributeDict && [attributeDict count] > 0)
				{
					index = [[attributeDict allKeys] indexOfObject:@"src"];
					if (index != NSNotFound)
						advt.imageUrl = [attributeDict objectForKey:@"src"];
					index = [[attributeDict allKeys] indexOfObject:@"alt"];
					if (index != NSNotFound)
						advt.altText = [attributeDict objectForKey:@"alt"];
				}
			}
		}
		else if ([elementName compare:@"text"] == NSOrderedSame)
		{
			if (nodeType == EM_ADVT_ITEM)
			{
				nodeType = EM_ADVT_TEXT;
			}
		}
	}	
	self.character = nil;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	if (elementName != nil)
	{
		if ([elementName compare:@"ad"] == NSOrderedSame)
		{
			nodeType = EM_UNKNOW_ADVT;
		}
		else if ([elementName compare:@"item"] == NSOrderedSame)
		{
			if (advt != nil && advt.type != nil)
			{
				
				NSDictionary *dict = [NSDictionary dictionaryWithObject:advt forKey:@"advt"];
				if ([advt.type compare:@"roster-footer-ad"] == NSOrderedSame)
				{
					[ClientNetWorkController postNotification:RosterFooterAdNotification info:dict];
				}
				else if ([advt.type compare:@"chat-header-ad"] == NSOrderedSame)
				{
					[ClientNetWorkController postNotification:ChatHeaderAdNotification info:dict];
				}
				else if ([advt.type compare:@"login-prog-ad"] == NSOrderedSame)
				{
					[ClientNetWorkController postNotification:LoginProgAdNotification info:dict];
				}
				self.advt = nil;
			}
			nodeType = EM_ADVT_AD;
		}
		else if ([elementName compare:@"text"] == NSOrderedSame)
		{
			if (advt != nil)
			{
				advt.text = character;
			}
			nodeType = EM_ADVT_ITEM;
		}
		else if ([elementName compare:@"media"] == NSOrderedSame)
		{
			nodeType = EM_ADVT_ITEM;
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
