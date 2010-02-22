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

//Socket �����뼰֪ͨ��Ϣ�¼�������ֵ
#define WINKS_SO_CONNECT     0x01    //����
#define WINKS_SO_READ        0x02    //��ȡ
#define WINKS_SO_WRITE       0x04    //д��
#define WINKS_SO_CLOSE       0x08    //�ر�

#define WINKS_SO_GHBFLEN     100        //������ѯ���������鳤��

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
            �ļ�ϵͳ�ӿ�
**************************************************/

#define WINKS_FS_ATTR_IS_RDONLY     0x01    /* ֻ������ */
#define WINKS_FS_ATTR_IS_HIDDEN     0x02    /* �������� */
#define WINKS_FS_ATTR_IS_SYSTEM     0x04    /* ϵͳ���� */
#define WINKS_FS_ATTR_IS_ARCHIVE    0x08    /* �鵵���� */
#define WINKS_FS_ATTR_IS_DIR        0x10    /* Ŀ¼ */
#define WINKS_FS_ATTR_IS_ASCII      0x100   /* ascii���ַ��� */
#define WINKS_FS_ATTR_IS_DOT        0x200   /* ��Ŀ¼'.'���ϼ�Ŀ¼'..' */

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
/*ȡ�ļ����������뺯��������ֵ: 0�ɹ��� -1ʧ�ܡ�
*errorCode Ϊ���صĴ����롣*/
int Winks_GetFileSysErrorCode(unsigned long *errorCode);

/* unicode �� ascii ת������ */
#if 0
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

/*************************************************
           Ŀ¼�����ӿ�
**************************************************/
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

/*ɾ��ָ����Ŀ¼��(ѭ���ݹ�ʵ��) 
 * function: Winks_XDelDir
 * parameters:
 *   char *dir: [in]��ɾ����Ŀ¼��Utf8���롣
 * return:
 *   �ɹ�����:�ɹ�����0.
 *   ʧ�ܷ���:-1
 */
int Winks_XDelDir(char *dir);

/* ��ȡ�ļ����������ļ���С,����ǰ*RetSize������0 */
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


/*************************************************
           ȡϵͳʱ��ӿ�
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

/*ȡϵͳʱ��*/
int Winks_GetSysDateTime(Winks_DateTime_s *dateTime_p);
int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result);

/*����ָ����ʽ��ʱ��ת�����ַ�����
dateTime_pΪ��ʾʱ��Ľṹ��ָ�룬
��dateTime_p==NULLʱȡϵͳʱ�䣬���ָ����ʽ���ء�*/
int Winks_FormatDateTime(Winks_DateTimeFormat_e format, char *buff, unsigned char size, Winks_DateTime_s *dateTime_p);

/*�����1970 ��1 ��1 ��0 ʱ��ָ��ʱ�侭��������.
tm:�����ʱ�䡣���tm=NULL��ȡϵͳʱ��(�൱��time)��
return unsigned long�͵�������0��ʾʧ�ܡ�*/
unsigned long Winks_mktime (Winks_DateTime_s *dtp);

/** ȡϵͳ��ʱ�� **/
int Winks_GetSysTimeZone(Winks_TimeZone_e *time_zone);

/** ȡϵͳ��TICK **/
typedef unsigned long WINKS_TICKS;
WINKS_TICKS Winks_GetSysTick(void);





/*************************************************
            ��Ϣͨ���ӿ�
**************************************************/
/* WINKS��Ϣ��ʼֵ */
#define WINKS_MSG_BASE                           0xF000
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
#define WINKS_MSG_CALL_MULTIPART                (WINKS_MSG_OSAL+0x0e)   /* �෽ͨ��  */
#define WINKS_MSG_IMSI_REQ                      (WINKS_MSG_OSAL+0x0f)   /* ȡIMSI�� */
#define WINKS_MSG_CALL_QUERY_EMERGENCY          (WINKS_MSG_OSAL+0x10)   /* ȡ�������к���*/
#define WINKS_MSG_PHONE_BOOK_MODIFIED           (WINKS_MSG_OSAL+0x11)   /* �绰�������Ϣ*/
#ifdef WINKS_DUAL_SIM
#define WINKS_MSG_IMSI_2_REQ                    (WINKS_MSG_OSAL+0x12)   /* ȡIMSI2�� */
#endif
#define WINKS_MSG_USB_DETECT                    (WINKS_MSG_OSAL+0x13)   /* USB �����Ϣ*/
#define WINKS_MSG_TEST                          (WINKS_MSG_OSAL+0x14)   /* For test. */ 
#define WINKS_MSG_FS_INVALID                    (WINKS_MSG_OSAL+0x15)   /* File system invalid.*/ 
#define WINKS_MSG_IMEI_REQ                      (WINKS_MSG_OSAL+0x16)   /* ȡIMEI�� */
#define WINKS_MSG_SEND_SMS_CFM                  (WINKS_MSG_OSAL+0x17)   /* ���ŷ��ͽ�����档�Ͱ�λΪ���;�߰�λΪ�������,0Ϊʧ��,1Ϊ�ɹ�.*/

/** ���ڳ�������Ϣ����HPPT���塣**/
#define WINKS_MSG_HTTP                          (WINKS_MSG_BASE+0x400)
/** ���ڳ�������Ϣ�������涨�塣**/
#define WINKS_MSG_ENGINE                        (WINKS_MSG_BASE+0x500)
#define WINKS_MSG_ENGINE_HTTP                   (WINKS_MSG_ENGINE+0x01)
#define WINKS_MSG_ENGINE_INIT_END               (WINKS_MSG_ENGINE+0x02) /*�����ʼ�����*/
#define WINKS_MSG_ENGINE_PUSH                   (WINKS_MSG_ENGINE+0x03)
#define WINKS_MSG_ENGINE_RETMSG                 (WINKS_MSG_ENGINE+0x04)
#define WINKS_MSG_ENGINE_UIRETMSG               (WINKS_MSG_ENGINE+0x05) /*�������������첽��Ϣ*/
#define WINKS_MSG_ENGINE_UPDATE_FL              (WINKS_MSG_ENGINE+0x06)
#define WINKS_MSG_ENGINE_DIYSET                 (WINKS_MSG_ENGINE+0x07)
#define WINKS_MSG_ENGINE_SETTINGSET             (WINKS_MSG_ENGINE+0x08)
#define WINKS_MSG_ENGINE_SNED_REG_SMS           (WINKS_MSG_ENGINE+0x09)
#define WINKS_MSG_ENGINE_SELECT_FRIEND_LIST     (WINKS_MSG_ENGINE+0x0a)
#define WINKS_MSG_ENGINE_REG_NOTIFY             (WINKS_MSG_ENGINE+0x0b)
#define WINKS_MSG_ENGINE_INIT_CACHE             (WINKS_MSG_ENGINE+0x0c)
#define WINKS_MSG_ENGINE_REBUILD_CACHE          (WINKS_MSG_ENGINE+0x0d)
#define WINKS_MSG_ENGINE_RECRUIT_DOWNLOAD       (WINKS_MSG_ENGINE+0x0e)
#define WINKS_MSG_ENGINE_APP_START              (WINKS_MSG_ENGINE+0x10) /*����Ӧ�û��������¼�*/
#define WINKS_MSG_ENGINE_APP_STOP               (WINKS_MSG_ENGINE+0x11) /*����Ӧ�û���ֹͣ�¼�*/
#define WINKS_MSG_ENGINE_APP_PAUSE              (WINKS_MSG_ENGINE+0x12) /*����Ӧ�û�����ͣ�¼�*/
#define WINKS_MSG_ENGINE_APP_RESUME             (WINKS_MSG_ENGINE+0x13) /*����Ӧ�û�����ͣ�ָ��¼�*/
#define WINKS_MSG_ENGINE_APP_RETMSG             (WINKS_MSG_ENGINE+0x14) /*����Ӧ�û������������¼�*/
#define WINKS_MSG_ENGINE_APP_DATAOUT            (WINKS_MSG_ENGINE+0x15) /*����Ӧ�û����������ݷ����¼�*/
#define WINKS_MSG_ENGINE_APP_IDLE               (WINKS_MSG_ENGINE+0x16) /*����Ӧ�û������һ��Ӧ���˳��¼�*/
#define WINKS_MSG_ENGINE_CC_DOWNLOAD_OK         (WINKS_MSG_ENGINE+0x17) /*����ʵʱ���ز��������Ϣ��*/

/** ���ڳ�������Ϣ����UI���塣**/
#define WINKS_MSG_UI                            (WINKS_MSG_BASE+0x800)
#define WINKS_MSG_UI_KEY_EVENT                  (WINKS_MSG_UI+0x01)
#define WINKS_MSG_UI_DESTROY_APP                (WINKS_MSG_UI+0x02)     /*��Ӧ�ñ�����ʱ����*/
#define WINKS_MSG_UI_CREATE_APP                 (WINKS_MSG_UI+0x03)     /*��Ӧ�ñ�����ʱ���������ܴӲ˵�����������¼�������*/
#define WINKS_MSG_UI_PAINT_EVENT                (WINKS_MSG_UI+0x04)     /*��������ʧЧ����ʱ�������յ���ʱ��UI��Ҫ�ػ�ָ������*/
#define WINKS_MSG_UI_PEN_EVENT                  (WINKS_MSG_UI+0x05)     /*������Ϣ*/
#define WINKS_MSG_UI_SELECT_FILE_RSP            (WINKS_MSG_UI+0x06)     /*ȡϵͳ��ý���ļ��ķ�����Ϣ*/
#define WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP    (WINKS_MSG_UI+0x07)     /*ȡϵͳ�����ķ�����Ϣ*/
#define WINKS_MSG_UI_PAUSE_RESUME_APP           (WINKS_MSG_UI+0x08)     /*Ӧ�ù���ָ���Ϣ*/
#define WINKS_MSG_UI_EDITOR_RSP                 (WINKS_MSG_UI+0x09)     /*�������뷨��ƽ̨�ķ�����Ϣ*/
#define WINKS_MSG_UI_SELECT_PHONENUM_RSP        (WINKS_MSG_UI+0x0a)     /*ȡ��ϵ�˷�����Ϣ*/
#define WINKS_MSG_UI_REG_IMMEDIATELY            (WINKS_MSG_UI+0x0b)     /*����ֱ�Ӽ���������*/
#define WINKS_MSG_UI_COL_IMMEDIATELY            (WINKS_MSG_UI+0x0c)     /*һ���ղ���Ϣ, Я�������ַ���wid*/
#define WINKS_MSG_UI_EDITOR_STATE_RSP           (WINKS_MSG_UI+0x0d)     /*״̬�����뷨����*/
#define WINKS_MSG_UI_EMBEDDED_BROWSER_RSP       (WINKS_MSG_UI+0x0e)     /*��Ƕ�������Ϣ*/
#define WINKS_MSG_UI_MODIFY_SIZE_RSP            (WINKS_MSG_UI+0x0f)     /*��Ļ�޸�(ת�����С�ı�)*/
#define WINKS_MSG_UI_PEERNUM_NAME               (WINKS_MSG_UI+0x10)     /* �����ϵ������ */
#define WINKS_MSG_UI_CREATE_WINDOW               (WINKS_MSG_UI+0x11)     /* �����ڱ�����ʱ���� */





#ifdef  WINKS_EMBEDDED_BROWSER
typedef struct tag_Winks_Editor_State_s
{
    unsigned char  open_state;      /* ����״̬(�� Winks_Input_Method_e)*/
    
    unsigned char  softkeyboard_state;       /* �����״̬������״̬��(�� Winks_Input_Method_e)*/
    unsigned short  input_type[128];     /*���뷨���� */
}Winks_Editor_State_s;
typedef struct tag_Winks_Embedded_Browser_State_s
{
    unsigned char  Progress_state;      /* ����*/

}Winks_Embedded_Browser_State_s;

/*  �����*/
typedef enum tag_Winks_Embedded_Browser_State_e{
    WINKS_Embedded_Browser_Start,/*��ʼ��*/
    WINKS_Embedded_Browser_Finsh,          /* ��������*/
    WINKS_Embedded_Browser_Fail     /* ����ʧ�ܣ�*/

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




/* USB ��⶯�� */
#define WINKS_USB_PLUG_IN 1
#define WINKS_USB_PLUG_OUT 2

typedef unsigned long WINKS_CHN_ID;
#define WINKS_CHN_NULL ((WINKS_CHN_ID)0)
typedef int (*WINKS_OSAL_CALLBACK)(unsigned long msg, void *data, unsigned long size, WINKS_CHN_ID chnId, void*chnData);

/** ������Ϣͨ�� **/
WINKS_CHN_ID Winks_CreateChn(WINKS_OSAL_CALLBACK fCallBack);

/** ע����Ϣͨ�� **/
int Winks_DestroyChn(WINKS_CHN_ID chn);

/*ȡ��Ϣͨ��������.������Ϣͨ�����������������ָ�롣*/
void *Winks_GetChnContext(WINKS_CHN_ID chn);

/*����Ϣͨ��������.������Ϣͨ���������������ָ�룬�ɹ�����0��ʧ�ܷ���-1.*/
int Winks_SetChnContext(WINKS_CHN_ID chn, void* chnData);

/** ����Ϣͨ�������첽��Ϣ **/
int Winks_PostMsg(WINKS_CHN_ID chn, unsigned long msg, void *data, unsigned long size);

/** ע������OSAL Handler **/
int Winks_CreateOsalHandler(void);

/** ע�������OSAL Handler **/
int Winks_DestroyOsalHandler(void);



/*************************************************
            ��ʱ���ӿ�
**************************************************/
typedef unsigned long WINKS_TIMER_ID;
#define WINKS_TIMER_NULL ((WINKS_TIMER_ID)0)

#define WINKS_MILLISECONDS2TICKS(S)  ((WINKS_TICKS)(S))
#define WINKS_SECONDS2TICKS(S)  ((WINKS_TICKS)((S)*1000))
#define WINKS_TICKS2MILLISECONDS(S)  ((S))
#define WINKS_TICKS2SECONDS(S)  ((S) / 1000)

#define WINKS_TIMER_DEFAULT    0x0000    /*Ĭ������*/
#define WINKS_TIMER_AUTO_START 0x0001    /*��ʱ���������Զ�����*/
#define WINKS_TIMER_AUTO_LOOP  0x0002    /*��ʱ����ʱ����������*/

typedef struct tag_winks_TimerBody_s{
    WINKS_CHN_ID chn;
    WINKS_TICKS timeOutPeriod;
    WINKS_TICKS timeOut;
    unsigned long flag;
    unsigned char state;            /*1:running , 0:stopped*/
}winks_TimerBody_s;

/** ������ʱ�� **/
WINKS_TIMER_ID Winks_CreateTimer(WINKS_CHN_ID chn, WINKS_TICKS timeOutPeriod, unsigned long flag);

/** ע����ʱ�� **/
int Winks_DestroyTimer(WINKS_TIMER_ID timerId);

/** ������ʱ�� **/
int Winks_StartTimer(WINKS_TIMER_ID timerId);

/** ֹͣ��ʱ�� **/
int Winks_StopTimer(WINKS_TIMER_ID timerId);

/** ȡ��ʱ����Ϣ **/
int Winks_GetTimerInfo(WINKS_TIMER_ID timerId, winks_TimerBody_s *timerInfo);





/*************************************************
            �ȴ��¼�����
**************************************************/
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
#define WINKS_EVENT_CALL_MULTIPART      0x00001000  /* �෽ͨ���¼� */
#define WINKS_EVENT_OF_ALL_CALL         0x0000FFFF  /*ȫ���绰�¼�*/
#define WINKS_EVENT_PB_MODIFIED         0x00010000  /* �绰����� */
#define WINKS_EVENT_USB_DETECT          0x00020000  /* USB ����¼� */
#define WINKS_EVENT_FS_INVALID          0x00040000  /* File system invalid. here is a event.*/ 
#define WINKS_EVENT_ENGINE_INIT_END     0x00080000  /* �����ʼ��������Ϣ�� */
#define WINKS_EVENT_CC_DOWNLOAD_OK      0x00100000  /* ����ʵʱ���ز�������¼���*/
#define WINKS_EVENT_TEST_KET            0x80000000  /* TEST KEY EVENT */

#define WINKS_FSINVALID_CAUSED_USB      ((unsigned long)1)      /* �ɲ���USB���¼����������ļ�ϵͳ����ʹ�á�*/

#define WINKS_CALL_NUMBER_LENGTH    40  /* ref "WINKS_FRIEND_NO_LEN" */
#define WINKS_CALL_NAME_LENGTH      24   /* ref "WINKS_FRIEND_NAME_LEN" */
typedef struct tag_winks_CallMsgInfo_s{
    char number[WINKS_CALL_NUMBER_LENGTH];
    /*char name[WINKS_CALL_NAME_LENGTH];*/
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

/*��Ϣԭ���������øú�����ʵ��ע����Ϣ��ת����*/
int Winks_ProEventWaitingQueue(unsigned long event, unsigned long msg, void *data, unsigned long size);

/** ����ͨ���ȴ���ϵͳ��Ϣ��flag��ÿһλ��־һ��ϵͳ��Ϣ **/
int Winks_SetWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);

/** ����ͨ���ȴ���ϵͳ��Ϣ��flag��ÿһλ��־һ��ϵͳ��Ϣ **/
int Winks_AddWaitingEvent(WINKS_CHN_ID chn, unsigned long flag);

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

/*���ý����ѡ��ģ�飬�첽��Ϣ����ѡ��Ľ����(WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP)��*/
int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account );

/*������ϵ��ģ�飬�첽��Ϣ����ѡ��ĵ绰����(WINKS_MSG_UI_SELECT_PHONENUM_RSP)��*/
int Winks_SelectPhoneNumber(WINKS_CHN_ID chn);
/*************************************************
          Sochet�ӿ�
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
        ������Ϣ�ӿ�
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
#ifdef  WINKS_DUAL_SIM
#define WINKS_KEY_SEND2       31    /* mtkƽ̨��WINKS_KEY_END���þ��� */
#endif
#define WINKS_KEY_MAX         32

typedef enum tag_Winks_KeyUpDown
{
    WINKS_KEY_FLAG_KEYDOWN = 0,          /*key down , lid open or slip open*/
    WINKS_KEY_FLAG_KEYUP = 1,          /*key up , lid close or slip close*/
    WINKS_KEY_FLAG_KEY_LONG_PRESS = 2,
    WINKS_KEY_FLAG_KEY_ALWAYS_PRESS = 3
}Winks_KeyUpDown;

/** ������Ϣ�Ľṹ��(WINKS_MSG_UI_KEY_EVENT) **/
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

/** ������������Ϣ�Ľṹ��(WINKS_MSG_UI_PEN_EVENT) **/
typedef struct tag_Winks_MsgBody_PenEvent_s
{
    int x;
    int y;
    Winks_PenUpDown flag;
}Winks_MsgBody_PenEvent_s;

/** ���뷨������Ϣ�Ľṹ��(WINKS_MSG_UI_EDITOR_RSP) **/
typedef struct tag_Winks_EditorRsp_s
{
    int result;/*1��ʾ�ύ��0��ʾȡ��*/
    char data[2];/*�ύ�ı༭���ݣ�Unicode���롣result==0ʱ���������塣*/
}Winks_EditorRsp_s;
/*************************************************
        ˢ����Ϣ�ӿ�(WINKS_MSG_UI_PAINT_EVENT)
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
         �������ܽӿ�
**************************************************/
/** ȡIMSI�Žӿ� **/
#define WINKS_IMSI_LEN 15
int Winks_GetImsi(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
#ifdef WINKS_DUAL_SIM
int Winks_GetImsi_2(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);
/* ����֪����ʹ�õ������ſ�ʱ���ô˺���
   ���øú���Ϊ��Winks_GetImsiȡ������ƥ���imsi
*/
int Winks_SetActiveSim(int SimNo);
int Winks_GetActiveSim(void);
#endif /* WINKS_DUAL_SIM */

/** ȡIMSI�Žӿ� **/
#define WINKS_IMEI_LEN 32
int Winks_GetImei(WINKS_CHN_ID chn, char *buff, unsigned long buff_size);

/** ȡ���������б� **/
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

/*ȡ�绰���ļ�¼����*/
//phone_cnt�����ر����绰������
//sim_cnt,����sim���绰������
//����ֵ��0��ʾ�ɹ���-1��ʾʧ��
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);
/*ȡָ���绰����¼�е����е绰����*/
//type��1��ʾ�绰������2��ʾsim��
//index����ʾ�ڼ�������0��ʼ
//numberItme�����صĵ绰������ڴ˲�����
//����ֵ��0��ʾ�ɹ���-1��ʾʧ��
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem);
/* �ӵ绰����ȡ�����Ӧ������(UTF-8 ����) */
void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len);

/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * ��Ĭ�ϵ�SIM�����Ͷ��š�
 * function: Winks_SendSmsUni
 * parameters:
 *   WINKS_CHN_ID chn: ��Ϣͨ����
 *   void *number: ���Ž����ߡ�����
 *   char *dataUni: �������ݣ�Unicode���롣
 * return:
 *   �ɹ�����:���ŷ��;��
 *   ʧ�ܷ���:-1
 */
int Winks_SendSmsUni(WINKS_CHN_ID chn, char *number, char *dataUni);

/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * ��Ĭ�ϵ�SIM�����Ͷ��š�
 * function: Winks_SendSms
 * parameters:
 *   WINKS_CHN_ID chn: ��Ϣͨ����
 *   void *number: ���Ž����ߡ�����
 *   char *data: �������ݣ�utf8����
 * return:
 *   �ɹ�����:���ŷ��;��
 *   ʧ�ܷ���:-1
 */
int Winks_SendSms(WINKS_CHN_ID chn, char *number, char *data);

/* ��ն��ŷ����б�
 * function: Winks_CleanSmsSendingQueue
 * parameters:
 *   ��
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_CleanSmsSendingQueue(void);

#ifdef WINKS_DUAL_SIM
/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * function: Winks_SendSmsUni_Ext
 * parameters:
 *   WINKS_CHN_ID chn: ��Ϣͨ����
 *   void *number: ���Ž����ߡ�����
 *   char *dataUni: �������ݣ�Unicode���롣
 *   int simNo: ˫���ֻ���ָ�������ŵ�sim����1--sim��1;2--sim��2
 * return:
 *   �ɹ�����:���ŷ��;��
 *   ʧ�ܷ���:-1
 */
int Winks_SendSmsUni_Ext(WINKS_CHN_ID chn, char *number, char *dataUni, int simNo);

/*��Ϣ���ͻ����ڲ�ά��һ���ŶӶ��У�Ȼ��˳�����ƽ̨�Ķ��ŷ��ͽӿ�(����һ��ͬ��ת�첽�Ĺ���)��
 * function: Winks_SendSms_Ext
 * parameters:
 *   WINKS_CHN_ID chn: ��Ϣͨ����
 *   void *number: ���Ž����ߡ�����
 *   char *data: �������ݣ�utf8����
 *   int simNo: ˫���ֻ���ָ�������ŵ�sim����1--sim��1;2--sim��2
 * return:
 *   �ɹ�����:���ŷ��;��
 *   ʧ�ܷ���:-1
 */
int Winks_SendSms_Ext(WINKS_CHN_ID chn, char *number, char *data, int simNo);

/* ���ò��������õ�Sim�� */
int Winks_api_SetNetLinkSim(int SimNo, int *SaveData);

/* �ָ��ֻ�Ĭ�������õ�Sim�� */
int Winks_api_RestoreNetLinkSim(int SaveData);

/*  Sim�����÷���1�����򷵻�0 */
int Winks_isSimEnabled(int sim_card_id);
#endif

/*����WAP�ӿ�*/
int Winks_OpenWapPage( char* pUrl );

/**  ȡFlash�ռ䣬byte **/
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

/*ȡ�汾�����ԣ�' '(�ո�release net)/'T'(test net)/'D'(device net)/'U'(unknow net)/'d'(debug)*/
char* Winks_getReleaseAtt(void);

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

/** ȡϵͳ��Ϣ **/
#define WINKS_VERSION_MAX_LEN 20
typedef struct tag_Winks_SysInfo_s
{
    char sv[WINKS_VERSION_MAX_LEN];           /*Software version*/
    char hv[WINKS_VERSION_MAX_LEN];           /*Hardware version*/
    char wv[WINKS_VERSION_MAX_LEN];           /*Winks version*/
}Winks_SysInfo_s;
int Winks_GetSysInfo(Winks_SysInfo_s *sysInfo);

/*����UI(��ڰ�)������ע������Ϣͨ�����Ի�ȡ���̵���Ϣ*/
int Winks_RegisterUiChn(WINKS_CHN_ID chn);
/*����UI(����)������ע������Ϣͨ�����Ի�ȡ���̵���Ϣ*/
int Winks_RegisterUiCcShowChn(WINKS_CHN_ID chn);

/*��ʶӦ�ã��û�����˵������Ӧ�ú͵绰�¼�������Ӧ��*/
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
/*�������ٳ�������Ӧ��*/
int Winks_DestroyApp(Winks_Hwnd_e opt);
/*��������*/
void Winks_LoadWinksCallApp(void);
/*������ڰ�*/
void Winks_LoadWinksMenuApp(void);

#ifdef WINKS_HELP_APP
/*����ϵͳ�İ�����Ϣ��*/
int Winks_LoadWinksHelpApp(int index);
#endif



extern int Winks_CreateWindow(int x_Ltop, int y_Ltop, int x_Rbottom,int y_Rbottom,Winks_Hwnd * P_handle);
extern int Winks_DestoryWindow( Winks_Hwnd  window_handle);

/* ���ļ�·���л�ȡ�ļ�����
 * function: Winks_GetFileName
 * parameters:
 *   const char* pname ָ���ļ�·��(utf8)��
 * return:
 *   �ɹ�����:const char* �ļ���
 *   ʧ�ܷ���:NULL
 */
char* Winks_GetFileName( const char* pname );

/* ���ļ�·��(���ļ���)�л�ȡ�ļ���׺��
 * function: Winks_GetFileNameExt
 * parameters:
 *   const char* pname ָ���ļ�·��(utf8)��
 * return:
 *   �ɹ�����:char* ��׺��
 *   ʧ�ܷ���:NULL
 */
char* Winks_GetFileNameExt( const char* pname );

/*���ÿ���λ*/
int Winks_SetBitFlag(unsigned long bit, unsigned long val, void *src);
/*��ȡ����λ*/
int Winks_GetBitFlag(unsigned long bit, unsigned long *val, void *src);
#define WINKS_OSBF_9 9/* �����λ��־9: ����UI�������pen��Ϣ�Ĵ��� */
int Winks_SetOsalTestBitFlag(unsigned long bit, unsigned long val);
int Winks_GetOsalTestBitFlag(unsigned long bit, unsigned long *val);
int Winks_SetBackgroundFlag(unsigned long val);
int Winks_GetBackgroundFlag(unsigned long *val);
/*ȡ��ӡ����*/
int Winks_GetPrintLevel(void);
/*���ӡ����*/
int Winks_SetPrintLevel(int level);
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
typedef enum tag_WinksAppState_e
{
    WINKS_APP_STATE_STOP = 0, 
    WINKS_APP_STATE_RUN = 1, 
    WINKS_APP_STATE_SUSPEND = 2
}WinksAppState_e;
/*ȡ�ײ�Ӧ�õ�����״̬*/
int winks_GetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e *focesFlag);
Winks_Hwnd Winks_Get_Given_Hwnd(Winks_Hwnd_e opt);

/*���ڻ�ȡ��ǰӦ�õľ��*/

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
/* �����ʽ����ö�� */
#define WK_WG_FONTBLACK         1   //����
#define WK_WG_FONTSONG          2   //����

#define WK_WG_FONTNOSCROLL    0       //������
#define WK_WG_FONTLEFTSCROLL    1       //�������ҹ���
#define WK_WG_FONTRIGHTSCROLL   2       //�����������
#define WK_WG_FONTLEFTTRUNC     3       //������ض�
#define WK_WG_FONTRIGHTTRUNC    4       //���ұ߽ض�
#define WK_WG_MULLINEUPRUNC     5       //�������Ϲ���



#define WK_WG_GRADIENTCOLORANIMATION   1 //��ɫɫ���������Ϲ���
#define WK_WG_TEXTCOLORSLIDE   2//ÿ������ÿ����ʾ��ͬ����ɫ

#define WK_WG_NOWRAP   0//���۵�
#define WK_WG_WRAP   1//�۵�


#define WK_WG_ONLYLINE  0//����
#define WK_WG_MULLINE   1//����



#define WK_WG_STRETCH_NONE  0//Ĭ��
#define WK_WG_STRETCH_UNIFORM  1//�ȱ�����
#define WK_WG_STRETCH_FILL   2//���
#define WK_WG_STRETCH_UNIFORMFILL  3//�ȱȼ���


#define WINKS_NEEDSCROLLING_FLAG   1   //��ʾ�ı���Ҫ����


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
#define WINKS_SEPARATOR       Winks_get_separator()    //ƽ̨�Ļ��з�



#define WINKS_ANIM_HANDLE unsigned long 
#define WINKS_ANIM_HANDLE_NULL 0

/* �ַ��������ṹ */
typedef struct tag_Winks_Widget_Font_s
{
    int style;    //�֣�б���»��� wk_osal.h
    int size;     //���У�С wk_osal.h
    unsigned short type;     //�ڣ��Ρ�����
    unsigned short way;      //�������ضϣ�����(����ʱ)
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
    unsigned short  tal_colornum;  //���ϵ���ɫ��
    unsigned short  curr_index;//����ʹ����ɫ
    winks_color * colorlist;    //��ɫ�б�
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
    unsigned char show_flags;/*WINKS_SHOW_LEFT        1   //������ʾ    WINKS_SHOW_CENTER    2   //������ʾ    WINKS_SHOW_RIGHT    3   //������ʾ*/   
    unsigned char effect_flags;//��Ч
    unsigned char wrapflags;  //WK_WG_NOWRAP   0//���۵�    WK_WG_WRAP   1//�۵� 
    unsigned char lineflags; // WK_WG_ONLYLINE  0//���� WK_WG_MULLINE   1//����

    unsigned long backgroudcolorflags; //  0 ���Ա�����ɫ// 1 ���Ʊ�����ɫ
    winks_text_colorlist_s text_colorlist;
    
    /* ����ɲ���д*/
    long int ismul_line_flags;//�Ƿ��Ƕ�������
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
/*�ַ�������*/
#define WK_STRING_TYPEASCII      1
#define WK_STRING_TYPEUNICODE    2



/*#include "Gdi_const.h"*/

#if 1//ndef WINKS_USE_PLATFORM_SOURCE_MANAGER
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
#else
#define winks_ui_imgcacheinit() 
#define winks_ui_imgcachedst()
#endif
/* ��������Ƶ���С���� */
#define WINKS_APN_NAME_MIN_LEN 40
/* �����˺ŵ���С���� */
#define WINKS_DATA_ACCOUNT_MIN_LEN 40

/** (mtk) ��ʼ���ʺ�����
*   NOTE: ��Ϊ�û������ֶ���ɾ�����, UI �����ڽ������ý���ǰ���á�
*/
void Winks_InitDataAccount(void (*callback_func)(void));

/* ͨ��ID ȡ����������, unicode ���� */
int Winks_GetApnName(unsigned long account_id, char *apn_name, unsigned long len);

/* ͨ��ID ��ȡ�˺�����, unicode ���� */
int Winks_GetDataAccountName(unsigned long account_id, char *data_account, unsigned long len);

/*ȡ����ʱ��*/
int Winks_GetBgLightTime(unsigned long *val);

/*����������绰*/
int Winks_GetAllKeyAcceptCall(unsigned long *val);

/*ȡUSB״̬��1:ϵͳ����USB, 0:ϵͳδ��USB.*/
int Winks_GetUsbState(unsigned long *val);

/* �����绰 */
void Winks_AcceptCall(void);

/* �ܽӵ绰 */
void Winks_RejectCall(void);

/* �Ҷϵ绰 */
void Winks_HangupCall(void);

/* ��ֹ���� */
void Winks_AbortCall(void);

/* ��ȡͨ���Ŀ�ʼʱ��ͳ���ʱ�� */
int Winks_GetCallTime(Winks_DateTime_s *start_time, Winks_DateTime_s *duration);

/*ȡ�����б����������룬����Ϊƴ������Ӣ��Ϊ��ĸ����*/
int Winks_GetPinyin(char *str, char *pyBuff, unsigned long pyBuffSize);
/* �ж�һ��unicode�ַ����е��ַ��Ƿ�ȫ��ascii */
int wk_is_string_ascii(const char *string);

/* ȡ˫���ַ������ַ�����������strlen. */
int WK_STRLEN(const char *str);

/* ˫���ַ�������������strcpy. */
char *WK_STRCPY(char* dst, const char *src);

/* ˫���ַ�������������strncpy��ע�������maxLen��˫�ֵĸ���������buffsize. */
char *WK_STRNCPY(char* dst, const char *src, int maxLen);

/* ˫���ַ�������������strncpy������������ע�������maxSize��Ԥ�ƿ���������ֽ���. */
char *WK_STRNCPYE(char* dst, const char *src, int maxSize);

/* ˫���ַ����Ƚϣ�����strcmp. */
long WK_STRCMP(const char* str1, const char *str2);

/* ˫���ַ����Ƚϣ�����strncmp��ע�������maxLen��˫�ֵĸ���������buffsize. */
long WK_STRNCMP(const char* str1, const char *str2, int maxLen);

/* ˫���ַ���ƴ�Ӻ���������strcat. */
char *WK_STRCAT(char* dst, const char *src);

/* ˫���ַ������ң�����strstr
���ַ���str1��Ѱ��str2��һ�γ��ֵ�λ�ã����ȽϽ�����NULL)�����û�ҵ�����NULL. */
char *WK_STRSTR(const char* str1, const char *str2);

/* �ַ�����ת������ */
int Winks_Ascii2UnicodeString(const char *in_ascii, char *out_unicode, unsigned int size);
int Winks_Unicode2AsciiString(const char *in_unicode, char *out_ascii, unsigned int size);
int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size);
int Winks_Unicode2UtfString(const char *in_unicode, char *out_utf8, unsigned int size);

#ifdef WINKS_DYNAMIC_UPDATE
/*������װ���� param,��װ�������ƺ�·���� size ���Ƶ�size*/
void winks_StartInstall(const char *param, unsigned long size);
#endif

#ifdef WINKS_ARENA
/*
����ת������for Arena:
����ֵ: �ɹ� - WINKS_RETURN_SUCCESS, ʧ�� - WINKS_RETURN_FAILURE.
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
/*����IDȡ�����������IP��prot��*/
int Winks_api_getProxy(void* apnID,char *Proxy, unsigned long proxySize, unsigned short *port);
#endif

/* �ú���ǿ����ϵͳ�Ľ�����б�������ָ���Ľ���㷽ʽ������������޸ģ����û����������
 * function: Winks_SysAddDataAccount
 * parameters:
 *   unsigned long defIndex: Ĭ�Ͻ�������кš�
 *   char *apn: ���������IP
 *   char *proxyIp: ���������IP
 *   unsigned short proxyPort: ����������˿�
 * return:
 *   �ɹ�����: 0
 *   ʧ�ܷ���:-1
 */
typedef enum  Winks_line_Dashstyle
{        
    WINKS_DASHSTYLE_NULL,   
    WINKS_LINE_SOLID,/*ʵ��*/   
    WINKS_LINE_DOT,  /*��*/    
    WINKS_LINE_DOTDASH,/*����*/
    WINKS_LINE_DOUBLELINE/*˫��*/
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
/*��UI����Ӧ�ù���ظ���Ϣ*/
int Winks_AppPauseResumeMsgToUI( Winks_Hwnd_e opt,  WinksAppPauseResume_e flag);


#define     WINKS_IMAGE_TOO_LARGE    3
#define     WINKS_IMAGE_SUCCESS   0




#if 1/**/
/*************************************************
           ������API�ӿ�
**************************************************/
#define WINKS_SERVER_UID_MAX_LEN 64  /*�������·���UID����󳤶�*/

/* չʾ���� */
#define WINKS_SETVAL_SWITCH_OFF 0  /*�������:��*/
#define WINKS_SETVAL_SWITCH_ON 1   /*�������:��*/
/* ͨ��������*/
#define WINKS_SETVAL_CCSHOW_RING 0x01   /*ͨ��ǰչʾ*/
#define WINKS_SETVAL_CCSHOW_TALK 0x02   /*ͨ����չʾ*/
#define WINKS_SETVAL_CCSHOW_HANGUP 0x04 /*�ҶϺ�չʾ*/
//typedef enum {
//    E_CALL_CONNECT = 0x01,
//    E_CALL_TALK = 0x02,
//    E_CALL_HANGUP = 0x04
//} Winks_ShowCase_e;
/* �����Ƭ���·�ʽѡ��*/
#define WINKS_SETVAL_PBU_AUTO 0x01      /*�Զ�����*/
#define WINKS_SETVAL_PBU_OFF 0x2        /*��������*/
#define WINKS_SETVAL_PBU_MANUALLY 0x3   /*��ʾ����*/
/* ������ʽ*/
#define WINKS_SETVAL_CW_TIMER 0x01      /*��ʱ����*/
#define WINKS_SETVAL_CW_PUSH 0x02       /*push����*/
/* �û�״̬*/
#define WINKS_SETVAL_US_NORMAL 0x00     /*�û�״̬:����*/
#define WINKS_SETVAL_US_MEETING 0x01    /*�û�״̬:����*/
/*��ʱ����ģʽ*/
#define WINKS_SETVAL_CPMOD_CUSTOM       0x00    /*�Զ���*/
#define WINKS_SETVAL_CPMOD_USUALLY      0x01    /*��������ģʽ*/
#define WINKS_SETVAL_CPMOD_UNUSUALLY    0x02    /*��ʡ�ʷ�ģʽ*/

#define WINKS_SMS_GATEWAY_NUM 2         /*��������������*/
#define WINKS_SMS_GATEWAY_LEN 16        /*����������󳤶�*/
#define WINKS_SMS_PREFIX_LEN 16         /*����ǰ׺��󳤶�*/

/* ����ѡ�� */
typedef enum tag_Winks_SettingType_e{
    WINKS_SETTING_SHOW_OTHER_SWITCH=0x00,   /* չʾ������ ���Ƿ�չʾ�Է����񿪹�*/
    WINKS_SETTING_ROAM_SWITCH,              /* ���������� ������ʱ��������*/
    WINKS_SETTING_SW_UPDATE_WAY,            /* ����������������Զ���������*/
    WINKS_SETTING_AUTO_START_SWITCH,        /* ������������أ�*/
    WINKS_SETTING_AUTO_ACTIVE,              /* �����Զ�����*/
    WINKS_SETTING_SERVICE_STOP_FLAG,        /* ֹͣ�����־*/
    WINKS_SETTING_FORBIDDEN_FLAG,           /* �û������ñ�־*/
    WINKS_SETTING_UNSUBSCRIBE_FLAG,         /* �û��˶���־*/
    WINKS_SETTING_HANGUP_UPDATE_SWITCH,     /* �ҶϺ���¿���*/
    WINKS_SETTING_SHOW_SPLASH_SWITCH,       /* ����Ƭͷ����*/
    WINKS_SETTING_CALL_CONNECT_SWITCH,      /* ������������*/
    WINKS_SETTING_ONLY_25G_SWITCH,          /* ֻ֧��2.5G����*/
    WINKS_SETTING_ALREADY_UPLOAD_SETTING,   /* �Ѿ��ϴ��������*/
    WINKS_SETTING_ALREADY_UPLOAD_PB,        /* �Ѿ��ϴ��绰������*/

/************************************/
/*****  Add switch here above.*******/
    WINKS_SETTING_SWITCH_END=0x40,       
/************************************/
    WINKS_SETTING_SHOW_CASE,                /* ͨ�����ã�ͨ��ǰ��ͨ���С��ҶϺ� */
    WINKS_SETTING_PBWC_UPDATE_WAY,          /* �����Ƭ���·�ʽ */
    WINKS_SETTING_CONNECT_WAY,              /* ������ʽ�� ��ʱ���¡�PUSH���� */
    WINKS_SETTING_USER_STATE,               /* �û�״̬�� ���������� */
    WINKS_SETTING_PROXY_PORT,               /* http����������˿� */
    WINKS_SETTING_SW_VERSION,               /* ����汾�� */
    WINKS_SETTING_APN_ID,                   /* http�����IDֵ */
    WINKS_SETTING_DIY_FILE_MAX_SIZE,        /* DIY�����ϴ��ļ������SIZE */
    WINKS_SETTING_HUSHOW_TIME,              /* �ҶϺ���񲥷�ʱ�� */
    WINKS_SETTING_CONNECT_PERIOD,           /* ��ʱ����ʱ������ */
    WINKS_SETTING_CONNECT_PERIOD_MOD,       /* ��ʱ����ģʽ*/
    WINKS_SETTING_CONNECT_PERIOD_USUALLY,   /* ��ʱ������������ģʽ,ʱ������*/
    WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY, /* ��ʱ������ʡ�ʷ�ģʽ,ʱ������*/
    WINKS_SETTING_CONNECT_TIMEOUT,          /* ������ʱʱ�� */
    WINKS_SETTING_SPLASH_DURATION,          /* Ƭͷ����ʱ��,0��ʾ������Ƭͷ */
    WINKS_SETTING_PUSH_PORT,                /* PUSH�˿� */
    WINKS_SETTING_CC_REPLACE_WA,            /* ������̭�㷨�ļ�Ȩֵ */
    WINKS_SETTING_HANGUP_UPDATE_DELAY_TIME, /* �ҶϺ���µ���ʱʱ�䣬Ϊ0��ʾ����ʱ�� */
    WINKS_SETTING_TODAY_BYTES,              /* ��������*/
    WINKS_SETTING_TODAY_DATE,               /* ��������*/
    WINKS_SETTING_LAST_MONTH_BYTES,         /* ��������*/
    WINKS_SETTING_THIS_MONTH_BYTES,         /* ��������*/
    WINKS_SETTING_THIS_MONTH_DATE,          /* ��������*/
    WINKS_SETTING_TOTAL_BYTES,              /* ͳ������*/
    WINKS_SETTING_TOTAL_DATE,               /* ��������*/
    WINKS_SETTING_TOTAL_TIME,               /* ����ʱ��*/
    
/************************************/
/*****  Add value here above. *******/
    WINKS_SETTING_VALUE_END=0x80,       
/************************************/
    WINKS_SETTING_PROXY_IP,                 /* http���������IP */
    WINKS_SETTING_APN,                      /* http����� */
    WINKS_SETTING_SMS_GATEWAY,              /* �������·��Ķ�������*/
    WINKS_SETTING_SMS_PREFIX,               /* ����ǰ׺ */
    //WINKS_SETTING_USER_STATE_TEXT,        /* �û�״̬�ı���*/
    WINKS_SETTING_OPR_SERVER_URL,           /* ���ݷ�������URL */
    WINKS_SETTING_MEDIA_SERVER_URL,         /* ý���������URL */
    WINKS_SETTING_SHOP_SERVER_URL,          /* �����̳ǵ�URL */
    WINKS_SETTING_UID,                      /* �������·���UID */
    WINKS_SETTING_ACTIVE_IMSI,              /* ����������IMSI���� */
    WINKS_SETTING_CONNECT_BACKUP,           /* ���ݶ�ʱ��������ʱ��*/

/************************************/
/*****  Add string here above. ******/
    WINKS_SETTING_STRING_END=0xa0,       
/************************************/
    WINKS_SETTING_BLOCK_DATA,               /*����*/
    WINKS_SETTING_FORCE_RELOAD_DATA,        /* ǿ�����¼������ݡ��û�symbianƽ̨(��Ƕ�����������EXE��������ʹ��)ǿ������������ݻָ� */
    WINKS_SETTING_BLOCK_SERVER_CONFIG,      /*�������·��ķ�������*/

/************************************/
/*****  Add block here above.  ******/
    WINKS_SETTING_NULL
/************************************/
} Winks_SettingType_e;

/* �ñ�־,����ͳһ�ύ����ֵ(���ļ�).�øñ�־�󣬵���Winks_SetSettingVal��
 * Winks_SetSettingString�����󲻴��ļ��������е��ú���Winks_WriteSetting�����������
 * function: Winks_SetSettingCommitTagether
 * parameters:
 *   
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetSettingCommitTagether(void);
    
/*�ָ�Ĭ�����á�
 * function: Winks_CommitTagether
 * parameters:
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_CommitTagether(void);

/*�ָ�Ĭ�����á�
 * function: Winks_SettingDefault
 * parameters:
 *   
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SettingDefault(void);

/* �����������ֵ
 * function: Winks_SetSettingValue
 * parameters:
 *   Winks_SettingType_e type: ���������
 *   unsigned long value: ������ֵ
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetSettingValue(Winks_SettingType_e type, unsigned long value);

/* ��ȡ�������ֵ
 * function: Winks_GetSettingValue
 * parameters:
 *   Winks_SettingType_e type: ���������
 *   unsigned long *value: (out)������ֵ
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetSettingValue(Winks_SettingType_e type, unsigned long *value);

/* �洢�ַ����������
 * function: Winks_SetSettingString
 * parameters:
 *   Winks_SettingType_e type: ���������
 *   unsigned long *srStr: ���õ��ַ���
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetSettingString(Winks_SettingType_e type, const char *srStr);

/* ��ȡ�ַ����������
 * function: Winks_GetSettingString
 * parameters:
 *   Winks_SettingType_e type: ���������
 *   const char *destStr: (out)��ȡ���ַ���
 *   unsigned long destSize: \destStr��buff size
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetSettingString(Winks_SettingType_e type, const char *destStr, unsigned long destSize);

/* �洢���ݿ��������
 * function: Winks_SetSettingBlock
 * parameters:
 *   Winks_SettingType_e type: ���������
 *   const void *block: ���洢�����ݿ�
 *   unsigned long blockSize: ���洢�����ݿ�Ĵ�С
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetSettingBlock(Winks_SettingType_e type, const void *block, unsigned long blockSize);

/* ��ȡ���ݿ��������
 * function: Winks_GetSettingBlock
 * parameters:
 *   Winks_SettingType_e type: ���������
 *   void *block: (out)��ȡ�����ݿ�Ĵ��λ��
 *   unsigned long blockSize: ����ȡ���ݿ�Ĵ�С
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetSettingBlock(Winks_SettingType_e type, void *block, unsigned long blockSize);

/* ���ü����IMSI����
 * function: Winks_SetRecentlyActivatedImsi
 * parameters:
 *   const char *imsi:imsi����
 *   unsigned long len:imsi����ĳ���
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetRecentlyActivatedImsi(const char *imsi, unsigned long len);

/* ��ȡ�����IMSI����
 * function: Winks_GetRecentlyActivatedImsi
 * parameters:
 *   const char *imsi:(out)imsi����
 *   unsigned long len:imsi��buff size
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetRecentlyActivatedImsi(char *imsi, unsigned long len);

/* �����û��Ƿ�Ϊ��Ĭ�û���״̬���û���IMSIΪ��ʶ
 * function: Winks_SetImsiPopFlag
 * parameters:
 *   const char *imsi:imsi����
 *   unsigned char flag:0:������; 1:����.
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetImsiPopFlag(const char *imsi, unsigned char flag);

/* ��ȡ�û��Ƿ�Ϊ��Ĭ�û���״̬���û���IMSIΪ��ʶ
 * function: Winks_GetImsiPopFlag
 * parameters:
 *   const char *imsi:imsi����
 *   unsigned char flag:(out)0:������; 1:����.
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetImsiPopFlag(const char *imsi, unsigned char *flag);

/* ����HTTP���������.
 * function: Winks_SetProxyServer
 * parameters:
 *   const char *proxyIp: ���������IP
 *   unsigned short proxyPort: ����������˿�
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetProxyServer(const char *proxyIp, unsigned short proxyPort);

/* ��ȡHTTP���������.
 * function: Winks_GetProxyServer
 * parameters:
 *   const char *proxyIp: (out)���������IP
 *   unsigned short *proxyPort: (out)����������˿�
 *   unsigned long proxyIpSize: \proxyIp��buff size
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetProxyServer(char *proxyIp, unsigned short *proxyPort, unsigned long proxyIpSize);

/*���ò�����̭��Ȩֵ.ÿ��ȨֵС��256.
 * function: Winks_SetCcRwa
 * parameters:
 *   unsigned long rwaX: ���ʶ�Ȩֵ
 *   unsigned long rwaX: ���Ŵ���Ȩֵ
 *   unsigned long rwaX: ����ʱ��Ȩֵ
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetCcRwa(unsigned long rwaX, unsigned long rwaY, unsigned long rwaZ);

/*��ȡ������̭��Ȩֵ.ÿ��ȨֵС��256.
 * function: Winks_SetCcRwa
 * parameters:
 *   unsigned long *rwaX: ���ʶ�Ȩֵ
 *   unsigned long *rwaX: ���Ŵ���Ȩֵ
 *   unsigned long *rwaX: ����ʱ��Ȩֵ
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetCcRwa(unsigned long *rwaX, unsigned long *rwaY, unsigned long *rwaZ);

/*��ȡ�Ƿ�չʾƬͷ��־��1: ����, ����: ����*/
int Winks_if_show_titleCC(void);    

/*��ȡ�Ƿ�չʾ�����־. 1:չʾ,0:��չʾ*/
int Winks_if_show_cc(void);

/*�жϺ����Ƿ�������뺯����0: ����, 1: �ǡ�*/
int Winks_if_emergency_number(const char *no);

/*����ͳ�ơ�byte:������Ҫ���ӵ�����.�ɹ�����0��ʧ�ܷ���-1.*/
int Winks_TrafficStatistics(unsigned long byte);

/*����ͳ��:���㡣�ɹ�����0��ʧ�ܷ���-1.*/
int Winks_TrafficClean(void);
#endif

#define WINKS_MSG_BOX_TYPE_INFO 0 /* �����Ի�������: ѯ�ʷ�ʽ */
#define WINKS_MSG_BOX_TYPE_QUERY 1 /* �����Ի�������: ��Ϣģʽ */
typedef void (*WINKS_CALLBACK_FUNC)(void);

/* ����ϵͳ�Ի�������Ҫ��Ϊ��������ʽ��
 * function: Winks_LoadSysMsgBox
 * parameters:
 *   int msgBoxType: (in)�Ի������͡�
 *          WINKS_MSG_BOX_TYPE_QUERY ѯ�ʶԻ���(��ʾ�û�ѡ�������)��
 *          WINKS_MSG_BOX_TYPE_INFO  ��Ϣ�Ի���(��ʾ�û�ȷ��)
 *   char *msg: (in)��ʾ�����ݣ�Unicode���롣
 *   WINKS_CALLBACK_FUNC lskcbf: (in)������ص��������û�ѡ���ǻ�ȷ�Ϻ�Ļص�������û�д�NULL��
 *   WINKS_CALLBACK_FUNC rskcbf: (in)������ص��������û�ѡ����Ļص�������
 *          û�д�NULL�������ʾ������ΪWINKS_MSG_BOX_TYPE_INFO�ò��������塣
 * return:
 *   void * ���سɹ����ضԻ���ľ����
 */
void * Winks_LoadSysMsgBox(int msgBoxType, char *msg, WINKS_CALLBACK_FUNC lskcbf, WINKS_CALLBACK_FUNC rskcbf);

/* �����Ѿ������ĶԻ���
 * function: Winks_UnloadSysMsgBox
 * parameters:
 *   void* WHND: (in)�Ի���ľ����
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
 int Winks_UnloadSysMsgBox(void* WHND);

#ifdef __cplusplus
}
#endif

#endif
