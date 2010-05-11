//
//  API.m
//  MSN
//
//  Created by  apple on 08-10-30.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "API.h"
#import "define.h"

#define ESC_LEFT_BRACKET	(@"&lt;")
#define ESC_RIGHT_BRACKET	(@"&gt;")
#define ESC_SPACE			(@"&nbsp;")
#define ESC_SYMBAL_AND		(@"&amp;")
#define ESC_SINGLE_QUOTE	(@"&apos;")
#define ESC_QUOTES			(@"&quot;")


NSComparisonResult compareStrings(NSString *string1, NSString *string2)
{
	NSComparisonResult ret = -1;
	if (string1 != nil)
	{
		if (string2 != nil)
		{
			ret = [string1 compare:string2];
		}
		else
		{
			ret = 1;
		}
	}
	else
	{
		if (string2 == nil)
		{
			ret = 0;
		}
	}
	
	return ret;
}

BOOL isEmail(NSString* theString)
{
	BOOL ret = NO;
	if (theString != nil
		&& [theString length] > 0)
	{
		NSArray *array = [theString componentsSeparatedByString:@"@"];
		if ([array count] == 2)
		{
			int i = [theString rangeOfString:@"@"].location;
			int j = [theString rangeOfString:@"." options:NSBackwardsSearch].location;
			if (j != NSNotFound && j > i)
				ret = YES;
		}
	}
	return ret;
}

BOOL isValidEmailCharacter(NSString* theCharacters)
{
	BOOL ret = NO;
	NSRange wordRange = NSMakeRange('a', 26);
	NSMutableCharacterSet *separatorSet = [NSMutableCharacterSet characterSetWithRange:wordRange];
	wordRange = NSMakeRange('A', 26);
	[separatorSet formUnionWithCharacterSet:[NSCharacterSet characterSetWithRange:wordRange]];
	wordRange = NSMakeRange('0', 10);
	[separatorSet formUnionWithCharacterSet:[NSCharacterSet characterSetWithRange:wordRange]];
	[separatorSet addCharactersInString:@"._-@"];
	
	NSCharacterSet *tokenSet = [separatorSet invertedSet];
	if ([theCharacters rangeOfCharacterFromSet:tokenSet].location == NSNotFound)
		ret = YES;

	return ret;
}

NSString *getStatusString(NSString *theString,BOOL displayOffline)
{
	NSString *status = nil;
	if (theString == nil)
	{
		if (displayOffline)
			status = NSLocalizedString(@"qtn_msn_group_offline",nil);
		else
			status = NSLocalizedString(@"qtn_status_appearoffline",nil);
	}
	else if ([theString compare:@"online"] == NSOrderedSame)
	{
		status = NSLocalizedString(@"qtn_status_online",nil);
	}
	else if ([theString compare:@"busy"] == NSOrderedSame)
	{
		status = NSLocalizedString(@"qtn_status_busy",nil);
	}
	else if ([theString compare:@"berightback"] == NSOrderedSame)
	{
		status = NSLocalizedString(@"qtn_status_berightback",nil);
	}
	else if ([theString compare: @"away"] == NSOrderedSame)
	{
		status = NSLocalizedString(@"qtn_status_away",nil);
	}
	else if ([theString compare:@"onthephone"] == NSOrderedSame)
	{
		status = NSLocalizedString(@"qtn_status_onthephone",nil);
	}
	else if ([theString compare:@"outtolunch"] == NSOrderedSame)
	{
		status = NSLocalizedString(@"qtn_status_outtolunch",nil);
	}
	else if ([theString compare:@"offline"] == NSOrderedSame)
	{
		if (displayOffline)
			status = NSLocalizedString(@"qtn_msn_group_offline",nil);
		else
			status = NSLocalizedString(@"qtn_status_appearoffline",nil);
	}
	return status;
}

NSString *getStatus(NSString *theString)
{
	NSString *status = nil;
	if ([theString compare:NSLocalizedString(@"qtn_status_online",nil)] == NSOrderedSame)
	{
		status = @"online";
	}
	else if ([theString compare:NSLocalizedString(@"qtn_status_busy",nil)] == NSOrderedSame)
	{
		status = @"busy";
	}
	else if ([theString compare:NSLocalizedString(@"qtn_status_berightback",nil)] == NSOrderedSame)
	{
		status = @"berightback";
	}
	else if ([theString compare:NSLocalizedString(@"qtn_status_away",nil)] == NSOrderedSame)
	{
		status = @"away";
	}
	else if ([theString compare:NSLocalizedString(@"qtn_status_onthephone",nil)] == NSOrderedSame)
	{
		status = @"onthephone";
	}
	else if ([theString compare:NSLocalizedString(@"qtn_status_outtolunch",nil)] == NSOrderedSame)
	{
		status = @"outtolunch";
	}
	else if ([theString compare:NSLocalizedString(@"qtn_status_appearoffline",nil)] == NSOrderedSame)
	{
		status = @"offline";
	}
	return status;
}

UIImage* makeImage(UIImage* backgroundImage, UIImage*subImage, CGRect subImageRect)
{
	CGRect rect = CGRectMake(0, 0, backgroundImage.size.width, backgroundImage.size.height);

	UIImage *uiImage = nil;
	if (subImage != nil
		&& CGRectContainsRect(rect, subImageRect))
	{ 
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		CGContextRef context = CGBitmapContextCreate(NULL, backgroundImage.size.width, backgroundImage.size.height, 8, 
													 backgroundImage.size.width*4, colorSpace, 
													 kCGImageAlphaPremultipliedLast);
		CGColorSpaceRelease(colorSpace);
		if (context!= nil) 
		{
			CGContextTranslateCTM(context, 0, backgroundImage.size.height);
			CGContextScaleCTM(context, 1.0, -1.0);
			UIGraphicsPushContext(context);
			CGContextSetAlpha(context,1.0);

			[backgroundImage drawInRect:rect blendMode:kCGBlendModeNormal alpha:1.0];
			[subImage drawInRect:subImageRect blendMode:kCGBlendModeNormal alpha:0.7];

			CGImageRef cgImage = CGBitmapContextCreateImage(context);  
			uiImage = [[UIImage alloc] initWithCGImage:cgImage];
			UIGraphicsPopContext();
			CGContextRelease(context);
			CGImageRelease(cgImage);
		}
	}
	if (uiImage == nil)
	{
		[backgroundImage retain];
		uiImage = backgroundImage;
	}
	//UIImageWriteToSavedPhotosAlbum(uiImage, nil, nil, nil);
	return uiImage;
}

//NSData *jpegData = UIImageJPEGRepresentation(newImage, 0.5);
//[jpegData writeToFile:imagePath atomically:NO];

UIImage* makeGreyImage(UIImage*theImage)
{
	UIImage *uiImage = nil;
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
	int nu = CGColorSpaceGetNumberOfComponents(colorSpace);
	CGContextRef context = CGBitmapContextCreate(NULL, theImage.size.width, theImage.size.height, 8, 
												 theImage.size.width*nu, colorSpace, 
												 kCGImageAlphaNone);
	CGColorSpaceRelease(colorSpace);
	if (context!= nil) 
	{
		CGContextTranslateCTM(context, 0, theImage.size.height);
		CGContextScaleCTM(context, 1.0, -1.0);
		UIGraphicsPushContext(context);
		
		CGRect rect = CGRectMake(0, 0, theImage.size.width, theImage.size.height);
		CGContextSetRGBFillColor (context, 1, 1, 1, 1);
		CGContextFillRect(context, rect);
		[theImage drawInRect:rect blendMode:kCGBlendModeMultiply alpha:1.0];
		CGImageRef cgImage = CGBitmapContextCreateImage(context);     
		uiImage = [[UIImage alloc] initWithCGImage:cgImage];
		UIGraphicsPopContext();
		CGContextRelease(context);
		CGImageRelease(cgImage);
	}
	if (uiImage == nil)
	{
		[theImage retain];
		uiImage = theImage;
	}
	return uiImage;
}

UIImage *makeHeadPhotoImage(UIImage *theImage)
{
	int value;
	CGRect thumbRect;
	UIImage *uiImage = nil;
	
	if (theImage.size.height > theImage.size.width)
	{
		value = (theImage.size.height - theImage.size.width) / 2;
		thumbRect = CGRectMake(0, value, theImage.size.width, theImage.size.width);
	}
	else
	{
		value = (theImage.size.width - theImage.size.height) / 2;
		thumbRect = CGRectMake(value, 0, theImage.size.height, theImage.size.height);
	}
	
	CGImageRef squreImage =  CGImageCreateWithImageInRect(theImage.CGImage, thumbRect);

	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGContextRef context = CGBitmapContextCreate(NULL, 32, 32, 8, 
												 32*4, colorSpace, 
												 kCGImageAlphaPremultipliedLast);
	CGColorSpaceRelease(colorSpace);
	if (context!= nil) 
	{
		UIGraphicsPushContext(context);
		CGContextDrawImage(context, CGRectMake(0, 0, 32, 32), squreImage);
		CGImageRelease(squreImage);
		
		squreImage = CGBitmapContextCreateImage(context);     
		uiImage = [[UIImage alloc] initWithCGImage:squreImage];
		UIGraphicsPopContext();
		CGContextRelease(context);
		CGImageRelease(squreImage);
	}
	//UIImageWriteToSavedPhotosAlbum(uiImage, nil, nil, nil);
	return uiImage;
}


UInt16 char16(int i)
{
	UInt16 o = ((i >> 8) & 0xff) | ((i & 0xff) << 8);

	return o;
}

UInt32 char32(int i)
{
	UInt32 o = ((i >> 24) & 0xff) | (((i >> 16) & 0xff) << 8) | (((i >> 8) & 0xff) << 16) | ((i & 0xff) << 24);
	return o;
}

NSString*	audioClipPath(NSString* clipName){
	BOOL ret = NO;
	BOOL isDirectory = NO;
	NSArray* aPaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString* aFilePath = [aPaths objectAtIndex: 0];
	aFilePath = [aFilePath stringByAppendingPathComponent: VOICE_CLIP_FOLDER];
	ret =[[NSFileManager defaultManager] fileExistsAtPath: aFilePath isDirectory: &isDirectory]; 
	if (!ret){
		[[NSFileManager defaultManager] createDirectoryAtPath: aFilePath attributes: nil];
	}
	else if (!isDirectory){
		[[NSFileManager defaultManager] removeItemAtPath: aFilePath error: nil];
		[[NSFileManager defaultManager] createDirectoryAtPath: aFilePath attributes: nil];
	}
	if ([clipName length] <= 0)
		return aFilePath;
	else
		return [aFilePath stringByAppendingPathComponent: clipName];
}

BOOL	isBuddyImid(NSString* anImid){
	if ([anImid length] == 0) return NO;
	
	NSInteger i = 0;
	char *anArg[] = PICA_BUDDY_IMID;
	NSInteger aCount = sizeof(anArg)/sizeof(char*);
	const char* aString = [anImid UTF8String];
	NSInteger aLen = [anImid length];
	for (i = 0; i < aCount; i++)
	{
		if (memcmp(anArg[i], "NULL", 4) == 0)
			return NO;
		if (memcmp(aString, anArg[i], aLen) == 0)
			return YES;
	}
	return NO;
}

NSString*	stringAPSDuration(NSInteger anIndex){
	NSString* aString = nil;
	switch (anIndex) {
		case APS_DURATION_NONE:
			aString		= NSLocalizedString(@"Right Now", nil);
			break;
		case APS_DURATION_5_MINUTES:
			aString		= NSLocalizedString(@"After 5 minutes", nil);
			break;
		case APS_DURATION_10_MINUTES:
			aString		= NSLocalizedString(@"After 10 minutes", nil);
			break;
		case APS_DURATION_30_MINUTES:
			aString		= NSLocalizedString(@"After 30 minutes", nil);
			break;
		case APS_DURATION_2_HOURS:
			aString		= NSLocalizedString(@"After 2 hours", nil);
			break;
		case APS_DURATION_6_HOURS:
			aString		= NSLocalizedString(@"After 6 hours", nil);
			break;
		case APS_DURATION_12_HOURS:
			aString		= NSLocalizedString(@"After 12 hours", nil);
			break;
	}
	return aString;
}

//extern NSInteger	indexAPSDuration(NSString* aString);

NSMutableArray* apsDurationStrings(){
	return [[NSMutableArray alloc] initWithObjects:
			NSLocalizedString(@"Right Now", nil),
			NSLocalizedString(@"After 5 minutes", nil),
			NSLocalizedString(@"After 10 minutes", nil),
			NSLocalizedString(@"After 30 minutes", nil),
            NSLocalizedString(@"After 2 hours", nil),
            NSLocalizedString(@"After 6 hours", nil),
            NSLocalizedString(@"After 12 hours", nil),
            nil];
}

NSInteger durationFrom(NSInteger anIndex){
	NSInteger aDuration = 0;
	switch (anIndex) {
		case APS_DURATION_NONE:
			aDuration		= 0;
			break;
		case APS_DURATION_5_MINUTES:
			aDuration		= 5 * 60;
			break;
		case APS_DURATION_10_MINUTES:
			aDuration		= 10 * 60;
			break;
		case APS_DURATION_30_MINUTES:
			aDuration		= 30 * 60;
			break;
		case APS_DURATION_2_HOURS:
			aDuration		= 2 * 60 * 60;
			break;
		case APS_DURATION_6_HOURS:
			aDuration		= 6 * 60 * 60;
			break;
		case APS_DURATION_12_HOURS:
			aDuration		= 12 * 60 * 60;
			break;
	}
	return aDuration;
}

NSString* convertString(NSString* aString){
	if ([aString length] == 0) return aString;
	NSString* theString = [aString stringByReplacingOccurrencesOfString: @"&"
															 withString: ESC_SYMBAL_AND];
	theString = [theString stringByReplacingOccurrencesOfString: @"<"
												   withString: ESC_LEFT_BRACKET];
	theString = [theString stringByReplacingOccurrencesOfString: @">"
												   withString: ESC_RIGHT_BRACKET];
	theString = [theString stringByReplacingOccurrencesOfString: @"'"
												   withString: ESC_SINGLE_QUOTE];
	theString = [theString stringByReplacingOccurrencesOfString: @"\""
												   withString: ESC_QUOTES];
	return theString;
}

