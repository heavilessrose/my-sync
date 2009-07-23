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

#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include "device.h" // 短信

#ifdef __cplusplus
extern "C" {
#endif
	
// 调试	
#define debug(format, ...) CFShow([NSString stringWithFormat:format, ## __VA_ARGS__]);
	
#ifdef WINKS_SUPPORT_MEMORY_CARD
	int Winks_IsMemoryCardExist(void);
	int Winks_GetMemoryCardDriver(char *drv, unsigned int len);
	int Winks_GetMemoryCardSpace(Winks_FlashSpace_MB_s *space);
#endif

	/*取背光时间*/
	int Winks_GetBgLightTime(unsigned long *val);
	
	/*取好友列表的排序规测代码，中文为拼音排序，英文为字母排序*/
	int Winks_GetPinyin(char *str, char *pyBuff, unsigned long pyBuffSize);
	
	/*取USB状态。1:系统插了USB, 0:系统未插USB.*/
	int Winks_GetUsbState(unsigned long *val);
	
	/* NOTE: MTK上name编码为unicode, 转换为UTF-8 */
	void Winks_GetPhonebookName(const char *friend_no, char *friend_name, unsigned long len);
	
	// 定时器
	int Winks_api_StartTimer(unsigned long delay);
	int Winks_api_StopTimer(void);

	// 系统心跳数, 与ms进行换算, 得到系统启动运行了多少毫秒
	WINKS_TICKS Winks_GetSysTick(void);
//	
//	/* 接听电话 */
//	void Winks_AcceptCall(void);
//	
//	/* 拒接电话 */
//	void Winks_RejectCall(void);
//	
//	/* 挂断电话 */
//	void Winks_HangupCall(void);
	
	/* 通过ID 取接入点的名称, unicode 编码 */
	int Winks_GetApnName(unsigned long account_id, char *apn_name, unsigned long len);
	
	/* 通过ID 获取账号名称, unicode 编码 */
	int Winks_GetDataAccountName(unsigned long account_id, char *data_account, unsigned long len);
	
	int Winks_api_GetImei(void);
#ifdef WINKS_DUAL_SIM
	int Winks_api_GetImsi_2(void);
#endif /* WINKS_DUAL_SIM */
	
	int winks_api_osinit(void);
	
	int winks_api_osdestroy(void);
	
	int winks_api_PostMessage(void);
	
	int Winks_api_SendSmsUni(char *number, char * buff);
	
	void UnicodeToUtf8(CHAR **pChDes,WCHAR *pwSrc);
	int Utf8ToUnicode(void *src8,unsigned long size8,unsigned short **src16,unsigned long *size16);
	
	/*取时间差函数*/
	int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result);
	//调用平台wap浏览器访问指定地址
	int Winks_api_OpenWapPage( char* pUrl );
	////////////////内部函数
	static int getTimeZoneOffset();
	// 系统当前语言
	- (NSString *)GetCurrentLanguage();
#ifdef __cplusplus
}
#endif

#endif/*_WK_API_H_*/