#if 1
#include "wk_public.h"
#include "wk_osal.h"

#include "wk_enfnc.h"
#endif

#define ENAPP_NAME_BGGETCC      "bggetcc"
#define ENAPP_NAME_FGGETCC      "fggetcc"
#define ENAPP_NAME_CALLGETCC    "call"

static int winks_GetCCStop(WINKS_ENAPP_HND appHnd, WinksCCDownCtxt_s * pGetCC)
{
    if(pGetCC)
        Winks_EngCCDownCtxtDestory(pGetCC);
    return Winks_EngAppStop(appHnd);
}

static int winks_GetCCStepSend(WinksCCDownCtxt_s * pGetCC)
{
    int iRet = WINKS_RETURN_FAILURE;

    Winks_assert(pGetCC);
    Winks_assert(pGetCC->pPNoCurr);
    Winks_assert(pGetCC->pPNoCurr->str);
    Winks_printf("winks_GetCCStepSend(\"%s\") Step=%d\r\n", pGetCC->AppName, pGetCC->Step);
    switch(pGetCC->Step)
    {
    case GETCC_STEP_GETCCINFO:
        if(strlen(pGetCC->pPNoCurr->str) == 2)
        {
            int wt = WINKS_SPECIAL_SPLASH;

            if(strcmp(WINKS_PRI_CC_NO, pGetCC->pPNoCurr->str) == 0)
                wt = WINKS_SPECIAL_SPLASH;
            else if(strcmp(WINKS_BUS_CC_NO, pGetCC->pPNoCurr->str) == 0)
                wt = WINKS_SPECIAL_CARRIER;
                
            iRet = Winks_EngProtReq( WINKS_PROTOCOL_SHOWSPECIALWINKS, 
                                     &wt, sizeof(wt) );
        }
        else
        {
            iRet = Winks_EngProtReq( WINKS_PROTOCOL_SHOWWINKS, 
                                     pGetCC->pPNoCurr->str, strlen(pGetCC->pPNoCurr->str) );
        }
        break;
    case GETCC_STEP_GETURLFILE:
        Winks_assert(pGetCC->mediaurl);
        Winks_assert(pGetCC->mediaurl->url);
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_GETURLFILE, 
                                 pGetCC->mediaurl->url, strlen(pGetCC->mediaurl->url)+1 );
        break;
    default:
        Winks_fail("winks_GetCCStepSend");
        break;
    }

    return iRet;
}

static int winks_GetNextNoCC( WinksCCDownCtxt_s * pGetCC, 
                                      int *retFinished, int *retMsg )
{
    int     iRet = 0;

    if(pGetCC->CCDownType == WINKS_CCDOWN_TYPE_RECRUIT)
        Winks_StrgULDelete(pGetCC->pPNoCurr->str);
    
    if(pGetCC->mediaurl)
    {
        Winks_salFree(pGetCC->mediaurl);
        pGetCC->mediaurl = NULL;
    }
    if(pGetCC->thumburl)
    {
        Winks_salFree(pGetCC->thumburl);
        pGetCC->thumburl = NULL;
    }
    if(pGetCC->pPNoCurr->pnext == NULL)
    {
        //TODO -- 完成 
        if(retFinished)
            *retFinished = 1;
    }
    else
    {
        /* 下一个号码 */
        pGetCC->pPNoCurr = pGetCC->pPNoCurr->pnext;
        pGetCC->Step = GETCC_STEP_GETCCINFO;
        if((iRet = winks_GetCCStepSend(pGetCC)) 
                            != WINKS_RETURN_SUCCESS)
        {
            if(retMsg)
                *retMsg = iRet;
        }
        pGetCC->Curr++;
    }

    return 0;
}

static int winks_GetCCStepRcv( WINKS_ENAPP_HND appHnd,
                                  WinksCCDownCtxt_s * pGetCC, 
                                  WinksEng2AppMsg_s * pAppMsg )
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     msg = 0;
    void  * msg_body = NULL;
    int     msg_size = 0;
    WinksProt_result_s *presult = NULL;
    int     NeedWashOut = 0;
    int     Finished = 0;

    Winks_assert(pGetCC);
    
    if( pAppMsg == NULL)
        msg = WINKS_MSG_ENG2UI_APP_ERR;
    else if( pAppMsg->AppMsg < WINKS_PROT_RET_TYPE_result )
    {
        msg = pAppMsg->AppMsg;
    }

    if(msg)
    {
        if( pGetCC->CCDownType == WINKS_CCDOWN_TYPE_CALL &&
            pGetCC->Curr == 0 )
        {
            Winks_EngUpdateCCShowFail();
            msg = WINKS_MSG_ENG2UI_CALL_FAIL;
        }
        Winks_EngPostMsgToUI(pGetCC->Pri,pGetCC->AppType, msg, NULL, 0);
        return  winks_GetCCStop(appHnd, pGetCC);
    }
    
    if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result )
    {
        presult = (WinksProt_result_s *)pAppMsg->pData;
    }
    
    if( pGetCC->CCDownType == WINKS_CCDOWN_TYPE_FRIEND ||
        pGetCC->CCDownType == WINKS_CCDOWN_TYPE_CALL )
        NeedWashOut = 1;
                
    if(pGetCC->Step == GETCC_STEP_GETCCINFO)
    {
        if(pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_winksShow)/* 返回用户彩像基本信息 */
        {
            int Count;
            WinksProt_winksShow_s * pwinksShow = 
                (WinksProt_winksShow_s *)pAppMsg->pData;
            
            if( pwinksShow->wid &&
                strlen(pwinksShow->wid) < sizeof(pGetCC->wid) )
                strncpy(pGetCC->wid, pwinksShow->wid, sizeof(pGetCC->wid));
            else
                msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;

            if( pwinksShow->media_num && pwinksShow->media_num < 256 &&
                pwinksShow->media )
            {
                pGetCC->media_num = (unsigned char)Winks_EngCCDownCtxtSaveUrl(&pGetCC->mediaurl, pwinksShow->media);
                if(pGetCC->media_num == 0)
                {
                    msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;
                }
            }
            
            if( pwinksShow->thumbnail_num && pwinksShow->thumbnail_num < 256 &&
                pwinksShow->thumbnail )
            {
                pGetCC->thumbnail_num = (unsigned char)Winks_EngCCDownCtxtSaveUrl(&pGetCC->thumburl, pwinksShow->thumbnail);
                if(pGetCC->thumbnail_num == 0)
                {
                    msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;
                }
            }
                
            if( pwinksShow->greeting)
            {
                Count = Winks_Utf2UnicodeString((char *)pwinksShow->greeting, 
                                (char *)pGetCC->greet, sizeof(pGetCC->greet));
                pGetCC->greetlen = (unsigned char)(Count<<1);
            }
            else if(pwinksShow->greetValid)
            {
                pGetCC->greetlen = 0xff;
            }
            
            if( pwinksShow->status )
            {
                if(pwinksShow->status == WINKS_status_metting)
                    pGetCC->status = 1;
                else
                    pGetCC->status = 0;
            }
            if( pwinksShow->statusText)
            {
                Count = Winks_Utf2UnicodeString((char *)pwinksShow->statusText, 
                                (char *)pGetCC->statustxt, sizeof(pGetCC->statustxt));
                pGetCC->statustxtlen = Count<<1;
            }

            if(pwinksShow->vcard)
            {
                Winks_EngCCDownCtxtSaveVCard(pwinksShow->vcard);
            }

            if(msg == 0)
            {
                iRet = Winks_EngSearchCCInfo(pGetCC, NeedWashOut);
                if(iRet<0)
                {
                    msg = WINKS_MSG_ENG2UI_ENG_ERR;
                }
                else if(iRet==0)/* 彩像已存在，不需要下载 */
                {
                    winks_GetNextNoCC(pGetCC, &Finished, &msg);
                }
                else if(pGetCC->mediaurl)
                {
                    /* 下载彩像文件 */
                    Winks_assert(pGetCC->mediaurl->url);
                    
                    pGetCC->media_idx = 0;
                    pGetCC->Step = GETCC_STEP_GETURLFILE;
                    if((iRet = winks_GetCCStepSend(pGetCC)) 
                                            != WINKS_RETURN_SUCCESS)
                    {
                        msg = iRet;
                    }
                }
                else
                {
                    Winks_fail("mediaurl is null");
                    msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;
                }
            }
        }
        else if(presult)
        {
            if(presult->code == SERVICE_NOT_MEMBER)
            {
                winks_GetNextNoCC(pGetCC, &Finished, &msg);
            }
            else if(Winks_EngResultErrDftHandler(presult, &msg) != 1)
                msg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
        }
        else
            msg = WINKS_MSG_ENG2UI_UNKNOWN;
    }
    else
    {
        if(pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_file)/* 返回彩像文件 */
        {
            int     IfCached = 0;
            Winks_fileinfo_s * pfileinfo = 
                (Winks_fileinfo_s *)pAppMsg->pData;

            Winks_assert(pGetCC->pPNoCurr);
            Winks_assert(pGetCC->pPNoCurr->str);
            Winks_assert(pGetCC->wid);
            Winks_assert(pfileinfo);

            if( pfileinfo->ext>WINKS_FILE_EXT_xml && 
                pfileinfo->ext<WINKS_FILE_EXT_bin &&
                pfileinfo->filesize )
            {
                if(pfileinfo->blocklen)
                {
                    if(pGetCC->SaveIdx == 0)
                    {
                        pGetCC->img_type = (unsigned char)pfileinfo->ext;
                        Winks_mem_set(&pGetCC->CCImage, 0, sizeof(pGetCC->CCImage));
                        pGetCC->CCImage.NO = pGetCC->pPNoCurr->str;
                        pGetCC->CCImage.ID = pGetCC->wid;
                        pGetCC->CCImage.img_type = (unsigned char)pfileinfo->ext;
                        pGetCC->CCImage.img_idx = pGetCC->media_idx;
                        pGetCC->CCImage.img_cnt = pGetCC->media_num;
                        pGetCC->CCImage.FileSize = pfileinfo->filesize;
                        if(Winks_EngSaveCCInfo(pGetCC, NeedWashOut)!= WINKS_RETURN_SUCCESS)
                        {
                            msg = WINKS_MSG_ENG2UI_ENG_ERR;
                        }
                        else
                        {
                            if( pGetCC->CCDownType == WINKS_CCDOWN_TYPE_CALL &&
                                pGetCC->media_idx == 0 )
                                IfCached = 1;
                            
                            if(Winks_StrgSaveCCImageBegin(pGetCC->CCIdx, &pGetCC->CCImage, IfCached) 
                                        != WINKS_RETURN_SUCCESS)
                            {
                                msg = WINKS_MSG_ENG2UI_ENG_ERR;
                            }
                            pGetCC->SaveIdx++;
                        }
                    }
                    if(pGetCC->CCImage.file_handle)
                    {
                        if(pfileinfo->pMem && pfileinfo->blocklen)
                        {
                            pGetCC->CCImage.pMem = pfileinfo->pMem;
                            pGetCC->CCImage.blocklen = pfileinfo->blocklen;

                            if(Winks_StrgSaveCCImageDo(pGetCC->CCIdx, &pGetCC->CCImage)
                                        != WINKS_RETURN_SUCCESS)
                            {
                                msg = WINKS_MSG_ENG2UI_ENG_ERR;
                            }
                            pGetCC->SaveIdx++;
                        }
                    }
                }
                else if(pGetCC->CCImage.file_handle)
                {
                    if(Winks_StrgSaveCCImageEnd(pGetCC->CCIdx, &pGetCC->CCImage)
                                != WINKS_RETURN_SUCCESS)
                    {
                        msg = WINKS_MSG_ENG2UI_ENG_ERR;
                    }
                    pGetCC->SaveIdx = 0;
                    
                    pGetCC->media_idx++;
                    
                    if( pGetCC->media_idx<pGetCC->media_num &&
                        pGetCC->mediaurl->pnext )
                    {
                        /* 下载彩像的下一幅图 */
                        pGetCC->mediaurl = pGetCC->mediaurl->pnext;
                        pGetCC->Step = GETCC_STEP_GETURLFILE;
                        if((iRet = winks_GetCCStepSend(pGetCC)) != WINKS_RETURN_SUCCESS)
                        {
                            msg = iRet;
                        }
                    }
                    else
                    {
                        winks_GetNextNoCC(pGetCC, &Finished, &msg);
                    }
                }
            }
        }
        else if(presult)
        {
            if(Winks_EngResultErrDftHandler(presult, &msg) != 1)
                msg = WINKS_MSG_ENG2UI_SRV_ERRCODE;
        }
        else
            msg = WINKS_MSG_ENG2UI_UNKNOWN;
    }

    if(msg)
    {
        switch(pGetCC->CCDownType)
        {
        #ifdef WINKS_DEBUG
        case WINKS_CCDOWN_TYPE_PERIOD:
            break;
        case WINKS_CCDOWN_TYPE_CCCACHE:
            break;
        case WINKS_CCDOWN_TYPE_RECRUIT:
            break;
        #endif
        case WINKS_CCDOWN_TYPE_FRIEND:
            msg = WINKS_MSG_ENG2UI_FRIEND_FAIL;
            break;
        case WINKS_CCDOWN_TYPE_INIT:
            msg = WINKS_MSG_ENG2UI_INIT_FAIL;
            break;
        case WINKS_CCDOWN_TYPE_CALL:
            if(pGetCC->Curr == 0)
            {
                msg = WINKS_MSG_ENG2UI_CALL_FAIL;
            }
            break;
        }
        if(msg)
            iRet = Winks_EngPostMsgToUI(pGetCC->Pri, pGetCC->AppType, msg, msg_body, msg_size);
    }
    else
    {
        WinksProgress_s Progress;

        Progress.Total = pGetCC->Total;
        Progress.Finished = pGetCC->Curr;
        switch(pGetCC->CCDownType)
        {
        #ifdef WINKS_DEBUG
        case WINKS_CCDOWN_TYPE_PERIOD:
            break;
        case WINKS_CCDOWN_TYPE_CCCACHE:
            if(Finished)
            {
                Winks_EngCCCacheOk();
            }
            break;
        case WINKS_CCDOWN_TYPE_RECRUIT:
            break;
        #endif
        case WINKS_CCDOWN_TYPE_FRIEND:
            if(Finished)
                msg = WINKS_MSG_ENG2UI_FRIEND_OK;
            else
                msg = WINKS_MSG_ENG2UI_FRIEND;
            break;
        case WINKS_CCDOWN_TYPE_INIT:
            if(Finished)
            {
                msg = WINKS_MSG_ENG2UI_INIT_OK;
                Winks_EngInitOk();
            }
            else
                msg = WINKS_MSG_ENG2UI_INIT;
            break;
        case WINKS_CCDOWN_TYPE_CALL:
            if(pGetCC->Step == GETCC_STEP_GETCCINFO)
            {
                msg = WINKS_MSG_ENG2UI_CALL;
            }
            else if(Finished)
            {
                Winks_EngUpdateCCShow();
                msg = WINKS_MSG_ENG2UI_CALL_OK;
            }
            break;
        }
        if(msg)
            iRet = Winks_EngPostMsgToUI(pGetCC->Pri,pGetCC->AppType, msg, (void *)&Progress, sizeof(WinksProgress_s));
    }

    if((msg < 0) || Finished)
    {
        iRet = winks_GetCCStop(appHnd, pGetCC);
        if(pGetCC->CCDownType == WINKS_CCDOWN_TYPE_CALL)
        {
            if(msg < 0)
                Winks_EngUpdateCCShowFail();
            else
                Winks_EngInitSpecialCC(WINKS_ENGINE_APP_PRI_NORMAL);
        }
    }

    return iRet;
}

int Winks_GetCCProc(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData)
{
    int     iRet = 0;
    WinksCCDownCtxt_s * pGetCC = (WinksCCDownCtxt_s *)addData;
    
    Winks_assert(pGetCC);
    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        {
        if( pGetCC->pPNoRoot == NULL ||
            pGetCC->pPNoRoot->str == NULL )
        {
            Winks_EngPostMsgToUI(pGetCC->Pri,pGetCC->AppType, WINKS_MSG_ENG2UI_APP_ERR, NULL, 0);
            return WINKS_RETURN_FAILURE;
        }
        pGetCC->Step = GETCC_STEP_GETCCINFO;
        pGetCC->pPNoCurr = pGetCC->pPNoRoot;
        pGetCC->Curr = 0;
        if((iRet = winks_GetCCStepSend(pGetCC)) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pGetCC->Pri,pGetCC->AppType, iRet, NULL, 0);
            return winks_GetCCStop(appHnd, pGetCC);
        }
        }
        break;
    case WINKS_MSG_ENGINE_APP_STOP  :
        if(pGetCC)
            Winks_EngCCDownCtxtDestory(pGetCC);
        break;
    case WINKS_MSG_ENGINE_APP_PAUSE :
        
        break;
    case WINKS_MSG_ENGINE_APP_RESUME:
        {
        if( pGetCC->pPNoRoot == NULL ||
            pGetCC->pPNoCurr == NULL ||
            pGetCC->pPNoCurr->str == NULL )
        {
            Winks_EngPostMsgToUI(pGetCC->Pri,pGetCC->AppType, WINKS_MSG_ENG2UI_APP_ERR, NULL, 0);
            return WINKS_RETURN_FAILURE;
        }
        pGetCC->Step = GETCC_STEP_GETCCINFO;
        if(pGetCC->mediaurl)
        {
            Winks_salFree(pGetCC->mediaurl);
            pGetCC->mediaurl = NULL;
        }
        if(pGetCC->thumburl)
        {
            Winks_salFree(pGetCC->thumburl);
            pGetCC->thumburl = NULL;
        }
        if((iRet = winks_GetCCStepSend(pGetCC)) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pGetCC->Pri,pGetCC->AppType, iRet, NULL, 0);
            return winks_GetCCStop(appHnd, pGetCC);
        }
        }
        break;
    case WINKS_MSG_ENGINE_RETMSG    :
        winks_GetCCStepRcv(appHnd, pGetCC, (WinksEng2AppMsg_s *)msgData);
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_GetCCStart(int Pri, WinksCCDownType_e CCDownType, char *pNo)
{
    WINKS_ENAPP_HND appHnd = WINKS_ENGINE_APP_HANDLER_NULL;
    WinksCCDownCtxt_s * pGetCC = NULL;
    int     AppType = -1;

    if( Pri < WINKS_ENGINE_APP_PRI_NORMAL || 
        Pri > WINKS_ENGINE_APP_PRI_CALL )
        return WINKS_RETURN_FAILURE;
    
    switch(CCDownType)
    {
    case WINKS_CCDOWN_TYPE_PERIOD:
        AppType = WINKS_ENGPROTYPE_REGULARLY_UPDATE;
        break;
    case WINKS_CCDOWN_TYPE_CCCACHE:
        AppType = WINKS_ENGPROTYPE_REGULARLY_UPDATE;
        break;
    case WINKS_CCDOWN_TYPE_RECRUIT:
        AppType = WINKS_ENGPROTYPE_HUNGUP_UPDATE;
        break;
    case WINKS_CCDOWN_TYPE_FRIEND:
        AppType = WINKS_ENGPROTYPE_UPDATE_FRIENDS;
        break;
    case WINKS_CCDOWN_TYPE_INIT:
        if(Pri == WINKS_ENGINE_APP_PRI_NORMAL)
            AppType = WINKS_ENGPROTYPE_REGULARLY_UPDATE;
        else
            AppType = WINKS_ENGPROTYPE_INIT;
        break;
    case WINKS_CCDOWN_TYPE_CALL:
        AppType = WINKS_ENGPROTYPE_CALL_UPDATE;
        break;
    case WINKS_CCDOWN_TYPE_NUMBER:
        AppType = WINKS_ENGPROTYPE_UPDATE_FRIENDS;
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_EngCCDownCtxtCreate(CCDownType, pNo, &pGetCC) != 0)
    {
        if(pGetCC)
            Winks_EngCCDownCtxtDestory(pGetCC);
        return WINKS_RETURN_FAILURE;
    }
    
    if( pGetCC == NULL || 
        pGetCC->pPNoRoot == NULL ||
        pGetCC->pPNoRoot->str == NULL )
    {
        Winks_EngPostMsgToUI(Pri,AppType, WINKS_MSG_ENG2UI_NO_NUMBER, NULL, 0);
        return WINKS_RETURN_SUCCESS;
    }
    
    pGetCC->AppType = AppType;
    pGetCC->CCDownType = CCDownType;
    pGetCC->Pri = Pri;
    switch(Pri)
    {
    case WINKS_ENGINE_APP_PRI_NORMAL:
        pGetCC->AppName = ENAPP_NAME_BGGETCC;
        break;
    case WINKS_ENGINE_APP_PRI_USER:
        pGetCC->AppName = ENAPP_NAME_FGGETCC;
        break;
    case WINKS_ENGINE_APP_PRI_CALL:
        pGetCC->AppName = ENAPP_NAME_CALLGETCC;
        break;
    }
    appHnd = Winks_EngAppStart(pGetCC->Pri, Winks_GetCCProc, pGetCC->AppName, pGetCC);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_GetCCStop(WinksCCDownType_e CCDownType)
{
    WINKS_ENAPP_HND appHnd = WINKS_ENGINE_APP_HANDLER_NULL;
    WinksCCDownCtxt_s * pGetCC = NULL;
    int  cnt = 0;

    while(cnt<3)
    {
        switch(cnt)
        {
        case 0:
            appHnd = winks_FindEngAppHndByName(ENAPP_NAME_CALLGETCC);
            break;
        case 1:
            appHnd = winks_FindEngAppHndByName(ENAPP_NAME_FGGETCC);
            break;
        case 2:
        default:
            appHnd = winks_FindEngAppHndByName(ENAPP_NAME_BGGETCC);
            break;
        }
        if(appHnd != WINKS_ENGINE_APP_HANDLER_NULL)
        {
            pGetCC = (WinksCCDownCtxt_s *)winks_GetEngAppAddData(appHnd);
            if(pGetCC->CCDownType == CCDownType)
                break;
        }
        cnt++;
    }
    if(cnt == 3)
    {
        return WINKS_RETURN_FAILURE;
    }

	return winks_GetCCStop(appHnd, pGetCC);
}


