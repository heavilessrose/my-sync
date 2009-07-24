/*
 *  wk_api.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/17/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_api.h"

#pragma mark 定时器
const unsigned long KMaxTimerOut = 30*60*1000;//30 min

void timefunc(int sig) /* 定时事件代码 */
{
	
}


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

#pragma mark 系统信息
/**************************************************************************\
 功能描述:
 获取当前系统日期、时间
 参数说明:
 无
 返回值:
 无
 实现描述:
 \**************************************************************************/
int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p){
	if (NULL == dateTime_p){
		return WINKS_RETURN_FAILURE;
	}
	
	NSCalendar *gregorian = [[NSCalendar alloc]
							 initWithCalendarIdentifier:NSGregorianCalendar];
	NSDate *date = [NSDate date];
	
	unsigned unitFlags = NSYearCalendarUnit
	| NSMonthCalendarUnit 
	| NSDayCalendarUnit
	| NSHourCalendarUnit
	| NSMinuteCalendarUnit
	| NSSecondCalendarUnit
	| NSWeekdayCalendarUnit;
	NSDateComponents *dateComponents = [gregorian components:unitFlags fromDate:date];
	
	unsigned long year = [dateComponents year];
	unsigned char month = [dateComponents month];
	unsigned char day = [dateComponents day];
	unsigned char hour = [dateComponents hour];
	unsigned char minute = [dateComponents minute];
	unsigned char second = [dateComponents second];
	unsigned char weekday = [dateComponents weekday];
	//???:释放有问题？
	//	[gregorian release];
	//	[dateComponents release];
	//	[date release];
	
	//	printf("year = %d\n", year);
	//	printf("month = %d\n", month);
	//	printf("day = %d\n", day);
	//	printf("hour = %d\n", hour);
	//	printf("minute = %d\n", minute);
	//	printf("second = %d\n", second);
	//	printf("weekday = %d\n", weekday);
	
	if(weekday == 1)
		weekday = 0;
	else
		weekday -= 1;
	
	dateTime_p->year = year;
	dateTime_p->month = month;
	dateTime_p->day = day;
	dateTime_p->week = weekday;
	
	dateTime_p->hour = hour;
	dateTime_p->min = minute;
	dateTime_p->sec = second;
	return WINKS_RETURN_SUCCESS;
}

/**************************************************************************\
 功能描述:
 获取开机到现在的tick 开机后经过的毫秒数
 参数说明:
 无
 返回值:
 开机到现在的tick值
 实现描述:
 使用clock函数得到当前进程（deamon进程所以开机即会运行）运行的clock数。
 \**************************************************************************/
WINKS_TICKS Winks_GetSysTick(void)
{
	//FIXME: 得到调用进程运行的clock
	WINKS_TICKS t = clock();
	
	unsigned long ms = t / (CLOCKS_PER_SEC / 1000);
    Winks_printf("Winks_GetSysTick %d\n", t);
    return ms;
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
 获取系统的当前时区
 参数说明:
 time_zone  out参数，返回WINKS平台统一定义的时区值
 返回值:
 成功返回  WINKS_RETURN_SUCCESS
 失败返回  WINKS_RETURN_FAILURE
 实现描述:
 \**************************************************************************/
int Winks_GetSysTimeZone(Winks_TimeZone_e *time_zone){
	*time_zone = (Winks_TimeZone_e)(WINKS_TIME_ZONE_ZERO_TIMEZONE + getTimeZoneOffset());
	return WINKS_RETURN_SUCCESS;
}

// Local Time Zone [Asia/Shanghai (CST) offset 28800]
// 返回与0时区偏移了几个时区， 向东＋，向西－
// 时区变化时需resettimezone
static int getTimeZoneOffset(){
	NSTimeZone * zone = [NSTimeZone systemTimeZone];
	NSLog([zone description]);
	
	NSInteger offsetSec = [zone secondsFromGMT];
	
	printf("\n%d, %d\n", offsetSec, );
	
	return (offsetSec / 3600);
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
	return WINKS_RETURN_FAILURE;
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
	return WINKS_RETURN_FAILURE;
}

/**************************************************************************\
 功能描述:
 获取汉字字符对应的拼音字符串
 参数说明:
 str        in   汉字字符
 pyBuff     out  拼音字符串
 pyBuffSize      pyBuff的大小
 返回值:
 成功返回  WINKS_RETURN_SUCCESS
 失败返回  WINKS_RETURN_FAILURE
 实现描述:
 \**************************************************************************/
int Winks_GetPinyin(char *str, char *pyBuff, unsigned long pyBuffSize){
	return WINKS_RETURN_FAILURE;
}

/**************************************************************************\
 功能描述:
 判断sim卡是否可用
 参数说明:
 sim_card_id    1:表示sim卡1,  2:表示sim卡2
 返回值:
 可用    返回非0
 不可用  返回0
 实现描述:
 \**************************************************************************/
int Winks_isSimEnabled(int sim_card_id){
	return 0;
}

/**************************************************************************\
 功能描述:
 获取sim卡1的imsi号
 参数说明:
 无
 返回值:
 成功返回 WINKS_RETURN_SUCCESS
 失败返回 WINKS_RETURN_FAILURE
 
 实现描述:
 \**************************************************************************/
int Winks_api_GetImsi(void){
	return 0;
}

/**************************************************************************\
 功能描述:
 获取sim卡2的imsi号
 参数说明:
 无
 返回值:
 成功返回 WINKS_RETURN_SUCCESS
 失败返回 WINKS_RETURN_FAILURE
 
 实现描述:
 调用展讯平台的MNSIM_GetImsiEx接口获取imsi号。
 然后调用上层抽象层的wk_osal_DoWaitingImsi_2函数通知上层
 这里总是返回成功
 \**************************************************************************/
#ifdef WINKS_DUAL_SIM
int Winks_api_GetImsi_2(void){
	return 0;
}
#endif

/**************************************************************************\
 功能描述:
 设置联网用的sim卡号
 参数说明:
 SimNo    sim卡号，值为1或2
 SaveData  抽象层需要保存的信息，
 在调用Winks_api_RestoreNetLinkSim时该值会被传给抽象层
 
 返回值:
 成功返回 WINKS_RETURN_SUCCESS
 失败返回 WINKS_RETURN_FAILURE
 
 实现描述:
 将SimNo保存到抽象层全局变量结构体中,在socket联网的时候，要激活该
 SimNo对应的gprs pdp context
 SaveData没有用到.因为展讯平台同时只能有一个gprs pdp context,
 如果当前已经在联网,则不能中止当前联网,而只能是也使用当前的联网pdp
 \**************************************************************************/
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData){
	return 0;
}

/**************************************************************************\
 功能描述:
 恢复手机默认联网用的Sim卡
 参数说明:
 SaveData   在Winks_api_SetNetLinkSim(int SimNo, int *SaveData)函数
 中通过SaveData传给上层的数值
 返回值:
 成功返回 WINKS_RETURN_SUCCESS
 失败返回 WINKS_RETURN_FAILURE
 
 实现描述:
 因为展讯平台不会改变当前手机的默认值.因此该函数体为空
 \**************************************************************************/
int Winks_api_RestoreNetLinkSim(int SaveData){
	return 0;
}

/**************************************************************************\
 功能描述:
 获取当前USB状态
 参数说明:
 val   out  在使用val返回1，没有使用val返回0
 返回值:
 1:系统插了USB, 0:系统未插USB.
 返回值和*val的值相同		
 实现描述:
 \**************************************************************************/
int Winks_GetUsbState(unsigned long *val){
	*val = 0;
	return 0;
}


#pragma mark 字符转换
int Winks_Ascii2UnicodeString(const char *in_ascii, char *out_unicode, unsigned int size)
{
    unsigned char *src, *dest;
	
    if (in_ascii == NULL || out_unicode == NULL || size < 4)
        return -1;
	
    memset(out_unicode, 0, size);
    src = (unsigned char *)in_ascii;
    dest = (unsigned char *)out_unicode;
	
    while (*src != 0 && *src < 0x80)
    {
        if ((char *)dest > out_unicode + size - 4)
            break;
		
		//这里要区分big_endlian和little_endlian了
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        dest[0] = 0x00;
        dest[1] = *src;
#else
		dest[0] = *src;
        dest[1] = 0x00;
#endif
        
		
        src++;
        dest += 2;
    }
    return ((char *)src - in_ascii);
}

int Winks_Unicode2AsciiString(const char *in_unicode, char *out_ascii, unsigned int size){
	unsigned char *src, *dest;
	
    if (in_unicode == NULL || out_ascii == NULL || size < 2)
        return -1;
	
    memset(out_ascii, 0, size);
	//这里要区分big_endlian和little_endlian了
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
	src = (unsigned char *)(in_unicode+1);
#else
	src = (unsigned char *)in_unicode;
#endif
    
    dest = (unsigned char *)out_ascii;
	
    while (*src != 0 && *src < 0x80 && *(src + 1) == 0)
    {
        if ((char *)dest > out_ascii + size - 2)
            break;
		
        *dest = *src;
		
        src += 2;
        dest++;
    }
	
    return ((char *)dest - out_ascii);
}

int Winks_Unicode2UtfString(const char *in_unicode, char *out_utf8, unsigned int size){
	unsigned char *src, *dest;
    unsigned short unicode;
    unsigned int left_bytes;
	
    if (in_unicode == NULL || out_utf8 == NULL || size < 2)
        return -1;
	
    memset(out_utf8, 0, size);
    src = (unsigned char *)in_unicode;
    dest = (unsigned char *)out_utf8;
	
    while (src[0] || src[1])    
    {
        left_bytes = out_utf8 + size - (char *)dest;
		
		//这里要区分big_endlian和little_endlian了
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
		unicode = (unsigned short)( (src[0] << 8) | src[1] );
#else
		unicode = (unsigned short)( (src[1] << 8) | src[0] );
#endif
        
		
        if (unicode < 0x80)     
        {
            if (left_bytes < 2) break;
			
            *dest = (unsigned char)unicode;
			
            src += 2;
            dest++;
        }
        else if (unicode < 0x800)   /* 两字节编码 */
        {
            if (left_bytes < 3) break;
			
            dest[0] = (unsigned char)((unicode >> 6) | 0xC0);
            dest[1] = (unsigned char)((unicode & 0x3F) | 0x80);
			
            src += 2;
            dest += 2;
        }
        else if (unicode < 0xFFFF)  /* 三字节编码 */
        {
            if (left_bytes < 4) break;
			
            dest[0] = (unsigned char)((unicode >> 12) | 0xE0);
            dest[1] = (unsigned char)(((unicode >> 6) & 0x3F) | 0x80);
            dest[2] = (unsigned char)((unicode & 0x3F) | 0x80);
			
            src += 2;
            dest += 3;
        }
        else        /* 三字节以上的忽略 */
            break;
    }	
    return ((char *)src - in_unicode) / 2;
}

int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size){
	unsigned char *src, *dest;
    unsigned short unicode;
    unsigned int left_bytes;
	uint8 high_byte_index;
	uint8 low_byte_index;
	
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
	high_byte_index = 0;
	low_byte_index = 1;
#else
	high_byte_index = 1;
	low_byte_index = 0;
#endif
	
    if (in_utf8 == NULL || out_unicode == NULL || size < 4)
        return -1;
	
    memset(out_unicode, 0, size);
    src = (unsigned char *)in_utf8;
    dest = (unsigned char *)out_unicode;
	
    while (*src != 0)
    {
        left_bytes = out_unicode + size - (char *)dest;
        if (left_bytes < 4)
            break;
		
        if ((*src & 0x80) == 0)  /* 一字节编码 */
        {
			//这里要区分big_endlian和little_endlian了
            dest[low_byte_index] = *src;
            dest[high_byte_index] = 0x00;
			
            src++;
            dest += 2;
        }
        else if ((*src & 0xE0) == 0xC0)  /* 两字节编码 */
        {
            unicode = (unsigned short)( ((src[0] & 0x1F) << 6) | (src[1] & 0x3F) );
			//这里要区分big_endlian和little_endlian了
            dest[low_byte_index] = (unsigned char)(unicode & 0xFF);
            dest[high_byte_index] = (unsigned char)(unicode >> 8);
			
            src += 2;
            dest += 2;
        }
        else if ((*src & 0xF0) == 0xE0)  /* 三字节编码 */
        {
            unicode = (unsigned short)( ((src[0] & 0x0F ) << 12) | ((src[1] & 0x3F) << 6) | (src[2] & 0x3F) );
			//这里要区分big_endlian和little_endlian了
            dest[low_byte_index] = (unsigned char)(unicode & 0xFF);
            dest[high_byte_index] = (unsigned char)(unicode >> 8);
			
            src += 3;
            dest += 2;
        }
        else    /* 三字节以上的忽略 */
            break;
    }	
    return ((char *)dest - out_unicode) / 2;
}

unsigned short Winks_htons(unsigned short port ){
	unsigned short ret_port;
    ret_port =  htons(port);
	return ret_port;
}


#pragma mark 与平台有间接关系的代码
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *     与平台无关(不直接调用平台的函数)，但与平台有间接关系的代码
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define WINKS_OSAPI_CONTEXT_DEF(osApiPtr)   \
winks_OsApiContext_s *osApiPtr = (winks_OsApiContext_s *)Winks_GetOsalApiPtr((void**)0)
#ifdef HUAXMTEST
static    winks_OsApiContext_s *osApiPtr = NULL;
#endif
/*置抽象层控制位*/
int winks_SetOsApiBitOpt(unsigned long bit, unsigned long val)
{
    WINKS_OSAPI_CONTEXT_DEF(osApiPtr);
    if(osApiPtr == NULL)
    {
        Winks_fail("osApiPtr != NULL");
        return WINKS_RETURN_FAILURE;
    }
    Winks_assert(val <= 1);
    Winks_assert(bit < 8);
    Winks_SetBitFlag(bit, val, (void*)&(osApiPtr->bitOpt));
    return WINKS_RETURN_SUCCESS;
}

/*读取抽象层控制位*/
int winks_GetOsApiBitOpt(unsigned long bit, unsigned long *val)
{
    WINKS_OSAPI_CONTEXT_DEF(osApiPtr);
    if(osApiPtr == NULL)
    {
        Winks_fail("osApiPtr != NULL");
        return WINKS_RETURN_FAILURE;
    }
    Winks_assert(val != NULL);
    Winks_assert(bit < 8);
    Winks_GetBitFlag(bit, val, (void*)&(osApiPtr->bitOpt));
    return WINKS_RETURN_SUCCESS;
}

/* opt: 0--不输出，1--输出到文件，2--输出Trace，3--输出文件和Trace */
void Winks_printf2file(int opt)
{
    if(!opt)
        return;
	////
	const char *path = [@"/debug.txt" fileSystemRepresentation];
	// Specify stderr writes to a file (truncating contents first) 
	freopen(path, "w"/*"a"*/, stderr);
	////
	return;
}

#pragma mark 接入点
/* 通过ID 取接入点的名称, unicode 编码 */
int Winks_GetApnName(unsigned long account_id, char *apn_name, unsigned long len){return 0;}		// NO USE
/* 通过ID 获取接入点名称, unicode 编码 */
int Winks_GetDataAccountName(unsigned long account_id, char *data_account, unsigned long len){return 0;}

#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
/* 该函数强行在系统的接入点列表中设置指定的接入点方式，如果系统已有则修改成指定的接入点，如果没有则新增一个。
 
 * function: Winks_api_SysAddDataAccount
 
 * parameters:
 
 *   unsigned long defIndex: 默认接入点序列号。
 
 *   char *apn: 代理服务器IP
 
 *   char *proxyIp: 代理服务器IP
 
 *   unsigned short proxyPort: 代理服务器端口
 
 * return:
 
 *   成功返回接入点的序列号(句柄)
 
 *   失败返回:WINKS_INVALID_DATA_ACCOUNT_INDEX
 
 */
unsigned long Winks_api_SysAddDataAccount(unsigned long defIndex, char *apn, char* proxyIp, unsigned short proxyPort)
#else
unsigned long Winks_api_SysAddDataAccount(unsigned long defIndex,char *apn)
#endif
{
	return 0;
}

#pragma mark 短信
// 发送短信
int Winks_api_SendSmsUni(char *number, char * buff)
{
	if(number == NULL || sizeof(number) == 0){
		return -1;
	}
	if(buff == NULL || sizeof(buff) == 0){
		return -1;
	}

	char code[4]; //FIXME: 干嘛的
	return sendmessage(number,buff,code,NULL) == 0;
}

#pragma mark 互斥量
/**************************************************************************\
 功能描述:
 获取互斥量  
 参数说明:
 mutex_id    互斥量ID,
 可能的值如下
 #define WINKS_MUTEX_CHN 1
 #define WINKS_MUTEX_MSG 2
 #define WINKS_MUTEX_TIMER 3
 返回值:
 成功返回WINKS_RETURN_SUCCESS
 失败返回WINKS_RETURN_FAILURE
 实现描述:
 抽象层初始化的时候创建三个互斥量
 \**************************************************************************/
int Winks_api_GetMutex(int mutex_id){
    if (1 > mutex_id || 3 < mutex_id) {
		return WINKS_RETURN_FAILURE;
    }
	//???:如何组织此三个互斥量
    if (SCI_SUCCESS == )) {
		return WINKS_RETURN_FAILURE;
    }
	return WINKS_RETURN_SUCCESS;    
}

/**************************************************************************\
 功能描述:
 释放互斥量  
 参数说明:
 mutex_id    互斥量ID,
 可能的值如下
 #define WINKS_MUTEX_CHN 1
 #define WINKS_MUTEX_MSG 2
 #define WINKS_MUTEX_TIMER 3
 返回值:
 成功返回WINKS_RETURN_SUCCESS
 失败返回WINKS_RETURN_FAILURE
 实现描述:
 \**************************************************************************/
int Winks_api_PutMutex(int mutex_id){
	
	return WINKS_RETURN_SUCCESS;   
}

#pragma mark wap
//调用平台wap浏览器访问指定地址
int Winks_api_OpenWapPage( char* pUrl ){
	return 0;
}

#pragma mark 取时差
/*取时间差函数*/
int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result)
{
    unsigned long s1, s2, s3;
	
    Winks_assert(t1 && t2 && result);
	
    s1 = t1->hour * 60 * 60 + t1->min * 60 + t1->sec;
    s2 = t2->hour * 60 * 60 + t2->min * 60 + t2->sec;
	
    if (s1 > s2)
        s3 = s1 - s2;
    else
        s3 = s2 - s1;
	
    Winks_mem_set(result, 0, sizeof(Winks_DateTime_s));
    result->hour = (unsigned char)( s3 / (60 * 60) );
    result->min = (unsigned char)( s3 / 60 - result->hour * 60 );
    result->sec = (unsigned char)( s3 - result->hour * 60 * 60 - result->min * 60 );
	
    return WINKS_RETURN_SUCCESS;
}


#pragma mark -
// ppc平台为初始化电话监控
int winks_api_osinit(void){
	
}

// ppc平台为关闭电话监控
int winks_api_osdestroy(void){
	
}


int winks_api_PostMessage(void){
	
}