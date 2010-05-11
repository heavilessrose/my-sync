//
//  LoginResponseParaser.m
//  msn_for_mac
//
//  Created by  apple on 08-9-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "AccountResponseParser.h"
#import "define.h"

@implementation AccountResponseParser

@synthesize character, parserData, resultString, errorString, errorCode, parseError;

- (id)initWithData:(NSData *)theData
{
	self = [super init];
	if (self != nil) 
	{
		errorCode = 0;
		resultString = nil;
		errorString = nil;
		self.parserData = theData;
	}
	return self;
	
}

- (void) dealloc
{
	[parserData release];
	[character release];
	[resultString release];
	[errorString release];
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
		theError = &parseError;
	[parser release];
	return resultString;
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI 
 qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
{
	int index = 0;
	if (elementName != nil)
	{
		if ([elementName compare:@"iq"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"type"];
				if (index != NSNotFound)
				{
					self.resultString = [attributeDict objectForKey:@"type"];	
				}
			}
			else
				MYLog(@"bad response!");
		}
		if ([elementName compare:@"query"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				index = [[attributeDict allKeys] indexOfObject:@"xmlns"];
				if (index != NSNotFound)
				{
					NSString *value = [attributeDict objectForKey:@"xmlns"];
					if ([value compare:@"jabber:iq:auth"] != NSOrderedSame)
						self.resultString = @"BAD";	
				}
				else
					self.resultString = @"BAD";
			}
			else
				self.resultString = @"BAD";
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
	}
	
	self.character = nil;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	if (elementName != nil)
	{
		if ([elementName compare:@"error"] == NSOrderedSame)
		{
			self.errorString = character;
		}	
	}
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
{
	if (character == nil)
		self.character = string;
	else
		self.character = [character stringByAppendingFormat:string];
}

@end
