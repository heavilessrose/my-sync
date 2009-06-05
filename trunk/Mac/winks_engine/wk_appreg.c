#if 1
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_enfnc.h"
#endif

#define ENAPP_NAME_REG  "regist"

#define WINKS_REGIST_POLL_TIMES     3

typedef enum tag_RegistStep_e
{
    REGIST_STEP_GETSERVICECONFIG = 0,
    REGIST_STEP_GETUSERSTATUS_0,
    REGIST_STEP_ACTIVATE,
    REGIST_STEP_DELAY_QUERY,
    REGIST_STEP_GETUSERSTATUS_1,
    REGIST_STEP_SETSERVICEMODE,
    REGIST_STEP_GETUSERCONFIG_ALL,
    REGIST_STEP_MERGEPHONEBOOK,
    REGIST_STEP_MAX
}RegistStep_e;

typedef struct tag_WinksSmsItem_s
{
    char gateway[WINKS_SMS_GATEWAY_LEN];
    char prefix[WINKS_SMS_PREFIX_LEN];
}WinksSmsItem_s;

typedef struct tag_WinksSms_s
{
    int count;
    WinksSmsItem_s sms[WINKS_SMS_GATEWAY_NUM];
}WinksSms_s;

typedef struct tag_WinksEnappRegist_s
{
    unsigned long controller; /*运行流程控制选项*/
    unsigned int    pause:1,       /*应用终端运行标志，在等待用户选择时使用，如等待用户确认是否上传电话本。*/
                    initCC:1,
                    end:1;
    int     Step;   /* 注册步骤(即运行状态),RegistStep_e */
    int     Pri;    /* 应用优先级 */
    int     PollCount;
    int     registerMethod;
    char    uid[WINKS_SERVER_UID_MAX_LEN];
    WINKS_TIMER_ID hTimer;
    WinksSms_s smss;
    ServicesType_s ServiceMode;
    Winks_EngProType_e appType;
}WinksEnappRegist_s;


typedef struct tag_WinksPrtRet_s
{
    int     msg;
    void  * msg_body;
    int     msg_size;
    int     runctr;/* 协议流程控制器，0:继续运行; 1:中断,等待UI事件; 2:结束运行 */
}WinksPrtRet_s;

int Winks_DoPrtServiceConfig(WinksProt_serviceConfig_s *dp);
int Winks_registorSuccessPrc(char * uid);
static int Winks_RegistProcRun(WINKS_ENAPP_HND appHnd, WinksPrtRet_s *pRetData, WinksEnappRegist_s * pRegist);

static int Winks_RegistPollTimeMs(int cnt)
{
    switch(cnt)
    {
        case 0:
        case 1:
            return 20000L;
        case 2:
            return 40000L;
        default:
            return 3600000L;
    }
}

static int winks_sendRegSms(WinksEnappRegist_s * pRegist)
{
    char    buff[256];
    char    buff1[46];
    char    imsiCode[WINKS_IMSI_LEN + 1];
    int     iRet = 0;
    WinksSmsItem_s *p = NULL;
    int i = 0;
    
    Winks_assert(pRegist);
    if(pRegist->smss.count == 0 || 
        pRegist->smss.count > WINKS_SMS_GATEWAY_NUM || 
        !strlen(pRegist->smss.sms[0].gateway))
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(imsiCode, 0, sizeof(imsiCode));
    iRet = Winks_GetImsi(WINKS_CHN_NULL, imsiCode, sizeof(imsiCode));
    imsiCode[sizeof(imsiCode)-1] = 0;
    if(iRet != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }

    for(i=0;i<MIN(pRegist->smss.count,WINKS_SMS_GATEWAY_NUM);++i)
    {
        p = &pRegist->smss.sms[i];
        if( !strlen(p->gateway))
            continue;

#if 0   // 0--normal; 1--debug
        snprintf(buff, sizeof(buff)-1, "auth token=%s mode=debug", imsiCode);
        //snprintf(buff, sizeof(buff)-1, "auth token=%s", imsiCode);
#else 
        Winks_printf("Call Winks_Encode imsi[%s]!",imsiCode);
        Winks_mem_set(buff1, 0, sizeof(buff1));
        if(Winks_Encode(imsiCode, strlen(imsiCode), buff1, sizeof(buff1)) != 0 )
    		return -1;
        sprintf(buff,"%sauth token=%s",p->prefix,buff1);
#endif


#ifndef WINKS_DUAL_SIM
        iRet = Winks_SendSms(p->gateway, strlen(p->gateway), buff, strlen(buff)+1);
#else /* WINKS_DUAL_SIM */
        iRet = Winks_SendSms(p->gateway, strlen(p->gateway), buff, strlen(buff)+1, Winks_GetActiveSim());
#endif /* WINKS_DUAL_SIM */
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_RegistStop(WINKS_ENAPP_HND appHnd, WinksEnappRegist_s * pRegist)
{
    if(pRegist)
    {
        if(pRegist->hTimer)
        {
            Winks_EngDestroyTimer(appHnd, pRegist->hTimer);
            pRegist->hTimer = WINKS_TIMER_NULL;
        }
        Winks_salFree(pRegist);
    }
    return Winks_EngAppStop(appHnd);
}

static int winks_RegistStepSend(WINKS_ENAPP_HND appHnd, WinksEnappRegist_s * pRegist)
{
    int iRet = WINKS_RETURN_FAILURE;

    Winks_assert(pRegist);
    Winks_printf("winks_RegistStepSend Step=%d\r\n", pRegist->Step);
    pRegist->pause = 0;
    
    switch(pRegist->Step)
    {
    case REGIST_STEP_GETSERVICECONFIG:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETSERVICECONFIG, 
                                 NULL, 0 );
        break;
    case REGIST_STEP_GETUSERSTATUS_0:
    case REGIST_STEP_GETUSERSTATUS_1:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_AUTH, 
                                 NULL, 0 );
        break;
    case REGIST_STEP_SETSERVICEMODE:
    {
        WinksSetServiceMode_s setdata;
        Winks_mem_set(&setdata, 0, sizeof(setdata));
        strcpy(setdata.uid, pRegist->uid);
        setdata.serviceMode.subscription = WINKS_SUBSCRIPTION_SUBSCRIER;
        setdata.serviceMode.diy = 1;
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_SETSERVICEMODE, 
                                 (void *)&setdata, sizeof(setdata) );
        break;
    }
    case REGIST_STEP_MERGEPHONEBOOK:
        {
        Winks_MemPool_s * pMemPool = NULL;
        WinksProt_phoneBook_s * phoneBook = NULL;
		if(Winks_EngMemPoolCreate(&pMemPool) == NULL)
            return WINKS_RETURN_FAILURE;
        if(Winks_EngReadPhoneBook(pMemPool, &phoneBook) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        if(phoneBook == NULL)/* 电话簿为空 */
        {
            WinksPrtRet_s retData;
            
            /*置位 上传电话本标志*/
            Winks_SetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_PB, 1);            
            
            Winks_mem_set(&retData, 0, sizeof(retData));
            if(pRegist->controller == WINKS_ENGAPP_REG_QUERYUS)
                retData.msg = WINKS_MSG_ENG2UI_QUERY_OK;
            else
                retData.msg = WINKS_MSG_ENG2UI_ULPB_OK;
            //retData.msg_body = NULL;
            //retData.msg_size = 0;
            retData.runctr = WINKS_REG_RUNCTR_STOP;
            Winks_EngMemPoolDestory(pMemPool);
            Winks_RegistProcRun(appHnd, &retData, pRegist);
            return 0;
        }
        else
        {
            iRet = Winks_EngProtReq( WINKS_PROTOCOL_MERGEPHONEBOOK, 
                                     phoneBook, sizeof(WinksProt_phoneBook_s) );
            Winks_EngMemPoolDestory(pMemPool);
        }
        }
        break;
    case REGIST_STEP_ACTIVATE:
    {
        pRegist->PollCount = 0;
        iRet = winks_sendRegSms(pRegist);
        Winks_printf("winks_sendRegSms ret %d\r\n", iRet);
//        if(iRet != WINKS_RETURN_SUCCESS)
//            break;
//        else
        {
            /* 继续下一步 */
            pRegist->Step = REGIST_STEP_DELAY_QUERY;
            pRegist->PollCount = 0;
            /*继续执行下一个case的语句。*/
        }
    }
    case REGIST_STEP_DELAY_QUERY:
    {
        int polltime = Winks_RegistPollTimeMs(pRegist->PollCount);
        Winks_printf("Regist sms waiting (%d) %d\r\n", pRegist->PollCount, polltime);
        pRegist->hTimer = Winks_EngCreateTimer(appHnd,polltime, WINKS_TIMER_AUTO_START);
        iRet = WINKS_RETURN_SUCCESS;
        if(pRegist->hTimer == WINKS_TIMER_NULL)
        {
            Winks_assert(pRegist->hTimer > 0);
            iRet = WINKS_RETURN_FAILURE;
        }
        break;
    }
        
    case REGIST_STEP_GETUSERCONFIG_ALL:
    {
        int userConfigFlag = 1;
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETUSERCONFIG, &userConfigFlag, sizeof(userConfigFlag));
        break;
    }
        
    default:
        Winks_fail("winks_RegistStepSend");
        break;
    }

    return iRet;
}

static int Winks_SaveSmssToContext(WinksEnappRegist_s *pRegist, struct Prot_address_s *address)
{
    WinksSmsItem_s* p;
    struct Prot_sms_s * q;
    int i = 0;
    
    Winks_assert(address != NULL);
    Winks_assert(pRegist != NULL);

    pRegist->smss.count = address->sms_num;
    q = address->sms;
    
    for(i=0; i<MIN(address->sms_num, WINKS_SMS_GATEWAY_NUM); ++i)
    {
        if(q == NULL)
        {
            Winks_printf("ERR::Unknow sms infomation.");
            return WINKS_RETURN_FAILURE;
        }

        if(q->number == NULL || !strlen(q->number))
            return WINKS_RETURN_FAILURE;
        
        p = &pRegist->smss.sms[i];
        strncpy(p->gateway, q->number, sizeof(p->gateway)-1);

        if(q->prefix != NULL && strlen(q->prefix))
            strncpy(p->prefix, q->prefix, sizeof(p->prefix)-1);
        q = q->pnext;
    }
    
    return WINKS_RETURN_SUCCESS;
}

/*检查并保存UID*/
static int Winks_cSaveUid(char* uid)
{
    char oUid[WINKS_SERVER_UID_MAX_LEN];
    int iRet = 0;
    Winks_mem_set(oUid, 0, sizeof(oUid));
    if(uid == NULL)
        return WINKS_RETURN_FAILURE;
    Winks_GetSettingString(WINKS_SETTING_UID, oUid, sizeof(oUid));
    if(strcmp(oUid, uid) != 0)
    {
        iRet = Winks_SetSettingString(WINKS_SETTING_UID, uid);
        if(iRet!= WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }
    return WINKS_RETURN_SUCCESS;
}
static int winks_RegistStepRcv( WINKS_ENAPP_HND appHnd,
                                      WinksEnappRegist_s * pRegist, 
                                      WinksEng2AppMsg_s * pAppMsg,
                                      WinksPrtRet_s *ret)
{
    WinksProt_result_s *presult = NULL;
    int  iRet = WINKS_RETURN_FAILURE;
    int* msg;
    void** msg_body;
    int* msg_size;
    int* runctr;
    int enState = 0;
    unsigned long val = 0;

    Winks_assert(ret != NULL);
    Winks_mem_set(ret, 0, sizeof(ret));
    msg = &(ret->msg);
    msg_body = &ret->msg_body;
    msg_size = &(ret->msg_size);
    runctr = &(ret->runctr);/* 协议流程控制器，0:继续运行; 1:中断,等待UI事件; 2:结束运行 */

#if 1
    if( pAppMsg == NULL || pAppMsg->pData == NULL)
    {
        *msg = WINKS_MSG_ENG2UI_APP_ERR;
    }
#else
    if( pAppMsg == NULL)
        *msg = WINKS_MSG_ENG2UI_APP_ERR;
    else if(pAppMsg->pData == NULL)
    {
        switch(pRegist->Step)
        {
            case WINKS_PROT_RET_TYPE_serviceConfig:
                *msg = WINKS_MSG_ENG2UI_SRV_ERRDATA; break;
            case REGIST_STEP_GETUSERSTATUS_0:
            case REGIST_STEP_GETUSERSTATUS_1:
                *msg = WINKS_MSG_ENG2UI_REG_FAIL; break;
            defult:
                *mag = WINKS_MSG_ENG2UI_APP_ERR; break;
        }
    }
#endif
    else if( pAppMsg->AppMsg < WINKS_PROT_RET_TYPE_result )
    {
        *msg = pAppMsg->AppMsg;
    }    
    else if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result )
    {
        presult = (WinksProt_result_s *)pAppMsg->pData;
        Winks_EngResultErrDftHandler(presult, msg);
    }
    
    if(*msg)
        WINKS_RETURN_SUCCESS;

    
    switch(pRegist->Step)
    {
    case REGIST_STEP_GETSERVICECONFIG:
    {
        if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_serviceConfig )
        {
            WinksProt_serviceConfig_s *pserviceConfig = (WinksProt_serviceConfig_s *)pAppMsg->pData;
            
            /*save data*/
            iRet = Winks_DoPrtServiceConfig(pserviceConfig);
            if(iRet == WINKS_RETURN_FAILURE)
            {
                *msg = WINKS_MSG_ENG2UI_APP_ERR;
            }
            else
            {
                if(pserviceConfig->address && pserviceConfig->address->sms)
                    Winks_SaveSmssToContext(pRegist, pserviceConfig->address);
                /* 继续下一步 */
                pRegist->Step = REGIST_STEP_GETUSERSTATUS_0;
            }
        }
        break;
    }

    case REGIST_STEP_GETUSERSTATUS_0:
    case REGIST_STEP_GETUSERSTATUS_1:
    {
        if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_userServiceStatus )
        {
            WinksProt_userServiceStatus_s *puserServiceStatus = (WinksProt_userServiceStatus_s *)pAppMsg->pData;
            
            enState = Winks_GetEngineCurrentState();

            //服务器返回:用户已激活
            if((puserServiceStatus->IsRegistered)
                && (puserServiceStatus->RegStat.reg.uid != NULL)
                && (puserServiceStatus->RegStat.reg.service.subscription > WINKS_SUBSCRIPTION_NONE)
                && (puserServiceStatus->RegStat.reg.service.subscription < WINKS_SUBSCRIPTION_MAX))
            {
                if(!Winks_IsEngStateRegister(enState))
                {
                    *runctr = 0;
                    //终端与服务器激活状态不一致，终端需要置为激活状态并作初始化工作。
                    iRet = Winks_registorSuccessPrc(puserServiceStatus->RegStat.reg.uid); 
                    if(iRet != WINKS_RETURN_SUCCESS)
                    {
                        *msg = WINKS_MSG_ENG2UI_REG_FAIL;
                        break;
                    }
                    else if(pRegist->controller == WINKS_ENGAPP_REG_QUERYUS)
                    {
                        /*终端与服务器用户状态不一致，需要重新初始化运营商彩像。*/
                        pRegist->initCC = 1;
                    }
                    /*未break， 继续往下执行代码*/
                }
                else if(Winks_cSaveUid(puserServiceStatus->RegStat.reg.uid) != WINKS_RETURN_SUCCESS)
                {
                    *msg = WINKS_MSG_ENG2UI_REG_FAIL;
                    break;
                }
                
                if(Winks_GetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_SETTING, &val)==0 && val==0)
                {
                    *msg = WINKS_MSG_ENG2UI_SETUP;
                    pRegist->Step = REGIST_STEP_GETUSERCONFIG_ALL;
                }
                else if(Winks_GetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_PB, &val)==0 && val==0)
                {
                    *msg = WINKS_MSG_ENG2UI_ULPB;
                    pRegist->Step = REGIST_STEP_MERGEPHONEBOOK;
                }
                else
                {
                    if(pRegist->controller == WINKS_ENGAPP_REG_QUERYUS)
                        *msg = WINKS_MSG_ENG2UI_QUERY_OK;
                    else
                        *msg = WINKS_MSG_ENG2UI_INIT_OK;
                    *runctr = WINKS_REG_RUNCTR_STOP; /*运行结束。退出流程标志。*/
                }
            }
            else//服务器返回:用户未激活
            {
                if(pRegist->controller == WINKS_ENGAPP_REG_QUERYUS)
                {
                    Winks_SetEngStateUnRegister(enState);
                    Winks_SetEngineCurrentState(enState);
                    *msg = WINKS_MSG_ENG2UI_QUERY_OK;
                    *runctr = WINKS_REG_RUNCTR_STOP; /*运行结束。退出流程标志。*/
                }
                else if(puserServiceStatus->IsRegistered)
                {
                    if((puserServiceStatus->RegStat.reg.uid != NULL)
                        && (puserServiceStatus->RegStat.reg.service.subscription == WINKS_SUBSCRIPTION_NONE))
                    {
                        strcpy(pRegist->uid, puserServiceStatus->RegStat.reg.uid);
                        *msg = WINKS_MSG_ENG2UI_REG;
                        pRegist->Step = REGIST_STEP_SETSERVICEMODE;
                    }
                    else
                        *msg = WINKS_MSG_ENG2UI_REG_FAIL;
                }
                else//服务器返回:用户未激活，未下发 UID
                {
                    if(pRegist->Step == REGIST_STEP_GETUSERSTATUS_0)
                    {
                        pRegist->registerMethod = puserServiceStatus->RegStat.unregistered.registerMethod;
                        if(pRegist->registerMethod == WINKS_registerMethod_sms)/* 短信注册 */
                        {
                            /* 等待用户发短信 */
                            *msg = WINKS_MSG_ENG2UI_SMS_REG;
                            pRegist->Step = REGIST_STEP_ACTIVATE;
                            if(pRegist->controller == WINKS_ENGAPP_REG_FREGIST)
                                *runctr = WINKS_REG_RUNCTR_PAUSE; /*引擎暂停，等待UI控制。*/
                        }
                        else/* 不知道注册方法 */
                        {
                            *msg = WINKS_MSG_ENG2UI_REG_FAIL;
                        }
                    }
                    else if(pRegist->Step == REGIST_STEP_GETUSERSTATUS_1)
                    {
                        if(pRegist->registerMethod == WINKS_registerMethod_sms)/* 短信注册 */
                        {
                            pRegist->PollCount++;
                            if(pRegist->PollCount>=WINKS_REGIST_POLL_TIMES)
                            {
                                *msg = WINKS_MSG_ENG2UI_REG_FAIL;
                            }
                            else
                            {
                                pRegist->Step = REGIST_STEP_DELAY_QUERY;
                            }
                        }
                    }
                }
            }
        }
        break;
    }

    case REGIST_STEP_SETSERVICEMODE:
    {
        if(pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result)
        {
            iRet = Winks_registorSuccessPrc(pRegist->uid);
            if(iRet == WINKS_RETURN_SUCCESS)
            {
                *msg = WINKS_MSG_ENG2UI_REG_OK;
                *msg_body = presult->rst_txt.greeting;
                *msg_size = strlen(presult->rst_txt.greeting)+1;
                pRegist->Step = REGIST_STEP_GETUSERCONFIG_ALL;
            }
            else
            {
                *msg = WINKS_MSG_ENG2UI_REG_FAIL;
            }
        }
        else
            *msg = WINKS_MSG_ENG2UI_UNKNOWN;
        break;
    }
    
    case REGIST_STEP_GETUSERCONFIG_ALL:/*下载设置项*/
    {
        if(pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_userConfig)
        {
            if(Winks_DoPrtUserConfig((WinksProt_userConfig_s *)pAppMsg->pData) != 0)
            {
                *msg = WINKS_MSG_ENG2UI_USERCFG_FAIL;
            }
            else
            {
                /*置位 下载设置项标志*/
                Winks_SetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_SETTING, 1);
                
                if(Winks_GetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_PB, &val)==0 && val == 0 )
                {
                    pRegist->Step = REGIST_STEP_MERGEPHONEBOOK;
                    *msg = WINKS_MSG_ENG2UI_ULPB;
                    if(pRegist->controller == WINKS_ENGAPP_REG_FREGIST)/*前台激活: 提示是否上传电话本*/
                    {
                        *msg = WINKS_MSG_ENG2UI_ULPB_CONFIRM;
                        *runctr = WINKS_REG_RUNCTR_PAUSE; /*引擎暂停，等待UI控制。*/
                    }
                }
                else
                {
                    if(pRegist->controller == WINKS_ENGAPP_REG_QUERYUS)
                    {
                        *msg = WINKS_MSG_ENG2UI_QUERY_OK;
                    }
                    else
                    {
                        *msg = WINKS_MSG_ENG2UI_INIT_OK; 
                    }
                    *runctr = WINKS_REG_RUNCTR_STOP; /*运行结束。退出流程标志。*/
                }
            }
        }
        else
            *msg = WINKS_MSG_ENG2UI_UNKNOWN;
        break;
    }
    
    case REGIST_STEP_MERGEPHONEBOOK:
    {
        if(pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result)
        {
            /*置位 上传电话本标志*/
            Winks_SetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_PB, 1);
                
            if(pRegist->controller == WINKS_ENGAPP_REG_QUERYUS)/*查询用户状态*/
            {
                *msg = WINKS_MSG_ENG2UI_QUERY_OK;
            }
            else
            {
            
                *msg = WINKS_MSG_ENG2UI_ULPB_OK;
                if(presult->rst_txt.greeting)
                {
                    *msg_body = presult->rst_txt.greeting;
                    *msg_size = strlen(presult->rst_txt.greeting)+1;
                }
            }
            *runctr = WINKS_REG_RUNCTR_STOP; /*运行结束。退出流程标志。*/
        }
        else
            *msg = WINKS_MSG_ENG2UI_UNKNOWN;
        break;
    }
    default:
        break;
    }


    return WINKS_RETURN_SUCCESS;
}

int Winks_RegistTimerExpiry( WINKS_ENAPP_HND appHnd,
                                     WinksEnappRegist_s * pRegist, 
                                     void *data, unsigned long size)
{
    if(pRegist->hTimer == *((WINKS_TIMER_ID *)data))
    {
        if(pRegist->hTimer)
        {
            Winks_EngDestroyTimer(appHnd, pRegist->hTimer);
            pRegist->hTimer = WINKS_TIMER_NULL;
        }
        if(pRegist->Step == REGIST_STEP_DELAY_QUERY)
            pRegist->Step = REGIST_STEP_GETUSERSTATUS_1;
        else
            return WINKS_RETURN_FAILURE;
        Winks_printf("Winks_RegistTimerExpiry PollCount=%d\r\n", pRegist->PollCount);
    }
    else
        return WINKS_RETURN_FAILURE;

    return WINKS_RETURN_SUCCESS;
}

static int Winks_RegistProcRun(WINKS_ENAPP_HND appHnd, WinksPrtRet_s *pRetData, WinksEnappRegist_s * pRegist)
{
    int iRet = 0;
    if(pRetData->msg)
        iRet = Winks_EngPostMsgToUI(pRegist->Pri, pRegist->appType, pRetData->msg, pRetData->msg_body, pRetData->msg_size);

    if((pRetData->msg < 0) || (pRetData->runctr == WINKS_REG_RUNCTR_STOP))
    {
        int NeedInitCC = (int)pRegist->initCC;
        winks_RegistStop(appHnd, pRegist);

        /*激活流程完成后，自动运行初始化流程*/
        if((pRetData->runctr == WINKS_REG_RUNCTR_STOP) && (NeedInitCC == 1))
            Winks_EngInitSpecialCC(WINKS_ENGINE_APP_PRI_USER);
        return 0;
    }

    if(pRetData->runctr == WINKS_REG_RUNCTR_PAUSE)/*引擎机制终端处理，等待UI返回。*/
        pRegist->pause = 1;
    if(!(pRegist->pause))
    {
        if((iRet = winks_RegistStepSend(appHnd, pRegist)) != WINKS_RETURN_SUCCESS)
        {
            if(iRet != WINKS_RETURN_FAILURE)
                Winks_EngPostMsgToUI(pRegist->Pri, pRegist->appType, iRet, NULL, 0);
            else
                Winks_EngPostMsgToUI(pRegist->Pri, pRegist->appType, WINKS_MSG_ENG2UI_PROT_PACK, NULL, 0);
            winks_RegistStop(appHnd, pRegist);
            return 0;
        }
    }
    return 0;
}

static int Winks_RegistProc(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData)
{
    int iRet = 0;
    WinksEnappRegist_s * pRegist = (WinksEnappRegist_s *)addData;
    WinksPrtRet_s retData = {0};

    Winks_mem_set(&retData, 0, sizeof(WinksPrtRet_s));
    Winks_assert(pRegist);
    
    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        break;
        
    case WINKS_MSG_ENGINE_APP_STOP  :
    {
        winks_RegistStop(appHnd, pRegist);
        return 0;
    }
    
    case WINKS_MSG_ENGINE_APP_RESUME:
        break;
        
    case WINKS_MSG_ENGINE_APP_PAUSE :
        if(pRegist->hTimer != WINKS_TIMER_NULL)
        {
            Winks_EngDestroyTimer(appHnd, pRegist->hTimer);
            pRegist->hTimer = WINKS_TIMER_NULL;
        }
        return 0;
        
    case WINKS_MSG_ENGINE_RETMSG    :
        Winks_assert(pRegist->Step < REGIST_STEP_MAX);
        winks_RegistStepRcv(appHnd, pRegist, (WinksEng2AppMsg_s *)msgData, &retData);
        break;
        
    case WINKS_MSG_TIMER_EXPIRY:
        iRet = Winks_RegistTimerExpiry(appHnd, pRegist, msgData, msgDataSize);
        if(iRet != WINKS_RETURN_SUCCESS)
            retData.msg = WINKS_MSG_ENG2UI_APP_ERR;
        break;
        
    default:
        return -1;
    }

    Winks_RegistProcRun(appHnd, &retData, pRegist);
    
    return 0;
}

int Winks_UserRegistExt(unsigned long controller)
{
    WINKS_ENAPP_HND appHnd;
    WinksEnappRegist_s * pRegist;
    int iRet = 0;

#if 0//def WINKS_DUAL_SIM
if(controller==WINKS_ENGAPP_REG_FREGIST || controller==WINKS_ENGAPP_REG_BREGIST)
{
    unsigned short SimStatus = Winks_GetSimStatus();
    Winks_printf("Winks_UserRegist SimStatus=%04x ActiveSim=%d\r\n", SimStatus, Winks_GetActiveSim());
    if( ((SimStatus & 0x0300) == 0x0300) &&  /* 发现两张卡 */
        (!Winks_GetActiveSim()) )/* 两张非注册的Sim卡需要用户先选择用哪张 */
    {
        Winks_EngPostMsgToUI(pRegist->Pri, WINKS_ENGPROTYPE_REGISTER, WINKS_MSG_ENG2UI_SIM_SEL_ERR, NULL, 0);
        return WINKS_RETURN_FAILURE;
    }
    if(Winks_isSimEnabled(Winks_GetActiveSim()) == 0)
    {
        Winks_EngPostMsgToUI(pRegist->Pri, WINKS_ENGPROTYPE_REGISTER, WINKS_MSG_ENG2UI_SIM_UNUSABLE, NULL, 0);
        return WINKS_RETURN_FAILURE;
    }
}
#endif /* WINKS_DUAL_SIM */

    pRegist = Winks_salAlloc(sizeof(WinksEnappRegist_s));
    
    if(pRegist == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }

    /*设置激活流程控制块*/
    Winks_mem_set(pRegist, 0, sizeof(WinksEnappRegist_s));
    pRegist->controller = controller;
    pRegist->Pri = WINKS_ENGINE_APP_PRI_USER;
    switch(pRegist->controller)
    {
        case WINKS_ENGAPP_REG_FREGIST:/*前台激活*/
        case WINKS_ENGAPP_REG_BREGIST:/*后台激活*/
            pRegist->Step = REGIST_STEP_GETSERVICECONFIG; 
            pRegist->appType = WINKS_ENGPROTYPE_REGISTER;
            pRegist->initCC = 1;
            break;
        case WINKS_ENGAPP_REG_QUERYUS:/*用户属性查询，是否激活、禁用、退订用户*/
        {
            char url[WINKS_SIMPLE_URL_MAX_LEN] = {0};
            iRet = Winks_GetSettingString(WINKS_SETTING_OPR_SERVER_URL, url, sizeof(url));
            if(iRet != WINKS_RETURN_SUCCESS || strlen(url) == 0)
                pRegist->Step = REGIST_STEP_GETSERVICECONFIG;/*先联配置服务器*/
            else
                pRegist->Step = REGIST_STEP_GETUSERSTATUS_0;
            
            pRegist->appType = WINKS_ENGPROTYPE_QUREY_US;
            break;
        }
        case WINKS_ENGAPP_REG_UPLOADPB:/*上传电话本*/
            pRegist->Step = REGIST_STEP_MERGEPHONEBOOK; 
            pRegist->appType = WINKS_ENGPROTYPE_UPLOAD_PB;
            break;
        default: 
            Winks_printf("ERR: unknow regist controller.\r\n");
            Winks_salFree(pRegist);
            return WINKS_RETURN_FAILURE;
    }

    /*将流程控制块，注册到引擎中运行。*/
    appHnd = Winks_EngAppStart(pRegist->Pri, Winks_RegistProc, ENAPP_NAME_REG, pRegist);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        Winks_salFree(pRegist);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_UserRegistStop(void)
{
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_REG);
    WinksEnappRegist_s * pRegist = NULL;
    
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }

    pRegist = (WinksEnappRegist_s *)winks_GetEngAppAddData(appHnd);
    return winks_RegistStop(appHnd, pRegist);
}



int Winks_UserRegist(void)
{
    return Winks_UserRegistExt(WINKS_ENGAPP_REG_FREGIST);
}
int Winks_UserCheckState(void)
{
    return Winks_UserRegistExt(WINKS_ENGAPP_REG_QUERYUS);
}
//int Winks_UserGetState(unsigned long *val)

/*用户确认注册，发送注册短信息。返回值:0成功 其他失败。
reg_flag: 0取消注册，1发送注册*/
int Winks_UserSendRegSms(int reg_flag)
{
    int iRet = 0;
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_REG);
    WinksEnappRegist_s * pRegist = NULL;

    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    
    pRegist = (WinksEnappRegist_s *)winks_GetEngAppAddData(appHnd);
    if(pRegist == NULL)
    {
        Winks_assert(pRegist != NULL);
        return WINKS_RETURN_FAILURE;
    }

    if(pRegist->Step != REGIST_STEP_ACTIVATE)
    {
        Winks_EngPostMsgToUI(pRegist->Pri, pRegist->appType, WINKS_MSG_ENG2UI_ENG_BUSY, NULL, 0);
        return WINKS_RETURN_FAILURE;
    }
    
    if(reg_flag == 0)
    {
        iRet = winks_RegistStop(appHnd, pRegist);
    }
    else
    {
        Winks_assert(pRegist->Step == REGIST_STEP_ACTIVATE);
        iRet = winks_RegistStepSend(appHnd, pRegist);
    }
    return iRet;    
}

/*用户确认是否发送电话簿。返回值:0成功 其他失败。
  非注册流程也可以调用该函数上传电话簿
ulpb_flag: 0取消上传，1上传电话簿*/
int Winks_UserUploadPhoneBook(int ulpb_flag)
{
    int iRet = 0;
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_REG);
    WinksEnappRegist_s * pRegist = NULL;

    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        if(ulpb_flag != 1)
        {
            Winks_assert(ulpb_flag == 1);
            return WINKS_RETURN_FAILURE;
        }
        
        iRet = Winks_UserRegistExt(WINKS_ENGAPP_REG_UPLOADPB);
        return iRet;
    }
    else
    {
        pRegist = (WinksEnappRegist_s *)winks_GetEngAppAddData(appHnd);
        if(pRegist == NULL)
        {
            Winks_assert(pRegist != NULL);
            return WINKS_RETURN_FAILURE;
        }

        if(ulpb_flag == 0)
        {
            int NeedInitCC = 0;
            if((pRegist->controller == WINKS_ENGAPP_REG_FREGIST) || 
                (pRegist->controller == WINKS_ENGAPP_REG_BREGIST))
                NeedInitCC = 1;
            
            iRet = winks_RegistStop(appHnd, pRegist);
            
            if(iRet==WINKS_RETURN_SUCCESS && NeedInitCC)
                iRet = Winks_EngInitSpecialCC(WINKS_ENGINE_APP_PRI_USER);
        }
        else
        {
            /* 继续下一步 */
            Winks_assert(pRegist->Step == REGIST_STEP_MERGEPHONEBOOK);
            if(winks_RegistStepSend(appHnd, pRegist) != WINKS_RETURN_SUCCESS)
                return winks_RegistStop(appHnd, pRegist);
        }
    }
    
    return iRet;    
}


int Winks_DoPrtServiceConfig(WinksProt_serviceConfig_s *dp)
{
    struct Prot_address_s *addressp = NULL;
//    struct Prot_config_s *configp = NULL;
//    struct Prot_subscription_s *subscriptionp = NULL;
    int iRet = 0;
    
    if(dp->address)
    {
        addressp = dp->address;
        if(addressp->winks && (iRet = strlen(addressp->winks))>0)
        {
            if(addressp->winks[iRet-1] == '/')
                addressp->winks[iRet-1] = '\0';
            if(Winks_SetSettingString(WINKS_SETTING_OPR_SERVER_URL,
                        addressp->winks) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
      #if 0
        if(addressp->media && (iRet = strlen(addressp->media)))
        {
            if(addressp->media[iRet-1] == '/')
                addressp->media[iRet-1] = '\0';
            if(Winks_SetSettingString(WINKS_SETTING_MEDIA_SERVER_URL,
                        addressp->media) != WINKS_RETURN_SUCCESS)
                msg = WINKS_MSG_ENG2UI_REG_FAIL;
        }
      #endif
        if(addressp->store && (iRet = strlen(addressp->store))>0)
        {
            if(addressp->store[iRet-1] == '/')
                addressp->store[iRet-1] = '\0';
            if(Winks_SetSettingString(WINKS_SETTING_SHOP_SERVER_URL, addressp->store) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }/*
        if(addressp->sms)
        {
            if(Winks_SetSettingValue(WINKS_SETTING_SMS_GATEWAY, addressp->sms) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }*/
        if(addressp->push)
        {
            if(Winks_SetSettingValue(WINKS_SETTING_PUSH_PORT,(unsigned long)addressp->push) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
    }
#if 0/*按新协议调整。*/
    if(dp->config)
    {
        configp = dp->config;
        Winks_mem_set(&pRegist->ServiceMode, 0, sizeof(ServicesType_s));
        if(subscriptionp->subscriber)
        {
            pRegist->ServiceMode.subscription = WINKS_SUBSCRIPTION_SUBSCRIER;
            pRegist->ServiceMode.diy = configp->subscriber_diy;
            pRegist->ServiceMode.vcard = configp->subscriber_vcard;
            pRegist->ServiceMode.activity = configp->subscriber_activity;
        }
        else if(subscriptionp->adware)
        {
            pRegist->ServiceMode.subscription = WINKS_SUBSCRIPTION_ADWARE;
            pRegist->ServiceMode.diy = configp->adware_diy;
            pRegist->ServiceMode.vcard = configp->adware_vcard;
            pRegist->ServiceMode.activity = configp->adware_activity;
        }
        else if(subscriptionp->trial)
        {
            pRegist->ServiceMode.subscription = WINKS_SUBSCRIPTION_TRIAL;
            pRegist->ServiceMode.diy = configp->trial_diy;
            pRegist->ServiceMode.vcard = configp->trial_vcard;
            pRegist->ServiceMode.activity = configp->trial_activity;
        }
        else if(subscriptionp->limited)
        {
            pRegist->ServiceMode.subscription = WINKS_SUBSCRIPTION_LIMITED;
            pRegist->ServiceMode.diy = configp->limited_diy;
            pRegist->ServiceMode.vcard = configp->limited_vcard;
            pRegist->ServiceMode.activity = configp->limited_activity;
        }
        else
            pRegist->ServiceMode.subscription = WINKS_SUBSCRIPTION_SUBSCRIER;/*WINKS_SUBSCRIPTION_NONE;*/

        if(configp->vcash)
            pRegist->ServiceMode.vcash = 0;//TODO: 用户点卡计数
            
        if(pRegist->ServiceMode.subscription == WINKS_SUBSCRIPTION_NONE)
            msg = WINKS_MSG_ENG2UI_REG_FAIL;
        else if(Winks_SetSettingBlock(WINKS_SETTING_BLOCK_SERVER_CONFIG,
                    dp->config, sizeof(struct Prot_config_s)) != WINKS_RETURN_SUCCESS)
            msg = WINKS_MSG_ENG2UI_REG_FAIL;
    }
#endif
    return WINKS_RETURN_SUCCESS;
}

int Winks_registorSuccessPrc(char * uid)
{
    char ImsiBuf[WINKS_IMSI_LEN+1];
    char ActivImsiBuf[WINKS_IMSI_LEN+1];
    int iRet = 0;
    
    /* 存IMSI号 */
    Winks_mem_set(ImsiBuf, 0, sizeof(ImsiBuf));
    iRet = Winks_GetImsi(WINKS_CHN_NULL, ImsiBuf, sizeof(ImsiBuf));
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);

    Winks_mem_set(ActivImsiBuf, 0, sizeof(ActivImsiBuf));
    iRet = Winks_GetRecentlyActivatedImsi(ActivImsiBuf, sizeof(ActivImsiBuf));
    if(strcmp(ImsiBuf, ActivImsiBuf) != 0)
    {
        /*1 需要清掉旧的用户*/
        Winks_StrgDeleteDiskFiles(3);
        iRet = Winks_SetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_SETTING, 0);
        if(iRet!= WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        iRet = Winks_SetSettingValue(WINKS_SETTING_ALREADY_UPLOAD_PB, 0);
         if(iRet!= WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
         
        /*2 设置成新的用户*/
        iRet = Winks_SetRecentlyActivatedImsi(ImsiBuf, WINKS_IMSI_LEN);
        if(iRet!= WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_cSaveUid(uid)!= WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    Winks_SetEngineCurrentState(WINKS_ENGINE_STATE_ENABLED);
    return WINKS_RETURN_SUCCESS;
}
