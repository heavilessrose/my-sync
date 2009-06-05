    
#ifndef _WK_HTTPI_H_
#define _WK_HTTPI_H_

//错误码定义：
#define WINKS_HTTP_SUCCESS	0   //本函数成功返回	0	
#define WINKS_HTTP_FAILURE	-1   //本函数失败返回	-1	
#define WINKS_HTTP_EBUFF	-2   //用户提供的缓冲区不够	-2	
#define WINKS_HTTP_ESTATUS	-3   //状态错误，尚无数据可读	-3	
#define WINKS_HTTP_EBLOCK	-4   //数据尚未完全到达，需等待	-4	
#define WINKS_HTTP_EBROKE   -5   //连接异常中断 -5   
#define WINKS_HTTP_EDATA    -6   //数据异常
#define WINKS_HTTP_EUSER    -50  //用户要求
//选项操作码定义
#define WINKS_HTTP_OPPIPE	1    //持久连接操作选项	1	
#define WINKS_HTTP_OPUAGENG	2   //User-Agent操作选项	2	
#define WINKS_HTTP_OPUPROFILE	3   //UA profile操作选项	3	
#define WINKS_HTTP_OPCACHE	4   //Cache大小设置选项	4	
#define WINKS_HTTP_OPCOOKIE	5   //Cookie大小设置选项	5	

//预设默认设置
#define WINKS_HTTP_DEFAULTCACHESIZE     (0)//(10 * 1024)
#define WINKS_HTTP_DEFAULTCOOKIESIZE    (10 * 1024)
//传输方法定义
#define WINKS_HTTP_MEGET	1   //GET		
#define WINKS_HTTP_MEPOST	2   //POST		
#define WINKS_HTTP_MEHEAD	3   //HEAD		
#define WINKS_HTTP_MECONNECT	4   //CONNECT		

//传输途径定义
#define WINKS_HTTP_WAYHTTP	0   //通过HTTP协议		
#define WINKS_HTTP_WAYHTTPS	1   //通过安全的HTTP协议		本版不支持
#define WINKS_HTTP_WAYFILE	2   //通过文件		

#define WINKS_HTTP_WAYNUM   4

//事件通知码定义
#define WINKS_HTTP_GH	    1   //域名查询事件	1	
#define WINKS_HTTP_CONNECT	2   //连接事件	2	
#define WINKS_HTTP_REDIRECT	3	//重定向	3
#define WINKS_HTTP_DATAIN	4   //数据到来事件	4
#define WINKS_HTTP_DATAOUT	5   //数据发送事件  5
#define WINKS_HTTP_DATAEND	6   //数据结束事件	6	
#define WINKS_HTTP_CLOSE	7   //传输关闭事件	7	

//状态机状态码定义
#define WINKS_HTTP_STIDLE	    0   //空闲		
#define WINKS_HTTP_STGETHOST	1   //域名查询		
#define WINKS_HTTP_STCONNECT	2   //连接		
#define WINKS_HTTP_STWAITHEADER	3   //等待头部数据	
#define WINKS_HTTP_STWAITDATA	4   //等待数据		
#define WINKS_HTTP_STFINISH	    5   //数据传输结束		
#define WINKS_HTTP_STERROR	    6   //数据传输错误		

//状态机事件码定义
#define WINKS_HTTP_EVSTART	    1   //启动事件	1	
#define WINKS_HTTP_EVGETHOST	2   //域名查询事件	2	
#define WINKS_HTTP_EVCONNECT	3   //连接事件	3	
#define WINKS_HTTP_EVDATAIN	    4   //数据到来事件	4	
#define WINKS_HTTP_EVDATAEND	5   //断连事件	5	
#define WINKS_HTTP_EVDISCONNECT	6   //断连事件	6	
#define WINKS_HTTP_EVCLOSE	    7   //关闭事件	7	
#define WINKS_HTTP_EVREDIRECT   8   //重定向事件 8	
#define WINKS_HTTP_EVDATAOUT    9   //数据发送事件 9

//HTTP返回码定义
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

//Socket控制块状态码
#define WINKS_HTTP_SOSFREE       0
#define WINKS_HTTP_SOSIDLE       1
#define WINKS_HTTP_SOSCONNECT    2
#define WINKS_HTTP_SOSRCCONNECT  3
#define WINKS_HTTP_SOSTRANS      4
#define WINKS_HTTP_SOSSILENCE    5

//Transfer-Encoding状态码
#define WINKS_HTTP_UNKNOW       0
#define WINKS_HTTP_CHUNK        1
#define WINKS_HTTP_PLANE        2
//Connect状态码
#define WINKS_HTTP_CONCLOSE     0
#define WINKS_HTTP_CONDEFAULT   1
//内部缓存每块大小  
#define WINKS_HTTP_BUFLEN   1024

//可设置的最大User-Agent长度
#define WINKS_HTTP_MAXUALEN 256
//可设置的最大UAProfile长度
#define WINKS_HTTP_MAXUPLEN 256

//域名查询消息
#define WINKS_HTTP_GHMSG    (WINKS_MSG_UI + 0x100)
//socket消息
#define WINKS_HTTP_SOCKETMSG    (WINKS_MSG_UI + 0x101)

//重传次数检测阀值 rfc2616 P61
#define WINKS_HTTP_REDMAXNUM    5
//默认连接端口
#define WINKS_HTTP_DEFPORT  80

//持久连接断连重试次数
#define WINKS_HTTP_RETNUM   3

//句柄错误原因
#define WINKS_HTTP_EREAUSER     0x100
#define WINKS_HTTP_EREAPACKHEAD 0x101
//socket timeout value
#define WINKS_HTTP_SOTIMEOUT    (WINKS_SECONDS2TICKS(30))
#define WINKS_HTTP_HPTIMEOUT    (WINKS_SECONDS2TICKS(15))


//代理服务器描述结构
typedef struct tag_Winks_Proxy
{
	char* phostname;
	char* pusername;
	char* ppassword;
	unsigned short port;
	unsigned short unused;
}Winks_Proxy;

//WWW认证信息描述结构：
typedef struct tag_Winks_Wwwau
{
	char* pusername;
	char* ppassword;
}Winks_Wwwau;

//传输句柄创建参数：
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

//HTTP头部信息描述结构：
typedef struct tag_Winks_HttpInfo
{
	unsigned short mainver;
	unsigned short slavever;
	int statuscode;
	int contentlength;
    int connection;
}Winks_HttpInfo;

//HTTP头部数据描述结构：
typedef struct tag_Winks_HttpTag
{
    char* tag;
    char* pvalue;
}Winks_HttpTag;

//HTTP内存存储结构：
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

//最大可设置头部数据长度
#define WINKS_HTTP_MAXTAG       10

//HTTP APPENDDATA API传入数据类型定义
#define WINKS_HTTP_DTDATA		0
#define WINKS_HTTP_DTFILE		1
//HTTP控制块结构：
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
	char* pfilename;		//支持发送文件，需要发送的文件名
	int fileoff;			//已经发送的文件长度
	FILE* pfile;			//需要发送的文件指针，用来加速。
    Winks_HttpBuff* pdata;
}Winks_HttpCB;

//SOCKET控制块结构：
typedef struct tag_Winks_HttpSocket
{ 
    int s;
    int state;
    int retnum;
    int cway;       //是否持久连接
	struct winks_sockaddr_in addr;
	Winks_HttpCB* phttp;
	WINKS_TIMER_ID timer;
}Winks_HttpSocket;

//HTTP全局设置结构：
typedef struct tag_Winks_HttpGlobalSetting
{
	int ifpipe;
	int cachesize;
	int cookiesize;
	char pUseragent[WINKS_HTTP_MAXUALEN];
	char pUAprofile[WINKS_HTTP_MAXUPLEN];
    Winks_HttpBuff* pdata;
}Winks_HttpGlobalSetting;

/* 最大HTTP控制块数量 */
#define WINKS_HTTP_MAXHTTPHD    5
#define WINKS_HTTP_MAXSOCKETHD  5

typedef int (* Winks_HttpHeadParse)( Winks_HttpCB* phttp );
//HTTP状态字符串结构
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

//HTTP状态机处理函数
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
//HTTP全局控制块结构：
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