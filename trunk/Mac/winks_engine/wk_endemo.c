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
    {WINKS_MSG_ENG2UI_NONE           , L"WINKS_MSG_ENG2UI_NONE"           , L"�޹���Ϣ"},
    
/*������Ϣ*/
    {WINKS_MSG_ENG2UI_SVR_NO_ANSWER  , L"WINKS_MSG_ENG2UI_SVR_NO_ANSWER"  , L"��������Ӧ��"},
    {WINKS_MSG_ENG2UI_UNREG_USER     , L"WINKS_MSG_ENG2UI_UNREG_USER"     , L"��ע���û�����"},
    {WINKS_MSG_ENG2UI_ROAMING_LIMIT  , L"WINKS_MSG_ENG2UI_ROAMING_LIMIT"  , L"�������� "},
    {WINKS_MSG_ENG2UI_ENG_BUSY       , L"WINKS_MSG_ENG2UI_ENG_BUSY"       , L"����æ"},
    {WINKS_MSG_ENG2UI_DIAL_INT       , L"WINKS_MSG_ENG2UI_DIAL_INT"       , L"���绰�ж�"},
    {WINKS_MSG_ENG2UI_STORAGE_LIMIT  , L"WINKS_MSG_ENG2UI_STORAGE_LIMIT"  , L"�洢�ռ䲻��"},
    {WINKS_MSG_ENG2UI_CC_CLOSED      , L"WINKS_MSG_ENG2UI_CC_CLOSED"      , L"��������ر� "},
#ifdef WINKS_SUPPORT_MEMORY_CARD
    {WINKS_MSG_ENG2UI_MCARD_NOT_EXIST, L"WINKS_MSG_ENG2UI_MCARD_NOT_EXIST", L"�洢��������"},
#endif
    {WINKS_MSG_ENG2UI_SYS_FILES_ERR  , L"WINKS_MSG_ENG2UI_SYS_FILES_ERR"  , L"ϵͳ�ļ����ƻ�"},
    {WINKS_MSG_ENG2UI_CC_FILES_ERR   , L"WINKS_MSG_ENG2UI_CC_FILES_ERR"   , L"�����ļ����ƻ�"},
#ifdef WINKS_DUAL_SIM
    {WINKS_MSG_ENG2UI_SIM_SEL_ERR    , L"WINKS_MSG_ENG2UI_SIM_SEL_ERR"    , L"�û�δѡ��SIM��"},
    {WINKS_MSG_ENG2UI_SIM_UNUSABLE   , L"WINKS_MSG_ENG2UI_SIM_UNUSABLE"   , L"�û�ѡ���Sim��������"},
#endif
    {WINKS_MSG_ENG2UI_NO_SUBSCRIPTION, L"WINKS_MSG_ENG2UI_NO_SUBSCRIPTION", L"���Ƕ����û�"},
    {WINKS_MSG_ENG2UI_USER_DISABLED  , L"WINKS_MSG_ENG2UI_USER_DISABLED"  , L"�û�������"},
    {WINKS_MSG_ENG2UI_NO_NUMBER      , L"WINKS_MSG_ENG2UI_NO_NUMBER"      , L"û�е绰������Ҫ����"},

    {WINKS_MSG_ENG2UI_PROT_NO        , L"WINKS_MSG_ENG2UI_PROT_NO"        , L"�������·�Э��Ų�ƥ��"},
    {WINKS_MSG_ENG2UI_HTTP_ERRCODE   , L"WINKS_MSG_ENG2UI_HTTP_ERRCODE"   , L"�յ�Http������"},
    {WINKS_MSG_ENG2UI_SRV_ERRCODE    , L"WINKS_MSG_ENG2UI_SRV_ERRCODE"    , L"�յ����������ش�����"},
    {WINKS_MSG_ENG2UI_SRV_ERRDATA    , L"WINKS_MSG_ENG2UI_SRV_ERRDATA"    , L"���������������д���"},
    {WINKS_MSG_ENG2UI_MEM            , L"WINKS_MSG_ENG2UI_MEM"            , L"�ڴ��������"},
    {WINKS_MSG_ENG2UI_FS             , L"WINKS_MSG_ENG2UI_FS"             , L"�ļ�ϵͳ����"},
    {WINKS_MSG_ENG2UI_HTTP           , L"WINKS_MSG_ENG2UI_HTTP"           , L"HTTP����"},
    {WINKS_MSG_ENG2UI_HTTP_TIMEOUT   , L"WINKS_MSG_ENG2UI_HTTP_TIMEOUT"   , L"HTTP��ʱ����"},
    {WINKS_MSG_ENG2UI_PROT_PACK      , L"WINKS_MSG_ENG2UI_PROT_PACK"      , L"Э����"},
    {WINKS_MSG_ENG2UI_PROT_PARSE     , L"WINKS_MSG_ENG2UI_PROT_PARSE"     , L"Э�����"},
    {WINKS_MSG_ENG2UI_XML_GEN        , L"WINKS_MSG_ENG2UI_XML_GEN"        , L"����xml����"},
    {WINKS_MSG_ENG2UI_XML_PARSE      , L"WINKS_MSG_ENG2UI_XML_PARSE"      , L"xml��������"},
    {WINKS_MSG_ENG2UI_APP_ERR        , L"WINKS_MSG_ENG2UI_APP_ERR"        , L"Ӧ�õ��ô�"},
    {WINKS_MSG_ENG2UI_ENG_ERR        , L"WINKS_MSG_ENG2UI_ENG_ERR"        , L"������ô�"},
    
    {WINKS_MSG_ENG2UI_UNKNOWN        , L"WINKS_MSG_ENG2UI_UNKNOWN"        , L"δ֪����"},

/*1 ע�ἰ��ʼ������*/
    {WINKS_MSG_ENG2UI_SETUP_SVR_FAIL , L"WINKS_MSG_ENG2UI_SETUP_SVR_FAIL" , L"�������÷�����ʧ��"},
    {WINKS_MSG_ENG2UI_REG_FAIL       , L"WINKS_MSG_ENG2UI_REG_FAIL"       , L"ע��ʧ��"},
    {WINKS_MSG_ENG2UI_ULPB_FAIL      , L"WINKS_MSG_ENG2UI_ULPB_FAIL"      , L"�ϴ��绰��ʧ��"},
    {WINKS_MSG_ENG2UI_INIT_FAIL      , L"WINKS_MSG_ENG2UI_INIT_FAIL"      , L"��ʼ��ʧ��"},
                                                                       
    {WINKS_MSG_ENG2UI_SETUP_SVR      , L"WINKS_MSG_ENG2UI_SETUP_SVR"      , L"�����������÷�����..."},
    {WINKS_MSG_ENG2UI_SETUP_SVR_OK   , L"WINKS_MSG_ENG2UI_SETUP_SVR_OK"   , L"�������÷������ɹ�"},
    {WINKS_MSG_ENG2UI_SMS_REG        , L"WINKS_MSG_ENG2UI_SMS_REG"        , L"δע�ᣬ��Ҫ����ע�����"},
    {WINKS_MSG_ENG2UI_WAIT_PUSH      , L"WINKS_MSG_ENG2UI_WAIT_PUSH"      , L"δע�ᣬ�ȴ�����push"},
    {WINKS_MSG_ENG2UI_REG            , L"WINKS_MSG_ENG2UI_REG"            , L"����ע��..."},
    {WINKS_MSG_ENG2UI_REG_OK         , L"WINKS_MSG_ENG2UI_REG_OK"         , L"ע��OK"},
    {WINKS_MSG_ENG2UI_ULPB_CONFIRM   , L"WINKS_MSG_ENG2UI_ULPB_CONFIRM"   , L"�û�ȷ���Ƿ��ϴ��绰��"},
    {WINKS_MSG_ENG2UI_ULPB           , L"WINKS_MSG_ENG2UI_ULPB"           , L"�����ϴ��绰��..."},
    {WINKS_MSG_ENG2UI_ULPB_OK        , L"WINKS_MSG_ENG2UI_ULPB_OK"        , L"�ϴ��绰�����"},
    {WINKS_MSG_ENG2UI_INIT           , L"WINKS_MSG_ENG2UI_INIT"           , L"���ڳ�ʼ��..."},
    {WINKS_MSG_ENG2UI_INIT_OK        , L"WINKS_MSG_ENG2UI_INIT_OK"        , L"��ʼ�����"},

/*2 ���º���¼*/
    {WINKS_MSG_ENG2UI_FRIEND_FAIL    , L"WINKS_MSG_ENG2UI_FRIEND_FAIL"    , L"���º���¼ʧ��"},
    
    {WINKS_MSG_ENG2UI_FRIEND         , L"WINKS_MSG_ENG2UI_FRIEND"         , L"���ڸ��º���¼..."},
    {WINKS_MSG_ENG2UI_FRIEND_OK      , L"WINKS_MSG_ENG2UI_FRIEND_OK"      , L"����¼�������"},

/*3 �ϴ�DIY*/
    {WINKS_MSG_ENG2UI_DIY_FAIL       , L"WINKS_MSG_ENG2UI_DIY_FAIL"       , L"�ϴ�DIYʧ��"},
    {WINKS_MSG_ENG2UI_SCRIPT_FAIL    , L"WINKS_MSG_ENG2UI_SCRIPT_FAIL"    , L"���Ʋ���ʧ��"},
                                                                  
    {WINKS_MSG_ENG2UI_DIY            , L"WINKS_MSG_ENG2UI_DIY"            , L"�����ϴ�DIY..."},
    {WINKS_MSG_ENG2UI_DIY_OK         , L"WINKS_MSG_ENG2UI_DIY_OK"         , L"�ϴ�DIY�ɹ�"},
    {WINKS_MSG_ENG2UI_SCRIPT_OK      , L"WINKS_MSG_ENG2UI_SCRIPT_OK"      , L"���Ʋ���ɹ�"},

/*4 �Ƽ�������*/
    {WINKS_MSG_ENG2UI_RECOMMEND_FAIL , L"WINKS_MSG_ENG2UI_RECOMMEND_FAIL" , L"�Ƽ�ʧ��"},
                                                                      
    {WINKS_MSG_ENG2UI_RECOMMEND      , L"WINKS_MSG_ENG2UI_RECOMMEND"      , L"�����Ƽ�..."},
    {WINKS_MSG_ENG2UI_RECOMMEND_OK   , L"WINKS_MSG_ENG2UI_RECOMMEND_OK"   , L"�Ƽ��ɹ�"},

/*5 �绰����*/
    {WINKS_MSG_ENG2UI_CALL_FAIL      , L"WINKS_MSG_ENG2UI_CALL_FAIL"      , L"��ȡͨ���û�����ʧ��"},
    {WINKS_MSG_ENG2UI_CALL_TIMEOUT   , L"WINKS_MSG_ENG2UI_CALL_TIMEOUT"   , L"��ȡͨ���û�����ʱ"},
                                                                    
    {WINKS_MSG_ENG2UI_CALL           , L"WINKS_MSG_ENG2UI_CALL"           , L"���ڻ�ȡͨ���û�����..."},
    {WINKS_MSG_ENG2UI_CALL_OK        , L"WINKS_MSG_ENG2UI_CALL_OK"        , L"��ȡͨ���û�����ɹ�"},

/*6 ͬ���������õ�������*/
    {WINKS_MSG_ENG2UI_SETUP_FAIL     , L"WINKS_MSG_ENG2UI_SETUP_FAIL"     , L"ͬ����������ʧ��"},
    {WINKS_MSG_ENG2UI_SRVCFG_FAIL    , L"WINKS_MSG_ENG2UI_SRVCFG_FAIL"    , L"����������ʧ��"},
    {WINKS_MSG_ENG2UI_USERCFG_FAIL   , L"WINKS_MSG_ENG2UI_USERCFG_FAIL"   , L"��������ʧ��"},
    {WINKS_MSG_ENG2UI_ULSTAT_FAIL    , L"WINKS_MSG_ENG2UI_ULSTAT_FAIL"    , L"ͳ����Ϣ�ϴ�ʧ��"},
    {WINKS_MSG_ENG2UI_ULLOG_FAIL     , L"WINKS_MSG_ENG2UI_ULLOG_FAIL"     , L"log��Ϣ�ϴ�ʧ��"},
                                                                       
    {WINKS_MSG_ENG2UI_SETUP          , L"WINKS_MSG_ENG2UI_SETUP"          , L"����ͬ����������..."},
    {WINKS_MSG_ENG2UI_SETUP_OK       , L"WINKS_MSG_ENG2UI_SETUP_OK"       , L"ͬ���������óɹ�"},
    {WINKS_MSG_ENG2UI_SRVCFG_OK      , L"WINKS_MSG_ENG2UI_SRVCFG_OK"      , L"���������óɹ�"},
    {WINKS_MSG_ENG2UI_USERCFG_OK     , L"WINKS_MSG_ENG2UI_USERCFG_OK"     , L"�������óɹ�"},
    {WINKS_MSG_ENG2UI_ULSTAT_OK      , L"WINKS_MSG_ENG2UI_ULSTAT_OK"      , L"ͳ����Ϣ�ϴ��ɹ�"},
    {WINKS_MSG_ENG2UI_ULLOG_OK       , L"WINKS_MSG_ENG2UI_ULLOG_OK"       , L"log��Ϣ�ϴ��ɹ�"},

/*7 ����°汾*/
    {WINKS_MSG_ENG2UI_RUPGRADE_FAIL  , L"WINKS_MSG_ENG2UI_RUPGRADE_FAIL"  , L"����ʧ��"},
                                                                       
    {WINKS_MSG_ENG2UI_RUPGRADE       , L"WINKS_MSG_ENG2UI_RUPGRADE"       , L"���ڼ���°汾..."},
    {WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW, L"WINKS_MSG_ENG2UI_RUPGRADE_NOT_NEW" , L"û���°汾"},
    {WINKS_MSG_ENG2UI_RUPGRADE_NEW   , L"WINKS_MSG_ENG2UI_RUPGRADE_NEW"   , L"���°汾"},
    {WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK, L"WINKS_MSG_ENG2UI_RUPGRADE_NEW_OK", L"�°汾���سɹ�"},
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
			case WINKS_KEY_3:/* �������� */
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

	//UIͨ����ע����ǰ
	iRet = Winks_RegisterUiChn(uiPtr->uiChn);
	Winks_assert(iRet == WINKS_RETURN_SUCCESS);
    uiPtr->winksCallAppRun = 0;
	return;
}
