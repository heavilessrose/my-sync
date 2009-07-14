/*
 *  wk_timer.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/14/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_timer.h"
#include <sys/select.h>
#include <sys/itimer.h>
#include <sys/time.h>

static winks_TimerBody_s winks_TimerBody

/**********************************************************\
 分配定时器资源，创建一个新的定时器；
 根据输入参数判断是否启动定时。
 说明：
 为了保证在不同平台之间的移植时引擎代码可重用，抽象层对定时器模块做了统一的封装。
 抽象层定时器模块会统一管理引擎和UI对定时器的调用，通过内部的管理和计算，最终通过调用平台提供的一个定时器来完成功能。
 定时器超时后以消息的形式通知定时器的使用者。
 
 WINKS_CHN_ID chn 返回的消息通道；
 unsigned long timeOutPeriods 定时器的定时周期（单位WINKS_TICKS），可通过以下宏定义进行毫秒、秒与TICK的转换
 #define WINKS_MILLISECONDS2TICKS(S)  ((WINKS_TICKS)((S) / 16))
 #define WINKS_SECONDS2TICKS(S)  ((WINKS_TICKS)(((S) * 125L) / 2))
 #define WINKS_TICKS2MILLISECONDS(S)  ((S) * 16)
 #define WINKS_SECONDS2TICKS(S)  (((S) * 2L) / 125)；
 unsigned long flag 
 是定时器的控制参数，每一位代表一个控制项，具体如下：
 #define WINKS_TIMER_DEFAULT    0x0000    //默认属性
 #define WINKS_TIMER_AUTO_START 0x0001    //定时器创建后自动启动
 #define WINKS_TIMER_AUTO_LOOP  0x0002    //定时器超时后重新启动
 返回
 返回消息WINKS_MSG_TIMER_EXPIRY；消息内容WINKS_TIMER_ID。
 \*********************************************************/
WINKS_TIMER_ID Winks_CreateTimer(WINKS_CHN_ID chn, unsigned long timeOutPeriod, unsigned long flag){
	
//	winks_TimerBody.chn = chn;
//	winks_TimerBody.timeOutPeriod = timeOutPeriod;
//	winks_TimerBody.flag = flag;
//	winks_TimerBody.state = 0; // stoped
	
	// 定时器不自动重新启动
	struct itimerval timerval;
	timerval.it_value.tv_sec = timeOutPeriod / 1000;
	timerval.it_value.tv_usec = (timeOutPeriod % 1000) * 1000;
	timerval.it_interval.tv_sec = 0;
	timerval.it_interval.tv_usec = 0;
	if(setitimer(ITIMER_PROF, timerval, NULL) == -1)// 开始计时
	{
		perror("setitimer");
	}
}

/**********************************************************\
 释放指定的定时器资源
 \*********************************************************/
int Winks_DestroyTimer(WINKS_TIMER_ID timerId){
	
}


/**********************************************************\
 由于定时器模块共用一个平台的定时器，故这里需要对彩像软件自己的所有定时器做统一的管理，以最快到时的定时器为准启动平台的定时器。
 调用Winks_api_StartTimer函数，触发平台的定时器启动，需要抽象层移植人员实现。
 调用Winks_api_StopTimer函数，触发平台的定时器停止，需要抽象层移植人员实现。
 在Winks_CreateOsalHandler中被调用，目的是为了完成一些与平台相关的操作，如平台消息通道注册、平台定时器的申请等。可以为空函数。
 \*********************************************************/
int Winks_StartTimer(WINKS_TIMER_ID timerId){
	
}

/**********************************************************\
 停止定时器, 将定时器的启动标志，置为停止.
 在Winks_CreateOsalHandler中被调用，目的是为了完成一些与平台相关的操作，如平台消息通道注册、平台定时器的申请等。可以为空函数。
 \*********************************************************/
int Winks_StopTimer(WINKS_TIMER_ID timerId){
	
}
