//
//  FFSettings.m
//  FFriends
//
//  Created by luke on 11-7-7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "FFSettings.h"
#import "LKTools.h"

@interface FFSettings ()
- (BOOL)archiveSettings;
+ (FFSettings *)unarchiveSettings;
- (void)loadDefaultSettings;
@end

static FFSettings *shareInstance = nil;

@implementation FFSettings

@synthesize FirstRun, ClientVersion, pass, username, email, logined;

#pragma mark - singleton

+ (FFSettings *)shareSettings {
	
	if (!shareInstance) {
        if ([FFSettings isFirstRun]) {
            shareInstance = [[FFSettings alloc] init];
        } else {
            shareInstance = [FFSettings unarchiveSettings];
            DLog(@"");
        }
	}
	return shareInstance;
}

- (id)init {
	
    if ((self = [super init])) {
        if ([FFSettings isFirstRun]) {
			[self loadDefaultSettings];
            if ([self archiveSettings]) {
                self.FirstRun = NO;
            }
        }
    }
    
	return self;
}

- (void)dealloc {
	
	MLog(@"");
    [email release];
    [ClientVersion release];
    [pass release];
    [username release];
	[super dealloc];
}

#pragma mark - 

+ (BOOL)isFirstRun
{
    //TODO: 
    DLog(@"%@", docPath());
    if ([[NSFileManager defaultManager] fileExistsAtPath:FFSettings_Path isDirectory:NO]) {
        return NO;
    }
    return YES;
}

- (void)loadDefaultSettings
{
	NSString *defaultPrefsPath = [[NSBundle mainBundle] pathForResource:@"Prefs" ofType:@"plist"];
	NSDictionary *defaultPrefs = [NSDictionary dictionaryWithContentsOfFile:defaultPrefsPath];
	NSLog(@"%@", defaultPrefs);
	//	[[NSUserDefaults standardUserDefaults] setValuesForKeysWithDictionary:defaultPrefs];
	//	[[NSUserDefaults standardUserDefaults] synchronize];
	[self setValuesForKeysWithDictionary:defaultPrefs];
}

- (BOOL)archiveSettings
{
    // write
    NSMutableData *aData = [[NSMutableData alloc] init];
    NSKeyedArchiver *archiver = [[NSKeyedArchiver alloc] initForWritingWithMutableData:aData];          
    [archiver encodeObject:self forKey:FFSettings_Key];
    [archiver finishEncoding];
    if (![aData writeToFile:FFSettings_Path atomically:YES]) {
		DLog(@"write file %@ fail", FFSettings_Path);
		[archiver release];
		[aData release];
		return NO;
	}
    [archiver release];
    [aData release];
	DLog(@"write ok");
    return YES;
}

+ (FFSettings *)unarchiveSettings
{
    NSData *codedData = [[[NSData alloc] initWithContentsOfFile:FFSettings_Path] autorelease];
    if (codedData == nil)
        return nil;
    
    NSKeyedUnarchiver *unarchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:codedData];
    shareInstance = [unarchiver decodeObjectForKey:FFSettings_Key];    
    [unarchiver finishDecoding];
    [unarchiver release];
	return [shareInstance retain];
}

#pragma mark - NSCoding

- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeBool:FirstRun forKey:@"FirstRun"];
    [aCoder encodeBool:logined forKey:@"logined"];
    [aCoder encodeObject:ClientVersion forKey:@"ClientVersion"];
    [aCoder encodeObject:username forKey:@"username"];
    [aCoder encodeObject:pass forKey:@"pass"];
    [aCoder encodeObject:email forKey:@"email"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if ((self = [super init])) {
        FirstRun = [aDecoder decodeBoolForKey:@"FirstRun"];
        logined = [aDecoder decodeBoolForKey:@"logined"];
        ClientVersion = [[aDecoder decodeObjectForKey:@"ClientVersion"] retain];
        username = [[aDecoder decodeObjectForKey:@"username"] retain];
        pass = [[aDecoder decodeObjectForKey:@"pass"] retain];
        email = [[aDecoder decodeObjectForKey:@"email"] retain];
    }
    return self;
}

@end
