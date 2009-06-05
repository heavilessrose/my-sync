#if 1
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_enfnc.h"
#endif

#define ENAPP_NAME_RECOMMEND  "recommend"

typedef enum tag_RecommendStep_e
{
    RECOMMEND_STEP_RECOMMENDSERVICE = 0,
    RECOMMEND_STEP_MAX
}RecommendStep_e;

typedef struct tag_WinksEnappRecommend_s
{
    int     Step;   /* 上传Recommend步骤,RecommendStep_e */
    int     Pri;    /* 应用优先级 */
    WinksProt_phoneBook_s phoneBook;
    Winks_MemPool_s * pMemPool;
}WinksEnappRecommend_s;

static int winks_RecommendStop(WINKS_ENAPP_HND appHnd, WinksEnappRecommend_s * pRecommend)
{
    if(pRecommend)
    {
        if(pRecommend->pMemPool)
        {
            Winks_EngMemPoolDestory(pRecommend->pMemPool);
        }
        Winks_salFree(pRecommend);
    }
    return  Winks_EngAppStop(appHnd);
}

static int winks_RecommendStepSend(WINKS_ENAPP_HND appHnd, WinksEnappRecommend_s * pRecommend)
{
    int iRet = WINKS_RETURN_FAILURE;

    Winks_assert(pRecommend);
    Winks_printf("winks_RecommendStepSend Step=%d\r\n", pRecommend->Step);
    switch(pRecommend->Step)
    {
    case RECOMMEND_STEP_RECOMMENDSERVICE:
        iRet = Winks_EngProtReq( WINKS_PROTOCOL_RECOMMENDSERVICE, 
                                 &pRecommend->phoneBook, sizeof(pRecommend->phoneBook) );
        break;
    default:
        Winks_fail("winks_RecommendStepSend");
        break;
    }

    return iRet;
}

static int winks_RecommendStepRcv( WINKS_ENAPP_HND appHnd,
                                  WinksEnappRecommend_s * pRecommend, 
                                  WinksEng2AppMsg_s * pAppMsg )
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     msg = 0;
    void  * msg_body = NULL;
    int     msg_size = 0;
    WinksProt_result_s *presult = NULL;
    int     Finished = 0;

    Winks_assert(pRecommend);
    
    if( pAppMsg == NULL)
        msg = WINKS_MSG_ENG2UI_APP_ERR;
    else if( pAppMsg->AppMsg < WINKS_PROT_RET_TYPE_result )
    {
        msg = pAppMsg->AppMsg;
    }

    if(msg)
    {
        Winks_EngPostMsgToUI(pRecommend->Pri,WINKS_ENGPROTYPE_RECOMMEND, msg, NULL, 0);
        return  winks_RecommendStop(appHnd, pRecommend);
    }
    
    if( pAppMsg->AppMsg == WINKS_PROT_RET_TYPE_result )
    {
        presult = (WinksProt_result_s *)pAppMsg->pData;
    }
    
    if(pRecommend->Step == RECOMMEND_STEP_RECOMMENDSERVICE)
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
        iRet = Winks_EngPostMsgToUI(pRecommend->Pri, WINKS_ENGPROTYPE_RECOMMEND, msg, msg_body, msg_size);
    }
    else
    {
        //WinksProgress_s Progress;

        //Winks_mem_set(&Progress, 0, sizeof(WinksProgress_s));
        if(Finished)
            msg = WINKS_MSG_ENG2UI_RECOMMEND_OK;
        else
        {
         //   Progress.Finished = 0;
         //   Progress.Total = 1;
            msg = WINKS_MSG_ENG2UI_RECOMMEND;
        }
        if(msg)
            iRet = Winks_EngPostMsgToUI(pRecommend->Pri,WINKS_ENGPROTYPE_RECOMMEND, msg, NULL/*(void *)&Progress*/, 0/*sizeof(WinksProgress_s)*/);
    }

    if((msg < 0) || Finished)
    {
        iRet = winks_RecommendStop(appHnd, pRecommend);
    }

    return iRet;
}

int Winks_RecommendProc(WINKS_ENAPP_HND appHnd, unsigned long msg, void *msgData, unsigned long msgDataSize, void *addData)
{
    int iRet = 0;
    WinksEnappRecommend_s * pRecommend = (WinksEnappRecommend_s *)addData;
    Winks_assert(pRecommend);
    
    switch(msg)
    {
    case WINKS_MSG_ENGINE_APP_START :
        Winks_assert(pRecommend->Step == RECOMMEND_STEP_RECOMMENDSERVICE);
        if((iRet = winks_RecommendStepSend(appHnd, pRecommend)) != WINKS_RETURN_SUCCESS)
        {
            Winks_EngPostMsgToUI(pRecommend->Pri,WINKS_ENGPROTYPE_RECOMMEND, iRet, NULL, 0);
            return winks_RecommendStop(appHnd, pRecommend);
        }
        break;
    case WINKS_MSG_ENGINE_APP_STOP  :
        if(pRecommend)
        {
            if(pRecommend->pMemPool)
            {
                Winks_EngMemPoolDestory(pRecommend->pMemPool);
            }
            Winks_salFree(pRecommend);
        }
        break;
    case WINKS_MSG_ENGINE_APP_PAUSE :
        
        break;
    case WINKS_MSG_ENGINE_APP_RESUME:
        
        break;
    case WINKS_MSG_ENGINE_RETMSG    :
        Winks_assert(pRecommend->Step < RECOMMEND_STEP_MAX);
        iRet = winks_RecommendStepRcv(appHnd, pRecommend, (WinksEng2AppMsg_s *)msgData);
        break;
    default:
        return WINKS_RETURN_FAILURE;
    }

    return iRet;
}

/* 输入推荐号码的数组RcmdArr，号码个数为RcmdNum，
   RcmdGreet--邀请问候语 */
int Winks_UserRecommend(int RcmdNum, Winks_RecommendInfo_s * RcmdArr, 
                                  unsigned char * RcmdGreet)
{
    WINKS_ENAPP_HND appHnd;
    WinksEnappRecommend_s * pRecommend = (WinksEnappRecommend_s *)Winks_salAlloc(sizeof(WinksEnappRecommend_s));
    Winks_MemPool_s * pMemPool = NULL;
    WinksProt_phoneBook_s * pphoneBook = NULL;
    struct Prot_pbi_s * pbi = NULL;
    struct Prot_pbi_s * pbi_p = NULL;
    int cnt, ValidCnt;

    if( RcmdNum==0 || RcmdArr == NULL || 
        pRecommend == NULL )
    {
        if(pRecommend)
            Winks_salFree(pRecommend);
        return WINKS_RETURN_FAILURE;
    }
    Winks_mem_set(pRecommend, 0, sizeof(WinksEnappRecommend_s));

    if(Winks_EngMemPoolCreate( &pRecommend->pMemPool ) == NULL)
    {
        Winks_salFree(pRecommend);
        return WINKS_RETURN_FAILURE;
    }
    pMemPool = pRecommend->pMemPool;
    pphoneBook = &pRecommend->phoneBook;
    
    cnt = 0;
    ValidCnt = 0;
    while(cnt<RcmdNum)
    {
        char name_utf[32];
        unsigned short * wStr;
        int len_n = 0, len_p = 0;

        if(RcmdArr[cnt].pno == NULL || RcmdArr[cnt].pno[0] == 0)
        {
            cnt++;
            continue;
        }

        wStr = (unsigned short *)RcmdArr[cnt].name;
        if(wStr)
        {
            Winks_mem_set(name_utf, 0, sizeof(name_utf));
            if(Winks_Unicode2UtfString((char *)wStr, name_utf, sizeof(name_utf)) > 0)
            {
                len_n = strlen(name_utf)+1;
            }
        }
        len_p = strlen(RcmdArr[cnt].pno)+1;

        pbi = Winks_EngMemPoolAlloc(pMemPool, sizeof(struct Prot_pbi_s) + len_n + len_p, 4);
        if(pbi == NULL)
            break;
        if(pRecommend->phoneBook.pbi == NULL)
            pRecommend->phoneBook.pbi = pbi;

        pbi->p = (char *)(pbi+1);
        strncpy(pbi->p, RcmdArr[cnt].pno, len_p);
        if(len_n)
        {
            pbi->n = pbi->p + len_p;
            strncpy(pbi->n, name_utf, len_n);
        }
        if(pbi_p)
            pbi_p->pnext = pbi;
        pbi_p = pbi;
        ValidCnt++;
        cnt++;
    }
    pRecommend->phoneBook.pbi_num = cnt;
    
    pRecommend->Step = RECOMMEND_STEP_RECOMMENDSERVICE;
    pRecommend->Pri = WINKS_ENGINE_APP_PRI_USER;
    appHnd = Winks_EngAppStart(pRecommend->Pri, Winks_RecommendProc, ENAPP_NAME_RECOMMEND, pRecommend);
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        if(pRecommend)
        {
            if(pRecommend->pMemPool)
            {
                Winks_EngMemPoolDestory(pRecommend->pMemPool);
            }
            Winks_salFree(pRecommend);
        }
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_UserRecommendStop(void)
{
    WINKS_ENAPP_HND appHnd = winks_FindEngAppHndByName(ENAPP_NAME_RECOMMEND);
    WinksEnappRecommend_s * pRecommend = NULL;
    
    if(appHnd == WINKS_ENGINE_APP_HANDLER_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    
    if(Winks_EngAppStop(appHnd) != WINKS_RETURN_SUCCESS)
    {
        return WINKS_RETURN_FAILURE;
    }

    pRecommend = (WinksEnappRecommend_s *)winks_GetEngAppAddData(appHnd);
	return winks_RecommendStop(appHnd, pRecommend);
}



