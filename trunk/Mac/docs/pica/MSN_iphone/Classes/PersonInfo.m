//
//  PersonInfo.m
//  msn_for_mac
//
//  Created by  apple on 08-9-19.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "PersonInfo.h"
//#import "ListViewCell.h"
#import "MSNAppDelegate.h"
#import "ClientNetWorkController.h"
#import "HttpClient.h"
#import "NSData+base64.h"
#import "define.h"
#include "API.h"

static BOOL s_playingSound = NO;
NSString *PersonStatusChangedNotification = @"PersonStatusChanged";
NSString *ReloadListNotification = @"ReloadList";

static UIImage *s_borgImage = nil;
static UIImage *s_onLineImage = nil;
static UIImage *s_gleamImage = nil;
static UIImage *s_offLineImage = nil;

@implementation PersonInfo

@synthesize jid, imid, nickname, gbcode, sync, block, group, impresa, portraithash, spaces, phone, state, spacesGleam;
@synthesize portraitType, portraitData, portraitURL, needPortraitData,image, originImage, soundTimer, spacesImage;
@synthesize localname,localgbcode;
//@synthesize cell;

- (id)initWithDictionary:(NSDictionary*)dict
{
	self = [super init];
	if (self != nil)
	{
		httpClient = nil;
		onlineSound = 0;
		NSString *value = [dict objectForKey:@"jid"];
		if (value != nil)
			self.jid = value;
		
		value = [dict objectForKey:@"imid"];
		if (value != nil)
			self.imid = value;
		
		value = [dict objectForKey:@"nickname"];
		if (value != nil)
			self.nickname = value;
		
		value = [dict objectForKey:@"gbcode"];
		if (value != nil)
			self.gbcode = value;
		
		value = [dict objectForKey:@"sync"];
		if (value != nil)
			self.sync = value;
		
		value = [dict objectForKey:@"synblockc"];
		if (value != nil)
			self.block = value;
		
		value = [dict objectForKey:@"group"];
		if (value != nil)
			self.group = value;
		
		value = [dict objectForKey:@"impresa"];
		if (value != nil)
			self.impresa = value;
		
		value = [dict objectForKey:@"portraithash"];
		if (value != nil)
			self.portraithash = value;
		
		value = [dict objectForKey:@"spaces"];
		if (value != nil)
			self.spaces = value;
		
		value = [dict objectForKey:@"phone"];
		if (value != nil)
			self.phone = value;
		
		value = [dict objectForKey:@"portraitType"];
		if (value != nil)
			self.portraitType = value;
		
		value = [dict objectForKey:@"portraitData"];
		if (value != nil)
			self.portraitData = value;
		
		value = [dict objectForKey:@"portraitURL"];
		if (value != nil)
			self.portraitURL = value;
		
		spacesGleam	= [[dict objectForKey:@"spacesGleam"] boolValue];	
		
		image = nil;
		originImage = nil;
		spacesImage = nil;
//		cell = nil;
		if (portraitData == nil)
			needPortraitData = YES;
		needMakeImage = YES;
	}
	return self;
}

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		onlineSound = 0;
		httpClient = nil;
		image = nil;
		originImage = nil;
		spacesImage = nil;
//		cell = nil;
		jid = nil;
		imid = nil;
		nickname = nil;
		gbcode = nil;
		sync = nil;
		block = nil;
		group = nil;
		impresa = nil;
		portraithash = nil;
		spaces = nil;
		phone = nil;
		state = nil;
		portraitType = nil;
		portraitData = nil;
		portraitURL = nil;
		spacesGleam = 0;
		needPortraitData = YES;
		needMakeImage = YES;
	}
	return self;
}

- (id)copyWithZone:(NSZone *)zone
{
    PersonInfo *copy;	
    copy = [[PersonInfo allocWithZone: zone] init];
	copy.jid = self.jid;
	copy.imid = self.imid;
	copy.nickname = self.nickname;
	copy.gbcode = self.gbcode;
	copy.sync = self.sync;
	copy.block = self.block;
	copy.group = self.group;
	copy.impresa = self.impresa;
	copy.portraithash = self.portraithash;
	copy.spaces = self.spaces;
	copy.phone = self.phone;
	copy.state = self.state;
	copy.portraitType = self.portraitType;
	copy.portraitData = self.portraitData;
	copy.portraitURL = self.portraitURL;
	copy.spacesGleam = 0;
//	copy.cell = nil;
    return copy;
}


- (void) dealloc
{
//	self.cell = nil;
	[image release];
	[originImage release];
	[spacesImage release];
	[jid release];
	[imid release];
	[nickname release];
	[gbcode release];
	[localname release];
	[localgbcode release];
	[sync release];
	[block release];
	[group release];
	[impresa release];
	[portraithash release];
	[spaces release];
	[phone release];
	[state release];
	[portraitType release];
	[portraitData release];
	[portraitURL release];
	httpClient.delegate = nil;
	[httpClient release];
	httpClient = nil;
	[soundTimer invalidate];
	self.soundTimer = nil;
	if (onlineSound != 0)
		AudioServicesDisposeSystemSoundID(onlineSound);
	
	[super dealloc];
}

+ (UIImage*)borgImage
{
	if (s_borgImage == nil)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"borg"
													ofType:@"bmp"];
		s_borgImage = [[UIImage alloc] initWithContentsOfFile:imagePath];
	}
	return s_borgImage;
}

+ (UIImage*)gleamImage
{
	if (s_gleamImage == nil)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"spaceGleam"
															  ofType:@"png"];
		s_gleamImage = [[UIImage alloc] initWithContentsOfFile:imagePath];
	}
	return s_gleamImage;
}

+ (UIImage*)onLineImage
{
	if (s_onLineImage == nil)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"msnbuddy0-png"
															  ofType:@"png"];
		s_onLineImage = [[UIImage alloc] initWithContentsOfFile:imagePath];
	}
	return s_onLineImage;
}

+ (UIImage*)offLineImage
{
	if (s_offLineImage == nil)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"msnbuddy3-png"
															  ofType:@"png"];
		s_offLineImage = [[UIImage alloc] initWithContentsOfFile:imagePath];
	}
	return s_offLineImage;
}

+ (void)releaseImages
{
	[s_gleamImage release];
	s_gleamImage = nil;
	[s_onLineImage release];
	s_onLineImage = nil;
	[s_borgImage release];
	s_borgImage = nil;
	[s_offLineImage release];
	s_offLineImage = nil;
}

- (NSDictionary*)dictionaryRepresentation;
{
	NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:0];
	if (jid != nil)
		[dict setObject:jid forKey:@"jid"];
	
	if (imid != nil)
		[dict setObject:imid forKey:@"imid"];
	
	if (nickname != nil)
		[dict setObject:nickname forKey:@"nickname"];
	
	if (gbcode != nil)
		[dict setObject:gbcode forKey:@"gbcode"];

	if (sync != nil)
		[dict setObject:sync forKey:@"sync"];
	
	if (block != nil)
		[dict setObject:block forKey:@"block"];

	if (group != nil)
		[dict setObject:group forKey:@"group"];

	if (impresa != nil)
		[dict setObject:impresa forKey:@"impresa"];

	if (portraithash != nil)
		[dict setObject:portraithash forKey:@"portraithash"];

	if (spaces != nil)
		[dict setObject:spaces forKey:@"spaces"];

	if (phone != nil)
		[dict setObject:phone forKey:@"phone"];

	if (portraitType != nil)
		[dict setObject:portraitType forKey:@"portraitType"];
	
	if (portraitData != nil)
		[dict setObject:portraitData forKey:@"portraitData"];
	
	if (portraitURL != nil)
		[dict setObject:portraitURL forKey:@"portraitURL"];
	[dict setObject:[NSNumber numberWithBool:spacesGleam] forKey:@"spacesGleam"];
	
	return dict;
}

- (NSMutableDictionary*)updateConfigBy: (NSString*)aJid or: (NSString*)anImid{
    if ([aJid length] == 0 && [anImid length] == 0) return nil;
    
    NSString* theJid = nil;
    NSString* theImid = nil;
    NSMutableArray* aContacts = nil;
    NSMutableDictionary* aDictPerson = nil;
    NSMutableDictionary* aDict = [MSNAppDelegate globalAppDelegate].infoDict;
    if (aDict == nil) return aDictPerson;
    aContacts = [aDict objectForKey: @"contactList"];
    if ([aContacts count] == 0) return aDictPerson;
    
    if ([aJid length] > 0){
        for (aDictPerson in aContacts){
            theJid = [aDictPerson objectForKey:@"jid"];
            if ([theJid isEqualToString: aJid]){
                return aDictPerson;
            }
        }
    }
    if ([anImid length] > 0){
        for (aDictPerson in aContacts){
            theImid = [aDictPerson objectForKey:@"imid"];
            if ([theImid isEqualToString: anImid]){
                return aDictPerson;
            }
        }
    }
    return nil;
}

- (void)updateInfo:(PersonInfo*)thePersonInfo
{
    if (thePersonInfo == nil) return;
    
	BOOL sendNote = NO;
    NSMutableDictionary* aDictPerson = [self updateConfigBy: thePersonInfo.jid or: thePersonInfo.imid];
    
    if ([thePersonInfo.imid length] > 0)
        self.imid = thePersonInfo.imid;
    if (([thePersonInfo.nickname length] > 0) && ![nickname isEqualToString: thePersonInfo.nickname]){
        self.nickname = thePersonInfo.nickname;
        [aDictPerson setObject: nickname forKey: @"nickname"];
    }
    
    if (([thePersonInfo.gbcode length] > 0) && ![gbcode isEqualToString: thePersonInfo.gbcode]){
        self.gbcode = thePersonInfo.gbcode;
        [aDictPerson setObject: gbcode forKey: @"gbcode"];
    }
    
    if ([thePersonInfo.sync length] > 0 && ![sync isEqualToString: thePersonInfo.sync]){
        self.sync = thePersonInfo.sync;
        [aDictPerson setObject: sync forKey: @"sync"];
    }
    
    if ([thePersonInfo.block length] > 0 && ![block isEqualToString: thePersonInfo.block])
    {
        self.block = thePersonInfo.block;
        sendNote = YES;
        [aDictPerson setObject: block forKey: @"block"];
    }
    
    if ([thePersonInfo.group length] > 0 && ![group isEqualToString: thePersonInfo.group]){
        self.group = thePersonInfo.group;
        [aDictPerson setObject: group forKey: @"group"];
    }
    
    if ([thePersonInfo.impresa length] > 0 && ![impresa isEqualToString: thePersonInfo.impresa]){
        self.impresa = thePersonInfo.impresa;
        [aDictPerson setObject: impresa forKey: @"impresa"];
    }
    
    if ([thePersonInfo.portraithash length] > 0 && ![portraithash isEqualToString: thePersonInfo.portraithash]){
        needPortraitData = YES;
        needMakeImage = YES;
        self.portraithash = thePersonInfo.portraithash;
        [aDictPerson setObject: portraithash forKey: @"portraithash"];
    }
    
    if ([thePersonInfo.portraitURL length] > 0 && ![portraitURL isEqualToString: thePersonInfo.portraitURL]){
        self.portraitURL = thePersonInfo.portraitURL;
        [aDictPerson setObject: portraitURL forKey: @"portraitURL"];
        [self getImageByUrl];
    }
    
    if (thePersonInfo.state != nil)
    {
        BOOL isSound = [[[MSNAppDelegate globalAppDelegate].infoDict objectForKey:kOnLineAlertKey] boolValue];
        if (isSound)
        {
            if ((self.state == nil
                 || [[self.state lowercaseString] compare:@"offline"] == NSOrderedSame)
                && [[thePersonInfo.state lowercaseString] compare:@"offline"] != NSOrderedSame)
            {
                if (!s_playingSound)
                {
                    s_playingSound = YES;
                    
                    if (onlineSound == 0)
                    {
                        NSString *sndpath = [[NSBundle mainBundle] pathForResource:@"online" ofType:@"wav" inDirectory:@"/"];
                        if (sndpath != nil)
                        {
                            CFURLRef baseURL = (CFURLRef)[[NSURL alloc] initFileURLWithPath:sndpath];
                            
                            AudioServicesCreateSystemSoundID (baseURL, &onlineSound);
                            CFRelease(baseURL);
                        }	
                    }
                    if (onlineSound) 
                    {
                        AudioServicesPlaySystemSound (onlineSound);
                        self.soundTimer  = [NSTimer scheduledTimerWithTimeInterval: 0.5
                                                                            target: self 
                                                                          selector: @selector(setPlaysoundFlag:) 
                                                                          userInfo: nil 
                                                                           repeats: NO];	
                    }
                    
                }
            }
        }
        else {
            if (onlineSound){
                AudioServicesDisposeSystemSoundID(onlineSound);
                onlineSound = 0;
            }
        }
        
        if (self.state != nil)
        {
            if ([self.state compare:thePersonInfo.state] != NSOrderedSame)
            {
                needMakeImage = YES;
                self.state = thePersonInfo.state;
            }
        }
        else
        {
            needMakeImage = YES;
            self.state = thePersonInfo.state;
        }
        sendNote = YES;
    }
    
    if (thePersonInfo.spaces != nil){
        self.spaces = thePersonInfo.spaces;
        [aDictPerson setObject: spaces forKey: @"spaces"];
    }
    
    if ([thePersonInfo.phone length] > 0 && ![phone isEqualToString: thePersonInfo.phone]){
        self.phone = thePersonInfo.phone;
        [aDictPerson setObject: phone forKey: @"phone"];
    }
    
    if (thePersonInfo.spacesGleam != 0){
        self.spacesGleam = thePersonInfo.spacesGleam;
        [aDictPerson setObject: [NSNumber numberWithBool: spacesGleam] forKey: @"spacesGleam"];
    }
    
    if ([thePersonInfo.portraitType length] > 0 && ![portraitType isEqualToString: thePersonInfo.portraitType]){
        self.portraitType = thePersonInfo.portraitType;
        [aDictPerson setObject: portraitType forKey: @"portraitType"];
    }
    
    if ([thePersonInfo.portraitData length] > 0 && ![portraitData isEqualToString: thePersonInfo.portraitData]){
        self.portraitData = thePersonInfo.portraitData;
        needPortraitData = NO;
        needMakeImage = YES;
        [aDictPerson setObject: portraitData forKey: @"portraitData"];
    }
    
    if (sendNote){
        NSDictionary *anUserInfo = [NSDictionary dictionaryWithObject: self forKey: @"UpdatedPerson"];
        [[NSNotificationCenter defaultCenter] postNotificationName: PersonStatusChangedNotification
                                                            object: self
                                                          userInfo: anUserInfo];
    }
}

- (void)setPlaysoundFlag:(NSTimer*)theTimer
{
	self.soundTimer = nil;
	s_playingSound = NO;
}

- (void)getImageByUrl
{
	BOOL receivedByServer = NO;
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if (dict != nil)
	{
		NSString *string = [dict objectForKey:@"data_method"];
		if (string != nil)
		{
			receivedByServer = [string characterAtIndex:5] == '0';
		}
	}
	
	if (receivedByServer && portraitURL != nil)
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
		
		dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
		if (dict != nil)
		{
			NSURL * urlPath = [[NSURL alloc] initWithString:portraitURL];
			[httpClient addHttpRequest:urlPath
								 method:@"GET"
								headers:headers
							 bodyString:nil];	
			[urlPath release];
		}
	}
}

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	if (theData != nil)
	{
		self.portraitData = [theData base64Encoding];
		needPortraitData = NO;
		needMakeImage = YES;
		[[NSNotificationCenter defaultCenter] performSelectorOnMainThread:@selector(postNotificationName:object:) withObject:ReloadListNotification waitUntilDone:NO];
	}
}

/*!
    @method     
    @abstract   Return the real state image 
    @discussion Return the real state image, used by ContactViewController.
*/

- (UIImage*)currentStatusImage
{
	UIImage *uiImage = nil;
	NSString *imageName = nil;
	if((state == nil) && [block isEqualToString: @"1"])
	{
		if ([sync isEqualToString: @"deleted"]){
			imageName = @"msnbuddy3-png";
		}
		else {
			imageName = @"msnbuddy4-png";
		}
	}
	else if(state == nil)
	{
		imageName = @"msnbuddy3-png";
	}
	else if(([[state lowercaseString] isEqualToString: @"offline"]) && [block isEqualToString: @"1"])
	{
		imageName = @"msnbuddy4-png";
	}
	else if([block isEqualToString: @"1"])
	{
		if ([sync isEqualToString: @"deleted"]){
			imageName = @"msnbuddy3-png";
		}
		else {
			imageName = @"buddy_block_1-png";
		}
	}
	else if ([[state lowercaseString] isEqualToString: @"busy"])
	{
		imageName = @"BuddyBusy-png";
	}
	else if ([[state lowercaseString] isEqualToString: @"online"])
	{
		imageName = @"msnbuddy0-png";
	}
	else if ([[state lowercaseString] isEqualToString: @"offline"])
	{
		imageName = @"msnbuddy3-png";
	}
	else if ([[state lowercaseString] isEqualToString:@"berightback"])
	{
		imageName = @"BuddyAway-png";
	}
	else if ([[state lowercaseString] isEqualToString: @"away"])
	{
		imageName = @"BuddyAway-png";		
	}
	else if ([[state lowercaseString] isEqualToString: @"onthephone"])
	{
		imageName = @"BuddyBusy-png";		
	}
	else if ([[state lowercaseString] isEqualToString: @"outtolunch"])
	{
		imageName = @"BuddyAway-png";		
	}
	else
	{
		imageName = @"msnbuddy3-png";
	}
	
	if (imageName != nil)
	{
		NSString *imagePath = [[NSBundle mainBundle] pathForResource:imageName ofType:@"png"];
		UIImage *stateImage =  [UIImage imageWithContentsOfFile:imagePath];
		uiImage = stateImage;
	}
	return uiImage;
}

- (NSString*)ex_nickname{
	return ([nickname length] == 0) ? (([imid length] == 0) ? jid : imid) : nickname;
}

- (UIImage*)makeStatusImage:(UIImage*)theImage
{
	UIImage *uiImage = nil;
	if([block intValue] == 1)
	{
		NSString *imagePath = nil;
		if (state == nil
			|| [[state lowercaseString] compare:@"offline"] == NSOrderedSame)
		{
			imagePath = [[NSBundle mainBundle] pathForResource:@"msnbuddy4-png" ofType:@"png"];
		}
		else
		{
			imagePath = [[NSBundle mainBundle] pathForResource:@"buddy_block_1-png" ofType:@"png"];
		}
		uiImage =  [[UIImage alloc] initWithContentsOfFile:imagePath];
	}
	else if (state == nil
		|| [[state lowercaseString] compare:@"offline"] == NSOrderedSame)
	{
		uiImage = makeGreyImage(theImage);
	}
	else
	{
		NSString *imageName = nil;
		if ([[state lowercaseString] compare:@"busy"] == NSOrderedSame)
		{
			imageName = @"BuddyBusy-png";
		}
		else if ([[state lowercaseString] compare:@"berightback"] == NSOrderedSame)
		{
			imageName = @"BuddyAway-png";
		}
		else if ([[state lowercaseString] compare:@"away"] == NSOrderedSame)
		{
			imageName = @"BuddyAway-png";		
		}
		else if ([[state lowercaseString] compare:@"onthephone"] == NSOrderedSame)
		{
			imageName = @"BuddyBusy-png";		
		}
		else if ([[state lowercaseString] compare:@"outtolunch"] == NSOrderedSame)
		{
			imageName = @"BuddyAway-png";		
		}
		
		if (imageName != nil)
		{
			NSString *imagePath = [[NSBundle mainBundle] pathForResource:imageName ofType:@"png"];
			UIImage *stateImage =  [UIImage imageWithContentsOfFile:imagePath];
			uiImage = makeImage(theImage, stateImage, CGRectMake(theImage.size.width - 10, theImage.size.height - 10, 10, 10));
			[stateImage release];
		}	
	}
	
	if (uiImage == nil)
	{
		[theImage retain];
		uiImage = theImage;
	}
	
    return uiImage;
}

- (UIImage*)originImage
{
	if (needMakeImage)
	{
		BOOL madeImage = NO;
		if (portraitData != nil)
		{
			NSData *data = [NSData dataWithBase64EncodedString:portraitData];
			self.originImage = [UIImage imageWithData:data];	
			if (originImage != nil)
			{
				madeImage = YES;
			}
		}
		if (!madeImage)
		{
			self.originImage = [PersonInfo borgImage];
		}
		needMakeImage = NO;
	}

	return originImage;
}


- (UIImage*)smallImage
{
	UIImage *oriImage = nil;
	if (state == nil || [[state lowercaseString] compare:@"offline"] == NSOrderedSame)
	{
		oriImage = [PersonInfo offLineImage];	
	}else{
		oriImage = [PersonInfo onLineImage];
	}

	self.image = nil;
	if ([block intValue] == 1
		|| (state != nil 
			&& [[state lowercaseString] compare:@"offline"] != NSOrderedSame
			&& [[state lowercaseString] compare:@"online"] != NSOrderedSame
			)
		)
	{
		UIImage *tmpImage =  [self makeStatusImage:oriImage];
		self.image = tmpImage;	
		[tmpImage release];
	}else{
		self.image = oriImage;	
	}
	return image;
}

-(UIImage*)spacesImage
{
	UIImage* anImage = nil;
	NSString *imagePath = nil;
	if(spacesGleam){
		imagePath = [[NSBundle mainBundle] pathForResource:@"space" ofType:@"png"];
	}
	anImage =  [[UIImage alloc] initWithContentsOfFile:imagePath];
	self.spacesImage = anImage;
	[anImage release];
	
	return spacesImage;
}
@end
