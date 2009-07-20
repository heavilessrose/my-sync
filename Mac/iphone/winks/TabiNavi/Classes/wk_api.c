/*
 *  wk_api.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/17/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_api.h"
#include "wk_osfnc.h"
#include <sys/time.h>
#include <signal.h>
#include <time.h>

/************************************************************************\
 系统信息
 \************************************************************************/

/**************************************************************************\
 功能描述:
 获取当前系统日期、时间
 参数说明:
 无
 返回值:
 无
 实现描述:
 判断参数
 \**************************************************************************/
int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p){
	if (NULL == dateTime_p){
		return WINKS_RETURN_FAILURE;
	}
	
	
	NSDate *now = [NSDate date];
	[NSDateFormatter ]
	
	
	dateTime_p->year = cur_date.year;
	dateTime_p->month = cur_date.mon;
	dateTime_p->day = cur_date.mday;
	dateTime_p->week = cur_date.wday;
	
	dateTime_p->hour = cur_time.hour;
	dateTime_p->min = cur_time.min;
	dateTime_p->sec = cur_time.sec;
	return WINKS_RETURN_SUCCESS;
}




const unsigned long KMaxTimerOut = 30*60*1000;//30 min

void timefunc(int sig) /* 定时事件代码 */
{

}


// 定时器
int Winks_api_StartTimer(unsigned long delay){
	// 定时器不自动重新启动
	struct itimerval value;
	value.it_value.tv_sec = delay / 1000;
	value.it_value.tv_usec = (delay % 1000) * 1000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	//???:
	signal(SIGALRM, timefunc); /* 捕获定时信号 */
	setitimer(ITIMER_REAL, &value, NULL); /* 定时开始 */
	
	return 0;
}

int Winks_api_StopTimer(void){
	struct itimerval value;
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	if(setitimer(ITIMER_REAL, &value, NULL) == -1)
	{
		perror("stoptimer");
	}
	printf("stoptimer call end\n");
	
	return 0;
}

/**************************************************************************\
 功能描述:
 获取开机到现在的tick
 参数说明:
 无
 返回值:
 开机到现在的tick值
 实现描述:
 使用clock函数得到当前进程（deamon进程所以开机即会运行）运行的clock数。
 \**************************************************************************/
WINKS_TICKS Winks_GetSysTick(void)
{
	WINKS_TICKS t = clock();
    Winks_printf("Winks_GetSysTick %d\n", t);
    return t;
}


/**************************************************************************\
 功能描述:
 获取系统当前的设置语言
 参数说明:
 无
 返回值:
 返回WINKS平台统一定义的语言代码
 实现描述:
 \**************************************************************************/
Winks_Language_s Winks_GetCurrentLanguage(void){
	NSString *lang;
	Winks_Language_s ret_lang;
	lang = GetCurrentLanguage();
	if([lang isEqualToString:@"en"]){
		ret_lang = WINKS_LANG_ENGLISH;
	}else if([lang isEqualToString:@"zh-Hans"]){ // 简体中文
		ret_lang = WINKS_LANG_CHINESE;
	}else if([lang isEqualToString:@"zh-Hant"]){ // 繁体中文
		ret_lang = WINKS_LANG_CHINESETW;
	}

	return ret_lang;
}

// 系统当前语言
- (NSString *)GetCurrentLanguage(){
	NSArray* languages = [NSLocale preferredLanguages];
	
	return [languages objectAtIndex:0];
}

/**************************************************************************\
 功能描述:
 获取系统当前的漫游状态
 参数说明:
 flag   out参数，当前漫游则返回1，否则返回0
 返回值:
 成功返回  WINKS_RETURN_SUCCESS
 失败返回  WINKS_RETURN_FAILURE
 实现描述:
 \**************************************************************************/
int Winks_GetRoamingFlag(unsigned char *flag){
	
}

/**************************************************************************\
 功能描述:
 获取系统当前的屏幕背光时间
 参数说明:
 val   out参数，返回背光时间，单位为s
 返回值:
 成功返回  WINKS_RETURN_SUCCESS
 失败返回  WINKS_RETURN_FAILURE
 实现描述:
 \**************************************************************************/
int Winks_GetBgLightTime(unsigned long *val){

}