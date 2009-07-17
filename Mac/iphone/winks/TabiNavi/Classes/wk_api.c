/*
 *  wk_api.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/17/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_api.h"
#include <sys/time.h>
#include <signal.h>

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


// 系统心跳数， 与ms进行换算
WINKS_TICKS Winks_GetSysTick(void)
{
#ifdef EKA2
	WINKS_TICKS t = User::NTickCount();
#else
	WINKS_TICKS t = User::TickCount();
    Winks_printf("Winks_GetSysTick %d\n", t);
#endif
    return t;
}
