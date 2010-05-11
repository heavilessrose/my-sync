//
//  ClientResponseParser.m
//  msn_for_mac
//
//  Created by  apple on 08-9-17.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ClientResponseParser.h"
#import "ClientNetWorkController.h"
#import "define.h"


NSString *GetSystemInfoNotification = @"GetSystemInfo";
NSString *GetUpgradeInfoNotification= @"GetUpgradeInfo";

@implementation ClientResponseParser

@synthesize parserData, character, upgradeProperties, parseError;
@synthesize configProperties;
@synthesize loginProperties;
@synthesize transparentProperties;
@synthesize systemProperties;

- (id)initWithData:(NSData *)theData
{
	self = [super init];
	if (self != nil) 
	{
		nodeType = EM_UNKNOWN;
		systemProperties = nil;
		upgradeProperties = nil;
		loginProperties = nil;
		configProperties = nil;
		transparentProperties = nil;
		self.parserData = theData;
	}
	return self;
}

- (void) dealloc
{
//	MYLog(@"-----------------%@ dealloc----------------", self);
	[systemProperties release];
	[upgradeProperties release];
	[configProperties release];
	[loginProperties release];
	[transparentProperties release];
	[parserData release];
	[character release];
	[parseError release];
	[super dealloc];
}


- (NSInteger)praseResponseData:(NSError**)theError
{
	resultType = -1;
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
	return resultType;
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI 
										qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
{
	if (elementName != nil)
	{
		if ([elementName compare:@"sso"] == NSOrderedSame)
		{
			nodeType = EM_SSO;
			if (attributeDict && [attributeDict count] > 0)
			{
				NSString *key = [[attributeDict allKeys] objectAtIndex:0];
				resultType = [[attributeDict objectForKey:key] intValue];
			}
		}
		else if ([elementName compare:@"system"] == NSOrderedSame)
		{
			nodeType = EM_SYSTEM;
		}
		else if ([elementName compare:@"upgrade"] == NSOrderedSame)
		{
			nodeType = EM_UPGRADE;
		}	
		else if ([elementName compare:@"login"] == NSOrderedSame)
		{
			nodeType = EM_LOGIN;
		}
		else if ([elementName compare:@"config"] == NSOrderedSame)
		{
			nodeType = EM_CONFIG;
		}
		else if ([elementName compare:@"transparent"] == NSOrderedSame)
		{
			nodeType = EM_TRANSPARENT;
		}		
	}
	self.character = nil;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{   
	if (elementName != nil)
	{
		switch (nodeType)
		{
			case EM_SSO:
				if ([elementName compare:@"notice"] == NSOrderedSame)
				{
					if (systemProperties == nil){
						systemProperties = [[NSMutableArray alloc] init];
					}
					[systemProperties addObject:character];	
				}
				else if ([elementName isEqualToString: @"url"]){
					NSMutableDictionary* aDict = [[NSMutableDictionary alloc] initWithObjectsAndKeys: character, elementName, nil];
					if (systemProperties == nil){
						systemProperties = [[NSMutableArray alloc] init];
					}
					[systemProperties addObject: aDict];
					[aDict release];
				}
				break;
			case EM_SYSTEM:
				if ([elementName compare:@"system"] == NSOrderedSame){
					nodeType = EM_UNKNOWN;
				}
				else if ([elementName compare:@"notice"] == NSOrderedSame)
				{
					if (systemProperties == nil){
						systemProperties = [[NSMutableArray alloc] init];
					}
					[systemProperties addObject:character];	
				}
				break;
				
			case EM_UPGRADE:
				if ([elementName compare:@"upgrade"] == NSOrderedSame){
					nodeType = EM_UNKNOWN;
				}
				else
				{
					if (upgradeProperties == nil){
						upgradeProperties = [[NSMutableDictionary alloc] init];
					}
					if (character != nil)
						[upgradeProperties setObject:character forKey:elementName];	
					if (systemProperties == nil)
						systemProperties = [[NSMutableArray alloc] init];
					if (character != nil && [elementName isEqualToString: @"notice"])
						[systemProperties addObject: character];
				}
				break;
				
			case EM_LOGIN:
				if ([elementName compare:@"login"] == NSOrderedSame)
				{
					if (loginProperties != nil){
						[[NSUserDefaults standardUserDefaults] setObject:loginProperties forKey:@"login"];
					}
					nodeType = EM_UNKNOWN;
				}
				else
				{
					if (loginProperties == nil){
						loginProperties = [[NSMutableDictionary alloc] init];
					}
					if (character != nil)
						[loginProperties setObject:character forKey:elementName];
				}
				break;
				
			case EM_CONFIG:
				if ([elementName compare:@"config"] == NSOrderedSame)
				{
					if (configProperties != nil)
					{
						// Added for 5.0
						[configProperties removeObjectForKey:@"second_zwp"];
						[[NSUserDefaults standardUserDefaults] setObject:configProperties forKey:@"config"];
						[[NSUserDefaults standardUserDefaults] synchronize];
					}
					nodeType = EM_UNKNOWN;
				}
				else
				{
					if (configProperties == nil)
					{
						configProperties = [[NSMutableDictionary alloc] init];
					}
					if (character != nil)
					{
						[configProperties setObject:character forKey:elementName];	
					}
				}
				break;	
			case EM_TRANSPARENT:
				if ([elementName compare:@"transparent"] == NSOrderedSame)
				{
					if (transparentProperties != nil)
					{
						[[NSUserDefaults standardUserDefaults] setObject:transparentProperties forKey:@"transparent"];
					}
					nodeType = EM_UNKNOWN;
				}else{
					if (transparentProperties == nil)
					{
						transparentProperties = [[NSMutableDictionary alloc] init];
					}
					if (character != nil)
					{
						[transparentProperties setObject:character forKey:elementName];
					}
				}
				break;
			default:
				break;
		}
		
	}
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
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

@end
