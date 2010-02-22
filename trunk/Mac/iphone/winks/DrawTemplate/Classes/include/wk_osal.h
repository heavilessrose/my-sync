/**************************************************************************
Copyright (C) Beijing Mobile Winks Co., Ltd
All rights reserved.
 
This source code is part of winks software and is licensed by WINKS SOFTWARE LICENSE AGREEMENT.
 
The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.
 
THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL ANY PERSON WHO HAS CONTRIBUTED TO OR IS THE OWNER OF ANY PART OF THIS SOFTWARE BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************
*Id: wk_osal.h,v 1.00 04/03/2009
**************************************************************************    
* File Description
* ----------------
* Color call AL global API. 
**************************************************************************/
#ifndef _WK_OSAL_H_
#define _WK_OSAL_H_

#ifdef __cplusplus
extern "C" {
#endif

//Socket 操作码及通知消息事件类型数值
#define WINKS_SO_CONNECT     0x01    //连接
#define WINKS_SO_READ        0x02    //读取
#define WINKS_SO_WRITE       0x04    //写入
#define WINKS_SO_CLOSE       0x08    //关闭

#define WINKS_SO_GHBFLEN     100        //域名查询缓冲区建议长度

#define WINKS_SO_SUCCESS     0
#define WINKS_SO_FAILURE     -1

#define WINKS_SO_EWOULDBLOCK        -10
#define WINKS_SO_ELOWSYSRESOURCE    -11
#define WINKS_SO_EINVALID_SOCKET    -12
#define WINKS_SO_EINVALIDPARA       -14
#define WINKS_SO_EINPROGRESS        -15
#define WINKS_SO_EOPNOTSUPP         -16
#define WINKS_SO_ECONNABORTED       -17
#define WINKS_SO_EINVAL             -18
#define WINKS_SO_ENOTCONN           -19
#define WINKS_SO_EMSGSIZE           -20
#define WINKS_SO_ENETDOWN           -21
#define WINKS_SO_ERESET             -22
#define WINKS_SO_EUNKNOWERROR       -23

/* Push recv error code */
#define WINKS_SO_PUSHEDEST  -2
#define WINKS_SO_PUSHEAPPID -3

#ifndef _WK_PUBLIC_H_
#include "wk_public.h"
#endif

/*************************************************
            文件系统接口
**************************************************/

#define WINKS_FS_ATTR_IS_RDONLY     0x01    /* 只读属性 */
#define WINKS_FS_ATTR_IS_HIDDEN     0x02    /* 隐藏属性 */
#define WINKS_FS_ATTR_IS_SYSTEM     0x04    /* 系统属性 */
#define WINKS_FS_ATTR_IS_ARCHIVE    0x08    /* 归档属性 */
#define WINKS_FS_ATTR_IS_DIR        0x10    /* 目录 */
#define WINKS_FS_ATTR_IS_ASCII      0x100   /* ascii码字符串 */
#define WINKS_FS_ATTR_IS_DOT        0x200   /* 根目录'.'或上级目录'..' */

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
/*取文件操作错误码函数。返回值: 0成功； -1失败。
*errorCode 为返回的错误码。*/
int Winks_GetFileSysErrorCode(unsigned long *errorCode);

/* unicode 与 ascii 转换函数 */
#if 0
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

/*************************************************
           目录操作接口
**************************************************/
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

/*删除指定的目录。(循环递归实现) 
 * function: Winks_XDelDir
 * parameters:
 *   char *dir: [in]待删除的目录，Utf8编码。
 * return:
 *   成功返回:成功返回0.
 *   失败返回:-1
 */
int Winks_XDelDir(char *dir);

/* 获取文件夹中所有文件大小,调用前*RetSize必须置0 */
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


/*************************************************
           取系统时间接口
**************************************************/
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
int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result);

/*按照指定格式将时间转换成字符串。
dateTime_p为表示时间的结构体指针，
当dateTime_p==NULL时取系统时间，随后按指定格式返回。*/
int Winks_FormatDateTime(Winks_DateTimeFormat_e format, char *buff, unsigned char size, Winks_DateTime_s *dateTime_p);

/*计算从1970 年1 月1 日0 时起到指定时间经过的秒数.
tm:输入的时间。如果tm=NULL，取系统时间(相当于time)。
return unsigned long型的秒数，0表示失败。*/
unsigned long Winks_mktime (Winks_DateTime_s *dtp);

/** 取系统的时区 **/
int Winks_GetSysTimeZone(Winks_TimeZone_e *time_zone);

/** 取系统的TICK **/
typedef unsigned long WINKS_TICKS;
WINKS_TICKS Winks_GetSysTick(void);





/*************************************************
            消息通道接口
**************************************************/
/* WINKS消息起始值 */
#define WINKS_MSG_BASE                           0xF000
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
#define WINKS_MSG_CALL_MULTIPART                (WINKS_MSG_OSAL+0x0e)   /* 多方通话  */
#define WINKS_MSG_IMSI_REQ                      (WINKS_MSG_OSAL+0x0f)   /* 取IMSI号 */
#define WINKS_MSG_CALL_QUERY_EMERGENCY          (WINKS_MSG_OSAL+0x10)   /* 取紧急呼叫号码*/
#define WINKS_MSG_PHONE_BOOK_MODIFIED           (WINKS_MSG_OSAL+0x11)   /* 电话本变更消息*/
#ifdef WINKS_DUAL_SIM
#define WINKS_MSG_IMSI_2_REQ                    (WINKS_MSG_OSAL+0x12)   /* 取IMSI2号 */
#endif
#define WINKS_MSG_USB_DETECT                    (WINKS_MSG_OSAL+0x13)   /* USB 检测消息*/
#define WINKS_MSG_TEST                          (WINKS_MSG_OSAL+0x14)   /* For test. */ 
#define WINKS_MSG_FS_INVALID                    (WINKS_MSG_OSAL+0x15)   /* File system invalid.*/ 
#define WINKS_MSG_IMEI_REQ                      (WINKS_MSG_OSAL+0x16)   /* 取IMEI号 */
#define WINKS_MSG_SEND_SMS_CFM                  (WINKS_MSG_OSAL+0x17)   /* 短信发送结果报告。低八位为句柄;高八位为操作结果,0为失败,1为成功.*/

/** 基于抽象层的消息，由HPPT定义。**/
#define WINKS_MSG_HTTP                          (WINKS_MSG_BASE+0x400)
/** 基于抽象层的消息，由引擎定义。**/
#define WINKS_MSG_ENGINE                        (WINKS_MSG_BASE+0x500)
#define WINKS_MSG_ENGINE_HTTP                   (WINKS_MSG_ENGINE+0x01)
#define WINKS_MSG_ENGINE_INIT_END               (WINKS_MSG_ENGINE+0x02) /*引擎初始化完毕*/
#define WINKS_MSG_ENGINE_PUSH                   (WINKS_MSG_ENGINE+0x03)
#define WINKS_MSG_ENGINE_RETMSG                 (WINKS_MSG_ENGINE+0x04)
#define WINKS_MSG_ENGINE_UIRETMSG               (WINKS_MSG_ENGINE+0x05) /*引擎联网返回异步消息*/
#define WINKS_MSG_ENGINE_UPDATE_FL              (WINKS_MSG_ENGINE+0x06)
#define WINKS_MSG_ENGINE_DIYSET                 (WINKS_MSG_ENGINE+0x07)
#define WINKS_MSG_ENGINE_SETTINGSET             (WINKS_MSG_ENGINE+0x08)
#define WINKS_MSG_ENGINE_SNED_REG_SMS           (WINKS_MSG_ENGINE+0x09)
#define WINKS_MSG_ENGINE_SELECT_FRIEND_LIST     (WINKS_MSG_ENGINE+0x0a)
#define WINKS_MSG_ENGINE_REG_NOTIFY             (WINKS_MSG_ENGINE+0x0b)
#define WINKS_MSG_ENGINE_INIT_CACHE             (WINKS_MSG_ENGINE+0x0c)
#define WINKS_MSG_ENGINE_REBUILD_CACHE          (WINKS_MSG_ENGINE+0x0d)
#define WINKS_MSG_ENGINE_RECRUIT_DOWNLOAD       (WINKS_MSG_ENGINE+0x0e)
#define WINKS_MSG_ENGINE_APP_START              (WINKS_MSG_ENGINE+0x10) /*引擎应用机制启动事件*/
#define WINKS_MSG_ENGINE_APP_STOP               (WINKS_MSG_ENGINE+0x11) /*引擎应用机制停止事件*/
#define WINKS_MSG_ENGINE_APP_PAUSE              (WINKS_MSG_ENGINE+0x12) /*引擎应用机制暂停事件*/
#define WINKS_MSG_ENGINE_APP_RESUME             (WINKS_MSG_ENGINE+0x13) /*引擎应用机制暂停恢复事件*/
#define WINKS_MSG_ENGINE_APP_RETMSG             (WINKS_MSG_ENGINE+0x14) /*引擎应用机制联网返回事件*/
#define WINKS_MSG_ENGINE_APP_DATAOUT            (WINKS_MSG_ENGINE+0x15) /*引擎应用机制联网数据发送事件*/
#define WINKS_MSG_ENGINE_APP_IDLE               (WINKS_MSG_ENGINE+0x16) /*引擎应用机制最后一个应用退出事件*/
#define WINKS_MSG_ENGINE_CC_DOWNLOAD_OK         (WINKS_MSG_ENGINE+0x17) /*引擎实时下载彩像完成消息。*/

/** 基于抽象层的消息，由UI定义。**/
#define WINKS_MSG_UI                            (WINKS_MSG_BASE+0x800)
#define WINKS_MSG_UI_KEY_EVENT                  (WINKS_MSG_UI+0x01)
#define WINKS_MSG_UI_DESTROY_APP                (WINKS_MSG_UI+0x02)     /*当应用被销毁时发生*/
#define WINKS_MSG_UI_CREATE_APP                 (WINKS_MSG_UI+0x03)     /*当应用被创建时发生。可能从菜单进入或来电事件触发。*/
#define WINKS_MSG_UI_PAINT_EVENT                (WINKS_MSG_UI+0x04)     /*当界面有失效区域时发生，收到该时间UI需要重绘指定区域。*/
#define WINKS_MSG_UI_PEN_EVENT                  (WINKS_MSG_UI+0x05)     /*画笔消息*/
#define WINKS_MSG_UI_SELECT_FILE_RSP            (WINKS_MSG_UI+0x06)     /*取系统多媒体文件的返回消息*/
#define WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP    (WINKS_MSG_UI+0x07)     /*取系统接入点的返回消息*/
#define WINKS_MSG_UI_PAUSE_RESUME_APP           (WINKS_MSG_UI+0x08)     /*应用挂起恢复消息*/
#define WINKS_MSG_UI_EDITOR_RSP                 (WINKS_MSG_UI+0x09)     /*调用输入法后平台的返回消息*/
#define WINKS_MSG_UI_SELECT_PHONENUM_RSP        (WINKS_MSG_UI+0x0a)     /*取联系人返回消息*/
#define WINKS_MSG_UI_REG_IMMEDIATELY            (WINKS_MSG_UI+0x0b)     /*来电直接激活彩像软件*/
#define WINKS_MSG_UI_COL_IMMEDIATELY            (WINKS_MSG_UI+0x0c)     /*一键收藏消息, 携带数据字符串wid*/
#define WINKS_MSG_UI_EDITOR_STATE_RSP           (WINKS_MSG_UI+0x0d)     /*状态、输入法类型*/
#define WINKS_MSG_UI_EMBEDDED_BROWSER_RSP       (WINKS_MSG_UI+0x0e)     /*内嵌浏览器消息*/
#define WINKS_MSG_UI_MODIFY_SIZE_RSP            (WINKS_MSG_UI+0x0f)     /*屏幕修改(转屏或大小改变)*/
#define WINKS_MSG_UI_PEERNUM_NAME               (WINKS_MSG_UI+0x10)     /* 获得联系人姓名 */
#define WINKS_MSG_UI_CREATE_WINDOW               (WINKS_MSG_UI+0x11)     /* 当窗口被创建时发生 */





#ifdef  WINKS_EMBEDDED_BROWSER
typedef struct tag_Winks_Editor_State_s
{
    unsigned char  open_state;      /* 开启状态(用 Winks_Input_Method_e)*/
    
    unsigned char  softkeyboard_state;       /* 软件盘状态（开启状态）(用 Winks_Input_Method_e)*/
    unsigned short  input_type[128];     /*输入法类型 */
}Winks_Editor_State_s;
typedef struct tag_Winks_Embedded_Browser_State_s
{
    unsigned char  Progress_state;      /* 进度*/

}Winks_Embedded_Browser_State_s;

/*  浏览器*/
typedef enum tag_Winks_Embedded_Browser_State_e{
    WINKS_Embedded_Browser_Start,/*开始点*/
    WINKS_Embedded_Browser_Finsh,          /* 、结束点*/
    WINKS_Embedded_Browser_Fail     /* 加载失败）*/

}Winks_Embedded_Browser_State_e;



extern int winks_Embedded_Browser_open(char * url,int x,int y, int width,int height, unsigned long *  browser_handle);
extern int winks_Embedded_Browser_resize(int x,int y, int width,int height, unsigned long  browser_handle);
extern int winks_Embedded_Browser_close( unsigned long   browser_handle);
extern int winks_Embedded_Browser_refresh( unsigned long   browser_handle);
    
typedef enum Winks_Input_Method
{
	WINKS_INPUT_METHOD_INVALIDATE = 0,
    WINKS_INPUT_METHOD_OPEN,
    WINKS_INPUT_METHOD_CLOSE
}Winks_Input_Method_e;

extern  int winks_Input_Method_SoftKeyboard_Option( Winks_Input_Method_e Option);

#ifdef WINKS_BREW
extern int winks_SetHelpType(helptype_id id);
#endif // #ifdef WINKS_BREW

#endif




/* USB 检测动作 */
#define WINKS_USB_PLUG_IN 1
#define WINKS_USB_PLUG_OUT 2

typedef unsigned long WINKS_CHN_ID;
#define WINKS_CHN_NULL ((WINKS_CHN_ID)0)
typedef int (*WINKS_OSAL_CALLBACK)(unsigned long msg, void *data, unsigned long size, WINKS_CHN_ID chnId, void*chnData);

/** 创建消息通道 **/
WINKS_CHN_ID Winks_CreateChn(WINKS_OSAL_CALLBACK fCallBack);

/** 注销消息通道 **/
int Winks_DestroyChn(WINKS_CHN_ID chn);

/*取消息通道上下文.输入消息通道句柄，返回上下文指针。*/
void *Winks_GetChnContext(WINKS_CHN_ID chn);

/*设置息通道上下文.输入消息通道句柄，和上下文指针，成功返回0，失败返回-1.*/
int Winks_SetChnContext(WINKS_CHN_ID chn, void* chnData);

/** 向消息通道发送异步消息 **/
int Winks_PostMsg(WINKS_CHN_ID chn, unsigned long msg, void *data, unsigned long size);

/** 注册抽象层OSAL Handler **/
int Winks_CreateOsalHandler(void);

/** 注销抽象层OSAL Handler **/
int Winks_DestroyOsalHandler(void);



/*************************************************
            定时器接口
**************************************************/
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





/*************************************************
            等待事件开关
**************************************************/
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
#define WINKS_EVENT_CALL_MULTIPART      0x00001000  /* 多方通话事件 */
#define WINKS_EVENT_OF_ALL_CALL         0x0000FFFF  /*全部电话事件*/
#define WINKS_EVENT_PB_MODIFIED         0x00010000  /* 电话本变更 */
#define WINKS_EVENT_USB_DETECT          0x00020000  /* USB 检测事件 */
#define WINKS_EVENT_FS_INVALID          0x00040000  /* File system invalid. here is a event.*/ 
#define WINKS_EVENT_ENGINE_INIT_END     0x00080000  /* 引擎初始化结束消息。 */
#define WINKS_EVENT_CC_DOWNLOAD_OK      0x00100000  /* 引擎实时下载彩像完成事件。*/
#define WINKS_EVENT_TEST_KET            0x80000000  /* TEST KEY EVENT */

#define WINKS_FSINVALID_CAUSED_USB      ((unsigned long)1)      /* 由插入USB线事件所引发的文件系统不能使用。*/

#define WINKS_CALL_NUMBER_LENGTH    40  /* ref "WINKS_FRIEND_NO_LEN" */
#define WINKS_CALL_NAME_LENGTH      24   /* ref "WINKS_FRIEND_NAME_LEN" */
typedef struct tag_winks_CallMsgInfo_s{
    char number[WINKS_CALL_NUMBER_LENGTH];
    /*char name[WINKS_CALL_NAME_LENGTH];*/
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

/*消息原函数，调用该函数，实现注册消息的转发。*/
int Winks_ProEventWaitingQueue(unsigned long event, unsigned long msg, void *data, unsigned long size);

/** 设置通道等待的系统消息，flag的每一位标志一个系统消息 **/
int Winks_SetWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);

/** 增加通道等待的系统消息，flag的每一位标志一个系统消息 **/
int Winks_AddWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);

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

/*调用接入点选择模块，异步消息返回选择的接入点(WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP)。*/
int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account );

/*调用联系人模块，异步消息返回选择的电话号码(WINKS_MSG_UI_SELECT_PHONENUM_RSP)。*/
int Winks_SelectPhoneNumber(WINKS_CHN_ID chn);
/*************************************************
          Sochet接口
**************************************************/
#ifndef AF_INET

#define SOCKET_ERROR    (-1)
#define SOCK_STREAM     1               /* stream socket */
#define SOCK_DGRAM      2               /* datagram socket */
#define SOCK_RAW        3               /* raw-protocol interface */

#define SOCK_RDM    4
#define SOCK_SEQPACKET    5
#define TCP_NODELAY    0x0001
#define TCP_KEEPIDLE    0x0004          /* Start keeplives after this period */

/*
 * Address families.
 */
#define AF_UNSPEC    0
#define AF_UNIX      1
#define AF_INET      2
#define AF_IMPLINK   3
#define AF_PUP       4
#define AF_CHAOS     5
#define AF_IPX       6
#define AF_NS        6
#define AF_ISO       7
#define AF_OSI       AF_ISO
#define AF_ECMA      8
#define AF_DATAKIT   9
#define AF_CCITT     10
#define AF_SNA       11
#define AF_DECnet    12
#define AF_DLI       13
#define AF_LAT       14
#define AF_HYLINK    15
#define AF_APPLETALK 16
#define AF_NETBIOS   17
#define AF_VOICEVIEW 18
#define AF_FIREFOX   19
#define AF_UNKNOWN1  20
#define AF_BAN       21
#define AF_ATM       22
#define AF_INET6     23
#define AF_CLUSTER   24
#define AF_12844     25
#define AF_IRDA      26
#define AF_NETDES    28                     
#define AF_MAX       29

#endif
typedef struct tag_Winks_Socketmsg_s
{
    unsigned long wParam;
    unsigned long lParam;
}Winks_Socketmsg_s;

typedef struct tag_Winks_Push_s
{
    char* Appid;
    WINKS_CHN_ID channel;
    int msg;
    unsigned short port;
}Winks_Push_s;

typedef struct tag_Winks_hostent 
{
    char* h_name;           /* official name of host */
    char** h_aliases;          /* alias list */
    short h_addrtype;       /* host address type */
    short h_length;         /* length of address */
    char** h_addr_list;     /* list of addresses */
#define h_addr h_addr_list[0]   /* address, for backward compat */
}Winks_hostent;

struct winks_sockaddr
{ 
    unsigned short sa_family; 
    char sa_data[14]; 
}; 

struct winks_in_addr
{
    unsigned int wk_addr;
};

struct winks_sockaddr_in
{ 
    unsigned short sin_family; 
    unsigned short int sin_port; 
    struct winks_in_addr sin_addr; 
    unsigned char sin_zero[8]; 
};


int Winks_SoStartup( void );
int Winks_SoCleanup( void );
int Winks_getlasterror( void );
int Winks_socket( int family ,int type ,int protocol );
int Winks_asyncselect( int socket, int opcode, WINKS_CHN_ID channel, int msg );
int Winks_connect(int socket, struct winks_sockaddr* serv_addr, int addrlen );
int Winks_send( int socket, void *buf, int len, int flags );
int Winks_recv( int socket, void *buf, int len, int flags );
int Winks_closesocket( int socket );
int Winks_AsyncGetHostByName( char* name, char* pHost, int hostlen, WINKS_CHN_ID channel, int msg );
int Winks_CancelGetHostByName( int handle );
unsigned short Winks_htons( unsigned short port );
#ifdef WINKS_SO_PUSHSUPPORT
int Winks_pushcreate( Winks_Push_s* pPush );
int Winks_pushrecv(int push, char* pDest, int* destlen, 
                   char* pAppid, int* appidlen, char* pdata, int* datalen );
int Winks_pushclose( int push );
#endif
int Winks_setdialid( unsigned long id );
int Winks_Get_Dialer_id( void );

/*************************************************
        按键消息接口
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
#ifdef  WINKS_DUAL_SIM
#define WINKS_KEY_SEND2       31    /* mtk平台与WINKS_KEY_END复用就行 */
#endif
#define WINKS_KEY_MAX         32

typedef enum tag_Winks_KeyUpDown
{
    WINKS_KEY_FLAG_KEYDOWN = 0,          /*key down , lid open or slip open*/
    WINKS_KEY_FLAG_KEYUP = 1,          /*key up , lid close or slip close*/
    WINKS_KEY_FLAG_KEY_LONG_PRESS = 2,
    WINKS_KEY_FLAG_KEY_ALWAYS_PRESS = 3
}Winks_KeyUpDown;

/** 按键消息的结构体(WINKS_MSG_UI_KEY_EVENT) **/
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

/** 触摸屏画笔消息的结构体(WINKS_MSG_UI_PEN_EVENT) **/
typedef struct tag_Winks_MsgBody_PenEvent_s
{
    int x;
    int y;
    Winks_PenUpDown flag;
}Winks_MsgBody_PenEvent_s;

/** 输入法返回消息的结构体(WINKS_MSG_UI_EDITOR_RSP) **/
typedef struct tag_Winks_EditorRsp_s
{
    int result;/*1表示提交，0表示取消*/
    char data[2];/*提交的编辑数据，Unicode编码。result==0时该项无意义。*/
}Winks_EditorRsp_s;
/*************************************************
        刷屏消息接口(WINKS_MSG_UI_PAINT_EVENT)
**************************************************/
typedef struct tag_Winks_Rect_s
{
    /**
     * the x coordinate of the upper-left corner of the rectangle.
     */
    int left;
    /**
     * the y coordinate of the upper-left corner of the rectangle.
     */
    int top;
    /**
     * the x coordinate of the lower-right corner of the rectangle.
     */
    int right;
    /**
     * the y coordinate of the lower-right corner of the rectangle.
     */
    int bottom;
}Winks_Rect_s;

void Winks_Refresh(Winks_Rect_s rtc);
/**********************************
                Graphic API
#define Winks_pixel gal_pixel
#define Winks_Bitmap_s BITMAP
void Winks_DrawPixel(long x, long y, Winks_pixel color);
void Winks_DrawPixelRgb(long x, long y, unsigned long r, unsigned long g, unsigned long b);
void Winks_DrawLine(long x0, long y0, long x1, long y1, Winks_pixel color);
void Winks_DrawRectangle(long x, long y, long w, long h, Winks_pixel color);
void Winks_DrawCircle(long sx, long sy, unsigned long r, Winks_pixel color);
void Winks_DrawEllipse(long sx, long sy, long rx, long ry, Winks_pixel color);
void Winks_FillBox(long x, long y, long w, long h, Winks_pixel color);

#define WINKS_BITMAP_KEY_COLOR_DISABLED 0x00
#define WINKS_BITMAP_KEY_COLOR_ENABLED 0x01
int Winks_FillBoxWithBitmap(long x, long y, long w, long h, Winks_Bitmap_s *pBitmap, unsigned char keyColorFlag);
int Winks_FillBoxWithBitmapFile(long x, long y, long w, long h, char *filename, unsigned char keyColorFlag);

***********************************/

/*************************************************
         其他功能接口
**************************************************/
/** 取IMSI号接口 **/
#define WINKS_IMSI_LEN 15
int Winks_GetImsi(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
#ifdef WINKS_DUAL_SIM
int Winks_GetImsi_2(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
/* 当得知彩像使用的是那张卡时调用此函数
   调用该函数为了Winks_GetImsi取到彩像匹配的imsi
*/
int Winks_SetActiveSim(int SimNo);
int Winks_GetActiveSim(void);
#endif /* WINKS_DUAL_SIM */

/** 取IMSI号接口 **/
#define WINKS_IMEI_LEN 32
int Winks_GetImei(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);

/** 取紧急号码列表 **/
typedef struct tag_Winks_EmergencyNumList_s
{
    unsigned long count;
    char data[1];
}Winks_EmergencyNumList_s;

// Phone number max len
#define WINKS_FRIEND_NO_LEN 40
#define WINKS_MAX_NUMBER_COUNT 8
typedef struct tag_Winks_PhoneBookItem_s
{
    char number[WINKS_MAX_NUMBER_COUNT][WINKS_FRIEND_NO_LEN];
}Winks_PhoneBookItem_s;

/*取电话本的记录个数*/
//phone_cnt，返回本机电话的条数
//sim_cnt,返回sim卡电话的条数
//返回值：0表示成功，-1表示失败
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);
/*取指定电话本记录中的所有电话号码*/
//type，1表示电话本机，2表示sim卡
//index，表示第几条，从0开始
//numberItme，返回的电话号码放在此参数中
//返回值，0表示成功，-1表示失败
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem);
/* 从电话本中取号码对应的名字(UTF-8 编码) */
void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len);

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * 用默认的SIM卡发送短信。
 * function: Winks_SendSmsUni
 * parameters:
 *   WINKS_CHN_ID chn: 消息通道。
 *   void *number: 短信接受者、网关
 *   char *dataUni: 短信内容，Unicode编码。
 * return:
 *   成功返回:短信发送句柄
 *   失败返回:-1
 */
int Winks_SendSmsUni(WINKS_CHN_ID chn, char *number, char *dataUni);

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * 用默认的SIM卡发送短信。
 * function: Winks_SendSms
 * parameters:
 *   WINKS_CHN_ID chn: 消息通道。
 *   void *number: 短信接受者、网关
 *   char *data: 短信内容，utf8编码
 * return:
 *   成功返回:短信发送句柄
 *   失败返回:-1
 */
int Winks_SendSms(WINKS_CHN_ID chn, char *number, char *data);

/* 清空短信发送列表。
 * function: Winks_CleanSmsSendingQueue
 * parameters:
 *   空
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_CleanSmsSendingQueue(void);

#ifdef WINKS_DUAL_SIM
/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * function: Winks_SendSmsUni_Ext
 * parameters:
 *   WINKS_CHN_ID chn: 消息通道。
 *   void *number: 短信接受者、网关
 *   char *dataUni: 短信内容，Unicode编码。
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:短信发送句柄
 *   失败返回:-1
 */
int Winks_SendSmsUni_Ext(WINKS_CHN_ID chn, char *number, char *dataUni, int simNo);

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * function: Winks_SendSms_Ext
 * parameters:
 *   WINKS_CHN_ID chn: 消息通道。
 *   void *number: 短信接受者、网关
 *   char *data: 短信内容，utf8编码
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:短信发送句柄
 *   失败返回:-1
 */
int Winks_SendSms_Ext(WINKS_CHN_ID chn, char *number, char *data, int simNo);

/* 设置彩像联网用的Sim卡 */
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData);

/* 恢复手机默认联网用的Sim卡 */
int Winks_api_RestoreNetLinkSim(int SaveData);

/*  Sim卡可用返回1，否则返回0 */
int Winks_isSimEnabled(int sim_card_id);
#endif

/*调用WAP接口*/
int Winks_OpenWapPage( char* pUrl );

/**  取Flash空间，byte **/
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

/*取版本的属性，' '(空格release net)/'T'(test net)/'D'(device net)/'U'(unknow net)/'d'(debug)*/
char* Winks_getReleaseAtt(void);

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

/** 取系统信息 **/
#define WINKS_VERSION_MAX_LEN 20
typedef struct tag_Winks_SysInfo_s
{
    char sv[WINKS_VERSION_MAX_LEN];           /*Software version*/
    char hv[WINKS_VERSION_MAX_LEN];           /*Hardware version*/
    char wv[WINKS_VERSION_MAX_LEN];           /*Winks version*/
}Winks_SysInfo_s;
int Winks_GetSysInfo(Winks_SysInfo_s *sysInfo);

/*用于UI(大黑板)向抽象层注册其消息通道，以获取键盘等消息*/
int Winks_RegisterUiChn(WINKS_CHN_ID chn);
/*用于UI(大红板)向抽象层注册其消息通道，以获取键盘等消息*/
int Winks_RegisterUiCcShowChn(WINKS_CHN_ID chn);

/*标识应用，用户点击菜单进入的应用和电话事件触发的应用*/
 typedef enum tag_Winks_Hwnd_e
{
    WINKS_MINGUI_HWND_USER = 1,
    WINKS_MINGUI_HWND_CALL = 2
}Winks_Hwnd_e;
#define Winks_Hwnd unsigned long 

#define WINKS_HANDLER_WINDOWS_NULL 0x00
int Winks_RegisterHwnd(Winks_Hwnd_e opt, void* hWnd);
Winks_Hwnd Winks_Get_Given_Hwnd(Winks_Hwnd_e opt);
int Winks_GetHwnd(Winks_Hwnd_e opt, void* *hWnd);
int Winks_PostMsgToUiCcShow(unsigned long msg, void *data, unsigned long size);
int Winks_PostMsgToUi(unsigned long msg, void *data, unsigned long size);
/*用于销毁抽象层的主应用*/
int Winks_DestroyApp(Winks_Hwnd_e opt);
/*启动大红板*/
void Winks_LoadWinksCallApp(void);
/*启动大黑板*/
void Winks_LoadWinksMenuApp(void);

#ifdef WINKS_HELP_APP
/*启动系统的帮助信息。*/
int Winks_LoadWinksHelpApp(int index);
#endif



extern int Winks_CreateWindow(int x_Ltop, int y_Ltop, int x_Rbottom,int y_Rbottom,Winks_Hwnd * P_handle);
extern int Winks_DestoryWindow( Winks_Hwnd  window_handle);

/* 从文件路径中获取文件名称
 * function: Winks_GetFileName
 * parameters:
 *   const char* pname 指定文件路径(utf8)。
 * return:
 *   成功返回:const char* 文件名
 *   失败返回:NULL
 */
char* Winks_GetFileName( const char* pname );

/* 从文件路径(或文件名)中获取文件后缀名
 * function: Winks_GetFileNameExt
 * parameters:
 *   const char* pname 指定文件路径(utf8)。
 * return:
 *   成功返回:char* 后缀名
 *   失败返回:NULL
 */
char* Winks_GetFileNameExt( const char* pname );

/*设置控制位*/
int Winks_SetBitFlag(unsigned long bit, unsigned long val, void *src);
/*读取控制位*/
int Winks_GetBitFlag(unsigned long bit, unsigned long *val, void *src);
#define WINKS_OSBF_9 9/* 抽象层位标志9: 用于UI界面控制pen消息的处理。 */
int Winks_SetOsalTestBitFlag(unsigned long bit, unsigned long val);
int Winks_GetOsalTestBitFlag(unsigned long bit, unsigned long *val);
int Winks_SetBackgroundFlag(unsigned long val);
int Winks_GetBackgroundFlag(unsigned long *val);
/*取打印基级*/
int Winks_GetPrintLevel(void);
/*设打印基级*/
int Winks_SetPrintLevel(int level);
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
typedef enum tag_WinksAppState_e
{
    WINKS_APP_STATE_STOP = 0, 
    WINKS_APP_STATE_RUN = 1, 
    WINKS_APP_STATE_SUSPEND = 2
}WinksAppState_e;
/*取底层应用的运行状态*/
int winks_GetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e *focesFlag);
Winks_Hwnd Winks_Get_Given_Hwnd(Winks_Hwnd_e opt);

/*用于获取当前应用的句柄*/

Winks_Hwnd winks_GetCurhWnd(void);

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
    WINKS_FONT_STANDARD = 1,
    WINKS_FONT_BOLDFACED = 2,
    WINKS_FONT_INCLINE = 4,
    WINKS_FONT_UNDERLINE = 8

}WINKS_FONT_FORMAT_E;
/* 字体格式描述枚举 */
#define WK_WG_FONTBLACK         1   //黑体
#define WK_WG_FONTSONG          2   //宋体

#define WK_WG_FONTNOSCROLL    0       //不滚动
#define WK_WG_FONTLEFTSCROLL    1       //从左向右滚动
#define WK_WG_FONTRIGHTSCROLL   2       //从右向左滚动
#define WK_WG_FONTLEFTTRUNC     3       //从左面截断
#define WK_WG_FONTRIGHTTRUNC    4       //从右边截断
#define WK_WG_MULLINEUPRUNC     5       //多行向上滚动



#define WK_WG_GRADIENTCOLORANIMATION   1 //彩色色块在文字上滚动
#define WK_WG_TEXTCOLORSLIDE   2//每个文字每秒显示不同的颜色

#define WK_WG_NOWRAP   0//不折叠
#define WK_WG_WRAP   1//折叠


#define WK_WG_ONLYLINE  0//单行
#define WK_WG_MULLINE   1//多行



#define WK_WG_STRETCH_NONE  0//默认
#define WK_WG_STRETCH_UNIFORM  1//等比拉伸
#define WK_WG_STRETCH_FILL   2//填充
#define WK_WG_STRETCH_UNIFORMFILL  3//等比剪裁


#define WINKS_NEEDSCROLLING_FLAG   1   //显示文本需要滚动


typedef struct Winks_font_feature
{
    WINKS_FONT font_size;
    unsigned long font_format;
    
}Winks_font_feature_s;

typedef unsigned int         winks_color;
#define WINKS_PIXEL_WHITE                 0xffffffff
#define WINKS_PIXEL_TRANSPARENT           0x00000100    // transparent   --0  
#define WINKS_PIXEL_DARKBLUE              0x00008000    // dark blue     --1  
#define WINKS_PIXEL_DARKGREEN             0x00800000    // dark green    --2  
#define WINKS_PIXEL_DARKCYAN              0x00808000     // dark cyan     --3 
#define WINKS_PIXEL_DARKRED               0x80000000    // dark red      --4  
#define WINKS_PIXEL_DARKMAGENTA           0x80008000     // dark magenta  --5 
#define WINKS_PIXEL_DARKYELLOW            0x80800000     // dark yellow   --6 
#define WINKS_PIXEL_DARKGRAY              0x80808000     // dark gray     --7 
#define WINKS_PIXEL_LIGHTGRAY             0xC0C0C000    // light gray    --8  
#define WINKS_PIXEL_BLUE                  0x0000FF00    // blue          --9  
#define WINKS_PIXEL_GREEN                 0x00FF0000    // green         --10 
#define WINKS_PIXEL_CYAN                  0x00FFFF00    // cyan          --11 
#define WINKS_PIXEL_RED                   0xFF000000     // red           --12
#define WINKS_PIXEL_MAGENTA               0xFF00FF00     // magenta       --13
#define WINKS_PIXEL_YELLOW                0xFFFF0000    // yellow        --14 
#define WINKS_PIXEL_LIGHTWHITE            0xFFFFFF00     // light white   --15 
#define WINKS_PIXEL_BLACK                 0x00000000    // black         --16
#define WINKS_HILIGHT_COLOR                 0x50900000    // black         --16
#define WINKS_PIXEL_GRAY                  0xE9E9E900    // gray
                            //#define WINKS_TRANSPARENT_COLOR winks_transparent_color()
                            //#define  winks_color    gal_pixel

                            //#define    WINKS_SET_FONT(WINKS_TYPE font) SelectFont(winks_hdc, logfontgb16);
#define    WINKS_SET_TEXTCOLOR( pixel)       Winks_UI_set_current_text_color(pixel)
#define    WINKS_SET_BKMODE( bkmode) //SetBkMode (winks_hdc, WINKS_BM_TRANSPARENT); 
#define    WINKS_GETFONTHEIGHT  Winks_UI_get_character_height()
#define    WINKS_GETCHARWIDTH(c)  Winks_UI_get_character_width(c)
#define    WINKS_RGB2PIXEl(r,g,b)   GDI_RGB_TO_BUFFER_FORMAT(r, g,b) );//pixel = RGB2Pixel (HDC_SCREEN, r, g, b);

#define  WINKS_BUFFER_ON    { Winks_dbuff_lock(Winks_Get_Given_Hwnd(WINKS_MINGUI_HWND_USER));}
#define  WINKS_BUFFER_OFF   { Winks_dbuff_unlockandblt(Winks_Get_Given_Hwnd(WINKS_MINGUI_HWND_USER));}
#define  WINKS_CC_BUFFER_ON    { Winks_dbuff_lock(Winks_Get_Given_Hwnd(WINKS_MINGUI_HWND_CALL));}
#define  WINKS_CC_BUFFER_OFF   { Winks_dbuff_unlockandblt(Winks_Get_Given_Hwnd(WINKS_MINGUI_HWND_CALL));}
#define WINKS_GRAY_BEGIN(flag,x1,y1, x2, y2)  winks_gary_begin(flag,x1,y1, x2, y2) 

#define WINKS_GRAY_END(flag,x1,y1, x2, y2) winks_gary_end(flag,x1,y1, x2, y2) 

#define WINKS_CLIP_ON(x,y,x1,y1)  Winks_set_clip_on(x,y, x1, y1);
#define WINKS_CLIP_OFF(x,y,x1,y1)     Winks_set_clip_off(x,y,x1,y1);
#define WINKS_SEPARATOR       Winks_get_separator()    //平台的换行符



#define WINKS_ANIM_HANDLE unsigned long 
#define WINKS_ANIM_HANDLE_NULL 0

/* 字符串描述结构 */
typedef struct tag_Winks_Widget_Font_s
{
    int style;    //粗，斜，下划线 wk_osal.h
    int size;     //大，中，小 wk_osal.h
    unsigned short type;     //黑，宋。。。
    unsigned short way;      //滚动，截断，左右(超长时)
    unsigned long color;
}Winks_Widget_Font_s;

/*text_String*/
#define WINKS_MAX_LINE_NUM_TEXTSHOW  6
#define WINKS_UNICODE 1
#define WINKS_GB2312  0
#define WINKS_ENCODE_TYPE WINKS_UNICODE
typedef struct winks_text_String_line
{ 
    int      v_stinglenth;
    char *   p_string;
}winks_text_String_line_s;

#if 1
typedef enum WINKS_SHOW_FLAG_TAG
{
        WINKS_SHOW_LEFT = 1,
        WINKS_SHOW_CENTER,
        WINKS_SHOW_RIGHT
}WINKS_SHOW_FLAG;
#endif
    #define WINKS_SEPARATOR_FLAG 0X00000001
#define WINKS_BLANK_FLAG 0X00000010
typedef struct winks_text_String_rect
{ 
    int      v_Stringline_talNum;
    int      v_FirstlineIndx;
    unsigned long      v_ShowFlag;
    int      line_number_area;
    char * first_inshow_text;
    //winks_text_String_line_s * p_lines;
    //winks_text_String_line_s a_linesData[WINKS_MAX_LINE_NUM_TEXTSHOW];
    
}winks_text_String_rect_s; 
typedef struct winks_text_scrollingString_rect
{ 
    winks_text_String_rect_s  text_scrollingString;
    
    int x, y;
    int width, height;

    long int flags;
    int scroll_size;
    int scroll_gap;
    int scroll_pause;
    //compatable with old ui
    int scroll_counter; 



    winks_color backgroud_color;
    int  bgimage_x;
    int  bgimage_y;
    char * draw_backgroundimage;
    Winks_Widget_Font_s  font_feature;
    


}winks_text_scrollingString_rect_s; 





#define WINKS_UI_TEXT_MENUITEM_SCROLL_TYPE_NONE       0
#define WINKS_UI_TEXT_MENUITEM_SCROLL_TYPE_MARQUEE    1
#define WINKS_UI_TEXT_MENUITEM_SCROLL_TYPE_TWO_DIRECTION 2

/* Type of scrolling to use in text menuitems            */
/* Valid values are one of the above symbols          */
#define WINKS_UI_TEXT_MENUITEM_SCROLL_TYPE   WINKS_UI_TEXT_MENUITEM_SCROLL_TYPE_MARQUEE

/* Time for scrolling text in menu items              */
/* Value must be an integer (milliseconds)               */
/* Typical values: 100 to 500                      */
#define  WINKS_UI_TEXT_MENUITEM_SCROLL_TIME  300
#define WINKS_UI_TEXT_MENUITEM_SCROLL_SIZE   6

/* For Marquee type text scrolling in menu items, this      */
/* specifies the blank gap that should appear at the end */
/* of the string.                                  */
/* Value must be an integer (units are in pixels)        */
/* Typical values: 16 to 64 (This depends on font size)  */
#define WINKS_UI_TEXT_MENUITEM_SCROLL_GAP    (int)(WINKS_SCREEN_WIDTH * 0.2)//(WINKS_GETCHARWIDTH('M') << 1)//32(int)(WINKS_SCREEN_WIDTH_UI * 0.0568)//
#define WINKS_UI_TEXT_FIRST_SCROLL_GAP   10
/* This specifies the delay after which a highlighted item  */
/* will scroll. This is a multiple of                 */
/* UI_TEXT_MENUITEM_SCROLL_TIME                    */
/* Value must be an integer. Typically 1, 2 -- etc.      */
/* Depends on the value of UI_TEXT_MENUITEM_SCROLL_TIME  */
#define WINKS_UI_TEXT_MENUITEM_SCROLL_PAUSE   4
#define WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT  1
#define WINKS_SCROLLING_TEXT_DISPLAY_SCROLL           2
#define WINKS_SCROLLING_TEXT_TWO_DIRECTION_SCROLL 4
#define WINKS_SCROLLING_TEXT_ENABLE_SCROLL    8
#define WINKS_SCROLLING_TEXT_BORDERED_TEXT  16
#define WINKS_SCROLLING_TEXT_SCROLL_TRAIL_TO_HEAD 32

#define WINKS_SCROLLBAR_WIDTH  11
typedef enum  Winks_string_scrollway
{
    
    WINKS_STRINGSCROLL_NO,
        WINKS_STRINGSCROLL_LEFT = WINKS_SCROLLING_TEXT_SCROLL_TRAIL_TO_HEAD,
        WINKS_STRINGSCROLL_RIGHT = WINKS_SCROLLING_TEXT_SCROLL_DIRECTION_RIGHT
}Winks_string_scrollway_e;

typedef struct winks_text_colorlist
{
    unsigned short  tal_colornum;  //集合的颜色数
    unsigned short  curr_index;//最先使用颜色
    winks_color * colorlist;    //颜色列表
} winks_text_colorlist_s;

typedef struct winks_scrolling_text
{
    int x, y;
    int width, height;
    
    winks_color backgroud_color;
    int  bgimage_x;
    int  bgimage_y;
    char * draw_backgroundimage;
    Winks_Widget_Font_s  font_feature;
    char* text;
    short txtshadow_x;
    short txtshadow_y;
    winks_color txtshadow_color;
    unsigned char show_flags;/*WINKS_SHOW_LEFT        1   //居左显示    WINKS_SHOW_CENTER    2   //居中显示    WINKS_SHOW_RIGHT    3   //居右显示*/   
    unsigned char effect_flags;//特效
    unsigned char wrapflags;  //WK_WG_NOWRAP   0//不折叠    WK_WG_WRAP   1//折叠 
    unsigned char lineflags; // WK_WG_ONLYLINE  0//单行 WK_WG_MULLINE   1//多行

    unsigned long backgroudcolorflags; //  0 忽略背景颜色// 1 绘制背景颜色
    winks_text_colorlist_s text_colorlist;
    
    /* 下面可不填写*/
    long int ismul_line_flags;//是否是多行数据
    int text_width, text_height;
    long int flags;
    int scroll_size;
    int scroll_gap;
    int scroll_pause;
    //compatable with old ui
    WINKS_TIMER_ID timer_id;
    struct winks_scrolling_text * p_next;
    

    int scroll_x;
    int scroll_counter;


    
    int      v_Stringline_talNum;
    int      v_FirstlineIndx;

    int      line_number_area;
    char * first_inshow_text;
    int      string_y;
    //int scroll_time;
    //Winks_Hwnd Hwnd;  //

    
} winks_scrolling_text_s;


int winks_show_multiline_scrollingtext(winks_scrolling_text_s * rect_text_scrollingString);
void winks_ui_redraw_scrolling_text_rect(winks_scrolling_text_s *s);
void winks_ui_handle_multiline_scrollingtext(winks_scrolling_text_s *s);
/*字符串类行*/
#define WK_STRING_TYPEASCII      1
#define WK_STRING_TYPEUNICODE    2



/*#include "Gdi_const.h"*/

#if 1//ndef WINKS_USE_PLATFORM_SOURCE_MANAGER
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
#else
#define winks_ui_imgcacheinit() 
#define winks_ui_imgcachedst()
#endif
/* 接入点名称的最小长度 */
#define WINKS_APN_NAME_MIN_LEN 40
/* 网络账号的最小长度 */
#define WINKS_DATA_ACCOUNT_MIN_LEN 40

/** (mtk) 初始化帐号资料
*   NOTE: 因为用户可以手动增删接入点, UI 必须在进入设置界面前调用。
*/
void Winks_InitDataAccount(void (*callback_func)(void));

/* 通过ID 取接入点的名称, unicode 编码 */
int Winks_GetApnName(unsigned long account_id, char *apn_name, unsigned long len);

/* 通过ID 获取账号名称, unicode 编码 */
int Winks_GetDataAccountName(unsigned long account_id, char *data_account, unsigned long len);

/*取背光时间*/
int Winks_GetBgLightTime(unsigned long *val);

/*任意键接听电话*/
int Winks_GetAllKeyAcceptCall(unsigned long *val);

/*取USB状态。1:系统插了USB, 0:系统未插USB.*/
int Winks_GetUsbState(unsigned long *val);

/* 接听电话 */
void Winks_AcceptCall(void);

/* 拒接电话 */
void Winks_RejectCall(void);

/* 挂断电话 */
void Winks_HangupCall(void);

/* 中止呼叫 */
void Winks_AbortCall(void);

/* 获取通话的开始时间和持续时间 */
int Winks_GetCallTime(Winks_DateTime_s *start_time, Winks_DateTime_s *duration);

/*取好友列表的排序规测代码，中文为拼音排序，英文为字母排序*/
int Winks_GetPinyin(char *str, char *pyBuff, unsigned long pyBuffSize);
/* 判断一个unicode字符串中的字符是否全是ascii */
int wk_is_string_ascii(const char *string);

/* 取双字字符串的字符个数，类似strlen. */
int WK_STRLEN(const char *str);

/* 双字字符串拷贝，类似strcpy. */
char *WK_STRCPY(char* dst, const char *src);

/* 双字字符串拷贝，类似strncpy。注意这里的maxLen是双字的个数，不是buffsize. */
char *WK_STRNCPY(char* dst, const char *src, int maxLen);

/* 双字字符串拷贝，类似strncpy，含结束符。注意这里的maxSize是预计拷贝的最大字节数. */
char *WK_STRNCPYE(char* dst, const char *src, int maxSize);

/* 双字字符串比较，类似strcmp. */
long WK_STRCMP(const char* str1, const char *str2);

/* 双字字符串比较，类似strncmp。注意这里的maxLen是双字的个数，不是buffsize. */
long WK_STRNCMP(const char* str1, const char *str2, int maxLen);

/* 双字字符串拼接函数，类似strcat. */
char *WK_STRCAT(char* dst, const char *src);

/* 双字字符串查找，类似strstr
从字符串str1中寻找str2第一次出现的位置（不比较结束符NULL)，如果没找到返回NULL. */
char *WK_STRSTR(const char* str1, const char *str2);

/* 字符编码转换函数 */
int Winks_Ascii2UnicodeString(const char *in_ascii, char *out_unicode, unsigned int size);
int Winks_Unicode2AsciiString(const char *in_unicode, char *out_ascii, unsigned int size);
int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size);
int Winks_Unicode2UtfString(const char *in_unicode, char *out_utf8, unsigned int size);

#ifdef WINKS_DYNAMIC_UPDATE
/*启动安装程序 param,安装程序名称和路径， size 名称的size*/
void winks_StartInstall(const char *param, unsigned long size);
#endif

#ifdef WINKS_ARENA
/*
编码转换函数for Arena:
返回值: 成功 - WINKS_RETURN_SUCCESS, 失败 - WINKS_RETURN_FAILURE.
*/
int Winks_Utf2GbkString(const char *in_utf8, char *out_gbk, unsigned int size);
int Winks_Gbk2UtfString(const char *in_gbk, char *out_utf8, unsigned int size);
int Winks_Unicode2GbkString(const char *in_unicode, char *out_gbk, unsigned int size);
int Winks_Gbk2UnicodeString(const char *in_gbk, char *out_unicode, unsigned int size);

extern HWND Winks_Get_Window_Id(void);
extern SINT32 winks_DestroyEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif /* WINKS_ARENA */

#ifdef WINKS_SUPPORT_MEMORY_CARD
int Winks_IsMemoryCardExist(void);
int Winks_GetMemoryCardDriver(char *drv, unsigned int len);
int Winks_GetMemoryCardSpace(Winks_FlashSpace_MB_s *space);
#endif

#ifndef WINKS_APN_WITH_PROXY_IN_PLATFORM
/*根据ID取代理服务器的IP与prot。*/
int Winks_api_getProxy(void* apnID,char *Proxy, unsigned long proxySize, unsigned short *port);
#endif

/* 该函数强行在系统的接入点列表中设置指定的接入点方式，如果已有则修改，如果没有则新增。
 * function: Winks_SysAddDataAccount
 * parameters:
 *   unsigned long defIndex: 默认接入点序列号。
 *   char *apn: 代理服务器IP
 *   char *proxyIp: 代理服务器IP
 *   unsigned short proxyPort: 代理服务器端口
 * return:
 *   成功返回: 0
 *   失败返回:-1
 */
typedef enum  Winks_line_Dashstyle
{        
    WINKS_DASHSTYLE_NULL,   
    WINKS_LINE_SOLID,/*实线*/   
    WINKS_LINE_DOT,  /*点*/    
    WINKS_LINE_DOTDASH,/*点线*/
    WINKS_LINE_DOUBLELINE/*双线*/
}Winks_line_Dashstyle_e;


int Winks_SysAddDataAccount(unsigned long defIndex, char *apn, char* proxyIp, unsigned short proxyPort);
extern  int  winks_if_anim(char* v_FileStr , int type);
// wk_draw 's assuming
extern void winks_ui_drawPoint(int v_CoorX, int v_CoorY,  unsigned int v_Color);
extern void winks_ui_drawLine(int v_StartX, int v_StartY, int v_EndX,int v_EndY,  unsigned int v_Color);
extern void winks_ui_drawRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color );
extern void winks_ui_fillRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color );
extern void winks_ui_drawDashRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y, unsigned int v_Color ,int line_height,Winks_line_Dashstyle_e line_style);
extern void winks_ui_drawGraphicFromFile(int v_Top, int v_Left,const char *file_name );
extern void winks_ui_drawGraphicFromMem(int x, int y, const void* mem, int size, const char *ext );
extern int winks_ui_drawanimation_by_filenametype( const char *file_name ,int string_type,int x, int y,int view_width, int view_height,unsigned long show_flag,WINKS_ANIM_HANDLE  *P_handle);
extern void winks_ui_textOutlen(int x, int y, const char * text, int len);
extern void winks_get_string_wight_high(char *text,int len,int * wight , int * hight);

int winks_draw_condition(Winks_Hwnd_e opt);
extern int winks_get_anim_display_count(WINKS_ANIM_HANDLE  handle);
extern int winks_set_anim_display_count(int count, WINKS_ANIM_HANDLE  handle);
//extern int winks_image_draw_animation(int x, int y, char* image_name, unsigned long handle,int view_width, int view_height);
extern int winks_image_draw_animation(int x, int y, char* image_name, WINKS_ANIM_HANDLE * p_handle ,int view_width, int view_height,unsigned long show_flag);
extern void winks_image_stop_animation(WINKS_ANIM_HANDLE  handle);
extern void winks_anim_set_last_frame_callback(WINKS_ANIM_HANDLE  handle);
extern int winks_anim_frame_count(char* file_name);
extern void winks_opacity_on(int x,int y,int width, int hight);
extern void winks_opacity_off(int x,int y,int width, int hight);
extern int Winks_set_clip_on(int x,int y,int x1,int y1);
extern int Winks_set_clip_off(int x,int y,int x1,int y1);
extern void winks_clear_screen(void);
extern void winks_clear_time_fillRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y);
extern void winks_clear_screen_fillRect(int LeftTop_x,int LeftTop_y,int RightBottom_x, int RightBottom_y);
extern void winks_draw_background(char * file_name);
extern void winks_draw_background_colorcall(char * file_name);

extern void winks_layer_free(void);
extern int Winks_dbuff_lock(Winks_Hwnd Hwnd);
extern int Winks_dbuff_unlockandblt(Winks_Hwnd Hwnd);
extern int Winks_bitblt(winks_Rect_s* v_freshlcd);

//extern void winks_greyscale_screen_backgruand(void);
extern int winks_gary_begin(unsigned long flag,int x1,int y1,int x2,int y2);
extern int winks_gary_end(unsigned long flag,int x1, int y1,int x2,int y2);

//extern void winks_TurnOn_Backlight();
extern int winks_Multiline_editor( WINKS_CHN_ID chn, char* pbuff, int buflen ,winks_mmi_chset_enum input_chset, int str_id);
extern long Winks_UI_get_character_width( char c );
extern long Winks_UI_get_character_height(void);

extern int Winks_UI_set_current_text_color( unsigned long c );
extern int Winks_UI_StringBordered( int x, int y,unsigned long border_color , const char* text, int lineheight );
extern int winks_ui_drawGraphicFromFile_ucstype_filename (int x, int y, const char *file_name ,int view_width, int view_height);
extern int winks_ui_drawGraphic_uc_file_and_cache (int x, int y, const char *file_name ,int view_width, int view_height,unsigned long show_flag);
void winks_draw_base(void );
void winks_draw_up_base(void );
void winks_draw_opacity_off(void);
void winks_draw_opacity_on(void);
extern void winks_set_font(WINKS_FONT font);
extern int Winks_set_font_feature (Winks_font_feature_s * font_feature );
extern int Winks_set_Widget_Font (Winks_Widget_Font_s * Widget_Font );
//extern int Winks_set_Widget_Font_withSize (Winks_Widget_Font_s * Widget_Font );
extern int Winks_gettext_height_in_area (char* string_text,int string_text_lenth, int width);
extern int Winks_showtext_in_area1 (char* string_text,int string_text_lenth,int x, int y, int width,int height ,int TextAlign );
extern int Winks_showtext_in_area (winks_scrolling_text_s * text);
extern void winks_ui_handle_scrolling_text(winks_scrolling_text_s * s);
extern int creat_text_String_line_type(char* text,int line_width,int* string_lenth);
extern int winks_strlen_ucs2_type(char * text);

extern char * Winks_get_separator( void);
extern int  winks_show_string_in_rect(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, unsigned long TextAlign);
extern int winks_creat_text_string_rect_type(winks_text_String_rect_s * rect_text_String, winks_Rect_s* pRect, WINKS_SHOW_FLAG TextAlign, char * string_text, int string_text_lenth);
/*向UI发送应用挂起回复消息*/
int Winks_AppPauseResumeMsgToUI( Winks_Hwnd_e opt,  WinksAppPauseResume_e flag);


#define     WINKS_IMAGE_TOO_LARGE    3
#define     WINKS_IMAGE_SUCCESS   0




#if 1/**/
/*************************************************
           设置项API接口
**************************************************/
#define WINKS_SERVER_UID_MAX_LEN 64  /*服务器下发的UID的最大长度*/

/* 展示设置 */
#define WINKS_SETVAL_SWITCH_OFF 0  /*设置项开关:关*/
#define WINKS_SETVAL_SWITCH_ON 1   /*设置项开关:开*/
/* 通话设置项*/
#define WINKS_SETVAL_CCSHOW_RING 0x01   /*通话前展示*/
#define WINKS_SETVAL_CCSHOW_TALK 0x02   /*通话中展示*/
#define WINKS_SETVAL_CCSHOW_HANGUP 0x04 /*挂断后展示*/
//typedef enum {
//    E_CALL_CONNECT = 0x01,
//    E_CALL_TALK = 0x02,
//    E_CALL_HANGUP = 0x04
//} Winks_ShowCase_e;
/* 随电名片更新方式选项*/
#define WINKS_SETVAL_PBU_AUTO 0x01      /*自动更新*/
#define WINKS_SETVAL_PBU_OFF 0x2        /*永不更新*/
#define WINKS_SETVAL_PBU_MANUALLY 0x3   /*提示更新*/
/* 联网方式*/
#define WINKS_SETVAL_CW_TIMER 0x01      /*定时更新*/
#define WINKS_SETVAL_CW_PUSH 0x02       /*push更新*/
/* 用户状态*/
#define WINKS_SETVAL_US_NORMAL 0x00     /*用户状态:正常*/
#define WINKS_SETVAL_US_MEETING 0x01    /*用户状态:开会*/
/*定时更新模式*/
#define WINKS_SETVAL_CPMOD_CUSTOM       0x00    /*自定义*/
#define WINKS_SETVAL_CPMOD_USUALLY      0x01    /*数据最新模式*/
#define WINKS_SETVAL_CPMOD_UNUSUALLY    0x02    /*最省资费模式*/

#define WINKS_SMS_GATEWAY_NUM 2         /*短信网关最大个数*/
#define WINKS_SMS_GATEWAY_LEN 16        /*短信网关最大长度*/
#define WINKS_SMS_PREFIX_LEN 16         /*短信前缀最大长度*/

/* 设置选项 */
typedef enum tag_Winks_SettingType_e{
    WINKS_SETTING_SHOW_OTHER_SWITCH=0x00,   /* 展示设置项 ，是否展示对方彩像开关*/
    WINKS_SETTING_ROAM_SWITCH,              /* 漫游设置项 ，漫游时联网开关*/
    WINKS_SETTING_SW_UPDATE_WAY,            /* 软件升级设置项，软件自动升级开关*/
    WINKS_SETTING_AUTO_START_SWITCH,        /* 软件自启动开关，*/
    WINKS_SETTING_AUTO_ACTIVE,              /* 开机自动激活*/
    WINKS_SETTING_SERVICE_STOP_FLAG,        /* 停止服务标志*/
    WINKS_SETTING_FORBIDDEN_FLAG,           /* 用户被禁用标志*/
    WINKS_SETTING_UNSUBSCRIBE_FLAG,         /* 用户退订标志*/
    WINKS_SETTING_HANGUP_UPDATE_SWITCH,     /* 挂断后更新开关*/
    WINKS_SETTING_SHOW_SPLASH_SWITCH,       /* 播放片头开关*/
    WINKS_SETTING_CALL_CONNECT_SWITCH,      /* 来电联网开关*/
    WINKS_SETTING_ONLY_25G_SWITCH,          /* 只支持2.5G开关*/
    WINKS_SETTING_ALREADY_UPLOAD_SETTING,   /* 已经上传设置项开关*/
    WINKS_SETTING_ALREADY_UPLOAD_PB,        /* 已经上传电话本开关*/

/************************************/
/*****  Add switch here above.*******/
    WINKS_SETTING_SWITCH_END=0x40,       
/************************************/
    WINKS_SETTING_SHOW_CASE,                /* 通话设置，通话前、通话中、挂断后 */
    WINKS_SETTING_PBWC_UPDATE_WAY,          /* 随电名片更新方式 */
    WINKS_SETTING_CONNECT_WAY,              /* 联网方式， 定时更新、PUSH更新 */
    WINKS_SETTING_USER_STATE,               /* 用户状态， 正常、会议 */
    WINKS_SETTING_PROXY_PORT,               /* http代理服务器端口 */
    WINKS_SETTING_SW_VERSION,               /* 软件版本号 */
    WINKS_SETTING_APN_ID,                   /* http接入点ID值 */
    WINKS_SETTING_DIY_FILE_MAX_SIZE,        /* DIY允许上传文件的最大SIZE */
    WINKS_SETTING_HUSHOW_TIME,              /* 挂断后彩像播放时间 */
    WINKS_SETTING_CONNECT_PERIOD,           /* 定时联网时间周期 */
    WINKS_SETTING_CONNECT_PERIOD_MOD,       /* 定时更新模式*/
    WINKS_SETTING_CONNECT_PERIOD_USUALLY,   /* 定时更新数据最新模式,时间周期*/
    WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY, /* 定时更新最省资费模式,时间周期*/
    WINKS_SETTING_CONNECT_TIMEOUT,          /* 联网超时时间 */
    WINKS_SETTING_SPLASH_DURATION,          /* 片头播放时间,0表示不播放片头 */
    WINKS_SETTING_PUSH_PORT,                /* PUSH端口 */
    WINKS_SETTING_CC_REPLACE_WA,            /* 彩像淘汰算法的加权值 */
    WINKS_SETTING_HANGUP_UPDATE_DELAY_TIME, /* 挂断后更新的延时时间，为0表示不延时。 */
    WINKS_SETTING_TODAY_BYTES,              /* 今日流量*/
    WINKS_SETTING_TODAY_DATE,               /* 今日日期*/
    WINKS_SETTING_LAST_MONTH_BYTES,         /* 上月流量*/
    WINKS_SETTING_THIS_MONTH_BYTES,         /* 本月流量*/
    WINKS_SETTING_THIS_MONTH_DATE,          /* 本月日期*/
    WINKS_SETTING_TOTAL_BYTES,              /* 统计流量*/
    WINKS_SETTING_TOTAL_DATE,               /* 清零日期*/
    WINKS_SETTING_TOTAL_TIME,               /* 清零时间*/
    
/************************************/
/*****  Add value here above. *******/
    WINKS_SETTING_VALUE_END=0x80,       
/************************************/
    WINKS_SETTING_PROXY_IP,                 /* http代理服务器IP */
    WINKS_SETTING_APN,                      /* http接入点 */
    WINKS_SETTING_SMS_GATEWAY,              /* 服务器下发的短信网关*/
    WINKS_SETTING_SMS_PREFIX,               /* 短信前缀 */
    //WINKS_SETTING_USER_STATE_TEXT,        /* 用户状态文本，*/
    WINKS_SETTING_OPR_SERVER_URL,           /* 数据服务器的URL */
    WINKS_SETTING_MEDIA_SERVER_URL,         /* 媒体服务器的URL */
    WINKS_SETTING_SHOP_SERVER_URL,          /* 彩像商城的URL */
    WINKS_SETTING_UID,                      /* 服务器下发的UID */
    WINKS_SETTING_ACTIVE_IMSI,              /* 最近激活过的IMSI号码 */
    WINKS_SETTING_CONNECT_BACKUP,           /* 备份定时更新联网时间*/

/************************************/
/*****  Add string here above. ******/
    WINKS_SETTING_STRING_END=0xa0,       
/************************************/
    WINKS_SETTING_BLOCK_DATA,               /*测试*/
    WINKS_SETTING_FORCE_RELOAD_DATA,        /* 强制重新加载数据。用户symbian平台(内嵌浏览器需重启EXE才能正常使用)强制重启后的数据恢复 */
    WINKS_SETTING_BLOCK_SERVER_CONFIG,      /*服务器下发的服务配置*/

/************************************/
/*****  Add block here above.  ******/
    WINKS_SETTING_NULL
/************************************/
} Winks_SettingType_e;

/* 置标志,用以统一提交设置值(存文件).置该标志后，调用Winks_SetSettingVal、
 * Winks_SetSettingString函数后不存文件，需自行调用函数Winks_WriteSetting来保存设置项。
 * function: Winks_SetSettingCommitTagether
 * parameters:
 *   
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetSettingCommitTagether(void);
    
/*恢复默认设置。
 * function: Winks_CommitTagether
 * parameters:
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_CommitTagether(void);

/*恢复默认设置。
 * function: Winks_SettingDefault
 * parameters:
 *   
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SettingDefault(void);

/* 设置设置项的值
 * function: Winks_SetSettingValue
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   unsigned long value: 设置项值
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetSettingValue(Winks_SettingType_e type, unsigned long value);

/* 获取设置项的值
 * function: Winks_GetSettingValue
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   unsigned long *value: (out)设置项值
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetSettingValue(Winks_SettingType_e type, unsigned long *value);

/* 存储字符串设置项函数
 * function: Winks_SetSettingString
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   unsigned long *srStr: 设置的字符串
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetSettingString(Winks_SettingType_e type, const char *srStr);

/* 获取字符串设置项函数
 * function: Winks_GetSettingString
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   const char *destStr: (out)获取的字符串
 *   unsigned long destSize: \destStr的buff size
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetSettingString(Winks_SettingType_e type, const char *destStr, unsigned long destSize);

/* 存储数据块设置项函数
 * function: Winks_SetSettingBlock
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   const void *block: 待存储的数据块
 *   unsigned long blockSize: 待存储的数据块的大小
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetSettingBlock(Winks_SettingType_e type, const void *block, unsigned long blockSize);

/* 读取数据块设置项函数
 * function: Winks_GetSettingBlock
 * parameters:
 *   Winks_SettingType_e type: 设置项类别
 *   void *block: (out)获取的数据块的存放位置
 *   unsigned long blockSize: 待读取数据块的大小
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetSettingBlock(Winks_SettingType_e type, void *block, unsigned long blockSize);

/* 设置激活的IMSI号码
 * function: Winks_SetRecentlyActivatedImsi
 * parameters:
 *   const char *imsi:imsi号码
 *   unsigned long len:imsi号码的长度
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetRecentlyActivatedImsi(const char *imsi, unsigned long len);

/* 读取激活的IMSI号码
 * function: Winks_GetRecentlyActivatedImsi
 * parameters:
 *   const char *imsi:(out)imsi号码
 *   unsigned long len:imsi的buff size
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetRecentlyActivatedImsi(char *imsi, unsigned long len);

/* 设置用户是否为沉默用户的状态，用户以IMSI为标识
 * function: Winks_SetImsiPopFlag
 * parameters:
 *   const char *imsi:imsi号码
 *   unsigned char flag:0:不屏蔽; 1:屏蔽.
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetImsiPopFlag(const char *imsi, unsigned char flag);

/* 获取用户是否为沉默用户的状态，用户以IMSI为标识
 * function: Winks_GetImsiPopFlag
 * parameters:
 *   const char *imsi:imsi号码
 *   unsigned char flag:(out)0:不屏蔽; 1:屏蔽.
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetImsiPopFlag(const char *imsi, unsigned char *flag);

/* 设置HTTP代理服务器.
 * function: Winks_SetProxyServer
 * parameters:
 *   const char *proxyIp: 代理服务器IP
 *   unsigned short proxyPort: 代理服务器端口
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetProxyServer(const char *proxyIp, unsigned short proxyPort);

/* 获取HTTP代理服务器.
 * function: Winks_GetProxyServer
 * parameters:
 *   const char *proxyIp: (out)代理服务器IP
 *   unsigned short *proxyPort: (out)代理服务器端口
 *   unsigned long proxyIpSize: \proxyIp的buff size
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetProxyServer(char *proxyIp, unsigned short *proxyPort, unsigned long proxyIpSize);

/*设置彩像淘汰加权值.每个权值小于256.
 * function: Winks_SetCcRwa
 * parameters:
 *   unsigned long rwaX: 新鲜度权值
 *   unsigned long rwaX: 播放次数权值
 *   unsigned long rwaX: 播放时间权值
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetCcRwa(unsigned long rwaX, unsigned long rwaY, unsigned long rwaZ);

/*获取彩像淘汰加权值.每个权值小于256.
 * function: Winks_SetCcRwa
 * parameters:
 *   unsigned long *rwaX: 新鲜度权值
 *   unsigned long *rwaX: 播放次数权值
 *   unsigned long *rwaX: 播放时间权值
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_GetCcRwa(unsigned long *rwaX, unsigned long *rwaY, unsigned long *rwaZ);

/*获取是否展示片头标志。1: 播放, 其它: 不播*/
int Winks_if_show_titleCC(void);    

/*获取是否展示彩像标志. 1:展示,0:不展示*/
int Winks_if_show_cc(void);

/*判断号码是否紧急号码函数。0: 不是, 1: 是。*/
int Winks_if_emergency_number(const char *no);

/*流量统计。byte:传入需要增加的流量.成功返回0，失败返回-1.*/
int Winks_TrafficStatistics(unsigned long byte);

/*流量统计:清零。成功返回0，失败返回-1.*/
int Winks_TrafficClean(void);
#endif

#define WINKS_MSG_BOX_TYPE_INFO 0 /* 弹出对话框类型: 询问方式 */
#define WINKS_MSG_BOX_TYPE_QUERY 1 /* 弹出对话框类型: 信息模式 */
typedef void (*WINKS_CALLBACK_FUNC)(void);

/* 弹出系统对话框函数。要求为非阻塞方式。
 * function: Winks_LoadSysMsgBox
 * parameters:
 *   int msgBoxType: (in)对话框类型。
 *          WINKS_MSG_BOX_TYPE_QUERY 询问对话框(提示用户选择是与否)；
 *          WINKS_MSG_BOX_TYPE_INFO  消息对话框(提示用户确认)
 *   char *msg: (in)提示语内容，Unicode编码。
 *   WINKS_CALLBACK_FUNC lskcbf: (in)左软键回调函数，用户选择是或确认后的回调函数，没有传NULL。
 *   WINKS_CALLBACK_FUNC rskcbf: (in)右软键回调函数，用户选择否后的回调函数，
 *          没有传NULL，如果提示框类型为WINKS_MSG_BOX_TYPE_INFO该参数无意义。
 * return:
 *   void * 返回成功加载对话框的句柄。
 */
void * Winks_LoadSysMsgBox(int msgBoxType, char *msg, WINKS_CALLBACK_FUNC lskcbf, WINKS_CALLBACK_FUNC rskcbf);

/* 销毁已经弹出的对话框
 * function: Winks_UnloadSysMsgBox
 * parameters:
 *   void* WHND: (in)对话框的句柄。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
 int Winks_UnloadSysMsgBox(void* WHND);

#ifdef __cplusplus
}
#endif

#endif
