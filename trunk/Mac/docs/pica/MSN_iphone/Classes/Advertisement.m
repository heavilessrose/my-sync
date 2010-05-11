//
//  advertisement.m
//  MSN
//
//  Created by  apple on 09-3-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Advertisement.h"
#import "HttpClient.h"
#import "AdvertisementLabel.h"
#import "AdvertisementView.h"
#import "MSNAppDelegate.h"
#import "define.h"

NSString		*ShowZwpViewNotification = @"ShowZwpView";
NSString		*AdImageChangedNotification = @"AdImageChanged";
NSString		*ShowStandbyZwpViewNotification = @"ShowStandbyZwpViewNotification";

static NSMutableDictionary *s_imageDict = nil;

@implementation Advertisement

@synthesize sale, adId, dbid, type, target, action, textLabel, imageView, param, altText;
@synthesize myAlertView;

- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		sale = NO;
		dbid = 0;
		type = nil;
		imageUrl = nil;
		target = nil;
		action = nil;
		text = nil;
		imageView = nil;
		textLabel = nil;
		sysDismiss = NO;
		if (s_imageDict == nil)
			s_imageDict = [[NSMutableDictionary alloc] initWithCapacity:0];
	}
	return self;
}

- (id)initWithDictionary:(NSDictionary*)dict
{
	self = [super init];
	if (self != nil)
	{
		httpClient = nil;
		imageView = nil;
		textLabel = nil;
		myAlertView = nil;
		sysDismiss = NO;
		if (s_imageDict == nil)
			s_imageDict = [[NSMutableDictionary alloc] initWithCapacity:0];
		
		NSString *value = [dict objectForKey:@"sale"];
		if (value != nil)
			self.sale = [value intValue];
		
		value = [dict objectForKey:@"adId"];
		if (value != nil)
			self.adId = [value intValue];
		
		value = [dict objectForKey:@"dbid"];
		if (value != nil)
			self.dbid = [value intValue];
		
		value = [dict objectForKey:@"type"];
		if (value != nil)
			self.type = value;
		
		value = [dict objectForKey:@"imageUrl"];
		if (value != nil)
			self.imageUrl = value;
		
		value = [dict objectForKey:@"target"];
		if (value != nil)
			self.target = value;
		
		value = [dict objectForKey:@"action"];
		if (value != nil)
			self.action = value;
		
		value = [dict objectForKey:@"text"];
		if (value != nil)
			self.text = value;
		
		value = [dict objectForKey:@"param"];
		if (value != nil)
			self.param = value;
		
		value = [dict objectForKey:@"altText"];
		if (value != nil)
			self.altText = value;
	}
	return self;
}

- (NSDictionary*)dictionaryRepresentation;
{
	NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:0];
	if (sale != 0)
		[dict setObject:[NSNumber numberWithBool:sale] forKey:@"sale"];
	
	if (adId != 0)
		[dict setObject:[NSNumber numberWithInt:adId] forKey:@"adId"];
	
	if (dbid != 0)
		[dict setObject:[NSNumber numberWithInt:dbid] forKey:@"dbid"];
	
	if (type != nil)
		[dict setObject:type forKey:@"type"];
	
	if (imageUrl != nil)
		[dict setObject:imageUrl forKey:@"imageUrl"];
	
	if (target != nil)
		[dict setObject:target forKey:@"target"];
	
	if (action != nil)
		[dict setObject:action forKey:@"action"];
	
	if (text != nil)
		[dict setObject:text forKey:@"text"];
	
	if (param != nil)
		[dict setObject:param forKey:@"param"];
	
	if (altText != nil)
		[dict setObject:altText forKey:@"altText"];
	
	return dict;
}

- (void) dealloc
{
	if (myAlertView != nil)
	{
		myAlertView.delegate = nil;
		[myAlertView dismissWithClickedButtonIndex:0 animated:NO];
		self.myAlertView = nil;
	}

	if ([type compare:@"login-prog-ad"] == NSOrderedSame)
	{
		[[NSUserDefaults standardUserDefaults] setObject:[self dictionaryRepresentation]
												  forKey:@"login-prog-ad"];		
	}
	[altText release];
	[type release];
	[imageUrl release];
	[target release];
	[action release];
	[text release];
	[param release];
	textLabel.target = nil;
	[textLabel release];
	imageView.target = nil;
	[imageView release];
	httpClient.delegate = nil;
	[httpClient release];
	[super dealloc];
}

+ (void)saveImage
{
	[[NSUserDefaults standardUserDefaults] setObject:s_imageDict forKey:@"Advertisement"];
	[s_imageDict release];
	s_imageDict = nil;
}

- (NSString*)text
{
	return text;
}

- (void)setText:(NSString*)theText
{
	[theText retain];
	[text release];
	text = theText;
	if (text != nil)
	{
		if (textLabel == nil)
		{
			textLabel = [[AdvertisementLabel alloc] initWithFrame:CGRectZero];
			[textLabel setTextAlignment:UITextAlignmentCenter];
			[textLabel addTarget:self action:@selector(buttonClicked:)];					
			[textLabel setTextColor:[UIColor blueColor]];
		}
		[textLabel setText:text];
		textLabel.frame = [textLabel textRectForBounds:CGRectMake(0, 0, 320, 60) limitedToNumberOfLines:3];
	}
}

- (NSString*)imageUrl
{
	return imageUrl;
}

- (void)setImageUrl:(NSString*)theUrl
{
	[theUrl retain];
	[imageUrl release];
	imageUrl = theUrl;
	if (imageUrl != nil && [imageUrl length] > 0)
	{
		NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"Advertisement"];
		NSData *imagedata = [dict objectForKey:theUrl];
		UIImage *image = nil;
		if (imagedata != nil)
			image = [UIImage imageWithData:imagedata];
		if (image != nil)
		{
			[s_imageDict setObject:UIImagePNGRepresentation(image) forKey:imageUrl];
			if (imageView == nil)
			{
				imageView = [[AdvertisementView alloc] initWithFrame:CGRectMake(0, 0, [image size].width, [image size].height)];
				imageView.image = image;
				[imageView addTarget:self action:@selector(buttonClicked:)];						
			}
			else
			{
				imageView.image = image;
				imageView.frame = CGRectMake(0, 0, [image size].width, [image size].height);				
			}
		}
		else
		{
			if (httpClient == nil)
			{
				httpClient = [[HttpClient alloc] init];
				httpClient.callbackFun = @selector(callBackData:dictionary:);
				httpClient.delegate = self;	
			}
			
			NSURL * urlPath = [[NSURL alloc] initWithString:imageUrl];
			if (urlPath != nil)
			{
				getImageData = YES;
				[httpClient addHttpRequest:urlPath
									method:@"GET"
								   headers:nil
								bodyString:nil];
				[urlPath release];			
			}	
		}
	}
}

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	if (getImageData)
	{
		UIImage *image = [UIImage imageWithData:theData];
		if (image != nil)
		{
			[s_imageDict setObject:UIImagePNGRepresentation(image) forKey:imageUrl];
			if (imageView == nil)
			{
				imageView = [[AdvertisementView alloc] initWithFrame:CGRectMake(0, 0, [image size].width, [image size].height)];
				imageView.image = image;
				[imageView addTarget:self action:@selector(buttonClicked:)];					
			}
			else
			{
				imageView.image = image;
				imageView.frame = CGRectMake(0, 0, [image size].width, [image size].height);				
			}
			[[NSNotificationCenter defaultCenter] postNotificationName:AdImageChangedNotification object:self];
		}
	}
	else
	{	
		if (action == nil || [action compare:@"zwp"] == NSOrderedSame)
		{
			MYLog(@"----------nil or live up--------");
		}
		else if ([action compare:@"wap"] == NSOrderedSame)
		{
			MYLog(@"----------open safari--------");

			if (target != nil)
			{
				NSURL *myURL = [NSURL URLWithString:self.target];
				[[UIApplication sharedApplication] openURL:myURL];
			}
		}
		else if ([action compare:@"tel"] == NSOrderedSame)
		{
			MYLog(@"----------tel--------");

			if (target != nil)
			{
				NSString *theURL = [NSString stringWithString:@"tel:"];
				theURL = [theURL stringByAppendingString:self.target];
				NSURL *myURL = [NSURL URLWithString:theURL];
				[[UIApplication sharedApplication] openURL:myURL];
			}
		}
		else if ([action compare:@"sms"] == NSOrderedSame)
		{
			MYLog(@"----------sms--------");
			if (target != nil)
			{
				NSString *theURL = [NSString stringWithString:@"sms:"];
				theURL = [theURL stringByAppendingString:self.target];
				NSURL *myURL = [NSURL URLWithString:theURL];
				[[UIApplication sharedApplication] openURL:myURL];
			}
		}
	}
}

- (void)buttonClicked:(id)sender
{
	NSString *message = nil;
	if (action == nil)
	{
	}
	else if ([action compare:@"zwp"] == NSOrderedSame)
	{
		[self sendClickNote];
		NSDictionary *dict = [NSDictionary dictionaryWithObject:target forKey:@"url"];
		[[NSNotificationCenter defaultCenter] postNotificationName:ShowZwpViewNotification object:self userInfo:dict];
		//goto...
	}
	else if ([action compare:@"wap"] == NSOrderedSame)
	{
		message = [NSString stringWithFormat:NSLocalizedString(@"AdvertisementWarning",nil),@"Safari"];
	}
	else if ([action compare:@"tel"] == NSOrderedSame)
	{
		message = [NSString stringWithFormat:NSLocalizedString(@"AdvertisementWarning",nil),NSLocalizedString(@"Tel",nil)];		
	}
	else if ([action compare:@"sms"] == NSOrderedSame)
	{
		message = [NSString stringWithFormat:NSLocalizedString(@"AdvertisementWarning",nil),NSLocalizedString(@"SMS",nil)];
	}
	if (message != nil)
	{
		self.myAlertView = [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",nil)
													   message:message
													  delegate:self cancelButtonTitle: NSLocalizedString(@"qtn_cancel",nil)
											 otherButtonTitles:NSLocalizedString(@"qtn_ok",nil),nil] autorelease];
		[myAlertView setDelegate:self];
		sysDismiss = YES;
		[myAlertView show];
	}
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	sysDismiss = NO;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (!sysDismiss)
	{
		alertView.delegate = nil;
		self.myAlertView = nil;

		if ([alertView.title compare:NSLocalizedString(@"Alert",nil)] == NSOrderedSame)
		{
			if (buttonIndex == 1)
			{
				[self sendClickNote];
			}
		}
	}
}

- (void)sendClickNote
{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if (dict != nil)
	{
		if (httpClient == nil)
		{
			httpClient = [[HttpClient alloc] init];
			httpClient.callbackFun = @selector(callBackData:dictionary:);
			httpClient.delegate = self;	
		}
		NSString *url = [dict objectForKey:@"adurl"];
		if (url != nil)
		{
			NSRange range = [url rangeOfString:@"/" options:NSBackwardsSearch range:NSMakeRange(0, [url length])];
			if (range.location != NSNotFound)
			{
				url = [url substringToIndex:range.location];
				url = [url stringByAppendingString:@"/advisit"];
			}
			NSURL * urlPath = [[NSURL alloc] initWithString:url];
			if (urlPath != nil)
			{
				MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
				NSString *account = appdelegate.myAccount;
				if (account != nil)
				{
					NSString *body = [NSString stringWithFormat:@"<advisit from='%@' source='msn' dbid='%d' sale='%d'/>",
									  account,dbid,sale];
					getImageData = NO;
					[httpClient addHttpRequest:urlPath
										method:@"POST"
									   headers:nil
									bodyString:[body dataUsingEncoding:NSUTF8StringEncoding]];
				}
				[urlPath release];			
			}	
		}
	}
}
@end
