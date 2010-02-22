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
	

/*�Ƿ�����*/	
#define WINKS_LONG_SOCKET
/*�Ƿ���߳�ͨ��*/
#define WINKS_UNMULTI_THREAD_IPC

//���̼�ͨ��ʱ��socket��ַ
#define WINKS_SOCKET_CLIENT_ADDRESS "/var/mobile/Winks_Dylib"
#define WINKS_SOCKET_SERVER_ADDRESS "/var/mobile/winks/Winks_Server"

/*�绰������󳤶�*/
#define MAX_PHONENUMBER_LENGTH 128
	
//��¼�绰������ļ�
#define WINKS_STORE_FILE_PATH "/var/mobile/winks_store"
//��¼�绰״̬���ļ�
#define WINKS_CALLSTATE_FILE_PATH "/var/mobile/winks_call_state"
	
//panqj add
	//��¼�绰�Ҷϡ½�ͨ���ļ�
#define WINKS_CALLSTATUS_FILE_PATH "/var/mobile/winks_call_status"
//��־�ļ���ַ
#define WINKS_CALLLOG_PATH "/var/mobile/callLog"
	
/*Ƭͷ����ʱ������*/
#define WINKS_SPLASH_SHOW_TIMER_DEF (3)         /*3"*/
#define WINKS_SPLASH_SHOW_TIMER_MAX (15)        /*15"*/
#define WINKS_SPLASH_SHOW_TIMER_MIN (0)         /*not show*/

	
/* Ƭͷ��Ӫ�̰�������Ѵ��� */
#define WINKS_DFT_CC_NO     "dft"   /* Ĭ�ϲ���ID */
#define WINKS_PRI_CC_NO     "pri"   /* Ƭͷ������ʼID */
#define WINKS_PRI_CC_MAX    1       /* Ƭͷ���������� <10 */
#define WINKS_BUS_CC_NO     "bus"   /* ��Ӫ�̲�����ʼID */
#define WINKS_BUS_CC_MAX    1       /* ��Ӫ�̲�������� <10 */



	typedef struct tag_WinksStrgFile_CCShow_s
		{
			int             RetStatus;                         /* �����Ƿ���ú����Ӧ
			 0 -- ok
			 1 -- Ĭ�ϲ���
			 2 -- Ƭͷ 
			 3 -- ��Ӫ�� */
			char            WK_NO[52/*WINKS_STRG_PHONE_LEN*/];    /* �绰�����ַ��� */
			char            Name[32/*WINKS_STRG_NICK_LEN*/];   /* �����ǳ� */
			char            WK_ID[16/*WINKS_STRG_CC_ID_LEN*/];    /* ����ID */
			unsigned short  CC_Idx;     /* ����Idx */
			//unsigned short  interval;   /* ͼƬ���ż�� */
			unsigned char   img_type;   /* �������� WinksFileExt_e*/
			unsigned char   img_cnt;    /* ͼƬ�� ���Ϊ4 */
			unsigned char   GreetLen;   /* �ʺ��ﳤ�� */
			unsigned char   SaveType;   /* ����洢���� 0--�ļ� 1--�ڴ� */
			unsigned char * pCCImage;   /* �洢�Ĳ����ڴ����� */
			unsigned long   CCImageSize;/* �洢�Ĳ����ڴ����ݴ�С */
			char            CCImgPath[1][256];     /* �洢�Ĳ���·�� */
			short           GreetStr[101/*WINKS_STRG_GREET_LEN/2*/];     /* �洢�Ĳ����ʺ��� */
			int             allowCollect;    /* �Ƿ�����һ���ղصı�־��0��ֹ��1���� */
			
		}WinksStrgFile_CCShow_s;	
	
	
	/*************************************************
						iphoneƽ̨��
	 **************************************************/
	
	
#define WINKS_PROCESSID 2 /*���̺�*/
#define WINKS_INT_LENGTH sizeof(int) /*int�ͳ��ȣ�Ҳ��TLV������T��L�ĳ���*/
#define WINKS_CHAR_LEN sizeof(char) /*char, unsigned char�ͳ���*/


/*Return code*/
#define WINKS_RETURN_SUCCESS    (0)
#define WINKS_RETURN_FAILURE    (-1)
#define WINKS_RETURN_WAIT_MSG   (1)
	
	
	/** ȡ��ǰ�������� **/
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
	
	/* ����ѡ�� */
#if 0
	typedef enum tag_Winks_SettingType_e{
		WINKS_SETTING_SHOW_OTHER_SWITCH=0x00,/* չʾ������ ���Ƿ�չʾ�Է����񿪹�*/
		WINKS_SETTING_ROAM_SWITCH,      /* ���������� ������ʱ��������*/
		WINKS_SETTING_SW_UPDATE_WAY,    /* ����������������Զ���������*/
		WINKS_SETTING_AUTO_START_SWITCH,/* ������������أ�*/
		WINKS_SETTING_AUTO_ACTIVE,      /* �����Զ�����*/
		WINKS_SETTING_SERVICE_STOP_FLAG,/* ֹͣ�����־*/
		WINKS_SETTING_FORBIDDEN_FLAG,   /* �û������ñ�־*/
		WINKS_SETTING_UNSUBSCRIBE_FLAG, /* �û��˶���־*/
		WINKS_SETTING_HANGUP_UPDATE_SWITCH, /* �ҶϺ���¿���*/
		WINKS_SETTING_SHOW_SPLASH_SWITCH,   /* ����Ƭͷ����*/
		WINKS_SETTING_CALL_CONNECT_SWITCH,  /* ������������*/
		WINKS_SETTING_ONLY_25G_SWITCH,  /* ֻ֧��2.5G����*/
		WINKS_SETTING_ALREADY_UPLOAD_SETTING,  /* �Ѿ��ϴ��������*/
		WINKS_SETTING_ALREADY_UPLOAD_PB,  /* �Ѿ��ϴ��绰������*/
		
		/************************************/
		/*****  Add switch here above.*******/
		WINKS_SETTING_SWITCH_END=0x40,       
		/************************************/
		WINKS_SETTING_SHOW_CASE,        /* ͨ�����ã�ͨ��ǰ��ͨ���С��ҶϺ� */
		WINKS_SETTING_PBWC_UPDATE_WAY,  /* �����Ƭ���·�ʽ */
		WINKS_SETTING_CONNECT_WAY,      /* ������ʽ�� ��ʱ���¡�PUSH���� */
		WINKS_SETTING_USER_STATE,       /* �û�״̬�� ���������� */
		WINKS_SETTING_PROXY_PORT,       /* http����������˿� */
		WINKS_SETTING_SW_VERSION,       /* ����汾�� */
		WINKS_SETTING_APN_ID,           /* http�����IDֵ */
		WINKS_SETTING_DIY_FILE_MAX_SIZE,    /* DIY�����ϴ��ļ������SIZE */
		WINKS_SETTING_HUSHOW_TIME,      /* �ҶϺ���񲥷�ʱ�� */
		WINKS_SETTING_CONNECT_PERIOD,   /* ��ʱ����ʱ������ */
		WINKS_SETTING_CONNECT_PERIOD_MOD,   /* ��ʱ����ģʽ*/
		WINKS_SETTING_CONNECT_PERIOD_USUALLY,  /* ��ʱ������������ģʽ,ʱ������*/
		WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY,/* ��ʱ������ʡ�ʷ�ģʽ,ʱ������*/
		WINKS_SETTING_CONNECT_TIMEOUT,  /* ������ʱʱ�� */
		WINKS_SETTING_SPLASH_DURATION,  /* Ƭͷ����ʱ��,0��ʾ������Ƭͷ */
		WINKS_SETTING_PUSH_PORT,        /* PUSH�˿� */
		WINKS_SETTING_CC_REPLACE_WA,    /* ������̭�㷨�ļ�Ȩֵ */
		
		/************************************/
		/*****  Add value here above. *******/
		WINKS_SETTING_VALUE_END=0x80,       
		/************************************/
		WINKS_SETTING_PROXY_IP,         /* http���������IP */
		WINKS_SETTING_APN,              /* http����� */
		//WINKS_SETTING_SMS_GATEWAY,      /* �������·��Ķ�������*/
		//WINKS_SETTING_USER_STATE_TEXT,  /* �û�״̬�ı���*/
		WINKS_SETTING_OPR_SERVER_URL,   /* ���ݷ�������URL */
		WINKS_SETTING_MEDIA_SERVER_URL, /* ý���������URL */
		WINKS_SETTING_SHOP_SERVER_URL,  /* �����̳ǵ�URL */
		WINKS_SETTING_UID,              /* �������·���UID */
		WINKS_SETTING_ACTIVE_IMSI,      /* ����������IMSI���� */
		WINKS_SETTING_CONNECT_BACKUP,   /* ���ݶ�ʱ��������ʱ��*/
		
		/************************************/
		/*****  Add string here above. ******/
		WINKS_SETTING_STRING_END=0xa0,       
		/************************************/
		WINKS_SETTING_BLOCK_DATA,       /*����*/
		WINKS_SETTING_BLOCK_SERVER_CONFIG,/*�������·��ķ�������*/
		
		/************************************/
		/*****  Add block here above.  ******/
		WINKS_SETTING_NULL
		/************************************/
	} Winks_SettingType_e;
#endif
	/* ����ѡ�� */
	typedef enum tag_Winks_SettingType_e{
		WINKS_SETTING_SHOW_OTHER_SWITCH=0x00,   /* չʾ������ ���Ƿ�չʾ�Է����񿪹�*/
		WINKS_SETTING_ROAM_SWITCH,              /* ���������� ������ʱ��������*/
		WINKS_SETTING_SW_UPDATE_WAY,            /* ����������������Զ���������*/
		WINKS_SETTING_AUTO_START_SWITCH,        /* ������������أ�*/
		WINKS_SETTING_AUTO_ACTIVE,              /* �����Զ�����*/
		WINKS_SETTING_SERVICE_STOP_FLAG,        /* ֹͣ�����־*/
		WINKS_SETTING_FORBIDDEN_FLAG,           /* �û������ñ�־*/
		WINKS_SETTING_UNSUBSCRIBE_FLAG,         /* �û��˶���־*/
		WINKS_SETTING_HANGUP_UPDATE_SWITCH,     /* �ҶϺ���¿���*/
		WINKS_SETTING_SHOW_SPLASH_SWITCH,       /* ����Ƭͷ����*/
		WINKS_SETTING_CALL_CONNECT_SWITCH,      /* ������������*/
		WINKS_SETTING_ONLY_25G_SWITCH,          /* ֻ֧��2.5G����*/
		WINKS_SETTING_ALREADY_UPLOAD_SETTING,   /* �Ѿ��ϴ��������*/
		WINKS_SETTING_ALREADY_UPLOAD_PB,        /* �Ѿ��ϴ��绰������*/
		
		/************************************/
		/*****  Add switch here above.*******/
		WINKS_SETTING_SWITCH_END=0x40,       
		/************************************/
		WINKS_SETTING_SHOW_CASE,                /* ͨ�����ã�ͨ��ǰ��ͨ���С��ҶϺ� */
		WINKS_SETTING_PBWC_UPDATE_WAY,          /* �����Ƭ���·�ʽ */
		WINKS_SETTING_CONNECT_WAY,              /* ������ʽ�� ��ʱ���¡�PUSH���� */
		WINKS_SETTING_USER_STATE,               /* �û�״̬�� ���������� */
		WINKS_SETTING_PROXY_PORT,               /* http����������˿� */
		WINKS_SETTING_SW_VERSION,               /* ����汾�� */
		WINKS_SETTING_APN_ID,                   /* http�����IDֵ */
		WINKS_SETTING_DIY_FILE_MAX_SIZE,        /* DIY�����ϴ��ļ������SIZE */
		WINKS_SETTING_HUSHOW_TIME,              /* �ҶϺ���񲥷�ʱ�� */
		WINKS_SETTING_CONNECT_PERIOD,           /* ��ʱ����ʱ������ */
		WINKS_SETTING_CONNECT_PERIOD_MOD,       /* ��ʱ����ģʽ*/
		WINKS_SETTING_CONNECT_PERIOD_USUALLY,   /* ��ʱ������������ģʽ,ʱ������*/
		WINKS_SETTING_CONNECT_PERIOD_UNUSUALLY, /* ��ʱ������ʡ�ʷ�ģʽ,ʱ������*/
		WINKS_SETTING_CONNECT_TIMEOUT,          /* ������ʱʱ�� */
		WINKS_SETTING_SPLASH_DURATION,          /* Ƭͷ����ʱ��,0��ʾ������Ƭͷ */
		WINKS_SETTING_PUSH_PORT,                /* PUSH�˿� */
		WINKS_SETTING_CC_REPLACE_WA,            /* ������̭�㷨�ļ�Ȩֵ */
		WINKS_SETTING_HANGUP_UPDATE_DELAY_TIME, /* �ҶϺ���µ���ʱʱ�䣬Ϊ0��ʾ����ʱ�� */
		WINKS_SETTING_TODAY_BYTES,              /* ��������*/
		WINKS_SETTING_TODAY_DATE,               /* ��������*/
		WINKS_SETTING_LAST_MONTH_BYTES,         /* ��������*/
		WINKS_SETTING_THIS_MONTH_BYTES,         /* ��������*/
		WINKS_SETTING_THIS_MONTH_DATE,          /* ��������*/
		WINKS_SETTING_TOTAL_BYTES,              /* ͳ������*/
		WINKS_SETTING_TOTAL_DATE,               /* ��������*/
		WINKS_SETTING_TOTAL_TIME,               /* ����ʱ��*/
		
		/************************************/
		/*****  Add value here above. *******/
		WINKS_SETTING_VALUE_END=0x80,       
		/************************************/
		WINKS_SETTING_PROXY_IP,                 /* http���������IP */
		WINKS_SETTING_APN,                      /* http����� */
		WINKS_SETTING_SMS_GATEWAY,              /* �������·��Ķ�������*/
		WINKS_SETTING_SMS_PREFIX,               /* ����ǰ׺ */
		//WINKS_SETTING_USER_STATE_TEXT,        /* �û�״̬�ı���*/
		WINKS_SETTING_OPR_SERVER_URL,           /* ���ݷ�������URL */
		WINKS_SETTING_MEDIA_SERVER_URL,         /* ý���������URL */
		WINKS_SETTING_SHOP_SERVER_URL,          /* �����̳ǵ�URL */
		WINKS_SETTING_UID,                      /* �������·���UID */
		WINKS_SETTING_ACTIVE_IMSI,              /* ����������IMSI���� */
		WINKS_SETTING_CONNECT_BACKUP,           /* ���ݶ�ʱ��������ʱ��*/
		
		/************************************/
		/*****  Add string here above. ******/
		WINKS_SETTING_STRING_END=0xa0,       
		/************************************/
		WINKS_SETTING_BLOCK_DATA,               /*����*/
		WINKS_SETTING_FORCE_RELOAD_DATA,        /* ǿ�����¼������ݡ��û�symbianƽ̨(��Ƕ�����������EXE��������ʹ��)ǿ������������ݻָ� */
		WINKS_SETTING_BLOCK_SERVER_CONFIG,      /*�������·��ķ�������*/
		
		/************************************/
		/*****  Add block here above.  ******/
		WINKS_SETTING_NULL
		/************************************/
	} Winks_SettingType_e;
#if 0	
	//����չʾģ������
	typedef struct tag_Winks_CCDW_Global_s
		{
			Winks_CCDW_RgBase_s* pRegion;
			Winks_CCDW_Media_s* pWinks;
			Winks_CCDW_Text_s* pGreet;			
			Winks_Xml_s* pXml;
		}Winks_CCDW_Global_s;
	
	//����չʾ��ȫ�ֽṹ��
	typedef struct tag_Winks_CCSW_Global_s
		{	
			unsigned short timerflag;      /*��ʱ����־��Ϣ 1:��ʱ������ 0:��ʱ���ر�*/
			unsigned short callstate;     /*�绰�¼�״̬WinksCallEvent_e*/
			unsigned short showstate;      /*����չʾ״̬��Ϣ 1:չʾ 0:��չʾ*/
			short Socketstate;      /*����socket״̬��Ϣ 1:���� -1:������*/
			unsigned short flag;    /*�û�״̬*/
			unsigned short titleflag;/*�Ƿ�չʾƬͷ��־��1չʾ��0��չʾ*/
			char* number;
			WinksStrgFile_CCShow_s *Info; /*������Ϣ*/
			Winks_CCDW_Global_s *pCCDWglb;/*����ģ������*/
		}Winks_CCSW_Global_s;
#endif
	
	//�绰״̬
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
	
	
	/* �ļ����� */
	typedef enum tag_WinksFileExt_e
		{
			WINKS_FILE_EXT_none = 0,    /* δ֪ */
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
			WINKS_FILE_EXT_gw           /* �ƶ����ؼƷ����� */
		}WinksFileExt_e;
	
	
	/*ͨ��socket���õĺ����б�*/
	typedef enum tag_Winks_EnginFunction_e
		{
			/*����չʾ*/
			Winks_UserGetCCShowSetup_e,  /*��ȡչʾ����*/
			Winks_UserGetCCShow_e,     /*��ȡͨ������Ĳ���*/
			Winks_UserStopCCShow_e,    /*ֹͣչʾ����*/
			Winks_log_cc_show_e,   /* ��¼�˴β���չʾ��������ظ�ֵ��ʾʧ�� */
			Winks_if_show_cc_e,   /*��ȡ�Ƿ�չʾ�����־. 1:չʾ,0:��չʾ*/
			Winks_GetSettingValue_e,
			Winks_UserGetState_e,
			Winks_if_show_titleCC_e,  /*��ȡ�Ƿ�չʾƬͷ��־��1: ����, ����: ����*/
			WINKS_GETSHOWTEMPLATE,     /* ��ȡ����չʾģ��*/
			WINKS_GETSHOWELEMENT      /* ��ȡչʾ��Ա����Ԫ��*/
		}Winks_EnginFunction_e;
	

//panqj add 2010.01.27
int Winks_GetShowElement(char *number, unsigned long extDataFlag, char *eTag, unsigned long *fileFlag, 
							 char * eDataBuf, unsigned long* eDataBufSize, char *tIdBuf, unsigned long tIdBufSize);	
int Winks_GetShowTemplate(char* number, char* fileName, unsigned long fileSize, int *allowCollectFlag);
	
	
/* retBuff -- ���ص��ڴ滺���� 
   retLen -- �����ڴ滺������С�����ʵ��ʹ�õ��ڴ滺������С
   IsDft -- �Ƿ��ȡĬ�ϵ�����
*/
int Winks_UserGetCCShowSetup(char * retBuff, int * retLen, int IsDft);

/*��ȡͨ������Ĳ���*/
//int Winks_UserGetCCShow(char * phonenumber, int dir, WinksStrgFile_CCShow_s *pCCShow);
	
/*ֹͣչʾ����*/
int Winks_UserStopCCShow(void);
	
/* ��¼�˴β���չʾ��������ظ�ֵ��ʾʧ�� */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);
	
/*��ȡ�Ƿ�չʾ�����־. 1:չʾ,0:��չʾ*/
int Winks_if_show_cc(void);
	
/*��ȡ�Ƿ�չʾƬͷ��־��1: ����, ����: ����*/
int Winks_if_show_titleCC(void);

	
	/* ��ȡ�������ֵ
	 * function: Winks_GetSettingValue
	 * parameters:
	 *   Winks_SettingType_e type: ���������
	 *   unsigned long *value: (out)������ֵ
	 * return:
	 *   �ɹ�����:0
	 *   ʧ�ܷ���:-1
	 */
int Winks_GetSettingValue(Winks_SettingType_e type, unsigned long *value);

	/*��ѯ�����û�����״̬��
	 
	 state 5:�����ʼ��ʧ��
	 state 4:�����ʼ����
	 state 3:�����û�
	 state 2:�˶��û�
	 state 1:�Ѽ����û�
	 state 0:δ�����û�
	 */
	int Winks_UserGetState(int *state);
	
	/*�洢�绰״̬ 1:ͨ���� 0:�����У���ֹ��·�绰*/
	void Winks_save_multiCallstate (int state);
	
	/*��ȡ�绰״̬ 1:ͨ���� 0:������,��·�绰ʱʹ��*/
	int Winks_get_multiCallstate ();
	
	void Winks_save_Callstatus ();
	void Winks_delete_Callstatus();
	int Winks_get_Callstatus ();
	
	//panqj add  ����ȫ��ʹ�õĺ���
	char* Winks_save_globelNumber(const char *num);
	
	/* ��ȡָ��ģ����ƶ�Ԫ��
	 * function: Winks_GetShowElement
	 * parameters:
	 *   char *number (in)�绰���룬����ΪNULL��
	 *          ��ͨ������꣨WINKS_DFT_CC_NO��WINKS_PRI_CC_NO��WINKS_BUS_CC_NO�����ﵽ�ض���Ч����
	 *   unsigned long extDataFlag (in)�ⲿ���ݱ�ʶ��
	 *          ΪWINKS_TRUE��ʾ�ⲿ����(pageshow�е�����)��ΪWINKS_FALSE��ʾ�ڲ�����(template��multi-part����)��
	 *   char *eTag (in)����ȡԪ�صı�ʶ������ΪNULL����Ӧtempate�е�CID.
	 *   unsigned long *fileFlag (out)������ʶ���������Ƿ����ļ�����
	 *          ΪWINKS_TRUE��ʾeBuff�з��ص����ļ�������ȡ���ݴ��ڸ��ļ��У�
	 *          ΪWINKS_FALSE��ʾeBuff�д�ŵ������ݿ���ֱ��ʹ�á�
	 *   char * eDataBuf (out)�������ݡ�
	 *   unsigned long *eDataBufSize (in, out)����eDataBufSize��size���������buffSize����ʱ��������Ҫ��size��
	 *          �û�Ӧ�����㹻������ռ䣬���µ��øú�����ȡ���ݡ�
	 *   char * tIdBuf (out)���ض�Ӧ��TID���������Ҫ��ȡ�ò��������Դ���NULL��
	 *   unsigned long tIdBufSize (in)����tIdBuf��size��
	 * return:
	 *   �ɹ�����:0
	 *   ʧ�ܷ���:-1
	 */
	int Winks_GetShowElement(char *number, unsigned long extDataFlag, char *eTag, unsigned long *fileFlag, 
							 char * eDataBuf, unsigned long* eDataBufSize, char *tIdBuf, unsigned long tIdBufSize);	

/*����ȥ����������*/
//void Winks_save_number (const char *num);	
	
/*��ȡȥ����������*/
//char* Winks_get_number ();
	
/*�����汨��绰�¼�*/
int Winks_post_call_event (WinksCallEvent_e event,char* number);
	
	/******************************************************************\
	 ��ʱ�����Ʋ���Ҫcreateֱ����ITIMER_REAL�����Ҳ����Ҫ������ʱ��
	 \******************************************************************/
int Winks_api_StartTimer(unsigned long delay,void (*timerfunc)(int));
	
	/***********************************
	 ��ʱ������������٣�����ֹͣ��ʱ����
	 **********************************/
int Winks_api_StopTimer(void);

//flush��������ʵ����socketд����
ssize_t Winks_Write_flush();
/*��ͻ���socketд���ݣ�ʵ��д��������*/
ssize_t Winks_Write(const void *buf, size_t nbyte);
/*�ӿͻ���socket������*/
ssize_t Winks_Read(void *buf, size_t nbyte);
	
off_t Winks_Lseek(off_t offset, int whence);
	
//��ʼ���ͻ���socket
int Winks_InitSocket();

//�رտͻ���socket
void Winks_CloseSocket();
	
/**************************************************************************\
	 ��������:
	 ��ȡϵͳ��ǰ����������
	 ����˵��:
	 ��
	 ����ֵ:
	 ����WINKSƽ̨ͳһ��������Դ���
	 ʵ������:
\**************************************************************************/
Winks_Language_s Winks_GetCurrentLanguage(void);

/*����unicode�ַ���*/
int winks_ccsw_UnicodeStrcpy( char* pdest, const char* psour );
	
/*��ȡunicode�ַ����ĳ���*/
int winks_strlen_ucs2_type(char * text);
	
/*��ȡδ����״̬ʱ��unicode������ʺ���*/
char* winks_ccsw_GetUniGreeting ();


//�رն�socket�Ļ�����
void Winks_Close_Read_flush();
	
//flush��������ʵ�ʴ�socket������
ssize_t Winks_Read_flush();

#ifdef __cplusplus
}
#endif

#endif/*_WK_ENGIN_H_*/
