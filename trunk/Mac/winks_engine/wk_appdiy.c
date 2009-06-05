#if 1
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_enfnc.h"
#endif

#define ENAPP_NAME_DIY  "diy"

typedef enum tag_DiyStep_e
{
    DIY_STEP_UPLOADWINKS = 0,
    DIY_STEP_CREATEWINKSSCRIPT,
    DIY_STEP_MAX
}DiyStep_e;

typedef struct tag_WinksEnappDiy_s
{
    int     Step;   /* 上传Diy步骤,DiyStep_e */
    int     Pri;      /* 应用优先级 */
    int     action;  /* 0 - 只上传不定制; 1 - 上传并定制 */
    char    wid[WINKS_PROT_WID_MAX];
    unsigned char path[WINKS_PATH_MAX]; /* 文件路径 (unicode编码) */
    char greet[302];           /* 问候语 (utf-8格式) */
    char name[96];             /* 文件名 (utf-8格式) */
    char ext[8];               /* 扩展名 */
    WinksProt_UploadWinks_s UploadWinks;
    WinksProt_winksScript_s winksScript;
}WinksEnappDiy_s;

static int winks_DiyStop(WINKS_ENAPP_HND appHnd, WinksEnappDiy_s * pDiy)
{
    if(pDiy)
    {
        if(pDiy->winksScript.IsForAll==0 && pDiy->winksScript.pno)
        {
            Winks_salFree(pDiy->winksScript.pno);
        }
        Winks_salFree(pDiy);
    }
    return  Winks_EngAppStop(appHnd);
}

static int winks_DiyStepSend(WINKS_ENAPP_HND appHnd, WinksEnappDiy_s * pDiy)
{
    int iRet = WINKS_RETURN_FAILURE;

    Winks_assert(pDiy);
    Winks_printf("winks_DiyStepSend Step=%d\r\n", pDiy->Step);
    switch(pDiy->Step)
    {
    case DIY_STEP_UPLOADWINKS:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_UPLOADWINKS, 
                                 &pDiy->UploadWinks, sizeof(pDiy->UploadWinks) );
        break;
    case DIY_STEP_CREATEWINKSSCRIPT:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_CREATEWINKSSCRIPT, 
                                 &pDiy->winksScript, sizeof(pDiy->winksScript) );
        break;
    default:
        Winks_fail("winks_DiyStepSend");
        break;
    }

    return iRet;
}

static int winks_DiyStepRcv( WINKS_ENAPP_HND appHnd,
                                  WinksEnappDiy_s * pDiy, 
                                  WinksEng2AppMsg_s * pAppMsg )
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     msg = 0;
    void  * msg_body = NULL;
    int     msg_size = 0;
    WinksProt_result_s *presult = NULL;
    int     Finished = 0;

    Winks_assert(pDiy);

    /* 检测错误 */
    if( pAppMsg == NULL)
    {
        msg = WINKS_MSG_ENG2UI_APP_ERR;
    }
    else if( pAppMsg->AppMsg < WINKS_PROT_RET_TYPE_result )
    {
        msg = pAppMsg->AppMsg;
    }

    /* 给UI发错误信息 */
    if(msg)
    {
        Winks_EngPostMsgToUI(pDiy->Pri, WINKS_ENGPROTYPE_DIY, msg, NULL, 0);
        return  winks_DiyStop(appHnd, pDiy);
    }
    
    if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result )
    {
        presult = (WinksProt_result_s *)pAppMsg->pData;
    }

    if(pDiy->Step == DIY_STEP_UPLOADWINKS)
    {
        if(pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_UserWinksType)/* 返回彩像ID */
        {
            WinksProt_UserWinksType_s * pUserWinksType = 
                (WinksProt_UserWinksType_s *)pAppMsg->pData;
            
            if( pUserWinksType && 
                pUserWinksType->wid && 
                strlen(pUserWinksType->wid) < sizeof(pDiy->wid) )
                strncpy(pDiy->wid, pUserWinksType->wid, sizeof(pDiy->wid));
            else
                msg = WINKS_MSG_ENG2UI_SRV_ERRDATA;

                if(msg == 0)
                {
                    /* 如果有后续的定制操作, 且上传彩像没问题, 上发定制信息 */
                    if(pDiy->action == 1)
                    {
                        pDiy->winksScript.wid = pDiy->wid;
                        pDiy->Step = DIY_STEP_CREATEWINKSSCRIPT;
                        if((iRet = winks_DiyStepSend(appHnd, pDiy)) != WINKS_RETURN_SUCCESS)
                        {
                            msg = iRet;
                        }
                    }
                    else
                    {
                        msg = WINKS_MSG_ENG2UI_DIY_OK;
                        msg_body = pDiy->wid;
                        msg_size = strlen(pDiy->wid)+1;
                        Finished = 1;
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
    else
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

    if(msg)
    {
        iRet = Winks_EngPostMsgToUI(pDiy->Pri, WINKS_ENGPROTYPE_DIY, msg, msg_body, msg_size);
    }
    else
    {
        WinksProgress_s Progress;

        Winks_mem_set(&Progress, 0, sizeof(WinksProgress_s));
        if(Finished)
            msg = WINKS_MSG_ENG2UI_SCRIPT_OK;
        else
        {
            if(pDiy->Step == DIY_STEP_CREATEWINKSSCRIPT)
            {
                msg = WINKS_MSG_ENG2UI_DIY_OK;
                msg_body = pDiy->wid;
                msg_size = strlen(pDiy->wid)+1;
            }
            else
            {
                Progress.Finished = 0;
                Progress.Total = 1;
                msg = WINKS_MSG_ENG2UI_DIY;
                msg_body = (void *)&Progress;
                msg_size = sizeof(WinksProgress_s);
            }
        }
        if(msg)
            iRet = Winks_EngPostMsgToUI(pDiy->Pri, WINKS_ENGPROTYPE_DIY, msg, msg_body, msg_size);
    }

    if((msg < 0) || Finished)
    {
        iRet = winks_DiyStop(appHnd, pDiy);
    }

    return iRet;
}

int Winks_DiyProc(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData)
{
    int iRet = 0;
    WinksEnappDiy_s * pDiy = (WinksEnappDiy_s *)addData;
    Winks_assert(pDiy);
    
    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        Winks_assert(pDiy->Step == DIY_STEP_UPLOADWINKS);
        if((iRet = winks_DiyStepSend(appHnd, pDiy)) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pDiy->Pri, WINKS_ENGPROTYPE_DIY, iRet, NULL, 0);
            return winks_DiyStop(appHnd, pDiy);
        }
        break;
    case WINKS_MSG_ENGINE_APP_STOP  :
        if(pDiy)
        {
            if(pDiy->winksScript.IsForAll==0 && pDiy->winksScript.pno)
            {
                Winks_salFree(pDiy->winksScript.pno);
            }
            Winks_salFree(pDiy);
        }
        break;
    case WINKS_MSG_ENGINE_APP_PAUSE :
        
        break;
    case WINKS_MSG_ENGINE_APP_RESUME:
        
        break;
    case WINKS_MSG_ENGINE_RETMSG    :
        Winks_assert(pDiy->Step < DIY_STEP_MAX);
        iRet = winks_DiyStepRcv(appHnd, pDiy, (WinksEng2AppMsg_s *)msgData);
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    return iRet;
}

/*用户上传DIY及定制给好友
  diy_info->action: 0 - 只上传不定制; 1 - 上传并定制
  diy_info->pnolist: 在action为1时使用，定制好友的手机号码串，以|分隔，以|结尾；为NULL表示所有人 
  diy_info->name: 用户设置的彩像名称，unicode编码，前2字节为0则取文件名
  diy_info->greet字符串为空(Unicode前2字节为0) : 不设置问候语
  diy_info->path字符串为空(Unicode前2字节为0) : 只修改问候语
*/
int Winks_UserUploadDiyImage(Winks_DiyInfo_s *diy_info)
{
    WINKS_ENAPP_HND appHnd;
    WinksEnappDiy_s * pDiy = (WinksEnappDiy_s *)Winks_salAlloc(sizeof(WinksEnappDiy_s));
    int     PathLen, off;
    unsigned short *pwStr = NULL;
    unsigned short ExtFlag = '.';
    unsigned short PathFlag = WINKS_PATH_SPACE[0];
    unsigned int   FileSize;   /* input 文件大小 */
    unsigned long  MaxFileSize;

    if(pDiy == NULL)
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(pDiy, 0, sizeof(WinksEnappDiy_s));

    /* 整理输入参数UploadWinks */
    PathLen  = WK_STRLEN((char *)diy_info->path);
    if(PathLen == 0)
    {
        Winks_salFree(pDiy);
        return WINKS_RETURN_FAILURE;
    }
    if( Winks_GetSettingValue(WINKS_SETTING_DIY_FILE_MAX_SIZE, &MaxFileSize) != 0 ||
        Winks_fgetsize((char *)diy_info->path, 0, &FileSize) != 0 || 
        (MaxFileSize && FileSize>MaxFileSize) )
    {
        Winks_salFree(pDiy);
        return WINKS_RETURN_FAILURE;
    }

    /* 确定此次DIY的动作 */
    Winks_assert(diy_info->action == 0 || diy_info->action == 1);
    pDiy->action = diy_info->action;

    pDiy->UploadWinks.attachNameLen = (PathLen+1)<<1;
    Winks_mem_cpy(pDiy->path, diy_info->path, pDiy->UploadWinks.attachNameLen);
    pDiy->UploadWinks.attachName = pDiy->path;
    pDiy->UploadWinks.FileSize = FileSize;
    pwStr = (unsigned short *)pDiy->path;

    /* 查找文件名 */
    if ( (diy_info->name[0] || diy_info->name[1]) && (Winks_Unicode2UtfString((char *)diy_info->name, pDiy->name, sizeof(pDiy->name)) > 0) )
    {
        pDiy->UploadWinks.name = pDiy->name;
    }
    else
    {
        off = PathLen-1;
        while(off>=0)
        {
            if(pwStr[off] == PathFlag)
                break;
            off--;
        }
        if(off)
        {
            char *pstr = (char *)&pwStr[off+1];
            if(Winks_Unicode2UtfString(pstr, pDiy->name, sizeof(pDiy->name)) > 0)
            {
                pDiy->UploadWinks.name = pDiy->name;
            }
        }
    }

    /* 查找后缀名 */
    off = PathLen-1;
    while(off>=0)
    {
        if(pwStr[off] == ExtFlag)
            break;
        off--;
    }
    if(off)
    {
        char *pstr = (char *)&pwStr[off];
        if(Winks_Unicode2AsciiString(pstr, pDiy->ext, sizeof(pDiy->ext)) > 0)
            pDiy->UploadWinks.ext = pDiy->ext;
        if(pDiy->ext)
        {
            pDiy->UploadWinks.pContenttype = Winks_Ext2ContType(pDiy->ext);
        }
    }

    /* 整理输入参数winksScript (wid在第1步返回) */
    if(WK_STRLEN((char *)diy_info->greet))
    {
        if(Winks_Unicode2UtfString((char *)diy_info->greet, pDiy->greet, sizeof(pDiy->greet)) > 0)
        {
            pDiy->winksScript.greeting = pDiy->greet;
        }
    }

    if(diy_info->action == 1)  /* 上传并定制 */
    {
        if(diy_info->pnolist == NULL)   /* 定制给所有人 */
        {
            pDiy->winksScript.IsForAll = 1;
            pDiy->winksScript.pno = NULL;
        }
        else
        {
            int cnt = 0, len, len1;
            char * pbuf = NULL;
            char * pno = diy_info->pnolist;
            struct Prot_pno_s * pnol = NULL;

            Winks_assert(diy_info->pnolist[strlen(diy_info->pnolist) - 1] == '|');

            pDiy->winksScript.IsForAll = 0;

            /* FIXME: 号码中会不会有+ 号 */ 
            while(pno && *pno)
            {
                if(*pno == '|')
                    cnt++;
                else if(*pno < '0' || *pno > '9')
                    break;

                pno++;
            }

            len = pno-diy_info->pnolist + 1;
            len1 = cnt*sizeof(struct Prot_pno_s);
            pbuf = (char *)Winks_salAlloc(len + len1);
            if(pbuf == NULL)
            {
                Winks_salFree(pDiy);
                return WINKS_RETURN_FAILURE;
            }
        
            Winks_mem_set(pbuf, 0, len+len1);
            pnol = (struct Prot_pno_s *)pbuf;
            pDiy->winksScript.pno = pnol;
            strncpy(&pbuf[len1], diy_info->pnolist, len);
            pno = &pbuf[len1];
            pnol->p = pno;
            while(cnt>0 && *pno)
            {
                if(*pno == '|')
                {
                    *pno = '\0';
                    pnol->pnext = pnol+1;
                    pnol = pnol->pnext;
                    pnol->p = pno+1;
                    cnt--;
                }
                else if(*pno < '0' || *pno > '9')
                {
                    *pno = '\0';
                    if(strlen(pnol->p) == 0)
                        pnol->p = NULL;
                    break;
                }
                pno++;
            }
        }
    }
    
    pDiy->Step = DIY_STEP_UPLOADWINKS;
    pDiy->Pri = WINKS_ENGINE_APP_PRI_USER;
    appHnd = Winks_EngAppStart(WINKS_ENGINE_APP_PRI_USER, Winks_DiyProc, ENAPP_NAME_DIY, pDiy);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        if(pDiy->winksScript.IsForAll==0 && pDiy->winksScript.pno)
        {
            Winks_salFree(pDiy->winksScript.pno);
        }
        Winks_salFree(pDiy);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_UserUploadDiyStop(void)
{
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_DIY);
    WinksEnappDiy_s * pDiy = NULL;
    
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    
    pDiy = (WinksEnappDiy_s *)winks_GetEngAppAddData(appHnd);
	return winks_DiyStop(appHnd, pDiy);
}



