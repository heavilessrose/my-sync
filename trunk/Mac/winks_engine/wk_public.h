/*
File:		wk_public.h
Author:		Huaxm
Create Time:18/01/2008
*/
#ifndef _WK_PUBLIC_H_
#define _WK_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WINKS_SIMULATOR
#pragma warning(disable : 4100)    /* unreferenced formal parameter */
#pragma warning(disable : 4505) /* unreferenced local function has been removed */
#endif

#ifndef _WK_PROJECT_H_
#include "wk_project.h"
#endif

#ifndef _WK_OSPUB_H_
#include "wk_ospub.h"
#endif


/*PUBLIC*/
#define WINKS_ARRAY_SIZE(x)    (sizeof(x)/sizeof(x[0]))     /*取数组元素个数*/

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MAX
#define MAX(x, y)               (((x) > (y))?(x):(y))       /*取最大值*/
#endif

#ifndef MIN
#define MIN(x, y)               (((x) < (y))?(x):(y))       /*取最小值*/
#endif

#ifndef ABS
#define ABS(x)                  (((x)<0) ? -(x) : (x))      /*取绝对值*/
#endif

#define Winks_offsetof(type, mem) ((size_t) ((char*)&((type*)0)->mem - (char*)((type*)0)))

#define WINKS_URL_MAX_LEN       512                          /*RUL最大长度*/
#define WINKS_SIMPLE_URL_MAX_LEN        64                  /*简单RUL最大长度*/
#define WINKS_DATA_ACCOUNT_MAX_LEN      16                  /*接入点最大长度*/
#define WINKS_IP_ADDR_LEN               16                  /*代理服务器设置接口*/
#define WINKS_PATH_MAX          256                          /*定义文件绝对路径最大长度*/
#define WINKS_NAME_MAX          64                           /*定义文件名最大长度(含扩展名)*/
#define WINKS_VERSION_MAX_LEN           20                  /*版本号最大长度*/

#define WINKS_PRIOR_IMAGE_MAX_NUMBER    2

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

/*Winks内部Tick相关的转换宏*/
#define WINKS_MILLISECONDS2TICKS(S)  ((WINKS_TICKS)(S))
#define WINKS_SECONDS2TICKS(S)  ((WINKS_TICKS)((S)*1000))
#define WINKS_TICKS2MILLISECONDS(S)  ((S))
#define WINKS_TICKS2SECONDS(S)  ((S) / 1000)

/*位运算*/
#define WINKS_BIT_FALG_D0   0x01
#define WINKS_BIT_FALG_D1   0x02
#define WINKS_BIT_FALG_D2   0x04
#define WINKS_BIT_FALG_D3   0x08
#define WINKS_BIT_FALG_D4   0x10
#define WINKS_BIT_FALG_D5   0x20
#define WINKS_BIT_FALG_D6   0x40
#define WINKS_BIT_FALG_D7   0x80
#define WINKS_BIT_FALG_D8   0x0100
#define WINKS_BIT_FALG_D9   0x0200
#define WINKS_BIT_FALG_D10  0x0400
#define WINKS_BIT_FALG_D11  0x0800
#define WINKS_BIT_FALG_D12  0x1000
#define WINKS_BIT_FALG_D13  0x2000
#define WINKS_BIT_FALG_D14  0x4000
#define WINKS_BIT_FALG_D15  0x8000
#define WINKS_BIT_FALG_D16  0x00010000
#define WINKS_BIT_FALG_D17  0x00020000
#define WINKS_BIT_FALG_D18  0x00040000
#define WINKS_BIT_FALG_D19  0x00080000
#define WINKS_BIT_FALG_D20  0x00100000
#define WINKS_BIT_FALG_D21  0x00200000
#define WINKS_BIT_FALG_D22  0x00400000
#define WINKS_BIT_FALG_D23  0x00800000
#define WINKS_BIT_FALG_D24  0x01000000
#define WINKS_BIT_FALG_D25  0x02000000
#define WINKS_BIT_FALG_D26  0x04000000
#define WINKS_BIT_FALG_D27  0x08000000
#define WINKS_BIT_FALG_D28  0x10000000
#define WINKS_BIT_FALG_D29  0x20000000
#define WINKS_BIT_FALG_D30  0x40000000
#define WINKS_BIT_FALG_D31  0x80000000


/*Return code*/
#define WINKS_RETURN_SUCCESS    (0)
#define WINKS_RETURN_FAILURE    (-1)
#define WINKS_RETURN_WAIT_MSG   (1)

/* 文件类型 */
typedef enum tag_WinksFileExt_e
{
    WINKS_FILE_EXT_none = 0,    /* 空 */
    WINKS_FILE_EXT_xml,         /* .xml */
    WINKS_FILE_EXT_gif,         /* .gif */
    WINKS_FILE_EXT_jpg,         /* .jpg */
    WINKS_FILE_EXT_png,         /* .png */
    WINKS_FILE_EXT_tiff,        /* .tiff */
    WINKS_FILE_EXT_spl,         /* .spl */
    WINKS_FILE_EXT_swf,         /* .swf */
    WINKS_FILE_EXT_avi,         /* .avi */
    WINKS_FILE_EXT_bin          /* .bin */
}WinksFileExt_e;

typedef struct tag_Winks_MemPool_s
{
	struct tag_Winks_MemPool_s* pnext;	//链表指针
	int len;							//data len
	int offset;							//data offset
	char* pdata;						//数据指针
}Winks_MemPool_s;

/*ASSERT*/
#ifdef WINKS_CHECK_ASSERTIONS
#define Winks_assert(cOND)                           \
	{                                                \
	if (!(cOND))                                     \
	{                                                \
	Winks_printf("Assert Fail: %s, #%d, (%s)\n",     \
		__FILE__, __LINE__, #cOND);                  \
	Winks_assertfail(0,#cOND, __FILE__, __LINE__);   \
	}                                                \
	}
#define Winks_fail(msg)                              \
	{                                                \
	Winks_printf("Assert Fail: %s, #%d, (%s)\n",     \
		__FILE__, __LINE__, (char *)msg);            \
	Winks_assertfail(1,msg, __FILE__, __LINE__);     \
	}
#else /*WINKS_CHECK_ASSERTIONS*/
#define Winks_assert(cOND)
#define Winks_fail(msg)
#endif /*WINKS_CHECK_ASSERTIONS*/

#if (defined  WINKS_SIMULATOR) && (!defined _VA_LIST_DEFINED)
	typedef char *  va_list;
	#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
	#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )
	#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
	#define va_end(ap)      ( ap = (va_list)0 )

#elif defined  __ARM__
	typedef int *va_list[1];
	#define va_start(ap, parmN) (void)(*(ap) = __va_start(parmN))
	#define va_arg(ap, type) __va_arg(*(ap), type)
	#define va_copy(dest, src) ((void)(*(dest) = *(src)))
	#define va_end(ap) ((void)(*(ap) = 0))
#endif


#ifndef _SIZE_T_DEFINED
    typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#define WINKS_DB_FILE           WINKS_APP_PATH"debugf.txt"
#define WINKS_MEMDBG_FILE       WINKS_APP_PATH"memdbg.txt"
#define WINKS_STAT_FILE         WINKS_APP_PATH"stat.txt"

#define WINKS_SKIN_FILE_PATH    WINKS_APP_PATH"image"

#ifdef __cplusplus
}
#endif

#endif/*End _WK_PUBLIC_H_*/
