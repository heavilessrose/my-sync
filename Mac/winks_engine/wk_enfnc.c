/*
File:       wk_enfnc.c
Author:     Huaxm
Create Time:04/03/2009
*/

#include "wk_public.h"
#include "wk_osal.h"
#include "wk_http.h"
#include "wk_enfnc.h"
#include "wk_strg_if.h"
#include "wk_xmlgb.h"

#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
static Winks_EngineContext Winks_engineContext;
#endif

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

#define WINKS_ALIGN(len, Align)     (((len) + Align - 1) & (unsigned long)(0-Align))

static int winks_EngineIdleHandler(unsigned long msg, void *data, unsigned long size);
static int winks_EngAppTimerProc(WINKS_TIMER_ID timerId);
static int Winks_EngAppDestroy(void);
static int Winks_SendMsgToRunningEngApp(unsigned long msgid, void *data, unsigned long dataSize);
//static int Winks_SendReturnMsgToEngApp(void *data, unsigned long dataSize);

static void winks_StartSrvMsgTimer(void);
static void winks_StopSrvMsgTimer(void);
static void winks_StartBuildCacheTimer(void);
static void winks_StopBuildCacheTimer(void);
#ifdef WINKS_SUPPORT_3G
static void winks_StartCallWaitTimer(void);
static void winks_StopCallWaitTimer(void);
#endif
static int winks_StartPeriodPushTimer(void);
static int winks_StopPeriodPushTimer(int cause);
static int winks_DelayPeriodPushTimer(void);

/*取漫游限制连网的标志. allowedFlag:0限制;1不限制*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag);

#ifdef WINKS_DEBUG
static int Winks_TestModeManager(char * keyStr);
#endif

void * get_strg_filemap(void)
{
    WINKS_ENGINE_CONTEXT_DEF()
    WINKS_ENGINE_CONTEXT_INIT()
    return (void *)&Winks_engineContext_p->StrgFileMap;
}

#ifdef WINKS_DEBUG
typedef struct tagWinks_Eng2UIMsgInfo_s
{
    int Msg;
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    char  ID_Info[36];
    char  StrInfo[128];
#else
    char * ID_Info;
    char * StrInfo;
#endif
}Winks_Eng2UIMsgInfo_s;

static const Winks_Eng2UIMsgInfo_s Eng2UiMsgInfo[] = 
{
    {WINKS_MSG_ENG2UI_NONE           , "WINKS_MSG_ENG2UI_NONE"           , "无关消息"},
    
/*出错消息*/
    {WINKS_MSG_ENG2UI_SVR_NO_ANSWER  , "WINKS_MSG_ENG2UI_SVR_NO_ANSWER"  , "服务器无应答"},
    {WINKS_MSG_ENG2UI_UNREG_USER     , "WINKS_MSG_ENG2UI_UNREG_USER"     , "非注册用户限制"},
    {WINKS_MSG_ENG2UI_ROAMING_LIMIT  , "WINKS_MSG_ENG2UI_ROAMING_LIMIT"  , "漫游限制 "},
    {WINKS_MSG_ENG2UI_ENG_BUSY       , "WINKS_MSG_ENG2UI_ENG_BUSY"       , "引擎忙"},
    {WINKS_MSG_ENG2UI_DIAL_INT       , "WINKS_MSG_ENG2UI_DIAL_INT"       , "被电话中断"},
    {WINKS_MSG_ENG2UI_STORAGE_LIMIT  , "WINKS_MSG_ENG2UI_STORAGE_LIMIT"  , "存储空间不足"},
    {WINKS_MSG_ENG2UI_CC_CLOSED      , "WINKS_MSG_ENG2UI_CC_CLOSED"      , "彩像软件关闭 "},
#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_MCARD_NOT_EXIST, "WINKS_MSG_ENG2UI_MCARD_NOT_EXIST", "存储卡不存在"},
#endif
    {WINKS_MSG_ENG2UI_SYS_FILES_ERR  , "WINKS_MSG_ENG2UI_SYS_FILES_ERR"  , "系统文件被破坏"},
    {WINKS_MSG_ENG2UI_CC_FILES_ERR   , "WINKS_MSG_ENG2UI_CC_FILES_ERR"   , "彩像文件被破坏"},
#ifdef WINKS_DUAL_SIM
    {WINKS_MSG_ENG2UI_SIM_SEL_ERR    , "WINKS_MSG_ENG2UI_SIM_SEL_ERR"    , "用户未选择SIM卡"},
    {WINKS_MSG_ENG2UI_SIM_UNUSABLE   , "WINKS_MSG_ENG2UI_SIM_UNUSABLE"   , "用户选择的Sim卡不可用"},
#endif
    {WINKS_MSG_ENG2UI_NO_SUBSCRIPTION, "WINKS_MSG_ENG2UI_NO_SUBSCRIPTION", "不是订阅用户"},
    {WINKS_MSG_ENG2UI_USER_DISABLED  , "WINKS_MSG_ENG2UI_USER_DISABLED"  , "用户被禁用"},
    {WINKS_MSG_ENG2UI_NO_NUMBER      , "WINKS_MSG_ENG2UI_NO_NUMBER"      , "没有电话号码需要更新"},

    {WINKS_MSG_ENG2UI_PROT_NO        , "WINKS_MSG_ENG2UI_PROT_NO"        , "服务器下发协议号不匹配"},
    {WINKS_MSG_ENG2UI_HTTP_ERRCODE   , "WINKS_MSG_ENG2UI_HTTP_ERRCODE"   , "收到Http错误码"},
    {WINKS_MSG_ENG2UI_SRV_ERRCODE    , "WINKS_MSG_ENG2UI_SRV_ERRCODE"    , "收到服务器返回错误码"},
    {WINKS_MSG_ENG2UI_SRV_ERRDATA    , "WINKS_MSG_ENG2UI_SRV_ERRDATA"    , "服务器返回数据有错误"},
    {WINKS_MSG_ENG2UI_MEM            , "WINKS_MSG_ENG2UI_MEM"            , "内存申请出错"},
    {WINKS_MSG_ENG2UI_FS             , "WINKS_MSG_ENG2UI_FS"             , "文件系统出错"},
    {WINKS_MSG_ENG2UI_HTTP           , "WINKS_MSG_ENG2UI_HTTP"           , "HTTP错误"},
    {WINKS_MSG_ENG2UI_HTTP_TIMEOUT   , "WINKS_MSG_ENG2UI_HTTP_TIMEOUT"   , "HTTP超时错误"},
    {WINKS_MSG_ENG2UI_PROT_PACK      , "WINKS_MSG_ENG2UI_PROT_PACK"      , "协议打包"},
    {WINKS_MSG_ENG2UI_PROT_PARSE     , "WINKS_MSG_ENG2UI_PROT_PARSE"     , "协议解析"},
    {WINKS_MSG_ENG2UI_XML_GEN        , "WINKS_MSG_ENG2UI_XML_GEN"        , "生成xml出错"},
    {WINKS_MSG_ENG2UI_XML_PARSE      , "WINKS_MSG_ENG2UI_XML_PARSE"      , "xml解析出错"},
    {WINKS_MSG_ENG2UI_APP_ERR        , "WINKS_MSG_ENG2UI_APP_ERR"        , "应用调用错"},
    {WINKS_MSG_ENG2UI_ENG_ERR        , "WINKS_MSG_ENG2UI_ENG_ERR"        , "引擎调用错"},
    
    {WINKS_MSG_ENG2UI_UNKNOWN        , "WINKS_MSG_ENG2UI_UNKNOWN"        , "未知错误"},

/*1 注册及初始化过程*/
    {WINKS_MSG_ENG2UI_SETUP_SVR_FAIL , "WINKS_MSG_ENG2UI_SETUP_SVR_FAIL" , "连接配置服务器失败"},
    {WINKS_MSG_ENG2UI_REG_FAIL       , "WINKS_MSG_ENG2UI_REG_FAIL"       , "注册失败"},
    {WINKS_MSG_ENG2UI_ULPB_FAIL      , "WINKS_MSG_ENG2UI_ULPB_FAIL"      , "上传电话簿失败"},
    {WINKS_MSG_ENG2UI_INIT_FAIL      , "WINKS_MSG_ENG2UI_INIT_FAIL"      , "初始化失败"},
                                                                       
    {WINKS_MSG_ENG2UI_SETUP_SVR      , "WINKS_MSG_ENG2UI_SETUP_SVR"      , "正在连接配置服务器..."},
    {WINKS_MSG_ENG2UI_SETUP_SVR_OK   , "WINKS_MSG_ENG2UI_SETUP_SVR_OK"   , "连接配置服务器成功"},
    {WINKS_MSG_ENG2UI_SMS_REG        , "WINKS_MSG_ENG2UI_SMS_REG"        , "未注册，需要发送注册短信"},
    {WINKS_MSG_ENG2UI_WAIT_PUSH      , "WINKS_MSG_ENG2UI_WAIT_PUSH"      , "未注册，等待接收push"},
    {WINKS_MSG_ENG2UI_REG            , "WINKS_MSG_ENG2UI_REG"            , "正在注册..."},
    {WINKS_MSG_ENG2UI_REG_OK         , "WINKS_MSG_ENG2UI_REG_OK"         , "注册OK"},
    {WINKS_MSG_ENG2UI_ULPB_CONFIRM   , "WINKS_MSG_ENG2UI_ULPB_CONFIRM"   , "用户确认是否上传电话簿"},
    {WINKS_MSG_ENG2UI_ULPB           , "WINKS_MSG_ENG2UI_ULPB"           , "正在上传电话簿..."},
    {WINKS_MSG_ENG2UI_ULPB_OK        , "WINKS_MSG_ENG2UI_ULPB_OK"        , "上传电话簿完毕"},
    {WINKS_MSG_ENG2UI_INIT           , "WINKS_MSG_ENG2UI_INIT"           , "正在初始化..."},
    {WINKS_MSG_ENG2UI_INIT_OK        , "WINKS_MSG_ENG2UI_INIT_OK"        , "初始化完毕"},
    {WINKS_MSG_ENG2UI_QUERY_OK       , "WINKS_MSG_ENG2UI_QUERY_OK"        , "查询用户状态完成"},

/*2 更新好友录*/
    {WINKS_MSG_ENG2UI_FRIEND_FAIL    , "WINKS_MSG_ENG2UI_FRIEND_FAIL"    , "更新好友录失败"},
    
    {WINKS_MSG_ENG2UI_FRIEND         , "WINKS_MSG_ENG2UI_FRIEND"         , "正在更新好友录..."},
    {WINKS_MSG_ENG2UI_FRIEND_OK      , "WINKS_MSG_ENG2UI_FRIEND_OK"      , "好友录更新完毕"},

/*3 上传DIY*/
    {WINKS_MSG_ENG2UI_DIY_FAIL       , "WINKS_MSG_ENG2UI_DIY_FAIL"       , "上传DIY失败"},
    {WINKS_MSG_ENG2UI_SCRIPT_FAIL    , "WINKS_MSG_ENG2UI_SCRIPT_FAIL"    , "定制彩像失败"},
                                                                  
    {WINKS_MSG_ENG2UI_DIY            , "WINKS_MSG_ENG2UI_DIY"            , "正在上传DIY..."},
    {WINKS_MSG_ENG2UI_DIY_OK         , "WINKS_MSG_ENG2UI_DIY_OK"         , "上传DIY成功"},
    {WINKS_MSG_ENG2UI_SCRIPT_OK      , "WINKS_MSG_ENG2UI_SCRIPT_OK"      , "定制彩像成功"},

/*4 推荐给好友*/
    {WINKS_MSG_ENG2UI_RECOMMEND_FAIL , "WINKS_MSG_ENG2UI_RECOMMEND_FAIL" , "推荐失败"},
                                                                      
    {WINKS_MSG_ENG2UI_RECOMMEND      , "WINKS_MSG_ENG2UI_RECOMMEND"      , "正在推荐..."},
    {WINKS_MSG_ENG2UI_RECOMMEND_OK   , "WINKS_MSG_ENG2UI_RECOMMEND_OK"   , "推荐成功"},

/*5 电话下载*/
    {WINKS_MSG_ENG2UI_CALL_FAIL      , "WINKS_MSG_ENG2UI_CALL_FAIL"      , "获取通话用户彩像失败"},
    {WINKS_MSG_ENG2UI_CALL_TIMEOUT   , "WINKS_MSG_ENG2UI_CALL_TIMEOUT"   , "获取通话用户彩像超时"},
                                                                    
    {WINKS_MSG_ENG2UI_CALL           , "WINKS_MSG_ENG2UI_CALL"           , "正在获取通话用户彩像..."},
    {WINKS_MSG_ENG2UI_CALL_OK        , "WINKS_MSG_ENG2UI_CALL_OK"        , "获取通话用户彩像成功"},

/*6 同步本地配置到服务器*/
    {WINKS_MSG_ENG2UI_SETUP_FAIL     , "WINKS_MSG_ENG2UI_SETUP_FAIL"    , "同步本地配置失败"},
    {WINKS_MSG_ENG2UI_SRVCFG_FAIL    , "WINKS_MSG_ENG2UI_SRVCFG_FAIL"    , "服务器配置失败"},
    {WINKS_MSG_ENG2UI_USERCFG_FAIL   , "WINKS_MSG_ENG2UI_USERCFG_FAIL"   , "本地配置失败"},
    {WINKS_MSG_ENG2UI_ULSTAT_FAIL    , "WINKS_MSG_ENG2UI_ULSTAT_FAIL"    , "统计信息上传失败"},
    {WINKS_MSG_ENG2UI_ULLOG_FAIL     , "WINKS_MSG_ENG2UI_ULLOG_FAIL"     , "log信息上传失败"},
                                                                       
    {WINKS_MSG_ENG2UI_SETUP          , "WINKS_MSG_ENG2UI_SETUP"          , "正在同步本地配置..."},
    {WINKS_MSG_ENG2UI_SETUP_OK       , "WINKS_MSG_ENG2UI_SETUP_OK"       , "同步本地配置成功"},
    {WINKS_MSG_ENG2UI_SRVCFG_OK      , "WINKS_MSG_ENG2UI_SRVCFG_OK"      , "服务器配置成功"},
    {WINKS_MSG_ENG2UI_USERCFG_OK     , "WINKS_MSG_ENG2UI_USERCFG_OK"     , "本地配置成功"},
    {WINKS_MSG_ENG2UI_ULSTAT_OK      , "WINKS_MSG_ENG2UI_ULSTAT_OK"      , "统计信息上传成功"},
    {WINKS_MSG_ENG2UI_ULLOG_OK       , "WINKS_MSG_ENG2UI_ULLOG_OK"       , "log信息上传成功"},

/*7 检查新版本*/
    {WINKS_MSG_ENG2UI_RUPGRADE_FAIL  , "WINKS_MSG_ENG2UI_RUPGRADE_FAIL"  , "升级失败"},
                                                                       
    {WINKS_MSG_ENG2UI_RUPGRADE       , "WINKS_MSG_ENG2UI_RUPGRADE"       , "正在检查新版本..."},
    {WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW, "WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW" , "没有新版本"},
    {WINKS_MSG_ENG2UI_RUPGRADE_NEW   , "WINKS_MSG_ENG2UI_RUPGRADE_NEW"   , "有新版本"},
    {WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK, "WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK", "新版本下载成功"},
};

#define ENG2UI_MSG_NUM    (sizeof(Eng2UiMsgInfo)/sizeof(Eng2UiMsgInfo[0]))

int winks_EngUiMsgGetProgress(Winks_EngineToUiReturnMsg *pEng2UIMsg,
                              unsigned short *retFin, unsigned short *retTotal)
{
    switch(pEng2UIMsg->code)
    {
    case WINKS_MSG_ENG2UI_INIT:
    case WINKS_MSG_ENG2UI_FRIEND:
    //case WINKS_MSG_ENG2UI_DIY:
    {
        WinksProgress_s *pProgress = (WinksProgress_s *)pEng2UIMsg->body;
    
        *retFin = pProgress->Finished;
        *retTotal = pProgress->Total;
    }
        break;
    default:
        *retFin = *retTotal = 0;
        break;
    }

    return 0;
}

void winks_PrintEng2UiMsgInfo(Winks_EngineToUiReturnMsg *pEng2UIMsg)
{
    int iCnt;
    unsigned short Fin, Total;
    int Msg = pEng2UIMsg->code;

    for(iCnt=0; iCnt<ENG2UI_MSG_NUM; iCnt++)
    {
        if(Msg == Eng2UiMsgInfo[iCnt].Msg)
        {
            winks_EngUiMsgGetProgress(pEng2UIMsg, &Fin, &Total);
            Winks_printf("Eng2UiMsg: [%d] [%s] [%d/%d] %s\r\n", pEng2UIMsg->code, 
                Eng2UiMsgInfo[iCnt].ID_Info, Fin, Total, Eng2UiMsgInfo[iCnt].StrInfo);
            break;
        }
    }
    if(iCnt == ENG2UI_MSG_NUM)
    {
        Winks_printf("Eng2UiMsg: [%d] ????????????????????????????????????? \r\n", pEng2UIMsg->code);
    }
}

static const Winks_Eng2UIMsgInfo_s ResultCodeInfo[] = 
{
    {WINKS_RST_CODE_OK   , "WINKS_RST_CODE_OK"   , "正确"},
    
    {RESOURCE_NOT_FOUND  , "RESOURCE_NOT_FOUND"  , "资源未找到"},
    {ACCESS_DENIED       , "ACCESS_DENIED"       , "访问被拒绝"},
    {INVALID_PARAMETER   , "INVALID_PARAMETER"   , "调用参数(形参)错"},
    {BAD_ARGUMENTS       , "BAD_ARGUMENTS"       , "参数错"},
    {NOT_IMPLEMENTED     , "NOT_IMPLEMENTED"     , "功能尚未实现"},
    {DATABASE_EXCEPTION  , "DATABASE_EXCEPTION"  , "数据库访问异常"},

/* 1001~1999    网络通信错误 */
    {PROTOCOL_VERSION_NOT_SUPPORTED  , "PROTOCOL_VERSION_NOT_SUPPORTED"  , "不支持的协议版本"},

/* 5001~5999    彩像业务逻辑错误 */
    {SERVICE_NO_SUBSCRIPTION         , "SERVICE_NO_SUBSCRIPTION"         , "不是订阅用户"},
    {SERVICE_MEMBER_DISABLED         , "SERVICE_MEMBER_DISABLED"         , "用户被禁用"},
    {SERVICE_NOT_MEMBER              , "SERVICE_NOT_MEMBER"              , "不是彩像用户"},
};

#define  RST_CODE_NUM    (sizeof(ResultCodeInfo)/sizeof(ResultCodeInfo[0]))

void winks_PrintResultCodeInfo(int ResultCode)
{
    int iCnt;
    int Msg = ResultCode;

    for(iCnt=0; iCnt<ENG2UI_MSG_NUM; iCnt++)
    {
        if(Msg == ResultCodeInfo[iCnt].Msg)
        {
            Winks_printf("ResultCode: [%d] [%s] %s\r\n", ResultCode, 
                ResultCodeInfo[iCnt].ID_Info, ResultCodeInfo[iCnt].StrInfo);
            break;
        }
    }
    if(iCnt == ENG2UI_MSG_NUM)
    {
        Winks_printf("ResultCode: [%d] ????????????????????????????????????? \r\n", ResultCode);
    }
}

#endif

#define EXT_gif     ".gif"
#define EXT_jpg     ".jpg"
#define EXT_jpeg    ".jpeg"
#define EXT_png     ".png"
#define EXT_mpeg    ".avi"
#define EXT_swf     ".swf"
#define EXT_xml     ".xml"

/* 输入带'.'的扩展名，输出ContentType */
char * Winks_Ext2ContType(char *ext)
{
    if(strnicmp(ext, EXT_gif, strlen(EXT_gif)) == 0)
        return HTTP_CONTENTTYPE_gif;
    else if( strnicmp(ext, EXT_jpg, strlen(EXT_jpg)) == 0 ||
             strnicmp(ext, EXT_jpeg, strlen(EXT_jpeg)) == 0 )
        return HTTP_CONTENTTYPE_jpeg;
    else if(strnicmp(ext, EXT_png, strlen(EXT_png)) == 0)
        return HTTP_CONTENTTYPE_png;
    else if(strnicmp(ext, EXT_mpeg, strlen(EXT_mpeg)) == 0)
        return HTTP_CONTENTTYPE_mpeg;
    else if(strnicmp(ext, EXT_swf, strlen(EXT_swf)) == 0)
        return HTTP_CONTENTTYPE_flash;
    else if(strnicmp(ext, HTTP_CONTENTTYPE_xml, strlen(HTTP_CONTENTTYPE_xml)) == 0)
        return HTTP_CONTENTTYPE_xml;
    else
        return HTTP_CONTENTTYPE_octet;
}

static int winks_Ext2ExtType(char *ext)
{
    if(strnicmp(ext, EXT_gif, strlen(EXT_gif)) == 0)
        return WINKS_FILE_EXT_gif;
    else if( strnicmp(ext, EXT_jpg, strlen(EXT_jpg)) == 0 ||
             strnicmp(ext, EXT_jpeg, strlen(EXT_jpeg)) == 0 )
        return WINKS_FILE_EXT_jpg;
    else if(strnicmp(ext, EXT_png, strlen(EXT_png)) == 0)
        return WINKS_FILE_EXT_png;
    else if(strnicmp(ext, EXT_mpeg, strlen(EXT_mpeg)) == 0)
        return WINKS_FILE_EXT_avi;
    else if(strnicmp(ext, EXT_swf, strlen(EXT_swf)) == 0)
        return WINKS_FILE_EXT_swf;
    else if(strnicmp(ext, EXT_xml, strlen(EXT_xml)) == 0)
        return WINKS_FILE_EXT_xml;
    else
        return WINKS_FILE_EXT_none;
}

static char * winks_ExtType2Ext(int ExtType)
{
    switch(ExtType)
    {
    case WINKS_FILE_EXT_gif:
        return EXT_gif;
    case WINKS_FILE_EXT_jpg:
        return EXT_jpg;
    case WINKS_FILE_EXT_png:
        return EXT_png;
    case WINKS_FILE_EXT_swf:
        return EXT_swf;
    case WINKS_FILE_EXT_xml:
        return EXT_xml;
    }
    return NULL;
}

static int winks_ContType2ExtType(char * ContType)
{
    if(strncmp(ContType, HTTP_CONTENTTYPE_xml, strlen(HTTP_CONTENTTYPE_xml)) == 0)
        return WINKS_FILE_EXT_xml;
    else if(strncmp(ContType, HTTP_CONTENTTYPE_gif, strlen(HTTP_CONTENTTYPE_gif)) == 0)
        return WINKS_FILE_EXT_gif;
    else if(strncmp(ContType, HTTP_CONTENTTYPE_jpeg, strlen(HTTP_CONTENTTYPE_jpeg)) == 0)
        return WINKS_FILE_EXT_jpg;
    else if(strncmp(ContType, HTTP_CONTENTTYPE_png, strlen(HTTP_CONTENTTYPE_png)) == 0)
        return WINKS_FILE_EXT_png;
    else if(strncmp(ContType, HTTP_CONTENTTYPE_mpeg, strlen(HTTP_CONTENTTYPE_mpeg)) == 0)
        return WINKS_FILE_EXT_avi;
    else if(strncmp(ContType, HTTP_CONTENTTYPE_flash, strlen(HTTP_CONTENTTYPE_flash)) == 0)
        return WINKS_FILE_EXT_swf;
    else if(strncmp(ContType, HTTP_CONTENTTYPE_octet, strlen(HTTP_CONTENTTYPE_octet)) == 0)
        return WINKS_FILE_EXT_bin;
    else
        return WINKS_FILE_EXT_none;
}

static int winks_GetSupportedExt() 
{
    char * mt;
    unsigned short SupportExtNum = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_mem_set(Winks_engineContext_p->SupportExt, 0, sizeof(Winks_engineContext_p->SupportExt));
    mt = WINKS_MT;
    if(mt != NULL)
    {
        if(strstr(mt, "jpg") || strstr(mt, "jpeg"))
            Winks_engineContext_p->SupportExt[SupportExtNum++] = WINKS_FILE_EXT_jpg;
        if(strstr(mt, "gif"))
            Winks_engineContext_p->SupportExt[SupportExtNum++] = WINKS_FILE_EXT_gif;
        if(strstr(mt, "png"))
            Winks_engineContext_p->SupportExt[SupportExtNum++] = WINKS_FILE_EXT_png;
        if(strstr(mt, "swf"))
            Winks_engineContext_p->SupportExt[SupportExtNum++] = WINKS_FILE_EXT_swf;
        Winks_engineContext_p->SupportExtNum = SupportExtNum;

        return SupportExtNum;
    }

    return -1;
}

static int winks_GetSupportedDim() 
{
    char  * cl;
    int     len;
    unsigned short SupportDimNum = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_mem_set(Winks_engineContext_p->SupportDim, 0, sizeof(Winks_engineContext_p->SupportDim));
    cl = WINKS_CL;
    len = strlen(cl);
    if(cl != NULL)
    {
        int Tmp = 0;
        int Cnt = 0;
        
        while(cl && len>0)
        {
            if(*cl>='0' && *cl<='9')
            {
                Tmp *= 10;
                Tmp += *cl-'0';
            }
            else if(Tmp)
            {
                SupportDimNum = (unsigned short)(Cnt/2);
                if(Cnt%2)
                    Winks_engineContext_p->SupportDim[SupportDimNum][1] = (unsigned short)Tmp;
                else
                    Winks_engineContext_p->SupportDim[SupportDimNum][0] = (unsigned short)Tmp;
                if(*cl=='x' || *cl=='x')
                    Cnt++;
                Tmp = 0;
            }
            cl++;
            len--;
        }
        
        if(Tmp)
        {
            Winks_assert(Cnt%2);
            Winks_engineContext_p->SupportDim[SupportDimNum][1] = (unsigned short)Tmp;
        }
        
        Winks_engineContext_p->SupportDimNum = (unsigned short)(SupportDimNum+1);
        return SupportDimNum;
    }

    return -1;
}

static int winks_IsSupportedExt(char * url) 
{
    int i;
    char * ext = NULL;
    unsigned char ExtType;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    while(url && *url)
    {
        if(*url == '.')
            ext = url;
        url++;
    }

    if(ext==NULL)
        return 0;
    
    ExtType = (unsigned char)winks_Ext2ExtType(ext);
    if(ExtType == WINKS_FILE_EXT_none)
        return 0;
    
    for(i=0; i<Winks_engineContext_p->SupportExtNum; i++)
    {
        if( ExtType == Winks_engineContext_p->SupportExt[i] )
            break;
    }

    if(i==Winks_engineContext_p->SupportExtNum)
        return 0;
    
    return 1;
}

static int winks_IsSupportedDim(unsigned short w, unsigned short h) 
{
    int i;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    for(i=0; i<Winks_engineContext_p->SupportDimNum; i++)
    {
        if( w <= Winks_engineContext_p->SupportDim[i][0] &&
            h <= Winks_engineContext_p->SupportDim[i][1] )
            break;
    }

    if(i==Winks_engineContext_p->SupportDimNum)
        return 0;
    return 1;
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
    Winks_engineContext_p->curState = WINKS_ENGINE_STATE_INIT;
    engineChn = Winks_CreateChn(winks_EngineIdleHandler);
    Winks_assert(engineChn != WINKS_CHN_NULL);

    winks_GetSupportedExt();
    winks_GetSupportedDim();
    
    return engineChn;
}

/*引擎注销*/
int Winks_EngineDestroy(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    /*销毁引擎已注册的应用*/
    Winks_EngAppDestroy();

    /*引擎存储销毁函数*/
    Winks_StrgDeinitMapFile();
    
    /*释放引擎数据*/
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    if(Winks_engineContext_p != NULL)
    {
        if(Winks_engineContext_p->pCall)
        {
            Winks_salFree(Winks_engineContext_p->pCall);
            Winks_engineContext_p->pCall = NULL;
        }
        if(Winks_engineContext_p->pSaveSendData)
        {
            Winks_salFree(Winks_engineContext_p->pSaveSendData);
            Winks_engineContext_p->pSaveSendData = NULL;
        }
        Winks_salFree(Winks_engineContext_p);
        Winks_engineContext_p = NULL;
        set_module_glob(WK_MODULE_ENGINE, NULL); 
    }
#endif
    return WINKS_RETURN_SUCCESS;
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

        /*Destroy http timer.*/
        if(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL)
        {
            Winks_DestroyTimer(Winks_engineContext_p->httpTimer);
            Winks_engineContext_p->httpTimer = WINKS_TIMER_NULL;
        }
    }
    return iRet;
}

/* 针对WINKS_ENG_ERROR_CODE_6,7,8, 不需改变当前状态，直接返回给UI出错信息 */
static int winks_EngRetErrDirectly(int ErrCode)
{
#if 0
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
#else
	return WINKS_RETURN_FAILURE;
#endif
}

static unsigned long winks_HttpTimeLimit()
{
    unsigned long value;
    
    if( Winks_GetSettingValue(WINKS_SETTING_CONNECT_TIMEOUT, &value) != WINKS_RETURN_SUCCESS ||
        value == 0 )
        return WINKS_HTTP_TIME_LIMIT;
    return value;
}

/*发送数据*/
static int winks_SendData(WinksProt_PackData_s * pProtPack)
{
    int iRet = WINKS_RETURN_SUCCESS;
    unsigned long   ap = 0;
    Winks_Http      httpstr;
    Winks_Proxy     Proxy;
    char            ProxyIP[16];
    int             ifpipe = WINKS_HTTP_CONCLOSE;
//    Winks_ConnectWayOperation_s cwo;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(pProtPack);
    Winks_assert(pProtPack->url);
    
    Winks_mem_set(&httpstr, 0, sizeof(httpstr));
    Winks_mem_set(&Proxy, 0, sizeof(Proxy));

    Winks_printf( "winks_SendData(%d): srv=%d len=%d url(%d)=\"%s\"\r\n", 
                                   Winks_engineContext_p->FrameNo,
                                   pProtPack->server, 
                                   pProtPack->DataLen,
                                   strlen(pProtPack->url),
                                   pProtPack->url );

    iRet = Winks_GetSettingValue(WINKS_SETTING_APN_ID, &ap);
    if(iRet == WINKS_RETURN_FAILURE)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_printf("Get apn id=[%d]\r\n",ap);
    Winks_setdialid( ap );

    httpstr.channel = Winks_engineContext_p->engChn;
    /*设置消息接收函数的接收消息。*/
    httpstr.msg = WINKS_MSG_ENGINE_HTTP;
    /*设置请求方法为POST。*/
    if(pProtPack->method == WINKS_METHOD_POST)
        httpstr.method = WINKS_HTTP_MEPOST;
    else
        httpstr.method = WINKS_HTTP_MEGET;
    /*无内容类型*/
    if(pProtPack->pContenttype)
        httpstr.pContenttype = pProtPack->pContenttype;
    else
        httpstr.pContenttype = HTTP_CONTENTTYPE_xml_post;


#ifndef WINKS_UNSUPPORT_PROXY
    Winks_mem_set(ProxyIP, 0, sizeof(ProxyIP));
  #ifndef WINKS_SIMULATOR
    if(Winks_GetProxyServer(ProxyIP, &Proxy.port, sizeof(ProxyIP)) != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }
{
    char *pIp = ProxyIP;
    while((*pIp == '.') || ((*pIp >= '0')&&(*pIp <= '9')))
        pIp++;
    *pIp = '\0';
}
  //#else
  //  strcpy(ProxyIP, "192.168.10.92");
  //  Proxy.port = 80;
  #endif
    if((strcmp(ProxyIP, "0.0.0.0") != 0) &&
        (strlen(ProxyIP) != 0))
    {
        /*Proxy.phostname = "10.0.0.172";
        Proxy.port = 80;*/
        Proxy.phostname = ProxyIP;
        //Proxy.port = ProxyPort;
        Proxy.ppassword = NULL;
        Proxy.pusername = NULL;
        httpstr.pwwwau = NULL;

        httpstr.pproxy = &Proxy;
        
        Winks_printf("Proxy: IP[%s] port[%d]\r\n", ProxyIP, Proxy.port);
    }
    else
#endif
    {
        httpstr.pproxy = NULL;
        httpstr.pwwwau = NULL;
    }
    
    /*可按需要设置不支持持久连接*/
    Winks_httpsetopt( WINKS_HTTP_OPPIPE, &ifpipe, sizeof(ifpipe) );
    Winks_httpsetopt( WINKS_HTTP_OPUAGENG, WINKS_USRAGENT, strlen(WINKS_USRAGENT) );

    /*设置请求网页的URL。*/
    httpstr.pURL = pProtPack->url;

    /*创建HTTP传输句柄。*/
    Winks_assert(Winks_engineContext_p->http == WINKS_HTTP_NULL);
    Winks_engineContext_p->http = Winks_httpopen( &httpstr );
    if(Winks_engineContext_p->http == WINKS_HTTP_NULL)
    {
        Winks_assert(Winks_engineContext_p->http != WINKS_HTTP_NULL);
        return WINKS_RETURN_FAILURE;
    }

    /*写入数据*/
    if(httpstr.method == WINKS_HTTP_MEPOST)
    {
        if( pProtPack->IsOnlyPath && 
            pProtPack->pPath && pProtPack->PathLen )
            iRet = Winks_httpappenddata( Winks_engineContext_p->http, 
                                         (char *)pProtPack->pPath, 
                                         pProtPack->PathLen, 
                                         pProtPack->IsOnlyPath );
        else if(pProtPack->pData && pProtPack->DataLen)
            iRet = Winks_httpappenddata( Winks_engineContext_p->http, 
                                         pProtPack->pData, 
                                         pProtPack->DataLen, 
                                         0 );
        else
        {
            Winks_assert(pProtPack->pData && pProtPack->DataLen);
            iRet = -2;
        }
        
        if(iRet < 0)
        {
            winks_EngHttpClose(Winks_engineContext_p->http);
            Winks_assert(iRet >= 0);
            return WINKS_RETURN_FAILURE;
        }

        if(httpstr.pContenttype)
        {
            iRet = Winks_httpsethead( Winks_engineContext_p->http, 
                                      HTTP_HEAD_CONTENTTYPE,
                                      httpstr.pContenttype );
            if(iRet>=0 && pProtPack->ContentLen)
            {
                char Buf[8];
                sprintf(Buf, "%lu", pProtPack->ContentLen);
                iRet = Winks_httpsethead( Winks_engineContext_p->http, 
                                          HTTP_HEAD_CONTENTLEN,
                                          Buf );
            }
            if(iRet < 0)
            {
                winks_EngHttpClose(Winks_engineContext_p->http);
                Winks_assert(iRet >= 0);
                return WINKS_RETURN_FAILURE;
            }
        }
    }

    /*启动传输。*/
    iRet = Winks_httpstart( Winks_engineContext_p->http );
    if(iRet == -1)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_assert(iRet != 0);
        return WINKS_RETURN_FAILURE;
    }

    Winks_engineContext_p->FrameNo++;
    
    Winks_assert(Winks_engineContext_p->httpTimer == WINKS_TIMER_NULL);
//    Winks_GetSupportedConnectWays(&cwo);
//    Winks_ConnectWaysAdjust(&cwo);
    //Winks_engineContext_p->httpTimer = Winks_CreateTimer(Winks_engineContext_p->engChn, WINKS_SECONDS2TICKS(cwo.twuTimeLimit/*WINKS_HTTP_TIME_LIMIT*/), WINKS_TIMER_AUTO_START);
    Winks_engineContext_p->httpTimer = Winks_CreateTimer(Winks_engineContext_p->engChn, WINKS_SECONDS2TICKS(winks_HttpTimeLimit()), WINKS_TIMER_AUTO_START);
    Winks_assert(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL);
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_EngCreateCcInfo(WinksCCDownCtxt_s *pDownCC, WinksStrgFile_CCInfo_s *pCCInfo)
{
    Winks_assert(pDownCC != NULL);
    Winks_assert(pCCInfo != NULL);
    
    Winks_mem_set(pCCInfo, 0, sizeof(WinksStrgFile_CCInfo_s));
    strncpy(pCCInfo->ID, pDownCC->wid, MIN(sizeof(pCCInfo->ID), strlen(pDownCC->wid)+1));
    pCCInfo->img_type = pDownCC->img_type;
    pCCInfo->img_cnt = pDownCC->media_num;
    pCCInfo->interval = pDownCC->interval;

    return WINKS_RETURN_SUCCESS;
}

#if 0
#define WINKS_UPLOAD_DIY_MAX    (200*1024)

unsigned long Winks_UploadDiyMax()
{
    unsigned long value;
    if( Winks_GetSettingValue(WINKS_SETTING_SHOW_CASE, &value)!=WINKS_RETURN_FAILURE &&
        value>0 )
    {
        return value;
    }
    return WINKS_UPLOAD_DIY_MAX;
}
#endif

static int winks_SaveSendData(WinksProt_PackData_s * pProtPack)
{
    int AllocSize = 0;
    int Off = 0;
    unsigned char * pAllocBuf = NULL;
    WinksProt_PackData_s * pProtPackCopy = NULL;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->pSaveSendData != NULL)
    {
        Winks_salFree(Winks_engineContext_p->pSaveSendData);
        Winks_engineContext_p->pSaveSendData = NULL;
    }
    
    AllocSize += WINKS_ALIGN(sizeof(WinksProt_PackData_s), 4);
    AllocSize += pProtPack->PathLen+2;
    AllocSize += pProtPack->DataLen;
    AllocSize += strlen(pProtPack->url)+1;
    if(pProtPack->pContenttype)
        AllocSize += strlen(pProtPack->pContenttype)+1;
    pAllocBuf = (unsigned char *)Winks_salAlloc(AllocSize);
    if(pAllocBuf == NULL)
    {
        Winks_printf("ERROR: Winks_salAlloc(%d)\r\n", AllocSize);
        return -1;
    }
    Winks_engineContext_p->pSaveSendData = pAllocBuf;

    Winks_mem_set(pAllocBuf, 0, AllocSize);
    Winks_mem_cpy(pAllocBuf, pProtPack, sizeof(WinksProt_PackData_s));
    pProtPackCopy = (WinksProt_PackData_s *)pAllocBuf;
    Off = WINKS_ALIGN(sizeof(WinksProt_PackData_s), 4);
    if(pProtPack->pPath)
    {
        pProtPackCopy->pPath = (unsigned char *)&pAllocBuf[Off];
        Winks_mem_cpy(pProtPackCopy->pPath, pProtPack->pPath, pProtPack->PathLen);
        Off += pProtPack->PathLen+2;
    }
    if(pProtPack->pData)
    {
        pProtPackCopy->pData = (char *)&pAllocBuf[Off];
        Winks_mem_cpy(pProtPackCopy->pData, pProtPack->pData, pProtPack->DataLen);
        Off += pProtPack->DataLen;
    }
    {
    pProtPackCopy->url = (char *)&pAllocBuf[Off];
    strcpy(pProtPackCopy->url, pProtPack->url);
    Off += strlen(pProtPack->url)+1;
    }
    if(pProtPack->pContenttype)
    {
        pProtPackCopy->pContenttype = (char *)&pAllocBuf[Off];
        strcpy(pProtPackCopy->pContenttype, pProtPack->pContenttype);
        Off += strlen(pProtPack->pContenttype)+1;
    }
    
    return 0;
}

static int winks_ReSendData()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_engineContext_p->ifHttpReady++;
    if(Winks_engineContext_p->pSaveSendData == NULL)
        return -1;

    return winks_SendData((WinksProt_PackData_s *)Winks_engineContext_p->pSaveSendData);
}

/* 协议请求: 请求引擎按指定的协议发送到服务器
输入:
    ProtocolNo  : 需要打包的协议号 
    indata      : 需要输入的参数(无参数可以为空)
    inLen       : 输入参数的长度(无参数可以为0)
输出:
返回:
    成功返回SUCCESS,失败返回错误码。
*/
int Winks_EngProtReq(WinksProtocolEnum_e ProtocolNo, void  * inData, int inLen)
{
    int iRet = 0;
    WinksProt_PackData_s * pProtPack = NULL;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->http != WINKS_HTTP_NULL)
        winks_EngHttpClose(Winks_engineContext_p->http);

#if 1
{
    unsigned long   allowFlag = 0;
    /*检查漫游标志:国际漫游不联网设置*/
    iRet = Winks_isRoamingConnectionAllowed((unsigned char *)&allowFlag);
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        return WINKS_MSG_ENG2UI_PROT_PACK;
    }
    if(allowFlag == 0)
    {
        return WINKS_MSG_ENG2UI_ROAMING_LIMIT;
    }
}
#endif
    
    iRet = Winks_ProtPack( ProtocolNo,
                           inData, inLen, &pProtPack);
    if(iRet<0 || pProtPack == NULL)
        return WINKS_MSG_ENG2UI_PROT_PACK;

#ifdef WINKS_DUAL_SIM
    if(Winks_api_SetNetLinkSim(Winks_GetActiveSim(), &Winks_engineContext_p->SavePhoneWapSimSel) != WINKS_RETURN_SUCCESS)
    {
        Winks_assert(Winks_GetActiveSim());
        return WINKS_MSG_ENG2UI_PROT_PACK;
    }
    Winks_engineContext_p->NeedRestoreWapSimSel = 1;
#endif /* WINKS_DUAL_SIM */

    Winks_engineContext_p->ifHttpReady = 0;
    iRet = winks_SendData(pProtPack);
    
    if(winks_SaveSendData(pProtPack) != 0)
        Winks_printf("winks_SaveSendData error!\r\n");
    else
        Winks_printf("winks_SaveSendData(0x%08x) return 0x%08x\r\n", \
            pProtPack, Winks_engineContext_p->pSaveSendData);
    
    Winks_ProtPackFree(pProtPack);

    if(iRet != WINKS_RETURN_SUCCESS)
        return WINKS_MSG_ENG2UI_PROT_PACK;
    Winks_engineContext_p->ProtNo = ProtocolNo;

    return WINKS_RETURN_SUCCESS;
}

/*
    读取电话簿
    调用前创建内存池pMemPool以便存储电话本，等打包发送完毕需要释放内存池
*/
int Winks_EngReadPhoneBook( Winks_MemPool_s * pMemPool,
                                       WinksProt_phoneBook_s ** ppphoneBook )
{
    int     iRet = 0;
    int     cnt = 0;
    unsigned long phone_cnt = 0, sim_cnt = 0;
    Winks_PhoneBookItem_s PBItem;
    
    WinksProt_phoneBook_s * pphoneBook = NULL;
    struct Prot_pbi_s * pbi = NULL;
    struct Prot_pbi_s * pbi_p = NULL;

    Winks_assert(pMemPool);
    Winks_assert(ppphoneBook);

    if(Winks_GetPhonebookCount(&phone_cnt, &sim_cnt)!= WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    if(!phone_cnt && !sim_cnt)/* 电话簿为空 */
    {
        return WINKS_RETURN_SUCCESS;
    }
    
    pphoneBook = Winks_EngMemPoolAlloc(pMemPool, sizeof(WinksProt_phoneBook_s), 4);
    if(pphoneBook == NULL)
        return WINKS_RETURN_FAILURE;

    pphoneBook->pbi_num = phone_cnt+sim_cnt;
    
    cnt = 0;
    while(cnt < pphoneBook->pbi_num)/* 逐个添加电话号码到到发送缓存 */
    {
        int iNo;

        Winks_mem_set(&PBItem, 0, sizeof(PBItem));
        if(cnt < (int)phone_cnt)
            iRet = Winks_ReadPhonebook(1, cnt, &PBItem);
        else
            iRet = Winks_ReadPhonebook(2, cnt-phone_cnt, &PBItem);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            break;
        }
        
        for(iNo=0; iNo<WINKS_MAX_NUMBER_COUNT && PBItem.number[iNo][0]; iNo++)
        {
            char    friend_name_utf[96];
            int     numberLen, namelen;
            
            numberLen = strlen(PBItem.number[iNo]);
            if((numberLen <= 0) || (numberLen >= WINKS_FRIEND_NO_LEN))/*无效的电话号码,忽略*/
                continue;
            numberLen += 1;
        #if 0
            Winks_printf("cnt:%02d iNo%d: %s\r\n",cnt, iNo, PBItem.number[iNo]);
        #endif
        
            pbi = Winks_EngMemPoolAlloc(pMemPool, sizeof(struct Prot_pbi_s), 4);
            if(pbi == NULL)
                break;
            if(pphoneBook->pbi == NULL)
                pphoneBook->pbi = pbi;
            else
            {
                pbi_p = pphoneBook->pbi;
                while(pbi_p && pbi_p->pnext)
                    pbi_p = pbi_p->pnext;
                pbi_p->pnext = pbi;
            }
            
            pbi->p = Winks_EngMemPoolAlloc(pMemPool, numberLen, 0);
            if(pbi->p == NULL)
                break;
            strcpy((char *)pbi->p, PBItem.number[iNo]);
            Winks_mem_set(friend_name_utf, 0, sizeof(friend_name_utf));    
            Winks_GetPhonebookName(PBItem.number[iNo], friend_name_utf, sizeof(friend_name_utf));
            if(friend_name_utf[0])
            {
                pbi->n = Winks_EngMemPoolAlloc(pMemPool, strlen(friend_name_utf)+1, 0);
                if(pbi->n == NULL)
                    break;
                strcpy((char *)pbi->n, friend_name_utf);
            }
        }
        cnt++;
    }

    if(cnt < pphoneBook->pbi_num)
        return WINKS_RETURN_FAILURE;

    if(ppphoneBook)
        *ppphoneBook = pphoneBook;
    
    return WINKS_RETURN_SUCCESS;
}

/*
    注册完毕调用，引擎需要置状态
*/
int Winks_EngRegistOk(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_engineContext_p->curState = WINKS_ENGINE_STATE_ENABLED;
    return WINKS_RETURN_SUCCESS;
}
/*
    取引擎当前的状态。
*/
int Winks_GetEngineCurrentState(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    return (int)Winks_engineContext_p->curState;
}
/*
    设置引擎当前的状态。
*/
int Winks_SetEngineCurrentState(int state)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    switch(state)
    {
        case WINKS_ENGINE_STATE_INIT:
        case WINKS_ENGINE_STATE_START:
        case WINKS_ENGINE_STATE_DISABLED:
        case WINKS_ENGINE_STATE_ENABLED:
        //case WINKS_ENGINE_STATE_SHOWA:
        //case WINKS_ENGINE_STATE_SHOWB:
            Winks_engineContext_p->curState = (Winks_EngineState_e)state;
            return WINKS_RETURN_SUCCESS;
        default:
            Winks_printf("Err:Unknow engine state.[0x%x]\r\n", state);
            return WINKS_RETURN_FAILURE;
    }
}
/*
    上传电话簿及下载片头运营商彩像结束，引擎启动定时更新
*/
int Winks_EngInitOk(void)
{
    /* 启动定时更新服务 */
    winks_StartPeriodPushTimer();

    Winks_EngCCCacheOk();
    
    return WINKS_RETURN_SUCCESS;
}

typedef struct tag_WinksCCDownCtxtX_s
{
    int     Step;   /* 获取一个号码的彩像步骤,GetCCStep_e */
    int     Pri;    /* 应用优先级 */
    int     AppType;/* 应用类型 */
    char  * AppName;
/* 原始输入 */
    WinksCCDownType_e CCDownType;     /*  */
    WinksStrList_s  * pPNoRoot;       /* 电话号码根列表 */
    WinksStrList_s  * pPNoCurr;       /* 当前电话号码 */
    unsigned short    Total;          /* 电话号码数 */
    unsigned short    Curr;           /* 当前第几个 */
/* 第1步输出结果 */
    WinksFileType_s * mediaurl;       /* 存放彩像的url */
    WinksFileType_s * thumburl;       /* 存放缩略图的url */
    unsigned char     media_num;      /* 彩像张数 */
    unsigned char     media_idx;      /* 彩像序列号 */
    unsigned char     thumbnail_num;  /* 缩略图数 */
    unsigned char     thumbnail_idx;  /* 缩略图序列号 */
    char              wid[WINKS_PROT_WID_MAX];/* 彩像ID */
    unsigned char     greet[202];     /* 问候语(utf8) */
    unsigned char     greetlen;       /* 问候语长度，当等于255表示删除当前问候语 */
    unsigned char     img_type;       /* 图片类型 WinksFileExt_e */
    unsigned short    interval;       /* 图片播放间隔 */
    int               status;         /* 我的状态 */
    unsigned char     statustxt[100]; /* 我的状态字符串(utf8) */
    int               statustxtlen;   /* 我的状态字符串长 */
/* 保存文件需要的数据 */
    unsigned short    CCIdx;          /* 彩像ID */
    unsigned short    SaveIdx;        /* 保存文件的序列号，第1次保存需初始化CCImage，再打开文件句柄，
                                         保存完一个文件后关闭文件句柄SaveIdx置0 */
    WinksStrgFile_CCImage_s CCImage;      /* 供存储文件使用的文件信息 */
    
    Winks_MemPool_s * pMemPool;     /* 存放号码列表 */
}WinksCCDownCtxtX_s;

static WinksStrList_s * winks_AddToPhoneNoList( 
                                        Winks_MemPool_s* pMemPool,
                                        WinksStrList_s * pPNoPrev,
                                        char * PhoneNumber )
{
    WinksStrList_s * pPNo = NULL;
    pPNo = (WinksStrList_s *)Winks_EngMemPoolAlloc(pMemPool, sizeof(WinksStrList_s)+strlen(PhoneNumber)+1, 4);
    if(pPNo)
    {
        pPNo->str = (char *)(pPNo+1);
        strcpy(pPNo->str, PhoneNumber);

        if(pPNoPrev)
            pPNoPrev->pnext = pPNo;
        //pPNoPrev = pPNo;
    }
    
    return pPNo;
}

static int winks_AddFriend2List( Winks_MemPool_s* pMemPool,
                                       WinksStrList_s * pPNoList,
                                       WinksStrList_s ** ppPNoLast )
{
    int BeginIdx = 0;
    int ValidCnt = 0;
    char    PhoneNumber[WINKS_STRG_PHONE_LEN];
    WinksStrList_s * pPNoNew = NULL;

    pPNoNew = pPNoList;
    while(pPNoNew && pPNoNew->pnext)
        pPNoNew = pPNoNew->pnext;
    
    while((BeginIdx = Winks_StrgFriendGet(BeginIdx, PhoneNumber, sizeof(PhoneNumber), NULL, 0)) > 0)
    {
        pPNoNew = winks_AddToPhoneNoList(pMemPool, pPNoNew, PhoneNumber);
        if(pPNoNew == NULL)
            break;
        if(ppPNoLast)
            *ppPNoLast = pPNoNew;
        ValidCnt++;
    }

    return ValidCnt;
}

static int winks_AddPriBus2List( Winks_MemPool_s* pMemPool,
                                       WinksStrList_s * pPNoList,
                                       WinksStrList_s ** ppPNoLast )
{
    int ValidCnt = 0;
    WinksStrList_s * pPNoNew = NULL;

    pPNoNew = pPNoList;
    while(pPNoNew && pPNoNew->pnext)
        pPNoNew = pPNoNew->pnext;

    for(ValidCnt=0; ValidCnt<2; ValidCnt++)
    {
        char *pno;
        if(!ValidCnt)
            pno = WINKS_PRI_CC_NO;
        else
            pno = WINKS_BUS_CC_NO;
            
        pPNoNew = winks_AddToPhoneNoList(pMemPool, pPNoNew, pno);
        if(pPNoNew == NULL)
            break;
        
        if(ppPNoLast)
            *ppPNoLast = pPNoNew;
    }

    return ValidCnt;
}

static int winks_AddRecruit2List( Winks_MemPool_s* pMemPool,
                                       WinksStrList_s * pPNoList,
                                       WinksStrList_s ** ppPNoLast )
{
    int BeginIdx = 0;
    int ValidCnt = 0;
    char    PhoneNumber[WINKS_STRG_PHONE_LEN];
    WinksStrList_s * pPNoNew = NULL;

    pPNoNew = pPNoList;
    while(pPNoNew && pPNoNew->pnext)
        pPNoNew = pPNoNew->pnext;
    
    while((BeginIdx = Winks_StrgULGet(BeginIdx, PhoneNumber)) > 0)
    {
        pPNoNew = winks_AddToPhoneNoList(pMemPool, pPNoNew, PhoneNumber);
        if(pPNoNew == NULL)
            break;
        if(ppPNoLast)
            *ppPNoLast = pPNoNew;
        ValidCnt++;
    }

    return ValidCnt;
}

static int winks_AddCall2List( Winks_MemPool_s* pMemPool,
                                       WinksStrList_s * pPNoList,
                                       WinksStrList_s ** ppPNoLast )
{
    int ValidCnt = 0;
    WinksStrList_s * pPNoNew = NULL;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    pPNoNew = pPNoList;
    while(pPNoNew && pPNoNew->pnext)
        pPNoNew = pPNoNew->pnext;
    
    if(Winks_engineContext_p->pCall->NO[0] != '\0')
    {
        pPNoNew = winks_AddToPhoneNoList(pMemPool, pPNoNew, Winks_engineContext_p->pCall->NO);
        if(pPNoNew == NULL)
            return ValidCnt;
        if(ppPNoLast)
            *ppPNoLast = pPNoNew;
        ValidCnt++;
    }

    return ValidCnt;
}

static int winks_EngGetCtxt_Friend(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, Winks_MemPool_s * pMemPool)
{
    int ValidCnt = 0;
    
    ValidCnt = winks_AddFriend2List(pMemPool, NULL, NULL);
    pCCDownCtxtX->pPNoRoot = (WinksStrList_s *)pMemPool->pdata;
    
    return ValidCnt;
}

static int winks_EngGetCtxt_Period(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, Winks_MemPool_s * pMemPool)
{
    int ValidCnt = 0;
    WinksStrList_s * pPNoNew = NULL;
    WinksStrList_s * pPNoLast = NULL;
    
    ValidCnt = winks_AddPriBus2List(pMemPool, NULL, &pPNoNew);
    if(ValidCnt <= 0)
    {
        Winks_EngMemPoolDestory(pMemPool);
        return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_StrgFriendCnt() > 0)
    {
        ValidCnt += winks_AddFriend2List(pMemPool, pPNoNew, &pPNoLast);
    }

  {
/* 如果选择定时更新+挂断后不能更新，则，更新亲密好友数据+电话事件优先的用户的彩像数据。*/
    unsigned long value = 0;
    
    if( Winks_GetSettingValue(WINKS_SETTING_HANGUP_UPDATA_SWITCH, &value) == WINKS_RETURN_SUCCESS &&
        value ==0 )
    {
        if(Winks_StrgULCnt() > 0)
        {
            ValidCnt += winks_AddRecruit2List(pMemPool, pPNoNew, &pPNoLast);
        }
    }
  }
    
    pCCDownCtxtX->pPNoRoot = (WinksStrList_s *)pMemPool->pdata;
    
    return ValidCnt;
}

static int winks_EngGetCtxt_Recruit(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, Winks_MemPool_s * pMemPool)
{
    int ValidCnt = 0;
    
    ValidCnt = winks_AddRecruit2List(pMemPool, NULL, NULL);
    pCCDownCtxtX->pPNoRoot = (WinksStrList_s *)pMemPool->pdata;

    return ValidCnt;
}

static int winks_EngGetCtxt_Init(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, Winks_MemPool_s * pMemPool)
{
    int ValidCnt = 0;
    
    ValidCnt = winks_AddPriBus2List(pMemPool, NULL, NULL);
    pCCDownCtxtX->pPNoRoot = (WinksStrList_s *)pMemPool->pdata;
    
    return ValidCnt;
}

static int winks_EngGetCtxt_Call(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, Winks_MemPool_s * pMemPool)
{
    int     ValidCnt = 0;
    WinksStrList_s * pPNoNew = NULL;
    
    ValidCnt = winks_AddCall2List(pMemPool, NULL, &pPNoNew);
    pCCDownCtxtX->pPNoRoot = (WinksStrList_s *)pMemPool->pdata;

    return ValidCnt;
}

static int winks_EngGetCtxt_Number(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, char *pNo, Winks_MemPool_s * pMemPool)
{
    int     ValidCnt = 0;
    WinksStrList_s * pPNoNew = NULL;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(pNo && pNo[0] != '\0')
    {
        pPNoNew = winks_AddToPhoneNoList(pMemPool, pPNoNew, pNo);
        if(pPNoNew == NULL)
            return ValidCnt;
        ValidCnt++;
    }
    pCCDownCtxtX->pPNoRoot = (WinksStrList_s *)pMemPool->pdata;

    return ValidCnt;
}


#define SORTED_LIST_FOR_BSEARCH
#define DELETE_UNUSED_IMAGE

/* 该结构与WinksStrList_s相同大小，排序好后将Power转化为pnext即可 */
typedef struct tag_WinksCSList_s
{
	unsigned long Power;
	char  * pno;    /* 电话号码 */
}WinksCSList_s;

int winks_SearchCSPos( WinksCSList_s ** ppCSArr,
                              int     Count,
                              unsigned long Power )

{
    int pos;
    int min, max, mid=0;

    if(!Count)
        return 0;
    min = 0;
    max = Count-1;

#ifdef SORTED_LIST_FOR_BSEARCH
    if(Power > ppCSArr[min]->Power)
        return min;
    else if(Power <= ppCSArr[max]->Power)
        return max+1;
    while(min<max)
    {
        mid = (min+max)>>1;

        if(Power == ppCSArr[mid]->Power)
            return mid;
        else if(Power < ppCSArr[mid]->Power)
            min = mid+1;
        else
            max = mid-1;
    }

	/* 从min,mid,max中找出最小值和最大值 */
	pos = min;
    
	if(min > mid)
		min = mid;
	if(min > max)
		min = max;
    
	if(max < pos)
		max = pos;
	if(max < mid)
		max = mid;

#endif /* SORTED_LIST_FOR_BSEARCH */
    for(pos=min; pos<=max; pos++)
    {
        if(ppCSArr[pos]->Power < Power)
            break;
    }
	return pos;
}

static WinksCSList_s * winks_AddToCSList( 
                                        Winks_MemPool_s * pMemPool,
                                        char  * PhoneNumber,
                                        unsigned long Power )
{
    WinksCSList_s * pPNo = NULL;

    pPNo = (WinksCSList_s *)Winks_EngMemPoolAlloc(pMemPool, sizeof(WinksCSList_s)+strlen(PhoneNumber)+1, 4);
    if(pPNo)
    {
        pPNo->pno = (char *)(pPNo+1);
        strcpy(pPNo->pno, PhoneNumber);
        pPNo->Power = Power;
    }
    
    return pPNo;
}

static int winks_EngGetCtxt_CCCache(WinksCCDownCtxtX_s * pCCDownCtxtX, 
                                    int TotalCnt, Winks_MemPool_s * pMemPool)
{
    WinksCSList_s * pCSArr[WINKS_MAX_USER];
    int Cnt, ValidCnt, i;
    WinksStrgFile_CallStat_s CallStat;
#ifdef DELETE_UNUSED_IMAGE
    int     CCLeftCnt;
    unsigned short CCIdxLeft[WINKS_MAX_USER];
#endif /* DELETE_UNUSED_IMAGE */

    Winks_assert(pCCDownCtxtX);
    
    for(i=0; i<WINKS_MAX_USER; i++)
        pCSArr[i] = NULL;

#ifdef DELETE_UNUSED_IMAGE
    CCLeftCnt = 0;
    Winks_mem_set(CCIdxLeft, 0, sizeof(CCIdxLeft));
#else
    /* 存储卡上彩像全部删除 */
    Winks_StrgDeleteDiskFiles(2);
#endif /* DELETE_UNUSED_IMAGE */
    
    Cnt = 0;
    ValidCnt = 0;
    while(Cnt < TotalCnt)
    {
        int Pos;
        WinksCSList_s * pCSNew = NULL;
        unsigned long   Power;
        char            PhoneNo[WINKS_STRG_PHONE_LEN];
        unsigned short  UserIdx;
        WinksStrgFile_UserInfo_s UserInfo;
        
        Winks_StrgCallEvtGet((unsigned short)Cnt, &CallStat);
        Winks_PhoneNoBcd2Str((unsigned char *)CallStat.NoBcd, WINKS_STRG_NOBCD_LEN, 
                                 sizeof(PhoneNo), PhoneNo);
        if(PhoneNo[0] == 0)
        {
            /* 号码无效 */
            Cnt++;
            continue;
        }
        Winks_mem_set(&UserInfo, 0, sizeof(UserInfo));
        if(Winks_StrgSearchUser(CallStat.NoBcd, &UserIdx, &UserInfo) != WINKS_STRG_SEARCH_RESULT_FOUND)
        {
            /* 不是原来的彩像用户 */
            Cnt++;
            continue;
        }
        
        /* 号码对应的彩像存在，可不考虑 */
        if( Winks_StrgCCIsExist(UserInfo.CCIdx, NULL) 
        #ifdef DELETE_UNUSED_IMAGE
            && CCLeftCnt<WINKS_MAX_USER
        #endif
            )
        {
        #ifdef DELETE_UNUSED_IMAGE
            CCIdxLeft[CCLeftCnt++] = UserInfo.CCIdx;
        #endif
            Cnt++;
            continue;
        }

        if(UserInfo.GrtOff)
        {
            WinksStrgFile_GreetInfo_s GreetInfo;
            /* 删除问候语 */
            if(Winks_StrgGetGreet(UserInfo.GrtOff, &GreetInfo) == WINKS_RETURN_SUCCESS)
            {
                Winks_mem_set(&GreetInfo.NoBcd, 0, WINKS_STRG_NOBCD_LEN);
                Winks_StrgSetGreet(UserInfo.GrtOff, &GreetInfo);
            }
        }
        /* 删除用户信息，与彩像问候语断链 */
        Winks_mem_set(&UserInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
        Winks_StrgSetUserInfo(UserIdx, &UserInfo);

        Power = Winks_CalculatePower(&CallStat);
        pCSNew = winks_AddToCSList(pMemPool, PhoneNo, Power);
        if(pCSNew == NULL)
            break;
        Pos = winks_SearchCSPos(pCSArr, ValidCnt, Power);
        Winks_assert(Pos < WINKS_MAX_USER)/* 该号码很久没有通电话，可不考虑 */
        /* 移动后面数据，插入新号码 */
        for(i=ValidCnt-1; i>=Pos; i--)
        {
            if(i+1<WINKS_MAX_USER)
            {
                pCSArr[i+1] = pCSArr[i];
            }
        }
        pCSArr[Pos] = pCSNew;
        
        if(ValidCnt<WINKS_MAX_USER)
            ValidCnt++;
            
        Cnt++;
    }

#ifdef WINKS_DEBUG
{
    Winks_printf("Winks_EngCCCache ValidCnt=%d \r\n", ValidCnt);
    Winks_printf("CNT : POWER      PHONENUMBER \r\n");
    for(i=0; i<ValidCnt; i++)
    {
        Winks_printf("%4d: %8lu, %s\r\n", Cnt, pCSArr[i]->Power, pCSArr[i]->pno);
    }
}
#endif
    for(i=0; i<ValidCnt; i++)
    {
        WinksStrList_s * pPNo = (WinksStrList_s *)pCSArr[i];
        pPNo->pnext = (WinksStrList_s *)pCSArr[i+1];
    }
    if(pCSArr[i])
        pCSArr[i]->Power = 0;

#ifdef DELETE_UNUSED_IMAGE
/* 删除没用的彩像 */
    if(CCLeftCnt)
    {
        Winks_printf("winks_StrgDeleteUnUsedCC CCLeftCnt=%d\r\n", CCLeftCnt);
        winks_StrgDeleteUnUsedCC(CCIdxLeft, CCLeftCnt);
    }
#endif /* DELETE_UNUSED_IMAGE */

    return ValidCnt;
}

int Winks_EngCCDownCtxtCreate(WinksCCDownType_e CCDownType, char *pNo, WinksCCDownCtxt_s ** ppCCDownCtxt)
{
    int     TotalCnt = 0;
    int     ValidCnt = 0;
    Winks_MemPool_s * pMemPool = NULL;
    WinksCCDownCtxtX_s * pCCDownCtxtX = NULL;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(CCDownType>=0 && CCDownType<WINKS_CCDOWN_TYPE_MAX);
    Winks_assert(ppCCDownCtxt);

    if(ppCCDownCtxt)
        *ppCCDownCtxt = (WinksCCDownCtxt_s *)NULL;
    switch(CCDownType)
    {
    case WINKS_CCDOWN_TYPE_FRIEND:
        TotalCnt = Winks_StrgFriendCnt();
        break;
    case WINKS_CCDOWN_TYPE_CCCACHE:
        TotalCnt = Winks_StrgCallEvtCnt();
        break;
    case WINKS_CCDOWN_TYPE_RECRUIT:
        TotalCnt = Winks_StrgULCnt();
        break;
    case WINKS_CCDOWN_TYPE_PERIOD:
    case WINKS_CCDOWN_TYPE_INIT:
        TotalCnt = 1;
        break;
    case WINKS_CCDOWN_TYPE_CALL:
        if(Winks_engineContext_p->pCall->NO[0] == '\0')
            return 0;
        TotalCnt = 1;
        break;
    case WINKS_CCDOWN_TYPE_NUMBER:
        Winks_assert(pNo);
        TotalCnt = 1;
        break;
    default:
        break;
    }
        
    if(TotalCnt == 0)
        return 0;
    else if(TotalCnt < 0)
        return -1;
        
    pCCDownCtxtX = (WinksCCDownCtxtX_s *)Winks_salAlloc(sizeof(WinksCCDownCtxtX_s));
    if(pCCDownCtxtX == NULL)
        return -1;

    Winks_mem_set(pCCDownCtxtX, 0, sizeof(WinksCCDownCtxtX_s));
    
    if(Winks_EngMemPoolCreate(&pMemPool) == NULL)
    {
        return -1;
    }
    
    switch(CCDownType)
    {
    case WINKS_CCDOWN_TYPE_FRIEND:
        ValidCnt = winks_EngGetCtxt_Friend(pCCDownCtxtX, TotalCnt, pMemPool);
        break;
    case WINKS_CCDOWN_TYPE_PERIOD:
        ValidCnt = winks_EngGetCtxt_Period(pCCDownCtxtX, TotalCnt, pMemPool);
        break;
    case WINKS_CCDOWN_TYPE_CCCACHE:
        ValidCnt = winks_EngGetCtxt_CCCache(pCCDownCtxtX, TotalCnt, pMemPool);
        break;
    case WINKS_CCDOWN_TYPE_RECRUIT:
        ValidCnt = winks_EngGetCtxt_Recruit(pCCDownCtxtX, TotalCnt, pMemPool);
        break;
    case WINKS_CCDOWN_TYPE_INIT:
        ValidCnt = winks_EngGetCtxt_Init(pCCDownCtxtX, TotalCnt, pMemPool);
        break;
    case WINKS_CCDOWN_TYPE_CALL:
        ValidCnt = winks_EngGetCtxt_Call(pCCDownCtxtX, TotalCnt, pMemPool);
        break;
    case WINKS_CCDOWN_TYPE_NUMBER:
        ValidCnt = winks_EngGetCtxt_Number(pCCDownCtxtX, TotalCnt, pNo, pMemPool);
        break;
    default:
        break;
    }

    pCCDownCtxtX->pMemPool = pMemPool;
    pCCDownCtxtX->Total = (unsigned short)ValidCnt;
    
    if(ppCCDownCtxt)
        *ppCCDownCtxt = (WinksCCDownCtxt_s *)pCCDownCtxtX;
    
    return 0;
}

int Winks_EngCCDownCtxtDestory(WinksCCDownCtxt_s * pCCDownCtxt)
{
    WinksCCDownCtxtX_s * pCCDownCtxtX = (WinksCCDownCtxtX_s *)pCCDownCtxt;
    
    if(pCCDownCtxtX)
    {
        if(pCCDownCtxtX->pMemPool)
            Winks_EngMemPoolDestory(pCCDownCtxtX->pMemPool);
        else if(pCCDownCtxtX->pPNoRoot)
            Winks_salFree(pCCDownCtxtX->pPNoRoot);

        if(pCCDownCtxtX->mediaurl)
            Winks_salFree(pCCDownCtxtX->mediaurl);
        if(pCCDownCtxtX->thumburl)
            Winks_salFree(pCCDownCtxtX->thumburl);

        Winks_salFree(pCCDownCtxtX);
    }
    
    return WINKS_RETURN_SUCCESS;
}

int Winks_EngCCDownCtxtSaveUrl( WinksFileType_s ** ppurlList, 
                                           WinksFileType_s * pWinksUrl )
{
    int     Cnt = 0;
    
    if(ppurlList && pWinksUrl)
    {
        int Off, Size = 0;
        char  * pBuf = NULL;
        WinksFileType_s * purlList_pre = NULL;
        WinksFileType_s * purlList = NULL;
        WinksFileType_s * pTmpUrl = pWinksUrl;
        
        while(pTmpUrl)
        {
            Size += sizeof(WinksFileType_s) + strlen(pTmpUrl->url) + 1;
            Size = WINKS_ALIGN(Size, 4);
            pTmpUrl = pTmpUrl->pnext;
        }
        pBuf = (char *)Winks_salAlloc(Size);
        if(pBuf == NULL)
            return WINKS_RETURN_FAILURE;

        Winks_mem_set(pBuf, 0, Size);
        *ppurlList = (WinksFileType_s *)pBuf;
        pTmpUrl = pWinksUrl;
        Off=0;
        while(pTmpUrl)
        {
            if( winks_IsSupportedDim((unsigned short)pTmpUrl->w, (unsigned short)pTmpUrl->h) &&
                winks_IsSupportedExt(pTmpUrl->url) )
            {
                purlList = (WinksFileType_s *)&pBuf[Off];
                purlList->w = pTmpUrl->w;
                purlList->h = pTmpUrl->h;
                purlList->url = (char *)(purlList+1);
                strcpy(purlList->url, pTmpUrl->url);
                Off += sizeof(WinksFileType_s) + strlen(pTmpUrl->url) + 1;
                Off = WINKS_ALIGN(Off, 4);
                if(purlList_pre)
                    purlList_pre->pnext = purlList;
                purlList_pre = purlList;
            }
            pTmpUrl = pTmpUrl->pnext;
            Cnt++;
        }
    }
    
    return Cnt;
}

int Winks_EngCCDownCtxtSaveVCard(VcardType_s * vcard) 
{
    return WINKS_RETURN_SUCCESS;
}

static int winks_EngSaveGreet( WinksCCDownCtxt_s *pDownCC,
                                      unsigned short UsrIdx,
                                      WinksStrgFile_UserInfo_s *pUsrInfo )
{
    int iRet;
    int NeedUpdateGreet = 2; /* 2--SearchGreetEpt then save, 1--only save, 0--none */
    WinksStrgFile_GreetInfo_s GreetInfo;

    Winks_mem_set(&GreetInfo, 0, sizeof(WinksStrgFile_GreetInfo_s));
    if(pUsrInfo->GrtOff == 0)
    {
        if( pDownCC->greetlen == 0 || pDownCC->greetlen == 0xff )
            return WINKS_RETURN_SUCCESS;
    }
    else if(Winks_StrgGetGreet(pUsrInfo->GrtOff, &GreetInfo) == WINKS_RETURN_SUCCESS)
    {
        if( pDownCC->greetlen == 0 || pDownCC->greetlen == 0xff )
        {
            /* 问候语为空，跳过 */
            NeedUpdateGreet = 0;
            
            /* 原先有问候语，删除 */
            if(GreetInfo.GreetLen)
            {
                Winks_mem_set(GreetInfo.NoBcd, 0, WINKS_STRG_NOBCD_LEN);
                Winks_StrgSetGreet(pUsrInfo->GrtOff, &GreetInfo);
            }
        }
        else if( pDownCC->greetlen == GreetInfo.GreetLen &&
                (memcmp(GreetInfo.GreetStr, pDownCC->greet, pDownCC->greetlen) == 0) )
        {
                /* 问候语没有变，跳过 */
                NeedUpdateGreet = 0;
        }
        else
        {
            int     Occupy = GreetInfo.Occupy;
            Occupy -= sizeof(GreetInfo) - sizeof(GreetInfo.GreetStr);
            if(Occupy >= pDownCC->greetlen)
            {
                /* 原先有问候语，且原存储位置能容纳目前问候语 */
                NeedUpdateGreet = 1;
            }
            else
            {
                /* 删除问候语，准备替换 */
                Winks_mem_set(GreetInfo.NoBcd, 0, WINKS_STRG_NOBCD_LEN);
                Winks_StrgSetGreet(pUsrInfo->GrtOff, &GreetInfo);
            }
        }
    }

    if(NeedUpdateGreet == 0)
        return WINKS_RETURN_SUCCESS;
    else if(NeedUpdateGreet == 2)
    {
        /* 查找问候语空位 */
        if(Winks_StrgSearchGreetEpt(pDownCC->greetlen, &pUsrInfo->GrtOff) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
        {
            /*  整理问候语文件,以便腾出空间;然后继续找出空闲空间 */
            if(Winks_StrgTrimGreet() != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;/* 文件系统错误 */

            /* 整理完问候语文件,还是没地方存放问候语,放弃 */
            if(Winks_StrgSearchGreetEpt(pDownCC->greetlen, &pUsrInfo->GrtOff) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
                return WINKS_RETURN_FAILURE;    /* 磁盘满不能存储问候语 */
        }
    }
    
    /* 存储问候语 */
    if(pDownCC->greetlen && pUsrInfo->GrtOff)
    {
        unsigned char NoBcd[WINKS_STRG_NOBCD_LEN];
        Winks_PhoneNoStr2Bcd(pDownCC->pPNoCurr->str, strlen(pDownCC->pPNoCurr->str), WINKS_STRG_NOBCD_LEN, NoBcd);
        Winks_mem_set(GreetInfo.GreetStr, 0, GreetInfo.GreetLen);
        Winks_mem_cpy(GreetInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
        GreetInfo.GreetLen = pDownCC->greetlen;
        Winks_mem_cpy(GreetInfo.GreetStr, pDownCC->greet, pDownCC->greetlen);
        if(Winks_StrgSetGreet(pUsrInfo->GrtOff, &GreetInfo)== WINKS_RETURN_SUCCESS)
        {
            return Winks_StrgSetUserInfo(UsrIdx, pUsrInfo);
        }
        else
            return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/* 查找号码及彩像信息，看是否需要下载彩像
   返回: 1--需要下载彩像, 0--不需要，-1--错误 */
int Winks_EngSearchCCInfo(WinksCCDownCtxt_s *pDownCC, int NeedWashOut)
{
    int NeedUpdateCC = 1;    /* 1--need down cc, 0--need not down, save curr cc*/
    unsigned short retIdx = 0;
    unsigned char NoBcd[WINKS_STRG_NOBCD_LEN];
    WinksStrgFile_UserInfo_s UsrInfo;

    Winks_assert(pDownCC);
    Winks_assert(pDownCC->pPNoCurr);
    Winks_assert(pDownCC->pPNoCurr->str);

    /* 判断是否需要下载彩像 */
    Winks_mem_set(&UsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    if(pDownCC->wid[0] == 0)
        /* 彩像为空，跳过 */
        NeedUpdateCC = 0;
    else
    {
        Winks_PhoneNoStr2Bcd(pDownCC->pPNoCurr->str, strlen(pDownCC->pPNoCurr->str), WINKS_STRG_NOBCD_LEN, NoBcd);
        if(Winks_StrgSearchUser((char *)NoBcd, &retIdx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND)
        {
            WinksStrgFile_CCInfo_s CCInfo;
            Winks_mem_set(&CCInfo, 0, sizeof(WinksStrgFile_CCInfo_s));
            if(Winks_StrgCCIsExist(UsrInfo.CCIdx, &CCInfo))
            {
                if(strcmp(pDownCC->wid, CCInfo.ID) == 0)
                {
                    Winks_printf("Winks_EngSearchCCInfo OKOK : user cc not changed! (%d, 0x%04x)\r\n", retIdx, UsrInfo.CCIdx);
                    /* 彩像没有更新，跳过 */
                    NeedUpdateCC = 0;
                }
            }
        }
        
        if( NeedUpdateCC &&
            Winks_StrgSearchCC(pDownCC->wid, &UsrInfo.CCIdx) == WINKS_STRG_SEARCH_RESULT_FOUND &&
            Winks_StrgCCIsExist(UsrInfo.CCIdx, NULL) )
        {
            /* 彩像已存在 */
            Winks_printf("Winks_EngSearchCCInfo OKOK : user cc changed!, but new cc exist (%d, 0x%04x)\r\n", retIdx, UsrInfo.CCIdx);
            NeedUpdateCC = 0;
        }
    }

    /* 需要下载彩像，一切等到下载彩像时处理 */
    if(NeedUpdateCC)
        return 1;

    /* 有彩像，但没有该用户，创建用户指向该彩像，保存 */
    if(retIdx == 0)
    {
        Winks_assert(UsrInfo.CCIdx);/* 彩像肯定已找到 */
        /* 查找空位存储当前用户 */
        if(Winks_StrgSearchUserEpt(&retIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
        /* 没有地方存放该用户 */
        {
            if(NeedWashOut)
            {
                if(Winks_StrgFreeDiskSpace(EACH_CC_TAKEUP_SIZE) != WINKS_RETURN_SUCCESS)
                    return WINKS_RETURN_FAILURE;
                if(Winks_StrgSearchUserEpt(&retIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
                    return WINKS_RETURN_FAILURE;/* 无法添加该用户 */
            }
            else
                return WINKS_RETURN_FAILURE;/* 用户数超限 */
        }
        Winks_mem_cpy(UsrInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
    }
    if( pDownCC->wid[0] && 
        Winks_StrgSetUserInfo(retIdx, &UsrInfo) != WINKS_RETURN_SUCCESS )
        return WINKS_RETURN_FAILURE;

    pDownCC->CCIdx = UsrInfo.CCIdx;
    return winks_EngSaveGreet(pDownCC, retIdx, &UsrInfo);
}

/* 收到下载彩像的第1块时调用，
   成功返回彩像IDX */
int Winks_EngSaveCCInfo(WinksCCDownCtxt_s *pDownCC, int NeedWashOut)
{
    int iRet;
    unsigned short retIdx = 0;
    unsigned char  NoBcd[WINKS_STRG_NOBCD_LEN];
    WinksStrgFile_UserInfo_s  UsrInfo;
    WinksStrgFile_CCInfo_s    CCInfo;
    WinksStrgFile_GreetInfo_s GreetInfo;
    unsigned long FileSize;

    Winks_assert(pDownCC);
    Winks_assert(pDownCC->pPNoCurr);
    Winks_assert(pDownCC->pPNoCurr->str);
    
    if(pDownCC->wid[0] == 0 || pDownCC->CCImage.FileSize == 0)
    {
        Winks_assert(pDownCC->wid[0]);
        Winks_assert(pDownCC->CCImage.FileSize);
        return WINKS_RETURN_FAILURE;
    }

    Winks_mem_set(&UsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    Winks_mem_set(&GreetInfo, 0, sizeof(WinksStrgFile_GreetInfo_s));
    Winks_PhoneNoStr2Bcd(pDownCC->pPNoCurr->str, strlen(pDownCC->pPNoCurr->str), WINKS_STRG_NOBCD_LEN, NoBcd);
    if(Winks_StrgSearchUser((char *)NoBcd, &retIdx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND)
    {
        
        Winks_mem_set(&CCInfo, 0, sizeof(WinksStrgFile_CCInfo_s));
        if(Winks_StrgCCIsExist(UsrInfo.CCIdx, &CCInfo))
        {
        #if 1
            if(strcmp(pDownCC->wid, CCInfo.ID) == 0)
            {
                Winks_fail("I'm crazy! why SearchCCInfo not found?\r\n");
            }
            else
        #endif
            {
                /* 尝试删除不用的彩像 */
                if(Winks_StrgCCUserNum(UsrInfo.CCIdx, NULL) <= 1)
                    Winks_StrgDeleteCCImage(UsrInfo.CCIdx);
            }
        }
    }
    else
    {
        if(Winks_StrgSearchUserEpt(&retIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
        /* 没有地方存放该用户 */
        {
            if(NeedWashOut)
            {
                if(Winks_StrgFreeDiskSpace(EACH_CC_TAKEUP_SIZE) != WINKS_RETURN_SUCCESS)
                    return WINKS_RETURN_FAILURE;
                if(Winks_StrgSearchUserEpt(&retIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
                    return WINKS_RETURN_FAILURE;/* 无法添加该用户 */
            }
            else
                return WINKS_RETURN_FAILURE;/* 用户数超限 */
        }
        Winks_mem_cpy(UsrInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
    }

    if(Winks_StrgSearchCC(pDownCC->wid, &UsrInfo.CCIdx) == WINKS_STRG_SEARCH_RESULT_FOUND)
    {
#if 1
        if(Winks_StrgCCIsExist(UsrInfo.CCIdx, NULL))
		{
            Winks_fail("I'm crazy! why Winks_StrgCCIsExist?");
		}
        else
#endif
        Winks_StrgDeleteCCImage(UsrInfo.CCIdx);
    }
    /* 查找存储彩像信息空位 */
    FileSize = MAX(EACH_CC_TAKEUP_SIZE, pDownCC->CCImage.FileSize);
    if(Winks_StrgSearchCCEpt(FileSize, &UsrInfo.CCIdx) 
            != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
    {
        if(NeedWashOut)
        {
            if(Winks_StrgFreeDiskSpace(FileSize) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
            if(Winks_StrgSearchCCEpt(FileSize, &UsrInfo.CCIdx) 
                    != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY)
                return WINKS_RETURN_FAILURE;    /* 磁盘满不能存储彩像 */
        }
        else
            return WINKS_RETURN_FAILURE;    /* 磁盘满不能存储彩像 */
    }
    
    /* 存储彩像基本信息和用户与彩像问候语关联信息 */
    iRet = winks_EngCreateCcInfo(pDownCC, &CCInfo);
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    iRet = Winks_StrgSetCCInfo(UsrInfo.CCIdx, &CCInfo);
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    
    Winks_mem_cpy(UsrInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
    iRet = Winks_StrgSetUserInfo(retIdx, &UsrInfo);
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);

    pDownCC->CCIdx = UsrInfo.CCIdx;
    return winks_EngSaveGreet(pDownCC, retIdx, &UsrInfo);
}

/* 当下载通话号码的彩像后调用该函数 */
int Winks_EngUpdateCCShow(void)
{
#ifdef WINKS_SUPPORT_3G
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(Winks_engineContext_p->pCall);
    
    Winks_engineContext_p->pCall->CallCCStatus = WINKS_CALLCC_STATUS_OK;
    if(Winks_engineContext_p->pCall)
    {
        if(Winks_StrgGetCCShow( Winks_engineContext_p->pCall->NO, Winks_engineContext_p->pCall->CallType, 
                                &Winks_engineContext_p->pCall->CCShow ) == WINKS_RETURN_SUCCESS)
        {
            Winks_StrgUnlockCC();
            Winks_StrgLockCC(Winks_engineContext_p->pCall->CCShow.CC_Idx);
            return WINKS_RETURN_SUCCESS;
        }
    }
#endif
    return WINKS_RETURN_FAILURE;
}

/* 当下载通话号码的彩像失败 */
int Winks_EngUpdateCCShowFail(void)
{
#ifdef WINKS_SUPPORT_3G
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(Winks_engineContext_p->pCall);
    
    Winks_engineContext_p->pCall->CallCCStatus = WINKS_CALLCC_STATUS_FAIL;
    winks_StopCallWaitTimer();
    Winks_StrgULAdd(Winks_engineContext_p->pCall->NO);
#endif
	return 0;
}

/* 当彩像Cache创建成功调用 */
int Winks_EngCCCacheOk(void)
{
    int iRet = WINKS_RETURN_FAILURE;
    char    buff[16];
    unsigned long   CacheTime = 0;
    Winks_DateTime_s    v_dateTime;
    
    iRet = Winks_GetSysDateTime(&v_dateTime);

    Winks_mem_set(buff, 0, sizeof(buff));
    iRet = Winks_GetImsi(WINKS_CHN_NULL, buff, sizeof(buff));
    if(iRet == WINKS_RETURN_SUCCESS && buff[0])
    {
        Winks_StrgSetSelfNo(buff);
    }
    
    if( (iRet == WINKS_RETURN_SUCCESS) &&
        ((iRet = Winks_SysTime2CacheTime(&v_dateTime, &CacheTime)) == WINKS_RETURN_SUCCESS) )
    {
        return Winks_StrgSetCacheTime(CacheTime);
    }

    return iRet;
}

/* 当注册完毕初始化片头运营商彩像时调用 */
int Winks_EngInitSpecialCC(int Pri)
{
    return Winks_GetCCStart(Pri, WINKS_CCDOWN_TYPE_INIT, NULL);
}

/* 服务器返回的错误码处理函数
   返回值: -1 -- 处理失败情况，可以认为已处理
            0 -- 返回成功，需要后继处理。
*/
int Winks_EngResultErrDftHandler(WinksProt_result_s * presult, int *retUIMsg)
{
    int iRet = 0;

    if(presult==NULL || retUIMsg==NULL)
    {
        Winks_assert(presult);
        Winks_assert(retUIMsg);
        *retUIMsg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
        return WINKS_RETURN_FAILURE;
    }

#ifdef WINKS_DEBUG
    winks_PrintResultCodeInfo(presult->code);
#endif
    switch(presult->code)
    {
    case WINKS_RST_CODE_OK:
        return WINKS_RETURN_SUCCESS;
    case SERVICE_NO_SUBSCRIPTION:
        iRet = (Winks_SetSettingValue(WINKS_SETTING_UNSUBSCRIBE_FLAG, 1) == 0);
        *retUIMsg = WINKS_MSG_ENG2UI_NO_SUBSCRIPTION;
        break;
    case SERVICE_MEMBER_DISABLED:
        iRet = (Winks_SetSettingValue(WINKS_SETTING_FORBIDDEN_FLAG, 1) == 0);
        *retUIMsg = WINKS_MSG_ENG2UI_USER_DISABLED;
        break;
    case SERVICE_NOT_MEMBER:
        *retUIMsg = WINKS_MSG_ENG2UI_UNREG_USER;
        break;
    default:
        *retUIMsg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
        break;
    }
#ifdef WINKS_DEBUG
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        Winks_printf("ERROR: Do something return error failed.[%d]\r\n",presult->code);
    }
#endif

    return WINKS_RETURN_FAILURE;
}

/* 直接获取用户彩像 */
int Winks_UserGetCCDirectly(char * pNo)
{
    return Winks_GetCCStart(WINKS_ENGINE_APP_PRI_USER, WINKS_CCDOWN_TYPE_NUMBER, pNo);
}

/*用户更新好友列录*/
int Winks_UserResetFriendList(void)
{
    return Winks_GetCCStart(WINKS_ENGINE_APP_PRI_USER, WINKS_CCDOWN_TYPE_FRIEND, NULL);
}

int Winks_UserUpdateFriendListStop(void)
{
    return Winks_GetCCStop(WINKS_CCDOWN_TYPE_FRIEND);
}
/*用户更新cash用户彩像*/
int Winks_UserResetCacheFriendList(void)
{
    return Winks_GetCCStart(WINKS_ENGINE_APP_PRI_USER, WINKS_CCDOWN_TYPE_CCCACHE, NULL);
}

int Winks_UserUpdateCacheFriendListStop(void)
{
    return Winks_GetCCStop(WINKS_CCDOWN_TYPE_CCCACHE);
}

/* retBuff -- 返回的内存缓冲区 
   retLen -- 输入内存缓冲区大小，输出实际使用的内存缓冲区大小
   IsDft -- 是否获取默认的配置
*/
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft)
{
    int     iRet = 0;
    unsigned int  FileSize = 0;
    char* pData = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<ccShow>"
            "<0>"
                "<high>5</high>"
                "<0>"
                    "<content>time</content>"
                    "<width>100</width>"
                    "<font>small</font>"
                    "<ftformat>standard</ftformat>"
                    "<bgcolor>0x00000000</bgcolor>"
                "</0>"
            "</0>"
            "<1>"
                "<high>6</high>"
                "<0>"
                    "<content>greeting</content>"
                    "<width>100</width>"
                    "<blank>10</blank>"
                    "<scrollway>right</scrollway>"
                    "<font>small</font>"
                    "<ftformat>standard</ftformat>"
                    "<bgcolor>0x00000000</bgcolor>"
                "</0>"
            "</1>"
            "<2>"
                "<high>74</high>"
                "<0>"
                    "<content>colorimage</content>"
                    "<width>100</width>"
                "</0>"
            "</2>"
            "<3>"
               "<high>8</high>"
               "<0>"
                    "<content>telnum</content>"
                    "<width>100</width>"
                    "<font>big</font>"
                    "<bgcolor>0x00000000</bgcolor>"
                "</0>"
            "</3>"
        "</ccShow>";
    if(IsDft)
        FileSize = strlen(pData)+1;
    else
        iRet = Winks_fgetsize(WINKS_FILE_CCSSET, 1, &FileSize);

    if(iRet < 0)
        return -1;

    if(retLen && *retLen<FileSize)
    {
        *retLen = FileSize;
        return -1;
    }

    Winks_assert(retBuff);
    Winks_assert(retLen);
    
    *retLen = FileSize;
    
    if(IsDft)
    {
        //here "{}"  is necessary.  
        Winks_mem_cpy(retBuff, pData, FileSize);
    }
    else
        iRet = Winks_ReadFile2Buf(WINKS_FILE_CCSSET, 0, (unsigned char *)retBuff, FileSize);
    
    return iRet;
}

/*获取通话号码的彩像*/
int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow)
{
    unsigned char regFlag = 0;
    char * pNo = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(pCCShow);

    if(phonenumber == NULL)
        return WINKS_RETURN_FAILURE;
    
    pNo = Winks_SkipPlus(phonenumber);
    if(pNo == NULL || pNo[0] == 0)
        return WINKS_RETURN_FAILURE;
    
    if( Winks_engineContext_p->pCall &&
        strcmp(pNo, Winks_engineContext_p->pCall->NO) == 0 )
    {
        Winks_mem_cpy(pCCShow, &Winks_engineContext_p->pCall->CCShow, sizeof(WinksStrgFile_CCShow_s));
        if( ((pCCShow->SaveType == 0) && pCCShow->CCImgPath[0][0]) ||
            ((pCCShow->SaveType == 1) && pCCShow->pCCImage && pCCShow->CCImageSize) )
        {
            pCCShow->RetStatus = 0;
            Winks_printf("Winks_UserGetCCShow(\"%s\") ret ok (%d,%d) \r\n", pNo,  \
                ((pCCShow->CC_Idx&0x8000)!=0), pCCShow->CC_Idx&0x7fff);
            return WINKS_RETURN_SUCCESS;
        }
    }
    else if(strcmp(pNo, WINKS_DFT_CC_NO) == 0)/* 默认彩像 */
    {
        pCCShow->RetStatus = 0;
        Winks_printf("Winks_UserGetCCShow(\"%s\") ret default cc\r\n", pNo);
        return Winks_StrgGetDftCCShow(dir, pCCShow);
    }

    /* 尝试直接从存储查找该彩像 */
    if(Winks_StrgGetCCShow(pNo, dir, pCCShow) == WINKS_RETURN_SUCCESS)
    {
        if(pCCShow->CC_Idx)
            Winks_StrgLockCC(pCCShow->CC_Idx);
        pCCShow->RetStatus = 0;
        Winks_printf("Winks_UserGetCCShow(\"%s\") ret (%d,%d) \r\n", pNo, \
            ((pCCShow->CC_Idx&0x8000)!=0), pCCShow->CC_Idx&0x7fff);
        return WINKS_RETURN_SUCCESS;
    }

    /* 尝试查找运营商彩像 */
    if(Winks_isUserRegistered(&regFlag) == 0 && regFlag == 1)
    {
        Winks_printf("Winks_UserGetCCShow(\"%s\") ret carrier cc\r\n", pNo);
        pCCShow->RetStatus = 3;
        if(Winks_StrgGetCCShow(WINKS_BUS_CC_NO, dir, pCCShow) == WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_SUCCESS;
    }

    /* 只能给默认彩像了 */
    pCCShow->RetStatus = 1;
    Winks_printf("Winks_UserGetCCShow(\"%s\") ret default cc\r\n", pNo);
    return Winks_StrgGetDftCCShow(dir, pCCShow);
}

/*停止展示彩像*/
int Winks_UserStopCCShow(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_StrgUnlockCC();
    Winks_salFree(Winks_engineContext_p->pCall);
    Winks_engineContext_p->pCall = NULL;
    return Winks_GetCCStop(WINKS_CCDOWN_TYPE_CALL);
}

/*设置开机不提示用户注册。*/
int Winks_UserRememberImsi(void)
{
    char buff[WINKS_IMSI_LEN + 1];
    int iRet = 0;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_engineContext_p->ifRemindRegister = 0;
    
    Winks_mem_set(buff, 0, sizeof(buff));

#ifndef WINKS_DUAL_SIM
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
#else
    iRet = Winks_GetImsi(WINKS_CHN_NULL, buff, sizeof(buff));
    if(iRet == WINKS_RETURN_SUCCESS)
    {
        iRet = Winks_SetImsiPopFlag(buff, 1);
        if(iRet != 0)
        {
            Winks_printf("ERR: Call Winks_SetImsiPopFlag error. return[%d]\r\n",iRet);
            //Winks_fail("iRet != 0");
            return WINKS_RETURN_FAILURE;
        }
    }
    iRet = Winks_GetImsi_2(WINKS_CHN_NULL, buff, sizeof(buff));
    if(iRet == WINKS_RETURN_SUCCESS)
    {
        iRet = Winks_SetImsiPopFlag(buff, 1);
        if(iRet != 0)
        {
            Winks_printf("ERR: Call Winks_SetImsiPopFlag error. return[%d]\r\n",iRet);
            //Winks_fail("iRet != 0");
            return WINKS_RETURN_FAILURE;
        }
    }
    return WINKS_RETURN_SUCCESS;
#endif

    return WINKS_RETURN_SUCCESS;
}
/* 关闭彩像 */
int Winks_UserCloseWinkCC(void)
{
    int iRet = 0;
    unsigned long value;
#if 1
    iRet = Winks_GetSettingValue(WINKS_SETTING_SERVICE_STOP_FLAG, &value);
    if((iRet!=WINKS_RETURN_FAILURE) && (value==WINKS_SETVAL_SWITCH_OFF))/* 现在为开启状态，关闭 */
    {
        iRet = winks_StopPeriodPushTimer(5);
        if(iRet!=WINKS_RETURN_FAILURE)
        {
            value = WINKS_SETVAL_SWITCH_ON;
            iRet = Winks_SetSettingValue(WINKS_SETTING_SERVICE_STOP_FLAG, value);
        }
    }
#endif
    return iRet;
}

/* 开启彩像 */
int Winks_UserOpenWinkCC(void)
{
    int iRet = 0;
    unsigned long value;
#if 1
    iRet = Winks_GetSettingValue(WINKS_SETTING_SERVICE_STOP_FLAG, &value);
    if((iRet!=WINKS_RETURN_FAILURE) && (value==WINKS_SETVAL_SWITCH_ON))/* 现在为关闭状态，开启 */
    {
        value = WINKS_SETVAL_SWITCH_OFF;
        iRet = Winks_SetSettingValue(WINKS_SETTING_SERVICE_STOP_FLAG, value);
        if(iRet!=WINKS_RETURN_FAILURE)
        {
            winks_StartBuildCacheTimer();
            iRet = winks_StartPeriodPushTimer();
        }
    }
#endif
    return iRet;
}

/*取漫游限制连网的标志. allowedFlag:0限制;1不限制*/
int Winks_isRoamingConnectionAllowed(unsigned char *allowedFlag)
{
    int iRet = 0;
    unsigned long roamSwitch = 0;
    unsigned char roamflag = 0;
    
    *allowedFlag = 1;
    iRet = Winks_GetSettingValue(WINKS_SETTING_ROAM_SWITCH, &roamSwitch);
    //Winks_assert(iRet == 0);
    if (iRet != 0)
        return WINKS_RETURN_FAILURE;
    
    if(roamSwitch == 1) /*国际漫游时不连接服务器*/
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
    if(Winks_engineContext_p->curState >= WINKS_ENGINE_STATE_ENABLED)
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

/*判断是否沉默用户。 SilenceFlag:0非沉默，1沉默用户*/
int Winks_isUserSilenced(unsigned char *SilenceFlag)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

	if(SilenceFlag)
		*SilenceFlag = Winks_engineContext_p->ifRemindRegister;

    return WINKS_RETURN_SUCCESS;
}

/*
4.14.	WAP网站入口链接规范
部分彩像业务的功在终端软件中是直接调用浏览器打开彩像业务WAP网站相应页面。所有链接使用以下URL格式。
Link-URL = {store-base-url} "/link/" page-name [ "?uid=" uid ]
page-name = (store | mywinks | customize) ; 分别对应：彩像商城，我的彩像库，定制业务

示例：
功能	示例URL
彩像商城	http://wap.winksi.com/link/store
彩像商城	http://wap.winksi.com/link/store?uid=jyuyfew8y42hf
我的彩像库	http://wap.winksi.com/link/mywinks?uid=jyuyfew8y42hf
定制业务	http://wap.winksi.com/link/customize?uid=jyuyfew8y42hf
购买彩像	http://wap.winksi.com/link/purchase?uid=jyuyfew8y42hf&wid=jksdfuoier763
*/
/*
打开Wap网页: 彩像商城/我的彩像库/定制业务/购买彩像
输入:
    UrlType -- Winks_UrlType_e
    wid -- 当UrlType为WINKS_URLTYPE_PURCHASE或WINKS_URLTYPE_DIYCUSTOMIZE时必须填写该参数，其他为NULL
*/
int Winks_UserOpenWapPage(int UrlType, char * wid)
{
    int     iRet = 0;
    int     off = 0;
    char    uid[WINKS_PROT_UID_MAX];
    char    sUrl[WINKS_URL_MAX_LEN];
    
    Winks_assert(UrlType>=WINKS_URLTYPE_CCSHOP && UrlType<WINKS_URLTYPE_MAX);

    Winks_mem_set(uid, 0, sizeof(uid));
    iRet = Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid));

    Winks_mem_set(sUrl, 0, sizeof(sUrl));
    if(Winks_GetSettingString(WINKS_SETTING_SHOP_SERVER_URL,
                sUrl, sizeof(sUrl)) != 0  || sUrl[0]==0)
        return -1;

  #ifdef WINKS_DEBUG
    if(wid)
        Winks_printf("Winks_UserOpenWapPage(%d, \"%s\")\r\n", UrlType, wid);
    else
        Winks_printf("Winks_UserOpenWapPage(%d, nul)\r\n", UrlType);
  #endif

    off = strlen(sUrl);
    
    switch(UrlType)
    {
    case WINKS_URLTYPE_CCSHOP:
        if(iRet!=0 || uid[0]==0)
            strcpy(&sUrl[off], "/link/store");
        else
            snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/store?uid=%s", uid);
        break;
    case WINKS_URLTYPE_FAVORITE:
        if(iRet!=0 || uid[0]==0)
            return -1;
        snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/mywinks?uid=%s", uid);
        break;
    case WINKS_URLTYPE_SCRIPT:
        if(iRet!=0 || uid[0]==0)
            return -1;
        snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/customize?uid=%s", uid);
        break;
    case WINKS_URLTYPE_PURCHASE:
        if(iRet!=0 || uid[0]==0)
            return -1;
        if(wid == NULL || wid[0]==0)
        {
            //snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/purchase?uid=%s", uid);
            Winks_assert(wid && wid[0]);
        }
        else
            snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/purchase?uid=%s&wid=%s", uid, wid);
        break;

    case WINKS_URLTYPE_INTRO:
        strncpy(&sUrl[off], "/link/intro", sizeof(sUrl)-off);
        break;
    case WINKS_URLTYPE_HELP:
        strncpy(&sUrl[off], "/link/help", sizeof(sUrl)-off);
        break;
    case WINKS_URLTYPE_ABOUT:
        strncpy(&sUrl[off], "/link/about", sizeof(sUrl)-off);
        break;
    case WINKS_URLTYPE_SETTINGS:
        if(iRet!=0 || uid[0]==0)
            return -1;
        snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/settings?uid=%s", uid);
        break;
    case WINKS_URLTYPE_DIYCUSTOMIZE:
        if(iRet!=0 || uid[0]==0)
            return -1;
        if(wid == NULL || wid[0]==0)
        {
            //snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/diycustomize?uid=%s", uid);
            Winks_assert(wid && wid[0]);
        }
        else
            snprintf(&sUrl[off], sizeof(sUrl)-off, "/link/diycustomize?uid=%s&wid=%s", uid, wid);
        break;

    default:
        Winks_fail("Winks_UserGetUrl error case")
        return -1;
    }

    Winks_printf("Winks_OpenWapPage(\"%s\")\r\n", sUrl);
    
    return Winks_OpenWapPage(sUrl);
}

/* 打包发给UI的包含body的消息 */
int Winks_EngPostMsgToUI(int Pri, Winks_EngProType_e appType, int msg, void *data, int size)
{
    int iRet = 0;
    Winks_EngineToUiReturnMsg   RetMsg;
    Winks_EngineToUiReturnMsg *pRetMsg = &RetMsg;
    int hlen = Winks_offsetof(Winks_EngineToUiReturnMsg,body);
    int len = sizeof(RetMsg);

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    /*构建消息体*/
    if(size > sizeof(RetMsg.body))
    {
        len = hlen+size;
        pRetMsg = (Winks_EngineToUiReturnMsg *)Winks_salAlloc(len);
        if(pRetMsg == NULL)
            return WINKS_RETURN_FAILURE;
    }
    Winks_mem_set(pRetMsg, 0, len);

    /*给消息体赋值*/
    pRetMsg->code = msg;
    pRetMsg->appType = (int)appType;/*应用类型*/
    Winks_mem_cpy(pRetMsg->body, data, size);

    /*发消息*/
    if(Pri<WINKS_ENGINE_APP_PRI_USER)
        iRet = Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_APP_RETMSG, pRetMsg, len );
    else
        iRet = Winks_PostMsgToUi(WINKS_MSG_ENGINE_UIRETMSG, pRetMsg, len);

    /*释放构建的消息体*/
    if(pRetMsg != &RetMsg)
        Winks_salFree(pRetMsg);

    return iRet;
}

#define WINKS_ENG_MEM_BLOCK_LEN     1024

Winks_MemPool_s * Winks_EngMemPoolCreate( Winks_MemPool_s ** ppMemPool )
{
	Winks_MemPool_s* pmem = NULL;
	Winks_MemPool_s* pnew = NULL;

    Winks_assert(ppMemPool);
    
    if(*ppMemPool != NULL)
    {
    	pmem = *ppMemPool;

    	while( pmem->pnext )
    		pmem = pmem->pnext;
    }

	if( (pnew = (Winks_MemPool_s *)Winks_salAlloc( WINKS_ENG_MEM_BLOCK_LEN + sizeof( Winks_MemPool_s) )) == NULL )
		return NULL;
	Winks_mem_set( pnew, 0, WINKS_ENG_MEM_BLOCK_LEN + sizeof( Winks_MemPool_s) );
	
	pnew->len = WINKS_ENG_MEM_BLOCK_LEN;
	pnew->offset = 0;
	pnew->pdata = (char* )(pnew + 1);

    if(pmem)
	    pmem->pnext = pnew;
    else
        *ppMemPool = pnew;

	return pnew;
}

int Winks_EngMemPoolDestory( Winks_MemPool_s * pMemPool )
{
	Winks_MemPool_s* pmem = NULL;
    Winks_assert(pMemPool);
    
	while( pMemPool )
	{
		pmem = pMemPool;
        pMemPool = pMemPool->pnext;

		Winks_salFree( pmem );
	}

	return 0;
}

void * Winks_EngMemPoolAlloc( Winks_MemPool_s* pMemPool, 
                                             int AllocSize,
                                             int Align )
{
	Winks_MemPool_s* pmem = NULL;
	void * pret = NULL;

    Winks_assert(pMemPool);
    
	pmem = pMemPool;

	while( pmem->pnext )
		pmem = pmem->pnext;

    if(Align)
        pmem->offset = WINKS_ALIGN(pmem->offset, Align);
    
	if( (pmem->len - pmem->offset) < AllocSize )
	{
		if( (pmem = Winks_EngMemPoolCreate(&pMemPool)) == NULL)
            return NULL;
	}
    
	pret = (void * )(pmem->pdata + pmem->offset);
    Winks_mem_set( pret, 0, AllocSize );
	pmem->offset += AllocSize;
    
	return pret;
}

#ifdef WINKS_DEBUG/*WINKS_DEBUG*/
void Winks_PrintfXml(char *pbuf, int BufSize)
{
    int  Count, Off;
    int  LineSize = 64;
    char Buf[128];

    Winks_assert(LineSize < sizeof(Buf));
    
    Off = 0;
    Count = 0;
    while(Off+Count < BufSize)
    {
        if(pbuf[Off+Count] == '\r' || pbuf[Off+Count] == '\n')
        {
            strncpy(Buf, &pbuf[Off], Count);
            Buf[Count] = 0;
            Winks_printf("%s\r\n", Buf);
            
            while(pbuf[Off+Count] == '\r' || pbuf[Off+Count] == '\n')
            {
                Count++;
            }
            Off += Count;
            Count = 0;
        }
        else if(Count >= LineSize)
        {
            strncpy(Buf, &pbuf[Off], Count);
            Buf[Count] = 0;
            Winks_printf("%s\r\n", Buf);

            Off += Count;
            Count = 0;
        }
        Count++;
    }
}
#endif

static int winks_EngProtAnalyze(int http, unsigned long length)
{
    int iRet = 0;
    Winks_MemPool_s* pMemPool = NULL;
    Winks_Xml_s *pXml= NULL;
    WinksProt_ParseData_s * pParseData = NULL;
    WinksEng2AppMsg_s Eng2AppMsg;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_mem_set(&Eng2AppMsg, 0, sizeof(WinksEng2AppMsg_s));
    if(Winks_EngMemPoolCreate(&pMemPool) == NULL)
    {
        Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_MEM;
    }
    else
    {
        unsigned long  Off= 0;

        do{
            char * pbuf = NULL;
            int    BufSize = 0;
            
            if(Off+WINKS_ENG_MEM_BLOCK_LEN < length)
                BufSize = WINKS_ENG_MEM_BLOCK_LEN;
            else
                BufSize = length-Off;
            
            if( (pbuf = Winks_EngMemPoolAlloc(pMemPool, BufSize, 0)) == NULL )
            {
                    Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_MEM;
                    break;
            }
            if(Winks_httprecv(http, NULL, Off, pbuf, BufSize) < 0)
            {
                    Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP;
                break;
            }
        #ifdef WINKS_DEBUG/*WINKS_DEBUG*/
            Winks_PrintfXml(pbuf, BufSize);
            //Winks_printHex((unsigned char*)pbuf, (unsigned short)BufSize, (unsigned char *)"[ENG ANALYZE + + + + + + + + + ]:\r\n");
        #endif
            Off += BufSize;
        }while(Off<length);
    
        winks_EngHttpClose(http);

        iRet = Winks_XML_Parse((Winks_XmlMem_s *)pMemPool, &pXml);
        Winks_EngMemPoolDestory(pMemPool);
        pMemPool = NULL;
        
        if(iRet < 0 || pXml == NULL || pXml->pelement == NULL)
        {
            Winks_printf("winks_EngProtAnalyze ifHttpReady=%d pSaveSendData=0x%08x\r\n", 
                Winks_engineContext_p->ifHttpReady, Winks_engineContext_p->pSaveSendData);
            if( Winks_engineContext_p->ifHttpReady < WINKS_HTTP_RETRY_MAX &&
                winks_ReSendData() == WINKS_RETURN_SUCCESS )
                return WINKS_RETURN_SUCCESS;
            else
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_XML_PARSE;
        }/*
        else if(pXml->pelement == NULL)
        {
            if(Winks_engineContext_p->ifHttpReady == 0)
                Winks_engineContext_p->ifHttpReady = WINKS_HTTP_RETRY_MAX;
            Eng2AppMsg.AppMsg = WINKS_PROT_RET_TYPE_null;
        }*/
        else
        {
            if(Winks_engineContext_p->ifHttpReady == 0)
                Winks_engineContext_p->ifHttpReady = WINKS_HTTP_RETRY_MAX;
            iRet = Winks_ProtParse( pXml->pelement, &pParseData );
            Winks_XML_Free(pXml);
            pXml = NULL;
            
            if(iRet == -2)
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_MEM;
            else if(iRet<0 || pParseData == NULL)
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_PROT_PARSE;
            else
            {
                Eng2AppMsg.AppMsg = pParseData->RetType;
                Eng2AppMsg.pData = pParseData->pData;
            }
        }
    
    }
    
    iRet = Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(WinksEng2AppMsg_s));
    
    if(pParseData)
        Winks_ProtParseFree(pParseData);
    
    return iRet;
}

static int winks_EngHttpGetInfo( int http, 
                                        Winks_HttpInfo *retHttpInfo,
                                        Winks_fileinfo_s *retfileinfo,
                                        Winks_SrvNotify_s *retNotify)/*WINKS_NOTIFY_MASK*/
{
    int     iRet = 0;
    char    CotentType[64];
    int     Len;
    
    /* 获取HttpInfo */
    if(retHttpInfo)
    {
        if((Len = Winks_httpgetinfo(http, retHttpInfo))<0)
        {
            /* 还未接收数据 */
            return WINKS_RETURN_FAILURE;
        }
    }

    if(retfileinfo)
    {
        Len = sizeof(CotentType);
        Winks_mem_set(CotentType, 0, Len);
        iRet = Winks_httpgethead(http, HTTP_HEAD_CONTENTTYPE, CotentType, &Len);
        if(iRet == WINKS_HTTP_EBUFF)
            strcpy(CotentType, HTTP_CONTENTTYPE_octet);

        if(iRet >= 0)
        {
            retfileinfo->ext = winks_ContType2ExtType(CotentType);

            if(retfileinfo->ext > WINKS_FILE_EXT_xml)/* 下载文件(彩像或升级文件) */
            {
                Len = sizeof(CotentType);
                Winks_mem_set(CotentType, 0, Len);
                iRet = Winks_httpgethead(http, HTTP_HEAD_CONTENTLEN, CotentType, &Len);
                if(iRet >= 0)
                    retfileinfo->filesize = atol(CotentType);
            }
        }
    }

    if(retNotify)
    {
        Len = sizeof(CotentType);
        Winks_mem_set(CotentType, 0, Len);
        iRet = Winks_httpgethead(http, HTTP_HEAD_WINKSNOTIFY, CotentType, &Len);
        if(iRet >= 0)
        {
            if(strstr(CotentType, HTTP_WINKSNOTIFY_newver) != NULL)
                retNotify->Notify |= WINKS_NOTIFY_newver;
            if(strstr(CotentType, HTTP_WINKSNOTIFY_ccfg) != NULL)
                retNotify->Notify |= WINKS_NOTIFY_ccfg;
            if(strstr(CotentType, HTTP_WINKSNOTIFY_scfg) != NULL)
                retNotify->Notify |= WINKS_NOTIFY_scfg;
            if(strstr(CotentType, HTTP_WINKSNOTIFY_upldstat) != NULL)
            {
                retNotify->Notify |= WINKS_NOTIFY_upldstat;
                Len = strlen(HTTP_WINKSNOTIFY_upldstat);
                retNotify->filenumber_stat = (unsigned long)atol(&CotentType[Len]);
            }
            if(strstr(CotentType, HTTP_WINKSNOTIFY_upldlog) != NULL)
            {
                retNotify->Notify |= WINKS_NOTIFY_upldlog;
                Len = strlen(HTTP_WINKSNOTIFY_upldstat);
                retNotify->filenumber_log = (unsigned long)atol(&CotentType[Len]);
            }
        }
    }
    
    return WINKS_RETURN_SUCCESS;
}

static Winks_MemPool_s * winks_EngHttpGetData(int http, unsigned long * plen)
{
    Winks_MemPool_s * pMemPool = NULL;
    unsigned long  Off = 0;
    unsigned long  length = 0;
    
    if(Winks_EngMemPoolCreate(&pMemPool) == NULL)
    {
        return NULL;
    }

    if(plen)
        length = *plen;
    
    Off= 0;
    do{
        char * pbuf = NULL;
        int    BufSize = 0;
        
        if(Off+WINKS_ENG_MEM_BLOCK_LEN < length)
            BufSize = WINKS_ENG_MEM_BLOCK_LEN;
        else
            BufSize = length-Off;
        
        if( (pbuf = Winks_EngMemPoolAlloc(pMemPool, BufSize, 0)) == NULL )
        {
                break;
        }
        if(Winks_httppulldata(http, pbuf, BufSize) < 0)
        {
            break;
        }
        Off += BufSize;
    }while(Off<length);
    
    Winks_printf("winks_EngHttpGetData %d\r\n", Off);
    
    if(plen)
        *plen = Off;
    
    return pMemPool;
}

/* Http消息处理函数 */
static int winks_HttpMsgProc(const void* data, unsigned int size )
{
    int http = WINKS_HTTP_NULL;

    char pURL[WINKS_URL_MAX_LEN+1];
    int ulen = 0;
    Winks_Socketmsg_s* p = (Winks_Socketmsg_s* )data;
    WinksEng2AppMsg_s Eng2AppMsg;
    
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
    if(Winks_engineContext_p->http != http)
    {
        Winks_printf( "Error: WINKS http has free (%d != %d)\r\n",Winks_engineContext_p->http, http );
        return 0;
    }
    /*ulen = WINKS_URL_MAX_LEN;*/
    Winks_mem_set(pURL, 0, sizeof(pURL));
    Winks_mem_set(&Eng2AppMsg, 0, sizeof(WinksEng2AppMsg_s));
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
            Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP;
            Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(WinksEng2AppMsg_s));
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
            Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP;
            Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(WinksEng2AppMsg_s));
            break;
        }
        /*  连接成功，提示。*/
        Winks_printf( "[ENG]WINKS http %s get http message connect ok\r\n", pURL );
        break;
    case WINKS_HTTP_DATAOUT:
    {
        if( p->wParam & 0xff0000 )
        {
            /*连接出现错误，提示。*/
            Winks_printf( "[ENG]WINKS http %s get http message dataout get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP;
            Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(WinksEng2AppMsg_s));
            break;
        }
        
        Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_APP_DATAOUT, (void *)p->lParam, sizeof(p->lParam));
        break;
    }

    case WINKS_HTTP_DATAIN:
    {
        int iRet = 0;
        Winks_fileinfo_s fileinfo;
        Winks_HttpInfo   HttpInfo;

        /*数据到达消息。本消息不需要进行处理，属于进程状态通知消息。随消息会告知用户目前已经接收到的数据长度。
        获取对应的HTTP传输句柄的请求的URL数据。*/
        Winks_httpgeturl( http, pURL, &ulen );
        if( p->wParam & 0xff0000 )
        {
            /*数据接收出错，提示。*/
            Winks_printf( "[ENG]WINKS http %s get http message datain get a error msg %d\r\n", pURL, (p->wParam & 0xff0000) >> 16 );
            winks_EngHttpClose(http);
            Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP;
            Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(WinksEng2AppMsg_s));
            break;
        }
        /*数据接收成功，提示并提示接收数据长度。*/
        Winks_printf( "[ENG]WINKS http %s get http message datain ok, len %d\r\n", pURL, p->lParam );

        /*有数据相应时重置等待数据的时间*/
        Winks_assert(Winks_engineContext_p->httpTimer != WINKS_TIMER_NULL);
        Winks_StopTimer(Winks_engineContext_p->httpTimer);
        Winks_StartTimer(Winks_engineContext_p->httpTimer);

        Winks_mem_set(&HttpInfo, 0, sizeof(HttpInfo));
        Winks_mem_set(&fileinfo, 0, sizeof(fileinfo));
        iRet = winks_EngHttpGetInfo(http, &HttpInfo, &fileinfo, NULL);

        if( iRet == WINKS_RETURN_SUCCESS && 
            ((HttpInfo.statuscode >= HTTP_OK) 
            && (HttpInfo.statuscode < HTTP_MultipleChoices) 
            || (HttpInfo.statuscode == HTTP_Notmodified)) &&/* [200,300) 数据OK */
            (fileinfo.ext > WINKS_FILE_EXT_xml) )/* 下载彩像或升级文件 */
        {
            fileinfo.blocklen = p->lParam;
            fileinfo.pMem = winks_EngHttpGetData(http, &fileinfo.blocklen);
            if(fileinfo.pMem && fileinfo.blocklen)
            {
                Eng2AppMsg.AppMsg = WINKS_PROT_RET_TYPE_file;
                Eng2AppMsg.pData = &fileinfo;
                Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
            }
            if(fileinfo.pMem)
                Winks_EngMemPoolDestory(fileinfo.pMem);
        }
    }
        break;

    case WINKS_HTTP_DATAEND:
    {
        int     iRet = 0;
        Winks_fileinfo_s fileinfo;
        Winks_HttpInfo   HttpInfo;

        Winks_printf( "[ENG]WINKS http get http message dataend ok\r\n");

        Winks_mem_set(&HttpInfo, 0, sizeof(HttpInfo));
        Winks_mem_set(&fileinfo, 0, sizeof(fileinfo));
        iRet = winks_EngHttpGetInfo(http, &HttpInfo, &fileinfo, &Winks_engineContext_p->SrvNotify);
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP;
            Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
            return WINKS_RETURN_FAILURE;
        }
        else if( (HttpInfo.statuscode!=HTTP_Notmodified) && 
            ((HttpInfo.statuscode < HTTP_OK) 
            || (HttpInfo.statuscode >= HTTP_MultipleChoices)))/* [200,300) 数据OK */
        {
            Winks_printf("ERROR: HTTP statuscode[%d] : %s\r\n", HttpInfo.statuscode, Winks_httpStatus(HttpInfo.statuscode));
        #if 1
            if(HttpInfo.statuscode >= HTTP_BadRequest) /* >=400 连接错误或服务器错误 */
            {
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP_ERRCODE;
            }
            else/* [300,400) 重定向 */
            {
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP_ERRCODE;
            }
        #else
            /* 具体分析错误 */
            switch(HttpInfo.statuscode)
            {
            case HTTP_BadGateway:
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP_ERRCODE;
                break;
            default:
                Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP_ERRCODE;
                break;
            }
        #endif
        #ifdef WINKS_DEBUG/*WINKS_DEBUG*/
        {
            char  * pbuf = NULL;

            if( (pbuf = Winks_salAlloc(WINKS_ENG_MEM_BLOCK_LEN)) != NULL )
            {
                int     BufSize = 0;
                unsigned long  Off= 0;
                int     length = p->lParam;
                
                do{
                    Winks_mem_set(pbuf, 0, WINKS_ENG_MEM_BLOCK_LEN);
                    
                    if(Off+WINKS_ENG_MEM_BLOCK_LEN-1 < length)
                        BufSize = WINKS_ENG_MEM_BLOCK_LEN-1;
                    else
                        BufSize = length-Off;
                    
                    if(Winks_httprecv(http, NULL, Off, pbuf, BufSize) < 0)
                    {
                        break;
                    }
                //#ifdef WINKS_SIMULATOR
                    Winks_PrintfXml(pbuf, BufSize);
                    //Winks_printHex((unsigned char*)pbuf, (unsigned short)BufSize, (unsigned char *)"[ENG ANALYZE + + + + + + + + + ]:\r\n");
                //#endif
                    Off += BufSize;
                }while(Off<length);
                
                Winks_salFree(pbuf);
                pbuf = NULL;
            }
        }
        #endif
            Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
            return WINKS_RETURN_FAILURE;
        }

        if( (Winks_engineContext_p->SrvNotify.Notify & WINKS_NOTIFY_MASK) &&
            ((Winks_engineContext_p->SrvNotify.Notify>>16) & WINKS_NOTIFY_MASK) == 0 )
        {
            winks_StartSrvMsgTimer();
        }

        if(fileinfo.ext > WINKS_FILE_EXT_xml)
        {
            fileinfo.blocklen = p->lParam;
            if(fileinfo.blocklen)
            {
                fileinfo.pMem = winks_EngHttpGetData(http, &fileinfo.blocklen);
                if(fileinfo.pMem)
                {
                    if(fileinfo.blocklen)
                    {
                        Eng2AppMsg.AppMsg = WINKS_PROT_RET_TYPE_file;
                        Eng2AppMsg.pData = &fileinfo;
                        Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
                    }
                    Winks_EngMemPoolDestory(fileinfo.pMem);
                }
                else
                {
                    Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_MEM;
                    Eng2AppMsg.pData = NULL;
                    Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
                }
            }
            if(Eng2AppMsg.AppMsg >= 0)
            {
                /* 通知应用文件下载完毕，pMem为NULL而且blocklen为0 */
                fileinfo.pMem = NULL;
                fileinfo.blocklen = 0;
                Eng2AppMsg.AppMsg = WINKS_PROT_RET_TYPE_file;
                Eng2AppMsg.pData = &fileinfo;
                Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
            }
        }
    #if 0
        else if(Winks_engineContext_p->ProtNo == WINKS_PROTOCOL_GETCCSHOWSETUP)
        {
            char  * pbuf = NULL;
            int     CreateFlag = 1;

            if( (pbuf = Winks_salAlloc(WINKS_ENG_MEM_BLOCK_LEN)) != NULL )
            {
                int     BufSize = 0;
                unsigned long  Off= 0;
                int     length = p->lParam;
                
                do{
                    Winks_mem_set(pbuf, 0, WINKS_ENG_MEM_BLOCK_LEN);
                    
                    if(Off+WINKS_ENG_MEM_BLOCK_LEN-1 < length)
                        BufSize = WINKS_ENG_MEM_BLOCK_LEN-1;
                    else
                        BufSize = length-Off;
                    
                    if(Winks_httprecv(http, NULL, Off, pbuf, BufSize) < 0)
                    {
                        break;
                    }
                    
                    if(Winks_WriteBuf2File(WINKS_FILE_CCSSET, Off, (unsigned char *)pbuf, BufSize, CreateFlag) 
                            != WINKS_RETURN_SUCCESS)
                        break;
                    if(CreateFlag)
                        CreateFlag = 0;
                    Off += BufSize;
                }while(Off<length);
                
                Winks_salFree(pbuf);
                pbuf = NULL;
            }
        }
    #endif
        else
        {
            /*数据解析*/
            iRet = winks_EngProtAnalyze(http, p->lParam);
        }
        if(Winks_engineContext_p->pSaveSendData)
        {
            Winks_salFree(Winks_engineContext_p->pSaveSendData);
            Winks_engineContext_p->pSaveSendData = NULL;
        }
        //Winks_engineContext_p->ProtNo = 0;
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
            Winks_printf("ERROR: %4d : http %d!=%d not march\r\n", 
                        __LINE__, http, Winks_engineContext_p->http);
        Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP_TIMEOUT;
        Eng2AppMsg.pData = NULL;
        Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(Eng2AppMsg));
        winks_EngHttpClose(http);
        break;
    }

    default:
        Winks_printf( "[ENG]WINKS http get http message unknow msg %d\r\n", p->wParam & 0xffff );
        break;
    }

    return 0;
}

static int winks_SrvMsgDeal()
{
    unsigned long i, Bit;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_SrvMsgDeal: 0x%08x, %d\r\n",  \
        Winks_engineContext_p->SrvNotify.Notify,   \
        Winks_engineContext_p->SrvNotify.Bit);
    /* 依次查找服务器消息进行处理 */
    i = Winks_engineContext_p->SrvNotify.Bit;
    
    do{
        Bit = 1L<<i;
        if(Winks_engineContext_p->SrvNotify.Notify & Bit)
        {
            Winks_engineContext_p->SrvNotify.Bit = (i+1)%WINKS_NOTIFY_BITS;
            break;
        }
        i = (i+1)%WINKS_NOTIFY_BITS;
    }while( i != Winks_engineContext_p->SrvNotify.Bit );
    
    if( i == Winks_engineContext_p->SrvNotify.Bit)
    {
        /* 没有需要处理的服务器信息了 */
        Winks_engineContext_p->SrvNotify.Bit = 0;
        return WINKS_RETURN_SUCCESS;
    }
    
    /* 一次只能处理一个消息项 */
    switch(Bit)
    {
    case WINKS_NOTIFY_newver:
        {
        unsigned long value = 0;
        if( Winks_GetSettingValue(WINKS_SETTING_SW_UPDATA_WAY, &value) == 0 &&
            value == 1 )/* 自动升级 */
        {
            if(Winks_UpgradeStart(WINKS_ENGINE_APP_PRI_NORMAL) == 0)
                Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_newver<<16;
        }
        else
        {
            /* 手动升级，通知UI有新版本升级就可以了 */
            Winks_EngPostMsgToUI(WINKS_ENGINE_APP_PRI_USER, WINKS_ENGPROTYPE_UPDATE_SW, WINKS_MSG_ENG2UI_RUPGRADE_NEW, NULL, 0);
            Winks_engineContext_p->SrvNotify.Notify &= ~WINKS_NOTIFY_newver;
        }
        }
        break;
    case WINKS_NOTIFY_scfg:
        {
        if(Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_SERVICECONFIG, NULL) == 0)
            Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_scfg<<16;
        }
        break;
    case WINKS_NOTIFY_ccfg:
        {
        if(Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_USERCONFIG, NULL) == 0)
            Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_ccfg<<16;
        }
        break;
    case WINKS_NOTIFY_upldstat:
        {
        if(Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_UPLOAD_STAT, 
            &Winks_engineContext_p->SrvNotify.filenumber_stat) == 0)
            Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_upldstat<<16;
        }
        break;
    case WINKS_NOTIFY_upldlog:
        {
        if(Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_UPLOAD_LOG, 
            &Winks_engineContext_p->SrvNotify.filenumber_log) == 0)
            Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_upldlog<<16;
        }
        break;
    default:
        break;
    }

    if(Winks_engineContext_p->SrvNotify.Notify>>16)
        return WINKS_RETURN_SUCCESS;
    else
        return WINKS_RETURN_FAILURE;
}

/* 定时服务程序 */
static int winks_TimerExpiryProc(void *data, unsigned long size)
{
    int iRet = 0;
    WINKS_TIMER_ID *timer_id_ptr = (WINKS_TIMER_ID *)data;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->httpTimer == *timer_id_ptr)
    {
        WinksEng2AppMsg_s Eng2AppMsg;
        Winks_printf( "WARN: httpTimer Expiry %d %d, close http\r\n", Winks_engineContext_p->http, Winks_engineContext_p->httpTimer);
        winks_EngHttpClose(Winks_engineContext_p->http);
        Winks_mem_set(&Eng2AppMsg, 0, sizeof(WinksEng2AppMsg_s));
        Eng2AppMsg.AppMsg = WINKS_MSG_ENG2UI_HTTP_TIMEOUT;
        Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, &Eng2AppMsg, sizeof(WinksEng2AppMsg_s));
    }
#ifdef WINKS_SUPPORT_3G
    else if(Winks_engineContext_p->CallWaitTimer == *timer_id_ptr)
    {
        winks_StopCallWaitTimer();

        if(Winks_engineContext_p->pCall)
        {
            if(Winks_engineContext_p->pCall->CallCCStatus == WINKS_CALLCC_STATUS_FAIL)
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_CALL_FAIL);
            else if(Winks_engineContext_p->pCall->CallCCStatus < WINKS_CALLCC_STATUS_OK)
                winks_EngRetErrDirectly(WINKS_MSG_ENG2UI_CALL_TIMEOUT);
    }
    }
#endif /* WINKS_SUPPORT_3G */
    else if(Winks_engineContext_p->PeriodPushTimer == *timer_id_ptr)
    {
    #ifdef WINKS_DEBUG
        Winks_DateTime_s v_dateTime;
        Winks_GetSysDateTime( &v_dateTime );
        Winks_printf( "PeriodPushTimer Expiry: %04d-%02d-%02d %02d:%02d:%02d state[%d]\r\n", 
            v_dateTime.year, v_dateTime.month, v_dateTime.day,
            v_dateTime.hour, v_dateTime.min, v_dateTime.sec,
            Winks_engineContext_p->curState);
    #endif
        if( (Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED) /*&& 
            (Winks_engineContext_p->subState == WINKS_ENGINE_SUBSTATE_ZERO)*/ )
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
    else if(Winks_engineContext_p->ServerMsgTimer == *timer_id_ptr)
    {
        winks_StopSrvMsgTimer();
        /* 处理消息 */
        if(Winks_engineContext_p->SrvNotify.Notify & WINKS_NOTIFY_MASK)
        {
            /* 正在处理服务器通知项，延时等待 */
            if((Winks_engineContext_p->SrvNotify.Notify>>16) & WINKS_NOTIFY_MASK)
            {
                return WINKS_RETURN_SUCCESS;
            }
            
            if(winks_SrvMsgDeal() != WINKS_RETURN_SUCCESS)
                winks_StartSrvMsgTimer();
        }
    }
    else if(Winks_engineContext_p->BuildCacheTimer == *timer_id_ptr)
    {
        int StrgStatus = Winks_StrgGetStatus();

        switch(StrgStatus)
        {
        case WINKS_STRG_CARD_STATUS_NO_CCCACHE:
            //iRet = Winks_GetCCStart(WINKS_ENGINE_APP_PRI_NORMAL, WINKS_CCDOWN_TYPE_CCCACHE, NULL);
            break;
        case WINKS_STRG_CARD_STATUS_OLD_CARD:
            iRet = Winks_GetCCStart(WINKS_ENGINE_APP_PRI_NORMAL, WINKS_CCDOWN_TYPE_CCCACHE, NULL);
            break;
        case WINKS_STRG_CARD_STATUS_NEW_CARD:
            Winks_StrgDeleteDiskFiles(2);
            iRet = Winks_GetCCStart(WINKS_ENGINE_APP_PRI_NORMAL, WINKS_CCDOWN_TYPE_CCCACHE, NULL);
            break;
        case WINKS_STRG_CARD_STATUS_NOT_EXIST:
            iRet = Winks_GetCCStart(WINKS_ENGINE_APP_PRI_NORMAL, WINKS_CCDOWN_TYPE_CCCACHE, NULL);
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
                    Winks_printf("timer found 1 Winks_GetImsi_2 return %d\r\n", iRet);
                }
                else/* 找到了卡2,试图取卡1的imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    Winks_assert(Winks_GetActiveSim()==0);/* Init,Start阶段不能调用Winks_SetActiveSim否则Winks_GetImsi出错 */

                    Winks_engineContext_p->CurInquireSimNo = 1;
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    Winks_printf("timer found 1 Winks_GetImsi return %d\r\n", iRet);
                }
            }
            else/* 没有找到Sim卡,依次继续轮询两张卡 */
            {
                if(Winks_engineContext_p->CurInquireSimNo == 1)
                {
                    Winks_engineContext_p->CurInquireSimNo = 2;
                    iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    Winks_printf("timer found 0 Winks_GetImsi_2 return %d\r\n", iRet);
                }
                else
                {
                    Winks_engineContext_p->CurInquireSimNo = 1;
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    Winks_printf("timer found 0 Winks_GetImsi return %d\r\n", iRet);
                }
            }
        #else /* WINKS_DUAL_SIM */
            iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
            Winks_printf("timer Winks_GetImsi return %d\r\n", iRet);
        #endif /* WINKS_DUAL_SIM */
            if(iRet == WINKS_RETURN_SUCCESS)/* 读取imsi成功 */
            {
                Winks_printf("timer get imsi success: %s\r\n", buff);
            #ifdef WINKS_DUAL_SIM
                if(Winks_engineContext_p->CurInquireSimNo == 1)
                    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_REQ, (void *)buff, strlen(buff));
                else
                    Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_2_REQ, (void *)buff, strlen(buff));
            #else /* WINKS_DUAL_SIM */
                Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_IMSI_REQ, (void *)buff, strlen(buff));
            #endif /* WINKS_DUAL_SIM */
            }
            else if(iRet == WINKS_RETURN_FAILURE)/* 读取imsi失败 */
            {
                Winks_printf("timer get imsi fail\r\n");
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
        iRet = winks_EngAppTimerProc(*timer_id_ptr);
        if(iRet == WINKS_RETURN_FAILURE)
        {
            Winks_printf("Unknow timer msg. timerId=[%d]\r\n",*timer_id_ptr);
            //Winks_fail("");
        }
    }
    
    return WINKS_RETURN_SUCCESS;
}

static int Winks_GetTimeForImsi(unsigned short cnt)
{
	int ret_time = 0;

#ifdef WINKS_DUAL_SIM
    cnt /= 2;
#endif
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

#ifdef WINKS_DUAL_SIM
#if 1
/* 高字节低2位表示发现哪张卡，  b0--卡1; b1--卡2
   低字节表示当前激活的是哪张卡，1--卡1;  2--卡2
*/
unsigned short Winks_GetSimStatus(void)
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
/* Ret: 1--match OK; 0--do'nt match */
static int winks_CheckFoundSim(char *imsiCode, unsigned long len)
{
    int iRet = 0;
    char SavedImsi[WINKS_IMSI_LEN+1];
    
    Winks_mem_set(&SavedImsi, 0, sizeof(SavedImsi));
    if(Winks_GetRecentlyActivatedImsi(SavedImsi, sizeof(SavedImsi)) == WINKS_RETURN_SUCCESS)
    {
/*
        char    uid[WINKS_PROT_UID_MAX]; 
        if(Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid))<0 || strlen(uid)==0)
        {
            return 0;
        }
*/
        iRet = (strncmp(SavedImsi, imsiCode, WINKS_IMSI_LEN) == 0);
        Winks_printf("winks_CheckFoundSim[%s]?=[%s] ret %d\r\n",imsiCode, SavedImsi, iRet);
        return iRet;
    }
    Winks_printf("winks_CheckFoundSim no old imsi\r\n");
    return 0;
}
/*
    初始化SIM卡函数，选择一张SIM卡作为彩像业务的承载卡，进行引擎初始化。
  BootMethod:
        0: 发现了SIM卡，但不是激活过的SIM卡，尝试初始化为未激活状态。
            如果是双卡手机，发现了小于两张sim卡的时候直接返回失败。
        1: 发现SIM卡1为已注册的Sim卡,正常启动
        2: 发现SIM卡2为已注册的Sim卡,正常启动
*/
static int winks_BootingWithSimCard(
        int BootMethod,
        char *imsiCode1, unsigned long len1,
        char *imsiCode2, unsigned long len2)
{
    int iRet = 0;
    int StrgStatus;
    int imsiCnt = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_BootingWithSimCard(%d, \"%s\", %d, \"%s\", %d)\r\n",
        BootMethod, (imsiCode1 ? imsiCode1 : "NULL"), len1, (imsiCode2 ? imsiCode2 : "NULL"), len2);
    Winks_assert(BootMethod<3);


    switch(BootMethod)
    {
        case 2: ++imsiCnt;
        case 1: ++imsiCnt;
        {

        if(Winks_StrgInitMapFile() != WINKS_RETURN_SUCCESS)
    	{
    		Winks_printf("Winks_StrgInitMapFile ERROR\r\n");
            return WINKS_RETURN_FAILURE;
    	}
        //Winks_StrgFLDeleteAll();
        //Winks_StrgNDDeleteAll();
        
        #ifdef WINKS_DUAL_SIM
            iRet = Winks_SetActiveSim(imsiCnt);
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
        #endif
        
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_ENABLED;
            //Winks_engineContext_p->isImsiEnabled = WINKS_IMSI_ENABLED;
            //Winks_engineContext_p->ifRemindRegister = 0;
            /* 启动定时更新服务 */
            winks_StartPeriodPushTimer();

            StrgStatus = Winks_StrgGetStatus();
            if( StrgStatus == WINKS_STRG_CARD_STATUS_NO_CCCACHE ||
                StrgStatus == WINKS_STRG_CARD_STATUS_OLD_CARD )
            {
                /* 未完成Cache初始化 */
                winks_StartBuildCacheTimer();
            }
            return WINKS_RETURN_SUCCESS;
        }

        case 0:
        {
            /*引擎状态错误*/
            if(Winks_engineContext_p->curState >= WINKS_ENGINE_STATE_DISABLED)
                return WINKS_RETURN_FAILURE;

        #ifdef WINKS_DUAL_SIM
        {       
            unsigned short stt = Winks_engineContext_p->SimStatus;

            /*已激活了SIM卡*/
            if(Winks_engineContext_p->SimStatus & 0x00ff)
                return WINKS_RETURN_FAILURE;
                
            /*取两个卡的IMSI还有至少一张没有返回，故等待其返回。*/
            if((((stt>>8)&0x0f) | stt>>12)<3)
                return WINKS_RETURN_FAILURE;

            iRet = 0;
            if(stt&0x0100)
                iRet = Winks_SetActiveSim(1);
            else if(stt&0x0200)
                iRet = Winks_SetActiveSim(2);
            else
                return WINKS_RETURN_FAILURE;
            Winks_assert(iRet == WINKS_RETURN_SUCCESS);
        }
        #endif
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_DISABLED;  
        

#if 0//去掉沉默用户的需求
            unsigned char flag = 1;
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_DISABLED;  
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
#endif
            break;
        }
        
        default:
            return WINKS_RETURN_FAILURE;
            
    }

    if(Winks_StrgInitMapFile() != WINKS_RETURN_SUCCESS)
	{
		Winks_printf("Winks_StrgInitMapFile ERROR\r\n");
        return WINKS_RETURN_FAILURE;
	}
    
#ifdef WINKS_DEBUG
    /*测试功能开关*/
    Winks_SetOsalTestBitFlag(2, 0);/*debug打印开关*/
#endif
    /*Post msg to ui for pop infomation.*/
    //iRet = Winks_PostMsgToUi(WINKS_MSG_ENGINE_REG_NOTIFY, &Winks_engineContext_p->ifRemindRegister, 1);
    return WINKS_RETURN_SUCCESS;
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

static void winks_StartSrvMsgTimer(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    winks_EngStartTimer(&Winks_engineContext_p->ServerMsgTimer,
                        WINKS_SECONDS2TICKS(WINKS_SRVMSG_RETRY_SLOT), 
                        WINKS_TIMER_AUTO_START);
    Winks_printf("winks_StartSrvMsgTimer[%d] TimeSlot=%d \r\n", Winks_engineContext_p->ServerMsgTimer, WINKS_SRVMSG_RETRY_SLOT);
}

static void winks_StopSrvMsgTimer(void)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_printf("winks_StopSrvMsgTimer[%d] \r\n", Winks_engineContext_p->ServerMsgTimer );
    winks_EngStopTimer(&Winks_engineContext_p->ServerMsgTimer);
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
//    Winks_ConnectWayOperation_s CWOp;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_mem_set(TimeFlag, 0, sizeof(TimeFlag));
//    Winks_GetTimeFlag(TimeFlag, 32, 3);
//    Winks_mem_set(&CWOp, 0, sizeof(CWOp));
//    Winks_GetSupportedConnectWays( &CWOp);
    /*
    sprintf(ErrMsgBuf, "PeriodTimerStop: CWOp[0x%08x, %lu] cause(%d) at %s\r\n", \
        (int)CWOp.operation, CWOp.twuTimeSlot, Winks_engineContext_p->PTCloseCause, TimeFlag);*/

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
//    Winks_ConnectWayOperation_s CWOp;
//    unsigned long ClientConnectWay = 0;
    unsigned long IsPeriodWakeMode = 1;
    unsigned long CCShowSW;
    unsigned long TimeSlot;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->engChn == WINKS_CHN_NULL)
    {
        Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
        return WINKS_RETURN_FAILURE;
    }

    if( Winks_GetSettingValue(WINKS_SETTING_SERVICE_STOP_FLAG, &CCShowSW) ||
        CCShowSW == WINKS_SETVAL_SWITCH_ON)/* 现在为开启状态，关闭 */
    {
        Winks_printf("Err: WINKS_SETVAL_SWITCH_OFF \r\n");
        return WINKS_RETURN_FAILURE;
    }
#if 0
//    memset(&CWOp, 0, sizeof(CWOp));
//    Winks_GetSupportedConnectWays(&CWOp);/* 服务器支持的方式和默认的方式 */
    
//    if((CWOp.twuTimeSlot < WINKS_TIME_CONNECT_PERIOD_MIN) || (CWOp.twuTimeSlot > WINKS_TIME_CONNECT_PERIOD_MAX))
//        CWOp.twuTimeSlot = WINKS_TIME_CONNECT_PERIOD_DEF;
    
#ifdef WINKS_SIMULATOR     /* renzhan add for debug */
//    CWOp.twuTimeSlot = 60;
#endif
#else
    if(Winks_GetSettingValue(WINKS_SETTING_CONNECT_PERIOD, &TimeSlot))/* 定时更新周期 */
    {
        Winks_printf("Err: WINKS_SETTING_CONNECT_PERIOD \r\n");
        return WINKS_RETURN_FAILURE;
    }
    if((TimeSlot < WINKS_TIME_CONNECT_PERIOD_MIN) || (TimeSlot > WINKS_TIME_CONNECT_PERIOD_MAX))
        TimeSlot = WINKS_TIME_CONNECT_PERIOD_DEF;
#ifdef WINKS_SIMULATOR     /* renzhan add for debug */
    TimeSlot = /*60*/WINKS_TIME_CONNECT_PERIOD_DEF;
#endif
#endif

    /* 检查是否需要重启定时器 */
    if(Winks_engineContext_p->PeriodPushTimer)/* 定时更新服务已启动 */
    {
        winks_TimerBody_s timerInfo;
        
        Winks_mem_set(&timerInfo, 0, sizeof(timerInfo));
        if( (Winks_GetTimerInfo(Winks_engineContext_p->PeriodPushTimer, &timerInfo) == WINKS_RETURN_SUCCESS) &&
            //(CWOp.operation&CONNECT_WAY_TIMER_WAKE_UP) &&   /* 服务器支持定时更新 */
            (timerInfo.timeOutPeriod == WINKS_SECONDS2TICKS(TimeSlot/*CWOp.twuTimeSlot*/))) /* 定时服务周期与服务器下发的一致 */
        {
            Winks_printf("winks_StartPeriodPushTimer[%d] already ok! TimeSlot[%d] \r\n",
                Winks_engineContext_p->PeriodPushTimer, TimeSlot );
            return iRet;/* 直接返回不用更新定时器 */
        }
    }
#if 0
//    Winks_GetPhoneConnectWay(&ClientConnectWay);/* 用户选择的方式 */
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
#endif    
    if(IsPeriodWakeMode)
    {
        iRet = winks_EngStartTimer( &Winks_engineContext_p->PeriodPushTimer,
                    WINKS_SECONDS2TICKS(TimeSlot/*CWOp.twuTimeSlot*/), 
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
            Winks_printf("winks_StartPeriodPushTimer[%d] TimeSlot=%d \r\n",
                Winks_engineContext_p->PeriodPushTimer, TimeSlot );
        }
    }
    else
    {
//        if(!(CWOp.operation&CONNECT_WAY_TIMER_WAKE_UP))/* 服务器不支持定时更新 */
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
            Winks_printf("winks_DelayPeriodPushTimer[%d] already Delay:%d秒 state[%d] \r\n", 
                    Winks_engineContext_p->PeriodPushTimer, WINKS_PERIODTIMER_RETRY_SLOT,
                    Winks_engineContext_p->curState);
            return iRet;/* 直接返回不用更新定时器 */
        }
    }
    
    iRet = winks_EngStartTimer( &Winks_engineContext_p->PeriodPushTimer, 
                WINKS_SECONDS2TICKS(WINKS_PERIODTIMER_RETRY_SLOT), 
                WINKS_TIMER_AUTO_LOOP|WINKS_TIMER_AUTO_START );
    if(iRet != WINKS_RETURN_SUCCESS)
    {
    #ifdef WINKS_DEBUG
        Winks_GetSysDateTime( &Winks_engineContext_p->v_dateTime1 );
        Winks_engineContext_p->PTCloseCause = 4;
    #endif
    }
    else
    {
        Winks_printf("winks_DelayPeriodPushTimer[%d] Delay:%d秒 state[%d] \r\n", 
                Winks_engineContext_p->PeriodPushTimer, WINKS_PERIODTIMER_RETRY_SLOT,
                Winks_engineContext_p->curState);
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

static int winks_EngCallEvtInit(int CallType, char *pNo)
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(pNo == NULL)
        return WINKS_RETURN_FAILURE;
    if(Winks_engineContext_p->pCall == NULL)
        Winks_engineContext_p->pCall = (WinksEngCall_s *)Winks_salAlloc(sizeof(WinksEngCall_s));
    if(Winks_engineContext_p->pCall == NULL)
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(Winks_engineContext_p->pCall, 0, sizeof(WinksEngCall_s));
    strncpy(Winks_engineContext_p->pCall->NO, Winks_SkipPlus(pNo), sizeof(Winks_engineContext_p->pCall->NO));
    if(strlen(Winks_engineContext_p->pCall->NO) <= 2)
        return WINKS_RETURN_FAILURE;
    Winks_printf("winks_EngCallEvtInit \"%s\"-->\"%s\"\r\n", pNo, Winks_engineContext_p->pCall->NO);
    //Winks_mem_set(Winks_engineContext_p->pCall->CCShow.ID, 0, sizeof(Winks_engineContext_p->pCall->CCShow.ID));
    Winks_engineContext_p->pCall->CallType = CallType;
    Winks_GetSysDateTime(&Winks_engineContext_p->pCall->CallStartTime);

    return WINKS_RETURN_SUCCESS;
}

static int winks_EngRecordCallEvt()
{
    int iRet = WINKS_RETURN_FAILURE;
    WinksStrgFile_CallInfo_s CallInfo;
    unsigned long   CallTime = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->pCall == NULL)
        return WINKS_RETURN_FAILURE;
    if(strlen(Winks_engineContext_p->pCall->NO) <= 2)
    {
        Winks_salFree(Winks_engineContext_p->pCall);
        Winks_engineContext_p->pCall = NULL;
        return WINKS_RETURN_FAILURE;
    }

    Winks_printf("winks_EngRecordCallEvt \"%s\"\r\n", Winks_engineContext_p->pCall->NO);
    Winks_GetCCStop(WINKS_CCDOWN_TYPE_CALL);
    Winks_mem_set(&CallInfo, 0, sizeof(CallInfo));
    if((iRet = Winks_SysTime2CacheTime(&Winks_engineContext_p->pCall->CallStartTime, &CallTime)) == WINKS_RETURN_SUCCESS)
    {
        Winks_DateTime_s    CurrTime;
        Winks_DateTime_s    DiffTime;
        
        CallInfo.CallClock = CallTime;
        CallTime = 0;
        Winks_GetSysDateTime(&CurrTime);
        if(Winks_GetTimeDifference(&CurrTime, &Winks_engineContext_p->pCall->CallStartTime, &DiffTime) == WINKS_RETURN_SUCCESS)
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
        CallInfo.CallType = (unsigned char)Winks_engineContext_p->pCall->CallType;
        Winks_assert(Winks_engineContext_p->pCall->NO[0]);
        strncpy(CallInfo.NO, Winks_engineContext_p->pCall->NO, sizeof(CallInfo.NO));
        if(Winks_engineContext_p->pCall->CCShow.ID[0])
            strncpy(CallInfo.ID, Winks_engineContext_p->pCall->CCShow.ID, sizeof(CallInfo.ID));
        {
            Winks_DateTime_s *pCST = &Winks_engineContext_p->pCall->CallStartTime;
            Winks_printf("StartTime: %04d-%02d-%02d %02d:%02d:%02d\r\n", 
                pCST->year, pCST->month, pCST->day,
                pCST->hour, pCST->min, pCST->sec);
            Winks_printf("CallClock: %lu\r\n", CallInfo.CallClock);
            Winks_printf("CallTime : %lu\r\n", CallInfo.CallTime);
            Winks_printf("CallType : %lu\r\n", CallInfo.CallType);
            Winks_printf("Call_No  : %s\r\n", CallInfo.NO);
            Winks_printf("Call_ID  : %s\r\n", CallInfo.ID);
        }
        Winks_salFree(Winks_engineContext_p->pCall);
        Winks_engineContext_p->pCall = NULL;
        return Winks_StrgCallEvtAdd(&CallInfo);
    }

    return iRet;
}

static int winks_EngAppMsgHandler(Winks_EngineToUiReturnMsg * pAppRetMsg, unsigned long size)
{
    unsigned long Bit;
    int Finished = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    if(pAppRetMsg == NULL || size == 0)
        return 0;

    Winks_printf("########## winks_EngAppMsgHandler! code=[%d] size=%d \r\n", pAppRetMsg->code, size);
#ifdef WINKS_DEBUG
    winks_PrintEng2UiMsgInfo(pAppRetMsg);
#endif
    switch(pAppRetMsg->appType)
    {
    case WINKS_ENGPROTYPE_UPDATE_SW    :     /*软件升级*/
    case WINKS_ENGPROTYPE_UPDATE_SC    :    /*更新服务器配置信息*/
    case WINKS_ENGPROTYPE_UPDATE_UC    :    /*更新用户配置信息*/
    case WINKS_ENGPROTYPE_UPLOAD_LOG   :    /*上传log日志*/
    case WINKS_ENGPROTYPE_UPLOAD_STA   :    /*上传统计信息*/
        Bit = ((Winks_engineContext_p->SrvNotify.Notify>>16) & WINKS_NOTIFY_MASK);
        if(Bit)
        {
            if(pAppRetMsg->code < 0)
                Finished = 1;
            else
            {
                switch(pAppRetMsg->code)
                {
                case WINKS_MSG_ENG2UI_SRVCFG_OK:             /* 服务器配置成功 */
                case WINKS_MSG_ENG2UI_USERCFG_OK:            /* 本地配置成功 */
                case WINKS_MSG_ENG2UI_ULSTAT_OK:             /* 统计信息上传成功 */
                case WINKS_MSG_ENG2UI_ULLOG_OK:              /* log信息上传成功 */
                case WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW:     /* 没有新版本 */
                case WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK:      /* 新版本下载成功，安装 */
                    /* 清低位的消息标志 -- 只在成功完成时清 */
                    //Winks_engineContext_p->SrvNotify.Notify &= ~Bit;
                    Finished = 1;
                    break;
                }
            }

            if(Finished)
            {
                /* 清高位的正在执行标志 */
                Winks_engineContext_p->SrvNotify.Notify &= WINKS_NOTIFY_MASK;
                /* 清低位的消息标志 -- 不论成功失败，处理过消息就清 */
                Winks_engineContext_p->SrvNotify.Notify &= ~Bit;
              
                if(winks_SrvMsgDeal() != WINKS_RETURN_SUCCESS)
                    winks_StartSrvMsgTimer();
            }
        }
        break;
    }

    return 0;
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
        case WINKS_MSG_TIMER_EXPIRY:
            winks_TimerExpiryProc(data, size);
            return 0;
        case WINKS_MSG_TEST:
#ifdef WINKS_DEBUG
            if(strlen(data) == WINKS_TEST_KEY_WORDS_LEN)
                Winks_TestModeManager((char *)data);            
#endif
            return 0;
        default:
            break;
    }

    Winks_printf("########## winks_EngineIdleHandler! state=[%d] msg=[0x%04x] size=%d\r\n", Winks_engineContext_p->curState, msg, size);

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
            Winks_printf("Engine INIT.\r\n");
            Winks_engineContext_p->curState = WINKS_ENGINE_STATE_START;
            Winks_engineContext_p->engChn = *((WINKS_CHN_ID *)data);
            Winks_engineContext_p->http = WINKS_HTTP_NULL;
            Winks_assert(Winks_engineContext_p->engChn != WINKS_CHN_NULL);
            /*Get all call event.*/
            Winks_SetWaitingEvent(Winks_engineContext_p->engChn, WINKS_EVENT_OF_ALL_CALL|WINKS_EVENT_TEST_KET);
            Winks_AddWaitingEvent(Winks_engineContext_p->engChn, WINKS_EVENT_USB_DETECT);


            /*开始比对IMSI激活状态*/            
#if 0
        #ifdef WINKS_DUAL_SIM
            Winks_engineContext_p->CurInquireSimNo = 1;
        #endif /* WINKS_DUAL_SIM */
#endif
            iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
            if(iRet == WINKS_RETURN_SUCCESS)
            {
                Winks_printf("Engine INIT Winks_GetImsi success.\r\n");
#ifdef WINKS_DUAL_SIM
                Winks_engineContext_p->SimStatus |= 0x0100;/* Sim卡1的Imsi已获取 */
#endif
                if(winks_CheckFoundSim(buff, sizeof(buff)) == 1)/* 找到了已注册的Imsi,不必获取卡2的IMSI了 */
                {
                    winks_BootingWithSimCard(1, NULL, 0, NULL, 0);
                    break;//已经匹配了SIM卡，故跳出流程。
                }
                else
                {
#ifdef WINKS_DUAL_SIM
#else
                    winks_BootingWithSimCard(0, buff, sizeof(buff), NULL, 0);
                    break;//单卡且已经匹配过SIM卡，故跳出流程。
#endif
                }
            }
            else 
            {
                if(iRet == WINKS_RETURN_FAILURE)
                {
                    Winks_printf("Waring:Engine got imsi failed.\r\n");
#ifdef WINKS_DUAL_SIM
                    Winks_engineContext_p->SimStatus |= 0x1000;
#endif
#if 0
                    Winks_printf("Engine INIT Winks_GetImsi ret fail!\r\n");
                    /*延时读取IMSI code*/
                    if(Winks_engineContext_p->getImsiTimer == WINKS_TIMER_NULL)
                    {
                        int timeOutPeriod = Winks_GetTimeForImsi(Winks_engineContext_p->getImsiCount);
                        Winks_engineContext_p->getImsiTimer = Winks_CreateTimer(
                            Winks_engineContext_p->engChn,
                            WINKS_MILLISECONDS2TICKS(timeOutPeriod), 
                            WINKS_TIMER_AUTO_START);
                    }
#endif
                }
                /*else waiting message [WINKS_MSG_IMSI_REQ] at state [WINKS_ENGINE_STATE_START]*/
            }
            
#ifdef WINKS_DUAL_SIM
            iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
            if(iRet == WINKS_RETURN_SUCCESS)
            {
                Winks_printf("Engine INIT Winks_GetImsi_2 success.\r\n");
                Winks_engineContext_p->SimStatus |= 0x0200;/* Sim卡2的Imsi已获取 */
                if(winks_CheckFoundSim(buff, sizeof(buff)) == 1)/* 找到了已注册的Imsi,不必获取卡2的IMSI了 */
                {
                    winks_BootingWithSimCard(2, NULL, 0, NULL, 0);
                }
                else
                {
                    winks_BootingWithSimCard(0, NULL, 0, buff, sizeof(buff));
                }
            }
            else 
            {
                if(iRet == WINKS_RETURN_FAILURE)
                {
                    Winks_printf("Waring:Engine got imsi_2 failed.\r\n");
                    Winks_engineContext_p->SimStatus |= 0x2000;

                    /*第二张卡失败，第一张卡成功以第一张卡为主卡激活。*/
                    if(Winks_engineContext_p->SimStatus&0x0100)
                        winks_BootingWithSimCard(0, NULL, 0, NULL, 0);
#if 0
                    Winks_printf("Engine INIT Winks_GetImsi ret fail!\r\n");
                    /*延时读取IMSI code*/
                    if(Winks_engineContext_p->getImsiTimer == WINKS_TIMER_NULL)
                    {
                        int timeOutPeriod = Winks_GetTimeForImsi(Winks_engineContext_p->getImsiCount);
                        Winks_engineContext_p->getImsiTimer = Winks_CreateTimer(
                            Winks_engineContext_p->engChn,
                            WINKS_MILLISECONDS2TICKS(timeOutPeriod), 
                            WINKS_TIMER_AUTO_START);
                    }
#endif
                }
                /*else waiting message [WINKS_MSG_IMSI_REQ] at state [WINKS_ENGINE_STATE_START]*/
            }
#endif/*End WINKS_DUAL_SIM*/

            break;
        }
        default:
            break;
        }
        break;
    }

    case WINKS_ENGINE_STATE_START:
    {
        int  imsiCnt = 0;
        char buff[WINKS_IMSI_LEN+1];
        
        Winks_mem_set(buff, 0, sizeof(buff));
        switch (msg)
        {
#ifdef WINKS_DUAL_SIM
        case WINKS_MSG_IMSI_2_REQ: ++imsiCnt;
#endif
        case WINKS_MSG_IMSI_REQ: ++imsiCnt;
        {
            if(size == WINKS_IMSI_LEN)
                strncpy(buff, (char *)data, WINKS_IMSI_LEN);
            if(strlen(buff) == WINKS_IMSI_LEN)
            {
#ifdef WINKS_DUAL_SIM
                Winks_engineContext_p->SimStatus |= (imsiCnt<<8);/* Sim卡Imsi已获取 */
#endif
                
                if(winks_CheckFoundSim(buff, sizeof(buff)) == 1)/* 找到了已注册的Imsi,不必获取卡2的IMSI了 */
                {
                    winks_BootingWithSimCard(imsiCnt, NULL, 0, NULL, 0);
                }
                else
                {
                    winks_BootingWithSimCard(0, buff, sizeof(buff), NULL, 0);
                }
            }
            else
            {
                Winks_printf("Waring:Engine got imsi[%] failed..\r\n",imsiCnt);
#ifdef WINKS_DUAL_SIM
                Winks_engineContext_p->SimStatus |= (imsiCnt<<12);
#endif
                winks_BootingWithSimCard(0, NULL, 0, NULL, 0);
            }
            break;
        }

#if 0
            
        case WINKS_MSG_IMSI_REQ:
    #ifdef WINKS_DUAL_SIM
        case WINKS_MSG_IMSI_2_REQ:
    #endif
        {
            int  FoundRegImsi = -1;
            char buff[WINKS_IMSI_LEN+1];

            Winks_printf("Engine START.\r\n");
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
                if(msg == WINKS_MSG_IMSI_REQ)
                    Winks_printf("IMSI: %s\r\n", buff);
                else
                    Winks_printf("IMSI2: %s\r\n", buff);
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
                    iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff1, sizeof(buff1));
                    Winks_printf("start found 2 Winks_GetImsi_2 return %d\r\n", iRet);
                }
                else/* 当前buff存放的是Sim卡2的imsi,取出先前已获取到的Sim卡1的imsi */
                {
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff1, sizeof(buff1));
                    Winks_printf("start found 2 Winks_GetImsi return %d\r\n", iRet);
                }
                Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                winks_BootingWithSimCard(2,  buff, sizeof(buff), buff1, sizeof(buff1));
            }
            /* 已获取1张Sim卡的imsi,另一张Sim卡多试了5次仍没有获取到imsi,认为另一卡不存在,引擎启动到未注册状态 -- 单卡 */
            else if( (Winks_engineContext_p->SimStatus & 0xff00) &&
                     (Winks_engineContext_p->OtherSimRetryCnt >= 5) )
            {
                if((Winks_engineContext_p->SimStatus & 0xff00) == 0x0100)/* Sim卡1的imsi已获取,读出该imsi */
                {
                    //Winks_assert(Winks_HasGotImsi(1) == TRUE);
                    iRet = Winks_GetImsi(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                    Winks_printf("start found 1 Winks_GetImsi return %d\r\n", iRet);
                    iRet = Winks_UserSetActiveSim(1);
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                }
                else
                {
                    //Winks_assert(Winks_HasGotImsi(2) == TRUE);
                    iRet = Winks_GetImsi_2(Winks_engineContext_p->engChn, buff, sizeof(buff));
                    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
                    Winks_printf("start found 1 Winks_GetImsi_2 return %d\r\n", iRet);
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
#endif

#if 0
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
#endif
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
            Winks_printf("error case: state[%d] msg[0x%08x]\r\n", 
                Winks_engineContext_p->curState, msg);
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
            unsigned long value;
            
            if(winks_EngCallEvtInit((msg==WINKS_MSG_CALL_DIAL), (char *)data) != WINKS_RETURN_SUCCESS)
                break;

            /* 设置不展示彩像时不联网不取彩像 */
            if( Winks_GetSettingValue(WINKS_SETTING_SHOW_CASE, &value) == WINKS_RETURN_SUCCESS &&
                value ==0 )
                break;
            
            if(data)/* 避免无来电显示引发问题 */
            {
                if(Winks_StrgGetCCShow( Winks_engineContext_p->pCall->NO, 
                                        Winks_engineContext_p->pCall->CallType, 
                                        &Winks_engineContext_p->pCall->CCShow)
                                     == WINKS_RETURN_SUCCESS )
                {
                    Winks_StrgLockCC(Winks_engineContext_p->pCall->CCShow.CC_Idx);
                }
                
                if( Winks_GetSettingValue(WINKS_SETTING_SERVICE_STOP_FLAG, &value) == WINKS_RETURN_SUCCESS &&
                    value==1 )/* 现在为停止状态，不联网更新 */
                    break;
                
            #ifdef WINKS_SUPPORT_3G
              {
                unsigned long   allowFlag = 0;
                /*检查漫游标志:国际漫游不联网设置*/
                Winks_isRoamingConnectionAllowed((unsigned char *)&allowFlag);
                if( allowFlag == 1 )
                {
                    Winks_GetCCStart(WINKS_ENGINE_APP_PRI_CALL, WINKS_CCDOWN_TYPE_CALL, NULL);
                    winks_StartCallWaitTimer();
                    Winks_engineContext_p->pCall->CallCCStatus = WINKS_CALLCC_STATUS_GETINFO;
                }
              }
            #else /* WINKS_SUPPORT_3G */
                Winks_StrgULAdd(Winks_SkipPlus((char *)data));
            #endif /* WINKS_SUPPORT_3G */
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
            {
            unsigned long value;
            
            winks_EngRecordCallEvt();
            
            /* 设置不展示彩像时不联网不取彩像 */
            if( Winks_GetSettingValue(WINKS_SETTING_SHOW_CASE, &value) == WINKS_RETURN_SUCCESS &&
                value ==0 )
                break;
            
            if( Winks_GetSettingValue(WINKS_SETTING_HANGUP_UPDATA_SWITCH, &value) == WINKS_RETURN_SUCCESS &&
                value ==0 )/* 现在不支持挂断后更新，挂断后不更新 */
                break;
            
            Winks_GetCCStart(WINKS_ENGINE_APP_PRI_NORMAL, WINKS_CCDOWN_TYPE_RECRUIT, NULL);
            }
            break;
        case WINKS_MSG_CALL_ANSWER:             /* 本地接听 */    
        case WINKS_MSG_CALL_REMOTE_ANSWER:      /* 对方接听 */  
            break;

        case WINKS_MSG_ENGINE_PUSH:/*连网事件*/
            /*连接*/
            if(Winks_GetCCStart(WINKS_ENGINE_APP_PRI_NORMAL, WINKS_CCDOWN_TYPE_PERIOD, NULL) != WINKS_RETURN_SUCCESS)
            {
                /* 引擎忙，延时120秒再启动定时更新服务 */
                winks_DelayPeriodPushTimer();
            }
        #ifdef WINKS_DEBUG
            Winks_GetSysDateTime( &Winks_engineContext_p->v_dateTime );
        #endif
            break;
        case WINKS_MSG_ENGINE_APP_RETMSG:
            return winks_EngAppMsgHandler((Winks_EngineToUiReturnMsg *)data, size);
        default:
            Winks_printf("error case: state[%d] msg[0x%08x]\r\n", 
                Winks_engineContext_p->curState, msg);
            break;
        }
        break;
    }
    default: 
        Winks_printf("error case: state[%d] msg[0x%08x]\r\n", 
            Winks_engineContext_p->curState, msg);
        break;
    }
	return 0;
}


/*取当前空闲的引擎应用控制块ID
 * function: winks_GetCurFreeEngAppId
 * parameters:
 *   void
 * return:
 *   成功返回:ID(>=0)
 *   失败返回:-1
 */
static int winks_GetCurFreeEngAppId(void)
{
    int i = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    while(Winks_engineContext_p->engAppStack[i].appHnd)
    {
        ++i;
        if(i >= WINKS_ENGINE_APP_QUEUE_SIZE)
            return WINKS_RETURN_FAILURE;
    }
    return i;
}

/*取当前运行状态的引擎应用控制块ID
 * function: winks_GetCurRunEngAppId
 * parameters:
 *   void
 * return:
 *   成功返回:ID(>=0)
 *   失败返回:-1
 */
static int winks_GetCurRunEngAppId(void)
{
    int i = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    for(i = WINKS_ENGINE_APP_QUEUE_SIZE-1; i>=0; --i)
    {
        if(Winks_engineContext_p->engAppStack[i].appHnd)
            return i;
    }
    return WINKS_RETURN_FAILURE;
}

/*取指定的引擎应用控制块ID
 * function: winks_GetEngAppId
 * parameters:
 *   unsigned long appHnd
 * return:
 *   成功返回:ID(>=0)
 *   失败返回:-1
 */
static int winks_GetEngAppId(unsigned long appHnd)
{
    int i = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(appHnd > 0);

    for(i = WINKS_ENGINE_APP_QUEUE_SIZE-1; i>=0; --i)
    {
        if(Winks_engineContext_p->engAppStack[i].appHnd)
        {
            if(Winks_engineContext_p->engAppStack[i].appHnd != appHnd)
            {
                Winks_printf("/appHnd is not the head in stack.[%ld]\r\n",appHnd);
                Winks_fail("");
                return WINKS_RETURN_FAILURE;
            }
            else
                return i;
        }
    }
    return WINKS_RETURN_FAILURE;
}

#if 0
/*根据应用名称查找应用ID
 * function: winks_FindEngAppIdByName
 * parameters:
 *   char * appName: 引擎应用的名称
 * return:
 *   成功返回:ID(>=0)
 *   失败返回(没找到):-1
 */
static int winks_FindEngAppIdByName(char * appName)
{
    int i = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    for(i = WINKS_ENGINE_APP_QUEUE_SIZE-1; i>=0; --i)
    {
        if(strcmp(Winks_engineContext_p->engAppStack[i].appName,appName) == 0)
            return i;
    }
    return WINKS_RETURN_FAILURE;
}
#endif

/*根据应用名称查找应用句柄
 * function: winks_FindEngAppHndByName
 * parameters:
 *   char * appName: 引擎应用的名称
 * return:
 *   成功返回:应用句柄(>0)
 *   失败返回:0(WINKS_ENGIEN_APP_HANDLER_NULL)
 */
WINKS_ENAPP_HND winks_FindEngAppHndByName(char * appName)
{
    int i = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    for(i = WINKS_ENGINE_APP_QUEUE_SIZE-1; i>=0; --i)
    {
        if(strcmp(Winks_engineContext_p->engAppStack[i].appName,appName) == 0)
            return Winks_engineContext_p->engAppStack[i].appHnd;
    }
    return WINKS_ENGINE_APP_HANDLER_NULL;
}

/*根据应用名称查找应用句柄
 * function: winks_GetEngAppAddData
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 * return:
 *   成功返回:addData指针
 *   失败返回:NULL
 */
void * winks_GetEngAppAddData(WINKS_ENAPP_HND appHnd)
{
    int i = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    Winks_assert(appHnd != WINKS_ENGINE_APP_HANDLER_NULL);

    for(i = WINKS_ENGINE_APP_QUEUE_SIZE-1; i>=0; --i)
    {
        if(Winks_engineContext_p->engAppStack[i].appHnd == appHnd)
            return Winks_engineContext_p->engAppStack[i].addData;
    }
    return NULL;
}


/*创建并启动引擎应用(例如激活流程和DIY流程就各是一个引擎应用。
 * function: Winks_EngAppStart
 * parameters:
 *   Winks_EngineAppPri_e appPri: 应用优先级
 *   WINKS_ENGAPP_PROCESS prcFn: 处理回调函数
 *   char * appName: 引擎应用的名称
 *   void * addData: (in)数据指针
 * return:
 *   成功返回:应用句柄(>0)
 *   失败返回:0(WINKS_ENGIEN_APP_HANDLER_NULL)
 */
WINKS_ENAPP_HND Winks_EngAppStart(Winks_EngineAppPri_e appPri, WINKS_ENGAPP_PROCESS prcFnc, char * appName, void *addData)
{
    int iRet = 0;
    int appId = 0;
    unsigned long appHnd = 0;
	Winks_EngineAppControl_s *acp = NULL;

    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_assert(prcFnc != NULL);
    Winks_assert(appName != NULL);
    Winks_assert(strlen(appName) > 0);
    Winks_assert(strlen(appName) < WINKS_ENAPP_NAME_MAX_LEN);
    Winks_assert(appPri < WINKS_ENGINE_APP_PRI_NUM);

    if((Winks_engineContext_p->curState > WINKS_ENGINE_STATE_NUMBER) 
        || (Winks_engineContext_p->curState <= WINKS_ENGINE_STATE_START))
    {
        /*引擎状态异常。*/
        Winks_printf("Err: engine curState is invalid.[%d]\r\n", Winks_engineContext_p->curState);
        return WINKS_ENGINE_APP_HANDLER_NULL;
    }

    iRet = winks_GetCurFreeEngAppId();
    if(iRet == WINKS_RETURN_FAILURE)
    {
        /*引擎应用控制块满*/
        Winks_printf("Err: engine app controle block if full.\r\n");
        return WINKS_ENGINE_APP_HANDLER_NULL;
    }
    
    appId = iRet;
    Winks_assert(appId >= 0);
    
    if(winks_FindEngAppHndByName(appName) != WINKS_ENGINE_APP_HANDLER_NULL)
    {
        /*引擎应用已经存在*/
        Winks_printf("Err: engine app already exist. appName[%s]\r\n",appName);
        return WINKS_ENGINE_APP_HANDLER_NULL;
    }

    if(appId != 0)
    {
        if(Winks_engineContext_p->engAppStack[appId-1].appPri >= appPri)
        {
            /*优先级低，无法打断当前应用。*/
            Winks_printf("Warn: input pri is lower than current app's  appName[%s].\r\n", \
                    Winks_engineContext_p->engAppStack[appId-1].appName);
            return WINKS_ENGINE_APP_HANDLER_NULL;
        }
        else
        {
            /*先关闭Http*/
            if(Winks_engineContext_p->http != WINKS_HTTP_NULL)
            {
                winks_EngHttpClose(Winks_engineContext_p->http);
                //Winks_engineContext_p->http = WINKS_HTTP_NULL;
            }
            
            /*发同步挂起消息*/
            acp = &(Winks_engineContext_p->engAppStack[appId-1]);
            Winks_printf("Engine App pause[%s].\r\n",acp->appName);
            (*(acp->prcFnc))(acp->appHnd, WINKS_MSG_ENGINE_APP_PAUSE, NULL, 0, acp->addData);
        }
    }
        
    Winks_engineContext_p->engAppStack[appId].addData = addData;
    Winks_engineContext_p->engAppStack[appId].appPri = appPri;
    Winks_engineContext_p->engAppStack[appId].prcFnc = prcFnc;
    strcpy(Winks_engineContext_p->engAppStack[appId].appName, appName);
    Winks_engineContext_p->engAppStack[appId].appHnd = ++(Winks_engineContext_p->engAppSequence);

    /*设置appHnd != 0*/
    if(!(Winks_engineContext_p->engAppStack[appId].appHnd))
        Winks_engineContext_p->engAppStack[appId].appHnd = ++(Winks_engineContext_p->engAppSequence);

    appHnd = Winks_engineContext_p->engAppStack[appId].appHnd;
    
    Winks_printf("Engine App start[%s].\r\n",appName);
    (*prcFnc)(appHnd, (unsigned long)WINKS_MSG_ENGINE_APP_START, NULL, 0, addData);
    
    return appHnd;
}


/*退出并注销动引擎应用(例如激活流程和DIY流程就各是一个引擎应用。
 * function: Winks_EngAppStop
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_EngAppStop(WINKS_ENAPP_HND appHnd)
{
    int iRet = 0;
    int appId = 0;
    Winks_EngineAppControl_s *acp = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_assert(appHnd != 0);

    iRet = winks_GetEngAppId(appHnd);
    if(iRet == WINKS_RETURN_FAILURE)
    {
        /*指定的应用不在栈顶*/
        Winks_printf("Err: not stack head.\r\n");
        return WINKS_RETURN_FAILURE;
    }

    appId = iRet;
    Winks_assert(appId >= 0);

    Winks_printf("Engine App stop[%s].\r\n",Winks_engineContext_p->engAppStack[appId].appName);
    Winks_mem_set(&(Winks_engineContext_p->engAppStack[appId]), 0, sizeof(Winks_EngineAppControl_s));
        
    /*关闭Http*/
    if(Winks_engineContext_p->http != WINKS_HTTP_NULL)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        //Winks_engineContext_p->http = WINKS_HTTP_NULL;
    }

    /*挂起态转为运行态*/
    if(appId > 0)
    {
        acp = &(Winks_engineContext_p->engAppStack[appId-1]);
        Winks_printf("Engine App resume[%s].\r\n",acp->appName);
        (*(acp->prcFnc))(acp->appHnd, WINKS_MSG_ENGINE_APP_RESUME, NULL, 0, acp->addData);
    }
    return WINKS_RETURN_SUCCESS;
}

/*销毁引擎应用机制。
 * function: Winks_EngAppDestroy
 * parameters:
 *   
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
static int Winks_EngAppDestroy(void)
{
    int i = 0;
    Winks_EngineAppControl_s *acp = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    /*关闭Http*/
    if(Winks_engineContext_p->http != WINKS_HTTP_NULL)
    {
        winks_EngHttpClose(Winks_engineContext_p->http);
        //Winks_engineContext_p->http = WINKS_HTTP_NULL;
    }

    /*停止已注册的所有应用*/
    for(i = WINKS_ENGINE_APP_QUEUE_SIZE-1; i>=0; --i)
    {
        if(Winks_engineContext_p->engAppStack[i].appHnd == 0)
            continue;

        Winks_assert(Winks_engineContext_p->engAppStack[i].prcFnc != NULL);

        acp = &(Winks_engineContext_p->engAppStack[i]);
        (*(acp->prcFnc))(acp->appHnd, WINKS_MSG_ENGINE_APP_STOP, NULL, 0, acp->addData);
    }

    Winks_mem_set(Winks_engineContext_p->engAppStack, 0, sizeof(Winks_engineContext_p->engAppStack));
    return WINKS_RETURN_SUCCESS;
}

/*发同步消息给当前运行的引擎应用。
 * function: Winks_SendMsgToRunningEngApp
 * parameters:
 *   unsigned long msgid:同步消息的ID
 *   void *data: 发送的数据
 *   unsigned long dataSize: 发送数据的长度
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
static int Winks_SendMsgToRunningEngApp(unsigned long msgid, void *data, unsigned long dataSize)
{
    int iRet = 0;
    Winks_EngineAppControl_s *acp = NULL;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    iRet = winks_GetCurRunEngAppId();
    if(iRet == WINKS_RETURN_FAILURE)
    {
        Winks_printf("All engine app isn't running.\r\n");
        Winks_fail("");
        return WINKS_RETURN_FAILURE;
    }

    acp = &(Winks_engineContext_p->engAppStack[iRet]);
    Winks_assert(acp->prcFnc != NULL);
    
    (*(acp->prcFnc))(acp->appHnd, msgid, data, dataSize, acp->addData);

    return WINKS_RETURN_SUCCESS;
}


/*将数据解析的结果同步返回给当前运行的引擎应用。
 * function: Winks_SendReturnMsgToEngApp
 * parameters:
 *   void *data: 发送的数据
 *   unsigned long dataSize: 发送数据的长度
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
static int Winks_SendReturnMsgToEngApp(void *data, unsigned long dataSize)
{
    return Winks_SendMsgToRunningEngApp(WINKS_MSG_ENGINE_RETMSG, data, dataSize);
}

/*引擎应用创建TIMER。
 * function: Winks_EngCreateTimer
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 *   WINKS_TICKS timeOutPeriod:创建定时器的定时周期
 *   unsigned long flag: 创建定时器的属性标志。
 *                       WINKS_TIMER_DEFAULT: 默认
 *                       WINKS_TIMER_AUTO_START:自启动; 
 *                       WINKS_TIMER_AUTO_LOOP: 循环启动;
 * return:
 *   成功返回:定时器句柄(>0)
 *   失败返回:0
 */
WINKS_TIMER_ID Winks_EngCreateTimer(WINKS_ENAPP_HND appHnd, WINKS_TICKS timeOutPeriod, unsigned long flag)
{
    WINKS_TIMER_ID timerId = WINKS_TIMER_NULL;
    int iRet = 0;
    int appId = 0;
    int i = 0;
    Winks_EngineAppControl_s *acp = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_assert(appHnd != 0);
    Winks_assert(timeOutPeriod != 0);

    iRet = winks_GetEngAppId(appHnd);
    if(iRet == WINKS_RETURN_FAILURE)
    {
        /*指定的应用不在栈顶*/
        Winks_printf("Err: not stack head.\r\n");
        return 0;
    }

    appId = iRet;
    Winks_assert(appId >= 0);

    timerId = Winks_CreateTimer(Winks_engineContext_p->engChn, timeOutPeriod, flag);
    if(timerId == WINKS_TIMER_NULL)
    {
        return timerId;
    }
    
    acp = &(Winks_engineContext_p->engAppStack[appId]);
    for(i = 0; i<WINKS_ENGINE_APP_TIMER_NUM ; ++i)
    {
        if(acp->timer[i] == WINKS_TIMER_NULL)
        {
            acp->timer[i] = timerId;
            return timerId;
        }
    }

    Winks_DestroyTimer(timerId);
    Winks_printf("Timer queue is full.\r\n");
    Winks_fail("");
    return WINKS_TIMER_NULL;    
}

/*引擎应用删除定时器。
 * function: Winks_EngDestroyTimer
 * parameters:
 *   WINKS_ENAPP_HND appHnd: 应用句柄(>0)
 *   WINKS_TIMER_ID timerId: 定时器句柄(>0)
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_EngDestroyTimer(WINKS_ENAPP_HND appHnd, WINKS_TIMER_ID timerId)
{
    int iRet = 0;
    int appId = 0;
    int i = 0;
    Winks_EngineAppControl_s *acp = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_assert(appHnd != 0);
    Winks_assert(timerId != 0);

    iRet = winks_GetEngAppId(appHnd);
    if(iRet == WINKS_RETURN_FAILURE)
    {
        /*指定的应用不在栈顶*/
        Winks_printf("Err: not stack head.\r\n");
        return WINKS_RETURN_FAILURE;
    }

    appId = iRet;
    Winks_assert(appId >= 0);
    acp = &(Winks_engineContext_p->engAppStack[appId]);
    for(i = 0; i<WINKS_ENGINE_APP_TIMER_NUM ; ++i)
    {
        if(acp->timer[i] == timerId)
        {
            acp->timer[i] = WINKS_TIMER_NULL;
            iRet = Winks_DestroyTimer(timerId);
            return iRet;
        }
    }

    Winks_DestroyTimer(timerId);
    Winks_printf("Not find spacail time[%d] in Timer queue.\r\n",timerId);
    Winks_fail("");
    return WINKS_RETURN_FAILURE;
}

/*引擎应用模块定时器处理函数。
 * function: winks_EngAppTimerProc
 * parameters:
 *   WINKS_TIMER_ID timerId: 定时器句柄(>0)
 * return:
 *   成功返回:0
 *   失败返回:-1(没找到指定的处理函数)
 */
static int winks_EngAppTimerProc(WINKS_TIMER_ID timerId)
{
    int i = 0;
    int j = 0;
    
    Winks_EngineAppControl_s *acp = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    for(j = 0; j < WINKS_ENGINE_APP_QUEUE_SIZE; ++j)
    {
        acp = &(Winks_engineContext_p->engAppStack[j]);
        if(acp->appHnd != 0)
        {
            Winks_assert(acp->prcFnc != NULL);
            for(i = 0; i<WINKS_ENGINE_APP_TIMER_NUM ; ++i)
            {
                if(acp->timer[i] == timerId)
                {
                    (*(acp->prcFnc))(acp->appHnd, WINKS_MSG_TIMER_EXPIRY, &timerId, sizeof(timerId), acp->addData);
                    return WINKS_RETURN_SUCCESS;
                }
            }
        }
    }
    
    return WINKS_RETURN_FAILURE;
}

/*设置通话设置,从不选到某些项被选时需做一次定时更新。反之从选到不选要停止定时更新。
 * function: Winks_SetShowCaseProc
 * parameters:
 *   unsigned long val: 通话设置：WINKS_SETVAL_CCSHOW_RING、WINKS_SETVAL_CCSHOW_TALK、WINKS_SETVAL_CCSHOW_HANGUP
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SetShowCaseProc(unsigned long val)
{
    int iRet = 0;
    unsigned long valold = 0;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    iRet = Winks_GetSettingValue(WINKS_SETTING_SHOW_CASE, &valold);
    if(iRet == WINKS_RETURN_FAILURE)
        return WINKS_RETURN_FAILURE;

    iRet = Winks_SetSettingValue(WINKS_SETTING_SHOW_CASE, val);
    if(iRet == WINKS_RETURN_FAILURE)
        return WINKS_RETURN_FAILURE;
    if((valold == 0) && (val > 0))
    {
        /*do 定时更新*/
        iRet = winks_StartPeriodPushTimer();
        Winks_PostMsg(Winks_engineContext_p->engChn, WINKS_MSG_ENGINE_PUSH, NULL, 0);
        //Winks_fail("");
    }
    else if((valold > 0) && (val == 0))
    {
        /*stop 定时更新*/
        iRet = winks_StopPeriodPushTimer(6);
        //Winks_fail("");
    }
    return WINKS_RETURN_SUCCESS;
}

int Winks_GetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo)
{

    int iRet = 0;
    unsigned long ConnWay = 0, val;
    
    Winks_assert(pPeriodUpdateInfo);
    if(Winks_GetSettingValue(WINKS_SETTING_CONNECT_WAY, &ConnWay) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    pPeriodUpdateInfo->Method = ConnWay;
    switch(ConnWay)
    {
    case WINKS_PERIODUPDATEMETHOD_SAVEMONEY:
        pPeriodUpdateInfo->Hour = 24;
        pPeriodUpdateInfo->Min = 60;
        pPeriodUpdateInfo->Period = PU_SAVEMONEY_DFT_PERIOD;
        break;
    case WINKS_PERIODUPDATEMETHOD_DATANEW:
        pPeriodUpdateInfo->Hour = 24;
        pPeriodUpdateInfo->Min = 60;
        pPeriodUpdateInfo->Period = PU_DATANEW_DFT_PERIOD;
        break;
    case WINKS_PERIODUPDATEMETHOD_CUSTOM:
        //TODO:
        //pPeriodUpdateInfo->Hour = ;
        //pPeriodUpdateInfo->Min = ;
        if(Winks_GetSettingValue(WINKS_SETTING_CONNECT_PERIOD, &val) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        pPeriodUpdateInfo->Period = val;
        break;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_SetPeriodUpdate(Winks_PeriodUpdateInfo_s * pPeriodUpdateInfo)
{

    int iRet = 0;
    
    Winks_assert(pPeriodUpdateInfo);
    if(pPeriodUpdateInfo->Method == WINKS_PERIODUPDATEMETHOD_CUSTOM)
    {
        //TODO:
        //pPeriodUpdateInfo->Hour = ;
        //pPeriodUpdateInfo->Min = ;
        if(Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD, pPeriodUpdateInfo->Period)
                != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    if(Winks_SetSettingValue(WINKS_SETTING_CONNECT_WAY, pPeriodUpdateInfo->Method) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    return WINKS_RETURN_SUCCESS;
}

/*设置服务器下发的服务配置项*/
int Winks_SetSrvConfig(WinksSrvConfig_s srvConfig)
{   
    return Winks_SetSettingBlock(WINKS_SETTING_BLOCK_SERVER_CONFIG, &srvConfig, sizeof(WinksSrvConfig_s));    
}
/*获取服务器下发的服务配置项*/
int Winks_GetSrvConfig(WinksSrvConfig_s *srvConfig)
{
    Winks_assert(srvConfig != NULL);
    return Winks_GetSettingBlock(WINKS_SETTING_BLOCK_SERVER_CONFIG, (void *)srvConfig, sizeof(WinksSrvConfig_s));
}

/*查询本地用户激活状态*/
int Winks_UserGetState(int *state)
{
    char uid[WINKS_SERVER_UID_MAX_LEN]={0};
    int iRet = 0;
    unsigned long val = 0;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if((Winks_engineContext_p->curState <= WINKS_ENGINE_STATE_START) ||
       (Winks_engineContext_p->curState >= WINKS_ENGINE_STATE_NUMBER) )
    {
        *state = 4;
        return 0;
    }

    iRet = Winks_GetSettingValue(WINKS_SETTING_FORBIDDEN_FLAG, &val);
    if(val != 0)
    {
        *state = 3;
        return 0;
    }
    
    iRet = Winks_GetSettingValue(WINKS_SETTING_UNSUBSCRIBE_FLAG, &val);
    if(val != 0)
    {
        *state = 2;
        return 0;
    }

    iRet = Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid));
    if(strlen(uid) != 0 &&
        (Winks_engineContext_p->curState == WINKS_ENGINE_STATE_ENABLED
        /*|| Winks_engineContext_p->curState == WINKS_ENGINE_STATE_SHOWB*/))
    {
        *state = 1;/*已激活*/
        return 0;
    }
    
    *state = 0;
    return 0;
}

/*加密算法(base64)
 * function: Winks_Encode
 * parameters:
 *   char *src: 明文
 *   int src_len: 明文长度
 *   char *dst: 密文
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
#if 0
int Winks_Encode(char *src, int src_len, char *dst)
{
    /*base64算法*/
    int i = 0, j = 0;
	int r = src_len - src_len % 3;
    char base64_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    
    for (; i < r; i += 3)
    {
        dst[j++] = base64_map[(src[i] >> 2) & 0x3F];
        dst[j++] = base64_map[((src[i] << 4) & 0x30) + ((src[i + 1] >> 4) & 0xF)];
        dst[j++] = base64_map[((src[i + 1] << 2) & 0x3C) + ((src[i + 2] >> 6) & 0x3)];
        dst[j++] = base64_map[src[i + 2] & 0x3F];
    }
    
    if (src_len % 3 == 1) 
    {
        dst[j++] = base64_map[(src[i] >> 2) & 0x3F];
        dst[j++] = base64_map[(src[i] << 4) & 0x30];
        dst[j++] = 0;
        dst[j++] = 0;
    }
    else if (src_len % 3 == 2) 
    {
        dst[j++] = base64_map[(src[i] >> 2) & 0x3F];
        dst[j++] = base64_map[((src[i] << 4) & 0x30) + ((src[i + 1] >> 4) & 0xF)];
        dst[j++] = base64_map[(src[i + 1] << 2) & 0x3C];
        dst[j++] = 0;
    }
    
    dst[j] = '\0';
    return WINKS_RETURN_SUCCESS;
}
#else
#include "wk_crypto.h"

void DigitStr2PackedBcd(const char* /*in*/ src, size_t src_len, unsigned char* /*out*/ bcd)
{
	size_t i;
	int odds = src_len & 1;
	size_t bcd_len = src_len / 2;

	if (odds == 0)
		bcd[0] = (src[0] - 0x30);
	for (i = 1 - odds; i < bcd_len; i++)
		bcd[i] = ((src[i * 2 - 1 + odds] - 0x30) << 4) | (src[i * 2 + odds] - 0x30);
	bcd[bcd_len] = ((src[src_len - 1] - 0x30) << 4) | 0x0c;
}

int Winks_Encode(char *src, unsigned int src_len, char *dst, unsigned int dst_len)
{
    int iRet = 0;
    Winks_DateTime_s    v_dateTime;
    unsigned long       Time;
    int i, off = 0;
    int random;
    unsigned char * pRandSrc = NULL;

    Winks_assert(src);
    Winks_assert(src_len>0);
    Winks_assert(dst);
	Winks_assert(dst_len>src_len);

/*
token = CBC AES256 encrypted of the original-token
original-token = current-time random-seeds imsi
*/
    pRandSrc = Winks_salAlloc(32);
    if(pRandSrc == NULL)
        return WINKS_RETURN_FAILURE;

    Winks_mem_set(pRandSrc, 0, 32);
    /* current-time = 64-bits integer indicate the milliseconds(1/1000) since 1970-01-01 00:00:00 UTC */
//    Winks_GetSysDateTime(&v_dateTime);
//    if(Winks_SysTime2CacheTime(&v_dateTime, &Time) != 0)
//        Time = 0;
    Time = Winks_mktime(NULL);
    Winks_mem_cpy(pRandSrc, &Time, 4);
    off += 8;

    /* random-seeds = 128-bits random-seeds */
    srand(Time);
    for(i=0; i<4; i++)
    {
        random = rand();
        Winks_mem_cpy(&pRandSrc[off], &random, 4);
        off += 4;
    }

    /* imsi = IMSI converted to 64-bits integer */
    DigitStr2PackedBcd(src, src_len, &pRandSrc[off]);
    off += 8;

    Winks_assert(off == 32);
    iRet = Winks_EncryptAndBase64(pRandSrc, off, (unsigned char *)dst, dst_len);

    Winks_salFree(pRandSrc);
    
    return iRet;
}
#endif
#if 0
char base64_decode_map(char c)
{
    char oc = 0;
    if((c >= 'A') && (c <= 'Z'))
        oc = (c-'A');
    else if((c >= 'a') && (c <= 'z'))
        oc = (c-'a'+ 26);
    else if((c >= '0') && (c <= '9'))
        oc = (c-'0'+ 52);
    else if(c == '-')
        oc = 62;
    else if(c == '_')
        oc = 63;
    else
        oc = 0;
    return oc;
}

/*解密算法
 * function: Winks_Decode
 * parameters:
 *   char *src: 密文
 *   int src_len: 密文长度
 *   char *dst: 明文
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_Decode(char *src, int src_len, char *dst)
{
    int i = 0, j = 0;

    for (; i < src_len; i += 4)
    {
        dst[j++] = base64_decode_map(src[i]) << 2 | base64_decode_map(src[i + 1]) >> 4;
        dst[j++] = base64_decode_map(src[i + 1]) << 4 | base64_decode_map(src[i + 2]) >> 2;
        dst[j++] = base64_decode_map(src[i + 2]) << 6 | base64_decode_map(src[i + 3]);
    }
    
    dst[j] = '\0';
    return 0;
}
#endif

#ifdef WINKS_DEBUG

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

static const char LocalNumber[][12] = {
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

int winks_TstEngAddFriend()
{
    int iRet = 0;
    int pos = 0;
    char    FriendNo[WINKS_STRG_PHONE_LEN];
    char    FriendNick[WINKS_STRG_NICK_LEN];
  #ifdef WINKS_DEBUG
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    pos = Winks_engineContext_p->DbgCount%LOCALNO_NUM;
    Winks_engineContext_p->DbgCount = (Winks_engineContext_p->DbgCount+1)%LOCALNO_NUM;
  #endif
    strncpy(FriendNo, LocalNumber[pos], sizeof(FriendNo));
    sprintf(FriendNick, "%d", pos);
    iRet = Winks_StrgFriendAdd(FriendNo, (unsigned char *)FriendNick);
    Winks_printf("winks_TstEngAddFriend %s %s ret %d\r\n", FriendNo, FriendNick, iRet);
    return iRet;
}

int winks_TstEngDelFriend()
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

int winks_TstEngDelAllFriends()
{
    int iRet = Winks_StrgFriendDelAll();
    Winks_printf("winks_TstEngDelAllFriends ret %d\r\n", iRet);
    return iRet;
}

void winks_TstEngPrintFriends()
{
    int cnt = 0;
    int Idx = 0;
    char    FriendNo[WINKS_STRG_PHONE_LEN];
    unsigned char FriendNick[WINKS_STRG_NICK_LEN];
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
        Winks_printf("%4d: %-16s %s\r\n", cnt+1, FriendNo, (char *)FriendNick);
        cnt++;
    }
}

void winks_TstEngPrintUsers(unsigned long Flag)
{
    int cnt = 0;
    unsigned short Idx = 1;
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

int winks_TstEngAddUL()
{
    int iRet = 0;
    int pos = 0;
  #ifdef WINKS_DEBUG
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    pos = Winks_engineContext_p->DbgCount%LOCALNO_NUM;
    Winks_engineContext_p->DbgCount = (Winks_engineContext_p->DbgCount+1)%LOCALNO_NUM;
  #endif
    iRet = Winks_StrgULAdd((char *)LocalNumber[pos]);
    Winks_printf("winks_TstEngAddUL %s ret %d\r\n", LocalNumber[pos], iRet);
    pos = (pos+1)%LOCALNO_NUM;
    return iRet;
}

int winks_TstEngDelUL()
{
    int iRet = 0;
    int Idx = 0;
    char    PhoneNo[16];
    int Total = Winks_StrgULCnt();

    Idx = Winks_StrgULGet(Idx, PhoneNo);

    if(Idx>0)
    {
        iRet = Winks_StrgULDelete(PhoneNo);
        Winks_printf("winks_TstEngDelUL %s (%d) ret %d\r\n", PhoneNo, Total, iRet);
        return iRet;
    }
    else
        return WINKS_RETURN_FAILURE;
}

int winks_TstEngDelAllUL()
{
    int iRet = Winks_StrgULDeleteAll();
    Winks_printf("winks_TstEngDelAllFriends ret %d\r\n", iRet);
    return iRet;
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


typedef struct tag_WinksEnappTstProt_s
{
    int     ProtocolNo;
    void  * inData;
    int     inLen;
    ServicesType_s ServiceMode;
    int     wt;
    char    url[256];
    char    wid[16];
    int     fileType;
    int     fileNumber;
    WinksProt_winksPO_s winksPO;
    WinksProt_messageList_s messageList;
    WinksProt_confimMessage_s confimMessage;
    struct Prot_mi_s mi[16];
    char    mid[16][16];
}WinksEnappTstProt_s;

static int winks_TstProtStop(WINKS_ENAPP_HND appHnd, WinksEnappTstProt_s * pTstProt)
{
    if(pTstProt)
    {
    //    Winks_salFree(pTstProt);
    }
    return  Winks_EngAppStop(appHnd);
}

static int winks_TstProtProc(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData)
{
    int iRet = 0;
    WinksEnappTstProt_s * pTstProt = (WinksEnappTstProt_s *)addData;
    Winks_assert(pTstProt);
    
    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        if(Winks_EngProtReq(pTstProt->ProtocolNo, pTstProt->inData, pTstProt->inLen) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(WINKS_ENGINE_APP_PRI_USER,WINKS_ENGPROTYPE_NUMBER, WINKS_MSG_ENG2UI_PROT_PACK, NULL, 0);
            return winks_TstProtStop(appHnd, pTstProt);
        }
        break;
    case WINKS_MSG_ENGINE_APP_STOP  :
        if(pTstProt)
        {
            Winks_salFree(pTstProt);
        }
        break;
    case WINKS_MSG_ENGINE_APP_PAUSE :
        
        break;
    case WINKS_MSG_ENGINE_APP_RESUME:
        
        break;
    case WINKS_MSG_ENGINE_RETMSG    :
    {
        WinksEng2AppMsg_s * pAppMsg = (WinksEng2AppMsg_s *)msgData;
        int Finished = 0;
        
        if(pAppMsg)
        {
            switch(pAppMsg->AppMsg)
            {
            case WINKS_PROT_RET_TYPE_userServiceStatus:
            {
                WinksProt_userServiceStatus_s * puserServiceStatus = 
                    (WinksProt_userServiceStatus_s *)pAppMsg->pData;

                if( puserServiceStatus &&
                    puserServiceStatus->IsRegistered && 
                    puserServiceStatus->RegStat.reg.uid )
                    Winks_SetSettingString(WINKS_SETTING_UID, 
                            puserServiceStatus->RegStat.reg.uid);
            }
                Finished = 1;
                break;
            case WINKS_PROT_RET_TYPE_serviceConfig:
            {
                 WinksProt_serviceConfig_s *pserviceConfig = 
                    (WinksProt_serviceConfig_s *)pAppMsg->pData;
                if(pserviceConfig == NULL)
                {
                    msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;
                }
                else
                {
                    if(pserviceConfig->config)
                    {
                        Winks_mem_set(&pTstProt->ServiceMode, 0, sizeof(ServicesType_s));
                        if(pserviceConfig->config->subscription.subscriber)
                        {
                            pTstProt->ServiceMode.subscription = WINKS_SUBSCRIPTION_SUBSCRIER;
                            pTstProt->ServiceMode.diy = pserviceConfig->config->subscriber_diy;
                            pTstProt->ServiceMode.vcard = pserviceConfig->config->subscriber_vcard;
                            pTstProt->ServiceMode.activity = pserviceConfig->config->subscriber_activity;
                        }
                        else if(pserviceConfig->config->subscription.adware)
                        {
                            pTstProt->ServiceMode.subscription = WINKS_SUBSCRIPTION_ADWARE;
                            pTstProt->ServiceMode.diy = pserviceConfig->config->adware_diy;
                            pTstProt->ServiceMode.vcard = pserviceConfig->config->adware_vcard;
                            pTstProt->ServiceMode.activity = pserviceConfig->config->adware_activity;
                        }
                        else if(pserviceConfig->config->subscription.trial)
                        {
                            pTstProt->ServiceMode.subscription = WINKS_SUBSCRIPTION_TRIAL;
                            pTstProt->ServiceMode.diy = pserviceConfig->config->trial_diy;
                            pTstProt->ServiceMode.vcard = pserviceConfig->config->trial_vcard;
                            pTstProt->ServiceMode.activity = pserviceConfig->config->trial_activity;
                        }
                        else if(pserviceConfig->config->subscription.limited)
                        {
                            pTstProt->ServiceMode.subscription = WINKS_SUBSCRIPTION_LIMITED;
                            pTstProt->ServiceMode.diy = pserviceConfig->config->limited_diy;
                            pTstProt->ServiceMode.vcard = pserviceConfig->config->limited_vcard;
                            pTstProt->ServiceMode.activity = pserviceConfig->config->limited_activity;
                        }
                        else
                            pTstProt->ServiceMode.subscription = WINKS_SUBSCRIPTION_SUBSCRIER;/*WINKS_SUBSCRIPTION_NONE;*/

                        if(pserviceConfig->config->vcash)
                            pTstProt->ServiceMode.vcash = 0;//TODO: 用户点卡计数

                        if(pTstProt->ServiceMode.subscription == WINKS_SUBSCRIPTION_NONE)
                            msg = WINKS_MSG_ENG2UI_REG_FAIL;
                        else if(Winks_SetSettingBlock(WINKS_SETTING_BLOCK_SERVER_CONFIG,
                                    pserviceConfig->config, sizeof(struct Prot_config_s)) != WINKS_RETURN_SUCCESS)
                            msg = WINKS_MSG_ENG2UI_REG_FAIL;
                    }
                }
            }
                Finished = 1;
			    break;
            case WINKS_PROT_RET_TYPE_winksShow:
            {
                WinksProt_winksShow_s * pwinksShow = 
                    (WinksProt_winksShow_s *)pAppMsg->pData;

                if( pwinksShow &&
                    pwinksShow->media && 
                    pwinksShow->media->url )
                    strcpy(pTstProt->url, pwinksShow->media->url);
            }
                Finished = 1;
                break;
            case WINKS_PROT_RET_TYPE_UserWinksType:
            {
                WinksProt_UserWinksType_s * pUserWinksType = 
                    (WinksProt_UserWinksType_s *)pAppMsg->pData;

                if( pUserWinksType &&
                    pUserWinksType->wid )
                    strcpy(pTstProt->wid, pUserWinksType->wid);
            }
                Finished = 1;
                break;
          #if 0
            case WINKS_PROT_RET_TYPE_winksScript:
            {
                WinksProt_winksScript_s * pwinksScript = 
                    (WinksProt_winksScript_s *)pAppMsg->pData;

                if( pwinksScript &&
                    pwinksScript->wid )
                    strcpy(pTstProt->wid, pwinksScript->wid);
            }
                Finished = 1;
                break;
          #endif
            case WINKS_PROT_RET_TYPE_softwarePackage:
            {
                WinksProt_softwarePackage_s * psoftwarePackage = 
                    (WinksProt_softwarePackage_s *)pAppMsg->pData;

                if( psoftwarePackage &&
                    psoftwarePackage->file )
                    strcpy(pTstProt->url, psoftwarePackage->file);
            }
                Finished = 1;
                break;
            case WINKS_PROT_RET_TYPE_messageList:
            {
                WinksProt_messageList_s * pmessageList = 
                    (WinksProt_messageList_s *)pAppMsg->pData;

                if( pmessageList &&
                    pmessageList->mi_num )
                {
                    int cnt = 0;
                    struct Prot_mi_s * pmi = pmessageList->mi;
                    Winks_mem_set(pTstProt->mi, 0, 16*sizeof(struct Prot_mi_s));
                    while(pmi && (cnt<16))
                    {
                        strcpy(pTstProt->mid[cnt], pmi->mid);
                        pTstProt->mi[cnt].mid = pTstProt->mid[cnt];
                        if(pmi->pnext)
                            pTstProt->mi[cnt].pnext = &pTstProt->mi[cnt+1];
                        pmi = pmi->pnext;
                        cnt++;
                    }

                    pTstProt->messageList.mi = pTstProt->mi;
                    pTstProt->messageList.mi_num = cnt;
                }
            }
                Finished = 1;
                break;
            case WINKS_PROT_RET_TYPE_file:
            {
            Winks_fileinfo_s * pfileinfo = 
                (Winks_fileinfo_s *)pAppMsg->pData;
                Finished = 1;
            }
                break;
            case WINKS_PROT_RET_TYPE_result:
            {
                if(pTstProt->ProtocolNo == WINKS_PROTOCOL_UPLOADFILE)
                    Winks_fdelete(WINKS_DB_FILE_IN);
            }
                break;
            default:
                break;
            }
        }
        iRet = winks_TstProtStop(appHnd, pTstProt);
    }
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    return iRet;
}

static int winks_TstProtApp(WinksEnappTstProt_s * pTstProt)
{
    char    Buf[16];
    WINKS_ENAPP_HND appHnd;

    sprintf(Buf, "TstProt_%d", pTstProt->ProtocolNo);
    appHnd = Winks_EngAppStart(WINKS_ENGINE_APP_PRI_USER, winks_TstProtProc, Buf, (void *)pTstProt);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_TstProtMemAlloc()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->TstProtMem == NULL)
    {
        Winks_engineContext_p->TstProtMem = Winks_salAlloc(sizeof(WinksEnappTstProt_s));
        if(Winks_engineContext_p->TstProtMem == NULL)
            return WINKS_RETURN_FAILURE;
        Winks_mem_set(Winks_engineContext_p->TstProtMem, 0, sizeof(WinksEnappTstProt_s));
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_TstProtMemFree()
{
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(Winks_engineContext_p->TstProtMem)
    {
        Winks_salFree(Winks_engineContext_p->TstProtMem);
        Winks_engineContext_p->TstProtMem = NULL;
    }
    
    return WINKS_RETURN_SUCCESS;
}

int winks_TstEngProtReq(int ProtocolNo)
{
    WinksEnappTstProt_s * pTstProt = NULL;
    
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();

    if(winks_TstProtMemAlloc() != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    pTstProt = (WinksEnappTstProt_s *)Winks_engineContext_p->TstProtMem;
    pTstProt->ProtocolNo = ProtocolNo;
    pTstProt->inData = NULL;
    pTstProt->inLen = 0;

	switch(ProtocolNo)
	{
    case WINKS_PROTOCOL_GETSERVICECONFIG:
        winks_TstProtApp(pTstProt);
        break;
  #if 0
    case WINKS_PROTOCOL_ACTIVATE:
        winks_TstProtApp(pTstProt);
        break;
  #endif
    case WINKS_PROTOCOL_AUTH:
        winks_TstProtApp(pTstProt);
        break;
    case WINKS_PROTOCOL_SETSERVICEMODE:
        {
        pTstProt->inData = (void *)&pTstProt->ServiceMode;
        pTstProt->inLen = sizeof(ServicesType_s);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_GETSERVICEMODE:
        winks_TstProtApp(pTstProt);
        break;
    case WINKS_PROTOCOL_GETUSERCONFIG:
        winks_TstProtApp(pTstProt);
        break;
    case WINKS_PROTOCOL_SETUSERCONFIG:
        {
        WinksProt_userConfig_s userConfig;

        Winks_mem_set(&userConfig, 0, sizeof(WinksProt_userConfig_s));
        userConfig.showWinks = 1;
        userConfig.showWinks_Mask = 1;
        
        pTstProt->inData = (void *)&userConfig;
        pTstProt->inLen = sizeof(WinksProt_userConfig_s);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_MERGEPHONEBOOK:
        {
        int i;
        WinksProt_phoneBook_s phoneBook;
        struct Prot_pbi_s pbi[3];
        char   nameArr[3][8];
        char   phoneArr[3][16];

        phoneBook.pbi_num = 3;

        for(i=0;i<phoneBook.pbi_num;i++)
        {
            sprintf(nameArr[i], "name%d", i);
            sprintf(phoneArr[i], "1316666000%d", i);
            pbi[i].n = (char *)nameArr[i];
            pbi[i].p = phoneArr[i];
            if(i+1 < phoneBook.pbi_num)
                pbi[i].pnext = &pbi[i+1];
            else
                pbi[i].pnext = NULL;
        }
        phoneBook.pbi = &pbi[0];

        pTstProt->inData = (void *)&phoneBook;
        pTstProt->inLen = sizeof(phoneBook)+3*24;
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_GETPHONEBOOK:
        winks_TstProtApp(pTstProt);
        break;
    case WINKS_PROTOCOL_UPLOADWINKS:
        {
        WinksProt_UploadWinks_s UploadWinks;

        UploadWinks.name = "testdiy00001.jpg";
        UploadWinks.ext = ".jpg";
        UploadWinks.attachName = (unsigned char *)L"c:\\Images\\red.jpg";
        UploadWinks.FileSize = 0;
        UploadWinks.attachNameLen = (WK_STRLEN((char *)L"c:\\Images\\red.jpg")+1)<<1;
        UploadWinks.pContenttype = Winks_Ext2ContType(UploadWinks.ext);

        pTstProt->inData = (void *)&UploadWinks;
        pTstProt->inLen = sizeof(WinksProt_UploadWinks_s) + strlen(UploadWinks.name) + UploadWinks.attachNameLen;
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_GETWINKSSCRIPT:
        {
        char * phoneNumber = "13521180449";
        pTstProt->inData = (void *)phoneNumber;
        pTstProt->inLen = strlen(phoneNumber);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_SETSHARED:
        {
        WinksProt_shared_s shared;

        Winks_mem_set(&shared, 0, sizeof(WinksProt_shared_s));
        shared.status = WINKS_status_metting;
        shared.statusText = NULL;
        pTstProt->inData = (void *)&shared;
        pTstProt->inLen = sizeof(WinksProt_shared_s);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_GETSHARED:
        winks_TstProtApp(pTstProt);
        break;
    case WINKS_PROTOCOL_CREATEWINKSSCRIPT:
        {
        WinksProt_winksScript_s winksScript;
        struct Prot_pno_s pno[3];
        char   phoneArr[3][16];

        if(pTstProt->wid[0])
            winksScript.wid = pTstProt->wid;
        else
            winksScript.wid = "936-1";
        winksScript.greeting = "greeting";
        winksScript.IsForAll = 1;
        if(winksScript.IsForAll)
        {
            winksScript.pno = NULL;
            pTstProt->inLen = sizeof(winksScript);
        }
        else
        {
            int i;
            
            for(i=0;i<3;i++)
            {
                sprintf(phoneArr[i], "1316666000%d", i);
                pno[i].p = phoneArr[i];
                if(i+1 < 3)
                    pno[i].pnext = &pno[i+1];
                else
                    pno[i].pnext = NULL;
            }
            winksScript.pno = &pno[0];
            pTstProt->inLen = sizeof(winksScript)+3*16;
        }

        pTstProt->inData = (void *)&winksScript;
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_RECOMMENDSERVICE:
        {
        int i;
        WinksProt_phoneBook_s phoneBook;
        struct Prot_pbi_s pbi[3];
        char   nameArr[3][8];
        char   phoneArr[3][16];

        phoneBook.pbi_num = 3;

        for(i=0;i<phoneBook.pbi_num;i++)
        {
            if(i==0)
            {
                strcpy(nameArr[i], "renzhan");
                strcpy(phoneArr[i], "13681087247");
            }
            else if(i==1)
            {
                strcpy(nameArr[i], "hxm");
                strcpy(phoneArr[i], "13426235603");
            }
            else if(i==2)
            {
                Winks_mem_set(nameArr[i], 0, 8);
                strcpy(phoneArr[i], "15010564051");
            }
            else
            {
                sprintf(nameArr[i], "name%d", i);
                sprintf(phoneArr[i], "1318765432%d", i);
            }
            pbi[i].n = (char *)nameArr[i];
            pbi[i].p = phoneArr[i];
            if(i+1 < phoneBook.pbi_num)
                pbi[i].pnext = &pbi[i+1];
            else
                pbi[i].pnext = NULL;
        }
        phoneBook.pbi = &pbi[0];

        pTstProt->inData = (void *)&phoneBook;
        pTstProt->inLen = sizeof(phoneBook)+3*24;
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_PURCHASEWINKS:
        {
        pTstProt->winksPO.count = 1;
        pTstProt->winksPO.price = 1;
        pTstProt->winksPO.amount = 2;
        pTstProt->winksPO.duration = 1;
        if(pTstProt->wid[0])
            pTstProt->winksPO.wid = pTstProt->wid;
        else
            pTstProt->winksPO.wid = "2";
        pTstProt->inData = (void *)&pTstProt->winksPO;
        pTstProt->inLen = sizeof(pTstProt->winksPO);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_GETUSERWINKS:
        {
        char * wid = NULL;

        if(pTstProt->wid[0])
            wid = pTstProt->wid;
        else
            wid = "2";
        pTstProt->inData = (void *)wid;
        pTstProt->inLen = strlen(wid);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_SHOWWINKS:
        {
        char * phoneNumber = "13166668888";
        pTstProt->inData = (void *)phoneNumber;
        pTstProt->inLen = strlen(phoneNumber);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_SHOWSPECIALWINKS:
        {
        pTstProt->inData = (void *)&pTstProt->wt;
        pTstProt->wt = (pTstProt->wt+1)%2;
        pTstProt->inLen = sizeof(pTstProt->wt);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_GETURLFILE:
        //break;
        {
        char  * url = NULL;

        if(pTstProt->url[0])
            url = pTstProt->url;
        else
            url = "/winks/image/img1234.jpg";
        pTstProt->inData = (void *)url;
        pTstProt->inLen = strlen(url);
        winks_TstProtApp(pTstProt);
        }
        break;
    case WINKS_PROTOCOL_UPLOADFILE:
        {
        WinksProt_UploadFile_s UploadFile;
        char    Path[256];
        
        Winks_mem_set(&UploadFile, 0, sizeof(WinksProt_UploadFile_s));
        UploadFile.fileType = pTstProt->fileType;
        UploadFile.fileNumber = pTstProt->fileNumber;

        if(UploadFile.fileType == WINKS_UPLD_FILETYPE_stat)
        {
            Winks_Ascii2UnicodeString(WINKS_STAT_FILE, Path, sizeof(Path));
            UploadFile.attachName = (unsigned char *)Path;
            UploadFile.pContenttype = HTTP_CONTENTTYPE_bin;
        }
        else
        {
            Winks_fdelete(WINKS_DB_FILE_IN);
            if(Winks_frename(WINKS_DB_FILE, WINKS_DB_FILE_IN) != 0)
                return -1;
            Winks_Ascii2UnicodeString(WINKS_DB_FILE_IN, Path, sizeof(Path));
            UploadFile.attachName = (unsigned char *)Path;
            UploadFile.pContenttype = HTTP_CONTENTTYPE_bin;
        }
        UploadFile.attachNameLen = (WK_STRLEN(Path)+1)<<1;
        
        pTstProt->fileType = (pTstProt->fileType+1)%2;
        pTstProt->fileNumber = (pTstProt->fileNumber+1)%2;
        
        pTstProt->inData = (void *)&UploadFile;
        pTstProt->inLen = sizeof(WinksProt_UploadFile_s);
        winks_TstProtApp(pTstProt);
        }
        break;
  #if 0
    case WINKS_PROTOCOL_GETMESSAGE:
        winks_TstProtApp(pTstProt);
        break;
  #endif
    case WINKS_PROTOCOL_CONFIRMMESSAGE:
    {
        if(pTstProt->confimMessage.mid != WINKS_MESSAGEIDTYPE_SCFG)
            pTstProt->confimMessage.mid = WINKS_MESSAGEIDTYPE_SCFG;
        else
            pTstProt->confimMessage.mid = WINKS_MESSAGEIDTYPE_CCFG;
        pTstProt->inData = (void *)&pTstProt->confimMessage;
        pTstProt->inLen = sizeof(pTstProt->confimMessage);
        winks_TstProtApp(pTstProt);
    }
        break;
    case WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE:
        winks_TstProtApp(pTstProt);
        break;
    default:
        return -1;
    }	

    return 0;
}

int winks_TstEngPrintAppStack()
{
    int j = 0;
    
    Winks_EngineAppControl_s *acp = NULL;
    WINKS_ENGINE_CONTEXT_DEF();
    WINKS_ENGINE_CONTEXT_INIT();
    
    Winks_printf("winks_TstEngPrintAppStack\r\n");
    for(j = 0; j < WINKS_ENGINE_APP_QUEUE_SIZE; ++j)
    {
        acp = &(Winks_engineContext_p->engAppStack[j]);
        if(acp->appHnd != 0)
        {
            Winks_assert(acp->prcFnc != NULL);
            Winks_printf("appHnd=%d pri=%d \"%s\"", acp->appHnd, acp->appPri, acp->appName);
        }
    }
    
    return WINKS_RETURN_FAILURE;
}
#endif

static int Winks_DoTestMode(int keyWord)
{
    int iRet = 0;
    int iRet1 = 0;
    int iRet2 = 0;
    unsigned long val = 0;
    char buff[256];
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
    case 602: /* Read sim 1 imsi */
    {
        Winks_GetPhonebookName("13426235603", buff, sizeof(buff)-1);
        Winks_printf("[TEST].get user name [%s]. \r\n",buff);
        break;
    }
    case 603: /* Read sim 1 imsi */
    {
        Winks_GetPhonebookName("13400010001", buff, sizeof(buff)-1);
        Winks_printf("[TEST].get user name [%s]. \r\n",buff);
        break;
    }
    case 604: /* Read sim 1 imsi */
    {
        Winks_GetPhonebookName("13400010002", buff, sizeof(buff)-1);
        Winks_printf("[TEST].get user name [%s]. \r\n",buff);
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
    #if 0
        Winks_CurStateRollback(1);
        Winks_XdeleteDir(WINKS_STRG_IMAGE_PATH);
        Winks_XdeleteDir(WINKS_STRG_BCARD_PATH);
        Winks_XdeleteDir(WINKS_STRG_PATH);
        Winks_XdeleteDir(WINKS_SKIN_FILE_PATH);
        Winks_XdeleteDir(WINKS_APP_PATH);
    #endif
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
    #if 0
        iRet1 = Winks_GetMaxCountOfFriendList(&cnt_frd);
        iRet2 = Winks_GetMaxCountOfBusinessColorCall(&cnt_cnp);
    #endif
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
    #if 0
        iRet = Winks_get_setting_value(E_SETTING_CONNECT_POINT, &val);
        iRet1 = Winks_GetDataAccountName(val, buff, sizeof(buff)-2);
        Winks_printf("[TEST] Get data account[%d] info. return[%d][%d]\r\n",val, iRet, iRet2);
        Winks_printHex((unsigned char*)buff, (unsigned short)WK_STRLEN(buff), (unsigned char*)"[Name:]\r\n");
            
        Winks_mem_set(buff, 0, sizeof(buff));
        iRet2 = Winks_GetApnName(val, buff, sizeof(buff)-2);
        Winks_printHex((unsigned char*)buff, (unsigned short)WK_STRLEN(buff), (unsigned char*)"[Name:]\r\n");
    #endif
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
        Winks_mem_set(&timerInfo, 0, sizeof(timerInfo));
        if(Winks_engineContext_p->PeriodPushTimer != WINKS_TIMER_NULL);
        {
            Winks_GetTimerInfo(Winks_engineContext_p->PeriodPushTimer, &timerInfo);
        }
        Winks_printf("[TEST] Connect TIMER infomation. timerId[%d],expirod[%d],state[%d]\r\n",
            Winks_engineContext_p->PeriodPushTimer, timerInfo.timeOutPeriod, timerInfo.state);
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


    case 51:/* trim cc cache */
    {
        WinksCCDownCtxt_s * pCCDownCtxt = NULL;

        Winks_EngCCDownCtxtCreate(WINKS_CCDOWN_TYPE_CCCACHE, NULL, &pCCDownCtxt);

        Winks_printf("[TEST] Winks_EngCCDownCtxtCreate. \r\n");
        if(pCCDownCtxt)
        {
            Winks_EngCCDownCtxtDestory(pCCDownCtxt);
        }
        break;
    }
    case 52:/* free cc cache */
    {
        iRet = winks_TstEngFreeDiskSpace();
        Winks_printf("[TEST] winks_TstEngFreeDiskSpace. return[%d]\r\n", iRet);
        break;
    }
    case 53:/* Load WAP browser. */
    {
        Winks_printf("[TEST] Winks_OpenWapPage. \r\n");
        Winks_OpenWapPage("http://wap.winksi.cn");
        break;
    }
    case 54:/* Winks_UserOpenWapPage. */
    {
        int UrlType = 0;
        char * wid = "2011-9";
      #ifdef WINKS_DEBUG
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        UrlType = Winks_engineContext_p->DbgCount%WINKS_URLTYPE_MAX;
        Winks_engineContext_p->DbgCount = (Winks_engineContext_p->DbgCount+1)%WINKS_URLTYPE_MAX;
      #endif
        Winks_printf("[TEST] Winks_OpenWapPage. \r\n");
        Winks_UserOpenWapPage(UrlType, wid);
        break;
    }
#endif
        
    case 60:
    {
        iRet = Winks_UserRegist();
        Winks_printf("[TEST] Winks_UserRegist. return[%d]\r\n", iRet);
        break;
    }
    case 61: /* Sim卡1发送短信 */
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
    case 62: /* Sim卡2发送短信 */
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
    case 63:
        Winks_UserUploadPhoneBook(1);
        break;
    case 64:
        Winks_UserUploadPhoneBook(0);
        break;
    case 65:
        Winks_UserRegistStop();
        break;
    case 66:
        Winks_UserResetFriendList();
        break;
    case 67:
        Winks_UserResetCacheFriendList();
        break;
    case 70: /* 启动及IMSI读取状态 */
    {
#ifdef WINKS_DUAL_SIM
    Winks_printf("state[%d]\r\n", Winks_engineContext_p->curState);
    Winks_printf("SimStatus:0x%04x Req:%d OtherRetry:%d\r\n", Winks_engineContext_p->SimStatus,
        Winks_engineContext_p->CurInquireSimNo, Winks_engineContext_p->OtherSimRetryCnt);
  #ifdef WINKS_DEBUG
    Winks_printf("IMSI[0]:%s\r\n", Winks_engineContext_p->ImsiCode[0]);
    Winks_printf("IMSI[1]:%s\r\n", Winks_engineContext_p->ImsiCode[1]);
  #endif
#endif
        break;
    }
    case 71:
    {
        iRet = Winks_GetImei(WINKS_CHN_NULL, buff, sizeof(buff));
        Winks_printf("[TEST] Winks_GetImei[%s]. return[%d]\r\n",buff, iRet);
        break;
    }
#ifdef WINKS_DEBUG
    case 101:
      #if 0
        winks_TstEngProtReq(WINKS_PROTOCOL_GETSERVICECONFIG);
      #else
        Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_SERVICECONFIG, NULL);
      #endif
        break;
    case 102:
        winks_TstEngProtReq(WINKS_PROTOCOL_AUTH);
        break;
    case 103:
        winks_TstEngProtReq(WINKS_PROTOCOL_SETSERVICEMODE);
        break;
    case 104:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETSERVICEMODE);
        break;
    case 105:
      #if 0
        winks_TstEngProtReq(WINKS_PROTOCOL_GETUSERCONFIG);
      #else
      {
       int flag = 0; 
        #ifdef WINKS_DEBUG
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        flag = Winks_engineContext_p->DbgCount%2;
        Winks_engineContext_p->DbgCount = (Winks_engineContext_p->DbgCount+1)%2;
        #endif
        if(flag)
            Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_USERCONFIG, NULL);
        else
            Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_UPDUSERCONFIG, NULL);
      }
      #endif
        break;
    case 106:
      #if 0
        winks_TstEngProtReq(WINKS_PROTOCOL_SETUSERCONFIG);
      #else
        {
        WinksProt_userConfig_s userConfig;

        Winks_mem_set(&userConfig, 0, sizeof(WinksProt_userConfig_s));
        userConfig.showWinks = 1;
        userConfig.showWinks_Mask = 1;
        
        Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_SETUSERCONFIG, &userConfig);
        }
      #endif
        break;
    case 107:
        winks_TstEngProtReq(WINKS_PROTOCOL_MERGEPHONEBOOK);
        break;
    case 108:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETPHONEBOOK);
        break;
    case 109:
        winks_TstEngProtReq(WINKS_PROTOCOL_UPLOADWINKS);
        break;
    case 110:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETWINKSSCRIPT);
        break;
    case 111:
        winks_TstEngProtReq(WINKS_PROTOCOL_CREATEWINKSSCRIPT);
        break;
    case 112:
        winks_TstEngProtReq(WINKS_PROTOCOL_SETSHARED);
        break;
    case 113:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETSHARED);
        break;
    case 114:
      #if 0
        winks_TstEngProtReq(WINKS_PROTOCOL_RECOMMENDSERVICE);
      #else
        {
        Winks_RecommendInfo_s RcmdArr[3];
        
        Winks_mem_set(RcmdArr, 0, 3*sizeof(Winks_RecommendInfo_s));
        Winks_UnicodeStrncpy((char *)RcmdArr[1].name, L"renzhan", sizeof(RcmdArr[1].name));
        strcpy(RcmdArr[1].pno, "13681087247");
        Winks_UnicodeStrncpy((char *)RcmdArr[0].name, L"hxm", sizeof(RcmdArr[0].name));
        strcpy(RcmdArr[0].pno, "13426235603");
        strcpy(RcmdArr[2].pno, "15010564051");

        Winks_UserRecommend(3, RcmdArr, "da jia hao");
        }
      #endif
        break;
    case 115:
        winks_TstEngProtReq(WINKS_PROTOCOL_PURCHASEWINKS);
        break;
    case 116:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETUSERWINKS);
        break;
    case 117:
        winks_TstEngProtReq(WINKS_PROTOCOL_SHOWWINKS);
        break;
    case 118:
        winks_TstEngProtReq(WINKS_PROTOCOL_SHOWSPECIALWINKS);
        break;
    case 119:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETURLFILE);
        break;
    case 120:
      #if 0
        winks_TstEngProtReq(WINKS_PROTOCOL_UPLOADFILE);
      #else
        {
        unsigned long filenumber = 0;
      #ifdef WINKS_DEBUG
        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        filenumber = Winks_engineContext_p->DbgCount%2;
        Winks_engineContext_p->DbgCount = (Winks_engineContext_p->DbgCount+1)%2;
      #endif
        if(filenumber == 0)
        {
            Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_UPLOAD_STAT, &filenumber);
            filenumber = 1;
        }
        else
        {
            Winks_SynsetStart(WINKS_ENGINE_APP_PRI_NORMAL, SYNSET_TYPE_UPLOAD_LOG, &filenumber);
            filenumber = 0;
        }
        }
      #endif
        break;
  #if 0
    case 121:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETMESSAGE);
        break;
  #endif
    case 122:
        winks_TstEngProtReq(WINKS_PROTOCOL_CONFIRMMESSAGE);
        break;
    case 123:
        winks_TstEngProtReq(WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE);
        break;
    case 140:/* 释放协议调试内存 */
    {
        WinksEnappTstProt_s * pTstProt = NULL;
        char    Buf[16];
        WINKS_ENAPP_HND appHnd;

        WINKS_ENGINE_CONTEXT_DEF();
        WINKS_ENGINE_CONTEXT_INIT();

        if(Winks_engineContext_p->TstProtMem == NULL)
            return WINKS_RETURN_FAILURE;

        pTstProt = (WinksEnappTstProt_s *)Winks_engineContext_p->TstProtMem;
        sprintf(Buf, "TstProt_%d", pTstProt->ProtocolNo);
        appHnd = winks_FindEngAppHndByName(Buf);
        if(appHnd != WINKS_ENGINE_APP_HANDLER_NULL)
        {
            winks_TstProtStop(appHnd, pTstProt);
        }
        winks_TstProtMemFree();
    }
        break;
    case 150:
    #if 0
        Winks_SendLogToServer();
    #endif
        break;
    case 151:
    #if 0
        Winks_GetLogFromServer();
    #endif
        break;
#endif
    case 160:/* 版本升级通知 */
        Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_newver;
        winks_StartSrvMsgTimer();
        break;
    case 161:/* 用户配置更新通知 */
        Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_ccfg;
        winks_StartSrvMsgTimer();
        break;
    case 162:/* 服务器配置更新通知 */
        Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_scfg;
        winks_StartSrvMsgTimer();
        break;
    case 163:/* 服务器要求上传统计信息 */
        Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_upldstat;
        winks_StartSrvMsgTimer();
        break;
    case 164:/* 服务器要求上传log */
        Winks_engineContext_p->SrvNotify.Notify |= WINKS_NOTIFY_upldlog;
        winks_StartSrvMsgTimer();
        break;
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
    case 300:/*wap*/
    case 301:/*wap*/
    case 302:/*wap*/
    case 303:/*wap*/
    case 304:/*wap*/
    case 305:/*wap*/
    case 306:/*wap*/
    case 307:/*wap*/
    case 308:/*wap*/
    case 309:/*wap*/
    {
        char urlHead[32]={0};
        char uid[16] = {0};
        iRet = Winks_GetSettingString(WINKS_SETTING_SHOP_SERVER_URL, urlHead, sizeof(urlHead));
        iRet = Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid));
        if(strlen(urlHead) && urlHead[strlen(urlHead)-1] == '/')
            urlHead[strlen(urlHead)-1] = 0;
        switch(keyWord)
        {
        case 300:/*wap 在线体验*/
            sprintf(buff,"%s/link/intro?uid=%s", urlHead, uid);
            break;
        case 301:/*wap 帮助页面*/
            sprintf(buff,"%s/link/help", urlHead);
            break;
        case 302:/*wap 关于页面*/
            sprintf(buff,"%s/link/about", urlHead);
            break;
        case 303:/*wap 彩像商城（已激活用户）*/
            sprintf(buff,"%s/link/store?uid=%s", urlHead, uid);
            break;
        case 304:/*wap 彩像商城（未激活用户）*/
            sprintf(buff,"%s/link/store", urlHead);
            break;
        case 305:/*wap 我的彩像库*/
            sprintf(buff,"%s/link/mywinks?uid=%s", urlHead, uid);
            break;
        case 306:/*wap 定制业务*/
            sprintf(buff,"%s/link/customize?uid=%s", urlHead, uid);
            break;
        case 307:/*wap 彩像设置*/
            sprintf(buff,"%s/link/settings?uid=%s", urlHead, uid);
            break;
        case 308:/*wap DIY定制*/
            sprintf(buff,"%s/link/diycustomize?uid=%s&wid=%s", urlHead, uid, "98-1");
            break;
        case 309:/*wap 购买彩像*/
            sprintf(buff,"%s/link/purchase?uid=%s&wid=%s", urlHead, uid, "2011-9");
            break;
        default:
            break;
        }
        
        Winks_printf("Wap[%d] url[%s]", keyWord, buff);
        Winks_OpenWapPage(buff);
        break;
    }
    
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
        Winks_printf("[TEST] AV[%s].\r\n", WINKS_AV);
        break;
    }
    case 900:/* PrintAppStack.*/
    {
#ifdef WINKS_DEBUG
        winks_TstEngPrintAppStack();
#endif
        break;
    }
    case 990:/* 990 Clean all winks file.*/
    {
        Winks_printf("[TEST] Winks_CurStateRollback_1.\r\n");
    //    Winks_CurStateRollback(1);
        
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
        Winks_printf("error case %d\r\n", keyWord);
        break;
    }
    return WINKS_RETURN_SUCCESS;
}

typedef struct tag_Winks_TestModeTable
{
    char *keyString;
    int key;
}Winks_EngDSItem_s;

static int Winks_TestModeManager(char * keyStr)
{
    int i = 0;
    const Winks_EngDSItem_s keyTable[]= {
         {"000#", 0}    /*   0 Reset.*/
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
        ,{"051#", 51}   /*  54 trim cc cache */
        ,{"052#", 52}   /*  55 free cc cache */
        ,{"053#", 53}   /*  53 Load WAP Browser. */
        ,{"054#", 54}   /*  54 Winks_UserOpenWapPage */
        ,{"060#", 60}   /*  60 user regist. */  
        ,{"061#", 61}   /*  61 Sim卡1发送短信 */  
        ,{"062#", 62}   /*  62 Sim卡2发送短信 */  
        ,{"063#", 63}   /*  63 Winks_UserUploadPhoneBook(1) */
        ,{"064#", 64}   /*  64 Winks_UserUploadPhoneBook(0) */
        ,{"065#", 65}   /*  65 Winks_UserRegistStop() */
        ,{"066#", 66}   /*  66 Winks_UserResetFriendList() */
        ,{"067#", 67}   /*  67 Winks_UserResetCacheFriendList() */
        ,{"070#", 70}   /*  70 启动及IMSI读取状态. */  
        ,{"071#", 71}   /*  71 Get Imei*/
        ,{"101#", 101}  /* 101 测试协议 WINKS_PROTOCOL_GETSERVICECONFIG */  
        //,{"102#", 102}  /* 102 测试协议 WINKS_PROTOCOL_ACTIVATE */  
        ,{"102#", 102}  /* 102 测试协议 WINKS_PROTOCOL_AUTH */  
        ,{"103#", 103}  /* 103 测试协议 WINKS_PROTOCOL_SETSERVICEMODE */  
        ,{"104#", 104}  /* 104 测试协议 WINKS_PROTOCOL_GETSERVICEMODE */
        ,{"105#", 105}  /* 105 测试协议 WINKS_PROTOCOL_GETUSERCONFIG */
        //,{"106#", 106}  /* 106 测试协议 WINKS_PROTOCOL_SETUSERCONFIG */
        ,{"107#", 107}  /* 107 测试协议 WINKS_PROTOCOL_MERGEPHONEBOOK */  
        //,{"108#", 108}  /* 108 测试协议 WINKS_PROTOCOL_GETPHONEBOOK */  
        ,{"109#", 109}  /* 109 测试协议 WINKS_PROTOCOL_UPLOADWINKS */  
        //,{"110#", 110}  /* 110 测试协议 WINKS_PROTOCOL_GETWINKSSCRIPT */  
        ,{"111#", 111}  /* 111 测试协议 WINKS_PROTOCOL_CREATEWINKSSCRIPT */  
        //,{"112#", 112}  /* 112 测试协议 WINKS_PROTOCOL_SETSHARED */
        //,{"113#", 113}  /* 113 测试协议 WINKS_PROTOCOL_GETSHARED */
        ,{"114#", 114}  /* 114 测试协议 WINKS_PROTOCOL_RECOMMENDSERVICE */  
        //,{"115#", 115}  /* 115 测试协议 WINKS_PROTOCOL_PURCHASEWINKS */  
        //,{"116#", 116}  /* 116 测试协议 WINKS_PROTOCOL_GETUSERWINKS */  
        ,{"117#", 117}  /* 117 测试协议 WINKS_PROTOCOL_SHOWWINKS */  
        ,{"118#", 118}  /* 118 测试协议 WINKS_PROTOCOL_SHOWSPECIALWINKS */  
        ,{"119#", 119}  /* 119 测试协议 WINKS_PROTOCOL_GETURLFILE */  
        ,{"120#", 120}  /* 120 测试协议 WINKS_PROTOCOL_UPLOADFILE */  
        //,{"121#", 121}  /* 121 测试协议 WINKS_PROTOCOL_GETMESSAGE */  
        ,{"122#", 122}  /* 122 测试协议 WINKS_PROTOCOL_CONFIRMMESSAGE */  
        ,{"123#", 123}  /* 123 测试协议 WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE */  
        ,{"140#", 140}  /* 140 释放协议调试内存 */
        //,{"150#", 150}  /* 150 发送log到服务器 */
        //,{"151#", 151}  /* 151 从服务器取log */
        ,{"160#", 160}  /* 160 版本升级通知 */
        ,{"161#", 161}  /* 161 用户配置更新通知 */
        ,{"162#", 162}  /* 162 服务器配置更新通知 */
        ,{"163#", 163}  /* 163 服务器要求上传统计信息 */
        ,{"164#", 164}  /* 164 服务器要求上传log */
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
        ,{"300#", 300}  /* 300 WAP.*/
        ,{"301#", 301}  /* 301 WAP.*/
        ,{"302#", 302}  /* 302 WAP.*/
        ,{"303#", 303}  /* 303 WAP.*/
        ,{"304#", 304}  /* 304 WAP.*/
        ,{"305#", 305}  /* 305 WAP.*/
        ,{"306#", 306}  /* 306 WAP.*/
        ,{"307#", 307}  /* 307 WAP.*/
        ,{"308#", 308}  /* 308 WAP.*/
        ,{"309#", 309}  /* 309 WAP.*/
        ,{"600#", 600}  /* 600 Read sim 2 imsi.*/
        ,{"601#", 601}  /* 601 Read sim 1 imsi.*/
        ,{"602#", 602}  /* 602 .*/
        ,{"603#", 603}  /* 603 .*/
        ,{"604#", 604}  /* 604 .*/
        ,{"605#", 605}  /* 605 .*/
        ,{"606#", 606}  /* 606 .*/
        ,{"607#", 607}  /* 607 .*/
#ifdef WINKS_ENGINE_RUN_BY_STEP
        ,{"608#", 608}  /* 608 .*/
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

        ,{"900#", 900}  /* 900 PrintAppStack.*/
        ,{"990#", 990}  /* 990 Clean all winks file.*/
        ,{"997#", 997}  /* 997 Restart test.*/
        ,{"998#", 998}  /* 998 Assert test.*/
        ,{"999#", 999}  /* 999 Winks_fail.*/
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

