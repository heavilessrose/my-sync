/*
 *  wk_api.h
 *  TabiNavi
 *
 *  Created by wang luke on 7/17/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */
#ifndef _WK_API_H_
#define _WK_API_H_

#include "wk_public.h"
#include "wk_osfnc.h"

#ifdef __cplusplus
extern "C" {
#endif

// 定时器
extern int Winks_api_StartTimer(unsigned long delay);
extern int Winks_api_StopTimer(void);

// 系统心跳数， 与ms进行换算
WINKS_TICKS Winks_GetSysTick(void);
	

#ifdef __cplusplus
}
#endif

#endif/*_WK_API_H_*/