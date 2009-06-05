#if 1
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_enfnc.h"
#endif

#define ENAPP_NAME_UPGRADE  "upgrade"

typedef enum tag_UpgradeStep_e
{
    UPGRADE_STEP_CHECK_NEW_VERSION = 0,
    UPGRADE_STEP_DOWNLOAD_NEW_VERSION,
    UPGRADE_STEP_MAX
}UpgradeStep_e;

typedef struct tag_WinksEnappUpgrade_s
{
    int     Step;   /* 升级步骤,UpgradeStep_e */
    int     Pri;    /* 应用优先级 */
    Winks_MemPool_s *pMem;
    int blocklen;
    FILE *sisFileHandle;
    char sisName[256];
    char url[1024];
    int SaveIdx;
    int isAutoUpgrade;
    void *data;
}WinksEnappUpgrade_s;

static int winks_UpgradeStop(WINKS_ENAPP_HND appHnd, WinksEnappUpgrade_s * pUpgrade)
{
    if(pUpgrade)
    {
        if(pUpgrade->sisFileHandle)
        {
            Winks_fclose(pUpgrade->sisFileHandle);
            pUpgrade->sisFileHandle = NULL;
        }        
        Winks_salFree(pUpgrade);
    }
    return Winks_EngAppStop(appHnd);
}


static int winks_UpgradeStepSend(WINKS_ENAPP_HND appHnd, WinksEnappUpgrade_s * pUpgrade)
{
    int iRet = WINKS_RETURN_FAILURE;
    
    switch(pUpgrade->Step)
    {
    case UPGRADE_STEP_CHECK_NEW_VERSION:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE, NULL, 0 );
        break;
    case UPGRADE_STEP_DOWNLOAD_NEW_VERSION:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETURLFILE, pUpgrade->url, strlen(pUpgrade->url)+1 );
        break;
        
    default:
        Winks_fail("winks_UpgradeStepSend");
        break;
    }

    return iRet;
}

static int Winks_SaveSis(WinksEnappUpgrade_s *pUpgrade)
{
    FILE  * fp;
    int     FsErr = 0;
    unsigned long   wsize = 0;

    Winks_assert(pUpgrade != NULL);
    
    if(pUpgrade->sisFileHandle == 0 || pUpgrade->pMem == NULL)
        return WINKS_RETURN_FAILURE;
    
    fp = (FILE *)pUpgrade->sisFileHandle;

    while( pUpgrade->pMem && 
            pUpgrade->pMem->pdata && 
            pUpgrade->pMem->offset && 
           wsize < pUpgrade->blocklen )
    {
        FsErr = Winks_fwrite(pUpgrade->pMem->pdata, pUpgrade->pMem->offset, 1, fp);
        if(FsErr < 0)
        {
            return WINKS_RETURN_FAILURE;
        }
        
        wsize += pUpgrade->pMem->offset;
        pUpgrade->pMem = pUpgrade->pMem->pnext;
    }
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_UpgradeStepRcv( WINKS_ENAPP_HND appHnd,
                                      WinksEnappUpgrade_s * pUpgrade, 
                                      WinksEng2AppMsg_s * pAppMsg )
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     msg = 0;
    void  * msg_body = NULL;
    int     msg_size = 0;
    WinksProt_result_s *presult = NULL;

    if( pAppMsg == NULL)
        msg = WINKS_MSG_ENG2UI_APP_ERR;
    else if( pAppMsg->AppMsg < WINKS_PROT_RET_TYPE_result )
    {
        msg = pAppMsg->AppMsg;
    }

    if(msg)
    {
        Winks_EngPostMsgToUI(pUpgrade->Pri, WINKS_ENGPROTYPE_UPDATE_SW, msg, NULL, 0);
        return  winks_UpgradeStop(appHnd, pUpgrade);
    }
    
    if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result )
    {
        presult = (WinksProt_result_s *)pAppMsg->pData;
    }
    switch(pUpgrade->Step)
    {
    case UPGRADE_STEP_CHECK_NEW_VERSION:
        if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_softwarePackage )
        {
            WinksProt_softwarePackage_s *psoftwarePackage = 
                (WinksProt_softwarePackage_s *)pAppMsg->pData;
            if(psoftwarePackage == NULL)
            {
                msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;
            }
            else
            {
                int r = strcmp(psoftwarePackage->version, WINKS_AV);
                if(r>0)
                {
                    strcpy(pUpgrade->sisName, WINKS_APP_PATH);
                    strcat(pUpgrade->sisName, (const char *)psoftwarePackage->name);
                    strcat(pUpgrade->sisName, ".sis");
                    strcpy(pUpgrade->url, psoftwarePackage->file);
                    
                    /* 继续下一步 */
                    pUpgrade->Step = UPGRADE_STEP_DOWNLOAD_NEW_VERSION;
                    if(winks_UpgradeStepSend(appHnd, pUpgrade) != WINKS_RETURN_SUCCESS)
                        return winks_UpgradeStop(appHnd, pUpgrade);
                }
                else
                {
                    /* 不需要升级 */
                    msg = WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW;
                }
            }
        }
        else if(presult)
        {
            if(presult->code == 0)
                msg = WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW;
            else if(Winks_EngResultErrDftHandler(presult, &msg) != 1)
                msg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
        }
        break;
    case UPGRADE_STEP_DOWNLOAD_NEW_VERSION:
        if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_file )
        {
            Winks_fileinfo_s *pfileinfo = 
                (Winks_fileinfo_s *)pAppMsg->pData;
            if(pfileinfo == NULL)
            {
                msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;
            }
            else
            {
                if(pfileinfo->ext==WINKS_FILE_EXT_bin && pfileinfo->filesize )
                {
                    if(pfileinfo->blocklen)
                    {
                        if(pUpgrade->SaveIdx==0)
                        {
                            if(Winks_fexist(pUpgrade->sisName))
                                Winks_fdelete(pUpgrade->sisName);
                            pUpgrade->sisFileHandle = Winks_fopen(pUpgrade->sisName, "wb");
                            pUpgrade->SaveIdx ++;
                        }
                        if(pUpgrade->sisFileHandle)
                        {
                            pUpgrade->pMem = pfileinfo->pMem;
                            pUpgrade->blocklen = pfileinfo->blocklen;
                            
                            if(Winks_SaveSis(pUpgrade) != WINKS_RETURN_SUCCESS)
                            {
                                msg = WINKS_MSG_ENG2UI_FS;
                            }
                            pUpgrade->SaveIdx ++;
                        }
                    }
                    else if(pUpgrade->sisFileHandle)
                    {
                        Winks_fclose(pUpgrade->sisFileHandle);
                        pUpgrade->sisFileHandle = NULL;
                        pUpgrade->SaveIdx = 0;
                        
                        //通知调用者新版本的安装包存放路径，
                        //如果为手动更新，直接启动安装
                        //如果自动更新，只是记下此路径，在退出软件时提示用户安装
                        msg_body = pUpgrade->sisName;
                        msg_size = strlen(pUpgrade->sisName) + 1;
                        msg = WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK;
                    }
                }
            }
        }
        else if(presult)
        {
            if(Winks_EngResultErrDftHandler(presult, &msg) != 1)
                msg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
        }

        break;

    default:
        break;
    }

    if(msg)
        iRet = Winks_EngPostMsgToUI(pUpgrade->Pri, WINKS_ENGPROTYPE_UPDATE_SW, msg, msg_body, msg_size);

    if((msg < 0) || (msg == WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK)\
            || (msg == WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW))
    {
        iRet = winks_UpgradeStop(appHnd, pUpgrade);
    //    if(msg == WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK)
    //        iRet = Winks_EngInitSpecialCC(WINKS_ENGINE_APP_PRI_USER);
    }

    return iRet;
}

int Winks_UpgradeProc(WINKS_ENAPP_HND appHnd, unsigned long msg, \
        void *msgData, unsigned long msgDataSize, void *addData)
{
    int iRet = 0;
    WinksEnappUpgrade_s * pUpgrade = (WinksEnappUpgrade_s *)addData;
    Winks_assert(pUpgrade);
    
    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        Winks_assert(pUpgrade->Step == UPGRADE_STEP_CHECK_NEW_VERSION);
        if(winks_UpgradeStepSend(appHnd, pUpgrade) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pUpgrade->Pri, WINKS_ENGPROTYPE_UPDATE_SW, WINKS_MSG_ENG2UI_APP_ERR, NULL, 0);
            return winks_UpgradeStop(appHnd, pUpgrade);
        }
        break;
    case WINKS_MSG_ENGINE_APP_STOP  :
        
        break;
    case WINKS_MSG_ENGINE_APP_PAUSE :
        
        break;
    case WINKS_MSG_ENGINE_APP_RESUME:
        
        break;
    case WINKS_MSG_ENGINE_RETMSG    :
        Winks_assert(pUpgrade->Step < UPGRADE_STEP_MAX);
        iRet = winks_UpgradeStepRcv(appHnd, pUpgrade, (WinksEng2AppMsg_s *)msgData);
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    return iRet;
}

int Winks_UpgradeStart(int type)
{
    WINKS_ENAPP_HND appHnd;
    WinksEnappUpgrade_s * pUpgrade = Winks_salAlloc(sizeof(WinksEnappUpgrade_s));
    
    if(pUpgrade == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_mem_set(pUpgrade, 0, sizeof(WinksEnappUpgrade_s));
    pUpgrade->Step = UPGRADE_STEP_CHECK_NEW_VERSION;
    pUpgrade->Pri = WINKS_ENGINE_APP_PRI_NORMAL;
    if(type == 1)
        pUpgrade->Pri = WINKS_ENGINE_APP_PRI_USER;
    appHnd = Winks_EngAppStart((Winks_EngineAppPri_e)pUpgrade->Pri, \
            Winks_UpgradeProc, ENAPP_NAME_UPGRADE, pUpgrade);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        Winks_salFree(pUpgrade);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_UpgradeStop(void)
{
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_UPGRADE);
    
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


