//
//  ZwpResponseParser.m
//  MSN
//
//  Created by  apple on 09-3-20.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ZwpResponseParser.h"
#import "ClientNetWorkController.h"
#import "API.h"
#import "define.h"


#define kAlreadyElement   @"kAlreadyElement"
#define kNeedProcElement  @"kNeedProcElement"

NSString *ZwpDataChangedNotification = @"ZwpDataChanged";
NSString *ZwpTitleChangedNotification = @"ZwpTitleChanged";

NSString *StandbyZwpDataChangedNotification = @"StandbyZwpDataChanged";
NSString *StandbyZwpTitleChangedNotification = @"StandbyZwpTitleChanged";

@implementation ZwpResponseParser
@synthesize parserData, htmlString;
@synthesize requestFrom;

- (id)initWithType:(int)theType
{
	self = [super init];
	if (self != nil) 
	{
		requestFrom = theType;
		parserdLength = 0;
		formId = -1;
		self.htmlString = [NSMutableString stringWithCapacity:0];
		if (htmlString != @"")
		{
			[htmlString deleteCharactersInRange:NSMakeRange(0,[htmlString length])];
		}
		[htmlString appendFormat:@"<html><header><meta name=viewport width=320; content=initial-scale=1.0, user-scalable=yes />\
			 </header><body>"];			

	}
	return self;
}

- (void)setZwpHeaders:(NSDictionary*)theHeaders
{
	LogMethod();
	[htmlString appendFormat:@"</body></html>"];
	NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:htmlString,@"html",
																	theHeaders,@"headers",nil];
//	MYLog(@">>>[whole htmlString]:%@",htmlString);

	if (requestFrom == ZWP_REQUEST){
		[ClientNetWorkController postNotification:ZwpDataChangedNotification info:dict];		
	}else if (requestFrom == STANDBY_ZWP_REQUEST){
		[ClientNetWorkController postNotification:StandbyZwpDataChangedNotification info:dict];		
	}
	self.htmlString = [NSMutableString stringWithCapacity:0];
	[htmlString appendFormat:@"<html><header><meta name=viewport width=320; content=initial-scale=1.0, user-scalable=yes />\
	 </header><body>"];
}
	

- (void) setData:(NSData *)theData
{
	parserdLength = 0;
	if (parserData == nil)
	{
		self.parserData = [NSMutableData dataWithLength:0];
	}
		
	[parserData appendData:theData];
	
	// Just for debug
	/*
	FILE *file = fopen("/Users/dujun/Desktop/test_data","a+");
	fseek(file,0, SEEK_END);
	fwrite([theData bytes], 1, [theData length], file);
	fclose(file);
	 */
}


- (void) dealloc
{
	[parserData release];
	[htmlString release];
	[super dealloc];
}

- (NSString*)praseResponseData:(NSError**)theError
{
	BOOL error = NO;
	while (parserdLength < [parserData length])
	{
		if (![self getTag])
		{
			error = YES;
			self.parserData = [NSMutableData dataWithData:[parserData subdataWithRange:NSMakeRange(parserdLength - 2, [parserData length]-parserdLength + 2)]];
			parserdLength = 0;
			break;
		}
	}
	if (!error)
	{
		self.parserData = [NSMutableData dataWithData:[parserData subdataWithRange:NSMakeRange(parserdLength, [parserData length]-parserdLength)]];
		parserdLength = 0;
	}
	
	NSDictionary *dict = [NSDictionary dictionaryWithObject:htmlString forKey:@"html"];
	if (requestFrom == ZWP_REQUEST){
		[ClientNetWorkController postNotification:ZwpDataChangedNotification info:dict];		
	}else if (requestFrom == STANDBY_ZWP_REQUEST){
		[ClientNetWorkController postNotification:StandbyZwpDataChangedNotification info:dict];		
	}

	return nil;
}

#pragma mark -
#pragma mark utilities

// You should insert your form element directly if this func return a not NSNotFound value;
// otherwise, you need add a form wrapper yourself as follow:
// <form>< your element /></form>
- (NSUInteger)getFormElementLocation:(NSString *)targetString
{
	NSRange lastFormRange = [targetString rangeOfString:@"/form>" options:NSBackwardsSearch];
	if (NSNotFound == lastFormRange.location)
	{
		NSRange formBeginRange = [targetString rangeOfString:@"<form"];
		if (NSNotFound == formBeginRange.location)
		{
			return NSNotFound;
		}else {
			NSRange restRange    = NSMakeRange(formBeginRange.location, [targetString length]-formBeginRange.location);
			NSRange formEndRange = [targetString rangeOfString:@">" options:NSCaseInsensitiveSearch range:restRange];
			return formEndRange.location + formEndRange.length;
		}

	}else {
		NSRange restRange    = NSMakeRange(lastFormRange.location, [targetString length]-lastFormRange.location);
		NSRange formBeginRange = [targetString rangeOfString:@"<form" options:NSCaseInsensitiveSearch range:restRange];
		
		if (NSNotFound == formBeginRange.location)
		{
			return NSNotFound;
		}else {
			restRange.location = formBeginRange.location;
			restRange.length   = [targetString length] - formBeginRange.location;
			NSRange formEndRange = [targetString rangeOfString:@">" options:NSCaseInsensitiveSearch range:restRange];
			return formEndRange.location + formEndRange.length;
		}

	}
}

// Add element string into form.
- (void)addElementIntoForm:(NSString *)elementString originalString:(NSMutableString *)originalString
{
	NSUInteger resultIndex = [self getFormElementLocation:originalString];
	if (NSNotFound == resultIndex)
	{
		[originalString appendFormat:@"<form>%@",elementString];
	}else {
		[originalString insertString:elementString atIndex:resultIndex];
	}
}

// Convert submit_data data into dictionary.
- (NSDictionary *)convertSubmitdataToDict:(NSString *) submitString
{
	NSMutableDictionary *restDict = [NSMutableDictionary dictionaryWithCapacity:2];// [NSDictionary dictionary];
	NSMutableDictionary *alreadyDict = [NSMutableDictionary dictionaryWithCapacity:0];
	NSMutableDictionary *needProcDict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	@try {
		NSArray *keyValues = [submitString componentsSeparatedByString:@"&"];
		NSString *keyValueString = nil;
		for (keyValueString in keyValues)
		{
			NSArray *keyValue = [keyValueString componentsSeparatedByString:@"="];
			if (NSNotFound == [keyValueString rangeOfString:@"$"].location)
			{
				// Fill up Dict with key-values
				[alreadyDict setObject:[keyValue objectAtIndex:1] forKey:[keyValue objectAtIndex:0]];
			}else {
				NSString *tmpObj = [keyValue objectAtIndex:1];
				tmpObj = [tmpObj stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"$()"]];
				[needProcDict setObject:tmpObj forKey:[keyValue objectAtIndex:0]];
			}
		}
		[restDict setObject:alreadyDict forKey:kAlreadyElement];
		[restDict setObject:needProcDict forKey:kNeedProcElement];
	}
	@catch (NSException * e) {
		;
	}	
	return restDict;
}

- (int)read16
{
	UInt16 tag = 0;
	memcpy(&tag,[parserData mutableBytes]+parserdLength,2);
	parserdLength += 2;
	return char16(tag);
}

- (int)read32
{
	UInt32 tag = 0;
	memcpy(&tag,[parserData mutableBytes]+parserdLength,4);
	parserdLength += 4;
	return char32(tag);
}

- (BOOL)getTag
{
	BOOL ret = NO;
	UInt16 tag = [self read16];

	NSString *tagId = nil;
	switch (tag)
	{
		case 0X1000:
			tagId = @"PageStyle";
			break;
		case 0X1001:
			tagId = @"FontStyle";
			break;
		case 0X1002:
			tagId = @"ColorStyle";
			break;
		case 0X1003:			
			tagId = @"AlignmentStyle";
			break;
		case 0X1004:			
			tagId = @"SizeStyle";
			break;
		case 0X1005:							
			tagId = @"OffsetStyle";
			break;
		case 0X2000:
			tagId = @"ResData";
			break;
		case 0X2001:
			tagId = @"ResponseEnd";
			break;
		case 0X3000:
			tagId = @"Title";
			break;		
		case 0X3001:
			tagId = @"BR";
			break;			
		case 0X3002:
			tagId = @"HR";
			break;			
		case 0X3003:
			tagId = @"Marquee";
			break;			
		case 0X3004:
			tagId = @"Text";
			break;	
		case 0X3005:
			tagId = @"Link";
			break;			
		case 0X3006:
			tagId = @"LocalAnchor";
			break;			
		case 0X3007:
			tagId = @"Image";
			break;			
		case 0X3008:
			tagId = @"AutoRefresh";
			break;			
		case 0X3009:
			tagId = @"File";
			break;
		case 0X4000:
			tagId = @"TextBox";
			break;
		case 0X4001:
			tagId = @"SingleSelection";
			break;
		case 0X4002:
			tagId = @"MultiSelection";
			break;
		case 0X4003:
			tagId = @"Hidden";
			break;
		case 0X4004:
			tagId = @"FileBrowser";
			break;
		case 0X4005:
			tagId = @"Record";
			break;
		case 0X4006:
			tagId = @"Camera";
			break;
		case 0X4007:
			tagId = @"ValueResetButton";
			break;
		case 0X4008:
			tagId = @"ResetButton";
			break;
		case 0X4009:
			tagId = @"SubmitButton";
			break;
		case 0X400A:
			tagId = @"Checkbox";
			break;
		case 0X5000:
			tagId = @"ExtCommand";
			break;
		default:
			break;
	}
	if (tagId != nil)
	{
		NSString *selectorString = [NSString stringWithFormat:@"get%@String",tagId];
		@try {
			ret = [[self performSelector:NSSelectorFromString(selectorString)] boolValue];
		}
		@catch (NSException * e) {
			MYLog(@">>> [Except!]");
			ret = YES;
		}
	}
	return ret;
}

#pragma mark -
#pragma mark tag handlers

- (NSNumber*)getPageStyleString
{
	LogMethod();
	BOOL ret = NO;
	int foreColor = -1;
	int backColor = -1;
	int linkColor = -1;
	int readLinkColor = -1;
	int leftMargin = -1;
	int rightMargin = -1;
	int alignMode = -1;
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int fontSize = 0;
			memcpy(&fontSize,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			foreColor = [self read32];
			backColor = [self read32];
			linkColor = [self read32];
			readLinkColor = [self read32];		
			
			memcpy(&leftMargin,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			memcpy(&rightMargin,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			memcpy(&alignMode,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	
	MYLog(@">>> PAGE STYLE: foreColor:%d,backColor:%d,linkColor:%d,readLinkColor:%d,leftMargin:%d,rightMargin:%d,alignMode:%d,",\
							foreColor,backColor,linkColor,readLinkColor,leftMargin,rightMargin,alignMode);
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getFontStyleString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int fontSize = 0;
			memcpy(&fontSize,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int fontStyle = 0;
			memcpy(&fontStyle,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];	
}

- (NSNumber*)getColorStyleString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
//			int foregrndColor = [self read32];			
//			int backgrndColor = [self read32];

			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getAlignmentStyleString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int leftMargin = 0;
			memcpy(&leftMargin,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int rightMargin = 0;
			memcpy(&rightMargin,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int alignMode = 0;
			memcpy(&alignMode,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getSizeStyleString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
//			int width = [self read16];
//			int height = [self read16];
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getOffsetStyleString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
//			int offsetX = [self read16];
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getResDataString
{
//	LogMethod();
	BOOL ret = NO;
	NSString *name = nil;
	NSString *format = nil;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
//			int dataId = [self read32];
//			MYLog(@"dataId:%d",dataId);
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				format = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (format != nil)
				{
//					MYLog(@"format>>>%@",format);
				}
			}
			int dataLen = [self read32];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, dataLen)];
			parserdLength += dataLen;
			if (name != nil && format != nil && data != nil)
			{
				if ([format compare:@"png"] == NSOrderedSame
					|| [format compare:@"bmp"] == NSOrderedSame
					|| [format compare:@"gif"] == NSOrderedSame
					|| [format compare:@"jpg"] == NSOrderedSame)
				{
					NSString *filePath = [NSTemporaryDirectory() stringByAppendingPathComponent:name];
					[data writeToFile:filePath atomically:YES];
//					MYLog(@"sData>>>%@",filePath);
				}
			}
			[name release];
			[format release];

			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getResponseEndString
{
	LogMethod();
	BOOL ret = NO;
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getTitleString
{
//	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *title = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (title != nil)
				{
					NSDictionary *dict = [NSDictionary dictionaryWithObject:title forKey:@"title"];
					if (requestFrom == ZWP_REQUEST){
						[ClientNetWorkController postNotification:ZwpTitleChangedNotification info:dict];
					}else if (requestFrom == STANDBY_ZWP_REQUEST){
						[ClientNetWorkController postNotification:StandbyZwpTitleChangedNotification info:dict];		
					}
				}
				[title release];
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}

	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getBRString
{
//	LogMethod();
 	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		ret = YES;
		[self read32];
		[htmlString appendString:@"<br/>"];
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getHRString
{
//	LogMethod();
	BOOL ret = NO;
	int width = 0;
	int style = 0;
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			memcpy(&width,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;

			memcpy(&style,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	if (style !=0)
	{
		[self.htmlString appendFormat:@"<hr/>",width];//width='%d' style='border:1px dashed ; height:1px'
	}else {
		[self.htmlString appendFormat:@"<hr/>",width];
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getMarqueeString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *text = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (text != nil)
				{
				}
				[text release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *href = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (href != nil)
				{

				}
				[href release];
			}
			
			int direction = 0;
			memcpy(&direction,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			int scroll_pixes = 0;
			memcpy(&scroll_pixes,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
//			int scroll_interval = [self read16];
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];	
}

- (NSNumber*)getTextString
{
//	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *text = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (text != nil)
				{
					ret = YES;
					[htmlString appendFormat:@"%@",text];
					//MYLog(@">>>text:%@",htmlString);
				}
				[text release];
			}
		}
		else
		{
			parserdLength -= 4;
		}
	}
	
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getLinkString
{
//	LogMethod();
	BOOL ret = NO;
	NSString *text = nil;
	NSString *href = nil;
	NSString *advisit = nil;
	int target_type = 0;
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				text = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (text != nil)
				{
					//MYLog(@">>>link text: %@",text);
				}
			}
			memcpy(&target_type,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				href = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (href != nil)
				{
//					MYLog(@"link href: %@",href);
				}
			}
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				advisit = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (advisit != nil)
				{
//					MYLog(@">>>advisit: %@",advisit);
				}
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	if (href == nil)
	{
		if (text != nil)
			[htmlString appendFormat:@"<a>%@</a>",text];
	}
	else
	{
		if (text == nil)
		{
			if (advisit != nil && [advisit length] > 0)
				[htmlString appendFormat:@" <a href=\"link***%d***%@***%@\"/> ",target_type,advisit,href];
			else
				[htmlString appendFormat:@" <a href='%@'/> ",href];
				
		}
		else
		{
			if (advisit != nil && [advisit length] > 0)
				[htmlString appendFormat:@" <a href=\"link***%d***%@***%@\">%@</a> ",target_type,advisit,href,text];	
			else
				[htmlString appendFormat:@" <a href='%@'>%@</a> ",href,text];			
		}
	}

	if (text)
		[text release];
	if (href)
		[href release];
	if (advisit)
		[advisit release];


	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getLocalAnchorString 
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
					ret = YES;
					//MYLog(@">>>title:%@",htmlString);
				}
				[name release];
			}
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getImageString
{
//	LogMethod();
	BOOL ret = NO;
	int resdata_id = 0xFFFF;
	int width = 0;
	int height = 0;
	int target_type = 0;
	NSString *text = nil;
	NSString *href = nil;
	NSString *href_original_img = nil;
	NSString *advisit = nil;
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength < [parserData length])
		{
			len =  [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				text = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (text != nil)
				{
//					MYLog(@">>> text: %@",text);
				}
			}
			resdata_id = [self read32];
			width = [self read16];
			height = [self read16];
//			MYLog(@">>> resdata_id=%d, width=%d, height=%d",resdata_id,width,height);
			
			len =  [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				href_original_img = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (href_original_img != nil)
				{
					if (resdata_id != 0xFFFF)
					{
						NSString *path = [href_original_img lastPathComponent];
						if (path != nil)
						{
							[path retain];
							[href_original_img release];
							href_original_img = path;
						}
					}
				}
			}
			len =  [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				href = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (href != nil)
				{
				}
			}

			memcpy(&target_type,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			len =  [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				advisit = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (advisit != nil)
				{
					//MYLog(@">>> advisit:%@",advisit);
				}
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}

	if (nil == text) 
	{
		text = @"";		
	}

	if (0 == width ||0 == height)
	{
		if (advisit != nil && [advisit length] > 0)
		{
			[self.htmlString appendFormat:@"<a href=\"link***%d***%@***%@\"><img src='%@' alt='%@'></a>",\
			 target_type,advisit,href,href_original_img,text,resdata_id];
		}
		else
		{
			[self.htmlString appendFormat:@"<a href='%@'><img src='%@' alt='%@'></a>",\
			 href,href_original_img,text,resdata_id];
		}
	}
	else 
	{
		if (advisit != nil && [advisit length] > 0)
		{
			[self.htmlString appendFormat:@"<a href=\"link***%d***%@***%@\"><img src='%@' alt='%@' width='%d' height='%d'></a>",\
			 target_type,advisit,href,href_original_img,text,width,height,resdata_id];
		}
		else
		{
			[self.htmlString appendFormat:@"<a href='%@'><img src='%@' alt='%@' width='%d' height='%d'></a>",\
			 href,href_original_img,text,width,height,resdata_id];
		}
	}


	if (text)
		[text release];
	if (href)
		[href release];
	if (href_original_img)
		[href_original_img release];
	if (advisit)
		[advisit release];
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getAutoRefreshString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *href = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (href != nil)
				{
				}
				[href release];
			}
//			int timeout = [self read16];
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getFileString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *url = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (url != nil)
				{
					MYLog(@"url>>>%@",url);
				}
				[url release];
			}
//			int fileLength = [self read16];
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *contentType = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (contentType != nil)
				{
					MYLog(@"contentType>>>%@",contentType);
				}
				[contentType release];
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];	
}

- (NSNumber*)getTextBoxString
{
//	LogMethod();
	BOOL ret = NO;
	int form_id = 0;
	NSString *name = nil;
	NSString *value = nil;
	int password_mask = 0;
	int lines = 0;
	int size = 0;

	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{

			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
			}
			
			memcpy(&password_mask,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			memcpy(&lines,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			[self read16];  //??????????????????
			size = [self read16];
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	
//	MYLog(@">>> TextBox: form_id:%d,name:%@,value:%@,password_mask:%d,lines:%d,size:%d",form_id,name,value,password_mask,lines,size);
	
	NSString *iType = nil;
	if( 35 == password_mask || 42 == password_mask) // ord('#') == 35, ord('*')==42
	{
		iType = kPasswordKey;
	}else {
		iType = @"text";
	}

	NSMutableString *textBoxString = [NSMutableString stringWithCapacity:0];
	if (lines>1) // Should be textarea.
	{
		[textBoxString appendFormat:@"<textarea type='%@' name='%@' value='%@' size='%d' rows='%@' />",iType,name,value,size,lines];			
	}else {
		[textBoxString appendFormat:@"<input type='%@' name='%@' value='%@' />",iType,name,value];//size='%d'
	}

	[self addElementIntoForm:textBoxString originalString:self.htmlString];
	
	[name release];
	[value release];
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getSingleSelectionString
{
//	LogMethod();
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	BOOL ret = NO;
	int form_id = 0;
	NSString *name = nil;
	NSString *value = nil;
	int type = 0;
	int flag = 0;
	int items = 0;
	NSString *itemText = nil;
	NSString *itemValue = nil;
	
	NSMutableDictionary *itemsDict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
			}
			
			memcpy(&type,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			

			memcpy(&flag,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			memcpy(&items,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int i = 0;
			for (i = 0; i < items; i++)
			{
				len = [self read16];
				NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
				parserdLength += len;
				if (data != nil)
				{
					itemText = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
					if (itemText != nil)
					{
//						MYLog(@"itemText>>>%@",itemText);
					}

				}
				
				len = [self read16];
				data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
				parserdLength += len;
				if (data != nil)
				{
					itemValue = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
					if (itemValue != nil)
					{
//						MYLog(@"itemValue>>>%@",itemValue);
					}
				}
				
				[itemsDict setObject:itemText forKey:itemValue];
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	
	NSMutableString *selectString = [NSMutableString stringWithCapacity:0];
	{
		[selectString appendFormat:@"<select name='%@' single>",name];
		id item = nil;
		for (item in [itemsDict allKeys])
		{
			if (item == value)
			{
				[selectString appendFormat:@"<option value='%@' selected>%@</option>",item,[itemsDict objectForKey:item]];
			}else {
				[selectString appendFormat:@"<option value='%@'>%@</option>",item,[itemsDict objectForKey:item]];
			}
			
		}
		[selectString appendString:@"</select>"];
//		
	}

	[self addElementIntoForm:selectString originalString:self.htmlString];
	
	[name release];
	[value release];
	[itemText release];
	[itemValue release];
	[pool release];
	
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getMultiSelectionString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
				[name release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
				[value release];
			}
			
			int flag = 0;
			memcpy(&flag,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int items = 0;
			memcpy(&items,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int i = 0;
			NSString *itemText = nil;
			NSString *itemValue = nil;
			for (i = 0; i < items; i++)
			{
				len = [self read16];
				NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
				parserdLength += len;
				if (data != nil)
				{
					itemText = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
					if (itemText != nil)
					{
//						MYLog(@"itemText>>>%@",itemText);
					}
					[itemText release];
				}
				
				len = [self read16];
				data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
				parserdLength += len;
				if (data != nil)
				{
					itemValue = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
					if (itemValue != nil)
					{
//						MYLog(@"itemValue>>>%@",itemValue);
					}
					[itemValue release];
				}
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getHiddenString
{
//	LogMethod();
	BOOL ret = NO;
	NSString *name = nil;
	NSString *value = nil;
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
				}
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
				}
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	
	NSRange indexRange   = [self.htmlString rangeOfString:@"<form" options:NSBackwardsSearch];//location = 0xaf, length=0x5
	NSRange restRange    = NSMakeRange(indexRange.location, [self.htmlString length]-indexRange.location);// 0xaf,0x8f
	NSRange formEndRange = [self.htmlString rangeOfString:@">" options:NSCaseInsensitiveSearch range:restRange];// 0xe3,0x1
	[self.htmlString deleteCharactersInRange:formEndRange];
	[self.htmlString insertString:[NSString stringWithFormat:@"><input type='hidden' name='%@' value='%@'/>",name,value]
						  atIndex:formEndRange.location];
	
	[name release];
	[value release];
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getFileBrowserString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
				[name release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
				[value release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *caption = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (caption != nil)
				{
//					MYLog(@"caption>>>%@",caption);
				}
				[caption release];
			}
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getRecordString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
				[name release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
				[value release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *caption = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (caption != nil)
				{
//					MYLog(@"caption>>>%@",caption);
				}
				[caption release];
			}
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getCameraString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
				[name release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
				[value release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *caption = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (caption != nil)
				{
//					MYLog(@"caption>>>%@",caption);
				}
				[caption release];
			}
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getValueResetButtonString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *caption = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (caption != nil)
				{
//					MYLog(@"value>>>%@",caption);
				}
				[caption release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name_to_reset = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name_to_reset != nil)
				{
//					MYLog(@"name_to_reset>>>%@",name_to_reset);
				}
				[name_to_reset release];
			}
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getResetButtonString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *caption = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (caption != nil)
				{
//					MYLog(@"caption>>>%@",caption);
				}
				[caption release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
				[name release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
				[value release];
			}
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getSubmitButtonString
{
//	LogMethod();
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	BOOL ret = NO;
	int form_id = 0;
	NSString *caption = nil;
	NSMutableString *href = nil;
	int method = 0;
	NSString *name = nil;
	NSString *value = nil;
	int type = 0;
	NSString *submit_data = nil;
	NSDictionary *submitDict = nil;
	NSDictionary *needProcDict = nil;
	
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{

			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				caption = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (caption != nil)
				{
//					MYLog(@"caption>>>%@",caption);
				}
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				href = [[NSMutableString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (href != nil)
				{
//					MYLog(@"href>>>%@",href);
				}
			}
			
			memcpy(&method,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
			}
			
			memcpy(&type,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				submit_data = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (submit_data != nil)
				{
//					MYLog(@"submit_data>>>%@",submit_data);
				}
			}
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	
	MYLog(@">>> submit button content:\n>>> form_id:%d,caption:%@,href:%@,method:%d,name:%@,value:%@,type:%d,submit_data:%@",\
		  form_id,caption,href,method,name,value,type,submit_data);

	NSMutableString *formElementFromSubmitData = [[NSMutableString alloc] init];
	if (submit_data != nil)
	{
		MYLog(@">>>[submit_data]:%@",submit_data);
		NSMutableDictionary *originalDict = [[self convertSubmitdataToDict:submit_data] retain];
		submitDict = [originalDict objectForKey:kAlreadyElement];
		needProcDict = [originalDict objectForKey:kNeedProcElement];
		[originalDict release];
		
		for (NSString *key in [submitDict allKeys])
		{
			// Convert the "?a=1&b=2" like data to hidden input element.
			[formElementFromSubmitData appendFormat:@"<input name='%@' type='hidden' value='%@' />",key,[submitDict objectForKey:key]];
		}
		
		// Processing the "item=$xxxx" type data. replace input element named "xxxx" with "item".
		if ([needProcDict count]>0)
		{
			for (NSString *keyString in [needProcDict allKeys])
			{
				// Find & replace the "xxxx" content with "item" content.
				NSRange theRange = [self.htmlString rangeOfString:[needProcDict objectForKey:keyString] options:NSBackwardsSearch];
				if (theRange.location != NSNotFound)
				{
					[self.htmlString deleteCharactersInRange:theRange];
					[self.htmlString insertString:keyString atIndex:theRange.location];					
				}
			}
		}
	}

	NSString *iMethod = nil;
	switch (method) {
		case 0:
		default:
			iMethod = @"GET";	
			// Parsing href and fill up input tags.
			NSRange indexRange = [href rangeOfString:@"?"];
			if (indexRange.location != NSNotFound)
			{
				NSString *tagsString = [href substringFromIndex:indexRange.location + 1];
				[href deleteCharactersInRange:NSMakeRange(indexRange.location, [href length]-indexRange.location)];
				NSDictionary *testDict = [[self convertSubmitdataToDict:tagsString] retain];
				NSDictionary *alreadyDict  = [testDict objectForKey:kAlreadyElement];

				for (NSString *key in [alreadyDict allKeys])
				{
					// Convert the "?a=1&b=2" like data to hidden input element.
					[formElementFromSubmitData appendFormat:@"<input name='%@' type='hidden' value='%@' />",key,[alreadyDict objectForKey:key]];
				}
				
				[testDict release];
				testDict = nil;				
			}
			break;
		case 1:
			iMethod = @"POST";
			break;
		case 2:
			iMethod = @"PUT";
			break;
	}
	
	NSRange indexRange = [self.htmlString rangeOfString:@"<form" options:NSBackwardsSearch];
	[self.htmlString deleteCharactersInRange:indexRange];

	[self.htmlString insertString:[NSString stringWithFormat:@"<form action='%@' method='%@'",href,iMethod]
						  atIndex:indexRange.location];		
	
	[self.htmlString appendFormat:@"%@<input type='submit' value='%@' /></form>",formElementFromSubmitData,caption];
	
	[formElementFromSubmitData release];
	[caption release];
	[href release];
	[name release];
	[value release];
	[submit_data release];
	submit_data = nil;

	[pool release];
	
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getCheckboxString
{
//	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int form_id = 0;
			memcpy(&form_id,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			len = [self read16];
			NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (name != nil)
				{
//					MYLog(@"name>>>%@",name);
				}
				[name release];
			}
			
			len = [self read16];
			data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
			parserdLength += len;
			if (data != nil)
			{
				NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
				if (value != nil)
				{
//					MYLog(@"value>>>%@",value);
				}
				[value release];
			}
			int checked = 0;
			memcpy(&checked,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

- (NSNumber*)getExtCommandString
{
	LogMethod();
	BOOL ret = NO;
	if (parserdLength + 4 < [parserData length])
	{
		int len = [self read32];
		if (len + parserdLength <= [parserData length])
		{
			int count = 0;
			memcpy(&count,[parserData mutableBytes]+parserdLength,1);
			parserdLength += 1;
			
			int i = 0;
			for (; i < count; i++)
			{
				int type = 0;
				memcpy(&type,[parserData mutableBytes]+parserdLength,1);
				parserdLength += 1;
				
				len = [self read16];
				NSData *data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
				parserdLength += len;
				if (data != nil)
				{
					NSString *name = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
					if (name != nil)
					{
//						MYLog(@"name>>>%@",name);
					}
					[name release];
				}
				
				len = [self read16];
				data = [parserData subdataWithRange:NSMakeRange(parserdLength, len)];
				parserdLength += len;
				if (data != nil)
				{
					NSString *value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];		
					if (value != nil)
					{
//						MYLog(@"value>>>%@",value);
					}
					[value release];
				}
			}
			
			ret = YES;
		}
		else
		{
			parserdLength -= 4;
		}
	}
	return [NSNumber numberWithBool:ret];
}

@end