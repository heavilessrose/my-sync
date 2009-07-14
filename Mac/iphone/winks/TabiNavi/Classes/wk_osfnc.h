/*
File:        wk_osfnc.h
Author:        Huaxm
Create Time:22/01/2008
*/
#ifndef _WK_OSFNC_H_
#define _WK_OSFNC_H_


#include "wk_public.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WINKS_FS_ATTR_IS_RDONLY     0x01
#define WINKS_FS_ATTR_IS_HIDDEN     0x02
#define WINKS_FS_ATTR_IS_SYSTEM     0x04
#define WINKS_FS_ATTR_IS_ARCHIVE    0x08
#define WINKS_FS_ATTR_IS_DIR        0x10
#define WINKS_FS_ATTR_IS_ASCII      0x100
#define WINKS_FS_ATTR_IS_DOT        0x200

typedef struct tag_WinksFindAttrib_s
{
    int     DirHd;          /* 打开的目录句柄 */
    void *  pDirEnt;        /* 系统返回的目录入口,给FindNext用 */
    char    FileName[128];  /* 返回的文件名或目录名存储 */
    unsigned long FileSize; /* 返回的文件大小 */
    unsigned long Attrib;   /* 返回的文件属性 */
    unsigned long year;      /* 1752~2999 */
    unsigned char month;     /* 1~12 */
    unsigned char day;       /* 1~31 */
    unsigned char hour;      /* 0~23 */
    unsigned char min;       /* 0~59 */
    unsigned char sec;       /* 0~59 */
}WinksFindAttrib_s;

typedef enum tag_Winks_FsErrCode_E
{
    WINKS_FS_NO_ERROR = 0x0,
    WINKS_FS_FILE_NOT_FOUND,
    WINKS_FS_INVALID_FILENAME,
    WINKS_FS_ERROR_CODE_NUMBER
}Winks_FsErrCode_E;

/*设置文件操作错误码函数。返回值: 0成功； -1失败。
errorCode 为指定的错误码。*/
int Winks_SetFileSysErrorCode(unsigned long errorCode);

/*取文件操作错误码函数。返回值: 0成功； -1失败。
*errorCode 为返回的错误码。*/
int Winks_GetFileSysErrorCode(unsigned long *errorCode);

#if 0
/* unicode 与 ascii 转换函数 */
int Winks_Ascii2UnicodeString(char *pInBuffer, char *pOutBuffer);
int Winks_Ascii2UnicodeStringN(char *pInBuffer, int len, char *pOutBuffer);
int Winks_Unicode2AsciiString(char *pInBuffer, char *pOutBuffer);
int Winks_Unicode2AsciiStringN(char *pInBuffer, int len, char *pOutBuffer);

/* unicode字符串操作函数 */
int Winks_UnicodeStrlen(const char *arrOut);
char * Winks_UnicodeStrcpy(char *strDestination, const char *strSource);
char * Winks_UnicodeStrncpy(char *strDestination, const char *strSource, int size);
int Winks_UnicodeStrcmp(const char *string1, const char *string2);
int Winks_UnicodeStrncmp(const char *string1, const char *string2, int size);
char * Winks_UnicodeStrcat(char *strDestination, const char *strSource);
char * Winks_UnicodeStrncat(char *strDestination, const char *strSource, int size);
#endif

/*打开指定的文件*/
FILE* Winks_fopen(const char *filename,const char *type);
/*打开指定的文件(unicode)*/
FILE* Winks_fopenUni(const unsigned short *filename,const char *type);

/*关闭一个文件*/
int Winks_fclose(FILE *stream);

/*测试是否到达指定文件的末尾*/
int Winks_feof(FILE *stream);

/*测试指定文件是否存在,如果存在返回0，否则-1*/
int Winks_fexist(char *filename);

/*从所给文件stream中读取的count个数据数据单元，每一个数据单元的长度为size字节，到由data所指的块中*/
size_t Winks_fread(void *data, size_t size, size_t count, FILE *stream);

/*从指针data开始把count个数据单元项添加到给定文件stream中，每个数据单元的长度为size个字节*/
size_t Winks_fwrite(const void *data, size_t size, size_t count, FILE *stream);

/*本函数返回文件fp 的指针偏移位(offset) 值。成功偏移量，失败返回-1。*/
int Winks_ftell(FILE *stream);

/*将文件指针移到指定位*/
#define WINKS_SEEK_SET 0 /* Seek from beginning of file.  */
#define WINKS_SEEK_CUR 1 /* Seek from current position.  */
#define WINKS_SEEK_END 2 /* Set file pointer to EOF plus "offset" */
int Winks_fseek(FILE *stream, long offset, int fromwhere);

/*删除指定的文件。成功返回0，失败返回-1。*/
int Winks_fdelete(char *file_name);

/*更新指定文件的文件名。成功返回0，失败返回-1。*/
int Winks_frename(char *old_file_name, char *new_file_name);

/* 获取文件大小, 返回负值表示失败 */
int Winks_fgetsize(const char *filename, unsigned int is_ascii, unsigned int *size);

/*取文件大小，成功返回0*/
int Winks_fGetFileSize(FILE *stream, unsigned long *size);

/*获取当前路径。成功返回0，失败返回-1。*/
int Winks_getcwd(char *path_name_ptr, size_t size);

/*设置当前路径。成功返回0，失败返回-1。*/
int Winks_chdir(char *dir_name);

/*创建目录。成功返回0，失败返回-1。*/
int Winks_mkdir(char *dir_name);

/*删除指定的空目录。成功返回0，失败返回-1。*/
int Winks_rmdir(char *dir_name);

/*修改目录名。成功返回0，失败返回-1。*/
int Winks_rndir(char *old_dir_name, char *new_dir_name);

/* 删除一个目录下所有文件, 不递归子目录 */
int Winks_XdeleteDir(char *dir_name);

/* 获取文件夹中所有文件大小 */
int Winks_GetFolderSize(
    const char * Path,      /* Src path string */
    int          IsAscii,   /* Src path is ascii ? */
    unsigned long * RetSize,  /* return folder size */
    unsigned long CmpSize);  /* CmpSize==0 获取整个文件夹的文件大小，否则只判断是否大于CmpSize */

int Winks_FindDirFirst(
    const char * Path,      /* Src path string */
    int          IsAscii,   /* Src path is ascii ? */
    WinksFindAttrib_s * retFsAttrib); /* return attrib */

int Winks_FindDirNext(WinksFindAttrib_s * pFsAttrib);

int Winks_FindDirClose(WinksFindAttrib_s * pFsAttrib);


/*目录初始化*/
void Winks_OsalInitDir(void);



typedef struct tag_Winks_DateTime_s
{
    unsigned long year;      /* 1752~2999 */
    unsigned char month;     /* 1~12 */
    unsigned char day;       /* 1~31 */
    unsigned char hour;      /* 0~23 */
    unsigned char min;       /* 0~59 */
    unsigned char sec;       /* 0~59 */
    unsigned char week;      /* 1~6,0(mon~sun)*/
}Winks_DateTime_s;

typedef enum tag_Winks_DateTimeFormat_e
{
    WINKS_DTFRM_YYYYMMDD_1 = 0,          /*20080101*/
    WINKS_DTFRM_YYYYMMDD_2,              /*2008/01/01*/
    WINKS_DTFRM_YYYYMMDD_3,              /*2008-01-01*/
    WINKS_DTFRM_HH24MISS_1,              /*245959*/
    WINKS_DTFRM_HH24MISS_2,              /*24:59:59*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_1,      /*20080101245959*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_2,      /*2008010124:59:59*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_3,      /*2008/01/0124:59:59*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_4,      /*2008-01-0124:59:59*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_5,      /*20080101 24:59:59*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_6,      /*2008/01/01 24:59:59*/
    WINKS_DTFRM_YYYYMMDDHH24MISS_7,      /*2008-01-01 24:59:59*/
    WINKS_DTFRM_YYMMDDHH24MISS_1,        /*080101245959*/
    WINKS_DTFRM_MAX_NUM
}Winks_DateTimeFormat_e;

typedef enum tag_Winks_TimeZone_e
{
    WINKS_TIME_ZONE_WEST_12 = 0,
    WINKS_TIME_ZONE_WEST_11,
    WINKS_TIME_ZONE_WEST_10,
    WINKS_TIME_ZONE_WEST_9,
    WINKS_TIME_ZONE_WEST_8,
    WINKS_TIME_ZONE_WEST_7,
    WINKS_TIME_ZONE_WEST_6,
    WINKS_TIME_ZONE_WEST_5,
    WINKS_TIME_ZONE_WEST_4,
    WINKS_TIME_ZONE_WEST_3,
    WINKS_TIME_ZONE_WEST_2,
    WINKS_TIME_ZONE_WEST_1,
    WINKS_TIME_ZONE_ZERO_TIMEZONE,
    WINKS_TIME_ZONE_EAST_1,
    WINKS_TIME_ZONE_EAST_2,
    WINKS_TIME_ZONE_EAST_3,
    WINKS_TIME_ZONE_EAST_4,
    WINKS_TIME_ZONE_EAST_5,
    WINKS_TIME_ZONE_EAST_6,
    WINKS_TIME_ZONE_EAST_7,
    WINKS_TIME_ZONE_EAST_8,
    WINKS_TIME_ZONE_EAST_9,
    WINKS_TIME_ZONE_EAST_10,
    WINKS_TIME_ZONE_EAST_11,
    WINKS_TIME_ZONE_EAST_12
}Winks_TimeZone_e;


/*取系统时间*/
int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p);

/*按照指定格式将时间转换成字符串。
dateTime_p为表示时间的结构体指针，
当dateTime_p==NULL时取系统时间，随后按指定格式返回。*/
int Winks_FormatDateTime(Winks_DateTimeFormat_e format, char *buff, unsigned char size, Winks_DateTime_s *dateTime_p);
/*计算从1970 年1 月1 日0 时起到指定时间经过的秒数.
tm:输入的时间。如果tm=NULL，取系统时间(相当于time)。
return unsigned long型的秒数，0表示失败。*/
unsigned long Winks_mktime (Winks_DateTime_s *dtp);
/*取时间差函数*/
int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result);
/** 取系统的时区 **/
int Winks_GetSysTimeZone(Winks_TimeZone_e *time_zone);

/**消息定义**/
/**  基于平台的消息，由HTTP定义。**/
//#define WINKS_PMSG_HTTP                         (0xF800)
/** 基于平台的消息，由抽象层定义。**/
//#define WINKS_PMSG_OSAL                         (0xF820)
/** 基于平台的消息，由UI定义。**/
//#define WINKS_PMSG_UI                           (0xF840)

/*************************************************
            WINKS内部消息
**************************************************/
/* WINKS消息起始值 */
#define WINKS_MSG_BASE                          0xF000
/** 基于抽象层的消息，由抽象层定义。**/
#define WINKS_MSG_OSAL                          (WINKS_MSG_BASE)
#define WINKS_MSG_CHN_INIT                      (WINKS_MSG_OSAL+0x00)
#define WINKS_MSG_TIMER_EXPIRY                  (WINKS_MSG_OSAL+0x01)
#define WINKS_MSG_CALL_INCOMING                 (WINKS_MSG_OSAL+0x02)   /* 来电     */
#define WINKS_MSG_CALL_DIAL                     (WINKS_MSG_OSAL+0x03)   /* 拨号     */
#define WINKS_MSG_CALL_ABORT                    (WINKS_MSG_OSAL+0x04)   /* 本地取消 */
#define WINKS_MSG_CALL_REMOTE_ABORT             (WINKS_MSG_OSAL+0x05)   /* 对方取消 */
#define WINKS_MSG_CALL_ANSWER                   (WINKS_MSG_OSAL+0x06)   /* 本地接听 */
#define WINKS_MSG_CALL_REMOTE_ANSWER            (WINKS_MSG_OSAL+0x07)   /* 对方接听 */
#define WINKS_MSG_CALL_HANGUP                   (WINKS_MSG_OSAL+0x08)   /* 本地挂断 */
#define WINKS_MSG_CALL_REMOTE_HANGUP            (WINKS_MSG_OSAL+0x09)   /* 对方挂断 */
#define WINKS_MSG_CALL_REJECT                   (WINKS_MSG_OSAL+0x0a)   /* 本地拒接 */
#define WINKS_MSG_CALL_REMOTE_REJECT            (WINKS_MSG_OSAL+0x0b)   /* 对方拒接 */
#define WINKS_MSG_CALL_IN_WAIT                  (WINKS_MSG_OSAL+0x0c)   /* 来电等待 */
#define WINKS_MSG_CALL_ENDALL                   (WINKS_MSG_OSAL+0x0d)   /* 挂断ALL  */
#define WINKS_MSG_IMSI_REQ                      (WINKS_MSG_OSAL+0x0e)   /* 取IMSI号 */
#define WINKS_MSG_CALL_QUERY_EMERGENCY          (WINKS_MSG_OSAL+0x0f)   /* 取紧急呼叫号码*/
#define WINKS_MSG_PHONE_BOOK_MODIFIED           (WINKS_MSG_OSAL+0x10)   /* 电话本变更消息*/
#ifdef WINKS_DUAL_SIM
#define WINKS_MSG_IMSI_2_REQ                    (WINKS_MSG_OSAL+0x11)   /* 取IMSI2号 */
#endif
#define WINKS_MSG_USB_DETECT                    (WINKS_MSG_OSAL+0x12)   /* USB 检测消息*/
#define WINKS_MSG_TEST                          (WINKS_MSG_OSAL+0x13)   /* For test. */ 
#define WINKS_MSG_FS_INVALID                    (WINKS_MSG_OSAL+0x14)   /* File system invalid.*/ 
#define WINKS_MSG_IMEI_REQ                      (WINKS_MSG_OSAL+0x15)   /* 取IMEI号 */

/** 基于抽象层的消息，由HPPT定义。**/
#define WINKS_MSG_HTTP                          (WINKS_MSG_BASE+0x400)
/** 基于抽象层的消息，由引擎定义。**/
#define WINKS_MSG_ENGINE                        (WINKS_MSG_BASE+0x500)
#define WINKS_MSG_ENGINE_HTTP                   (WINKS_MSG_ENGINE+0x01)
#define WINKS_MSG_ENGINE_INIT_END               (WINKS_MSG_ENGINE+0x02)
#define WINKS_MSG_ENGINE_PUSH                   (WINKS_MSG_ENGINE+0x03)
#define WINKS_MSG_ENGINE_RETMSG                 (WINKS_MSG_ENGINE+0x04)
#define WINKS_MSG_ENGINE_UIRETMSG               (WINKS_MSG_ENGINE+0x05)
#define WINKS_MSG_ENGINE_UPDATA_FL              (WINKS_MSG_ENGINE+0x06)
#define WINKS_MSG_ENGINE_DIYSET                 (WINKS_MSG_ENGINE+0x07)
#define WINKS_MSG_ENGINE_SETTINGSET             (WINKS_MSG_ENGINE+0x08)
#define WINKS_MSG_ENGINE_SNED_REG_SMS           (WINKS_MSG_ENGINE+0x09)
#define WINKS_MSG_ENGINE_SELECT_FRIEND_LIST     (WINKS_MSG_ENGINE+0x0a)
#define WINKS_MSG_ENGINE_REG_NOTIFY             (WINKS_MSG_ENGINE+0x0b)
#define WINKS_MSG_ENGINE_INIT_CACHE             (WINKS_MSG_ENGINE+0x0c)
#define WINKS_MSG_ENGINE_REBUILD_CACHE          (WINKS_MSG_ENGINE+0x0d)
#define WINKS_MSG_ENGINE_RECRUIT_DOWNLOAD       (WINKS_MSG_ENGINE+0x0e)
#define WINKS_MSG_ENGINE_APP_START              (WINKS_MSG_ENGINE+0x10)
#define WINKS_MSG_ENGINE_APP_STOP               (WINKS_MSG_ENGINE+0x11)
#define WINKS_MSG_ENGINE_APP_PAUSE              (WINKS_MSG_ENGINE+0x12)
#define WINKS_MSG_ENGINE_APP_RESUME             (WINKS_MSG_ENGINE+0x13)
#define WINKS_MSG_ENGINE_APP_RETMSG             (WINKS_MSG_ENGINE+0x14)
#define WINKS_MSG_ENGINE_APP_DATAOUT            (WINKS_MSG_ENGINE+0x15)

/** 基于抽象层的消息，由UI定义。**/
#define WINKS_MSG_UI                            (WINKS_MSG_BASE+0x800)
#define WINKS_MSG_UI_KEY_EVENT                  (WINKS_MSG_UI +0x01)
#define WINKS_MSG_UI_DESTROY_APP                (WINKS_MSG_UI +0x02)   /*当应用被销毁时发生*/
#define WINKS_MSG_UI_CREATE_APP                 (WINKS_MSG_UI +0x03)   /*当应用被创建时发生。可能从菜单进入或来电事件触发。*/
#define WINKS_MSG_UI_PAINT_EVENT                (WINKS_MSG_UI +0x04)   /*当界面有失效区域时发生，收到该时间UI需要重绘指定区域。*/
#define WINKS_MSG_UI_PEN_EVENT                  (WINKS_MSG_UI +0x05)   /*画笔消息*/
#define WINKS_MSG_UI_SELECT_FILE_RSP            (WINKS_MSG_UI +0x06)   /*取系统多媒体文件的返回消息*/
#define WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP    (WINKS_MSG_UI +0x07)   /*取系统接入点的返回消息*/
#define WINKS_MSG_UI_PAUSE_RESUME_APP           (WINKS_MSG_UI +0x08)   /*应用挂起恢复消息*/
#define WINKS_MSG_UI_EDITOR_RSP                 (WINKS_MSG_UI +0x09)   /*调用输入法后平台的返回消息*/
#define WINKS_MSG_UI_SELECT_PHONENUM_RSP        (WINKS_MSG_UI +0x0a)   /*取联系人返回消息*/
#define WINKS_MSG_UI_REG_IMMEDIATELY            (WINKS_MSG_UI +0x0b)   /*来电直接激活彩像软件*/




#define WINKS_OSALHANDER_STOP ((char)0)
#define WINKS_OSALHANDER_RUNNING ((char)1)

#define WINKS_MESSAGE_BODY_MAX_SIZE 3000/*2048*/
#define WINKS_IMSI_LEN 15
#define WINKS_IMEI_LEN 32
#define WINKS_OSAL_CHN_MAXNUM    8
#define WINKS_OSAL_MSG_MAXNUM    64
#define WINKS_OSAL_TIMMER_MAXNUM    32
#define WINKS_OSAL_EVENT_WAITING_QUEUE_MAXNUM    WINKS_OSAL_CHN_MAXNUM

/* USB 检测动作 */
#define WINKS_USB_PLUG_IN 1
#define WINKS_USB_PLUG_OUT 2

typedef unsigned long WINKS_CHN_ID;
typedef int (*WINKS_OSAL_CALLBACK)(unsigned long msg, void *data, unsigned long size);
typedef unsigned long WINKS_TIMER_ID;
#define WINKS_CHN_NULL ((WINKS_CHN_ID)0)
typedef unsigned long WINKS_TICKS;
#define WINKS_TIMER_NULL ((WINKS_TIMER_ID)0)


#define WINKS_TIMER_DEFAULT    0x0000    /*默认属性*/
#define WINKS_TIMER_AUTO_START 0x0001    /*定时器创建后自动启动*/
#define WINKS_TIMER_AUTO_LOOP  0x0002    /*定时器超时后重新启动*/

#ifdef WINKS_MULTITHREADING
/*多线程的互斥管理，在多线程中对于消息通道的创建、消息的发送，以及定时器的创建都需要用互斥量来控制。*/
/*define mutex_id*/
#define WINKS_MUTEX_CHN 1
#define WINKS_MUTEX_MSG 2
#define WINKS_MUTEX_TIMER 3

/*创建互斥量，在抽象层初始化的时候被调用。    成功返回0，失败返回-1*/
//int Winks_api_CreateMutex(void);
/*注销互斥量，在抽象层销毁的时候被调用。    成功返回0，失败返回-1*/
//int Winks_api_DestroyMutex(void);
/*得到互斥量。mutex_id:互斥量ID(如:WINKS_MUTEX_CHN)    成功返回0，失败返回-1*/
int Winks_api_GetMutex(int mutex_id);
/*释放互斥量。mutex_id:互斥量ID(如:WINKS_MUTEX_CHN)    成功返回0，失败返回-1*/
int Winks_api_PutMutex(int mutex_id);
#endif


/*设置通道等待的系统消息，flag的每一位标志一个系统消息*/
#define WINKS_CALL_NUMBER_LENGTH 40
#define WINKS_CALL_NAME_LENGTH 24


/*IMSI、IMEI*/
typedef struct tag_winks_MsgWaitingQueue_s
{
WINKS_CHN_ID chn;
unsigned long msg;
struct tag_winks_MsgWaitingQueue_s *next;
}winks_MsgWaitingQueue_s;


/*等待事件开关*/
#define WINKS_EVENT_NULL                0x00000000
#define WINKS_EVENT_CALL_INCOMING       0x00000001  /* 来电 */
#define WINKS_EVENT_CALL_DIAL           0x00000002  /* 拨号 */
#define WINKS_EVENT_CALL_ABORT          0x00000004  /* 本地取消 */
#define WINKS_EVENT_CALL_REMOTE_ABORT   0x00000008  /* 对方取消 */
#define WINKS_EVENT_CALL_ANSWER         0x00000010  /* 本地接听 */
#define WINKS_EVENT_CALL_REMOTE_ANSWER  0x00000020  /* 对方接听 */
#define WINKS_EVENT_CALL_HANGUP         0x00000040  /* 本地挂断 */
#define WINKS_EVENT_CALL_REMOTE_HANGUP  0x00000080  /* 对方挂断 */
#define WINKS_EVENT_CALL_REJECT         0x00000100  /* 本地拒接 */
#define WINKS_EVENT_CALL_REMOTE_REJECT  0x00000200  /* 对方拒接 */
#define WINKS_EVENT_CALL_ENDALL         0x00000400  /* 挂断ALL */
#define WINKS_EVENT_CALL_IN_WAIT        0x00000800  /* 来电等待 */
#define WINKS_EVENT_OF_ALL_CALL         0x0000FFFF  /*全部电话事件*/
#define WINKS_EVENT_PB_MODIFIED         0x00010000  /* 电话本变更 */
#define WINKS_EVENT_USB_DETECT          0x00020000  /* USB 检测事件 */
#define WINKS_EVENT_FS_INVALID          0x00040000  /* File system invalid. here is a event.*/ 
#define WINKS_EVENT_TEST_KET            0x80000000  /* TEST KEY EVENT */

#define WINKS_FSINVALID_CAUSED_USB      ((unsigned long)1)      /* 由插入USB线事件所引发的文件系统不能使用。*/

typedef struct tag_winks_EventWaitQueue_s
{
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
WINKS_CHN_ID chn;
#endif
unsigned long flag;             /*每一位表示等待一个事件*/
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
struct tag_winks_EventWaitQueue_s *next;
#endif
}winks_EventWaitingQueue_s;


#define TIMER_INTERVAL_OSAL_WAITINGIMSI  50000
#define TIMER_INTERVAL_OSAL_WAITINGIMEI  50000
#define TIMER_INTERVAL_OSAL_WAITINGEMERGENCY  2000


#ifndef WINKS_MSG_WITHOUT_DATA
#define WINKS_MAX_LEN_OF_MSG_WITH_DATA 16
#endif
typedef struct tag_winks_MsgBody_s{
    WINKS_CHN_ID chn;
    unsigned long msg;
    void *wParam;
    unsigned long lParam;
#ifndef WINKS_MSG_WITHOUT_DATA
    char sData[WINKS_MAX_LEN_OF_MSG_WITH_DATA];
#endif
}winks_MsgBody_s;


typedef struct tag_winks_MsgRingQueue_s{
    unsigned long head;
    unsigned long tail;
    winks_MsgBody_s queue[WINKS_OSAL_MSG_MAXNUM];
}winks_MsgRingQueue_s;


typedef struct tag_winks_TimerBody_s{
    WINKS_CHN_ID chn;
    WINKS_TICKS timeOutPeriod;
    WINKS_TICKS timeOut;
    unsigned long flag;
    unsigned char state;            /*1:running , 0:stopped*/
}winks_TimerBody_s;


typedef struct tag_winks_TimerQueue_s{
    unsigned long sequence;         /*记录创建时的检索起始点*/
    unsigned long curInd;
    unsigned long beginTime;
    WINKS_TICKS curTicks;
    winks_TimerBody_s queue[WINKS_OSAL_TIMMER_MAXNUM];
}winks_TimerQueue_s;

typedef enum tag_WinksAppState_e
{
    WINKS_APP_STATE_STOP = 0,
    WINKS_APP_STATE_RUN = 1,
    WINKS_APP_STATE_SUSPEND = 2
}WinksAppState_e;

typedef struct tag_winks_PbModifiedTmp_s
{
    unsigned short index;
    unsigned short type;
    char number1[WINKS_CALL_NUMBER_LENGTH];
}winks_PbModifiedTmp_s;
#define WINKS_MAX_NUM_OF_PB_MODIFIED_TMP 4


#ifndef WINKS_USE_PLATFORM_SOURCE_MANAGER

typedef struct tagWinks_Img2Cstr
{
    struct tagWinks_Img2Cstr* pnext;
    char* fname;
    int flen;
    unsigned char* fdata;
}Winks_Img2Cstr;

typedef struct tagWinks_ImgdataCache
{
    int resCnt;
    int remain;
    Winks_Img2Cstr* pCache;
}Winks_ImgdataCache;

typedef struct tagWinks_ImgType
{
    int type;
    char* extname;
}Winks_ImgType;

extern const Winks_Img2Cstr* Winks_GloImgRes[];
#endif

#ifdef WINKS_SENDSMS_UPGRADE
//#define WINKS_SENDSMS_QUEUE_NUM     10              /*短信队列最大长度*/
/* 短信队列的结构 */
typedef struct tag_Winks_SmsQueueItem_s
{   struct tag_Winks_SmsQueueItem_s *next;
    char *number;
    char *sendData;
    int simNo;
}Winks_SmsQueueItem_s;
/*短息发送机的异步回调函数。需要在平台发送短信返回后调用该函数。
 * function: Winks_SendSmsCallBack
 * parameters:
 *   void *number: 短信发送结果。0:失败，1:成功。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSmsCallBack(int result);

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * 用默认的SIM卡发送短信。
 * function: Winks_SendSms
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *dataUni: 短信内容，Unicode编码。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSmsUni(char *number, char *dataUni);

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * 用默认的SIM卡发送短信。
 * function: Winks_SendSms
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *data: 短信内容，utf8编码
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSms(char *number, char *data);

#ifdef WINKS_DUAL_SIM
/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * function: Winks_SendSms
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *dataUni: 短信内容，Unicode编码。
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSmsUni_Ext(char *number, char *dataUni, int simNo);

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * function: Winks_SendSms
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *data: 短信内容，utf8编码
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSms_Ext(char *number, char *data, int simNo);

/* 设置彩像联网用的Sim卡 */
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData);
/* 恢复手机默认联网用的Sim卡 */
int Winks_api_RestoreNetLinkSim(int SaveData);
/*  Sim卡可用返回1，否则返回0 */
int Winks_isSimEnabled(int sim_card_id);
#endif
#endif


typedef struct tag_winks_OsalContext_s{
    /***************Global Variable*********************/
    /*     将抽象层模块中所有的全局变量整合到一起      */
    /**/WINKS_OSAL_CALLBACK osalCnhList[WINKS_OSAL_CHN_MAXNUM];
    /**/winks_MsgRingQueue_s osalMsgRingQueue;
    /**/winks_TimerQueue_s osalTimerQueue;
    /**/unsigned char g_post_msg_debug_flag;
    /**/unsigned char osalHandlerStart;
#ifdef WINKS_PB_INTERCEPTION
    /**/winks_PbModifiedTmp_s pbModifiedTmp[WINKS_MAX_NUM_OF_PB_MODIFIED_TMP];
#endif
#ifndef WINKS_USE_PLATFORM_SOURCE_MANAGER
    /**/Winks_ImgdataCache wk_imgcache;
#endif
    /*********************************************/
    void* setting;                              /*系统设置项*/
    WINKS_CHN_ID uiChn;                         /*UI channel,大黑板消息通道*/
    WINKS_CHN_ID uiCcShowChn;                   /*UI channel,大红板消息通道*/
    WINKS_CHN_ID engChn;                        /*Engine channel.*/
    char curImsi[WINKS_IMSI_LEN+1];             /*IMSI*/
    WINKS_TIMER_ID waitingImsi_T;               /*定时器，用于IMSI号码等待计时*/
    winks_MsgWaitingQueue_s *waitingImsi_p;     /*单项链表指针，用于IMSI号码等待队列*/
    char curImei[WINKS_IMEI_LEN+1];             /*IMEI*/
    WINKS_TIMER_ID waitingImei_T;               /*定时器，用于IMEI号码等待计时*/
    winks_MsgWaitingQueue_s *waitingImei_p;     /*单项链表指针，用于IMEI号码等待队列*/
#ifdef WINKS_DUAL_SIM
    unsigned short ActiveSim;                   /* 高字节为1表示找到彩像使用的sim卡;低字节表示使用curImsi还是curImsi_2 */
    WINKS_TIMER_ID waitingImsi_2_T;               /*定时器，用于IMSI号码等待计时*/
    char curImsi_2[WINKS_IMSI_LEN+1];             /*IMSI*/
#endif /* WINKS_DUAL_SIM */
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
    winks_EventWaitingQueue_s *eventWaiting_p;  /*系统事件等待单向链表指针*/
#else
    winks_EventWaitingQueue_s eventWaitingQueue[WINKS_OSAL_EVENT_WAITING_QUEUE_MAXNUM];  /*系统事件等待队列数组*/
#endif
    WINKS_CHN_ID fileChn;                       /*消息通道，用于接收调用文件管理器应用后的返回消息*/
    /*unsigned long filemsg;*/
    WINKS_CHN_ID DaChn;                         /*消息通道，用于接收调用接入点应用后的返回消息*/
    /*unsigned long Damsg;*/
    WINKS_CHN_ID pnChn;                         /*消息通道，用于接收调用联系人应用后的返回消息*/
    unsigned long fileSysErrorCode;
    void* hWnd1;                                /*APP1句柄*/
    void* hWnd2;                                /*APP2句柄*/
    WinksAppState_e hd1GetFocus;                /*  APP1运行状态.  0:应用退出； 1:应用启用；2:应用挂起*/
    WinksAppState_e hd2GetFocus;                /*  APP2运行状态.  0:应用退出； 1:应用启用；2:应用挂起*/
    unsigned long keyCode;                      /*按键down\up匹配记录*/
    unsigned long penCode;                      /*触摸笔down\up匹配记录*/
#ifdef WINKS_DEBUG
    long fileSysErrorCodeS;                     /*用于记录文件系统操作的错误代码*/
    int msgInQueueCnt;
    int msgOutQueueCnt;
#endif
    unsigned long testBitFlag;                  /*位标志。
                                                  	d0:应用背景开关; 
                                                    d1:电话接听标志位; 
                                                    d2:debug打印开关;
                                                    d3:按键打印信息开关;
                                                    d4:连网配服URL控制;
                                                    d5:任意键接听来电开关;
                                                    d6:设置项统一提交的标志;
                                                */
    char keyStack[WINKS_TEST_KEY_HISTORY_STACK_LEN+1];/*历史kaycode，用于密码，例如"*#100022#???#" */

    WINKS_CHN_ID osalChn;                       /*抽象层消息通道*/
#ifdef WINKS_SENDSMS_UPGRADE
    Winks_SmsQueueItem_s * smsQueueTail;        /*发送短信的消息队列-队尾*/
    Winks_SmsQueueItem_s * smsQueueHead;        /*发送短信的消息队列-对头*/
#endif

    char version[32];
/*  抽象层API的全局结构winks_OsApiContext_s
    注意这里 WINKS_OSAL_API_SELF_DATA_LEN>=sizeof(winks_OsApiContext_s)
*/  char osapis[WINKS_OSAL_API_SELF_DATA_LEN];	/*对应抽象层API的winks_OsApiContext_s*/

}winks_OsalContext_s;


/*创建消息通道*/
WINKS_CHN_ID Winks_CreateChn(WINKS_OSAL_CALLBACK fCallBack);

/*注销消息通道*/
int Winks_DestroyChn(WINKS_CHN_ID chn);

/*向消息通道发送异步消息*/
int Winks_PostMsg(WINKS_CHN_ID chn, unsigned long msg, void *data, unsigned long size);

/*注册抽象层OSAL Handler*/
int Winks_CreateOsalHandler(void);

/*注销抽象层OSAL Handler*/
int Winks_DestroyOsalHandler(void);




/*创建定时器*/
WINKS_TIMER_ID Winks_CreateTimer(WINKS_CHN_ID chn, WINKS_TICKS timeOutPeriod, unsigned long flag);

/*注销定时器*/
int Winks_DestroyTimer(WINKS_TIMER_ID timerId);

/*启动定时器*/
int Winks_StartTimer(WINKS_TIMER_ID timerId);

/*停止定时器*/
int Winks_StopTimer(WINKS_TIMER_ID timerId);


/*************************************************
        按键消息接口(WINKS_MSG_UI_KEY_EVENT)
**************************************************/
/** 按键的key code**/
typedef unsigned long Winks_KeyCode;
#define WINKS_KEY_NULL        0  
#define WINKS_KEY_1           1
#define WINKS_KEY_2           2
#define WINKS_KEY_3           3
#define WINKS_KEY_4           4
#define WINKS_KEY_5           5
#define WINKS_KEY_6           6
#define WINKS_KEY_7           7
#define WINKS_KEY_8           8
#define WINKS_KEY_9           9
#define WINKS_KEY_0           10
#define WINKS_KEY_STAR        11
#define WINKS_KEY_HASH        12
#define WINKS_KEY_CLEAR       30
#define WINKS_KEY_LSK         13
#define WINKS_KEY_RSK         14
#define WINKS_KEY_UP          15
#define WINKS_KEY_DOWN        16
#define WINKS_KEY_LEFT        17
#define WINKS_KEY_RIGHT       18
#define WINKS_KEY_SEND        19
#define WINKS_KEY_OK          20
#define WINKS_KEY_END         21
#define WINKS_KEY_SIDE_UP     22
#define WINKS_KEY_SIDE_DOWN   23
#define WINKS_KEY_CAMERA      24
#define WINKS_KEY_LID         25
#define WINKS_KEY_SLIP        26
#define WINKS_KEY_KEYPAD_LOCK 27
#define WINKS_KEY_EARPHONEKEY 28
#define WINKS_KEY_POWER       29

typedef enum tag_Winks_KeyUpDown
{
    WINKS_KEY_FLAG_KEYDOWN = 0,          /*key down , lid open or slip open*/
    WINKS_KEY_FLAG_KEYUP = 1,          /*key up , lid close or slip close*/
    WINKS_KEY_FLAG_KEY_LONG_PRESS = 2,
    WINKS_KEY_FLAG_KEY_ALWAYS_PRESS = 3
}Winks_KeyUpDown;

/** 按键消息的结构体 **/
typedef struct tag_Winks_MsgBody_KeyEvent_s
{
    Winks_KeyCode keyCode;
    Winks_KeyUpDown flag;
}Winks_MsgBody_KeyEvent_s;

typedef enum tag_Winks_PenUpDown
{
    WINKS_PEN_FLAG_PENDOWN = 0,             /*pen down , lid open or slip open*/
    WINKS_PEN_FLAG_PENUP = 1,               /*pen up , lid close or slip close*/
    WINKS_PEN_FLAG_PEN_LONG_PRESS = 2,
    WINKS_PEN_FLAG_PEN_ALWAYS_PRESS = 3,
    WINKS_PEN_FLAG_PENMOVE = 4
}Winks_PenUpDown;

/** 按键消息的结构体 **/
typedef struct tag_Winks_MsgBody_PenEvent_s
{
    int x;
    int y;
    Winks_PenUpDown flag;
}Winks_MsgBody_PenEvent_s;

/*************************************************
        刷屏消息接口(WINKS_MSG_UI_PAINT_EVENT)
**************************************************/
typedef struct tag_Winks_Rect_s
{
    int left;
    int top;
    int right;
    int bottom;
}Winks_Rect_s;
/*************************************************
        输入法返回消息(WINKS_MSG_UI_EDITOR_RSP)
**************************************************/
typedef struct tag_Winks_EditorRsp_s
{
    int result;/*1表示提交，0表示取消*/
    char data[1];/*提交的编辑数据，Unicode编码。result==0时该项无意义。*/
}Winks_EditorRsp_s;
/*输入法返回函数
    flag:1确认， 0取消
    str:输入法返回的字符串，Unicode编码
    成功返回0，失败返回-1*/
int Winks_EditorCallback(const char *str, int flag);
/*************************************************
         其他功能接口
**************************************************/

/*电话事件消息的返回结构*/
typedef struct tag_winks_CallMsgInfo_s
{
    char number[WINKS_CALL_NUMBER_LENGTH];
}winks_CallMsgInfo_s;

/*电话本变更事件消息的返回结构*/
#define WINKS_PBEVENT_ADD       1
#define WINKS_PBEVENT_REPLACE   2
#define WINKS_PBEVENT_DELETE    3
typedef struct tag_winks_PbModifiedMsgInfo_s
{
    unsigned char type;
    char number1[WINKS_CALL_NUMBER_LENGTH]; /*新增号码、变更前的号码、删除号码*/
    char number2[WINKS_CALL_NUMBER_LENGTH];/*变更后的号码*/
}winks_PbModifiedMsgInfo_s;

int Winks_SetWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);


/*增加通道等待的系统消息，flag的每一位标志一个系统消息*/
int Winks_AddWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);



/*取IMSI号*/
int Winks_GetImsi(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
#ifdef WINKS_DUAL_SIM
int Winks_GetImsi_2(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
/* 当得知彩像使用的是那张卡时调用此函数
   调用该函数为了Winks_GetImsi取到彩像匹配的imsi
*/
int Winks_SetActiveSim(int SimNo);
int Winks_GetActiveSim(void);
#endif /* WINKS_DUAL_SIM */


/*取IMEI号*/
int Winks_GetImei(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);

/*取紧急号码列表*/
typedef struct tag_Winks_EmergencyNumList_s
{
    unsigned long count;
    char data[1];
}Winks_EmergencyNumList_s;
int Winks_GetEmergencyNumbers(WINKS_CHN_ID chn, unsigned long msg);


#ifndef WINKS_SENDSMS_UPGRADE
#ifndef WINKS_DUAL_SIM
/*发送短消息*/
int Winks_SendSms(void *number, unsigned long number_size, void * buff, unsigned long buff_size);
#else /* WINKS_DUAL_SIM */
/*
* Winks_SendSms():
* 参数number是要发送的号码，格式为ASCII
* 参数buff是要发送的内容，格式为ASCII
* 参数sim是用那张Sim卡 1--sim卡1;2--sim卡2
*/
int Winks_SendSms(void *number, unsigned long number_size, void * buff, unsigned long buff_size, int SimNo);
/* 设置彩像联网用的Sim卡 */
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData);
/* 恢复手机默认联网用的Sim卡 */
int Winks_api_RestoreNetLinkSim(int SaveData);
/*  Sim卡可用返回1，否则返回0 */
int Winks_isSimEnabled(int sim_card_id);
#endif /* WINKS_DUAL_SIM */
#endif
/*调用WAP接口*/
int Winks_OpenWapPage( char* pUrl );

/*取Flash剩余空间，byte*/
typedef struct tag_Winks_FlashSpace_s
{
    unsigned long total;
    unsigned long free;
}Winks_FlashSpace_s;
int Winks_GetFlashSpace(Winks_FlashSpace_s *space);

/* 获取存储卡的空间, 单位: MB */
typedef struct tag_Winks_FlashSpace_MB_s
{
	unsigned long total;
	unsigned long free;
}Winks_FlashSpace_MB_s;

/*取国际漫游的状态标志，flag: 0非漫游; 1漫游*/
int Winks_GetRoamingFlag(unsigned char *flag);


/* 获取通话的开始时间和持续时间 */
int Winks_GetCallTime(Winks_DateTime_s *start_time, Winks_DateTime_s *duration);

/** 取当前语言种类 **/
 typedef enum tag_Winks_Language_s
{
    WINKS_LANG_ENGLISH = 0,
    WINKS_LANG_CHINESE = 1,
    WINKS_LANG_CHINESETW = 2,
    WINKS_LANG_CHINESEHK = 3,
    WINKS_LANG_CHINESESG = 4,
    WINKS_LANG_JAPANESE = 5,
    WINKS_LANG_NUMBER,
    WINKS_LANG_NULL
}Winks_Language_s;
Winks_Language_s Winks_GetCurrentLanguage(void);

/* 文件系统获取 */
#define WINKS_FILE_IMAGE    1         /*平台支持的所有图像文件*/
#define WINKS_FILE_JPG      10        /*jpg文件*/
#define WINKS_FILE_AUDIO    2         /*平台支持的所有声音文件*/
#define WINKS_FILE_VIDEO    3         /*平台支持的所有视频文件*/
#define WINKS_FILE_ALL      4         /*平台支持的所有文件*/

/*调用文件管理器模块，异步消息返回选择的文件(WINKS_MSG_UI_SELECT_FILE_RSP)。*/
int Winks_SelectFile( WINKS_CHN_ID chn, char* defpath, int type );

#define WINKS_DATAACC_CSD   1
#define WINKS_DATAACC_GPRS  2

/** (mtk) 初始化帐号资料, NOTE: 因为用户可以手动增删接入点, UI 必须在进入设置界面前调用。*/
void Winks_InitDataAccount(void (*callback_func)(void));

/*调用接入点选择模块，异步消息返回选择的接入点(WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP)。*/
int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account );

/*调用联系人模块，异步消息返回选择的电话号码(WINKS_MSG_UI_SELECT_PHONENUM_RSP)。*/
int Winks_SelectPhoneNumber(WINKS_CHN_ID chn);

/** 取系统信息 **/
#define WINKS_VERSION_MAX_LEN 20
typedef struct tag_Winks_SysInfo_s
{
    char sv[WINKS_VERSION_MAX_LEN];           /*Software version*/
    char hv[WINKS_VERSION_MAX_LEN];           /*Hardware version*/
    char wv[WINKS_VERSION_MAX_LEN];           /*Winks version*/
}Winks_SysInfo_s;
int Winks_GetSysInfo(Winks_SysInfo_s *sysInfo);



/** 取系统的TICK **/
WINKS_TICKS Winks_GetSysTick(void);

/*用于UI向抽象层注册其消息通道，以获取键盘等消息*/
int Winks_RegisterUiChn(WINKS_CHN_ID chn);
int Winks_SetOsalTestBitFlag(unsigned long bit, unsigned long val);
int Winks_GetOsalTestBitFlag(unsigned long bit, unsigned long *val);
int Winks_SetBackgroundFlag(unsigned long val);
int Winks_GetBackgroundFlag(unsigned long *val);
winks_OsalContext_s * winks_GetOsalContextPtr(void);

#define WINKS_APN_NAME_MIN_LEN 40
#define WINKS_DATA_ACCOUNT_MIN_LEN 40

#define WINKS_FRIEND_NO_LEN 40
#define WINKS_MAX_NUMBER_COUNT 8
typedef struct tag_Winks_PhoneBookItem_s
{
    char number[WINKS_MAX_NUMBER_COUNT][WINKS_FRIEND_NO_LEN];
}Winks_PhoneBookItem_s;

/*取电话本的记录个数*/
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);
/*取指定电话本记录中的所有电话号码*/
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem);

/*取Unicode的strlen.*/
int WK_STRLEN(const char *str);
/* 字符编码转换函数 */
int Winks_Ascii2UnicodeString(const char *in_ascii, char *out_unicode, unsigned int size);
int Winks_Unicode2AsciiString(const char *in_unicode, char *out_ascii, unsigned int size);
int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size);
int Winks_Unicode2UtfString(const char *in_unicode, char *out_utf8, unsigned int size);
int Winks_SetStartCallTime(Winks_DateTime_s *start_time);
int Winks_ProEventWaitingQueue(unsigned long event, unsigned long msg, void *data, unsigned long size);
int winks_strcmp_last_4(const char *str1, const char *str2);
int Winks_SelectFileCallBack( void* pPath, unsigned long len );
int Winks_SelectDataAccountCallBack(unsigned long accountid);
int Winks_SelectPhoneNumberCallBack( void* pPhoneNum);
void Winks_OsalTimerExpiry(void);
void winks_osalCallBack( void);
void wk_osal_DoWaitingImsi(char *msg_p);
void wk_osal_DoWaitingImei(char *msg_p);
#ifdef WINKS_DUAL_SIM
void wk_osal_DoWaitingImsi_2(char *imsi_str);
#endif /* WINKS_DUAL_SIM */

/*设置控制位*/
int Winks_SetBitFlag(unsigned long bit, unsigned long val, void *src);
/*读取控制位*/
int Winks_GetBitFlag(unsigned long bit, unsigned long *val, void *src);

int Winks_IsOsalEnable(void);
/*************************************************
       应用
**************************************************/
/*标识应用，用户点击菜单进入的应用和电话事件触发的应用*/
typedef enum tag_Winks_Hwnd_e
{
    WINKS_MINGUI_HWND_USER = 1,
    WINKS_MINGUI_HWND_CALL = 2
}Winks_Hwnd_e;

typedef enum tag_WinksAppPauseResume_e
{
    WINKS_APP_PAUSE = 1,
    WINKS_APP_RESUME = 2
}WinksAppPauseResume_e;

/*WINKS_MSG_UI_PAUSE_RESUME_APP消息携带结构*/
typedef struct tag_WinksPauseResume_s
{
    Winks_Hwnd_e app;              /*标识应用*/
    WinksAppPauseResume_e flag;    /*挂起恢复标志*/
}WinksPauseResume_s;

#define WINKS_HANDLER_WINDOWS_NULL 0x00
int Winks_RegisterHwnd(Winks_Hwnd_e opt, void* hWnd);
int Winks_GetHwnd(Winks_Hwnd_e opt, void* *hWnd);

int Winks_PostMsgToUi(unsigned long msg, void *data, unsigned long size);

/*设置应用获得焦点标志*/
int winks_SetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e focesFlag);
int winks_GetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e *focesFlag);

int winks_SetUiDestroyAppFlag(Winks_Hwnd_e opt, unsigned char destroyFlag);
int winks_GetUiDestroyAppFlag(Winks_Hwnd_e opt, unsigned char *destroyFlag);
/*键盘处理*/
int winks_keyEvent(Winks_Hwnd_e opt, Winks_MsgBody_KeyEvent_s keyEvent);
/*触摸屏处理*/
int winks_penEvent(Winks_Hwnd_e opt, Winks_MsgBody_PenEvent_s penEvent);
/*屏幕重绘处理*/
int winks_PaintEvent(Winks_Hwnd_e opt, Winks_Rect_s rtc);

/*向UI发送应用启动消息*/
int Winks_AppCreateMsgToUI( Winks_Hwnd_e opt );
/*向UI发送应用退出消息*/
int Winks_AppDestroyMsgToUI( Winks_Hwnd_e opt );
/*向UI发送应用挂起回复消息*/
int Winks_AppPauseResumeMsgToUI( Winks_Hwnd_e opt,  WinksAppPauseResume_e flag);
/*启动应用处理*/
int winks_procLoadApp(Winks_Hwnd_e opt, void* hWnd);
/*暂停后重新启动应用处理*/
int winks_procResumeApp(Winks_Hwnd_e opt);
/*暂停应用处理*/
int winks_procPauseApp(Winks_Hwnd_e opt);
/*退出应用处理*/
int winks_procUnloadApp(Winks_Hwnd_e opt);
/*取抽象层API模块的私有全局变量的指针*/
void* Winks_GetOsalApiPtr(void **p);

/*************************************************
        屏幕功能接口
**************************************************/
typedef struct winks_Rect
{
    int  v_Top;
    int  v_Left;
    int  v_Bottom;
    int  v_Right;
} winks_Rect_s;
/*add for editor by zouhy*/
typedef enum winks_mmi_chset
{
    WINKS_INPUT_TYPE_NUMERIC_ONLY,
    WINKS_TYPE_PINYIN,
    WINKS_INPUT_TYPE_UPPERCASE_ABC,
    WINKS_INPUT_TYPE_LOWERCASE_ABC,
    WINKS_INPUT_TYPE_NUMERIC_EN,
    WINKS_INPUT_TYPE_EN,
    WINKS_TYPE_PINYIN_CUR
} winks_mmi_chset_enum;
/*add for editor by zouhy*/
typedef enum winks_mmi_editor_title
{
    WINKS_MODIFY_NAME_TITLE,
    WINKS_EDIT_GREET,
    WINKS_POEXY_SETTING,
    WINKS_PORT_SETTING,
	WINKS_TIPS_INPUT_NUMBER,
	WINKS_TIPS_PICK_PICTURE,
	WINKS_TIPS_SHARE_TEXT
} winks_mmi_editor_title_enum;
typedef enum WINKS_FONT_TAG
{
    WINKS_SMALL_FONT = 1,
    WINKS_MEDIUM_FONT,
    WINKS_LARGE_FONT
}WINKS_FONT;
typedef enum WINKS_FONT_FORMAT
{
	WINKS_FONT_STANDARD = 0x00000001,
	WINKS_FONT_BOLDFACED = 0x00000010,
	WINKS_FONT_INCLINE = 0x00000100,
	WINKS_FONT_UNDERLINE = 0x00001000

}WINKS_FONT_FORMAT_E;
typedef struct Winks_font_feature
{
	WINKS_FONT font_size;
	unsigned long font_format;
	
}Winks_font_feature_s;
/* 字符串描述结构 */
typedef struct tag_Winks_Widget_Font_s
{
    int style;    //粗，斜，下划线 wk_osal.h
    int size;     //大，中，小 wk_osal.h
    unsigned short type;     //黑，宋。。。
    unsigned short way;      //滚动，截断，左右(超长时)
    unsigned long color;
}Winks_Widget_Font_s;
/*#include "Gdi_const.h"*/
#ifndef WINKS_USE_PLATFORM_SOURCE_MANAGER
/*资源处理*/
#define WINKS_IMG_TYPE_INVALID 0
#define WINKS_IMG_TYPE_BMP 1
#define WINKS_IMG_TYPE_GIF 2
#define WINKS_IMG_TYPE_JPG 3
#define WINKS_IMG_TYPE_PNG 4
/*初始化资源管理机制*/
int winks_ui_imgcacheinit(void);
/*注销资源管理机制*/
int winks_ui_imgcachedst(void);
/*取文件对应的数据*/
int winks_ui_getimgdata( const char* pname, char** ppdata, int* plen, int* ptype );

/*初始化皮肤*/
int winks_ui_initskinimag( void );
#endif
extern void winks_opacity_on(int x,int y,int width, int hight);
extern void winks_opacity_off(int x,int y,int width, int hight);
#define WINKS_OPACITY_ON(x,y,width,hight ) {winks_opacity_on( x,y,width,hight);}
#define WINKS_OPACITY_OFF(x,y,width,hight ) {winks_opacity_off( x,y,width,hight);}

extern int winks_gary_begin(unsigned long flag,int x1,int y1,int x2,int y2);
extern int winks_gary_end(unsigned long flag,int x1, int y1,int x2,int y2);

#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
/*根据ID取代理服务器的IP与prot。*/
int Winks_api_getProxy(void* apnID,char *Proxy, unsigned long proxySize, unsigned short *port);
#endif

#define WINKS_INVALID_DATA_ACCOUNT_INDEX 0xffffffff
#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
/* 该函数强行在系统的接入点列表中设置指定的接入点方式，如果已有则修改，如果没有则新增。
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
unsigned long Winks_api_SysAddDataAccount(unsigned long defIndex, char *apn, char* proxyIp, unsigned short proxyPort);
#else
unsigned long Winks_api_SysAddDataAccount(unsigned long defIndex,char *apn);
#endif


#ifdef WINKS_DEBUG
/*将数据块写入debug file。实现Winks_printf函数的时候调用。
 * function: Winks_DebugFile
 * parameters:
 *   char *fileName: 文件名，用于写debug信息。
 *   char* buff: debug数据块。
 *   unsigned long buffSize: debug数据块的大小。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_DebugFile(char *fileName, char* buff, unsigned long buffSize);
#endif



extern int Winks_set_font_feature (Winks_font_feature_s * font_feature );


#ifdef __cplusplus
}
#endif

#endif/*_WK_OSFNC_H_*/
