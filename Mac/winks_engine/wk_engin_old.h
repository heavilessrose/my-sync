/*
File:		wk_engin.h
Author:		Huaxm
Create Time:17/03/2008
*/
#ifndef _WK_ENGIN_H_
#define _WK_ENGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tag_Winks_Eng2UiMsg_e
{
    WINKS_MSG_ENG2UI_NONE           = 0,    /* XX �޹���Ϣ */
    
/* �ɹ���������Ϣ */
    
/* 1 ע�ἰ��ʼ������ */
    WINKS_MSG_ENG2UI_SETUP_SVR_OK   = 0x11, /* XX �������÷������ɹ� */
    WINKS_MSG_ENG2UI_SMS_REG        ,       /* δע�ᣬ��Ҫ����ע�����     [Winks_UserSendRegSms] */
    WINKS_MSG_ENG2UI_SMS_REG_OK     ,       /* ����ע��OK */
    WINKS_MSG_ENG2UI_INIT           ,       /* XX ���ڳ�ʼ��... */
    WINKS_MSG_ENG2UI_INIT_OK        ,       /* ��ʼ����� */
/* 2 ���º���¼ */
    WINKS_MSG_ENG2UI_FL_OK          = 0x21, /* XX �����б�������� */
    WINKS_MSG_ENG2UI_FL_NOTIFY      ,       /* ֪ͨ�û��к��Ѹ��� */
    WINKS_MSG_ENG2UI_FL_SEL_FRIEND  ,       /* �����б�������ϣ�ѡ�����¼ [Winks_UserSetFriendList] */
    WINKS_MSG_ENG2UI_FRIEND_SET     ,       /* XX �ϴ�����¼�ɹ� */
    WINKS_MSG_ENG2UI_FRIEND_UPD     ,       /* XX �ϴ�����¼�����غ������� */
    WINKS_MSG_ENG2UI_DL_FRIENDCC    ,       /* �������غ��Ѳ���... */
    WINKS_MSG_ENG2UI_FRIEND_OK      ,       /* ����¼������� */
#if 0//def WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_FL_SEL_MCARD   ,       /* �����б�������ϣ�ѡ��洢�� [Winks_UserSelMCard] */
#endif
    
/* 3 ͬ���������õ������� */
    WINKS_MSG_ENG2UI_SETUP_OK       = 0x31, /* ͬ���������óɹ� */
    
/* 4 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY            = 0x41, /* XX �����ϴ�DIY... */
    WINKS_MSG_ENG2UI_DIY_OK         ,       /* �ϴ�DIY�ɹ� */
    
/* 5 ���¼����ѯ������ע��״̬ */
    WINKS_MSG_ENG2UI_REREG_OK       = 0x51, /* ���¼���ɹ� */

/* 6 �绰���� */
    WINKS_MSG_ENG2UI_CALLCC_GRT_OK  = 0x61, /* ��ȡ�û��ʺ���ɹ� */
    WINKS_MSG_ENG2UI_CALLCC_OK      ,       /* ��ȡ�û��ʺ���ɹ� */
    WINKS_MSG_ENG2UI_CALLCC_OTHER   ,       /* XX��ȡƬͷ��Ӫ�̲��� */
    
/* 7 ��̨���� */
    WINKS_MSG_ENG2UI_PUSH           = 0x71, /* XX ��̨���ڶ�ʱ���·���... */
    WINKS_MSG_ENG2UI_PUSH_FRIEND    ,       /* XX ��̨���ڸ��º���... */
    WINKS_MSG_ENG2UI_PUSH_OK        ,       /* XX ��̨��ʱ���·������ */
    WINKS_MSG_ENG2UI_PUSH_CHECK_FL  ,       /* XX ��̨��ʱ���·������,����Ҫ�������б� */

    WINKS_MSG_ENG2UI_INITCACHE_FL   ,       /* XX ��̨�������غ����б��Ա��ʼ��Cache... */
    WINKS_MSG_ENG2UI_INITCACHE      ,       /* XX ��̨���ڳ�ʼ��Cache... */
    WINKS_MSG_ENG2UI_INITCACHE_OK   ,       /* XX ��̨��ʼ��Cache��� */

    WINKS_MSG_ENG2UI_RECACHE_TRIM   ,       /* XX ��̨�������ػ�����Ϣ���ؽ�Cache... */
    WINKS_MSG_ENG2UI_RECACHE        ,       /* XX ��̨�����ؽ�Cache... */
    WINKS_MSG_ENG2UI_RECACHE_OK     ,       /* XX ��̨�ؽ�Cache��� */

    WINKS_MSG_ENG2UI_RECRUIT_DL     ,       /* XX ��̨���ڲ������ص绰����... */
    WINKS_MSG_ENG2UI_RECRUIT_DL_OK  ,       /* XX ��̨�������ص绰������� */

/* ������Ϣ */

/* 1 ע�ἰ��ʼ������ */
    WINKS_MSG_ENG2UI_REG_FAIL       = 0x8011,   /* ע��ʧ�� */
    WINKS_MSG_ENG2UI_INIT_FAIL      ,           /* ��ʼ��ʧ�� */

/* 2 ���º���¼ */
    WINKS_MSG_ENG2UI_FL_FAIL        = 0x8021,   /* ���º���¼ʧ�� */

/* 3 ͬ���������õ������� */
    WINKS_MSG_ENG2UI_SETUP_FAIL     = 0x8031,   /* ͬ����������ʧ�� */

/* 4 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = 0x8041,   /* �ϴ�DIYʧ�� */

/* 5 ���¼����ѯ������ע��״̬ */
    WINKS_MSG_ENG2UI_REREG_FAIL     = 0x8051,   /* ���¼���ʧ�� */

/* 6 �绰���� */
    WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL  = 0x8061, /* �ʺ�������ʧ�� */
    WINKS_MSG_ENG2UI_CALLCC_FAIL      ,         /* �û���������ʧ�� */
    WINKS_MSG_ENG2UI_CALLCC_TIMEOUT   ,         /* �û��������س�ʱ */
    WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL ,         /* XX��ȡƬͷ��Ӫ�̲���ʧ�� */

/* 7 ��̨���� */
    WINKS_MSG_ENG2UI_PUSH_FAIL      = 0x8071,   /* XX ��̨��ʱ���·���ʧ�� */

    WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL      ,   /* XX ��̨���غ����б�ʧ�� */
    WINKS_MSG_ENG2UI_INITCACHE_FAIL         ,   /* XX ��̨��ʼ��Cacheʧ�� */

    WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL      ,   /* XX ��̨���ػ�����Ϣʧ�� */
    WINKS_MSG_ENG2UI_RECACHE_FAIL           ,   /* XX ��̨�ؽ�Cacheʧ�� */

    WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL        ,   /* XX ��̨�������ص绰����ʧ�� */

/* ����Э����� */
    WINKS_ENG_ERROR_CALL_FUNC = 0x8101,     /* ���ú������� */
    WINKS_ENG_ERROR_PROT_NO     ,           /* �������·�Э��Ų�ƥ�� */
    WINKS_ENG_ERROR_SVR_RET     ,           /* �������������ݴ��� */
    WINKS_ENG_ERROR_DATA_PARSE  ,           /* �������������ݽ������� */
    WINKS_ENG_ERROR_DATA_RSV    ,           /* ��������ʧ��*/
/* ϵͳ���� */
    WINKS_MSG_ENG2UI_SVR_NO_ANSWER  = 0x8201,   /* ��������Ӧ�� */
    WINKS_MSG_ENG2UI_UNREG_USER     ,           /* ��ע���û� */
    WINKS_MSG_ENG2UI_ROAMING_LIMIT  ,           /* ��������  */
    WINKS_MSG_ENG2UI_ENG_BUSY       ,           /* ����æ,�Ժ����� */
    WINKS_MSG_ENG2UI_DIAL_INT       ,           /* ���汻�绰�ж� */
    WINKS_MSG_ENG2UI_STORAGE_LIMIT  ,           /* �洢�ռ䲻�� */
    WINKS_MSG_ENG2UI_CC_CLOSED      ,           /* ��������ر�  */
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,           /* �洢�������� */
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    WINKS_MSG_ENG2UI_SYS_FILES_ERR  ,           /* ϵͳ�ļ����ƻ� */
//#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_CC_FILES_ERR   ,           /* �����ļ����ƻ� */
//#endif /* WINKS_SUPPORT_MEMORY_CARD */
#ifdef WINKS_DUAL_SIM
    WINKS_MSG_ENG2UI_SIM_SEL_ERR    ,           /* ��ʾ�û�ѡ��Ҫ�����Sim�� */
    WINKS_MSG_ENG2UI_SIM_UNUSABLE   ,           /* �û�ѡ���Sim�������� */
#endif /* WINKS_DUAL_SIM */

}Winks_Eng2UiMsg_e;

typedef struct tag_Winks_EngineToUiReturnMsg_s
{
	unsigned long err_state;
	unsigned long err_code;
	char    msg_body[1];
}Winks_EngineToUiReturnMsg;

/* �����б�֪ͨ���� */
typedef enum {
    E_FL_UPD_NONE = 0,  /* �޸��� */
    E_FL_UPD_NOTIFY,    /* �и��� */
    E_FL_UPD_SELFRIEND, /* ѡ�����¼ */
    E_FL_UPD_IF_SELMCARD   /* �Ƿ�ѡ��洢�� */
} Winks_FLNotifyType_e;

typedef struct tag_Winks_EngPeriodComInfo_s
{
    unsigned char   FA_UpdFlag;         /*�û����ѹ�ϵ���±�־ 0--�޸���,1--�и���,2--ѡ�����¼*/
    int   Reserved;                     /*���� -- �ϴ��ֻ�ͳ����Ϣ����*/
}Winks_EngPeriodComInfo_s;

#define WINKS_ENGINE_RETURN_MSG_MAX_LEN (1024)
typedef struct tag_Winks_EngineRetMsgBody_s
{
    unsigned char msg[WINKS_ENGINE_RETURN_MSG_MAX_LEN];

}Winks_EngineRetMsgBody_s;

typedef struct tag_Winks_EngUpdComInfo_s
{
    unsigned long   patchTotal;         /*��Ƭ���ص���Ƭ��*/
    unsigned long   patchCount;         /*��Ƭ���صļ�����*/
    unsigned long   prtRetIndex;        /*���������ص����к�*/
}Winks_EngUpdComInfo_s;

typedef struct tag_Winks_EngDLCCInfo_s
{
    unsigned short   Total;         /* ��Ҫ���صĲ������� */
    unsigned short   Count;         /* ���ز������ */
    unsigned short   FinCnt;        /* ���سɹ������� */
    unsigned short   ErrCnt;        /* ����ʧ�ܲ����� */
}Winks_EngDLCCInfo_s;

typedef struct tag_Winks_EngDiyProgInfo_s
{
    unsigned char   image_count;            /*����������(n),ĿǰֻΪ1���ɲ�����*/
    unsigned char   curIndex;               /*��ǰ�ϴ��ڼ���(x/n),ĿǰֻΪ1���ɲ�����*/
    unsigned short  subIndex;               /*��ǰ�ϴ���Ϊ�ڼ���(y/m)*/
    unsigned short  subTotalCount;          /*��ǰ�����Ϊ����(��curIndex��Ϊm��)*/
}Winks_EngDiyProgInfo_s;

/* with end char '\0' */

/* �������·��Ĳ���ID��󳤶� */
#define WINKS_MAX_LEN_OF_COLOR_CALL_ID 40 // 52
/* һ�������е�ͼƬ������ */
#define WINKS_MAX_NUM_OF_IMAGE_LIST 1 /*4*/

/* �ʺ����ַ������� */
#define WINKS_MAX_NUM_OF_WELCOME_INFO 100
/* �ʺ�����󳤶�(UTF-8 ����) */
#define WINKS_MAX_LEN_OF_WELCOME_INFO (100 * 3 + 1) /* bytes */

/* ���Ѻ�����󳤶� */
#define WINKS_FRIEND_NO_LEN 40
/* ����������󳤶� */
#define WINKS_FRIEND_NAME_LEN 24 // 20

/* ����������󳤶� */
#define WINKS_LOCAL_NO_LEN 40

/* �������� */
typedef enum {
    E_CC_Friend = 1,      /* ���Ѳ��� */
    E_CC_Title = 2,       /* Ƭͷ���� */
    E_CC_Operator = 3    /* ��Ӫ�̲��� */
} Winks_CcType_e;

/* ����ѡ�� */
typedef enum {
    E_SETTING_NULL = 0,

    E_SETTING_INTEGER = 100,
    E_SETTING_SHOW_CC_TITLE,
    E_SETTING_SHOW_CTRL,        /* չʾ���� */
    E_SETTING_SHOW_CASE,        /* ͨ������ */
    E_SETTING_CONNECT_POINT,    /* �������� */
    E_SETTING_ROAM_SWITCH,      /* ���ο��� */
    E_SETTING_CLIENT_CONNECT_WAY,       /* �ն����ӷ�ʽ */
    E_SETTING_CLIENT_CONNECT_TIMEOUT,   /* �ն����ӳ�ʱ */

    E_SETTING_PUSH_PORT,        /* PUSH �˿� */
    E_SETTING_LONG_CYCLE,       /* ������ */
    E_SETTING_SHORT_CYCLE,      /* ������ */
    E_SETTING_BCARD_ATTR,           /* ������Ƭ���� */
    E_SETTING_FRIEND_LIST_MAX_CNT,  /* �ֻ�Ԥ����Ѽ�¼���ֵ */
    E_SETTING_TaO_MAX_CNT,          /* �ֻ�Ԥ����Ӫ�̺�Ƭͷ���ֵ */
    E_SETTING_TEMP_FILE_CNT,         /* Ԥռ����ʱ�ļ����� */
    E_SETTING_STORAGE_LOCATION,     /* ����洢λ�� */
//    E_SETTING_GREET_UPDATE_FLAG,    /* �ʺ�����±�־ */

    E_SETTING_STRING = 200,
    E_SETTING_STR_PHONE_NO,     /* �������� */
    E_SETTING_STR_SMS_GW,       /* �������غ��� */
    E_SETTING_STR_TIME_FLAG,     /* ʱ��� */

    E_SETTING_STRUCT = 300,
    E_SETTING_STRUCT_COMM_SERVER,       /* ͨѶ������ */
    E_SETTING_STRUCT_PROXY_SERVER,      /* HTTP ��������� */
    E_SETTING_STRUCT_CONNECT_WAY,      /* ���������ӷ�ʽ */

    E_SETTING_ALL
} Winks_SettingType_e;

typedef enum {
    E_GFLAG_NORMAL = 0,     /* ���� */
    E_GFLAG_NO_MC = -100,   /* û�д洢�� */
    E_GFLAG_NEW_MC = -101  /* �����洢�� */
} Winks_GreetingFlag_e;

/* ����ʹ��, ����ɾ�� */
#define E_I_SHOW_OTHER 0x01
#define E_OTHER_SHOW_ME 0x02

/* չʾ���� */
typedef enum {
    E_CC_SHOW_OFF = 0,
    E_CC_SHOW_ON = 1
} Winks_ShowCtrl_e;

/* ͨ������ */
typedef enum {
	E_CALL_CONNECT = 0x01,
	E_CALL_TALK = 0x02,
	E_CALL_HANGUP = 0x04
} Winks_ShowCase_e;

/* ����洢λ�� */
typedef enum
{
    E_STORAGE_PHONE = 0,    /* �ֻ� */
    E_STORAGE_MCARD         /* �洢�� */
} Winks_StorageLocation_s;

/* ���ο��� */
typedef enum
{
	 WINKS_ROAM_SWITCH_OFF = 0,
	 WINKS_ROAM_SWITCH_ON = 1
 } Winks_RoamSwitch_s;

/* �������� */
typedef enum
{
	WINKS_AUDIO_SWITCH_OFF = 0,
	WINKS_AUDIO_SWITCH_ON = 1
} Winks_AudioSwitch_s;

/* ����ͼƬ���� */
typedef enum {
	E_IMAGE_JPG = 1,
	E_IMAGE_PNG,
	E_IMAGE_TIFF,
	E_IMAGE_SPL,
	E_IMAGE_SWF,
	E_IMAGE_GIF,

	E_IMAGE_PPT
} Winks_CcImageType_e;

typedef struct {
	char ID[WINKS_MAX_LEN_OF_COLOR_CALL_ID];	/* ����ID */
	unsigned short show_times;		/* չʾ���� */
	unsigned short int_times;		/* ���жϴ��� */
	unsigned long time;				/* չʾʱ�� */
} Winks_CC4Stat_s;

/* DIY���� */
typedef struct
{
	char number[WINKS_FRIEND_NO_LEN]; /*���ƺ��ѵ��ֻ�����*/
	unsigned char image_type;	/*��������*/
	unsigned char image_count;
	char image_files[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX]; /* UTF-8 ���� */
	char welcome[WINKS_MAX_LEN_OF_WELCOME_INFO]; /* �ʺ���(UTF-8 ����) */
} Winks_DiyInfo_s;

/* �������з��� */
typedef enum {
	E_DIAL_IN = 1,
	E_DIAL_OUT
} Winks_DialDirection_e;

#if 0
typedef struct {
	unsigned short index;		/* �������� */
	unsigned short _pad;		/* for 4 bytes alignment */

	unsigned char img_type;		/* ͼƬ���� */
	unsigned char img_cnt;		    /* ͼƬ���� */
	unsigned short interval;		/* ����ͼƬ�Ĳ��ż�� */
	unsigned long play_time;		/* ����ʱ��(Ƭͷʹ��) */

    char img_path[WINKS_MAX_NUM_OF_IMAGE_LIST][WINKS_PATH_MAX]; /* ͼƬ�洢·��(UTF-8 ����) */
    char grt[WINKS_MAX_LEN_OF_WELCOME_INFO];        /* �ʺ���(UTF-8 ����) */
    Winks_GreetingFlag_e GFlag;      /* �Ƿ������ʺ��� */
} Winks_CC4UI_s;
#else
typedef struct tag_WinksStrgFile_CCShow_s
{
    char            NO[16/*WINKS_STRG_PHONE_LEN*/];    /* �绰�����ַ��� */
    char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* �����ǳ� */
    char            ID[40/*WINKS_STRG_CC_ID_LEN*/];    /* ����ID */
    unsigned short  CC_Idx;     /* ����Idx */
    unsigned short  interval;   /* ͼƬ���ż�� */
    unsigned char   img_type;   /* �������� */
    unsigned char   img_cnt;    /* ͼƬ�� ���Ϊ4 */
    unsigned char   GreetLen;   /* �ʺ��ﳤ�� */
    unsigned char   SaveType;   /* ����洢���� 0--�ļ� 1--�ڴ� */
    unsigned char * pCCImage;   /* �洢�Ĳ����ڴ����� */
    unsigned long   CCImageSize;/* �洢�Ĳ����ڴ����ݴ�С */
    char            CCImgPath[4][256];     /* �洢�Ĳ���·�� */
    short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* �洢�Ĳ����ʺ��� */
}WinksStrgFile_CCShow_s;
#endif

typedef struct tag_Winks_FriendItem_s
{
	char number[WINKS_FRIEND_NO_LEN];	/* �����ֻ����� */
	char name[WINKS_FRIEND_NAME_LEN];	/* �����ǳ� */
    unsigned long flag;	           /* �Ƿ��ڱ��غ�����*/
} Winks_FriendItem_s;


/**
*   ��utf8��ʽ���ַ���ת��Ϊunicode������ת����unicode �ַ�����, -1��ʾʧ�ܡ�

*   @in_utf8: �����utf8�ַ���
*   @out_unicode: �����unicode�ַ���, С�˷�ʽ
*   @size: ����Ŀ��ó���
*/
int Winks_Utf2UnicodeString(const char *in_utf8, char *out_unicode, unsigned int size);

int Winks_set_setting_value(Winks_SettingType_e type, unsigned long value);
int Winks_get_setting_value(Winks_SettingType_e type, unsigned long *value);

/*��ȡ�Ƿ�չʾƬͷ��־��1: ����, ����: ����*/
int Winks_if_show_titleCC(void);

/*��ȡ�Ƿ�չʾ�����־*/
int Winks_if_show_cc(void);

/* �жϲ����Ƿ�� */
int Winks_if_open_cc(void);

/* ���ò���ʱ�� */
int Winks_set_show_case(unsigned long flag);

/* ��ȡ����ʱ�� */
int Winks_get_show_case(unsigned long *flag);

/*�жϺ����Ƿ�������뺯��*/
int Winks_if_emergency_number(const char *no);

/* ��ȡƬͷ����Ӫ�̲��� */
//int Winks_next_title_cc(Winks_CC4UI_s *cc4ui);
//int Winks_next_operator_cc(Winks_CC4UI_s *cc4ui);

/* ��ȡĬ�ϲ��� */
//int Winks_get_default_cc(Winks_CC4UI_s *cc4ui);
/* ��ȡ���Ѳ��� */
//int Winks_get_friend_cc(const char *friend_no, Winks_DialDirection_e direction, Winks_CC4UI_s *cc4ui);
//int Winks_get_friend_name(const char *friend_no, char *friend_name, unsigned long len);
//int Winks_modify_friend_name(const char *friend_no, const char *friend_name);

/* ��ȡ���غ���¼ */
//int Winks_get_friend_cnt(void);
//int Winks_get_friend_list(Winks_FriendItem_s *array, unsigned long begin, unsigned long cnt);

/* ��¼�˴β��񲥷������interrupted: 0 - δ���, 1 - �����  */
//int Winks_log_cc_show(unsigned long index, unsigned long time, unsigned char interrupted);

/*
*   ��������б�
*   NOTE: Ҫ��array�е�number��name��unicode���롣
*/
//void Winks_SortFriendList(Winks_FriendItem_s *array, unsigned int count);

/*�����ʼ��*/
int Winks_EngineInit(void);
/*����ע��*/
int Winks_EngineDestroy(void);

#ifdef WINKS_DUAL_SIM
/* �����ſ�ʱ,�û�ѡ�����ſ����� */
int Winks_UserSetActiveSim(int SimNo);
#endif /* WINKS_DUAL_SIM */
/*�û�ע�����ҵ�񣬹�UIʹ�á�����ֵ:0�ɹ�,-1ʧ�ܡ�*/
int Winks_UserRegist(void);

/*�û�ȷ��ע�ᣬ����ע�����Ϣ,0:ȡ��ע�ᣬ1:����ע��*/
int Winks_UserSendRegSms(int reg_flag);

/*�û����º���¼����ȡ�������·��ĺ����б��Ա����*/
//int Winks_GetFullFriendListCount(void);

/*�û����º���¼��ȡ�����б��еĺ���*/
//int Winks_UserGetFullFriendItem(Winks_FriendItem_s *item, unsigned long index);

/*�����û�ѡ�����¼��friend_list:��"|"�ָ��ĺ���.����"13900000001|13900020002|"*/
int Winks_UserSetFriendList(char *friend_list, unsigned long friend_list_len);

#if 0//def WINKS_SUPPORT_MEMORY_CARD
/* ���ֻ��ռ䲻���Ҵ洢���ռ����ʱѯ���û��Ƿ�ʹ�ô洢��,��ѡ��洢��ʱ���ô˺��� */
int Winks_UserSelMCard(int YesOrNo);
#endif /* WINKS_SUPPORT_MEMORY_CARD */

/*�û����º�����¼*/
int Winks_UserResetFriendList(void);
/*�û�DIY����:�ϴ�������*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);
int Winks_UserGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow);
int Winks_UserStopCCShow();
/*���ÿ�������ʾ�û�ע�ᡣ*/
int Winks_UserRememberImsi(void);
/*ֹͣ����*/
int Winks_EngineStop(void);
#ifdef WINKS_DEL_PV2_0    
/*�û����Ի�����: ͬ��������*/
typedef struct tag_Winks_SelfSetting_s
{
    /* 1: ����, 0: �ر� */
    unsigned char ccSwitch;  /* �Է�չʾ�ҵĲ���(�ϴ�������+���ر���) */
    unsigned char ShowCC;  /* ��չʾ�Է�����(���ر���) */
} Winks_SelfSetting_s;
int Winks_UserUploadSelfSetting(Winks_SelfSetting_s *self_info);
#else
typedef struct tag_Winks_SelfSetting_s
{
    /* 1: ����, 0: �ر� */
    unsigned char ccSwitch;  /* �Է�չʾ�ҵĲ���(�ϴ�������+���ر���) */
    unsigned char ShowCC;  /* ��չʾ�Է�����(���ر���) */
} Winks_SelfSetting_s;
#define Winks_UserUploadSelfSetting(x) 
#endif
/* �رղ��� */
int Winks_UserCloseWinkCC(void);
/* �������� */
int Winks_UserOpenWinkCC(void);
/* �û����ӷ�ʽ����:Push��ʽ��ʱ���� */
int Winks_UserSetConnectWay(unsigned long ConnWay);

/* ��ȡ���ز���Ľ��� */
int Winks_GetDLCCProgress(Winks_EngDLCCInfo_s *pDLCCInfo);

/*ȡ��ǰ�����*/
int Winks_GetCurAccessPoint(unsigned long *accessPoint);
/*���õ�ǰ�����*/
int Winks_SetCurAccessPoint(unsigned long accessPoint);

/*��ȡ��������������*/
int Winks_GetMaxCountOfFriendList(unsigned long *count);

/* ��Ч�˿� */
#define WINKS_INVALID_PORT (0xFFFF + 1)

typedef struct {
    char ip_addr[WINKS_IP_ADDR_LEN];
    unsigned long port;
} Winks_ProxyServer_s;

#ifndef WINKS_APN_WITH_PROXY_IN_PLATFORM
/* ����HTTP��������� */
int Winks_SetProxyServer(const Winks_ProxyServer_s *proxy);
#endif/*WINKS_SUPPORT_APN_BAND_PROXY*/
/* ��ȡHTTP��������� */
int Winks_GetProxyServer(Winks_ProxyServer_s *proxy);

/*ȡ�������������ı�־. allowedFlag:0����;1������*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*�ж��Ƿ�ע���û��� regFlag:0δע�ᣬ1��ע��*/
int Winks_isUserRegistered(unsigned char *regFlag);

/* �������ṩ���ն����ӷ�ʽ */
typedef enum tag_Winks_ConnectWays_e
{
    CONNECT_WAY_LONG_CONNECT = 1 << 1,    /* ������ */
    CONNECT_WAY_LONG_ATTACH = 1 << 2,     /* ������ */
    CONNECT_WAY_NON_ATTACH = 1 << 3,      /* �Ǹ��� */
    CONNECT_WAY_SERVER_PUSH = 1 << 4,      /* ����PUSH */
    CONNECT_WAY_TIMER_WAKE_UP = 1 << 5    /* ��ʱ���� */
} Winks_ConnectWays_e;

typedef struct
{
    unsigned long operation;        /*��16λ��ÿһλ��ʾһ�ֲ�ͬ�����ӷ�ʽ����16λ��ʾĬ�ϵ����ӷ�ʽ��[operation&(operation>>16)!=0]*/
    unsigned long spTimeGiveUp; /*Push��ʽ�������������ն˵�ʱ��*/
    unsigned long twuTimeSlot;      /*��ʱ����ʱ����*/
    unsigned long twuTimeLimit;     /*��ʱ�������糬ʱʱ��*/

} Winks_ConnectWayOperation_s;

/*��ȡ��Ӫ���ṩ�����ӷ�ʽ�����ຯ��*/
int Winks_GetSupportedConnectWays(Winks_ConnectWayOperation_s *operation);

/*�����ն����ӷ�ʽ����*/
int Winks_SetPhoneConnectWay(unsigned long connect_way);

/*��ȡ�ն����ӷ�ʽ����*/
int Winks_GetPhoneConnectWay(unsigned long *connect_way);

/* ����Winks_check_storage_files() ����ֵ */
#define WINKS_STRG_CHECK_RET_0 0        /* �洢�ļ������� */
#define WINKS_STRG_CHECK_RET_1 (-1)     /* ��ʧ�ؼ��ļ�, �ؽ��ɹ�, ��ʾ�û�����ע�� */
#define WINKS_STRG_CHECK_RET_2 (-2)     /* ��ʧ�ؼ��ļ�, �ؽ�ʧ��, �޷�ʹ�ò������ */
#define WINKS_STRG_CHECK_RET_3 (-3)     /* �ؽ��ǹؼ��ļ��ɹ� */
#define WINKS_STRG_CHECK_RET_4 (-4)     /* �ؽ��ǹؼ��ļ�ʧ�� */
#define WINKS_STRG_CHECK_RET_5 (-5)     /* ����ͼƬ��ʧ, ������������ */
#define WINKS_STRG_CHECK_RET_6 (-6)     /* ����ͼƬ��ʧ, �޷��������� */

/**
*    ���һϵ�д洢�ļ��Ƿ���ڡ�
*   [flag]
*       0: ���洢�����ļ��Ͳ���ͼƬ��
        1: ֻ���洢�����ļ�, ��������ͼƬ��
    [fnc return]:
        0: ���������޸�OK,
        1: �����ļ���ʧ����Ҫ�������أ�
        3: ��Ҫ�ļ����ƻ�����Ҫ���¼���,
*/
//int Winks_check_storage_files(int flag);

#if 0//def WINKS_SUPPORT_MEMORY_CARD
int Winks_check_mc_status(void);
#endif

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
