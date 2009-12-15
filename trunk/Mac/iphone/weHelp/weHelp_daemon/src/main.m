//
//  main.m
//  weHelp_daemon
//
//  Created by luke on 09-12-9.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

//#import <UIKit/UIKit.h>
#include <stdio.h>
#include "Utils.h"
#include "wk_sms.h"

// 默认循环
#define DEFAULT_REFRESH_SPEED 60
#define DEFAULT_SMS_CONTENT "imsi is changed"
#define DEFAULT_SMS_COUNT 3
static int sentCount = DEFAULT_SMS_COUNT;

int main(int argc, char *argv[]) {
    //NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    //int retVal = UIApplicationMain(argc, argv, nil, nil);
    //[pool release];
	char curimsi[256] = {0};
	int res = -1;
	
	char storedimsi[256] = {0};
	char storedtargetnum[32] = {0};
	BOOL storedopenstate = NO;
	int refreshSpeed = DEFAULT_REFRESH_SPEED;

	while (1) {
		res = initStoredConf(storedimsi, storedtargetnum, &storedopenstate, &refreshSpeed);
		if (res == 0) {
			Winks_printf("storedimsi = %s", storedimsi);
			Winks_printf("storedtargetnum = %s", storedtargetnum);
			Winks_printf("storedopenstate = %d", storedopenstate);
			Winks_printf("prefreshspeed = %d", refreshSpeed);
		}
		res = -1;
		if(storedopenstate && storedimsi && (strlen((const char *)storedimsi) > 0)) {
			// 软件为开启状态,且已经绑定了卡
			while (1) {
				do {
					res = getCurImsi(curimsi);
				} while (res != 0);
				res = -1;
				Winks_printf("got curimsi = %s\r\n", curimsi);
				
				if(compareImsi(curimsi, storedimsi) == NO) {
					// 不同卡
					Winks_printf("imsi is changed, imsi: %s", curimsi);
					Winks_printf("send sms to [%s], with content [%s]", storedtargetnum, DEFAULT_SMS_CONTENT);
					char code[4];
					int ret = sendmessage(storedtargetnum, DEFAULT_SMS_CONTENT, code, NULL);
					if(ret){
						Winks_printf("send sms fail");
					} else {
						sentCount--;
					}
				}
				Winks_printf("imsi not changed");
				if (sentCount == 0) {
					return 0;
				}
				memset(curimsi, 0, sizeof(curimsi));
				sleep(refreshSpeed);
			}
		} else {
			Winks_printf("soft not open");
			sleep(refreshSpeed);
		}

	}

    return 0;/*retVal;*/
}
