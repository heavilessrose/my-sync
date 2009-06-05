    
#ifndef _WK_HTTPI_H_
#define _WK_HTTPI_H_

//�����붨�壺
#define WINKS_HTTP_SUCCESS	0   //�������ɹ�����	0	
#define WINKS_HTTP_FAILURE	-1   //������ʧ�ܷ���	-1	
#define WINKS_HTTP_EBUFF	-2   //�û��ṩ�Ļ���������	-2	
#define WINKS_HTTP_ESTATUS	-3   //״̬�����������ݿɶ�	-3	
#define WINKS_HTTP_EBLOCK	-4   //������δ��ȫ�����ȴ�	-4	
#define WINKS_HTTP_EBROKE   -5   //�����쳣�ж� -5   
#define WINKS_HTTP_EDATA    -6   //�����쳣
#define WINKS_HTTP_EUSER    -50  //�û�Ҫ��
//ѡ������붨��
#define WINKS_HTTP_OPPIPE	1    //�־����Ӳ���ѡ��	1	
#define WINKS_HTTP_OPUAGENG	2   //User-Agent����ѡ��	2	
#define WINKS_HTTP_OPUPROFILE	3   //UA profile����ѡ��	3	
#define WINKS_HTTP_OPCACHE	4   //Cache��С����ѡ��	4	
#define WINKS_HTTP_OPCOOKIE	5   //Cookie��С����ѡ��	5	

//Ԥ��Ĭ������
#define WINKS_HTTP_DEFAULTCACHESIZE     (0)//(10 * 1024)
#define WINKS_HTTP_DEFAULTCOOKIESIZE    (10 * 1024)
//���䷽������
#define WINKS_HTTP_MEGET	1   //GET		
#define WINKS_HTTP_MEPOST	2   //POST		
#define WINKS_HTTP_MEHEAD	3   //HEAD		
#define WINKS_HTTP_MECONNECT	4   //CONNECT		

//����;������
#define WINKS_HTTP_WAYHTTP	0   //ͨ��HTTPЭ��		
#define WINKS_HTTP_WAYHTTPS	1   //ͨ����ȫ��HTTPЭ��		���治֧��
#define WINKS_HTTP_WAYFILE	2   //ͨ���ļ�		

#define WINKS_HTTP_WAYNUM   4

//�¼�֪ͨ�붨��
#define WINKS_HTTP_GH	    1   //������ѯ�¼�	1	
#define WINKS_HTTP_CONNECT	2   //�����¼�	2	
#define WINKS_HTTP_REDIRECT	3	//�ض���	3
#define WINKS_HTTP_DATAIN	4   //���ݵ����¼�	4
#define WINKS_HTTP_DATAOUT	5   //���ݷ����¼�  5
#define WINKS_HTTP_DATAEND	6   //���ݽ����¼�	6	
#define WINKS_HTTP_CLOSE	7   //����ر��¼�	7	

//״̬��״̬�붨��
#define WINKS_HTTP_STIDLE	    0   //����		
#define WINKS_HTTP_STGETHOST	1   //������ѯ		
#define WINKS_HTTP_STCONNECT	2   //����		
#define WINKS_HTTP_STWAITHEADER	3   //�ȴ�ͷ������	
#define WINKS_HTTP_STWAITDATA	4   //�ȴ�����		
#define WINKS_HTTP_STFINISH	    5   //���ݴ������		
#define WINKS_HTTP_STERROR	    6   //���ݴ������		

//״̬���¼��붨��
#define WINKS_HTTP_EVSTART	    1   //�����¼�	1	
#define WINKS_HTTP_EVGETHOST	2   //������ѯ�¼�	2	
#define WINKS_HTTP_EVCONNECT	3   //�����¼�	3	
#define WINKS_HTTP_EVDATAIN	    4   //���ݵ����¼�	4	
#define WINKS_HTTP_EVDATAEND	5   //�����¼�	5	
#define WINKS_HTTP_EVDISCONNECT	6   //�����¼�	6	
#define WINKS_HTTP_EVCLOSE	    7   //�ر��¼�	7	
#define WINKS_HTTP_EVREDIRECT   8   //�ض����¼� 8	
#define WINKS_HTTP_EVDATAOUT    9   //���ݷ����¼� 9

//HTTP�����붨��
#define HTTP_Continue		    100	
#define HTTP_Switching		    101	
#define HTTP_OK		            200	
#define HTTP_Created		    201	
#define HTTP_Accepted		    202	
#define HTTP_NonAuthoritative	203	
#define HTTP_NoContent		    204	
#define HTTP_Reset		        205	
#define HTTP_PartialContent		206	
#define HTTP_MultipleChoices	300	
#define HTTP_MovedPermanently	301	
#define HTTP_Movedtemporarily	302	
#define HTTP_SeeOther		    303	
#define HTTP_Notmodified		304	
#define HTTP_UseProxy		    305	
#define HTTP_BadRequest		    400	
#define HTTP_Unauthorized		401	
#define HTTP_Paymentrequired	402	
#define HTTP_Forbidden		    403	
#define HTTP_NotFound		    404	
#define HTTP_Methodnotallowed	405	
#define HTTP_NotAcceptable		406	
#define HTTP_ProxyAuthenticationrequired		407	
#define HTTP_RequestTimeout		408	
#define HTTP_Conflict		    409	
#define HTTP_Gone		        410	
#define HTTP_LengthRequired		411	
#define HTTP_Preconditionfailed	412	
#define HTTP_Requestedentitytoolarge	413	
#define HTTP_RequestURItoolarge		    414	
#define HTTP_Unsupportedmediatype		415	
#define HTTP_InternalServerError		500	
#define HTTP_NotImplemented		        501	
#define HTTP_BadGateway		    502	
#define HTTP_ServiceUnavailable	503	
#define HTTP_GatewayTimeout		504	
#define HTTP_HTTPversionnotsupported	505	

//Socket���ƿ�״̬��
#define WINKS_HTTP_SOSFREE       0
#define WINKS_HTTP_SOSIDLE       1
#define WINKS_HTTP_SOSCONNECT    2
#define WINKS_HTTP_SOSRCCONNECT  3
#define WINKS_HTTP_SOSTRANS      4
#define WINKS_HTTP_SOSSILENCE    5

//Transfer-Encoding״̬��
#define WINKS_HTTP_UNKNOW       0
#define WINKS_HTTP_CHUNK        1
#define WINKS_HTTP_PLANE        2
//Connect״̬��
#define WINKS_HTTP_CONCLOSE     0
#define WINKS_HTTP_CONDEFAULT   1
//�ڲ�����ÿ���С  
#define WINKS_HTTP_BUFLEN   1024

//�����õ����User-Agent����
#define WINKS_HTTP_MAXUALEN 256
//�����õ����UAProfile����
#define WINKS_HTTP_MAXUPLEN 256

//������ѯ��Ϣ
#define WINKS_HTTP_GHMSG    (WINKS_MSG_UI + 0x100)
//socket��Ϣ
#define WINKS_HTTP_SOCKETMSG    (WINKS_MSG_UI + 0x101)

//�ش�������ֵⷧ rfc2616 P61
#define WINKS_HTTP_REDMAXNUM    5
//Ĭ�����Ӷ˿�
#define WINKS_HTTP_DEFPORT  80

//�־����Ӷ������Դ���
#define WINKS_HTTP_RETNUM   3

//�������ԭ��
#define WINKS_HTTP_EREAUSER     0x100
#define WINKS_HTTP_EREAPACKHEAD 0x101
//socket timeout value
#define WINKS_HTTP_SOTIMEOUT    (WINKS_SECONDS2TICKS(30))
#define WINKS_HTTP_HPTIMEOUT    (WINKS_SECONDS2TICKS(15))


//��������������ṹ
typedef struct tag_Winks_Proxy
{
	char* phostname;
	char* pusername;
	char* ppassword;
	unsigned short port;
	unsigned short unused;
}Winks_Proxy;

//WWW��֤��Ϣ�����ṹ��
typedef struct tag_Winks_Wwwau
{
	char* pusername;
	char* ppassword;
}Winks_Wwwau;

//����������������
typedef struct tag_Winks_Http
{
	char* pURL;
	unsigned short method;
    unsigned short unused;
	Winks_Proxy* pproxy;
	WINKS_CHN_ID channel;
	int msg;
	Winks_Wwwau* pwwwau;
    char* pContenttype;
}Winks_Http;

//HTTPͷ����Ϣ�����ṹ��
typedef struct tag_Winks_HttpInfo
{
	unsigned short mainver;
	unsigned short slavever;
	int statuscode;
	int contentlength;
    int connection;
}Winks_HttpInfo;

//HTTPͷ�����������ṹ��
typedef struct tag_Winks_HttpTag
{
    char* tag;
    char* pvalue;
}Winks_HttpTag;

//HTTP�ڴ�洢�ṹ��
typedef struct tag_Winks_HttpBuff
{
	struct tag_Winks_HttpBuff* pnext;
	unsigned char* pdata;
    int offset;
	int len;
}Winks_HttpBuff;

/* chunk decode state code */
#define WINKS_HTTP_CUIDLE       0
#define WINKS_HTTP_CULENGTH     1
#define WINKS_HTTP_CULENGTHRT   2
#define WINKS_HTTP_CUEXTEN      3
#define WINKS_HTTP_CUEXTENRT    4
#define WINKS_HTTP_CUDATA       5
//#define WINKS_HTTP_CULASTDATA   6
#define WINKS_HTTP_CUTRAILER    7
#define WINKS_HTTP_CUEND        8

//��������ͷ�����ݳ���
#define WINKS_HTTP_MAXTAG       10

//HTTP APPENDDATA API�����������Ͷ���
#define WINKS_HTTP_DTDATA		0
#define WINKS_HTTP_DTFILE		1
//HTTP���ƿ�ṹ��
typedef struct tag_Winks_HttpCB
{
    struct tag_Winks_HttpCB* pnext;
	char* pURL;
	char* phost;
	char* ppath;
    char* pContenttype;
    unsigned short renum;
	unsigned char ifused;
    unsigned char ifredirect;
    unsigned short backmethod;
	unsigned short method;
    unsigned short port;
	unsigned short way;
	Winks_Proxy proxy;
	Winks_Wwwau wwwau;
	Winks_HttpInfo info;
	int state;
    int ereason;
	WINKS_CHN_ID channel;
    struct tag_Winks_HttpSocket* pSocket;
	int msg;
	Winks_HttpTag tag[WINKS_HTTP_MAXTAG];
	Winks_HttpBuff* psend;
	Winks_HttpBuff* precv;
    int GHHandle;
    char pGH[WINKS_SO_GHBFLEN];
    struct winks_sockaddr_in addr;
    int Encoding;
    int Chunkstate;
    int Chunkblock;
    int Chunklen;
    WINKS_TICKS Sendtick;
#ifdef WINKS_DEBUG
    Winks_HttpBuff* ptsend;
	Winks_HttpBuff* ptrecv;
#endif
    WINKS_TICKS Recvtick;
    int RemainRcv;
    int totalsend;
	char* pfilename;		//֧�ַ����ļ�����Ҫ���͵��ļ���
	int fileoff;			//�Ѿ����͵��ļ�����
	FILE* pfile;			//��Ҫ���͵��ļ�ָ�룬�������١�
    Winks_HttpBuff* pdata;
}Winks_HttpCB;

//SOCKET���ƿ�ṹ��
typedef struct tag_Winks_HttpSocket
{ 
    int s;
    int state;
    int retnum;
    int cway;       //�Ƿ�־�����
	struct winks_sockaddr_in addr;
	Winks_HttpCB* phttp;
	WINKS_TIMER_ID timer;
}Winks_HttpSocket;

//HTTPȫ�����ýṹ��
typedef struct tag_Winks_HttpGlobalSetting
{
	int ifpipe;
	int cachesize;
	int cookiesize;
	char pUseragent[WINKS_HTTP_MAXUALEN];
	char pUAprofile[WINKS_HTTP_MAXUPLEN];
    Winks_HttpBuff* pdata;
}Winks_HttpGlobalSetting;

/* ���HTTP���ƿ����� */
#define WINKS_HTTP_MAXHTTPHD    5
#define WINKS_HTTP_MAXSOCKETHD  5

typedef int (* Winks_HttpHeadParse)( Winks_HttpCB* phttp );
//HTTP״̬�ַ����ṹ
#define WINKS_HTTP_HSTATUSMAXLEN	64
typedef struct tag_Winks_HttpStatus
{
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    char pStr[WINKS_HTTP_HSTATUSMAXLEN];
#else
    char *pStr;
#endif
    int code;
    Winks_HttpHeadParse parse;
}Winks_HttpStatus;

//HTTP״̬��������
typedef int (* Winks_HttpFsm)( Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );

/* return 0 means buffer too short, return -1 means can not generate the string, if success, return length of this string */
typedef int (* Winks_HttpHeadDeal)( Winks_HttpCB* phttp, char* pbuf, int* len );
#define WINKS_HTTP_HEADTAGMAXLEN	24
typedef struct tag_Winks_HttpHead_s
{
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    char HeaderTag[WINKS_HTTP_HEADTAGMAXLEN];
#else
    char *HeaderTag;
#endif
    Winks_HttpHeadDeal HttpHeadDeal;
}Winks_HttpHead_s;

#define WINKS_HTTP_MAXSTATUSNUM	40
#define WINKS_HTTP_MAXSENDHEAD	12
#define WINKS_HTTP_MAXRECVHEAD	4
#define WINKS_HTTP_MAXSTATE		8
//HTTPȫ�ֿ��ƿ�ṹ��
typedef struct tag_Winks_HttpGlobalCB
{
	Winks_HttpStatus HttpStatus[WINKS_HTTP_MAXSTATUSNUM];
	Winks_HttpHead_s HttpSendHead[WINKS_HTTP_MAXSENDHEAD];
	Winks_HttpHead_s HttpRecvHead[WINKS_HTTP_MAXRECVHEAD];
	Winks_HttpFsm HttpFsm[WINKS_HTTP_MAXSTATE];

    int ifinit;
	WINKS_CHN_ID channel;
    int hdindex;
	Winks_HttpGlobalSetting Setting;
	Winks_HttpCB HttpHD[WINKS_HTTP_MAXHTTPHD];
	Winks_HttpSocket SocketHD[WINKS_HTTP_MAXSOCKETHD];
}Winks_HttpGlobalCB;


#define WINKS_HTTP_HEADVALLEN   256

#define WINKS_HTTP_HTCONNECTCLOSE   ((char* )"close")
#define WINKS_HTTP_HTTEXT           ((char*)"text/*")

#define WINKS_HTTP_HTCONTLEN        ((char* )"\nContent-Length")
#define WINKS_HTTP_HTTRANSENCODING  ((char* )"\nTransfer-Encoding")
#define WINKS_HTTP_HTLOCATION       ((char* )"\nLocation")

#define WINKS_HTTP_HTDATE           ((char* )"\nDate")
#define WINKS_HTTP_HTEXPIRES        ((char* )"\nExpires")
#define WINKS_HTTP_HTCACHECON       ((char* )"\nCache-Control")
#define WINKS_HTTP_HTLASTMOD        ((char* )"\nLast-Modified")
#define WINKS_HTTP_HTAGE            ((char* )"\nAge")
#define WINKS_HTTP_HTSETCOOKIE2     ((char* )"\nSet-Cookie2")
#define WINKS_HTTP_HTCONTTYPE       ((char*)"\nContent-Type")

#endif