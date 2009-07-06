/*
File:       wk_osfnc.c
Author:     Huaxm
Create Time:21/08/2008
*/

#include "wk_public.h"
#include "wk_osfnc.h"
#include "wk_setting.h"
//#define WK_MODULE_HIGH_OSAL      5


#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_OSAL_CONTEXT_DEF(osPrt)  \
    winks_OsalContext_s *osPtr = get_module_glob(WK_MODULE_HIGH_OSAL);
#else
#define WINKS_OSAL_CONTEXT_DEF(osPrt)  \
    winks_OsalContext_s *osPtr = &Winks_osalContext;
#endif


#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
static winks_OsalContext_s Winks_osalContext;
#endif

#ifdef HUAXMTEST
static    winks_OsalContext_s *osPtr = NULL;
#endif

static int wk_osal_channel(unsigned long msg, void *data, unsigned long size);
extern int winks_api_osinit(void);
extern int winks_api_osdestroy(void);
extern int winks_api_PostMessage(void);
extern int Winks_api_SelectFile( char* defpath, int type );
extern int Winks_api_SelectDataAccount(int type, unsigned long dataAccount);
extern int Winks_api_SelectPhoneNumber( void );
extern int Winks_api_StartTimer(unsigned long delay);
extern int Winks_api_StopTimer(void);
extern int Winks_api_GetImsi(void);
extern int Winks_api_GetImei(void);
#ifdef WINKS_DUAL_SIM
extern int Winks_api_GetImsi_2(void);
#endif /* WINKS_DUAL_SIM */
extern int Winks_GetTimeDifference(Winks_DateTime_s *t1, Winks_DateTime_s *t2, Winks_DateTime_s *result);

#ifdef WINKS_DUAL_SIM
/*短息发送。
 * 调用平台的短信发送机制。
 * function: Winks_api_SendSmsUni
 * parameters:
 *   void *number: 短信接受者、网关。(ascii 以'\0'结尾)
 *   char *dataUni: 短信内容。(Unicode 以((unsigned short)0)结尾)
 *   int simNo: 用于双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_api_SendSmsUni(char *number, char * buff, int SimNo);
#else
int Winks_api_SendSmsUni(char *number, char * buff);
#endif
int Winks_IsOsalEnable(void)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(osPtr && (osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING))
        return 1;
    return 0;
}

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
/*取setting的指针，用于存储模块的调用*/
void *winks_GetSettingData(void)
{
    WINKS_CHN_ID chn = WINKS_CHN_NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    return osPtr->setting;
}

/*存setting的指针，用于存储模块的调用*/
int winks_SetSettingData(void* setting_p)
{
    WINKS_CHN_ID chn = WINKS_CHN_NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);

    osPtr->setting = setting_p;
    return WINKS_RETURN_SUCCESS;
}
#endif

//改为由引擎发送消息给UI
//#ifdef WINKS_DYNAMIC_UPDATE
//extern int winks_api_EngInitEndCallBack(int result);
//int Winks_EngInitEnd(int result)
//{
//    winks_api_EngInitEndCallBack(result);
//    return 0;
//}
//#endif

extern int Winks_SettingInit(void);
extern int Winks_SettingDestroy(void);
/*注册抽象层OSAL Handler*/
int Winks_CreateOsalHandler(void)
{
    winks_OsalContext_s *osPtr = NULL;


    /*调用平台的初始化。可以是资源的占用、系统事件消息的注册等*/
    winks_api_osinit();

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    osPtr = get_module_glob(WK_MODULE_HIGH_OSAL);
#else
    osPtr = &Winks_osalContext;
#endif

#ifdef WINKS_PB_INTERCEPTION
	{
    int i=0;
    for(i= 0; i < WINKS_MAX_NUM_OF_PB_MODIFIED_TMP; ++i)
    {
        osPtr->pbModifiedTmp[i].index = ((unsigned short)(0xffff));
    }
	}
#endif

    {/*释放短信队列申请的资源。*/
    Winks_SmsQueueItem_s *p = NULL;
    while(osPtr->smsQueueHead != NULL)
    {
        p = osPtr->smsQueueHead;
        osPtr->smsQueueHead = osPtr->smsQueueHead->next;
        Winks_salFree(p);
    }
    }

#if 0 /*HUAPORTING*/ /*Move in function winks_api_osinit*/
    /*Create all useful mutexes.*/
#ifdef WINKS_MULTITHREADING
    Winks_api_CreateMutex();
#endif 
#endif

    osPtr->osalHandlerStart = WINKS_OSALHANDER_RUNNING;

    osPtr->osalChn = Winks_CreateChn(wk_osal_channel);
    Winks_assert(osPtr->osalChn != WINKS_CHN_NULL);

    /*初始化设置模块*/
    Winks_SettingInit();

#ifdef WINKS_DEBUG
    /*测试功能开关*/
    Winks_SetOsalTestBitFlag(2, 1);/*debug打印开关*/
    //Winks_SetOsalBitFlag(3, 1);/*按键打印信息开关*/
    //Winks_SetBackgroundFlag(1);/*大黑板*/
#endif
    return WINKS_RETURN_SUCCESS;
}

/*注销抽象层机制*/
int Winks_DestroyOsalHandler(void)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    {   /*释放消息包*/
        winks_MsgBody_s *p ;
        int i = 0;
        p = osPtr->osalMsgRingQueue.queue;
        for(i=0; i<WINKS_OSAL_MSG_MAXNUM; ++i )
        {
            if(p[i].wParam != NULL)
            {
#ifndef WINKS_MSG_WITHOUT_DATA
                if(p[i].wParam != p[i].sData)
#endif
                {
                    Winks_salFree(p[i].wParam);
                    p[i].wParam = NULL;
                }
            }
        }
    }

    {   /*释放Imsi等待队列*/
        winks_MsgWaitingQueue_s *p ;
        winks_MsgWaitingQueue_s *q ;
        p = q = osPtr->waitingImsi_p;
        while(p!= NULL)
        {
            p = p->next;
            Winks_salFree(q);
            q = p;
        }
        osPtr->waitingImsi_p = NULL;
    }

    /*注销设置模块*/
    Winks_SettingDestroy();
#if 0 /*HUAPORTING*/ /*Move in function winks_api_osdestroy*/
#ifdef WINKS_MULTITHREADING
    Winks_api_DestroyMutex();
#endif
#endif

    winks_api_osdestroy();

    return WINKS_RETURN_SUCCESS;

}

/*创建消息通道*/
WINKS_CHN_ID Winks_CreateChn(WINKS_OSAL_CALLBACK fCallBack)
{
    unsigned long i;
    WINKS_CHN_ID chn = WINKS_CHN_NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(fCallBack != NULL);

#ifdef WINKS_MULTITHREADING
    Winks_api_GetMutex(WINKS_MUTEX_CHN);
#endif

    for(i = 1L; i<WINKS_OSAL_CHN_MAXNUM; ++i)
    {
        if(osPtr->osalCnhList[i] == NULL) break;
    }
    if(i >= WINKS_OSAL_CHN_MAXNUM)
    {
        Winks_fail("OSAL channel list is filled\n");
        return 0L;
    }
    chn  = (WINKS_CHN_ID)i;
    osPtr->osalCnhList[i] = fCallBack;

#ifdef WINKS_MULTITHREADING
    Winks_api_PutMutex(WINKS_MUTEX_CHN);
#endif

    Winks_PostMsg(chn, WINKS_MSG_CHN_INIT, &chn, sizeof(chn));
    /*(*fCallBack)(WINKS_MSG_CHN_INIT, NULL, 0);*/

    return chn;
}


/*注销消息通道*/
int Winks_DestroyChn(WINKS_CHN_ID chn)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(chn != WINKS_CHN_NULL);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);

    /*注销通道接收系统消息*/
    //Winks_SetWaitingEvent(chn, WINKS_EVENT_NULL);

    /*注销通道返回函数*/
    osPtr->osalCnhList[chn] = NULL;

    return WINKS_RETURN_SUCCESS;
}

#ifdef WINKS_MSG_UNION
#define WINKS_HTTP_DATAIN	4   //数据到来事件	4	
#define WINKS_HTTP_DATAOUT  5   //数据发送事件  5
/* socket message structure */
typedef struct tag_Winks_Socketmsg_s
{
	unsigned long wParam;
	unsigned long lParam;
}Winks_Socketmsg_s;

/*消息合并函数，返回值 -1:操作失败， 1:消息被合并， 0:消息未被合并*/
static int winks_MsgUnion(winks_MsgBody_s *msgStruct,WINKS_CHN_ID newchn, unsigned long newmsg, void *newdata, unsigned long newsize)
{
    int opt = 0;
    if((msgStruct == NULL) || (newdata == NULL) || (msgStruct->lParam != newsize))
        return 0;
    
    switch(newmsg)
    {
        case WINKS_MSG_UI_PEN_EVENT:
        {
            Winks_MsgBody_PenEvent_s *np = (Winks_MsgBody_PenEvent_s*)newdata;
            Winks_MsgBody_PenEvent_s *op = (Winks_MsgBody_PenEvent_s*)(msgStruct->wParam);
            if((msgStruct->lParam == newsize) && 
                (np->flag == WINKS_PEN_FLAG_PENMOVE) && 
                (op->flag == np->flag))
                opt = 1;
            break;
        }
        case WINKS_MSG_ENGINE_HTTP:
        {
            Winks_Socketmsg_s *np = (Winks_Socketmsg_s*)newdata;
            Winks_Socketmsg_s *op = (Winks_Socketmsg_s*)(msgStruct->wParam);
            if((msgStruct->lParam == newsize) && (newsize > 0) &&
                (op != NULL) && (np != NULL) && (op->wParam == np->wParam))
            {
                if((np->wParam>>24)==WINKS_HTTP_DATAIN 
                    || (np->wParam>>24)==WINKS_HTTP_DATAOUT)
                opt = 1;
            }
            break;
        }
        default:
            break;
    }
    
    if((opt == 1) && (msgStruct->lParam == newsize))
    {
        Winks_mem_cpy(msgStruct->wParam, newdata, newsize);
        return 1;
    }
    else
        return 0;
}
#endif

/*向消息通道发送异步消息*/
int Winks_PostMsg(WINKS_CHN_ID chn, unsigned long msg, void *data, unsigned long size)
{
    unsigned long head;
    void *buff = NULL;
    char flag = 0;
    int iRet  = 0;

#ifdef WINKS_MSG_UNION
    unsigned long prev = 0;
#endif

    winks_MsgRingQueue_s *osalMsgRingQueue_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    osalMsgRingQueue_p = &(osPtr->osalMsgRingQueue);
    
    Winks_assert(chn != WINKS_CHN_NULL);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);
    Winks_assert(msg != 0);
    Winks_assert(size <= WINKS_MESSAGE_BODY_MAX_SIZE);

#ifdef WINKS_MULTITHREADING
    Winks_api_GetMutex(WINKS_MUTEX_MSG);
#endif
#ifdef WINKS_DEBUG/*多线程重入测试机制*/
    ++(osPtr->msgInQueueCnt);
    if(osPtr->msgInQueueCnt>1)
    {
        Winks_printf("--------Count[%d]--------\r\n",osPtr->msgInQueueCnt);
        Winks_fail("In msg queue reload.\r\n");
    }
#endif

    /*head = osalMsgRingQueue_p->head+1;
    head %= WINKS_OSAL_MSG_MAXNUM;*/
    head = (osalMsgRingQueue_p->head == WINKS_OSAL_MSG_MAXNUM-1)?0:osalMsgRingQueue_p->head+1;

    flag = (char)((osalMsgRingQueue_p->head == osalMsgRingQueue_p->tail)? 1: 0);
#ifdef WINKS_MSG_UNION
    /*做消息合并*/
    prev = (osalMsgRingQueue_p->head==0)?WINKS_OSAL_MSG_MAXNUM-1:osalMsgRingQueue_p->head-1;
    if(!flag && (osalMsgRingQueue_p->queue[prev].chn == chn) &&
        (osalMsgRingQueue_p->queue[prev].msg == msg))
    {
        iRet = winks_MsgUnion(&(osalMsgRingQueue_p->queue[prev]), chn, msg, data, size);
        if(iRet == 1)/*新消息通过消息合并被消化*/
        {
        #ifdef WINKS_DEBUG/*多线程重入测试机制*/
            --(osPtr->msgInQueueCnt);
        #endif
        #ifdef WINKS_MULTITHREADING
            Winks_api_PutMutex(WINKS_MUTEX_MSG);
        #endif
           return WINKS_RETURN_SUCCESS;
        }
    }
#endif
    if(head == osalMsgRingQueue_p->tail)
    {
        Winks_fail("Osal message ring Queue is filled!\n");
    #ifdef WINKS_DEBUG/*多线程重入测试机制*/
        --(osPtr->msgInQueueCnt);
    #endif
    #ifdef WINKS_MULTITHREADING
        Winks_api_PutMutex(WINKS_MUTEX_MSG);
    #endif
        return WINKS_RETURN_FAILURE;
    }

    if((data!=NULL) && (size>0))
    {

#ifndef WINKS_MSG_WITHOUT_DATA
        if(size <= WINKS_MAX_LEN_OF_MSG_WITH_DATA)
        {
            buff = ((void*)&osalMsgRingQueue_p->queue[osalMsgRingQueue_p->head].sData[0]);
        }
        else
        {
#endif
        buff = Winks_salAlloc(size);
        if(buff == NULL)
        {
            Winks_fail("Osal message request memory NULL!\n");
        #ifdef WINKS_DEBUG/*多线程重入测试机制*/
            --(osPtr->msgInQueueCnt);
        #endif
        #ifdef WINKS_MULTITHREADING
            Winks_api_PutMutex(WINKS_MUTEX_MSG);
        #endif
            return WINKS_RETURN_FAILURE;
        }
#ifndef WINKS_MSG_WITHOUT_DATA
        }
#endif
        Winks_mem_cpy(buff, data, size);
    }
    osalMsgRingQueue_p->queue[osalMsgRingQueue_p->head].chn = chn;
    osalMsgRingQueue_p->queue[osalMsgRingQueue_p->head].msg = msg;
    osalMsgRingQueue_p->queue[osalMsgRingQueue_p->head].wParam = buff ;
    osalMsgRingQueue_p->queue[osalMsgRingQueue_p->head].lParam = size;

    osalMsgRingQueue_p->head = head;

#ifdef WINKS_DEBUG/*多线程重入测试机制*/
    --(osPtr->msgInQueueCnt);
#endif
#ifdef WINKS_MULTITHREADING
    Winks_api_PutMutex(WINKS_MUTEX_MSG);
#endif

    /*Callback OSAL handler*/
    if((flag == 1) || (!(osPtr->g_post_msg_debug_flag)))
    {
        /*Winks_printf( "Winks_PostMsg called, chn %d, msg %d, data %x, size %d\r\n", chn, msg, data, size );*/
        iRet = winks_api_PostMessage();
        if (iRet != 0)
            Winks_printf("post msg failed.[%d]\n",iRet);
        osPtr->g_post_msg_debug_flag= 1;
    }

    return WINKS_RETURN_SUCCESS;

}


/*OSAL Handler回调函数*/
void winks_osalCallBack( void)
{

    winks_MsgRingQueue_s *osalMsgRingQueue_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    osalMsgRingQueue_p = &(osPtr->osalMsgRingQueue);
    
#ifdef WINKS_DEBUG/*多线程重入测试机制*/
    ++(osPtr->msgOutQueueCnt);
    if(osPtr->msgOutQueueCnt>1)
    {
        Winks_printf("--------Count[%d]--------\r\n",osPtr->msgOutQueueCnt);
        Winks_fail("Out msg queue reload.\r\n");
    }
#endif 

    /*Winks_printf("++-Enter in function winks_osalCallBack()![%d][%d] \n",id,TPM_CALL_DIAL_RESULT);*/
    osPtr->g_post_msg_debug_flag = 0;
    //Winks_printf("got msg:WINKS_PMSG_NEW_MSG\n");
    while(osalMsgRingQueue_p->head != osalMsgRingQueue_p->tail)
    {
        unsigned long tail = osalMsgRingQueue_p->tail;
        winks_MsgBody_s *msg_p =NULL;

        msg_p = &(osalMsgRingQueue_p->queue[tail]);
        (*(osPtr->osalCnhList[msg_p->chn]))(msg_p->msg, msg_p->wParam, msg_p->lParam);
#ifndef WINKS_MSG_WITHOUT_DATA
        if(msg_p->wParam != (void *)&(msg_p->sData[0]))
        {
#endif
            if(msg_p->wParam != NULL)
            {
                Winks_salFree(msg_p->wParam);
            }
#ifndef WINKS_MSG_WITHOUT_DATA
        }
#endif
        msg_p->wParam = NULL;
        ++osalMsgRingQueue_p->tail;
        osalMsgRingQueue_p->tail %= WINKS_OSAL_MSG_MAXNUM;
    }

#ifdef WINKS_DEBUG/*多线程重入测试机制*/
    --(osPtr->msgOutQueueCnt);
#endif
}


static int wk_WaitingTimerExpiry(void *data, unsigned long size)
{
    WINKS_TIMER_ID timerId = WINKS_TIMER_NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(data != NULL);

    timerId = *((WINKS_TIMER_ID *)data);
    if(timerId == osPtr->waitingImsi_T)
    {
        wk_osal_DoWaitingImsi(NULL);
    }
#ifdef WINKS_DUAL_SIM
    else if(timerId == osPtr->waitingImsi_2_T)
    {
        wk_osal_DoWaitingImsi_2(NULL);
    }
#endif /* WINKS_DUAL_SIM */
    return WINKS_RETURN_SUCCESS;
}

static int wk_osal_channel(unsigned long msg, void *data, unsigned long size)
{
    switch (msg)
    {
    case WINKS_MSG_CHN_INIT:
        Winks_api_GetImei();
        Winks_api_GetImsi();
#ifdef WINKS_DUAL_SIM
        Winks_api_GetImsi_2();
#endif
        break;

    case WINKS_MSG_TIMER_EXPIRY:
        Winks_assert(data != NULL);
        wk_WaitingTimerExpiry(data, size);
        break;

    default:
        Winks_printf("CallBack get Other msg[%ld]\n",msg);
        break;
    }
    return WINKS_RETURN_SUCCESS;
}


#if 0
/*Regist ID of engine channel.*/
int Winks_RegisterEngineChn(WINKS_CHN_ID chn)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(chn != WINKS_CHN_NULL);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);

    if(osPtr->engChn != WINKS_CHN_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    else
    {
        osPtr->engChn = chn;
        return WINKS_RETURN_SUCCESS;
    }
}

int Winks_PostMsgToEngine(unsigned long msg, void *data, unsigned long size)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(osPtr->engChn != WINKS_CHN_NULL);
    Winks_assert(osPtr->engChn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[osPtr->engChn] != NULL);
    Winks_assert(msg != 0);
    Winks_assert(size <= WINKS_MESSAGE_BODY_MAX_SIZE);

    return Winks_PostMsg(osPtr->engChn, msg, data, size);
}
#endif

int Winks_SetBackgroundFlag(unsigned long val)
{
    return Winks_SetOsalTestBitFlag(0, val);
}

int Winks_GetBackgroundFlag(unsigned long *val)
{
    return Winks_GetOsalTestBitFlag(0, val);
}


/*置抽象层控制位*/
/* B0 :  Change background display flag */
/* B2 :  debug打印开关 */
/* B3 :  Key event print flag */
/* B4 :  URL controller flag */
/* B5 :  Change every key accept call flag.*/
/* B6 :  usb状态调试监测 */
/* B7 :  usb状态改变 */
/* B8 :  memory card状态改变 */
/* B30:  engine step by step */
/* B31:  engine step by step */
int Winks_SetOsalTestBitFlag(unsigned long bit, unsigned long val)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    Winks_assert(val <= 1);
    Winks_assert(bit < 32);
    Winks_SetBitFlag(bit, val, (void*)&(osPtr->testBitFlag));
    return WINKS_RETURN_SUCCESS;
}

/*读取抽象层控制位*/
/* B0 :  Change background display flag */
/* B2 :  debug打印开关 */
/* B3 :  Key event print flag */
/* B4 :  URL controller flag */
/* B5 :  Change every key accept call flag.*/
/* B6 :  usb状态调试监测 */
/* B7 :  usb状态改变 */
/* B8 :  memory card状态改变 */
/* B30:  engine step by step */
/* B31:  engine step by step */
int Winks_GetOsalTestBitFlag(unsigned long bit, unsigned long *val)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    Winks_assert(val != NULL);
    Winks_assert(bit < 32);
    Winks_GetBitFlag(bit, val, (void*)&(osPtr->testBitFlag));
    return WINKS_RETURN_SUCCESS;
}

/*设置控制位*/
int Winks_SetBitFlag(unsigned long bit, unsigned long val, void *src)
{
    unsigned char *p = (unsigned char*)src;
    unsigned char bitKey = 0xff;
    int index = 0;
    
    Winks_assert(val <= 1);
    Winks_assert(src != NULL);

    index = bit>>3;/*bit/8*/    
    bitKey = (unsigned char)(1<<(bit&0x07));
    
    if(val == 1)/*置位标志*/
        p[index] |= (bitKey);
    else/*清位标志*/
        p[index] &= (~bitKey);
    
    return WINKS_RETURN_SUCCESS;
}

/*读取控制位*/
int Winks_GetBitFlag(unsigned long bit, unsigned long *val, void *src)
{
    unsigned char *p = (unsigned char*)src;
    unsigned char bitKey = 0xff;
    int index = 0;
    
    Winks_assert(val != NULL);
    Winks_assert(src != NULL);
    
    index = bit>>3;/*bit/8*/    
    bitKey = (unsigned char)(1<<(bit&0x07));

    if((p[index] & bitKey) != 0)/*置位标志*/
        *val = 1UL;
    else/*清位标志*/
        *val = 0UL;
    
    return WINKS_RETURN_SUCCESS;
}


/*取抽象层数据结构指针*/
winks_OsalContext_s * winks_GetOsalContextPtr(void)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    return osPtr;
}

/*设置消息通道的等待消息，flag的每一位标志一个系统消息*/
int Winks_SetWaitingEvent(WINKS_CHN_ID chn, unsigned long flag)
{
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
    winks_EventWaitingQueue_s *p = NULL;
    winks_EventWaitingQueue_s *q = NULL;
#endif

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    

    Winks_assert(osPtr->osalHandlerStart== WINKS_OSALHANDER_RUNNING);
    Winks_assert(chn != WINKS_CHN_NULL);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);


#ifndef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
    osPtr->eventWaitingQueue[chn].flag = flag;
#else

    p = osPtr->eventWaiting_p;
    q = osPtr->eventWaiting_p;

    if(flag == WINKS_EVENT_NULL)
    {
        while( p != NULL )
        {
            if(p->chn == chn)
            {
                if(p == q)
                {
                    osPtr->eventWaiting_p = p->next;
                }
                else
                {
                    q->next = p->next;
                }
                Winks_salFree(p);
                p = NULL;
            }
            q = p;
            p = p->next;
        }
    }
    else
    {
        while( p != NULL )
        {
            if(p->chn == chn)
            {
                p->flag = flag;
                return WINKS_RETURN_SUCCESS;
            }
            p = p->next;
        }

        p = Winks_salAlloc(sizeof(winks_EventWaitingQueue_s));
        Winks_assert( p != NULL );
        p->chn = chn;
        p->flag = flag;
        p->next = osPtr->eventWaiting_p;
        osPtr->eventWaiting_p = p;
    }
#endif
    return WINKS_RETURN_SUCCESS;
}

/*增加消息通道的等待消息，flag的每一位标志一个系统消息*/
int Winks_AddWaitingEvent(WINKS_CHN_ID chn, unsigned long flag)
{
#ifdef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
    winks_EventWaitingQueue_s *p = NULL;
    winks_EventWaitingQueue_s *q = NULL;
#endif

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    

    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(chn != WINKS_CHN_NULL);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);
    Winks_assert(flag != WINKS_EVENT_NULL);


#ifndef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
    osPtr->eventWaitingQueue[chn].flag |= (flag);
#else
    p = osPtr->eventWaiting_p;
    while( p != NULL )
    {
        if(p->chn == chn)
        {
            p->flag |= flag;
            return WINKS_RETURN_SUCCESS;
        }
        p = p->next;
    }

    p = Winks_salAlloc(sizeof(winks_EventWaitingQueue_s));
    Winks_assert( p != NULL );
    p->chn = chn;
    p->flag = flag;
    p->next = osPtr->eventWaiting_p;
    osPtr->eventWaiting_p = p;
#endif
    return WINKS_RETURN_SUCCESS;
}

/*向注册等待事件的消息通道中发送相应的消息。*/
int Winks_ProEventWaitingQueue(unsigned long event, unsigned long msg, void *data, unsigned long size)
{
    int iRet = 0;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    /*抽象层完成初始化前不做处理*/
    if((osPtr == NULL) ||
        (osPtr->osalHandlerStart == WINKS_OSALHANDER_STOP))
    {
        Winks_printf("ERR: Load function[Winks_EventHook],but osal not running!\n");
        return WINKS_RETURN_FAILURE;
    }
    
    Winks_printf("event_id = %u, msg_id = %d\n", event, msg);
    if((event != WINKS_EVENT_NULL) && (msg!=0))
    {

#ifndef WINKS_SUPPORT_EVENT_WAITING_LIST_ENTRY
        int i = 0;
        for(i=0; i<WINKS_OSAL_EVENT_WAITING_QUEUE_MAXNUM; ++i)
        {
            if (osPtr->eventWaitingQueue[i].flag & event)
            {
                iRet = Winks_PostMsg((WINKS_CHN_ID)i, msg, data, size);
                Winks_assert(iRet == 0);
            }
        }
#else
        winks_EventWaitingQueue_s *p = osPtr->eventWaiting_p;
        while (p != NULL)
        {
            if (p->flag & event)
            {
                iRet = Winks_PostMsg(p->chn, msg, data, size);
                Winks_assert(iRet == 0);
            }
            p = p->next;
        }
#endif
    }
    return WINKS_RETURN_SUCCESS;
}

/*  pPath:  Unicode. 
    len:    size of \pPath. */
int Winks_SelectFileCallBack( void* pPath, unsigned long len )
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_PostMsg( osPtr->fileChn, WINKS_MSG_UI_SELECT_FILE_RSP/*osPtr->filemsg*/, pPath, len );
    osPtr->fileChn = WINKS_CHN_NULL;
    /*osPtr->filemsg = 0;*/
    return WINKS_RETURN_SUCCESS;
}

int Winks_SelectFile( WINKS_CHN_ID chn, char* defpath, int type )
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    /*if( chn == WINKS_CHN_NULL)
    {
        Winks_fail("chn != WINKS_CHN_NULL");
        return WINKS_RETURN_FAILURE;
    }*/
    if( defpath && (((strlen(defpath) + 1) * 2) > WINKS_PATH_MAX) )
    {
        Winks_printf( "Winks_SelectFile max default path len too long\r\n" );
        return WINKS_RETURN_FAILURE;
    }
    if( chn == WINKS_CHN_NULL)
        osPtr->fileChn = osPtr->uiChn;
    else
        osPtr->fileChn = chn;
    //osPtr->filemsg = WINKS_MSG_UI_SELECT_FILE_RSP;
    Winks_api_SelectFile( defpath, type );
    return WINKS_RETURN_SUCCESS;
}

int Winks_SelectDataAccountCallBack(unsigned long accountid)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_PostMsg(osPtr->DaChn, WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP/*osPtr->Damsg*/, &accountid, sizeof(unsigned long));
    osPtr->DaChn = WINKS_CHN_NULL;
    /*osPtr->Damsg = 0;*/
    return WINKS_RETURN_SUCCESS;
}

int Winks_SelectDataAccount( WINKS_CHN_ID chn, int type, unsigned long data_account )
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    /*if( chn == WINKS_CHN_NULL )
    {
        Winks_fail("chn != WINKS_CHN_NULL");
        return WINKS_RETURN_FAILURE;
    }*/

    if( chn == WINKS_CHN_NULL)
        osPtr->DaChn = osPtr->uiChn;
    else
        osPtr->DaChn = chn;
    
    //osPtr->Damsg = WINKS_MSG_UI_SELECT_DATA_ACCOUNT_RSP;
    Winks_api_SelectDataAccount(type, data_account);
    return WINKS_RETURN_SUCCESS;
}

#if 1
/* 该函数强行在系统的接入点列表中设置指定的接入点方式，如果已有则修改，如果没有则新增。
 * function: Winks_SysAddDataAccount
 * parameters:
 *   unsigned long defIndex: 默认接入点序列号。
 *   char *apn: 代理服务器IP
 *   char *proxyIp: 代理服务器IP
 *   unsigned short proxyPort: 代理服务器端口
 * return:
 *   成功返回: 0
 *   失败返回:-1
 */
int Winks_SysAddDataAccount(unsigned long defIndex, char *apn, char* proxyIp, unsigned short proxyPort)
{
    unsigned long index;

    Winks_assert(apn != NULL);
    Winks_assert(strlen(apn) != 0);
#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
    index = Winks_api_SysAddDataAccount(defIndex, apn, proxyIp, proxyPort);
#else
    index = Winks_api_SysAddDataAccount(defIndex, apn);
#endif
    if(index == WINKS_INVALID_DATA_ACCOUNT_INDEX)
        return WINKS_RETURN_FAILURE;

#ifdef WINKS_APN_WITH_PROXY_IN_PLATFORM
    Winks_SetSettingValue(WINKS_SETTING_APN_ID, index);
#else
    /*设置统一保存的标志*/
    Winks_SetSettingCommitTagether();
    Winks_SetSettingValue(WINKS_SETTING_APN_ID, index);
    //Winks_SetSettingString(WINKS_SETTING_APN, apn);
    if(proxyIp != NULL)
        Winks_SetSettingString(WINKS_SETTING_PROXY_IP, proxyIp);
    else
        Winks_SetSettingString(WINKS_SETTING_PROXY_IP, "");
    Winks_SetSettingValue(WINKS_SETTING_PROXY_PORT, proxyPort);
    /*统一写文件。*/
    Winks_CommitTagether();
#endif
    Winks_printf("Winks_SysAddDataAccount[real=%ld]?==[def=%ld],[%s][%s:%d]\r\n", 
            index, defIndex, apn, proxyIp, proxyPort);
    return WINKS_RETURN_SUCCESS;
}
#endif

int Winks_SelectPhoneNumberCallBack( void* pPhoneNum)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    Winks_PostMsg( osPtr->pnChn, WINKS_MSG_UI_SELECT_PHONENUM_RSP, pPhoneNum, 
        (pPhoneNum == NULL || strlen(pPhoneNum) == 0)?(0):(strlen(pPhoneNum)+1) );
    osPtr->pnChn = WINKS_CHN_NULL;
    return WINKS_RETURN_SUCCESS;
}

int Winks_SelectPhoneNumber(WINKS_CHN_ID chn)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    /*if( chn == WINKS_CHN_NULL )
    {
        Winks_fail("chn != WINKS_CHN_NULL");
        return WINKS_RETURN_FAILURE;
    }*/

    if( chn == WINKS_CHN_NULL)
        osPtr->pnChn = osPtr->uiChn;
    else
        osPtr->pnChn = chn;

    Winks_api_SelectPhoneNumber();
    return WINKS_RETURN_SUCCESS;
}

void Winks_OsalTimerExpiry(void)
{
    winks_TimerBody_s *timer_p = NULL;

    WINKS_TIMER_ID firstTimer = WINKS_TIMER_NULL;
    WINKS_TICKS minTicks = 0xffffffff;
    WINKS_TICKS curTicks = 0UL;
    unsigned long  i;


    winks_TimerQueue_s *osalTimerQueue_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    osalTimerQueue_p = &(osPtr->osalTimerQueue);
    
    curTicks = osalTimerQueue_p->curTicks;
    for(i = 1; i < WINKS_OSAL_TIMMER_MAXNUM; ++i)
    {
        timer_p = &(osalTimerQueue_p->queue[i]);

        /*定时器关闭*/
        if(timer_p->state == 0)     continue;

        timer_p->timeOut -= curTicks;

         /*定时器到时*/
        if(timer_p->timeOut == 0)
        {
            WINKS_TIMER_ID timerId = (WINKS_TIMER_ID)i;
            Winks_PostMsg(timer_p->chn, WINKS_MSG_TIMER_EXPIRY, &timerId, sizeof(timerId));
            timer_p->state = 0;

            if(0  == (timer_p->flag & WINKS_TIMER_AUTO_LOOP))
            {
                continue;
            }
            else
            {
                timer_p->timeOut = timer_p->timeOutPeriod;
                timer_p->state = 1;
                /*循环定时器重新启动*/
            }
        }

        Winks_assert(timer_p->timeOut > 0);
        /*判断最近到时的定时器*/
        if(timer_p->timeOut < minTicks)
        {
            firstTimer = i;
            minTicks = timer_p->timeOut;
        }
    }

    if(WINKS_TIMER_NULL == firstTimer)
    {
        /*没有在运行的TIMER*/
        osalTimerQueue_p->curInd = WINKS_TIMER_NULL;
        osalTimerQueue_p->curTicks = 0;
        osalTimerQueue_p->beginTime = 0;
    }
    else
    {
        /*还有在运行的TIMER*/
        //StopTimer( WINKS_CCTIMER_ID );
        Winks_api_StartTimer((unsigned long)(WINKS_TICKS2MILLISECONDS(minTicks)));

        osalTimerQueue_p->curInd = firstTimer;
        osalTimerQueue_p->curTicks = minTicks;
        osalTimerQueue_p->beginTime = Winks_GetSysTick(); /*取系统tick*/
    }
    return;
}

/*创建定时器*/
WINKS_TIMER_ID Winks_CreateTimer(WINKS_CHN_ID chn, WINKS_TICKS timeOutPeriod, unsigned long flag)
{
    WINKS_TIMER_ID curTimer = WINKS_TIMER_NULL;
    winks_TimerBody_s *timer_p = NULL;
    unsigned long  i;
    int iRet = 0;
    int index = 0;


    winks_TimerQueue_s *osalTimerQueue_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    osalTimerQueue_p = &(osPtr->osalTimerQueue);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(chn != 0);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);
    Winks_assert(timeOutPeriod != 0);

#ifdef WINKS_MULTITHREADING
    Winks_api_GetMutex(WINKS_MUTEX_TIMER);
#endif

    /*分配Timer资源*/
    for(i = 1; i < WINKS_OSAL_TIMMER_MAXNUM; ++i)
    {
        index = osalTimerQueue_p->sequence + i;
        if (index >= WINKS_OSAL_TIMMER_MAXNUM)
            index -= (WINKS_OSAL_TIMMER_MAXNUM-1);
        if(WINKS_CHN_NULL == osalTimerQueue_p->queue[index].chn)
        {
            osalTimerQueue_p->sequence = index;
            curTimer = index;
            break;
        }
    }
    if(WINKS_TIMER_NULL == curTimer)
    {
        Winks_fail("OSAL Timer queue is filled\n");
        return 0L;
    }

    timer_p = &osalTimerQueue_p->queue[curTimer];
    timer_p->chn = chn;
    timer_p->timeOutPeriod = timeOutPeriod;
    timer_p->timeOut = 0;
    timer_p->flag= flag;
    timer_p->state= 0;

#ifdef WINKS_MULTITHREADING
    Winks_api_PutMutex(WINKS_MUTEX_TIMER);
#endif

    /*自启动处理*/
    if(0 != (flag & WINKS_TIMER_AUTO_START))
    {
        iRet = Winks_StartTimer(curTimer);
        Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    }

    return (curTimer);
}



/*删除定时器*/
int Winks_DestroyTimer(WINKS_TIMER_ID timerId)
{
    winks_TimerBody_s *timer_p = NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    timer_p = &(osPtr->osalTimerQueue.queue[timerId]);
    Winks_mem_set(timer_p, 0, sizeof(winks_TimerBody_s));

    return WINKS_RETURN_SUCCESS;
}

/*启动一个已经创建的定时器*/
int Winks_StartTimer(WINKS_TIMER_ID timerId)
{
    WINKS_TIMER_ID firstTimer = WINKS_TIMER_NULL;
    winks_TimerBody_s *timer_p = NULL;
    unsigned long  curTicks = 0L;
    unsigned long  endTime = 0L;
    WINKS_TICKS minTicks = 0xffffffff;
    int iRet = 0;
    unsigned long  i;


    winks_TimerQueue_s *osalTimerQueue_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    osalTimerQueue_p = &(osPtr->osalTimerQueue);
    

    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(timerId != 0);
    Winks_assert(timerId < WINKS_OSAL_TIMMER_MAXNUM);

    Winks_assert(osalTimerQueue_p->queue[timerId].chn != 0);

    /*更新所有定时器*/
    if(WINKS_TIMER_NULL != osalTimerQueue_p->curInd)
    {
        /*取当前系统TICKS，比较差值*/
        Winks_api_StopTimer();

        endTime = Winks_GetSysTick();
        curTicks = (endTime - osalTimerQueue_p->beginTime)*WINKS_SYS_TICK;

        /*更新各个定时器的TIMEOUT*/
        for(i = 1; i < WINKS_OSAL_TIMMER_MAXNUM; ++i)
        {
            timer_p = &osalTimerQueue_p->queue[i];
            if(timer_p->state == 0)  continue;

            /*timer_p->timeOut -= curTicks;*/
            if(timer_p->timeOut > curTicks)
            {
                timer_p->timeOut -= curTicks;
            }
            else
            {
                timer_p->timeOut = 0;
            }

             /*定时器到时*/
            if(timer_p->timeOut == 0)
            {
                WINKS_TIMER_ID timerIdTmp = (WINKS_TIMER_ID)i;
                iRet = Winks_PostMsg(timer_p->chn, WINKS_MSG_TIMER_EXPIRY, &timerIdTmp, sizeof(timerIdTmp));
                Winks_assert(iRet == WINKS_RETURN_SUCCESS);

                timer_p->state = 0;

                if(0 == (timer_p->flag & WINKS_TIMER_AUTO_LOOP))
                {
                    continue;
                }
                else
                {
                    timer_p->timeOut = timer_p->timeOutPeriod;
                    timer_p->state = 1;
                    /*循环定时器重新启动*/
                }
            }

            Winks_assert(timer_p->timeOut > 0);
            /*判断最近到时的定时器*/
            if(timer_p->timeOut < minTicks)
            {
                firstTimer = i;
                minTicks = timer_p->timeOut;
            }
        }
    }


    timer_p = &osalTimerQueue_p->queue[timerId];
    timer_p->timeOut = timer_p->timeOutPeriod;
    timer_p->state= 1;

    /*比较新建定时器的TIMEOUT是否最小*/
    if(timer_p->timeOut < minTicks)
    {
        firstTimer = timerId;
        minTicks = timer_p->timeOut;
    }

    /*重新启动OSAL定时器*/
    /*StopTimer( WINKS_CCTIMER_ID );*/
    Winks_api_StartTimer((unsigned long)(WINKS_TICKS2MILLISECONDS(minTicks)));

    osalTimerQueue_p->curInd = firstTimer;
    osalTimerQueue_p->curTicks = minTicks;
    osalTimerQueue_p->beginTime = Winks_GetSysTick(); /*取系统tick*/

    return WINKS_RETURN_SUCCESS;
}

/*停止指定的定时器*/
int Winks_StopTimer(WINKS_TIMER_ID timerId)
{
    winks_TimerBody_s *timer_p = NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    timer_p = &(osPtr->osalTimerQueue.queue[timerId]);
    timer_p->timeOut = 0;
    timer_p->state = 0;
    return WINKS_RETURN_SUCCESS;
}

/*取定时器内部信息*/
int Winks_GetTimerInfo(WINKS_TIMER_ID timerId, winks_TimerBody_s *timerInfo)
{
    winks_TimerBody_s *timer_p = NULL;

    WINKS_OSAL_CONTEXT_DEF(osPtr);
	Winks_assert(timerInfo != NULL);
    
    if(timerId >= WINKS_OSAL_TIMMER_MAXNUM)
        return WINKS_RETURN_FAILURE;
    timer_p = &(osPtr->osalTimerQueue.queue[timerId]);
    if(timer_p->chn == WINKS_CHN_NULL)
        return WINKS_RETURN_FAILURE;
    Winks_mem_cpy(timerInfo, timer_p, sizeof(winks_TimerBody_s));
    return WINKS_RETURN_SUCCESS;
}

/* 抽象层创建彩像目录 */
void Winks_OsalInitDir(void)
{
    Winks_mkdir(WINKS_APP_PATH);
    return;
}

/*按照指定格式将时间转换成字符串。
dateTime_p为表示时间的结构体指针，
当dateTime_p==NULL时取系统时间，随后按指定格式返回。*/
int Winks_FormatDateTime(Winks_DateTimeFormat_e format, char *buff, unsigned char size, Winks_DateTime_s *dateTime_p)
{
    Winks_DateTime_s dateTime, *p = NULL;
    int iRet = 0;

    Winks_assert(buff != NULL);
    Winks_mem_set(&dateTime, 0, sizeof(dateTime));
    if(dateTime_p == NULL)
    {
        iRet = Winks_GetSysDateTime(&dateTime);
        if(iRet != 0)
        {
            return WINKS_RETURN_FAILURE;
        }
    }
    else
    {
        dateTime = *dateTime_p;
    }

    p = &dateTime;
    switch (format)
    {
        #if 0
    case WINKS_DTFRM_YYYYMMDD_1:/*20080101*/
        Winks_assert(size > 8);
        sprintf(buff,"%04ld%02d%02d", p->year, p->month, p->day);
        break;
    case WINKS_DTFRM_YYYYMMDD_2:/*2008/01/01*/
        Winks_assert(size > 10);
        sprintf(buff,"%04ld/%02d/%02d", p->year, p->month, p->day);
        break;
    case WINKS_DTFRM_YYYYMMDD_3:/*2008-01-01*/
        Winks_assert(size > 10);
        sprintf(buff,"%04ld-%02d-%02d", p->year, p->month, p->day);
        break;
    case WINKS_DTFRM_HH24MISS_1:/*245959*/
        Winks_assert(size > 6);
        sprintf(buff,"%02d%02d%02d", p->hour, p->min, p->sec);
        break;
    case WINKS_DTFRM_HH24MISS_2:/*24:59:59*/
        Winks_assert(size > 8);
        sprintf(buff,"%02d:%02d:%02d", p->hour, p->min, p->sec);
        break;
        #endif
    case WINKS_DTFRM_YYYYMMDDHH24MISS_1:/*20080101245959*/
        Winks_assert(size > 14);
        sprintf(buff,"%04ld%02d%02d%02d%02d%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
        #if 0
    case WINKS_DTFRM_YYYYMMDDHH24MISS_2:/*2008010124:59:59*/
        Winks_assert(size > 16);
        sprintf(buff,"%04ld%02d%02d%02d:%02d:%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
    case WINKS_DTFRM_YYYYMMDDHH24MISS_3:/*2008/01/0124:59:59*/
        Winks_assert(size > 18);
        sprintf(buff,"%04ld/%02d/%02d%02d:%02d:%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
    case WINKS_DTFRM_YYYYMMDDHH24MISS_4:/*2008-01-0124:59:59*/
        Winks_assert(size > 18);
        sprintf(buff,"%04ld-%02d-%02d%02d:%02d:%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
    case WINKS_DTFRM_YYYYMMDDHH24MISS_5:/*20080101 24:59:59*/
        Winks_assert(size > 17);
        sprintf(buff,"%04ld%02d%02d% 02d:%02d:%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
    case WINKS_DTFRM_YYYYMMDDHH24MISS_6:/*2008/01/01 24:59:59*/
        Winks_assert(size > 19);
        sprintf(buff,"%04ld/%02d/%02d %02d:%02d:%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
    case WINKS_DTFRM_YYYYMMDDHH24MISS_7: /*2008-01-01 24:59:59*/
        Winks_assert(size > 19);
        sprintf(buff,"%04ld-%02d-%02d %02d:%02d:%02d", p->year, p->month, p->day, p->hour, p->min, p->sec);
        break;
        #endif
    case WINKS_DTFRM_YYMMDDHH24MISS_1: /*0801012459*/
        Winks_assert(size > 13);
        sprintf(buff,"%02ld%02d%02d%02d%02d%02d", p->year%100, p->month, p->day, p->hour, p->min, p->sec);
        break;
    default:
        Winks_fail("Format error!");
        break;
    }
    return WINKS_RETURN_SUCCESS;
}

/*计算从1970 年1 月1 日0 时起到指定时间经过的秒数.
    tm:输入的时间。如果tm=NULL，取系统时间(相当于time)。
    return unsigned long型的秒数，0表示失败。
*/
unsigned long Winks_mktime (Winks_DateTime_s *dtp)
{
    #define MINUTE 60   // 1 分钟的秒数。
    #define HOUR (60*MINUTE) // 1 小时的秒数。
    #define DAY (24*HOUR)   // 1 天的秒数。
    #define YEAR (365*DAY)   // 1 年的秒数。
	
	/* interestingly, we assume leap-years */
	/* 有趣的是我们考虑进了闰年 */
	// 下面以年为界限，定义了每个月开始时的秒数时间数组。
	const int monTab[12] = {
		0,
			DAY * (31),
			DAY * (31 + 29),
			DAY * (31 + 29 + 31),
			DAY * (31 + 29 + 31 + 30),
			DAY * (31 + 29 + 31 + 30 + 31),
			DAY * (31 + 29 + 31 + 30 + 31 + 30),
			DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),
			DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
			DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
			DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
			DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
	};
	
	unsigned long res;
	int year;
    Winks_DateTime_s dateTime, *p = NULL;
    int iRet = 0;

    if(dtp == NULL)
    {
        iRet = Winks_GetSysDateTime(&dateTime);
        if(iRet != 0)
            return 0;
        p = &dateTime;
    }
    else
    {
        p = dtp;
    }

    if(p->year < 1970)
        return 0;/*失败*/

	year = (int)p->year - 1970; // 从70 年到现在经过的年数(2 位表示方式)，
	// 因此会有2000 年问题。
	/* magic offsets (y+1) needed to get leapyears right. */
	/* 为了获得正确的闰年数，这里需要这样一个魔幻偏值(y+1) */
	res = (unsigned long)(YEAR * year + DAY * ((year + 1) / 4)); // 这些年经过的秒数时间 + 每个闰年时多1 天
	res += (unsigned long)monTab[p->month]; // 的秒数时间，在加上当年到当月时的秒数。
	/* and (y+2) here. If it wasn't a leap-year, we have to adjust */
	/* 以及(y+2)。如果(y+2)不是闰年，那么我们就必须进行调整(减去一天的秒数时间)。 */
	if (p->month > 2 && ((year + 2) % 4))
		res -= (unsigned long)DAY;
	res += (unsigned long)(DAY * (p->day - 1)); // 再加上本月过去的天数的秒数时间。
	res += (unsigned long)(HOUR * p->hour); // 再加上当天过去的小时数的秒数时间。
	res += (unsigned long)(MINUTE * p->min); // 再加上1 小时内过去的分钟数的秒数时间。
	res += (unsigned long)p->sec;   // 再加上1 分钟内已过的秒数。
	return res;    // 即等于从1970 年以来经过的秒数时间。
}

/*取Unicode的strlen.*/
int WK_STRLEN(const char *str)
{
    const short *p = NULL;
    int ret = 0;
    
    if(str == NULL) return 0;
    
    p = (const short *)str;
    while(*p++) ret++;
    return ret;
}

int Winks_strnicmp(const char *s1, const char * s2, unsigned int n)
{
    if (n == 0)
        return 0;

    while (n-- != 0 && toupper(*s1) == toupper(*s2))
    {
        if (n == 0 || *s1 == '\0' || *s2 == '\0')
            break;
        s1++;
        s2++;
    }

    return toupper(*(unsigned char *) s1) - toupper(*(unsigned char *) s2);
}

int Winks_stricmp(const char* s1, const char* s2 )
{
    while (*s1 != '\0' && toupper(*s1) == toupper(*s2))
    {
        s1++;
        s2++;
    }

    return toupper(*(unsigned char *) s1) - toupper(*(unsigned char *) s2);
}

char* Winks_strcpy(char *dst0, const char *src0)
{
    char* s;

    if( src0 == NULL )
        return NULL;

    s = dst0;

    while ((*dst0++ = *src0++) != '\0');

    return s;

}

char* Winks_strncpy(char *dst0, const char *src0, unsigned int count)
{
    char *dst;

    if( src0 == NULL || count == 0 )
        return NULL;

    dst = dst0;
    while (count > 0)
    {
        --count;
        if ((*dst0++ = *src0++) == '\0')
            break;
    }
    while (count-- > 0)
        *dst0++ = '\0';

    return dst;
}

/* 比较两个字符串的最后4 个字符 */
int winks_strcmp_last_4(const char *str1, const char *str2)
{
    char *p1, *p2;
    unsigned int len1, len2;

    if (str1 == NULL || str2 == NULL)
        return 0;

    len1 = strlen(str1);
    len2 = strlen(str2);

    if (len1 > 4)
        p1 = (char *)&str1[len1 - 4];
    else
        p1 = (char *)str1;

    if (len2 > 4)
        p2 = (char *)&str2[len2 - 4];
    else
        p2 = (char *)str2;

    return (strcmp(p1, p2) == 0);
}

/* 返回IMSI CODE. imsi_str end with '\0'.*/
void wk_osal_DoWaitingImei(char *imei_str)
{
    winks_MsgWaitingQueue_s *p = NULL;
    void *msg_data = NULL;
    unsigned long msg_len = 0;
    int iRet = 0;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(osPtr->waitingImei_T!= WINKS_TIMER_NULL)
    {
        Winks_DestroyTimer(osPtr->waitingImei_T);
        osPtr->waitingImei_T = WINKS_TIMER_NULL;
    }

    if( (imei_str!=NULL) && (strlen(imei_str)!=0) )
    {
        Winks_mem_set(osPtr->curImei, 0, sizeof(osPtr->curImei));
        strncpy(osPtr->curImei, imei_str, WINKS_IMEI_LEN);
        msg_data = osPtr->curImei;
        msg_len = strlen(osPtr->curImei);
    }
    
    p = osPtr->waitingImei_p;
    while(p != NULL)
    {
        iRet = Winks_PostMsg(p->chn, p->msg, (void *)msg_data, msg_len);
        Winks_assert(iRet == 0);

        p = p->next;
        Winks_salFree(osPtr->waitingImei_p);
        osPtr->waitingImei_p = p;
    }

    return;
}



int Winks_GetImei(WINKS_CHN_ID chn, char *buff, unsigned long buff_size)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(buff != NULL);
    Winks_assert(buff_size > WINKS_IMEI_LEN);

    Winks_mem_set(buff, 0, buff_size);

    if(chn != WINKS_CHN_NULL)
    {
        Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
        Winks_assert(osPtr->osalCnhList[chn] != NULL);
    }

    if(strlen(osPtr->curImei)<15)
    {
        if(osPtr->waitingImei_p == NULL)
        {
            Winks_api_GetImei();
        }      
        
        if(chn == WINKS_CHN_NULL)
        {
            return WINKS_RETURN_FAILURE;
        }
        else/*异步方式*/
        {
            winks_MsgWaitingQueue_s *curWaitInfo_p = NULL;

            /*动态申请资源*/
            curWaitInfo_p = Winks_salAlloc(sizeof(winks_MsgWaitingQueue_s));
            Winks_assert(curWaitInfo_p != NULL);
            if(curWaitInfo_p == NULL)
            {
                return WINKS_RETURN_FAILURE;
            }
            curWaitInfo_p->chn = chn;
            curWaitInfo_p->msg = WINKS_MSG_IMEI_REQ;
            curWaitInfo_p->next = osPtr->waitingImei_p;
            osPtr->waitingImei_p = curWaitInfo_p;
            
            osPtr->waitingImei_T =  Winks_CreateTimer(
                osPtr->osalChn,
                WINKS_MILLISECONDS2TICKS(TIMER_INTERVAL_OSAL_WAITINGIMEI),
                WINKS_TIMER_AUTO_START
                );
            Winks_assert(osPtr->waitingImei_T != WINKS_TIMER_NULL);
            if(osPtr->waitingImei_T == WINKS_TIMER_NULL)
            {
                curWaitInfo_p = osPtr->waitingImei_p;
                osPtr->waitingImei_p = osPtr->waitingImei_p->next;
                Winks_salFree(curWaitInfo_p);
                curWaitInfo_p = NULL;
                return WINKS_RETURN_FAILURE;
            }
            Winks_printf("WINKS_RETURN_WAIT_MSG.\n");
            return WINKS_RETURN_WAIT_MSG;
        }
    }
    else
    {
        strncpy(buff, osPtr->curImei, buff_size - 1);
        return WINKS_RETURN_SUCCESS;
    }
}


#ifndef WINKS_DUAL_SIM
/* 返回IMSI CODE. imsi_str end with '\0'.*/
void wk_osal_DoWaitingImsi(char *imsi_str)
{
    winks_MsgWaitingQueue_s *p = NULL;
    void *msg_data = NULL;
    unsigned long msg_len = 0;
    int iRet = 0;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(osPtr->waitingImsi_T!= WINKS_TIMER_NULL)
    {
        Winks_DestroyTimer(osPtr->waitingImsi_T);
        osPtr->waitingImsi_T = WINKS_TIMER_NULL;
    }

    if( (imsi_str!=NULL) && (strlen(imsi_str)!=0) )
    {
        Winks_mem_set(osPtr->curImsi, 0, sizeof(osPtr->curImsi));
        strncpy(osPtr->curImsi, imsi_str, WINKS_IMSI_LEN);
        msg_data = osPtr->curImsi;
        msg_len = strlen(osPtr->curImsi);
    }
    
    p = osPtr->waitingImsi_p;
    while(p != NULL)
    {
        iRet = Winks_PostMsg(p->chn, p->msg, (void *)msg_data, msg_len);
        Winks_assert(iRet == 0);

        p = p->next;
        Winks_salFree(osPtr->waitingImsi_p);
        osPtr->waitingImsi_p = p;
    }

    return;
}



int Winks_GetImsi(WINKS_CHN_ID chn, char *buff, unsigned long buff_size)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(buff != NULL);
    Winks_assert(buff_size > WINKS_IMSI_LEN);

    Winks_mem_set(buff, 0, buff_size);

    if(chn != WINKS_CHN_NULL)
    {
        Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
        Winks_assert(osPtr->osalCnhList[chn] != NULL);
    }

    if((strlen(osPtr->curImsi) != WINKS_IMSI_LEN) ||
        (strncmp(osPtr->curImsi,"000000000000000",WINKS_IMSI_LEN) == 0))
    {
        if(osPtr->waitingImsi_p == NULL)
        {
            Winks_api_GetImsi();
        }      
        
        if(chn == WINKS_CHN_NULL)
        {
            return WINKS_RETURN_FAILURE;
        }
        else/*异步方式*/
        {
            winks_MsgWaitingQueue_s *curWaitInfo_p = NULL;

            /*动态申请资源*/
            curWaitInfo_p = Winks_salAlloc(sizeof(winks_MsgWaitingQueue_s));
            Winks_assert(curWaitInfo_p != NULL);
            if(curWaitInfo_p == NULL)
            {
                return WINKS_RETURN_FAILURE;
            }
            curWaitInfo_p->chn = chn;
            curWaitInfo_p->msg = WINKS_MSG_IMSI_REQ;
            curWaitInfo_p->next = osPtr->waitingImsi_p;
            osPtr->waitingImsi_p = curWaitInfo_p;
            
            osPtr->waitingImsi_T =  Winks_CreateTimer(
                osPtr->osalChn,
                WINKS_MILLISECONDS2TICKS(TIMER_INTERVAL_OSAL_WAITINGIMSI),
                WINKS_TIMER_AUTO_START
                );
            Winks_assert(osPtr->waitingImsi_T != WINKS_TIMER_NULL);
            if(osPtr->waitingImsi_T == WINKS_TIMER_NULL)
            {
                curWaitInfo_p = osPtr->waitingImsi_p;
                osPtr->waitingImsi_p = osPtr->waitingImsi_p->next;
                Winks_salFree(curWaitInfo_p);
                curWaitInfo_p = NULL;
                return WINKS_RETURN_FAILURE;
            }
            Winks_printf("WINKS_RETURN_WAIT_MSG.\n");
            return WINKS_RETURN_WAIT_MSG;
        }
    }
    else
    {
        strncpy(buff, osPtr->curImsi, buff_size - 1);
        return WINKS_RETURN_SUCCESS;
    }
}

#else /* WINKS_DUAL_SIM */
/* 返回IMSI CODE. imsi_str end with '\0'.*/
static void wk_osal_DoWaitingImsiMux(char *imsi_str, int Sim)
{
    winks_MsgWaitingQueue_s *p = NULL;
    winks_MsgWaitingQueue_s *q = NULL;
    void *msg_data = NULL;
    unsigned long msg_len = 0;
    int iRet = 0;
    WINKS_TIMER_ID *pWaitImsi_T;
    char * pCurImsi;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(!Sim)
    {
        pWaitImsi_T = &osPtr->waitingImsi_T;
        pCurImsi = osPtr->curImsi;
    }
    else
    {
        pWaitImsi_T = &osPtr->waitingImsi_2_T;
            pCurImsi = osPtr->curImsi_2;
    }
    if(*pWaitImsi_T != WINKS_TIMER_NULL)
    {
        Winks_DestroyTimer(*pWaitImsi_T);
        *pWaitImsi_T = WINKS_TIMER_NULL;
    }

    if( (imsi_str!=NULL) && (strlen(imsi_str)!=0) )
    {
        Winks_mem_set(pCurImsi, 0, sizeof(osPtr->curImsi));
        strncpy(pCurImsi, imsi_str, WINKS_IMSI_LEN);
        msg_data = pCurImsi;
        msg_len = strlen(pCurImsi);
    }

    /* 释放前找到第一个不释放的节点 */
    q = p = osPtr->waitingImsi_p;
    while(p != NULL)
    {
        if( (!Sim && p->msg == WINKS_MSG_IMSI_REQ) ||
            (Sim == 1 && p->msg == WINKS_MSG_IMSI_2_REQ) )
        {
        }
        else
        {
            break;
        }
        p = p->next;
    }
    osPtr->waitingImsi_p = p;

    /* 释放获取imsi返回消息的节点 */
    p = q;
    while(p != NULL)
    {
        if( (!Sim && p->msg == WINKS_MSG_IMSI_REQ) ||
            (Sim == 1 && p->msg == WINKS_MSG_IMSI_2_REQ) )
        {
            iRet = Winks_PostMsg(p->chn, p->msg, (void *)msg_data, msg_len);
            Winks_assert(iRet == 0);

            if(q->next == p)
            {
                q->next = p->next;
                Winks_salFree(p);
                p = q->next;
            }
            else
            {
                q = p->next;
                Winks_salFree(p);
                p = q;
            }
        }
        else
        {
            p = p->next;
            q->next = p;
        }
    }

    return;
}

static int Winks_GetImsiMux(WINKS_CHN_ID chn, char *buff, unsigned long buff_size, int Sim)
{
    char *pCurImsi = NULL;
    WINKS_TIMER_ID *pWaitImsi_T;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(buff != NULL);
    Winks_assert(buff_size > WINKS_IMSI_LEN);

    if(!Sim)
    {
        pCurImsi = osPtr->curImsi;
        pWaitImsi_T = &osPtr->waitingImsi_T;
    }
    else
    {
        pCurImsi = osPtr->curImsi_2;
        pWaitImsi_T = &osPtr->waitingImsi_2_T;
    }

    Winks_mem_set(buff, 0, buff_size);

    if(chn != WINKS_CHN_NULL)
    {
        Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
        Winks_assert(osPtr->osalCnhList[chn] != NULL);
    }

    if((strlen(pCurImsi) != WINKS_IMSI_LEN) ||
        (strncmp(pCurImsi,"000000000000000",WINKS_IMSI_LEN) == 0))
    {
        if(!Sim)
            Winks_api_GetImsi();
        else
            Winks_api_GetImsi_2();
        
        if(chn == WINKS_CHN_NULL)
        {
            return WINKS_RETURN_FAILURE;
        }
        else/*异步方式*/
        {
            winks_MsgWaitingQueue_s *curWaitInfo_p = NULL;

            /*动态申请资源*/
            curWaitInfo_p = Winks_salAlloc(sizeof(winks_MsgWaitingQueue_s));
            Winks_assert(curWaitInfo_p != NULL);
            if(curWaitInfo_p == NULL)
            {
                return WINKS_RETURN_FAILURE;
            }
            curWaitInfo_p->chn = chn;
            if(!Sim)
                curWaitInfo_p->msg = WINKS_MSG_IMSI_REQ;
            else
                curWaitInfo_p->msg = WINKS_MSG_IMSI_2_REQ;
            curWaitInfo_p->next = osPtr->waitingImsi_p;
            osPtr->waitingImsi_p = curWaitInfo_p;
            
            *pWaitImsi_T =  Winks_CreateTimer(
                osPtr->osalChn,
                WINKS_MILLISECONDS2TICKS(TIMER_INTERVAL_OSAL_WAITINGIMSI),
                WINKS_TIMER_AUTO_START
                );
            Winks_assert(*pWaitImsi_T != WINKS_TIMER_NULL);
            if(*pWaitImsi_T == WINKS_TIMER_NULL)
            {
                curWaitInfo_p = osPtr->waitingImsi_p;
                osPtr->waitingImsi_p = osPtr->waitingImsi_p->next;
                Winks_salFree(curWaitInfo_p);
                curWaitInfo_p = NULL;
                return WINKS_RETURN_FAILURE;
            }
            Winks_printf("WINKS_RETURN_WAIT_MSG.\n");
            return WINKS_RETURN_WAIT_MSG;
        }
    }
    else
    {
        strncpy(buff, pCurImsi, buff_size - 1);
        return WINKS_RETURN_SUCCESS;
    }
}

void wk_osal_DoWaitingImsi(char *imsi_str)
{
    wk_osal_DoWaitingImsiMux(imsi_str, 0);
}

void wk_osal_DoWaitingImsi_2(char *imsi_str)
{
    wk_osal_DoWaitingImsiMux(imsi_str, 1);
}

/* 如果已知彩像激活的sim卡，则返回该卡的imsi；否则返回第0卡的imsi */
int Winks_GetImsi(WINKS_CHN_ID chn, char *buff, unsigned long buff_size)
{
    int Sim = 0;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(osPtr->ActiveSim == 0x0202)
        Sim = 1;
    else
        Sim = 0;
    
    return Winks_GetImsiMux(chn, buff, buff_size, Sim);
}

int Winks_GetImsi_2(WINKS_CHN_ID chn, char *buff, unsigned long buff_size)
{
    return Winks_GetImsiMux(chn, buff, buff_size, 1);
}

#if 0
/* 询问是否已获取到Sim卡的IMSI: FALSE--未获取;TRUE--已获取 */
int Winks_HasGotImsi(int SimNo)
{
    if( SimNo == 1 && 
        (strlen(osPtr->curImsi) == WINKS_IMSI_LEN) &&
        (strncmp(osPtr->curImsi, "000000000000000", WINKS_IMSI_LEN)))
        return TRUE;
    else if(SimNo == 2 &&
            (strlen(osPtr->curImsi_2) == WINKS_IMSI_LEN) &&
            (strncmp(osPtr->curImsi_2, "000000000000000", WINKS_IMSI_LEN)))
        return TRUE;
    
    return FALSE;
}
#endif

/* 当得知彩像使用的是那张卡时调用此函数
   设置激活卡后调用Winks_GetImsi可直接返回彩像使用的Sim卡imsi */
int Winks_SetActiveSim(int SimNo)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);

    if( SimNo == 1 && 
        (strlen(osPtr->curImsi) == WINKS_IMSI_LEN) &&
        (strncmp(osPtr->curImsi, "000000000000000", WINKS_IMSI_LEN)))
        osPtr->ActiveSim = 0x0101;
    else if(SimNo == 2 &&
            (strlen(osPtr->curImsi_2) == WINKS_IMSI_LEN) &&
            (strncmp(osPtr->curImsi_2, "000000000000000", WINKS_IMSI_LEN)))
        osPtr->ActiveSim = 0x0202;
    else
    {
        if(SimNo == 0)
            osPtr->ActiveSim = 0;
    #ifdef WINKS_DEBUG
        else
        {
            char * pCurImsi;
            if(SimNo == 1)
                pCurImsi = osPtr->curImsi;
            else
                pCurImsi = osPtr->curImsi_2;
            Winks_printf("Err: Winks_SetActiveSim(%d) %s\r\n", SimNo, pCurImsi);
            return WINKS_RETURN_FAILURE;
        }
    #endif
    }

    return WINKS_RETURN_SUCCESS;
}

int Winks_GetActiveSim()
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(Winks_IsOsalEnable() == 0)
        return 0;
    
    if(osPtr->ActiveSim == 0x0101)
        return 1;
    else if(osPtr->ActiveSim == 0x0202)
        return 2;
    else
        return 0;
}
#endif /* WINKS_DUAL_SIM */

/*设置文件操作错误码函数。返回值: 0成功； -1失败。
errorCode 为指定的错误码。*/
int Winks_SetFileSysErrorCode(unsigned long errorCode)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    if(osPtr->osalChn == WINKS_CHN_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    osPtr->fileSysErrorCode = errorCode;
    return WINKS_RETURN_SUCCESS;
}


/*取文件操作错误码函数。返回值: 0成功； -1失败。
*errorCode 为返回的错误码。*/
int Winks_GetFileSysErrorCode(unsigned long *errorCode)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(errorCode != NULL)
    *errorCode = 0UL;
    if(osPtr->osalChn == WINKS_CHN_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    *errorCode = osPtr->fileSysErrorCode;
#if 0/*def WINKS_DEBUG*/
    Winks_printf("file error code [%d]\n", osPtr->fileSysErrorCodeS);
#endif
    return WINKS_RETURN_SUCCESS;
}

/*调用平台的应用--WAP*/
extern int Winks_api_OpenWapPage( char* pUrl );
int Winks_OpenWapPage( char* pUrl )
{
    return Winks_api_OpenWapPage(pUrl);
}

/* 判断一个unicode字符串中的字符是否全是ascii */
int wk_is_string_ascii(const char *string)
{
#if 1
    const unsigned short *start = (const unsigned short *)string;

    while(*start)
    {
        if(((*start)&0xff80) != 0)
            return 0;;
        start++;
    }
    return 1;
    
#else    

    Winks_assert(string[0] || string[1]);

    while (string[0] || string[1])
    {
    #ifdef __BIG_ENDIAN
        if (string[1] > 0x7F || string[0])
    #else
        if (string[0] > 0x7F || string[1])
    #endif
            return 0;

        string += 2;
    }

    return 1;
#endif
}

#if 0
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

    Winks_mem_set(temp, 0, count * sizeof(wk_SortItem));
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
*   NOTE: 要求array中的number与name是unicode小端编码。
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

    Winks_mem_set(temp1, 0, count * sizeof(wk_SortItem));
    Winks_mem_set(temp2, 0, count * sizeof(Winks_FriendItem_s));

    for (i = 0; i < count; i++)
    {
#if 1
        Winks_assert(WK_STRLEN(array[i].number) > 0);
#else
    #ifdef __BIG_ENDIAN
        Winks_assert(array[i].number[1]);
    #else
        Winks_assert(array[i].number[0]);
    #endif
#endif

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
#endif


#ifndef WINKS_USE_PLATFORM_SOURCE_MANAGER
/***************************************
**彩像图片展示的缓存机制
 ****************************************/
/*资源处理*/
const Winks_ImgType wk_ImgType[] =
{
    WINKS_IMG_TYPE_BMP, "bmp",
    WINKS_IMG_TYPE_GIF, "gif",
    WINKS_IMG_TYPE_JPG, "jpg",
    WINKS_IMG_TYPE_PNG, "png",
    WINKS_IMG_TYPE_INVALID, NULL
};
static char* winks_ui_getfname( const char* pname )
{
    char* pfname = (char* )pname;
    char* ptname = (char* )pname;

    while( *ptname )
    {
        if( *ptname == '\\' || *ptname == '/' )
            pfname = ptname + 1;
        ptname ++;
    }

    if( *pfname )
        return pfname;

    return NULL;
}

int winks_ui_imgcacheinit(void)
{
    Winks_ImgdataCache *wk_imgcache_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    wk_imgcache_p = &(osPtr->wk_imgcache);
    
    if(wk_imgcache_p->pCache == NULL)
    {
        wk_imgcache_p->pCache = NULL;
        wk_imgcache_p->remain = WINK_IMG_CACHELEN;
    }
    else
    {
        ++wk_imgcache_p->resCnt;
    }

    return 0;
}

int winks_ui_imgcachedst(void)
{
    Winks_Img2Cstr* ptmp = NULL;

    Winks_ImgdataCache *wk_imgcache_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    wk_imgcache_p = &(osPtr->wk_imgcache);
    
    if(wk_imgcache_p->resCnt)
    {
        --wk_imgcache_p->resCnt;
    }
    else
    {
        ptmp = wk_imgcache_p->pCache;
        while( ptmp )
        {
            wk_imgcache_p->pCache = wk_imgcache_p->pCache->pnext;
            Winks_salFree( ptmp );
            ptmp = wk_imgcache_p->pCache;
        }
        wk_imgcache_p->remain = WINK_IMG_CACHELEN;
    }

    return 0;
}

static int winks_ui_imgcachegarbage( int len )
{
    Winks_Img2Cstr* ptmp = NULL;

    Winks_ImgdataCache *wk_imgcache_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    wk_imgcache_p = &(osPtr->wk_imgcache);
    
    ptmp = wk_imgcache_p->pCache;

    while( len > 0 )
    {
        if( ptmp == NULL )
        {
            Winks_printf( " WK winks_ui_imgcachegarbage failure\r\n " );
            return -1;
        }
        len -= ptmp->flen;
        wk_imgcache_p->remain += ptmp->flen;
        wk_imgcache_p->pCache = wk_imgcache_p->pCache->pnext;
        Winks_salFree( ptmp );
        ptmp = wk_imgcache_p->pCache;
    }

    return 0;
}

static Winks_Img2Cstr* winks_ui_imgcacheadd( FILE* pfile, const char* pname )
{
    char* pfname = NULL;
    Winks_Img2Cstr* ptmp = NULL;
    Winks_Img2Cstr* pnew = NULL;
    int flen = 0, ret = 0;

    Winks_ImgdataCache *wk_imgcache_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    wk_imgcache_p = &(osPtr->wk_imgcache);
    
    pfname = winks_ui_getfname( pname );

    if( (ret = Winks_fGetFileSize(pfile, (unsigned long *)(&flen))) != 0 || flen < 0 )
    {
        return NULL;
    }
    if( wk_imgcache_p->remain < flen )
    {
        if( winks_ui_imgcachegarbage( flen - wk_imgcache_p->remain ) < 0 )
        {
            return NULL;
        }
    }

    pnew = Winks_salAlloc( sizeof(Winks_Img2Cstr) + ((strlen(pfname) + 1 + 3) & 0xfffffffc) + flen );
    Winks_mem_set( pnew, 0, sizeof(Winks_Img2Cstr) + ((strlen(pfname) + 1 + 3) & 0xfffffffc) + flen );
    pnew->fname = (char* )(pnew + 1);
    pnew->fdata = (unsigned char* )((char* )(pnew + 1) + ((strlen(pfname) + 1 + 3) & 0xfffffffc));
    pnew->flen = flen;
    Winks_mem_cpy( pnew->fname, pfname, strlen(pfname) );
    Winks_fseek( pfile, 0, WINKS_SEEK_SET );

    Winks_fread( pnew->fdata, pnew->flen, 1, pfile );

    ptmp = wk_imgcache_p->pCache;

    if( ptmp == NULL )
        wk_imgcache_p->pCache = pnew;
    else
    {
        while( ptmp->pnext )
            ptmp = ptmp->pnext;
        ptmp->pnext = pnew;
    }
    wk_imgcache_p->remain -= flen;

    return pnew;
}

static Winks_Img2Cstr* winks_ui_getimagefcache( const char* pname )
{
    char* pfname = (char* )pname;
    Winks_Img2Cstr* ptmp = NULL;

    Winks_ImgdataCache *wk_imgcache_p = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    wk_imgcache_p = &(osPtr->wk_imgcache);
    
    ptmp = wk_imgcache_p->pCache;

    pfname = winks_ui_getfname( pname );

    while( ptmp )
    {
        if( stricmp( ptmp->fname, pfname ) == 0 )
        {
            return (Winks_Img2Cstr* )ptmp;
        }
        ptmp = ptmp->pnext;
    }
#ifdef WINKS_SIMULATOR
    Winks_printf( "WK FILE %s find failure, not in our cache\r\n", pname );
#endif
    return NULL;
}

static Winks_Img2Cstr* winks_ui_getimagefres( const char* pname )
{
    char* pfname = (char* )pname;
    int i = 0;

    pfname = winks_ui_getfname( pname );

    while( Winks_GloImgRes[i] )
    {
        if( stricmp( Winks_GloImgRes[i]->fname, pfname ) == 0 )
        {
            return (Winks_Img2Cstr* )Winks_GloImgRes[i];
        }
        i++;
    }
#ifdef WINKS_SIMULATOR
    Winks_printf( "WK FILE %s find failure, not in our resource\r\n", pname );
#endif
    return NULL;
}

static int winks_ui_createfile( FILE* pfile, const char* pname )
{
    Winks_Img2Cstr* pimg = NULL;
    int iRet = 0;

    if( (pimg = winks_ui_getimagefres(pname)) == NULL )
    {
    #ifdef WINKS_SIMULATOR
        Winks_printf( "WK FILE %s find failure, not in our resource\r\n", pname );
    #endif
        return -1;
    }
    else
    {
        iRet = Winks_fwrite( pimg->fdata, 1, pimg->flen, pfile );
        if(iRet == 0)
            return -1;
    }

    return 0;

}

static int winks_ui_getftype( const char* pname )
{
    char* pext = (char* )pname;
    int i = 0;

    while( *pext )
    {
        if( *pext == '.' )
        {
            pext ++;
            break;
        }
        pext ++;
    }

    while( wk_ImgType[i].extname )
    {
        if( stricmp( pext, wk_ImgType[i].extname ) == 0 )
            return wk_ImgType[i].type;
        i++;
    }

    return WINKS_IMG_TYPE_INVALID;

}

int winks_ui_getimgdata( const char* pname, char** ppdata, int* plen, int* ptype )
{
    Winks_Img2Cstr* pimg = NULL;
    FILE* pfile = NULL;
    unsigned long error = 0;

    /* In getimg data, we assume two conditions:
        1. All img must can find from our res.
        2. Add in cache must success */

    /* get file type */
    if( (*ptype = winks_ui_getftype( pname )) == WINKS_IMG_TYPE_INVALID )
        return -1;

    /* 1. We find it from cache, success */
    if( (pimg = winks_ui_getimagefcache(pname)) != NULL )
    {
        *ppdata = (char* )(pimg->fdata);
        *plen = pimg->flen;

        return 0;
    }

    /* 2. We check if can get file data */
#ifdef WINKS_SIMULATOR
    Winks_printf( "WK draw image from call fopen for read %s\r\n", pname );
#endif
    if( (pfile = Winks_fopen( pname, "r" )) == NULL )
    {
        /* 3. We get file data fail, no file*/
        //if(Winks_fexist(pname) != 0)
        {
        #ifdef WINKS_SIMULATOR
            Winks_printf( "WK draw image from call fopen for write %s\r\n", pname );
        #endif
            if( (pfile = Winks_fopen( pname, "w+" )) != NULL )
            {
                /* 4. We try to create file through our res data */
                if( winks_ui_createfile( pfile, pname ) >= 0 )
                {
                    /* 5. We add it in file and cache, success */
                    if( (pimg = winks_ui_imgcacheadd(pfile, pname)) != NULL )
                    {
                        *ppdata = (char* )(pimg->fdata);
                        *plen = pimg->flen;
                        Winks_fclose( pfile );
                        return 0;
                    }
                    Winks_fclose( pfile );
                }
                else
                {
                    Winks_fclose( pfile );
                    Winks_fdelete((char *)pname);
                }
            }
        }
        /* 6. get file fail, file system have some error, get it from our res */
        if( (pimg = winks_ui_getimagefres(pname)) != NULL )
        {
            *ppdata = (char* )(pimg->fdata);
            *plen = pimg->flen;

            return 0;
        }
        else
        {
            /* 7. get file fail, file system have some error, get it from our res failure, give up*/
            Winks_printf( "WK FILE %s find failure, not in our resource, give up\r\n", pname );
            return -1;
        }
    }
    else
    {

        if( (pimg = winks_ui_imgcacheadd(pfile, pname)) != NULL )
        {
            /* 8. get it from file, add it in cache, success */
            *ppdata = (char* )(pimg->fdata);
            *plen = pimg->flen;
            Winks_fclose( pfile );
            return 0;
        }
        else
        {
            /* 8. get it from file, add it in cache failure, give up */
            Winks_fclose( pfile );
            Winks_printf( "WK FILE %s add in cache failure, give up\r\n", pname );
            return -1;
        }
    }

    return -1;
}

int winks_ui_initskinimag( void )
{
    char fpath[100];
    int i = 0;
    FILE *pfile = NULL;
    int iRet = 0;
    Winks_Img2Cstr* pimg = NULL;

    Winks_mkdir(WINKS_SKIN_FILE_PATH);

    sprintf(fpath,"%s%s%s", WINKS_SKIN_FILE_PATH, WINKS_PATH_SPACE, Winks_GloImgRes[0]->fname);

    if(Winks_fexist(fpath) == -1)/*file not exist.*/
    {
        while( Winks_GloImgRes[i] )
        {
            pimg = (Winks_Img2Cstr*)(Winks_GloImgRes[i]);

            sprintf(fpath,"%s%s%s",WINKS_SKIN_FILE_PATH, WINKS_PATH_SPACE, pimg->fname);

            Winks_fdelete(fpath);
            if((pfile = Winks_fopen( fpath, "w+" ))!= NULL )
            {
                iRet = Winks_fwrite( pimg->fdata, 1, pimg->flen, pfile );
                Winks_fclose(pfile);
                if(iRet == 0)
                {
                    Winks_fdelete(fpath);
                    return -1;
                }
            }
            i++;
        }
    }
    return 0;
}
#endif

#if 1
/***************************************
**应用的调用机制
 ****************************************/
static int Winks_PostMsgToUiExt(Winks_Hwnd_e opt, unsigned long msg, void *data, unsigned long size)
{
    WINKS_CHN_ID *pchn = NULL;
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);

    if(opt == WINKS_MINGUI_HWND_USER)
        pchn = &(osPtr->uiChn);
    else if(opt == WINKS_MINGUI_HWND_CALL)
        pchn = &(osPtr->uiCcShowChn);
    else
    {
        Winks_fail("Unknow Hwnd type..\r\n");
        return WINKS_RETURN_FAILURE;
    }
    Winks_assert(*pchn != WINKS_CHN_NULL);
    Winks_assert(*pchn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[*pchn] != NULL);
    Winks_assert(msg != 0);
    Winks_assert(size <= WINKS_MESSAGE_BODY_MAX_SIZE);

    switch(msg)
    {
        case WINKS_MSG_UI_KEY_EVENT:
//        case WINKS_MSG_UI_PEN_EVENT:
        case WINKS_MSG_UI_CREATE_APP:
        case WINKS_MSG_UI_PAUSE_RESUME_APP:
        case WINKS_MSG_UI_DESTROY_APP:
        case WINKS_MSG_UI_PAINT_EVENT:
        {
            (*(osPtr->osalCnhList[*pchn]))(msg, data, size);
            return WINKS_RETURN_SUCCESS;
        }
        default:
            return Winks_PostMsg(*pchn, msg, data, size);
    }
}

int Winks_PostMsgToUiCcShow(unsigned long msg, void *data, unsigned long size)
{
    return Winks_PostMsgToUiExt(WINKS_MINGUI_HWND_CALL, msg, data, size);
}

int Winks_PostMsgToUi(unsigned long msg, void *data, unsigned long size)
{
    return Winks_PostMsgToUiExt(WINKS_MINGUI_HWND_USER, msg, data, size);
}

/*检查keydown\keyup的匹配问题. 匹配返回0,不匹配返回-1.*/
static int winks_KeyMatch(Winks_MsgBody_KeyEvent_s keyEvent)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(keyEvent.flag == WINKS_KEY_FLAG_KEYDOWN)
    {
        osPtr->keyCode = keyEvent.keyCode;
        return WINKS_RETURN_SUCCESS;
    }
    else if(keyEvent.keyCode == osPtr->keyCode)
    {
        if(keyEvent.flag == WINKS_KEY_FLAG_KEYUP)
        {
            osPtr->keyCode = WINKS_KEY_NULL;
        }
        return WINKS_RETURN_SUCCESS;
    }
    else
    {
        return WINKS_RETURN_FAILURE;
    }
}

/*检查pendown\penup的匹配问题. 匹配返回0,不匹配返回-1.*/
static int winks_PenMatch(Winks_MsgBody_PenEvent_s penEvent)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    if(penEvent.flag == WINKS_PEN_FLAG_PENDOWN)
    {
        osPtr->penCode = 1;
        return WINKS_RETURN_SUCCESS;
    }
    else if(osPtr->penCode == 1)
    {
        if(penEvent.flag == WINKS_PEN_FLAG_PENUP)
        {
            osPtr->penCode = 0;
        }
        return WINKS_RETURN_SUCCESS;
    }
    else
    {
        return WINKS_RETURN_FAILURE;
    }
}

int Winks_ProcTestMode(Winks_MsgBody_KeyEvent_s keyEvent)
{
    char c = '\0';
    
    if(keyEvent.flag == WINKS_KEY_FLAG_KEYUP)
    {
        winks_OsalContext_s *osal_p = winks_GetOsalContextPtr();
        switch(keyEvent.keyCode)
        {
        case WINKS_KEY_1: c = '1'; break;
        case WINKS_KEY_2: c = '2'; break;
        case WINKS_KEY_3: c = '3'; break;
        case WINKS_KEY_4: c = '4'; break;
        case WINKS_KEY_5: c = '5'; break;
        case WINKS_KEY_6: c = '6'; break;
        case WINKS_KEY_7: c = '7'; break;
        case WINKS_KEY_8: c = '8'; break;
        case WINKS_KEY_9: c = '9'; break;
        case WINKS_KEY_0: c = '0'; break;
        case WINKS_KEY_STAR:c = '*'; break;
        case WINKS_KEY_HASH:c = '#'; break;
        default: c = '-'; break;
        }
        if(strlen(osal_p->keyStack) < WINKS_TEST_KEY_HISTORY_STACK_LEN)
        {
            sprintf(osal_p->keyStack,"%s%c",osal_p->keyStack, c);
        }
        else
        {
            strncpy(osal_p->keyStack, osal_p->keyStack+1, WINKS_TEST_KEY_HISTORY_STACK_LEN-1);
            osal_p->keyStack[WINKS_TEST_KEY_HISTORY_STACK_LEN-1] = c;
        }
        osal_p->keyStack[WINKS_TEST_KEY_HISTORY_STACK_LEN] = 0;

        if(strncmp(osal_p->keyStack,WINKS_TEST_KEY_STR,strlen(WINKS_TEST_KEY_STR)) == 0)
        {
            /*WINKS_TEST_KEY_BASE_LEN == WINKS_TEST_KEY_HISTORY_STACK_LEN-WINKS_TEST_KEY_WORDS_LEN*/
            char* p = &osal_p->keyStack[WINKS_TEST_KEY_BASE_LEN];
            Winks_printf("Call Winks_DoTestMode[%s]\n", osal_p->keyStack);
            Winks_ProEventWaitingQueue(WINKS_EVENT_TEST_KET, WINKS_MSG_TEST, (void *)p, strlen(p)+1);
            //Winks_TestModeManager(&(osal_p->keyStack[9]));
        }

    }
    return WINKS_RETURN_SUCCESS;
}

/*键盘处理*/
int winks_keyEvent(Winks_Hwnd_e opt, Winks_MsgBody_KeyEvent_s keyEvent)
{
    if(winks_KeyMatch(keyEvent) == WINKS_RETURN_SUCCESS)
    {
    #ifdef WINKS_DEBUG
        {
        unsigned long val = 0;
        Winks_GetOsalTestBitFlag(3, &val);
        if(val)
        {
            Winks_printf("key[%d][%d]\n", keyEvent.keyCode, keyEvent.flag);
        }
        }
    #endif
        Winks_ProcTestMode(keyEvent);
        Winks_PostMsgToUiExt(opt, WINKS_MSG_UI_KEY_EVENT, &keyEvent, sizeof(Winks_MsgBody_KeyEvent_s));
    }
    #ifdef WINKS_DEBUG
    else
    {
        Winks_printf("WARNING: unmatch key event. [%d][%d]\n", keyEvent.keyCode, keyEvent.flag);
    }
    #endif
    return WINKS_RETURN_SUCCESS;
}

/*触摸屏处理*/
int winks_penEvent(Winks_Hwnd_e opt, Winks_MsgBody_PenEvent_s penEvent)
{
    if(winks_PenMatch(penEvent) == WINKS_RETURN_SUCCESS)
    {
        Winks_PostMsgToUiExt(opt, WINKS_MSG_UI_PEN_EVENT, &penEvent, sizeof(penEvent));
    }
    #ifdef WINKS_DEBUG
    else
    {
        Winks_printf("WARNING: unmatch pey event. [%d]\n", penEvent.flag);
    }
    #endif
    return WINKS_RETURN_SUCCESS;
}

/*屏幕重绘处理*/
int winks_PaintEvent(Winks_Hwnd_e opt, Winks_Rect_s rtc)
{
    int iRet = 0;
    iRet = Winks_PostMsgToUiExt(opt, WINKS_MSG_UI_PAINT_EVENT, &rtc, sizeof(Winks_Rect_s));
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    return WINKS_RETURN_SUCCESS;
}

/*向UI发送应用启动消息*/
int Winks_AppCreateMsgToUI( Winks_Hwnd_e opt )
{
    int iRet = 0;
    iRet = Winks_PostMsgToUiExt(opt, WINKS_MSG_UI_CREATE_APP, NULL, 0);
    Winks_assert(iRet== WINKS_RETURN_SUCCESS);
    return WINKS_RETURN_SUCCESS;
}

/*向UI发送应用退出消息*/
int Winks_AppDestroyMsgToUI( Winks_Hwnd_e opt )
{
    int iRet = 0;
    iRet = Winks_PostMsgToUiExt(opt, WINKS_MSG_UI_DESTROY_APP, NULL, 0);
    Winks_assert(iRet== WINKS_RETURN_SUCCESS);
    return WINKS_RETURN_SUCCESS;
}

/*向UI发送应用挂起回复消息*/
int Winks_AppPauseResumeMsgToUI( Winks_Hwnd_e opt,  WinksAppPauseResume_e flag)
{
    int iRet = 0;
    iRet = Winks_PostMsgToUiExt(opt, WINKS_MSG_UI_PAUSE_RESUME_APP, &flag, sizeof(WinksAppPauseResume_e));
    Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    return WINKS_RETURN_SUCCESS;
}

/*用于记录应用的句柄*/
int Winks_RegisterHwnd(Winks_Hwnd_e opt, void* hWnd)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    if(opt == WINKS_MINGUI_HWND_USER)
    {
        osPtr->hWnd1 = hWnd;
    }
    else if(opt == WINKS_MINGUI_HWND_CALL)
    {
        osPtr->hWnd2 = hWnd;
    }
    else
    {
        Winks_printf("Unknow type Winks_Hwnd_e[%d].\n",opt);
        Winks_fail("");
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/*用于获取应用的句柄*/
int Winks_GetHwnd(Winks_Hwnd_e opt, void* *hWnd)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    if(opt == WINKS_MINGUI_HWND_USER)
    {
        *hWnd =  osPtr->hWnd1;
    }
    else if(opt == WINKS_MINGUI_HWND_CALL)
    {
        *hWnd = osPtr->hWnd2;
    }
    else
    {
        Winks_printf("Unknow type Winks_Hwnd_e[%d].\n",opt);
        Winks_fail("");
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/*设置应用的运行状态*/
int winks_SetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e focesFlag)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    if(opt == WINKS_MINGUI_HWND_USER)
    {
        osPtr->hd1GetFocus = focesFlag;
    }
    else if(opt == WINKS_MINGUI_HWND_CALL)
    {
        osPtr->hd2GetFocus = focesFlag;
    }
    else
    {
        Winks_printf("Unknow type Winks_Hwnd_e[%d].\n",opt);
        Winks_fail("");
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/*获取应用的运行状态*/
int winks_GetHwndFocusFlag(Winks_Hwnd_e opt, WinksAppState_e *focesFlag)
{

    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    if(opt == WINKS_MINGUI_HWND_USER)
    {
        *focesFlag =  osPtr->hd1GetFocus;
    }
    else if(opt == WINKS_MINGUI_HWND_CALL)
    {
        *focesFlag = osPtr->hd2GetFocus;
    }
    else
    {
        Winks_printf("Unknow type Winks_Hwnd_e[%d].\n",opt);
        Winks_fail("");
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/*启动应用处理*/
int winks_procLoadApp(Winks_Hwnd_e opt, void* hWnd)
{
    /*注册应用的句柄*/
    Winks_RegisterHwnd(opt, (void*)hWnd);
    /*注册应用的运行状态*/
    winks_SetHwndFocusFlag(opt, WINKS_APP_STATE_RUN);
    /*消息通知UI*/
    Winks_AppCreateMsgToUI( opt );
    return WINKS_RETURN_SUCCESS;
}

/*暂停后重新启动应用处理*/
int winks_procResumeApp(Winks_Hwnd_e opt)
{
    /*注册应用的运行状态*/
    winks_SetHwndFocusFlag(opt, WINKS_APP_STATE_RUN);
    /*消息通知UI*/
    Winks_AppPauseResumeMsgToUI(opt,WINKS_APP_RESUME);
    return WINKS_RETURN_SUCCESS;
}


/*暂停应用处理*/
int winks_procPauseApp(Winks_Hwnd_e opt)
{
    /*置应用为挂起状态*/
    winks_SetHwndFocusFlag(opt, WINKS_APP_STATE_SUSPEND );
    /*消息通知UI*/
    Winks_AppPauseResumeMsgToUI(opt,WINKS_APP_PAUSE);
    return WINKS_RETURN_SUCCESS;
}

/*退出应用处理*/
int winks_procUnloadApp(Winks_Hwnd_e opt)
{
    /*清应用句柄*/
    Winks_RegisterHwnd(opt, WINKS_HANDLER_WINDOWS_NULL);
    /*置应用为停止状态*/
    winks_SetHwndFocusFlag(opt, WINKS_APP_STATE_STOP);
    return WINKS_RETURN_SUCCESS;
}

/*取抽象层API全局变量结构指针*/
void* Winks_GetOsalApiPtr(void **p)
{
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    if(p != (void**)0)
    *p  =  osPtr->osapis;
    return  osPtr->osapis;
}

/*输入法返回函数
    确认flag=1， 取消flag=0
    str 输入法返回的字符串，Unicode编码
    成功返回0，失败返回-1*/
int Winks_EditorCallback(const char *str, int flag)
{
    Winks_EditorRsp_s defEditor;
    Winks_EditorRsp_s *editor = NULL;
    void*p = NULL;
    int sz = 0;
    int iRet = 0;

    Winks_mem_set(&defEditor, 0, sizeof(Winks_EditorRsp_s));
    editor = &defEditor;
    if(flag == 1 && str != NULL)
    {
        sz = WK_STRLEN(str)<<1;
        p = Winks_salAlloc(sizeof(Winks_EditorRsp_s) + sz);
        if(p != NULL)
        {
            Winks_mem_set(p, 0, sizeof(Winks_EditorRsp_s) + sz);
            editor = (Winks_EditorRsp_s *)p;
            editor->result = 1;
            Winks_mem_cpy(editor->data, str, sz);
        }
    }
    iRet = Winks_PostMsgToUi(WINKS_MSG_UI_EDITOR_RSP, editor, sizeof(Winks_EditorRsp_s) + sz);
    if(editor != &defEditor)
        Winks_salFree(editor);
    return iRet;
}

/*注册UI的消息通道到抽象层.*/
static int winks_RegisterUiChnExt(Winks_Hwnd_e opt, WINKS_CHN_ID chn)
{
    WINKS_CHN_ID *pchn;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    
    Winks_assert(osPtr->osalHandlerStart == WINKS_OSALHANDER_RUNNING);
    Winks_assert(chn != WINKS_CHN_NULL);
    Winks_assert(chn < WINKS_OSAL_CHN_MAXNUM);
    Winks_assert(osPtr->osalCnhList[chn] != NULL);
    

    if(opt == WINKS_MINGUI_HWND_USER)
        pchn = &(osPtr->uiChn);
    else if(opt == WINKS_MINGUI_HWND_CALL)
        pchn = &(osPtr->uiCcShowChn);
    else
    {
        Winks_fail("Unknow Hwnd type.\r\n");
        return WINKS_RETURN_FAILURE;
    }
        
    if(*pchn != WINKS_CHN_NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    else
    {
        *pchn = chn;
        return WINKS_RETURN_SUCCESS;
    }
}

int Winks_RegisterUiCcShowChn(WINKS_CHN_ID chn)
{
    return winks_RegisterUiChnExt(WINKS_MINGUI_HWND_CALL, chn);
}

int Winks_RegisterUiChn(WINKS_CHN_ID chn)
{
    return winks_RegisterUiChnExt(WINKS_MINGUI_HWND_USER, chn);
}

#endif
/*
int Winks_mem_cmp(void* data1, void* data2, unsigned long size)
{
    const unsigned char *p = (const unsigned char *)data1;
    const unsigned char *q = (const unsigned char *)data2;
    unsigned long i = 0;
    if((data1 == NULL) || (data2 == NULL))
    {
        Winks_assert(data1 != NULL);
        Winks_assert(data2 != NULL);
        if(data1 == NULL)
        {
            if((data2 == NULL))
                return 0;
            else
                return -1;
        }
        else
            return 1;
    }

    for(i = 0;i<size,p++,q++; ++i)
    {
        if(*p == *q)
        {
            continue;
        }
        else if(*p > *q)
            return 1;
        else
            return -1;
    }
    return 0;
}
*/

int Winks_getversionExt(char *buff)
{
    const char *mstr[]={
        "Jan"
        ,"Feb"
        ,"Mar"
        ,"Apr"
        ,"May"
        ,"Jun"
        ,"Jul"
        ,"Aug"
        ,"Sep"
        ,"Oct"
        ,"Nov"
        ,"Dec"
        ,NULL
    };
    long y = 0;
    long m = 0;
    long d = 0;
    long h = 0;
    long mi = 0;
    long s = 0;
    const char** mp = mstr;
        
    strcpy(buff, __DATE__);

    while(*mp != NULL)
    {
        ++m;
        if(strncmp(buff, *mp, 3) == 0)
        {
            break;
        }
        ++mp;
    }

    if(m>12)
        return -1;
    
    d = atol(&buff[4]);
    y = atol(&buff[9]);
    
    strcpy(buff, __TIME__);
    h = atol(&buff[0]);
    mi = atol(&buff[3]);
    s = atol(&buff[6]);

    sprintf(buff,"%s.%02ld%02ld%02ld%02ld%02ld", WINKS_MV, y,m,d,h,mi);
    return 0;
}

/*取版本号，flag=1时取全版本号，flag=0取编译版本号。 */
char* Winks_getversion(int flag)
{
    const char *NULSTR = "";
    WINKS_OSAL_CONTEXT_DEF(osPtr);
    if(osPtr==NULL)
        return (char *)NULSTR;
    if(strlen(osPtr->version) == 0)
    {
        if(Winks_getversionExt(osPtr->version) != WINKS_RETURN_SUCCESS)
            Winks_mem_set(osPtr->version, 0, sizeof(osPtr->version));
    }
	if(flag == 0)
		return (char*)(&osPtr->version[strlen(WINKS_MV)]);
	else
	{
		return (char*)(osPtr->version);
	}
}




#ifdef WINKS_SENDSMS_UPGRADE
#define WINKS_SENDSMS_QUEUE_NUM     10              /*短信队列最大长度*/

#ifdef WINKS_DEBUG
int Winks_api_SendSmsUni_print(char *number, char* dataUni, int simNo)
{
	char * p = NULL;
	unsigned long len = 0;
	int iRet = 0;

	len = WK_STRLEN(dataUni)*3+1;
	
	p = Winks_salAlloc(len);
    if(p == NULL)
        return -1;

	iRet = Winks_Unicode2UtfString(dataUni, p, len);
	Winks_printf("%d[%s][%s][%d]\n", iRet,number, p, simNo);

    Winks_salFree(p);
	return 0;
}
#else
#define Winks_api_SendSmsUni_print(a, b, c)
#endif

/*短息发送机的异步回调函数。需要在平台发送短信返回后调用该函数。
 * function: Winks_SendSmsCallBack
 * parameters:
 *   void *number: 短信发送结果。0:失败，1:成功。
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSmsCallBack(int result)
{
    Winks_SmsQueueItem_s * node = NULL;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    Winks_assert(osPtr->smsQueueHead != NULL);
    Winks_assert(osPtr->smsQueueTail != NULL);
    
    if(osPtr->smsQueueHead == osPtr->smsQueueTail)
    {
        Winks_assert(osPtr->smsQueueHead->next == NULL);
        osPtr->smsQueueTail = NULL;
    }
    
    node = osPtr->smsQueueHead;
    osPtr->smsQueueHead = osPtr->smsQueueHead->next;

    //print for test
    Winks_api_SendSmsUni_print(node->number, node->sendData, node->simNo);
    Winks_salFree(node);

    if(osPtr->smsQueueHead != NULL)
    {
        node = osPtr->smsQueueHead;
#ifdef WINKS_DUAL_SIM
        Winks_api_SendSmsUni(node->number, node->sendData, node->simNo);
#else
        Winks_api_SendSmsUni(node->number, node->sendData);
#endif
    }
    return 0;
}

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * function: Winks_SendSmsUni_Ext
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *dataUni: 短信内容，Unicode编码。
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
#ifdef WINKS_DUAL_SIM
int Winks_SendSmsUni_Ext(char *number, char *dataUni, int simNo)
#else
int Winks_SendSmsUni(char *number, char *dataUni)
#endif
{
    unsigned long len  = 0;
    char *p = NULL;
    char *pnum = NULL;
    char *pdata = NULL;
    Winks_SmsQueueItem_s * node = NULL;
    int flag = 0;
    int iRet = 0;
    
    WINKS_OSAL_CONTEXT_DEF(osPtr);

    Winks_assert(number != NULL);
    Winks_assert(dataUni != NULL);
    Winks_assert(strlen(number) > 0);

    len = sizeof(Winks_SmsQueueItem_s) + WK_STRLEN(dataUni)*2 + 2 + strlen(number) + 1; 

    p = Winks_salAlloc(len);
    if(p == NULL)
    {
        return WINKS_RETURN_FAILURE;
    }
    Winks_mem_set(p, 0 ,len);

    node = (Winks_SmsQueueItem_s*)p;
    pdata = p + sizeof(Winks_SmsQueueItem_s);
    pnum = pdata + WK_STRLEN(dataUni)*2 + 2;
    Winks_mem_cpy(pdata, dataUni, WK_STRLEN(dataUni)<<1);
    Winks_mem_cpy(pnum, number, strlen(number));
    node->number = pnum;
    node->sendData = pdata;
#ifdef WINKS_DUAL_SIM
    node->simNo = simNo;
#endif
    node->next = NULL;
    
    
    if(osPtr->smsQueueHead == NULL)
    {
        Winks_assert(osPtr->smsQueueHead == osPtr->smsQueueTail);
        osPtr->smsQueueHead = node;
        flag = 1;
    }

    if(osPtr->smsQueueTail != NULL)
        osPtr->smsQueueTail->next = node;
    osPtr->smsQueueTail = node;


    if(flag)
#ifdef WINKS_DUAL_SIM
        iRet = Winks_api_SendSmsUni(node->number, node->sendData, node->simNo);
#else
        iRet = Winks_api_SendSmsUni(node->number, node->sendData);
#endif
    return iRet;
}

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * function: Winks_SendSms_Ext
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *data: 短信内容，utf8编码
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
#ifdef WINKS_DUAL_SIM
int Winks_SendSms_Ext(char *number, char *data, int simNo)
#else
int Winks_SendSms(char *number, char *data)
#endif
{
    unsigned long len = 0;
    char * p = NULL;
    int iRet = 0;
    char tmp[2] = {0};
    
    Winks_assert(number != NULL);
    Winks_assert(data != NULL);
    Winks_assert(strlen(number) > 0);


    if(strlen(data) > 0)
    {
        len = strlen(data)<<1+2;

        p = (char *)Winks_salAlloc(len);
        if(p == NULL)
        {
            return WINKS_RETURN_FAILURE;
        }
        Winks_mem_set(p, 0 ,len);
    }
    else
    {
        p = tmp;
    }

        Winks_Utf2UnicodeString((const char *)data, p, len-2);

#ifdef WINKS_DUAL_SIM
    iRet = Winks_SendSmsUni_Ext(number, p, simNo);
#else
    iRet = Winks_SendSmsUni(number, p);
#endif

    if(p != tmp)
        Winks_salFree(p);

    return iRet;
}

#ifdef WINKS_DUAL_SIM
/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * 用默认的SIM卡发送短信。
 * function: Winks_SendSms
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *dataUni: 短信内容，Unicode编码。
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSmsUni(char *number, char *dataUni)
{
    int simNo = 1;
    simNo = Winks_GetActiveSim();
    return Winks_SendSmsUni_Ext(number, dataUni, simNo);
}

/*短息发送机。内部维护一个排队队列，然后顺序调用平台的短信发送接口(这是一个同步转异步的过程)。
 * 用默认的SIM卡发送短信。
 * function: Winks_SendSms
 * parameters:
 *   void *number: 短信接受者、网关
 *   char *data: 短信内容，utf8编码
 *   int simNo: 双卡手机，指定发短信的sim卡。1--sim卡1;2--sim卡2
 * return:
 *   成功返回:0
 *   失败返回:-1
 */
int Winks_SendSms(char *number, char *dataUni)
{
    int simNo = 1;
    simNo = Winks_GetActiveSim();
    return Winks_SendSms_Ext(number, dataUni, simNo);
}
#endif

#endif
