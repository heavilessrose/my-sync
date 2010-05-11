//
//  PresenceResponseParser.m
//  msn_for_mac
//
//  Created by  apple on 08-9-18.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "PresenceResponseParser.h"
#import "define.h"

@implementation PresenceResponseParser

@synthesize character, parserData, sourceTypeString, resultString, parseError;
- (id)initWithData:(NSData *)theData
{
	self = [super init];
	if (self != nil) 
	{
		character = nil;
		resultString = nil;
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
	[super dealloc];
}

- (NSString*)praseResponseData:(NSError**)theError
{
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
	
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
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];
	return resultString;
}


- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI 
													qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
{
	if (elementName != nil)
	{
		if ([elementName compare:@"presence"] == NSOrderedSame)
		{
			if (attributeDict && [attributeDict count] > 0)
			{
				int index = [[attributeDict allKeys] indexOfObject:@"type"];
				if (index != NSNotFound)
					self.resultString = [attributeDict objectForKey:@"type"];
				
				index = [[attributeDict allKeys] indexOfObject:@"from"];
				if (index != NSNotFound)
				{
					//self.character = [attributeDict objectForKey:@"from"];
					//[[NSUserDefaults standardUserDefaults] setObject:character forKey:@"from"];
				}
			}
			else{
				MYLog(@">>> presence bad response!");
			}
		}
		else{
			MYLog(@">>> presence bad response!");			
			MYLog(@">>> presence data:%@",[[[NSString alloc] initWithData:parserData encoding:NSUTF8StringEncoding] autorelease]);
		}

	}	
	self.character = nil;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	if ([elementName compare:@"presence"] == NSOrderedSame)
	{

	}
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
{
}

@end
