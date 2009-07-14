/*
 *  wk_timer.h
 *  TabiNavi
 *
 *  Created by wang luke on 7/14/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

typedef unsigned long WINKS_TIMER_ID;
#define WINKS_TIMER_NULL ((WINKS_TIMER_ID)0)

#define WINKS_MILLISECONDS2TICKS(S)  ((WINKS_TICKS)(S))
#define WINKS_SECONDS2TICKS(S)  ((WINKS_TICKS)((S)*1000))
#define WINKS_TICKS2MILLISECONDS(S)  ((S))
#define WINKS_TICKS2SECONDS(S)  ((S) / 1000)

#define WINKS_TIMER_DEFAULT    0x0000    /*默认属性*/
#define WINKS_TIMER_AUTO_START 0x0001    /*定时器创建后自动启动*/
#define WINKS_TIMER_AUTO_LOOP  0x0002    /*定时器超时后重新启动*/

typedef struct tag_winks_TimerBody_s{
    WINKS_CHN_ID chn;
    WINKS_TICKS timeOutPeriod;
    WINKS_TICKS timeOut;
    unsigned long flag;
    unsigned char state;            /*1:running , 0:stopped*/
}winks_TimerBody_s;

/** 创建定时器 **/
WINKS_TIMER_ID Winks_CreateTimer(WINKS_CHN_ID chn, WINKS_TICKS timeOutPeriod, unsigned long flag);

/** 注销定时器 **/
int Winks_DestroyTimer(WINKS_TIMER_ID timerId);

/** 启动定时器 **/
int Winks_StartTimer(WINKS_TIMER_ID timerId);

/** 停止定时器 **/
int Winks_StopTimer(WINKS_TIMER_ID timerId);

/** 取定时器信息 **/
int Winks_GetTimerInfo(WINKS_TIMER_ID timerId, winks_TimerBody_s *timerInfo);