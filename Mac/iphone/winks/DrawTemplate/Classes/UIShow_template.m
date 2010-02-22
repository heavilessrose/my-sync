//
//  UIShow_template.m
//  sp3
//
//  Created by qingjun pan on 1/27/10.
//  Copyright 2010 winksi. All rights reserved.
//
#import <Foundation/NSArray.h>
#import "UIShow_template.h"
#import "wk_iphoneccshw.h"
#import "wk_engin.h"
#import "MyLog.h"
#import "wk_iphoneccshw.h"
#import <UIKit/UIApplication.h>

//@implementation UIShow_template

//@end
//extern Winks_CCSW_Global_s gCCSW;

int SpringBoardpid = -1;
int Mobilepid = -1;

extern int dialCallCount;
extern Winks_CCSW_Global_s gCCSW;
#define SHOW_TITLE  1  //做是否展示片头判断

/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/

//来电时展示彩像
static void showCommingColorView_template ()
{
	Winks_printf("into showCommingColorView");
	
	NSMutableArray *windows = (NSMutableArray *)[[UIApplication sharedApplication] windows];
	
	UIWindow *mainWindow = nil;
	UIWindow *mainWindow1[10];
	const char *tmpMainWindow2;
	BOOL UIWindowFlag = false;
	BOOL SBAppWindowFlag = false;
	
	//UIWindow *mainWindow1 = [windows objectAtIndex:1];
#if 1		
	int countWindows = [windows count];
	Winks_printf("showCommingColorView:mainwindow counts=%d",countWindows);
	Class _mainWindow = objc_getClass("SBAlertWindow");
	Class _SBAppWindow = objc_getClass("SBAppWindow");
	for(int i = 0;i<countWindows;i++)
	{
		mainWindow1[i] = [windows objectAtIndex:i];
		
		UIWindowFlag = [mainWindow1[i] isMemberOfClass:_mainWindow];			
		
		if(UIWindowFlag == true)
		{
			mainWindow = mainWindow1[i];
			break;
		}
		
		SBAppWindowFlag = [mainWindow1[i] isMemberOfClass:_SBAppWindow];
		if(SBAppWindowFlag == true)
		{
			mainWindow = mainWindow1[i];
			Winks_printf("showCommingColorView:mainwindow is SBAppWindow ,i=%d",i);
		}
		
		tmpMainWindow2 = [[mainWindow1[i] description] cStringUsingEncoding:NSASCIIStringEncoding];
		Winks_printf("showCommingColorView:the times=%d mainWindow=%s",i,tmpMainWindow2);
	}
	
	
	if(UIWindowFlag == false)
	{
		if(countWindows>6)
		{
			mainWindow = [windows objectAtIndex:4];
		}
		else if(countWindows <= 6)
		{
			mainWindow = [windows objectAtIndex:1];
		}
		
		Winks_printf("showCommingColorView:the mainWindow is counts=%d",countWindows);
	}
#endif
	
	//UIWindow *mainWindow = [[UIApplication sharedApplication] keyWindow];
	//mainWindow = [[UIApplication sharedApplication] keyWindow];
	//tmpMainWindow2 = [[mainWindow description] cStringUsingEncoding:NSASCIIStringEncoding];
	//Winks_printf("showCommingColorView:the keyWindow mainWindow=%s",tmpMainWindow2);
	
	if(mainWindow)
	{
		//const char *tmpMainWindow = [[mainWindow description] cStringUsingEncoding:NSASCIIStringEncoding];
		//Winks_printf("showCommingColorView:the mainWindow=%s",tmpMainWindow);
	}
	
	
	Winks_printf("the version is = %s",[[UIDevice currentDevice].systemVersion cStringUsingEncoding:NSASCIIStringEncoding]);
	if (mainWindow != nil && [mainWindow.subviews count] > 0)
	{
		//展示彩像标志
		gCCSW.showstate = 1;
		
	}
	else 
	{			
		//释放彩像信息
		//if (gCCSW.Info) 
		{
			//Winks_printf("closeShowCommingColorView:gCCSW.Info adress");
			//free(gCCSW.Info);
			//Winks_printf("WinksStrgFile_CCShow_s free");
			//gCCSW.Info = NULL;
		}	
	}
	
	
	Winks_printf("out showCommingColorView");
	
}

/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/
//关闭来电时展示彩像
static void closeShowCommingColorView_template () 
{
		
	//Winks_printf("closeShowCommingColorView: the erro4 is=%s",strerror(errno));
	Winks_printf("out closeShowCommingColorView");
}

/*-------------------------------------------------------------
 *
 *
 *
 *
 *----------------------------------------------------------***/
//电话接通后展示彩像，以及拨电话时展示彩像
void showColorView_template() 
{
	Winks_printf("into showColorView");
	
	//panqj test
	NSMutableArray *windows = (NSMutableArray *)[[UIApplication sharedApplication] windows];
	UIWindow *mainWindow = nil;
	UIWindow *mainWindow1[10];
	const char *tmpMainWindow2;
	BOOL UIWindowFlag = false;
	//BOOL SBAppWindowFlag = false;
	//UIWindow *mainWindow1 = [windows objectAtIndex:1];
#if 1		
	int countWindows = [windows count];
	Winks_printf("showColorView:mainwindow counts=%d",countWindows);
	Class _mainWindow = objc_getClass("UIWindow");
	
	for(int i = 0;i<countWindows;i++)
	{
		mainWindow1[i] = [windows objectAtIndex:i];
		
		UIWindowFlag = [mainWindow1[i] isMemberOfClass:_mainWindow];		
		
		if(UIWindowFlag == true)
		{
			mainWindow = mainWindow1[i];
			break;
		}
		
		tmpMainWindow2 = [[mainWindow1[i] description] cStringUsingEncoding:NSASCIIStringEncoding];
		Winks_printf("showCommingColorView:the times=%d mainWindow=%s",i,tmpMainWindow2);
	}
#endif		
	
	if(mainWindow)
	{
		//const char *tmpMainWindow = [[mainWindow description] cStringUsingEncoding:NSASCIIStringEncoding];
		//Winks_printf("showCommingColorView:the mainWindow=%s",tmpMainWindow);
	}
	
	if (mainWindow != NULL && [mainWindow.subviews count] > 0)
	{
		//展示彩像标志
		gCCSW.showstate = 1;
		
	} 
	
}

/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/
//关闭电话接通后展示彩像，以及拨电话时展示彩像
void closeShowColorView_template()
{
	Winks_printf("out closeShowColorView");
}



/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/
//电话事件处理逻辑过程
int winks_DealCallEvent(unsigned short callstate,short Socketstate,int titleFlag)
{
	int ret = 0;	
	//保存当前电话状态
	gCCSW.callstate = callstate;

	
	//初始化客户端socket
#ifdef WINKS_LONG_SOCKET
	if(Socketstate <= 0)
	{
		if (Winks_InitSocket() != WINKS_RETURN_SUCCESS)
		{
			gCCSW.Socketstate = -1;
			return -1;
		}
		
		gCCSW.Socketstate = 1;
	}
	
#endif
	
	//获取电话号码			
	if (gCCSW.number == NULL)
	{
		return -1;
	}
	
	//向引擎报告事件
	ret = Winks_post_call_event(callstate,gCCSW.number);
	if(ret == WINKS_RETURN_FAILURE)
	{
		gCCSW.Socketstate = -1;
		return -1;
	}
	
	Winks_printf("InCallController__callStatusChanged:number= %s",gCCSW.number);
	
	/*判断是否展示彩像*/
	ret = Winks_if_show_cc();
	if(ret == 1)  
	{
		//获取用户状态
		unsigned long tval = 0;
		ret = Winks_UserGetState( (int* )(&tval) );
		if(ret == WINKS_RETURN_FAILURE)
		{			
			gCCSW.Socketstate = -1;
			return -1;
		}
		if( tval == 0 || tval == 2 )    //未激活或者退订
		{
			gCCSW.flag |= WK_CCSW_LSKACTIVE;
		}
		
		//获取彩像展示信息
		if (gCCSW.pCCDWglb) {
			free(gCCSW.pCCDWglb);
			gCCSW.pCCDWglb = NULL;
			Winks_printf("WinksStrgFile_CCShow_s free");
		}
		
		Winks_CCDW_Global_s *pCCShow = (Winks_CCDW_Global_s*)malloc(sizeof(Winks_CCDW_Global_s));
		if(pCCShow == NULL)
		{			
			return -1;			
		}		
		
		Winks_printf("Winks_CCDW_Global_s alloc");
		gCCSW.pCCDWglb = pCCShow;			
		
		
		//如果用户状态是未激活或者退订就直接获取未激活彩像而不管展示不展示片头和对方电话号码是什么
		if(gCCSW.flag & WK_CCSW_LSKACTIVE)
		{			
			pCCShow->pRegion = (Winks_CCDW_RgBase_s*)winks_ccsw_GetRegion(WINKS_DFT_CC_NO );
			if ((gCCSW.pCCDWglb->pRegion ) == NULL) 
			{				
				free(pCCShow);
				Winks_printf("WinksStrgFile_CCShow_s free");
				gCCSW.pCCDWglb = NULL;
				gCCSW.Socketstate = -1;
				return -1;
			}
		}
		else
		{//正常情况下的处理逻辑
			if(titleFlag == SHOW_TITLE)
			{
				//是否展示片头
				ret = Winks_if_show_titleCC();	
			}
			else
			{
				ret = 0;//不展示片头
			}
					
			
			if( ret == 1 )
			{
				if((gCCSW.pCCDWglb->pRegion = (Winks_CCDW_RgBase_s*)winks_ccsw_GetRegion(WINKS_PRI_CC_NO) ) == NULL) 
				{				
					free(pCCShow);
					Winks_printf("WinksStrgFile_CCShow_s free");
					gCCSW.pCCDWglb = NULL;
					return -1;
				}
				
			} 
			else 
			{
				if(ret == WINKS_RETURN_FAILURE)
				{
					gCCSW.Socketstate = -1;
					free(pCCShow);
					Winks_printf("WinksStrgFile_CCShow_s free");
					gCCSW.pCCDWglb = NULL;
					return -1;
				}
				
				Winks_printf("inComingCall:show not the title");
				
				if ((gCCSW.pCCDWglb->pRegion = (Winks_CCDW_RgBase_s*)winks_ccsw_GetRegion(gCCSW.number) ) == NULL) 
				{				
					free(pCCShow);
					Winks_printf("WinksStrgFile_CCShow_s free");
					gCCSW.pCCDWglb = NULL;
					gCCSW.Socketstate = -1;
					return -1;
				}						
			}
		}
		//Winks_printf("InCallController__callStatusChanged:pCCShow=0x%x",pCCShow);
		//Winks_HEX_DataOut((const unsigned char*)pCCShow->GreetStr,101,(const unsigned char*)"InCallController__callStatusChanged111:GreetStr");
		
		if(gCCSW.flag & WK_CCSW_LSKACTIVE)    //未激活或者退订
		{
			Winks_printf("InCallController__callStatusChanged:unregister");
			/* change greeting */
			//欢迎使用彩像，您尚未激活，马上激活体验彩像的精彩吧
			//winks_ccsw_UnicodeStrcpy( (char* )(pCCEng->GreetStr),(const char* )winks_ccsw_GetString(WK_CCSW_ACTIVESUGGEST_STRID) );
			
			char *uni_str = winks_ccsw_GetUniGreeting();
			if(gCCSW.pCCDWglb->pGreet)
			{
				winks_ccsw_UnicodeStrcpy((gCCSW.pCCDWglb->pGreet->pbuf),uni_str);
			}
			
			free(uni_str);
		}
		Winks_printf("InCallController__callStatusChanged:show the color");
		Winks_HEX_DataOut((const unsigned char*)pCCShow->GreetStr,(int)pCCShow->GreetLen,(const unsigned char*)"InCallController__callStatusChanged222:GreetStr");
		//Winks_HEX_DataOut((const unsigned char*)gCCSW.Info->GreetStr,(int)pCCShow->GreetLen,(const unsigned char*)"InCallController__callStatusChanged:GreetStr");
		
		//展示彩像
		//showColorView();
	}
	else
	{
		if(ret == WINKS_RETURN_FAILURE)
		{
			gCCSW.Socketstate = -1;
			return -1;
		}				
	}
	
	
	return 0;
}

/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/

//dialing call. 拨出时或者接通后的电话处理事件方法
int winks_DialCallEvent_template(int mystatus)
{
	int ret = 0;
	//判断电话状态
	if (mystatus == 3) //拨电话
	{		
		if (dialCallCount == 0)
		{		
			Winks_printf("dial call");
			//如果已经通话中，则不做处理
			if(Winks_get_multiCallstate() == 1) 
			{
				dialCallCount++;
			}
			
			ret = winks_DealCallEvent(IPHONE_OUTGOINGCALL_EVENT_RSP,gCCSW.Socketstate,SHOW_TITLE);
			if(ret == 0)
			{
				//展示彩像
				showColorView_template() ;
			}
			else
			{
				dialCallCount++;
			}			
		}		
	}
	else if (mystatus == 1) //接通电话
	{
		unsigned short callstate = 0;
		Winks_printf("turn on call");
		Winks_printf("InCallController__callStatusChanged: the dialCallCount = %d",dialCallCount);
		
		//如果已经通话中，则不做处理
		if(Winks_get_multiCallstate() == 1)
		{
			goto wk_continue;			
		}		
		
		//保存当前电话状态
		Winks_save_multiCallstate(1);
		
		if (dialCallCount != 0) 
		{//拨的电话接通		
			callstate = IPHONE_OUTGOINGCALL_CONNECTED;				
		}
		else
		{	
			//来电接通
			Winks_printf("InCallController__callStatusChanged: acess the incomming call gCCSW.callstate=%d",gCCSW.callstate);
			callstate = IPHONE_INCOMINGCALL_ACCEPT_SUCCESS;			
		}
		
		ret = winks_DealCallEvent(callstate,gCCSW.Socketstate,0);
		if(ret == 0)
		{
			//展示彩像
			showColorView_template();
		}
		else
		{
			goto wk_continue;
		}		
		
	}
	else if (mystatus == 4) {//接通来电					
		
		//Winks_printf("InCallController__cendatusChanged: accept incomming call");
	}
	else if (mystatus == 5) //挂断电话
	{
		closeMobilePhoneCall_template();
	}
	else
	{
		
	}
	//Winks_printf("ready for out InCallController__callStatusChanged");
	
wk_continue:

	return 0;
}

/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/
//panqj add 2010.01.26
void inComingCall_Template(const char * numbercall)
{
	//SpringBoardpid = (int)getpid();
	//Winks_printf("inComingCall:SpringBoardpid=%d, the callstate=%d",SpringBoardpid,gCCSW.callstate);

	int ret = 0;
	
	//如果已经通话中，则不做处理
	if(Winks_get_multiCallstate() == 1) 
	{
		Winks_printf("inComingCall:one call holded!");
		return;
	}		
	
	ret = winks_DealCallEvent(IPHONE_INCOMINGCALL_EVENT,gCCSW.Socketstate,SHOW_TITLE);
	if(ret == 0)
	{
		//展示彩像
		showCommingColorView_template();
	}
	
	//Winks_printf("SBCallAlert__handleCallerIDEvent:the error7 is=%s",strerror(errno));
	Winks_printf("inComingCall:incoming call end ;gCCSW.timerflag=%d",gCCSW.timerflag);	
}

/*______________________________________________________________
 *
 *
 *
 *_____________________________________________________________*/
//panqj add
void closeMobilePhoneCall_template()
{
	int ret = 0;
	
	if(gCCSW.callstate == 0)
	{
		goto wk_continue;
	}
	
	//来电接通状态清零
	//incomingCallStatus = 0;
	
	//保存当前电话状态
	gCCSW.callstate = 0;
	//panqj add恢复状态
	Winks_delete_Callstatus();
	//保存当前电话状态
	Winks_save_multiCallstate(0);
	
	Winks_printf("closeMobilePhoneCall: Number=%s,Socketstate=%d",gCCSW.number,gCCSW.Socketstate);
#if 0	
	//获取电话号码			
	if ((gCCSW.number == NULL)||(gCCSW.Socketstate == -1))
	{
#ifdef WINKS_LONG_SOCKET
		Winks_CloseSocket();
#endif
		goto wk_continue;
	}
	
#endif	
	
	
	Winks_printf("closeMobilePhoneCall: close the call1");
	
	
	//挂断后删除展示的彩像
	
	//关闭来电接通后展示彩像，以及拨电话时展示彩像
	closeShowColorView_template();
	
	dialCallCount = 0;		
	
	//删除存储号码文件
	//Winks_save_number(NULL);
	
	
	if ((gCCSW.number != NULL)&&(gCCSW.Socketstate != -1))
	{
		if (dialCallCount != 0) 
		{
			
			//向引擎报告事件
			ret = Winks_post_call_event(IPHONE_OUTGOING_DISCONNECT_IND,gCCSW.number);			
		} 
		else 
		{
			//保存当前电话状态
			//gCCSW.callstate = IPHONE_NWRK_CALL_RELEASE;
			//向引擎报告事件
			ret = Winks_post_call_event(IPHONE_NWRK_CALL_RELEASE,gCCSW.number);
		}
		
		if(ret == WINKS_RETURN_FAILURE)
		{
			//gCCSW.Socketstate = -1;
			//goto wk_continue;
		}
	}
	
	//Winks_printf("InCallController__callStatusChanged: close the call2");
	//关闭客户端socket
#ifdef WINKS_LONG_SOCKET
	Winks_CloseSocket();
#endif
	
	Winks_printf("closeMobilePhoneCall:turn off call");
	if(gCCSW.number)
	{
		free(gCCSW.number);
		gCCSW.number = NULL;
	}  
	
	gCCSW.Socketstate = 0;
	
	//清除日志
	Winks_clear_log();
	//Winks_printf("InCallController__callStatusChanged: close the call3");
	
wk_continue:
	Winks_printf("out closeMobilePhoneCall");
	
}


