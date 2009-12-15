//
//  Utils.h
//  weHelp_daemon
//
//  Created by luke on 09-12-9.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
#ifndef _Utils_
#define _Utils_

#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>
//#import <CoreTelephony/CoreTelephony.h>

#define CONFIG_FILE @"/var/mobile/weConf.plist"

int getCurImsi(char *imsi);
// 读取plist属性文件
id plistFromFile(NSString *filePath);
// 得到配置信息
int initStoredConf(char *pimsi, char *pnumber, BOOL *popenstate, int *prefreshspeed);
// 比较imsi是否相同
BOOL compareImsi(char *curImsi, char *storedimsi);

void Winks_printf(const char* fmt, ... );

#endif