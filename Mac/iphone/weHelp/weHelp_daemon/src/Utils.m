//
//  Utils.m
//  weHelp_daemon
//
//  Created by luke on 09-12-9.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Utils.h"

extern NSString *CTSIMSupportCopyMobileSubscriberIdentity(void* Temp);


#pragma mark -
#pragma mark Log

#if TARGET_IPHONE_SIMULATOR
#define SHOW_LOG
#endif

static void CallLog(const char* str)
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#if TARGET_IPHONE_SIMULATOR
	char* path = "/Users/luke/weLog";
#else
	char* path = "/var/mobile/weLog";
#endif
	
	FILE *myLog = NULL;
	myLog = fopen(path,"a+");
	
	if (myLog != NULL) 
	{
		NSDate *time = [NSDate date];
		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
		//char tmp[50] = {0};
		//getCurTime(tmp);
		
		fputs(tmp,myLog);
		fputs("[DAEMON] ",myLog);
		fputs(str,myLog);
		fputs("\n",myLog);
		//
		printf("[DAEMON] %s\n",str);
	}
	fclose(myLog);
	[pool release];
}

void Winks_printf(const char* fmt, ... )
{
#ifdef SHOW_LOG
    const int MAX_DBG_STR = 1024;
	
    int written = 0;
    char buffer[MAX_DBG_STR];
	memset(buffer,0,MAX_DBG_STR);
	
    va_list va;
    va_start( va, fmt );
    written = vsprintf( &buffer[0], fmt, va);
    va_end(va);
	
	if(strlen(buffer))
		CallLog(buffer);
#endif
}

#pragma mark -
int getCurImsi(char *imsi)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	//NSString *CTSIMSupportCopyMobileSubscriberIdentity(void* Temp);
	NSString *imsiNSStr = nil;
	imsiNSStr = CTSIMSupportCopyMobileSubscriberIdentity(nil);
	
	if (imsiNSStr != nil) {
		const char *imsiStr = [imsiNSStr cStringUsingEncoding:NSASCIIStringEncoding];
		int imsiStrLen = strlen(imsiStr);
		strcpy(imsi, imsiStr);
	}else {
		Winks_printf("getCurImsi err");
		[pool release];
		return -1;
	}
	[pool release];
	return 0;
}

// 得到配置信息
int initStoredConf(char *pimsi, char *pnumber, BOOL *popenstate, int *prefreshspeed)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSDictionary *confPlist = (NSDictionary *)plistFromFile(CONFIG_FILE);
	if (confPlist != nil) {
		Winks_printf("got confile -- ");
		// 读取
		NSString *imsi = (NSString *)[confPlist objectForKey:@"IMSI"];
		NSString *smsto = (NSString *)[confPlist objectForKey:@"targetNumber"];
		NSNumber *openState = (NSNumber *)[confPlist objectForKey:@"openState"];
		NSNumber *refreshSpeed = (NSNumber *)[confPlist objectForKey:@"refreshSpeed"];
		BOOL openStateBool = [openState boolValue] ? YES:NO;
		
		strcpy(pimsi, [imsi UTF8String]);
		strcpy(pnumber, [smsto UTF8String]);
		*popenstate = openStateBool;
		*prefreshspeed = [refreshSpeed intValue];
	} else {
		Winks_printf("confile not exist -- ");
		return -1;
	}
	[pool release];
	return 0;
}

// 比较imsi是否相同
BOOL compareImsi(char *curImsi, char *storedimsi)
{
	Winks_printf("compare imsi: [%s] and [%s]", curImsi, storedimsi);
	if (!strcmp((const char *)curImsi, (const char *)storedimsi)) {
		// 相同
		Winks_printf("Imsi is not changed");
		return YES;
	}else {
		Winks_printf("----Imsi is changed");
		return NO;
	}
}

// 读取plist属性文件
id plistFromFile(NSString *filePath)
{
    NSData *plistXML;
    NSString *err;
    id retPlist;
    NSPropertyListFormat format;
	NSString *plistPath = filePath;
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:plistPath])
	{
		NSLog(@"plist file not exist %@", plistPath);
		//plistPath = [[NSBundle mainBundle] pathForResource:@"Data" ofType:@"plist"];
		return nil;
	}
	
	plistXML = [[NSFileManager defaultManager] contentsAtPath:plistPath];
    if (!plistXML) {
        NSLog(@"fail to get data from plist file %@", plistPath);
        return nil;
    }
    retPlist = [NSPropertyListSerialization propertyListFromData:plistXML 
												mutabilityOption:NSPropertyListImmutable format:&format errorDescription:&err];
    if (!retPlist){
        NSLog(@"CONFIG_FILE not returned, error: %@", err);
		retPlist = nil;
    }
    return retPlist;
}
