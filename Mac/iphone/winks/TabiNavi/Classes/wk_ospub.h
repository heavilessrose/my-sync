/**************************************************************************
* Winks Software Copyright (c) 2008 Winks Technologies, Inc.
* Licensed to Mobile Winks (China) Co., Ltd
**************************************************************************
*Id: wk_ospub.h,v 1.00 2008/06/03 03:05:49 
*Revision: 1.00 
*DateTime: 2008/06/03 11:11:37 
**************************************************************************    
* File Description
* ----------------
* Configuration about project. 
**************************************************************************/
#ifndef _WK_OSPUB_H_
#define _WK_OSPUB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************
            系统接口
**************************************************/
#if defined (WINKS_ARENA_V_1_2) || defined (WINKS_ARENA_V_1_4)
    #ifndef _APP_INCLUDE_FILES_H_
    #include "app_include_files.h"
    #endif
#elif defined (WINKS_MTK)
    #ifndef _STDC_H
    #include "stdC.h"
    #endif

    #if (!defined(_FILE_DEFINED) && !defined(__STDIO_DECLS))
    typedef struct filestr
    {
        int unused;
    }FILE;
    #endif
#elif defined (WINKS_BREW)
#include "AEEFile.h"
typedef IFile FILE;
#endif


/************************************************
                  WINKS_ARENA_V_1_2
************************************************/
#ifdef WINKS_ARENA_V_1_2
#define Winks_SysAssert tp_os_assert((BOOL)(0))
#define Winks_mem_cpy(dp,sp,size)          tp_os_mem_cpy((void *)(dp),(void *)(sp),size)
#define Winks_mem_set(dp,val,size)          tp_os_mem_set((void *)(dp),val,size)
#ifdef WINKS_DEBUG
void Winks_printf( char* format, ... );
void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree( void* point, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
void Winks_xsalPrintMemInfo(void);
int WK_IsMemCleanup(char* file);
#define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
#define Winks_salFree(x)       Winks_xsalFree( x, __FILE__, __LINE__ )
#define Winks_printf           Winks_printf
#define Winks_printHex(buff,len,head) HEX_DataOut(buff, len, head)
#else
#define Winks_printf(a)
#define Winks_printHex(buff,len,head)
#define Winks_salAlloc         tp_os_mem_malloc
#define Winks_salFree          tp_os_mem_free
#endif




/************************************************
                  WINKS_ARENA_V_1_4
************************************************/
#elif defined (WINKS_ARENA_V_1_4)
#include "os_api.h"
#define Winks_SysAssert tp_os_assert((BOOL)(0))
#define Winks_Alloc(size)                   tp_os_mem_malloc(size)
#define Winks_Free(ptr)                     tp_os_mem_free(ptr)
#define Winks_mem_cpy(dp,sp,size)          tp_os_mem_cpy((void *)(dp),(void *)(sp),size)
#define Winks_mem_set(dp,val,size)          tp_os_mem_set((void *)(dp),val,size)
#ifdef WINKS_DEBUG
void Winks_printf( char* format, ... );
void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree( void* point, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
void Winks_xsalPrintMemInfo(void);
int WK_IsMemCleanup(char* file);
#define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
#define Winks_salFree(x)          Winks_xsalFree( x, __FILE__, __LINE__ )
#define Winks_printf           Winks_printf
#define Winks_printHex(buff,len,head) HEX_DataOut(buff, len, head)
#else
#define Winks_printf(...)
#define Winks_printHex(buff,len,head)
#define Winks_salAlloc         Winks_Alloc
#define Winks_salFree          Winks_Free
#endif




/************************************************
                  WINKS_MTK
************************************************/
#elif defined (WINKS_MTK)
    #ifndef _STDC_H
    #include "stdC.h"
    #endif
    #ifndef _DEBUG_INT_H
    #include "DebugInitDef.h"
    #endif
#define Winks_SysAssert MMI_ASSERT(0)
#ifdef MMI_ON_WIN32
#define Winks_Alloc(size)                   malloc(size)
#define Winks_Free(ptr)                     free(ptr)
#else
#include "med_utility.h"
#define Winks_Alloc(size)                   (void *)med_alloc_ext_mem(size)
#define Winks_Free(ptr)                     med_free_ext_mem((void**)(&ptr))
#endif
#define Winks_mem_cpy(dp,sp,size)           memcpy(dp,sp,size)
#define Winks_mem_set(dp,val,size)          memset(dp,val,size)

#ifdef WINKS_DEBUG
void Winks_printf( char* format, ... );
/* opt: 0--不输出，1--输出到文件，2--输出Trace，3--输出文件和Trace */
void Winks_printf2file(int opt);
void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree(void* ptr, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
void Winks_xsalPrintMemInfo(void);
int WK_IsMemCleanup(char* file);
#define Winks_salAlloc(x)      Winks_xsalAlloc(x, __FILE__, __LINE__)
#define Winks_salFree(ptr)     Winks_xsalFree(ptr, __FILE__, __LINE__)
#define Winks_printf           Winks_printf
#define Winks_printHex(buff,len,head) HEX_DataOut(buff, len, head)
#else

#ifdef WINKS_SIMULATOR
void Winks_printf( char* format, ... );
#define Winks_printf(a)
#else
#define Winks_printf(...)
#endif

#define Winks_printHex(buff,len,head) 
#define Winks_salAlloc         Winks_Alloc
#define Winks_salFree          Winks_Free
#define WK_IsMemCleanup(x)
#endif



#if !defined (WINKS_MTK_V_0_7_b) || !defined (WINKS_SIMULATOR)
/* CLib API */
void *memcpy(void *dst0, const void *src0, unsigned int len0);
void *memmove(void *dst_void, const void *src_void, unsigned int length);
int memcmp(const void *m1, const void *m2, unsigned int n);
void *memchr(const void *src_void, int _c, unsigned int length);
void *memset(void *m, int c, unsigned int n);

char *strcat(char* dst, const char* src);
char *strncat(char *s1, const char *s2, unsigned int n);
int strcmp(const char* src, const char* dst);
int strncmp(const char* first, const char* last, unsigned int count);
char *strchr(const char* str, int c);
unsigned int strcspn(const char* str, const char* strCharSet);
char *strpbrk(const char* str, const char* strCharSet);
char *strrchr(const char* str, int c);
unsigned int strspn(const char* str, const char* strCharSet);
char *strstr(const char* str, const char* strCharSet);
char *strtok(char* strToken, const char* strDelimit);
unsigned int strlen(const char* str);
char *strupr(char *string);
int toupper(int c);
//int islower(int c);
char *strdup (const char *s);
//int isupper(int c);
int tolower(int c);
char *strtok_r(char *s, const char *delim, char **lasts);
void srand( unsigned int seed );
int rand( void );
int sprintf( char * pStr, const char *fmt, ... );
//int vsprintf(char * where, const char *fmt, va_list varg);
//int isdigit(int c);
//int isspace(int c);
//int isalpha(int c);
long strtol(const char *nptr, char **endptr, int base);
int atoi(const char *s);
char *itoa (int val, char *buf, int radix );
double atof(const char *s);
double sqrt( double x );
double pow( double x, double y );
long atol(const char *string);
double strtod (const char *nptr, char **endptr );
int Winks_stricmp(const char *s1, const char *s2);
int Winks_strnicmp(const char *s1, const char *s2, unsigned int n);
char *Winks_strcpy(char *dst0, const char *src0);
char *Winks_strncpy(char *dst0, const char *src0, unsigned int count);
#endif
#define stricmp   Winks_stricmp
#define strnicmp  Winks_strnicmp
#define strcpy    Winks_strcpy
#define strncpy   Winks_strncpy

#ifdef WINKS_SIMULATOR
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif







/************************************************
                  WINKS_BREW
************************************************/
#elif defined (WINKS_BREW)
#include "AEEStdLib.h"
#define Winks_SysAssert (*(int*)(0) = 1)
#define Winks_mem_cpy(dp,sp,size)      MEMCPY(dp,sp,size)
#define Winks_mem_set(dp,val,size)      MEMSET(dp,val,size)


#ifdef  WINKS_DEBUG
void  Winks_printf( char* format, ... );
void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree(void* ptr, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
void Winks_xsalPrintMemInfo(void);
int WK_IsMemCleanup(char* file);
#define Winks_salAlloc(x)            Winks_xsalAlloc(x, __FILE__, __LINE__)
#define Winks_salFree(ptr)           Winks_xsalFree(ptr, __FILE__, __LINE__)
#define Winks_printHex(buff,len,head) HEX_DataOut(buff, len, head)
#else 

#define Winks_printHex(buff,len,head)
#define Winks_salAlloc         MALLOC
#define Winks_salFree         FREE

#ifdef WINKS_SIMULATOR
void Winks_printf( char* format, ... );
#define Winks_printf(a)
#else
#define Winks_printf(...)
#endif

#endif //WINKS_DEBUG



/* C Lib */
#define  memcpy MEMCPY

#undef  MEMSET
#define MEMSET memset
#define memset(dest,ch,size)        GET_HELPER()->memset((dest),(ch),(size))
//#define  memset MEMSET

#undef STRCPY
#define  STRCPY strcpy
#define  strcpy(dest,src)       (GET_HELPER()->strcpy((dest),(src)))

#undef STRLEN
#define  STRLEN strlen
#define  strlen(s1)             (GET_HELPER()->strlen((s1)))

#undef STRSTR
#define  STRSTR strstr 
#define  strstr(h,n)            (GET_HELPER()->strstr((h),(n)))

#undef STRICMP 
#define STRICMP stricmp
#define stricmp(a, b)               GET_HELPER()->stricmp((a),(b))

#undef STRNICMP
#define STRNICMP strnicmp
#define strnicmp(a,b,count)         (GET_HELPER()->strnicmp((a),(b),(count)))

#undef STRNCPY
#define STRNCPY strncpy
#define strncpy(dest,src,count)     (GET_HELPER()->strncpy((dest),(src),(count)))

#undef STRCMP
#define STRCMP strcmp
#define strcmp(s1,s2)               (GET_HELPER()->strcmp((s1),(s2)))

#undef STRNCMP 
#define STRNCMP strncmp
#define strncmp(a,b,count)          (GET_HELPER()->strncmp((a),(b),(count)))

#undef STRCAT
#define STRCAT strcat
#define strcat(dest,src)            (GET_HELPER()->strcat((dest),(src)))

#undef STRCHR
#define STRCHR strchr
#define strchr(s1,ch)               (GET_HELPER()->strchr((s1),(ch)))

#undef STRRCHR
#define STRRCHR strrchr
#define strrchr(s1,ch)              GET_HELPER()->strrchr((s1),(ch))

#undef SPRINTF
#define SPRINTF sprintf
#define sprintf GET_HELPER()->sprintf

#undef SNPRINTF
#define SNPRINTF snprintf
#define snprintf                    GET_HELPER()->snprintf

#undef ATOI
#define ATOI atoi
#define atoi(psz)                   (GET_HELPER()->atoi((psz)))

#define atol(s)     STRTOUL((s),NULL,10)


// C Lib fun needs to impl 
#define toupper Winks_toupper
#define strncat Winks_strncat
int Winks_toupper(int c);
char *Winks_strncat(char *s1, const char *s2, unsigned int n);

//end fo C Lib

#define WINKS_SYS_TICK (1)

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define MODULE_OSAL_MSGCHANNEL  (1)
#define MODULE_OSAL_TIMER       (2)
#define MODULE_OSAL_PUSH        (3)
#define MODULE_OSAL_MEM         (5)
#define MODULE_OSAL_FILE        (6)
#define MODULE_OSAL_SOCKET      (7)
#define MODULE_OSAL_UI          (8)
#define MODULE_OSAL_NUM_MONITOR (9)
#define MODULE_OSAL_ADDRBOOK   (10)
#endif

/****************************************
 *            symbian os
 * **************************************/
#elif defined (WINKS_SYMBIAN_V_9_0)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define stricmp   Winks_stricmp
#define strnicmp  Winks_strnicmp

#define snprintf  Winks_snprintf  
#define sprintf  Winks_sprintf  

#define Winks_printHex(buff,len,head)
#ifdef WINKS_DEBUG
void Winks_printf( char* format, ... );
#define __WINKS_LOGOUT__
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree(void* ptr, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
#define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
#define Winks_salFree(x)       Winks_xsalFree( x, __FILE__, __LINE__ )
#else
#define Winks_salAlloc         malloc
#define Winks_salFree         free
#define Winks_printf(...)
#endif
//#define Winks_printf           printf
//#define Winks_mem_cpy(dp,sp,size)      memcpy(dp,sp,size)
//#define Winks_mem_set(dp,val,size)      memset(dp,val,size)
#define memcpy Winks_mem_cpy
#define memset Winks_mem_set

int winks_screen_width();
int winks_screen_height();


/****************************************
 *            Windows Mobile for PPC
 * **************************************/
#elif defined (WINKS_WM_PPC)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Winks_printHex(buff,len,head)

#ifdef WINKS_DEBUG

#define __WINKS_LOGOUT__
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree(void* ptr, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
#define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
#define Winks_salFree(x)       Winks_xsalFree( x, __FILE__, __LINE__ )
void Winks_printf( char* format, ... );
#else

#define Winks_salAlloc         malloc
#define Winks_salFree         free
#define Winks_printf(...)

#endif

#define snprintf _snprintf
//#define Winks_snprintf           _snprintf

//#define Winks_printf           printf
int Winks_snprintf(char *, size_t, const char *, ...);
void *Winks_mem_set(void* dest, int c, size_t count );
void *Winks_mem_cpy(void* dest, const void* src, size_t count );
void Winks_Free(void* memblock );
void *Winks_Alloc(size_t size );
int stricmp( const char *string1, const char *string2 );
int strnicmp( const char *string1, const char *string2, size_t count );

//#define Winks_mem_cpy(dp,sp,size)      memcpy(dp,sp,size)
//#define Winks_mem_set(dp,val,size)      memset(dp,val,size)
//#define memcpy Winks_mem_cpy
//#define memset Winks_mem_set


/***********************************************************
 *            spreadtrum(展讯) platform
 * ********************************************************/
#elif defined (WINKS_SPTRUM_MOCOR)
#ifdef WIN32
#ifndef _FILE_DEFINED
#include "sfs.h"
typedef SFS_HANDLE FILE;
#define _FILE_DEFINED
#endif
#endif
#include "os_api.h"

#define  Winks_SysAssert SCI_ASSERT(0)
#define Winks_Alloc(size)           SCI_ALLOC(size)
#define Winks_Free(ptr)             SCI_FREE(ptr)
#define Winks_mem_set  SCI_MEMSET
#define Winks_mem_cpy  SCI_MEMCPY
#define MEMSET SCI_MEMSET

#ifdef WINKS_DEBUG

#define Winks_printHex(buff,len,head) HEX_DataOut(buff, len, head)
void Winks_printf( char* format, ... );
void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);

void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree(void* ptr, char* file, unsigned long line);
#define Winks_assertfail(assertFailType,cond,file,line) Winks_SysAssert
#define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
#define Winks_salFree(x)       Winks_xsalFree( x, __FILE__, __LINE__ )

void Winks_xsalPrintMemInfo(void);
int WK_IsMemCleanup(char* file);

#else //WINKS_DEBUG
//#define Winks_printf(a)
#define Winks_printf(...)
#define Winks_printHex(buff,len,head)
#define Winks_salAlloc        SCI_ALLOC
#define Winks_salFree         SCI_FREE
#endif //WINKS_DEBUG

#define MALLOC Winks_salAlloc
#define FREE Winks_salFree

#undef STRCMP
#define STRCMP strcmp

#undef STRCPY
#define STRCPY strcpy

#define snprintf _snprintf

	
	
/***********************************************************
 *            IPhone platform
 * ********************************************************/
#elseif define (IPhone)


	
/************************************************
                  other platform
************************************************/
#else/*other platform*/

#define stricmp   Winks_stricmp
#define strnicmp  Winks_strnicmp

#define snprintf  Winks_snprintf  
#define sprintf  Winks_sprintf  

#define Winks_printHex(buff,len,head)
#ifdef WINKS_DEBUG
void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
int Winks_xsalFree(void* ptr, char* file, unsigned long line);
void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
#define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
#define Winks_salFree(x)       Winks_xsalFree( x, __FILE__, __LINE__ )
#else
#define Winks_salAlloc         malloc
#define Winks_salFree         free
#endif
//#define Winks_printf           printf
void Winks_printf( char* format, ... );
//#define Winks_mem_cpy(dp,sp,size)      memcpy(dp,sp,size)
//#define Winks_mem_set(dp,val,size)      memset(dp,val,size)
#define memcpy Winks_mem_cpy
#define memset Winks_mem_set

#endif  //other platform








#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_GLOB_PTR_SIZE 64
#define WK_MODULE_HTTP         (11)
#define WK_MODULE_ENGINE       (12)
#define WK_MODULE_UI           (13)
#define WK_MODULE_CACHE        (14)
#define WK_MODULE_COOKIE       (15)
#define WK_MODULE_HIGH_OSAL     (16)    /*抽象层全局结构标识*/
#define WK_MODULE_DMM           (17)    /*动态内存管理全局结构标识*/
#define WK_MODULE_WIDGET        (18)   /*UI控件*/
#define WK_MODULE_CCSW          (19)    /* 彩像展示程序 */
void* get_module_glob(int moduleID);
int set_module_glob(int moduleID ,void* pNewVal);
#endif/*WINKS_UNSUPPORTED_GLOBAL_VAR*/
typedef enum Winks_screen_coordinate
{
	WINKS_TITLE_STRING_X_ID,//0

	WINKS_MAINMENU_ICON_WIDTH_ID,
	WINKS_MAINMENU_BACKGROUND_WIDTH_ID,//0
	WINKS_LIST_ICON_WIDTH_ID,
	WINKS_UPSCROLLBAR_WIDTH_ID,//0
	WINKS_INFOBOX_BG_WIDTH_ID,
	
	WINKS_INFOBOX_STRING_WIDTH_ID,


	WINKS_TITLE_STRING_Y_ID,
	WINKS_TITLE_HEIGHT_ID,
	WINKS_CLIENT_HEIGHT_ID,
	WINKS_MAINMENU_ICON_HEIGHT_ID,

	WINKS_MAINMENU_BACKGROUND_HEIGHT_ID,
	WINKS_SOFTKEY_HEIGHT_ID,
	WINKS_LIST_MAX_NUMBER_ID,
	WINKS_HIGHLIGHT_LISTITEM_HEIGHT_ID,
	WINKS_LIST_ICON_HEIGHT_ID,

	WINKS_UPSCROLLBAR_HEIGHT_ID,

	WINKS_INFOBOX_BG_HEIGHT_ID,
	WINKS_INFOBOX_STRING_AND_BG_GAP_ID,
	WINKS_INFOBOX_STRING_HEIGHT_ID

}Winks_screen_coordinate_e;
extern int winks_get_coordinate(Winks_screen_coordinate_e coordinate_id);
#ifdef __cplusplus
}
#endif
#endif/*_WK_OSPUB_H_*/
