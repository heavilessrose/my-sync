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
*Id: wk_engin.h,v 1.00 04/03/2009
**************************************************************************    
* File Description
* ----------------
* Color call ENGINE global API. 
**************************************************************************/
#ifndef _WK_ENGIN_H_
#define _WK_ENGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WINKS_MSG_ENG2UI_NONE       0       /* �޹���Ϣ */
    
typedef enum tag_WinksEng2UiMsg_SysErr_e
{
/* ������Ϣ */
    WINKS_MSG_ENG2UI_SVR_NO_ANSWER  = -127, /* ��������Ӧ�� */
    WINKS_MSG_ENG2UI_UNREG_USER     ,       /* ��ע���û����� [Winks_SrvErrCode_s]*/
    WINKS_MSG_ENG2UI_ROAMING_LIMIT  ,       /* ��������  */
    WINKS_MSG_ENG2UI_ENG_BUSY       ,       /* ����æ */
    WINKS_MSG_ENG2UI_DIAL_INT       ,       /* ���绰�ж� */
    WINKS_MSG_ENG2UI_STORAGE_LIMIT  ,       /* �洢�ռ䲻�� */
    WINKS_MSG_ENG2UI_CC_CLOSED      ,       /* ��������ر�  */
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,       /* �洢�������� */
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    WINKS_MSG_ENG2UI_SYS_FILES_ERR  ,       /* ϵͳ�ļ����ƻ� */
    WINKS_MSG_ENG2UI_CC_FILES_ERR   ,       /* �����ļ����ƻ� */
#ifdef WINKS_DUAL_SIM
    WINKS_MSG_ENG2UI_SIM_SEL_ERR    ,       /* �û�δѡ��SIM�� */
    WINKS_MSG_ENG2UI_SIM_UNUSABLE   ,       /* �û�ѡ���Sim�������� */
#endif /* WINKS_DUAL_SIM */
    WINKS_MSG_ENG2UI_NO_SUBSCRIPTION,       /* ���Ƕ����û� [Winks_SrvErrCode_s]*/
    WINKS_MSG_ENG2UI_USER_DISABLED   ,      /* �û������� [Winks_SrvErrCode_s]*/
    WINKS_MSG_ENG2UI_NO_NUMBER      ,       /* û�е绰������Ҫ���� */
    WINKS_MSG_ENG2UI_UID_CHANGE     ,       /* UID�ı䣬��Ҫ�ط�Э�� */

    WINKS_MSG_ENG2UI_PROT_NO        ,       /* �������·�Э��Ų�ƥ�� */
    WINKS_MSG_ENG2UI_HTTP_ERRCODE   ,       /* �յ�Http������ */
    WINKS_MSG_ENG2UI_SRV_ERRCODE    ,       /* �յ����������ش����� [Winks_SrvErrCode_s]*/
    WINKS_MSG_ENG2UI_SRV_ERRDATA    ,       /* ���������������д��� */
    WINKS_MSG_ENG2UI_MEM            ,       /* �ڴ�������� */
    WINKS_MSG_ENG2UI_FS             ,       /* �ļ�ϵͳ���� */
    WINKS_MSG_ENG2UI_HTTP           ,       /* HTTP���� */
    WINKS_MSG_ENG2UI_HTTP_TIMEOUT   ,       /* HTTP��ʱ���� */
    WINKS_MSG_ENG2UI_PROT_PACK      ,       /* Э���� */
    WINKS_MSG_ENG2UI_PROT_PARSE     ,       /* Э����� */
    WINKS_MSG_ENG2UI_XML_GEN        ,       /* ����xml���� */
    WINKS_MSG_ENG2UI_XML_PARSE      ,       /* xml�������� */
    WINKS_MSG_ENG2UI_APP_ERR        ,       /* Ӧ�õ��ô� */
    WINKS_MSG_ENG2UI_ENG_ERR        ,       /* ������ô� */
    
    WINKS_MSG_ENG2UI_UNKNOWN        = -80   /* δ֪���� */
}WinksEng2UiMsg_SysErr_e;

typedef enum tag_WinksEng2UiMsg_Regist_e
{
/* 1 ע�ἰ��ʼ������ */
    WINKS_MSG_ENG2UI_SETUP_SVR_FAIL = -9   /* �������÷�����ʧ�� */
    ,WINKS_MSG_ENG2UI_REG_FAIL              /* ע��ʧ�� */
    ,WINKS_MSG_ENG2UI_ULPB_FAIL             /* �ϴ��绰��ʧ�� */
    ,WINKS_MSG_ENG2UI_INIT_FAIL             /* ��ʼ��ʧ�� */
    
    ,WINKS_MSG_ENG2UI_SETUP_SVR      = 1    /* �����������÷�����... */
    ,WINKS_MSG_ENG2UI_SETUP_SVR_OK          /* �������÷������ɹ� */
    ,WINKS_MSG_ENG2UI_SMS_REG               /* δע�ᣬ��Ҫ����ע����� */
    ,WINKS_MSG_ENG2UI_WAIT_PUSH             /* δע�ᣬ�ȴ�����push */
    ,WINKS_MSG_ENG2UI_REG                   /* ����ע��... */
    ,WINKS_MSG_ENG2UI_REG_OK                /* ע��OK [ע��ɹ��ʺ���(utf8��ʽ)] */
    ,WINKS_MSG_ENG2UI_ULPB_CONFIRM          /* �û�ȷ���Ƿ��ϴ��绰�� */
    ,WINKS_MSG_ENG2UI_ULPB                  /* �����ϴ��绰��...[WinksProgress_s]*/
    ,WINKS_MSG_ENG2UI_ULPB_OK               /* �ϴ��绰����� [�ɹ���ʾ��(utf8��ʽ)]*/
    ,WINKS_MSG_ENG2UI_INIT                  /* ���ڳ�ʼ��...[WinksProgress_s] */
    ,WINKS_MSG_ENG2UI_INIT_OK               /* ��ʼ����� */
    ,WINKS_MSG_ENG2UI_QUERY_OK              /* ��ѯ�û�״̬��� */
}WinksEng2UiMsg_Regist_e;

typedef enum tag_WinksEng2UiMsg_Friend_e
{
/* 2 ���º���¼ */
    WINKS_MSG_ENG2UI_FRIEND_FAIL    = -79,  /* ���º���¼ʧ�� */
    
    WINKS_MSG_ENG2UI_FRIEND         = 71,   /* ���ڸ��º���¼...[WinksProgress_s] */
    WINKS_MSG_ENG2UI_FRIEND_OK              /* ����¼������� */
}WinksEng2UiMsg_Friend_e;

#define WINKS_RET_DIY_IMG_TOO_BIG (-100) /* ͼƬ���� */

typedef enum tag_WinksEng2UiMsg_Diy_e
{
/* 3 �ϴ�DIY */
    WINKS_MSG_ENG2UI_DIY_FAIL       = -29,  /* �ϴ�DIYʧ�� */
    WINKS_MSG_ENG2UI_SCRIPT_FAIL,           /* ���Ʋ���ʧ�� */

    WINKS_MSG_ENG2UI_DIY            = 21,   /* �����ϴ�DIY... [WinksProgress_s] */
    WINKS_MSG_ENG2UI_DIY_OK,                /* �ϴ�DIY�ɹ�[wid����id] */
    WINKS_MSG_ENG2UI_SCRIPT_OK              /* ���Ʋ���ɹ� */
}WinksEng2UiMsg_Diy_e;

typedef enum tag_WinksEng2UiMsg_Recommend_e
{
/* 4 �Ƽ������� */
    WINKS_MSG_ENG2UI_RECOMMEND_FAIL = -39,  /* �Ƽ�ʧ�� */
    WINKS_MSG_ENG2UI_COLLECT_FAIL,          /* �ղز���ʧ�� */

    WINKS_MSG_ENG2UI_RECOMMEND      = 30,   /* �����Ƽ�... [���봮] */
    WINKS_MSG_ENG2UI_RECOMMEND_OK,          /* �Ƽ��ɹ� */
    WINKS_MSG_ENG2UI_COLLECT,               /* �����ղ�... */
    WINKS_MSG_ENG2UI_COLLECT_OK             /* �ղسɹ� */
}WinksEng2UiMsg_Recommend_e;

typedef enum tag_WinksEng2UiMsg_Call_e
{
/* 5 �绰���� */
    WINKS_MSG_ENG2UI_CALL_FAIL      = -49,  /* ��ȡͨ���û�����ʧ�� */
    WINKS_MSG_ENG2UI_CALL_TIMEOUT   ,       /* ��ȡͨ���û�����ʱ */

    WINKS_MSG_ENG2UI_CALL           = 41,   /* ���ڻ�ȡͨ���û�����... */
    WINKS_MSG_ENG2UI_CALL_OK                /* ��ȡͨ���û�����ɹ� */
}WinksEng2UiMsg_Call_e;

typedef enum tag_WinksEng2UiMsg_Setup_e
{
/* 6 ͬ���������õ������� */
    WINKS_MSG_ENG2UI_SETUP_FAIL     = -59,  /* ͬ����������ʧ�� */
    WINKS_MSG_ENG2UI_SRVCFG_FAIL,           /* ����������ʧ�� */
    WINKS_MSG_ENG2UI_USERCFG_FAIL,          /* ��������ʧ�� */
    WINKS_MSG_ENG2UI_ULSTAT_FAIL,           /* ͳ����Ϣ�ϴ�ʧ�� */
    WINKS_MSG_ENG2UI_ULLOG_FAIL,            /* log��Ϣ�ϴ�ʧ�� */
    
    WINKS_MSG_ENG2UI_SETUP          = 51,   /* ����ͬ����������... */
    WINKS_MSG_ENG2UI_SETUP_OK,              /* ͬ���������óɹ� */
    WINKS_MSG_ENG2UI_SRVCFG_OK,             /* ���������óɹ� */
    WINKS_MSG_ENG2UI_USERCFG_OK,            /* �������óɹ� */
    WINKS_MSG_ENG2UI_ULSTAT_OK,             /* ͳ����Ϣ�ϴ��ɹ� */
    WINKS_MSG_ENG2UI_ULLOG_OK               /* log��Ϣ�ϴ��ɹ� */
}WinksEng2UiMsg_Setup_e;

typedef enum tag_WinksEng2UiMsg_Upgrade_e
{
/* 7 �汾���� */
    WINKS_MSG_ENG2UI_RUPGRADE_FAIL = -69,  /* ����ʧ�� */

    WINKS_MSG_ENG2UI_RUPGRADE      = 61,   /* ���ڼ���°汾... */
    WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW  ,   /* û���°汾 */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW      ,   /* ���°汾 */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW_PROGRESS,/* �°汾���ؽ�����Ϣ */
    WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK       /* �°汾���سɹ�����װ */
}WinksEng2UiMsg_Upgrade_e;

typedef struct tag_WinksProgress_s
{
    unsigned long Finished;/*�������*/
    unsigned long Total;/*����Ҫ�����*/
}WinksProgress_s;

typedef struct tag_Winks_SrvErrCode_s
{
    int     code;/*�������·��Ĵ�����*/
    char    msg[1];/* �������·�����ʾ�UTF-8���롣 */
}Winks_SrvErrCode_s;

/*Ӧ�����*/
typedef enum tag_Winks_EngProType_e
{
    WINKS_ENGPROTYPE_REGISTER = 0x00    /*����*/
    ,WINKS_ENGPROTYPE_INIT              /*��ʼ��*/
    ,WINKS_ENGPROTYPE_REGULARLY_UPDATE  /*��ʱ����*/
    ,WINKS_ENGPROTYPE_DIY               /*DIY����*/
    ,WINKS_ENGPROTYPE_RECOMMEND         /*�Ƽ�*/
    ,WINKS_ENGPROTYPE_QUREY_US          /*�û����Ͳ�ѯ*/
    ,WINKS_ENGPROTYPE_UPDATE_FRIENDS    /*���º���*/
    ,WINKS_ENGPROTYPE_UPLOAD_PB         /*�ϴ��绰��*/
    ,WINKS_ENGPROTYPE_UPDATE_SW         /*�������*/
    ,WINKS_ENGPROTYPE_CALL_UPDATE       /*�绰�¼�����*/
    ,WINKS_ENGPROTYPE_HUNGUP_UPDATE     /*�ҶϺ����*/
    ,WINKS_ENGPROTYPE_UPDATE_SC         /*���·�����������Ϣ*/
    ,WINKS_ENGPROTYPE_UPDATE_UC         /*�����û�������Ϣ*/
    ,WINKS_ENGPROTYPE_UPDATE_LS         /*���±���������*/
    ,WINKS_ENGPROTYPE_UPLOAD_LOG        /*�ϴ�log��־*/
    ,WINKS_ENGPROTYPE_UPLOAD_STA        /*�ϴ�ͳ����Ϣ*/
    ,WINKS_ENGPROTYPE_NUMBER
}Winks_EngProType_e;

/*��ϢWINKS_MSG_ENGINE_APP_RETMSG/WINKS_MSG_ENGINE_UIRETMSG�Ľṹ�����������������ص���Ϣ��.*/
typedef struct tag_Winks_EngineToUiReturnMsg_s
{
    int     code;
    int     appType;/*Winks_EngProType_e*/
    char    body[4];
}Winks_EngineToUiReturnMsg;

/*�û�DIY����*/
typedef struct tag_Winks_DiyInfo_s
{
    int     action;  /* 0 - ֻ�ϴ�������; 1 - �ϴ������� */
    char  * pnolist; /* ��actionΪ1ʱʹ�ã����ƺ��ѵ��ֻ����봮����|�ָ�����|��β��ΪNULL��ʾ������ */
    unsigned char name[40]; /* �û����õĲ�������(unicode����) */
    unsigned char path[WINKS_PATH_MAX]; /* �ļ�·��(unicode����) */
    unsigned char greet[202/*WINKS_STRG_GREET_LEN*/];   /* �ʺ���(unicode����) */
} Winks_DiyInfo_s;

typedef struct tag_Winks_RecommendInfo_s
{
    unsigned char name[32/*WINKS_STRG_NICK_LEN*/];  /* ����(unicode����) */
    char    pno[52/*WINKS_STRG_PHONE_LEN*/];        /* �绰����(ascii����) */
} Winks_RecommendInfo_s;

typedef enum tag_Winks_UrlType_e
{
    WINKS_URLTYPE_CCSHOP =0,  /* �����̳� */
    WINKS_URLTYPE_FAVORITE,   /* �ҵĲ���� */
    WINKS_URLTYPE_SCRIPT,     /* ����ҵ�� */
    WINKS_URLTYPE_PURCHASE,   /* ������� */
#ifdef WINKS_COLLECT
    WINKS_URLTYPE_COLLECT,    /* �ղز��� */
    WINKS_URLTYPE_MYCOLLECT,  /* ���ղصĲ��� */
#endif /* WINKS_COLLECT */
    WINKS_URLTYPE_INTRO,      /* �������� */
    WINKS_URLTYPE_HELP,       /* ����ҳ�� */
    WINKS_URLTYPE_ABOUT,      /* ����ҳ�� */
    WINKS_URLTYPE_SETTINGS,   /* �������� */
    WINKS_URLTYPE_DIYCUSTOMIZE,/* DIY���� */

    WINKS_URLTYPE_MAX
}Winks_UrlType_e;

/* Ƭͷ��Ӫ�̰�������Ѵ��� */
#define WINKS_DFT_CC_NO     "dft"   /* Ĭ�ϲ���ID */
#define WINKS_PRI_CC_NO     "pri"   /* Ƭͷ������ʼID */
#define WINKS_PRI_CC_MAX    1       /* Ƭͷ���������� <10 */
#define WINKS_BUS_CC_NO     "bus"   /* ��Ӫ�̲�����ʼID */
#define WINKS_BUS_CC_MAX    1       /* ��Ӫ�̲�������� <10 */

typedef struct tag_WinksStrgFile_CCShow_s
{
    int             RetStatus;                         /* �����Ƿ���ú����Ӧ
                                                          0 -- ok
                                                          1 -- Ĭ�ϲ���
                                                          2 -- Ƭͷ 
                                                          3 -- ��Ӫ�� */
    char            NO[52/*WINKS_STRG_PHONE_LEN*/];    /* �绰�����ַ��� */
    char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* �����ǳ� */
    char            ID[16/*WINKS_STRG_CC_ID_LEN*/];    /* ����ID */
    unsigned short  CC_Idx;     /* ����Idx */
    //unsigned short  interval;   /* ͼƬ���ż�� */
    unsigned char   img_type;   /* �������� WinksFileExt_e*/
    unsigned char   img_cnt;    /* ͼƬ�� ���Ϊ4 */
    unsigned char   GreetLen;   /* �ʺ��ﳤ�� */
    unsigned char   SaveType;   /* ����洢���� 0--�ļ� 1--�ڴ� */
    unsigned char * pCCImage;   /* �洢�Ĳ����ڴ����� */
    unsigned long   CCImageSize;/* �洢�Ĳ����ڴ����ݴ�С */
    char            CCImgPath[256];     /* �洢�Ĳ���·�� */
    short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* �洢�Ĳ����ʺ��� */
    int             allowCollect;    /* �Ƿ�����һ���ղصı�־��0��ֹ��1���� */
}WinksStrgFile_CCShow_s;

typedef struct tag_Winks_PeriodUpdateInfo_s
{
    int             Method;     /* ���·�ʽ: 
                                        �Զ���:       WINKS_SETVAL_CPMOD_CUSTOM
                                        ��������ģʽ: WINKS_SETVAL_CPMOD_USUALLY
                                        ��ʡ�ʷ�ģʽ: WINKS_SETVAL_CPMOD_UNUSUALLY
                                 */
    unsigned long   Period;     /* ��������(����Ϊ��λ) */
}Winks_PeriodUpdateInfo_s;

typedef enum tag_WinksGetCCType_e
{
    WINKS_GETCC_TYPE_INIT = 0,
    WINKS_GETCC_TYPE_NUMBER,
    WINKS_GETCC_TYPE_FRIEND,
    WINKS_GETCC_TYPE_CCCACHE,
    WINKS_GETCC_TYPE_PERIOD,
    WINKS_GETCC_TYPE_MAX
}WinksGetCCType_e;

#define PU_SAVEMONEY_DFT_PERIOD     (24*60)     /* 1�� */
#define PU_DATANEW_DFT_PERIOD       (2*60)      /* 2Сʱ */


/*�����ʼ��*/
int Winks_EngineInit(void);
/*����ע��*/
int Winks_EngineDestroy(void);

#ifdef WINKS_DUAL_SIM
/* �����ſ�ʱ,�û�ѡ�����ſ����� */
int Winks_UserSetActiveSim(int SimNo);
#endif /* WINKS_DUAL_SIM */


/*-------------------------
        ��������UI�ӿ�
---------------------------*/
/*��������Ӧ�û��ơ�
 * function: Winks_EngAppDestroy
 * parameters:
 *   
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_EngAppDestroy(void);
typedef enum tag_Winks_EngineAppPri_e
{
    WINKS_ENGINE_APP_PRI_NORMAL = 0x00,     /* ����������:��ʱ���£��Զ�������*/
    WINKS_ENGINE_APP_PRI_USER = 0x01,       /* �û��������û������Ĳ������缤�DIY���ֶ����µ�*/
    WINKS_ENGINE_APP_PRI_CALL,              /* �绰���� �����紥��������������*/
    WINKS_ENGINE_APP_PRI_NUM                /* ���ȼ�����*/
}Winks_EngineAppPri_e;
/*�˳�����ָ�����ȼ�(��)������Ӧ�á�
 * function: Winks_EngAppStopUserPri
 * parameters:
 *   Winks_EngineAppPri_e pri: ָ�������ȼ��� 
 *   
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_EngAppStopByPri(Winks_EngineAppPri_e pri);

/*�û�ע�����ҵ�񣬹�UIʹ�á�����ֵ:0�ɹ�,-1ʧ�ܡ�*/
int Winks_UserRegist(void);/*���첽���ء�*/

/*������ȡ�û�����״̬�����������غ������Winks_UserGetState��ȡ״̬��*/
int Winks_UserCheckState(void);/*���첽���ء�*/

/*��ȡ�����û�����״̬������Winks_UserCheckState�����±���״̬.
    state 5:�����ʼ��ʧ��
    state 4:�����ʼ����
    state 3:�����û�
    state 2:�˶��û�
    state 1:�Ѽ����û�
    state 0:δ�����û�
*/
int Winks_UserGetState(int *state);

/*�˳��������̡���������ѯ�û�״̬����*/
int Winks_UserRegistStop(void);

/*�û�ȷ��ע�ᣬ����ע�����Ϣ,0:ȡ��ע�ᣬ1:����ע��*/
int Winks_UserSendRegSms(int reg_flag);/*���첽���ء�*/

/*�û�ȷ���Ƿ��͵绰��������ֵ:0�ɹ� ����ʧ�ܡ�
  ��ע������Ҳ���Ե��øú����ϴ��绰��
ulpb_flag: 0ȡ���ϴ���1�ϴ��绰��*/
int Winks_UserUploadPhoneBook(int ulpb_flag);/*���첽���ء�*/

/*��������汾��0:����Ƿ����°汾��������°汾��ֻ��ʾ�û����������°汾, 
 * 1: �ȼ���Ƿ����°汾��������Զ������°汾��û���°汾֪ͨ�û��Ѿ������°汾*/
int Winks_UpgradeStart(int type);/*���첽���ء�*/

/*�û���Ӻ��ѻ��޸ĺ����ǳƣ��ǳ�Ϊunicode���룩*/
int Winks_StrgFriendAdd(char *pNo, unsigned char *pNick);
/* ��ȡ���Ѹ��� */
int Winks_StrgFriendCnt(void);
/* ��ȡ������Ϣ��BeginIdx��0��ʼ��������ֵ��ΪBeginIdx�������ã�ֱ������Ϊ��ֵ����ȡȫ������ */
int Winks_StrgFriendGet( int BeginIdx, 
                               char *pNo, int NoLen,
                               unsigned char *pNick, int NickLen);
/*�û�ɾ������*/
int Winks_StrgFriendDel(char *pNo);
/*�û�ɾ��ȫ������*/
int Winks_StrgFriendDelAll(void);
/*�û��Ƿ������ܺ���*/
int Winks_StrgUserIsFriend(char *pNo, unsigned short *retFriendIdx);
/*��ȡ�û�������Ϣ*/
int Winks_StrgGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow);
/*�ж��û��Ƿ��в���*/
int Winks_StrgUserHasCC(char *pNo);
#ifdef WINKS_COLLECT
/*�ղز���*/
int Winks_UserAddCollect(char *pWid);
#endif /* WINKS_COLLECT */
/* �û���ȡ���� 
   GetCCType -- WinksGetCCType_e 
   pNoList -- ��GetCCType == WINKS_GETCC_TYPE_NUMBER ʱ��Ҫ����'|'�ָ�ĵ绰���봮 
˵��:
    Winks_UserGetCC(WINKS_GETCC_TYPE_INIT, NULL) ����Ƭͷ��Ӫ�̲���
    Winks_UserGetCC(WINKS_GETCC_TYPE_NUMBER, pNoList) �����Զ�����봮�Ĳ���
    Winks_UserGetCC(WINKS_GETCC_TYPE_FRIEND, NULL) ���º��Ѳ���
    Winks_UserGetCC(WINKS_GETCC_TYPE_CCCACHE, NULL) ��������绰�¼���ϵ�˲���
    Winks_UserGetCC(WINKS_GETCC_TYPE_PERIOD, NULL) �붨ʱ���»�ȡ������ͬ
*/
int Winks_UserGetCC(int GetCCType, char *pNoList);/*���첽���ء�*/
/* �û�ֹͣ��ȡ���� */
int Winks_UserGetCCStop(int GetCCType);
#if 0
/* ֱ�ӻ�ȡ�û����� */
int Winks_UserGetCCDirectly(char * pNo);
/*�û����º���¼*/
int Winks_UserResetFriendList(void);
int Winks_UserUpdateFriendListStop(void);
/*�û�����cash�û�����*/
int Winks_UserResetCacheFriendList(void);
int Winks_UserUpdateCacheFriendListStop(void);
#endif
/*�û��ϴ�DIY�����Ƹ�����
  diy_info->action: 0 - ֻ�ϴ�������; 1 - �ϴ�������
  diy_info->pnolist: ��actionΪ1ʱʹ�ã����ƺ��ѵ��ֻ����봮����|�ָ���ΪNULL��ʾ������ 
  diy_info->name: �û����õĲ������ƣ�unicode���룬ǰ2�ֽ�Ϊ0��ȡ�ļ���
  diy_info->greet�ַ���Ϊ��(Unicodeǰ2�ֽ�Ϊ0) : �������ʺ���
  diy_info->path�ַ���Ϊ��(Unicodeǰ2�ֽ�Ϊ0) : ֻ�޸��ʺ���
*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info);/*���첽���ء�*/
int Winks_UserUploadDiyStop(void);
/*�ϴ����������*/
int Winks_UserUploadSetting(void);/*���첽���ء�*/
/*����������*/
int Winks_UserDownLoadSetting(void);/*���첽���ء�*/
/*ֹͣ���ػ��ϴ�������*/
int Winks_SynsetStop(void);

/* �����Ƽ����������RcmdArr���������ΪRcmdNum��
   RcmdGreet--�����ʺ���(Unicode����) */
int Winks_UserRecommend(int RcmdNum, Winks_RecommendInfo_s * RcmdArr, 
                                  unsigned char * RcmdGreet);/*���첽���ء�*/
int Winks_UserRecommendStop(void);

/* retBuff -- ���ص��ڴ滺���� 
   retLen -- �����ڴ滺������С�����ʵ��ʹ�õ��ڴ滺������С
   IsDft -- �Ƿ��ȡĬ�ϵ�����
*/
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft);

/*��ȡͨ������Ĳ���*/
int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow);
/*��ȡ�����Ԥ������*/
int Winks_UserGetCCPreview(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow);
/*ֹͣչʾ����*/
int Winks_UserStopCCShow(void);

#define WINKS_CCSHOW_CID_MAX_LEN 20
#define WINKS_CCSHOW_TID_MAX_LEN 60

/* ͨ���绰�����ȡ����չʾģ�塣
 * function: Winks_GetShowTemplate
 * parameters:
 *   char* number (in)�绰���룬����ΪNULL��
 *          ��ͨ�����������꣨WINKS_DFT_CC_NO��WINKS_PRI_CC_NO��WINKS_BUS_CC_NO�����ﵽ�ض���Ч����
 *   char* fileName (out)����ģ���ŵ��ļ�·����
 *   unsigned long fileSize (in)fileName��size��
 *   int *allowCollectFlag (out)�����Ƿ������ղصı�־��
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetShowTemplate(char* number, char* fileName, unsigned long fileSize, int *allowCollectFlag);

/* ��ȡָ��ģ����ƶ�Ԫ��
 * function: Winks_GetShowElement
 * parameters:
 *   char *number (in)�绰���룬����ΪNULL��
 *          ��ͨ������꣨WINKS_DFT_CC_NO��WINKS_PRI_CC_NO��WINKS_BUS_CC_NO�����ﵽ�ض���Ч����
 *   unsigned long extDataFlag (in)�ⲿ���ݱ�ʶ��
 *          ΪWINKS_TRUE��ʾ�ⲿ����(pageshow�е�����)��ΪWINKS_FALSE��ʾ�ڲ�����(template��multi-part����)��
 *   char *eTag (in)����ȡԪ�صı�ʶ������ΪNULL����Ӧtempate�е�CID.
 *   unsigned long *fileFlag (out)������ʶ���������Ƿ����ļ�����
 *          ΪWINKS_TRUE��ʾeBuff�з��ص����ļ�������ȡ���ݴ��ڸ��ļ��У�
 *          ΪWINKS_FALSE��ʾeBuff�д�ŵ������ݿ���ֱ��ʹ�á�
 *   char * eDataBuf (out)�������ݡ�
 *   unsigned long *eDataBufSize (in, out)����eDataBufSize��size���������buffSize����ʱ��������Ҫ��size��
 *          �û�Ӧ�����㹻������ռ䣬���µ��øú�����ȡ���ݡ�
 *   char * tIdBuf (out)����TID���������Ҫ��ȡ�ò��������Դ���NULL��
 *   unsigned long tIdBufSize (in)����tIdBuf��size��
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_GetShowElement(char *number, unsigned long extDataFlag, char *eTag, unsigned long *fileFlag, 
    char * eDataBuf, unsigned long* eDataBufSize, char *tIdBuf, unsigned long tIdBufSize);

/*���ÿ�������ʾ�û�ע�ᡣ*/
int Winks_UserRememberImsi(void);

/* �رղ��� */
int Winks_UserCloseWinkCC(void);
/* �������� */
int Winks_UserOpenWinkCC(void);

/*ȡ�������������ı�־. allowedFlag:0����;1������*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

/*�ж��Ƿ�ע���û��� regFlag:0δע�ᣬ1��ע��*/
int Winks_isUserRegistered(unsigned char *regFlag);
/*�ж��Ƿ��Ĭ�û��� SilenceFlag:0�ǳ�Ĭ��1��Ĭ�û�*/
int Winks_isUserSilenced(unsigned char *SilenceFlag);
/* �ж��Ƿ����°汾������ֵ: 0 -- û�У�1 -- �� */
int Winks_HasNewVersion(void);

/*����UrlType,ȡָ��WAP��URL.*/
int Winks_GetWapUrl(int UrlType, char *wid, char *sUrl, unsigned long sUrlSize);
/* �û����ӷ�ʽ����:Push��ʽ��ʱ���� */
//int Winks_UserSetConnectWay(unsigned long ConnWay);

/*
��Wap��ҳ: �����̳�/�ҵĲ����/����ҵ��/�������
����:
    UrlType -- Winks_UrlType_e
    wid -- ��UrlTypeΪWINKS_URLTYPE_PURCHASEʱ������д�ò���������ΪNULL
*/
int Winks_UserOpenWapPage(int UrlType, char * wid);

/*����ͨ������,�Ӳ�ѡ��ĳЩ�ѡʱ����һ�ζ�ʱ���¡���֮��ѡ����ѡҪֹͣ��ʱ���¡�
 * function: Winks_SetShowCaseProc
 * parameters:
 *   unsigned long val: ͨ�����ã�WINKS_SETVAL_CCSHOW_RING|WINKS_SETVAL_CCSHOW_TALK|WINKS_SETVAL_CCSHOW_HANGUP
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetShowCaseProc(unsigned long val);

/* ��ȡ��ʱ���²��� */
int Winks_GetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo);
/* ���ö�ʱ���²��� */
int Winks_SetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo);

/*****************************************************************************
DESCRIPTION:
    check file
INPUT:
    CreateFlag  : 0 -- only check ; 1 -- if not exist, create the file
    Disk        : phone [1] �� memory card[2] ��phone and card[3]
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgCheckDiskFiles(int CreateFlag, int Disk);

/* ��¼�˴β���չʾ��������ظ�ֵ��ʾʧ�� */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);
int Winks_if_show_cc(void); //panqj
int Winks_if_show_titleCC(void);//panqj
/* ��ȡ�������·����ʺ���(Unicode����) 
����:
    pUniRecom   : �洢Unicode������Ƽ���Ļ���
    *retlen     : ���뻺���С
���:
    *retlen     : ���ʵ��ʹ�û����С
����:
    �ɹ�����0��ʧ�ܷ���-1��
    �������Ĳ���pUniRecomΪNULL��*retlenΪ0�򷵻�ʧ����*retlen����ʵ����Ҫ�ĳ���
*/
int Winks_StrgGetRecommendGreet(unsigned char *pUniRecom, int *retlen);

/* ����������·����ʺ���(Unicode����) */
int Winks_StrgSetRecommendGreet(unsigned char *pUniRecom);

/* ���ʮ����IP��ʽ��麯����
 * function: Winks_CheckIpStr
 * parameters:
 *   char *str: �����IP�ַ���(ascii).
 * return:
 *   ��ʽ��ȷ����:0
 *   ��ʽ���󷵻�:<0
 */
int Winks_CheckIpStr(char *str);

/*�ָ�Ĭ�����á�
 * function: Winks_SetSettingDefault
 * parameters:
 *   
 * return:
 *   �ɹ�����:0
 *   ʧ�ܷ���:-1
 */
int Winks_SetSettingDefault(void);

/*����ǿ�������ı������ݡ�����symbianƽ̨�� */
int Winks_SetForceReloadData(void);

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
