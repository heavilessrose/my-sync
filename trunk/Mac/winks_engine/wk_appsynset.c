#if 1
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_enfnc.h"
#endif

#define ENAPP_NAME_SYNSET  "synset"

typedef enum tag_SynsetStep_e
{
    SYNSET_STEP_GETSERVICECONFIG = 0,
    SYNSET_STEP_GETUSERCONFIG,
    SYNSET_STEP_CONFIRMMESSAGE,
    SYNSET_STEP_UPLOADFILE,
    SYNSET_STEP_SETUSERCONFIG,
    SYNSET_STEP_MAX
}SynsetStep_e;

typedef struct tag_WinksEnappSynset_s
{
    int     Step;   /* 同步设置的步骤,SynsetStep_e */
    int     Pri;    /* 应用优先级 */
    int     AppType;/* 应用类型 */
    WinksProt_confimMessage_s confimMessage;
    //WinksProt_messageList_s * messageList;
    char    Path[256];
    WinksProt_UploadFile_s UploadFile;
    //char    proxy[256];
    char    apn[32];
    WinksProt_userConfig_s userConfig;
    WinksProt_userConfigEx_s userConfigEx;
    Winks_MemPool_s * pMemPool;
}WinksEnappSynset_s;

int Winks_DoPrtUserConfig(WinksProt_userConfig_s * puserConfig)
{
    if(puserConfig == NULL)
        return -1;

    //TODO: 保存用户设置
    if(puserConfig->proxy && puserConfig->proxyPort)
        Winks_SetProxyServer(puserConfig->proxy, puserConfig->proxyPort);

    if(puserConfig->apn)
        Winks_SetSettingString(WINKS_SETTING_APN, puserConfig->apn);

    if(puserConfig->durationAfterHangup)
        Winks_SetSettingValue(WINKS_SETTING_HUSHOW_TIME, puserConfig->durationAfterHangup);

    if(puserConfig->showWinks_Mask)
        Winks_SetSettingValue(WINKS_SETTING_SHOW_OTHER_SWITCH, puserConfig->showWinks);
        
    if(puserConfig->showMode_Mask)
        Winks_SetSettingValue(WINKS_SETTING_SHOW_CASE, puserConfig->showMode);
    
    if(puserConfig->roaming_Mask)
        Winks_SetSettingValue(WINKS_SETTING_ROAM_SWITCH, puserConfig->roaming);
    
    if(puserConfig->autorun_Mask)
        Winks_SetSettingValue(WINKS_SETTING_AUTO_START_SWITCH, puserConfig->autorun);

    if(puserConfig->updateAfterHangup_Mask)
        Winks_SetSettingValue(WINKS_SETTING_HANGUP_UPDATA_SWITCH, puserConfig->updateAfterHangup);

    if(puserConfig->upgradeMode_Mask)
        Winks_SetSettingValue(WINKS_SETTING_SW_UPDATA_WAY, puserConfig->upgradeMode);
    
    //if(puserConfig->vcardOverwrite_Mask)
    //    Winks_SetSettingValue(WINKS_SETTING_, puserConfig->vcardOverwrite);
    
    if(puserConfig->connectInCall_Mask)
        Winks_SetSettingValue(WINKS_SETTING_CALL_CONNECT_SWITCH, puserConfig->connectInCall);
    
    if(puserConfig->support3G_Mask)
        Winks_SetSettingValue(WINKS_SETTING_ONLY_25G_SWITCH, puserConfig->support3G);
    
    if(puserConfig->networkTimeout)
        Winks_SetSettingValue(WINKS_SETTING_CONNECT_TIMEOUT, puserConfig->networkTimeout);
    
    if(puserConfig->splashDuration)
        Winks_SetSettingValue(WINKS_SETTING_SPLASH_DURATION, puserConfig->splashDuration);

    if(puserConfig->updateOptionMinFee)
        Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY, puserConfig->updateOptionMinFee);
    if(puserConfig->updateOptionMostRecent)
        Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD_USUALLY, puserConfig->updateOptionMostRecent);
    if(puserConfig->updateConfig)
    {
        if(puserConfig->updateConfig->option == WINKS_updateConfig_minFee)
        {
            Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD_MOD, WINKS_SETVAL_CPMOD_UNUSUALLY);
            if(puserConfig->updateOptionMinFee)
                Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD, puserConfig->updateOptionMinFee);
        }
        else if(puserConfig->updateConfig->option == WINKS_updateConfig_mostRecent)
        {
            Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD_MOD, WINKS_SETVAL_CPMOD_USUALLY);
            if(puserConfig->updateOptionMostRecent)
                Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD, puserConfig->updateOptionMostRecent);
        }
        else
        {
            Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD_MOD, WINKS_SETVAL_CPMOD_CUSTOM);
            if(puserConfig->updateConfig->period)
                Winks_SetSettingValue(WINKS_SETTING_CONNECT_PERIOD, puserConfig->updateConfig->period);
        }
    }

	return 0;
}

static int winks_SynsetStop(WINKS_ENAPP_HND appHnd, WinksEnappSynset_s * pSynset)
{
    if(pSynset)
    {
        if(pSynset->pMemPool)
        {
            Winks_EngMemPoolDestory(pSynset->pMemPool);
        }
        Winks_salFree(pSynset);
    }
    return  Winks_EngAppStop(appHnd);
}

static int winks_SynsetStepSend(WINKS_ENAPP_HND appHnd, WinksEnappSynset_s * pSynset)
{
    int iRet = WINKS_RETURN_FAILURE;

    Winks_assert(pSynset);

    switch(pSynset->Step)
    {
    case SYNSET_STEP_GETSERVICECONFIG:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETSERVICECONFIG, NULL, 0 );
        break;
    case SYNSET_STEP_GETUSERCONFIG:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETUSERCONFIG, &pSynset->userConfigEx.AllOrDiff, 0 );
        break;
    case SYNSET_STEP_CONFIRMMESSAGE:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_CONFIRMMESSAGE, 
                                 &pSynset->confimMessage, sizeof(pSynset->confimMessage) );
        break;
    case SYNSET_STEP_UPLOADFILE:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_UPLOADFILE, 
                                 &pSynset->UploadFile, sizeof(pSynset->UploadFile) );
        break;
  #if 0
    case SYNSET_STEP_SETUSERCONFIG:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_SETUSERCONFIG, 
                                 &pSynset->userConfigEx, sizeof(pSynset->userConfigEx) );
        break;
  #endif
    default:
        Winks_fail("winks_SynsetStepSend");
        break;
    }

    return iRet;
}

static int winks_SynsetStepRcv( WINKS_ENAPP_HND appHnd,
                                  WinksEnappSynset_s * pSynset, 
                                  WinksEng2AppMsg_s * pAppMsg )
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     msg = 0;
    void  * msg_body = NULL;
    int     msg_size = 0;
    WinksProt_result_s *presult = NULL;
    int     Finished = 0;

    Winks_assert(pSynset);
    
    if( pAppMsg == NULL)
        msg = WINKS_MSG_ENG2UI_APP_ERR;
    else if( pAppMsg->AppMsg < WINKS_PROT_RET_TYPE_result )
    {
        msg = pAppMsg->AppMsg;
    }

    if(msg)
    {
        Winks_EngPostMsgToUI(pSynset->Pri, pSynset->AppType, msg, NULL, 0);
        return  winks_SynsetStop(appHnd, pSynset);
    }
    
    if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result )
    {
        presult = (WinksProt_result_s *)pAppMsg->pData;
    }
    
    if( pSynset->Step == SYNSET_STEP_GETSERVICECONFIG || 
        pSynset->Step == SYNSET_STEP_GETUSERCONFIG )
    {
        if( pSynset->Step == SYNSET_STEP_GETSERVICECONFIG &&
            pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_serviceConfig )
        {
            if(Winks_DoPrtServiceConfig((WinksProt_serviceConfig_s *)pAppMsg->pData) != 0)
                msg = WINKS_MSG_ENG2UI_UNKNOWN;
            pSynset->confimMessage.mid = WINKS_MESSAGEIDTYPE_SCFG;
        }
        else if( pSynset->Step == SYNSET_STEP_GETUSERCONFIG  &&
            pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_userConfig )
        {
            if(Winks_DoPrtUserConfig((WinksProt_userConfig_s *)pAppMsg->pData) != 0)
                msg = WINKS_MSG_ENG2UI_UNKNOWN;
            pSynset->confimMessage.mid = WINKS_MESSAGEIDTYPE_CCFG;
            if(pSynset->AppType == WINKS_ENGPROTYPE_UPDATE_LS)
                Finished = 1;
        }
        else if(presult)
        {
            if(presult->code == 0)
            {
            }
            else
            {
                if(Winks_EngResultErrDftHandler(presult, &msg) != 1)
                    msg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
            }
            
        }
        else
            msg = WINKS_MSG_ENG2UI_UNKNOWN;
        
        /* 服务器下发的各默认值都成功保存，继续下一步 */
        if(msg == 0)
        {
            pSynset->Step = SYNSET_STEP_CONFIRMMESSAGE;
            winks_SynsetStepSend(appHnd, pSynset);
        }
    }
    else if( pSynset->Step == SYNSET_STEP_CONFIRMMESSAGE ||
             pSynset->Step == SYNSET_STEP_UPLOADFILE ||
             pSynset->Step == SYNSET_STEP_SETUSERCONFIG )
    {
        if(presult)
        {
            if(presult->code == 0)
            {
                Finished = 1;
            }
            else
            {
                if(Winks_EngResultErrDftHandler(presult, &msg) != 1)
                    msg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
            }
        }
        else
            msg = WINKS_MSG_ENG2UI_UNKNOWN;
    }
    else
    {
        msg = WINKS_MSG_ENG2UI_APP_ERR;
    }

    if(msg)
    {
        iRet = Winks_EngPostMsgToUI(pSynset->Pri, pSynset->AppType, msg, msg_body, msg_size);
    }
    else
    {
        //WinksProgress_s Progress;

        //Winks_mem_set(&Progress, 0, sizeof(WinksProgress_s));
        if(Finished)
        {
            if(pSynset->Step == SYNSET_STEP_CONFIRMMESSAGE)
            {
                if(pSynset->confimMessage.mid == WINKS_MESSAGEIDTYPE_SCFG)
                    msg = WINKS_MSG_ENG2UI_SRVCFG_OK;
                else/* if(pSynset->confimMessage.mid == WINKS_MSG_ENG2UI_USERCONFIG_OK)*/
                    msg = WINKS_MSG_ENG2UI_USERCFG_OK;
            }
            else if(pSynset->Step == SYNSET_STEP_UPLOADFILE)
            {
                if(pSynset->UploadFile.fileType == WINKS_UPLD_FILETYPE_stat)
                    msg = WINKS_MSG_ENG2UI_ULSTAT_OK;
                else/* if(pSynset->UploadFile.fileType == WINKS_UPLD_FILETYPE_log)*/
                    msg = WINKS_MSG_ENG2UI_ULLOG_OK;
            }
            else
                msg = WINKS_MSG_ENG2UI_SETUP_OK;
        }
        else
            msg = WINKS_MSG_ENG2UI_SETUP;
        if(msg)
            iRet = Winks_EngPostMsgToUI(pSynset->Pri, pSynset->AppType, msg, NULL/*(void *)&Progress*/, 0/*sizeof(WinksProgress_s)*/);
    }

    if((msg < 0) || Finished)
    {
        iRet = winks_SynsetStop(appHnd, pSynset);
    }

    return iRet;
}

int Winks_SynsetProc(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData)
{
    int iRet = 0;
    WinksEnappSynset_s * pSynset = (WinksEnappSynset_s *)addData;
    Winks_assert(pSynset);

    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        Winks_assert(pSynset->Step >= 0 &&  pSynset->Step < SYNSET_STEP_MAX);
        if((iRet = winks_SynsetStepSend(appHnd, pSynset)) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pSynset->Pri, pSynset->AppType, iRet, NULL, 0);
            return winks_SynsetStop(appHnd, pSynset);
        }
        break;
    case WINKS_MSG_ENGINE_APP_STOP  :
        if(pSynset)
        {
            if(pSynset->pMemPool)
            {
                Winks_EngMemPoolDestory(pSynset->pMemPool);
            }
            Winks_salFree(pSynset);
        }
        break;
    case WINKS_MSG_ENGINE_APP_PAUSE :
        break;
    case WINKS_MSG_ENGINE_APP_RESUME:
        {
        switch(pSynset->AppType)
        {
        case WINKS_ENGPROTYPE_UPDATE_SC:
            pSynset->Step = SYNSET_STEP_GETSERVICECONFIG;
            break;
        case WINKS_ENGPROTYPE_UPDATE_UC:
            pSynset->Step = SYNSET_STEP_GETUSERCONFIG;
            break;
        case WINKS_ENGPROTYPE_UPLOAD_STA:
            pSynset->Step = SYNSET_STEP_UPLOADFILE;
            break;
        case WINKS_ENGPROTYPE_UPLOAD_LOG:
            pSynset->Step = SYNSET_STEP_UPLOADFILE;
            break;
        case WINKS_ENGPROTYPE_UPDATE_LS:
            pSynset->Step = SYNSET_STEP_GETUSERCONFIG;
        default:
            Winks_fail("pSynset->AppType");
            break;
        }
        if((iRet = winks_SynsetStepSend(appHnd, pSynset)) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pSynset->Pri, pSynset->AppType, iRet, NULL, 0);
            return winks_SynsetStop(appHnd, pSynset);
        }
        }
        break;
    case WINKS_MSG_ENGINE_RETMSG    :
        Winks_assert(pSynset->Step < SYNSET_STEP_MAX);
        iRet = winks_SynsetStepRcv(appHnd, pSynset, (WinksEng2AppMsg_s *)msgData);
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    return iRet;
}

/*
输入:
    Pri --- 优先级
    Type--- SynsetType_e
    pData:当 Type == SYNSET_TYPE_UPLOAD_STAT或SYNSET_TYPE_UPLOAD_LOG时,该项为UploadFile.fileNumber
             Type == SYNSET_TYPE_SETUSERCONFIG时,该项为WinksProt_userConfig_s结构
*/
int Winks_SynsetStart(int Pri, SynsetType_e Type, void * pData)
{
    WINKS_ENAPP_HND appHnd;
    WinksEnappSynset_s * pSynset = Winks_salAlloc(sizeof(WinksEnappSynset_s));
    
    if(pSynset == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    
    Winks_mem_set(pSynset, 0, sizeof(WinksEnappSynset_s));
    switch(Type)
    {
    case SYNSET_TYPE_SERVICECONFIG:
        pSynset->Step = SYNSET_STEP_GETSERVICECONFIG;
        pSynset->AppType = WINKS_ENGPROTYPE_UPDATE_SC;
        break;
    case SYNSET_TYPE_USERCONFIG:
        pSynset->Step = SYNSET_STEP_GETUSERCONFIG;
        pSynset->AppType = WINKS_ENGPROTYPE_UPDATE_UC;
        pSynset->userConfigEx.AllOrDiff = 0;
        break;
    case SYNSET_TYPE_UPLOAD_STAT:
        pSynset->Step = SYNSET_STEP_UPLOADFILE;
        pSynset->AppType = WINKS_ENGPROTYPE_UPLOAD_STA;
        pSynset->UploadFile.fileType = WINKS_UPLD_FILETYPE_stat;
        if(pData)
            pSynset->UploadFile.fileNumber = *((unsigned long *)pData);
        Winks_Ascii2UnicodeString(WINKS_STAT_FILE, pSynset->Path, sizeof(pSynset->Path));
        pSynset->UploadFile.attachName = (unsigned char *)pSynset->Path;
        pSynset->UploadFile.attachNameLen = (WK_STRLEN(pSynset->Path)+1)<<1;
        pSynset->UploadFile.pContenttype = HTTP_CONTENTTYPE_bin;
        break;
    case SYNSET_TYPE_UPLOAD_LOG:
        Winks_fdelete(WINKS_DB_FILE_IN);
        if(Winks_frename(WINKS_DB_FILE, WINKS_DB_FILE_IN) != 0)
            return WINKS_RETURN_FAILURE;
        
        pSynset->Step = SYNSET_STEP_UPLOADFILE;
        pSynset->AppType = WINKS_ENGPROTYPE_UPLOAD_LOG;
        pSynset->UploadFile.fileType = WINKS_UPLD_FILETYPE_log;
        if(pData)
            pSynset->UploadFile.fileNumber = *((unsigned long *)pData);
        Winks_Ascii2UnicodeString(WINKS_DB_FILE_IN, pSynset->Path, sizeof(pSynset->Path));
        pSynset->UploadFile.attachName = (unsigned char *)pSynset->Path;
        pSynset->UploadFile.attachNameLen = (WK_STRLEN(pSynset->Path)+1)<<1;
        pSynset->UploadFile.pContenttype = HTTP_CONTENTTYPE_bin;
        break;
  #if 0
    case SYNSET_TYPE_SETUSERCONFIG:
        {
        WinksProt_userConfig_s * puserConfig = NULL;
        Winks_assert(pData);
        
        puserConfig = (WinksProt_userConfig_s *)pData;
        Winks_mem_cpy(&pSynset->userConfig, puserConfig, sizeof(WinksProt_userConfig_s));
        pSynset->userConfigEx.userConfig = &pSynset->userConfig;
        pSynset->userConfigEx.AllOrDiff = 1;
        if(puserConfig->proxy)
        {
            strcpy(pSynset->Path, puserConfig->proxy);
            pSynset->userConfig.proxy = pSynset->Path;
        }
        if(puserConfig->apn)
        {
            strcpy(pSynset->apn, puserConfig->apn);
            pSynset->userConfig.apn = pSynset->apn;
        }
        pSynset->Step = SYNSET_STEP_SETUSERCONFIG;
        pSynset->AppType = WINKS_ENGPROTYPE_UPDATE_LS;
        }
        break;
  #endif
    case SYNSET_TYPE_UPDUSERCONFIG:
        pSynset->Step = SYNSET_STEP_GETUSERCONFIG;
        pSynset->AppType = WINKS_ENGPROTYPE_UPDATE_LS;
        pSynset->userConfigEx.AllOrDiff = 1;
        break;
    default:
        Winks_fail("Winks_SynsetStart");
        break;
    }
    
    pSynset->Pri = Pri;
    appHnd = Winks_EngAppStart(pSynset->Pri, Winks_SynsetProc, ENAPP_NAME_SYNSET, pSynset);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        if(pSynset)
        {
            if(pSynset->pMemPool)
            {
                Winks_EngMemPoolDestory(pSynset->pMemPool);
            }
            Winks_salFree(pSynset);
        }
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_SynsetStop(void)
{
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_SYNSET);
    
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_EngAppStop(appHnd) != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }

	return WINKS_RETURN_SUCCESS;
}


/*上传设置项，备份*/
int Winks_UserUploadSetting(void)
{
    return 0;
}
/*下载设置项*/
int Winks_UserDownLoadSetting(void)
{
    return Winks_SynsetStart(WINKS_ENGINE_APP_PRI_USER, SYNSET_TYPE_UPDUSERCONFIG, NULL);
}