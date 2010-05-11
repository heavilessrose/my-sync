//
//  ImageButton.m
//  MSN
//
//  Created by  apple on 08-10-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ImageButton.h"
#import "NSData+base64.h"
#import "MSNAppDelegate.h"
#import "PersonInfo.h"
#import "HttpClient.h"
#import "define.h"
#import "API.h"

@implementation ImageButton

- (id)initWithImage:(UIImage *)image
{
	self = [super initWithImage:image];
	if (self != nil)
	{
		httpClient = nil;
	}
	return self;
}

- (void)drawRect:(CGRect)rect 
{
	[super drawRect:rect];
	// Drawing code
}


- (void)getImageByUrl:(NSString*)theUrl;
{
	if (theUrl != nil)
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
		
		NSURL * urlPath = [[NSURL alloc] initWithString:theUrl];
		[httpClient addHttpRequest:urlPath
							 method:@"GET"
							headers:headers
						 bodyString:nil];
		[urlPath release];
	}
}

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	UIImage *backgroundImage = nil;
	if (theData != nil)
	{
		backgroundImage = [UIImage imageWithData:theData];
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		[appdelegate.infoDict setObject:[theData base64Encoding] forKey:@"portraithashData"];
	}
	else
	{
		backgroundImage = [PersonInfo borgImage];
	}
	[self setButtonImage:backgroundImage];
}

- (void)updateButtonImage:(BOOL)urlChanged
{
	UIImage *backgroundImage = nil;

	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;

	NSString *portraitData = nil;
	NSString *portraitURL = [appdelegate.infoDict objectForKey:@"portraithashUrl"];
	
	BOOL receivedByServer = NO;
	BOOL getdefaultImage = YES;
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if (dict != nil)
	{
		NSString *string = [dict objectForKey:@"data_method"];
		if (string != nil)
		{
			receivedByServer = [string characterAtIndex:5] == '0';
		}
	}
	
	if (receivedByServer && urlChanged)
	{
		if (portraitURL != nil)
		{
			getdefaultImage = NO;
			[self getImageByUrl:portraitURL];
		}
	}
	
	if (getdefaultImage)
	{
		portraitData = [appdelegate.infoDict objectForKey:@"portraithashData"];
		if (portraitData != nil)
		{
			NSData *data = [NSData dataWithBase64EncodedString:portraitData];
			backgroundImage = [UIImage imageWithData:data];
			if (backgroundImage != nil)
				getdefaultImage = NO;
		}
	}
	
	if (getdefaultImage)
	{
		backgroundImage = [PersonInfo borgImage];;
	}
	[self setButtonImage:backgroundImage];
}

- (void)setButtonImage:(UIImage*)theImage{
	if (theImage != nil){
		self.image = theImage;
	}
}

- (void)dealloc 
{
	httpClient.delegate = nil;
	[httpClient release];
	httpClient = nil;
	[super dealloc];
}


@end
