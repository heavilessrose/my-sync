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

//#define TEST_HTTP   "http://www.msn.com.cn"    /* 测试Http协议 */

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
    PROTOCOL_PATH_7FEB_1,   /*更新好友录*/
    PROTOCOL_PATH_7FED_1,   /*更新运营商彩像*/
    PROTOCOL_PATH_7FED_2,
    PROTOCOL_PATH_7FED_3,   /*更新好友彩像*/
    PROTOCOL_PATH_5006,     /*未注册*/
    PROTOCOL_PATH_2002,     /*未注册*/
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
        http = 10;/* 通讯服务器内部错误 */
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
    {WINKS_MSG_ENG2UI_NONE           , "WINKS_MSG_ENG2UI_NONE         ",  " XX 无关消息 "},
        
/* 成功及进度消息 */
    
/* 1 注册及初始化过程 */
    {WINKS_MSG_ENG2UI_SETUP_SVR_OK   , "WINKS_MSG_ENG2UI_SETUP_SVR_OK ",  " XX 连接配置服务器成功 "},
    {WINKS_MSG_ENG2UI_SMS_REG        , "WINKS_MSG_ENG2UI_SMS_REG      ",  " 未注册，需要发送注册短信     [Winks_UserSendRegSms] "},
    {WINKS_MSG_ENG2UI_SMS_REG_OK     , "WINKS_MSG_ENG2UI_SMS_REG_OK   ",  " 短信注册OK"},
    {WINKS_MSG_ENG2UI_INIT           , "WINKS_MSG_ENG2UI_INIT         ",  " XX 正在初始化... "},
    {WINKS_MSG_ENG2UI_INIT_OK        , "WINKS_MSG_ENG2UI_INIT_OK      ",  " 初始化完毕"},

/* 2 更新好友录 */
    {WINKS_MSG_ENG2UI_FL_OK          , "WINKS_MSG_ENG2UI_FL_OK        ",  " XX 好友列表下载完毕 "},
    {WINKS_MSG_ENG2UI_FL_NOTIFY      , "WINKS_MSG_ENG2UI_FL_NOTIFY    ",  " 通知用户有好友更新"},
    {WINKS_MSG_ENG2UI_FL_SEL_FRIEND  , "WINKS_MSG_ENG2UI_FL_SEL_FRIEND",  " 好友列表下载完毕，选择好友录 [Winks_UserSetFriendList] "},
    {WINKS_MSG_ENG2UI_FRIEND_SET     , "WINKS_MSG_ENG2UI_FRIEND_SET   ",  " XX 上传好友录成功 "},
    {WINKS_MSG_ENG2UI_FRIEND_UPD     , "WINKS_MSG_ENG2UI_FRIEND_UPD   ",  " XX 上传好友录及下载好友数据 "},
    {WINKS_MSG_ENG2UI_DL_FRIENDCC    , "WINKS_MSG_ENG2UI_DL_FRIENDCC  ",  " 正在下载好友彩像... "},
    {WINKS_MSG_ENG2UI_FRIEND_OK      , "WINKS_MSG_ENG2UI_FRIEND_OK    ",  " 好友录更新完毕"},
#if 0//def WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_FL_SEL_MCARD   , "WINKS_MSG_ENG2UI_FL_SEL_MCARD ",  " 好友列表下载完毕，选择存储卡 [Winks_UserSelMCard] "},
#endif /* WINKS_SUPPORT_MEMORY_CARD */

/* 3 同步本地配置到服务器 */
    {WINKS_MSG_ENG2UI_SETUP_OK       , "WINKS_MSG_ENG2UI_SETUP_OK     ",  " 同步本地配置成功"},

/* 4 上传DIY */
    {WINKS_MSG_ENG2UI_DIY            , "WINKS_MSG_ENG2UI_DIY          ",  " XX 正在上传DIY... "},
    {WINKS_MSG_ENG2UI_DIY_OK         , "WINKS_MSG_ENG2UI_DIY_OK       ",  " 上传DIY成功"},

/* 5 重新激活，查询服务器注册状态 */
    {WINKS_MSG_ENG2UI_REREG_OK       , "WINKS_MSG_ENG2UI_REREG_OK     ",  " 重新激活成功"},

/* 6 电话下载 */
    {WINKS_MSG_ENG2UI_CALLCC_GRT_OK  , "WINKS_MSG_ENG2UI_CALLCC_GRT_OK",  " XX通话获取用户问候语成功"},
    {WINKS_MSG_ENG2UI_CALLCC_OK      , "WINKS_MSG_ENG2UI_CALLCC_OK    ",  " 通话获取用户彩像成功"},
    {WINKS_MSG_ENG2UI_CALLCC_OTHER   , "WINKS_MSG_ENG2UI_CALLCC_OTHER ",  " XX通话获取片头运营商彩像"},

/* 7 后台服务 */
    {WINKS_MSG_ENG2UI_PUSH           , "WINKS_MSG_ENG2UI_PUSH         ",  " XX 后台正在定时更新服务... "},
    {WINKS_MSG_ENG2UI_PUSH_FRIEND    , "WINKS_MSG_ENG2UI_PUSH_FRIEND  ",  " XX 后台正在更新好友... "},
    {WINKS_MSG_ENG2UI_PUSH_OK        , "WINKS_MSG_ENG2UI_PUSH_OK      ",  " XX 后台定时更新服务完毕 "},
    {WINKS_MSG_ENG2UI_PUSH_CHECK_FL  , "WINKS_MSG_ENG2UI_PUSH_CHECK_FL",  " XX 后台定时更新服务完毕,但需要检查好友列表 "},

    {WINKS_MSG_ENG2UI_INITCACHE_FL   , "WINKS_MSG_ENG2UI_INITCACHE_FL ",  " XX 后台正在下载好友列表以便初始化Cache... "},
    {WINKS_MSG_ENG2UI_INITCACHE      , "WINKS_MSG_ENG2UI_INITCACHE    ",  " XX 后台正在初始化Cache... "},
    {WINKS_MSG_ENG2UI_INITCACHE_OK   , "WINKS_MSG_ENG2UI_INITCACHE_OK ",  " XX 后台初始化Cache完成 "},

    {WINKS_MSG_ENG2UI_RECACHE_TRIM   , "WINKS_MSG_ENG2UI_RECACHE_TRIM ",  " XX 后台正在下载基本信息以重建Cache... "},
    {WINKS_MSG_ENG2UI_RECACHE        , "WINKS_MSG_ENG2UI_RECACHE      ",  " XX 后台正在重建Cache... "},
    {WINKS_MSG_ENG2UI_RECACHE_OK     , "WINKS_MSG_ENG2UI_RECACHE_OK   ",  " XX 后台重建Cache完成 "},

    {WINKS_MSG_ENG2UI_RECRUIT_DL     , "WINKS_MSG_ENG2UI_RECRUIT_DL   ",  " XX 后台正在补充下载电话彩像... "},
    {WINKS_MSG_ENG2UI_RECRUIT_DL_OK  , "WINKS_MSG_ENG2UI_RECRUIT_DL_OK",  " XX 后台补充下载电话彩像完成 "},

/* 出错消息 */

/* 1 注册及初始化过程 */
    {WINKS_MSG_ENG2UI_REG_FAIL       , "WINKS_MSG_ENG2UI_REG_FAIL     ",  " 注册失败"},
    {WINKS_MSG_ENG2UI_INIT_FAIL      , "WINKS_MSG_ENG2UI_INIT_FAIL    ",  " 初始化失败"},

/* 2 更新好友录 */
    {WINKS_MSG_ENG2UI_FL_FAIL        , "WINKS_MSG_ENG2UI_FL_FAIL      ",  " 更新好友录失败"},

/* 3 同步本地配置到服务器 */
    {WINKS_MSG_ENG2UI_SETUP_FAIL     , "WINKS_MSG_ENG2UI_SETUP_FAIL   ",  " 同步本地配置失败"},

/* 4 上传DIY */
    {WINKS_MSG_ENG2UI_DIY_FAIL       , "WINKS_MSG_ENG2UI_DIY_FAIL     ",  " 上传DIY失败"},

/* 5 重新激活，查询服务器注册状态 */
    {WINKS_MSG_ENG2UI_REREG_FAIL     , "WINKS_MSG_ENG2UI_REREG_FAIL   ",  " 重新激活失败"},

/* 6 电话下载 */
    {WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL, "WINKS_MSG_ENG2UI_CALLCC_GRT_FAIL ", " 通话问候语下载失败"},
    {WINKS_MSG_ENG2UI_CALLCC_FAIL    , "WINKS_MSG_ENG2UI_CALLCC_FAIL     ", " 通话用户彩像下载失败"},
    {WINKS_MSG_ENG2UI_CALLCC_TIMEOUT , "WINKS_MSG_ENG2UI_CALLCC_TIMEOUT  ", " 油ɑ懊户彩像下载超时"},
    {WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL,"WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL", " XX通话获取片头运营商彩像失败"},

/* 7 后台服务 */
    {WINKS_MSG_ENG2UI_PUSH_FAIL        , "WINKS_MSG_ENG2UI_PUSH_FAIL        ",  " XX 后台定时更新失败 "},
    {WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL, "WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL",  " XX 后台下载好友列表失败 "},
    {WINKS_MSG_ENG2UI_INITCACHE_FAIL   , "WINKS_MSG_ENG2UI_INITCACHE_FAIL   ",  " XX 后台初始化Cache失败 "},

    {WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL, "WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL",  " XX 后台下载基本信息失败 "},
    {WINKS_MSG_ENG2UI_RECACHE_FAIL     , "WINKS_MSG_ENG2UI_RECACHE_FAIL     ",  " XX 后台重建Cache失败 "},

    {WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL  , "WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL  ",  " XX 后台补充下载电话彩像失败 "},

/* 系统错误 */
    {WINKS_MSG_ENG2UI_SVR_NO_ANSWER  , "WINKS_MSG_ENG2UI_SVR_NO_ANSWER",  " 服务器无应答"},
    {WINKS_MSG_ENG2UI_UNREG_USER     , "WINKS_MSG_ENG2UI_UNREG_USER   ",  " 非注册用户"},
    {WINKS_MSG_ENG2UI_ROAMING_LIMIT  , "WINKS_MSG_ENG2UI_ROAMING_LIMIT",  " 漫游限制"},
    {WINKS_MSG_ENG2UI_ENG_BUSY       , "WINKS_MSG_ENG2UI_ENG_BUSY     ",  " 引擎忙,稍候再试"},
    {WINKS_MSG_ENG2UI_DIAL_INT       , "WINKS_MSG_ENG2UI_DIAL_INT     ",  " 引擎被电话中断"},
    {WINKS_MSG_ENG2UI_STORAGE_LIMIT  , "WINKS_MSG_ENG2UI_STORAGE_LIMIT",  " 存储空间不足"},
    {WINKS_MSG_ENG2UI_CC_CLOSED      , "WINKS_MSG_ENG2UI_CC_CLOSED    ",  " 彩像软件关闭"},
#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_MCARD_NOT_EXIST,"WINKS_MSG_ENG2UI_MCARD_NOT_EXIST", " 存储卡不存在"},
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    {WINKS_MSG_ENG2UI_SYS_FILES_ERR  , "WINKS_MSG_ENG2UI_SYS_FILES_ERR",  " 系统文件被破坏"},
//#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_CC_FILES_ERR   , "WINKS_MSG_ENG2UI_CC_FILES_ERR ",  " 彩像文件被破"},
//#endif /* WINKS_SUPPORT_MEMORY_CARD */
#ifdef WINKS_DUAL_SIM
    {WINKS_MSG_ENG2UI_SIM_SEL_ERR    , "WINKS_MSG_ENG2UI_SIM_SEL_ERR  ",  " 提示用户选择要激活的Sim卡"},
    {WINKS_MSG_ENG2UI_SIM_UNUSABLE   , "WINKS_MSG_ENG2UI_SIM_UNUSABLE ",  " 用户选择的Sim卡不可用"},
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

/* 针对WINKS_ENG_ERROR_CODE_6,7,8, 不需改变当前状态，直接返回给UI出错信息 */
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

    /* 系统错误 */
    if( (pMsg->err_state & 0x000000ff) && pMsg->err_code )
        Eng2UiMsg = pMsg->err_code;
    /* 注册流程 */
    else if((pMsg->err_state & 0xff000000) == 0x02000000)
    {
        switch(pMsg->err_state)
        {
            /* 状态代码 */      /* 说明                                                                   */
            /* 注册及初始化过程 */
            case 0x02010202:    /* 连接配置服务器成功                                                     */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_SVR_OK;
            break;
            
            case 0x02010200:    /* 连接配置服务器失败                                                     */
            case 0x02020200:    /* 查询用户注册信息失败                                                   */
            case 0x02040200:    /* 注册失败                                                               */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REG_FAIL;
            break;
            
            case 0x02020203:    /* 未注册，需要发送注册短信                                               */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SMS_REG;
            break;
            
            case 0x02020205:    /* 注册成功（已经是注册用户），开始初始化，取基本配置信息                 */
            case 0x02040205:    /* 注册成功，开始初始化，取基本配置信息                                   */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SMS_REG_OK;
            break;
            
            case 0x02020204:    /* 正在发短信注册...                                                      */
            case 0x02030204:    /* 正在发短信注册...                                                      */
            case 0x02040204:    /* 正在发短信注册...                                                      */
            case 0x02050205:    /* 取基本配置信息成功，准备取片头、运营商彩像                             */
            case 0x02050206:    /* 取基本配置信息成功，准备取片头、运营商彩像                             */
            case 0x02060206:    /* 正在下载片头、运营商彩像···                                         */
            Eng2UiMsg = WINKS_MSG_ENG2UI_INIT;
            break;
            
            case 0x020502ff:    /* 取基本配置信息成功，初始化完毕；无片头、运营商彩像需要下载；完成初始化 */
            case 0x020602ff:    /* 取片头、运营商彩像成功，初始化完毕；完成初始化                         */
            Eng2UiMsg = WINKS_MSG_ENG2UI_INIT_OK;
            break;
            
            case 0x02050200:    /* 取基本配置信息失败                                                     */
            case 0x02060200:    /* 取片头、运营商彩像失败                                                 */
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
            /* 重新激活，查询服务器注册状态 */
            case 0x0409040a:    /* 连接配置服务器成功，准备向通讯服务器查询注册信息                       */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_SVR_OK;
            break;
            
            case 0x04090400:    /* 连接配置服务器失败                                                     */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REREG_FAIL;
            break;
            
            case 0x040a04ff:    /* 注册成功（已经是注册用户）                                             */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REREG_OK;
            break;
            
            case 0x040a0203:    /* 需要发送注册短信重新注册                                               */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SMS_REG;
            break;
            
            case 0x040a0400:    /* 查询用户注册信息失败                                                   */
            Eng2UiMsg = WINKS_MSG_ENG2UI_REREG_FAIL;
            break;
            }
        }
        else if(Winks_engineContext_p->app == WINKS_ENGINE_APP_UPLOAD_DIY)
        {
            switch(pMsg->err_state)
            {
            /* 上传DIY */
            case 0x04080408:    /* 正在上传DIY···                                                      */
            Eng2UiMsg = WINKS_MSG_ENG2UI_DIY;
            break;
            
            case 0x040804ff:    /* 上传DIY成功                                                            */
            Eng2UiMsg = WINKS_MSG_ENG2UI_DIY_OK;
            break;
            
            case 0x04080400:    /* 设置DIY失败                                                            */
            Eng2UiMsg = WINKS_MSG_ENG2UI_DIY_FAIL;
            break;
            }
        }
        else if(Winks_engineContext_p->app == WINKS_ENGINE_APP_SYNC_SETTING)
        {
            switch(pMsg->err_state)
            {
            /* 同步本地配置到服务器 */
            case 0x040704ff:    /* 设置信息同步服务器成功                                                 */
            Eng2UiMsg = WINKS_MSG_ENG2UI_SETUP_OK;
            break;
            
            case 0x04070400:    /* 设置信息同步服务器失败                                                 */
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
                case WINKS_ENGINE_APP_UPDATE_FRIEND:    /* 更新好友录 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_FRIEND_OK;
                    break;
            #ifdef WINKS_SUPPORT_3G
                case WINKS_ENGINE_APP_CALL_DOWNLOAD:    /* 电话下载 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_CALLCC_OK;
                    break;
            #endif /* WINKS_SUPPORT_3G */
                /* 后台服务 */
                case WINKS_ENGINE_APP_INIT_CACHE:        /* 初始化Cache */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_OK;
                    break;
                case WINKS_ENGINE_APP_REBULID_CACHE:     /* 重建Cache */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_OK;
                    break;
                case WINKS_ENGINE_APP_PERIOD_UPDATE:     /* 定时更新 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH_OK;
                    break;
                case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:  /* 补充下载 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECRUIT_DL_OK;
                    break;
                }
            }
            else if((pMsg->err_state&0x000000ff) == 0x00000000)
            {
                switch(Winks_engineContext_p->app)
                {
                case WINKS_ENGINE_APP_UPDATE_FRIEND:    /* 更新好友录 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_FL_FAIL;
                    break;
           #ifdef WINKS_SUPPORT_3G
                case WINKS_ENGINE_APP_CALL_DOWNLOAD:    /* 电话下载 */
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
                /* 后台服务 */
                case WINKS_ENGINE_APP_INIT_CACHE:        /* 初始化Cache */
                    if((pMsg->err_state&0x00ff0000) == 0x00030000)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_FAIL;
                    break;
                case WINKS_ENGINE_APP_REBULID_CACHE:     /* 重建Cache */
                    if((pMsg->err_state&0x00ff0000) == 0x000b0000)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_FAIL;
                    break;
                case WINKS_ENGINE_APP_PERIOD_UPDATE:     /* 定时更新 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH_FAIL;
                    break;
                case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:  /* 补充下载 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL;
                    break;
                }
            }
            else
            {
                switch(Winks_engineContext_p->app)
                {
                case WINKS_ENGINE_APP_UPDATE_FRIEND:    /* 更新好友录 */
                        Eng2UiMsg = WINKS_MSG_ENG2UI_DL_FRIENDCC;
                    break;
            #ifdef WINKS_SUPPORT_3G
                case WINKS_ENGINE_APP_CALL_DOWNLOAD:    /* 电话下载 */
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
                /* 后台服务 */
                case WINKS_ENGINE_APP_INIT_CACHE:        /* 初始化Cache */
                    if((pMsg->err_state&0x00ff00ff) == 0x00030005)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE_FL;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_INITCACHE;
                    break;
                case WINKS_ENGINE_APP_REBULID_CACHE:     /* 重建Cache */
                    if((pMsg->err_state&0x00ff0000) == 0x000b0000)
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE_TRIM;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_RECACHE;
                    break;
                case WINKS_ENGINE_APP_PERIOD_UPDATE:     /* 定时更新 */
                    if( ((pMsg->err_state&0x00ff0000) == 0x00010000) ||
                        ((pMsg->err_state&0x00ff0000) == 0x00020000) )
                        Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH;
                    else
                        Eng2UiMsg = WINKS_MSG_ENG2UI_PUSH_FRIEND;
                    break;
                case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:  /* 补充下载 */
                    Eng2UiMsg = WINKS_MSG_ENG2UI_RECRUIT_DL;
                    break;
                }
            }
        }
    }

#if 0   /* mask屏蔽UI不处理的消息 */ 
    /* 将UI不关心的消息转化为WINKS_MSG_ENG2UI_NONE */
    switch(Eng2UiMsg)
    {
    case WINKS_MSG_ENG2UI_NONE:
        Winks_printf("Winks_Eng2UiMsgMap: [%08x] ????????????????????????????????????? \r\n", pMsg->err_state);
        break;
/* 1 注册及初始化过程 */
    case WINKS_MSG_ENG2UI_SETUP_SVR_OK  :   /* XX 连接配置服务器成功 */
    case WINKS_MSG_ENG2UI_INIT          :   /* XX 正在初始化... */
/* 2 更新好友录 */
    case WINKS_MSG_ENG2UI_FL_OK         :   /* XX 好友列表下载完毕 */
    case WINKS_MSG_ENG2UI_FRIEND_SET    :   /* XX 上传好友录成功 */
    case WINKS_MSG_ENG2UI_FRIEND_UPD    :   /* XX 上传好友录及下载好友数据 */
/* 3 同步本地配置到服务器 */
/* 4 上传DIY */
    case WINKS_MSG_ENG2UI_DIY           :   /* XX 正在上传DIY... */
/* 5 重新激活，查询服务器注册状态 */
/* 6 电话下载 */
    case WINKS_MSG_ENG2UI_CALLCC_OTHER  :   /* XX获取片头运营商彩像 */
    case WINKS_MSG_ENG2UI_CALLCC_OTHR_FAIL: /* XX获取片头运营商彩像失败 */
/* 7 后台服务 */
    case WINKS_MSG_ENG2UI_PUSH          :   /* XX 后台正在定时更新服务... */
    case WINKS_MSG_ENG2UI_PUSH_FRIEND   :   /* XX 后台正在更新好友... */
    case WINKS_MSG_ENG2UI_PUSH_OK       :   /* XX 后台定时更新服务完毕 */
    case WINKS_MSG_ENG2UI_PUSH_CHECK_FL :   /* XX 后台定时更新服务完毕,但需要检查好友列表 */
    case WINKS_MSG_ENG2UI_PUSH_FAIL     :   /* XX 后台定时更新服务失败 */

    case WINKS_MSG_ENG2UI_INITCACHE_FL  :   /* XX 后台正在下载好友列表以便初始化Cache... */
    case WINKS_MSG_ENG2UI_INITCACHE     :   /* XX 后台正在初始化Cache... */
    case WINKS_MSG_ENG2UI_INITCACHE_OK  :   /* XX 后台初始化Cache完成 */
    case WINKS_MSG_ENG2UI_INITCACHE_FL_FAIL :   /* XX 后台下载好友列表失败 */
    case WINKS_MSG_ENG2UI_INITCACHE_FAIL    :   /* XX 后台初始化Cache失败 */

    case WINKS_MSG_ENG2UI_RECACHE_TRIM  :   /* XX 后台正在下载基本信息以重建Cache... */
    case WINKS_MSG_ENG2UI_RECACHE       :   /* XX 后台正在重建Cache... */
    case WINKS_MSG_ENG2UI_RECACHE_OK    :   /* XX 后台重建Cache完成 */
    case WINKS_MSG_ENG2UI_RECACHE_TRIM_FAIL :   /* XX 后台下载基本信息失败 */
    case WINKS_MSG_ENG2UI_RECACHE_FAIL      :   /* XX 后台重建Cache失败 */

    case WINKS_MSG_ENG2UI_RECRUIT_DL    :   /* XX 后台正在补充下载电话彩像... */
    case WINKS_MSG_ENG2UI_RECRUIT_DL_OK :   /* XX 后台补充下载电话彩像完成 */
    case WINKS_MSG_ENG2UI_RECRUIT_DL_FAIL   :   /* XX 后台补充下载电话彩像失败 */

/* 其他UI不处理的消息 */
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
    /* 不用检查存储是否成功 */
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
/*保存查询注册成功长周期函数*/
static int winks_EngSetQueryLongTime(unsigned long time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    data_p->longTime = time;
    /* 不用检查存储是否成功 */
    Winks_set_setting_value(E_SETTING_LONG_CYCLE, time);
    
    return 0;
}
#if 0
/*读取查询注册成功长周期函数*/
static int winks_EngGetQueryLongTime(unsigned long *time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    *time = data_p->longTime;
    return 0;
}
#endif
/*保存查询注册成功短周期函数*/
static int winks_EngSetQueryShortTime(unsigned long time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    data_p->shortTime = time;
    /* 不用检查存储是否成功 */
    Winks_set_setting_value(E_SETTING_SHORT_CYCLE, time);
    
    return 0;
}
#if 0
/*读取查询注册成功短周期函数*/
static int winks_EngGetQueryShortTime(unsigned long *time)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

    *time = data_p->shortTime;
    return 0;
}
#endif
/*保存查询注册成功问候语函数*/
static int winks_EngSetQueryRetMsg(unsigned char *msg, unsigned long msg_len)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);

#if 1/* UI没有处理该消息内容,不用给UI发送 */
    data_p->msg[0] = 0;
#else
    if(msg_len<=WINKS_ENGINE_RETURN_MSG_MAX_LEN)
        Winks_mem_cpy(data_p->msg, msg, msg_len);
    data_p->msg[sizeof(data_p->msg) - 1] = 0;
#endif
    return 0;
}
#if 0
/*读取查询注册成功问候语函数*/
static int winks_EngGetQueryRetMsg(unsigned char *msg, unsigned long msg_len)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);
    
    strncpy((char *)msg, (char *)data_p->msg, msg_len - 1);
    msg[msg_len - 1] = 0;
    return 0;
}
#endif
/* 保存通讯服务器ip和端口 */
static int winks_EngSetCommSvr(Winks_server_list_s *pServer)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);
    Winks_assert(pServer);

    memcpy(&data_p->server, pServer, sizeof(Winks_server_list_s));
    return 0;
}
#if 0
/* 读取通讯服务器ip和端口 */
static int winks_EngGetCommSvr(Winks_server_list_s *pServer)
{
    Winks_EngPrtReg_s *data_p = winks_EngGetRegContext();
    Winks_assert(data_p);
    
    memcpy(pServer, &data_p->server, sizeof(Winks_server_list_s));
    return 0;
}
#endif
/*校正连网方式的数据。*/
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

/* 发送注册短信 */
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

/*发送数据*/
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

    /*检查漫游标志:国际漫游不联网设置*/
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

    /*记录传输信息*/
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
            if(1)/*开关开启，变URL*/
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
#ifdef WINKS_SIMULATOR/*模拟器从第1个ip开始测试连接*/
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
    /*设置请求方法为POST。*/
    httpstr.method = WINKS_HTTP_MEPOST;
    /*设置消息接收函数的接收消息。*/
    httpstr.msg = WINKS_MSG_ENGINE_HTTP;
    /*无内容类型*/
    httpstr.pContenttype = NULL;

#ifdef TEST_HTTP /* 测试Http协议 */
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
    
    /*可按需要设置不支持持久连接*/
    Winks_httpsetopt( WINKS_HTTP_OPPIPE, &ifpipe, sizeof(ifpipe) );
    Winks_httpsetopt( WINKS_HTTP_OPUAGENG, WINKS_USRAGENT, strlen(WINKS_USRAGENT) );

    /*设置请求网页的URL。*/
    httpstr.pURL = sUrl;

    /*创建HTTP传输句柄。*/
    Winks_engineContext_p->http = Winks_httpopen( &httpstr );
    if(Winks_engineContext_p->http == WINKS_HTTP_NULL)
    {
        Winks_assert(Winks_engineContext_p->http != WINKS_HTTP_NULL);
        return WINKS_RETURN_FAILURE;
    }

#ifdef TEST_HTTP /* 测试Http协议 */
    /*写入数据*/
    iRet = Winks_httpappenddata( Winks_engineContext_p->http, data, len );
    if(iRet == -1)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_assert(iRet >= 0);
        return WINKS_RETURN_FAILURE;
    }
#else
    /*写入数据*/
    iRet = Winks_httpappenddata( Winks_engineContext_p->http, data, len );
    if(iRet == -1)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_assert(iRet >= 0);
        return WINKS_RETURN_FAILURE;
    }
#endif

    /*启动传输。*/
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
    
    /*循环匹配多个通讯服务器机制*/
    memset(&server, 0, sizeof(server));
    iRet = Winks_GetCommServerIpPort(&server);
    if( (Winks_engineContext_p->server.srv == 0) || /* 配置服务器不必重试 */
        ((unsigned long)(Winks_engineContext_p->server.tryCnt + 1) >= server.cnt) || /* 所有IpPort都试过了 */
        (Winks_engineContext_p->server.datainFlag == 1) )/* 当前IpPort收到过有效数据 */
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
        /*句柄关闭消息，本消息通常由于传输出错或者用户请求产生，
        用户可在本消息接收到以后关闭句柄，释放资源。*/
        Winks_printf( "WINKS http get http message close ok\r\n" );
        return winks_EngRetErr(WINKS_MSG_ENG2UI_SVR_NO_ANSWER);
    }

    return iRet;
}

/* Http消息处理函数 */
static int winks_HttpMsgProc(const void* data, unsigned int size )
{
    int http = WINKS_HTTP_NULL;

    char pURL[WINKS_URL_MAX_LEN+1];
    int ulen = 0;
    Winks_Socketmsg_s* p = (Winks_Socketmsg_s* )data;
    /*Winks_HttpInfo Httpinfo;*/

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    /*消息数据有效性检查。*/
    if( p == NULL )
    {
        Winks_printf( "WINKS http get http message no data\r\n" );
        return 0;
    }
    
    /*获取消息对应的HTTP传输句柄。*/
    http = p->wParam & 0x0000ffff;
    /*ulen = WINKS_URL_MAX_LEN;*/
    Winks_mem_set(pURL, 0, sizeof(pURL));
    switch( p->wParam >> 24 )
    {
    case WINKS_HTTP_GH:
        /*域名查询消息。本消息不需要进行处理，属于进程状态通知消息。
        获取对应的HTTP传输句柄的请求的URL数据。*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*域名查询出错，提示。*/
            Winks_printf( "[ENG]WINKS http %s get http message gethost get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            break;
        }
        else
            /*  域名查询成功，提示。*/
            Winks_printf( "[ENG]WINKS http %s get http message gethost ok\r\n", pURL );
        break;

    case WINKS_HTTP_CONNECT:
        /*连接消息。本消息不需要进行处理，属于进程状态通知消息。
        获取对应的HTTP传输句柄的请求的URL数据。*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*连接出现错误，提示。*/
            Winks_printf( "[ENG]WINKS http %s get http message connect get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            break;
        }
        /*  连接成功，提示。*/
        Winks_printf( "[ENG]WINKS http %s get http message connect ok\r\n", pURL );
        break;

    case WINKS_HTTP_DATAIN:
        /*数据到达消息。本消息不需要进行处理，属于进程状态通知消息。随消息会告知用户目前已经接收到的数据长度。
        获取对应的HTTP传输句柄的请求的URL数据。*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*数据接收出错，提示。*/
            Winks_printf( "[ENG]WINKS http %s get http message datain get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            break;
        }
        /*数据接收成功，提示并提示接收数据长度。*/
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
        /*有数据相应时重置等待数据的时间*/
        Winks_assert(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL);
        Winks_StopTimer(Winks_engineContext_p->httpTimer);
        Winks_StartTimer(Winks_engineContext_p->httpTimer);

        break;

    case WINKS_HTTP_DATAEND:
    {
        int iRet = 0;
        unsigned long ret_code = 0;
        Winks_printf( "[ENG]WINKS http get http message dataend ok\r\n");
        
        /*数据解析*/
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
            else if(winks_HttpTryNextIpPort(http) != WINKS_RETURN_FAILURE)/* 开始尝试下1组IpPort */
            {
                return 0;
            }
          #if 0
            else/* 几组IpPort都不能连接成功,返回连接失败 */
            {
            }
          #endif
        }
        else    /* 收到正确的数据 */
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
            
            /* 收到正确数据锁定当前IpPort */
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
        else 按错误处理，关闭Http
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
    0--没有发现Sim卡,不能启动             (后面参数不用输入)
    1--发现1张Sim卡,但不是已注册的Sim卡   (后面参数输入1组即可)
    2--发现2张Sim卡,但都不是已注册的Sim卡 (后面参数必须都输入)
    3--发现已注册的Sim卡,正常启动         (后面参数不用输入)
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
    
    if(BootMethod == 0)/* 没有Sim卡，继续停留在启动状态 */
        return WINKS_RETURN_FAILURE;
    else if(BootMethod == 3)/* 找到已注册Sim卡 */
    {
        Winks_engineContext_p->curState = WINKS_ENGINE_STATE_ENABLED;
        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
        //Winks_engineContext_p->IsPeriodUpdating = 0;
        Winks_engineContext_p->isImsiEnabled = WINKS_IMSI_ENABLED;
        Winks_engineContext_p->ifRemindRegister = 0;
        /* 启动定时更新服务 */
        winks_StartPeriodPushTimer();
        
        StrgStatus = Winks_StrgGetStatus();
        if( StrgStatus == WINKS_STRG_CARD_STATUS_NO_CCCACHE ||
            StrgStatus == WINKS_STRG_CARD_STATUS_OLD_CARD )
        {
            /* 未完成Cache初始化 */
            winks_StartBuildCacheTimer();
        }
    }
    else/* 未找到已注册Sim卡 */
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
        if( (flag==1) &&  /* 第1Sim卡不需要提示,继续查看第2Sim卡是否需要提示 */
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
    /*测试功能开关*/
    Winks_SetOsalTestBitFlag(2, 0);/*debug打印开关*/
#endif

    /*Post msg to ui for pop infomation.*/
    iRet = Winks_PostMsgToUi(WINKS_MSG_ENGINE_REG_NOTIFY, &Winks_engineContext_p->ifRemindRegister, 1);

    return 0;
}

/* 组桢并发送 */
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

    if(AttrId == 0x7FF9)    /* 连接配置服务器 */
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

/* 根据状态区分不同协议号，并调用组桢发送程序 */
static int winks_EngineSendDataToServer(unsigned long state, unsigned long sub_state, void *data)
{
    int iRet = 0;
    int PD_ErrCode = 0;
    unsigned short AttrId = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_printf("winks_EngineSendDataToServer(%x, %x, 0x%08x)\r\n", state, sub_state, data);
    
    if(Winks_engineContext_p->InitSettingOk == 0)/* 存储初始化失败 */
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
        case WINKS_ENGINE_SUBSTATE_ONE:     /* 连接配置服务器 */
            AttrId = 0x7FF9;
            break;
        case WINKS_ENGINE_SUBSTATE_TOW:     /* 查询注册信息 */
        case WINKS_ENGINE_SUBSTATE_FOUR:
            AttrId = 0x7FFD;
            break;
        case WINKS_ENGINE_SUBSTATE_FIVE:    /* 下载基本信息 */
            AttrId = 0x7FEC;
            break;              
        case WINKS_ENGINE_SUBSTATE_SIX:     /* 下载片头运营商彩像 */
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
        case WINKS_ENGINE_SUBSTATE_ONE:/*定时更新*/
            AttrId = 0x7FED;
            break;
        case WINKS_ENGINE_SUBSTATE_TOW:/*根据彩像邋ID下载彩像资源*/
        case WINKS_ENGINE_SUBSTATE_SIX:
        case WINKS_ENGINE_SUBSTATE_TWELVE:
            AttrId = 0x7FFA;
            break;
        case WINKS_ENGINE_SUBSTATE_THREE:/*请求好友列表*/   
            AttrId = 0x7FF0;
            break;
        case WINKS_ENGINE_SUBSTATE_FIVE:/*下载好友信息*/
        case WINKS_ENGINE_SUBSTATE_ELEVEN:/*下载好友信息*/
            AttrId = 0x7FF8;
            break;
#ifdef WINKS_DEL_PV2_0    
        case WINKS_ENGINE_SUBSTATE_SEVEN:/*实时同步服务器*/
            AttrId = 0x7FF7;
            break;
#endif
        case WINKS_ENGINE_SUBSTATE_EIGHT:/*DIY上传*/
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
        case WINKS_ENGINE_SUBSTATE_NINE:/*Enable状态点击注册，重连配服并向通讯服务器查询注册状态*/
            AttrId = 0x7FF9;
            break;
        case WINKS_ENGINE_SUBSTATE_TEN:/*Enable状态点击注册，向通讯服务器查询注册状态*/
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
/* 定时服务程序 */
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
            /* 检查定时服务周期是否与服务器下发的一致，若不一致则重启定时器 */
            winks_StartPeriodPushTimer();
        }
        else
        {
            /* 引擎忙，延时120秒再启动定时更新服务 */
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
            winks_StartBuildCacheTimer();/* 继续等待下个周期检查 */
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
            if(Winks_engineContext_p->SimStatus & 0xff00)/* 至少已找到1张卡了,重试1次另一张卡 */
            {
                if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0100)/* 找到了卡1,试图取卡2的imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(1) == TRUE);
                    Winks_engineContext_p->CurInquireSimNo = 2;
                    iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
                }
                else/* 找到了卡2,试图取卡1的imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    Winks_assert(Winks_GetActiveSim()==0);/* Init,Start阶段不能调用Winks_SetActiveSim否则Winks_GetImsi出错 */

                    Winks_engineContext_p->CurInquireSimNo = 1;
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                }
            }
            else/* 没有找到Sim卡,依次继续轮询两张卡 */
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
            if(iRet == WINKS_RETURN_SUCCESS)/* 读取imsi成功 */
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
            else if(iRet == WINKS_RETURN_FAILURE)/* 读取imsi失败 */
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
                /* 等待Winks_GetImsi给引擎发WINKS_MSG_IMSI_REQ消息 */
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
        case WINKS_ENGINE_APP_INIT_CACHE:/* 未完成Cache初始化 */
        case WINKS_ENGINE_APP_REBULID_CACHE:/* 未完成Cache重建 */
            winks_StartBuildCacheTimer();
            break;
        default:
            break;
        }
    }
}

/* 服务器返回出错信息的引擎处理函数 
   返回值为WINKS_RETURN_SUCCESS，则错误得到了处理，不需要后续处理
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
    /*客户端相关错误代码范围：        (0001H  ---  0FFFH)*/
    case 0x0001:/*a)    未知错误                        （ 0001H ）*/
        Winks_printf("EEEE[%04x]: 未知错误 \r\n", high16Bit);
        break;
        
    /*配置服务器相关错误代码：        (1001H  ---  1FFFH)*/
    case 0x1002:/*a)    配置服务器内部错误                （ 1002H ）*/
        Winks_printf("EEEE[%04x]: 配置服务器内部错误 \r\n", high16Bit);
        break;

    case 0x1003:/*b)    无有效的通信服务器地址            （ 1003H ）*/
        Winks_printf("EEEE[%04x]: 无有效的通信服务器地址 \r\n", high16Bit);
        break;
        
    case 0x1004:/*c)    无有效的短信特服号码            （ 1004H ）*/
        Winks_printf("EEEE[%04x]: 无有效的短信特服号码 \r\n", high16Bit);
        break;

    /*通讯服务器相关错误代码：        (2001H  ---  2FFFH)*/
    case 0x2002:/*a)    通信服务器内部错误                （ 2002H ）*/
        Winks_printf("EEEE[%04x]: 通信服务器内部错误 \r\n", high16Bit);
        break;
        
    /*数据库相关错误代码：            (4001H  ---  4FFFH)*/
    
    /*其他错误：                    (5001H  ---  FFFFH)*/
    case 0x1001:/*a)    协议解析错误                    （1001H）*/
        Winks_printf("EEEE[%04x]: 协议解析错误 \r\n", high16Bit);
        break;
        
    case 0x4001:/*b)    客户没有定制信息                （4001H）*/
        Winks_printf("EEEE[%04x]: 客户没有定制信息 \r\n", high16Bit);
        break;
        
    case 0x4002:/*c)    客户没有匹配当前时间的定制彩像     （4002H）*/
        Winks_printf("EEEE[%04x]: 客户没有匹配当前时间的定制彩像 \r\n", high16Bit);
        break;
        
    case 0x4003:/*d)    无适合此型号客户端的彩像        （4003H）*/
        Winks_printf("EEEE[%04x]: 无适合此型号客户端的彩像 \r\n", high16Bit);
        break;
        
    case 0x4004:/*e)    彩像文件未找到                    （4004H）*/
        Winks_printf("EEEE[%04x]: 彩像文件未找到 \r\n", high16Bit);
        break;
        
    case 0x4005:/*f)    用户被运营商锁定                （4005H）*/
        Winks_printf("EEEE[%04x]: 用户被运营商锁定 \r\n", high16Bit);
        break;
        
    case 0x5001:/*g)    本机号码为空错误                 （5001H）*/
        Winks_printf("EEEE[%04x]: 本机号码为空错误 \r\n", high16Bit);
        break;
        
    case 0x5002:/*h)    对方号码为空错误                 （5002H）*/
        Winks_printf("EEEE[%04x]: 对方号码为空错误 \r\n", high16Bit);
        break;
        
    case 0x5003:/*i)    CRC校验错误                          （5003H）*/
        Winks_printf("EEEE[%04x]: CRC校验错误 \r\n", high16Bit);
        break;
        
    case 0x5004:/*j)    上传文件错误                      （5004H）*/
        Winks_printf("EEEE[%04x]: 上传文件错误 \r\n", high16Bit);
        break;
        
    case 0x5005:/*k)    上传文件信息处理错误            （5005H）*/
        Winks_printf("EEEE[%04x]: 上传文件信息处理错误 \r\n", high16Bit);
        break;
        
    case 0x5006:/*l)    用户未注册                        （5006H）*/
    {        
        Winks_printf("EEEE[%04x]: 用户未注册 \r\n", high16Bit);
        switch(Winks_engineContext_p->curState)
        {
        case WINKS_ENGINE_STATE_ENABLED:
        case WINKS_ENGINE_STATE_SHOWB:
            switch(Winks_engineContext_p->subState)
            {
                case WINKS_ENGINE_SUBSTATE_FIVE:
                case WINKS_ENGINE_SUBSTATE_ELEVEN:
                case WINKS_ENGINE_SUBSTATE_TEN:
                    /*在函数winks_HttpRetMsgProc中处理*/
                    break;
                default:
                    /*服务器返回未注册，退到未注册等待状态*/
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
                    /*在函数winks_HttpRetMsgProc中处理*/
                    break;
                default:
                    /*服务器返回未注册，退到未注册等待状态*/
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
    case 0x5007:/*m)    用户超过最大上传彩像数             （5007H）*/
        Winks_printf("EEEE[%04x]: 用户超过最大上传彩像数 \r\n", high16Bit);
        break;
        
    case 0x5008:/*n)    服务器超过有效期停止服务错误     （5008H）*/
        Winks_printf("EEEE[%04x]: 服务器超过有效期停止服务错误 \r\n", high16Bit);
        break;
        
    case 0x5009:/*o)    用户上传问候语修改失败            （5009H）*/
        Winks_printf("EEEE[%04x]: 用户上传问候语修改失败 \r\n", high16Bit);
        break;
        
    case 0x500A:/*p)    超过用户最大定制业务数错误        （500AH）*/
        Winks_printf("EEEE[%04x]: 超过用户最大定制业务数错误 \r\n", high16Bit);
        break;
        
    case 0x500B:/*q)    客户端电话本号码属性空错误        （500BH）*/
        Winks_printf("EEEE[%04x]: 客户端电话本号码属性空错误 \r\n", high16Bit);
        break;
        
    case 0x500C:/*r)    上传属性值错误                    （500CH）*/
        Winks_printf("EEEE[%04x]: 上传属性值错误 \r\n", high16Bit);
        break;
        
    case 0x500D:/*s)    无最新版本软件                    （500DH）*/
        Winks_printf("EEEE[%04x]: 无最新版本软件 \r\n", high16Bit);
        break;
        
    case 0x500E:/*t)    用户不是收费用户                （500EH）*/
        Winks_printf("EEEE[%04x]: 用户不是收费用户 \r\n", high16Bit);
        break;
        
    case 0x500F:/*u)    重新获取通讯服务器地址            （500FH）*/
        Winks_printf("EEEE[%04x]: 重新获取通讯服务器地址 \r\n", high16Bit);
        break;
        
    case 0x5010:/*v)    查询的用户信息块不存在            （5010H）*/
        Winks_printf("EEEE[%04x]: 查询的用户信息块不存在 \r\n", high16Bit);
        break;
        
    case 0x5011:/*w)    手机空间不足                    （5011H）*/
        Winks_printf("EEEE[%04x]: 手机空间不足 \r\n", high16Bit);
        break;
        
    case 0x5012:/*x)    用户没有任何好友信息            （5012H）*/
        Winks_printf("EEEE[%04x]: 用户没有任何好友信息 \r\n", high16Bit);
        break;
        
    /*未定义错误*/
    default:
        Winks_printf("EEEE[%04x]: 未定义错误 \r\n", high16Bit);
        break;
    }

    if(iRet == WINKS_RETURN_FAILURE)/*未处理直接返回*/
    {
        return iRet;
    }
    if(FlagSendMsgToUi == 1)
    {
        retMsg.err_state <<= 16;
        if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_COMPLETE)
            retMsg.err_state |= (unsigned long)(retMsg.err_state & 0xff000000)>>16;/*curstate=前一状态*/
        else
            retMsg.err_state |= (unsigned long)(Winks_engineContext_p->curState & 0x000000ff)<<8;
        retMsg.err_state |= (unsigned long)(Winks_engineContext_p->subState & 0x000000ff);
        
        if(pSendMsg)
        {
            pSendMsg->err_state = retMsg.err_state;
            Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", pSendMsg->err_state, pSendMsg->err_code);
            pSendMsg->err_code = Winks_Eng2UiMsgMap(pSendMsg);
            winks_EngRetMsg2UI(pSendMsg, SendMsgLen);/* msg_body有数据发送 */
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
    
    /*完成变更后复位子状态机*/
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

/* 打包发给UI的包含msg_body的消息 */
static Winks_EngineToUiReturnMsg * winks_AppendUiMsgBody(Winks_EngineToUiReturnMsg *pOrgMsg, void *msgbody, int msgbodylen, unsigned long *RetLen)
{
    Winks_EngineToUiReturnMsg *pRetMsg;
    
    *RetLen = sizeof(Winks_EngineToUiReturnMsg) + msgbodylen;/* 多申请了1字节*/
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

/* 为临时上下文分配空间 Winks_engineContext_p->communicationContext_p */
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

    if(!phone_cnt && !sim_cnt)/* 电话簿为空 */
    {
        return 0;
    }

    cnt = 0;
    while(cnt < phone_cnt + sim_cnt)/* 逐个添加电话号码到到发送缓存 */
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
            if((NumberLen <= 0) || (NumberLen >= WINKS_FRIEND_NO_LEN))/*无效的电话号码,忽略*/
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
                if(winks_EngNoInPhoneBook(pNo) > 0)/* 主动好友 */
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
    while(len < FriendListLen)/* 被动好友 */
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


/* 检查是否需要重建Cache */
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

    /* 重建Cache */
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

#define WINKS_DOWNCC_OP_WASHOUT     0x01    /* 需要淘汰 */
#define WINKS_DOWNCC_OP_REC_ND      0x02    /* 需要记录到<nd.dat> */

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
            /* 彩像没有更新，跳过 */
            NeedUpdateCC = 0;
        }
        else if(Winks_StrgCCIsExist(UsrInfo.CCIdx, &CCInfo))
        {
            if(strcmp(pDownCC->ND_ID, CCInfo.ID) == 0)
            {
                Winks_printf("winks_EngDealUserBaseInfo OKOK : user cc not changed! (%d, 0x%04x)\r\n", retIdx, UsrInfo.CCIdx);
                /* 彩像没有更新，跳过 */
                NeedUpdateCC = 0;
            }
            else 
            {
                if(Winks_StrgCCUserNum(UsrInfo.CCIdx, &UsedCCFriendNum) <= 1)
                {
                    /* 用户更换彩像,没有别人使用原彩像,删除 */
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
                /* 原先有问候语,且原存储位置能容纳目前问候语 */
                NeedUpdateGreet = 0;
            }
            else
            {
                /* 删除问候语 */
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
        /* 没有地方存放该用户 */
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
                return WINKS_RETURN_FAILURE;/* 用户数超限 */
        }
        Winks_mem_cpy(UsrInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
    }

    /* 查找存储彩像信息空位 */
    if(NeedUpdateCC)
    {
        if( Winks_StrgSearchCC(pDownCC->ND_ID, &UsrInfo.CCIdx) == WINKS_STRG_SEARCH_RESULT_FOUND &&
            Winks_StrgCCIsExist(UsrInfo.CCIdx, NULL) )
        {
            /* 彩像已存在 */
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
                    return WINKS_RETURN_FAILURE;    /* 磁盘满不能存储彩像 */
            }
        }
    }
    
    /* 查找保存问候语空位 */
    if(NeedUpdateGreet)
    {
        if(Winks_StrgSearchGreetEpt(pDownCC->GreetLen, &UsrInfo.GrtOff) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
        {
            /*  整理问候语文件,以便腾出空间;然后继续找出空闲空间 */
            if(Winks_StrgTrimGreet() != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;/* 文件系统错误 */

            /* 整理完问候语文件,还是没地方存放问候语,放弃 */
            if(Winks_StrgSearchGreetEpt(pDownCC->GreetLen, &UsrInfo.GrtOff) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
                return WINKS_RETURN_FAILURE;    /* 磁盘满不能存储问候语 */
        }
    }
    
    /* 添加下载彩像信息 */
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
                    
                return WINKS_RETURN_FAILURE;    /* 磁盘满不能添加下载信息 */
            }
        }
        
        if(iRet != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    /* 存储问候语 */
    if(pDownCC->GreetValid && UsrInfo.GrtOff)
    {
        Winks_mem_set(GreetInfo.GreetStr, 0, GreetInfo.GreetLen);
        Winks_mem_cpy(GreetInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
        GreetInfo.GreetLen = pDownCC->GreetLen;
        Winks_mem_cpy(GreetInfo.GreetStr, pDownCC->GreetStr, pDownCC->GreetLen);
        iRet = Winks_StrgSetGreet(UsrInfo.GrtOff, &GreetInfo);
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    }

    /* 存储彩像基本信息和用户与彩像问候语关联信息 */
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

/* 引擎解析Http返回包后消息处理函数 */
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
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_THREE;/*提示用户需要发短信注册*/
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
        case WINKS_ENGINE_SUBSTATE_FOUR:/*查询注册是否成功*/
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

                FlagForceBreak = 1;/* 等定时器时间到再发送联网查询是否注册信息 */
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
            }
            break;
        }
        case WINKS_ENGINE_SUBSTATE_FIVE:/*下载基本信息*/
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
                /* 基本信息成功,直接到state6下载彩像，所以没有break */
            }
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                break;
            }
        }
        case WINKS_ENGINE_SUBSTATE_SIX:/*下载片头和运营商彩像*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
         
            if( low16Bit || 
                ((high16Bit != 0) && (high16Bit != 0x4003) && (high16Bit != 0x4004)) ) /* 本地错误 或 服务器返回非彩像错误 */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                pDownCC->img_idx++;/* 一个ID对应的多张图片，依次获取 */
                if(pDownCC->img_idx >= pDownCC->ND_img_cnt)/* 上一个彩像ID的几张图片全部下载完 */
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
                        /* 下载完成 */
                        char ImsiBuf[WINKS_IMSI_LEN+1];
                        /* 存IMSI号 */
                        iRet = Winks_GetImsi(WINKS_CHN_NULL, ImsiBuf, sizeof(ImsiBuf));
                        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                        ImsiBuf[WINKS_IMSI_LEN] = 0;
                        Winks_SetRecentlyActivatedImsi(ImsiBuf,WINKS_IMSI_LEN);

                        Winks_StrgNDDeleteAll();
                        
                        /* 启动定时更新服务 */
                        winks_StartPeriodPushTimer();
                        /* 启动初始化Cache定时器 */
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
        case WINKS_ENGINE_SUBSTATE_ONE:/*下载基本信息*/
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
                
                /* 检查定时服务周期是否与服务器下发的一致，若不一致则重启定时器 */
                if(Winks_engineContext_p->app == WINKS_ENGINE_APP_PERIOD_UPDATE)
                    winks_StartPeriodPushTimer();

                /* 基本信息成功,直接到state6下载彩像，所以没有break */
            }         
            else
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                break;
            } 
        }
        case WINKS_ENGINE_SUBSTATE_TOW:/*下载片头、运营商、及用户彩像*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
         
            if( low16Bit || 
                ((high16Bit != 0) && (high16Bit != 0x4003) && (high16Bit != 0x4004)) ) /* 本地错误 或 服务器返回非彩像错误 */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                pDownCC->img_idx++;/* 一个ID对应的多张图片，依次获取 */
                if(pDownCC->img_idx >= pDownCC->ND_img_cnt)/* 上一个彩像ID的几张图片全部下载完 */
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
        case WINKS_ENGINE_SUBSTATE_THREE:/*下载好友列表*/
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
        case WINKS_ENGINE_SUBSTATE_FIVE:/*下载用户彩像信息*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            if( high16Bit == 0x5006 )
            {
                retMsg.err_code = error_code = 0x40010000;
                high16Bit = 0x4001;
            }
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* 本地错误 或 服务器返回非彩像错误 */
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
                /* 直接到state6下载下一用户基本信息，所以没有break */
            }
        }
        case WINKS_ENGINE_SUBSTATE_SIX:/*根据彩像ID下载彩像资源*/
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
            
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* 本地错误 或 服务器返回非彩像错误 */
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
        case WINKS_ENGINE_SUBSTATE_SEVEN:/*实时设置同步服务器*/
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
        case WINKS_ENGINE_SUBSTATE_EIGHT:/*DIY上传*/
        {
            if(error_code == 0x00000000)
            {
                Winks_EngDiyComInfo_s *pULInfo = winks_EngGetDiyContext();
                
                Winks_assert(pULInfo != NULL);
                
                Winks_printf("第[%d/%d]文件第[%d/%d]块上传成功!\r\n", pULInfo->curIndex+1, pULInfo->image_count, pULInfo->subIndex+1, pULInfo->subTotalCount);
                if(pULInfo->subTotalCount>1)/* 分块上传*/
                {
                    pULInfo->subIndex++;
                    if(pULInfo->subIndex >= pULInfo->subTotalCount)/*已传完一个文件*/
                    {
                        pULInfo->curIndex++;
                        pULInfo->curFileSize = 0;/* 将文件大小置0 以便后续程序上传下个文件*/
                    }
                }
                else/* 整个文件上传*/
                {
                    pULInfo->curIndex++;
                    pULInfo->curFileSize = 0;/* 将文件大小置0 以便后续程序上传下个文件*/
                }

                if(pULInfo->curIndex >= pULInfo->image_count)/*所有文件上传完毕*/
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
                /* 已激活用户重新注册，等待发送短信（服务器与本地状态不一致） */
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_THREE;
                {
                Winks_EngPrtReg_s *data_p = (Winks_EngPrtReg_s *)Winks_engineContext_p->communicationContext_p;
                if((data_p==NULL) || (Winks_SetCommServerIpPort(&data_p->server)!=0))
                {
                    Winks_SubStateRollback();
                    /* 已激活用户重新注册，服务器与本地状态不一致，重启注册流程 */
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
        case WINKS_ENGINE_SUBSTATE_ELEVEN: /* 下载用户基本信息 */
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
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* 本地错误 或 服务器返回非彩像错误 */
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
                        /* 文件系统不能访问[可能在Usb状态],暂时挂起后台任务 */
                        Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                        break;
                    }
                    else
                    {
                        /* 没有足够磁盘空间，进入状态12，开始下载彩像 */
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
                    /* 下载完成 */
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
                /* 直接到state12下载下彩像，所以没有break */
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
        case WINKS_ENGINE_SUBSTATE_TWELVE: /* 根据彩像ID下载彩像资源 */
        {
            Winks_EngCCDownloadContext_s *pDownCC = NULL;
            
            pDownCC = winks_EngGetCCDownContext();
            Winks_assert(pDownCC != NULL);
         
            if( low16Bit || 
                ((high16Bit != 0) && ((high16Bit < 0x4001) || (high16Bit > 0x4005))) ) /* 本地错误 或 服务器返回非彩像错误 */
            {
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ZERO;
                Winks_StrgTmpTakeupDelete(pDownCC->CC_Idx);
                break;
            }
            else
            {
                retMsg.err_code = 0x00000000;
                
                pDownCC->img_idx++;/* 一个ID对应的多张图片，依次获取 */
                if(pDownCC->img_idx >= pDownCC->ND_img_cnt)/* 上一个彩像ID的几张图片全部下载完 */
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
                        /* 下载完成 */
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
            retMsg.err_state |= (unsigned long)(retMsg.err_state & 0xff000000)>>16;/*curstate=前一状态*/
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
            winks_EngRetMsg2UI(pSendMsg, SendMsgLen);/* msg_body有数据发送 */
        }
        else
        {
            Winks_printf("MSGTOUI: err_statr[0x%08x] err_code[0x%08x]\r\n", retMsg.err_state, retMsg.err_code);
            retMsg.err_code = Winks_Eng2UiMsgMap(&retMsg);
            winks_EngRetMsg2UI(&retMsg, sizeof(retMsg));
        }
    }
    /* 释放消息包资源 */
    if(pSendMsg != NULL)
    {
        Winks_salFree(pSendMsg);
        pSendMsg = NULL;
    }
    
    /*完成变更后复位子状态机*/
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
/*引擎单步运行控制*/
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
            /* CallInfo.CallTime最大65535秒(18小时),超过1天都按65535计 */
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

/* 引擎消息处理入口函数 */
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
                Winks_mem_set(&Winks_engineContext_p->setting.imsi_list, 0, sizeof(Winks_ImsiList_s));/* 保证进入未注册状态 */
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
                Winks_engineContext_p->SimStatus |= 0x0100;/* Sim卡1的Imsi已获取 */
            #endif /* WINKS_DUAL_SIM */
                if(winks_CheckFoundSim(buff, sizeof(buff)) == 1)/* 找到了已注册的Imsi,不必获取卡2的IMSI了 */
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
                    Winks_engineContext_p->SimStatus |= 0x0100;/* Sim卡1的Imsi已获取 */
                else
                    Winks_engineContext_p->SimStatus |= 0x0200;/* Sim卡2的Imsi已获取 */
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
            
            /* 找到了已注册的Imsi,引擎正常启动到已注册状态，不必获取另一卡的IMSI了 */
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
            /* 两张Sim卡的imsi都已获取到,但都不是已注册的imsi,引擎启动到未注册状态 -- 双卡可选择*/
            else if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0300)
            {
                char buff1[WINKS_IMSI_LEN+1];
                
                if(msg == WINKS_MSG_IMSI_REQ)/* 当前buff存放的是Sim卡1的imsi,取出先前已获取到的Sim卡2的imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    Winks_GetImsi_2(Winks_engineContext_p->engChn, buff1, sizeof(buff1));
                }
                else/* 当前buff存放的是Sim卡2的imsi,取出先前已获取到的Sim卡1的imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(1) == TRUE);
                    Winks_GetImsi(Winks_engineContext_p->engChn, buff1, sizeof(buff1));
                }
                winks_BootingWithSimCard(2,  buff, sizeof(buff), buff1, sizeof(buff1));
            }
            /* 已获取1张Sim卡的imsi,另一张Sim卡多试了5次仍没有获取到imsi,认为另一卡不存在,引擎启动到未注册状态 -- 单卡 */
            else if( (Winks_engineContext_p->SimStatus & 0xff00) &&
                     (Winks_engineContext_p->OtherSimRetryCnt >= 5) )
            {
                if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0100)/* Sim卡1的imsi已获取,读出该imsi */
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
            /* 定时反复读取imsi */
            else if(Winks_engineContext_p->getImsiCount++ < 0x7FFF)
            {
            #ifdef WINKS_DUAL_SIM
                if(Winks_engineContext_p->SimStatus & 0xff00)/* 已获取到1Sim卡的imsi,重试获取另一卡的imsi*/
                    Winks_engineContext_p->OtherSimRetryCnt++;
                else
                    Winks_engineContext_p->OtherSimRetryCnt = 0;
            #endif /* WINKS_DUAL_SIM */
                
                /*延时读取IMSI code*/
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
                /*测试功能开关*/
                Winks_SetOsalTestBitFlag(2, 0);/*debug打印开关*/
            #endif
            }
            
            break;
        }
        case WINKS_MSG_ENGINE_USER_REG:/*注册*/
        case WINKS_MSG_ENGINE_SNED_REG_SMS:/*发短信后查询注册成功与否*/
        case WINKS_MSG_ENGINE_UPDATA_FL:/*更新好友*/
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*上传同步好友录*/
#ifdef WINKS_DEL_PV2_0    
        case WINKS_MSG_ENGINE_SETTINGSET:/*设置*/
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
        case WINKS_MSG_CALL_REJECT:             /* 本地拒接 */
        case WINKS_MSG_CALL_REMOTE_REJECT:      /* 对方拒接 */
        case WINKS_MSG_CALL_ABORT:              /* 本地取消 */    
        case WINKS_MSG_CALL_REMOTE_ABORT:       /* 对方取消 */
        case WINKS_MSG_CALL_HANGUP:             /* 本地挂断 */
        case WINKS_MSG_CALL_ENDALL:             /* 挂断ALL  */      
        case WINKS_MSG_CALL_REMOTE_HANGUP:      /* 对方挂断 */
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
                /*状态回滚*/
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
        case WINKS_MSG_CALL_REJECT:             /* 本地拒接 */
        case WINKS_MSG_CALL_REMOTE_REJECT:      /* 对方拒接 */
        case WINKS_MSG_CALL_ABORT:              /* 本地取消 */    
        case WINKS_MSG_CALL_REMOTE_ABORT:       /* 对方取消 */
        case WINKS_MSG_CALL_HANGUP:             /* 本地挂断 */
        case WINKS_MSG_CALL_ENDALL:             /* 挂断ALL  */      
        case WINKS_MSG_CALL_REMOTE_HANGUP:      /* 对方挂断 */
            winks_EngRecordCallEvt();
            break;
        
        case WINKS_MSG_ENGINE_USER_REG:/*注册*/
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
                
                /*连接*/
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
        case WINKS_MSG_ENGINE_SNED_REG_SMS:/*发短信后查询注册成功与否*/
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
            
        case WINKS_MSG_ENGINE_UPDATA_FL:/*更新好友*/
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*上传同步好友录*/
#ifdef WINKS_DEL_PV2_0    
        case WINKS_MSG_ENGINE_SETTINGSET:/*设置*/
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
        case WINKS_MSG_CALL_REJECT:             /* 本地拒接 */
        case WINKS_MSG_CALL_REMOTE_REJECT:      /* 对方拒接 */
        case WINKS_MSG_CALL_ABORT:              /* 本地取消 */    
        case WINKS_MSG_CALL_REMOTE_ABORT:       /* 对方取消 */
        case WINKS_MSG_CALL_HANGUP:             /* 本地挂断 */
        case WINKS_MSG_CALL_ENDALL:             /* 挂断ALL  */      
        case WINKS_MSG_CALL_REMOTE_HANGUP:      /* 对方挂断 */
            winks_EngRecruitDownload();
            winks_EngRecordCallEvt();
            break;
        case WINKS_MSG_CALL_ANSWER:             /* 本地接听 */    
        case WINKS_MSG_CALL_REMOTE_ANSWER:      /* 对方接听 */  
            break;
        case WINKS_MSG_ENGINE_PUSH:/*连网事件*/
        {
            /*连接*/
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)
            {
            #if 0 //  TODO: modify
                if(winks_IsUsbState() == 1)
                    break;
                Winks_engineContext_p->subState = WINKS_ENGINE_SUBSTATE_ONE;
                Winks_engineContext_p->IsPeriodUpdating = 1;
                
                iRet = Winks_check_storage_files(0);
                if(iRet != 0)/* 彩像文件被破坏 */
                {
                    if(iRet == 3)
                    {
                        Winks_EngErr("系统文件被破坏\r\n");
                        Winks_CurStateRollback(1);
                        return winks_EngRetErr(WINKS_MSG_ENG2UI_SYS_FILES_ERR);
                    }
                    else
                    {
                        Winks_EngErr("彩像文件被破坏\r\n");
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
                /* 引擎忙，延时120秒再启动定时更新服务 */
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

        case WINKS_MSG_ENGINE_UPDATA_FL:/*更新好友*/
        {
            /*连接*/
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
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*上传同步好友录*/
        {
            if(Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_FOUR)
            {
                int iRet = 0;
              
                if((data == NULL) && (size != 0))/* (size != 0)按出错处理 */
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
                    
                    /*连接*/
                    iRet = winks_EngineSendDataToServer(
                        Winks_engineContext_p->curState, 
                        Winks_engineContext_p->subState, 
                        data);
                }
                /*释放好友列表*/
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
            /*连接*/
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
            /*连接*/
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
        
        case WINKS_MSG_ENGINE_USER_REG:/*注册*/
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
                /*连接*/
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
        
        case WINKS_MSG_ENGINE_SELECT_FRIEND_LIST:/*上传同步好友录*/
        case WINKS_MSG_ENGINE_SNED_REG_SMS:/*发短信后查询注册成功与否*/
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

/*引擎初始化*/
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

/*引擎注销*/
int Winks_EngineDestroy(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
#if 0
    /*释放动态申请的数据*/
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


    /*释放引擎数据*/
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
/* 高字节低2位表示发现哪张卡，  b0--卡1; b1--卡2
   低字节表示当前激活的是哪张卡，1--卡1;  2--卡2
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
/* 当两张卡时,用户选择哪张卡激活 */
int Winks_UserSetActiveSim(int SimNo)
{
    if(Winks_SetActiveSim(SimNo) == WINKS_RETURN_SUCCESS)
    {
        /* 存储用户选择的相关信息 */
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

/*用户注册彩像业务，供UI使用。返回值:0成功,-1失败。*/
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
    if( ((Winks_engineContext_p->SimStatus & 0x0300) == 0x0300) &&  /* 发现两张卡 */
        (!Winks_GetActiveSim()) )/* 两张非注册的Sim卡需要用户先选择用哪张 */
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
    /* 未注册状态:如果磁盘空间不足,不联网 */
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
    if(Winks_GetMaxCountOfFriendList((unsigned long*)&FR_Max))/*好友录最大可选好友数*/
        FR_Max = 0;
    Winks_printf("调试察看FR_Max=%d \r\n", FR_Max);
}
#endif
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_USER_REG, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);

    return iRet;
}

/*用户确认注册，发送注册短信息。返回值:0成功 其他失败。
reg_flag: 0取消注册，1发送注册*/
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

/*用户更新好友列录*/
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
    if(Winks_GetMaxCountOfFriendList((unsigned long*)&FR_Max))/*好友录最大可选好友数*/
        FR_Max = 0;
    Winks_printf("调试察看FR_Max=%d \r\n", FR_Max);
}
#endif
    iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_UPDATA_FL, NULL, 0);
    Winks_assert(iRet ==  WINKS_RETURN_SUCCESS);
    
    return iRet;
#else
    return Winks_UserUpdateFriend();
#endif
}

/*保存用户选择好友录，friend_list:用"|"分隔的号码.例如"13900000001|13900020002|"*/
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
        Winks_printf("上传空好友录: friend_list_len = %d len = %d friend_list = 0x%x \r\n", friend_list_len, len, friend_list);
        /* 发送空好友录 */
        iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_SELECT_FRIEND_LIST, NULL, 0);
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
        return 0;
    }

    if((unsigned long)len > friend_list_len) /* 输入号码串长度错误 */
    {
        Winks_printf("ERROR: friend_list_len = %d len = %d \r\n", friend_list_len, len);
        /*恢复引擎状态*/
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
    
    friend_list_len = len + 1;/* 校正字符串长度,应是包含结尾'\0' */
    
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

        /*保存好友录*/
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
    return Winks_PostMsgToUi(WINKS_MSG_ENGINE_UIRETMSG, &retMsg, sizeof(retMsg));/* msg_body有数据发送 */
}

/* 当手机空间不足且存储卡空间充足时询问用户是否使用存储卡,当选择使用存储卡时调用此函数 */
static int winks_SelMCard()
{
    int iRet = 0;
    int FL_Cnt = 0;
    int FR_Max = 0;
    
/* 判断是否使用卡作为存储介质 */
    int friend_cnt;
    Winks_FlashSpace_s PhoneSpace, CardSpace;
    
    if( (winks_GetStrgSize(&PhoneSpace, &CardSpace) != WINKS_RETURN_SUCCESS) || /* 获取系统flash容量失败 */
        (CardSpace.total == 0) )  /* 存储卡不存在 */
    {
        Winks_EngErr("winks_GetStrgSize\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_MCARD_NOT_EXIST);
        return WINKS_RETURN_FAILURE;
    }
    
    friend_cnt = Winks_get_friend_cnt();
    if(friend_cnt<0)
        friend_cnt = 0;
    
    FL_Cnt = Winks_GetFullFriendListCount();/*好友列表好友数*/
    FR_Max = winks_CalcFriendCCMax(CardSpace, FL_Cnt);
    if(FR_Max < friend_cnt) /* 存储卡的空闲空间不足以存放手机现有彩像 */
    {
        Winks_EngErr("(FR_Max < friend_cnt)\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
        return WINKS_RETURN_FAILURE;
    }
        
    /* 使用存储卡作为彩像存储介质 */
    /* 存储卡上创建彩像目录,再从手机上移动彩像到存储卡上 */
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
    /* 不足以存放所有好友列表彩像 */
    if((FR_Max>0)&&(FL_Cnt>FR_Max))
    {
        winks_EngPostMsg2UI_SelFriend();
    }

    /* 选定上传好友录 */
    winks_EngSelAllFriendList();
    
    return WINKS_RETURN_SUCCESS;
}

/* 当手机空间不足且存储卡空间充足时询问用户是否使用存储卡,当选择不使用存储卡时调用此函数 */
static int winks_UnselMCard()
{
    int FL_Cnt = 0;
    int FR_Max = 0;
    
/* 判断是否使用卡作为存储介质 */
    Winks_FlashSpace_s PhoneSpace, CardSpace;
    
    if( (winks_GetStrgSize(&PhoneSpace, &CardSpace) != WINKS_RETURN_SUCCESS) || /* 获取系统flash容量失败 */
        (CardSpace.total == 0) )  /* 存储卡不存在 */
    {
        Winks_EngErr("winks_GetStrgSize\r\n");
        winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_MCARD_NOT_EXIST);
        return WINKS_RETURN_FAILURE;
    }
    
    FL_Cnt = Winks_GetFullFriendListCount();/*好友列表好友数*/
    FR_Max = winks_CalcFriendCCMax(PhoneSpace, FL_Cnt);
    if(FR_Max < 1)
     {
         Winks_EngErr("winks_UnselMCard()\r\n");
         winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_STORAGE_LIMIT);
         return WINKS_RETURN_FAILURE;
     }
    
    if((FL_Cnt>0) && (FL_Cnt<=FR_Max))/* 好友列表的好友数小于手机可容纳最大好友数,全部加入好友录 */
    {
        winks_EngSelAllFriendList();
    }
    else if((FR_Max>0)&&(FL_Cnt>FR_Max))/*好友列表超过了手机可容纳最多好友数*/
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

/*用户DIY设置:上传服务器*/
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
/*用户个性化设置: 同步服务器*/
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
    if( self_info->ccSwitch == old_self_info.ccSwitch )/* "对方展示我的彩像" 没有修改,不用上传服务器 */
    {
        if( self_info->ShowCC != old_self_info.ShowCC )/* "我展示对方的彩像" 有修改,本地保存 */
        {
            if(self_info->ShowCC)
                value |= E_I_SHOW_OTHER;
            else
                value &= ~E_I_SHOW_OTHER;
            Winks_set_setting_value(E_SETTING_SHOW_CTRL, value);
        }
        /* 给UI发送完成消息 */
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
    
    if(strcmp(pNo, WINKS_DFT_CC_NO) == 0)/* 默认彩像 */
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

/*设置开机不提示用户注册。*/
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

/* 关闭彩像 */
int Winks_UserCloseWinkCC(void)
{
    int iRet = 0;
#if 0
    Winks_EngineToUiReturnMsg retMsg;
    Winks_SelfSetting_s self_info;
#endif
    unsigned long value;
    iRet = Winks_get_setting_value(E_SETTING_SHOW_CTRL, &value);
    if((iRet!=WINKS_RETURN_FAILURE) && (value==E_CC_SHOW_ON))/* 现在为开启状态，关闭 */
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
    /* 给UI发送完成消息 */
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

/* 开启彩像 */
int Winks_UserOpenWinkCC(void)
{
    int iRet = 0;
#if 0
    Winks_EngineToUiReturnMsg retMsg;
    Winks_SelfSetting_s self_info;
#endif
    unsigned long value;
    iRet = Winks_get_setting_value(E_SETTING_SHOW_CTRL, &value);
    if((iRet!=WINKS_RETURN_FAILURE) && (value==E_CC_SHOW_OFF))/* 现在为关闭状态，开启 */
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
    /* 给UI发送完成消息 */
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

/*取当前接入点*/
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
    /* 选择排序 */
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

    /* 第一遍扫描计算个数 */
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

    /* 分别对3个子数组排序 */
    wk_sort_ascii_string(temp, cnt1);
    wk_sort_ascii_string(temp + cnt1, cnt2);
    wk_sort_unicode_string(temp + cnt1 + cnt2, cnt3);

    Winks_mem_cpy(array, temp, count * sizeof(wk_SortItem));

    Winks_salFree(temp);
}


/*
*   排序好友列表。
*   NOTE: 要求array中的number与name是unicode编码。
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

        if (WK_STRLEN(array[i].name) == 0) /*  如果没有名称*/
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
                    temp1[i].flag = 2;      /* 拼音 */

                    /* 仅仅读取拼音, 过滤汉字 */
                    ucs_len = WK_STRLEN(py_buf1) - WK_STRLEN(array[i].name);
                    Winks_mem_cpy(py_buf2, py_buf1, ucs_len * 2);

                    Winks_Unicode2AsciiString(py_buf2, temp1[i].data, WINKS_FRIEND_NO_LEN);
                }
                else
                {
                    temp1[i].flag = 3;      /* 比如标点符号等特殊字符 */
                    Winks_mem_cpy(temp1[i].data, array[i].name, (WK_STRLEN(array[i].name) + 1) * 2);
                }
            }
        }
    }

    /* 对目标排序 */
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

/* 如定时更新服务未启动,则启动定时更新服务;
   如定时更新服务已启动,先检查定时服务周期是否与服务器下发的一致，若不一致则重启定时器 */
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
    Winks_GetSupportedConnectWays(&CWOp);/* 服务器支持的方式和默认的方式 */
    
    if((CWOp.twuTimeSlot < WINKS_TIME_CONNECT_PERIOD_MIN) || (CWOp.twuTimeSlot > WINKS_TIME_CONNECT_PERIOD_MAX))
        CWOp.twuTimeSlot = WINKS_TIME_CONNECT_PERIOD_DEF;
    
#ifdef WINKS_SIMULATOR     /* renzhan add for debug */
    CWOp.twuTimeSlot = 60;
#endif

    /* 检查是否需要重启定时器 */
    if(Winks_engineContext_p->PeriodPushTimer)/* 定时更新服务已启动 */
    {
        winks_TimerBody_s timerInfo;
        
        Winks_mem_set(&timerInfo, 0, sizeof(timerInfo));
        if( (Winks_GetTimerInfo(Winks_engineContext_p->PeriodPushTimer, &timerInfo) == WINKS_RETURN_SUCCESS) &&
            (CWOp.operation&CONNECT_WAY_TIMER_WAKE_UP) &&   /* 服务器支持定时更新 */
            (timerInfo.timeOutPeriod == WINKS_SECONDS2TICKS(CWOp.twuTimeSlot))) /* 定时服务周期与服务器下发的一致 */
        {
            Winks_printf("winks_StartPeriodPushTimer[%d] already ok! CWOp[0x%08x, %d] \r\n",
                Winks_engineContext_p->PeriodPushTimer, CWOp.operation, CWOp.twuTimeSlot );
            return iRet;/* 直接返回不用更新定时器 */
        }
    }
    
    Winks_GetPhoneConnectWay(&ClientConnectWay);/* 用户选择的方式 */
    /* 用户没有选择连接方式或选择的方式不是服务器支持的,则按服务器默认的方式判断；
       如果选择的方式是服务器支持的，则按用户选择的方式判断 */
    if( (ClientConnectWay == 0) || /* 用户没有选择连接方式 */
        (((CWOp.operation&0xffff)&ClientConnectWay) != ClientConnectWay) )/* 用户选择的方式不是服务器支持的 */
    {
        /* 按服务器默认的方式判断 */
        IsPeriodWakeMode = (CWOp.operation>>16)&CONNECT_WAY_TIMER_WAKE_UP;
    }
    else
    {
        /* 按用户选择的方式判断 */
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
            Winks_printf("winks_StartPeriodPushTimer[%d] 连接方式[0x%08x & 0x%04x] TimeSlot=%d \r\n",
                Winks_engineContext_p->PeriodPushTimer, CWOp.operation, ClientConnectWay, CWOp.twuTimeSlot );
        }
    }
    else
    {
        if(!(CWOp.operation&CONNECT_WAY_TIMER_WAKE_UP))/* 服务器不支持定时更新 */
        {
            /* 停止定时更新服务 */
            winks_StopPeriodPushTimer(3);
        }
    }
    
    return iRet;
}

/* 引擎忙，延时120秒再启动定时更新服务 */
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
            (timerInfo.timeOutPeriod == WINKS_SECONDS2TICKS(WINKS_PERIODTIMER_RETRY_SLOT)) )/* 定时服务周期与服务器下发的一致 */
        {
            Winks_printf("winks_DelayPeriodPushTimer[%d] already Delay:%d秒 state[%d][%d] \r\n", 
                Winks_engineContext_p->PeriodPushTimer, WINKS_PERIODTIMER_RETRY_SLOT,
                Winks_engineContext_p->curState, Winks_engineContext_p->subState);
            return iRet;/* 直接返回不用更新定时器 */
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
        Winks_printf("winks_DelayPeriodPushTimer[%d] Delay:%d秒 state[%d][%d] \r\n", 
            Winks_engineContext_p->PeriodPushTimer, WINKS_PERIODTIMER_RETRY_SLOT,
            Winks_engineContext_p->curState, Winks_engineContext_p->subState);
    }

    return iRet;
}

/* 停止定时更新服务 */
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

/* 用户连接方式设置: Push方式 或 定时更新 (ConnWay&CONNECT_WAY_TIMER_WAKE_UP) 
   如果服务器支持Push方式，用户可手动启动或关闭定时更新服务 */
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

    if(ConnWay&CONNECT_WAY_TIMER_WAKE_UP)/* 定时唤醒方式 */
    {
        /* 启动定时更新服务 */
        iRet = winks_StartPeriodPushTimer();
    }
    else
    {
        /* 停止定时更新服务 */
        iRet = winks_StopPeriodPushTimer(1);
    }
    
    return iRet;
}

/*停止引擎*/
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
        case WINKS_ENGINE_SUBSTATE_FOUR:    /*查询是否注册2(3短周期+3长周期)*/
        case WINKS_ENGINE_SUBSTATE_FIVE:    /*请求基本信息*/
        case WINKS_ENGINE_SUBSTATE_SIX:     /*下载片头运营商彩像*/
            /*后台执行初始化,状态不改变*/
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

#if 0 /*联调*/
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
    FriendCount = Cnt+6; /* 测试: 取6个好友作为好友录 */
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

/*定时通讯*/
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
    case 16:    /* 7FF0H 获取好友列表*/
        {
        iRet = Winks_UserResetFriendList();
        }
        break;
    case 17:    /* 7FEFH 上传好友录 + 7FEBH 获取好友信息 */
        {
        iRet = Winks_UpdateFriendRecord();
        }
        break;
    case 19:    /* 7FEDH 定时通讯 */
        {
        iRet = Winks_PeriodPushServer();
        }
        break;
    case 20:    /* 7FECH 初始化 */
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

/* 申请或返回已申请的解析数据需要的临时存储空间 */
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

/* 从HTTP缓冲中读取数据readlen字节长度数据到指定pbuf(最多容纳bufsize,所以要求(bufsize >= readlen)), */
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
    if((HttpInfo.statuscode >= HTTP_OK) && (HttpInfo.statuscode < HTTP_MultipleChoices))/* [200,300) 数据OK */
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
        if(HttpInfo.statuscode >= HTTP_BadRequest) /* >=400 连接错误或服务器错误 */
        {
            winks_EngAnalyzeErr(WINKS_MSG_ENG2UI_SVR_NO_ANSWER);
            return WINKS_RETURN_FAILURE;
        }
        else/* [300,400) 重定向 */
        {
            return WINKS_RETURN_FAILURE;
        }
    #else
        /* 具体分析错误 */
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
	#ifdef WINKS_SIMULATOR/* 天宇手机stack溢出 */
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
    if(dsa_head.sequence_num!=Winks_engineContext_p->ProtocolFrameNo)/* 桢序错 */
    {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
            return WINKS_RETURN_SUCCESS;
    }
#endif
    if(dsa_head.msg_type!=0xffff)
    {
        if((Winks_engineContext_p->ProtocolMsgType^dsa_head.msg_type)!= 0x8000)/* 协议号错 */
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
        (Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ONE) )/* 开始注册 */
    {
        int iRet = 0;
    #if 0//def WINKS_DEBUG
        Winks_FlashSpace_s flashInfo;
    #endif
        /*有可能上次未注册完，已下载一部分有用信息，这次删掉需要重新下载，浪费带宽和时间，但这个时刻删不会出错*/
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
        case 0xFFFF:        /* 出错啦 */
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

        /*出错返回*/
        if(*ret_code != WINKS_MSG_ENG2UI_NONE)
        {
            if(analyzeData != NULL)/* 出错需要释放临时数据 */
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
        case 0xFFEC:/*定时更新*/
        case 0xFFED:/*初始化*/
        {
            Winks_EngTimeFlag_s *pNewTimeFlag = (Winks_EngTimeFlag_s *)analyzeData;
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 0 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 1 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 2 );
            Winks_SetTimeFlag( pNewTimeFlag->TimeFlag, strlen(pNewTimeFlag->TimeFlag), 3 );
            break;
        }
        case 0xFFEB:/*下载好友信息*/
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

/* 第2/3层结构的通用解析函数 */
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
    return (WINKS_ENG_DS_LT_LEN + msgLen);      /* 返回整个字段长度 */
}

/* 对于协议中的3层结构, 通用的第2层数组解析函数 */
static int winks_EngDsaDo_M_array(int http, unsigned long offset, unsigned short msgType, unsigned long len, unsigned long *ret_code)
{
    void *ret_data = NULL;      /* NOTE: 在parseFunc()中申请 */
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

    /* 查表搜索对应的解析函数 */
    for (i=0; i<WINKS_ARRAY_SIZE(table); ++i)
    {
        if (msgType == table[i].msgType)
        {
            body= table[i];
            break;
        }
    }
    /* 如果没有找到对应的解析函数 */
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

    /* 处理解析后的返回数据 */
    if (body.retData)
    {
        body.retData(ret_code, ret_data);
        if(*ret_code != 0UL)
        {
            goto FREE_and_RET;
        }
    }
    
FREE_and_RET:
    /* 释放 parseFunc()中申请的内存*/
    if (ret_data != NULL)
    {
        Winks_salFree(ret_data);
    }

    return 0;
}

static void winks_EngProtocolUnsupported(unsigned short AttrId, unsigned short SeqNo)
{
    Winks_printf("UUUU 协议不支持属性 -- [%04x] [%02x] \r\n", AttrId, SeqNo);
}

static int winks_EngDsaDo_HFFF8(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
    /*
    配置服务器回复通信服务器地址信息属性            ( FFF8H )
    1)  IP属性  ( 01H )
    2)  PORT属性    ( 02H )
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
     ***    对于需要返回数据的, 在处理数组第1个元素时动态申请。
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
    
    if (p_server->cnt >= WINKS_SERVER_CNT)      /* 本地最多保存3组服务器"ip+port" */
    {
        return 0;
    }

    /* 服务器下发的字符串不以0结尾, 使用length */
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
    3.1.17 Feature 客户端与通信服务器的获取指定块好友信息交互过程

    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                 值为：7FEB H 
    2)  本机电话号码属性                        （0008H）
    3)  分块下传或请求好友信息属性          （8008H）
    4)  手机支持的名片信息属性               (002BH) 

    2.  通讯服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                 值为：FFEBH
    2)  用户名片信息属性                    （FFFFH）（如有更新则返回此属性）
    3)  用户好友彩像ID信息属性              （FFFCH）（如有更新则返回此属性）
    4)  分块下传或请求好友信息属性      （8008H）
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
    case 0x0020:/*时间戳*/
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
        buff[19] = 0;/*时间搓19位*/
        
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
    3.1.15 Feature 客户端与通信服务器初始化交互过程

    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                     值为：7FECH 
    2)  本机电话号码属性            （0008H）
    3)  客户端配置信息              （8000H）
    4)  初始化用户电话号码本属性（0011H）
    5)  用户手机空间大小            （0025H）

    2.  通讯服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                     值为：FFEC H
    2)  运营商提供终端连接方式属性  （8005H）
    3)  片头播放标志属性                （001BH）
    4)  紧急号码串属性                  （002AH）
    5)  片头彩像信息属性                （FFFBH）
    6)  运营商彩像信息属性              （FFFAH）
    7)  更新时间戳                          （0020H）
    8)  放弃终端用户时间                （0024H）（支持push终端使用）
    9)  用户好友关系更新标志属性    （002CH）
    10) 手机预留空间信息属性            （8009H）
*/

    int ret = 0;
    unsigned long push_gu = 0;  /* push 放弃时间 */
    char buf[WINKS_EMERGENCY_NO_MAX_LEN];
    Winks_ConnectWayOperation_s cwo;

    buf[0] = 0;
    *ret_code = 0;

    switch(msgType)
    {

    /* //该版本不区分片头播放标志标志，有片头就播放，没有则不播放。
    case 0x001B:
        if(winks_EngReadHttpBuf(http, offset, buf, 1, msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        if((buf[0] == '\1') || (buf[0] == '\2'))
            ret += Winks_SetPriorColorCallPlaySwitch(buf[0]-1);
        break;*/
    case 0x0020:/*时间戳*/
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
        buf[19] = 0;/*时间搓19位*/
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
    /*  //紧急号码不由服务器控制，终端认为3位的号码位紧急号码。
    case 0x002A:
        if(winks_EngReadHttpBuf(http, offset, (char *)buf, sizeof(buf-1), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        ret += Winks_SetEmergencyNumber(buf, MAX(msgLen,sizeof(buf-1)));
        break;*/
    /*  // 3.15中不需要处理此消息属性 ,激活流程默认更新好友。
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

    case 0xFFFA:        /* 运营商彩像信息属性*/
    case 0xFFFB:        /* 片头彩像信息属性*/
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
    3.1.19 Feature客户端与通讯服务器的定时通讯过程：
    （同步全部系统更改信息和用户选定好友的更改信息）

    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                 值为：7FEDH
    2)  本机电话号码属性            （0008H）
    3)  更新时间戳                      （0020H）
    4)    手机支持的名片信息属性       (002BH)
    5)  客户端电话本号码属性        （FFFDH）

    2.  通讯服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                 值为：FFEDH
    2)  运营商提供终端连接方式属性  （8005H）（如有更新则返回此属性）
    3)  片头播放标志属性                （001BH）（如有更新则返回此属性）
    4)  紧急号码串属性                  （002AH）（如有更新则返回此属性）
    5)  用户名片信息属性                （FFFFH） （用户已选择保存的好友和最新更新的好友的名片信息）
    6)  用户好友彩像ID信息属性          （FFFCH） （用户已选择保存的好友和最新更新的好友的彩像ID信息）
    7)  片头彩像信息属性                （FFFBH） （如有更新则返回此属性）
    8)  运营商彩像信息属性              （FFFAH） （如有更新则返回此属性）
    9)  更新时间戳                          （0020H）
    10)    上传手机统计信息请求属性     (0027H) （如有需要则返回此属性）
    11)    用户好友关系更新标志属性    （002CH）(有新增好友列表就下发此标志 )
*/

    int ret = 0;

    char buf[32];
    unsigned char update_flag = 0;      /* 好友更新标志 */
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
    /* //该版本不区分片头播放标志标志，有片头就播放，没有则不播放。
    case 0x001B:
        if(winks_EngReadHttpBuf(http, offset, buf, sizeof(buf), msgLen, ret_code) != WINKS_RETURN_SUCCESS)
            return 0;
        if((buf[0] == '\1') || (buf[0] == '\2'))
            ret += Winks_SetPriorColorCallPlaySwitch(buf[0]-1);
        break;*/

    case 0x0020:/*时间戳*/
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

    /*  //上传手机统计信息标志, 目前服务器不支持该功能。
    case 0x0027:
        Winks_printf("ERROR: 功能未实现\r\n");
        break;*/
    /*  //紧急号码不由服务器控制，终端认为3位的号码位紧急号码。
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
        if(pPeriodComInfo)/*如需要更新，则将pPeriodComInfo->FA_UpdFlag置1,之后的代码只识别1.*/
        {
            Winks_printf("****好友关系变动:%d\r\n", update_flag);
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
    3.1.18 Feature 客户端与通信服务器的通知服务器新修改的好友信息交互过程

    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                     值为：7FEFH 
    2)  本机电话号码属性                            （0008H）
    3)  Feature新修改本地保存的好友信息属性 （FFF9H）

    2.  通讯服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                     值为：FFEFH
    2)  分块下传或请求好友信息属性              （8008H）

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
    3.1.16 Feature 客户端与通信服务器的获取全部好友录好友信息交互过程

    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                 值为：7FF0H 
    2)  本机电话号码属性                            （0008H）

    2.  通讯服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                 值为：FFF0H
    2)  用户好友列表号码信息属性                (002DH)
    3)    更新时间戳                    （0020H）
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
    case 0x0020:/*时间戳*/
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
        buff[19] = 0;/*时间搓19位*/
        
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
    客户端与服务器之间的简单交互过程
    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                 值为：7FF9 H
    2)  IMSI号属性              （000FH）（JAVA终端无此属性）
    3)  本机电话号码            （0008H）（不能获取终端专用）

    2.  配置服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                 值为：FFF9H
    2)  配置服务器回复通信服务器地址信息属性（FFF8H）
    3)  平台短信的特服号码      （0010H）
    4)  发送短信短周期时间属性  （0018H）
    5)  发送短信长周期时间属性  （0019H）
    6)  客户端监听平台短信时使用的端口属性(001AH)
    */
    int iRet = 0;

    switch(msgType)
    {
    case 0xFFF8:
    {
        winks_EngDsaDo_M_array(http, offset, msgType, msgLen, ret_code);
        break;
    }

    case 0x0010:/*平台短信网关*/
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
        if(msgType == 0x0018)/*连网短周期*/
        {
            Winks_printf("Huaxm::short connect time[%d]\n ",time);
            if((time<WINKS_SMS_SHORT_PRIEOD_MIN) || (time>WINKS_SMS_SHORT_PRIEOD_MAX))/* 服务器下发值超限 */
            {
                if((Winks_get_setting_value(E_SETTING_SHORT_CYCLE, (unsigned long *)&time) != WINKS_RETURN_SUCCESS) || /* 本地没有保存值 */
                    (((time<WINKS_SMS_SHORT_PRIEOD_MIN) || (time>WINKS_SMS_SHORT_PRIEOD_MAX))) )/* 本地保存值也超限 */
                    time = WINKS_SMS_SHORT_PRIEOD_DFT;
                Winks_printf("ERROR(L%4d): ServerData -- 连网短周期 time=%d\r\n", __LINE__, time);
            }
            winks_EngSetQueryShortTime((unsigned long)time);
        }
        else /* 连网长周期 */
        {
            Winks_printf("Huaxm::long connect time[%d]\n ",time);
            if((time<WINKS_SMS_LONG_PRIEOD_MIN) || (time>WINKS_SMS_LONG_PRIEOD_MAX))/* 服务器下发值超限 */
            {
                if((Winks_get_setting_value(E_SETTING_LONG_CYCLE, (unsigned long *)&time) != WINKS_RETURN_SUCCESS) || /* 本地没有保存值 */
                    (((time<WINKS_SMS_LONG_PRIEOD_MIN) || (time>WINKS_SMS_LONG_PRIEOD_MAX))) )/* 本地保存值也超限 */
                    time = WINKS_SMS_LONG_PRIEOD_DFT;
                Winks_printf("ERROR(L%4d): ServerData -- 连网长周期 time=%d\r\n", __LINE__, time);
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
    3.1.2客户端向通讯服务器查询是否注册过程：

    1.  客户端：
    向平台发送使用到的消息属性：
    1)  MessageType属性                 值为：7FFD H
    2)  IMSI号属性              （000FH）（JAVA终端无此属性）
    3)  终端软件语言属性        （0021H）
    4)  本机电话号码            （0008H）（JAVA终端专用）

    2.  通讯服务器端：
    向客户端发送使用到的消息属性：
    1)  MessageType属性                 值为：FFFD H
    2)  本机电话号码              （0008H）
    3)  用户注册提示语            （0022H）
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
    3.  出错时服务器的返回中使用的错误属性：
    1)  MessageType属性                 值为：FFFFH
    2)  Error属性（ 0005H ）：  根据具体情况返回错误
    值长度两个字节
    客户端相关错误代码范围：        (0001H  ---  0FFFH)
    配置服务器相关错误代码：        (1001H  ---  1FFFH)
    通讯服务器相关错误代码：        (2001H  ---  2FFFH)
    数据库相关错误代码：            (4001H  ---  4FFFH)
    其他错误：                  (5001H  ---  FFFFH)

    1)  客户端相关错误
    a)  未知错误 （ 0001H ）
    2)  配置服务器相关错误 
    a)  配置服务器内部错误    （ 1002H ）
    b)  无有效的通信服务器地址（ 1003H ）
    c)  无有效的短信特服号码  （ 1004H ）
    3)  通信服务器相关错误
    a)  通信服务器内部错误 （ 2001H ）
    4)  其他错误
    a)  协议解析错误                    （1001H）
    b)  客户没有定制信息                    （4001H）
    c)  客户没有匹配当前时间的定制彩像  （4002H）
    d)  无适合此型号客户端的彩像            （4003H）
    e)  彩像文件未找到                  （4004H）
    f)  用户被运营商锁定                    （4005H）
    g)  本机号码为空错误                    （5001H）
    h)  对方号码为空错误                    （5002H）
    i)  CRC校验错误                     （5003H）
    j)  上传文件错误                        （5004H）
    k)  上传文件信息处理错误                （5005H）
    l)  用户未注册                      （5006H）
    m)  用户超过最大上传彩像数          （5007H）
    n)  服务器超过有效期停止服务错误        （5008H）
    o)  用户上传问候语修改失败          （5009H）
    p)  超过用户最大定制业务数错误      （500AH）
    q)  客户端电话本号码属性空错误      （500BH）
    r)  上传属性值错误                  （500CH）
    s)  无最新版本软件                  （500DH）
    t)  用户不是收费用户                    （500EH）
    u)  重新获取通讯服务器地址          （500FH）
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
    运营商提供终端连接方式属性（8005H）
    NOTE: 目前Feature Phone只支持短信push与定时唤醒两种方式。
*/
    unsigned long field_len = 0, end = 0;
    wink_DSA_StructInfo data_info = {0};

    unsigned char push = 0;     /* 短信push激活 */
    unsigned char wake = 0;     /* 定时唤醒 */
    unsigned char def_way = 0;  /* 默认连接方式 */

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
        case 0x04:      /* 短信push激活 */
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
            
        case 0x05:      /* 定时唤醒 */
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
        
        case 0x06:      /* 默认连接方式 */
        {
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&def_way, sizeof(def_way), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            if(def_way == 4/*PUSH更新*/ || def_way == 5/*定时唤醒*/)
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

        case 0x0A:      /* 定时唤醒方式定时通讯时间间隔 */
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

        case 0x0E:      /*定时唤醒方式网络连接超时时间*/
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

    /*避免不匹配*/
    Winks_ConnectWaysAdjust(cwo);
    Winks_printf("connect info: operation[%d] spTimeGiveUp[%d] twuTimeSlot[%d] twuTimeLimit[%d]\r\n",cwo->operation, cwo->spTimeGiveUp,cwo->twuTimeSlot, cwo->twuTimeLimit);
    return 0;
}

static int winks_EngDsaDo_M8008(int http, unsigned long offset, unsigned long len, unsigned long *ret_code)
{
/*
    分块下传或请求好友信息属性（8008H）

    1)  本次下传或请求第几块    （01H）：本次下传或请求的好友信息块的块号。
    2)  总块数                      （02H）：好友名片和彩像ID信息总块数。
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
                Winks_printf("ERROR(L%4d): ServerData -- 好友信息块号 iVal=%d\r\n", __LINE__, iVal);
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
                Winks_printf("ERROR(L%4d): ServerData -- 好友信息块数 iVal=%d\r\n", __LINE__, iVal);
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
    手机预留空间信息属性:
    1)  手机预存好友记录条数属性    （01H）
    2)  手机预存运营商和片头总个数  （02H）
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
    			UsrInfo.CCIdx = 0;/* 保留记录但删除彩像Idx */
    			iRet = Winks_StrgSetUserInfo(UsrIdx, &UsrInfo);
    		}
    		else
    		{
                Winks_mem_set(&UsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    			iRet = Winks_StrgSetUserInfo(UsrIdx, &UsrInfo);/* 删除记录 */
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
/*  运营商彩像信息属性

    1)  运营商彩像ID属性            （01H）
    2)  运营商彩像问候语属性    （02H）
    3)  操作类型属性                （03H）
    此属性用于标示平台对运营商彩像的操作。
    值长度1个字节
    值为： 
    增加或修改操作  ： 01H
    删除操作            ： 02H
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
        Winks_printf("++++增加运营商彩像:%s | %s\r\n", DownCCCxt.ND_ID,  grt);

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
        Winks_printf("---删除运营商彩像:%s | %s\r\n", DownCCCxt.ND_ID);
        if( winks_EngDeletePriBusCC(0, DownCCCxt.ND_ID) != WINKS_RETURN_SUCCESS )
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_CALL_FUNC);
        }
    }
    else
    {
        Winks_printf("ERROR: 服务器返回运营商基本信息错误 op=%d ID:%s grt:%s\r\n", op, DownCCCxt.ND_ID, grt);
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
    }

    return 0;
}

static int winks_EngDsaDo_HFFFB(int http, unsigned long index, unsigned long offset, unsigned long len, unsigned long *ret_code, void **ret_data)
{
/*
    片头彩像信息属性

    1)  彩像片头ID属性      （01H）
    2)  播放时间属性            （02H）
    3)  片头问候语属性      （03H）
    4)  操作类型属性            （04H）
        此属性用于标示平台对片头的操作。
        值长度1个字节
        值为： 
        增加或修改操作  ： 01H
        删除操作            ： 02H
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
        Winks_printf("++++增加片头彩像:%s | %d | %s\r\n", DownCCCxt.ND_ID, play_time, grt);

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
        Winks_printf("----删除片头彩像:%s | %s\r\n", DownCCCxt.ND_ID);
        if( winks_EngDeletePriBusCC(1, DownCCCxt.ND_ID) != WINKS_RETURN_SUCCESS )
        {
            winks_EngAnalyzeErr(WINKS_ENG_ERROR_CALL_FUNC);
        }
    }
    else
    {
        Winks_printf("ERROR: 服务器返回片头基本信息错误 op=%d ID:%s grt:%s\r\n", op, DownCCCxt.ND_ID, grt);
        winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
    }

    return 0;
}

#if 0
/* 判断电话号码是否在好友录中 */
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
    用户好友彩像ID信息属性
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
        case 0x01:      /* 好友号码属性 */
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
        case 0x02:      /* 操作类型属性 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x03:      /* 区分主被叫标志 */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&diff_flag, sizeof(diff_flag), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            break;
        case 0x04:      /* 好友为用户设置的主叫彩像ID */
            if(winks_EngReadHttpBuf(http, data_info.offset, DownCCCxt.ND_ID, sizeof(DownCCCxt.ND_ID)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            DownCCCxt.ND_ID[data_info.len] = 0;
            break;
        case 0x05:      /* 好友为用户设置的被叫彩像ID */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)ID_in, sizeof(ID_in)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            //ID_in[data_info.len] = 0;
            break;
        case 0x06:      /* 彩像的联网标志位属性 */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&nw_flag, sizeof(nw_flag), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            break;
        case 0x07:      /* 主叫彩像总张数属性 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, 4, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = 0;
            DownCCCxt.ND_img_cnt = (unsigned char)atol(buf);
            break;
        case 0x08:      /* 被叫彩像总张数属性 */
            //if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, sizeof(buf), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
            //    return 0;
            //buf[data_info.len] = 0;
            //ci_cnt = (unsigned long)atol(buf);
            break;
        case 0x09:      /* 彩像播放间隔时间属性 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buf, 5, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buf[data_info.len] = '\0';
            DownCCCxt.ND_interval = (unsigned char)atol(buf);
            if((DownCCCxt.ND_interval<WINKS_CC_PLAY_INTERVAL_MIN) || (DownCCCxt.ND_interval>WINKS_CC_PLAY_INTERVAL_MAX))
            {
                Winks_printf("ERROR(L%4d): ServerData -- 彩像播放间隔 interval=%d\r\n", __LINE__, DownCCCxt.ND_interval);
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
                Winks_printf("++++增加好友:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
                ret = winks_EngDealUserBaseInfo(&DownCCCxt, WINKS_DOWNCC_OP_WASHOUT);
            }
            
            break;
        }
    #ifdef WINKS_SUPPORT_3G
        case WINKS_ENGINE_APP_CALL_DOWNLOAD:
    #endif /* WINKS_SUPPORT_3G */
        case WINKS_ENGINE_APP_RECRUIT_DOWNLOAD:
            Winks_mem_cpy(&pDownCC->ND, &DownCCCxt.ND, sizeof(WinksStrgFile_NeedDown_s));
            Winks_printf("++++增加好友:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
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
                Winks_printf("++++增加好友:%s ID:%s %d\r\n", DownCCCxt.ND_NO, DownCCCxt.ND_ID, DownCCCxt.ND_img_cnt);
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
    用户名片信息属性
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
    /* winks_EngGetAnalyzeData中对ret_data其申请内存，然后赋给pbcard，在解析完数据自动释放 */
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
        case 0x01:      /* 好友号码 */
            if(winks_EngReadHttpBuf(http, data_info.offset, no, sizeof(no)-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            no[data_info.len] = 0;
            break;
        case 0x02:      /* 操作类型 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x03:      /* 是否公开随电名片 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&pub, sizeof(pub), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x04:      /* 用户状态属性 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&state, sizeof(state), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;

    #if 0   /* 第一版暂不支持随电名片功能 */
        case 0x05:      /* 用户状态文字属性 */
        case 0x06:      /* 姓 */
        case 0x07:      /* 名 */

        case 0x09:      /* 职务 */
        case 0x0A:      /* 公司名称 */
        case 0x0B:      /* 公司电话 */
        case 0x0C:      /* 国家或地区 */
        case 0x0D:      /* 省份或自治区 */
        case 0x0E:      /* 城市 */
        case 0x0F:      /* 详细地址 */
        case 0x10:      /* 邮编 */
        case 0x11:      /* 邮箱 */
        case 0x12:      /* 传真 */
        case 0x13:      /* 网址 */
        case 0x14:      /* 办公手机 */
            break;
        case 0x15:      /* 用户类型 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&user_type, sizeof(user_type), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
    #endif

        case 0x08:      /* 主叫时问候语 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)grt_out, WINKS_MAX_LEN_OF_WELCOME_INFO-1, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            grt_out[data_info.len] = 0;
            break;
        case 0x16:      /* 被叫时问候语 */
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

    if(no[0])/* 电话号码不能为空 */
    {
        switch (op)
        {
        case 1:     /* 增加或修改 */
            Winks_printf("<STRG>Winks_add_friend_bcard()\r\n");
          #if 0
            strcpy(bcard.friend_no, no);
            strcpy(bcard.grt_in, grt_in);
            strcpy(bcard.grt_out, grt_out);

            bcard.cc_flag = 1;
            bcard.pub_flag = pub;

            ret = Winks_add_friend_bcard(&bcard);   /* TODO: 合并变更*/
          #else
            strcpy(pbcard->friend_no, no);
            pbcard->cc_flag = 1;
            pbcard->pub_flag = pub;

            ret = Winks_add_friend_bcard(pbcard);   /* TODO: 合并变更*/
          #endif
            break;
        case 2:
            ret = Winks_delete_friend_bcard(no);
            break;
        case 3:     /* 屏蔽操作 unfinished */
            ret = Winks_set_cc_flag(no, 0);
            break;
        case 4:     /* 解除屏蔽操作 unfinished */
            ret = Winks_set_cc_flag(no, 1);
            break;
        default:
            break;

        }
    }
    else
    {
        Winks_printf("ERROR: 服务器返回电话号码为空 op=%d\r\n", op);
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
        case 0x01:      /* 好友号码 */
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
        case 0x02:      /* 操作类型 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)&op, sizeof(op), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x08:      /* 主叫时问候语 */
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
        Winks_printf("ERROR: 服务器返回电话号码为空 op=%d.\r\n", op);
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
    {       /*减少局部变量对stack的压力*/
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

/* 3.1.4客户端与通讯服务器的根据彩像ID获取彩像信息 */
static int winks_EngDsaDo_LFFFA(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
    2.  通讯服务器端：
        向客户端发送使用到的消息属性：
        1)  MessageType属性                 值为：FFFAH
        2)  下传彩像文件基本属性    （8001H）
        3)  下传彩像文件数据流      （0012H）
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
        2.  下传彩像文件基本属性                                （8001H）
        */
    end = offset+msgLen;
    while (offset < end)
        {
            /*
        2.  下传彩像文件基本属性                                （8001H）
        1)  下传彩像文件ID      （01H）：服务器下传彩像文件的ID。
        2)  下传彩像文件类型ID  （02H）：值为下表的Id字段值。
        3)  下传彩像文件扩展名  （03H）：服务器下传文件的文件扩展名（下表的EXTEND_NAME字段）。
        4)  下传彩像文件类型        （04H）
        值长度1个字节
        值为： 
        下传用户彩像文件：  01H；
        下传片头彩像文件：  02H；
        下传运营商彩像文件：03H；
        5)  当下传彩像为多张时，目前下传文件为总张数的第几张（05H）
        6)  下传彩像文件的大小  （06H）：服务器下传文件的大小（单位：字节）（通讯服务器使用）
        7)  下传彩像文件相对路径    （07H）：服务器下传文件在FTP服务器上的相对路径。（通讯服务器使用）
        8)  下传彩像文件是否在本域服务器    （08H）：不在本域时下列FTP属性生效。
        值长度1个字节
        值为： 
        在本域：  01H；
        不在本域：02H； 
        9)  下传彩像文件所在FTP地址     （09H）
        10) 下传彩像文件所在FTP用户名   （0AH）
        11) 下传彩像文件所在FTP密码     （0BH）
            */
        char buff[256];
        field_len= winks_EngDsaDo_struct(http, offset, ret_code, &data_info);
        Winks_assert(*ret_code == 0);

        switch (data_info.type)
        {
        case 0x01:  /* 服务器下传彩像文件的ID。 */
            if(winks_EngReadHttpBuf(http, data_info.offset, ID, sizeof(ID), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            ID[data_info.len] = '\0';
            if(strcmp(pDownCC->ND_ID, ID))
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
                return 0;
            }
            break;
        case 0x02:  /* 下传彩像文件类型ID */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buff[data_info.len]= '\0';
            pDownCC->img_type = (unsigned char)atoi(buff);
            break;
        case 0x03:  /* 下传彩像文件扩展名 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            buff[data_info.len]= '\0';
            break;
        case 0x04:  /* 下传彩像文件类型 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, 1/*sizeof(buff)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            cc_type = buff[0];
            
            if(pDownCC->ND_cc_type != cc_type)
            {
                winks_EngAnalyzeErr(WINKS_ENG_ERROR_SVR_RET);
                return 0;
            }
            break;
        case 0x05:  /* 当下传彩像为多张时，目前下传文件为总张数的第几张（ */
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
        case 0x06:  /* 下传彩像文件的大小 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, 4/*sizeof(buff)*/, data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            winks_GetDsaLong(&CCImgInfo_p->img_size, buff, 4, WINKS_ENG_DS_4BYTES_LEN);
            break;
        case 0x07:  /* 下传彩像文件相对路径 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x08:  /* 下传彩像文件是否在本域服务器 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x09:  /* 下传彩像文件所在FTP地址 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x0a:  /* 下传彩像文件所在FTP用户名 */
            if(winks_EngReadHttpBuf(http, data_info.offset, (char *)buff, sizeof(buff), data_info.len, ret_code) != WINKS_RETURN_SUCCESS)
                return 0;
            break;
        case 0x0b:  /* 下传彩像文件所在FTP密码 */
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

        /* 记录img_type */
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
        18. 彩像数据流（DataStream）（0012H）
        此属性值为下传彩像文件的数据流（可表示数据流最长为4G）
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

/* 3.1.10客户端和通信服务器端实时获取彩像交互过程（3G环境）：（JAVA和2.5G终端暂不支持此过程）*/
static int winks_EngDsaDo_LFFF8(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
2.  通讯服务器端：
向客户端发送使用到的消息属性：
1)  MessageType属性                 值为：FFF8H
2)  下传彩像文件基本属性        （8001H）
3)  用户好友彩像ID信息属性      （FFFCH）
4)  TimeStamp                   （0006H）
5)  用户名片信息属性            （FFFFH）
6)  新版客户端软件的URL信息属性（0016H）（如有新版软件返回此属性）
7)  实时下传多文件流属性        （8007H）
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
/* 3.1.9客户端与通信服务器关于实时更新用户状态信息*/
static int winks_EngDsaDo_LFFF7(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
2.  通讯服务器端：
向客户端发送使用到的消息属性：
1)  MessageType属性                 值为：FFF7H
2)  回复用户个人状态修改结果属性        （000CH）（根据需要返回）
3)  回复用户彩像服务开关修改结果属性    （000EH）（根据需要返回）
4)  回复是否公开随电名片修改结果属性    （0014H）（根据需要返回）
值为：
    更新成功 ：01H
    更新失败 ：02H
    */
    int iRet = 0;
    unsigned char buff[16];

    *ret_code = 0;

    switch(msgType)
    {
    case 0x000C:
    case 0x000E:/*回复用户彩像服务开关修改结果属性*/
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

/* 3.1.12客户端与通信服务器关于上传DIY文件信息交互*/
static int winks_EngDsaDo_LFFF6(unsigned short msgType, unsigned long msgLen, int http, unsigned long offset, unsigned long *ret_code, void **analyzeData)
{
    /*
2.  通讯服务器端：
向客户端发送使用到的消息属性：
1)  MessageType属性                 值为：FFF6H
2)  彩像文件分块传输成功块号      (0029H)
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
        /*返回上传的块号或文件号*/
        if(pULInfo->subTotalCount>1)
            CorrectIndex = (unsigned short)(pULInfo->subIndex+1);
        else
            CorrectIndex = (unsigned short)(pULInfo->curIndex+1);
        /* 返回不是上传的块号*/
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


/*申请大的缓存区(原来申请的缓存不够用)*/
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
            Winks_salFree(pSF->pSendBuf);/*将上次申请的缓存释放*/
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


/*恢复原始申请的缓存区*/
static int SendBufRestore(Winks_SendFrame_s *pSF)
{
    if(pSF->pSendBuf != pSF->baseBuff)
    {
        /*申请了大缓存将其释放*/
        Winks_salFree(pSF->pSendBuf);
    }
    pSF->pSendBuf = pSF->baseBuff;/*释放大缓存后恢复分配的原始缓存*/
    memset(pSF->pSendBuf,0,WINKS_SEND_FRAME_LEN);
    pSF->Len = WINKS_SEND_FRAME_LEN;
    pSF->Off = 0;

    return 0;
}


/*添加桢头*/
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
            NewLen += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*申请大缓存失败*/
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

/* 将pBuf的Size长度的数据追加到SF缓冲 */
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
            NewLen += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*申请大缓存失败*/
        {
            return WINKS_RETURN_FAILURE;
        }
    }
    
    memcpy(&pSF->pSendBuf[pSF->Off], pBuf, Size);
    pSF->Off += Size;
    
    return WINKS_RETURN_SUCCESS;
}

#if 1 //def WINKS_MTK
/* 从Unicode路径字符串中提取文件名 */
static int FindFileName_Unicode(char *pOutBuf, int OutLen, char *pUnicodeInBuf, int InLen)
{
    int retlen = 0;
    char * pStart = NULL;
    const char space[] = WINKS_PATH_SPACE;
    
    while ((InLen) && (!((*pUnicodeInBuf == 0) && (*(pUnicodeInBuf + 1) == 0))))
    {
        if((*(pUnicodeInBuf)==space[0]) && (*(pUnicodeInBuf+1)==0))/* 找到可能是文件名的开始位置，重置指针 */
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

    if(pStart == NULL)/* 未找到'\\' */
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
        if(TimeStamp[0] == 0)/* 没有记录对应的时间戳 */
            strcpy(TimeStamp,"2000-01-01 00:00:00");
        TimeStamp[19] = 0;
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)TimeStamp, strlen(TimeStamp));
        break;
    }
    case 0x0007:/* 呼叫类型属性 */
    {
        unsigned char CallType = 2;/* 1--主叫 2--被叫 */
        RetLen = AddBaseAttr(pSF, AttrId, &CallType, 1);
        break;
    }
    case 0x0008:/*本机电话号码*/
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
    case 0x0009:/* 对方电话号码 */
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
    
    case 0x0011:/*本机PB*/
    {
/*此属性用于终端上传用户的全部电话本号码信息."|"符号分割，"|"结尾。
例：13910712345|13691234567|13121900330|
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

        if(!phone_cnt && !sim_cnt)/* 电话簿为空 */
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
        while(cnt < phone_cnt + sim_cnt)/* 逐个添加电话号码到到发送缓存 */
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
                if((NumberLen <= 0) || (NumberLen >= WINKS_FRIEND_NO_LEN))/*无效的电话号码,忽略*/
                    continue;
            #if 0
                Winks_printf("cnt:%02d iNo%d: %s\r\n",cnt,iNo,PBItem.number[iNo]);
            #endif
                NumberLen += 1;
                if(buffOff + NumberLen >= sizeof(buff))/* buff满,移到发送桢缓冲中 */
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

        if(buffOff)/* buff中剩下的电话号码也添加到发送缓存 */
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
        
        /* 忽略日期 */
        if(Winks_fseek(fp, WINKS_DATETIME_STR_LEN, WINKS_SEEK_SET) != 0)
        {
            Winks_fclose(fp);
            winks_EngPrintFsErr("Winks_fseek", WINKS_STRG_PB_LOG_2, __LINE__);
            return 0;
        }

        /* 读电话号码个数 */
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
           
            if((readlen = Winks_fread(PBNoBuf, 1, WINKS_FRIEND_NO_LEN, fp)) != WINKS_FRIEND_NO_LEN)/* 读电话号码 */
                break;
            
            NumberLen = strlen(PBNoBuf);
            if((NumberLen <= 0) || (NumberLen >= WINKS_FRIEND_NO_LEN))/*无效的电话号码,忽略*/
            {
                cnt++;
                continue;
            }
          #if 0
            Winks_printf("cnt:%02d PBNo: %s\r\n",cnt, PBNoBuf);
          #endif
            NumberLen += 1;
            if(buffOff + NumberLen >= sizeof(buff))/* buff满,移到发送桢缓冲中 */
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
        if(buffOff)/* buff中剩下的电话号码也添加到发送缓存 */
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
    
    case 0x000F:/*IMSI号码*/
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
        13. 用户彩像服务开关属性（000DH）
        此消息用于客户端向通信服务器请求暂时关闭用户自己的彩像服务。关闭后其他任何人都将无法看到此用户的彩像信息。
        值长度1个字节
        值为：  01H : 用户开启彩像服务
        02H : 用户关闭彩像服务
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
        11. 用户个人状态属性（000BH） 
        客户端向通信服务器请求修改主叫个人状态属性。
        值长度1个字节
        值为：
        正常 ： 01H；
        离开 ： 02H；
        会议 ： 03H；
        忙碌 ： 04H；
        不便 ： 05H  …. 
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
        19. 是否公开随电名片属性（0013H）
        此消息用于客户端向通信服务器请求更改用户敏感信息是否公开属性。如果敏感信息不公开将无法显示用户的任何个人信息
        值长度1个字节
        值为：
        允许公开 ：     01H
        不允许公开为：  02H
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
        29. 用户状态文字属性（001FH）
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
    case 0x0015:/* 客户端软件版本信息属性 */
    {
        char *pVer = "V1.0";

        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)pVer, strlen(pVer));
        break;
    }
    case 0x0020:/*时间戳*/
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
            if(Tmp[0] == 0)/* 没有记录对应的时间戳 */
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
    
    case 0x0021:/*当前语言*/
    {
        char buff[20];

        Winks_mem_set(buff, 0, sizeof(buff));
        strncpy(buff, GetProtocolLanguage(), sizeof(buff));
        buff[sizeof(buff) - 1] = 0;
        
        RetLen = AddBaseAttr(pSF, AttrId, (unsigned char *)buff, strlen(buff));
        break;
    }
    
    case 0x0025:/*手机的存储空间*/
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
    case 0x002B:/*手机支持的名片信息属性*/
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
18. 彩像数据流（DataStream）（0012H）
此属性值为下传彩像文件的数据流（可表示数据流最长为4G）
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

        if(pULInfo->subTotalCount > 1)/*分多块*/
        {
            unsigned long Off;

            Winks_assert(pULInfo->curSubSize);
            if(pSF->Off+pULInfo->curSubSize+6 > pSF->Len)
            {
                int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLen <= (int)(pSF->Off+pULInfo->curSubSize+6)) NewLen += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
            if(SendBufReInit(pSF,NewLen) != NewLen)/*申请大缓存失败*/
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
        else/*不分块*/
        {
        unsigned long Len,Size;
            
            if(pSF->Off+pULInfo->curFileSize+6 > pSF->Len)
            {
                int NewLen = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLen <= (int)(pSF->Off+pULInfo->curFileSize+6)) NewLen += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
            if(SendBufReInit(pSF,NewLen) != NewLen)/*申请大缓存失败*/
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
            NewLen += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*申请大缓存失败*/
        {
            return 0;
        }
    }
    
    winks_SetDsaShort(&pSF->pSendBuf[SF_Off], 4, AttrId);
    pSF->Off += WINKS_ENG_DS_LT_LEN;
    RetLen += WINKS_ENG_DS_LT_LEN;
    
    switch(AttrId)
    {
    case 0x8000:/*客户端配置信息*/
    {
        /*1.    客户端配置信息属性      （8000H）
        1)  客户端型号信息ID属性        （01H）
        2)  手机软件类型属性            （02H）
        3)      手机屏幕高属性              （03H）
        4）  手机屏幕宽属性             （04H）
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
        3.  客户端请求彩像文件属性                              （8002H）
        1)  彩像文件ID      （01H） ：请求的彩像文件的ID。
        2)  请求彩像类型        （02H）
            值长度1个字节
            值为： 
            请求用户彩像文件：  01H；
            请求片头彩像文件：  02H；
            请求运营商彩像文件：03H；
        3)  总张数          （03H）：彩像总张数。（当下发彩像为DIY多张时使用）
        4)  请求第几张      （04H）：本次请求第几张图片。
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
    case 0x8008:/*分片请求好友信息属性*/
    {
        /*9.    分块下传或请求好友信息属性（8008H）
        1)  本次下传或请求第几块    （01H）：本次下传或请求的好友信息块的块号。
        2)  总块数              （02H） ：好友名片和彩像ID信息总块数。
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
        4.  上传DIY彩像文件基本属性                         （8003H）
        1)  用户号码                （01H） ：DIY针对的用户的号码。
        2)  用户问候语          （02H） ：DIY针对的用户的问候语。
        3)  彩像名称                （03H） ：彩像的名称（用于标识本彩像的名称）。
        4)  彩像尺寸                （04H） ：彩像文件的字节数。（通讯服务器使用）
        5)  彩像类型ID          （05H） ：值为下表的Id字段值。
        6)  总张数              （06H） ：总共上传几张DIY图片。
        7)  第几张              （07H） ：本次上传为第几张DIY图片。
        */
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        char * pfileName = NULL;
        char buff[16];

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);

        if(pULInfo->number[0])
        {
            iRet = AddBaseAttr(pSF, 0x01, (unsigned char *)pULInfo->number, strlen(pULInfo->number));/*用户号码*/
            if(iRet <= 0)
            {
            return iRet;
            }
            RetLen += iRet;
        }
        if(strlen(pULInfo->welcome))
        {
        iRet = AddBaseAttr(pSF, 0x02, (unsigned char *)pULInfo->welcome, strlen(pULInfo->welcome));/*用户问候语*/
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
            iRet = AddBaseAttr(pSF, 0x03, (unsigned char *)pfileName, strlen(pfileName));/*彩像名称*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #if 1
            snprintf(buff, sizeof(buff)-1, "%ld",pULInfo->curFileSize);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x04, (unsigned char *)buff, strlen(buff));/*彩像尺寸*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #endif
            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_type);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x05, (unsigned char *)buff, strlen(buff));/*彩像类型ID*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_count);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x06, (unsigned char *)buff, strlen(buff));/*总张数*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->curIndex + 1);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x07, (unsigned char *)buff, strlen(buff));/*第几张*/
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
        5.  修改用户问候语属性（8004H）
        1)  用户号码  （01H）   ：需要修改用户问候语的用户号码
        2)  用户问候语（02H）   ：需要修改的用户问候语

        注：
          如果用户号码为空，则为修改用户的默认问候语。
        */
        Winks_EngDiyComInfo_s *pULInfo = NULL;

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);

        if(pULInfo->number[0])
        {
            iRet = AddBaseAttr(pSF, 0x01, (unsigned char *)pULInfo->number, strlen(pULInfo->number));/*用户号码*/
            if(iRet <= 0)
            {
            return iRet;
            }
            RetLen += iRet;
        }
        if(pULInfo->welcome[0] == '\0')
            strcpy(pULInfo->welcome, " ");

        iRet = AddBaseAttr(pSF, 0x02, (unsigned char *)pULInfo->welcome, strlen(pULInfo->welcome));/*用户问候语*/
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
        7.  分块上传DIY大文件属性（8006H）
        1)  用户号码            （01H） ：DIY针对的用户的号码。
        2)  用户问候语          （02H） ：DIY针对的用户的问候语。
        3)  彩像名称            （03H） ：彩像的名称（用于标识本彩像的名称）。
        4)  彩像总尺寸          （04H） ：彩像文件的字节数。
        5)  本彩像的总块数      （05H）：彩像文件总的分块数。
        6)  本次上传块ID        （06H）：本次上传的文件块的块ID。
        7)  本次上传块大小      （07H） ：本次上传的彩像数据块的字节数
        8)  彩像类型ID          （08H） ：值为下表的Id字段值。
        9)  总张数              （09H） ：总共上传几张图片。
        10) 第几张              （0AH） ：本次上传为第几张图片。
        11) 客户彩像ID          （0BH） ：值为客户端生成的彩像ID，用于服务器定位具体的彩像文件。（目前java格式为手机号+3位随机数）
        */
            
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        char * pfileName = NULL;
        char buff[16];

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);

        if(pULInfo->number[0])
        {
            iRet = AddBaseAttr(pSF, 0x01, (unsigned char *)pULInfo->number, strlen(pULInfo->number));/*用户号码*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        }

        if(strlen(pULInfo->welcome))
        {
            iRet =  AddBaseAttr(pSF, 0x02, (unsigned char *)pULInfo->welcome, strlen(pULInfo->welcome));/*用户问候语*/
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
            iRet =  AddBaseAttr(pSF, 0x03, (unsigned char *)pfileName, strlen(pfileName));/*彩像名称*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #if 1
            snprintf(buff, sizeof(buff)-1, "%ld", pULInfo->curFileSize);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x04, (unsigned char *)buff, strlen(buff));/*彩像尺寸*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;
        #endif
            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->subTotalCount);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x05, (unsigned char *)buff, strlen(buff));/*本彩像的总块数*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->subIndex + 1);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x06, (unsigned char *)buff, strlen(buff));/*本次上传块ID*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%ld", pULInfo->curSubSize);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x07, (unsigned char *)buff, strlen(buff));/*本次上传块大小*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_type);
            buff[sizeof(buff)-1] = 0;
            iRet = AddBaseAttr(pSF, 0x08, (unsigned char *)buff, strlen(buff));/*彩像类型ID*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->image_count);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x09, (unsigned char *)buff, strlen(buff));/*总张数*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            snprintf(buff, sizeof(buff)-1, "%d", pULInfo->curIndex + 1);
            buff[sizeof(buff)-1] = 0;
            iRet =  AddBaseAttr(pSF, 0x0a, (unsigned char *)buff, strlen(buff));/*第几张*/
            if(iRet <= 0)
            {
                return iRet;
            }
            RetLen += iRet;

            /*iRet =  AddBaseAttr(pSF, 0x0b, pULInfo->, strlen());*//*客户彩像ID*/
        }
        break;
    }
    default:
        Winks_EngErr("AddL2Attr AttrId not found");
        return -1;
    }
    winks_SetDsaLong(&pSF->pSendBuf[SF_Off+2], 4, (RetLen - WINKS_ENG_DS_LT_LEN)/*去掉包装头的六字节*/);
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
            NewLen += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
        }
        if(SendBufReInit(pSF, NewLen) != NewLen)/*申请大缓存失败*/
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
            *pFriendRecord++ = '\0';/*将'|'改为'\0'提取出1个号码，好友录指针指向下个号码*/
            if(!strlen(pPhoneNo))continue;
            
            if((pSF->Off+WINKS_ENG_DS_LT_LEN) > pSF->Len)
            {
                int NewLenA = pSF->Len+WINKS_SEND_FRAME_LEN;
                while(NewLenA <= (int)(pSF->Off+WINKS_ENG_DS_LT_LEN)) 
                {
                    NewLenA += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
                }
                if(SendBufReInit(pSF, NewLenA) != NewLenA)/*申请大缓存失败*/
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
            
            winks_SetDsaLong(&pSF->pSendBuf[SF_Off_L2+2], 4, (RetLen_L2-WINKS_ENG_DS_LT_LEN)/*去掉包装头的六字节*/);
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
            if((ArrCount%20) == 0)/* 每次取20个事件记录*/
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
                    NewLenA += WINKS_SEND_FRAME_LEN;/*得到足以放下当前数据的缓存大小*/
                }
                if(SendBufReInit(pSF, NewLenA) != NewLenA)/*申请大缓存失败*/
                {
                    return 0;
                }
            }
            winks_SetDsaShort(&pSF->pSendBuf[SF_Off_L2], 4, (unsigned short)(ArrCount+1));
            pSF->Off += WINKS_ENG_DS_LT_LEN;
            RetLen_L2 += WINKS_ENG_DS_LT_LEN;
        #ifdef WINKS_DEBUG
            if(pCurPBEvt->event == 1)
                Winks_printf("++++增加号码:%s\r\n", pCurPBEvt->no);
            else if(pCurPBEvt->event == 2)
                Winks_printf("----删除号码:%s\r\n", pCurPBEvt->no);
            else
                Winks_printf("ERROR:未知电话本事件 %d\r\n", pCurPBEvt->event);
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
            
            winks_SetDsaLong(&pSF->pSendBuf[SF_Off_L2+2], 4, (RetLen_L2-WINKS_ENG_DS_LT_LEN)/*去掉包装头的六字节*/);
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
    winks_SetDsaLong(&pSF->pSendBuf[SF_Off+2], 4, (RetLen-WINKS_ENG_DS_LT_LEN)/*去掉包装头的六字节*/);
    return RetLen;
}

static int AddFrameItem(Winks_SendFrame_s* pSF, unsigned short AttrId, void *pData)
{
    int iRet = 0;

    if(AttrId<0x8000)/*只有1层协议*/
    {
        iRet = AddL1Attr(pSF, AttrId, pData);
    }
    else if(AttrId>0xc000)/*有3层协议*/
    {
        iRet = AddL3Attr(pSF, AttrId, pData);
    }
    else/*有2层协议*/
    {
        iRet = AddL2Attr(pSF, AttrId, pData);
    }

    return iRet;
}

/*添加桢数据*/
static int AddSendFrameData(Winks_SendFrame_s* pSF, unsigned short AttrId, void *pData)
{
    int RetLen = 0;
    int iRet = 0;

    switch(AttrId)
    {
    case 0x7FEB:/*获取好友信息*/
    {
        /*3.1.17 Feature客户端与通信服务器的获取指定块好友信息交互过程：
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FEB H 
        2)  本机电话号码属性                            （0008H）
        3)  分块下传或请求好友信息属性                  （8008H）
        4)  手机支持的名片信息属性                       (002BH) 
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

    case 0x7FEC:/*获取基本信息*/
    {
        /*3.1.15 Feature客户端与通信服务器初始化交互过程：
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FECH 
        2)  本机电话号码属性        （0008H）
        3)  客户端配置信息          （8000H）
        4)  初始化用户电话号码本属性 (0011H)
        5)  用户手机空间大小        （0025H）
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

    case 0x7FED:/*PUSH(或定时)更新*/
    {
        int PBEvt_Flag = 0;/* 是否添加电话簿变更事件 */
        /*3.1.19 Feature客户端与通讯服务器的定时通讯过程：（同步全部系统更改信息和用户选定好友的更改信息）
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FEDH
        2)  本机电话号码属性            （0008H）
        3)  更新时间戳                  （0020H）
        4)    手机支持的名片信息属性       (002BH)
        5)  客户端电话本号码属性        （FFFDH）
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

    #if 0   /* winks不能写文件(如手机充电或连接到PC等情况)，定时更新不上传电话本变更 */
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
    
    case 0x7FEF:/*上传好友录*/
    {
        /*3.1.18 Feature客户端与通信服务器的通知服务器新修改的好友信息交互过程：
        2.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FEFH 
        2)  本机电话号码属性                            （0008H）
        3)    用户好友列表号码信息属性             (002DH)
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
        /*3.1.16 Feature客户端与通信服务器的获取全部好友录好友信息交互过程：
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FF0H 
        2)  本机电话号码属性                            （0008H）
        3)    初始化用户电话号码本属性        (0011H)
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
          if((iRet = Winks_SaveCurrentPhoneNumber(WINKS_STRG_PB_LOG_2, 0, 0)) != WINKS_RETURN_SUCCESS)/* 上传电话簿之前保存一份log2 */
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
        /*3.1.10客户端和通信服务器端实时获取彩像交互过程（3G环境）：（JAVA和2.5G终端暂不支持此过程）
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FF8H 
        2)  本机电话号码属性            （0008H）
        3)  对方电话号码                （0009H）
        4)  呼叫类型属性                （0007H）
        5)  TimeStamp                   （0006H）
        6)  客户端软件版本信息属性      （0015H）
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
        /*3.1.1 客户端和配置服务器端通讯过程：
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FF9 H
        2)  IMSI号属性              （000FH）（JAVA终端无此属性）
        3)  本机电话号码            （0008H）（不能获取终端专用）
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
        /*3.1.2客户端向通讯服务器查询是否注册过程：
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FFD H
        2)  IMSI号属性              （000FH）（JAVA终端无此属性）
        3)  终端软件语言属性        （0021H）
        4)  本机电话号码            （0008H）（JAVA终端专用）
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
        /* 3.1.4客户端与通讯服务器的根据彩像ID获取彩像信息*/
        /*
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FFAH 
        2)  本机电话号码属性        （0008H）
        3)  客户端请求彩像文件属性  （8002H）
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
        /* 3.1.9客户端与通信服务器关于实时更新用户状态信息*/
        /*
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FF7H
        2)  本机电话号码属性            （0008H）
        3)      用户个人状态属性            （000BH）（根据需要上传）
        4)  用户彩像服务开关属性        （000DH）（根据需要上传）
        5)  是否公开随电名片属性        （0013H）（根据需要上传）
        6)  用户状态文字属性            （001FH）（根据需要上传）
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
        
    #if 0   /* unfinished 根据需要上传 */
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
        /* 3.1.12客户端与通信服务器关于上传DIY文件信息交互*/
        /*
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FF6H 
        2)  本机电话号码属性            （0008H）
        3)  上传DIY彩像文件基本属性 （8003H）
        4)  分块上传DIY大文件属性   （8006H）（分块上传彩像文件时使用）
        5)  终端软件语言属性            （0021H)
        6)  彩像生成规格属性            （FFF7H）（仅通讯服务器使用此属性）
        7)  彩像数据流（DataStream）属性（0012H）
        */
        {
        Winks_DiyInfo_s *pDiyInfo = (Winks_DiyInfo_s *)pData;
        Winks_EngDiyComInfo_s *pULInfo = NULL;
        char * pfilepath = NULL;
        unsigned long filesize = 0;

        RetLen += AddFrameItem(pSF, 0x0008, pData);

        pULInfo = winks_EngGetDiyContext();
        Winks_assert(pULInfo != NULL);
        
        if(pDiyInfo)/* 第一次调用传入参数 */
        {
            Winks_assert(pDiyInfo->image_count || strlen(pDiyInfo->welcome));/* 彩像或问候语至少有一项*/
            memset(pULInfo, 0, sizeof(Winks_EngDiyComInfo_s));
            memcpy(pULInfo, pDiyInfo, sizeof(Winks_DiyInfo_s));
            
        }
        else{
            Winks_assert(pULInfo->image_count || strlen(pULInfo->welcome));/* 彩像或问候语至少有一项*/
        }
        
        Winks_assert(pULInfo->image_count <= WINKS_MAX_NUM_OF_IMAGE_LIST);
        if(pULInfo->image_count)/* 上传彩像 */
        {
            if(!pULInfo->curFileSize)/* 当传完1张图片时，文件大小置0，以判断准备上传下一张 */
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
        3.1.8（7FF3H）客户端与通信服务器关于实时修改用户问候语交互过程：
        1.  客户端：
        向平台发送使用到的消息属性：
        1)  MessageType属性                 值为：7FF3H 
        2)  本机电话号码属性        （0008H）
        3)  修改用户问候语          （8004H）
        */
        Winks_DiyInfo_s *pDiyInfo = (Winks_DiyInfo_s *)pData;
        Winks_EngDiyComInfo_s *pULInfo;

        RetLen += AddFrameItem(pSF, 0x0008, pData);

        pULInfo = winks_EngGetDiyContext();
        
        if(pDiyInfo && pULInfo)/* 第一次调用传入参数 */
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
    winks_SetDsaLong(&pSF->pSendBuf[0], WINKS_ENG_DS_4BYTES_LEN, RetLen);/*桢总长度*/

    return RetLen;
}

static char *GetProtocolLanguage(void)
{
/*
31. 终端软件语言属性(0021H)
序号    语言类型码  语言类型名称    备注
1.      ZH_TW   台湾    
2.      ZH_HK   香港    
3.      ZH_SG   新加坡  
4.      ZH_CN   中文简体    
5.      EN  英语（美国）    
6.      JA  日文    
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

/*取漫游限制连网的标志. allowedFlag:0限制;1不限制*/
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
    
    if(roamSwitch == WINKS_ROAM_SWITCH_OFF) /*国际漫游时不连接服务器*/
    {
        Winks_GetRoamingFlag(&roamflag);
        if(roamflag == 1)
            *allowedFlag = 0;
    }

    return WINKS_RETURN_SUCCESS;
}

/*判断是否注册用户。 regFlag:0未注册，1已注册*/
int Winks_isUserRegistered(unsigned char *regFlag)
{
    int iRet = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    *regFlag = 0;
    if((Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED)
        ||(Winks_engineContext_p->curState == WINKS_ENGINE_STATE_SHOWB))
    {
        /*判断卡信息*/
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

    /*资源释放*/
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
    /*子状态回滚*/
    if(ClearAll)
        Winks_SubStateRollback();

    switch(Winks_engineContext_p->curState) 
    {
    case WINKS_ENGINE_STATE_ENABLED:
    case WINKS_ENGINE_STATE_SHOWB:
    {
        /* IMSI号复位 */
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
        
        /*停定时更新的TIMER*/
        winks_StopPeriodPushTimer(2);

        /*当前状态复位*/
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

    /* 清存储注册信息 */
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
        /*取USB状态。1:系统插了USB, 0:系统未插USB.*/
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
    case 101: /* Sim卡1发送短信 */
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
    case 102: /* Sim卡2发送短信 */
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
    case 110: /* 启动及IMSI读取状态 */
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

        Winks_SetOsalTestBitFlag(6, 0);/* 手动关闭usb状态调试监测 */
        
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

        Winks_SetOsalTestBitFlag(6, 1);/* 手动开启usb状态调试监测 */
        
        fp = Winks_fopen( "D:\\DbgUsb.txt", "w" );
        if(fp != NULL) Winks_fclose( fp );
        
        break;
    }
    case 202: /* USB插入 */
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
    case 203: /* USB拔除 */
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
    /* 默认非usb状态*/
    case 207:/* usb状态改变 */
    {
        iRet = Winks_GetOsalTestBitFlag(7,&val);
        iRet = Winks_SetOsalTestBitFlag(7,!val);
        Winks_printf("[TEST] Change usb status flag. val[%d]\r\n",!val);
        break;
    }
    /* 默认有存储卡 */
    case 208:/* memory card状态改变 */
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
        ,{"101#", 101}  /* 101 Sim卡1发送短信 */  
        ,{"102#", 102}  /* 102 Sim卡2发送短信 */  
        ,{"103#", 103}  /* 103 测试协议 */  
        ,{"110#", 110}  /* 110 启动及IMSI读取状态. */  
#if 1//debug usb state
        ,{"200#", 200}  /* 110 启动及IMSI读取状态. */  
        ,{"201#", 201}  /* 110 启动及IMSI读取状态. */  
        ,{"202#", 202}  /* 202 USB插入 */  
        ,{"203#", 203}  /* 203 USB拔除 */  
#endif//debug usb state
        ,{"207#", 207}  /* 207 usb状态改变 */
        ,{"208#", 208}  /* 208 memory card状态改变 */
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
