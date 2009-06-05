/*
File:       wk_enfnc.c
Author:     Huaxm
Create Time:17/03/2008
*/
#ifndef _WK_PUBLIC_H_
#include "wk_public.h"
#endif

#ifndef _WK_OSAL_H_
#include "wk_osal.h"
#endif

#ifndef _WK_HTTP_H_
#include "wk_http.h"
#endif

#include "wk_storage_pub.h"
#include "wk_storage_4eng.h"

#include "wk_strg_if.h"

#ifndef _WK_ENFNC_H_
#include "wk_enfnc.h"
#endif

#ifdef WINKS_DEBUG
#define WINKS_ENGINE_RUN_BY_STEP
#endif

#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
static Winks_EngineContext Winks_engineContext;
#endif

const unsigned int s_do_while = 0; /* for temp */

/*static Winks_EngineContext *Winks_engineContext_Test = NULL;*/

#ifdef HUAXM_TEST/*Just for code reading.*/
static Winks_EngineContext *Winks_engineContext_p = NULL; 
#endif

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_ENGINE_CONTEXT_DEF()  \
    Winks_EngineContext *Winks_engineContext_p = NULL;
#define WINKS_ENGINE_CONTEXT_INIT() \
    Winks_engineContext_p = (Winks_EngineContext *)get_module_glob(WK_MODULE_ENGINE); \
    Winks_assert(Winks_engineContext_p != NULL);
#else/*WINKS_UNSUPPORTED_GLOBAL_VAR*/
#define WINKS_ENGINE_CONTEXT_DEF()  \
    Winks_EngineContext *Winks_engineContext_p = NULL; 
#define WINKS_ENGINE_CONTEXT_INIT() \
    Winks_engineContext_p = &Winks_engineContext; 
#endif/*WINKS_UNSUPPORTED_GLOBAL_VAR*/


static void winks_EngSelAllFriendList(void);


Winks_Setting_s *get_engine_glob1(void)
{
    WINKS_ENGINE_CONTEXT_DEF()
    WINKS_ENGINE_CONTEXT_INIT()
    return &(Winks_engineContext_p->setting);
}
#if 0
Winks_FriendList_in_RAM_s *get_engine_glob2(void)
{
    WINKS_ENGINE_CONTEXT_DEF()
    WINKS_ENGINE_CONTEXT_INIT()
    return &(Winks_engineContext_p->friendlist_ram);
}
#endif
void * get_strg_filemap(void)
{
    WINKS_ENGINE_CONTEXT_DEF()
    WINKS_ENGINE_CONTEXT_INIT()
    return (void *)&Winks_engineContext_p->StrgFileMap;
}

#define IS_WINKS_CC_CLOSE() (Winks_engineContext_p->setting.show_ctrl == E_CC_SHOW_OFF)

//#define TEST_HTTP   "http://www.msn.com.cn"    /* ����HttpЭ�� */

#define Winks_EngErr(info)  Winks_printf("ERROR: %4d : %s [IN] %s\r\n", __LINE__, info, __FILE__)
#ifdef DBG_UNSUPPORT_PROTOCOL   /* renzhan add for debug */
char *DbgHttpBuf = NULL;

#define PROTOCOL_PATH_7FEC  WINKS_APP_PATH"FFEC.bin"
#define PROTOCOL_PATH_7FF0  WINKS_APP_PATH"FFF0.bin"
#define PROTOCOL_PATH_7FEB  WINKS_APP_PATH"FFEB.bin"
#define PROTOCOL_PATH_7FEF  WINKS_APP_PATH"FFEF.bin"
#define PROTOCOL_PATH_7FED  WINKS_APP_PATH"FFED.bin"
#define PROTOCOL_PATH_7FEB_1  WINKS_APP_PATH"FFEB_1.bin"
#define PROTOCOL_PATH_7FED_1  WINKS_APP_PATH"FFED_1.bin"
#define PROTOCOL_PATH_7FED_2  WINKS_APP_PATH"FFED_2.bin"
#define PROTOCOL_PATH_7FED_3  WINKS_APP_PATH"FFED_3.bin"
#define PROTOCOL_PATH_5006  WINKS_APP_PATH"5006.bin"
#define PROTOCOL_PATH_2002  WINKS_APP_PATH"2002.bin"

static const char * ProtocolPath[] = {
    PROTOCOL_PATH_7FEC,
    PROTOCOL_PATH_7FF0,
    PROTOCOL_PATH_7FEB,
    PROTOCOL_PATH_7FEF,
    PROTOCOL_PATH_7FED,
    PROTOCOL_PATH_7FEB_1,   /*���º���¼*/
    PROTOCOL_PATH_7FED_1,   /*������Ӫ�̲���*/
    PROTOCOL_PATH_7FED_2,
    PROTOCOL_PATH_7FED_3,   /*���º��Ѳ���*/
    PROTOCOL_PATH_5006,     /*δע��*/
    PROTOCOL_PATH_2002,     /*δע��*/
};

#define PROTOCOL_NUM    (sizeof(ProtocolPath)/sizeof(ProtocolPath[0]))

static unsigned long readFile2Buf(char *pFilePath){
    FILE * fp;
    unsigned long FileSize;

    Winks_assert(pFilePath!=NULL);
    fp = Winks_fopen(pFilePath, "rb");
    Winks_assert(fp!=NULL);
    
    Winks_fseek(fp, 0, WINKS_SEEK_END);
    FileSize = Winks_ftell(fp);
    Winks_assert(FileSize);
    Winks_fseek(fp, 0, WINKS_SEEK_SET);
    DbgHttpBuf = (char *)Winks_salAlloc(FileSize);
    Winks_assert(DbgHttpBuf!=NULL);

    Winks_fread(DbgHttpBuf, FileSize, 1, fp);
    Winks_fclose(fp);

    return FileSize;
}

static void FreeDbgHttpBuf(){
    if(DbgHttpBuf)
        Winks_salFree(DbgHttpBuf);
    DbgHttpBuf = NULL;
}

static int DbgUnsupportProtocol(unsigned short protocol)
{
    unsigned long retCode;
    unsigned long BufLen;
    char *pDbgFilePath = NULL;
    int http = 0;
static int Flag_7FEB = 0;
static int Flag_7FED = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
Winks_printf("<<<<<<<<<<<<<<<<<<<< DbgUnsupportProtocol %04x >>>>>>>>>>>>>>>>>>>>>>>>>>\r\n",protocol);
    switch(protocol){
    case 0x7FEC:
        http = 0;
        break;
    case 0x7FF0:
        http = 1;
        break;
    case 0x7FEB:
        //http = 2;
        if(!Flag_7FEB){
        http = 2;
            Flag_7FEB = 1;
        }
        else{
            http = 5;
            Flag_7FEB = 0;
        }
        break;
    case 0x7FEF:
      #if 0
        http = 3;
      #else
        http = 10;/* ͨѶ�������ڲ����� */
      #endif
        break;
    case 0x7FED:
        if(!Flag_7FED){
        http = 4;
            Flag_7FED = 1;
        }
     #if 0
        else if(Flag_7FED == 1){
            http = 6;
            Flag_7FED = 2;
        }
        else if(Flag_7FED == 2){
            http = 7;
            Flag_7FED = 3;
        }
        else /*if(Flag_7FED == 3)*/{
            http = 8;
            Flag_7FED = 0;
        }
    #else
        else{
            http = 6;
            Flag_7FED = 0;
        }
    #endif
        http = 9;
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    Winks_assert(http>=0 && http<PROTOCOL_NUM);
    pDbgFilePath = (char *)ProtocolPath[http];
    
    BufLen = readFile2Buf(pDbgFilePath);
    
    winks_EngDataStreamAnalyze(http, BufLen,&retCode);
    Winks_engineContext_p->ProtocolMsgType = 0;
    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_RETMSG, &retCode, sizeof(retCode));

    FreeDbgHttpBuf();

    return WINKS_RETURN_SUCCESS;
}

#define PROTOCOL_PATH_PB  WINKS_APP_PATH"PB.txt"

static int ReadLocalPB(char *pPB, int Size)
{
    FILE * fp;
    char * pFilePath = PROTOCOL_PATH_PB;
    int FileSize;

    Winks_assert(pFilePath!=NULL);
    Winks_assert(pPB!=NULL);
    Winks_assert(Size);
    
    fp = Winks_fopen(pFilePath, "rb");
    Winks_assert(fp!=NULL);
    
    Winks_fseek(fp, 0, WINKS_SEEK_END);
    FileSize = Winks_ftell(fp);
    Winks_assert(FileSize);
    Winks_fseek(fp, 0, WINKS_SEEK_SET);

    FileSize = (FileSize<Size)?FileSize:Size;
    Winks_fread(pPB, FileSize, 1, fp);
    Winks_fclose(fp);
    
    return FileSize;
}
#endif

#ifdef WINKS_DEBUG
typedef struct tagWinks_Eng2UIMsgInfo_s
{
    int Msg;
    char  ID_Info[36];
    char  StrInfo[128];
}Winks_Eng2UIMsgInfo_s;

const Winks_Eng2UIMsgInfo_s Eng2UiMsgInfo[] = 
{                                       
    {WINKS_MSG_ENG2UI_NONE           , "WINKS_MSG_ENG2UI_NONE         ",  " XX �޹���Ϣ "},
        
/* �ɹ���������Ϣ */
    
/* 1 ע�ἰ��ʼ������ */
    {WINKS_MSG_ENG2UI_SETUP_SVR_OK   , "WINKS_MSG_ENG2UI_SETUP_SVR_OK ",  " XX �������÷������ɹ� "},
    {WINKS_MSG_ENG2UI_SMS_REG        , "WINKS_MSG_ENG2UI_SMS_REG      ",  " δע�ᣬ��Ҫ����ע�����     [Winks_UserSendRegSms] "},
    {WINKS_MSG_ENG2UI_SMS_REG_OK     , "WINKS_MSG_ENG2UI_SMS_REG_OK   ",  " ����ע��OK"},
    {WINKS_MSG_ENG2UI_INIT           , "WINKS_MSG_ENG2UI_INIT         ",  " XX ���ڳ�ʼ��... "},
    {WINKS_MSG_ENG2UI_INIT_OK        , "WINKS_MSG_ENG2UI_INIT_OK      ",  " ��ʼ�����"},

/* 2 ���º���¼ */
    {WINKS_MSG_ENG2UI_FL_OK          , "WINKS_MSG_ENG2UI_FL_OK        ",  " XX �����б�������� "},
    {WINKS_MSG_ENG2UI_FL_NOTIFY      , "WINKS_MSG_ENG2UI_FL_NOTIFY    ",  " ֪ͨ�û��к��Ѹ���"},
    {WINKS_MSG_ENG2UI_FL_SEL_FRIEND  , "WINKS_MSG_ENG2UI_FL_SEL_FRIEND",  " �����б�������ϣ�ѡ�����¼ [Winks_UserSetFriendList] "},
    {WINKS_MSG_ENG2UI_FRIEND_SET     , "WINKS_MSG_ENG2UI_FRIEND_SET   ",  " XX �ϴ�����¼�ɹ� "},
    {WINKS_MSG_ENG2UI_FRIEND_UPD     , "WINKS_MSG_ENG2UI_FRIEND_UPD   ",  " XX �ϴ�����¼�����غ������� "},
    {WINKS_MSG_ENG2UI_DL_FRIENDCC    , "WINKS_MSG_ENG2UI_DL_FRIENDCC  ",  " �������غ��Ѳ���... "},
    {WINKS_MSG_ENG2UI_FRIEND_OK      , "WINKS_MSG_ENG2UI_FRIEND_OK    ",  " ����¼�������"},
#if 0//def WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_FL_SEL_MCARD   , "WINKS_MSG_ENG2UI_FL_SEL_MCARD ",  " �����б�������ϣ�ѡ��洢�� [Winks_UserSelMCard] "},
#endif /* WINKS_SUPPORT_MEMORY_CARD */

/* 3 ͬ���������õ������� */
    {WINKS_MSG_ENG2UI_SETUP_OK       , "WINKS_MSG_ENG2UI_SETUP_OK     ",  " ͬ���������óɹ�"},

/* 4 �ϴ�DIY */
    {WINKS_MSG_ENG2UI_DIY            , "WINKS_MSG_ENG2UI_DIY          ",  " XX �����ϴ�DIY... "},
    {WINKS_MSG_ENG2UI_DIY_OK         , "WINKS_MSG_ENG2UI_DIY_OK       ",  " �ϴ�DIY�ɹ�"},

/* 5 ���¼����ѯ������ע��״̬ */
    {WINKS_MSG_ENG2UI_REREG_OK       , "WINKS_MSG_ENG2UI_REREG_OK     ",  " ���¼���ɹ�"},

/* 6 �绰���� */
    {WINKS_MSG_ENG2UI_CALLCC_GRT_OK  , "WINKS_MSG_ENG2UI_CALLCC_GRT_OK",  " XXͨ����ȡ�û��ʺ���ɹ�"},
    {WINKS_MSG_ENG2UI_CALLCC_OK      , "WINKS_MSG_ENG2UI_CALLCC_OK    ",  " ͨ����ȡ�û�����ɹ�"},
    {WINKS_MSG_ENG2UI_CALLCC_OTHER   , "WINKS_MSG_ENG2UI_CALLCC_OTHER ",  " XXͨ����ȡƬͷ��Ӫ�̲���"},

/* 7 ��̨���� */
    {WINKS_MSG_ENG2UI_PUSH           , "WINKS_MSG_ENG2UI_PUSH         ",  " XX ��̨���ڶ�ʱ���·���... "},
    {WINKS_MSG_ENG2UI_PUSH_FRIEND    , "WINKS_MSG_ENG2UI_PUSH_FRIEND  ",  " XX ��̨���ڸ��º���... "},
    {WINKS_MSG_ENG2UI_PUSH_OK        , "WINKS_MSG_ENG2UI_PUSH_OK      ",  " XX ��̨��ʱ���·������ "},
    {WINKS_MSG_ENG2UI_PUSH_CHECK_FL  , "WINKS_MSG_ENG2UI_PUSH_CHECK_FL",  " XX ��̨��ʱ���·������,����Ҫ�������б� "},

    {WINKS_MSG_ENG2UI_INITCACHE_FL   , "WINKS_MSG_ENG2UI_INITCACHE_FL ",  " XX ��̨�������غ����б��Ա��ʼ��Cache... "},
    {WINKS_MSG_ENG2UI_INITCACHE      , "WINKS_MSG_ENG2UI_INITCACHE    ",  " XX ��̨���ڳ�ʼ��Cache... "},
    {WINKS_MSG_ENG2UI_INITCACHE_OK   , "WINKS_MSG_ENG2UI_INITCACHE_OK ",  " XX ��̨��ʼ��Cache��� "},

    {WINKS_MSG_ENG2UI_RECACHE_TRIM   , "WINKS_MSG_ENG2UI_RECACHE_TRIM ",  " XX ��̨�������ػ�����Ϣ���ؽ�Cache... "},
    {WINKS_MSG_ENG2UI_RECACHE        , "WINKS_MSG_ENG2UI_RECACHE      ",  " XX ��̨�����ؽ�Cache... "},
    {WINKS_MSG_ENG2UI_RECACHE_OK     , "WINKS_MSG_ENG2UI_RECACHE_OK   ",  " XX ��̨�ؽ�Cache��� "},

    {WINKS_MSG_ENG2UI_RECRUIT_DL     , "WINKS_MSG_ENG2UI_RECRUIT_DL   ",  " XX ��̨���ڲ������ص绰����... "},
    {WINKS_MSG_ENG2UI_RECRUIT_DL_OK  , "WINKS_MSG_ENG2UI_RECRUIT_DL_OK",  " XX ��̨�������ص绰������� "},

/* ������Ϣ */

/* 1 ע�ἰ��ʼ������ */
    {WINKS_MSG_ENG2UI_REG_FAIL       , "WINKS_MSG_ENG2UI_REG_FAIL     ",  " ע��ʧ��"},
    {WINKS_MSG_ENG2UI_INIT_FAIL      , "WINKS_MSG_ENG2UI_INIT_FAIL    ",  " ��ʼ��ʧ��"},

/* 2 ���º���¼ */
    {WINKS_MSG_ENG2UI_FL_FAIL        , "WINKS_MSG_ENG2UI_FL_FAIL      ",  " ���º���¼ʧ��"},

/* 3 ͬ���������õ������� */
    {WINKS_MSG_ENG2UI_SETUP_FAIL     , "WINKS_MSG_ENG2UI_SETUP_FAIL   ",  " ͬ����������ʧ��"},

/* 4 �ϴ�DIY */
    {WINKS_MSG_ENG2UI_DIY_FAIL       , "WINKS_MSG_ENG2UI_DIY_FAIL     ",  " �ϴ�DIYʧ��"},

/* 5 ���¼����ѯ������ע��״̬ */
    {WINKS_MSG_ENG2UI_REREG_FAIL     , "WINKS_MSG_ENG2UI_REREG_FAIL   ",  " ���¼���ʧ��"},

/* 6 �绰���� */
    {WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL, "WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL ", " ͨ���ʺ�������ʧ��"},
    {WINKS_MSG_ENG2UI_CALLCC_FAIL    , "WINKS_MSG_ENG2UI_CALLCC_FAIL     ", " ͨ���û���������ʧ��"},
    {WINKS_MSG_ENG2UI_CALLCC_TIMEOUT , "WINKS_MSG_ENG2UI_CALLCC_TIMEOUT  ", " �ͨ��û��������س�ʱ"},
    {WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL,"WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL", " XXͨ����ȡƬͷ��Ӫ�̲���ʧ��"},

/* 7 ��̨���� */
    {WINKS_MSG_ENG2UI_PUSH_FAIL        , "WINKS_MSG_ENG2UI_PUSH_FAIL        ",  " XX ��̨��ʱ����ʧ�� "},
    {WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL, "WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL",  " XX ��̨���غ����б�ʧ�� "},
    {WINKS_MSG_ENG2UI_INITCACHE_FAIL   , "WINKS_MSG_ENG2UI_INITCACHE_FAIL   ",  " XX ��̨��ʼ��Cacheʧ�� "},

    {WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL, "WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL",  " XX ��̨���ػ�����Ϣʧ�� "},
    {WINKS_MSG_ENG2UI_RECACHE_FAIL     , "WINKS_MSG_ENG2UI_RECACHE_FAIL     ",  " XX ��̨�ؽ�Cacheʧ�� "},

    {WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL  , "WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL  ",  " XX ��̨�������ص绰����ʧ�� "},

/* ϵͳ���� */
    {WINKS_MSG_ENG2UI_SVR_NO_ANSWER  , "WINKS_MSG_ENG2UI_SVR_NO_ANSWER",  " ��������Ӧ��"},
    {WINKS_MSG_ENG2UI_UNREG_USER     , "WINKS_MSG_ENG2UI_UNREG_USER   ",  " ��ע���û�"},
    {WINKS_MSG_ENG2UI_ROAMING_LIMIT  , "WINKS_MSG_ENG2UI_ROAMING_LIMIT",  " ��������"},
    {WINKS_MSG_ENG2UI_ENG_BUSY       , "WINKS_MSG_ENG2UI_ENG_BUSY     ",  " ����æ,�Ժ�����"},
    {WINKS_MSG_ENG2UI_DIAL_INT       , "WINKS_MSG_ENG2UI_DIAL_INT     ",  " ���汻�绰�ж�"},
    {WINKS_MSG_ENG2UI_STORAGE_LIMIT  , "WINKS_MSG_ENG2UI_STORAGE_LIMIT",  " �洢�ռ䲻��"},
    {WINKS_MSG_ENG2UI_CC_CLOSED      , "WINKS_MSG_ENG2UI_CC_CLOSED    ",  " ��������ر�"},
#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,"WINKS_MSG_ENG2UI_MCARD_NOT_EXIST", " �洢��������"},
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    {WINKS_MSG_ENG2UI_SYS_FILES_ERR  , "WINKS_MSG_ENG2UI_SYS_FILES_ERR",  " ϵͳ�ļ����ƻ�"},
//#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_CC_FILES_ERR   , "WINKS_MSG_ENG2UI_CC_FILES_ERR ",  " �����ļ�����"},
//#endif /* WINKS_SUPPORT_MEMORY_CARD */
#ifdef WINKS_DUAL_SIM
    {WINKS_MSG_ENG2UI_SIM_SEL_ERR    , "WINKS_MSG_ENG2UI_SIM_SEL_ERR  ",  " ��ʾ�û�ѡ��Ҫ�����Sim��"},
    {WINKS_MSG_ENG2UI_SIM_UNUSABLE   , "WINKS_MSG_ENG2UI_SIM_UNUSABLE ",  " �û�ѡ���Sim��������"},
#endif /* WINKS_DUAL_SIM */
};

#define ENG2UI_MSG_NUM    (sizeof(Eng2UiMsgInfo)/sizeof(Eng2UiMsgInfo[0]))

static void winks_EngGetProgress(Winks_EngineToUiReturnMsg *pEng2UIMsg,
                              unsigned short *retFin, unsigned short *retTotal)
{
    switch(pEng2UIMsg->err_code)
    {
    case WINKS_MSG_ENG2UI_INIT:
    case WINKS_MSG_ENG2UI_DL_FRIENDCC:
    case WINKS_MSG_ENG2UI_CALLCC_OTHER:
    case WINKS_MSG_ENG2UI_PUSH:
    case WINKS_MSG_ENG2UI_PUSH_FRIEND:
    case WINKS_MSG_ENG2UI_INITCACHE:
    case WINKS_MSG_ENG2UI_RECACHE_TRIM:
    case WINKS_MSG_ENG2UI_RECACHE:
    {
        Winks_EngDLCCInfo_s *pProgress = (Winks_EngDLCCInfo_s *)pEng2UIMsg->msg_body;
    
        *retFin = pProgress->Count;
        *retTotal = pProgress->Total;
    }
        break;
    case WINKS_MSG_ENG2UI_DIY:
    {
        Winks_EngDiyProgInfo_s *pDiyProg = (Winks_EngDiyProgInfo_s *)pEng2UIMsg->msg_body;
        
        *retFin = pDiyProg->subIndex;
        *retTotal = pDiyProg->subTotalCount;
    }
		break;
    //case WINKS_MSG_ENG2UI_RECRUIT_DL:
    //    break;
    default:
        *retFin = *retTotal = 0;
        break;
    }
}

void winks_PrintEng2UiMsgInfo(Winks_EngineToUiReturnMsg *pEng2UIMsg)
{
    int iCnt;
    unsigned short Fin, Total;
    int Msg = pEng2UIMsg->err_code;

    for(iCnt=0; iCnt<ENG2UI_MSG_NUM; iCnt++)
    {
        if(Msg == Eng2UiMsgInfo[iCnt].Msg)
        {
            winks_EngGetProgress(pEng2UIMsg, &Fin, &Total);
            Winks_printf("Eng2UiMsg: [%08x][%s] [%d/%d] %s\r\n", pEng2UIMsg->err_state, 
                Eng2UiMsgInfo[iCnt].ID_Info, Fin, Total, Eng2UiMsgInfo[iCnt].StrInfo);
            break;
        }
    }
    if(iCnt == ENG2UI_MSG_NUM)
    {
        Winks_printf("Eng2UiMsg: [%08x][%08x] ????????????????????????????????????? \r\n", pEng2UIMsg->err_state, Msg);
    }
}
#endif

static Winks_EngPrtReg_s *winks_EngGetRegContext(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return (Winks_EngPrtReg_s *)(Winks_engineContext_p->communicationContext_p);
}

static Winks_EngUpdComInfo_s *winks_EngGetUpdContext(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return (Winks_EngUpdComInfo_s *)(Winks_engineContext_p->communicationContext_p);
}

static Winks_EngDLCCInfo_s *winks_EngGetDLCCContext(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return (Winks_EngDLCCInfo_s *)(Winks_engineContext_p->communicationContext_p);
}

static Winks_EngDiyComInfo_s *winks_EngGetDiyContext(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return (Winks_EngDiyComInfo_s *)(Winks_engineContext_p->communicationContext_p);
}
        
static Winks_EngPeriodComInfo_s *winks_EngGetPeriodComInfoContext(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return (Winks_EngPeriodComInfo_s *)(Winks_engineContext_p->communicationContext_p);
}

static Winks_EngCCDownloadContext_s *winks_EngGetCCDownContext(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->communicationContext_p == NULL)
    {
        Winks_engineContext_p->communicationContext_p = Winks_salAlloc(sizeof(Winks_EngCCDownloadContext_s));
        if(Winks_engineContext_p->communicationContext_p == NULL)
        {
            Winks_printf("ERROR: winks_EngGetCCDownContext\r\n");
            Winks_assert(Winks_engineContext_p->communicationContext_p != NULL);
        }
        else
            Winks_mem_set(Winks_engineContext_p->communicationContext_p, 0, sizeof(Winks_EngCCDownloadContext_s));
    }
    return (Winks_EngCCDownloadContext_s *)(Winks_engineContext_p->communicationContext_p);
}

static int winks_EngRetErr(unsigned long ErrCode)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_RETMSG, &ErrCode, sizeof(ErrCode));
}

#ifdef WINKS_DEBUG
static void winks_EngPrintFsErr(char *funcStr, char *filename, int line)
{
    unsigned long FsErrCode;
    if(Winks_GetFileSysErrorCode(&FsErrCode) == WINKS_RETURN_SUCCESS)
    {
        if(funcStr && filename)
            Winks_printf("ERROR: %4d : %s(%s) ret %d [IN] %s\r\n", line, funcStr, filename, (int)FsErrCode, __FILE__);
        else if(funcStr)
            Winks_printf("ERROR: %4d : %s ret %d [IN] %s\r\n", line, funcStr, (int)FsErrCode, __FILE__);
        else if(filename)
            Winks_printf("ERROR: %4d : (%s) ret %d [IN] %s\r\n", line, filename, (int)FsErrCode, __FILE__);
        else
            Winks_printf("ERROR: %4d : file op ret %d [IN] %s\r\n", line, (int)FsErrCode, __FILE__);
    }
    else
    {
        if(funcStr)
            Winks_EngErr(funcStr);
        else
            Winks_EngErr("file op");
    }
}

#else
#define winks_EngPrintFsErr(funcStr, filename,line)
#endif

#if 0
#define winks_EngRetMsg2UI(data, size)	\
    ((Winks_engineContext_p->app < WINKS_ENGINE_APP_INIT_CACHE) ? \
        Winks_PostMsgToUi(WINKS_MSG_ENGINE_UIRETMSG, data, size) : WINKS_RETURN_FAILURE)
#else
#define winks_EngRetMsg2UI(data, size)	\
    Winks_PostMsgToUi(WINKS_MSG_ENGINE_UIRETMSG, data, size)
#endif

/* ���WINKS_ENG_ERROR_CODE_6,7,8, ����ı䵱ǰ״̬��ֱ�ӷ��ظ�UI������Ϣ */
static int winks_EngRetErrDirectly(unsigned long ErrCode)
{
    Winks_EngineToUiReturnMsg retMsg;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_mem_set(&retMsg, 0, sizeof(retMsg));
#if 1
    retMsg.err_state = Winks_engineContext_p->lastState;
    retMsg.err_state <<= 8;
    retMsg.err_state |= Winks_engineContext_p->curState;
    retMsg.err_state <<= 8;
    retMsg.err_state |= Winks_engineContext_p->subState;
    /*
    if(Winks_engineContext_p->IsPeriodUpdating == 2)
    {
        retMsg.err_state &= 0xff00ffff;
        retMsg.err_state |= 0x00020000;
    }
    */
#endif
    retMsg.err_code = ErrCode;
    Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
    return winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
}

static int Winks_Eng2UiMsgMap(Winks_EngineToUiReturnMsg *pMsg)
{
    int   Eng2UiMsg = WINKS_MSG_ENG2UI_NONE;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(pMsg != NULL);

    /* ϵͳ���� */
    if( (pMsg->err_state & 0x000000ff) && pMsg->err_code )
        Eng2UiMsg = pMsg->err_code;
    /* ע������ */
    else if((pMsg->err_state & 0xff000000) == 0x02000000)
    {
        switch(pMsg->err_state)
        {
            /* ״̬���� */      /* ˵��                                                                   */
            /* ע�ἰ��ʼ������ */
            case 0x02010202:    /* �������÷������ɹ�                                                     */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_SVR_OK;
            break;
            
            case 0x02010200:    /* �������÷�����ʧ��                                                     */
            case 0x02020200:    /* ��ѯ�û�ע����Ϣʧ��                                                   */
            case 0x02040200:    /* ע��ʧ��                                                               */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REG_FAIL;
            break;
            
            case 0x02020203:    /* δע�ᣬ��Ҫ����ע�����                                               */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SMS_REG;
            break;
            
            case 0x02020205:    /* ע��ɹ����Ѿ���ע���û�������ʼ��ʼ����ȡ����������Ϣ                 */
            case 0x02040205:    /* ע��ɹ�����ʼ��ʼ����ȡ����������Ϣ                                   */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SMS_REG_OK;
            break;
            
            case 0x02020204:    /* ���ڷ�����ע��...                                                      */
            case 0x02030204:    /* ���ڷ�����ע��...                                                      */
            case 0x02040204:    /* ���ڷ�����ע��...                                                      */
            case 0x02050205:    /* ȡ����������Ϣ�ɹ���׼��ȡƬͷ����Ӫ�̲���                             */
            case 0x02050206:    /* ȡ����������Ϣ�ɹ���׼��ȡƬͷ����Ӫ�̲���                             */
            case 0x02060206:    /* ��������Ƭͷ����Ӫ�̲��񡤡���                                         */
            Eng2UiMsg = WINKS_MSG_ENG2UI_INIT;
            break;
            
            case 0x020502ff:    /* ȡ����������Ϣ�ɹ�����ʼ����ϣ���Ƭͷ����Ӫ�̲�����Ҫ���أ���ɳ�ʼ�� */
            case 0x020602ff:    /* ȡƬͷ����Ӫ�̲���ɹ�����ʼ����ϣ���ɳ�ʼ��                         */
            Eng2UiMsg = WINKS_MSG_ENG2UI_INIT_OK;
            break;
            
            case 0x02050200:    /* ȡ����������Ϣʧ��                                                     */
            case 0x02060200:    /* ȡƬͷ����Ӫ�̲���ʧ��                                                 */
            Eng2UiMsg = WINKS_MSG_ENG2UI_INIT_FAIL;
            break;
        }
    }
    else
    {
        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_REREG)
        {
            switch(pMsg->err_state)
            {
            /* ���¼����ѯ������ע��״̬ */
            case 0x0409040a:    /* �������÷������ɹ���׼����ͨѶ��������ѯע����Ϣ                       */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_SVR_OK;
            break;
            
            case 0x04090400:    /* �������÷�����ʧ��                                                     */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REREG_FAIL;
            break;
            
            case 0x040a04ff:    /* ע��ɹ����Ѿ���ע���û���                                             */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REREG_OK;
            break;
            
            case 0x040a0203:    /* ��Ҫ����ע���������ע��                                               */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SMS_REG;
            break;
            
            case 0x040a0400:    /* ��ѯ�û�ע����Ϣʧ��                                                   */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REREG_FAIL;
            break;
            }
        }
        else if(Winks_engineContext_p->app == WINKS_ENGINE_APP_UPLOAD_DIY)
        {
            switch(pMsg->err_state)
            {
            /* �ϴ�DIY */
            case 0x04080408:    /* �����ϴ�DIY������                                                      */
            Eng2UiMsg = WINKS_MSG_ENG2UI_DIY;
            break;
            
            case 0x040804ff:    /* �ϴ�DIY�ɹ�                                                            */
            Eng2UiMsg = WINKS_MSG_ENG2UI_DIY_OK;
            break;
            
            case 0x04080400:    /* ����DIYʧ��                                                            */
            Eng2UiMsg = WINKS_MSG_ENG2UI_DIY_FAIL;
            break;
            }
        }
        else if(Winks_engineContext_p->app == WINKS_ENGINE_APP_SYNC_SETTING)
        {
            switch(pMsg->err_state)
            {
            /* ͬ���������õ������� */
            case 0x040704ff:    /* ������Ϣͬ���������ɹ�                                                 */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_OK;
            break;
            
            case 0x04070400:    /* ������Ϣͬ��������ʧ��                                                 */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_FAIL;
            break;
            }
        }
        else
        {
            if((pMsg->err_state&0x000000ff) == 0x000000ff)
            {
                switch(Winks_engineContext_p->app)
                {
                case WINKS_ENGINE_APP_UPDATE_FRIEND:    /* ���º���¼ */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_FRIEND_OK;
                    break;
            #ifdef WINKS_SUPPORT_3G
                case WINKS_ENGINE_APP_CALL_DOWNLOAD:    /* �绰���� */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_OK;
                    break;
            #endif /* WINKS_SUPPORT_3G */
                /* ��̨���� */
                case WINKS_ENGINE_APP_INIT_CACHE:        /* ��ʼ��Cache */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_OK;
                    break;
                case WINKS_ENGINE_APP_REBULID_CACHE:     /* �ؽ�Cache */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_OK;
                    break;
                case WINKS_ENGINE_APP_PERIOD_UPDATE:     /* ��ʱ���� */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH_OK;
                    break;
                case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:  /* �������� */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECRUIT_DL_OK;
                    break;
                }
            }
            else if((pMsg->err_state&0x000000ff) == 0x00000000)
            {
                switch(Winks_engineContext_p->app)
                {
                case WINKS_ENGINE_APP_UPDATE_FRIEND:    /* ���º���¼ */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_FL_FAIL;
                    break;
           #ifdef WINKS_SUPPORT_3G
                case WINKS_ENGINE_APP_CALL_DOWNLOAD:    /* �绰���� */
                    if(Winks_engineContext_p->isCallCCOk != 0xff)
                    {
                        if((pMsg->err_state&0x00ff00ff) == 0x00050000)
                            Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL;
                        else
                            Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_FAIL;
                    }
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL;
                    break;
           #endif /* WINKS_SUPPORT_3G */
                /* ��̨���� */
                case WINKS_ENGINE_APP_INIT_CACHE:        /* ��ʼ��Cache */
                    if((pMsg->err_state&0x00ff0000) == 0x00030000)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_FAIL;
                    break;
                case WINKS_ENGINE_APP_REBULID_CACHE:     /* �ؽ�Cache */
                    if((pMsg->err_state&0x00ff0000) == 0x000b0000)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_FAIL;
                    break;
                case WINKS_ENGINE_APP_PERIOD_UPDATE:     /* ��ʱ���� */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH_FAIL;
                    break;
                case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:  /* �������� */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL;
                    break;
                }
            }
            else
            {
                switch(Winks_engineContext_p->app)
                {
                case WINKS_ENGINE_APP_UPDATE_FRIEND:    /* ���º���¼ */
                        Eng2UiMsg = WINKS_MSG_ENG2UI_DL_FRIENDCC;
                    break;
            #ifdef WINKS_SUPPORT_3G
                case WINKS_ENGINE_APP_CALL_DOWNLOAD:    /* �绰���� */
                    if((pMsg->err_state&0x00ff00ff) == 0x00050006)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_GRT_OK;
                    else if((pMsg->err_state&0x00ff00ff) == 0x00060006)
                    {
                        if(Winks_engineContext_p->isCallCCOk == 0xff)
                            Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_OTHER;
                        else
                            Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_OK;
                    }
                    break;
            #endif /* WINKS_SUPPORT_3G */
                /* ��̨���� */
                case WINKS_ENGINE_APP_INIT_CACHE:        /* ��ʼ��Cache */
                    if((pMsg->err_state&0x00ff00ff) == 0x00030005)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_FL;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE;
                    break;
                case WINKS_ENGINE_APP_REBULID_CACHE:     /* �ؽ�Cache */
                    if((pMsg->err_state&0x00ff0000) == 0x000b0000)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_TRIM;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE;
                    break;
                case WINKS_ENGINE_APP_PERIOD_UPDATE:     /* ��ʱ���� */
                    if( ((pMsg->err_state&0x00ff0000) == 0x00010000) ||
                        ((pMsg->err_state&0x00ff0000) == 0x00020000) )
                        Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH_FRIEND;
                    break;
                case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:  /* �������� */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECRUIT_DL;
                    break;
                }
            }
        }
    }

#if 0   /* mask����UI���������Ϣ */ 
    /* ��UI�����ĵ���Ϣת��ΪWINKS_MSG_ENG2UI_NONE */
    switch(Eng2UiMsg)
    {
    case WINKS_MSG_ENG2UI_NONE:
        Winks_printf("Winks_Eng2UiMsgMap: [%08x] ????????????????????????????????????? \r\n", pMsg->err_state);
        break;
/* 1 ע�ἰ��ʼ������ */
    case WINKS_MSG_ENG2UI_SETUP_SVR_OK  :   /* XX �������÷������ɹ� */
    case WINKS_MSG_ENG2UI_INIT          :   /* XX ���ڳ�ʼ��... */
/* 2 ���º���¼ */
    case WINKS_MSG_ENG2UI_FL_OK         :   /* XX �����б�������� */
    case WINKS_MSG_ENG2UI_FRIEND_SET    :   /* XX �ϴ�����¼�ɹ� */
    case WINKS_MSG_ENG2UI_FRIEND_UPD    :   /* XX �ϴ�����¼�����غ������� */
/* 3 ͬ���������õ������� */
/* 4 �ϴ�DIY */
    case WINKS_MSG_ENG2UI_DIY           :   /* XX �����ϴ�DIY... */
/* 5 ���¼����ѯ������ע��״̬ */
/* 6 �绰���� */
    case WINKS_MSG_ENG2UI_CALLCC_OTHER  :   /* XX��ȡƬͷ��Ӫ�̲��� */
    case WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL: /* XX��ȡƬͷ��Ӫ�̲���ʧ�� */
/* 7 ��̨���� */
    case WINKS_MSG_ENG2UI_PUSH          :   /* XX ��̨���ڶ�ʱ���·���... */
    case WINKS_MSG_ENG2UI_PUSH_FRIEND   :   /* XX ��̨���ڸ��º���... */
    case WINKS_MSG_ENG2UI_PUSH_OK       :   /* XX ��̨��ʱ���·������ */
    case WINKS_MSG_ENG2UI_PUSH_CHECK_FL :   /* XX ��̨��ʱ���·������,����Ҫ�������б� */
    case WINKS_MSG_ENG2UI_PUSH_FAIL     :   /* XX ��̨��ʱ���·���ʧ�� */

    case WINKS_MSG_ENG2UI_INITCACHE_FL  :   /* XX ��̨�������غ����б��Ա��ʼ��Cache... */
    case WINKS_MSG_ENG2UI_INITCACHE     :   /* XX ��̨���ڳ�ʼ��Cache... */
    case WINKS_MSG_ENG2UI_INITCACHE_OK  :   /* XX ��̨��ʼ��Cache��� */
    case WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL :   /* XX ��̨���غ����б�ʧ�� */
    case WINKS_MSG_ENG2UI_INITCACHE_FAIL    :   /* XX ��̨��ʼ��Cacheʧ�� */

    case WINKS_MSG_ENG2UI_RECACHE_TRIM  :   /* XX ��̨�������ػ�����Ϣ���ؽ�Cache... */
    case WINKS_MSG_ENG2UI_RECACHE       :   /* XX ��̨�����ؽ�Cache... */
    case WINKS_MSG_ENG2UI_RECACHE_OK    :   /* XX ��̨�ؽ�Cache��� */
    case WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL :   /* XX ��̨���ػ�����Ϣʧ�� */
    case WINKS_MSG_ENG2UI_RECACHE_FAIL      :   /* XX ��̨�ؽ�Cacheʧ�� */

    case WINKS_MSG_ENG2UI_RECRUIT_DL    :   /* XX ��̨���ڲ������ص绰����... */
    case WINKS_MSG_ENG2UI_RECRUIT_DL_OK :   /* XX ��̨�������ص绰������� */
    case WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL   :   /* XX ��̨�������ص绰����ʧ�� */

/* ����UI���������Ϣ */
    //case WINKS_MSG_ENG2UI_DIAL_INT:   
    case WINKS_MSG_ENG2UI_ROAMING_LIMIT:
    #ifdef WINKS_DEBUG
    {
        int iCnt;
        for(iCnt=0; iCnt<ENG2UI_MSG_NUM; iCnt++)
        {
            if(Eng2UiMsg == Eng2UiMsgInfo[iCnt].Msg)
            {
                Winks_printf("XXXXX Msg: [%08x][%s] %s\r\n", pMsg->err_state, Eng2UiMsgInfo[iCnt].ID_Info, Eng2UiMsgInfo[iCnt].StrInfo);
                break;
            }
        }
    }
    #endif
        Eng2UiMsg = WINKS_MSG_ENG2UI_NONE;
    break;
    default:
    break;
    }
#endif

    return Eng2UiMsg; 
}

static int winks_EngSetSmsGateway(char *gateway, unsigned long len)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    strncpy(data_p->msgGateway, gateway, WINKS_MAX_LEN_OF_MSG_GATEWAY);
    data_p->msgGateway[WINKS_MAX_LEN_OF_MSG_GATEWAY - 1] = 0;
    /* ���ü��洢�Ƿ�ɹ� */
    Winks_SetSmsGateway(gateway, len);
    
    return 0;
}
#if 0
static int winks_EngGetSmsGateway(char *gateway, unsigned long len)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    Winks_assert(len>=WINKS_MAX_LEN_OF_MSG_GATEWAY);
    strncpy(gateway, data_p->msgGateway,len-1);
    gateway[len-1] = 0;

    return 0;
}
#endif
/*�����ѯע��ɹ������ں���*/
static int winks_EngSetQueryLongTime(unsigned long time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    data_p->longTime = time;
    /* ���ü��洢�Ƿ�ɹ� */
    Winks_set_setting_value(E_SETTING_LONG_CYCLE, time);
    
    return 0;
}
#if 0
/*��ȡ��ѯע��ɹ������ں���*/
static int winks_EngGetQueryLongTime(unsigned long *time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    *time = data_p->longTime;
    return 0;
}
#endif
/*�����ѯע��ɹ������ں���*/
static int winks_EngSetQueryShortTime(unsigned long time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    data_p->shortTime = time;
    /* ���ü��洢�Ƿ�ɹ� */
    Winks_set_setting_value(E_SETTING_SHORT_CYCLE, time);
    
    return 0;
}
#if 0
/*��ȡ��ѯע��ɹ������ں���*/
static int winks_EngGetQueryShortTime(unsigned long *time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    *time = data_p->shortTime;
    return 0;
}
#endif
/*�����ѯע��ɹ��ʺ��ﺯ��*/
static int winks_EngSetQueryRetMsg(unsigned char *msg, unsigned long msg_len)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

#if 1/* UIû�д������Ϣ����,���ø�UI���� */
    data_p->msg[0] = 0;
#else
    if(msg_len<=WINKS_ENGINE_RETURN_MSG_MAX_LEN)
        Winks_mem_cpy(data_p->msg, msg, msg_len);
    data_p->msg[sizeof(data_p->msg) - 1] = 0;
#endif
    return 0;
}
#if 0
/*��ȡ��ѯע��ɹ��ʺ��ﺯ��*/
static int winks_EngGetQueryRetMsg(unsigned char *msg, unsigned long msg_len)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);
    
    strncpy((char *)msg, (char *)data_p->msg, msg_len - 1);
    msg[msg_len - 1] = 0;
    return 0;
}
#endif
/* ����ͨѶ������ip�Ͷ˿� */
static int winks_EngSetCommSvr(Winks_server_list_s *pServer)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);
    Winks_assert(pServer);

    memcpy(&data_p->server, pServer, sizeof(Winks_server_list_s));
    return 0;
}
#if 0
/* ��ȡͨѶ������ip�Ͷ˿� */
static int winks_EngGetCommSvr(Winks_server_list_s *pServer)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);
    
    memcpy(pServer, &data_p->server, sizeof(Winks_server_list_s));
    return 0;
}
#endif
/*У��������ʽ�����ݡ�*/
static int Winks_ConnectWaysAdjust(Winks_ConnectWayOperation_s *cwo)
{
    unsigned long low16bit = 0;
    
    Winks_assert(cwo != NULL);
    cwo->operation &= (((CONNECT_WAY_SERVER_PUSH | CONNECT_WAY_TIMER_WAKE_UP)<<16) |
        (CONNECT_WAY_SERVER_PUSH | CONNECT_WAY_TIMER_WAKE_UP));
    low16bit = cwo->operation&0x0000ffff;
    if((low16bit == 0)||(low16bit == CONNECT_WAY_TIMER_WAKE_UP))
        cwo->operation = (CONNECT_WAY_TIMER_WAKE_UP<<16|CONNECT_WAY_TIMER_WAKE_UP);
    else if(low16bit == CONNECT_WAY_SERVER_PUSH)
        cwo->operation = (CONNECT_WAY_SERVER_PUSH<<16|CONNECT_WAY_SERVER_PUSH);
    else if (cwo->operation&0xffff0000 == 0)
        cwo->operation = (CONNECT_WAY_TIMER_WAKE_UP<<16);
        
    if((cwo->twuTimeSlot > WINKS_TIME_CONNECT_PERIOD_MAX) || 
        (cwo->twuTimeSlot < WINKS_TIME_CONNECT_PERIOD_MIN))
    {
        cwo->twuTimeSlot = WINKS_TIME_CONNECT_PERIOD_DEF;
    }
    
    if((cwo->twuTimeLimit > WINKS_TIME_CONNECT_OVERTIME_MAX) ||
        (cwo->twuTimeLimit < WINKS_TIME_CONNECT_OVERTIME_MIN))
    {
        cwo->twuTimeLimit = WINKS_TIME_CONNECT_OVERTIME_DEF;
    }
    
    if((cwo->spTimeGiveUp > WINKS_PUSH_GIVE_UP_TIMER_MAX) ||
        (cwo->spTimeGiveUp < WINKS_PUSH_GIVE_UP_TIMER_MIN))
    {
        cwo->spTimeGiveUp  = WINKS_PUSH_GIVE_UP_TIMER_DEF;
    }
    return WINKS_RETURN_SUCCESS;
}

static int Winks_GetTimeForImsi(unsigned short cnt)
{
	int ret_time = 0;
	switch(cnt/3)
	{
		case 0:
			ret_time = 600;
			break;
		case 1:
			ret_time = 1000;
			break;
		case 2:
			ret_time = 3000;
			break;
		case 3:
			ret_time = 10000;
			break;
		case 4:
			ret_time = 60000;
			break;
		case 5:
			ret_time = 300000;
			break;
		default:
			ret_time = 3600000;
			break;
	}
	return ret_time;
}

/* ����ע����� */
static int winks_sendRegSms(void)
{
    char buff[100];
    char imsiCode[WINKS_IMSI_LEN + 1];
    int iRet = 0;
    
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();

    Winks_assert(data_p);

    if(data_p == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_mem_set(imsiCode, 0, sizeof(imsiCode));
    iRet = Winks_GetImsi(WINKS_CHN_NULL, imsiCode, sizeof(imsiCode));
    imsiCode[sizeof(imsiCode)-1] = 0;
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }
        
    snprintf(buff, sizeof(buff)-1, "IMSI:%s;CLIENTTYPE:%d", imsiCode, WINKS_SODRWARE_TYPE-1);
    buff[sizeof(buff)-1] = 0;
#ifndef WINKS_DUAL_SIM
    return Winks_SendSms(data_p->msgGateway, strlen(data_p->msgGateway), buff, strlen(buff)+1);
#else /* WINKS_DUAL_SIM */
    return Winks_SendSms(data_p->msgGateway, strlen(data_p->msgGateway), buff, strlen(buff)+1, Winks_GetActiveSim());
#endif /* WINKS_DUAL_SIM */
}

static int winks_EngHttpClose(int http)
{
    int iRet = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    iRet = Winks_httpclose(http);
    if(Winks_engineContext_p->http == http)
    {
        Winks_engineContext_p->http = WINKS_HTTP_NULL;
    #ifdef WINKS_DUAL_SIM
        if(Winks_engineContext_p->NeedRestoreWapSimSel)
        {
            Winks_engineContext_p->NeedRestoreWapSimSel = 0;
            Winks_api_RestoreNetLinkSim(Winks_engineContext_p->SavePhoneWapSimSel);
        }
    #endif /* WINKS_DUAL_SIM */
    }
    return iRet;
}

/*��������*/
static int winks_SendData(unsigned char server,char *data, unsigned long len)
{
    unsigned char allowFlag = 0;
    unsigned long ap = 0;
    Winks_Http httpstr;
    Winks_Proxy Proxy;
    Winks_ProxyServer_s getProxy;
    char sUrl[WINKS_URL_MAX_LEN];
    int ifpipe = WINKS_HTTP_CONCLOSE;
    int iRet = 0;
    Winks_ConnectWayOperation_s cwo;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_mem_set(&httpstr, 0, sizeof(httpstr));
    Winks_mem_set(&Proxy, 0, sizeof(Proxy));
    Winks_mem_set(&getProxy, 0, sizeof(getProxy));

    Winks_printf("############## winks_SendData(%d, 0x%08x, %d) seq[%02x%02x] msg[0x%02X%02X]\r\n", server, data, len 
        , (unsigned char)data[32], (unsigned char)data[33], (unsigned char)data[24], (unsigned char)data[25]);
    Winks_printf("Winks_engineContext_p->server[%d, 0x%08x, %d]\r\n", 
        Winks_engineContext_p->server.srv,
        Winks_engineContext_p->server.data,
        Winks_engineContext_p->server.len);

    /*������α�־:�������β���������*/
    iRet = Winks_isRoamingConnectionAllowed(&allowFlag);
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }
    if((allowFlag == 0)&& (iRet == WINKS_RETURN_SUCCESS))
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ROAMING_LIMIT);
        return WINKS_RETURN_SUCCESS;
    }


    iRet = Winks_GetCurAccessPoint(&ap);
    if(iRet == WINKS_RETURN_FAILURE)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_printf("Winks_GetCurAccessPoint=[%d]\r\n",ap);
    Winks_setdialid( ap );

    /*��¼������Ϣ*/
    if(Winks_engineContext_p->server.data != data)
    {
        if(Winks_engineContext_p->server.data != NULL)
        {
            unsigned char *pData = (unsigned char *)Winks_engineContext_p->server.data;
            Winks_printf("Already somethine transfering. seq[%02x%02x] msg[0x%02X%02X] \r\n", pData[32], pData[33], pData[24], pData[25]);
            Winks_assert(Winks_engineContext_p->server.data == NULL);
            return WINKS_RETURN_FAILURE;
        }
        Winks_engineContext_p->server.data = Winks_salAlloc(len);
        if(Winks_engineContext_p->server.data == NULL)
        {
            Winks_printf("ERROR: Winks_salAlloc(%d)\r\n", len);
            Winks_assert(Winks_engineContext_p->server.data != NULL);
            return WINKS_RETURN_FAILURE;
        }
        Winks_engineContext_p->server.srv = server;
        Winks_mem_cpy(Winks_engineContext_p->server.data, data, len);
        Winks_engineContext_p->server.len = len;
        Winks_assert(Winks_engineContext_p->server.staDataSrv < WINKS_SERVER_CNT);
    }
    
    
    Winks_mem_set(sUrl,0,sizeof(sUrl));
    if(server == (unsigned char)0)
    {
#ifdef WINKS_DEBUG
//#ifndef WINKS_SIMULATOR /*phone*/
        unsigned long val = 0;
        Winks_GetOsalTestBitFlag(4,&val);
        if(val)
        {
            if(1)/*���ؿ�������URL*/
            {
                if(strcmp(WINKS_HOST_SERVER_URL,WINKS_URL_TEST)== 0)
                    snprintf(sUrl, sizeof(sUrl)-1, "%s", WINKS_URL_DEVICE);
#if 1
                else 
                    snprintf(sUrl, sizeof(sUrl)-1, "%s", WINKS_URL_TEST);
#else
                else if(strcmp(WINKS_HOST_SERVER_URL,WINKS_URL_DEVICE)== 0)
                    snprintf(sUrl, sizeof(sUrl)-1, "%s", WINKS_URL_TEST);
                else
                    snprintf(sUrl, sizeof(sUrl)-1, "%s", WINKS_HOST_SERVER_URL);
#endif
            }
            else
                snprintf(sUrl, sizeof(sUrl)-1, "%s", WINKS_HOST_SERVER_URL);
        }
        else
//#endif
#endif
        snprintf(sUrl, sizeof(sUrl)-1, "%s", WINKS_HOST_SERVER_URL);
        sUrl[sizeof(sUrl)-1] = 0;
    }
    else if(server == (unsigned char)1)
    {
        Winks_server_list_s server;
        unsigned char try_index;
        try_index = (unsigned char)(Winks_engineContext_p->server.staDataSrv + Winks_engineContext_p->server.tryCnt);
#ifdef WINKS_SIMULATOR/*ģ�����ӵ�1��ip��ʼ��������*/
        if((Winks_engineContext_p->server.tryCnt == 0) && 
            (Winks_engineContext_p->server.staDataSrv == 0))
        {
            try_index = 1;
        }
#endif       
        memset(&server, 0, sizeof(server));
        iRet = Winks_GetCommServerIpPort(&server);
        if(server.cnt)
            try_index %= server.cnt/*WINKS_SERVER_CNT*/;
        else
        {
            //Winks_assert(server.cnt);
            return WINKS_RETURN_FAILURE;
        }
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            return WINKS_RETURN_FAILURE;
        }
        snprintf(sUrl, sizeof(sUrl)-1, "http://%s",server.ip_port[try_index]);      
        /*snprintf(sUrl, sizeof(sUrl)-1, "http://192.168.10.11:5052");*/
        sUrl[sizeof(sUrl)-1] = 0;
    }
#ifdef WINKS_ENGINE_TEST_5PROTOCAL
    else if(server == (unsigned char)2)
    {
        snprintf(sUrl, sizeof(sUrl)-1, "%s",WINKS_COMM_SERVER_URL);
        sUrl[sizeof(sUrl)-1] = 0;
    }
#endif
    else
    {
        Winks_printf("ERROR: Unknow server type! type=[%d]",server);
        return WINKS_RETURN_FAILURE;
    }
    httpstr.channel = Winks_engineContext_p->engChn;
    /*�������󷽷�ΪPOST��*/
    httpstr.method = WINKS_HTTP_MEPOST;
    /*������Ϣ���պ����Ľ�����Ϣ��*/
    httpstr.msg = WINKS_MSG_ENGINE_HTTP;
    /*����������*/
    httpstr.pContenttype = NULL;

#ifdef TEST_HTTP /* ����HttpЭ�� */
    snprintf(sUrl, sizeof(sUrl)-1, TEST_HTTP);
    sUrl[sizeof(sUrl)-1] = 0;
#endif
    Winks_printf("sUrl[%s]\r\n",sUrl);

#ifndef WINKS_UNSUPPORT_PROXY
  #ifndef WINKS_SIMULATOR
    Winks_mem_set(&getProxy, 0, sizeof(getProxy));
    iRet = Winks_GetProxyServer(&getProxy);
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_printf("ProxyHost[%s] port[%d] iplen=%d\r\n", getProxy.ip_addr, getProxy.port, strlen(getProxy.ip_addr));
{
    char *pIp = getProxy.ip_addr;
    while((*pIp == '.') || ((*pIp >= '0')&&(*pIp <= '9')))
        pIp++;
    *pIp = '\0';
}
    if((strcmp(getProxy.ip_addr, "0.0.0.0") != 0) &&
        (strlen(getProxy.ip_addr) != 0))
    {
        /*Proxy.phostname = "10.0.0.172";
        Proxy.port = 80;*/
        Winks_printf("0000000000000000000000 %s:%d iplen=%d\r\n", getProxy.ip_addr, getProxy.port, strlen(getProxy.ip_addr));
        Proxy.phostname = getProxy.ip_addr;
        Proxy.port = getProxy.port;
        Proxy.ppassword = NULL;
        Proxy.pusername = NULL;
        httpstr.pwwwau = NULL;

        httpstr.pproxy = &Proxy;
    }
    else
  #endif
#endif
    {
        httpstr.pproxy = NULL;
        httpstr.pwwwau = NULL;
    }
    
    /*�ɰ���Ҫ���ò�֧�ֳ־�����*/
    Winks_httpsetopt( WINKS_HTTP_OPPIPE, &ifpipe, sizeof(ifpipe) );
    Winks_httpsetopt( WINKS_HTTP_OPUAGENG, WINKS_USRAGENT, strlen(WINKS_USRAGENT) );

    /*����������ҳ��URL��*/
    httpstr.pURL = sUrl;

    /*����HTTP��������*/
    Winks_engineContext_p->http = Winks_httpopen( &httpstr );
    if(Winks_engineContext_p->http == WINKS_HTTP_NULL)
    {
        Winks_assert(Winks_engineContext_p->http != WINKS_HTTP_NULL);
        return WINKS_RETURN_FAILURE;
    }

#ifdef TEST_HTTP /* ����HttpЭ�� */
    /*д������*/
    iRet = Winks_httpappenddata( Winks_engineContext_p->http, data, len );
    if(iRet == -1)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_assert(iRet >= 0);
        return WINKS_RETURN_FAILURE;
    }
#else
    /*д������*/
    iRet = Winks_httpappenddata( Winks_engineContext_p->http, data, len );
    if(iRet == -1)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_assert(iRet >= 0);
        return WINKS_RETURN_FAILURE;
    }
#endif

    /*�������䡣*/
    iRet = Winks_httpstart( Winks_engineContext_p->http );
    if(iRet == -1)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_assert(iRet != 0);
        return WINKS_RETURN_FAILURE;
    }

    Winks_assert(Winks_engineContext_p->httpTimer == WINKS_TIMER_NULL);
    Winks_GetSupportedConnectWays(&cwo);
    Winks_ConnectWaysAdjust(&cwo);
    Winks_engineContext_p->httpTimer = Winks_CreateTimer(Winks_engineContext_p->engChn, WINKS_SECONDS2TICKS(cwo.twuTimeLimit/*WINKS_HTTP_TIME_LIMIT*/), WINKS_TIMER_AUTO_START);
    Winks_assert(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL);
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_HttpTryNextIpPort(int http)
{
    int iRet = 0;
    Winks_server_list_s server;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    //Winks_assert(Winks_engineContext_p->http == http);
    winks_EngHttpClose(http);

    /*Destroy http timer.*/
    if(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL)
    {
        Winks_DestroyTimer(Winks_engineContext_p->httpTimer);
        Winks_engineContext_p->httpTimer = WINKS_TIMER_NULL;
    }     
    
    /*ѭ��ƥ����ͨѶ����������*/
    memset(&server, 0, sizeof(server));
    iRet = Winks_GetCommServerIpPort(&server);
    if( (Winks_engineContext_p->server.srv == 0) || /* ���÷������������� */
        ((unsigned long)(Winks_engineContext_p->server.tryCnt + 1) >= server.cnt) || /* ����IpPort���Թ��� */
        (Winks_engineContext_p->server.datainFlag == 1) )/* ��ǰIpPort�յ�����Ч���� */
    {
        Winks_engineContext_p->server.tryCnt = 0;
        if(Winks_engineContext_p->server.data != NULL)
        {
            Winks_salFree(Winks_engineContext_p->server.data);
            Winks_engineContext_p->server.data = NULL;
        }
        Winks_engineContext_p->server.len = 0;
        Winks_engineContext_p->server.datainFlag = 0;           

        return WINKS_RETURN_FAILURE;
    }
    else
    {
        ++(Winks_engineContext_p->server.tryCnt);
        winks_SendData(
            Winks_engineContext_p->server.srv, 
            Winks_engineContext_p->server.data, 
            Winks_engineContext_p->server.len);
    }
    
    return iRet;
}

static int winks_HttpCloseProc(int http)
{
    int iRet = 0;

    Winks_printf( "[ENG]WINKS http get http message close ok\r\n");
    
    if(winks_HttpTryNextIpPort(http) == WINKS_RETURN_FAILURE)
    {
        /*����ر���Ϣ������Ϣͨ�����ڴ����������û����������
        �û����ڱ���Ϣ���յ��Ժ�رվ�����ͷ���Դ��*/
        Winks_printf( "WINKS http get http message close ok\r\n" );
        return winks_EngRetErr(WINKS_MSG_ENG2UI_SVR_NO_ANSWER);
    }

    return iRet;
}

/* Http��Ϣ������ */
static int winks_HttpMsgProc(const void* data, unsigned int size )
{
    int http = WINKS_HTTP_NULL;

    char pURL[WINKS_URL_MAX_LEN+1];
    int ulen = 0;
    Winks_Socketmsg_s* p = (Winks_Socketmsg_s* )data;
    /*Winks_HttpInfo Httpinfo;*/

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    /*��Ϣ������Ч�Լ�顣*/
    if( p == NULL )
    {
        Winks_printf( "WINKS http get http message no data\r\n" );
        return 0;
    }
    
    /*��ȡ��Ϣ��Ӧ��HTTP��������*/
    http = p->wParam & 0x0000ffff;
    /*ulen = WINKS_URL_MAX_LEN;*/
    Winks_mem_set(pURL, 0, sizeof(pURL));
    switch( p->wParam >> 24 )
    {
    case WINKS_HTTP_GH:
        /*������ѯ��Ϣ������Ϣ����Ҫ���д������ڽ���״̬֪ͨ��Ϣ��
        ��ȡ��Ӧ��HTTP�������������URL���ݡ�*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*������ѯ������ʾ��*/
            Winks_printf( "[ENG]WINKS http %s get http message gethost get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            break;
        }
        else
            /*  ������ѯ�ɹ�����ʾ��*/
            Winks_printf( "[ENG]WINKS http %s get http message gethost ok\r\n", pURL );
        break;

    case WINKS_HTTP_CONNECT:
        /*������Ϣ������Ϣ����Ҫ���д������ڽ���״̬֪ͨ��Ϣ��
        ��ȡ��Ӧ��HTTP�������������URL���ݡ�*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*���ӳ��ִ�����ʾ��*/
            Winks_printf( "[ENG]WINKS http %s get http message connect get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            break;
        }
        /*  ���ӳɹ�����ʾ��*/
        Winks_printf( "[ENG]WINKS http %s get http message connect ok\r\n", pURL );
        break;

    case WINKS_HTTP_DATAIN:
        /*���ݵ�����Ϣ������Ϣ����Ҫ���д������ڽ���״̬֪ͨ��Ϣ������Ϣ���֪�û�Ŀǰ�Ѿ����յ������ݳ��ȡ�
        ��ȡ��Ӧ��HTTP�������������URL���ݡ�*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*���ݽ��ճ�����ʾ��*/
            Winks_printf( "[ENG]WINKS http %s get http message datain get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            break;
        }
        /*���ݽ��ճɹ�����ʾ����ʾ�������ݳ��ȡ�*/
        Winks_printf( "[ENG]WINKS http %s get http message datain ok, len %d\r\n", pURL, p->lParam );


    #if 0
        /**/
        Winks_engineContext_p->server.datainFlag = 1;
        if(Winks_engineContext_p->server.data != NULL)
        {
            Winks_salFree(Winks_engineContext_p->server.data);
            Winks_engineContext_p->server.data = NULL;
            Winks_engineContext_p->server.len = 0;
        }
    #endif
        /*��������Ӧʱ���õȴ����ݵ�ʱ��*/
        Winks_assert(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL);
        Winks_StopTimer(Winks_engineContext_p->httpTimer);
        Winks_StartTimer(Winks_engineContext_p->httpTimer);

        break;

    case WINKS_HTTP_DATAEND:
    {
        int iRet = 0;
        unsigned long ret_code = 0;
        Winks_printf( "[ENG]WINKS http get http message dataend ok\r\n");
        
        /*���ݽ���*/
        iRet = winks_EngDataStreamAnalyze(http, p->lParam, &ret_code);
        if(ret_code == WINKS_MSG_ENG2UI_SVR_NO_ANSWER)
        {
            Winks_printf( "ERROR: winks_EngDataStreamAnalyze(%d, %d, %d) winks_HttpTryNextIpPort\r\n", 
                http, p->lParam, ret_code);
            if(http != Winks_engineContext_p->http)
            {
                Winks_printf("ERROR: %4d : http %d!=%d not march\r\n", 
                            __LINE__, http, Winks_engineContext_p->http);
                winks_EngHttpClose(http);
            }
            else if(winks_HttpTryNextIpPort(http) != WINKS_RETURN_FAILURE)/* ��ʼ������1��IpPort */
            {
                return 0;
            }
          #if 0
            else/* ����IpPort���������ӳɹ�,��������ʧ�� */
            {
            }
          #endif
        }
        else    /* �յ���ȷ������ */
        {
            winks_EngHttpClose(Winks_engineContext_p->http);
            
            /*Destroy http timer.*/
            if(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL)
            {
                Winks_DestroyTimer(Winks_engineContext_p->httpTimer);
                Winks_engineContext_p->httpTimer = WINKS_TIMER_NULL;
            }        
            
            if(Winks_engineContext_p->server.data != NULL)
            {
                Winks_salFree(Winks_engineContext_p->server.data);
                Winks_engineContext_p->server.data = NULL;
            }
            Winks_engineContext_p->server.len = 0;
            
            /* �յ���ȷ����������ǰIpPort */
            if(Winks_engineContext_p->server.tryCnt != 0)
            {
                Winks_server_list_s server;
                memset(&server, 0, sizeof(server));
                Winks_GetCommServerIpPort(&server);
                if(server.cnt)
                {
                    Winks_engineContext_p->server.staDataSrv = (unsigned char)(Winks_engineContext_p->server.staDataSrv + Winks_engineContext_p->server.tryCnt);
                    Winks_engineContext_p->server.staDataSrv %= server.cnt;
                }
                Winks_engineContext_p->server.tryCnt = 0;
            }
            Winks_engineContext_p->server.datainFlag = 1;
        }
        
        Winks_engineContext_p->ProtocolMsgType = 0;
        Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_RETMSG, &ret_code, sizeof(ret_code));
        break;
    }
    case WINKS_HTTP_REDIRECT:
    {
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();
        
        Winks_printf( "[ENG]WINKS http get http message redirect...\r\n");
        
        if(Winks_httpstart( Winks_engineContext_p->http ) == WINKS_HTTP_SUCCESS)
        {
            break;
        }
        /* 
        else ���������ر�Http
        goto WINKS_HTTP_CLOSE:
        */
    }

    case WINKS_HTTP_CLOSE:
    {
        Winks_printf("WARN: Engine got Msg : WINKS_HTTP_CLOSE %d\r\n", http);
        if(http != Winks_engineContext_p->http)
        {
            Winks_printf("ERROR: %4d : http %d!=%d not march\r\n", 
                        __LINE__, http, Winks_engineContext_p->http);
            winks_EngHttpClose(http);
        }
        else
            winks_HttpCloseProc(http);
        break;
    }

    default:
        Winks_printf( "[ENG]WINKS http get http message unknow msg %d\r\n", p->wParam & 0xffff );
        break;
    }

    return 0;
}

/* Ret: 1--match OK; 0--do'nt match */
static int winks_CheckFoundSim(char *imsiCode, unsigned long len)
{
    int iRet = 0;
    char SavedImsi[WINKS_IMSI_LEN+1];
    
    Winks_mem_set(&SavedImsi, 0, sizeof(SavedImsi));
    if(Winks_GetRecentlyActivatedImsi(SavedImsi, sizeof(SavedImsi)) == WINKS_RETURN_SUCCESS)
    {
        iRet = (strncmp(SavedImsi, imsiCode, WINKS_IMSI_LEN) == 0);
        Winks_printf("winks_CheckFoundSim[%s]?=[%s] ret %d\r\n",imsiCode, SavedImsi, iRet);
        return iRet;
    }
    Winks_printf("winks_CheckFoundSim no old imsi\r\n");
    return 0;
}

/*
  BootMethod:
    0--û�з���Sim��,��������             (���������������)
    1--����1��Sim��,��������ע���Sim��   (�����������1�鼴��)
    2--����2��Sim��,����������ע���Sim�� (����������붼����)
    3--������ע���Sim��,��������         (���������������)
*/
static int winks_BootingWithSimCard(
        int BootMethod,
        char *imsiCode1, unsigned long len1,
        char *imsiCode2, unsigned long len2)
{
    int iRet = 0;
    int StrgStatus;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_BootingWithSimCard(%d, \"%s\", %d, \"%s\", %d)\r\n",
        BootMethod, (imsiCode1 ? imsiCode1 : "NULL"), len1, (imsiCode2 ? imsiCode2 : "NULL"), len2);
    Winks_assert(BootMethod<4);
    
    if(Winks_StrgInitMapFile() != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    Winks_StrgFLDeleteAll();
    Winks_StrgNDDeleteAll();
    
    if(BootMethod == 0)/* û��Sim��������ͣ��������״̬ */
        return WINKS_RETURN_FAILURE;
    else if(BootMethod == 3)/* �ҵ���ע��Sim�� */
    {
        Winks_engineContext_p->curState = WINKS_ENGINE_STATE_ENABLED;
        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
        //Winks_engineContext_p->IsPeriodUpdating = 0;
        Winks_engineContext_p->isImsiEnabled = WINKS_IMSI_ENABLED;
        Winks_engineContext_p->ifRemindRegister = 0;
        /* ������ʱ���·��� */
        winks_StartPeriodPushTimer();
        
        StrgStatus = Winks_StrgGetStatus();
        if( StrgStatus == WINKS_STRG_CARD_STATUS_NO_CCCACHE ||
            StrgStatus == WINKS_STRG_CARD_STATUS_OLD_CARD )
        {
            /* δ���Cache��ʼ�� */
            winks_StartBuildCacheTimer();
        }
    }
    else/* δ�ҵ���ע��Sim�� */
    {
        unsigned char flag = 1;
        Winks_engineContext_p->curState = WINKS_ENGINE_STATE_DISABLED;  
        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
        //Winks_engineContext_p->IsPeriodUpdating = 0;
    #ifdef WINKS_DUAL_SIM
        if(imsiCode1 && len1)
        {
            iRet = Winks_GetImsiPopFlag(imsiCode1, &flag);
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                return WINKS_RETURN_FAILURE;
            }
        }
        if( (flag==1) &&  /* ��1Sim������Ҫ��ʾ,�����鿴��2Sim���Ƿ���Ҫ��ʾ */
            imsiCode2 && len2)
        {
            iRet = Winks_GetImsiPopFlag(imsiCode2, &flag);
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                return WINKS_RETURN_FAILURE;
            }
        }
    #else /* WINKS_DUAL_SIM */
        iRet = Winks_GetImsiPopFlag(imsiCode1, &flag);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            return WINKS_RETURN_FAILURE;
        }
    #endif /* WINKS_DUAL_SIM */
        
        Winks_engineContext_p->ifRemindRegister = (unsigned char)((flag == 1)?0:1);
    }
    
#ifdef WINKS_DEBUG
    /*���Թ��ܿ���*/
    Winks_SetOsalTestBitFlag(2, 0);/*debug��ӡ����*/
#endif

    /*Post msg to ui for pop infomation.*/
    iRet = Winks_PostMsgToUi(WINKS_MSG_ENGINE_REG_NOTIFY, &Winks_engineContext_p->ifRemindRegister, 1);

    return 0;
}

/* ���岢���� */
static int winks_PackDataAndSend(unsigned short AttrId, void *pData, int *ErrCode)
{
    int iRet = 0;
    Winks_SendFrame_s SF;
    unsigned char iServer = 1;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    iRet = SendBufReInit(&SF, 0);
    if(iRet == 0)
    {
        SendBufRestore(&SF);
        return -1;
    }


    Winks_engineContext_p->ProtocolFrameNo = (unsigned short)((Winks_engineContext_p->ProtocolFrameNo+1) & 0xFFFF);
    Winks_printf("FrameNo:%-4x winks_PackDataAndSend(0x%04X, 0x%08x)\r\n", Winks_engineContext_p->ProtocolFrameNo, AttrId, pData);
    AddSendFrameHead(&SF, AttrId, Winks_engineContext_p->ProtocolFrameNo);
    iRet = AddSendFrameData(&SF, AttrId, pData);
    if(iRet <= 0)
    {
        *ErrCode = SF.ErrCode;
        SendBufRestore(&SF);
        return -1;
    }

    if(AttrId == 0x7FF9)    /* �������÷����� */
        iServer = 0;
    else
    {
#ifdef WINKS_ENGINE_TEST_5PROTOCAL
        switch(AttrId){
        case 0x7FEC:
        case 0x7FF0:
        case 0x7FEB:
        case 0x7FEF:
        case 0x7FED:
            iServer = 2;
            break;
        }
#elif defined(DBG_UNSUPPORT_PROTOCOL)
        switch(AttrId){
      #if 0
        case 0x7FEC:
        case 0x7FF0:
        case 0x7FEB:
        case 0x7FEF:
        case 0x7FED:
      #else
        case 0x7FEF:
      #endif
            Winks_engineContext_p->ProtocolMsgType = AttrId;
            iRet = DbgUnsupportProtocol(AttrId);
            SendBufRestore(&SF);
            return iRet;
        }
#endif
    }
    
    Winks_engineContext_p->server.datainFlag = 0;
    Winks_engineContext_p->ProtocolMsgType = AttrId;
#ifdef WINKS_DUAL_SIM
    if(Winks_api_SetNetLinkSim(Winks_GetActiveSim(), &Winks_engineContext_p->SavePhoneWapSimSel) != WINKS_RETURN_SUCCESS)
    {
        Winks_assert(Winks_GetActiveSim());
        return WINKS_RETURN_FAILURE;
    }
    Winks_engineContext_p->NeedRestoreWapSimSel = 1;
#endif /* WINKS_DUAL_SIM */

    iRet = winks_SendData(iServer, (char *)SF.pSendBuf, SF.Off);
    
    SendBufRestore(&SF);
    
    return iRet;
}

/* ����״̬���ֲ�ͬЭ��ţ����������巢�ͳ��� */
static int winks_EngineSendDataToServer(unsigned long state, unsigned long sub_state, void *data)
{
    int iRet = 0;
    int PD_ErrCode = 0;
    unsigned short AttrId = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_printf("winks_EngineSendDataToServer(%x, %x, 0x%08x)\r\n", state, sub_state, data);
    
    if(Winks_engineContext_p->InitSettingOk == 0)/* �洢��ʼ��ʧ�� */
        return winks_EngRetErr(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
#ifdef WINKS_DUAL_SIM
    if(Winks_isSimEnabled(Winks_GetActiveSim()) == 0)
        return winks_EngRetErr(WINKS_MSG_ENG2UI_SIM_UNUSABLE);
#endif /* WINKS_DUAL_SIM */
    
    switch(state)
    {
    case WINKS_ENGINE_STATE_DISABLED:
    {
        switch(sub_state)
        {
        case WINKS_ENGINE_SUBSTATE_ONE:     /* �������÷����� */
            AttrId = 0x7FF9;
            break;
        case WINKS_ENGINE_SUBSTATE_TOW:     /* ��ѯע����Ϣ */
        case WINKS_ENGINE_SUBSTATE_FOUR:
            AttrId = 0x7FFD;
            break;
        case WINKS_ENGINE_SUBSTATE_FIVE:    /* ���ػ�����Ϣ */
            AttrId = 0x7FEC;
            break;              
        case WINKS_ENGINE_SUBSTATE_SIX:     /* ����Ƭͷ��Ӫ�̲��� */
            AttrId = 0x7FFA;
            break;  
        default:
            Winks_EngErr("error case");
            break;
        }
        break;
    }

    case WINKS_ENGINE_STATE_ENABLED:
    {
        switch(sub_state)
        {
        case WINKS_ENGINE_SUBSTATE_ONE:/*��ʱ����*/
            AttrId = 0x7FED;
            break;
        case WINKS_ENGINE_SUBSTATE_TOW:/*���ݲ�����ID���ز�����Դ*/
        case WINKS_ENGINE_SUBSTATE_SIX:
        case WINKS_ENGINE_SUBSTATE_TWELVE:
            AttrId = 0x7FFA;
            break;
        case WINKS_ENGINE_SUBSTATE_THREE:/*��������б�*/   
            AttrId = 0x7FF0;
            break;
        case WINKS_ENGINE_SUBSTATE_FIVE:/*���غ�����Ϣ*/
        case WINKS_ENGINE_SUBSTATE_ELEVEN:/*���غ�����Ϣ*/
            AttrId = 0x7FF8;
            break;
#ifdef WINKS_DEL_PV2_0    
        case WINKS_ENGINE_SUBSTATE_SEVEN:/*ʵʱͬ��������*/
            AttrId = 0x7FF7;
            break;
#endif
        case WINKS_ENGINE_SUBSTATE_EIGHT:/*DIY�ϴ�*/
            {
            Winks_DiyInfo_s *pDiyInfo = (Winks_DiyInfo_s *)data;
            if(pDiyInfo)
            {
            if(pDiyInfo->image_count)
            AttrId = 0x7FF6;
            else
                AttrId = 0x7FF3;
            }
            else
            {
                Winks_EngDiyComInfo_s *pULInfo = winks_EngGetDiyContext();
                if(pULInfo && pULInfo->image_count)
                {
                    AttrId = 0x7FF6;
                }
            }
            }
            break;
        case WINKS_ENGINE_SUBSTATE_NINE:/*Enable״̬���ע�ᣬ�����������ͨѶ��������ѯע��״̬*/
            AttrId = 0x7FF9;
            break;
        case WINKS_ENGINE_SUBSTATE_TEN:/*Enable״̬���ע�ᣬ��ͨѶ��������ѯע��״̬*/
            AttrId = 0x7FFD;
            break;
        default:
            Winks_EngErr("error case");
            break;
        }
        break;
    }

    default:
        break;
    }

    if(AttrId)
        iRet = winks_PackDataAndSend(AttrId, data, &PD_ErrCode);
    else
        iRet = WINKS_RETURN_FAILURE;

    if(iRet != WINKS_RETURN_SUCCESS)
    {
        if(PD_ErrCode)
            return winks_EngRetErr(PD_ErrCode);
        else
            return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
    }
    return iRet;
}

#if 0
static int winks_PhoneBookEventProc(const void* data, unsigned int size )
{
    winks_PbModifiedMsgInfo_s *msgData_p = (winks_PbModifiedMsgInfo_s *)data;
    Winks_PhonebookEvent_e type = 0;
    int iRet = 0;
    
    /*Winks_printHex((unsigned char *)msgData_p, (unsigned short)sizeof(winks_PbModifiedMsgInfo_s), (unsigned char*)"struct winks_PbModifiedMsgInfo_s");*/
    Winks_assert(size == sizeof(winks_PbModifiedMsgInfo_s));

    switch(msgData_p->type)
    {
    case WINKS_PBEVENT_ADD: 
        type=E_PB_ADD; 
        iRet = Winks_log_pb_event(type, msgData_p->number1, NULL);
        break;
    case WINKS_PBEVENT_DELETE: 
        type=E_PB_RMV; 
        iRet = Winks_log_pb_event(type, msgData_p->number1, NULL);
        break;
    case WINKS_PBEVENT_REPLACE: 
        type=E_PB_MOD; 
        iRet = Winks_log_pb_event(type, msgData_p->number2, msgData_p->number1);
        break;
    default: type=0xff; break;
    }
    return iRet;
}
#endif
/*
static unsigned long winks_Get_CCStrgLocation()
{
    unsigned long CC_StrgLocation = E_STORAGE_PHONE;
    if( (Winks_get_setting_value(E_SETTING_STORAGE_LOCATION, &CC_StrgLocation) != WINKS_RETURN_SUCCESS) ||
        ((CC_StrgLocation != E_STORAGE_PHONE)&&(CC_StrgLocation != E_STORAGE_MCARD)) )
        CC_StrgLocation = E_STORAGE_PHONE;

    return CC_StrgLocation;
}
*/
/* ��ʱ������� */
static int winks_TimerExpiryProc(void *data, unsigned long size)
{
    int iRet = 0;
    WINKS_TIMER_ID *timer_id_ptr = (WINKS_TIMER_ID *)data;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->PeriodPushTimer == *timer_id_ptr)
    {
    #ifdef WINKS_DEBUG
        Winks_DateTime_s v_dateTime;
        Winks_GetSysDateTime( &v_dateTime );
        Winks_printf( "PeriodPushTimer Expiry: %04d-%02d-%02d %02d:%02d:%02d state[%d][%d]\r\n", 
            v_dateTime.year, v_dateTime.month, v_dateTime.day,
            v_dateTime.hour, v_dateTime.min, v_dateTime.sec,
            Winks_engineContext_p->curState, Winks_engineContext_p->subState);
    #endif
        if( (Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED) && \
            (Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO) )
        {
        #if 0//def WINKS_SUPPORT_MEMORY_CARD
            unsigned long CC_StrgLocation = winks_Get_CCStrgLocation();
            if( (CC_StrgLocation==E_STORAGE_PHONE) ||
                ((CC_StrgLocation==E_STORAGE_MCARD) && (Winks_IsMemoryCardExist() == 1)) )
        #endif
            {
                iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_PUSH, NULL, 0);
                Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
            }
            /* ��鶨ʱ���������Ƿ���������·���һ�£�����һ����������ʱ�� */
            winks_StartPeriodPushTimer();
        }
        else
        {
            /* ����æ����ʱ120����������ʱ���·��� */
            winks_DelayPeriodPushTimer();
        }
    }
    else if(Winks_engineContext_p->httpTimer == *timer_id_ptr)
    {
        Winks_printf( "WARN: httpTimer Expiry %d %d, close http\r\n", Winks_engineContext_p->http, Winks_engineContext_p->httpTimer);
        winks_HttpCloseProc(Winks_engineContext_p->http);
    }
    else if(Winks_engineContext_p->BuildCacheTimer == *timer_id_ptr)
    {
        int StrgStatus = Winks_StrgGetStatus();

        switch(StrgStatus)
        {
        case WINKS_STRG_CARD_STATUS_NO_CCCACHE:
            iRet = winks_EngInitCCCache();
            break;
        case WINKS_STRG_CARD_STATUS_OLD_CARD:
            iRet = winks_EngRebulidCCCache();
            break;
        case WINKS_STRG_CARD_STATUS_NEW_CARD:
            Winks_StrgDeleteDiskFiles(2);
            iRet = winks_EngRebulidCCCache();
            break;
        case WINKS_STRG_CARD_STATUS_NOT_EXIST:
            iRet = winks_EngRebulidCCCache();
            break;
        case WINKS_STRG_CARD_STATUS_OK_CARD:
            iRet = WINKS_RETURN_SUCCESS;
            break;
        default:
            iRet = WINKS_RETURN_FAILURE;
            break;
        }
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            winks_StartBuildCacheTimer();/* �����ȴ��¸����ڼ�� */
        }
        else
        {
            winks_StopBuildCacheTimer();
        }
    }
#ifdef WINKS_SUPPORT_3G
    else if(Winks_engineContext_p->CallWaitTimer == *timer_id_ptr)
    {
        winks_StopCallWaitTimer();

        if(Winks_engineContext_p->isCallCCOk == 1)
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_CALLCC_TIMEOUT);
        else if(Winks_engineContext_p->isCallCCOk != 0xff)
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_CALLCC_FAIL);
    }
#endif /* WINKS_SUPPORT_3G */
    else if(Winks_engineContext_p->getImsiTimer == *timer_id_ptr)
    {
        char buff[WINKS_IMSI_LEN + 1];
        Winks_DestroyTimer(Winks_engineContext_p->getImsiTimer);
        Winks_engineContext_p->getImsiTimer = WINKS_TIMER_NULL;
        if(WINKS_ENGINE_STATE_START == Winks_engineContext_p->curState)
        {
            Winks_mem_set(buff, 0, sizeof(buff));

        #ifdef WINKS_DUAL_SIM
            if(Winks_engineContext_p->SimStatus & 0xff00)/* �������ҵ�1�ſ���,����1����һ�ſ� */
            {
                if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0100)/* �ҵ��˿�1,��ͼȡ��2��imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(1) == TRUE);
                    Winks_engineContext_p->CurInquireSimNo = 2;
                    iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
                }
                else/* �ҵ��˿�2,��ͼȡ��1��imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    Winks_assert(Winks_GetActiveSim()==0);/* Init,Start�׶β��ܵ���Winks_SetActiveSim����Winks_GetImsi���� */

                    Winks_engineContext_p->CurInquireSimNo = 1;
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                }
            }
            else/* û���ҵ�Sim��,���μ�����ѯ���ſ� */
            {
                if(Winks_engineContext_p->CurInquireSimNo == 1)
                {
                    Winks_engineContext_p->CurInquireSimNo = 2;
                    iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
                }
                else
                {
                    Winks_engineContext_p->CurInquireSimNo = 1;
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                }
            }
        #else /* WINKS_DUAL_SIM */
            iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
        #endif /* WINKS_DUAL_SIM */
            if(iRet == WINKS_RETURN_SUCCESS)/* ��ȡimsi�ɹ� */
            {
            #ifdef WINKS_DUAL_SIM
                if(Winks_engineContext_p->CurInquireSimNo == 1)
                    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_REQ, (void *)buff, sizeof(buff));
                else
                    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_2_REQ, (void *)buff, sizeof(buff));
            #else /* WINKS_DUAL_SIM */
                Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_REQ, (void *)buff, sizeof(buff));
            #endif /* WINKS_DUAL_SIM */
            }
            else if(iRet == WINKS_RETURN_FAILURE)/* ��ȡimsiʧ�� */
            {
            #ifdef WINKS_DUAL_SIM
                if(Winks_engineContext_p->CurInquireSimNo == 1)
                    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_REQ, NULL, 0);
                else
                    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_2_REQ, NULL, 0);
            #else /* WINKS_DUAL_SIM */
                Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_REQ, NULL, 0);
            #endif /* WINKS_DUAL_SIM */
            }
            else /* WINKS_RETURN_WAIT_MSG */
            {
                /* �ȴ�Winks_GetImsi�����淢WINKS_MSG_IMSI_REQ��Ϣ */
            }
        }
    }
    else
    {
         if((Winks_engineContext_p->curState == WINKS_ENGINE_STATE_DISABLED) && \
            (Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_FOUR))
        {
            Winks_EngPrtReg_s *p = (Winks_EngPrtReg_s *)(Winks_engineContext_p->communicationContext_p);
            Winks_assert(p != NULL);
            if(p->timerId == *((WINKS_TIMER_ID *)data))
            {
                /*Connect Server*/
                Winks_DestroyTimer(p->timerId);
                p->timerId = WINKS_TIMER_NULL;

                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    NULL);
            }
        }
    }
    return WINKS_RETURN_SUCCESS;
}

static void winks_EngDelTmpFile()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED)
    {   
        switch(Winks_engineContext_p->app)
        {
        case WINKS_ENGINE_APP_INIT_CACHE:
            Winks_StrgFLDeleteAll();
            break;
        case WINKS_ENGINE_APP_REBULID_CACHE:
            Winks_StrgFLDeleteAll();
            Winks_StrgNDDeleteAll();
            break;
        case WINKS_ENGINE_APP_UPDATE_FRIEND:
        case WINKS_ENGINE_APP_PERIOD_UPDATE:
            Winks_StrgNDDeleteAll();
            break;
        case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
            
            break;
        default:
            break;
        }
    }
}

static void winks_EngRestartCacheTimerCheck()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED)
    {   
        switch(Winks_engineContext_p->app)
        {
        case WINKS_ENGINE_APP_INIT_CACHE:/* δ���Cache��ʼ�� */
        case WINKS_ENGINE_APP_REBULID_CACHE:/* δ���Cache�ؽ� */
            winks_StartBuildCacheTimer();
            break;
        default:
            break;
        }
    }
}

/* ���������س�����Ϣ�����洦���� 
   ����ֵΪWINKS_RETURN_SUCCESS�������õ��˴�������Ҫ��������
*/
static int winks_HttpRetErrHandler(const void* data, unsigned int size )
{
    int iRet = WINKS_RETURN_FAILURE;
    unsigned long error_code = 0;
    Winks_EngineToUiReturnMsg retMsg;
    Winks_EngineToUiReturnMsg *pSendMsg = NULL;
    unsigned long SendMsgLen = 0;
    unsigned long high16Bit = 0;
    unsigned long low16Bit = 0;
    unsigned long FlagSendMsgToUi = 1;
    unsigned long FlagForceBreak = 0;
    unsigned long FlagForceFree = 0;
    void *inData_p = NULL;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    error_code = *((unsigned long *)data);
    Winks_printf("Call winks_HttpRetErrHandler! ret_code[%lx] [%d][%d]\r\n", error_code,Winks_engineContext_p->curState, Winks_engineContext_p->subState);

    high16Bit = (error_code&0xffff0000)>>16;
    low16Bit = (error_code&0x0000ffff);
    
    Winks_mem_set(&retMsg, 0, sizeof(retMsg));
    retMsg.err_code = error_code;
    retMsg.err_state = ((unsigned long)(Winks_engineContext_p->curState) & 0x000000ff)<<8;
    retMsg.err_state |=  ((unsigned long)(Winks_engineContext_p->subState) & 0x000000ff);

    switch(high16Bit)
    {
    /*�ͻ�����ش�����뷶Χ��        (0001H  ---  0FFFH)*/
    case 0x0001:/*a)    δ֪����                        �� 0001H ��*/
        Winks_printf("EEEE[%04x]: δ֪���� \r\n", high16Bit);
        break;
        
    /*���÷�������ش�����룺        (1001H  ---  1FFFH)*/
    case 0x1002:/*a)    ���÷������ڲ�����                �� 1002H ��*/
        Winks_printf("EEEE[%04x]: ���÷������ڲ����� \r\n", high16Bit);
        break;

    case 0x1003:/*b)    ����Ч��ͨ�ŷ�������ַ            �� 1003H ��*/
        Winks_printf("EEEE[%04x]: ����Ч��ͨ�ŷ�������ַ \r\n", high16Bit);
        break;
        
    case 0x1004:/*c)    ����Ч�Ķ����ط�����            �� 1004H ��*/
        Winks_printf("EEEE[%04x]: ����Ч�Ķ����ط����� \r\n", high16Bit);
        break;

    /*ͨѶ��������ش�����룺        (2001H  ---  2FFFH)*/
    case 0x2002:/*a)    ͨ�ŷ������ڲ�����                �� 2002H ��*/
        Winks_printf("EEEE[%04x]: ͨ�ŷ������ڲ����� \r\n", high16Bit);
        break;
        
    /*���ݿ���ش�����룺            (4001H  ---  4FFFH)*/
    
    /*��������                    (5001H  ---  FFFFH)*/
    case 0x1001:/*a)    Э���������                    ��1001H��*/
        Winks_printf("EEEE[%04x]: Э��������� \r\n", high16Bit);
        break;
        
    case 0x4001:/*b)    �ͻ�û�ж�����Ϣ                ��4001H��*/
        Winks_printf("EEEE[%04x]: �ͻ�û�ж�����Ϣ \r\n", high16Bit);
        break;
        
    case 0x4002:/*c)    �ͻ�û��ƥ�䵱ǰʱ��Ķ��Ʋ���     ��4002H��*/
        Winks_printf("EEEE[%04x]: �ͻ�û��ƥ�䵱ǰʱ��Ķ��Ʋ��� \r\n", high16Bit);
        break;
        
    case 0x4003:/*d)    ���ʺϴ��ͺſͻ��˵Ĳ���        ��4003H��*/
        Winks_printf("EEEE[%04x]: ���ʺϴ��ͺſͻ��˵Ĳ��� \r\n", high16Bit);
        break;
        
    case 0x4004:/*e)    �����ļ�δ�ҵ�                    ��4004H��*/
        Winks_printf("EEEE[%04x]: �����ļ�δ�ҵ� \r\n", high16Bit);
        break;
        
    case 0x4005:/*f)    �û�����Ӫ������                ��4005H��*/
        Winks_printf("EEEE[%04x]: �û�����Ӫ������ \r\n", high16Bit);
        break;
        
    case 0x5001:/*g)    ��������Ϊ�մ���                 ��5001H��*/
        Winks_printf("EEEE[%04x]: ��������Ϊ�մ��� \r\n", high16Bit);
        break;
        
    case 0x5002:/*h)    �Է�����Ϊ�մ���                 ��5002H��*/
        Winks_printf("EEEE[%04x]: �Է�����Ϊ�մ��� \r\n", high16Bit);
        break;
        
    case 0x5003:/*i)    CRCУ�����                          ��5003H��*/
        Winks_printf("EEEE[%04x]: CRCУ����� \r\n", high16Bit);
        break;
        
    case 0x5004:/*j)    �ϴ��ļ�����                      ��5004H��*/
        Winks_printf("EEEE[%04x]: �ϴ��ļ����� \r\n", high16Bit);
        break;
        
    case 0x5005:/*k)    �ϴ��ļ���Ϣ�������            ��5005H��*/
        Winks_printf("EEEE[%04x]: �ϴ��ļ���Ϣ������� \r\n", high16Bit);
        break;
        
    case 0x5006:/*l)    �û�δע��                        ��5006H��*/
    {        
        Winks_printf("EEEE[%04x]: �û�δע�� \r\n", high16Bit);
        switch(Winks_engineContext_p->curState)
        {
        case WINKS_ENGINE_STATE_ENABLED:
        case WINKS_ENGINE_STATE_SHOWB:
            switch(Winks_engineContext_p->subState)
            {
                case WINKS_ENGINE_SUBSTATE_FIVE:
                case WINKS_ENGINE_SUBSTATE_ELEVEN:
                case WINKS_ENGINE_SUBSTATE_TEN:
                    /*�ں���winks_HttpRetMsgProc�д���*/
                    break;
                default:
                    /*����������δע�ᣬ�˵�δע��ȴ�״̬*/
                    Winks_CurStateRollback(1);
                    retMsg.err_code = WINKS_MSG_ENG2UI_UNREG_USER;
                    iRet = WINKS_RETURN_SUCCESS;
                    break;
            }
            break;
        case WINKS_ENGINE_STATE_DISABLED:
        case WINKS_ENGINE_STATE_SHOWA:
            switch(Winks_engineContext_p->subState)
            {
                case WINKS_ENGINE_SUBSTATE_TOW:
                case WINKS_ENGINE_SUBSTATE_FOUR:
                    /*�ں���winks_HttpRetMsgProc�д���*/
                    break;
                default:
                    /*����������δע�ᣬ�˵�δע��ȴ�״̬*/
                    Winks_CurStateRollback(1);
                    iRet = WINKS_RETURN_SUCCESS;
                    break;
            }
            break;
        default:
            break;
        }
        
        break;
    }
    case 0x5007:/*m)    �û���������ϴ�������             ��5007H��*/
        Winks_printf("EEEE[%04x]: �û���������ϴ������� \r\n", high16Bit);
        break;
        
    case 0x5008:/*n)    ������������Ч��ֹͣ�������     ��5008H��*/
        Winks_printf("EEEE[%04x]: ������������Ч��ֹͣ������� \r\n", high16Bit);
        break;
        
    case 0x5009:/*o)    �û��ϴ��ʺ����޸�ʧ��            ��5009H��*/
        Winks_printf("EEEE[%04x]: �û��ϴ��ʺ����޸�ʧ�� \r\n", high16Bit);
        break;
        
    case 0x500A:/*p)    �����û������ҵ��������        ��500AH��*/
        Winks_printf("EEEE[%04x]: �����û������ҵ�������� \r\n", high16Bit);
        break;
        
    case 0x500B:/*q)    �ͻ��˵绰���������Կմ���        ��500BH��*/
        Winks_printf("EEEE[%04x]: �ͻ��˵绰���������Կմ��� \r\n", high16Bit);
        break;
        
    case 0x500C:/*r)    �ϴ�����ֵ����                    ��500CH��*/
        Winks_printf("EEEE[%04x]: �ϴ�����ֵ���� \r\n", high16Bit);
        break;
        
    case 0x500D:/*s)    �����°汾���                    ��500DH��*/
        Winks_printf("EEEE[%04x]: �����°汾��� \r\n", high16Bit);
        break;
        
    case 0x500E:/*t)    �û������շ��û�                ��500EH��*/
        Winks_printf("EEEE[%04x]: �û������շ��û� \r\n", high16Bit);
        break;
        
    case 0x500F:/*u)    ���»�ȡͨѶ��������ַ            ��500FH��*/
        Winks_printf("EEEE[%04x]: ���»�ȡͨѶ��������ַ \r\n", high16Bit);
        break;
        
    case 0x5010:/*v)    ��ѯ���û���Ϣ�鲻����            ��5010H��*/
        Winks_printf("EEEE[%04x]: ��ѯ���û���Ϣ�鲻���� \r\n", high16Bit);
        break;
        
    case 0x5011:/*w)    �ֻ��ռ䲻��                    ��5011H��*/
        Winks_printf("EEEE[%04x]: �ֻ��ռ䲻�� \r\n", high16Bit);
        break;
        
    case 0x5012:/*x)    �û�û���κκ�����Ϣ            ��5012H��*/
        Winks_printf("EEEE[%04x]: �û�û���κκ�����Ϣ \r\n", high16Bit);
        break;
        
    /*δ�������*/
    default:
        Winks_printf("EEEE[%04x]: δ������� \r\n", high16Bit);
        break;
    }

    if(iRet == WINKS_RETURN_FAILURE)/*δ����ֱ�ӷ���*/
    {
        return iRet;
    }
    if(FlagSendMsgToUi == 1)
    {
        retMsg.err_state <<= 16;
        if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_COMPLETE)
            retMsg.err_state |= (unsigned long)(retMsg.err_state & 0xff000000)>>16;/*curstate=ǰһ״̬*/
        else
            retMsg.err_state |= (unsigned long)(Winks_engineContext_p->curState & 0x000000ff)<<8;
        retMsg.err_state |= (unsigned long)(Winks_engineContext_p->subState & 0x000000ff);
        
        if(pSendMsg)
        {
            pSendMsg->err_state = retMsg.err_state;
            Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", pSendMsg->err_state, pSendMsg->err_code);
            pSendMsg->err_code = Winks_Eng2UiMsgMap(pSendMsg);
            winks_EngRetMsg2UI(pSendMsg, SendMsgLen);/* msg_body�����ݷ��� */
            Winks_salFree(pSendMsg);
            pSendMsg = NULL;
        }
        else
        {
            Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
            retMsg.err_code = Winks_Eng2UiMsgMap(&retMsg);
            winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
        }
    }
    Winks_assert(pSendMsg == NULL);
    
    /*��ɱ����λ��״̬��*/
    if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_COMPLETE)
    {
        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
        FlagForceFree = 1;
    }
    else if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
    {
        winks_EngRestartCacheTimerCheck();
        Winks_SubStateRollback();
    }
    else
    {
        if(FlagForceBreak == 0)
        {
            winks_EngineSendDataToServer(
                Winks_engineContext_p->curState, 
                Winks_engineContext_p->subState, 
                inData_p);
        }
    }
    if((FlagForceFree == 1) && (Winks_engineContext_p->communicationContext_p != NULL))
    {
        Winks_salFree(Winks_engineContext_p->communicationContext_p);
        Winks_engineContext_p->communicationContext_p = NULL;
    }
    
    if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
    {
        //Winks_engineContext_p->IsPeriodUpdating = 0;
        Winks_engineContext_p->lastState = 0;
    }
    return iRet;
}

/* �������UI�İ���msg_body����Ϣ */
static Winks_EngineToUiReturnMsg * winks_AppendUiMsgBody(Winks_EngineToUiReturnMsg *pOrgMsg, void *msgbody, int msgbodylen, unsigned long *RetLen)
{
    Winks_EngineToUiReturnMsg *pRetMsg;
    
    *RetLen = sizeof(Winks_EngineToUiReturnMsg) + msgbodylen;/* ��������1�ֽ�*/
    pRetMsg = (Winks_EngineToUiReturnMsg *)Winks_salAlloc(*RetLen);
    if(pRetMsg == NULL)
    {
        Winks_printf("ERROR: Winks_salAlloc(%d)\r\n", *RetLen);
        Winks_assert(pRetMsg != NULL);
        return NULL;
    }
    memcpy(pRetMsg, pOrgMsg, sizeof(Winks_EngineToUiReturnMsg));
    memcpy(pRetMsg->msg_body, msgbody, msgbodylen);
    
    return pRetMsg;
}

/* Ϊ��ʱ�����ķ���ռ� Winks_engineContext_p->communicationContext_p */
static void *winks_CommContextAlloc(int AllocSize)
{
    void * p = Winks_salAlloc(AllocSize);
    if(p == NULL)
    {
        Winks_printf("ERROR: Winks_salAlloc(%d)\r\n", AllocSize);
        Winks_assert(p != NULL);
    }
    else
        Winks_mem_set(p, 0, AllocSize);
    return p;
}

static int winks_EngRecordCacheTime()
{
    int iRet = WINKS_RETURN_FAILURE;
    unsigned long   CacheTime = 0;
    Winks_DateTime_s    v_dateTime;
    
    iRet = Winks_GetSysDateTime(&v_dateTime);

    if( (iRet == WINKS_RETURN_SUCCESS) &&
        ((iRet = Winks_SysTime2CacheTime(&v_dateTime, &CacheTime)) == WINKS_RETURN_SUCCESS) )
    {
        return Winks_StrgSetCacheTime(CacheTime);
    }

    return iRet;
}

static int winks_EngNoInPhoneBook(char *pNo)
{
    int iRet;
    unsigned long phone_cnt = 0, sim_cnt = 0;
    unsigned long cnt = 0;
    Winks_PhoneBookItem_s PBItem;

    if(pNo == NULL || pNo[0]=='\0')
    {
        Winks_fail("winks_EngNoInPhoneBook parameter");
        return 0;
    }
        
    iRet = Winks_GetPhonebookCount(&phone_cnt, &sim_cnt);
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
        return 0;
    }

    if(!phone_cnt && !sim_cnt)/* �绰��Ϊ�� */
    {
        return 0;
    }

    cnt = 0;
    while(cnt < phone_cnt + sim_cnt)/* �����ӵ绰���뵽�����ͻ��� */
    {
        int iNo = 0, NumberLen = 0;

        memset(&PBItem, 0, sizeof(PBItem));
        if(cnt < phone_cnt)
            iRet = Winks_ReadPhonebook(1, cnt, &PBItem);
        else
            iRet = Winks_ReadPhonebook(2, cnt-phone_cnt, &PBItem);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            break;
        }
        
        for(iNo=0; iNo<WINKS_MAX_NUMBER_COUNT && PBItem.number[iNo][0]; iNo++)
        {
            NumberLen = strlen(PBItem.number[iNo]);
            if((NumberLen <= 0) || (NumberLen >= WINKS_FRIEND_NO_LEN))/*��Ч�ĵ绰����,����*/
            {
                cnt++;
                continue;
            }

            if(strcmp(PBItem.number[iNo], pNo) == 0)
                return (int)(cnt+1);
        }
        cnt++;
    }

    return 0;
}

static int winks_EngTrimFL(char *pFriendList, int FriendListLen)
{
    int     len, no_len, Count = 0;
    char  * pFL;
    char  * pNo;

    if(pFriendList == NULL || pFriendList[0] == '\0')
        return WINKS_RETURN_FAILURE;
    
    len = 0;
    pNo = pFL = pFriendList;
    while(len < FriendListLen)
    {
        if(pFL[len] == '|')
        {
            pFL[len] = '\0';

            no_len = strlen(pNo);
            if(no_len > 2)
            {
                if(winks_EngNoInPhoneBook(pNo) > 0)/* �������� */
                {
                    if(Winks_StrgFLAdd(Winks_SkipPlus(pNo)) == WINKS_RETURN_SUCCESS)
                        Count++;
                    Winks_mem_set(pNo, 0, no_len);
                }
            }
            pNo += no_len+1;
        }
        len++;
    }
    
    len = 0;
    pNo = pFriendList;
    while(len < FriendListLen)/* �������� */
    {
        if(pNo[len])
        {
            no_len = strlen(&pNo[len]);
            if(no_len > 2)
            {
                if(Winks_StrgFLAdd(Winks_SkipPlus(&pNo[len])) == WINKS_RETURN_SUCCESS)
                    Count++;
				else
					break;
            }
            len += no_len+1;
        }
        else
        {
            len++;
        }
    }
#ifdef WINKS_DEBUG
{
    int cnt = 0;
    int Idx = 0;
    char PhoneNo[20];
    Winks_printf("winks_EngTrimFL: FLCnt=%d \r\n", Winks_StrgFLCnt());
    while(cnt < Count)
    {
        Idx = Winks_StrgFLGet(Idx, PhoneNo);
        if(Idx <= 0)
            break;
        Winks_printf("%4d: %s\r\n", cnt+1, PhoneNo);
        cnt++;
    }
}
#endif
    return Count;
}


/* ����Ƿ���Ҫ�ؽ�Cache */
static void winks_EngCheckCCCache()
{
    int NeedRebulidCCCache = 0;
    int StrgStatus = Winks_StrgGetStatus();
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    switch(Winks_engineContext_p->app)
    {
    case WINKS_ENGINE_APP_UPDATE_FRIEND:
#ifdef WINKS_SUPPORT_3G
    case WINKS_ENGINE_APP_CALL_DOWNLOAD:
#endif /* WINKS_SUPPORT_3G */
    case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
        if(StrgStatus != WINKS_STRG_CARD_STATUS_OK_CARD)
            NeedRebulidCCCache = 1;
        break;
    case WINKS_ENGINE_APP_PERIOD_UPDATE:
        if( StrgStatus == WINKS_STRG_CARD_STATUS_OLD_CARD )
            NeedRebulidCCCache = 1;
        break;
    default:
        break;
    }

    /* �ؽ�Cache */
    if(NeedRebulidCCCache)
        winks_StartBuildCacheTimer();
}


static int winks_EngCCIsExist(Winks_EngCCDownloadContext_s *pDownCC)
{
    unsigned char NoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned short retIdx = 0;
    WinksStrgFile_UserInfo_s UsrInfo;
    
    Winks_assert(pDownCC != NULL);
    
    Winks_PhoneNoStr2Bcd(pDownCC->ND_NO, sizeof(pDownCC->ND_NO), WINKS_STRG_NOBCD_LEN, NoBcd);
    if(Winks_StrgSearchUser((char *)NoBcd, &retIdx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND)
    {
        return Winks_StrgCCIsExist(UsrInfo.CCIdx, NULL);
    }

    return 0;
}

static int winks_EngCreateCcReq(Winks_EngCCDownloadContext_s *pDownCC, Winks_CcReq_s *pCcReq)
{
    Winks_assert(pDownCC != NULL);
    Winks_assert(pCcReq != NULL);
    
    strncpy(pCcReq->ID, pDownCC->ND_ID, MIN(sizeof(pCcReq->ID), sizeof(pDownCC->ND_ID)));
    pCcReq->cc_type = pDownCC->ND_cc_type;
    pCcReq->img_cnt = pDownCC->ND_img_cnt;
    
    pCcReq->img_idx = pDownCC->img_idx;

    return WINKS_RETURN_SUCCESS;
}

static int winks_EngCreateCcInfo(Winks_EngCCDownloadContext_s *pDownCC, WinksStrgFile_CCInfo_s *pCCInfo)
{
    Winks_assert(pDownCC != NULL);
    Winks_assert(pCCInfo != NULL);
    
    Winks_mem_set(pCCInfo, 0, sizeof(WinksStrgFile_CCInfo_s));
    Winks_strncpy(pCCInfo->ID, pDownCC->ND_ID, MIN(sizeof(pCCInfo->ID), sizeof(pDownCC->ND_ID)));
    pCCInfo->img_type = pDownCC->img_type;
    pCCInfo->img_cnt = pDownCC->ND_img_cnt;
    pCCInfo->interval = pDownCC->ND_interval;

    return WINKS_RETURN_SUCCESS;
}

static void winks_EngStatResult(Winks_EngCCDownloadContext_s *pDownCC, unsigned long error_code, int lineno)
{
    if(pDownCC->DLS_Cnt)
    {
        if(error_code == 0x00000000)
            pDownCC->DLS_Fin++;
        else
            pDownCC->DLS_Err++;
        pDownCC->DLS_Idx++;
    }

    if(pDownCC->DLS_Err + pDownCC->DLS_Fin != pDownCC->DLS_Idx)
    {
        Winks_printf("winks_EngStatResult(L%04d): %d+%d ?= %d \r\n", lineno, pDownCC->DLS_Err, pDownCC->DLS_Fin, pDownCC->DLS_Idx);
        Winks_assert(pDownCC->DLS_Err + pDownCC->DLS_Fin == pDownCC->DLS_Idx);
    }
    if(pDownCC->DLS_Idx > pDownCC->DLS_Cnt)
    {
        Winks_printf("winks_EngStatResult(L%04d): %d ?<= %d \r\n", lineno, pDownCC->DLS_Idx, pDownCC->DLS_Cnt);
        pDownCC->DLS_Cnt = pDownCC->DLS_Idx;
        //Winks_assert(pDownCC->DLS_Idx <= pDownCC->DLS_Cnt);
    }
}

#define WINKS_DOWNCC_OP_WASHOUT     0x01    /* ��Ҫ��̭ */
#define WINKS_DOWNCC_OP_REC_ND      0x02    /* ��Ҫ��¼��<nd.dat> */

static int winks_EngDealUserBaseInfo(Winks_EngCCDownloadContext_s *pDownCC, int Op)
{
    int iRet;
    int NeedUpdateCC = 1;    /* UsrInfo.CCIdx need search? */
    int NeedUpdateGreet = 1; /* UsrInfo.GrtOff need search? */
    unsigned short retIdx = 0;
    unsigned char NoBcd[WINKS_STRG_NOBCD_LEN];
    WinksStrgFile_UserInfo_s UsrInfo;
    WinksStrgFile_GreetInfo_s GreetInfo;

    int IsFriend;
    int UsedCCFriendNum = 0;
    unsigned short FriendIdx;
    
    IsFriend = Winks_StrgUserIsFriend(pDownCC->ND_NO, &FriendIdx);
    
    Winks_mem_set(&UsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    Winks_mem_set(&GreetInfo, 0, sizeof(WinksStrgFile_GreetInfo_s));
    Winks_PhoneNoStr2Bcd(pDownCC->ND_NO, sizeof(pDownCC->ND_NO), WINKS_STRG_NOBCD_LEN, NoBcd);
    if(Winks_StrgSearchUser((char *)NoBcd, &retIdx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND)
    {
        int     Occupy;
        WinksStrgFile_CCInfo_s CCInfo;

        if(pDownCC->ND_ID[0] == 0)
        {
            /* ����û�и��£����� */
            NeedUpdateCC = 0;
        }
        else if(Winks_StrgCCIsExist(UsrInfo.CCIdx, &CCInfo))
        {
            if(strcmp(pDownCC->ND_ID, CCInfo.ID) == 0)
            {
                Winks_printf("winks_EngDealUserBaseInfo OKOK : user cc not changed! (%d, 0x%04x)\r\n", retIdx, UsrInfo.CCIdx);
                /* ����û�и��£����� */
                NeedUpdateCC = 0;
            }
            else 
            {
                if(Winks_StrgCCUserNum(UsrInfo.CCIdx, &UsedCCFriendNum) <= 1)
                {
                    /* �û���������,û�б���ʹ��ԭ����,ɾ�� */
                    if(Winks_StrgDeleteCCImage(UsrInfo.CCIdx) != WINKS_RETURN_SUCCESS)
                        return WINKS_RETURN_FAILURE;
                }
            #if 0
                if(IsFriend && UsedCCFriendNum == 1)
                    Winks_StrgPreTakeupDelete(UsrInfo.CCIdx);
            #endif
            }
        }

        if(pDownCC->GreetValid == 0)
            NeedUpdateGreet = 0;
        else if( UsrInfo.GrtOff && 
            Winks_StrgGetGreet(UsrInfo.GrtOff, &GreetInfo) == WINKS_RETURN_SUCCESS)
        {
            Occupy = GreetInfo.Occupy;
            Occupy -= sizeof(GreetInfo) - sizeof(GreetInfo.GreetStr);
            if(Occupy >= pDownCC->GreetLen)
            {
                /* ԭ�����ʺ���,��ԭ�洢λ��������Ŀǰ�ʺ��� */
                NeedUpdateGreet = 0;
            }
            else
            {
                /* ɾ���ʺ��� */
                Winks_mem_set(GreetInfo.NoBcd, 0, WINKS_STRG_NOBCD_LEN);
                Winks_StrgSetGreet(UsrInfo.GrtOff, &GreetInfo);
            }
        }
    }
    else
    {
        if(pDownCC->GreetValid == 0)
            NeedUpdateGreet = 0;

        if(Winks_StrgSearchUserEpt(&retIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
        /* û�еط���Ÿ��û� */
        {
            if(Op&WINKS_DOWNCC_OP_WASHOUT)
            {
                if(pDownCC->FileSize)
                    iRet = Winks_StrgFreeDiskSpace(pDownCC->FileSize);
                else
                    iRet = Winks_StrgFreeDiskSpace(EACH_CC_TAKEUP_SIZE);

                if(iRet != WINKS_RETURN_SUCCESS)
                    return WINKS_RETURN_FAILURE;
            }
            else
                return WINKS_RETURN_FAILURE;/* �û������� */
        }
        Winks_mem_cpy(UsrInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
    }

    /* ���Ҵ洢������Ϣ��λ */
    if(NeedUpdateCC)
    {
        if( Winks_StrgSearchCC(pDownCC->ND_ID, &UsrInfo.CCIdx) == WINKS_STRG_SEARCH_RESULT_FOUND &&
            Winks_StrgCCIsExist(UsrInfo.CCIdx, NULL) )
        {
            /* �����Ѵ��� */
            Winks_printf("winks_EngDealUserBaseInfo OKOK : user cc changed!, but new cc exist (%d, 0x%04x)\r\n", retIdx, UsrInfo.CCIdx);
            NeedUpdateCC = 0;
        #if 0
            if(IsFriend)
            {
                unsigned int FileSize;
                
                if( Winks_StrgGetCCImageSize(UsrInfo.CCIdx, &FileSize) != WINKS_RETURN_SUCCESS ||
                    Winks_StrgPreTakeup(UsrInfo.CCIdx, EACH_CC_TAKEUP_SIZE-FileSize, NULL) )
                    return WINKS_RETURN_FAILURE;
            }
        #endif
            if(Winks_StrgSetUserInfo(retIdx, &UsrInfo) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
        else
        {
            iRet = Winks_StrgSearchCCEpt(EACH_CC_TAKEUP_SIZE, &UsrInfo.CCIdx);
            if(iRet != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
            {
                if(Op&WINKS_DOWNCC_OP_WASHOUT)
                {
                    if(pDownCC->FileSize)
                        iRet = Winks_StrgFreeDiskSpace(pDownCC->FileSize);
                    else
                        iRet = Winks_StrgFreeDiskSpace(EACH_CC_TAKEUP_SIZE);

                    if(iRet != WINKS_RETURN_SUCCESS)
                        return WINKS_RETURN_FAILURE;

                    iRet = Winks_StrgSearchCCEpt(EACH_CC_TAKEUP_SIZE, &UsrInfo.CCIdx);
                }
                else
                    return WINKS_RETURN_FAILURE;    /* ���������ܴ洢���� */
            }
        }
    }
    
    /* ���ұ����ʺ����λ */
    if(NeedUpdateGreet)
    {
        if(Winks_StrgSearchGreetEpt(pDownCC->GreetLen, &UsrInfo.GrtOff) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
        {
            /*  �����ʺ����ļ�,�Ա��ڳ��ռ�;Ȼ������ҳ����пռ� */
            if(Winks_StrgTrimGreet() != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;/* �ļ�ϵͳ���� */

            /* �������ʺ����ļ�,����û�ط�����ʺ���,���� */
            if(Winks_StrgSearchGreetEpt(pDownCC->GreetLen, &UsrInfo.GrtOff) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
                return WINKS_RETURN_FAILURE;    /* ���������ܴ洢�ʺ��� */
        }
    }
    
    /* ������ز�����Ϣ */
    if(NeedUpdateCC)
    {
        Winks_printf("winks_EngDealUserBaseInfo NDND : need download cc!(%d, 0x%04x)\r\n", retIdx, UsrInfo.CCIdx);
    #if 0
        if(IsFriend)
            iRet = Winks_StrgPreTakeup(UsrInfo.CCIdx, EACH_CC_TAKEUP_SIZE, NULL);
        else
     #endif
            iRet = Winks_StrgTmpTakeup(UsrInfo.CCIdx, EACH_CC_TAKEUP_SIZE, NULL);

        pDownCC->CC_Idx = UsrInfo.CCIdx;
        if( (iRet == WINKS_RETURN_SUCCESS) &&
            (Op&WINKS_DOWNCC_OP_REC_ND) )
        {
            if(Winks_StrgNDAdd(&pDownCC->ND) != WINKS_RETURN_SUCCESS)
            {
            #if 0
                if(IsFriend)
                    Winks_StrgPreTakeupDelete(UsrInfo.CCIdx);
                else
            #endif
                    Winks_StrgTmpTakeupDelete(UsrInfo.CCIdx);
                    
                return WINKS_RETURN_FAILURE;    /* �������������������Ϣ */
            }
        }
        
        if(iRet != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    /* �洢�ʺ��� */
    if(pDownCC->GreetValid && UsrInfo.GrtOff)
    {
        Winks_mem_set(GreetInfo.GreetStr, 0, GreetInfo.GreetLen);
        Winks_mem_cpy(GreetInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
        GreetInfo.GreetLen = pDownCC->GreetLen;
        Winks_mem_cpy(GreetInfo.GreetStr, pDownCC->GreetStr, pDownCC->GreetLen);
        iRet = Winks_StrgSetGreet(UsrInfo.GrtOff, &GreetInfo);
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    }

    /* �洢���������Ϣ���û�������ʺ��������Ϣ */
    if(NeedUpdateGreet || NeedUpdateCC)
    {
        if(NeedUpdateCC)
        {
            WinksStrgFile_CCInfo_s CCInfo;
            iRet = winks_EngCreateCcInfo(pDownCC, &CCInfo);
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            iRet = Winks_StrgSetCCInfo(UsrInfo.CCIdx, &CCInfo);
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
        }
        
        Winks_mem_cpy(UsrInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
        iRet = Winks_StrgSetUserInfo(retIdx, &UsrInfo);
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    }
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_EnginePauseBackSrv()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if( (Winks_engineContext_p->curState >= WINKS_ENGINE_STATE_ENABLED) &&
        (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO) )
    {
        switch(Winks_engineContext_p->app)
        {
        case WINKS_ENGINE_APP_INIT_CACHE:
        case WINKS_ENGINE_APP_REBULID_CACHE:
        case WINKS_ENGINE_APP_PERIOD_UPDATE:
        case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
            winks_EngRestartCacheTimerCheck();
            return Winks_SubStateRollback();
        }
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_EngineResumeBackSrv(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();


    return WINKS_RETURN_SUCCESS;
}

/* �������Http���ذ�����Ϣ������ */
static int winks_HttpRetMsgProc(const void* data, unsigned int size )
{
    unsigned long error_code = 0;
    Winks_EngineToUiReturnMsg retMsg;
    Winks_EngineToUiReturnMsg *pSendMsg = NULL;
    unsigned long SendMsgLen = 0;
    unsigned long high16Bit = 0;
    unsigned long low16Bit = 0;
    unsigned long FlagSendMsgToUi = 1;
    unsigned long FlagForceBreak = 0;
    unsigned long FlagForceFree = 0;
    void *inData_p = NULL;
    Winks_CcReq_s CcReq;
    int iRet = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(data != NULL);
    Winks_assert(size == sizeof(unsigned long));

    error_code = *((unsigned long *)data);
    Winks_printf("Call winks_HttpRetMsgProc! ret_code[%lx] [%d][%d]\r\n", error_code,Winks_engineContext_p->curState, Winks_engineContext_p->subState);

    high16Bit = (error_code&0xffff0000)>>16;
    low16Bit = (error_code&0x0000ffff);

    Winks_mem_set(&retMsg, 0, sizeof(retMsg));
    retMsg.err_code = error_code;
    retMsg.err_state = ((unsigned long)(Winks_engineContext_p->curState) & 0x000000ff)<<8;
    retMsg.err_state |=  ((unsigned long)(Winks_engineContext_p->subState) & 0x000000ff);

    Winks_engineContext_p->lastState = retMsg.err_state;
    
    if(high16Bit != 0)/*server return error code!*/
    {
        iRet = winks_HttpRetErrHandler(data, size);
        if(iRet == WINKS_RETURN_SUCCESS) 
        {
            return 0;
        }
    }
    
    switch(Winks_engineContext_p->curState)
    {
    case WINKS_ENGINE_STATE_DISABLED:
    {
        switch(Winks_engineContext_p->subState)
        {
        case WINKS_ENGINE_SUBSTATE_ONE:
        {
            if(error_code == 0x00000000)
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TOW;
            else
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            break;
        }
        case WINKS_ENGINE_SUBSTATE_TOW:
        {
            if(error_code == 0x00000000)
            {
                Winks_EngPrtReg_s *data_p = (Winks_EngPrtReg_s *)Winks_engineContext_p->communicationContext_p;
                if(data_p && data_p->msg[0])
                {
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, data_p->msg, sizeof(Winks_EngineRetMsgBody_s), &SendMsgLen);
                }
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                FlagForceFree = 1;
            }
            else if(high16Bit == 0x5006)
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_THREE;/*��ʾ�û���Ҫ������ע��*/
                retMsg.err_code = 0x00000000;
                FlagForceBreak = 1;
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            }
            break;
        }
        /*
        case WINKS_ENGINE_SUBSTATE_THREE:
        {
            if(error_code == 0x00000000)
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FOUR;
            else
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            break;
        }*/
        case WINKS_ENGINE_SUBSTATE_FOUR:/*��ѯע���Ƿ�ɹ�*/
        {
            Winks_EngPrtReg_s * p = (Winks_EngPrtReg_s *)(Winks_engineContext_p->communicationContext_p);
            Winks_assert(p != NULL);
            
            if(error_code == 0x00000000)
            {
                Winks_EngPrtReg_s *data_p = (Winks_EngPrtReg_s *)Winks_engineContext_p->communicationContext_p;
                if(data_p && data_p->msg[0])
                {
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, data_p->msg, sizeof(Winks_EngineRetMsgBody_s), &SendMsgLen);
                }
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                FlagForceFree = 1;
            }
            else if(++p->count < 2/*6*/)
            {
                unsigned long timerOut = 0;
                
                FlagSendMsgToUi = 0;
                /*++(p->count);*/
                Winks_assert(p->timerId == WINKS_TIMER_NULL);

                if(p->count<1/*3*/)
                    timerOut = p->shortTime;
                else
                    timerOut = p->longTime;
                Winks_assert(timerOut > 5);
                if(timerOut < 5) timerOut = 60;                 
                
                p->timerId = Winks_CreateTimer(
                    Winks_engineContext_p->engChn, 
                    WINKS_SECONDS2TICKS(timerOut), 
                    WINKS_TIMER_AUTO_START);
                if(p->timerId == WINKS_TIMER_NULL)
                {
                    Winks_assert(p->timerId > 0);
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                }

                FlagForceBreak = 1;/* �ȶ�ʱ��ʱ�䵽�ٷ���������ѯ�Ƿ�ע����Ϣ */
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            }
            break;
        }
        case WINKS_ENGINE_SUBSTATE_FIVE:/*���ػ�����Ϣ*/
        {
            if(error_code == 0x00000000)
            {
                Winks_EngCCDownloadContext_s *pDownCC = NULL;
                
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if( (pDownCC = winks_EngGetCCDownContext()) == NULL )
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                    retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                    break;
                }

                if((iRet = Winks_StrgNDCnt()) < 0)
                    iRet = 0;
                pDownCC->DLS_Cnt = (unsigned short)iRet;
                pDownCC->DLS_Idx = 0;
                
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_SIX;
                /* ������Ϣ�ɹ�,ֱ�ӵ�state6���ز�������û��break */
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                break;
            }
        }
        case WINKS_ENGINE_SUBSTATE_SIX:/*����Ƭͷ����Ӫ�̲���*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
         
            if( low16Bit || 
                ((high16Bit != 0) && (high16Bit != 0x4003) && (high16Bit != 0x4004)) ) /* ���ش��� �� ���������طǲ������ */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                pDownCC->img_idx++;/* һ��ID��Ӧ�Ķ���ͼƬ�����λ�ȡ */
                if(pDownCC->img_idx >= pDownCC->ND_img_cnt)/* ��һ������ID�ļ���ͼƬȫ�������� */
                {
                    if(pDownCC->ND_img_cnt)
                        winks_EngStatResult(pDownCC, error_code, __LINE__);

                    Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s)-sizeof(pDownCC->DLS)-sizeof(pDownCC->ReqIdx));
                    pDownCC->ReqIdx = Winks_StrgNDGet(pDownCC->ReqIdx, &pDownCC->ND);

                    if(pDownCC->ND_ID[0])
                    {
                        Winks_assert(pDownCC->ReqIdx > 0);
                        //Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_SIX;
                        //pDownCC->img_idx = 0;
                    }
                    else
                    {
                        /* ������� */
                        char ImsiBuf[WINKS_IMSI_LEN+1];
                        /* ��IMSI�� */
                        iRet = Winks_GetImsi(WINKS_CHN_NULL, ImsiBuf, sizeof(ImsiBuf));
                        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                        ImsiBuf[WINKS_IMSI_LEN] = 0;
                        Winks_SetRecentlyActivatedImsi(ImsiBuf,WINKS_IMSI_LEN);

                        Winks_StrgNDDeleteAll();
                        
                        /* ������ʱ���·��� */
                        winks_StartPeriodPushTimer();
                        /* ������ʼ��Cache��ʱ�� */
                        winks_StartBuildCacheTimer();
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                        Winks_engineContext_p->curState= WINKS_ENGINE_STATE_ENABLED;
                    }
                }

                if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_SIX)
                {
                    winks_EngCreateCcReq(pDownCC, &CcReq);
                    inData_p = &CcReq;
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
                }
            }
            break;
        }

        default:
            Winks_EngErr("error case");
            break;
        }
        
        break;
    }

    case WINKS_ENGINE_STATE_ENABLED:
    {
        switch(Winks_engineContext_p->subState)
        {
        case WINKS_ENGINE_SUBSTATE_ONE:/*���ػ�����Ϣ*/
        {
            if(error_code == 0x00000000)
            {
                Winks_EngCCDownloadContext_s *pDownCC = NULL;
                
                //Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((pDownCC = winks_EngGetCCDownContext()) == NULL)
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                    retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                    break;
                }
                
                if((iRet = Winks_StrgNDCnt()) < 0)
                    iRet = 0;
                pDownCC->DLS_Cnt = (unsigned short)iRet;
                pDownCC->DLS_Idx = 0;
                
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TOW;
                
                /* ��鶨ʱ���������Ƿ���������·���һ�£�����һ����������ʱ�� */
                if(Winks_engineContext_p->app == WINKS_ENGINE_APP_PERIOD_UPDATE)
                    winks_StartPeriodPushTimer();

                /* ������Ϣ�ɹ�,ֱ�ӵ�state6���ز�������û��break */
            }         
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                break;
            } 
        }
        case WINKS_ENGINE_SUBSTATE_TOW:/*����Ƭͷ����Ӫ�̡����û�����*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
         
            if( low16Bit || 
                ((high16Bit != 0) && (high16Bit != 0x4003) && (high16Bit != 0x4004)) ) /* ���ش��� �� ���������طǲ������ */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                pDownCC->img_idx++;/* һ��ID��Ӧ�Ķ���ͼƬ�����λ�ȡ */
                if(pDownCC->img_idx >= pDownCC->ND_img_cnt)/* ��һ������ID�ļ���ͼƬȫ�������� */
                {
                    if(pDownCC->ND_img_cnt)
                        winks_EngStatResult(pDownCC, error_code, __LINE__);

                    Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s)-sizeof(pDownCC->DLS)-sizeof(pDownCC->ReqIdx));
                    pDownCC->ReqIdx = Winks_StrgNDGet(pDownCC->ReqIdx, &pDownCC->ND);
                    
                    if(pDownCC->ND_ID[0])
                    {
                        Winks_assert(pDownCC->ReqIdx > 0);
                        //Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TOW;
                        //pDownCC->img_idx = 0;
                    }
                    else
                    {
                        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_PERIOD_UPDATE)
                        {
                            int     FriendCnt;
                            char    FriendNo[WINKS_STRG_PHONE_LEN];

                            Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s));
                            FriendCnt = Winks_StrgFriendCnt();
                            pDownCC->ReqIdx = Winks_StrgFriendGet( pDownCC->ReqIdx, 
                                                                   FriendNo, sizeof(FriendNo),
                                                                   NULL, 0);
                            if(pDownCC->ReqIdx == WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
                            {
                                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                            }
                            else if(pDownCC->ReqIdx < 0)
                            {
                                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                                retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                            }
                            else
                            {
                                Winks_assert(FriendCnt > 0);
                                pDownCC->DLS_Cnt = (unsigned short)FriendCnt;
                                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                                Winks_mem_cpy(pDownCC->ND_NO, FriendNo, MIN(sizeof(FriendNo), sizeof(pDownCC->ND_NO)));
                                inData_p = pDownCC->ND_NO;
                            }
                        }
                        else
                            Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                        Winks_StrgNDDeleteAll();
                    }
                }

                if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_TOW)
                {
                    winks_EngCreateCcReq(pDownCC, &CcReq);
                    inData_p = &CcReq;
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
                }
            }
            break;
        }
        case WINKS_ENGINE_SUBSTATE_THREE:/*���غ����б�*/
        {
            if(error_code == 0x00000000)
            {
                char  * pFL;
                
                Winks_assert(Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE);
                pFL = (char *)Winks_engineContext_p->communicationContext_p;
                iRet = winks_EngTrimFL(pFL, strlen(pFL));
                if(Winks_engineContext_p->communicationContext_p)
                {
                    Winks_salFree(Winks_engineContext_p->communicationContext_p);
                    Winks_engineContext_p->communicationContext_p = NULL;
                }
                if(iRet > 0)
                {
                    Winks_EngCCDownloadContext_s *pDownCC = NULL;

                    Winks_assert(Winks_StrgFLCnt() == iRet);
                    
                    Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                    if((pDownCC = winks_EngGetCCDownContext()) == NULL)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                        break;
                    }

                    pDownCC->ReqIdx = Winks_StrgFLGet(pDownCC->ReqIdx, pDownCC->ND_NO);
                    iRet = Winks_StrgFLCnt();
                    if(iRet<0)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                    }
                    else if(pDownCC->ND_NO[0] == 0)
                    {
                        Winks_assert(pDownCC->ReqIdx < 0);
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                    }
                    else
                    {
                        Winks_assert(pDownCC->ReqIdx > 0);
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                        pDownCC->DLS_Cnt = (unsigned short)iRet;
                        pDownCC->DLS_Idx = 0;
                        inData_p = pDownCC->ND_NO;
                    }
                }
                else
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                }
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            }
            break;
        }
        case WINKS_ENGINE_SUBSTATE_FIVE:/*�����û�������Ϣ*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            if( high16Bit == 0x5006 )
            {
                retMsg.err_code = error_code = 0x40010000;
                high16Bit = 0x4001;
            }
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* ���ش��� �� ���������طǲ������ */
            {
                if(Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE)
                {
                    if((high16Bit == 0) && (low16Bit == WINKS_MSG_ENG2UI_STORAGE_LIMIT))
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                        break;
                    }
                }
            #ifdef WINKS_SUPPORT_3G
                else if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
                {
                    Winks_EngCCDownloadContext_s *pDownCC = NULL;
                    
                    pDownCC = winks_EngGetCCDownContext();
                    Winks_assert(pDownCC != NULL);
                    
                    Winks_StrgULAdd(pDownCC->ND_NO);
                    winks_EngCheckCCCache();
                }
            #endif /* WINKS_SUPPORT_3G */
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                break;
            }

            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
        #ifdef WINKS_SUPPORT_3G
            if( Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD &&
                pDownCC->ND_ID[0] )
                Winks_strncpy(Winks_engineContext_p->Call_ID, pDownCC->ND_ID, sizeof(Winks_engineContext_p->Call_ID));
        #endif /* WINKS_SUPPORT_3G */
            if(pDownCC->ND_ID[0] && !winks_EngCCIsExist(pDownCC))
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_SIX;
                winks_EngCreateCcReq(pDownCC, &CcReq);
                inData_p = &CcReq;
                pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
                winks_EngCheckCCCache();
                
                break;
            }
            else
            {
                /* ֱ�ӵ�state6������һ�û�������Ϣ������û��break */
            }
        }
        case WINKS_ENGINE_SUBSTATE_SIX:/*���ݲ���ID���ز�����Դ*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
            
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* ���ش��� �� ���������طǲ������ */
            {
            #ifdef WINKS_SUPPORT_3G
                if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
                {
                    Winks_StrgULAdd(pDownCC->ND_NO);
                    winks_EngCheckCCCache();
                }
            #endif /* WINKS_SUPPORT_3G */
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                retMsg.err_code = 0x00000000;
                
                winks_EngStatResult(pDownCC, error_code, __LINE__);
                
                if( (Winks_engineContext_p->app == WINKS_ENGINE_APP_UPDATE_FRIEND) ||
                    (Winks_engineContext_p->app == WINKS_ENGINE_APP_PERIOD_UPDATE) )
                {
                    char    FriendNo[WINKS_STRG_PHONE_LEN];
                    Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s)-sizeof(pDownCC->DLS)-sizeof(pDownCC->ReqIdx));
                    pDownCC->ReqIdx = Winks_StrgFriendGet( pDownCC->ReqIdx, 
                                                           FriendNo, sizeof(FriendNo),
                                                           NULL, 0);
                    if(pDownCC->ReqIdx == WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
                    {
                        Winks_assert(pDownCC->DLS_Idx <= pDownCC->DLS_Cnt);
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                    }
                    else if(pDownCC->ReqIdx < 0)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                        Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                    }
                    else
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                        Winks_mem_cpy(pDownCC->ND_NO, FriendNo, MIN(sizeof(FriendNo), sizeof(pDownCC->ND_NO)));
                        inData_p = pDownCC->ND_NO;
                    }
                }
                else if( (Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE) ||
                         (Winks_engineContext_p->app == WINKS_ENGINE_APP_RECRUIT_DOWNLOAD) )
                {
                    if(Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE)
                        iRet = Winks_StrgFLDelete(pDownCC->ND_NO);
                    else
                        iRet = Winks_StrgULDelete(pDownCC->ND_NO);
                    if(iRet != 0)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                        Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                    }
                    else
                    {
                        Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s)-sizeof(pDownCC->DLS)-sizeof(pDownCC->ReqIdx));
                        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE)
                            pDownCC->ReqIdx = Winks_StrgFLGet(pDownCC->ReqIdx, pDownCC->ND_NO);
                        else
                            pDownCC->ReqIdx = Winks_StrgULGet(pDownCC->ND_NO);
                        if(pDownCC->ReqIdx == WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
                        {
                            Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                            if(Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE)
                            {
                                winks_EngRecordCacheTime();
                            }
                        }
                        else if(pDownCC->ReqIdx < 0)
                        {
                            Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                            retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                            Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                        }
                        else
                        {
                            Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                            inData_p = pDownCC->ND_NO;
                        }
                    }
                }
            #ifdef WINKS_SUPPORT_3G
                else if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
                {
                    WinksStrgFile_NeedDown_s NeedDown;

                    if(Winks_engineContext_p->isCallCCOk != 0xff)
                    {
                        winks_StopCallWaitTimer();
                        Winks_engineContext_p->isCallCCOk = 0xff;
                        retMsg.err_code = WINKS_MSG_ENG2UI_CALLCC_OK;
                    }
                    if((iRet = Winks_StrgNDCnt()) < 0)
                        iRet = 0;
                    pDownCC->DLS_Cnt = (unsigned short)iRet;
                    pDownCC->DLS_Idx = 0;                    
                    Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s));
                    pDownCC->ReqIdx = Winks_StrgNDGet(pDownCC->ReqIdx, &NeedDown);
                    if(pDownCC->ReqIdx == WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                    }
                    else if(pDownCC->ReqIdx < 0)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                        Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                    }
                    else
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TOW;
                        Winks_mem_cpy(&pDownCC->ND, &NeedDown, sizeof(WinksStrgFile_NeedDown_s));
                        winks_EngCreateCcReq(pDownCC, &CcReq);
                        inData_p = &CcReq;
                        pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
                    }
                }
            #endif /* WINKS_SUPPORT_3G */
                else/* if(Winks_engineContext_p->app == WINKS_ENGINE_APP_REBULID_CACHE)*/
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                    retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                    Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                }

                pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
            }
            break;
        }
#ifdef WINKS_DEL_PV2_0    
        case WINKS_ENGINE_SUBSTATE_SEVEN:/*ʵʱ����ͬ��������*/
        {
            if(error_code == 0x00000000)
            {
                Winks_SelfSetting_s *self_info = (Winks_SelfSetting_s *)Winks_engineContext_p->communicationContext_p;
                if(self_info)
                {
                    unsigned char   value = 0;
                    
                    if(self_info->ccSwitch)
                        value |= E_OTHER_SHOW_ME;
                    if(self_info->ShowCC)
                        value |= E_I_SHOW_OTHER;
                    
                    Winks_set_setting_value(E_SETTING_SHOW_CTRL, (unsigned long)value);
                }
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            }
            break;
        }
#endif
        case WINKS_ENGINE_SUBSTATE_EIGHT:/*DIY�ϴ�*/
        {
            if(error_code == 0x00000000)
            {
                Winks_EngDiyComInfo_s *pULInfo = winks_EngGetDiyContext();
                
                Winks_assert(pULInfo != NULL);
                
                Winks_printf("��[%d/%d]�ļ���[%d/%d]���ϴ��ɹ�!\r\n", pULInfo->curIndex+1, pULInfo->image_count, pULInfo->subIndex+1, pULInfo->subTotalCount);
                if(pULInfo->subTotalCount>1)/* �ֿ��ϴ�*/
                {
                    pULInfo->subIndex++;
                    if(pULInfo->subIndex >= pULInfo->subTotalCount)/*�Ѵ���һ���ļ�*/
                    {
                        pULInfo->curIndex++;
                        pULInfo->curFileSize = 0;/* ���ļ���С��0 �Ա���������ϴ��¸��ļ�*/
                    }
                }
                else/* �����ļ��ϴ�*/
                {
                    pULInfo->curIndex++;
                    pULInfo->curFileSize = 0;/* ���ļ���С��0 �Ա���������ϴ��¸��ļ�*/
                }

                if(pULInfo->curIndex >= pULInfo->image_count)/*�����ļ��ϴ����*/
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                }
                else
                {
                #if 1   //renzhan add progress msg
                    Winks_EngDiyProgInfo_s DiyProgInfo;
                    DiyProgInfo.image_count = pULInfo->image_count;
                    DiyProgInfo.curIndex = pULInfo->curIndex;
                    DiyProgInfo.subTotalCount= pULInfo->subTotalCount;
                    DiyProgInfo.subIndex = pULInfo->subIndex;
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, &DiyProgInfo, sizeof(Winks_EngDiyProgInfo_s), &SendMsgLen);
                #endif
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_EIGHT;
                }
                pULInfo->RetryCnt = 0;
            }
            else
            {
                Winks_EngDiyComInfo_s *pULInfo = winks_EngGetDiyContext();
                
                Winks_assert(pULInfo != NULL);
                    pULInfo->RetryCnt++;
                if(pULInfo->RetryCnt < DIY_UPLOAD_RETRY_MAX)
                {
                    inData_p = (void *)NULL;
                }
                else
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                }
            }
            break;
        }
        
        case WINKS_ENGINE_SUBSTATE_NINE:
        {
            if(error_code == 0x00000000)
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TEN;
            else
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            break;
        }
        case WINKS_ENGINE_SUBSTATE_TEN:
        {
            if(error_code == 0x00000000)
            {
                Winks_EngPrtReg_s *data_p = (Winks_EngPrtReg_s *)Winks_engineContext_p->communicationContext_p;
                if(data_p && data_p->msg[0])
                {
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, data_p->msg, sizeof(Winks_EngineRetMsgBody_s), &SendMsgLen);
                }
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
            }
            else if(high16Bit == 0x5006)
            {
                Winks_CurStateRollback(0);
                retMsg.err_code = 0x00000000;
                FlagForceBreak = 1;
                /* �Ѽ����û�����ע�ᣬ�ȴ����Ͷ��ţ��������뱾��״̬��һ�£� */
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_THREE;
                {
                Winks_EngPrtReg_s *data_p = (Winks_EngPrtReg_s *)Winks_engineContext_p->communicationContext_p;
                if((data_p==NULL) || (Winks_SetCommServerIpPort(&data_p->server)!=0))
                {
                    Winks_SubStateRollback();
                    /* �Ѽ����û�����ע�ᣬ�������뱾��״̬��һ�£�����ע������ */
                    FlagForceBreak = 0;
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ONE;
                    Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                    if((Winks_engineContext_p->communicationContext_p = Winks_salAlloc(sizeof(Winks_EngPrtReg_s))) == NULL)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                        break;
                    }
                    Winks_mem_set(Winks_engineContext_p->communicationContext_p, 0, sizeof(Winks_EngPrtReg_s));
                }
                }
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            }
            break;
        }
        case WINKS_ENGINE_SUBSTATE_ELEVEN: /* �����û�������Ϣ */
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);

            if( high16Bit == 0x5006 )
            {
                retMsg.err_code = error_code = 0x40010000;
                high16Bit = 0x4001;
            }
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* ���ش��� �� ���������طǲ������ */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                break;
            }
            
            retMsg.err_code = 0x00000000;

                Winks_StrgFLDelete(pDownCC->ND_NO);
            if(pDownCC->ND_ID[0])
            {
                if(winks_EngDealUserBaseInfo(pDownCC, WINKS_DOWNCC_OP_REC_ND) != WINKS_RETURN_SUCCESS)
                {
                    if(Winks_StrgFLDeleteAll() != WINKS_RETURN_SUCCESS)
                    {
                        /* �ļ�ϵͳ���ܷ���[������Usb״̬],��ʱ�����̨���� */
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        break;
                    }
                    else
                    {
                        /* û���㹻���̿ռ䣬����״̬12����ʼ���ز��� */
                    }
                }
            }

            winks_EngStatResult(pDownCC, error_code, __LINE__);
            Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s)-sizeof(pDownCC->DLS)-sizeof(pDownCC->ReqIdx));
            pDownCC->ReqIdx = Winks_StrgFLGet(pDownCC->ReqIdx, pDownCC->ND_NO);
            if(pDownCC->ReqIdx == WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
            {
                Winks_assert(pDownCC->ND_NO[0] == 0);
                error_code = 0;
                iRet = Winks_StrgNDCnt();
                if(iRet == 0)
                {
                    /* ������� */
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                    winks_EngRecordCacheTime();
                    break;
                }
                else
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TWELVE;
                }
                Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s));
                pDownCC->DLS_Cnt = (unsigned short)iRet;
                /* ֱ�ӵ�state12�����²�������û��break */
            }
            else if(pDownCC->ReqIdx < 0)
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                retMsg.err_code = WINKS_ENG_ERROR_CALL_FUNC;
                break;
            }
            else
            {
                //Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ELEVEN;
                inData_p = pDownCC->ND_NO;
                pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
                break;
            }
        }
        case WINKS_ENGINE_SUBSTATE_TWELVE: /* ���ݲ���ID���ز�����Դ */
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
         
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* ���ش��� �� ���������طǲ������ */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                retMsg.err_code = 0x00000000;
                
                pDownCC->img_idx++;/* һ��ID��Ӧ�Ķ���ͼƬ�����λ�ȡ */
                if(pDownCC->img_idx >= pDownCC->ND_img_cnt)/* ��һ������ID�ļ���ͼƬȫ�������� */
                {
                    if(pDownCC->ND_img_cnt)
                        winks_EngStatResult(pDownCC, error_code, __LINE__);
                    Winks_mem_set(pDownCC, 0, sizeof(Winks_EngCCDownloadContext_s)-sizeof(pDownCC->DLS)-sizeof(pDownCC->ReqIdx));
                    pDownCC->ReqIdx = Winks_StrgNDGet(pDownCC->ReqIdx, &pDownCC->ND);
                    if(pDownCC->ND_ID[0])
                    {
                        Winks_assert(pDownCC->ReqIdx > 0);
                        //Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_TWELVE;
                        //pDownCC->img_idx = 0;
                    }
                    else
                    {
                        /* ������� */
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_COMPLETE;
                        winks_EngRecordCacheTime();
                    }
                }

                if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_TWELVE)
                {
                    winks_EngCreateCcReq(pDownCC, &CcReq);
                    inData_p = &CcReq;
                    pSendMsg = winks_AppendUiMsgBody(&retMsg, &pDownCC->DLS_Cnt, sizeof(Winks_EngDLCCInfo_s), &SendMsgLen);
                }
            }
            break;
        }
        default:
            Winks_EngErr("error case");
            break;
        }
        
        break;
    }
        
    default:
        Winks_printf("ERROR: get err_code [%lx], state[%d] substate[%d]\r\n",retMsg.err_code, Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        /*Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;*/
        break;
    }

    
    if(FlagSendMsgToUi == 1)
    {
        retMsg.err_state <<= 16;
        if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_COMPLETE)
            retMsg.err_state |= (unsigned long)(retMsg.err_state & 0xff000000)>>16;/*curstate=ǰһ״̬*/
        else
            retMsg.err_state |= (unsigned long)(Winks_engineContext_p->curState & 0x000000ff)<<8;
        retMsg.err_state |= (unsigned long)(Winks_engineContext_p->subState & 0x000000ff);
        /*
        if(Winks_engineContext_p->IsPeriodUpdating == 2)
        {
            retMsg.err_state &= 0xff00ffff;
            retMsg.err_state |= 0x00020000;
        }
        */
        if(pSendMsg)
        {
            pSendMsg->err_state = retMsg.err_state;
            Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", pSendMsg->err_state, pSendMsg->err_code);
            pSendMsg->err_code = Winks_Eng2UiMsgMap(pSendMsg);
            winks_EngRetMsg2UI(pSendMsg, SendMsgLen);/* msg_body�����ݷ��� */
        }
        else
        {
            Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
            retMsg.err_code = Winks_Eng2UiMsgMap(&retMsg);
            winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
        }
    }
    /* �ͷ���Ϣ����Դ */
    if(pSendMsg != NULL)
    {
        Winks_salFree(pSendMsg);
        pSendMsg = NULL;
    }
    
    /*��ɱ����λ��״̬��*/
    if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_COMPLETE)
    {
    #ifdef WINKS_SUPPORT_3G
        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
        {
            if(Winks_StrgGetCCShow( Winks_engineContext_p->Call_NO, Winks_engineContext_p->CallType, 
                                    Winks_engineContext_p->pCCShow ) == WINKS_RETURN_SUCCESS)
            {
                Winks_StrgUnlockCC();
                Winks_StrgLockCC(Winks_engineContext_p->pCCShow->CC_Idx);
            }
        }
    #endif /* WINKS_SUPPORT_3G */
        winks_EngDelTmpFile();
        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
        FlagForceFree = 1;
    }
    else if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
    {
    #ifdef WINKS_SUPPORT_3G
        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
        {
            if(Winks_engineContext_p->isCallCCOk != 0xff)
                Winks_engineContext_p->isCallCCOk = 2;
        }
    #endif /* WINKS_SUPPORT_3G */
        winks_EngRestartCacheTimerCheck();
        Winks_SubStateRollback();
    }
    else
    {
#ifdef WINKS_ENGINE_RUN_BY_STEP
        unsigned long val = 0; 
        Winks_GetOsalTestBitFlag(31,&val);
#endif
        if(!FlagForceBreak)
        {
#ifdef WINKS_ENGINE_RUN_BY_STEP
            if(!val || (inData_p!=NULL))
            {
#endif
                winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    inData_p);
            }
#ifdef WINKS_ENGINE_RUN_BY_STEP
            else/*step by step.*/
            {
                Winks_SetOsalTestBitFlag(30,FlagForceFree);
                return 0;
            }
#endif
        }
    }

    if((FlagForceFree == 1) && (Winks_engineContext_p->communicationContext_p != NULL))
    {
        Winks_salFree(Winks_engineContext_p->communicationContext_p);
        Winks_engineContext_p->communicationContext_p = NULL;
    }

    if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
    {
        //Winks_engineContext_p->IsPeriodUpdating = 0;
        Winks_engineContext_p->lastState = 0;
    }

    return 0;
}

#ifdef WINKS_ENGINE_RUN_BY_STEP
/*���浥�����п���*/
static int winks_EngineRunStepbyStep(void)
{
    unsigned long val = 0; 
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    Winks_GetOsalTestBitFlag(30,&val);

    if(Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO)
    {
        winks_EngineSendDataToServer(
            Winks_engineContext_p->curState, 
            Winks_engineContext_p->subState, 
            NULL);
    }
    if((val == 1) && (Winks_engineContext_p->communicationContext_p != NULL))
    {
        Winks_salFree(Winks_engineContext_p->communicationContext_p);
        Winks_engineContext_p->communicationContext_p = NULL;
    }
    return 0;
}
#endif

static int winks_IsUsbState()
{
    unsigned long UsbState;
    return Winks_GetUsbState(&UsbState);
}

static int winks_EngCallEvtInit(int CallType, char *pNo)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_strncpy(Winks_engineContext_p->Call_NO, Winks_SkipPlus(pNo), sizeof(Winks_engineContext_p->Call_NO));
    if(strlen(Winks_engineContext_p->Call_NO) <= 2)
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(Winks_engineContext_p->Call_ID, 0, sizeof(Winks_engineContext_p->Call_NO));
    Winks_engineContext_p->CallType = CallType;
    Winks_GetSysDateTime(&Winks_engineContext_p->CallStartTime);

    return WINKS_RETURN_SUCCESS;
}

static int winks_EngRecordCallEvt()
{
    int iRet = WINKS_RETURN_FAILURE;
    WinksStrgFile_CallInfo_s CallInfo;
    unsigned long   CallTime = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_EngRecordCallEvt\r\n");
    if(strlen(Winks_engineContext_p->Call_NO) <= 2)
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(&CallInfo, 0, sizeof(CallInfo));
    if((iRet = Winks_SysTime2CacheTime(&Winks_engineContext_p->CallStartTime, &CallTime)) == WINKS_RETURN_SUCCESS)
    {
        Winks_DateTime_s    CurrTime;
        Winks_DateTime_s    DiffTime;
        
        CallInfo.CallClock = CallTime;
        CallTime = 0;
        Winks_GetSysDateTime(&CurrTime);
        if(Winks_GetTimeDifference(&CurrTime, &Winks_engineContext_p->CallStartTime, &DiffTime) == WINKS_RETURN_SUCCESS)
        {
            /* CallInfo.CallTime���65535��(18Сʱ),����1�춼��65535�� */
            if(DiffTime.year || DiffTime.month || DiffTime.day || DiffTime.hour>18)
                CallTime = 0xffff;
            else
            {
                CallTime += DiffTime.hour*3600;
                CallTime += DiffTime.min*60;
                CallTime += DiffTime.sec;
            }
        }
        CallInfo.CallTime = (unsigned short)CallTime;
        CallInfo.CallType = Winks_engineContext_p->CallType;
        Winks_assert(Winks_engineContext_p->Call_NO[0]);
        Winks_strncpy(CallInfo.NO, Winks_engineContext_p->Call_NO, sizeof(CallInfo.NO));
        if(Winks_engineContext_p->Call_ID[0])
            Winks_strncpy(CallInfo.ID, Winks_engineContext_p->Call_ID, sizeof(CallInfo.ID));
        {
            Winks_DateTime_s *pCST = &Winks_engineContext_p->CallStartTime;
            Winks_printf("StartTime: %04d-%02d-%02d %02d:%02d:%02d\r\n", 
                pCST->year, pCST->month, pCST->day,
                pCST->hour, pCST->min, pCST->sec);
            Winks_printf("CallClock: %lu\r\n", CallInfo.CallClock);
            Winks_printf("CallTime : %lu\r\n", CallInfo.CallTime);
            Winks_printf("CallType : %lu\r\n", CallInfo.CallType);
            Winks_printf("Call_No  : %s\r\n", CallInfo.NO);
            Winks_printf("Call_ID  : %s\r\n", CallInfo.ID);
        }
        Winks_mem_set(Winks_engineContext_p->Call_NO, 0, sizeof(Winks_engineContext_p->Call_NO));
        Winks_mem_set(Winks_engineContext_p->Call_ID, 0, sizeof(Winks_engineContext_p->Call_ID));
        Winks_engineContext_p->CallType = 0;
        Winks_mem_set(&Winks_engineContext_p->CallStartTime, 0, sizeof(Winks_engineContext_p->CallStartTime));
        return Winks_StrgCallEvtAdd(&CallInfo);
    }

    return iRet;
}

/* ������Ϣ������ں��� */
static int winks_EngineIdleHandler(unsigned long msg, void *data, unsigned long size)
{
    int iRet = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    switch(msg)
    {
        case WINKS_MSG_ENGINE_HTTP:
            winks_HttpMsgProc(data, size);
            return 0;
        case WINKS_MSG_ENGINE_RETMSG:
            winks_HttpRetMsgProc(data, size);
            return 0;
        case WINKS_MSG_PHONE_BOOK_MODIFIED:
    #if 0
            winks_PhoneBookEventProc(data, size);
    #endif
            return 0;
        case WINKS_MSG_TIMER_EXPIRY:
            winks_TimerExpiryProc(data, size);
            return 0;
        case WINKS_MSG_TEST:
            if(strlen(data) == WINKS_TEST_KEY_WORDS_LEN)
                Winks_TestModeManager((char *)data);            
            return 0;
        case WINKS_FSINVALID_CAUSED_USB:
        {
            if(data)
            {
                Winks_printf("########## winks_EngineIdleHandler! state=[%d][%d] WINKS_MSG_USB_DETECT %d\r\n",
						Winks_engineContext_p->curState, Winks_engineContext_p->subState,msg, *(unsigned char *)data);
                if(*(unsigned char *)data == WINKS_USB_PLUG_IN)
                {
                    if(Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO)
                    {
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        winks_EngRetErrDirectly(WINKS_ENG_ERROR_CALL_FUNC);
                        winks_EngRestartCacheTimerCheck();
                        Winks_SubStateRollback();
                    }
                }
            }
            return 0;
        }
        default:
            break;
    }
    
    Winks_printf("########## winks_EngineIdleHandler! state=[%d][%d] msg=[0x%04lx]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState,msg);

    switch(Winks_engineContext_p->curState)
    {
    case WINKS_ENGINE_STATE_INIT:
    {
        switch (msg)
        {
        case WINKS_MSG_CHN_INIT:
        {
            int iRet = 0;
            char buff[WINKS_IMSI_LEN+1];
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_START;
            Winks_engineContext_p->engChn = *((WINKS_CHN_ID *)data);
            Winks_engineContext_p->http = WINKS_HTTP_NULL;
            Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
            /*Get all call event.*/
            Winks_SetWaitingEvent(Winks_engineContext_p->engChn, WINKS_EVENT_OF_ALL_CALL|WINKS_EVENT_TEST_KET);
            Winks_AddWaitingEvent(Winks_engineContext_p->engChn, WINKS_EVENT_USB_DETECT);
#ifdef WINKS_PB_INTERCEPTION
            Winks_AddWaitingEvent(Winks_engineContext_p->engChn, WINKS_EVENT_PB_MODIFIED);
#endif
            /*huaxm interface:             load seeting information*/
            /*(winks_ReadFileSetting(&Winks_engineContext_p->setting);*/
            Winks_printf("<STRG>Winks_ReadSetting...\r\n");
            iRet = Winks_ReadSetting(&Winks_engineContext_p->setting);
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                Winks_engineContext_p->InitSettingOk = 0;
                Winks_mem_set(&Winks_engineContext_p->setting.imsi_list, 0, sizeof(Winks_ImsiList_s));/* ��֤����δע��״̬ */
                Winks_EngErr("Winks_ReadSetting!\r\n");
            }
            else
            {
                Winks_engineContext_p->InitSettingOk = 1;
            }
        #ifdef WINKS_DUAL_SIM
            Winks_engineContext_p->CurInquireSimNo = 1;
        #endif /* WINKS_DUAL_SIM */
            iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
            if(iRet == WINKS_RETURN_FAILURE)
            {
                Winks_printf("Winks_GetImsi ret fail!\r\n");
            }
            else if(iRet == WINKS_RETURN_SUCCESS)
            {
              #ifdef WINKS_DEBUG
                strncpy(Winks_engineContext_p->ImsiCode[0], buff, WINKS_IMSI_LEN);
              #endif
            #ifdef WINKS_DUAL_SIM
                Winks_engineContext_p->SimStatus |= 0x0100;/* Sim��1��Imsi�ѻ�ȡ */
            #endif /* WINKS_DUAL_SIM */
                if(winks_CheckFoundSim(buff, sizeof(buff)) == 1)/* �ҵ�����ע���Imsi,���ػ�ȡ��2��IMSI�� */
                {
                #ifdef WINKS_DUAL_SIM
                    iRet = Winks_SetActiveSim(1);
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                #endif /* WINKS_DUAL_SIM */
                    winks_BootingWithSimCard(3, NULL, 0, NULL, 0);
                }
            }
            /*else waiting message [WINKS_MSG_IMSI_REQ] at state [WINKS_ENGINE_STATE_START]*/
            break;
        }
        default:
            break;
        }
        break;
    }
    case WINKS_ENGINE_STATE_START:
    {
        switch (msg)
        {
        case WINKS_MSG_IMSI_REQ:
    #ifdef WINKS_DUAL_SIM
        case WINKS_MSG_IMSI_2_REQ:
    #endif
        {
            int  FoundRegImsi = -1;
            char buff[WINKS_IMSI_LEN+1];

            if(size == WINKS_IMSI_LEN)
            {
            #ifdef WINKS_DUAL_SIM
                if(msg == WINKS_MSG_IMSI_REQ)
                    Winks_engineContext_p->SimStatus |= 0x0100;/* Sim��1��Imsi�ѻ�ȡ */
                else
                    Winks_engineContext_p->SimStatus |= 0x0200;/* Sim��2��Imsi�ѻ�ȡ */
            #endif /* WINKS_DUAL_SIM */
                Winks_mem_set(buff, 0, sizeof(buff));
                strncpy(buff, (char *)data, WINKS_IMSI_LEN);
              #ifdef WINKS_DEBUG
                if(msg == WINKS_MSG_IMSI_REQ)
                    strncpy(Winks_engineContext_p->ImsiCode[0], (char *)data, WINKS_IMSI_LEN);
                else
                    strncpy(Winks_engineContext_p->ImsiCode[1], (char *)data, WINKS_IMSI_LEN);
              #endif

                FoundRegImsi = winks_CheckFoundSim(buff, sizeof(buff));
            }
            
            /* �ҵ�����ע���Imsi,����������������ע��״̬�����ػ�ȡ��һ����IMSI�� */
            if(FoundRegImsi == 1)
            {
            #ifdef WINKS_DUAL_SIM
                if(msg == WINKS_MSG_IMSI_REQ)
                    iRet = Winks_SetActiveSim(1);
                else
                    iRet = Winks_SetActiveSim(2);
                Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            #endif /* WINKS_DUAL_SIM */
                winks_BootingWithSimCard(3,  NULL, 0, NULL, 0);
            }
        #ifdef WINKS_DUAL_SIM
            /* ����Sim����imsi���ѻ�ȡ��,����������ע���imsi,����������δע��״̬ -- ˫����ѡ��*/
            else if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0300)
            {
                char buff1[WINKS_IMSI_LEN+1];
                
                if(msg == WINKS_MSG_IMSI_REQ)/* ��ǰbuff��ŵ���Sim��1��imsi,ȡ����ǰ�ѻ�ȡ����Sim��2��imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    Winks_GetImsi_2(Winks_engineContext_p->engChn, buff1, sizeof(buff1));
                }
                else/* ��ǰbuff��ŵ���Sim��2��imsi,ȡ����ǰ�ѻ�ȡ����Sim��1��imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(1) == TRUE);
                    Winks_GetImsi(Winks_engineContext_p->engChn, buff1, sizeof(buff1));
                }
                winks_BootingWithSimCard(2,  buff, sizeof(buff), buff1, sizeof(buff1));
            }
            /* �ѻ�ȡ1��Sim����imsi,��һ��Sim��������5����û�л�ȡ��imsi,��Ϊ��һ��������,����������δע��״̬ -- ���� */
            else if( (Winks_engineContext_p->SimStatus & 0xff00) &&
                     (Winks_engineContext_p->OtherSimRetryCnt >= 5) )
            {
                if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0100)/* Sim��1��imsi�ѻ�ȡ,������imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(1) == TRUE);
                    Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    iRet = Winks_UserSetActiveSim(1);
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                }
                else
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    iRet = Winks_UserSetActiveSim(2);
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                }
                winks_BootingWithSimCard(1,  buff, sizeof(buff), NULL, 0);
            }
        #else /* WINKS_DUAL_SIM */
            else if(FoundRegImsi == 0)
            {
                winks_BootingWithSimCard(1,  buff, sizeof(buff), NULL, 0);
            }
        #endif /* WINKS_DUAL_SIM */
            /* ��ʱ������ȡimsi */
            else if(Winks_engineContext_p->getImsiCount++ < 0x7FFF)
            {
            #ifdef WINKS_DUAL_SIM
                if(Winks_engineContext_p->SimStatus & 0xff00)/* �ѻ�ȡ��1Sim����imsi,���Ի�ȡ��һ����imsi*/
                    Winks_engineContext_p->OtherSimRetryCnt++;
                else
                    Winks_engineContext_p->OtherSimRetryCnt = 0;
            #endif /* WINKS_DUAL_SIM */
                
                /*��ʱ��ȡIMSI code*/
                if(Winks_engineContext_p->getImsiTimer == WINKS_TIMER_NULL)
                {
                    int timeOutPeriod = Winks_GetTimeForImsi(Winks_engineContext_p->getImsiCount);
                    Winks_engineContext_p->getImsiTimer = Winks_CreateTimer(
                        Winks_engineContext_p->engChn,
                        WINKS_MILLISECONDS2TICKS(timeOutPeriod), 
                        WINKS_TIMER_AUTO_START);
                }
            }
            else
            {
                /*Winks_engineContext_p->curState = WINKS_ENGINE_STATE_DISABLED;  
                Winks_engineContext_p->ifRemindRegister = 0;*/;
            #ifdef WINKS_DEBUG
                /*���Թ��ܿ���*/
                Winks_SetOsalTestBitFlag(2, 0);/*debug��ӡ����*/
            #endif
            }
            
            break;
        }
        case WINKS_MSG_ENGINE_USER_REG:/*ע��*/
        case WINKS_MSG_ENGINE_SNED_REG_SMS:/*�����ź��ѯע��ɹ����*/
        case WINKS_MSG_ENGINE_UPDATA_FL:/*���º���*/
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*�ϴ�ͬ������¼*/
#ifdef WINKS_DEL_PV2_0    
        case WINKS_MSG_ENGINE_SETTINGSET:/*����*/
#endif
        case WINKS_MSG_ENGINE_DIYSET:/*DIY*/
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            break;
        case WINKS_MSG_CALL_DIAL:
        case WINKS_MSG_CALL_INCOMING:
        {
            winks_EngCallEvtInit((msg==WINKS_MSG_CALL_DIAL), (char *)data);
            break;
        }
        case WINKS_MSG_CALL_REJECT:             /* ���ؾܽ� */
        case WINKS_MSG_CALL_REMOTE_REJECT:      /* �Է��ܽ� */
        case WINKS_MSG_CALL_ABORT:              /* ����ȡ�� */    
        case WINKS_MSG_CALL_REMOTE_ABORT:       /* �Է�ȡ�� */
        case WINKS_MSG_CALL_HANGUP:             /* ���عҶ� */
        case WINKS_MSG_CALL_ENDALL:             /* �Ҷ�ALL  */      
        case WINKS_MSG_CALL_REMOTE_HANGUP:      /* �Է��Ҷ� */
            winks_EngRecordCallEvt();
            break;
        default:
            break;
        }
        break;
    }
    
    case WINKS_ENGINE_STATE_DISABLED:
    {
        switch (msg)
        {
        case WINKS_MSG_CALL_DIAL:
        case WINKS_MSG_CALL_INCOMING:
        {
            if(winks_EngCallEvtInit((msg==WINKS_MSG_CALL_DIAL), (char *)data) != WINKS_RETURN_SUCCESS)
                break;
            if(Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO)
            {
            #if 0
                Winks_EngineToUiReturnMsg retMsg;
            #endif
                /*״̬�ع�*/
            //    Winks_SubStateRollback();
            #if 0
                Winks_mem_set(&retMsg, 0, sizeof(retMsg));
                retMsg.err_code = WINKS_MSG_ENG2UI_DIAL_INT;
                retMsg.err_state = ((unsigned long)(Winks_engineContext_p->curState) & 0x000000ff)<<8;
                retMsg.err_state |=  ((unsigned long)(Winks_engineContext_p->subState) & 0x000000ff);
                Winks_engineContext_p->curState = WINKS_ENGINE_STATE_SHOWA;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                retMsg.err_state <<= 16;
                retMsg.err_state |=  (unsigned long)(((Winks_engineContext_p->curState & 0x0000ffff)<<8 ) | (Winks_engineContext_p->subState & 0x000000ff));
                Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
                winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
            #else
            //    Winks_engineContext_p->curState = WINKS_ENGINE_STATE_SHOWA;
            //    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            //    winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_DIAL_INT);
            #endif
            }
            else
            {
            //    Winks_engineContext_p->curState = WINKS_ENGINE_STATE_SHOWA;
            }
            break;
        }
        case WINKS_MSG_CALL_REJECT:             /* ���ؾܽ� */
        case WINKS_MSG_CALL_REMOTE_REJECT:      /* �Է��ܽ� */
        case WINKS_MSG_CALL_ABORT:              /* ����ȡ�� */    
        case WINKS_MSG_CALL_REMOTE_ABORT:       /* �Է�ȡ�� */
        case WINKS_MSG_CALL_HANGUP:             /* ���عҶ� */
        case WINKS_MSG_CALL_ENDALL:             /* �Ҷ�ALL  */      
        case WINKS_MSG_CALL_REMOTE_HANGUP:      /* �Է��Ҷ� */
            winks_EngRecordCallEvt();
            break;
        
        case WINKS_MSG_ENGINE_USER_REG:/*ע��*/
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                int iRet = 0;
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_REG;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ONE;
                
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(sizeof(Winks_EngPrtReg_s))) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                
                /*����*/
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    NULL);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
        case WINKS_MSG_ENGINE_SNED_REG_SMS:/*�����ź��ѯע��ɹ����*/
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_THREE)
            {
                unsigned long msg_info = (*(unsigned long *)data);

                if(msg_info == 0)
                {
                    Winks_engineContext_p->app = 0;
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                    if(Winks_engineContext_p->communicationContext_p != NULL)
                    {
                        Winks_salFree(Winks_engineContext_p->communicationContext_p);
                        Winks_engineContext_p->communicationContext_p = NULL;
                    }
                }
                else
                {
                    Winks_EngPrtReg_s *p = (Winks_EngPrtReg_s *)Winks_engineContext_p->communicationContext_p;
                    unsigned long timerOut = 0;
                    
                    Winks_assert(p->timerId == WINKS_TIMER_NULL);
                    Winks_engineContext_p->app = WINKS_ENGINE_APP_REG;
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FOUR;

                    timerOut = p->shortTime;
                    if(timerOut < 5) timerOut = 60;
                    if(timerOut > 8640) timerOut = 60;
                    
                    Winks_printf("Waiting... shortTime=%d,longTime=%d timerOut=%d\r\n", p->shortTime, p->longTime, timerOut);
                    p->timerId = Winks_CreateTimer(
                        Winks_engineContext_p->engChn, 
                        WINKS_SECONDS2TICKS(timerOut/*p->shortTime*/), 
                        WINKS_TIMER_AUTO_START);
                    if(p->timerId == WINKS_TIMER_NULL)
                    {
                        Winks_assert(p->timerId > 0);
                    }
                }
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
            
        case WINKS_MSG_ENGINE_UPDATA_FL:/*���º���*/
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*�ϴ�ͬ������¼*/
#ifdef WINKS_DEL_PV2_0    
        case WINKS_MSG_ENGINE_SETTINGSET:/*����*/
#endif
        case WINKS_MSG_ENGINE_DIYSET:/*DIY*/
        {
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            break;
        }
        
        default:
            Winks_printf("error case: state[%d][%d] msg[0x%04lx]\r\n", 
                Winks_engineContext_p->curState, 
                Winks_engineContext_p->subState,
                msg);
            break;
        }
        break;
    }
    case WINKS_ENGINE_STATE_ENABLED:
    {
        switch (msg)
        {
        case WINKS_MSG_CALL_DIAL:
        case WINKS_MSG_CALL_INCOMING:
        {
            if(winks_EngCallEvtInit((msg==WINKS_MSG_CALL_DIAL), (char *)data) != WINKS_RETURN_SUCCESS)
                break;

            winks_EnginePauseBackSrv();
        #ifdef WINKS_SUPPORT_3G
        {
            Winks_EngCCDownloadContext_s *pDownCC;
            
            Winks_engineContext_p->isCallCCOk = 0;
            
            Winks_engineContext_p->app = WINKS_ENGINE_APP_CALL_DOWNLOAD;
            Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
            Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
            if((pDownCC = winks_EngGetCCDownContext()) == NULL)
            {
                return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
            }

            Winks_strncpy(pDownCC->ND_NO, Winks_engineContext_p->Call_NO, sizeof(pDownCC->ND_NO));
            pDownCC->DLS_Cnt = 1;
            pDownCC->DLS_Idx = 0;
            
            iRet = winks_EngineSendDataToServer(
                Winks_engineContext_p->curState, 
                Winks_engineContext_p->subState, 
                pDownCC->ND_NO);

            Winks_engineContext_p->isCallCCOk = 1;
            winks_StartCallWaitTimer();
            if(Winks_StrgGetCCShow( Winks_engineContext_p->Call_NO, 
                                    Winks_engineContext_p->CallType, 
                                    Winks_engineContext_p->pCCShow)
                                 == WINKS_RETURN_SUCCESS )
            {
                Winks_StrgLockCC(Winks_engineContext_p->pCCShow->CC_Idx);
            }
        }
        #else /* WINKS_SUPPORT_3G */
            Winks_StrgULAdd(Winks_engineContext_p->Call_NO);
        #endif /* WINKS_SUPPORT_3G */
            break;
        }
        case WINKS_MSG_CALL_REJECT:             /* ���ؾܽ� */
        case WINKS_MSG_CALL_REMOTE_REJECT:      /* �Է��ܽ� */
        case WINKS_MSG_CALL_ABORT:              /* ����ȡ�� */    
        case WINKS_MSG_CALL_REMOTE_ABORT:       /* �Է�ȡ�� */
        case WINKS_MSG_CALL_HANGUP:             /* ���عҶ� */
        case WINKS_MSG_CALL_ENDALL:             /* �Ҷ�ALL  */      
        case WINKS_MSG_CALL_REMOTE_HANGUP:      /* �Է��Ҷ� */
            winks_EngRecruitDownload();
            winks_EngRecordCallEvt();
            break;
        case WINKS_MSG_CALL_ANSWER:             /* ���ؽ��� */    
        case WINKS_MSG_CALL_REMOTE_ANSWER:      /* �Է����� */  
            break;
        case WINKS_MSG_ENGINE_PUSH:/*�����¼�*/
        {
            /*����*/
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
            #if 0 //  TODO: modify
                if(winks_IsUsbState() == 1)
                    break;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ONE;
                Winks_engineContext_p->IsPeriodUpdating = 1;
                
                iRet = Winks_check_storage_files(0);
                if(iRet != 0)/* �����ļ����ƻ� */
                {
                    if(iRet == 3)
                    {
                        Winks_EngErr("ϵͳ�ļ����ƻ�\r\n");
                        Winks_CurStateRollback(1);
                        return winks_EngRetErr(WINKS_MSG_ENG2UI_SYS_FILES_ERR);
                    }
                    else
                    {
                        Winks_EngErr("�����ļ����ƻ�\r\n");
                        return winks_EngRetErr(WINKS_MSG_ENG2UI_CC_FILES_ERR);
                    }
                }

                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(sizeof(Winks_EngPeriodComInfo_s))) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
            #else
                Winks_EngCCDownloadContext_s *pDownCC;
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_PERIOD_UPDATE;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ONE;
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((pDownCC = winks_EngGetCCDownContext()) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
            #endif
                
                /* */
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    NULL);
            }
            else
            {
                /* ����æ����ʱ120����������ʱ���·��� */
                winks_DelayPeriodPushTimer();
            }
        #ifdef WINKS_DEBUG
            Winks_GetSysDateTime( &Winks_engineContext_p->v_dateTime );
        #endif
            break;
        }
        
        case WINKS_MSG_ENGINE_INIT_CACHE:
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                Winks_engineContext_p->app = WINKS_ENGINE_APP_INIT_CACHE;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_THREE;
                
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    NULL);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }

        case WINKS_MSG_ENGINE_REBUILD_CACHE:
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                Winks_EngCCDownloadContext_s *pDownCC;
                int FLCnt = Winks_StrgFLCnt();
                
                if(FLCnt <= 0)
                    break;
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_REBULID_CACHE;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ELEVEN;
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((pDownCC = winks_EngGetCCDownContext()) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }

                if( ((pDownCC->ReqIdx = Winks_StrgFLGet(pDownCC->ReqIdx, pDownCC->ND_NO)) < 0) ||
                    (pDownCC->ND_NO[0] == 0) )
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                pDownCC->DLS_Cnt = (unsigned short)FLCnt;
                pDownCC->DLS_Idx = 0;
                
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    pDownCC->ND_NO);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }

        case WINKS_MSG_ENGINE_RECRUIT_DOWNLOAD:
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                Winks_EngCCDownloadContext_s *pDownCC;
                int ULCnt = Winks_StrgULCnt();

                if(ULCnt <= 0)
                    break;
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_RECRUIT_DOWNLOAD;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((pDownCC = winks_EngGetCCDownContext()) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }

                if( ((pDownCC->ReqIdx = Winks_StrgULGet(pDownCC->ND_NO)) < 0) ||
                    (pDownCC->ND_NO[0] == 0) )
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                pDownCC->DLS_Cnt = (unsigned short)ULCnt;
                pDownCC->DLS_Idx = 0;
                
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    pDownCC->ND_NO);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }

        case WINKS_MSG_ENGINE_UPDATA_FL:/*���º���*/
        {
            /*����*/
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
            #if 0
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_THREE;
                
                create_storage_rootdir();
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    NULL);
            #else
                char    FriendNo[WINKS_STRG_PHONE_LEN];
                Winks_EngCCDownloadContext_s *pDownCC;
                int FriendCnt = Winks_StrgFriendCnt();

                if(FriendCnt <= 0)
                    break;
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_UPDATE_FRIEND;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FIVE;
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((pDownCC = winks_EngGetCCDownContext()) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }

                if( ((pDownCC->ReqIdx = Winks_StrgFriendGet(pDownCC->ReqIdx,  
                                                              FriendNo, sizeof(FriendNo),
                                                              NULL, 0)) < 0) ||
                    (FriendNo[0] == 0) )
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                Winks_strncpy(pDownCC->ND_NO, FriendNo, sizeof(pDownCC->ND_NO));
                pDownCC->DLS_Cnt = (unsigned short)FriendCnt;
                pDownCC->DLS_Idx = 0;
                
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    pDownCC->ND_NO);
            #endif
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
    #if 0
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*�ϴ�ͬ������¼*/
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_FOUR)
            {
                int iRet = 0;
              
                if((data == NULL) && (size != 0))/* (size != 0)�������� */
                {
                    //Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                    if(Winks_engineContext_p->communicationContext_p != NULL)
                    {
                        Winks_salFree(Winks_engineContext_p->communicationContext_p);
                        Winks_engineContext_p->communicationContext_p = NULL;
                    }
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                else
                {
                    Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                    if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(sizeof(Winks_EngUpdComInfo_s))) == NULL)
                    {
                        return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                    }
                    
                    /*����*/
                    iRet = winks_EngineSendDataToServer(
                        Winks_engineContext_p->curState, 
                        Winks_engineContext_p->subState, 
                        data);
                }
                /*�ͷź����б�*/
                Winks_SetFullFriendList(NULL, 2);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
    #endif
#ifdef WINKS_DEL_PV2_0    
        case WINKS_MSG_ENGINE_SETTINGSET:
        {
            /*����*/
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                /*void *p = NULL;*/
                Winks_RealTimeInfo_s inData;

                Winks_assert(data != NULL);
                inData = *((Winks_RealTimeInfo_s *)data);
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_SYNC_SETTING;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_SEVEN;
                
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    &inData);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
#endif
        
        case WINKS_MSG_ENGINE_DIYSET:
        {
            /*����*/
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                Winks_engineContext_p->app = WINKS_ENGINE_APP_UPLOAD_DIY;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_EIGHT;
                
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(sizeof(Winks_EngDiyComInfo_s))) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                create_storage_rootdir();
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    data);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
        
        case WINKS_MSG_ENGINE_USER_REG:/*ע��*/
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
                int iRet = 0;
                unsigned char regFlag;
                
                Winks_engineContext_p->app = WINKS_ENGINE_APP_REREG;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_NINE;
                
                Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
                if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(sizeof(Winks_EngPrtReg_s))) == NULL)
                {
                    return winks_EngRetErr(WINKS_ENG_ERROR_CALL_FUNC);
                }
                /* TODO: modify
                if((Winks_check_storage_files(1) != 0) ||
                    (Winks_isUserRegistered(&regFlag)==WINKS_RETURN_FAILURE) || 
                    (regFlag == 0))
                {
                    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ONE;
                    Winks_engineContext_p->curState = WINKS_ENGINE_STATE_DISABLED;
                }*/
                /*����*/
                iRet = winks_EngineSendDataToServer(
                    Winks_engineContext_p->curState, 
                    Winks_engineContext_p->subState, 
                    NULL);
            }
            else
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            }
            break;
        }
        
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*�ϴ�ͬ������¼*/
        case WINKS_MSG_ENGINE_SNED_REG_SMS:/*�����ź��ѯע��ɹ����*/
        {
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
            break;
        }
        default:
            Winks_printf("error case: state[%d][%d] msg[0x%04lx]\r\n", 
                Winks_engineContext_p->curState, 
                Winks_engineContext_p->subState,
                msg);
            break;
        }
        break;
    }
    default: 
        Winks_printf("error case: state[%d][%d] msg[0x%04lx]\r\n", 
            Winks_engineContext_p->curState, 
            Winks_engineContext_p->subState,
            msg);
        break;
    }

    return WINKS_RETURN_SUCCESS;
}

/*�����ʼ��*/
int Winks_EngineInit(void)
{
    WINKS_CHN_ID engineChn = WINKS_CHN_NULL;
    
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    Winks_EngineContext *Winks_engineContext_p = NULL;
    Winks_engineContext_p = (Winks_EngineContext *)Winks_salAlloc(sizeof(Winks_EngineContext));
    Winks_assert(Winks_engineContext_p != NULL);
    set_module_glob(WK_MODULE_ENGINE, (void*)Winks_engineContext_p);
#else
    Winks_EngineContext *Winks_engineContext_p = NULL;
    Winks_engineContext_p = &Winks_engineContext;
#endif

    Winks_mem_set(Winks_engineContext_p, 0, sizeof(Winks_EngineContext));
    Winks_engineContext_p->pCCShow = (WinksStrgFile_CCShow_s *)Winks_salAlloc(sizeof(WinksStrgFile_CCShow_s));
    Winks_assert(Winks_engineContext_p->pCCShow);
    Winks_engineContext_p->curState = WINKS_ENGINE_STATE_INIT;
    engineChn = Winks_CreateChn(winks_EngineIdleHandler);
    Winks_assert(engineChn != WINKS_CHN_NULL);
#ifdef WINKS_SIMULATOR
Winks_engineContext_p->server.staDataSrv = 1;
#endif
    return engineChn;
}

/*����ע��*/
int Winks_EngineDestroy(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
#if 0
    /*�ͷŶ�̬���������*/
    if(Winks_engineContext_p->setting.ffl != NULL)
    {
        Winks_salFree(Winks_engineContext_p->setting.ffl);
        Winks_engineContext_p->setting.ffl = NULL;
    }
    if(Winks_engineContext_p->friendlist_ram.array != NULL)
    {
        Winks_salFree(Winks_engineContext_p->friendlist_ram.array);
        Winks_engineContext_p->friendlist_ram.array = NULL;
    }
#endif
    if(Winks_engineContext_p->pCCShow)
    {
        Winks_salFree(Winks_engineContext_p->pCCShow);
        Winks_engineContext_p->pCCShow = NULL;
    }
    if(Winks_engineContext_p->communicationContext_p != NULL)
    {
        Winks_salFree(Winks_engineContext_p->communicationContext_p);
        Winks_engineContext_p->communicationContext_p = NULL;
    }
    if(Winks_engineContext_p->server.data!= NULL)
    {
        Winks_salFree(Winks_engineContext_p->server.data);
        Winks_engineContext_p->server.data = NULL;
    }


    /*�ͷ���������*/
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    if(Winks_engineContext_p != NULL)
    {
        Winks_salFree(Winks_engineContext_p);
        Winks_engineContext_p = NULL;
        set_module_glob(WK_MODULE_ENGINE, NULL); 
    }
#endif
    return WINKS_RETURN_SUCCESS;
}

#ifdef WINKS_DUAL_SIM
#if 1
/* ���ֽڵ�2λ��ʾ�������ſ���  b0--��1; b1--��2
   ���ֽڱ�ʾ��ǰ����������ſ���1--��1;  2--��2
*/
unsigned short Winks_GetSimStatus()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->curState < WINKS_ENGINE_STATE_DISABLED)
    {
        return 0;
    }
    return Winks_engineContext_p->SimStatus;
}
#endif
/* �����ſ�ʱ,�û�ѡ�����ſ����� */
int Winks_UserSetActiveSim(int SimNo)
{
    if(Winks_SetActiveSim(SimNo) == WINKS_RETURN_SUCCESS)
    {
        /* �洢�û�ѡ��������Ϣ */
        return WINKS_RETURN_SUCCESS;
    }
    return WINKS_RETURN_FAILURE;
}
#endif /* WINKS_DUAL_SIM */

static int winks_EngInitCCCache()
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if(Winks_engineContext_p->app != WINKS_ENGINE_APP_RECRUIT_DOWNLOAD)
        winks_EnginePauseBackSrv();
    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_INIT_CACHE, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
}

static int winks_EngRebulidCCCache()
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }

    if(Winks_StrgTrimCCCache() <= 0)
    {
        Winks_printf("ERROR: Winks_StrgTrimCCCache\r\n");
        return WINKS_RETURN_FAILURE;
    }
    
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_REBUILD_CACHE, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
}

static int winks_EngRecruitDownload()
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }

    if(Winks_StrgULCnt() <= 0)
    {
        Winks_printf("ERROR: Winks_StrgULCnt\r\n");
        return WINKS_RETURN_FAILURE;
    }
    
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_RECRUIT_DOWNLOAD, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
}

/*�û�ע�����ҵ�񣬹�UIʹ�á�����ֵ:0�ɹ�,-1ʧ�ܡ�*/
int Winks_UserRegist(void)
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

#ifdef WINKS_DUAL_SIM
    Winks_printf("Winks_UserRegist SimStatus=%04x ActiveSim=%d\r\n", Winks_engineContext_p->SimStatus, Winks_GetActiveSim());
    if( ((Winks_engineContext_p->SimStatus & 0x0300) == 0x0300) &&  /* �������ſ� */
        (!Winks_GetActiveSim()) )/* ���ŷ�ע���Sim����Ҫ�û���ѡ�������� */
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_SIM_SEL_ERR);
        return WINKS_RETURN_FAILURE;
    }
    if(Winks_isSimEnabled(Winks_GetActiveSim()) == 0)
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_SIM_UNUSABLE);
        return WINKS_RETURN_FAILURE;
    }
#endif /* WINKS_DUAL_SIM */
    winks_EnginePauseBackSrv();
    if(Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO)
    {
        if( (Winks_engineContext_p->curState == WINKS_ENGINE_STATE_DISABLED) &&
            (Winks_engineContext_p->subState >= WINKS_ENGINE_SUBSTATE_FOUR) &&
            (Winks_engineContext_p->subState <= WINKS_ENGINE_SUBSTATE_SIX) )
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_NONE);
        else
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    /* δע��״̬:������̿ռ䲻��,������ */
    if(Winks_engineContext_p->curState < WINKS_ENGINE_STATE_ENABLED)
    {
        Winks_FlashSpace_s flashInfo;

        if( (Winks_GetFlashSpace(&flashInfo) != WINKS_RETURN_SUCCESS) ||
            (flashInfo.free < WINKS_INIT_NEED_FLASH_SIZE) )
        {
            unsigned long FoldSize;
            
            Winks_printf("ERROR: FlashSpace total=%lu free=%lu\r\n", flashInfo.total, flashInfo.free);
            
            iRet = Winks_GetFolderSize(WINKS_STRG_PATH, 1, &FoldSize, WINKS_INIT_NEED_FLASH_SIZE);
            Winks_printf("Winks_GetFolderSize(%lu). return[%d]\r\n", FoldSize, iRet);
            if(iRet<0 || ((FoldSize+flashInfo.free) < WINKS_INIT_NEED_FLASH_SIZE))
            {
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
                return WINKS_RETURN_FAILURE;
            }
        }
        
        if(Winks_engineContext_p->InitSettingOk == 0)
        {
            Winks_printf("Winks_init_storage...\r\n");
            /* TODO: modify
            Winks_init_storage();
            */
            
            iRet = Winks_ReadSetting(&Winks_engineContext_p->setting);
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                Winks_engineContext_p->InitSettingOk = 0;
                Winks_EngErr("Winks_ReadSetting!\r\n");
            }
            else
            {
                Winks_engineContext_p->InitSettingOk = 1;
            }
        }
        
    }
#if 0//def WINKS_DEBUG
{
    int FR_Max;
    if(Winks_GetMaxCountOfFriendList((unsigned long*)&FR_Max))/*����¼����ѡ������*/
        FR_Max = 0;
    Winks_printf("���Բ쿴FR_Max=%d \r\n", FR_Max);
}
#endif
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_USER_REG, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);

    return iRet;
}

/*�û�ȷ��ע�ᣬ����ע�����Ϣ������ֵ:0�ɹ� ����ʧ�ܡ�
reg_flag: 0ȡ��ע�ᣬ1����ע��*/
int Winks_UserSendRegSms(int reg_flag)
{
    int iRet  = 0;
    int iRet_ = 0;
    unsigned long msg = 0UL;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_DISABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_THREE))
    {
        //winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    
    if(reg_flag == 1)
    {
        iRet = winks_sendRegSms();
        Winks_printf("winks_sendRegSms ret %d\r\n", iRet);
        if(iRet == WINKS_RETURN_SUCCESS)
        {
            msg = 1UL;
        }
    }
    iRet_ = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_SNED_REG_SMS, &msg, sizeof(msg));
    Winks_assert(iRet_ ==  WINKS_RETURN_SUCCESS);
    
    return iRet;    
}

int Winks_UserUpdateFriend(void)
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }
    
    winks_EnginePauseBackSrv();
    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }

    if(Winks_StrgFriendCnt() <= 0)
    {
        Winks_printf("ERROR: Winks_StrgFriendCnt\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_FRIEND_OK);
        return WINKS_RETURN_SUCCESS;//WINKS_RETURN_FAILURE;
    }
    
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_UPDATA_FL, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
}

/*�û����º�����¼*/
int Winks_UserResetFriendList(void)
{
#if 0
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

#if 0//def WINKS_SUPPORT_MEMORY_CARD
#ifndef WINKS_SUPPORT_MEMORY_CARD
{
    unsigned long CC_StrgLocation = E_STORAGE_MCARD;
    Winks_set_setting_value(E_SETTING_STORAGE_LOCATION, CC_StrgLocation);
}
    if( (winks_Get_CCStrgLocation()==E_STORAGE_MCARD) && 
        (Winks_IsMemoryCardExist() != 1) )
    {
        Winks_EngErr("Winks_IsMemoryCardExist\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_MCARD_NOT_EXIST);
        return WINKS_RETURN_FAILURE;
    }
#endif /* !WINKS_SUPPORT_MEMORY_CARD */
#endif
    winks_EnginePauseBackSrv();
    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    
#ifdef WINKS_SUPPORT_MEMORY_CARD
    if(winks_Get_CCStrgLocation()==E_STORAGE_MCARD)
    {
        if(Winks_IsMemoryCardExist() != 1)
        {
            Winks_printf("<STRG>Change location to E_STORAGE_PHONE ...\r\n");
            Winks_set_setting_value(E_SETTING_STORAGE_LOCATION, E_STORAGE_PHONE);
            Winks_set_setting_value(E_SETTING_TEMP_FILE_CNT, 0);
            iRet = Winks_clear_old_list();
            if(iRet != 0)
            {
                winks_EngRetErrDirectly(WINKS_ENG_ERROR_CALL_FUNC);
                return WINKS_RETURN_FAILURE;
            }
        }
        else if(Winks_check_mc_status() == E_GFLAG_NEW_MC)
        {
            Winks_printf("<STRG>Winks_delete_mc_data ...\r\n");
            iRet = Winks_delete_mc_data();
            if(iRet != 0)
            {
                winks_EngRetErrDirectly(WINKS_ENG_ERROR_CALL_FUNC);
                return WINKS_RETURN_FAILURE;
            }
        }
    }
#endif /* !WINKS_SUPPORT_MEMORY_CARD */
#ifdef WINKS_DEBUG
{
    int FR_Max = 0;
    if(Winks_GetMaxCountOfFriendList((unsigned long*)&FR_Max))/*����¼����ѡ������*/
        FR_Max = 0;
    Winks_printf("���Բ쿴FR_Max=%d \r\n", FR_Max);
}
#endif
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_UPDATA_FL, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
#else
    return Winks_UserUpdateFriend();
#endif
}

/*�����û�ѡ�����¼��friend_list:��"|"�ָ��ĺ���.����"13900000001|13900020002|"*/
int Winks_UserSetFriendList(char *friend_list, unsigned long friend_list_len)
{
#if 0
    char *p = NULL;
    char *q = NULL;
    long len = 0L;
    char phoneNum[WINKS_FRIEND_NO_LEN];
    const char compart = '|';
    Winks_FriendItem_s friendItem;
    int iRet = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
	
 winks_EngineStopPeriodUpdate();
#ifdef WINKS_SUPPORT_MEMORY_CARD
    if( (winks_Get_CCStrgLocation()==E_STORAGE_MCARD) && 
        (Winks_IsMemoryCardExist() != 1) )
    {
        Winks_EngErr("Winks_IsMemoryCardExist\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_MCARD_NOT_EXIST);
        return WINKS_RETURN_FAILURE;
    }
#endif
    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if((Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED) &&
		(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO))
		Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_FOUR;
	
    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
      ( Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_FOUR))
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    

    if((friend_list == NULL) || (friend_list_len == 0) || ((len = strlen(friend_list)) == 0))
    {
        Winks_printf("�ϴ��պ���¼: friend_list_len = %d len = %d friend_list = 0x%x \r\n", friend_list_len, len, friend_list);
        /* ���Ϳպ���¼ */
        iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_SELECT_FRIEND_LIST, NULL, 0);
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
        return 0;
    }

    if((unsigned long)len > friend_list_len) /* ������봮���ȴ��� */
    {
        Winks_printf("ERROR: friend_list_len = %d len = %d \r\n", friend_list_len, len);
        /*�ָ�����״̬*/
        iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_SELECT_FRIEND_LIST, NULL, friend_list_len+1);
        return 0;
    }
    Winks_printf("<STRG> Winks_UserSetFriendBegin\r\n");
    if((iRet = Winks_UserSetFriendBegin()) < 0)
    {
        Winks_printf("ERROR: Winks_UserSetFriendBegin iRet = %d\r\n", iRet);
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
        return WINKS_RETURN_FAILURE;
    }
    
    friend_list_len = len + 1;/* У���ַ�������,Ӧ�ǰ�����β'\0' */
    
    p = q = friend_list;
    
    while(*p)
    {
        while(*q) {if(*q++==compart) break;};
        len = q-p-1;
        memset(phoneNum, 0, sizeof(phoneNum));
        if(len)
        {
            strncpy(phoneNum, p, MIN((len), sizeof(phoneNum)-1));
            phoneNum[MIN((len), sizeof(phoneNum)-1)] = 0;
        }

        /*�������¼*/
        Winks_mem_set(&friendItem, 0, sizeof(friendItem));
        strncpy(friendItem.number, phoneNum, sizeof(friendItem.number));
        iRet = Winks_UserSetFriendAddItem(&friendItem);
        if(iRet != 0)
        {
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
            return WINKS_RETURN_FAILURE;
        }
        if(*q == 0) break;
        p = q;
    }

    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_SELECT_FRIEND_LIST, friend_list, friend_list_len);
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);

    return iRet;
#endif
    return WINKS_RETURN_SUCCESS;
}

#if 0
static void winks_EngSelAllFriendList(void)
{
    int FL_Len = 0;
    char *pFL = NULL;
    char *pFL_Bcd = NULL;
    pFL_Bcd = Winks_GetFullFriendList((unsigned long *)&FL_Len);
    if( pFL_Bcd == NULL )
    {
        Winks_UserSetFriendList(NULL, 0);
    }
    else if( FL_Len > 0 )
    {
        pFL = Winks_salAlloc(FL_Len+1);
    }
    if( pFL != NULL)
    {
        Winks_convert_multi_number_to_ascii(pFL_Bcd, pFL, FL_Len);

        Winks_UserSetFriendList(pFL, FL_Len);
        Winks_salFree(pFL);
        pFL = NULL;
    }
}
#endif

#if  0//def WINKS_SUPPORT_MEMORY_CARD
static int winks_EngPostMsg2UI_SelFriend()
{
    Winks_EngineToUiReturnMsg retMsg;

    retMsg.err_state = 0x04030404;
    retMsg.err_code = WINKS_MSG_ENG2UI_FL_SEL_FRIEND;
    return Winks_PostMsgToUi(WINKS_MSG_ENGINE_UIRETMSG, &retMsg, sizeof(retMsg));/* msg_body�����ݷ��� */
}

/* ���ֻ��ռ䲻���Ҵ洢���ռ����ʱѯ���û��Ƿ�ʹ�ô洢��,��ѡ��ʹ�ô洢��ʱ���ô˺��� */
static int winks_SelMCard()
{
    int iRet = 0;
    int FL_Cnt = 0;
    int FR_Max = 0;
    
/* �ж��Ƿ�ʹ�ÿ���Ϊ�洢���� */
    int friend_cnt;
    Winks_FlashSpace_s PhoneSpace, CardSpace;
    
    if( (winks_GetStrgSize(&PhoneSpace, &CardSpace) != WINKS_RETURN_SUCCESS) || /* ��ȡϵͳflash����ʧ�� */
        (CardSpace.total == 0) )  /* �洢�������� */
    {
        Winks_EngErr("winks_GetStrgSize\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_MCARD_NOT_EXIST);
        return WINKS_RETURN_FAILURE;
    }
    
    friend_cnt = Winks_get_friend_cnt();
    if(friend_cnt<0)
        friend_cnt = 0;
    
    FL_Cnt = Winks_GetFullFriendListCount();/*�����б������*/
    FR_Max = winks_CalcFriendCCMax(CardSpace, FL_Cnt);
    if(FR_Max < friend_cnt) /* �洢���Ŀ��пռ䲻���Դ���ֻ����в��� */
    {
        Winks_EngErr("(FR_Max < friend_cnt)\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
        return WINKS_RETURN_FAILURE;
    }
        
    /* ʹ�ô洢����Ϊ����洢���� */
    /* �洢���ϴ�������Ŀ¼,�ٴ��ֻ����ƶ����񵽴洢���� */
    Winks_printf("<STRG>Winks_move_cc_res() ...\r\n");
    if(Winks_move_cc_res() == 0)
    {
        unsigned long CC_StrgLocation;
        
        iRet = Winks_set_setting_value(E_SETTING_TEMP_FILE_CNT, friend_cnt);
        //Winks_assert(iRet == 0);
        
        CC_StrgLocation = E_STORAGE_MCARD;
        iRet = Winks_set_setting_value(E_SETTING_STORAGE_LOCATION, CC_StrgLocation);
        //Winks_assert(iRet == 0);
        Winks_printf("<STRG>Winks_takeup_storage_space(%d)\r\n", FR_Max);
        if(Winks_takeup_storage_space((unsigned long)FR_Max))
        {
            Winks_EngErr("Winks_takeup_storage_space()\r\n");
            winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
            return WINKS_RETURN_FAILURE;
        }
        Winks_SetMaxCountOfFriendList(FR_Max);
        
    }
    else
    {
        Winks_EngErr("Winks_move_cc_res()\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
        return WINKS_RETURN_FAILURE;
    }
    /* �����Դ�����к����б���� */
    if((FR_Max>0)&&(FL_Cnt>FR_Max))
    {
        winks_EngPostMsg2UI_SelFriend();
    }

    /* ѡ���ϴ�����¼ */
    winks_EngSelAllFriendList();
    
    return WINKS_RETURN_SUCCESS;
}

/* ���ֻ��ռ䲻���Ҵ洢���ռ����ʱѯ���û��Ƿ�ʹ�ô洢��,��ѡ��ʹ�ô洢��ʱ���ô˺��� */
static int winks_UnselMCard()
{
    int FL_Cnt = 0;
    int FR_Max = 0;
    
/* �ж��Ƿ�ʹ�ÿ���Ϊ�洢���� */
    Winks_FlashSpace_s PhoneSpace, CardSpace;
    
    if( (winks_GetStrgSize(&PhoneSpace, &CardSpace) != WINKS_RETURN_SUCCESS) || /* ��ȡϵͳflash����ʧ�� */
        (CardSpace.total == 0) )  /* �洢�������� */
    {
        Winks_EngErr("winks_GetStrgSize\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_MCARD_NOT_EXIST);
        return WINKS_RETURN_FAILURE;
    }
    
    FL_Cnt = Winks_GetFullFriendListCount();/*�����б������*/
    FR_Max = winks_CalcFriendCCMax(PhoneSpace, FL_Cnt);
    if(FR_Max < 1)
     {
         Winks_EngErr("winks_UnselMCard()\r\n");
         winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
         return WINKS_RETURN_FAILURE;
     }
    
    if((FL_Cnt>0) && (FL_Cnt<=FR_Max))/* �����б�ĺ�����С���ֻ���������������,ȫ���������¼ */
    {
        winks_EngSelAllFriendList();
    }
    else if((FR_Max>0)&&(FL_Cnt>FR_Max))/*�����б������ֻ���������������*/
    {
        winks_EngPostMsg2UI_SelFriend();
    }
    
    return WINKS_RETURN_SUCCESS;
}

int Winks_UserSelMCard(int YesOrNo)
{
    if(YesOrNo)
        return winks_SelMCard();
    else
        return winks_UnselMCard();
}
#endif /* WINKS_SUPPORT_MEMORY_CARD */

/*�û�DIY����:�ϴ�������*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info)
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    winks_EnginePauseBackSrv();
    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    
    if(diy_info == NULL)
    {
        Winks_printf("ERROR: diy_info == NULL \r\n");
        Winks_assert(diy_info != NULL);
        return WINKS_RETURN_FAILURE;
    }
    
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_DIYSET, diy_info, sizeof(Winks_DiyInfo_s));
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
}

#ifdef WINKS_DEL_PV2_0    
/*�û����Ի�����: ͬ��������*/
int Winks_UserUploadSelfSetting(Winks_SelfSetting_s *self_info)
{
    int iRet = 0;
    Winks_RealTimeInfo_s sndData;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }
    
    winks_EnginePauseBackSrv();
    if((Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED) ||
       (Winks_engineContext_p->subState != WINKS_ENGINE_SUBSTATE_ZERO))
    {
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_ENG_BUSY);
        Winks_printf("ERROR: Eng is busy in state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }
    
    if(self_info == NULL)
    {
        Winks_printf("ERROR: diy_info == NULL \r\n");
        Winks_assert(self_info != NULL);
        return WINKS_RETURN_FAILURE;
    }
    
    Winks_assert(self_info->ccSwitch <= 1);
    Winks_assert(self_info->ShowCC <= 1);
    
{
    Winks_EngineToUiReturnMsg retMsg;
    unsigned long value;
    Winks_SelfSetting_s old_self_info;
    
    Winks_get_setting_value(E_SETTING_SHOW_CTRL, &value);
    old_self_info.ccSwitch = (unsigned char )((value&E_OTHER_SHOW_ME)?1:0);
    old_self_info.ShowCC = (unsigned char )((value&E_I_SHOW_OTHER)?1:0);
    if( self_info->ccSwitch == old_self_info.ccSwitch )/* "�Է�չʾ�ҵĲ���" û���޸�,�����ϴ������� */
    {
        if( self_info->ShowCC != old_self_info.ShowCC )/* "��չʾ�Է��Ĳ���" ���޸�,���ر��� */
        {
            if(self_info->ShowCC)
                value |= E_I_SHOW_OTHER;
            else
                value &= ~E_I_SHOW_OTHER;
            Winks_set_setting_value(E_SETTING_SHOW_CTRL, value);
        }
        /* ��UI���������Ϣ */
        Winks_mem_set(&retMsg, 0, sizeof(retMsg));
        retMsg.err_state = Winks_engineContext_p->curState;
        retMsg.err_state <<= 8;
        retMsg.err_state |= WINKS_ENGINE_SUBSTATE_SEVEN;
        retMsg.err_state <<= 8;
        retMsg.err_state |= Winks_engineContext_p->curState;
        retMsg.err_state <<= 8;
        retMsg.err_state |= WINKS_ENGINE_SUBSTATE_COMPLETE;
        Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
        retMsg.err_code = Winks_Eng2UiMsgMap(&retMsg);
        winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
        
        return iRet;
    }
}

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    Winks_mem_set(&sndData, 0, sizeof(sndData));
    sndData.opt = WINKS_REAL_TIME_INFO_OPT_SHOW_FLAG;
    sndData.showFlag = (unsigned char)(self_info->ccSwitch == 1 ? 1 : 2);

    Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
    if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(sizeof(Winks_SelfSetting_s))) == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_mem_cpy(Winks_engineContext_p->communicationContext_p, self_info, sizeof(Winks_SelfSetting_s));

    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_SETTINGSET, &sndData, sizeof(sndData));
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
}
#endif

int Winks_UserGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(pCCShow);
    
    if(strcmp(pNo, WINKS_DFT_CC_NO) == 0)/* Ĭ�ϲ��� */
        return Winks_StrgGetDftCCShow(dir, pCCShow);

    if(strcmp(Winks_SkipPlus(pNo), Winks_engineContext_p->pCCShow->NO) == 0)
    {
        Winks_mem_cpy(pCCShow, Winks_engineContext_p->pCCShow, sizeof(WinksStrgFile_CCShow_s));
        if( ((pCCShow->SaveType == 0) && pCCShow->CCImgPath[0][0]) ||
            ((pCCShow->SaveType == 1) && pCCShow->pCCImage && pCCShow->CCImageSize) )
            return WINKS_RETURN_SUCCESS;
    }
    else
    {
        if(Winks_StrgGetCCShow(Winks_SkipPlus(pNo), dir, pCCShow) == WINKS_RETURN_SUCCESS)
        {
            if(pCCShow->CC_Idx)
                Winks_StrgLockCC(pCCShow->CC_Idx);
            return WINKS_RETURN_SUCCESS;
        }
    }

    return WINKS_RETURN_FAILURE;
}

int Winks_UserStopCCShow()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_StrgUnlockCC();
    Winks_mem_set(Winks_engineContext_p->pCCShow, 0, sizeof(WinksStrgFile_CCShow_s));
    return Winks_EngineStop();
}

/*���ÿ�������ʾ�û�ע�ᡣ*/
int Winks_UserRememberImsi(void)
{
    char buff[WINKS_IMSI_LEN + 1];
    int iRet = 0;
    Winks_mem_set(buff, 0, sizeof(buff));

    iRet = Winks_GetImsi(WINKS_CHN_NULL, buff, sizeof(buff));

    if(iRet != WINKS_RETURN_SUCCESS)
    {
        Winks_printf("ERR: Call Winks_GetImsi error. return[%d]\r\n",iRet);
        Winks_fail("iRet != WINKS_RETURN_SUCCESS");
        return WINKS_RETURN_FAILURE;
    }
    
    iRet = Winks_SetImsiPopFlag(buff, 1);

    if(iRet != 0)
    {
        Winks_printf("ERR: Call Winks_SetImsiPopFlag error. return[%d]\r\n",iRet);
        //Winks_fail("iRet != 0");
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/* �رղ��� */
int Winks_UserCloseWinkCC(void)
{
    int iRet = 0;
#if 0
    Winks_EngineToUiReturnMsg retMsg;
    Winks_SelfSetting_s self_info;
#endif
    unsigned long value;
    iRet = Winks_get_setting_value(E_SETTING_SHOW_CTRL, &value);
    if((iRet!=WINKS_RETURN_FAILURE) && (value==E_CC_SHOW_ON))/* ����Ϊ����״̬���ر� */
    {
        iRet = winks_StopPeriodPushTimer(5);
        if(iRet!=WINKS_RETURN_FAILURE)
        {
            value = E_CC_SHOW_OFF;
            iRet = Winks_set_setting_value(E_SETTING_SHOW_CTRL, value);
            Winks_SubStateRollback();
        }
    }
#if 0
    /* ��UI���������Ϣ */
    Winks_mem_set(&retMsg, 0, sizeof(retMsg));
    retMsg.err_state = Winks_engineContext_p->curState;
    retMsg.err_state <<= 8;
    retMsg.err_state |= WINKS_ENGINE_SUBSTATE_SEVEN;
    retMsg.err_state <<= 8;
    retMsg.err_state |= Winks_engineContext_p->curState;
    retMsg.err_state <<= 8;
    if(iRet != WINKS_RETURN_FAILURE)
        retMsg.err_state |= WINKS_ENGINE_SUBSTATE_COMPLETE;
    else
        retMsg.err_state |= WINKS_ENGINE_SUBSTATE_ZERO;
    Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
    winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
#endif
    return iRet;
}

/* �������� */
int Winks_UserOpenWinkCC(void)
{
    int iRet = 0;
#if 0
    Winks_EngineToUiReturnMsg retMsg;
    Winks_SelfSetting_s self_info;
#endif
    unsigned long value;
    iRet = Winks_get_setting_value(E_SETTING_SHOW_CTRL, &value);
    if((iRet!=WINKS_RETURN_FAILURE) && (value==E_CC_SHOW_OFF))/* ����Ϊ�ر�״̬������ */
    {
        value = E_CC_SHOW_ON;
        iRet = Winks_set_setting_value(E_SETTING_SHOW_CTRL, value);
        if(iRet!=WINKS_RETURN_FAILURE)
        {
            winks_StartBuildCacheTimer();
            iRet = winks_StartPeriodPushTimer();
        }
    }
#if 0
    /* ��UI���������Ϣ */
    Winks_mem_set(&retMsg, 0, sizeof(retMsg));
    retMsg.err_state = Winks_engineContext_p->curState;
    retMsg.err_state <<= 8;
    retMsg.err_state |= WINKS_ENGINE_SUBSTATE_SEVEN;
    retMsg.err_state <<= 8;
    retMsg.err_state |= Winks_engineContext_p->curState;
    retMsg.err_state <<= 8;
    if(iRet != WINKS_RETURN_FAILURE)
        retMsg.err_state |= WINKS_ENGINE_SUBSTATE_COMPLETE;
    else
        retMsg.err_state |= WINKS_ENGINE_SUBSTATE_ZERO;
    Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
    winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
#endif
    return iRet;
}

int Winks_GetDLCCProgress(Winks_EngDLCCInfo_s *pDLCCInfo)
{
    Winks_EngDLCCInfo_s *p = winks_EngGetDLCCContext();

    if(pDLCCInfo)
    {
        Winks_mem_set(pDLCCInfo, 0, sizeof(Winks_EngDLCCInfo_s));
        if(p)
        {
            Winks_mem_cpy(pDLCCInfo, p, sizeof(Winks_EngDLCCInfo_s));
            return WINKS_RETURN_SUCCESS;
        }
    }
    return WINKS_RETURN_FAILURE;
}

/*ȡ��ǰ�����*/
int Winks_GetCurAccessPoint(unsigned long *accessPoint)
{
    int iRet = 0;
    unsigned long ap = 0;
    iRet = Winks_get_setting_value(E_SETTING_CONNECT_POINT, &ap);

    if(accessPoint != NULL)
        *accessPoint = ap;
    
    if(iRet != 0)
        return WINKS_RETURN_FAILURE;
    else
        return (int)ap;
}

int Winks_SetCurAccessPoint(unsigned long accessPoint)
{
    int iRet = 0;
    iRet = Winks_set_setting_value(E_SETTING_CONNECT_POINT, accessPoint);

    if(iRet != 0)
        return WINKS_RETURN_FAILURE;
    return WINKS_RETURN_SUCCESS;
}

static void wk_sort_ascii_string(wk_SortItem *array, unsigned int count)
{
    /* ѡ������ */
    wk_SortItem tmp = {0};
    unsigned long i, j, min;

    if (array == NULL || count < 2)
        return;

    for (i = 0; i < count - 1; i++)
    {
        min = i;

        for (j = i + 1; j < count; j++)
            if (strcmp(array[j].data, array[min].data) < 0)
                min = j;

        if (min != i)
        {
            tmp = array[i];
            array[i] = array[min];
            array[min] = tmp;
        }
    }
}

static void wk_sort_unicode_string(wk_SortItem *array, unsigned int count)
{;}

static void wk_sort_string_ok(wk_SortItem *array, unsigned int count)
{
    unsigned int i;
    unsigned int cnt1, cnt2, cnt3;
    unsigned int idx1, idx2, idx3;
    wk_SortItem *temp = NULL;

    temp = Winks_salAlloc(count * sizeof(wk_SortItem));
    if (temp == NULL)
        return;

    memset(temp, 0, count * sizeof(wk_SortItem));
    cnt1 = cnt2 = cnt3 = 0;
    idx1 = idx2 = idx3 = 0;

    /* ��һ��ɨ�������� */
    for (i = 0; i < count; i++)
    {
        switch (array[i].flag)
        {
        case 1:
            cnt1++;
            break;
        case 2:
            cnt2++;
            break;
        case 3:
            cnt3++;
            break;
        }
    }

    for (i = 0; i < count; i++)
    {
        switch (array[i].flag)
        {
        case 1:
            temp[idx1++] = array[i];
            break;
        case 2:
            temp[cnt1 + idx2++] = array[i];
            break;
        case 3:
            temp[cnt1 + cnt2 + idx3++] = array[i];
            break;
        }
    }

    /* �ֱ��3������������ */
    wk_sort_ascii_string(temp, cnt1);
    wk_sort_ascii_string(temp + cnt1, cnt2);
    wk_sort_unicode_string(temp + cnt1 + cnt2, cnt3);

    Winks_mem_cpy(array, temp, count * sizeof(wk_SortItem));

    Winks_salFree(temp);
}


/*
*   ��������б�
*   NOTE: Ҫ��array�е�number��name��unicode���롣
*/
void Winks_SortFriendList(Winks_FriendItem_s *array, unsigned int count)
{
    unsigned int i;
    wk_SortItem *temp1 = NULL;
    Winks_FriendItem_s *temp2 = NULL;

    Winks_assert(array != NULL);
    Winks_assert(count > 0);

    Winks_printf("Winks_SortFriendList(0x%08x, %d)\r\n", array, count);
    if (count == 1)
        return;

    temp1 = Winks_salAlloc(count * sizeof(wk_SortItem));
    temp2 = Winks_salAlloc(count * sizeof(Winks_FriendItem_s));
    if (temp1 == NULL || temp2 == NULL)
    {
        if (temp1)
            Winks_salFree(temp1);

        if (temp2)
            Winks_salFree(temp2);

        return;
    }

    memset(temp1, 0, count * sizeof(wk_SortItem));
    memset(temp2, 0, count * sizeof(Winks_FriendItem_s));

    for (i = 0; i < count; i++)
    {
        Winks_assert(array[i].number[0]);

        temp1[i].index = i;

        if (WK_STRLEN(array[i].name) == 0) /*  ���û������*/
        {
            temp1[i].flag = 1;      /* ascii */
            Winks_Unicode2AsciiString(array[i].number, temp1[i].data, WINKS_FRIEND_NO_LEN);
        }
        else
        {
            if (wk_is_string_ascii(array[i].name))
            {
                temp1[i].flag = 1;      /* ascii */
                Winks_Unicode2AsciiString(array[i].name, temp1[i].data, WINKS_FRIEND_NO_LEN);
            }
            else
            {
                int j = 0, py_cnt = 0, ucs_len = 0;
                char py_buf1[WINKS_FRIEND_NO_LEN];
                char py_buf2[WINKS_FRIEND_NO_LEN];

                Winks_mem_set(py_buf1, 0, sizeof(py_buf1));
                Winks_mem_set(py_buf2, 0, sizeof(py_buf2));
                py_cnt = Winks_GetPinyin(array[i].name, py_buf1, sizeof(py_buf1) - 2);
                if (py_cnt > 0)
                {
                    temp1[i].flag = 2;      /* ƴ�� */

                    /* ������ȡƴ��, ���˺��� */
                    ucs_len = WK_STRLEN(py_buf1) - WK_STRLEN(array[i].name);
                    Winks_mem_cpy(py_buf2, py_buf1, ucs_len * 2);

                    Winks_Unicode2AsciiString(py_buf2, temp1[i].data, WINKS_FRIEND_NO_LEN);
                }
                else
                {
                    temp1[i].flag = 3;      /* ��������ŵ������ַ� */
                    Winks_mem_cpy(temp1[i].data, array[i].name, (WK_STRLEN(array[i].name) + 1) * 2);
                }
            }
        }
    }

    /* ��Ŀ������ */
    wk_sort_string_ok(temp1, count);

    for (i = 0; i < count; i++)
        temp2[i] = array[temp1[i].index];

    Winks_mem_cpy(array, temp2, count * sizeof(Winks_FriendItem_s));

    Winks_salFree(temp1);
    Winks_salFree(temp2);
}

static int winks_EngStartTimer(WINKS_TIMER_ID * pEngTimer,
                                                WINKS_TICKS timeOutPeriod, 
                                                unsigned long flag)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(*pEngTimer)
    {
        Winks_DestroyTimer(*pEngTimer);
        *pEngTimer = WINKS_TIMER_NULL;
    }
    
    *pEngTimer = Winks_CreateTimer(
        Winks_engineContext_p->engChn, 
        timeOutPeriod, 
        flag);
    if(*pEngTimer == WINKS_TIMER_NULL)
    {
        Winks_assert(*pEngTimer > 0);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

static void winks_EngStopTimer(WINKS_TIMER_ID * pEngTimer)
{
    if(pEngTimer && *pEngTimer != WINKS_TIMER_NULL)
    {
        Winks_DestroyTimer(*pEngTimer);
        *pEngTimer = WINKS_TIMER_NULL;
    }
}

static void winks_StartBuildCacheTimer(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    winks_EngStartTimer(&Winks_engineContext_p->BuildCacheTimer,
                        WINKS_SECONDS2TICKS(WINKS_CACHETIMER_RETRY_SLOT), 
                        WINKS_TIMER_AUTO_START);
    Winks_printf("winks_StartBuildCacheTimer[%d] TimeSlot=%d \r\n", Winks_engineContext_p->BuildCacheTimer, WINKS_CACHETIMER_RETRY_SLOT);
}

static void winks_StopBuildCacheTimer(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_StopBuildCacheTimer[%d] \r\n", Winks_engineContext_p->BuildCacheTimer );
    winks_EngStopTimer(&Winks_engineContext_p->BuildCacheTimer);
}
#ifdef WINKS_SUPPORT_3G
static void winks_StartCallWaitTimer(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    winks_EngStartTimer(&Winks_engineContext_p->CallWaitTimer,
                        WINKS_SECONDS2TICKS(WINKS_CALLTIMER_WAIT_SLOT), 
                        WINKS_TIMER_AUTO_START);
    Winks_printf("winks_StartCallWaitTimer[%d] TimeSlot=%d \r\n", Winks_engineContext_p->CallWaitTimer, WINKS_CALLTIMER_WAIT_SLOT);
}

static void winks_StopCallWaitTimer(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_StopCallWaitTimer[%d] \r\n", Winks_engineContext_p->CallWaitTimer );
    winks_EngStopTimer(&Winks_engineContext_p->CallWaitTimer);
}
#endif /* WINKS_SUPPORT_3G */
#ifdef WINKS_DEBUG
static void Record_PeriodTimerStop()
{
    FILE *pfile = NULL;
    char  TimeFlag[32];
    char  ErrMsgBuf[256];
    Winks_ConnectWayOperation_s CWOp;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_mem_set(TimeFlag, 0, sizeof(TimeFlag));
    Winks_GetTimeFlag(TimeFlag, 32, 3);
    Winks_mem_set(&CWOp, 0, sizeof(CWOp));
    Winks_GetSupportedConnectWays( &CWOp);
    
    sprintf(ErrMsgBuf, "PeriodTimerStop: CWOp[0x%08x, %lu] cause(%d) at %s\r\n", \
        (int)CWOp.operation, CWOp.twuTimeSlot, Winks_engineContext_p->PTCloseCause, TimeFlag);

    pfile = Winks_fopen( WINKS_APP_PATH"assert.txt", "a" );
	if( pfile != NULL )
    {
        Winks_fwrite( ErrMsgBuf, strlen(ErrMsgBuf), 1, pfile );
    	Winks_fclose( pfile );
    }   
}
#endif

/* �綨ʱ���·���δ����,��������ʱ���·���;
   �綨ʱ���·���������,�ȼ�鶨ʱ���������Ƿ���������·���һ�£�����һ����������ʱ�� */
static int winks_StartPeriodPushTimer(void)
{
    int iRet = 0;
    Winks_ConnectWayOperation_s CWOp;
    unsigned long ClientConnectWay = 0;
    unsigned long IsPeriodWakeMode = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if(IS_WINKS_CC_CLOSE())
    {
        Winks_printf("IS_WINKS_CC_CLOSE 0x%08x\r\n",Winks_engineContext_p->setting.show_ctrl);
        return WINKS_RETURN_FAILURE;
    }
    
    memset(&CWOp, 0, sizeof(CWOp));
    Winks_GetSupportedConnectWays(&CWOp);/* ������֧�ֵķ�ʽ��Ĭ�ϵķ�ʽ */
    
    if((CWOp.twuTimeSlot < WINKS_TIME_CONNECT_PERIOD_MIN) || (CWOp.twuTimeSlot > WINKS_TIME_CONNECT_PERIOD_MAX))
        CWOp.twuTimeSlot = WINKS_TIME_CONNECT_PERIOD_DEF;
    
#ifdef WINKS_SIMULATOR     /* renzhan add for debug */
    CWOp.twuTimeSlot = 60;
#endif

    /* ����Ƿ���Ҫ������ʱ�� */
    if(Winks_engineContext_p->PeriodPushTimer)/* ��ʱ���·��������� */
    {
        winks_TimerBody_s timerInfo;
        
        Winks_mem_set(&timerInfo, 0, sizeof(timerInfo));
        if( (Winks_GetTimerInfo(Winks_engineContext_p->PeriodPushTimer, &timerInfo) == WINKS_RETURN_SUCCESS) &&
            (CWOp.operation&CONNECT_WAY_TIMER_WAKE_UP) &&   /* ������֧�ֶ�ʱ���� */
            (timerInfo.timeOutPeriod == WINKS_SECONDS2TICKS(CWOp.twuTimeSlot))) /* ��ʱ����������������·���һ�� */
        {
            Winks_printf("winks_StartPeriodPushTimer[%d] already ok! CWOp[0x%08x, %d] \r\n",
                Winks_engineContext_p->PeriodPushTimer, CWOp.operation, CWOp.twuTimeSlot );
            return iRet;/* ֱ�ӷ��ز��ø��¶�ʱ�� */
        }
    }
    
    Winks_GetPhoneConnectWay(&ClientConnectWay);/* �û�ѡ��ķ�ʽ */
    /* �û�û��ѡ�����ӷ�ʽ��ѡ��ķ�ʽ���Ƿ�����֧�ֵ�,�򰴷�����Ĭ�ϵķ�ʽ�жϣ�
       ���ѡ��ķ�ʽ�Ƿ�����֧�ֵģ����û�ѡ��ķ�ʽ�ж� */
    if( (ClientConnectWay == 0) || /* �û�û��ѡ�����ӷ�ʽ */
        (((CWOp.operation&0xffff)&ClientConnectWay) != ClientConnectWay) )/* �û�ѡ��ķ�ʽ���Ƿ�����֧�ֵ� */
    {
        /* ��������Ĭ�ϵķ�ʽ�ж� */
        IsPeriodWakeMode = (CWOp.operation>>16)&CONNECT_WAY_TIMER_WAKE_UP;
    }
    else
    {
        /* ���û�ѡ��ķ�ʽ�ж� */
        IsPeriodWakeMode = ClientConnectWay&CONNECT_WAY_TIMER_WAKE_UP;
    }
    
    if(IsPeriodWakeMode)
    {
        iRet = winks_EngStartTimer( &Winks_engineContext_p->PeriodPushTimer,
            WINKS_SECONDS2TICKS(CWOp.twuTimeSlot), 
            WINKS_TIMER_AUTO_LOOP|WINKS_TIMER_AUTO_START);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
        #ifdef WINKS_DEBUG
            Winks_GetSysDateTime( &Winks_engineContext_p->v_dateTime1 );
            Winks_engineContext_p->PTCloseCause = 4;
        #endif
        }
        else
        {
            Winks_printf("winks_StartPeriodPushTimer[%d] ���ӷ�ʽ[0x%08x & 0x%04x] TimeSlot=%d \r\n",
                Winks_engineContext_p->PeriodPushTimer, CWOp.operation, ClientConnectWay, CWOp.twuTimeSlot );
        }
    }
    else
    {
        if(!(CWOp.operation&CONNECT_WAY_TIMER_WAKE_UP))/* ��������֧�ֶ�ʱ���� */
        {
            /* ֹͣ��ʱ���·��� */
            winks_StopPeriodPushTimer(3);
        }
    }
    
    return iRet;
}

/* ����æ����ʱ120����������ʱ���·��� */
static int winks_DelayPeriodPushTimer(void)
{
    int iRet = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->PeriodPushTimer != WINKS_TIMER_NULL)
    {
        winks_TimerBody_s timerInfo;
        
        Winks_mem_set(&timerInfo, 0, sizeof(timerInfo));
        if( (Winks_GetTimerInfo(Winks_engineContext_p->PeriodPushTimer, &timerInfo) == WINKS_RETURN_SUCCESS) &&
            (timerInfo.timeOutPeriod == WINKS_SECONDS2TICKS(WINKS_PERIODTIMER_RETRY_SLOT)) )/* ��ʱ����������������·���һ�� */
        {
            Winks_printf("winks_DelayPeriodPushTimer[%d] already Delay:%d�� state[%d][%d] \r\n", 
                Winks_engineContext_p->PeriodPushTimer, WINKS_PERIODTIMER_RETRY_SLOT,
                Winks_engineContext_p->curState, Winks_engineContext_p->subState);
            return iRet;/* ֱ�ӷ��ز��ø��¶�ʱ�� */
        }
    }
    
    iRet = winks_EngStartTimer( &Winks_engineContext_p->PeriodPushTimer, 
        WINKS_SECONDS2TICKS(WINKS_PERIODTIMER_RETRY_SLOT), 
        WINKS_TIMER_AUTO_LOOP|WINKS_TIMER_AUTO_START);
    if(iRet != WINKS_RETURN_SUCCESS)
    {
    #ifdef WINKS_DEBUG
        Winks_GetSysDateTime( &Winks_engineContext_p->v_dateTime1 );
        Winks_engineContext_p->PTCloseCause = 4;
    #endif
    }
    else
    {
        Winks_printf("winks_DelayPeriodPushTimer[%d] Delay:%d�� state[%d][%d] \r\n", 
            Winks_engineContext_p->PeriodPushTimer, WINKS_PERIODTIMER_RETRY_SLOT,
            Winks_engineContext_p->curState, Winks_engineContext_p->subState);
    }

    return iRet;
}

/* ֹͣ��ʱ���·��� */
static int winks_StopPeriodPushTimer(int cause)
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_printf("winks_StopPeriodPushTimer[%d] cause=%d \r\n", Winks_engineContext_p->PeriodPushTimer, cause);
    winks_EngStopTimer(&Winks_engineContext_p->PeriodPushTimer);
#ifdef WINKS_DEBUG
    Winks_GetSysDateTime( &Winks_engineContext_p->v_dateTime1 );
    Winks_engineContext_p->PTCloseCause = cause;
    Record_PeriodTimerStop();
#endif
    return iRet;
}

/* �û����ӷ�ʽ����: Push��ʽ �� ��ʱ���� (ConnWay&CONNECT_WAY_TIMER_WAKE_UP) 
   ���������֧��Push��ʽ���û����ֶ�������رն�ʱ���·��� */
int Winks_UserSetConnectWay(unsigned long ConnWay)
{
    int iRet = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_engineContext_p->curState != WINKS_ENGINE_STATE_ENABLED)
    {
        winks_EngRetErrDirectly(WINKS_ENG_ERROR_CALL_FUNC);
        Winks_printf("ERROR: Eng is not in enable state [%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
        return WINKS_RETURN_FAILURE;
    }

    if(ConnWay&CONNECT_WAY_TIMER_WAKE_UP)/* ��ʱ���ѷ�ʽ */
    {
        /* ������ʱ���·��� */
        iRet = winks_StartPeriodPushTimer();
    }
    else
    {
        /* ֹͣ��ʱ���·��� */
        iRet = winks_StopPeriodPushTimer(1);
    }
    
    return iRet;
}

/*ֹͣ����*/
int Winks_EngineStop(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
    {
    #ifdef WINKS_DUAL_SIM
        if( (Winks_engineContext_p->curState < WINKS_ENGINE_STATE_ENABLED) &&
            (Winks_engineContext_p->SimStatus & 0x0300) == 0x0300)
            Winks_SetActiveSim(0);
    #endif /* WINKS_DUAL_SIM */
        return WINKS_RETURN_SUCCESS;
    }
    
    switch(Winks_engineContext_p->curState)
    {
    case WINKS_ENGINE_STATE_DISABLED:
    {
        switch(Winks_engineContext_p->subState)
        {
        case WINKS_ENGINE_SUBSTATE_FOUR:    /*��ѯ�Ƿ�ע��2(3������+3������)*/
        case WINKS_ENGINE_SUBSTATE_FIVE:    /*���������Ϣ*/
        case WINKS_ENGINE_SUBSTATE_SIX:     /*����Ƭͷ��Ӫ�̲���*/
            /*��ִ̨�г�ʼ��,״̬���ı�*/
            Winks_printf("((((((((((((((((((((((((( Winks_EngineStop [%d][%d] to bg ))))))))))))))))))))\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
            return WINKS_RETURN_SUCCESS;
        default:
        #ifdef WINKS_DUAL_SIM
            if((Winks_engineContext_p->SimStatus & 0x0300) == 0x0300)
                Winks_SetActiveSim(0);
        #endif /* WINKS_DUAL_SIM */
            break;
        }
        break;
    }
    case WINKS_ENGINE_STATE_ENABLED:
    {
        switch(Winks_engineContext_p->app)
        {
        case WINKS_ENGINE_APP_INIT_CACHE:
        case WINKS_ENGINE_APP_REBULID_CACHE:
        case WINKS_ENGINE_APP_PERIOD_UPDATE:
        case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
#ifdef WINKS_BREW
            break;
#else
            Winks_printf("((((((((((((((((((((((((( Winks_EngineStop [%d][%d] to bg ))))))))))))))))))))\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
            return WINKS_RETURN_SUCCESS;
#endif
        default:
            break;
        }
    }
    default:
        break;
    }
    Winks_printf("((((((((((((((((((((((((( Winks_EngineStop [%d][%d] to zero ))))))))))))))))))))\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);

    return Winks_SubStateRollback();
}

#if 0 /*����*/
int Winks_UpdateFriendRecord()
{
    int iRet = 0;
    Winks_FriendItem_s  friendItem;
    int Cnt,FriendCount;
    char buff[2048];
    char *p = buff;
static int Begin = 0;

    FriendCount = Winks_GetFullFriendListCount();
    Winks_assert(FriendCount>0);

if(Begin)Cnt = FriendCount>>1;
else Cnt = 0;
    *p = '\0';
    FriendCount = Cnt+6; /* ����: ȡ6��������Ϊ����¼ */
    while(Cnt<FriendCount)
    {
        Winks_UserGetFullFriendItem(&friendItem, Cnt);
        if(friendItem.number[0] == 0)break;
        snprintf(p,MIN(strlen(friendItem.number), sizeof(friendItem.number))+2, "%s|", friendItem.number);
        p+=MIN(strlen(friendItem.number), sizeof(friendItem.number))+1;
        *p = '\0';
        Cnt++;
    }
    Winks_UserSetFriendList(buff, strlen(buff)+1);

if(!Begin){
Begin = 1;
}
else{
Begin = 0;
}
    return iRet;
}

/*��ʱͨѶ*/
int Winks_PeriodPushServer()
{
    int iRet = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_PUSH, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    return iRet;
}


const int UserTestStateArr[] = {16, /*17,*/ 9, 19, /*20,*/ 12};
int Winks_UserTestCheckReg(void)
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     ErrCode = 0;
    int     flag;
#if 1
    flag = 19;
#else
static int Step = 0;
int StateNum = sizeof(UserTestStateArr)/sizeof(UserTestStateArr[0]);
flag = UserTestStateArr[Step];
Step = (Step+1)%StateNum;
#endif
Winks_printf("&&&&&&&&&&&&&&&&&&&&&&&&& Winks_UserTestCheckReg %d &&&&&&&&&&&&&&&&&&&&&&&&&\r\n",flag);
    switch(flag)
    {
    case 2:
        iRet = winks_PackDataAndSend(0x7FFD, NULL, &ErrCode);
        break;
    case 3:
        winks_EngSetQueryShortTime(10);
        winks_EngSetQueryLongTime(20);
        Winks_UserSendRegSms(1);
        break;
    case 9:
        {
        Winks_SelfSetting_s self_info;
        self_info.ccSwitch = (unsigned char)1;
        self_info.ShowCC = (unsigned char)1;
        iRet = Winks_UserUploadSelfSetting(&self_info);
        }
        break;
    case 12:
        {
        Winks_DiyInfo_s DiyInfo;
        
        strcpy(DiyInfo.number,"15811077465");
        DiyInfo.image_type = 1;
        DiyInfo.image_count = 2;
    #if 1//def WINKS_MTK
        //Ascii2UnicodeString(DiyInfo.image_files[1], WINKS_APP_PATH"red.jpg" );
        memcpy(DiyInfo.image_files[0], L"c:\\winks\\red.jpg", 34);
        //Ascii2UnicodeString(DiyInfo.image_files[0], WINKS_APP_PATH"Water.jpg" );
        memcpy(DiyInfo.image_files[1], L"c:\\winks\\Water.jpg", 38);
    #else
        strcpy(DiyInfo.image_files[0],WINKS_APP_PATH"red.jpg");
        strcpy(DiyInfo.image_files[1],WINKS_APP_PATH"Water.jpg");
    #endif
        strcpy(DiyInfo.welcome,"DiyInfo.welcome");

        iRet = Winks_UserUploadDiyImage(&DiyInfo);
        }
        break;
    case 16:    /* 7FF0H ��ȡ�����б�*/
        {
        iRet = Winks_UserResetFriendList();
        }
        break;
    case 17:    /* 7FEFH �ϴ�����¼ + 7FEBH ��ȡ������Ϣ */
        {
        iRet = Winks_UpdateFriendRecord();
        }
        break;
    case 19:    /* 7FEDH ��ʱͨѶ */
        {
        iRet = Winks_PeriodPushServer();
        }
        break;
    case 20:    /* 7FECH ��ʼ�� */
        {
        iRet = winks_PackDataAndSend(0x7FEC, NULL, &ErrCode);
        }
        break;
    default:
        break;
    }

    return iRet;
}
#endif /* WINKS_DEBUG */

static unsigned long winks_GetDsaLong(unsigned long *oRet, const unsigned char *buff, unsigned long offset,  unsigned long len)
{
    int i = 0;
    unsigned long ret = 0;
    
    Winks_assert(len >= 4);
    for(i = 0; i < 4; ++i)
    {
        ret <<= 8;
        /*retLen &= 0xffffff00;*/
        ret |= buff[offset++];
    }

    if(oRet != NULL) *oRet = ret;
    return ret;
}

static unsigned short winks_GetDsaShort(unsigned short *oRet, const unsigned char *buff, unsigned long offset,  unsigned long len)
{
    int i = 0;
    unsigned short ret = 0;
    
    Winks_assert(len >= 2);
    for(i = 0; i < 2; ++i)
    {
        ret <<= 8;
        /*retType &= 0xff00;*/
        ret |= buff[offset++];
    }
    
    if(oRet != NULL) *oRet = ret;
    return ret;
}

static int winks_SetDsaLong(unsigned char *obuff, unsigned char buff_len,  unsigned long len)
{
    int i = 0;
    
    Winks_assert(buff_len >= 4);
    for(i = 0; i < 4; ++i)
    {
        obuff[3-i]=(unsigned char)(len&0x000000ff);
        len >>=8;
    }
    
    return 0;
}

static int winks_SetDsaShort(unsigned char *obuff, unsigned char buff_len,  unsigned short len)
{
    int i = 0;
    
    Winks_assert(buff_len >= 2);
    for(i = 0; i < 2; ++i)
    {
        obuff[1-i]=(unsigned char)(len&0x00ff);
        len >>=8;
    }
    
    return 0;
}

/* ����򷵻�������Ľ���������Ҫ����ʱ�洢�ռ� */
static void * winks_EngGetAnalyzeData( void **analyzeData, int Size )
{
#if 0
    if((analyzeData == NULL) || !Size)
        return NULL;
#endif
    if(*analyzeData)
        return *analyzeData;
    
    if((*analyzeData = Winks_salAlloc(Size)) == NULL)
    {
        Winks_printf("ERROR: Winks_salAlloc(%d)\r\n", Size);
        Winks_assert(*analyzeData != NULL);
        return NULL;
    }
    memset(*analyzeData, 0, Size);
    return *analyzeData;
}

#define winks_EngAnalyzeErr(ErrCode)   \
    (Winks_printf("AnalyzeErr(L%04d): 0x%x %s\r\n", __LINE__, ErrCode, #ErrCode),  \
    *ret_code = (unsigned long)ErrCode)

/* ��HTTP�����ж�ȡ����readlen�ֽڳ������ݵ�ָ��pbuf(�������bufsize,����Ҫ��(bufsize >= readlen)), */
static int winks_EngReadHttpBuf(int http, int offset, char* pbuf, int bufsize, int readlen, unsigned long *ret_code)
{
    int retlen = 0;

    if(/*(pbuf == NULL) || */ (readlen < 1) || (bufsize < readlen))
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
        return WINKS_RETURN_FAILURE;
    }
    /*Winks_mem_set(pbuf, 0, sizeof(bufsize));*/
    retlen = Winks_httprecv(http, NULL, offset, pbuf, readlen);
    if(retlen != readlen)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

#ifdef WINKS_DEBUG    
static int winks_engAnalyzePrint(int http, unsigned long length, unsigned long *ret_code)
{
    unsigned char dbgBuf[2048];
    unsigned long readlen =  MIN(sizeof(dbgBuf), length);

    if(winks_EngReadHttpBuf(http, 0, (char *)dbgBuf, sizeof(dbgBuf), readlen, ret_code) != WINKS_RETURN_SUCCESS)
        return -1;
    if((dbgBuf[24] == (unsigned char)(0xff)) && (dbgBuf[25] == (unsigned char)0xfa))
        Winks_printHex((unsigned char*)dbgBuf, (unsigned short)MIN(readlen,128), (unsigned char *)"[ENG ANALYZE + + + + + + + + + ]:\r\n");
    else
        Winks_printHex((unsigned char*)dbgBuf, (unsigned short)readlen, (unsigned char *)"[ENG ANALYZE + + + + + + + + + ]:\r\n");
    return 0;
}
#endif

static int winks_EngDataStreamAnalyze(int http, unsigned long length, unsigned long *ret_code)
{
    unsigned char buff[WINKS_ENG_DS_HEAD_LEN];
    unsigned char tmp_buff[WINKS_ENG_DS_LT_LEN];
    Winks_EngDSABaseInfo_s dsa_head;
    unsigned long offset = 0;
    unsigned long msgLen = 0; 
    unsigned short msgType = 0;
    unsigned long data_len = 0;
    void *analyzeData = NULL;
    Winks_HttpInfo  HttpInfo;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    *ret_code = (unsigned long)WINKS_MSG_ENG2UI_NONE;
    
#ifdef WINKS_DEBUG    
#ifdef WINKS_SIMULATOR
    if(winks_engAnalyzePrint(http, length, ret_code) == -1)
        return 0;
#endif
#endif

#if 1
    msgLen = Winks_httprecv(http, &HttpInfo, offset, (char *)buff, WINKS_ENG_DS_HEAD_LEN);
    if((HttpInfo.statuscode >= HTTP_OK) && (HttpInfo.statuscode < HTTP_MultipleChoices))/* [200,300) ����OK */
    {
        if(msgLen != WINKS_ENG_DS_HEAD_LEN)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
            return WINKS_RETURN_FAILURE;
        }
    }
    else
    {
        Winks_printf("ERROR: HTTP statuscode[%d] : %s\r\n", HttpInfo.statuscode, Winks_httpStatus(HttpInfo.statuscode));
    #if 1
        if(HttpInfo.statuscode >= HTTP_BadRequest) /* >=400 ���Ӵ������������� */
        {
            winks_EngAnalyzeErr(WINKS_MSG_ENG2UI_SVR_NO_ANSWER);
            return WINKS_RETURN_FAILURE;
        }
        else/* [300,400) �ض��� */
        {
            return WINKS_RETURN_FAILURE;
        }
    #else
        /* ����������� */
        switch(HttpInfo.statuscode)
        {
        case HTTP_BadGateway:
            *ret_code = (unsigned long)WINKS_MSG_ENG2UI_SVR_NO_ANSWER;
            break;
        default:
            *ret_code = (unsigned long)WINKS_MSG_ENG2UI_SVR_NO_ANSWER;
            break;
        }
        return WINKS_RETURN_FAILURE;
    #endif
    }
#else
    if(winks_EngReadHttpBuf(http, 0, (char *)buff, sizeof(buff), WINKS_ENG_DS_HEAD_LEN, ret_code) != WINKS_RETURN_SUCCESS)
        return 0;
#endif
    /*Winks_printHex( buff, (unsigned short)sizeof(buff), "Data stream analyze HEAD:\r\n");*/

    if(length < WINKS_ENG_DS_HEAD_LEN)
    {
        Winks_assert(length >= WINKS_ENG_DS_HEAD_LEN);
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
        return 0;
    }

    Winks_mem_set(&dsa_head, 0, sizeof(dsa_head));

    winks_GetDsaLong(&dsa_head.size, buff, offset, WINKS_ENG_DS_4BYTES_LEN);
    offset += WINKS_ENG_DS_4BYTES_LEN;
    if(dsa_head.size != length)
    {
        Winks_printf("Data stream error! [%d]!=[%d]",length,dsa_head.size);
        /*Winks_assert(dsa_head.size == length);*/
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
        return 0;
    }
    
    offset += WINKS_ENG_DS_LT_LEN;
    dsa_head.version = buff[offset++];
    
    offset +=WINKS_ENG_DS_LT_LEN;
    dsa_head.way = buff[offset++];
    
    offset +=WINKS_ENG_DS_LT_LEN;
    winks_GetDsaShort(&dsa_head.msg_type, buff, offset, WINKS_ENG_DS_2BYTES_LEN);
    offset += WINKS_ENG_DS_2BYTES_LEN;
    
    offset +=WINKS_ENG_DS_LT_LEN;
    winks_GetDsaShort(&dsa_head.sequence_num, buff, offset, WINKS_ENG_DS_2BYTES_LEN);
    offset += WINKS_ENG_DS_2BYTES_LEN;

    Winks_printf("DSA head:  offset[%ld]  len[%d=0x%04x]  version[0x%0x]  way[0x%0x]  msg_type[0x%02x]  seq_num[0x%02x]\r\n",
        offset, dsa_head.size, dsa_head.size, dsa_head.version, dsa_head.way, dsa_head.msg_type, dsa_head.sequence_num);

    data_len = dsa_head.size - WINKS_ENG_DS_HEAD_LEN;

#ifdef WINKS_DEBUG
	#ifdef WINKS_SIMULATOR/* �����ֻ�stack��� */
if(dsa_head.msg_type==0xfff0)
{
    char tbf[1024];
    int tbl = 0;
    tbl = Winks_httprecv(http, &HttpInfo, 0, tbf, sizeof(tbf));
    Winks_printHex((const unsigned char *)tbf, tbl, (const unsigned char *)"[- 0xfff0 -]");
}
    #endif
#endif
#if !(defined(DBG_UNSUPPORT_PROTOCOL) || defined(WINKS_ENGINE_TEST_5PROTOCAL))
    if(dsa_head.sequence_num!=Winks_engineContext_p->ProtocolFrameNo)/* ����� */
    {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
            return WINKS_RETURN_SUCCESS;
    }
#endif
    if(dsa_head.msg_type!=0xffff)
    {
        if((Winks_engineContext_p->ProtocolMsgType^dsa_head.msg_type)!= 0x8000)/* Э��Ŵ� */
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_PROT_NO);
            return WINKS_RETURN_SUCCESS;
        }
    }
    
    if(dsa_head.msg_type==0xfff3)
    {
        *ret_code = 0;
        return WINKS_RETURN_SUCCESS;
    }
    
    else if( (dsa_head.msg_type == 0xFFF9) &&
        (Winks_engineContext_p->curState == WINKS_ENGINE_STATE_DISABLED) &&
        (Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ONE) )/* ��ʼע�� */
    {
        int iRet = 0;
    #if 0//def WINKS_DEBUG
        Winks_FlashSpace_s flashInfo;
    #endif
        /*�п����ϴ�δע���꣬������һ����������Ϣ�����ɾ����Ҫ�������أ��˷Ѵ����ʱ�䣬�����ʱ��ɾ�������*/
        Winks_printf("<STRG>Winks_delete_storage_files(1) Begin reg\r\n");
        iRet = Winks_StrgDeleteDiskFiles(3);
        if(iRet != 0)
        {
            *ret_code = WINKS_ENG_ERROR_CALL_FUNC;
            return WINKS_RETURN_SUCCESS;
        }
        Winks_StrgDeinitMapFile();
        Winks_StrgInitMapFile();
        Winks_StrgFLDeleteAll();
        Winks_StrgNDDeleteAll();
        
        #if 0//def WINKS_DEBUG
        memset(&flashInfo, 0, sizeof(flashInfo));
        if(Winks_GetFlashSpace(&flashInfo) == WINKS_RETURN_SUCCESS)
        {
            Winks_printf("flashInfo. total=%lu free=%lu\r\n", flashInfo.total, flashInfo.free);
        }
    #endif
    }
    
    while(offset < length)
    {
        if(winks_EngReadHttpBuf(http, offset, (char *)tmp_buff, sizeof(tmp_buff), WINKS_ENG_DS_LT_LEN, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;

        offset += WINKS_ENG_DS_LT_LEN;

        winks_GetDsaShort(&msgType, tmp_buff, 0, WINKS_ENG_DS_2BYTES_LEN);
        winks_GetDsaLong(&msgLen, tmp_buff, WINKS_ENG_DS_2BYTES_LEN, WINKS_ENG_DS_4BYTES_LEN);

        Winks_printf("DSA(0x%04X): msgType=0x%04x offset=%-4d msgLen=%-4d\r\n", dsa_head.msg_type, msgType, offset, msgLen);
        switch(dsa_head.msg_type)
        {
        case 0xFFEB:
        {
            winks_EngDsaDo_LFFEB(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFEC:
        {
            winks_EngDsaDo_LFFEC(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFED:
        {
            winks_EngDsaDo_LFFED(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFEF:
        {
            winks_EngDsaDo_LFFEF(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFF0:
        {
            winks_EngDsaDo_LFFF0(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFF9:
        {
            winks_EngDsaDo_LFFF9(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFFD:
        {
            winks_EngDsaDo_LFFFD(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFFF:        /* ������ */
        {
            winks_EngDsaDo_LFFFF(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFFA:
        {
            winks_EngDsaDo_LFFFA(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFF8:
        {
            winks_EngDsaDo_LFFF8(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
#ifdef WINKS_DEL_PV2_0
        case 0xFFF7:
        {
            winks_EngDsaDo_LFFF7(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
#endif
        case 0xFFF6:
        {
            winks_EngDsaDo_LFFF6(msgType, msgLen, http, offset, ret_code, &analyzeData);
            break;
        }
        case 0xFFF3:
            break;
        default:
            break;
        }

        /*������*/
        if(*ret_code != WINKS_MSG_ENG2UI_NONE)
        {
            if(analyzeData != NULL)/* ������Ҫ�ͷ���ʱ���� */
            {
                Winks_salFree(analyzeData);
                analyzeData = NULL;
            }
            return 0;
        }

        offset += msgLen;
    }   
    
    if(analyzeData != NULL)
    {
        switch(dsa_head.msg_type)
        {
        case 0xFFF0:
            //if(Winks_engineContext_p->setting.ffl != NULL)
                break;
        case 0xFFEC:/*��ʱ����*/
        case 0xFFED:/*��ʼ��*/
        {
            Winks_EngTimeFlag_s *pNewTimeFlag = (Winks_EngTimeFlag_s *)analyzeData;
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 0 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 1 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 2 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 3 );
            break;
        }
        case 0xFFEB:/*���غ�����Ϣ*/
        {
            Winks_EngTimeFlag_s *pNewTimeFlag = (Winks_EngTimeFlag_s *)analyzeData;
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 0 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 1 );
            break;
        }
        default:
            break;
        }
        
        Winks_salFree(analyzeData);
        analyzeData = NULL;
    }
    
    return WINKS_RETURN_SUCCESS;
}

/* ��2/3��ṹ��ͨ�ý������� */
static unsigned long winks_EngDsaDo_struct(int http, unsigned long offset, unsigned long *ret_code, wink_DSA_StructInfo *data_info)
{
    unsigned short msgType = 0;
    unsigned long msgLen = 0; 
    unsigned char tmp_buff[WINKS_ENG_DS_LT_LEN];

    Winks_assert(ret_code != NULL);
    Winks_assert(data_info != NULL);

    *ret_code = 0;
    Winks_mem_set(data_info, 0, sizeof(wink_DSA_StructInfo));

    if(winks_EngReadHttpBuf(http, offset, (char *)tmp_buff, sizeof(tmp_buff), WINKS_ENG_DS_LT_LEN, ret_code) != WINKS_RETURN_SUCCESS)
        return 0;

    winks_GetDsaShort(&msgType, tmp_buff, 0, WINKS_ENG_DS_2BYTES_LEN);
    winks_GetDsaLong(&msgLen, tmp_buff, WINKS_ENG_DS_2BYTES_LEN, WINKS_ENG_DS_4BYTES_LEN);

    data_info->type = msgType;
    data_info->len = msgLen;
    data_info->offset = offset + WINKS_ENG_DS_LT_LEN;
    /*Winks_printf("struct: msgType = %x msgLen = %d offset = %d\r\n", data_info->type, data_info->len, data_info->offset);*/

    Winks_assert(msgLen > 0);
    return (WINKS_ENG_DS_LT_LEN + msgLen);      /* ���������ֶγ��� */
}

/* ����Э���е�3��ṹ, ͨ�õĵ�2������������� */
static int winks_EngDsaDo_M_array(int http, unsigned long offset, unsigned short msgType, unsigned long len, unsigned long *ret_code)
{
    void *ret_data = NULL;      /* NOTE: ��parseFunc()������ */
    unsigned long rlen = 0;
    unsigned long end = 0;
    unsigned long index = 0;
    winks_DSA_H_Table body;
    wink_DSA_StructInfo data_info;
    unsigned long i = 0;

#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
    const winks_DSA_H_Table table[] = 
    {
        {0xFFF8, winks_EngDsaDo_HFFF8, winks_retData_HFFF8},
        {0xFFFA, winks_EngDsaDo_HFFFA, NULL},
        {0xFFFB, winks_EngDsaDo_HFFFB, NULL},
        {0xFFFC, winks_EngDsaDo_HFFFC, NULL},
        {0xFFFF, winks_EngDsaDo_HFFFF, NULL}
    };
#else
    winks_DSA_H_Table table[5];
    table[0].msgType = 0xFFF8;
    table[0].parseFunc = winks_EngDsaDo_HFFF8;
    table[0].retData = winks_retData_HFFF8;
    table[1].msgType = 0xFFFA;
    table[1].parseFunc = winks_EngDsaDo_HFFFA;
    table[1].retData = NULL;
    table[2].msgType = 0xFFFB;
    table[2].parseFunc = winks_EngDsaDo_HFFFB;
    table[2].retData = NULL;
    table[3].msgType = 0xFFFC;
    table[3].parseFunc = winks_EngDsaDo_HFFFC;
    table[3].retData = NULL;
    table[4].msgType = 0xFFFF;
    table[4].parseFunc = winks_EngDsaDo_HFFFF;
    table[4].retData = NULL;
#endif

    *ret_code = 0;
    end = offset + len;

    Winks_mem_set(&body, 0, sizeof(body));

    /* ���������Ӧ�Ľ������� */
    for (i=0; i<WINKS_ARRAY_SIZE(table); ++i)
    {
        if (msgType == table[i].msgType)
        {
            body= table[i];
            break;
        }
    }
    /* ���û���ҵ���Ӧ�Ľ������� */
    if (i >= WINKS_ARRAY_SIZE(table))
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
        return 0;
    }
    Winks_assert(body.parseFunc != NULL);
    
    while (offset < end)
    {
        rlen = winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        if(*ret_code != 0UL)
        {
            goto FREE_and_RET;
        }
        
        /*Winks_printf("DSA_M: msgType = %x msg_sn = %d msg_len = %d\r\n", msgType, msg_sn, msg_len);*/
        /*Winks_assert(data_info.type == index + 1);*/
        body.parseFunc(http, index, data_info.offset, data_info.len, ret_code, &ret_data);
        if(*ret_code != 0UL)
        {
            goto FREE_and_RET;
        }
        
        offset += rlen;
        ++index;
    }

    /* ���������ķ������� */
    if (body.retData)
    {
        body.retData(ret_code, ret_data);
        if(*ret_code != 0UL)
        {
            goto FREE_and_RET;
        }
    }
    
FREE_and_RET:
    /* �ͷ� parseFunc()��������ڴ�*/
    if (ret_data != NULL)
    {
        Winks_salFree(ret_data);
    }

    return 0;
}

static void winks_EngProtocolUnsupported(unsigned short AttrId, unsigned short SeqNo)
{
    Winks_printf("UUUU Э�鲻֧������ -- [%04x] [%02x] \r\n", AttrId, SeqNo);
}

static int winks_EngDsaDo_HFFF8(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
    /*
    ���÷������ظ�ͨ�ŷ�������ַ��Ϣ����            ( FFF8H )
    1)  IP����  ( 01H )
    2)  PORT����    ( 02H )
    */
    unsigned long rlen = 0, end = 0;
    wink_DSA_StructInfo data_info;
    Winks_server_list_s *p_server = NULL;
    unsigned char ip[20];
    unsigned char port[8];

    ip[0] = 0;
    port[0] = 0;
    end = offset + len;

    /*** NOTE: 
     ***    ������Ҫ�������ݵ�, �ڴ��������1��Ԫ��ʱ��̬���롣
    ***/
    if (index == 0)
    {
        Winks_assert(*ret_data == NULL);
        p_server = Winks_salAlloc(sizeof(Winks_server_list_s));
        if (p_server == NULL)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        Winks_mem_set(p_server, 0, sizeof(Winks_server_list_s));
        *ret_data = p_server;
    }
    else
    {
        p_server = *ret_data;
    }
    
    if (p_server->cnt >= WINKS_SERVER_CNT)      /* ������ౣ��3�������"ip+port" */
    {
        return 0;
    }

    /* �������·����ַ�������0��β, ʹ��length */
    while (offset < end)
    {
        rlen = winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        if(*ret_code != 0UL)
        {
            return 0;
        }
        offset += rlen; 
            
        switch (data_info.type)
        {
        case 0x01:/* ip */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)ip, 15/*sizeof(ip)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            ip[data_info.len] = 0;
            break;
        case 0x02:/* port */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)port, 6/*sizeof(port)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            port[data_info.len] = 0;
            break;
        default:
            winks_EngProtocolUnsupported(0xFFF8, data_info.type);
            break;
        }
    }
    
    snprintf(p_server->ip_port[p_server->cnt], sizeof(p_server->ip_port[p_server->cnt])-1, "%s:%s", ip, port);
    p_server->ip_port[p_server->cnt][sizeof(p_server->ip_port[p_server->cnt])-1] = 0;
    
    ++(p_server->cnt);
    
    return 0;
}

static int winks_EngDsaDo_LFFEB(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
/*
    3.1.17 Feature �ͻ�����ͨ�ŷ������Ļ�ȡָ���������Ϣ��������

    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��7FEB H 
    2)  �����绰��������                        ��0008H��
    3)  �ֿ��´������������Ϣ����          ��8008H��
    4)  �ֻ�֧�ֵ���Ƭ��Ϣ����               (002BH) 

    2.  ͨѶ�������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��FFEBH
    2)  �û���Ƭ��Ϣ����                    ��FFFFH�������и����򷵻ش����ԣ�
    3)  �û����Ѳ���ID��Ϣ����              ��FFFCH�������и����򷵻ش����ԣ�
    4)  �ֿ��´������������Ϣ����      ��8008H��
*/

//  int iRet = 0;

    *ret_code = 0;

    switch(msgType)
    {
    case 0x8008:
        winks_EngDsaDo_M8008(http, offset, msgLen, ret_code);
        break;
    case 0xFFFC:
    case 0xFFFF:
        {
        winks_EngDsaDo_M_array(http, offset, msgType, msgLen, ret_code);
        }
        break;
    case 0x0020:/*ʱ���*/
        {
        char buff[WINKS_MAX_LEN_OF_TIME_FLAG];
        Winks_EngTimeFlag_s *pTimeFlag = (Winks_EngTimeFlag_s *)winks_EngGetAnalyzeData(analyzeData, sizeof(Winks_EngTimeFlag_s));
        if(pTimeFlag == NULL)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
            
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, WINKS_MAX_LEN_OF_TIME_FLAG/*sizeof(buff)*/, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        buff[19] = 0;/*ʱ���19λ*/
        
        memcpy(pTimeFlag->TimeFlag, buff, strlen(buff)+1);
        }
        break;
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_LFFEC(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
/*
    3.1.15 Feature �ͻ�����ͨ�ŷ�������ʼ����������

    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                     ֵΪ��7FECH 
    2)  �����绰��������            ��0008H��
    3)  �ͻ���������Ϣ              ��8000H��
    4)  ��ʼ���û��绰���뱾���ԣ�0011H��
    5)  �û��ֻ��ռ��С            ��0025H��

    2.  ͨѶ�������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                     ֵΪ��FFEC H
    2)  ��Ӫ���ṩ�ն����ӷ�ʽ����  ��8005H��
    3)  Ƭͷ���ű�־����                ��001BH��
    4)  �������봮����                  ��002AH��
    5)  Ƭͷ������Ϣ����                ��FFFBH��
    6)  ��Ӫ�̲�����Ϣ����              ��FFFAH��
    7)  ����ʱ���                          ��0020H��
    8)  �����ն��û�ʱ��                ��0024H����֧��push�ն�ʹ�ã�
    9)  �û����ѹ�ϵ���±�־����    ��002CH��
    10) �ֻ�Ԥ���ռ���Ϣ����            ��8009H��
*/

    int ret = 0;
    unsigned long push_gu = 0;  /* push ����ʱ�� */
    char buf[WINKS_EMERGENCY_NO_MAX_LEN];
    Winks_ConnectWayOperation_s cwo;

    buf[0] = 0;
    *ret_code = 0;

    switch(msgType)
    {

    /* //�ð汾������Ƭͷ���ű�־��־����Ƭͷ�Ͳ��ţ�û���򲻲��š�
    case 0x001B:
        if(winks_EngReadHttpBuf(http, offset, buf, 1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        if((buf[0] == '\1') || (buf[0] == '\2'))
            ret += Winks_SetPriorColorCallPlaySwitch(buf[0]-1);
        break;*/
    case 0x0020:/*ʱ���*/
        {
        Winks_EngTimeFlag_s *pTimeFlag = (Winks_EngTimeFlag_s *)winks_EngGetAnalyzeData(analyzeData, sizeof(Winks_EngTimeFlag_s));
        if(pTimeFlag == NULL)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
            
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, WINKS_MAX_LEN_OF_TIME_FLAG/*sizeof(buf)*/, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buf[msgLen] = 0;
        buf[19] = 0;/*ʱ���19λ*/
        memcpy(pTimeFlag->TimeFlag, buf, strlen(buf)+1);
        }
        break;
    case 0x0024:
    {
        Winks_mem_set(buf, 0, sizeof(buf));
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, 10/*sizeof(buf)*/,  msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buf[sizeof(buf)-1] = 0;
        push_gu = (unsigned long)atol(buf);
        
        Winks_mem_set(&cwo, 0, sizeof(cwo));
        Winks_GetSupportedConnectWays(&cwo);
        Winks_ConnectWaysAdjust(&cwo);
        if((push_gu<=WINKS_PUSH_GIVE_UP_TIMER_MAX)&&(push_gu>=WINKS_PUSH_GIVE_UP_TIMER_MIN))
        {
            cwo.spTimeGiveUp = push_gu;
        }
        ret += Winks_SetSupportedConnectWays(&cwo);
        break;
    }
    /*  //�������벻�ɷ��������ƣ��ն���Ϊ3λ�ĺ���λ�������롣
    case 0x002A:
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, sizeof(buf-1), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        ret += Winks_SetEmergencyNumber(buf, MAX(msgLen,sizeof(buf-1)));
        break;*/
    /*  // 3.15�в���Ҫ�������Ϣ���� ,��������Ĭ�ϸ��º��ѡ�
    case 0x002C:        
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, 1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        break;*/

    case 0x8005:
        winks_EngDsaDo_M8005(http, offset, msgLen, ret_code, &cwo);
        ret += Winks_SetSupportedConnectWays(&cwo);
        break;
    case 0x8009:
        winks_EngDsaDo_M8009(http, offset, msgLen, ret_code);
        break;

    case 0xFFFA:        /* ��Ӫ�̲�����Ϣ����*/
    case 0xFFFB:        /* Ƭͷ������Ϣ����*/
        {
        winks_EngDsaDo_M_array(http, offset, msgType, msgLen, ret_code);
        }
        break;
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    if (ret != 0)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_LFFED(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
/*
    3.1.19 Feature�ͻ�����ͨѶ�������Ķ�ʱͨѶ���̣�
    ��ͬ��ȫ��ϵͳ������Ϣ���û�ѡ�����ѵĸ�����Ϣ��

    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��7FEDH
    2)  �����绰��������            ��0008H��
    3)  ����ʱ���                      ��0020H��
    4)    �ֻ�֧�ֵ���Ƭ��Ϣ����       (002BH)
    5)  �ͻ��˵绰����������        ��FFFDH��

    2.  ͨѶ�������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��FFEDH
    2)  ��Ӫ���ṩ�ն����ӷ�ʽ����  ��8005H�������и����򷵻ش����ԣ�
    3)  Ƭͷ���ű�־����                ��001BH�������и����򷵻ش����ԣ�
    4)  �������봮����                  ��002AH�������и����򷵻ش����ԣ�
    5)  �û���Ƭ��Ϣ����                ��FFFFH�� ���û���ѡ�񱣴�ĺ��Ѻ����¸��µĺ��ѵ���Ƭ��Ϣ��
    6)  �û����Ѳ���ID��Ϣ����          ��FFFCH�� ���û���ѡ�񱣴�ĺ��Ѻ����¸��µĺ��ѵĲ���ID��Ϣ��
    7)  Ƭͷ������Ϣ����                ��FFFBH�� �����и����򷵻ش����ԣ�
    8)  ��Ӫ�̲�����Ϣ����              ��FFFAH�� �����и����򷵻ش����ԣ�
    9)  ����ʱ���                          ��0020H��
    10)    �ϴ��ֻ�ͳ����Ϣ��������     (0027H) ��������Ҫ�򷵻ش����ԣ�
    11)    �û����ѹ�ϵ���±�־����    ��002CH��(�����������б���·��˱�־ )
*/

    int ret = 0;

    char buf[32];
    unsigned char update_flag = 0;      /* ���Ѹ��±�־ */
    Winks_ConnectWayOperation_s cwo = {0};

    buf[0] = 0;
    *ret_code = 0;

    #if 0 //TODO: modify
    if(winks_IsUsbState() == 1)
    {
        Winks_EngErr("winks_IsUsbState");
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
        return WINKS_RETURN_FAILURE;
    }
    #endif
    Winks_mem_set(buf, 0, sizeof(buf));
    switch(msgType)
    {
    /* //�ð汾������Ƭͷ���ű�־��־����Ƭͷ�Ͳ��ţ�û���򲻲��š�
    case 0x001B:
        if(winks_EngReadHttpBuf(http, offset, buf, sizeof(buf), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        if((buf[0] == '\1') || (buf[0] == '\2'))
            ret += Winks_SetPriorColorCallPlaySwitch(buf[0]-1);
        break;*/

    case 0x0020:/*ʱ���*/
    {
        Winks_EngTimeFlag_s *pTimeFlag = (Winks_EngTimeFlag_s *)winks_EngGetAnalyzeData(analyzeData, sizeof(Winks_EngTimeFlag_s));
        if(pTimeFlag == NULL)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, sizeof(buf)-1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        Winks_mem_cpy(pTimeFlag->TimeFlag, buf, WINKS_CONN_TIME_FLAG_MAX_LEN);
        break;
    }

    case 0x0024:
    {
        long push_gu = 0;
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, 10/*sizeof(buf)*/,  msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        push_gu = atol(buf);
        
        Winks_mem_set(&cwo, 0, sizeof(cwo));
        Winks_GetSupportedConnectWays(&cwo);
        Winks_ConnectWaysAdjust(&cwo);
        if((push_gu<=WINKS_PUSH_GIVE_UP_TIMER_MAX)&&(push_gu>=WINKS_PUSH_GIVE_UP_TIMER_MIN))
        {
            cwo.spTimeGiveUp = push_gu;
        }
        ret += Winks_SetSupportedConnectWays(&cwo);
        break;
    }

    /*  //�ϴ��ֻ�ͳ����Ϣ��־, Ŀǰ��������֧�ָù��ܡ�
    case 0x0027:
        Winks_printf("ERROR: ����δʵ��\r\n");
        break;*/
    /*  //�������벻�ɷ��������ƣ��ն���Ϊ3λ�ĺ���λ�������롣
    case 0x002A:
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, sizeof(buf-1), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        ret += Winks_SetEmergencyNumber(buf, MAX(msgLen,sizeof(buf-1)));
        break;*/
    case 0x002C:
    {
        Winks_EngPeriodComInfo_s *pPeriodComInfo = winks_EngGetPeriodComInfoContext();

        if(winks_EngReadHttpBuf(http, offset, (char *)&update_flag, sizeof(update_flag), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        if(pPeriodComInfo)/*����Ҫ���£���pPeriodComInfo->FA_UpdFlag��1,֮��Ĵ���ֻʶ��1.*/
        {
            Winks_printf("****���ѹ�ϵ�䶯:%d\r\n", update_flag);
            pPeriodComInfo->FA_UpdFlag = (update_flag);
        }
        break;
    }
    
    case 0x8005:
        winks_EngDsaDo_M8005(http, offset, msgLen, ret_code, &cwo);
        ret += Winks_SetSupportedConnectWays(&cwo);
        break;
        
    case 0xFFFA:
    case 0xFFFB:
    case 0xFFFC:
    case 0xFFFF:
    {
        winks_EngDsaDo_M_array(http, offset, msgType, msgLen, ret_code);
        break;
    }
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    if (ret != 0)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_LFFEF(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
/*
    3.1.18 Feature �ͻ�����ͨ�ŷ�������֪ͨ���������޸ĵĺ�����Ϣ��������

    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                     ֵΪ��7FEFH 
    2)  �����绰��������                            ��0008H��
    3)  Feature���޸ı��ر���ĺ�����Ϣ���� ��FFF9H��

    2.  ͨѶ�������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                     ֵΪ��FFEFH
    2)  �ֿ��´������������Ϣ����              ��8008H��

*/

//  int iRet = 0;

    *ret_code = 0;

    switch(msgType)
    {
    case 0x8008:
        winks_EngDsaDo_M8008(http, offset, msgLen, ret_code);
        break;
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_LFFF0(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
/*
    3.1.16 Feature �ͻ�����ͨ�ŷ������Ļ�ȡȫ������¼������Ϣ��������

    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��7FF0H 
    2)  �����绰��������                            ��0008H��

    2.  ͨѶ�������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��FFF0H
    2)  �û������б������Ϣ����                (002DH)
    3)    ����ʱ���                    ��0020H��
*/

    int ret = 0;
    char buff[1024];
    //int flag = 0;

    memset(buff, 0, sizeof(buff));
    *ret_code = 0;

    switch(msgType)
    {
    case 0x002D:
    {
    
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();
    
        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_INIT_CACHE)
        {
            char    * pFL;
            
            Winks_assert(Winks_engineContext_p->communicationContext_p == NULL);
            if((Winks_engineContext_p->communicationContext_p = winks_CommContextAlloc(msgLen+1)) == NULL)
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
                return 0;
            }

            pFL = (char *)Winks_engineContext_p->communicationContext_p;
            if(winks_EngReadHttpBuf(http, offset, pFL, msgLen, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            pFL[msgLen] = 0;
        }
        break;
    }
    case 0x0020:/*ʱ���*/
    {
        char buff[WINKS_MAX_LEN_OF_TIME_FLAG];
        Winks_EngTimeFlag_s *pTimeFlag = (Winks_EngTimeFlag_s *)winks_EngGetAnalyzeData(analyzeData, sizeof(Winks_EngTimeFlag_s));
        if(pTimeFlag == NULL)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
            
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, WINKS_MAX_LEN_OF_TIME_FLAG/*sizeof(buff)*/, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        buff[19] = 0;/*ʱ���19λ*/
        
        memcpy(pTimeFlag->TimeFlag, buff, strlen(buff)+1);
        break;
    }

    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    if (ret != 0)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_LFFF9(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{       
    /*
    �ͻ����������֮��ļ򵥽�������
    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��7FF9 H
    2)  IMSI������              ��000FH����JAVA�ն��޴����ԣ�
    3)  �����绰����            ��0008H�������ܻ�ȡ�ն�ר�ã�

    2.  ���÷������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��FFF9H
    2)  ���÷������ظ�ͨ�ŷ�������ַ��Ϣ���ԣ�FFF8H��
    3)  ƽ̨���ŵ��ط�����      ��0010H��
    4)  ���Ͷ��Ŷ�����ʱ������  ��0018H��
    5)  ���Ͷ��ų�����ʱ������  ��0019H��
    6)  �ͻ��˼���ƽ̨����ʱʹ�õĶ˿�����(001AH)
    */
    int iRet = 0;

    switch(msgType)
    {
    case 0xFFF8:
    {
        winks_EngDsaDo_M_array(http, offset, msgType, msgLen, ret_code);
        break;
    }

    case 0x0010:/*ƽ̨��������*/
    {
        char buff[WINKS_MAX_LEN_OF_MSG_GATEWAY];
        if(winks_EngReadHttpBuf(http, offset, buff, sizeof(buff)-1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        Winks_printf("Huaxm:: gateway[%s]\n ",buff);
        /*iRet  = Winks_SetSmsGateway(buff, strlen(buff));*/
        winks_EngSetSmsGateway(buff, strlen(buff));
        if(iRet != 0)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        
        break;
    }
    
    case 0x0018:
    case 0x0019:
    {
        unsigned char buff[10];
        int time = 0;
        
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, sizeof(buff)-1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        time = atoi((char *)buff);
        if(msgType == 0x0018)/*����������*/
        {
            Winks_printf("Huaxm::short connect time[%d]\n ",time);
            if((time<WINKS_SMS_SHORT_PRIEOD_MIN) || (time>WINKS_SMS_SHORT_PRIEOD_MAX))/* �������·�ֵ���� */
            {
                if((Winks_get_setting_value(E_SETTING_SHORT_CYCLE, (unsigned long *)&time) != WINKS_RETURN_SUCCESS) || /* ����û�б���ֵ */
                    (((time<WINKS_SMS_SHORT_PRIEOD_MIN) || (time>WINKS_SMS_SHORT_PRIEOD_MAX))) )/* ���ر���ֵҲ���� */
                    time = WINKS_SMS_SHORT_PRIEOD_DFT;
                Winks_printf("ERROR(L%4d): ServerData -- ���������� time=%d\r\n", __LINE__, time);
            }
            winks_EngSetQueryShortTime((unsigned long)time);
        }
        else /* ���������� */
        {
            Winks_printf("Huaxm::long connect time[%d]\n ",time);
            if((time<WINKS_SMS_LONG_PRIEOD_MIN) || (time>WINKS_SMS_LONG_PRIEOD_MAX))/* �������·�ֵ���� */
            {
                if((Winks_get_setting_value(E_SETTING_LONG_CYCLE, (unsigned long *)&time) != WINKS_RETURN_SUCCESS) || /* ����û�б���ֵ */
                    (((time<WINKS_SMS_LONG_PRIEOD_MIN) || (time>WINKS_SMS_LONG_PRIEOD_MAX))) )/* ���ر���ֵҲ���� */
                    time = WINKS_SMS_LONG_PRIEOD_DFT;
                Winks_printf("ERROR(L%4d): ServerData -- ���������� time=%d\r\n", __LINE__, time);
            }
            winks_EngSetQueryLongTime(time);
        }
        break;
    }
    
    case 0x001A:
    {
        unsigned char buff[10];
        unsigned long port = 0;
        
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, sizeof(buff), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        port= atol((char *)buff);
        Winks_printf("Huaxm:: PUSH port[%d]\n ",port);
        iRet  = Winks_SetPushQureyPort(port);
        if(iRet != 0)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        break;
    }
    
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_LFFFD(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{       
    /*
    3.1.2�ͻ�����ͨѶ��������ѯ�Ƿ�ע����̣�

    1.  �ͻ��ˣ�
    ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��7FFD H
    2)  IMSI������              ��000FH����JAVA�ն��޴����ԣ�
    3)  �ն������������        ��0021H��
    4)  �����绰����            ��0008H����JAVA�ն�ר�ã�

    2.  ͨѶ�������ˣ�
    ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
    1)  MessageType����                 ֵΪ��FFFD H
    2)  �����绰����              ��0008H��
    3)  �û�ע����ʾ��            ��0022H��
    */
    int iRet = 0;

    switch(msgType)
    {
    case 0x0008:
    {
        char buff[WINKS_LOCAL_NO_LEN];
        if(winks_EngReadHttpBuf(http, offset, buff, sizeof(buff)-1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        Winks_printf("Huaxm:: Phone NO.[%s]\n ",buff);
        if( (iRet = Winks_SetPhoneNumber(buff, strlen(buff))) != 0  ||
             (iRet = Winks_StrgSetSelfNo(buff)) != 0 )
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        break;
    }
    
    case 0x0022:
    {
        unsigned char buff[WINKS_ENGINE_RETURN_MSG_MAX_LEN];
        
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, sizeof(buff)-1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        winks_EngSetQueryRetMsg((unsigned char *)buff, strlen((char *)buff));
        
        break;
    }
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_EngDsaDo_LFFFF(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{       
    /*
    3.  ����ʱ�������ķ�����ʹ�õĴ������ԣ�
    1)  MessageType����                 ֵΪ��FFFFH
    2)  Error���ԣ� 0005H ����  ���ݾ���������ش���
    ֵ���������ֽ�
    �ͻ�����ش�����뷶Χ��        (0001H  ---  0FFFH)
    ���÷�������ش�����룺        (1001H  ---  1FFFH)
    ͨѶ��������ش�����룺        (2001H  ---  2FFFH)
    ���ݿ���ش�����룺            (4001H  ---  4FFFH)
    ��������                  (5001H  ---  FFFFH)

    1)  �ͻ�����ش���
    a)  δ֪���� �� 0001H ��
    2)  ���÷�������ش��� 
    a)  ���÷������ڲ�����    �� 1002H ��
    b)  ����Ч��ͨ�ŷ�������ַ�� 1003H ��
    c)  ����Ч�Ķ����ط�����  �� 1004H ��
    3)  ͨ�ŷ�������ش���
    a)  ͨ�ŷ������ڲ����� �� 2001H ��
    4)  ��������
    a)  Э���������                    ��1001H��
    b)  �ͻ�û�ж�����Ϣ                    ��4001H��
    c)  �ͻ�û��ƥ�䵱ǰʱ��Ķ��Ʋ���  ��4002H��
    d)  ���ʺϴ��ͺſͻ��˵Ĳ���            ��4003H��
    e)  �����ļ�δ�ҵ�                  ��4004H��
    f)  �û�����Ӫ������                    ��4005H��
    g)  ��������Ϊ�մ���                    ��5001H��
    h)  �Է�����Ϊ�մ���                    ��5002H��
    i)  CRCУ�����                     ��5003H��
    j)  �ϴ��ļ�����                        ��5004H��
    k)  �ϴ��ļ���Ϣ�������                ��5005H��
    l)  �û�δע��                      ��5006H��
    m)  �û���������ϴ�������          ��5007H��
    n)  ������������Ч��ֹͣ�������        ��5008H��
    o)  �û��ϴ��ʺ����޸�ʧ��          ��5009H��
    p)  �����û������ҵ��������      ��500AH��
    q)  �ͻ��˵绰���������Կմ���      ��500BH��
    r)  �ϴ�����ֵ����                  ��500CH��
    s)  �����°汾���                  ��500DH��
    t)  �û������շ��û�                    ��500EH��
    u)  ���»�ȡͨѶ��������ַ          ��500FH��
    */

    switch(msgType)
    {
    case 0x0005:
    {
        unsigned char buff[3];
        unsigned short errCode = 0;
        
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, 2/*sizeof(buff)*/, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;
        
        winks_GetDsaShort(&errCode, buff, 0, WINKS_ENG_DS_2BYTES_LEN);
        *ret_code = ((unsigned long)errCode<<16) ;
        break;
    }

    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return WINKS_RETURN_SUCCESS;
}


static int winks_EngDsaDo_M8005(int http, unsigned long offset, unsigned long len, unsigned long *ret_code, Winks_ConnectWayOperation_s *cwo)
{
/*
    ��Ӫ���ṩ�ն����ӷ�ʽ���ԣ�8005H��
    NOTE: ĿǰFeature Phoneֻ֧�ֶ���push�붨ʱ�������ַ�ʽ��
*/
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    unsigned char push = 0;     /* ����push���� */
    unsigned char wake = 0;     /* ��ʱ���� */
    unsigned char def_way = 0;  /* Ĭ�����ӷ�ʽ */

    char wake_to[12] = {0};      /* wake timeout */
    char wake_itv[12] = {0};     /* wake interval */

    unsigned long iwake_to = 0, iwake_itv = 0;

    *ret_code = 0;
    end = offset + len;


    Winks_mem_set(cwo, 0, sizeof(Winks_ConnectWayOperation_s));
    Winks_GetSupportedConnectWays(cwo);
    Winks_ConnectWaysAdjust(cwo);

    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x04:      /* ����push���� */
        {
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&push, sizeof(push), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            if(push == 1)
                cwo->operation |= CONNECT_WAY_SERVER_PUSH;
            else if(push == 2)
                cwo->operation &= (~CONNECT_WAY_SERVER_PUSH);
            else
                Winks_EngErr("unsupport def_way[0x04]");
            break;
        }
            
        case 0x05:      /* ��ʱ���� */
        {
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&wake, sizeof(wake), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            if(wake == 1)
                cwo->operation |= CONNECT_WAY_TIMER_WAKE_UP;
            else if(wake == 2)
                cwo->operation &= (~CONNECT_WAY_TIMER_WAKE_UP);
            else
                Winks_EngErr("unsupport def_way[0x05]");
            break;
        }
        
        case 0x06:      /* Ĭ�����ӷ�ʽ */
        {
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&def_way, sizeof(def_way), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            if(def_way == 4/*PUSH����*/ || def_way == 5/*��ʱ����*/)
            {
                cwo->operation &= 0x0000ffff;
                if(def_way == 4)
                    cwo->operation |= ((CONNECT_WAY_SERVER_PUSH)<<16);
                else if(def_way == 5)
                    cwo->operation |= ((CONNECT_WAY_TIMER_WAKE_UP)<<16);
            }
            else
            {
                Winks_printf("unsupport def_way[0x06] def_way=[%d]",def_way);
                /*Winks_fail("(def_way == 4 || def_way == 5)");*/
            }
            break;
        }

        case 0x0A:      /* ��ʱ���ѷ�ʽ��ʱͨѶʱ���� */
        {
            Winks_mem_set(wake_itv, 0, sizeof(wake_itv));
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)wake_itv, sizeof(wake_itv), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            wake_itv[sizeof(wake_itv)-1] = 0;
            iwake_itv = (unsigned long)atol(wake_itv);

            Winks_printf("Expirod of server connecting is %d\".\r\n",iwake_itv);
            if((iwake_itv<=WINKS_TIME_CONNECT_PERIOD_MAX) && (iwake_itv>=WINKS_TIME_CONNECT_PERIOD_MIN))
                cwo->twuTimeSlot = iwake_itv;
            else
                Winks_EngErr("unsupport def_way[0x0A]");
            break;
        }

        case 0x0E:      /*��ʱ���ѷ�ʽ�������ӳ�ʱʱ��*/
        {
            Winks_mem_set(wake_to, 0, sizeof(wake_to));
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)wake_to, sizeof(wake_to), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            wake_to[sizeof(wake_to)-1] = 0;
            iwake_to = (unsigned long)atol(wake_to);

            Winks_printf("Overtime of server connecting is %d\".\r\n",iwake_to);
            if((iwake_to<=WINKS_TIME_CONNECT_OVERTIME_MAX)&&(iwake_to>=WINKS_TIME_CONNECT_OVERTIME_MIN))
                cwo->twuTimeLimit = iwake_to;
            else
                Winks_EngErr("unsupport def_way[0x0E]");
            break;
        }
        default:
            winks_EngProtocolUnsupported(0x8005, data_info.type);
            break;
        }

        offset += field_len;
    }

    /*���ⲻƥ��*/
    Winks_ConnectWaysAdjust(cwo);
    Winks_printf("connect info: operation[%d] spTimeGiveUp[%d] twuTimeSlot[%d] twuTimeLimit[%d]\r\n",cwo->operation, cwo->spTimeGiveUp,cwo->twuTimeSlot, cwo->twuTimeLimit);
    return 0;
}

static int winks_EngDsaDo_M8008(int http, unsigned long offset, unsigned long len, unsigned long *ret_code)
{
/*
    �ֿ��´������������Ϣ���ԣ�8008H��

    1)  �����´�������ڼ���    ��01H���������´�������ĺ�����Ϣ��Ŀ�š�
    2)  �ܿ���                      ��02H����������Ƭ�Ͳ���ID��Ϣ�ܿ�����
*/

    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    char buf[8] = {0};
    Winks_EngUpdComInfo_s * pUpdComInfo = winks_EngGetUpdContext();

    Winks_assert(pUpdComInfo != NULL);
    *ret_code = 0;
    end = offset + len;

    while (offset < end)
    {
        int iVal;
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            iVal = atoi(buf);
            if((iVal<WINKS_FRIEND_INFO_BLOCK_MIN) || (iVal>WINKS_FRIEND_INFO_BLOCK_MAX))
            {
                Winks_printf("ERROR(L%4d): ServerData -- ������Ϣ��� iVal=%d\r\n", __LINE__, iVal);
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
                return 0;
            }
            pUpdComInfo->prtRetIndex = (unsigned long)iVal;
            break;
        case 0x02:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            iVal = atoi(buf);
            if((iVal<WINKS_FRIEND_INFO_BLOCK_MIN) || (iVal>WINKS_FRIEND_INFO_BLOCK_MAX))
            {
                Winks_printf("ERROR(L%4d): ServerData -- ������Ϣ���� iVal=%d\r\n", __LINE__, iVal);
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
                return 0;
            }
            pUpdComInfo->patchTotal = (unsigned long)iVal;
            break;
        default:
            winks_EngProtocolUnsupported(0x8008, data_info.type);
            break;
        }

        offset += field_len;
    }

    /*FIXME: NEXT do what */

    return 0;
}

static int winks_EngDsaDo_M8009(int http, unsigned long offset, unsigned long len, unsigned long *ret_code)
{
/*
    �ֻ�Ԥ���ռ���Ϣ����:
    1)  �ֻ�Ԥ����Ѽ�¼��������    ��01H��
    2)  �ֻ�Ԥ����Ӫ�̺�Ƭͷ�ܸ���  ��02H��
*/
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};
    int ret = 0;
    char buf[8] = {0};
    long cnt1 = 0;
    long cnt2 = 0;

    *ret_code = 0;
    end = offset + len;

    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            cnt1 = atol(buf);
            break;
        case 0x02:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            cnt2 = atol(buf);
            break;
        default:
            winks_EngProtocolUnsupported(0x8009, data_info.type);
            break;
        }

        offset += field_len;
    }

    Winks_printf("friends_count[%d], prior_image_count[%d]\r\n",cnt1,cnt2);
    Winks_printf("<STRG>Winks_takeup_tao_space ...\r\n");
    if( /*((ret = Winks_takeup_tao_space()) != 0) ||*/
        ((ret = Winks_SetMaxCountOfBusinessColorCall((unsigned long)cnt2)) != 0) )
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);

    return 0;
}

static int winks_EngDeletePriBusCC(int PriOrBus, char *pCC_ID)
{
    int iRet, Found = 0;
    unsigned char Cnt, min, max;
    char NoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned short CCIdx;
    unsigned short UsrIdx;
    WinksStrgFile_UserInfo_s UsrInfo;

    Winks_assert(pCC_ID);
    
    if(Winks_StrgSearchCC(pCC_ID, &CCIdx) != WINKS_STRG_SEARCH_RESULT_FOUND)
        return WINKS_RETURN_FAILURE;
    
    if(PriOrBus)
    {
        min = WINKS_PRI_BCD_MIN;
        max = WINKS_PRI_BCD_MAX;
    }
    else
    {
        min = WINKS_BUS_BCD_MIN;
        max = WINKS_BUS_BCD_MAX;
    }

    UsrIdx = 0;
    Winks_mem_set(&UsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    Winks_mem_set(NoBcd, 0, WINKS_STRG_NOBCD_LEN);
    for(Cnt=min; Cnt<max; Cnt++)
    {
        NoBcd[WINKS_STRG_NOBCD_LEN-1] = Cnt;
        if( (Winks_StrgSearchUser(NoBcd, &UsrIdx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND) &&
            (UsrInfo.CCIdx == CCIdx) )
        {
            Found = 1;
            break;
        }
    }
    
    if( Found )
    {
        if( Winks_StrgCCUserNum(CCIdx, NULL) > 1)
        {
    		if(UsrInfo.GrtOff)
    		{
    			UsrInfo.CCIdx = 0;/* ������¼��ɾ������Idx */
    			iRet = Winks_StrgSetUserInfo(UsrIdx, &UsrInfo);
    		}
    		else
    		{
                Winks_mem_set(&UsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    			iRet = Winks_StrgSetUserInfo(UsrIdx, &UsrInfo);/* ɾ����¼ */
    		}

    		if(iRet != WINKS_RETURN_SUCCESS)
    		{
    			return WINKS_RETURN_FAILURE;
    		}
        }
        else
            return Winks_StrgDeleteCCImage(CCIdx);
    }

    return WINKS_RETURN_FAILURE;
}

static int winks_EngDsaDo_HFFFA(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
/*  ��Ӫ�̲�����Ϣ����

    1)  ��Ӫ�̲���ID����            ��01H��
    2)  ��Ӫ�̲����ʺ�������    ��02H��
    3)  ������������                ��03H��
    ���������ڱ�ʾƽ̨����Ӫ�̲���Ĳ�����
    ֵ����1���ֽ�
    ֵΪ�� 
    ���ӻ��޸Ĳ���  �� 01H
    ɾ������            �� 02H
*/

    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    int ret = 0;
    Winks_EngCCDownloadContext_s DownCCCxt;
    char grt[WINKS_MAX_LEN_OF_WELCOME_INFO];
    unsigned char op = 0;

    *ret_code = 0;
    ret_data = NULL;
    end = offset + len;

    memset(&DownCCCxt, 0, sizeof(Winks_EngCCDownloadContext_s));
    memset(grt, 0, sizeof(grt));

    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)DownCCCxt.ND_ID, sizeof(DownCCCxt.ND_ID)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            DownCCCxt.ND_ID[data_info.len] = 0;
            break;
        case 0x02:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)grt, sizeof(grt)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            grt[data_info.len] = 0;
            ret = Winks_Utf2UnicodeString(grt, (char *)DownCCCxt.GreetStr, sizeof(DownCCCxt.GreetStr));
            ret <<= 1;
            if(ret<0 || ret>sizeof(DownCCCxt.GreetStr))
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
                return 0;
            }
            DownCCCxt.GreetLen = (unsigned char)ret;
            break;
        case 0x03:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        default:
            winks_EngProtocolUnsupported(0xFFFA, data_info.type);
            break;
        }

        offset += field_len;
    }

    if ((op == 1) && DownCCCxt.ND_ID[0])
    {
        Winks_printf("++++������Ӫ�̲���:%s | %s\r\n", DownCCCxt.ND_ID,  grt);

        Winks_strcpy(DownCCCxt.ND_NO, WINKS_BUS_CC_NO);
        DownCCCxt.ND_cc_type = 3;
        DownCCCxt.ND_img_cnt = 1;
        DownCCCxt.ND_interval = 0;
        DownCCCxt.GreetValid = 1;

        ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_REC_ND|WINKS_DOWNCC_OP_WASHOUT);

        if (ret != 0)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_CALL_FUNC);
        }
    }
    else if ((op == 2) && DownCCCxt.ND_ID[0])
    {
        Winks_printf("---ɾ����Ӫ�̲���:%s | %s\r\n", DownCCCxt.ND_ID);
        if( winks_EngDeletePriBusCC(0, DownCCCxt.ND_ID) != WINKS_RETURN_SUCCESS )
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_CALL_FUNC);
        }
    }
    else
    {
        Winks_printf("ERROR: ������������Ӫ�̻�����Ϣ���� op=%d ID:%s grt:%s\r\n", op, DownCCCxt.ND_ID, grt);
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
    }

    return 0;
}

static int winks_EngDsaDo_HFFFB(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
/*
    Ƭͷ������Ϣ����

    1)  ����ƬͷID����      ��01H��
    2)  ����ʱ������            ��02H��
    3)  Ƭͷ�ʺ�������      ��03H��
    4)  ������������            ��04H��
        ���������ڱ�ʾƽ̨��Ƭͷ�Ĳ�����
        ֵ����1���ֽ�
        ֵΪ�� 
        ���ӻ��޸Ĳ���  �� 01H
        ɾ������            �� 02H
*/

    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    int ret = 0;
    Winks_EngCCDownloadContext_s DownCCCxt;
    char grt[WINKS_MAX_LEN_OF_WELCOME_INFO];
    char buf[8] = {0};

    unsigned char op = 0;
    int play_time = 0;

    *ret_code = 0;
    ret_data = NULL;
    end = offset + len;

    memset(&DownCCCxt, 0, sizeof(Winks_EngCCDownloadContext_s));
    memset(grt, 0, sizeof(grt));

    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)DownCCCxt.ND_ID, sizeof(DownCCCxt.ND_ID), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            DownCCCxt.ND_ID[data_info.len] = 0;
            break;
        case 0x02:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            play_time = atoi(buf);
            break;
        case 0x03:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)grt, WINKS_MAX_LEN_OF_WELCOME_INFO/*sizeof(grt)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            grt[data_info.len] = 0;
            ret = Winks_Utf2UnicodeString(grt, (char *)DownCCCxt.GreetStr, sizeof(DownCCCxt.GreetStr));
            ret <<= 1;
            if(ret<0 || ret>sizeof(DownCCCxt.GreetStr))
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
                return 0;
            }
            DownCCCxt.GreetLen = (unsigned char)ret;
            break;
        case 0x04:
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        default:
            winks_EngProtocolUnsupported(0xFFFB, data_info.type);
            break;
        }

        offset += field_len;
    }

    if ((op == 1) && DownCCCxt.ND_ID[0])
    {
        Winks_printf("++++����Ƭͷ����:%s | %d | %s\r\n", DownCCCxt.ND_ID, play_time, grt);

        Winks_strcpy(DownCCCxt.ND_NO, WINKS_PRI_CC_NO);
        DownCCCxt.ND_cc_type = 2;
        DownCCCxt.ND_img_cnt = 1;
        DownCCCxt.ND_interval = 0;
        DownCCCxt.GreetValid = 1;

        ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_REC_ND|WINKS_DOWNCC_OP_WASHOUT);

        if (ret != 0)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_CALL_FUNC);
        }
    }
    else if ((op == 2) && DownCCCxt.ND_ID[0])
    {
        Winks_printf("----ɾ��Ƭͷ����:%s | %s\r\n", DownCCCxt.ND_ID);
        if( winks_EngDeletePriBusCC(1, DownCCCxt.ND_ID) != WINKS_RETURN_SUCCESS )
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_CALL_FUNC);
        }
    }
    else
    {
        Winks_printf("ERROR: ����������Ƭͷ������Ϣ���� op=%d ID:%s grt:%s\r\n", op, DownCCCxt.ND_ID, grt);
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
    }

    return 0;
}

#if 0
/* �жϵ绰�����Ƿ��ں���¼�� */
static int winks_IsInFriendRecord(char *no)
{
    int Found = 0;
    int friend_cnt, read_base, read_cnt, cnt;
    Winks_FriendItem_s FriendItem_Arr[10];

    read_base = 0;
    friend_cnt = Winks_get_friend_cnt();
    while(read_base < friend_cnt)
    {
        read_cnt = Winks_get_friend_list(FriendItem_Arr, read_base, 10);
        if(read_cnt<=0)
            break;
        
        for(cnt=0; cnt<read_cnt; cnt++)
        {
            if(!strcmp(FriendItem_Arr[cnt].number, no))
            {
                Found = 1;
                break;
            }
        }
        read_base += read_cnt;
    }

    return Found;
}
#endif
static int winks_EngDsaDo_HFFFC(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
/*
    �û����Ѳ���ID��Ϣ����
*/

    int ret = 0;
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    Winks_EngCCDownloadContext_s DownCCCxt;
    char buf[256] = {0};
    unsigned char op = 0;

    //int IsDataInvalid = 0;

    *ret_code = 0;
    ret_data = NULL;
    end = offset + len;

    Winks_mem_set(&DownCCCxt, 0, sizeof(DownCCCxt));

    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:      /* ���Ѻ������� */
            {
            int no_len;
            
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;

            no_len = strlen(buf);

            if(no_len >= sizeof(DownCCCxt.ND_NO))
                Winks_strcpy(DownCCCxt.ND_NO, &buf[no_len-sizeof(DownCCCxt.ND_NO)]);
            else
                Winks_strcpy(DownCCCxt.ND_NO, buf);
            }
            break;
        case 0x02:      /* ������������ */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x03:      /* ���������б�־ */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&diff_flag, sizeof(diff_flag), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            break;
        case 0x04:      /* ����Ϊ�û����õ����в���ID */
            if(winks_EngReadHttpBuf(http, data_info.offset, DownCCCxt.ND_ID, sizeof(DownCCCxt.ND_ID)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            DownCCCxt.ND_ID[data_info.len] = 0;
            break;
        case 0x05:      /* ����Ϊ�û����õı��в���ID */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)ID_in, sizeof(ID_in)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            //ID_in[data_info.len] = 0;
            break;
        case 0x06:      /* �����������־λ���� */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&nw_flag, sizeof(nw_flag), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            break;
        case 0x07:      /* ���в������������� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, 4, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            DownCCCxt.ND_img_cnt = (unsigned char)atol(buf);
            break;
        case 0x08:      /* ���в������������� */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            //buf[data_info.len] = 0;
            //ci_cnt = (unsigned long)atol(buf);
            break;
        case 0x09:      /* ���񲥷ż��ʱ������ */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, 5, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = '\0';
            DownCCCxt.ND_interval = (unsigned char)atol(buf);
            if((DownCCCxt.ND_interval<WINKS_CC_PLAY_INTERVAL_MIN) || (DownCCCxt.ND_interval>WINKS_CC_PLAY_INTERVAL_MAX))
            {
                Winks_printf("ERROR(L%4d): ServerData -- ���񲥷ż�� interval=%d\r\n", __LINE__, DownCCCxt.ND_interval);
                DownCCCxt.ND_interval = WINKS_CC_PLAY_INTERVAL_DFT;
            }
            break;
        default:
            winks_EngProtocolUnsupported(0xFFFC, data_info.type);
            break;
        }

        offset += field_len;
    }

    if (op == 1 && DownCCCxt.ND_NO[0])
    {
        Winks_EngCCDownloadContext_s *pDownCC = NULL;
        
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        DownCCCxt.ND_cc_type = 1;
        DownCCCxt.GreetValid = 0;
        pDownCC = winks_EngGetCCDownContext();
        Winks_assert(pDownCC != NULL);

        switch(Winks_engineContext_p->app)
        {
        case WINKS_ENGINE_APP_PERIOD_UPDATE:
        case WINKS_ENGINE_APP_UPDATE_FRIEND:
        {
            unsigned short FriendIdx;
            Winks_mem_cpy(&pDownCC->ND, &DownCCCxt.ND, sizeof(WinksStrgFile_NeedDown_s));
            if(Winks_StrgUserIsFriend(DownCCCxt.ND_NO, &FriendIdx))
            {
                Winks_printf("++++���Ӻ���:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
                ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_WASHOUT);
            }
            
            break;
        }
    #ifdef WINKS_SUPPORT_3G
        case WINKS_ENGINE_APP_CALL_DOWNLOAD:
    #endif /* WINKS_SUPPORT_3G */
        case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
            Winks_mem_cpy(&pDownCC->ND, &DownCCCxt.ND, sizeof(WinksStrgFile_NeedDown_s));
            Winks_printf("++++���Ӻ���:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
          #if 0/* Debug RECRUIT_DOWNLOAD */
            if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
            {
                Winks_printf("!!!!!!!!!!Debug RECRUIT_DOWNLOAD return fail!!!!!!!!!!!\r\n");
                ret = WINKS_RETURN_FAILURE;
            }
            else
          #endif
            ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_WASHOUT);
            break;
        case WINKS_ENGINE_APP_INIT_CACHE:
            if(strcmp(pDownCC->ND_NO, DownCCCxt.ND_NO) == 0)
            {
                Winks_mem_cpy(&pDownCC->ND, &DownCCCxt.ND, sizeof(WinksStrgFile_NeedDown_s));
                Winks_printf("++++���Ӻ���:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
                ret = winks_EngDealUserBaseInfo(&DownCCCxt, 0);
            }
            break;
        case WINKS_ENGINE_APP_REBULID_CACHE:
        default:
            if(strcmp(pDownCC->ND_NO, DownCCCxt.ND_NO) == 0)
            {
                Winks_mem_cpy(&pDownCC->ND, &DownCCCxt.ND, sizeof(WinksStrgFile_NeedDown_s));
                //ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_REC_ND);
            }
            break;
        }

        if (ret != 0)
        {
            winks_EngAnalyzeErr(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
        }
    }
    else
    {
        Winks_printf("ERROR: winks_EngDsaDo_HFFFC:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
        //*ret_code = WINKS_ENG_ERROR_SVR_RET;
    }

    return 0;
}

static int winks_EngDsaDo_HFFFF(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
/*
    �û���Ƭ��Ϣ����
*/

#ifdef WINKS_SUPPORT_CALLING_CARD_UPDATE
    int ret = 0;
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    unsigned char op = 0;
    unsigned char pub = 0;
    unsigned char state = 0;

    char no[WINKS_FRIEND_NO_LEN];
  #if 0
    char grt_in[WINKS_MAX_LEN_OF_WELCOME_INFO];
    char grt_out[WINKS_MAX_LEN_OF_WELCOME_INFO];
    Winks_BCard_s bcard = {0};
  #else
    char *grt_in = NULL;
    char *grt_out = NULL;
    /* winks_EngGetAnalyzeData�ж�ret_data�������ڴ棬Ȼ�󸳸�pbcard���ڽ����������Զ��ͷ� */
    Winks_BCard_s *pbcard = (Winks_BCard_s *)winks_EngGetAnalyzeData(ret_data, sizeof(Winks_BCard_s));
    if(pbcard == NULL)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
        return 0;
    }
    memset(pbcard, 0, sizeof(Winks_BCard_s));
    grt_in = pbcard->grt_in;
    grt_out = pbcard->grt_out;
  #endif

    *ret_code = 0;
  #if 0
    ret_data = NULL;
  #endif
    end = offset + len;

    Winks_mem_set(no, 0 ,sizeof(no));
  #if 0
    Winks_mem_set(grt_out, 0 ,sizeof(grt_out));
    Winks_mem_set(grt_in, 0 ,sizeof(grt_in));
  #endif
    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:      /* ���Ѻ��� */
            if(winks_EngReadHttpBuf(http, data_info.offset, no, sizeof(no)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            no[data_info.len] = 0;
            break;
        case 0x02:      /* �������� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x03:      /* �Ƿ񹫿������Ƭ */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&pub, sizeof(pub), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x04:      /* �û�״̬���� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&state, sizeof(state), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;

    #if 0   /* ��һ���ݲ�֧�������Ƭ���� */
        case 0x05:      /* �û�״̬�������� */
        case 0x06:      /* �� */
        case 0x07:      /* �� */

        case 0x09:      /* ְ�� */
        case 0x0A:      /* ��˾���� */
        case 0x0B:      /* ��˾�绰 */
        case 0x0C:      /* ���һ���� */
        case 0x0D:      /* ʡ�ݻ������� */
        case 0x0E:      /* ���� */
        case 0x0F:      /* ��ϸ��ַ */
        case 0x10:      /* �ʱ� */
        case 0x11:      /* ���� */
        case 0x12:      /* ���� */
        case 0x13:      /* ��ַ */
        case 0x14:      /* �칫�ֻ� */
            break;
        case 0x15:      /* �û����� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&user_type, sizeof(user_type), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
    #endif

        case 0x08:      /* ����ʱ�ʺ��� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)grt_out, WINKS_MAX_LEN_OF_WELCOME_INFO-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            grt_out[data_info.len] = 0;
            break;
        case 0x16:      /* ����ʱ�ʺ��� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)grt_in, WINKS_MAX_LEN_OF_WELCOME_INFO-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            grt_in[data_info.len] = 0;
            break;
        default:
            winks_EngProtocolUnsupported(0xFFFF, data_info.type);
            break;
        }

        offset += field_len;
    }

    if(no[0])/* �绰���벻��Ϊ�� */
    {
        switch (op)
        {
        case 1:     /* ���ӻ��޸� */
            Winks_printf("<STRG>Winks_add_friend_bcard()\r\n");
          #if 0
            strcpy(bcard.friend_no, no);
            strcpy(bcard.grt_in, grt_in);
            strcpy(bcard.grt_out, grt_out);

            bcard.cc_flag = 1;
            bcard.pub_flag = pub;

            ret = Winks_add_friend_bcard(&bcard);   /* TODO: �ϲ����*/
          #else
            strcpy(pbcard->friend_no, no);
            pbcard->cc_flag = 1;
            pbcard->pub_flag = pub;

            ret = Winks_add_friend_bcard(pbcard);   /* TODO: �ϲ����*/
          #endif
            break;
        case 2:
            ret = Winks_delete_friend_bcard(no);
            break;
        case 3:     /* ���β��� unfinished */
            ret = Winks_set_cc_flag(no, 0);
            break;
        case 4:     /* ������β��� unfinished */
            ret = Winks_set_cc_flag(no, 1);
            break;
        default:
            break;

        }
    }
    else
    {
        Winks_printf("ERROR: ���������ص绰����Ϊ�� op=%d\r\n", op);
    }
    
    if (ret != 0)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
    }
    return 0;
#else

    int ret = 0;
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info;
    
    unsigned char op = 0;
    Winks_EngCCDownloadContext_s DownCCCxt;
    char    grt[WINKS_MAX_LEN_OF_WELCOME_INFO];
    char    buf[256];
    
    Winks_mem_set(&DownCCCxt, 0, sizeof(DownCCCxt));
    Winks_mem_set(grt, 0, sizeof(grt));
    Winks_mem_set(buf, 0, sizeof(buf));
    *ret_code = 0;
    end = offset + len;
    while (offset < end)
    {
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:      /* ���Ѻ��� */
            {
            int no_len;
            
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;

            no_len = strlen(buf);

            if(no_len >= sizeof(DownCCCxt.ND_NO))
                Winks_strcpy(DownCCCxt.ND_NO, &buf[no_len-sizeof(DownCCCxt.ND_NO)]);
            else
                Winks_strcpy(DownCCCxt.ND_NO, buf);
            }
            break;
        case 0x02:      /* �������� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x08:      /* ����ʱ�ʺ��� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)grt, WINKS_MAX_LEN_OF_WELCOME_INFO/*sizeof(grt)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            grt[data_info.len] = 0;
            ret = Winks_Utf2UnicodeString(grt, (char *)DownCCCxt.GreetStr, sizeof(DownCCCxt.GreetStr));
            ret <<= 1;
            if(ret<0 || ret>sizeof(DownCCCxt.GreetStr))
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_PARSE);
                return 0;
            }
            DownCCCxt.GreetLen = (unsigned char)ret;
            break;
        default:
            winks_EngProtocolUnsupported(0xFFFF, data_info.type);
            break;
        }

        offset += field_len;
    }

    if(op && DownCCCxt.ND_NO[0])
    {
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        DownCCCxt.GreetValid = 1;

        switch(Winks_engineContext_p->app)
        {
        case WINKS_ENGINE_APP_PERIOD_UPDATE:
        {
            unsigned short FriendIdx;
            if(Winks_StrgUserIsFriend(DownCCCxt.ND_NO, &FriendIdx))
            {
                ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_WASHOUT);
            }
            
            break;
        }
    #ifdef WINKS_SUPPORT_3G
        case WINKS_ENGINE_APP_CALL_DOWNLOAD:
    #endif /* WINKS_SUPPORT_3G */
        case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
            ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_WASHOUT);
            break;
        case WINKS_ENGINE_APP_INIT_CACHE:
        case WINKS_ENGINE_APP_REBULID_CACHE:
        default:
            ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_REC_ND);
            break;
        }
    }
    else
    {
        Winks_printf("ERROR: ���������ص绰����Ϊ�� op=%d.\r\n", op);
    }
    
    return 0;
#endif
}

static int winks_retData_HFFF8(unsigned long *ret_code, void *data)
{
    int iRet = 0;
    Winks_server_list_s *servers_p = (Winks_server_list_s *)data;

    Winks_assert(data != NULL);
    Winks_assert(servers_p->cnt <= WINKS_IP_PORT_LEN);
    
    iRet = Winks_SetCommServerIpPort(servers_p);
    if(iRet != 0)
    {
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
    }
    winks_EngSetCommSvr(servers_p);
    
    return 0;
}

#if 0
/*Save image to file.*/
static int winks_SaveDownloadImageToFile(Winks_SaveImage_s *CCImgInfo_p, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code)
{
    unsigned long Len = 0,Size = 0;
    FILE *fp = NULL;

    Winks_assert(CCImgInfo_p != NULL);
    
#ifdef WINKS_SUPPORT_MEMORY_CARD
    if( (winks_Get_CCStrgLocation()==E_STORAGE_MCARD) && 
        (Winks_IsMemoryCardExist() != 1) )
    {
        Winks_EngErr("Winks_IsMemoryCardExist\r\n");
        *ret_code = WINKS_MSG_ENG2UI_MCARD_NOT_EXIST;
        return 0;
    }
#endif
    //strcpy(CCImgInfo_p->img_file,WINKS_IMAGE_TMP_NAME);
    if(Winks_get_temp_path(CCImgInfo_p->img_file,WINKS_PATH_MAX,CCImgInfo_p->cc_type)<0)
    {
        Winks_EngErr("Winks_get_temp_path");
        *ret_code = (unsigned long)WINKS_ENG_ERROR_DATA_RSV;
        return 0;
    }
    CCImgInfo_p->img_size = msgLen;

    fp = Winks_fopen(CCImgInfo_p->img_file, "wb");
    if (fp == NULL)
    {
        winks_EngPrintFsErr("Winks_fopen", CCImgInfo_p->img_file, __LINE__);
        *ret_code = (unsigned long)WINKS_MSG_ENG2UI_STORAGE_LIMIT;
        return 0;
    } 

    //if (1)
    {       /*���پֲ�������stack��ѹ��*/
        unsigned char buff[1024];
        Winks_mem_set(buff, 0, sizeof(buff));
        Len = 0;
        while(Len < msgLen)
        {
            if(Len+sizeof(buff)<msgLen)
                Size = sizeof(buff);
            else
                Size = msgLen-Len;
            if(winks_EngReadHttpBuf(http, offset, (char *)buff, sizeof(buff), Size, ret_code) != WINKS_RETURN_SUCCESS)
            {
                Winks_fclose(fp);
                return 0;
            }
            if(Winks_fwrite(buff, Size, 1, fp) != 1)
            {
                winks_EngPrintFsErr("Winks_fwrite", CCImgInfo_p->img_file, __LINE__);
                *ret_code = (unsigned long)WINKS_MSG_ENG2UI_STORAGE_LIMIT;
                Winks_fclose(fp);
                return 0;
            }
            offset += Size;
            Len += Size;
        }

    }
    Winks_fclose(fp);
    return 0;
}
#endif

static int winks_EngUpdateCCInfo__img_type(Winks_EngCCDownloadContext_s *pDownCC)
{
    unsigned char NoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned short retIdx = 0;
    WinksStrgFile_UserInfo_s UsrInfo;
    WinksStrgFile_CCInfo_s   CCInfo;
    
    Winks_assert(pDownCC != NULL);
    
    Winks_PhoneNoStr2Bcd(pDownCC->ND_NO, sizeof(pDownCC->ND_NO), WINKS_STRG_NOBCD_LEN, NoBcd);
    if( pDownCC->img_type && 
        (Winks_StrgSearchUser((char *)NoBcd, &retIdx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND) )
    {
        if( Winks_StrgGetCCInfo(UsrInfo.CCIdx, &CCInfo) == WINKS_RETURN_SUCCESS &&
            CCInfo.ID[0] )
        {
            if(CCInfo.img_type != pDownCC->img_type)
            {
                CCInfo.img_type = pDownCC->img_type;
                return Winks_StrgSetCCInfo(UsrInfo.CCIdx, &CCInfo);
            }
            else
                return WINKS_RETURN_SUCCESS;
        }
    }

    return WINKS_RETURN_FAILURE;
}

/* 3.1.4�ͻ�����ͨѶ�������ĸ��ݲ���ID��ȡ������Ϣ */
static int winks_EngDsaDo_LFFFA(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
    2.  ͨѶ�������ˣ�
        ��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��FFFAH
        2)  �´������ļ���������    ��8001H��
        3)  �´������ļ�������      ��0012H��
    */
    int iRet = 0;
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};
    char    ID[WINKS_STRG_CC_ID_LEN];

    Winks_EngCCDownloadContext_s *pDownCC = NULL;
    unsigned char cc_type,img_idx;

    *ret_code = 0;

    pDownCC = winks_EngGetCCDownContext();
    Winks_assert(pDownCC != NULL);

    switch(msgType)
    {
    case 0x8001:
    {
        /*
        2.  �´������ļ���������                                ��8001H��
        */
    end = offset+msgLen;
    while (offset < end)
        {
            /*
        2.  �´������ļ���������                                ��8001H��
        1)  �´������ļ�ID      ��01H�����������´������ļ���ID��
        2)  �´������ļ�����ID  ��02H����ֵΪ�±��Id�ֶ�ֵ��
        3)  �´������ļ���չ��  ��03H�����������´��ļ����ļ���չ�����±��EXTEND_NAME�ֶΣ���
        4)  �´������ļ�����        ��04H��
        ֵ����1���ֽ�
        ֵΪ�� 
        �´��û������ļ���  01H��
        �´�Ƭͷ�����ļ���  02H��
        �´���Ӫ�̲����ļ���03H��
        5)  ���´�����Ϊ����ʱ��Ŀǰ�´��ļ�Ϊ�������ĵڼ��ţ�05H��
        6)  �´������ļ��Ĵ�С  ��06H�����������´��ļ��Ĵ�С����λ���ֽڣ���ͨѶ������ʹ�ã�
        7)  �´������ļ����·��    ��07H�����������´��ļ���FTP�������ϵ����·������ͨѶ������ʹ�ã�
        8)  �´������ļ��Ƿ��ڱ��������    ��08H�������ڱ���ʱ����FTP������Ч��
        ֵ����1���ֽ�
        ֵΪ�� 
        �ڱ���  01H��
        ���ڱ���02H�� 
        9)  �´������ļ�����FTP��ַ     ��09H��
        10) �´������ļ�����FTP�û���   ��0AH��
        11) �´������ļ�����FTP����     ��0BH��
            */
        char buff[256];
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:  /* �������´������ļ���ID�� */
            if(winks_EngReadHttpBuf(http, data_info.offset, ID, sizeof(ID), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            ID[data_info.len] = '\0';
            if(strcmp(pDownCC->ND_ID, ID))
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
                return 0;
            }
            break;
        case 0x02:  /* �´������ļ�����ID */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buff[data_info.len]= '\0';
            pDownCC->img_type = (unsigned char)atoi(buff);
            break;
        case 0x03:  /* �´������ļ���չ�� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buff[data_info.len]= '\0';
            break;
        case 0x04:  /* �´������ļ����� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, 1/*sizeof(buff)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            cc_type = buff[0];
            
            if(pDownCC->ND_cc_type != cc_type)
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
                return 0;
            }
            break;
        case 0x05:  /* ���´�����Ϊ����ʱ��Ŀǰ�´��ļ�Ϊ�������ĵڼ��ţ� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buff[data_info.len] = '\0';
            img_idx = (unsigned char)atol(buff);
            if((pDownCC->img_idx+1) != img_idx)
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
                return 0;
            }
            break;
#if 0
        case 0x06:  /* �´������ļ��Ĵ�С */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, 4/*sizeof(buff)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            winks_GetDsaLong(&CCImgInfo_p->img_size, buff, 4, WINKS_ENG_DS_4BYTES_LEN);
            break;
        case 0x07:  /* �´������ļ����·�� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x08:  /* �´������ļ��Ƿ��ڱ�������� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x09:  /* �´������ļ�����FTP��ַ */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x0a:  /* �´������ļ�����FTP�û��� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x0b:  /* �´������ļ�����FTP���� */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
#endif
            default:
                winks_EngProtocolUnsupported(msgType, data_info.type);
                break;
            }

            offset += field_len;
        }

        /* ��¼img_type */
        if(winks_EngUpdateCCInfo__img_type(pDownCC)!= WINKS_RETURN_SUCCESS)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
    break;
    }
    
    case 0x0012:
    {
        /*
        18. ������������DataStream����0012H��
        ������ֵΪ�´������ļ������������ɱ�ʾ�������Ϊ4G��
        */
        int IfCached = 0;
        unsigned short UsrIdx;
        int     Len, Size;
        unsigned char buff[1024];
        char    NoBcd[WINKS_STRG_NOBCD_LEN];
        WinksStrgFile_UserInfo_s UsrInfo;
        WinksStrgFile_CCImage_s  CCImage;
        
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        Winks_PhoneNoStr2Bcd(pDownCC->ND_NO, sizeof(pDownCC->ND_NO), WINKS_STRG_NOBCD_LEN, (unsigned char *)NoBcd);
        if(Winks_StrgSearchUser(NoBcd, &UsrIdx, &UsrInfo) != WINKS_RETURN_SUCCESS)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
            
    #ifdef WINKS_SUPPORT_3G
        if(Winks_engineContext_p->app == WINKS_ENGINE_APP_CALL_DOWNLOAD)
            IfCached = 1;
    #endif /* WINKS_SUPPORT_3G */
        Winks_mem_set(&CCImage, 0, sizeof(CCImage));
        Winks_mem_cpy(CCImage.NO, pDownCC->ND_NO, sizeof(pDownCC->ND_NO));
        Winks_mem_cpy(CCImage.ID, pDownCC->ND_ID, sizeof(pDownCC->ND_ID));
        CCImage.img_type = pDownCC->img_type;
        CCImage.img_Idx = pDownCC->img_idx;
        CCImage.img_cnt = pDownCC->ND_img_cnt;
        CCImage.FileSize = msgLen;
        if(Winks_StrgSaveCCImageBegin(UsrInfo.CCIdx, &CCImage, IfCached) != WINKS_RETURN_SUCCESS)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        Winks_mem_set(buff, 0, sizeof(buff));
        Len = 0;
        while(Len < msgLen)
        {
            if(Len+sizeof(buff)<msgLen)
                Size = sizeof(buff);
            else
                Size = msgLen-Len;
            if(winks_EngReadHttpBuf(http, offset, (char *)buff, sizeof(buff), Size, ret_code) != WINKS_RETURN_SUCCESS)
            {
                Winks_StrgSaveCCImageEnd(UsrInfo.CCIdx, &CCImage);
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
                return 0;
            }
            CCImage.pCCData = buff;
            CCImage.DataSize = Size;
            if(Winks_StrgSaveCCImageDo(UsrInfo.CCIdx, &CCImage) != WINKS_RETURN_SUCCESS)
            {
                Winks_StrgSaveCCImageEnd(UsrInfo.CCIdx, &CCImage);
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
                return 0;
            }
            offset += Size;
            Len += Size;
        }
        if(Winks_StrgSaveCCImageEnd(UsrInfo.CCIdx, &CCImage) != WINKS_RETURN_SUCCESS)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        break;
    }
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return iRet;
}

/* 3.1.10�ͻ��˺�ͨ�ŷ�������ʵʱ��ȡ���񽻻����̣�3G����������JAVA��2.5G�ն��ݲ�֧�ִ˹��̣�*/
static int winks_EngDsaDo_LFFF8(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
2.  ͨѶ�������ˣ�
��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
1)  MessageType����                 ֵΪ��FFF8H
2)  �´������ļ���������        ��8001H��
3)  �û����Ѳ���ID��Ϣ����      ��FFFCH��
4)  TimeStamp                   ��0006H��
5)  �û���Ƭ��Ϣ����            ��FFFFH��
6)  �°�ͻ��������URL��Ϣ���ԣ�0016H���������°�������ش����ԣ�
7)  ʵʱ�´����ļ�������        ��8007H��
    */
    int iRet = 0;
    //unsigned char buff[16];

    *ret_code = 0;

    switch(msgType)
    {
    case 0x8001:
        break;
    case 0x0006:
        break;
    case 0x0016:
        break;
    case 0xFFFC:
    case 0xFFFF:
        {
        winks_EngDsaDo_M_array(http, offset, msgType, msgLen, ret_code);
        }
        break;
    case 0x8007:
        break;
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return iRet;
}

#ifdef WINKS_DEL_PV2_0
/* 3.1.9�ͻ�����ͨ�ŷ���������ʵʱ�����û�״̬��Ϣ*/
static int winks_EngDsaDo_LFFF7(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
2.  ͨѶ�������ˣ�
��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
1)  MessageType����                 ֵΪ��FFF7H
2)  �ظ��û�����״̬�޸Ľ������        ��000CH����������Ҫ���أ�
3)  �ظ��û�������񿪹��޸Ľ������    ��000EH����������Ҫ���أ�
4)  �ظ��Ƿ񹫿������Ƭ�޸Ľ������    ��0014H����������Ҫ���أ�
ֵΪ��
    ���³ɹ� ��01H
    ����ʧ�� ��02H
    */
    int iRet = 0;
    unsigned char buff[16];

    *ret_code = 0;

    switch(msgType)
    {
    case 0x000C:
    case 0x000E:/*�ظ��û�������񿪹��޸Ľ������*/
    case 0x0014:
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, 1/*sizeof(buff)*/, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        
        if(buff[0] != 1)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        break;
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return iRet;
}
#endif

/* 3.1.12�ͻ�����ͨ�ŷ����������ϴ�DIY�ļ���Ϣ����*/
static int winks_EngDsaDo_LFFF6(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
2.  ͨѶ�������ˣ�
��ͻ��˷���ʹ�õ�����Ϣ���ԣ�
1)  MessageType����                 ֵΪ��FFF6H
2)  �����ļ��ֿ鴫��ɹ����      (0029H)
    */
    int iRet = 0;

    *ret_code = 0;

    switch(msgType)
    {
    case 0x0029:
        {
        unsigned char buff[16];
        unsigned short subIndex,CorrectIndex;
        Winks_EngDiyComInfo_s *pULInfo = winks_EngGetDiyContext();
        
        Winks_assert(pULInfo != NULL);
        
        if(winks_EngReadHttpBuf(http, offset, (char *)buff, sizeof(buff), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        buff[msgLen] = 0;

        subIndex = (unsigned short)atoi((char *)buff);
        /*�����ϴ��Ŀ�Ż��ļ���*/
        if(pULInfo->subTotalCount>1)
            CorrectIndex = (unsigned short)(pULInfo->subIndex+1);
        else
            CorrectIndex = (unsigned short)(pULInfo->curIndex+1);
        /* ���ز����ϴ��Ŀ��*/
        if(subIndex != CorrectIndex)
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_DATA_RSV);
            return 0;
        }
        }
        break;
    default:
        winks_EngProtocolUnsupported(msgType, 0);
        break;
    }

    return iRet;
}

static const unsigned char Winks_Head[30] = 
{ 
    0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x01,  0x00,  
    0x02,  0x00,  0x00,  0x00,  0x01,  0x01,  0x00,  0x03,  
    0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x04,  
    0x00,  0x00,  0x00,  0x02,  0x00,  0x00 
 };


/*�����Ļ�����(ԭ������Ļ��治����)*/
static int SendBufReInit(Winks_SendFrame_s* pSF, int NewSize)
{
    if(NewSize > WINKS_SEND_FRAME_LEN)
    {
        unsigned char *pSendBuf_New = (unsigned char *)Winks_salAlloc(NewSize);
        if(pSendBuf_New == NULL)
        {
            Winks_printf("ERROR: Winks_salAlloc(%d)\r\n", NewSize);
            Winks_assert(pSendBuf_New != NULL);
            return 0;
        }
        memcpy(pSendBuf_New, pSF->pSendBuf, pSF->Len);
        if(pSF->pSendBuf != pSF->baseBuff)
        {
            Winks_salFree(pSF->pSendBuf);/*���ϴ�����Ļ����ͷ�*/
        }
        pSF->pSendBuf = pSendBuf_New;
        pSF->Len = NewSize;
    }
    else if(NewSize == 0)
    {
        Winks_mem_set(pSF, 0, sizeof(Winks_SendFrame_s));
        pSF->pSendBuf = pSF->baseBuff;
        pSF->Len = WINKS_SEND_FRAME_LEN;
    }
    else
    {
        Winks_EngErr("SendBufReInit()");
    }

    return pSF->Len;
}


/*�ָ�ԭʼ����Ļ�����*/
static int SendBufRestore(Winks_SendFrame_s *pSF)
{
    if(pSF->pSendBuf != pSF->baseBuff)
    {
        /*�����˴󻺴潫���ͷ�*/
        Winks_salFree(pSF->pSendBuf);
    }
    pSF->pSendBuf = pSF->baseBuff;/*�ͷŴ󻺴��ָ������ԭʼ����*/
    memset(pSF->pSendBuf,0,WINKS_SEND_FRAME_LEN);
    pSF->Len = WINKS_SEND_FRAME_LEN;
    pSF->Off = 0;

    return 0;
}


/*�����ͷ*/
static int AddSendFrameHead(Winks_SendFrame_s* pSF, unsigned short msg_type,unsigned short sequence_num)
{
    pSF->Off = 4;
    memcpy(&pSF->pSendBuf[pSF->Off], Winks_Head, 30);
    pSF->pSendBuf[pSF->Off+20] = (unsigned char)(msg_type>>8);
    pSF->pSendBuf[pSF->Off+21] = (unsigned char)(msg_type&0xff);
    pSF->pSendBuf[pSF->Off+28] = (unsigned char)(sequence_num>>8);
    pSF->pSendBuf[pSF->Off+29] = (unsigned char)(sequence_num&0xff);
    pSF->Off += 30;
    return 30;
}

static int AddBaseAttr(Winks_SendFrame_s* pSF, unsigned short AttrId,unsigned char *pStrean, int Size)
{
    if((pSF->Off+Size+WINKS_ENG_DS_LT_LEN) > pSF->Len)
    {
        int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
        while(NewLen <= (int)(pSF->Off+Size+WINKS_ENG_DS_LT_LEN)) 
        {
            NewLen += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*����󻺴�ʧ��*/
        {
            return 0;
        }
    }

    winks_SetDsaShort(&pSF->pSendBuf[pSF->Off], WINKS_ENG_DS_2BYTES_LEN, AttrId);
    pSF->Off += WINKS_ENG_DS_2BYTES_LEN;
    winks_SetDsaLong(&pSF->pSendBuf[pSF->Off], WINKS_ENG_DS_4BYTES_LEN, Size);
    pSF->Off += WINKS_ENG_DS_4BYTES_LEN;
    memcpy(&pSF->pSendBuf[pSF->Off], pStrean, Size);
    pSF->Off += Size;

    return Size+WINKS_ENG_DS_LT_LEN;
}

/* ��pBuf��Size���ȵ�����׷�ӵ�SF���� */
static int Append2SFBuf(Winks_SendFrame_s* pSF, char *pBuf, int Size)
{
    Winks_assert((pBuf != NULL)&&(Size>0));
#if 0
    Winks_printHex( (const unsigned char* )(pBuf), (unsigned short )Size, 
            (const unsigned char* )"Append2SFBuf" );
#endif
    if((pSF->Off+Size+WINKS_ENG_DS_LT_LEN) > pSF->Len)
    {
        int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
        while(NewLen <= (int)(pSF->Off+Size+WINKS_ENG_DS_LT_LEN)) 
        {
            NewLen += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*����󻺴�ʧ��*/
        {
            return WINKS_RETURN_FAILURE;
        }
    }
    
    memcpy(&pSF->pSendBuf[pSF->Off], pBuf, Size);
    pSF->Off += Size;
    
    return WINKS_RETURN_SUCCESS;
}

#if 1 //def WINKS_MTK
/* ��Unicode·���ַ�������ȡ�ļ��� */
static int FindFileName_Unicode(char *pOutBuf, int OutLen, char *pUnicodeInBuf, int InLen)
{
    int retlen = 0;
    char * pStart = NULL;
    const char space[] = WINKS_PATH_SPACE;
    
    while ((InLen) && (!((*pUnicodeInBuf == 0) && (*(pUnicodeInBuf + 1) == 0))))
    {
        if((*(pUnicodeInBuf)==space[0]) && (*(pUnicodeInBuf+1)==0))/* �ҵ��������ļ����Ŀ�ʼλ�ã�����ָ�� */
        {
            pStart = pOutBuf;
            retlen = 0;
        }
        else if(pStart && (retlen<OutLen))
        {
            *pStart++ = *pUnicodeInBuf;
            retlen++;
        }

        pUnicodeInBuf += 2;
        InLen -= 2;
    }

    if(pStart == NULL)/* δ�ҵ�'\\' */
    {
        *pOutBuf = '\0';
        return 0;
    }
    
    *pStart = '\0';
    
    return retlen;
    
}
#endif

//#define READ_PB_FROM_LOG

static int AddL1Attr(Winks_SendFrame_s* pSF,unsigned short AttrId, void *pData)
{
    int RetLen = 0;
    int iRet = 0;

    switch(AttrId)
    {
    case 0x0006:/* TimeStamp */
    {
        char TimeStamp[WINKS_MAX_LEN_OF_TIME_FLAG];
        
        memset(TimeStamp, 0, sizeof(TimeStamp));
        Winks_GetTimeFlag(TimeStamp, WINKS_MAX_LEN_OF_TIME_FLAG, 2);
        if(TimeStamp[0] == 0)/* û�м�¼��Ӧ��ʱ��� */
            strcpy(TimeStamp,"2000-01-01 00:00:00");
        TimeStamp[19] = 0;
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)TimeStamp, strlen(TimeStamp));
        break;
    }
    case 0x0007:/* ������������ */
    {
        unsigned char CallType = 2;/* 1--���� 2--���� */
        RetLen = AddBaseAttr(pSF, AttrId, &CallType, 1);
        break;
    }
    case 0x0008:/*�����绰����*/
    {
        char buff[WINKS_LOCAL_NO_LEN + 1];
        
        Winks_mem_set(buff, 0, sizeof(buff));
        iRet = Winks_GetPhoneNumber(buff, sizeof(buff));
        buff[sizeof(buff) - 1] = 0;
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            //Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            return 0;
        }
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, strlen(buff));
        break;
    }
    case 0x0009:/* �Է��绰���� */
    {
        char *pNo = (char *)pData;
    
        if((pNo == NULL) || pNo[0]=='0')
        {
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            return 0;
        }
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)pNo, strlen(pNo));
        break;
    }
    
    case 0x0011:/*����PB*/
    {
/*�����������ն��ϴ��û���ȫ���绰��������Ϣ."|"���ŷָ"|"��β��
����13910712345|13691234567|13121900330|
*/
        char buff[1024];
#ifdef DBG_UNSUPPORT_PROTOCOL   /* renzhan add for debug */
        Winks_mem_set(buff, 0, sizeof(buff));
        
        ReadLocalPB(buff, sizeof(buff));
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, strlen(buff));
#else
    #if 1//ndef READ_PB_FROM_LOG
        unsigned long phone_cnt = 0, sim_cnt = 0;
        Winks_PhoneBookItem_s PBItem;
        
        unsigned long cnt = 0;
        unsigned long TotalSize = 0;
        unsigned long SF_Off= pSF->Off;
        int  buffOff = 0;

        iRet = Winks_GetPhonebookCount(&phone_cnt, &sim_cnt);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            //Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            return 0;
        }

        if(!phone_cnt && !sim_cnt)/* �绰��Ϊ�� */
        {
            return 0;
        }
    #if 0
        Winks_printf("SFBuf: 0x%08x\r\n",&pSF->pSendBuf[pSF->Off]);
    #endif
        winks_SetDsaShort(&pSF->pSendBuf[pSF->Off], WINKS_ENG_DS_2BYTES_LEN, AttrId);
        pSF->Off += WINKS_ENG_DS_2BYTES_LEN;
        
        pSF->Off += WINKS_ENG_DS_4BYTES_LEN;
        
        TotalSize = 0;
        buffOff = 0;
        Winks_mem_set(buff, 0, sizeof(buff));
        
        cnt = 0;
        while(cnt < phone_cnt + sim_cnt)/* �����ӵ绰���뵽�����ͻ��� */
        {
            int iNo = 0, NumberLen = 0;

            memset(&PBItem, 0, sizeof(PBItem));
            if(cnt < phone_cnt)
                iRet = Winks_ReadPhonebook(1, cnt, &PBItem);
            else
                iRet = Winks_ReadPhonebook(2, cnt-phone_cnt, &PBItem);
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                break;
            }
            
            for(iNo=0; iNo<WINKS_MAX_NUMBER_COUNT && PBItem.number[iNo][0]; iNo++)
            {
                NumberLen = strlen(PBItem.number[iNo]);
                if((NumberLen <= 0) || (NumberLen >= WINKS_FRIEND_NO_LEN))/*��Ч�ĵ绰����,����*/
                    continue;
            #if 0
                Winks_printf("cnt:%02d iNo%d: %s\r\n",cnt,iNo,PBItem.number[iNo]);
            #endif
                NumberLen += 1;
                if(buffOff + NumberLen >= sizeof(buff))/* buff��,�Ƶ������建���� */
                {
                    iRet = Append2SFBuf(pSF, buff, buffOff);
                    if(iRet != WINKS_RETURN_SUCCESS)
                    {
                        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                        return WINKS_RETURN_FAILURE;
                    }
                    TotalSize += buffOff;
                    buffOff = 0;
                    Winks_mem_set(buff, 0, sizeof(buff));
                }
                sprintf(&buff[buffOff], "%s|", PBItem.number[iNo]);
                buffOff += NumberLen;
            }
            cnt++;
        }

        if(buffOff)/* buff��ʣ�µĵ绰����Ҳ��ӵ����ͻ��� */
        {
            iRet = Append2SFBuf(pSF, buff, buffOff);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                return WINKS_RETURN_FAILURE;
            }
            TotalSize += buffOff;
        }

        winks_SetDsaLong(&pSF->pSendBuf[SF_Off+WINKS_ENG_DS_2BYTES_LEN], WINKS_ENG_DS_4BYTES_LEN, TotalSize);
      #if 0
        Winks_printHex( (const unsigned char* )(&pSF->pSendBuf[SF_Off]), (unsigned short )TotalSize+6, 
            (const unsigned char* )"SFBuf" );
      #endif
        RetLen = TotalSize + WINKS_ENG_DS_2BYTES_LEN + WINKS_ENG_DS_4BYTES_LEN;
        
    #else /* READ_PB_FROM_LOG */
        
        FILE *fp = NULL;
        char PBNoBuf[WINKS_FRIEND_NO_LEN+1];
        unsigned long PB_Cnt = 0;
        size_t readlen = 0;
        
        unsigned long cnt = 0;
        unsigned long TotalSize = 0;
        unsigned long SF_Off= pSF->Off;
        int  buffOff = 0;
        
        fp = Winks_fopen( WINKS_STRG_PB_LOG_2, "rb" );
        if (fp == NULL)
        {
            winks_EngPrintFsErr("Winks_fopen", WINKS_STRG_PB_LOG_2, __LINE__);
            return 0;
        }
        
        /* �������� */
        if(Winks_fseek(fp, WINKS_DATETIME_STR_LEN, WINKS_SEEK_SET) != 0)
        {
            Winks_fclose(fp);
            winks_EngPrintFsErr("Winks_fseek", WINKS_STRG_PB_LOG_2, __LINE__);
            return 0;
        }

        /* ���绰������� */
        if(Winks_fread(&PB_Cnt, sizeof(unsigned long), 1, fp) != 1)
        {
            Winks_fclose(fp);
            winks_EngPrintFsErr("Winks_fread", WINKS_STRG_PB_LOG_2, __LINE__);
            return 0;
        }
        if(PB_Cnt == 0)
        {
            Winks_fclose(fp);
            return 0;
        }

        winks_SetDsaShort(&pSF->pSendBuf[pSF->Off], WINKS_ENG_DS_2BYTES_LEN, AttrId);
        pSF->Off += WINKS_ENG_DS_2BYTES_LEN;
        
        pSF->Off += WINKS_ENG_DS_4BYTES_LEN;
        
        TotalSize = 0;
        buffOff = 0;
        Winks_mem_set(buff, 0, sizeof(buff));
        
        cnt = 0;
        while (cnt<PB_Cnt)
        {
            int NumberLen = 0;
           
            if((readlen = Winks_fread(PBNoBuf, 1, WINKS_FRIEND_NO_LEN, fp)) != WINKS_FRIEND_NO_LEN)/* ���绰���� */
                break;
            
            NumberLen = strlen(PBNoBuf);
            if((NumberLen <= 0) || (NumberLen >= WINKS_FRIEND_NO_LEN))/*��Ч�ĵ绰����,����*/
            {
                cnt++;
                continue;
            }
          #if 0
            Winks_printf("cnt:%02d PBNo: %s\r\n",cnt, PBNoBuf);
          #endif
            NumberLen += 1;
            if(buffOff + NumberLen >= sizeof(buff))/* buff��,�Ƶ������建���� */
            {
                iRet = Append2SFBuf(pSF, buff, buffOff);
                if(iRet != WINKS_RETURN_SUCCESS)
                {
                    Winks_fclose(fp);
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                    return WINKS_RETURN_FAILURE;
                }
                TotalSize += buffOff;
                buffOff = 0;
                Winks_mem_set(buff, 0, sizeof(buff));
            }
            sprintf(&buff[buffOff], "%s|", PBNoBuf);
            buffOff += NumberLen;
            cnt++;
        }
        if(buffOff)/* buff��ʣ�µĵ绰����Ҳ��ӵ����ͻ��� */
        {
            iRet = Append2SFBuf(pSF, buff, buffOff);
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                Winks_fclose(fp);
                Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                return WINKS_RETURN_FAILURE;
            }
            TotalSize += buffOff;
        }

        Winks_fclose(fp);
        
        winks_SetDsaLong(&pSF->pSendBuf[SF_Off+WINKS_ENG_DS_2BYTES_LEN], WINKS_ENG_DS_4BYTES_LEN, TotalSize);
    #if 0
        Winks_printHex( (const unsigned char* )(&pSF->pSendBuf[SF_Off]), (unsigned short )TotalSize+6, 
            (const unsigned char* )"SFBuf" );
    #endif
        RetLen = TotalSize + WINKS_ENG_DS_2BYTES_LEN + WINKS_ENG_DS_4BYTES_LEN;

    #endif /* READ_PB_FROM_LOG */
#endif
        break;
    }
    
    case 0x000F:/*IMSI����*/
    {
        
        char buff[WINKS_IMSI_LEN + 1];

        Winks_mem_set(buff, 0, sizeof(buff));
        iRet = Winks_GetImsi(WINKS_CHN_NULL, buff, sizeof(buff));
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            return 0;
        }
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, WINKS_IMSI_LEN);
        break;
    }
#ifdef WINKS_DEL_PV2_0    
    case 0x000D:
    {
        /*
        13. �û�������񿪹����ԣ�000DH��
        ����Ϣ���ڿͻ�����ͨ�ŷ�����������ʱ�ر��û��Լ��Ĳ�����񡣹رպ������κ��˶����޷��������û��Ĳ�����Ϣ��
        ֵ����1���ֽ�
        ֵΪ��  01H : �û������������
        02H : �û��رղ������
        */
        unsigned char buff[10];
        Winks_assert(pData != NULL);

        buff[0] = *((unsigned char *)pData);
        Winks_assert(buff[0] > ((unsigned char)'\0'));
        Winks_assert(buff[0] < ((unsigned char)'\3'));
        RetLen = AddBaseAttr(pSF, AttrId, buff, 1);
        break;
    }
#if 0
    case 0x000B:
    {
        /*
        11. �û�����״̬���ԣ�000BH�� 
        �ͻ�����ͨ�ŷ����������޸����и���״̬���ԡ�
        ֵ����1���ֽ�
        ֵΪ��
        ���� �� 01H��
        �뿪 �� 02H��
        ���� �� 03H��
        æµ �� 04H��
        ���� �� 05H  ��. 
        */
        unsigned char buff[10];
        Winks_assert(pData != NULL);

        buff[0] = *((unsigned char *)pData);
        Winks_assert(buff[0] > ((unsigned char)'\0'));
        Winks_assert(buff[0] < ((unsigned char)'\6'));
        RetLen = AddBaseAttr(pSF, AttrId, buff, 1);
        break;
    }
    
    case 0x0013:
    {
        /*
        19. �Ƿ񹫿������Ƭ���ԣ�0013H��
        ����Ϣ���ڿͻ�����ͨ�ŷ�������������û�������Ϣ�Ƿ񹫿����ԡ����������Ϣ���������޷���ʾ�û����κθ�����Ϣ
        ֵ����1���ֽ�
        ֵΪ��
        ������ ��     01H
        ��������Ϊ��  02H
        */
        unsigned char buff[10];
        Winks_assert(pData != NULL);

        buff[0] = *((unsigned char *)pData);
        Winks_assert(buff[0] > ((unsigned char)'\0'));
        Winks_assert(buff[0] < ((unsigned char)'\3'));
        RetLen = AddBaseAttr(pSF, AttrId, buff, 1);
        break;
    }
    case 0x001f:
    {
        /*
        29. �û�״̬�������ԣ�001FH��
        */
        unsigned char buff[10];
        Winks_assert(pData != NULL);

        buff[0] = *((unsigned char *)pData);
        /*Winks_assert(buff[0] > ((unsigned char)'\0'));
        Winks_assert(buff[0] < ((unsigned char)'\3'));*/
        RetLen = AddBaseAttr(pSF, AttrId, buff, 1);
        break;
    }
#endif
#endif
    case 0x0015:/* �ͻ�������汾��Ϣ���� */
    {
        char *pVer = "V1.0";

        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)pVer, strlen(pVer));
        break;
    }
    case 0x0020:/*ʱ���*/
    {
        char buff[WINKS_MAX_LEN_OF_TIME_FLAG];
        int  len = 0;
    #if 0/*def WINKS_DEBUG*/
        snprintf(buff, sizeof(buff)-1, "%s","2007-11-02 14:40:13|2007-11-22 15:04:45|2007-11-02 14:40:13|2007-11-22 15:04:45|");
        buff[sizeof(buff)-1] = 0;
        len = strlen(buff);
        if(!len || (len>=sizeof(buff)))
        {
            Winks_assert( (len>0) && (len<sizeof(buff)) );
            return 0;
        }
    #else
        {
        int  i;
        char Tmp[WINKS_MAX_LEN_OF_TIME_FLAG];
        
        memset(buff, 0, sizeof(buff));
        strcpy(buff, "");
        len = 0;
        for(i=0; i<4; i++)
        {
            Winks_GetTimeFlag(Tmp, WINKS_MAX_LEN_OF_TIME_FLAG, i);
            if(Tmp[0] == 0)/* û�м�¼��Ӧ��ʱ��� */
                strcpy(Tmp,"2000-01-01 00:00:00");
            Tmp[19] = 0;
            len += strlen(Tmp)+1;
            if(len < WINKS_MAX_LEN_OF_TIME_FLAG)
                strcat(buff, Tmp);
            else
                return 0;
            if(i<3)
                strcat(buff, "|");
        }
        }
    #endif
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, strlen(buff));
        break;
    }
    
    case 0x0021:/*��ǰ����*/
    {
        char buff[20];

        Winks_mem_set(buff, 0, sizeof(buff));
        strncpy(buff, GetProtocolLanguage(), sizeof(buff));
        buff[sizeof(buff) - 1] = 0;
        
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, strlen(buff));
        break;
    }
    
    case 0x0025:/*�ֻ��Ĵ洢�ռ�*/
    {
        char buff[20];
        Winks_FlashSpace_s flashInfo;

        memset(&flashInfo, 0, sizeof(flashInfo));
        iRet = Winks_GetFlashSpace(&flashInfo);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            //Winks_assert(iRet == WINKS_RETURN_SUCCESS);
            return 0;
        }

        if(flashInfo.free < WINKS_FLASHSPACE_RESERVED)
            flashInfo.free = 0;
        else
        {
            flashInfo.free -= WINKS_FLASHSPACE_RESERVED;
        }
        
        Winks_printf("AddL1Attr(0x0025) flashInfo. total=%lu free=%lu\r\n", flashInfo.total, flashInfo.free);
        Winks_mem_set(buff, 0, sizeof(buff));
        snprintf(buff,sizeof(buff)-1, "%ld", flashInfo.free);
        buff[sizeof(buff) - 1] = 0;
        
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, strlen(buff));
        break;
    }
    case 0x002B:/*�ֻ�֧�ֵ���Ƭ��Ϣ����*/
    {
        char buff[4];
        
        winks_SetDsaLong((unsigned char *)buff, sizeof(buff), WINKS_SUPPORTED_PB_INFO);
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, 4);
        break;
    }

    case 0x002D:
    {
        char *pFriendRecord = (char *)pData;

        if(pFriendRecord == NULL)
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)"|", strlen("|"));
        else
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)pFriendRecord, strlen(pFriendRecord));
        break;
    }
    case 0x0012:
    {
        /*
18. ������������DataStream����0012H��
������ֵΪ�´������ļ������������ɱ�ʾ�������Ϊ4G��
        */
        FILE *fp = NULL;
        char buff[WINKS_ENGINE_MAX_LEN_OF_UPLOAD];
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        
        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);
        
        Winks_assert(pULInfo->curIndex<WINKS_MAX_NUM_OF_IMAGE_LIST);
        pfilepath = pULInfo->image_files[pULInfo->curIndex];

        Winks_assert(pfilepath != NULL);

    #if 1 //def WINKS_MTK
        fp = Winks_fopenUni((unsigned short *)pfilepath,"rb");
    #else
        fp = Winks_fopen(pfilepath,"rb");
    #endif
        if (fp == NULL)
        {
            winks_EngPrintFsErr("Winks_fopen", pfilepath, __LINE__);
            return 0;
        }

        if(pULInfo->subTotalCount > 1)/*�ֶ��*/
        {
            unsigned long Off;

            Winks_assert(pULInfo->curSubSize);
            if(pSF->Off+pULInfo->curSubSize+6 > pSF->Len)
            {
                int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLen <= (int)(pSF->Off+pULInfo->curSubSize+6)) NewLen += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
            if(SendBufReInit(pSF,NewLen) != NewLen)/*����󻺴�ʧ��*/
            {
                    Winks_fclose(fp);
                    return 0;
            }
            }
            
            Off = pULInfo->subIndex * WINKS_ENGINE_MAX_LEN_OF_UPLOAD;
            if(Off >= pULInfo->curFileSize)
            {
                Winks_assert(Off < pULInfo->curFileSize);
                Winks_fclose(fp);
                return 0;
            }

        if(Winks_fseek(fp, Off, WINKS_SEEK_SET) != 0)
        {
            Winks_fclose(fp);
            winks_EngPrintFsErr("Winks_fseek", pfilepath, __LINE__);
            return 0;
        }
        if(Winks_fread(buff, pULInfo->curSubSize, 1, fp) != 1)
        {
            Winks_fclose(fp);
            winks_EngPrintFsErr("Winks_fread", pfilepath, __LINE__);
            return 0;
        }
            RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, pULInfo->curSubSize);
        }
        else/*���ֿ�*/
        {
        unsigned long Len,Size;
            
            if(pSF->Off+pULInfo->curFileSize+6 > pSF->Len)
            {
                int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLen <= (int)(pSF->Off+pULInfo->curFileSize+6)) NewLen += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
            if(SendBufReInit(pSF,NewLen) != NewLen)/*����󻺴�ʧ��*/
            {
                    Winks_fclose(fp);
                    return 0;
                }
            }
            
            winks_SetDsaShort(&pSF->pSendBuf[pSF->Off], 4, AttrId);
            pSF->Off += 2;
            winks_SetDsaLong(&pSF->pSendBuf[pSF->Off], 4, pULInfo->curFileSize);
            pSF->Off += 4;

            Len = 0;
            while(Len<pULInfo->curFileSize)
            {
                if(Len + WINKS_ENGINE_MAX_LEN_OF_UPLOAD < pULInfo->curFileSize)
                    Size = WINKS_ENGINE_MAX_LEN_OF_UPLOAD;
                else
                    Size = pULInfo->curFileSize - Len;
                
            if(Winks_fread(buff, Size, 1, fp) != 1)
            {
                Winks_fclose(fp);
                winks_EngPrintFsErr("Winks_fread", pfilepath, __LINE__);
                return 0;
            }
                memcpy(&pSF->pSendBuf[pSF->Off], buff, Size);
                pSF->Off += Size;
                Len += Size;
            }
            RetLen = pULInfo->curFileSize + 6;
        }
        Winks_fclose(fp);

        break;
    }
    default:
        Winks_EngErr("AddL1Attr AttrId not found");
        return -1;
    }
    return RetLen;
}

static int AddL2Attr(Winks_SendFrame_s* pSF,unsigned short AttrId, void *pData)
{
    int RetLen = 0;
    unsigned int SF_Off = pSF->Off;
    int iRet = 0;

    if((pSF->Off+WINKS_ENG_DS_LT_LEN) > pSF->Len)
    {
        int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
        while(NewLen <= (int)(pSF->Off+WINKS_ENG_DS_LT_LEN)) 
        {
            NewLen += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*����󻺴�ʧ��*/
        {
            return 0;
        }
    }
    
    winks_SetDsaShort(&pSF->pSendBuf[SF_Off], 4, AttrId);
    pSF->Off += WINKS_ENG_DS_LT_LEN;
    RetLen += WINKS_ENG_DS_LT_LEN;
    
    switch(AttrId)
    {
    case 0x8000:/*�ͻ���������Ϣ*/
    {
        /*1.    �ͻ���������Ϣ����      ��8000H��
        1)  �ͻ����ͺ���ϢID����        ��01H��
        2)  �ֻ������������            ��02H��
        3)      �ֻ���Ļ������              ��03H��
        4��  �ֻ���Ļ������             ��04H��
        */
        char buff[100];

        winks_SetDsaLong((unsigned char *)buff, sizeof(buff), WINKS_PHONE_ID);
        iRet = AddBaseAttr(pSF, 0x0001, (unsigned char *)buff, 4);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        buff[0] = (char)WINKS_SODRWARE_TYPE;
        iRet = AddBaseAttr(pSF, 0x0002, (unsigned char *)buff, 1);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        snprintf(buff, sizeof(buff)-1, "%d",WINKS_CC_HEIGHT);
        buff[sizeof(buff)-1] = 0;
        iRet = AddBaseAttr(pSF, 0x0003, (unsigned char *)buff, strlen(buff));
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        snprintf(buff, sizeof(buff)-1, "%d",WINKS_CC_WIDTH);
        buff[sizeof(buff)-1] = 0;
        iRet = AddBaseAttr(pSF, 0x0004, (unsigned char *)buff, strlen(buff));
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        break;
    }

    case 0x8002:
    {
        /*
        3.  �ͻ�����������ļ�����                              ��8002H��
        1)  �����ļ�ID      ��01H�� ������Ĳ����ļ���ID��
        2)  �����������        ��02H��
            ֵ����1���ֽ�
            ֵΪ�� 
            �����û������ļ���  01H��
            ����Ƭͷ�����ļ���  02H��
            ������Ӫ�̲����ļ���03H��
        3)  ������          ��03H���������������������·�����ΪDIY����ʱʹ�ã�
        4)  ����ڼ���      ��04H������������ڼ���ͼƬ��
        */
        {
        unsigned char buff[100];
        Winks_CcReq_s *pReqCcData = (Winks_CcReq_s *)pData;
        
        Winks_assert(pReqCcData != NULL);
        strncpy((char *)buff, pReqCcData->ID, WINKS_MAX_LEN_OF_COLOR_CALL_ID);
        buff[WINKS_MAX_LEN_OF_COLOR_CALL_ID] = 0; 
        iRet = AddBaseAttr(pSF, 0x0001, buff, strlen((char *)buff));
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        buff[0] = (char)pReqCcData->cc_type;
        iRet = AddBaseAttr(pSF, 0x0002, buff, 1);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        /*if(pReqCcData->totalCount > 1)*/
        {
            Winks_assert(pReqCcData->img_cnt <=4);

            snprintf((char *)buff, sizeof(buff)-1, "%d", pReqCcData->img_cnt);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x0003, buff, 1);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
            
            snprintf((char *)buff, sizeof(buff)-1, "%d", pReqCcData->img_idx+1);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x0004, buff, 1);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
        }
        break;
    }
    case 0x8008:/*��Ƭ���������Ϣ����*/
    {
        /*9.    �ֿ��´������������Ϣ���ԣ�8008H��
        1)  �����´�������ڼ���    ��01H���������´�������ĺ�����Ϣ��Ŀ�š�
        2)  �ܿ���              ��02H�� ��������Ƭ�Ͳ���ID��Ϣ�ܿ�����
        */
        char buff[100];
        Winks_EngUpdComInfo_s *p = winks_EngGetUpdContext();

        Winks_assert(p != NULL);
        snprintf(buff, sizeof(buff)-1, "%ld",(p->patchCount)+1);
        buff[sizeof(buff)-1] = 0;
        iRet = AddBaseAttr(pSF, 0x0001, (unsigned char *)buff, strlen(buff));
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        snprintf(buff, sizeof(buff)-1, "%ld",p->patchTotal);
        buff[sizeof(buff)-1] = 0;
        iRet = AddBaseAttr(pSF, 0x0002, (unsigned char *)buff, strlen(buff));
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        break;
    }
        
    case 0x8003:
    {
        /*
        4.  �ϴ�DIY�����ļ���������                         ��8003H��
        1)  �û�����                ��01H�� ��DIY��Ե��û��ĺ��롣
        2)  �û��ʺ���          ��02H�� ��DIY��Ե��û����ʺ��
        3)  ��������                ��03H�� ����������ƣ����ڱ�ʶ����������ƣ���
        4)  ����ߴ�                ��04H�� �������ļ����ֽ�������ͨѶ������ʹ�ã�
        5)  ��������ID          ��05H�� ��ֵΪ�±��Id�ֶ�ֵ��
        6)  ������              ��06H�� ���ܹ��ϴ�����DIYͼƬ��
        7)  �ڼ���              ��07H�� �������ϴ�Ϊ�ڼ���DIYͼƬ��
        */
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        char * pfileName = NULL;
        char buff[16];

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);

        if(pULInfo->number[0])
        {
            iRet = AddBaseAttr(pSF, 0x01, (unsigned char *)pULInfo->number, strlen(pULInfo->number));/*�û�����*/
            if(iRet <= 0)
            {
            return iRet;
            }
            RetLen += iRet;
        }
        if(strlen(pULInfo->welcome))
        {
        iRet = AddBaseAttr(pSF, 0x02, (unsigned char *)pULInfo->welcome, strlen(pULInfo->welcome));/*�û��ʺ���*/
        if(iRet <= 0)
        {
        return iRet;
        }
        RetLen += iRet;
        }

        if(pULInfo->image_count)
        {
        #if 1//def WINKS_MTK
            char OBuf[WINKS_PATH_MAX];
        #else
            char tmpbuf[] = WINKS_PATH_SPACE;
        #endif
            pfilepath = pULInfo->image_files[pULInfo->curIndex];
            Winks_assert(pfilepath != NULL);

        #if 1//def WINKS_MTK
            iRet = FindFileName_Unicode(OBuf, WINKS_PATH_MAX, pfilepath, WINKS_PATH_MAX);
            if( iRet<= 0 )
            {
                return iRet;
            }
            pfileName = OBuf;
        #else
            pfileName = strrchr(pfilepath, tmpbuf[0])+1;/*rindex*/
        #endif
            Winks_printf("DIY filename = %s\r\n", pfileName);
            iRet = AddBaseAttr(pSF, 0x03, (unsigned char *)pfileName, strlen(pfileName));/*��������*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #if 1
            snprintf(buff, sizeof(buff)-1, "%ld",pULInfo->curFileSize);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x04, (unsigned char *)buff, strlen(buff));/*����ߴ�*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #endif
            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_type);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x05, (unsigned char *)buff, strlen(buff));/*��������ID*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_count);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x06, (unsigned char *)buff, strlen(buff));/*������*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->curIndex + 1);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x07, (unsigned char *)buff, strlen(buff));/*�ڼ���*/
            if(iRet <= 0)
            {
            return iRet;
            }
            RetLen += iRet;

        }
        break;

    }        
    case 0x8004:
    {
        /*
        5.  �޸��û��ʺ������ԣ�8004H��
        1)  �û�����  ��01H��   ����Ҫ�޸��û��ʺ�����û�����
        2)  �û��ʺ��02H��   ����Ҫ�޸ĵ��û��ʺ���

        ע��
          ����û�����Ϊ�գ���Ϊ�޸��û���Ĭ���ʺ��
        */
        Winks_EngDiyComInfo_s *pULInfo = NULL;

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);

        if(pULInfo->number[0])
        {
            iRet = AddBaseAttr(pSF, 0x01, (unsigned char *)pULInfo->number, strlen(pULInfo->number));/*�û�����*/
            if(iRet <= 0)
            {
            return iRet;
            }
            RetLen += iRet;
        }
        if(pULInfo->welcome[0] == '\0')
            strcpy(pULInfo->welcome, " ");

        iRet = AddBaseAttr(pSF, 0x02, (unsigned char *)pULInfo->welcome, strlen(pULInfo->welcome));/*�û��ʺ���*/
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        break;
    }        
    case 0x8006:
    {
        /*
        7.  �ֿ��ϴ�DIY���ļ����ԣ�8006H��
        1)  �û�����            ��01H�� ��DIY��Ե��û��ĺ��롣
        2)  �û��ʺ���          ��02H�� ��DIY��Ե��û����ʺ��
        3)  ��������            ��03H�� ����������ƣ����ڱ�ʶ����������ƣ���
        4)  �����ܳߴ�          ��04H�� �������ļ����ֽ�����
        5)  ��������ܿ���      ��05H���������ļ��ܵķֿ�����
        6)  �����ϴ���ID        ��06H���������ϴ����ļ���Ŀ�ID��
        7)  �����ϴ����С      ��07H�� �������ϴ��Ĳ������ݿ���ֽ���
        8)  ��������ID          ��08H�� ��ֵΪ�±��Id�ֶ�ֵ��
        9)  ������              ��09H�� ���ܹ��ϴ�����ͼƬ��
        10) �ڼ���              ��0AH�� �������ϴ�Ϊ�ڼ���ͼƬ��
        11) �ͻ�����ID          ��0BH�� ��ֵΪ�ͻ������ɵĲ���ID�����ڷ�������λ����Ĳ����ļ�����Ŀǰjava��ʽΪ�ֻ���+3λ�������
        */
            
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        char * pfileName = NULL;
        char buff[16];

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);

        if(pULInfo->number[0])
        {
            iRet = AddBaseAttr(pSF, 0x01, (unsigned char *)pULInfo->number, strlen(pULInfo->number));/*�û�����*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }

        if(strlen(pULInfo->welcome))
        {
            iRet =  AddBaseAttr(pSF, 0x02, (unsigned char *)pULInfo->welcome, strlen(pULInfo->welcome));/*�û��ʺ���*/
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        }

        if(pULInfo->image_count)
        {
        #if 1//def WINKS_MTK
            char OBuf[WINKS_PATH_MAX];
        #else
            char tmpbuf[] = WINKS_PATH_SPACE;
        #endif
            pfilepath = pULInfo->image_files[pULInfo->curIndex];
            Winks_assert(pfilepath != NULL);

        #if 1//def WINKS_MTK
            iRet = FindFileName_Unicode(OBuf, WINKS_PATH_MAX, pfilepath, WINKS_PATH_MAX);
            if( iRet<= 0 )
            {
                return iRet;
            }
            pfileName = OBuf;
        #else
            pfileName = strrchr(pfilepath,tmpbuf[0])+1;/*rindex*/
        #endif
            Winks_printf("DIY filename = %s\r\n", pfileName);
            iRet =  AddBaseAttr(pSF, 0x03, (unsigned char *)pfileName, strlen(pfileName));/*��������*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #if 1
            snprintf(buff, sizeof(buff)-1, "%ld", pULInfo->curFileSize);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x04, (unsigned char *)buff, strlen(buff));/*����ߴ�*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #endif
            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->subTotalCount);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x05, (unsigned char *)buff, strlen(buff));/*��������ܿ���*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->subIndex + 1);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x06, (unsigned char *)buff, strlen(buff));/*�����ϴ���ID*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%ld", pULInfo->curSubSize);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x07, (unsigned char *)buff, strlen(buff));/*�����ϴ����С*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_type);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x08, (unsigned char *)buff, strlen(buff));/*��������ID*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_count);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x09, (unsigned char *)buff, strlen(buff));/*������*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->curIndex + 1);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x0a, (unsigned char *)buff, strlen(buff));/*�ڼ���*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            /*iRet =  AddBaseAttr(pSF, 0x0b, pULInfo->, strlen());*//*�ͻ�����ID*/
        }
        break;
    }
    default:
        Winks_EngErr("AddL2Attr AttrId not found");
        return -1;
    }
    winks_SetDsaLong(&pSF->pSendBuf[SF_Off+2], 4, (RetLen - WINKS_ENG_DS_LT_LEN)/*ȥ����װͷ�����ֽ�*/);
    return RetLen;
}

static int AddL3Attr(Winks_SendFrame_s* pSF,unsigned short AttrId, void *pData)
{
    int RetLen = 0;
    unsigned short ArrCount = 0;
    unsigned int SF_Off = pSF->Off;
    int iRet = 0;

    if((pSF->Off+WINKS_ENG_DS_LT_LEN) > pSF->Len)
    {
        int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
        while(NewLen <= (int)(pSF->Off+WINKS_ENG_DS_LT_LEN)) 
        {
            NewLen += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*����󻺴�ʧ��*/
        {
            return 0;
        }
    }
    winks_SetDsaShort(&pSF->pSendBuf[pSF->Off], 4, AttrId);
    pSF->Off += WINKS_ENG_DS_LT_LEN;
    RetLen += WINKS_ENG_DS_LT_LEN;

    switch(AttrId)
    {
  #if 0
    case 0xFFF9:
    {
        char *pFriendRecord = (char *)pData;
        char *pPhoneNo = pFriendRecord;

        Winks_assert(pFriendRecord != NULL);
        while(*pFriendRecord)
        {
            unsigned int SF_Off_L2 = pSF->Off;
            int RetLen_L2 = 0;
            char buff[50];

            /*Got send data*/
            pPhoneNo = pFriendRecord;
            pFriendRecord = strchr(pPhoneNo, '|');
            if(pFriendRecord == NULL)break;
            *pFriendRecord++ = '\0';/*��'|'��Ϊ'\0'��ȡ��1�����룬����¼ָ��ָ���¸�����*/
            if(!strlen(pPhoneNo))continue;
            
            if((pSF->Off+WINKS_ENG_DS_LT_LEN) > pSF->Len)
            {
                int NewLenA = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLenA <= (int)(pSF->Off+WINKS_ENG_DS_LT_LEN)) 
                {
                    NewLenA += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
                }
                if(SendBufReInit(pSF, NewLenA) != NewLenA)/*����󻺴�ʧ��*/
                {
                    return 0;
                }
            }
            winks_SetDsaShort((&pSF->pSendBuf[SF_Off_L2]), 4, (unsigned short)(ArrCount+1));
            pSF->Off += WINKS_ENG_DS_LT_LEN;
            RetLen_L2 += WINKS_ENG_DS_LT_LEN;

            /*Set send data*/   
            iRet = AddBaseAttr(pSF, 0x0001, (unsigned char *)pPhoneNo, strlen(pPhoneNo));
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen_L2 += iRet;  

            
            buff[0] = (char)1;
            iRet = AddBaseAttr(pSF, 0x0002, (unsigned char *)buff, 1);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen_L2 += iRet;
            
            winks_SetDsaLong(&pSF->pSendBuf[SF_Off_L2+2], 4, (RetLen_L2-WINKS_ENG_DS_LT_LEN)/*ȥ����װͷ�����ֽ�*/);
            ArrCount++;
            RetLen += RetLen_L2;
        }
        break;
    }
  #endif
  #if 0
    case 0xFFFD:
    {
        int     PB_EvtCnt = 0;
        Winks_PbEvent_s PB_EvtArr[20];
        Winks_PbEvent_s * pCurPBEvt = NULL;
        unsigned long Beg = 0, Cnt = 0, RetCnt = 0;

        /*if(Winks_set_pb_flag() != 0)
        {
            Winks_EngErr("Winks_set_pb_flag");
            return 0;
        }*/
        PB_EvtCnt = Winks_get_pb_event_cnt();
        Beg = 0;
        while(ArrCount < PB_EvtCnt)
        {
            unsigned int SF_Off_L2 = pSF->Off;
            int RetLen_L2 = 0;
            char buff[50];

            /*Got send data*/
            if((ArrCount%20) == 0)/* ÿ��ȡ20���¼���¼*/
            {
                if(Beg+20 < (unsigned long)PB_EvtCnt)
                    Cnt = 20;
                else
                    Cnt = PB_EvtCnt - Beg;
                RetCnt = Winks_get_pb_event_list(PB_EvtArr, Beg, Cnt);
                if(RetCnt != Cnt)
                {
                    //Winks_assert(RetCnt == Cnt);
                    return 0;
                }
                Beg += RetCnt;
            }
            pCurPBEvt = &PB_EvtArr[ArrCount%20];
            
            if((pSF->Off+WINKS_ENG_DS_LT_LEN) > pSF->Len)
            {
                int NewLenA = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLenA <= (int)(pSF->Off+WINKS_ENG_DS_LT_LEN)) 
                {
                    NewLenA += WINKS_SEND_FRAME_LEN;/*�õ����Է��µ�ǰ���ݵĻ����С*/
                }
                if(SendBufReInit(pSF, NewLenA) != NewLenA)/*����󻺴�ʧ��*/
                {
                    return 0;
                }
            }
            winks_SetDsaShort(&pSF->pSendBuf[SF_Off_L2], 4, (unsigned short)(ArrCount+1));
            pSF->Off += WINKS_ENG_DS_LT_LEN;
            RetLen_L2 += WINKS_ENG_DS_LT_LEN;
        #ifdef WINKS_DEBUG
            if(pCurPBEvt->event == 1)
                Winks_printf("++++���Ӻ���:%s\r\n", pCurPBEvt->no);
            else if(pCurPBEvt->event == 2)
                Winks_printf("----ɾ������:%s\r\n", pCurPBEvt->no);
            else
                Winks_printf("ERROR:δ֪�绰���¼� %d\r\n", pCurPBEvt->event);
        #endif
            /*Set send data*/   
            iRet = AddBaseAttr(pSF, 0x0001, (unsigned char *)pCurPBEvt->no, strlen(pCurPBEvt->no));
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen_L2 += iRet;  

            buff[0] = (char)pCurPBEvt->event;
            iRet = AddBaseAttr(pSF, 0x0002, (unsigned char *)buff, 1);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen_L2 += iRet;
            
            winks_SetDsaLong(&pSF->pSendBuf[SF_Off_L2+2], 4, (RetLen_L2-WINKS_ENG_DS_LT_LEN)/*ȥ����װͷ�����ֽ�*/);
            ArrCount++;
            RetLen += RetLen_L2;
        }
        break;
    }
  #endif
    default:
        Winks_EngErr("AddL3Attr AttrId not found");
        return -1;
    }
    winks_SetDsaLong(&pSF->pSendBuf[SF_Off+2], 4, (RetLen-WINKS_ENG_DS_LT_LEN)/*ȥ����װͷ�����ֽ�*/);
    return RetLen;
}

static int AddFrameItem(Winks_SendFrame_s* pSF, unsigned short AttrId, void *pData)
{
    int iRet = 0;

    if(AttrId<0x8000)/*ֻ��1��Э��*/
    {
        iRet = AddL1Attr(pSF, AttrId, pData);
    }
    else if(AttrId>0xc000)/*��3��Э��*/
    {
        iRet = AddL3Attr(pSF, AttrId, pData);
    }
    else/*��2��Э��*/
    {
        iRet = AddL2Attr(pSF, AttrId, pData);
    }

    return iRet;
}

/*���������*/
static int AddSendFrameData(Winks_SendFrame_s* pSF, unsigned short AttrId, void *pData)
{
    int RetLen = 0;
    int iRet = 0;

    switch(AttrId)
    {
    case 0x7FEB:/*��ȡ������Ϣ*/
    {
        /*3.1.17 Feature�ͻ�����ͨ�ŷ������Ļ�ȡָ���������Ϣ�������̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FEB H 
        2)  �����绰��������                            ��0008H��
        3)  �ֿ��´������������Ϣ����                  ��8008H��
        4)  �ֻ�֧�ֵ���Ƭ��Ϣ����                       (002BH) 
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x8008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x002B, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        break;
    }

    case 0x7FEC:/*��ȡ������Ϣ*/
    {
        /*3.1.15 Feature�ͻ�����ͨ�ŷ�������ʼ���������̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FECH 
        2)  �����绰��������        ��0008H��
        3)  �ͻ���������Ϣ          ��8000H��
        4)  ��ʼ���û��绰���뱾���� (0011H)
        5)  �û��ֻ��ռ��С        ��0025H��
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x8000, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x0025, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        break;
    }

    case 0x7FED:/*PUSH(��ʱ)����*/
    {
        int PBEvt_Flag = 0;/* �Ƿ���ӵ绰������¼� */
        /*3.1.19 Feature�ͻ�����ͨѶ�������Ķ�ʱͨѶ���̣���ͬ��ȫ��ϵͳ������Ϣ���û�ѡ�����ѵĸ�����Ϣ��
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FEDH
        2)  �����绰��������            ��0008H��
        3)  ����ʱ���                  ��0020H��
        4)    �ֻ�֧�ֵ���Ƭ��Ϣ����       (002BH)
        5)  �ͻ��˵绰����������        ��FFFDH��
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x0020, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x002B, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

    #if 0   /* winks����д�ļ�(���ֻ��������ӵ�PC�����)����ʱ���²��ϴ��绰����� */
        if(winks_IsUsbState() == 1)
            return -1;
        Winks_printf("<STRG>Winks_SaveCurrentPhoneNumber\r\n");
        PBEvt_Flag = (Winks_SaveCurrentPhoneNumber(WINKS_STRG_PB_LOG_2, WINKS_CHN_NULL, 0) == WINKS_RETURN_SUCCESS);
        if(PBEvt_Flag)
            PBEvt_Flag = (Winks_ComparePhoneNumber(
                    WINKS_STRG_PB_EVENT_PATH, 
                    WINKS_STRG_PB_LOG_1, 
                    WINKS_STRG_PB_LOG_2,
                    WINKS_CHN_NULL, 0) == WINKS_RETURN_SUCCESS);
        if(PBEvt_Flag)
            PBEvt_Flag = (Winks_set_pb_flag() == WINKS_RETURN_SUCCESS);
        
        if(PBEvt_Flag)
        {
            if( Winks_get_pb_event_cnt() > 0)
            {
                iRet = AddFrameItem(pSF, 0xFFFD, pData);
                if(iRet <= 0)
                {
                    return iRet;
                }
                RetLen += iRet;
            }
            iRet = Winks_clear_pb_flag(1);
            if(iRet != 0)
            {
                return iRet;
            }
        }
    #endif
        break;
    }
    
    case 0x7FEF:/*�ϴ�����¼*/
    {
        /*3.1.18 Feature�ͻ�����ͨ�ŷ�������֪ͨ���������޸ĵĺ�����Ϣ�������̣�
        2.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FEFH 
        2)  �����绰��������                            ��0008H��
        3)    �û������б������Ϣ����             (002DH)
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
      #if 1
        if(pData != NULL)
      #endif
        {
            iRet = AddFrameItem(pSF, 0x002D, pData);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
        break;
    }
        
    case 0x7FF0:
    {
        /*3.1.16 Feature�ͻ�����ͨ�ŷ������Ļ�ȡȫ������¼������Ϣ�������̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FF0H 
        2)  �����绰��������                            ��0008H��
        3)    ��ʼ���û��绰���뱾����        (0011H)
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
      {
        unsigned long phone_cnt = 0;
        unsigned long sim_cnt = 0;
        iRet = Winks_GetPhonebookCount(&phone_cnt, &sim_cnt);
        if((iRet == 0) && (phone_cnt||sim_cnt))
        {
    #ifdef READ_PB_FROM_LOG
          Winks_printf("<STRG>Winks_SaveCurrentPhoneNumber\r\n");
          if((iRet = Winks_SaveCurrentPhoneNumber(WINKS_STRG_PB_LOG_2, 0, 0)) != WINKS_RETURN_SUCCESS)/* �ϴ��绰��֮ǰ����һ��log2 */
          {
            pSF->ErrCode = WINKS_MSG_ENG2UI_STORAGE_LIMIT;
            return -1;
          }
        {
    #endif
            iRet = AddFrameItem(pSF, 0x0011, pData);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
    #ifdef READ_PB_FROM_LOG
        }
    #endif
      }
        }
        break;
    }
    case 0x7FF8:
    {
        /*3.1.10�ͻ��˺�ͨ�ŷ�������ʵʱ��ȡ���񽻻����̣�3G����������JAVA��2.5G�ն��ݲ�֧�ִ˹��̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FF8H 
        2)  �����绰��������            ��0008H��
        3)  �Է��绰����                ��0009H��
        4)  ������������                ��0007H��
        5)  TimeStamp                   ��0006H��
        6)  �ͻ�������汾��Ϣ����      ��0015H��
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
    
        iRet = AddFrameItem(pSF, 0x0009, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        iRet = AddFrameItem(pSF, 0x0007, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        iRet = AddFrameItem(pSF, 0x0006, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        iRet = AddFrameItem(pSF, 0x0015, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        break;
    }
    
    case 0x7FF9:
    {
        /*3.1.1 �ͻ��˺����÷�������ͨѶ���̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FF9 H
        2)  IMSI������              ��000FH����JAVA�ն��޴����ԣ�
        3)  �����绰����            ��0008H�������ܻ�ȡ�ն�ר�ã�
        */
        iRet = AddFrameItem(pSF, 0x000F, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        break;
    }
    case 0x7FFD:
    {
        /*3.1.2�ͻ�����ͨѶ��������ѯ�Ƿ�ע����̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FFD H
        2)  IMSI������              ��000FH����JAVA�ն��޴����ԣ�
        3)  �ն������������        ��0021H��
        4)  �����绰����            ��0008H����JAVA�ն�ר�ã�
        */
        iRet = AddFrameItem(pSF, 0x000F, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x0021, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        break;
    }
    case 0x7FFA:
    {
        /* 3.1.4�ͻ�����ͨѶ�������ĸ��ݲ���ID��ȡ������Ϣ*/
        /*
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FFAH 
        2)  �����绰��������        ��0008H��
        3)  �ͻ�����������ļ�����  ��8002H��
        */
        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        iRet = AddFrameItem(pSF, 0x8002, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        break;
    }
#ifdef WINKS_DEL_PV2_0
    case 0x7FF7:
    {
        /* 3.1.9�ͻ�����ͨ�ŷ���������ʵʱ�����û�״̬��Ϣ*/
        /*
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FF7H
        2)  �����绰��������            ��0008H��
        3)      �û�����״̬����            ��000BH����������Ҫ�ϴ���
        4)  �û�������񿪹�����        ��000DH����������Ҫ�ϴ���
        5)  �Ƿ񹫿������Ƭ����        ��0013H����������Ҫ�ϴ���
        6)  �û�״̬��������            ��001FH����������Ҫ�ϴ���
        */
        Winks_RealTimeInfo_s *info_p = (Winks_RealTimeInfo_s *)pData;
        Winks_assert(info_p != NULL);
        Winks_assert(((info_p->opt)&0x0fUL) != 0);

        iRet = AddFrameItem(pSF, 0x0008, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;
        
        if(((info_p->opt)&WINKS_REAL_TIME_INFO_OPT_SHOW_FLAG) != 0UL)
        {
            iRet = AddFrameItem(pSF, 0x000D, &(info_p->showFlag));
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
        
    #if 0   /* unfinished ������Ҫ�ϴ� */
        if(((info_p->opt)&WINKS_REAL_TIME_INFO_OPT_USER_STATE) != 0UL)
        {
            iRet = AddFrameItem(pSF, 0x000B,  &(info_p->userState));
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
        
        if(((info_p->opt)&WINKS_REAL_TIME_INFO_OPT_CARD_SWITCH) != 0UL)
        {
            iRet = AddFrameItem(pSF, 0x0013, &(info_p->cardSwitch));
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
        
        if(((info_p->opt)&WINKS_REAL_TIME_INFO_OPT_USER_STATE_MSG) != 0UL)
        {
            iRet = AddFrameItem(pSF, 0x001f, &(info_p->userStateMsg));
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
    #endif
        break;
    }
#endif
    case 0x7FF6:
    {
        /* 3.1.12�ͻ�����ͨ�ŷ����������ϴ�DIY�ļ���Ϣ����*/
        /*
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FF6H 
        2)  �����绰��������            ��0008H��
        3)  �ϴ�DIY�����ļ��������� ��8003H��
        4)  �ֿ��ϴ�DIY���ļ�����   ��8006H�����ֿ��ϴ������ļ�ʱʹ�ã�
        5)  �ն������������            ��0021H)
        6)  �������ɹ������            ��FFF7H������ͨѶ������ʹ�ô����ԣ�
        7)  ������������DataStream�����ԣ�0012H��
        */
        {
        Winks_DiyInfo_s *pDiyInfo = (Winks_DiyInfo_s *)pData;
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        unsigned long filesize = 0;

        RetLen += AddFrameItem(pSF, 0x0008, pData);

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);
        
        if(pDiyInfo)/* ��һ�ε��ô������ */
        {
            Winks_assert(pDiyInfo->image_count || strlen(pDiyInfo->welcome));/* ������ʺ���������һ��*/
            memset(pULInfo, 0, sizeof(Winks_EngDiyComInfo_s));
            memcpy(pULInfo, pDiyInfo, sizeof(Winks_DiyInfo_s));
            
        }
        else{
            Winks_assert(pULInfo->image_count || strlen(pULInfo->welcome));/* ������ʺ���������һ��*/
        }
        
        Winks_assert(pULInfo->image_count <= WINKS_MAX_NUM_OF_IMAGE_LIST);
        if(pULInfo->image_count)/* �ϴ����� */
        {
            if(!pULInfo->curFileSize)/* ������1��ͼƬʱ���ļ���С��0�����ж�׼���ϴ���һ�� */
            {
                Winks_assert(pULInfo->curIndex < pULInfo->image_count);
                pfilepath = pULInfo->image_files[pULInfo->curIndex];
                iRet = Winks_fgetsize(pfilepath, 0, (unsigned int *)&filesize);

                if((iRet < 0) || (filesize == 0))
                {
                    winks_EngPrintFsErr("Winks_fgetsize", pfilepath, __LINE__);
                    return -1;
                }
                
                pULInfo->curFileSize = filesize;
                if(pULInfo->curFileSize <= WINKS_ENGINE_MAX_LEN_OF_UPLOAD)
                {
                    pULInfo->subTotalCount = 1;
                    pULInfo->subIndex = 0;
                }
                else
                {
                    pULInfo->subTotalCount = (unsigned short)((pULInfo->curFileSize+WINKS_ENGINE_MAX_LEN_OF_UPLOAD-1)/WINKS_ENGINE_MAX_LEN_OF_UPLOAD);
                    pULInfo->subIndex = 0;
                }
            }

            if(pULInfo->subTotalCount>1)
                pULInfo->curSubSize = (pULInfo->subIndex+1 == pULInfo->subTotalCount)?  \
                                    (pULInfo->curFileSize%WINKS_ENGINE_MAX_LEN_OF_UPLOAD):WINKS_ENGINE_MAX_LEN_OF_UPLOAD;
            
            if(pULInfo->curFileSize <= WINKS_ENGINE_MAX_LEN_OF_UPLOAD)
                iRet = AddFrameItem(pSF, 0x8003, pData);
            else
                iRet = AddFrameItem(pSF, 0x8006, pData);
    #if 0
        Winks_printHex( (const unsigned char* )(&pSF->pSendBuf[pSF->Off-iRet]), (unsigned short )iRet, 
            (const unsigned char* )"SFBuf 0x8003|0x8006" );
    #endif
        }
    #if 0
        else
        {
            iRet = AddFrameItem(pSF, 0x8003, pData);
        }
    #endif
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;


        iRet = AddFrameItem(pSF, 0x0021, pData);
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        /*RetLen += AddFrameItem(pSF, 0xFFF7, pData);*/
        if( pULInfo->image_count && (pULInfo->curIndex<WINKS_MAX_NUM_OF_IMAGE_LIST) &&
            pULInfo->image_files[pULInfo->curIndex][0] )
        {
            iRet = AddFrameItem(pSF, 0x0012, pData);
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }
        }
        break;
    }
    case 0x7FF3:
    {
        /*
        3.1.8��7FF3H���ͻ�����ͨ�ŷ���������ʵʱ�޸��û��ʺ��ｻ�����̣�
        1.  �ͻ��ˣ�
        ��ƽ̨����ʹ�õ�����Ϣ���ԣ�
        1)  MessageType����                 ֵΪ��7FF3H 
        2)  �����绰��������        ��0008H��
        3)  �޸��û��ʺ���          ��8004H��
        */
        Winks_DiyInfo_s *pDiyInfo = (Winks_DiyInfo_s *)pData;
        Winks_EngDiyComInfo_s *pULInfo;

        RetLen += AddFrameItem(pSF, 0x0008, pData);

        pULInfo = winks_EngGetDiyContext();
        
        if(pDiyInfo && pULInfo)/* ��һ�ε��ô������ */
        {
            memset(pULInfo, 0, sizeof(Winks_EngDiyComInfo_s));
            memcpy(pULInfo, pDiyInfo, sizeof(Winks_DiyInfo_s));
            
        }
        else
        {
            return -1;
        }
        
        iRet = AddFrameItem(pSF, 0x8004, pData);
        
        if(iRet <= 0)
        {
            return iRet;
        }
        RetLen += iRet;

        break;
    }

    default:
        Winks_EngErr("AddSendFrameData AttrId not found");
        return -1;
    }

    RetLen += (sizeof(Winks_Head) + WINKS_ENG_DS_4BYTES_LEN);
    winks_SetDsaLong(&pSF->pSendBuf[0], WINKS_ENG_DS_4BYTES_LEN, RetLen);/*���ܳ���*/

    return RetLen;
}

static char *GetProtocolLanguage(void)
{
/*
31. �ն������������(0021H)
���    ����������  ������������    ��ע
1.      ZH_TW   ̨��    
2.      ZH_HK   ���    
3.      ZH_SG   �¼���  
4.      ZH_CN   ���ļ���    
5.      EN  Ӣ�������    
6.      JA  ����    
            */
    switch(Winks_GetCurrentLanguage())
    {
    case WINKS_LANG_ENGLISH:
        return "EN";
    case WINKS_LANG_CHINESE:
        return "ZH_CN";
    case WINKS_LANG_CHINESETW:
        return "ZH_TW";
    case WINKS_LANG_CHINESEHK:
        return "ZH_HK";
    case WINKS_LANG_CHINESESG:
        return "ZH_SG";
    case WINKS_LANG_JAPANESE:
        return "JA";
   default:
        return "EN";
    }
}

/*ȡ�������������ı�־. allowedFlag:0����;1������*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag)
{
    int iRet = 0;
    unsigned long roamSwitch = 0;
    unsigned char roamflag = 0;
    
    *allowedFlag = 1;
    iRet = Winks_get_setting_value(E_SETTING_ROAM_SWITCH, &roamSwitch);
    //Winks_assert(iRet == 0);
    if (iRet != 0)
        return WINKS_RETURN_FAILURE;
    
    if(roamSwitch == WINKS_ROAM_SWITCH_OFF) /*��������ʱ�����ӷ�����*/
    {
        Winks_GetRoamingFlag(&roamflag);
        if(roamflag == 1)
            *allowedFlag = 0;
    }

    return WINKS_RETURN_SUCCESS;
}

/*�ж��Ƿ�ע���û��� regFlag:0δע�ᣬ1��ע��*/
int Winks_isUserRegistered(unsigned char *regFlag)
{
    int iRet = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    *regFlag = 0;
    if((Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED)
        ||(Winks_engineContext_p->curState == WINKS_ENGINE_STATE_SHOWB))
    {
        /*�жϿ���Ϣ*/
        char imsi_a[WINKS_IMSI_LEN + 1];
        char imsi_c[WINKS_IMSI_LEN + 1];
        Winks_mem_set(imsi_a, 0, sizeof(imsi_a));
        Winks_mem_set(imsi_c, 0, sizeof(imsi_c));
        Winks_GetImsi(WINKS_CHN_NULL, imsi_c, sizeof(imsi_c));
        Winks_GetRecentlyActivatedImsi(imsi_a,sizeof(imsi_a));
        if((iRet = strncmp(imsi_a, imsi_c, WINKS_IMSI_LEN)) == 0)
        *regFlag = 1;
    }
    return WINKS_RETURN_SUCCESS;
}

static int Winks_SubStateRollback(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    Winks_printf( "Winks_SubStateRollback(%d, %d)\r\n", Winks_engineContext_p->curState,Winks_engineContext_p->subState);

    winks_EngDelTmpFile();
    
    Winks_engineContext_p->app = WINKS_ENGINE_APP_NULL;
    Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
    //Winks_engineContext_p->IsPeriodUpdating = 0;
    Winks_engineContext_p->lastState = 0;

    /*��Դ�ͷ�*/
    winks_EngHttpClose(Winks_engineContext_p->http);
    if(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL)
    {
        Winks_DestroyTimer(Winks_engineContext_p->httpTimer);
        Winks_engineContext_p->httpTimer = WINKS_TIMER_NULL;
    }        
    if(Winks_engineContext_p->communicationContext_p != NULL)
    {
        Winks_salFree(Winks_engineContext_p->communicationContext_p);
        Winks_engineContext_p->communicationContext_p = NULL;
    }
    if(Winks_engineContext_p->server.data != NULL)
    {
        Winks_salFree(Winks_engineContext_p->server.data);
        Winks_engineContext_p->server.data = NULL;
        Winks_engineContext_p->server.len = 0;
    }

    return WINKS_RETURN_SUCCESS;
}

#ifdef WINKS_DEBUG
static void Record_CurStateRollback(int ClearAll)
{
    FILE *pfile = NULL;
    char  TimeFlag[32];
    char  ErrMsgBuf[256];
    Winks_DateTime_s v_dateTime;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_mem_set(TimeFlag, 0, sizeof(TimeFlag));
    Winks_GetTimeFlag(TimeFlag, 32, 3);
    
    Winks_GetSysDateTime( &v_dateTime );
    
    sprintf(ErrMsgBuf, "Winks_CurStateRollback(%d)[%d][%d] at %04ld-%02d-%02d %02d:%02d:%02d before SVR[%s]\r\n", 
        ClearAll, Winks_engineContext_p->curState, Winks_engineContext_p->subState,
        v_dateTime.year, v_dateTime.month, v_dateTime.day, v_dateTime.hour, v_dateTime.min, v_dateTime.sec, TimeFlag);

    Winks_printf(ErrMsgBuf);
    pfile = Winks_fopen( WINKS_APP_PATH"assert.txt", "a" );
	if( pfile != NULL )
    {
        Winks_fwrite( ErrMsgBuf, strlen(ErrMsgBuf), 1, pfile );
    	Winks_fclose( pfile );
    }   
}
#endif

static int Winks_CurStateRollback(int ClearAll)
{
    int iRet  = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

#ifdef WINKS_DEBUG
    Record_CurStateRollback(ClearAll);
#endif
    /*��״̬�ع�*/
    if(ClearAll)
        Winks_SubStateRollback();

    switch(Winks_engineContext_p->curState) 
    {
    case WINKS_ENGINE_STATE_ENABLED:
    case WINKS_ENGINE_STATE_SHOWB:
    {
        /* IMSI�Ÿ�λ */
        char ImsiBuf[WINKS_IMSI_LEN + 1];
        Winks_mem_set(ImsiBuf, '0', sizeof(ImsiBuf));
        ImsiBuf[WINKS_IMSI_LEN] = 0;
        Winks_SetRecentlyActivatedImsi(ImsiBuf,WINKS_IMSI_LEN);
        Winks_engineContext_p->isImsiEnabled = WINKS_IMSI_DISABLED;
        if(Winks_engineContext_p->getImsiTimer != WINKS_TIMER_NULL)
        {
            Winks_DestroyTimer(Winks_engineContext_p->getImsiTimer);
            Winks_engineContext_p->getImsiTimer = WINKS_TIMER_NULL;
        }
        Winks_engineContext_p->getImsiCount = 0;
        
        /*ͣ��ʱ���µ�TIMER*/
        winks_StopPeriodPushTimer(2);

        /*��ǰ״̬��λ*/
        if(Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED)
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_DISABLED;
        else
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_SHOWA;

        Winks_StrgDeinitMapFile();
        break;
    }

    case WINKS_ENGINE_STATE_DISABLED:
    case WINKS_ENGINE_STATE_SHOWA:
    default:
        break;
    }

    /* ��洢ע����Ϣ */
    Winks_printf("<STRG>Winks_delete_storage_files(1) StateRollback\r\n");
    //iRet = Winks_delete_storage_files(1);
    Winks_StrgFriendDelAll();
    iRet = Winks_StrgDeleteDiskFiles(3);
    //Winks_assert( iRet == 0);

    return iRet;
}

#ifdef WINKS_DEBUG
int Winks_UserConnectServer(void)
{
    int iRet = 0;
    int ErrCode = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\ncurstate [%d], substate=[%d]\r\n",
        Winks_engineContext_p->curState,Winks_engineContext_p->subState);
    if((Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED)
        && (Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO))
    {
#if 1
        winks_PackDataAndSend(0x7fed, NULL, &ErrCode);
#else    
        iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_PUSH, NULL, 0);
#endif
    }
    return iRet;
}

int Winks_DownLoadCC(char *pNo)
{
    int iRet = 0;
    int ErrCode;
    Winks_EngCCDownloadContext_s * pDownCC = NULL;

    if((pDownCC = winks_EngGetCCDownContext()) == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_strcpy(pDownCC->ND_NO, pNo);
    winks_PackDataAndSend(0x7ff8, pNo, &ErrCode);

    return iRet;
}

#define FILE_CLUSTER_SIZE   4096
#define FILE_MAX_SIZE_0     (50*1024)           /* 50K */
#define FILE_MAX_SIZE_1     (10*1024*1024) /* 1M */
#define MAX_FILE_NUM        4096

static int winks_TstEngDiskWriteFull(int StrgLoc)
{
    FILE  * fp;
	char	Path[WINKS_PATH_MAX];
	char	RootPath[WINKS_PATH_MAX];

    int     FsErr;
    int     FileNum = 0;
    int     MaxFileNum = MAX_FILE_NUM;
    unsigned long   Off = 0;
    unsigned long   MaxFileSize;

    if(!StrgLoc)
        MaxFileSize = FILE_MAX_SIZE_0;
    else
        MaxFileSize = FILE_MAX_SIZE_1;

    if(!StrgLoc)
        strcpy(RootPath, WINKS_ROOT_PATH);
    else
    {
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        if(Winks_GetMemoryCardDriver(RootPath, sizeof(RootPath)) >=0 )
            strcat(RootPath, WINKS_PATH_SPACE);
        else
            return WINKS_RETURN_FAILURE;
    #else
        return WINKS_RETURN_FAILURE;
    #endif
    }
    
    while(FileNum < MaxFileNum)
    {
        sprintf(Path, "%sfull_%d.dat", RootPath, FileNum);

        if((fp = Winks_fopen(Path, "wb")) == NULL)
            return WINKS_RETURN_FAILURE;;

        Winks_printf("winks_TstEngDiskWriteFull %s\r\n", Path);

        Off = 0;
        while(Off < MaxFileSize)
        {
            Winks_fseek(fp, FILE_CLUSTER_SIZE-sizeof(unsigned long), WINKS_SEEK_CUR);

            FsErr = Winks_fwrite(&Off, sizeof(unsigned long), 1, fp);

            if(FsErr != 1)
			{
				Winks_fclose(fp);
                return WINKS_RETURN_SUCCESS;
			}

            Off += FILE_CLUSTER_SIZE;
        }
        
        Winks_fclose(fp);
        
		FileNum++;
    }
    
    return WINKS_RETURN_FAILURE;
}

static int winks_TstEngDelDiskFullFile(int StrgLoc, int DelAll)
{
	char	Path[WINKS_PATH_MAX];
	char	RootPath[WINKS_PATH_MAX];

    int     FileNum = 0;
    int     MaxFileNum = MAX_FILE_NUM;
    unsigned long   MaxFileSize;

    if(!StrgLoc)
        MaxFileSize = FILE_MAX_SIZE_0;
    else
        MaxFileSize = FILE_MAX_SIZE_1;

    if(!StrgLoc)
        strcpy(RootPath, WINKS_ROOT_PATH);
    else
    {
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        if(Winks_GetMemoryCardDriver(RootPath, sizeof(RootPath)) >=0 )
            strcat(RootPath, WINKS_PATH_SPACE);
        else
            return WINKS_RETURN_FAILURE;
    #else
        return WINKS_RETURN_FAILURE;
    #endif
    }

    while(FileNum < MaxFileNum)
    {
        sprintf(Path, "%sfull_%d.dat", RootPath, FileNum);

        if(Winks_fexist(Path) < 0)
        {
            FileNum++;
            continue;
        }

        Winks_printf("winks_TstEngDelDiskFullFile %s\r\n", Path);

        if(!DelAll)
            return Winks_fdelete(Path);
        else
            Winks_fdelete(Path);
        
		FileNum++;
    }
    
    return WINKS_RETURN_FAILURE;
}

static char LocalNumber[][12] = {
#if 1
    "13810281692",
    "62530410",
    "15101072205",
    "13488758600",
    "59711681",
    "15801447919",
    "13426235603",
    "13552590881",
    "15010382907",
    "15811077465",
    "13910693773",
    "13521437627",
    "13651253015",
    "13911133823",
    "13801313088",
    "15811244673",
    "13581937725",
    "13810945025",
    "13718361901",
    "13801362031",
    "13911790875",
    "13641042068",
//#else
    "13281482214",
    "13281482216",
    "13281482222",
    "13281482226",
    "13281482232",
    
    "13421482128",
    "13421482130",
    "13421482132",
    "13421482140",
    "13421482142",
    "13421482143",
    "13421482144",
    "13421482145",
    "13421482147",
    "13421482149",
    "13421482150",
    
    "13488758600",
    
    "13552321715",
    
    "13601179469",
    
    "13621482184",
    "13621482189",
    "13621482190",
    "13621482191",
    "13621482194",
    "13621482196",
    "13621482199",

    "13621482201",
    "13621482203",
    "13621482205",
    "13621482208",
    
    "13718361901",
    "13718362008",
    
    "13856208901",
    "13856208905",
    "13856208906",
    "13856208908",
    "13856208910",
    "13856208912",
    
    "13921502303",
    "13921502304",

    "15801447919",
    "15810394178",
    
    "15821482156",
    "15821482159",
    "15821482161",
    "15821482163",
    "15821482166",
    "15821482169",
    "15821482181",
    
    "15901537613",
    "15901537614",
#endif
};

#define LOCALNO_NUM    (sizeof(LocalNumber)/sizeof(LocalNumber[0]))

static int winks_TstEngAddFriend()
{
    int iRet = 0;
    static int pos = 0;
    char    FriendNo[WINKS_STRG_PHONE_LEN];
    char    FriendNick[WINKS_STRG_NICK_LEN];

    Winks_strncpy(FriendNo, LocalNumber[pos], sizeof(FriendNo));
    sprintf(FriendNick, "%d", pos);
    iRet = Winks_StrgFriendAdd(FriendNo, FriendNick);
    Winks_printf("winks_TstEngAddFriend %s %s ret %d\r\n", FriendNo, FriendNick, iRet);
    pos = (pos+1)%LOCALNO_NUM;
    return iRet;
}

static int winks_TstEngDelFriend()
{
    int iRet = 0;
    int Idx;
    char    FriendNo[WINKS_STRG_PHONE_LEN];
    int Total = Winks_StrgFriendCnt();

    Idx = Total>>1;

    Idx = Winks_StrgFriendGet(Idx, FriendNo, sizeof(FriendNo), NULL, 0);

    if(Idx>0)
    {
        iRet = Winks_StrgFriendDel(FriendNo);
        Winks_printf("winks_TstEngDelFriend %s ret %d\r\n", FriendNo, iRet);
        return iRet;
    }
    else
        return WINKS_RETURN_FAILURE;
}

static int winks_TstEngDelAllFriends()
{
    int iRet = Winks_StrgFriendDelAll();
    Winks_printf("winks_TstEngDelAllFriends ret %d\r\n", iRet);
    return iRet;
}

static void winks_TstEngPrintFriends()
{
    int cnt = 0;
    int Idx = 0;
    char    FriendNo[WINKS_STRG_PHONE_LEN];
    char    FriendNick[WINKS_STRG_NICK_LEN];
    int FriendCnt = Winks_StrgFriendCnt();
    
    Winks_printf("winks_TstEngPrintFriends: FriendCnt=%d\r\n", FriendCnt);
    Winks_printf("CNT : FRIEND NUMBER    FRIEND NICK\r\n");
    while(cnt < FriendCnt)
    {
        Idx = Winks_StrgFriendGet( Idx, 
                                   FriendNo, sizeof(FriendNo),
                                   FriendNick, sizeof(FriendNick) );
        if(Idx <= 0)
            break;
        Winks_printf("%4d: %-16s %s\r\n", cnt+1, FriendNo, FriendNick);
        cnt++;
    }
}

static void winks_TstEngPrintUsers(unsigned long Flag)
{
    int cnt = 0;
    unsigned short Idx = 0;
    WinksStrgFile_UserInfo_s UserInfo;
    unsigned char EmptyNoBcd[6];
    
    Winks_printf("winks_TstEngPrintUsers: UserCnt=%d \r\n", Winks_StrgGetUserCnt());
    Winks_printf("CNT : LOC  PHONE NUMBER     [CC    ] [GREET       ] IsFriend \r\n");
    Winks_mem_set(EmptyNoBcd, 0, sizeof(EmptyNoBcd));
    while(Idx < WINKS_MAX_USER)
    {
        if(Winks_StrgGetUserInfo(Idx, &UserInfo) != WINKS_RETURN_SUCCESS)
            break;
        if(memcmp(UserInfo.NoBcd, EmptyNoBcd, sizeof(UserInfo.NoBcd)))
        {
            char    PhoneNo[20];
            WinksStrgFile_GreetInfo_s GreetInfo;
            int     CC_loc = (UserInfo.CCIdx&0x8000)? 1 : 0;
            int     Grt_loc = (UserInfo.GrtOff&0x8000)? 1 : 0;

            UserInfo.CCIdx &= 0x7fff;
            UserInfo.GrtOff &= 0x7fff;

            if(Winks_PhoneNoBcd2Str((unsigned char *)UserInfo.NoBcd, sizeof(UserInfo.NoBcd), sizeof(PhoneNo), PhoneNo) == WINKS_RETURN_SUCCESS)
            {
                unsigned short FriendIdx;
                if(Winks_StrgUserIsFriend(PhoneNo, &FriendIdx))
                    Winks_printf("%4d: %-4d %-16s [%d,%-4d] [%d,0x%08x] %d\r\n", cnt+1, Idx, PhoneNo, CC_loc, UserInfo.CCIdx, Grt_loc, UserInfo.GrtOff, FriendIdx);
                else
                    Winks_printf("%4d: %-4d %-16s [%d,%-4d] [%d,0x%08x] N\r\n", cnt+1, Idx, PhoneNo, CC_loc, UserInfo.CCIdx, Grt_loc, UserInfo.GrtOff);
            }
            else
                Winks_printf("%4d: %-4d _____%X%X%X%X%X%X_____ [%d,%-4d] [%d,0x%08x] N/A\r\n", cnt+1, Idx, UserInfo.NoBcd[0], UserInfo.NoBcd[1], UserInfo.NoBcd[2],
                        UserInfo.NoBcd[3], UserInfo.NoBcd[4], UserInfo.NoBcd[5], CC_loc, UserInfo.CCIdx, Grt_loc, UserInfo.GrtOff);

            if(Flag & 0x01)
            {
                if( Winks_StrgGetGreet(UserInfo.GrtOff, &GreetInfo) == WINKS_RETURN_SUCCESS &&
                     memcmp(GreetInfo.NoBcd, UserInfo.NoBcd, sizeof(UserInfo.NoBcd)) == 0 &&
                     GreetInfo.GreetLen > 0 )
                {
                    Winks_printHex((unsigned char*)GreetInfo.GreetStr, (unsigned short)WK_STRLEN((char *)GreetInfo.GreetStr), (unsigned char*)"     Greet:");
                }
            }
            cnt++;
        }
        Idx++;
    }
}

static int winks_TstEngAddUL()
{
    int iRet = 0;
    static int pos = 0;

    iRet = Winks_StrgULAdd(LocalNumber[pos]);
    Winks_printf("winks_TstEngAddUL %s ret %d\r\n", LocalNumber[pos], iRet);
    pos = (pos+1)%LOCALNO_NUM;
    return iRet;
}

static int winks_TstEngDelUL()
{
    int iRet = 0;
    int Idx;
    char    PhoneNo[16];
    int Total = Winks_StrgULCnt();

    Idx = Winks_StrgULGet(PhoneNo);

    if(Idx>0)
    {
        iRet = Winks_StrgULDelete(PhoneNo);
        Winks_printf("winks_TstEngDelUL %s ret %d\r\n", PhoneNo, iRet);
        return iRet;
    }
    else
        return WINKS_RETURN_FAILURE;
}

static int winks_TstEngDelAllUL()
{
    int iRet = Winks_StrgULDeleteAll();
    Winks_printf("winks_TstEngDelAllFriends ret %d\r\n", iRet);
    return iRet;
}

static int winks_TstEngAddND()
{
    int iRet = 0;
    static int pos = 0;
    WinksStrgFile_NeedDown_s NeedDown;

    Winks_mem_set(&NeedDown, 0, sizeof(NeedDown));
    Winks_strncpy(NeedDown.NO, LocalNumber[pos], sizeof(NeedDown.NO));
    sprintf(NeedDown.ID, "%d", pos);
    iRet = Winks_StrgNDAdd(&NeedDown);
    Winks_printf("winks_TstEngAddND %s %s ret %d\r\n", NeedDown.NO, NeedDown.ID, iRet);
    pos = (pos+1)%LOCALNO_NUM;
    return iRet;
}

static int winks_TstEngDelND()
{
    int iRet = 0;
    int Idx;
    WinksStrgFile_NeedDown_s NeedDown;
    int Total = Winks_StrgNDCnt();

    Idx = Total>>1;

    Idx = Winks_StrgNDGet(Idx, &NeedDown);

    if(Idx>0)
    {
        iRet = Winks_StrgNDDelete(NeedDown.NO);
        Winks_printf("winks_TstEngDelND %s ret %d\r\n", NeedDown.NO, iRet);
        return iRet;
    }
    else
        return WINKS_RETURN_FAILURE;
}

static int winks_TstEngDelAllND()
{
    int iRet = Winks_StrgNDDeleteAll();
    Winks_printf("winks_TstEngDelAllND ret %d\r\n", iRet);
    return iRet;
}

static int winks_TstCreateFile(char *Path, int Size)
{
    int   iRet;
    FILE *fp;
    
    fp = Winks_fopen(Path, "wb");
    if(fp == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_fseek(fp, Size-4, WINKS_SEEK_SET) >= 0)
        iRet = Winks_fwrite(&Size, 4, 1, fp);

    Winks_fclose(fp);

    return iRet;
}

static int winks_TstEngTrimCCCache()
{
#if 1
    unsigned short Cnt, CallEvtMax = 550;//2048;
    WinksStrgFile_CallInfo_s CallInfo;

    for(Cnt=1; Cnt<=CallEvtMax; Cnt++)
    {
        sprintf(CallInfo.NO, "1310000%04d", Cnt);
        sprintf(CallInfo.ID, "IMG_%04d",Cnt);
        CallInfo.CallClock = (Cnt*(Cnt+1))%1024;
        CallInfo.CallTime = (Cnt+1)*33%7;
        CallInfo.CallType = 1;
        
        Winks_StrgCallEvtAdd(&CallInfo);
    }
    
    return Winks_StrgTrimCCCache();
#else
    int Cnt, Step;
    int CCLeftCnt;
    unsigned short CCIdxLeft[1124];
    char Path[256],Path1[256];

    Winks_GetAppPath(1, Path, sizeof(Path));
    for(Cnt=1; Cnt<=1024; Cnt++)
    {
        
        sprintf(Path1, "%sstrg\\img\\Img%04d.jpg", Path, Cnt);
        winks_TstCreateFile(Path1, 1024);
    }

    CCLeftCnt = 0;
    Step = 1;
    for(Cnt=1; Cnt<1124; Cnt+=Step)
    {
        CCIdxLeft[CCLeftCnt++] = (unsigned short)(0x8000|Cnt);

        Step = (CCLeftCnt%3)+1;
    }
    return winks_StrgDeleteUnUsedCC(CCIdxLeft, CCLeftCnt);
#endif
}

static int winks_TstEngFreeDiskSpace()
{
    unsigned short Cnt, CCMax = 987;
    WinksStrgFile_CCInfo_s  CCInfo;

    for(Cnt=1; Cnt<=CCMax; Cnt++)
    {
        sprintf(CCInfo.ID, "IMG_%04d",Cnt);
        CCInfo.img_type = 2;
        CCInfo.img_cnt = 1;
        CCInfo.interval = 0;
        CCInfo.UseCnt = Cnt%10;
        CCInfo.LatestUse =(Cnt*(Cnt+1))%1024;
        CCInfo.UseCnt++;
        Winks_StrgSetCCInfo(Cnt, &CCInfo);
    }
    
    return Winks_StrgFreeDiskSpace(50*1024);
}
#endif

extern int winks_GetOsApiBitOpt(unsigned long bit, unsigned long *val);
#ifdef WINKS_DUAL_SIM
extern int Winks_api_GetImsi_2(void);
#endif
extern int Winks_api_GetImsi(void);
static int Winks_DoTestMode(int keyWord)
{
    int iRet = 0;
    int iRet1 = 0;
    int iRet2 = 0;
    unsigned long val = 0;
    char buff[100];
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    
    Winks_mem_set(buff, 0, sizeof(buff));
    switch(keyWord)
    {
    case 600: /* Read sim 2 imsi */
    {
    #ifdef WINKS_DUAL_SIM
        Winks_api_GetImsi_2();
        Winks_printf("[TEST].Read Sim2 IMSI code. \r\n");
    #else
        Winks_printf("[TEST].Read Sim2 IMSI code. not supported\r\n");
    #endif
        break;
    }
    case 601: /* Read sim 1 imsi */
    {
        Winks_api_GetImsi();
        Winks_printf("[TEST].Read Sim IMSI code. \r\n");
        break;
    }
#ifdef WINKS_ENGINE_RUN_BY_STEP
    case 608: /*engine run by step. 806*/
    {
        winks_EngineRunStepbyStep();
        Winks_printf("[TEST].Engine run by step. \r\n");
        break;
    }
#endif
    case 0:
    {
        Winks_CurStateRollback(1);
        Winks_XdeleteDir(WINKS_STRG_IMAGE_PATH);
        Winks_XdeleteDir(WINKS_STRG_BCARD_PATH);
        Winks_XdeleteDir(WINKS_STRG_PATH);
        Winks_XdeleteDir(WINKS_SKIN_FILE_PATH);
        Winks_XdeleteDir(WINKS_APP_PATH);
        Winks_printf("[TEST] Erasure all winks files. \r\n");
        break;
    }
    case 1:
    {
        iRet = Winks_GetImsi(WINKS_CHN_NULL, buff, sizeof(buff));
        Winks_printf("[TEST] Winks_GetImsi[%s]. return[%d]\r\n",buff, iRet);
        break;
    }
    case 2:
    {
        iRet = Winks_GetBgLightTime(&val);
        Winks_printf("[TEST] Winks_GetBgLightTime[%d]. return[%d]\r\n",val, iRet);
        break;
    }
    case 3:
    {
        unsigned long cnt_frd = 0;
        unsigned long cnt_cnp = 0;
        iRet1 = Winks_GetMaxCountOfFriendList(&cnt_frd);
        iRet2 = Winks_GetMaxCountOfBusinessColorCall(&cnt_cnp);
        Winks_printf("[TEST] Winks_GetMaxCountOfFriendList[%d:%d]. return[%d:%d]\r\n",cnt_frd,cnt_cnp, iRet1,iRet2);
        break;
    }
    case 4:
    {
        /*ȡUSB״̬��1:ϵͳ����USB, 0:ϵͳδ��USB.*/
        iRet = Winks_GetUsbState(&val);
        Winks_printf("[TEST] Winks_GetUsbState[%d]. return[%d]\r\n",val, iRet);
        break;
    }
    case 5:
    {
        iRet = Winks_get_setting_value(E_SETTING_CONNECT_POINT, &val);
        iRet1 = Winks_GetDataAccountName(val, buff, sizeof(buff)-2);
        Winks_printf("[TEST] Get data account[%d] info. return[%d][%d]\r\n",val, iRet, iRet2);
        Winks_printHex((unsigned char*)buff, (unsigned short)WK_STRLEN(buff), (unsigned char*)"[Name:]\r\n");
            
        Winks_mem_set(buff, 0, sizeof(buff));
        iRet2 = Winks_GetApnName(val, buff, sizeof(buff)-2);
        Winks_printHex((unsigned char*)buff, (unsigned short)WK_STRLEN(buff), (unsigned char*)"[Name:]\r\n");
        break;
    }
    case 6:/*Get system memory usage*/
    {
        Winks_printf("[TEST] Winks_xsalPrintMemInfo.\r\n");
#ifdef WINKS_DEBUG
        Winks_xsalPrintMemInfo();
#endif
        break;
    }
    case 7:/*Save USB state to memery*/
    {
#ifdef WINKS_DEBUG
        unsigned long *p = Winks_engineContext_p->usbFlagHistory;
        Winks_GetUsbState(&val);
        if(p[0]<32)
        {
            p[1] &= ~(1<< p[0]);
            if(val)
                p[1] |= (1<< p[0]);            
        }
        (*p)++;
#endif
        break;
    }
    case 8:/*Print USB state*/
    {
#ifdef WINKS_DEBUG
        unsigned long *p = Winks_engineContext_p->usbFlagHistory;
        Winks_printf("[TEST] USB history.times[%d] flags[0x%08lx]\r\n",p[0],p[1]);
#endif
        break;
    }
    case 9:/*   9 Print memory trace.*/
    {
#ifdef WINKS_DEBUG
        Winks_printf2file(3);
        Winks_printf("[TEST] Run Winks_printf2file. \r\n");
#endif
        break;
    }
#ifdef WINKS_DEBUG
    case 10:/*  10 Check the server connecting timer's infomations.*/
    {
        winks_TimerBody_s timerInfo;
        Winks_ConnectWayOperation_s CWOp;
        char TimeFlag[32];
        Winks_mem_set(&timerInfo, 0, sizeof(timerInfo));
        if(Winks_engineContext_p->PeriodPushTimer != WINKS_TIMER_NULL);
        {
            Winks_GetTimerInfo(Winks_engineContext_p->PeriodPushTimer, &timerInfo);
        }
        Winks_mem_set(TimeFlag, 0, sizeof(TimeFlag));
        Winks_GetTimeFlag(TimeFlag, 32, 3);
        Winks_mem_set(&CWOp, 0, sizeof(CWOp));
        Winks_GetSupportedConnectWays( &CWOp);
        Winks_printf("[TEST] Connect TIMER infomation. timerId[%d],expirod[%d],state[%d] CWOp[0x%08x, %d, %d] success at %s \r\n",
            Winks_engineContext_p->PeriodPushTimer, timerInfo.timeOutPeriod, timerInfo.state, 
            CWOp.operation, CWOp.twuTimeSlot, CWOp.twuTimeLimit, TimeFlag);
        #ifdef WINKS_DEBUG
        {
            Winks_DateTime_s v_dateTime;
            Winks_DateTime_s *pDT_Old = &Winks_engineContext_p->v_dateTime;
            Winks_DateTime_s *pDT_Close = &Winks_engineContext_p->v_dateTime1;
            Winks_GetSysDateTime( &v_dateTime );
            
            Winks_printf( "[Last]%04d-%02d-%02d %02d:%02d:%02d [Now]%04d-%02d-%02d %02d:%02d:%02d \r\n",
                pDT_Old->year, pDT_Old->month, pDT_Old->day, pDT_Old->hour, pDT_Old->min, pDT_Old->sec,
                v_dateTime.year, v_dateTime.month, v_dateTime.day, v_dateTime.hour, v_dateTime.min, v_dateTime.sec);
            Winks_printf( "[Close]%04d-%02d-%02d %02d:%02d:%02d cause(%d)\r\n",
                pDT_Close->year, pDT_Close->month, pDT_Close->day, pDT_Close->hour, pDT_Close->min, pDT_Close->sec,
                Winks_engineContext_p->PTCloseCause);
        }
        #endif
        break;
    }
    case 11:/* Test Winks_GetFolderSize */
    {
        unsigned long FoldSize = 0;
        iRet = Winks_GetFolderSize("C:\\Winks", 1, &FoldSize, 0);
        //iRet = Winks_GetFolderSize(WINKS_STRG_PATH, 1, &FoldSize, WINKS_INIT_NEED_FLASH_SIZE);
        //iRet = Winks_GetFolderSize("C:\\xxx", 1, &FoldSize, 0);
        Winks_printf("[TEST] Winks_GetFolderSize(%lu). return[%d]\r\n", FoldSize, iRet);
        break;
    }
    case 12:/* write full handset */
    {
        winks_TstEngDiskWriteFull(0);
        Winks_printf("winks_TstEngDiskWriteFull(0) finished\r\n");
        break;
    }
    case 13:/* delete one write handset full file */
    {
        winks_TstEngDelDiskFullFile(0, 0);
        Winks_printf("winks_TstEngDelDiskFullFile(0, 1) finished\r\n");
        break;
    }
    
    case 14:/* delete all write handset full file */
    {
        winks_TstEngDelDiskFullFile(0, 1);
        Winks_printf("winks_TstEngDelDiskFullFile(0, 1) finished\r\n");
        break;
    }
    case 15:/* write full card */
    {
        winks_TstEngDiskWriteFull(1);
        Winks_printf("winks_TstEngDiskWriteFull(1) finished\r\n");
        break;
    }
    case 16:/* delete one write card full file */
    {
        winks_TstEngDelDiskFullFile(1, 0);
        Winks_printf("winks_TstEngDelDiskFullFile(1, 0) finished\r\n");
        break;
    }
    case 17:/* delete all write card full file */
    {
        winks_TstEngDelDiskFullFile(1, 1);
        Winks_printf("winks_TstEngDelDiskFullFile(1, 1) finished\r\n");
        break;
    }
    
    case 21:/* add friend */
    {
        winks_TstEngAddFriend();
        break;
    }
    case 22:/* delete friend */
    {
        winks_TstEngDelFriend();
        break;
    }
    case 23:/* delete all friend */
    {
        winks_TstEngDelAllFriends();
        break;
    }
    case 24:/* print friend */
    {
        winks_TstEngPrintFriends();
        break;
    }
        
    case 31:/* print user all*/
    {
        winks_TstEngPrintUsers(0xffffffff);
        break;
    }
    case 32:/* print user */
    {
        winks_TstEngPrintUsers(0);
        break;
    }
        
    case 41:/* add UL */
    {
        winks_TstEngAddUL();
        break;
    }
    case 42:/* delete UL */
    {
        winks_TstEngDelUL();
        break;
    }
    case 43:/* delete all UL */
    {
        winks_TstEngDelAllUL();
        break;
    }

    case 51:/* add ND */
    {
        winks_TstEngAddND();
        break;
    }
    case 52:/* delete ND */
    {
        winks_TstEngDelND();
        break;
    }
    case 53:/* delete all ND */
    {
        winks_TstEngDelAllND();
        break;
    }

    case 54:/* trim cc cache */
    {
        iRet = winks_TstEngTrimCCCache();
        Winks_printf("[TEST] winks_TstEngTrimCCCache. return[%d]\r\n", iRet);
        break;
    }
    case 55:/* free cc cache */
    {
        iRet = winks_TstEngFreeDiskSpace();
        Winks_printf("[TEST] winks_TstEngFreeDiskSpace. return[%d]\r\n", iRet);
        break;
    }
#endif
        
    case 100:
    {
        iRet = Winks_UserRegist();
        Winks_printf("[TEST] Winks_UserRegist. return[%d]\r\n", iRet);
        break;
    }
    case 101: /* Sim��1���Ͷ��� */
    {
        char Number[32];
        char buff[256];

        strcpy(Number, "8613681087247");
        sprintf(buff, "Winks_SendSms 101");
    #ifdef WINKS_DUAL_SIM
        Winks_SendSms(Number, strlen(Number)+1, buff, strlen(buff)+1, 1);
    #else
        Winks_SendSms(Number, strlen(Number)+1, buff, strlen(buff)+1);
    #endif
        break;
    }
    case 102: /* Sim��2���Ͷ��� */
    {
    #ifdef WINKS_DUAL_SIM
        char Number[32];
        char buff[256];

        strcpy(Number, "8613681087247");
        sprintf(buff, "Winks_SendSms 102");
        Winks_SendSms(Number, strlen(Number)+1, buff, strlen(buff)+1, 2);
    #else
        Winks_printf("Winks_SendSms sim2 not supported\r\n");
    #endif
        break;
    }
    case 103:
        Winks_DownLoadCC("13426476145");
        break;
    case 110: /* ������IMSI��ȡ״̬ */
    {
#ifdef WINKS_DUAL_SIM
    Winks_printf("state[%d][%d]\r\n", Winks_engineContext_p->curState, Winks_engineContext_p->subState);
    Winks_printf("SimStatus:0x%04x Req:%d OtherRetry:%d\r\n", Winks_engineContext_p->SimStatus,
        Winks_engineContext_p->CurInquireSimNo, Winks_engineContext_p->OtherSimRetryCnt);
  #ifdef WINKS_DEBUG
    Winks_printf("IMSI[0]:%s\r\n", Winks_engineContext_p->ImsiCode[0]);
    Winks_printf("IMSI[1]:%s\r\n", Winks_engineContext_p->ImsiCode[1]);
  #endif
#endif
        break;
    }
#if 1//debug usb state
    case 201: /* copy SYS_DRV"DbgUsb.txt" to WINKS_APP_PATH"DbgUsb.txt" */
    {
        FILE* fp = NULL;
        FILE* fp_w = NULL;
        int  Len = 0;
        char Buf[1024];

        Winks_SetOsalTestBitFlag(6, 0);/* �ֶ��ر�usb״̬���Լ�� */
        
        Winks_printf("Begin debug while in usb state, copy DbgUsb.txt\r\n");
        fp = Winks_fopen( "D:\\DbgUsb.txt", "rb" );
        if(fp == NULL)
            break;
        fp_w = Winks_fopen( WINKS_APP_PATH"DbgUsb.txt", "wb" );
        if(fp_w == NULL)
        {
            Winks_fclose( fp );
            break;
        }
        while((Len = Winks_fread(Buf, 1, sizeof(Buf), fp)) > 0)
            Winks_fwrite(Buf, Len, 1, fp_w);
        Winks_fclose( fp_w );
        Winks_fclose( fp );
        
        break;
    }
    case 200: /* create SYS_DRV"DbgUsb.txt" */
    {
        FILE* fp = NULL;

        Winks_SetOsalTestBitFlag(6, 1);/* �ֶ�����usb״̬���Լ�� */
        
        fp = Winks_fopen( "D:\\DbgUsb.txt", "w" );
        if(fp != NULL) Winks_fclose( fp );
        
        break;
    }
    case 202: /* USB���� */
    {
        unsigned char usb_action;
        unsigned long event_id = 0;
        unsigned long msg_id = 0;
        unsigned long msg_size = 0;
        void *msg_data = NULL;
    
        event_id = WINKS_EVENT_USB_DETECT;
        msg_id = WINKS_MSG_USB_DETECT;
        msg_data = &usb_action;
        msg_size = 1;

        Winks_printf("WINKS_EVENT_USB_DETECT: WINKS_USB_PLUG_IN\r\n");
        usb_action = WINKS_USB_PLUG_IN;
        Winks_ProEventWaitingQueue(event_id, msg_id, msg_data, msg_size);
        break;
    }
    case 203: /* USB�γ� */
    {
        unsigned char usb_action;
        unsigned long event_id = 0;
        unsigned long msg_id = 0;
        unsigned long msg_size = 0;
        void *msg_data = NULL;
    
        event_id = WINKS_EVENT_USB_DETECT;
        msg_id = WINKS_MSG_USB_DETECT;
        msg_data = &usb_action;
        msg_size = 1;

        Winks_printf("WINKS_EVENT_USB_DETECT: WINKS_USB_PLUG_OUT\r\n");
        usb_action = WINKS_USB_PLUG_OUT;
        Winks_ProEventWaitingQueue(event_id, msg_id, msg_data, msg_size);
        break;
    }
#endif//debug usb state
#if 1
    /* Ĭ�Ϸ�usb״̬*/
    case 207:/* usb״̬�ı� */
    {
        iRet = Winks_GetOsalTestBitFlag(7,&val);
        iRet = Winks_SetOsalTestBitFlag(7,!val);
        Winks_printf("[TEST] Change usb status flag. val[%d]\r\n",!val);
        break;
    }
    /* Ĭ���д洢�� */
    case 208:/* memory card״̬�ı� */
    {
        iRet = Winks_GetOsalTestBitFlag(8,&val);
        iRet = Winks_SetOsalTestBitFlag(8,!val);
        Winks_printf("[TEST] Change memory card status flag. val[%d]\r\n",!val);
        break;
    }
  #ifdef WINKS_DEBUG
    case 209:/* new memory card */
    {
        iRet = Winks_SetOsalTestBitFlag(8,0);
        Winks_StrgSetCardStatus(1);
        Winks_printf("[TEST] Insert new memory card.\r\n");
        break;
    }
    case 210:/* old memory card */
    {
        iRet = Winks_SetOsalTestBitFlag(8,0);
        Winks_StrgSetCardStatus(2);
        Winks_printf("[TEST] Insert old memory card.\r\n");
        break;
    }
    case 211:/* start bulid cccache timer */
    {
        winks_StartBuildCacheTimer();
        Winks_printf("[TEST] start bulid cccache timer.\r\n");
        break;
    }
  #endif
#endif
    case 800:/*Change background display flag*/
    {
        iRet = Winks_GetBackgroundFlag(&val);
        iRet = Winks_SetBackgroundFlag(!val);
        Winks_printf("[TEST] Change background display flag. val[%d]\r\n",!val);
        break;
    }
    case 801:/*Call accept flag*/
    {
        iRet = winks_GetOsApiBitOpt(2,&val);
        Winks_printf("[TEST] Check call accept flag. val[%d]\r\n",val);
        break;
    }
    case 802:/*Debug Write file flag*/
    {
        iRet = Winks_GetOsalTestBitFlag(2,&val);
        iRet = Winks_SetOsalTestBitFlag(2,!val);
        Winks_printf("[TEST] Change debug write file flag. val[%d]\r\n",!val);
        break;
    }
    case 803:/*Key event print flag*/
    {
        iRet = Winks_GetOsalTestBitFlag(3,&val);
        iRet = Winks_SetOsalTestBitFlag(3,!val);
        Winks_printf("[TEST] Change key event print flag. val[%d]\r\n",!val);
        break;
    }
    case 804:/*URL controller flag*/
    {
        iRet = Winks_GetOsalTestBitFlag(4,&val);
        iRet = Winks_SetOsalTestBitFlag(4,!val);
        Winks_printf("[TEST] Change URL controller flag. val[%d]\r\n",!val);
        break;
    }
    case 805:/* 805 Change every key accept call flag.*/
    {
#ifdef WINKS_DEBUG
        iRet = Winks_GetOsalTestBitFlag(5,&val);
        iRet = Winks_SetOsalTestBitFlag(5,!val);
        Winks_printf("[TEST] Change every key accept call flag. val[%d]\r\n",!val);
#endif
        break;
    }
#ifdef WINKS_ENGINE_RUN_BY_STEP
#ifdef WINKS_DEBUG
    case 806:/* 806 Change engine run mode.*/
    {
        iRet = Winks_GetOsalTestBitFlag(31,&val);
        iRet = Winks_SetOsalTestBitFlag(31,!val);
        Winks_printf("[TEST] Change engine run mode. val[%d]\r\n",!val);
        break;
    }
#endif
#endif
    case 888:/*Get software version tag*/
    {
        Winks_printf("[TEST] VERSION[%s].\r\n", WINKS_VERSION_TAG);
        break;
    }
    case 990:/* 990 Clean all winks file.*/
    {
        Winks_printf("[TEST] Winks_CurStateRollback_1.\r\n");
        Winks_CurStateRollback(1);
        
        break;
    }
    case 997:/* 997 Restart test.*/
    {
        //MMI_ASSERT(0);
        break;
    }
    case 998:/*Assert test.*/
    {
        //Winks_assert(0==1);
#ifdef WINKS_DEBUG
        Winks_assertfail(0, "[TEST] Winks_fail. 1234567890987654321qwertyuiop;lkjhgfdasa", "wk_enfnc.c", 8084);
#endif
        break;
    }
    case 999:
    {
        Winks_fail("[TEST] Winks_fail. \r\n");
        break;
    }
    default:
        break;
    }
    return WINKS_RETURN_SUCCESS;
}

static int Winks_TestModeManager(char * keyStr)
{
    int i = 0;
    const Winks_EngDSItem_s keyTable[]= {
        {"000#", 0}     /*  0 Reset.*/
        ,{"001#", 1}    /*   1 Get Imsi*/
        ,{"002#", 2}    /*   2 Get Background lighting time.*/
        ,{"003#", 3}    /*   3 Get max friends numbers and max other numbers.*/
        ,{"004#", 4}    /*   4 Get USB state.*/
        ,{"005#", 5}    /*   5 Get data account name.*/
        ,{"006#", 6}    /*   6 Get system memory usage.*/
        ,{"007#", 7}    /*   7 Save USB state to memery.*/
        ,{"008#", 8}    /*   8 Print USB state.*/
        ,{"009#", 9}    /*   9 Print memory trace.*/
        ,{"010#", 10}   /*  10 Check the server connecting timer's infomations.*/
        ,{"011#", 11}   /*  11 Test Winks_GetFolderSize.*/
        ,{"012#", 12}   /*  12 write full handset */
        ,{"013#", 13}   /*  13 delete one write handset full file */
        ,{"014#", 14}   /*  14 delete all write handset full file */
        ,{"015#", 15}   /*  15 write full card */
        ,{"016#", 16}   /*  16 delete one write card full file */
        ,{"017#", 17}   /*  17 delete all write card full file */
        ,{"021#", 21}   /*  21 add friend */
        ,{"022#", 22}   /*  22 delete friend */
        ,{"023#", 23}   /*  23 delete all friend */
        ,{"024#", 24}   /*  24 print friend */
        ,{"031#", 31}   /*  31 print user all */
        ,{"032#", 32}   /*  32 print user */
        ,{"041#", 41}   /*  41 add UL */
        ,{"042#", 42}   /*  42 delete UL */
        ,{"043#", 43}   /*  43 delete all UL */
        ,{"051#", 51}   /*  51 add ND */
        ,{"052#", 52}   /*  52 delete ND */
        ,{"053#", 53}   /*  53 delete all ND */
        ,{"054#", 54}   /*  54 trim cc cache */
        ,{"055#", 55}   /*  55 free cc cache */
        ,{"100#", 100}  /*100 user regist. */  
        ,{"101#", 101}  /* 101 Sim��1���Ͷ��� */  
        ,{"102#", 102}  /* 102 Sim��2���Ͷ��� */  
        ,{"103#", 103}  /* 103 ����Э�� */  
        ,{"110#", 110}  /* 110 ������IMSI��ȡ״̬. */  
#if 1//debug usb state
        ,{"200#", 200}  /* 110 ������IMSI��ȡ״̬. */  
        ,{"201#", 201}  /* 110 ������IMSI��ȡ״̬. */  
        ,{"202#", 202}  /* 202 USB���� */  
        ,{"203#", 203}  /* 203 USB�γ� */  
#endif//debug usb state
        ,{"207#", 207}  /* 207 usb״̬�ı� */
        ,{"208#", 208}  /* 208 memory card״̬�ı� */
#ifdef WINKS_DEBUG
        ,{"209#", 209}  /* 209 new memory card */
        ,{"210#", 210}  /* 210 old memory card */
        ,{"211#", 211}  /* 211 start bulid cccache timer */
#endif
        ,{"800#", 800}  /* 800 Change background display flag.*/
        ,{"801#", 801}  /* 801 Check call accept flag.*/
        ,{"802#", 802}  /* 802 Change debug write file flag.*/
        ,{"803#", 803}  /* 803 Change if key event print flag.*/
        ,{"804#", 804}  /* 804 Change URL flag.*/
        ,{"805#", 805}  /* 805 Change every key accept call flag.*/
#ifdef WINKS_ENGINE_RUN_BY_STEP
        ,{"806#", 806}  /* 806 Change engine run mode.*/
#endif
        ,{"888#", 888}  /* 888 Get software version tag.*/
        ,{"990#", 990}  /* 990 Clean all winks file.*/
        ,{"997#", 997}  /* 997 Restart test.*/
        ,{"998#", 998}  /*998 Assert test.*/
        ,{"999#", 999}  /* 999 Winks_fail.*/
        ,{"600#", 600}  /* 600 Read sim 2 imsi.*/
        ,{"601#", 601}  /* 601 Read sim 1 imsi.*/
        ,{"602#", 602}  /* 602 .*/
        ,{"603#", 603}  /* 603 .*/
        ,{"604#", 604}  /* 604 .*/
#ifdef WINKS_ENGINE_RUN_BY_STEP
        ,{"608#", 608}  /* 608 .*/
#endif
        };
    
    for(i = 0; i<WINKS_ARRAY_SIZE(keyTable); ++i)
    {
        if(strncmp(keyStr, keyTable[i].keyString, WINKS_TEST_KEY_WORDS_LEN) == 0)
        {   
            Winks_DoTestMode(keyTable[i].key);
            break;
        }
    }
    return WINKS_RETURN_SUCCESS;
}
