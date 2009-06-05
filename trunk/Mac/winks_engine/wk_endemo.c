/*
File 		:wk_demo.c
Description :UI manager.
Author		:Huaxm
Create Time :28/03/2008
*/
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_http.h"
#include "wk_engin.h"


/**********************************
                UI DEMO
***********************************/
typedef struct tagwinks_UiContext_s
{
    WINKS_CHN_ID uiChn;
    WINKS_TIMER_ID uiTimer;
    WINKS_TIMER_ID uiTimerB;
    unsigned char winksCallAppRun;
    unsigned char flag;
    unsigned char pic;
}winks_UiContext_s;


#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
    winks_UiContext_s Winks_UiContext;
#endif

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_UI_CONTEXT_DEF(uiPtr)  \
    winks_UiContext_s *uiPtr = get_module_glob(WK_MODULE_UI);
#else
#define WINKS_UI_CONTEXT_DEF(osPrt)  \
    winks_UiContext_s *uiPtr = &Winks_UiContext;
#endif

#ifdef HUAXMTEST
static    winks_UiContext_s *uiPtr = NULL;
#endif

typedef struct tagWinks_Eng2UIMsgInfo_s
{
    int Msg;
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    unsigned short  ID_Info[36];
    unsigned short  StrInfo[128];
#else
    unsigned short * ID_Info;
    unsigned short * StrInfo;
#endif
}Winks_Eng2UIMsgInfo_s;

static const Winks_Eng2UIMsgInfo_s Eng2UiMsgInfo[] = 
{
    {WINKS_MSG_ENG2UI_NONE           , L"WINKS_MSG_ENG2UI_NONE"           , L"无关消息"},
    
/*出错消息*/
    {WINKS_MSG_ENG2UI_SVR_NO_ANSWER  , L"WINKS_MSG_ENG2UI_SVR_NO_ANSWER"  , L"服务器无应答"},
    {WINKS_MSG_ENG2UI_UNREG_USER     , L"WINKS_MSG_ENG2UI_UNREG_USER"     , L"非注册用户限制"},
    {WINKS_MSG_ENG2UI_ROAMING_LIMIT  , L"WINKS_MSG_ENG2UI_ROAMING_LIMIT"  , L"漫游限制 "},
    {WINKS_MSG_ENG2UI_ENG_BUSY       , L"WINKS_MSG_ENG2UI_ENG_BUSY"       , L"引擎忙"},
    {WINKS_MSG_ENG2UI_DIAL_INT       , L"WINKS_MSG_ENG2UI_DIAL_INT"       , L"被电话中断"},
    {WINKS_MSG_ENG2UI_STORAGE_LIMIT  , L"WINKS_MSG_ENG2UI_STORAGE_LIMIT"  , L"存储空间不足"},
    {WINKS_MSG_ENG2UI_CC_CLOSED      , L"WINKS_MSG_ENG2UI_CC_CLOSED"      , L"彩像软件关闭 "},
#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_MCARD_NOT_EXIST, L"WINKS_MSG_ENG2UI_MCARD_NOT_EXIST", L"存储卡不存在"},
#endif
    {WINKS_MSG_ENG2UI_SYS_FILES_ERR  , L"WINKS_MSG_ENG2UI_SYS_FILES_ERR"  , L"系统文件被破坏"},
    {WINKS_MSG_ENG2UI_CC_FILES_ERR   , L"WINKS_MSG_ENG2UI_CC_FILES_ERR"   , L"彩像文件被破坏"},
#ifdef WINKS_DUAL_SIM
    {WINKS_MSG_ENG2UI_SIM_SEL_ERR    , L"WINKS_MSG_ENG2UI_SIM_SEL_ERR"    , L"用户未选择SIM卡"},
    {WINKS_MSG_ENG2UI_SIM_UNUSABLE   , L"WINKS_MSG_ENG2UI_SIM_UNUSABLE"   , L"用户选择的Sim卡不可用"},
#endif
    {WINKS_MSG_ENG2UI_NO_SUBSCRIPTION, L"WINKS_MSG_ENG2UI_NO_SUBSCRIPTION", L"不是订阅用户"},
    {WINKS_MSG_ENG2UI_USER_DISABLED  , L"WINKS_MSG_ENG2UI_USER_DISABLED"  , L"用户被禁用"},
    {WINKS_MSG_ENG2UI_NO_NUMBER      , L"WINKS_MSG_ENG2UI_NO_NUMBER"      , L"没有电话号码需要更新"},

    {WINKS_MSG_ENG2UI_PROT_NO        , L"WINKS_MSG_ENG2UI_PROT_NO"        , L"服务器下发协议号不匹配"},
    {WINKS_MSG_ENG2UI_HTTP_ERRCODE   , L"WINKS_MSG_ENG2UI_HTTP_ERRCODE"   , L"收到Http错误码"},
    {WINKS_MSG_ENG2UI_SRV_ERRCODE    , L"WINKS_MSG_ENG2UI_SRV_ERRCODE"    , L"收到服务器返回错误码"},
    {WINKS_MSG_ENG2UI_SRV_ERRDATA    , L"WINKS_MSG_ENG2UI_SRV_ERRDATA"    , L"服务器返回数据有错误"},
    {WINKS_MSG_ENG2UI_MEM            , L"WINKS_MSG_ENG2UI_MEM"            , L"内存申请出错"},
    {WINKS_MSG_ENG2UI_FS             , L"WINKS_MSG_ENG2UI_FS"             , L"文件系统出错"},
    {WINKS_MSG_ENG2UI_HTTP           , L"WINKS_MSG_ENG2UI_HTTP"           , L"HTTP错误"},
    {WINKS_MSG_ENG2UI_HTTP_TIMEOUT   , L"WINKS_MSG_ENG2UI_HTTP_TIMEOUT"   , L"HTTP超时错误"},
    {WINKS_MSG_ENG2UI_PROT_PACK      , L"WINKS_MSG_ENG2UI_PROT_PACK"      , L"协议打包"},
    {WINKS_MSG_ENG2UI_PROT_PARSE     , L"WINKS_MSG_ENG2UI_PROT_PARSE"     , L"协议解析"},
    {WINKS_MSG_ENG2UI_XML_GEN        , L"WINKS_MSG_ENG2UI_XML_GEN"        , L"生成xml出错"},
    {WINKS_MSG_ENG2UI_XML_PARSE      , L"WINKS_MSG_ENG2UI_XML_PARSE"      , L"xml解析出错"},
    {WINKS_MSG_ENG2UI_APP_ERR        , L"WINKS_MSG_ENG2UI_APP_ERR"        , L"应用调用错"},
    {WINKS_MSG_ENG2UI_ENG_ERR        , L"WINKS_MSG_ENG2UI_ENG_ERR"        , L"引擎调用错"},
    
    {WINKS_MSG_ENG2UI_UNKNOWN        , L"WINKS_MSG_ENG2UI_UNKNOWN"        , L"未知错误"},

/*1 注册及初始化过程*/
    {WINKS_MSG_ENG2UI_SETUP_SVR_FAIL , L"WINKS_MSG_ENG2UI_SETUP_SVR_FAIL" , L"连接配置服务器失败"},
    {WINKS_MSG_ENG2UI_REG_FAIL       , L"WINKS_MSG_ENG2UI_REG_FAIL"       , L"注册失败"},
    {WINKS_MSG_ENG2UI_ULPB_FAIL      , L"WINKS_MSG_ENG2UI_ULPB_FAIL"      , L"上传电话簿失败"},
    {WINKS_MSG_ENG2UI_INIT_FAIL      , L"WINKS_MSG_ENG2UI_INIT_FAIL"      , L"初始化失败"},
                                                                       
    {WINKS_MSG_ENG2UI_SETUP_SVR      , L"WINKS_MSG_ENG2UI_SETUP_SVR"      , L"正在连接配置服务器..."},
    {WINKS_MSG_ENG2UI_SETUP_SVR_OK   , L"WINKS_MSG_ENG2UI_SETUP_SVR_OK"   , L"连接配置服务器成功"},
    {WINKS_MSG_ENG2UI_SMS_REG        , L"WINKS_MSG_ENG2UI_SMS_REG"        , L"未注册，需要发送注册短信"},
    {WINKS_MSG_ENG2UI_WAIT_PUSH      , L"WINKS_MSG_ENG2UI_WAIT_PUSH"      , L"未注册，等待接收push"},
    {WINKS_MSG_ENG2UI_REG            , L"WINKS_MSG_ENG2UI_REG"            , L"正在注册..."},
    {WINKS_MSG_ENG2UI_REG_OK         , L"WINKS_MSG_ENG2UI_REG_OK"         , L"注册OK"},
    {WINKS_MSG_ENG2UI_ULPB_CONFIRM   , L"WINKS_MSG_ENG2UI_ULPB_CONFIRM"   , L"用户确认是否上传电话簿"},
    {WINKS_MSG_ENG2UI_ULPB           , L"WINKS_MSG_ENG2UI_ULPB"           , L"正在上传电话簿..."},
    {WINKS_MSG_ENG2UI_ULPB_OK        , L"WINKS_MSG_ENG2UI_ULPB_OK"        , L"上传电话簿完毕"},
    {WINKS_MSG_ENG2UI_INIT           , L"WINKS_MSG_ENG2UI_INIT"           , L"正在初始化..."},
    {WINKS_MSG_ENG2UI_INIT_OK        , L"WINKS_MSG_ENG2UI_INIT_OK"        , L"初始化完毕"},

/*2 更新好友录*/
    {WINKS_MSG_ENG2UI_FRIEND_FAIL    , L"WINKS_MSG_ENG2UI_FRIEND_FAIL"    , L"更新好友录失败"},
    
    {WINKS_MSG_ENG2UI_FRIEND         , L"WINKS_MSG_ENG2UI_FRIEND"         , L"正在更新好友录..."},
    {WINKS_MSG_ENG2UI_FRIEND_OK      , L"WINKS_MSG_ENG2UI_FRIEND_OK"      , L"好友录更新完毕"},

/*3 上传DIY*/
    {WINKS_MSG_ENG2UI_DIY_FAIL       , L"WINKS_MSG_ENG2UI_DIY_FAIL"       , L"上传DIY失败"},
    {WINKS_MSG_ENG2UI_SCRIPT_FAIL    , L"WINKS_MSG_ENG2UI_SCRIPT_FAIL"    , L"定制彩像失败"},
                                                                  
    {WINKS_MSG_ENG2UI_DIY            , L"WINKS_MSG_ENG2UI_DIY"            , L"正在上传DIY..."},
    {WINKS_MSG_ENG2UI_DIY_OK         , L"WINKS_MSG_ENG2UI_DIY_OK"         , L"上传DIY成功"},
    {WINKS_MSG_ENG2UI_SCRIPT_OK      , L"WINKS_MSG_ENG2UI_SCRIPT_OK"      , L"定制彩像成功"},

/*4 推荐给好友*/
    {WINKS_MSG_ENG2UI_RECOMMEND_FAIL , L"WINKS_MSG_ENG2UI_RECOMMEND_FAIL" , L"推荐失败"},
                                                                      
    {WINKS_MSG_ENG2UI_RECOMMEND      , L"WINKS_MSG_ENG2UI_RECOMMEND"      , L"正在推荐..."},
    {WINKS_MSG_ENG2UI_RECOMMEND_OK   , L"WINKS_MSG_ENG2UI_RECOMMEND_OK"   , L"推荐成功"},

/*5 电话下载*/
    {WINKS_MSG_ENG2UI_CALL_FAIL      , L"WINKS_MSG_ENG2UI_CALL_FAIL"      , L"获取通话用户彩像失败"},
    {WINKS_MSG_ENG2UI_CALL_TIMEOUT   , L"WINKS_MSG_ENG2UI_CALL_TIMEOUT"   , L"获取通话用户彩像超时"},
                                                                    
    {WINKS_MSG_ENG2UI_CALL           , L"WINKS_MSG_ENG2UI_CALL"           , L"正在获取通话用户彩像..."},
    {WINKS_MSG_ENG2UI_CALL_OK        , L"WINKS_MSG_ENG2UI_CALL_OK"        , L"获取通话用户彩像成功"},

/*6 同步本地配置到服务器*/
    {WINKS_MSG_ENG2UI_SETUP_FAIL     , L"WINKS_MSG_ENG2UI_SETUP_FAIL"     , L"同步本地配置失败"},
    {WINKS_MSG_ENG2UI_SRVCFG_FAIL    , L"WINKS_MSG_ENG2UI_SRVCFG_FAIL"    , L"服务器配置失败"},
    {WINKS_MSG_ENG2UI_USERCFG_FAIL   , L"WINKS_MSG_ENG2UI_USERCFG_FAIL"   , L"本地配置失败"},
    {WINKS_MSG_ENG2UI_ULSTAT_FAIL    , L"WINKS_MSG_ENG2UI_ULSTAT_FAIL"    , L"统计信息上传失败"},
    {WINKS_MSG_ENG2UI_ULLOG_FAIL     , L"WINKS_MSG_ENG2UI_ULLOG_FAIL"     , L"log信息上传失败"},
                                                                       
    {WINKS_MSG_ENG2UI_SETUP          , L"WINKS_MSG_ENG2UI_SETUP"          , L"正在同步本地配置..."},
    {WINKS_MSG_ENG2UI_SETUP_OK       , L"WINKS_MSG_ENG2UI_SETUP_OK"       , L"同步本地配置成功"},
    {WINKS_MSG_ENG2UI_SRVCFG_OK      , L"WINKS_MSG_ENG2UI_SRVCFG_OK"      , L"服务器配置成功"},
    {WINKS_MSG_ENG2UI_USERCFG_OK     , L"WINKS_MSG_ENG2UI_USERCFG_OK"     , L"本地配置成功"},
    {WINKS_MSG_ENG2UI_ULSTAT_OK      , L"WINKS_MSG_ENG2UI_ULSTAT_OK"      , L"统计信息上传成功"},
    {WINKS_MSG_ENG2UI_ULLOG_OK       , L"WINKS_MSG_ENG2UI_ULLOG_OK"       , L"log信息上传成功"},

/*7 检查新版本*/
    {WINKS_MSG_ENG2UI_RUPGRADE_FAIL  , L"WINKS_MSG_ENG2UI_RUPGRADE_FAIL"  , L"升级失败"},
                                                                       
    {WINKS_MSG_ENG2UI_RUPGRADE       , L"WINKS_MSG_ENG2UI_RUPGRADE"       , L"正在检查新版本..."},
    {WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW, L"WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW" , L"没有新版本"},
    {WINKS_MSG_ENG2UI_RUPGRADE_NEW   , L"WINKS_MSG_ENG2UI_RUPGRADE_NEW"   , L"有新版本"},
    {WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK, L"WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK", L"新版本下载成功"},
};

#define ENG2UI_MSG_NUM    (sizeof(Eng2UiMsgInfo)/sizeof(Eng2UiMsgInfo[0]))

static unsigned short * winks_EngUiMsgGetIDStr(Winks_EngineToUiReturnMsg *pEng2UIMsg)
{
    int iCnt;
    int Msg = pEng2UIMsg->code;

    for(iCnt=1; iCnt<ENG2UI_MSG_NUM; iCnt++)
    {
        if(Msg == Eng2UiMsgInfo[iCnt].Msg)
            return (unsigned short *)Eng2UiMsgInfo[iCnt].ID_Info;
    }
    return (unsigned short *)Eng2UiMsgInfo[0].ID_Info;
}

static unsigned short * winks_EngUiMsgGetIDInfo(Winks_EngineToUiReturnMsg *pEng2UIMsg)
{
    int iCnt;
    int Msg = pEng2UIMsg->code;

    for(iCnt=1; iCnt<ENG2UI_MSG_NUM; iCnt++)
    {
        if(Msg == Eng2UiMsgInfo[iCnt].Msg)
            return (unsigned short *)Eng2UiMsgInfo[iCnt].StrInfo;
    }
    return (unsigned short *)Eng2UiMsgInfo[0].StrInfo;
}

static int winks_ascii2unicode(unsigned char *pOutBuffer, char *pInBuffer)
{
    int count = -1;
    unsigned short Unicode;
    unsigned char * arrOut;

    while (*pInBuffer != '\0')
    {
        Unicode = (unsigned short)(*pInBuffer);
        arrOut = (unsigned char *)&Unicode;

        pOutBuffer[++count] = arrOut[0];
        pOutBuffer[++count] = arrOut[1];
        pInBuffer++;
    }

    pOutBuffer[++count] = '\0';
    pOutBuffer[++count] = '\0';
    return count + 1;
}

static int winks_UiIdle(unsigned long msg, void *data, unsigned long size)
{
    WINKS_UI_CONTEXT_DEF(uiPtr)
	unsigned char pic = uiPtr->pic;
	switch(msg)
	{
	case WINKS_MSG_CHN_INIT:
	    break;
	case WINKS_MSG_IMSI_REQ:
	    break;
	case WINKS_MSG_PHONE_BOOK_MODIFIED:
        Winks_printf("++++++++++++++++[%s]\n", (char *)data);
	    break;
        

	case WINKS_MSG_TIMER_EXPIRY:
	{
        if(uiPtr->uiTimer == *((WINKS_TIMER_ID*)data))
        {
            Winks_LoadWinksCallApp();
//            app_WinksMenu_create_main();
        }
        else if(uiPtr->uiTimerB == *((WINKS_TIMER_ID*)data))
        {
    		char buff[32];
    		Winks_FormatDateTime(WINKS_DTFRM_YYYYMMDDHH24MISS_7, buff, 32, NULL);
            Winks_printf("UI got EVENT :WINKS_MSG_TIMER_EXPIRY  A[%s][sys_tick:%ld]\n",buff,Winks_GetSysTick());
        }

        break;
	}

	case WINKS_MSG_UI_PAINT_EVENT:
		Winks_printf("WINKS_MSG_UI_PAINT_EVENT:\n");
		break;
        
	case WINKS_MSG_UI_KEY_EVENT:
	{
    #if 1
		Winks_MsgBody_KeyEvent_s keyEvent;
		Winks_mem_cpy(&keyEvent, (Winks_MsgBody_KeyEvent_s *)data, sizeof(keyEvent));
		if(keyEvent.flag<2)
		    Winks_printf("UI get key event !key_code[%d], key_flag[%d] \n", keyEvent.keyCode, keyEvent.flag);
		if(keyEvent.flag == WINKS_KEY_FLAG_KEYDOWN)
		{
			pic = 0;
			switch (keyEvent.keyCode)
			{
			case WINKS_KEY_0:
                Winks_UserRegist();
                break;
			case WINKS_KEY_1:
                Winks_UserSendRegSms(1);
                break;
			case WINKS_KEY_2:
                Winks_UserResetFriendList();
                break;
			case WINKS_KEY_3:/* 补充下载 */
                Winks_GetCCStart(0, 2);
                break;
			case WINKS_KEY_4:
            {
                Winks_DiyInfo_s diy_info;
                Winks_mem_set(&diy_info, 0, sizeof(Winks_DiyInfo_s));
                //diy_info.pnolist = NULL;
                diy_info.action = 1;
                diy_info.pnolist = "13810281692|62530410|15101072205|";
                Winks_mem_cpy(diy_info.path, L"e:\\Images\\Sunset.jpg", 
                          (WK_STRLEN((char *)L"e:\\Images\\Sunset.jpg")+1)<<1);
                Winks_mem_cpy(diy_info.greet, L"upload diy,welcome", 
                             (WK_STRLEN((char *)L"upload diy,welcome")+1)<<1);
                Winks_UserUploadDiyImage(&diy_info);
			}
                break;
            case WINKS_KEY_5:
            {
                int RcmdNum;
                Winks_RecommendInfo_s  RcmdArr[10];

                Winks_mem_set(RcmdArr, 0, sizeof(Winks_RecommendInfo_s)*10);
                for(RcmdNum = 0; RcmdNum < 3; RcmdNum++)
                {
                    Winks_mem_cpy(RcmdArr[RcmdNum].name, L"name", 
                                      (WK_STRLEN((char *)L"name")+1)<<1);
                    sprintf(RcmdArr[RcmdNum].pno, "1311234567%d", RcmdNum);
                }
                Winks_UserRecommend(RcmdNum, RcmdArr);
            }
                break;
        #ifdef WINKS_DEBUG
          #if 0
			case WINKS_KEY_6:
                winks_TstEngAddFriend();
                break;
			case WINKS_KEY_7:
                winks_TstEngDelFriend();
                break;
          #else
			case WINKS_KEY_6:
                winks_TstEngAddUL();
                break;
			case WINKS_KEY_7:
                winks_TstEngDelUL();
                break;
          #endif
        #endif
            case WINKS_KEY_8:
            {
                Winks_SetSettingString(WINKS_SETTING_UID, "987654321");
            }
                break;
			case WINKS_KEY_9:
            {
                static int Flag = 0;

                if(!Flag)
                    Winks_UserOpenWinkCC();
                else
                    Winks_UserCloseWinkCC();
                Flag = ~Flag;
			}
                break;
        #ifdef WINKS_DEBUG
            case WINKS_KEY_STAR:
                Winks_xsalPrintMemInfo();
                break;
            case WINKS_KEY_HASH:
                winks_TstEngPrintAppStack();
                break;
        #endif
            case WINKS_KEY_RSK:
			{
                void *hWnd = NULL;
                Winks_GetHwnd(WINKS_MINGUI_HWND_CALL, &hWnd);
				if(hWnd != NULL)
				{
					Winks_DestroyApp(WINKS_MINGUI_HWND_CALL);
				}
				else
				{
					Winks_DestroyApp(WINKS_MINGUI_HWND_USER);
				}
                break;
			}
			default: break;
			}
		}
    #endif
		break;
	}
	case WINKS_MSG_UI_CREATE_APP:
	{
		Winks_Hwnd_e appFlag;
		Winks_mem_cpy(&appFlag,(Winks_Hwnd_e *)data, sizeof(appFlag));
        
    	Winks_printf("UI get WINKS_MSG_UI_CREATE_APP event msg_data[%d]!\n",appFlag);
		break;	
	}
	case WINKS_MSG_ENGINE_UIRETMSG:
	{
        Winks_EngineToUiReturnMsg UIMsg;

        Winks_mem_cpy(&UIMsg , data, sizeof(Winks_EngineToUiReturnMsg));
		Winks_printf("UI Got return msg:[%d] len=%d\n", UIMsg.code, size-sizeof(int));
    #ifdef WINKS_DEBUG
        winks_PrintEng2UiMsgInfo((Winks_EngineToUiReturnMsg *)data);
    #endif
    #if 1
    {
        unsigned short * str = NULL;
        static unsigned int MsgCnt = 0;
        char Buf[32],Buf1[32];
        
    	Winks_dbuff_lock();

    	winks_ui_fillRect(0, 0, WINKS_SCREEN_WIDTH, WINKS_SCREEN_HEIGHT, 0);
        Winks_UI_set_current_text_color(0xFF000000);
        winks_set_font(WINKS_SMALL_FONT);

        sprintf(Buf1, "%d, %d", MsgCnt, UIMsg.code);
        winks_ascii2unicode((unsigned char *)Buf, Buf1);
        Winks_set_clip_on(0, 0, WINKS_SCREEN_WIDTH, 30);
        winks_ui_textOutlen(0, 0, Buf, WK_STRLEN(Buf)<<1);
        Winks_set_clip_off(0, 0, WINKS_SCREEN_WIDTH, 30);
        MsgCnt++;

        str = winks_EngUiMsgGetIDStr((Winks_EngineToUiReturnMsg *)data);
        Winks_set_clip_on(0, 30, WINKS_SCREEN_WIDTH, 60);
        winks_ui_textOutlen(0, 30, str, WK_STRLEN((char *)str)<<1);
        Winks_set_clip_off(0, 30, WINKS_SCREEN_WIDTH, 60);
        
        str = winks_EngUiMsgGetIDInfo((Winks_EngineToUiReturnMsg *)data);
        Winks_set_clip_on(0,60, WINKS_SCREEN_WIDTH, 120);
        winks_ui_textOutlen(0, 60, str, WK_STRLEN((char *)str)<<1);
        Winks_set_clip_off(0,60, WINKS_SCREEN_WIDTH, 120);
        
        Winks_dbuff_unlockandblt();
    }
    #endif
        break;
	}
    
    case WINKS_MSG_CALL_INCOMING:
    case WINKS_MSG_CALL_DIAL:
    {
        uiPtr->uiTimer = Winks_CreateTimer(
						uiPtr->uiChn, 
						1000, 
						WINKS_TIMER_AUTO_START);
        
    }

	default:
	{
		Winks_printf("winks_UiIdle get unknow msg[%0lx] winksCallAppRun[%d]\n",msg,uiPtr->winksCallAppRun);
		break;
	}
	}
	return 0;
}


void Winks_UI_Init(void)
{
	int iRet = 0;
    winks_UiContext_s *uiPtr = NULL;
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    uiPtr = (winks_UiContext_s *)Winks_salAlloc(sizeof(winks_UiContext_s));
    Winks_assert(uiPtr != NULL);
    set_module_glob(WK_MODULE_UI, (void*)uiPtr);
#else
    uiPtr = &Winks_UiContext;
#endif
    Winks_mem_set(uiPtr, 0, sizeof(winks_UiContext_s));
    
	uiPtr->uiChn = Winks_CreateChn(winks_UiIdle);
	Winks_assert(uiPtr->uiChn != WINKS_CHN_NULL);
    Winks_SetWaitingEvent( uiPtr->uiChn, WINKS_EVENT_OF_ALL_CALL );/*WINKS_EVENT_CALL_COMMING*/

	//UI通道的注册提前
	iRet = Winks_RegisterUiChn(uiPtr->uiChn);
	Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    uiPtr->winksCallAppRun = 0;
	return;
}
