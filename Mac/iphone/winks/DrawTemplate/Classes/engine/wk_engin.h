/**************************************************************************
Copyright (C) Beijing Mobile Winks Co., Ltd
All rights reserved.
 
This source code is part of winks software and is licensed by WINKS SOFTWARE LICENSE AGREEMENT.
 
The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.
 
THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL ANY PERSON WHO HAS CONTRIBUTED TO OR IS THE OWNER OF ANY PART OF THIS SOFTWARE BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************
*Id: wk_setting.c,v 1.00 04/03/2009
**************************************************************************    
* File Description
* ----------------
* Color call ENGINE global API. 
**************************************************************************//*
File:		wk_engin.h
Author:		Huaxm
Create Time:17/03/2008
*/
#ifndef _WK_ENGIN_H_
#define _WK_ENGIN_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>

#include <sys/shm.h>
#include <unistd.h>
#include "wk_iphoneccshw.h"
#ifdef __cplusplus
extern "C" {
#endif
	

/*是否长连接*/	
#define WINKS_LONG_SOCKET
/*是否多线程通信*/
#define WINKS_UNMULTI_THREAD_IPC

//进程间通信时的socket地址
#define WINKS_SOCKET_CLIENT_ADDRESS "/var/mobile/Winks_Dylib"
#define WINKS_SOCKET_SERVER_ADDRESS "/var/mobile/winks/Winks_Server"

/*电话号码最大长度*/
#define MAX_PHONENUMBER_LENGTH 128
	
//记录电话号码的文件
#define WINKS_STORE_FILE_PATH "/var/mobile/winks_store"
//记录电话状态到文件
#define WINKS_CALLSTATE_FILE_PATH "/var/mobile/winks_call_state"
	
//panqj add
	//记录电话挂断÷接通到文件
#define WINKS_CALLSTATUS_FILE_PATH "/var/mobile/winks_call_status"
//日志文件地址
#define WINKS_CALLLOG_PATH "/var/mobile/callLog"
	
/*片头播放时间限制*/
#define WINKS_SPLASH_SHOW_TIMER_DEF (3)         /*3"*/
#define WINKS_SPLASH_SHOW_TIMER_MAX (15)        /*15"*/
#define WINKS_SPLASH_SHOW_TIMER_MIN (0)         /*not show*/

	
/* 片头运营商按特殊好友处理 */
#define WINKS_DFT_CC_NO     "dft"   /* 默认彩像ID */
#define WINKS_PRI_CC_NO     "pri"   /* 片头彩像起始ID */
#define WINKS_PRI_CC_MAX    1       /* 片头彩像最大个数 <10 */
#define WINKS_BUS_CC_NO     "bus"   /* 运营商彩像起始ID */
#define WINKS_BUS_CC_MAX    1       /* 运营商彩像最大数 <10 */



	typedef struct tag_WinksStrgFile_CCShow_s
		{
			int             RetStatus;                         /* 彩像是否与该号码对应
			 0 -- ok
			 1 -- 默认彩像
			 2 -- 片头 
			 3 -- 运营商 */
			char            WK_NO[52/*WINKS_STRG_PHONE_LEN*/];    /* 电话号码字符串 */
			char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* 好友昵称 */
			char            WK_ID[16/*WINKS_STRG_CC_ID_LEN*/];    /* 彩像ID */
			unsigned short  CC_Idx;     /* 彩像Idx */
			//unsigned short  interval;   /* 图片播放间隔 */
			unsigned char   img_type;   /* 彩像类型 WinksFileExt_e*/
			unsigned char   img_cnt;    /* 图片数 最大为4 */
			unsigned char   GreetLen;   /* 问候语长度 */
			unsigned char   SaveType;   /* 彩像存储类型 0--文件 1--内存 */
			unsigned char * pCCImage;   /* 存储的彩像内存数据 */
			unsigned long   CCImageSize;/* 存储的彩像内存数据大小 */
			char            CCImgPath[1][256];     /* 存储的彩像路径 */
			short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* 存储的彩像问候语 */
			int             allowCollect;    /* 是否允许一键收藏的标志，0禁止，1允许 */
			
		}WinksStrgFile_CCShow_s;	
	
	
	/*************************************************
						iphone平台加
	 **************************************************/
	
	
#define WINKS_PROCESSID 2 /*进程号*/
#define WINKS_INT_LENGTH sizeof(int) /*int型长度，也是TLV包里面T和L的长度*/
#define WINKS_CHAR_LEN sizeof(char) /*char, unsigned char型长度*/


/*Return code*/
#define WINKS_RETURN_SUCCESS    (0)
#define WINKS_RETURN_FAILURE    (-1)
#define WINKS_RETURN_WAIT_MSG   (1)
	
	
	/** 取当前语言种类 **/
	typedef enum tag_Winks_Language_s
		{
			WINKS_LANG_ENGLISH = 0,
			WINKS_LANG_CHINESE = 1,
			WINKS_LANG_CHINESETW = 2,
			WINKS_LANG_CHINESEHK = 3,
			WINKS_LANG_CHINESESG = 4,
			WINKS_LANG_JAPANESE = 5,
			WINKS_LANG_NUMBER,
			WINKS_LANG_NULL
		}Winks_Language_s;
	
	/* 设置选项 */
#if 0
	typedef enum tag_Winks_SettingType_e{
		WINKS_SETTING_SHOW_OTHER_SWITCH=0x00,/* 展示设置项 ，是否展示对方彩像开关*/
		WINKS_SETTING_ROAM_SWITCH,      /* 漫游设置项 ，漫游时联网开关*/
		WINKS_SETTING_SW_UPDATE_WAY,    /* 软件升级设置项，软件自动升级开关*/
		WINKS_SETTING_AUTO_START_SWITCH,/* 软件自启动开关，*/
		WINKS_SETTING_AUTO_ACTIVE,      /* 开机自动激活*/
		WINKS_SETTING_SERVICE_STOP_FLAG,/* 停止服务标志*/
		WINKS_SETTING_FORBIDDEN_FLAG,   /* 用户被禁用标志*/
		WINKS_SETTING_UNSUBSCRIBE_FLAG, /* 用户退订标志*/
		WINKS_SETTING_HANGUP_UPDATE_SWITCH, /* 挂断后更新开关*/
		WINKS_SETTING_SHOW_SPLASH_SWITCH,   /* 播放片头开关*/
		WINKS_SETTING_CALL_CONNECT_SWITCH,  /* 来电联网开关*/
		WINKS_SETTING_ONLY_25G_SWITCH,  /* 只支持2.5G开关*/
		WINKS_SETTING_ALREADY_UPLOAD_SETTING,  /* 已经上传设置项开关*/
		WINKS_SETTING_ALREADY_UPLOAD_PB,  /* 已经上传电话本开关*/
		
		/************************************/
		/*****  Add switch here above.*******/
		WINKS_SETTING_SWITCH_END=0x40,       
		/************************************/
		WINKS_SETTING_SHOW_CASE,        /* 通话设置，通话前、通话中、挂断后 */
		WINKS_SETTING_PBWC_UPDATE_WAY,  /* 随电名片更新方式 */
		WINKS_SETTING_CONNECT_WAY,      /* 联网方式， 定时更新、PUSH更新 */
		WINKS_SETTING_USER_STATE,       /* 用户状态， 正常、会议 */
		WINKS_SETTING_PROXY_PORT,       /* http代理服务器端口 */
		WINKS_SETTING_SW_VERSION,       /* 软件版本号 */
		WINKS_SETTING_APN_ID,           /* http接入点ID值 */
		WINKS_SETTING_DIY_FILE_MAX_SIZE,    /* DIY允许上传文件的最大SIZE */
		WINKS_SETTING_HUSHOW_TIME,      /* 挂断后彩像播放时间 */
		WINKS_SETTING_CONNECT_PERIOD,   /* 定时联网时间周期 */
		WINKS_SETTING_CONNECT_PERIOD_MOD,   /* 定时更新模式*/
		WINKS_SETTING_CONNECT_PERIOD_USUALLY,  /* 定时更新数据最新模式,时间周期*/
		WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY,/* 定时更新最省资费模式,时间周期*/
		WINKS_SETTING_CONNECT_TIMEOUT,  /* 联网超时时间 */
		WINKS_SETTING_SPLASH_DURATION,  /* 片头播放时间,0表示不播放片头 */
		WINKS_SETTING_PUSH_PORT,        /* PUSH端口 */
		WINKS_SETTING_CC_REPLACE_WA,    /* 彩像淘汰算法的加权值 */
		
		/************************************/
		/*****  Add value here above. *******/
		WINKS_SETTING_VALUE_END=0x80,       
		/************************************/
		WINKS_SETTING_PROXY_IP,         /* http代理服务器IP */
		WINKS_SETTING_APN,              /* http接入点 */
		//WINKS_SETTING_SMS_GATEWAY,      /* 服务器下发的短信网关*/
		//WINKS_SETTING_USER_STATE_TEXT,  /* 用户状态文本，*/
		WINKS_SETTING_OPR_SERVER_URL,   /* 数据服务器的URL */
		WINKS_SETTING_MEDIA_SERVER_URL, /* 媒体服务器的URL */
		WINKS_SETTING_SHOP_SERVER_URL,  /* 彩像商城的URL */
		WINKS_SETTING_UID,              /* 服务器下发的UID */
		WINKS_SETTING_ACTIVE_IMSI,      /* 最近激活过的IMSI号码 */
		WINKS_SETTING_CONNECT_BACKUP,   /* 备份定时更新联网时间*/
		
		/************************************/
		/*****  Add string here above. ******/
		WINKS_SETTING_STRING_END=0xa0,       
		/************************************/
		WINKS_SETTING_BLOCK_DATA,       /*测试*/
		WINKS_SETTING_BLOCK_SERVER_CONFIG,/*服务器下发的服务配置*/
		
		/************************************/
		/*****  Add block here above.  ******/
		WINKS_SETTING_NULL
		/************************************/
	} Winks_SettingType_e;
#endif
	/* 设置选项 */
	typedef enum tag_Winks_SettingType_e{
		WINKS_SETTING_SHOW_OTHER_SWITCH=0x00,   /* 展示设置项 ，是否展示对方彩像开关*/
		WINKS_SETTING_ROAM_SWITCH,              /* 漫游设置项 ，漫游时联网开关*/
		WINKS_SETTING_SW_UPDATE_WAY,            /* 软件升级设置项，软件自动升级开关*/
		WINKS_SETTING_AUTO_START_SWITCH,        /* 软件自启动开关，*/
		WINKS_SETTING_AUTO_ACTIVE,              /* 开机自动激活*/
		WINKS_SETTING_SERVICE_STOP_FLAG,        /* 停止服务标志*/
		WINKS_SETTING_FORBIDDEN_FLAG,           /* 用户被禁用标志*/
		WINKS_SETTING_UNSUBSCRIBE_FLAG,         /* 用户退订标志*/
		WINKS_SETTING_HANGUP_UPDATE_SWITCH,     /* 挂断后更新开关*/
		WINKS_SETTING_SHOW_SPLASH_SWITCH,       /* 播放片头开关*/
		WINKS_SETTING_CALL_CONNECT_SWITCH,      /* 来电联网开关*/
		WINKS_SETTING_ONLY_25G_SWITCH,          /* 只支持2.5G开关*/
		WINKS_SETTING_ALREADY_UPLOAD_SETTING,   /* 已经上传设置项开关*/
		WINKS_SETTING_ALREADY_UPLOAD_PB,        /* 已经上传电话本开关*/
		
		/************************************/
		/*****  Add switch here above.*******/
		WINKS_SETTING_SWITCH_END=0x40,       
		/************************************/
		WINKS_SETTING_SHOW_CASE,                /* 通话设置，通话前、通话中、挂断后 */
		WINKS_SETTING_PBWC_UPDATE_WAY,          /* 随电名片更新方式 */
		WINKS_SETTING_CONNECT_WAY,              /* 联网方式， 定时更新、PUSH更新 */
		WINKS_SETTING_USER_STATE,               /* 用户状态， 正常、会议 */
		WINKS_SETTING_PROXY_PORT,               /* http代理服务器端口 */
		WINKS_SETTING_SW_VERSION,               /* 软件版本号 */
		WINKS_SETTING_APN_ID,                   /* http接入点ID值 */
		WINKS_SETTING_DIY_FILE_MAX_SIZE,        /* DIY允许上传文件的最大SIZE */
		WINKS_SETTING_HUSHOW_TIME,              /* 挂断后彩像播放时间 */
		WINKS_SETTING_CONNECT_PERIOD,           /* 定时联网时间周期 */
		WINKS_SETTING_CONNECT_PERIOD_MOD,       /* 定时更新模式*/
		WINKS_SETTING_CONNECT_PERIOD_USUALLY,   /* 定时更新数据最新模式,时间周期*/
		WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY, /* 定时更新最省资费模式,时间周期*/
		WINKS_SETTING_CONNECT_TIMEOUT,          /* 联网超时时间 */
		WINKS_SETTING_SPLASH_DURATION,          /* 片头播放时间,0表示不播放片头 */
		WINKS_SETTING_PUSH_PORT,                /* PUSH端口 */
		WINKS_SETTING_CC_REPLACE_WA,            /* 彩像淘汰算法的加权值 */
		WINKS_SETTING_HANGUP_UPDATE_DELAY_TIME, /* 挂断后更新的延时时间，为0表示不延时。 */
		WINKS_SETTING_TODAY_BYTES,              /* 今日流量*/
		WINKS_SETTING_TODAY_DATE,               /* 今日日期*/
		WINKS_SETTING_LAST_MONTH_BYTES,         /* 上月流量*/
		WINKS_SETTING_THIS_MONTH_BYTES,         /* 本月流量*/
		WINKS_SETTING_THIS_MONTH_DATE,          /* 本月日期*/
		WINKS_SETTING_TOTAL_BYTES,              /* 统计流量*/
		WINKS_SETTING_TOTAL_DATE,               /* 清零日期*/
		WINKS_SETTING_TOTAL_TIME,               /* 清零时间*/
		
		/************************************/
		/*****  Add value here above. *******/
		WINKS_SETTING_VALUE_END=0x80,       
		/************************************/
		WINKS_SETTING_PROXY_IP,                 /* http代理服务器IP */
		WINKS_SETTING_APN,                      /* http接入点 */
		WINKS_SETTING_SMS_GATEWAY,              /* 服务器下发的短信网关*/
		WINKS_SETTING_SMS_PREFIX,               /* 短信前缀 */
		//WINKS_SETTING_USER_STATE_TEXT,        /* 用户状态文本，*/
		WINKS_SETTING_OPR_SERVER_URL,           /* 数据服务器的URL */
		WINKS_SETTING_MEDIA_SERVER_URL,         /* 媒体服务器的URL */
		WINKS_SETTING_SHOP_SERVER_URL,          /* 彩像商城的URL */
		WINKS_SETTING_UID,                      /* 服务器下发的UID */
		WINKS_SETTING_ACTIVE_IMSI,              /* 最近激活过的IMSI号码 */
		WINKS_SETTING_CONNECT_BACKUP,           /* 备份定时更新联网时间*/
		
		/************************************/
		/*****  Add string here above. ******/
		WINKS_SETTING_STRING_END=0xa0,       
		/************************************/
		WINKS_SETTING_BLOCK_DATA,               /*测试*/
		WINKS_SETTING_FORCE_RELOAD_DATA,        /* 强制重新加载数据。用户symbian平台(内嵌浏览器需重启EXE才能正常使用)强制重启后的数据恢复 */
		WINKS_SETTING_BLOCK_SERVER_CONFIG,      /*服务器下发的服务配置*/
		
		/************************************/
		/*****  Add block here above.  ******/
		WINKS_SETTING_NULL
		/************************************/
	} Winks_SettingType_e;
#if 0	
	//彩像展示模板内容
	typedef struct tag_Winks_CCDW_Global_s
		{
			Winks_CCDW_RgBase_s* pRegion;
			Winks_CCDW_Media_s* pWinks;
			Winks_CCDW_Text_s* pGreet;			
			Winks_Xml_s* pXml;
		}Winks_CCDW_Global_s;
	
	//彩像展示的全局结构体
	typedef struct tag_Winks_CCSW_Global_s
		{	
			unsigned short timerflag;      /*定时器标志信息 1:定时器启动 0:定时器关闭*/
			unsigned short callstate;     /*电话事件状态WinksCallEvent_e*/
			unsigned short showstate;      /*彩像展示状态信息 1:展示 0:不展示*/
			short Socketstate;      /*彩像socket状态信息 1:可用 -1:不可用*/
			unsigned short flag;    /*用户状态*/
			unsigned short titleflag;/*是否展示片头标志；1展示；0不展示*/
			char* number;
			WinksStrgFile_CCShow_s *Info; /*彩像信息*/
			Winks_CCDW_Global_s *pCCDWglb;/*彩像模板内容*/
		}Winks_CCSW_Global_s;
#endif
	
	//电话状态
	typedef enum tag_WinksCallEvent_e
		{
			IPHONE_INCOMINGCALL_EVENT = 1001,    /*incoming*/
			IPHONE_OUTGOINGCALL_EVENT_RSP,   /*dial*/
			IPHONE_INCOMINGCALL_REJECTED,     /**/
			IPHONE_INCOMINGCALL_ACCEPT_SUCCESS,   /*accept call*/
			IPHONE_OUTGOINGCALL_CONNECTED,        /**/
			IPHONE_NWRK_CALL_RELEASE,     /*local end call*/
			IPHONE_OUTGOING_DISCONNECT_IND,     /**/
			IPHONE_INCOMING_CALL_WAIT      /**/
		}WinksCallEvent_e;
	
	
	/* 文件类型 */
	typedef enum tag_WinksFileExt_e
		{
			WINKS_FILE_EXT_none = 0,    /* 未知 */
			WINKS_FILE_EXT_xml,         /* .xml */
			WINKS_FILE_EXT_gif,         /* .gif */
			WINKS_FILE_EXT_jpg,         /* .jpg */
			WINKS_FILE_EXT_png,         /* .png */
			WINKS_FILE_EXT_tiff,        /* .tiff */
			WINKS_FILE_EXT_spl,         /* .spl */
			WINKS_FILE_EXT_swf,         /* .swf */
			WINKS_FILE_EXT_avi,         /* .avi */
			WINKS_FILE_EXT_cab,         /* .cab */
			WINKS_FILE_EXT_sis,         /* .sis */
			WINKS_FILE_EXT_bin,          /* .bin */
			WINKS_FILE_EXT_multi,
			WINKS_FILE_EXT_gw           /* 移动网关计费提醒 */
		}WinksFileExt_e;
	
	
	/*通过socket调用的函数列表*/
	typedef enum tag_Winks_EnginFunction_e
		{
			/*监听展示*/
			Winks_UserGetCCShowSetup_e,  /*获取展示设置*/
			Winks_UserGetCCShow_e,     /*获取通话号码的彩像*/
			Winks_UserStopCCShow_e,    /*停止展示彩像*/
			Winks_log_cc_show_e,   /* 记录此次彩像展示情况。返回负值表示失败 */
			Winks_if_show_cc_e,   /*获取是否展示彩像标志. 1:展示,0:不展示*/
			Winks_GetSettingValue_e,
			Winks_UserGetState_e,
			Winks_if_show_titleCC_e,  /*获取是否展示片头标志。1: 播放, 其它: 不播*/
			WINKS_GETSHOWTEMPLATE,     /* 获取彩像展示模板*/
			WINKS_GETSHOWELEMENT      /* 获取展示成员变量元素*/
		}Winks_EnginFunction_e;
	

//panqj add 2010.01.27
int Winks_GetShowElement(char *number, unsigned long extDataFlag, char *eTag, unsigned long *fileFlag, 
							 char * eDataBuf, unsigned long* eDataBufSize, char *tIdBuf, unsigned long tIdBufSize);	
int Winks_GetShowTemplate(char* number, char* fileName, unsigned long fileSize, int *allowCollectFlag);
	
	
/* retBuff -- 返回的内存缓冲区 
   retLen -- 输入内存缓冲区大小，输出实际使用的内存缓冲区大小
   IsDft -- 是否获取默认的配置
*/
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft);

/*获取通话号码的彩像*/
//int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow);
	
/*停止展示彩像*/
int Winks_UserStopCCShow(void);
	
/* 记录此次彩像展示情况。返回负值表示失败 */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);
	
/*获取是否展示彩像标志. 1:展示,0:不展示*/
int Winks_if_show_cc(void);
	
/*获取是否展示片头标志。1: 播放, 其它: 不播*/
int Winks_if_show_titleCC(void);

	
	/* 获取设置项的值
	 * function: Winks_GetSettingValue
	 * parameters:
	 *   Winks_SettingType_e type: 设置项类别
	 *   unsigned long *value: (out)设置项值
	 * return:
	 *   成功返回:0
	 *   失败返回:-1
	 */
int Winks_GetSettingValue(Winks_SettingType_e type, unsigned long *value);

	/*查询本地用户激活状态。
	 
	 state 5:引擎初始化失败
	 state 4:引擎初始化中
	 state 3:禁用用户
	 state 2:退订用户
	 state 1:已激活用户
	 state 0:未激活用户
	 */
	int Winks_UserGetState(int *state);
	
	/*存储电话状态 1:通话中 0:闲置中，防止多路电话*/
	void Winks_save_multiCallstate (int state);
	
	/*获取电话状态 1:通话中 0:闲置中,多路电话时使用*/
	int Winks_get_multiCallstate ();
	
	void Winks_save_Callstatus ();
	void Winks_delete_Callstatus();
	int Winks_get_Callstatus ();
	
	//panqj add  保存全局使用的号码
	char* Winks_save_globelNumber(const char *num);
	
	/* 获取指定模板的制定元素
	 * function: Winks_GetShowElement
	 * parameters:
	 *   char *number (in)电话号码，不能为NULL。
	 *          可通过输入宏（WINKS_DFT_CC_NO、WINKS_PRI_CC_NO、WINKS_BUS_CC_NO），达到特定的效果。
	 *   unsigned long extDataFlag (in)外部数据标识。
	 *          为WINKS_TRUE表示外部数据(pageshow中的数据)，为WINKS_FALSE表示内部数据(template中multi-part数据)。
	 *   char *eTag (in)待获取元素的标识，不能为NULL。对应tempate中的CID.
	 *   unsigned long *fileFlag (out)用来标识返回数据是否是文件名。
	 *          为WINKS_TRUE表示eBuff中返回的是文件名，所取数据存在该文件中；
	 *          为WINKS_FALSE表示eBuff中存放的是数据可以直接使用。
	 *   char * eDataBuf (out)返回数据。
	 *   unsigned long *eDataBufSize (in, out)传入eDataBufSize的size。当传入的buffSize不足时，返回需要的size，
	 *          用户应备好足够的输出空间，重新调用该函数获取数据。
	 *   char * tIdBuf (out)返回对应的TID。如果不需要获取该参数，可以传入NULL。
	 *   unsigned long tIdBufSize (in)传入tIdBuf的size。
	 * return:
	 *   成功返回:0
	 *   失败返回:-1
	 */
	int Winks_GetShowElement(char *number, unsigned long extDataFlag, char *eTag, unsigned long *fileFlag, 
							 char * eDataBuf, unsigned long* eDataBufSize, char *tIdBuf, unsigned long tIdBufSize);	

/*保存去电或来电号码*/
//void Winks_save_number (const char *num);	
	
/*获取去电或来电号码*/
//char* Winks_get_number ();
	
/*向引擎报告电话事件*/
int Winks_post_call_event (WinksCallEvent_e event,char* number);
	
	/******************************************************************\
	 定时器机制不需要create直接用ITIMER_REAL句柄故也不需要创建定时器
	 \******************************************************************/
int Winks_api_StartTimer(unsigned long delay,void (*timerfunc)(int));
	
	/***********************************
	 定时器句柄无需销毁，这里停止定时器。
	 **********************************/
int Winks_api_StopTimer(void);

//flush缓冲区，实际往socket写数据
ssize_t Winks_Write_flush();
/*向客户端socket写数据，实际写到缓冲区*/
ssize_t Winks_Write(const void *buf, size_t nbyte);
/*从客户端socket读数据*/
ssize_t Winks_Read(void *buf, size_t nbyte);
	
off_t Winks_Lseek(off_t offset, int whence);
	
//初始化客户端socket
int Winks_InitSocket();

//关闭客户端socket
void Winks_CloseSocket();
	
/**************************************************************************\
	 功能描述:
	 获取系统当前的设置语言
	 参数说明:
	 无
	 返回值:
	 返回WINKS平台统一定义的语言代码
	 实现描述:
\**************************************************************************/
Winks_Language_s Winks_GetCurrentLanguage(void);

/*拷贝unicode字符串*/
int winks_ccsw_UnicodeStrcpy( char* pdest, const char* psour );
	
/*获取unicode字符串的长度*/
int winks_strlen_ucs2_type(char * text);
	
/*获取未激活状态时的unicode编码的问候语*/
char* winks_ccsw_GetUniGreeting ();


//关闭读socket的缓冲区
void Winks_Close_Read_flush();
	
//flush缓冲区，实际从socket读数据
ssize_t Winks_Read_flush();

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
