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
    int     DirHd;          /* �򿪵�Ŀ¼��� */
    void *  pDirEnt;        /* ϵͳ���ص�Ŀ¼���,��FindNext�� */
    char    FileName[128];  /* ���ص��ļ�����Ŀ¼���洢 */
    unsigned long FileSize; /* ���ص��ļ���С */
    unsigned long Attrib;   /* ���ص��ļ����� */
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

/*�����ļ����������뺯��������ֵ: 0�ɹ��� -1ʧ�ܡ�
errorCode Ϊָ���Ĵ����롣*/
int Winks_SetFileSysErrorCode(unsigned long errorCode);

/*ȡ�ļ����������뺯��������ֵ: 0�ɹ��� -1ʧ�ܡ�
*errorCode Ϊ���صĴ����롣*/
int Winks_GetFileSysErrorCode(unsigned long *errorCode);

#if 0
/* unicode �� ascii ת������ */
int Winks_Ascii2UnicodeString(char *pInBuffer, char *pOutBuffer);
int Winks_Ascii2UnicodeStringN(char *pInBuffer, int len, char *pOutBuffer);
int Winks_Unicode2AsciiString(char *pInBuffer, char *pOutBuffer);
int Winks_Unicode2AsciiStringN(char *pInBuffer, int len, char *pOutBuffer);

/* unicode�ַ����������� */
int Winks_UnicodeStrlen(const char *arrOut);
char * Winks_UnicodeStrcpy(char *strDestination, const char *strSource);
char * Winks_UnicodeStrncpy(char *strDestination, const char *strSource, int size);
int Winks_UnicodeStrcmp(const char *string1, const char *string2);
int Winks_UnicodeStrncmp(const char *string1, const char *string2, int size);
char * Winks_UnicodeStrcat(char *strDestination, const char *strSource);
char * Winks_UnicodeStrncat(char *strDestination, const char *strSource, int size);
#endif

/*��ָ�����ļ�*/
FILE* Winks_fopen(const char *filename,const char *type);
/*��ָ�����ļ�(unicode)*/
FILE* Winks_fopenUni(const unsigned short *filename,const char *type);

/*�ر�һ���ļ�*/
int Winks_fclose(FILE *stream);

/*�����Ƿ񵽴�ָ���ļ���ĩβ*/
int Winks_feof(FILE *stream);

/*����ָ���ļ��Ƿ����,������ڷ���0������-1*/
int Winks_fexist(char *filename);

/*�������ļ�stream�ж�ȡ��count���������ݵ�Ԫ��ÿһ�����ݵ�Ԫ�ĳ���Ϊsize�ֽڣ�����data��ָ�Ŀ���*/
size_t Winks_fread(void *data, size_t size, size_t count, FILE *stream);

/*��ָ��data��ʼ��count�����ݵ�Ԫ����ӵ������ļ�stream�У�ÿ�����ݵ�Ԫ�ĳ���Ϊsize���ֽ�*/
size_t Winks_fwrite(const void *data, size_t size, size_t count, FILE *stream);

/*�����������ļ�fp ��ָ��ƫ��λ(offset) ֵ���ɹ�ƫ������ʧ�ܷ���-1��*/
int Winks_ftell(FILE *stream);

/*���ļ�ָ���Ƶ�ָ��λ*/
#define WINKS_SEEK_SET 0 /* Seek from beginning of file.  */
#define WINKS_SEEK_CUR 1 /* Seek from current position.  */
#define WINKS_SEEK_END 2 /* Set file pointer to EOF plus "offset" */
int Winks_fseek(FILE *stream, long offset, int fromwhere);

/*ɾ��ָ�����ļ����ɹ�����0��ʧ�ܷ���-1��*/
int Winks_fdelete(char *file_name);

/*����ָ���ļ����ļ������ɹ�����0��ʧ�ܷ���-1��*/
int Winks_frename(char *old_file_name, char *new_file_name);

/* ��ȡ�ļ���С, ���ظ�ֵ��ʾʧ�� */
int Winks_fgetsize(const char *filename, unsigned int is_ascii, unsigned int *size);

/*ȡ�ļ���С���ɹ�����0*/
int Winks_fGetFileSize(FILE *stream, unsigned long *size);

/*��ȡ��ǰ·�����ɹ�����0��ʧ�ܷ���-1��*/
int Winks_getcwd(char *path_name_ptr, size_t size);

/*���õ�ǰ·�����ɹ�����0��ʧ�ܷ���-1��*/
int Winks_chdir(char *dir_name);

/*����Ŀ¼���ɹ�����0��ʧ�ܷ���-1��*/
int Winks_mkdir(char *dir_name);

/*ɾ��ָ���Ŀ�Ŀ¼���ɹ�����0��ʧ�ܷ���-1��*/
int Winks_rmdir(char *dir_name);

/*�޸�Ŀ¼�����ɹ�����0��ʧ�ܷ���-1��*/
int Winks_rndir(char *old_dir_name, char *new_dir_name);

/* ɾ��һ��Ŀ¼�������ļ�, ���ݹ���Ŀ¼ */
int Winks_XdeleteDir(char *dir_name);

/* ��ȡ�ļ����������ļ���С */
int Winks_GetFolderSize(
    const char * Path,      /* Src path string */
    int          IsAscii,   /* Src path is ascii ? */
    unsigned long * RetSize,  /* return folder size */
    unsigned long CmpSize);  /* CmpSize==0 ��ȡ�����ļ��е��ļ���С������ֻ�ж��Ƿ����CmpSize */

int Winks_FindDirFirst(
    const char * Path,      /* Src path string */
    int          IsAscii,   /* Src path is ascii ? */
    WinksFindAttrib_s * retFsAttrib); /* return attrib */

int Winks_FindDirNext(WinksFindAttrib_s * pFsAttrib);

int Winks_FindDirClose(WinksFindAttrib_s * pFsAttrib);


/*Ŀ¼��ʼ��*/
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


/*ȡϵͳʱ��*/
int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p);

/*����ָ����ʽ��ʱ��ת�����ַ�����
dateTime_pΪ��ʾʱ��Ľṹ��ָ�룬
��dateTime_p==NULLʱȡϵͳʱ�䣬���ָ����ʽ���ء�*/
int Winks_FormatDateTime(Winks_DateTimeFormat_e format, char *buff, unsigned char size, Winks_DateTime_s *dateTime_p);
/*�����1970 ��1 ��1 ��0 ʱ��ָ��ʱ�侭��������.
tm:�����ʱ�䡣���tm=NULL��ȡϵͳʱ��(�൱��time)��
return unsigned long�͵�������0��ʾʧ�ܡ�*/
unsigned long Winks_mktime (Winks_DateTime_s *dtp);
/*ȡʱ����*/
int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result);
/** ȡϵͳ��ʱ�� **/
int Winks_GetSysTimeZone(Winks_TimeZone_e *time_zone);

/**��Ϣ����**/
/**  ����ƽ̨����Ϣ����HTTP���塣**/
//#define WINKS_PMSG_HTTP                         (0xF800)
/** ����ƽ̨����Ϣ���ɳ���㶨�塣**/
//#define WINKS_PMSG_OSAL                         (0xF820)
/** ����ƽ̨����Ϣ����UI���塣**/
//#define WINKS_PMSG_UI                           (0xF840)

/*************************************************
            WINKS�ڲ���Ϣ
**************************************************/
/* WINKS��Ϣ��ʼֵ */
#define WINKS_MSG_BASE                          0xF000
/** ���ڳ�������Ϣ���ɳ���㶨�塣**/
#define WINKS_MSG_OSAL                          (WINKS_MSG_BASE)
#define WINKS_MSG_CHN_INIT                      (WINKS_MSG_OSAL+0x00)
#define WINKS_MSG_TIMER_EXPIRY                  (WINKS_MSG_OSAL+0x01)
#define WINKS_MSG_CALL_INCOMING                 (WINKS_MSG_OSAL+0x02)   /* ����     */
#define WINKS_MSG_CALL_DIAL                     (WINKS_MSG_OSAL+0x03)   /* ����     */
#define WINKS_MSG_CALL_ABORT                    (WINKS_MSG_OSAL+0x04)   /* ����ȡ�� */
#define WINKS_MSG_CALL_REMOTE_ABORT             (WINKS_MSG_OSAL+0x05)   /* �Է�ȡ�� */
#define WINKS_MSG_CALL_ANSWER                   (WINKS_MSG_OSAL+0x06)   /* ���ؽ��� */
#define WINKS_MSG_CALL_REMOTE_ANSWER            (WINKS_MSG_OSAL+0x07)   /* �Է����� */
#define WINKS_MSG_CALL_HANGUP                   (WINKS_MSG_OSAL+0x08)   /* ���عҶ� */
#define WINKS_MSG_CALL_REMOTE_HANGUP            (WINKS_MSG_OSAL+0x09)   /* �Է��Ҷ� */
#define WINKS_MSG_CALL_REJECT                   (WINKS_MSG_OSAL+0x0a)   /* ���ؾܽ� */
#define WINKS_MSG_CALL_REMOTE_REJECT            (WINKS_MSG_OSAL+0x0b)   /* �Է��ܽ� */
#define WINKS_MSG_CALL_IN_WAIT                  (WINKS_MSG_OSAL+0x0c)   /* ����ȴ� */
#define WINKS_MSG_CALL_ENDALL                   (WINKS_MSG_OSAL+0x0d)   /* �Ҷ�ALL  */
#define WINKS_MSG_IMSI_REQ                      (WINKS_MSG_OSAL+0x0e)   /* ȡIMSI�� */
#define WINKS_MSG_CALL_QUERY_EMERGENCY          (WINKS_MSG_OSAL+0x0f)   /* ȡ�������к���*/
#define WINKS_MSG_PHONE_BOOK_MODIFIED           (WINKS_MSG_OSAL+0x10)   /* �绰�������Ϣ*/
#ifdef WINKS_DUAL_SIM
#define WINKS_MSG_IMSI_2_REQ                    (WINKS_MSG_OSAL+0x11)   /* ȡIMSI2�� */
#endif
#define WINKS_MSG_USB_DETECT                    (WINKS_MSG_OSAL+0x12)   /* USB �����Ϣ*/
#define WINKS_MSG_TEST                          (WINKS_MSG_OSAL+0x13)   /* For test. */ 
#define WINKS_MSG_FS_INVALID                    (WINKS_MSG_OSAL+0x14)   /* File system invalid.*/ 
#define WINKS_MSG_IMEI_REQ                      (WINKS_MSG_OSAL+0x15)   /* ȡIMEI�� */

/** ���ڳ�������Ϣ����HPPT���塣**/
#define WINKS_MSG_HTTP                          (WINKS_MSG_BASE+0x400)
/** ���ڳ�������Ϣ�������涨�塣**/
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

/** ���ڳ�������Ϣ����UI���塣**/
#define WINKS_MSG_UI                            (WINKS_MSG_BASE+0x800)
#define WINKS_MSG_UI_KEY_EVENT                  (WINKS_MSG_UI +0x01)
#define WINKS_MSG_UI_DESTROY_APP                (WINKS_MSG_UI +0x02)   /*��Ӧ�ñ�����ʱ����*/
#define WINKS_MSG_UI_CREATE_APP                 (WINKS_MSG_UI +0x03)   /*��Ӧ�ñ�����ʱ���������ܴӲ˵�����������¼�������*/
#define WINKS_MSG_UI_PAINT_EVENT                (WINKS_MSG_UI +0x04)   /*��������ʧЧ����ʱ�������յ���ʱ��UI��Ҫ�ػ�ָ������*/
#define WINKS_MSG_UI_PEN_EVENT                  (WINKS_MSG_UI +0x05)   /*������Ϣ*/
#define WINKS_MSG_UI_SELECT_FILE_RSP            (WINKS_MSG_UI +0x06)   /*ȡϵͳ��ý���ļ��ķ�����Ϣ*/
#define WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP    (WINKS_MSG_UI +0x07)   /*ȡϵͳ�����ķ�����Ϣ*/
#define WINKS_MSG_UI_PAUSE_RESUME_APP           (WINKS_MSG_UI +0x08)   /*Ӧ�ù���ָ���Ϣ*/
#define WINKS_MSG_UI_EDITOR_RSP                 (WINKS_MSG_UI +0x09)   /*�������뷨��ƽ̨�ķ�����Ϣ*/
#define WINKS_MSG_UI_SELECT_PHONENUM_RSP        (WINKS_MSG_UI +0x0a)   /*ȡ��ϵ�˷�����Ϣ*/
#define WINKS_MSG_UI_REG_IMMEDIATELY            (WINKS_MSG_UI +0x0b)   /*����ֱ�Ӽ���������*/




#define WINKS_OSALHANDER_STOP ((char)0)
#define WINKS_OSALHANDER_RUNNING ((char)1)

#define WINKS_MESSAGE_BODY_MAX_SIZE 3000/*2048*/
#define WINKS_IMSI_LEN 15
#define WINKS_IMEI_LEN 32
#define WINKS_OSAL_CHN_MAXNUM    8
#define WINKS_OSAL_MSG_MAXNUM    64
#define WINKS_OSAL_TIMMER_MAXNUM    32
#define WINKS_OSAL_EVENT_WAITING_QUEUE_MAXNUM    WINKS_OSAL_CHN_MAXNUM

/* USB ��⶯�� */
#define WINKS_USB_PLUG_IN 1
#define WINKS_USB_PLUG_OUT 2

typedef unsigned long WINKS_CHN_ID;
typedef int (*WINKS_OSAL_CALLBACK)(unsigned long msg, void *data, unsigned long size);
typedef unsigned long WINKS_TIMER_ID;
#define WINKS_CHN_NULL ((WINKS_CHN_ID)0)
typedef unsigned long WINKS_TICKS;
#define WINKS_TIMER_NULL ((WINKS_TIMER_ID)0)


#define WINKS_TIMER_DEFAULT    0x0000    /*Ĭ������*/
#define WINKS_TIMER_AUTO_START 0x0001    /*��ʱ���������Զ�����*/
#define WINKS_TIMER_AUTO_LOOP  0x0002    /*��ʱ����ʱ����������*/

#ifdef WINKS_MULTITHREADING
/*���̵߳Ļ�������ڶ��߳��ж�����Ϣͨ���Ĵ�������Ϣ�ķ��ͣ��Լ���ʱ���Ĵ�������Ҫ�û����������ơ�*/
/*define mutex_id*/
#define WINKS_MUTEX_CHN 1
#define WINKS_MUTEX_MSG 2
#define WINKS_MUTEX_TIMER 3

/*�������������ڳ�����ʼ����ʱ�򱻵��á�    �ɹ�����0��ʧ�ܷ���-1*/
//int Winks_api_CreateMutex(void);
/*ע�����������ڳ�������ٵ�ʱ�򱻵��á�    �ɹ�����0��ʧ�ܷ���-1*/
//int Winks_api_DestroyMutex(void);
/*�õ���������mutex_id:������ID(��:WINKS_MUTEX_CHN)    �ɹ�����0��ʧ�ܷ���-1*/
int Winks_api_GetMutex(int mutex_id);
/*�ͷŻ�������mutex_id:������ID(��:WINKS_MUTEX_CHN)    �ɹ�����0��ʧ�ܷ���-1*/
int Winks_api_PutMutex(int mutex_id);
#endif


/*����ͨ���ȴ���ϵͳ��Ϣ��flag��ÿһλ��־һ��ϵͳ��Ϣ*/
#define WINKS_CALL_NUMBER_LENGTH 40
#define WINKS_CALL_NAME_LENGTH 24


/*IMSI��IMEI*/
typedef struct tag_winks_MsgWaitingQueue_s
{
WINKS_CHN_ID chn;
unsigned long msg;
struct tag_winks_MsgWaitingQueue_s *next;
}winks_MsgWaitingQueue_s;


/*�ȴ��¼�����*/
#define WINKS_EVENT_NULL                0x00000000
#define WINKS_EVENT_CALL_INCOMING       0x00000001  /* ���� */
#define WINKS_EVENT_CALL_DIAL           0x00000002  /* ���� */
#define WINKS_EVENT_CALL_ABORT          0x00000004  /* ����ȡ�� */
#define WINKS_EVENT_CALL_REMOTE_ABORT   0x00000008  /* �Է�ȡ�� */
#define WINKS_EVENT_CALL_ANSWER         0x00000010  /* ���ؽ��� */
#define WINKS_EVENT_CALL_REMOTE_ANSWER  0x00000020  /* �Է����� */
#define WINKS_EVENT_CALL_HANGUP         0x00000040  /* ���عҶ� */
#define WINKS_EVENT_CALL_REMOTE_HANGUP  0x00000080  /* �Է��Ҷ� */
#define WINKS_EVENT_CALL_REJECT         0x00000100  /* ���ؾܽ� */
#define WINKS_EVENT_CALL_REMOTE_REJECT  0x00000200  /* �Է��ܽ� */
#define WINKS_EVENT_CALL_ENDALL         0x00000400  /* �Ҷ�ALL */
#define WINKS_EVENT_CALL_IN_WAIT        0x00000800  /* ����ȴ� */
#define WINKS_EVENT_OF_ALL_CALL         0x0000FFFF  /*ȫ���绰�¼�*/
#define WINKS_EVENT_PB_MODIFIED         0x00010000  /* �绰����� */
#define WINKS_EVENT_USB_DETECT          0x00020000  /* USB ����¼� */
#define WINKS_EVENT_FS_INVALID          0x00040000  /* File system invalid. here is a event.*/ 
#define WINKS_EVENT_TEST_KET            0x80000000  /* TEST KEY EVENT */

#define WINKS_FSINVALID_CAUSED_USB      ((unsigned long)1)      /* �ɲ���USB���¼����������ļ�ϵͳ����ʹ�á�*/

typedef struct tag_winks_EventWaitQueue_s
{
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
WINKS_CHN_ID chn;
#endif
unsigned long flag;             /*ÿһλ��ʾ�ȴ�һ���¼�*/
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
    unsigned long sequence;         /*��¼����ʱ�ļ�����ʼ��*/
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
//#define WINKS_SENDSMS_QUEUE_NUM     10              /*���Ŷ�����󳤶�*/
/* ���Ŷ��еĽṹ */
typedef struct tag_Winks_SmsQueueItem_s
{   struct tag_Winks_SmsQueueItem_s *next;
    char *number;
    char *sendData;
    int simNo;
}Winks_SmsQueueItem_s;
/*��Ϣ���ͻ����첽�ص���������Ҫ��ƽ̨���Ͷ��ŷ��غ���øú�����
 * function: Winks_SendSmsCallBack
 * parameters:
 *   void *number: ���ŷ��ͽ����0:ʧ�ܣ�1:�ɹ���
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SendSmsCallBack(int result);

/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * ��Ĭ�ϵ�SIM�����Ͷ��š�
 * function: Winks_SendSms
 * parameters:
 *   void *number: ���Ž����ߡ�����
 *   char *dataUni: �������ݣ�Unicode���롣
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SendSmsUni(char *number, char *dataUni);

/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * ��Ĭ�ϵ�SIM�����Ͷ��š�
 * function: Winks_SendSms
 * parameters:
 *   void *number: ���Ž����ߡ�����
 *   char *data: �������ݣ�utf8����
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SendSms(char *number, char *data);

#ifdef WINKS_DUAL_SIM
/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * function: Winks_SendSms
 * parameters:
 *   void *number: ���Ž����ߡ�����
 *   char *dataUni: �������ݣ�Unicode���롣
 *   int simNo: ˫���ֻ���ָ�������ŵ�sim����1--sim��1;2--sim��2
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SendSmsUni_Ext(char *number, char *dataUni, int simNo);

/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * function: Winks_SendSms
 * parameters:
 *   void *number: ���Ž����ߡ�����
 *   char *data: �������ݣ�utf8����
 *   int simNo: ˫���ֻ���ָ�������ŵ�sim����1--sim��1;2--sim��2
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SendSms_Ext(char *number, char *data, int simNo);

/* ���ò��������õ�Sim�� */
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData);
/* �ָ��ֻ�Ĭ�������õ�Sim�� */
int Winks_api_RestoreNetLinkSim(int SaveData);
/*  Sim�����÷���1�����򷵻�0 */
int Winks_isSimEnabled(int sim_card_id);
#endif
#endif


typedef struct tag_winks_OsalContext_s{
    /***************Global Variable*********************/
    /*     �������ģ�������е�ȫ�ֱ������ϵ�һ��      */
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
    void* setting;                              /*ϵͳ������*/
    WINKS_CHN_ID uiChn;                         /*UI channel,��ڰ���Ϣͨ��*/
    WINKS_CHN_ID uiCcShowChn;                   /*UI channel,������Ϣͨ��*/
    WINKS_CHN_ID engChn;                        /*Engine channel.*/
    char curImsi[WINKS_IMSI_LEN+1];             /*IMSI*/
    WINKS_TIMER_ID waitingImsi_T;               /*��ʱ��������IMSI����ȴ���ʱ*/
    winks_MsgWaitingQueue_s *waitingImsi_p;     /*��������ָ�룬����IMSI����ȴ�����*/
    char curImei[WINKS_IMEI_LEN+1];             /*IMEI*/
    WINKS_TIMER_ID waitingImei_T;               /*��ʱ��������IMEI����ȴ���ʱ*/
    winks_MsgWaitingQueue_s *waitingImei_p;     /*��������ָ�룬����IMEI����ȴ�����*/
#ifdef WINKS_DUAL_SIM
    unsigned short ActiveSim;                   /* ���ֽ�Ϊ1��ʾ�ҵ�����ʹ�õ�sim��;���ֽڱ�ʾʹ��curImsi����curImsi_2 */
    WINKS_TIMER_ID waitingImsi_2_T;               /*��ʱ��������IMSI����ȴ���ʱ*/
    char curImsi_2[WINKS_IMSI_LEN+1];             /*IMSI*/
#endif /* WINKS_DUAL_SIM */
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
    winks_EventWaitingQueue_s *eventWaiting_p;  /*ϵͳ�¼��ȴ���������ָ��*/
#else
    winks_EventWaitingQueue_s eventWaitingQueue[WINKS_OSAL_EVENT_WAITING_QUEUE_MAXNUM];  /*ϵͳ�¼��ȴ���������*/
#endif
    WINKS_CHN_ID fileChn;                       /*��Ϣͨ�������ڽ��յ����ļ�������Ӧ�ú�ķ�����Ϣ*/
    /*unsigned long filemsg;*/
    WINKS_CHN_ID DaChn;                         /*��Ϣͨ�������ڽ��յ��ý����Ӧ�ú�ķ�����Ϣ*/
    /*unsigned long Damsg;*/
    WINKS_CHN_ID pnChn;                         /*��Ϣͨ�������ڽ��յ�����ϵ��Ӧ�ú�ķ�����Ϣ*/
    unsigned long fileSysErrorCode;
    void* hWnd1;                                /*APP1���*/
    void* hWnd2;                                /*APP2���*/
    WinksAppState_e hd1GetFocus;                /*  APP1����״̬.  0:Ӧ���˳��� 1:Ӧ�����ã�2:Ӧ�ù���*/
    WinksAppState_e hd2GetFocus;                /*  APP2����״̬.  0:Ӧ���˳��� 1:Ӧ�����ã�2:Ӧ�ù���*/
    unsigned long keyCode;                      /*����down\upƥ���¼*/
    unsigned long penCode;                      /*������down\upƥ���¼*/
#ifdef WINKS_DEBUG
    long fileSysErrorCodeS;                     /*���ڼ�¼�ļ�ϵͳ�����Ĵ������*/
    int msgInQueueCnt;
    int msgOutQueueCnt;
#endif
    unsigned long testBitFlag;                  /*λ��־��
                                                  	d0:Ӧ�ñ�������; 
                                                    d1:�绰������־λ; 
                                                    d2:debug��ӡ����;
                                                    d3:������ӡ��Ϣ����;
                                                    d4:�������URL����;
                                                    d5:������������翪��;
                                                    d6:������ͳһ�ύ�ı�־;
                                                */
    char keyStack[WINKS_TEST_KEY_HISTORY_STACK_LEN+1];/*��ʷkaycode���������룬����"*#100022#???#" */

    WINKS_CHN_ID osalChn;                       /*�������Ϣͨ��*/
#ifdef WINKS_SENDSMS_UPGRADE
    Winks_SmsQueueItem_s * smsQueueTail;        /*���Ͷ��ŵ���Ϣ����-��β*/
    Winks_SmsQueueItem_s * smsQueueHead;        /*���Ͷ��ŵ���Ϣ����-��ͷ*/
#endif

    char version[32];
/*  �����API��ȫ�ֽṹwinks_OsApiContext_s
    ע������ WINKS_OSAL_API_SELF_DATA_LEN>=sizeof(winks_OsApiContext_s)
*/  char osapis[WINKS_OSAL_API_SELF_DATA_LEN];	/*��Ӧ�����API��winks_OsApiContext_s*/

}winks_OsalContext_s;


/*������Ϣͨ��*/
WINKS_CHN_ID Winks_CreateChn(WINKS_OSAL_CALLBACK fCallBack);

/*ע����Ϣͨ��*/
int Winks_DestroyChn(WINKS_CHN_ID chn);

/*����Ϣͨ�������첽��Ϣ*/
int Winks_PostMsg(WINKS_CHN_ID chn, unsigned long msg, void *data, unsigned long size);

/*ע������OSAL Handler*/
int Winks_CreateOsalHandler(void);

/*ע�������OSAL Handler*/
int Winks_DestroyOsalHandler(void);




/*������ʱ��*/
WINKS_TIMER_ID Winks_CreateTimer(WINKS_CHN_ID chn, WINKS_TICKS timeOutPeriod, unsigned long flag);

/*ע����ʱ��*/
int Winks_DestroyTimer(WINKS_TIMER_ID timerId);

/*������ʱ��*/
int Winks_StartTimer(WINKS_TIMER_ID timerId);

/*ֹͣ��ʱ��*/
int Winks_StopTimer(WINKS_TIMER_ID timerId);


/*************************************************
        ������Ϣ�ӿ�(WINKS_MSG_UI_KEY_EVENT)
**************************************************/
/** ������key code**/
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

/** ������Ϣ�Ľṹ�� **/
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

/** ������Ϣ�Ľṹ�� **/
typedef struct tag_Winks_MsgBody_PenEvent_s
{
    int x;
    int y;
    Winks_PenUpDown flag;
}Winks_MsgBody_PenEvent_s;

/*************************************************
        ˢ����Ϣ�ӿ�(WINKS_MSG_UI_PAINT_EVENT)
**************************************************/
typedef struct tag_Winks_Rect_s
{
    int left;
    int top;
    int right;
    int bottom;
}Winks_Rect_s;
/*************************************************
        ���뷨������Ϣ(WINKS_MSG_UI_EDITOR_RSP)
**************************************************/
typedef struct tag_Winks_EditorRsp_s
{
    int result;/*1��ʾ�ύ��0��ʾȡ��*/
    char data[1];/*�ύ�ı༭���ݣ�Unicode���롣result==0ʱ���������塣*/
}Winks_EditorRsp_s;
/*���뷨���غ���
    flag:1ȷ�ϣ� 0ȡ��
    str:���뷨���ص��ַ�����Unicode����
    �ɹ�����0��ʧ�ܷ���-1*/
int Winks_EditorCallback(const char *str, int flag);
/*************************************************
         �������ܽӿ�
**************************************************/

/*�绰�¼���Ϣ�ķ��ؽṹ*/
typedef struct tag_winks_CallMsgInfo_s
{
    char number[WINKS_CALL_NUMBER_LENGTH];
}winks_CallMsgInfo_s;

/*�绰������¼���Ϣ�ķ��ؽṹ*/
#define WINKS_PBEVENT_ADD       1
#define WINKS_PBEVENT_REPLACE   2
#define WINKS_PBEVENT_DELETE    3
typedef struct tag_winks_PbModifiedMsgInfo_s
{
    unsigned char type;
    char number1[WINKS_CALL_NUMBER_LENGTH]; /*�������롢���ǰ�ĺ��롢ɾ������*/
    char number2[WINKS_CALL_NUMBER_LENGTH];/*�����ĺ���*/
}winks_PbModifiedMsgInfo_s;

int Winks_SetWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);


/*����ͨ���ȴ���ϵͳ��Ϣ��flag��ÿһλ��־һ��ϵͳ��Ϣ*/
int Winks_AddWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);



/*ȡIMSI��*/
int Winks_GetImsi(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
#ifdef WINKS_DUAL_SIM
int Winks_GetImsi_2(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
/* ����֪����ʹ�õ������ſ�ʱ���ô˺���
   ���øú���Ϊ��Winks_GetImsiȡ������ƥ���imsi
*/
int Winks_SetActiveSim(int SimNo);
int Winks_GetActiveSim(void);
#endif /* WINKS_DUAL_SIM */


/*ȡIMEI��*/
int Winks_GetImei(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);

/*ȡ���������б�*/
typedef struct tag_Winks_EmergencyNumList_s
{
    unsigned long count;
    char data[1];
}Winks_EmergencyNumList_s;
int Winks_GetEmergencyNumbers(WINKS_CHN_ID chn, unsigned long msg);


#ifndef WINKS_SENDSMS_UPGRADE
#ifndef WINKS_DUAL_SIM
/*���Ͷ���Ϣ*/
int Winks_SendSms(void *number, unsigned long number_size, void * buff, unsigned long buff_size);
#else /* WINKS_DUAL_SIM */
/*
* Winks_SendSms():
* ����number��Ҫ���͵ĺ��룬��ʽΪASCII
* ����buff��Ҫ���͵����ݣ���ʽΪASCII
* ����sim��������Sim�� 1--sim��1;2--sim��2
*/
int Winks_SendSms(void *number, unsigned long number_size, void * buff, unsigned long buff_size, int SimNo);
/* ���ò��������õ�Sim�� */
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData);
/* �ָ��ֻ�Ĭ�������õ�Sim�� */
int Winks_api_RestoreNetLinkSim(int SaveData);
/*  Sim�����÷���1�����򷵻�0 */
int Winks_isSimEnabled(int sim_card_id);
#endif /* WINKS_DUAL_SIM */
#endif
/*����WAP�ӿ�*/
int Winks_OpenWapPage( char* pUrl );

/*ȡFlashʣ��ռ䣬byte*/
typedef struct tag_Winks_FlashSpace_s
{
    unsigned long total;
    unsigned long free;
}Winks_FlashSpace_s;
int Winks_GetFlashSpace(Winks_FlashSpace_s *space);

/* ��ȡ�洢���Ŀռ�, ��λ: MB */
typedef struct tag_Winks_FlashSpace_MB_s
{
	unsigned long total;
	unsigned long free;
}Winks_FlashSpace_MB_s;

/*ȡ�������ε�״̬��־��flag: 0������; 1����*/
int Winks_GetRoamingFlag(unsigned char *flag);


/* ��ȡͨ���Ŀ�ʼʱ��ͳ���ʱ�� */
int Winks_GetCallTime(Winks_DateTime_s *start_time, Winks_DateTime_s *duration);

/** ȡ��ǰ�������� **/
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

/* �ļ�ϵͳ��ȡ */
#define WINKS_FILE_IMAGE    1         /*ƽ̨֧�ֵ�����ͼ���ļ�*/
#define WINKS_FILE_JPG      10        /*jpg�ļ�*/
#define WINKS_FILE_AUDIO    2         /*ƽ̨֧�ֵ����������ļ�*/
#define WINKS_FILE_VIDEO    3         /*ƽ̨֧�ֵ�������Ƶ�ļ�*/
#define WINKS_FILE_ALL      4         /*ƽ̨֧�ֵ������ļ�*/

/*�����ļ�������ģ�飬�첽��Ϣ����ѡ����ļ�(WINKS_MSG_UI_SELECT_FILE_RSP)��*/
int Winks_SelectFile( WINKS_CHN_ID chn, char* defpath, int type );

#define WINKS_DATAACC_CSD   1
#define WINKS_DATAACC_GPRS  2

/** (mtk) ��ʼ���ʺ�����, NOTE: ��Ϊ�û������ֶ���ɾ�����, UI �����ڽ������ý���ǰ���á�*/
void Winks_InitDataAccount(void (*callback_func)(void));

/*���ý����ѡ��ģ�飬�첽��Ϣ����ѡ��Ľ����(WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP)��*/
int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account );

/*������ϵ��ģ�飬�첽��Ϣ����ѡ��ĵ绰����(WINKS_MSG_UI_SELECT_PHONENUM_RSP)��*/
int Winks_SelectPhoneNumber(WINKS_CHN_ID chn);

/** ȡϵͳ��Ϣ **/
#define WINKS_VERSION_MAX_LEN 20
typedef struct tag_Winks_SysInfo_s
{
    char sv[WINKS_VERSION_MAX_LEN];           /*Software version*/
    char hv[WINKS_VERSION_MAX_LEN];           /*Hardware version*/
    char wv[WINKS_VERSION_MAX_LEN];           /*Winks version*/
}Winks_SysInfo_s;
int Winks_GetSysInfo(Winks_SysInfo_s *sysInfo);



/** ȡϵͳ��TICK **/
WINKS_TICKS Winks_GetSysTick(void);

/*����UI������ע������Ϣͨ�����Ի�ȡ���̵���Ϣ*/
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

/*ȡ�绰���ļ�¼����*/
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);
/*ȡָ���绰����¼�е����е绰����*/
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem);

/*ȡUnicode��strlen.*/
int WK_STRLEN(const char *str);
/* �ַ�����ת������ */
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

/*���ÿ���λ*/
int Winks_SetBitFlag(unsigned long bit, unsigned long val, void *src);
/*��ȡ����λ*/
int Winks_GetBitFlag(unsigned long bit, unsigned long *val, void *src);

int Winks_IsOsalEnable(void);
/*************************************************
       Ӧ��
**************************************************/
/*��ʶӦ�ã��û�����˵������Ӧ�ú͵绰�¼�������Ӧ��*/
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

/*WINKS_MSG_UI_PAUSE_RESUME_APP��ϢЯ���ṹ*/
typedef struct tag_WinksPauseResume_s
{
    Winks_Hwnd_e app;              /*��ʶӦ��*/
    WinksAppPauseResume_e flag;    /*����ָ���־*/
}WinksPauseResume_s;

#define WINKS_HANDLER_WINDOWS_NULL 0x00
int Winks_RegisterHwnd(Winks_Hwnd_e opt, void* hWnd);
int Winks_GetHwnd(Winks_Hwnd_e opt, void* *hWnd);

int Winks_PostMsgToUi(unsigned long msg, void *data, unsigned long size);

/*����Ӧ�û�ý����־*/
int winks_SetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e focesFlag);
int winks_GetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e *focesFlag);

int winks_SetUiDestroyAppFlag(Winks_Hwnd_e opt, unsigned char destroyFlag);
int winks_GetUiDestroyAppFlag(Winks_Hwnd_e opt, unsigned char *destroyFlag);
/*���̴���*/
int winks_keyEvent(Winks_Hwnd_e opt, Winks_MsgBody_KeyEvent_s keyEvent);
/*����������*/
int winks_penEvent(Winks_Hwnd_e opt, Winks_MsgBody_PenEvent_s penEvent);
/*��Ļ�ػ洦��*/
int winks_PaintEvent(Winks_Hwnd_e opt, Winks_Rect_s rtc);

/*��UI����Ӧ��������Ϣ*/
int Winks_AppCreateMsgToUI( Winks_Hwnd_e opt );
/*��UI����Ӧ���˳���Ϣ*/
int Winks_AppDestroyMsgToUI( Winks_Hwnd_e opt );
/*��UI����Ӧ�ù���ظ���Ϣ*/
int Winks_AppPauseResumeMsgToUI( Winks_Hwnd_e opt,  WinksAppPauseResume_e flag);
/*����Ӧ�ô���*/
int winks_procLoadApp(Winks_Hwnd_e opt, void* hWnd);
/*��ͣ����������Ӧ�ô���*/
int winks_procResumeApp(Winks_Hwnd_e opt);
/*��ͣӦ�ô���*/
int winks_procPauseApp(Winks_Hwnd_e opt);
/*�˳�Ӧ�ô���*/
int winks_procUnloadApp(Winks_Hwnd_e opt);
/*ȡ�����APIģ���˽��ȫ�ֱ�����ָ��*/
void* Winks_GetOsalApiPtr(void **p);

/*************************************************
        ��Ļ���ܽӿ�
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
/* �ַ��������ṹ */
typedef struct tag_Winks_Widget_Font_s
{
    int style;    //�֣�б���»��� wk_osal.h
    int size;     //���У�С wk_osal.h
    unsigned short type;     //�ڣ��Ρ�����
    unsigned short way;      //�������ضϣ�����(����ʱ)
    unsigned long color;
}Winks_Widget_Font_s;
/*#include "Gdi_const.h"*/
#ifndef WINKS_USE_PLATFORM_SOURCE_MANAGER
/*��Դ����*/
#define WINKS_IMG_TYPE_INVALID 0
#define WINKS_IMG_TYPE_BMP 1
#define WINKS_IMG_TYPE_GIF 2
#define WINKS_IMG_TYPE_JPG 3
#define WINKS_IMG_TYPE_PNG 4
/*��ʼ����Դ�������*/
int winks_ui_imgcacheinit(void);
/*ע����Դ�������*/
int winks_ui_imgcachedst(void);
/*ȡ�ļ���Ӧ������*/
int winks_ui_getimgdata( const char* pname, char** ppdata, int* plen, int* ptype );

/*��ʼ��Ƥ��*/
int winks_ui_initskinimag( void );
#endif
extern void winks_opacity_on(int x,int y,int width, int hight);
extern void winks_opacity_off(int x,int y,int width, int hight);
#define WINKS_OPACITY_ON(x,y,width,hight ) {winks_opacity_on( x,y,width,hight);}
#define WINKS_OPACITY_OFF(x,y,width,hight ) {winks_opacity_off( x,y,width,hight);}

extern int winks_gary_begin(unsigned long flag,int x1,int y1,int x2,int y2);
extern int winks_gary_end(unsigned long flag,int x1, int y1,int x2,int y2);

#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
/*����IDȡ�����������IP��prot��*/
int Winks_api_getProxy(void* apnID,char *Proxy, unsigned long proxySize, unsigned short *port);
#endif

#define WINKS_INVALID_DATA_ACCOUNT_INDEX 0xffffffff
#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
/* �ú���ǿ����ϵͳ�Ľ�����б�������ָ���Ľ���㷽ʽ������������޸ģ����û����������
 * function: Winks_api_SysAddDataAccount
 * parameters:
 *   unsigned long defIndex: Ĭ�Ͻ�������кš�
 *   char *apn: ���������IP
 *   char *proxyIp: ���������IP
 *   unsigned short proxyPort: ����������˿�
 * return:
 *   �ɹ����ؽ��������к�(���)
 *   ʧ�ܷ���:WINKS_INVALID_DATA_ACCOUNT_INDEX
 */
unsigned long Winks_api_SysAddDataAccount(unsigned long defIndex, char *apn, char* proxyIp, unsigned short proxyPort);
#else
unsigned long Winks_api_SysAddDataAccount(unsigned long defIndex,char *apn);
#endif


#ifdef WINKS_DEBUG
/*�����ݿ�д��debug file��ʵ��Winks_printf������ʱ����á�
 * function: Winks_DebugFile
 * parameters:
 *   char *fileName: �ļ���������дdebug��Ϣ��
 *   char* buff: debug���ݿ顣
 *   unsigned long buffSize: debug���ݿ�Ĵ�С��
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_DebugFile(char *fileName, char* buff, unsigned long buffSize);
#endif



extern int Winks_set_font_feature (Winks_font_feature_s * font_feature );


#ifdef __cplusplus
}
#endif

#endif/*_WK_OSFNC_H_*/
