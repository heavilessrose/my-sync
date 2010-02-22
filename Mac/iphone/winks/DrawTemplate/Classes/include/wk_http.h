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
*Id: wk_http.h,v 1.00 
**************************************************************************    
* File Description
* ----------------
* Color call HTTP global define. 
**************************************************************************/

#ifndef _WK_HTTP_H_
#define _WK_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

//错误码定义：
#define WINKS_HTTP_SUCCESS    0    //本函数成功返回    0    
#define WINKS_HTTP_FAILURE    -1   //本函数失败返回    -1    
#define WINKS_HTTP_EBUFF    -2   //用户提供的缓冲区不够    -2    
#define WINKS_HTTP_ESTATUS    -3   //状态错误，尚无数据可读    -3    
#define WINKS_HTTP_EBLOCK    -4   //数据尚未完全到达，需等待    -4    
#define WINKS_HTTP_EBROKE   -5   //连接异常中断 -5   
#define WINKS_HTTP_EDATA    -6   //数据异常
#define WINKS_HTTP_EUSER    -50  //用户要求
//选项操作码定义
#define WINKS_HTTP_OPPIPE    1    //持久连接操作选项    1    
#define WINKS_HTTP_OPUAGENG    2   //User-Agent操作选项    2    
#define WINKS_HTTP_OPUPROFILE    3   //UA profile操作选项    3    
#define WINKS_HTTP_OPCACHE    4   //Cache大小设置选项    4    
#define WINKS_HTTP_OPCOOKIE    5   //Cookie大小设置选项    5    

//传输方法定义
#define WINKS_HTTP_MEGET    1   //GET        
#define WINKS_HTTP_MEPOST    2   //POST        
#define WINKS_HTTP_MEHEAD    3   //HEAD        
#define WINKS_HTTP_MECONNECT    4   //CONNECT        

//传输途径定义
#define WINKS_HTTP_WAYHTTP    0   //通过HTTP协议        
#define WINKS_HTTP_WAYHTTPS    1   //通过安全的HTTP协议        本版不支持
#define WINKS_HTTP_WAYFILE    2   //通过文件        

#define WINKS_HTTP_WAYNUM   4

//事件通知码定义
#define WINKS_HTTP_GH        1   //域名查询事件    1    
#define WINKS_HTTP_CONNECT    2   //连接事件    2    
#define WINKS_HTTP_REDIRECT    3    //重定向    3
#define WINKS_HTTP_DATAIN    4   //数据到来事件    4
#define WINKS_HTTP_DATAOUT    5   //数据发送事件  5
#define WINKS_HTTP_DATAEND    6   //数据结束事件    6    
#define WINKS_HTTP_CLOSE    7   //传输关闭事件    7    

//HTTP返回码定义
#define HTTP_Continue            100    
#define HTTP_Switching            101    
#define HTTP_OK                    200    
#define HTTP_Created            201    
#define HTTP_Accepted            202    
#define HTTP_NonAuthoritative    203    
#define HTTP_NoContent            204    
#define HTTP_Reset                205    
#define HTTP_PartialContent        206    
#define HTTP_MultipleChoices    300    
#define HTTP_MovedPermanently    301    
#define HTTP_Movedtemporarily    302    
#define HTTP_SeeOther            303    
#define HTTP_Notmodified        304    
#define HTTP_UseProxy            305    
#define HTTP_BadRequest            400    
#define HTTP_Unauthorized        401    
#define HTTP_Paymentrequired    402    
#define HTTP_Forbidden            403    
#define HTTP_NotFound            404    
#define HTTP_Methodnotallowed    405    
#define HTTP_NotAcceptable        406    
#define HTTP_ProxyAuthenticationrequired        407    
#define HTTP_RequestTimeout        408    
#define HTTP_Conflict            409    
#define HTTP_Gone                410    
#define HTTP_LengthRequired        411    
#define HTTP_Preconditionfailed    412    
#define HTTP_Requestedentitytoolarge    413    
#define HTTP_RequestURItoolarge            414    
#define HTTP_Unsupportedmediatype        415    
#define HTTP_InternalServerError        500    
#define HTTP_NotImplemented                501    
#define HTTP_BadGateway            502    
#define HTTP_ServiceUnavailable    503    
#define HTTP_GatewayTimeout        504    
#define HTTP_HTTPversionnotsupported    505    

//Connect状态码
#define WINKS_HTTP_CONCLOSE     0
#define WINKS_HTTP_CONDEFAULT   1

//HTTP APPENDDATA API传入数据类型定义
#define WINKS_HTTP_DTDATA        0
#define WINKS_HTTP_DTFILE        1


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

int Winks_httpinit(void);
int Winks_httpdeinit(void);
int Winks_httpopen( Winks_Http* phttp );
int Winks_httpsetopt( int opcode, void* pvalue, int vallen );
int Winks_httpgetopt( int opcode, void* pvalue, int* pvallen );
int Winks_httpsethead( int http, char* ptag, char* pvalue );
int Winks_httpappenddata( int http, char* pdata, int len, int dtype  );
int Winks_httpstart( int http );
int Winks_httprecv( int http, Winks_HttpInfo* pInfo, int offset, char* pbuf, int buflen );
int Winks_httpgetinfo( int http, Winks_HttpInfo* pInfo );
int Winks_httppulldata( int http, char* pbuf, int buflen );
int Winks_httpgethead( int http, char* ptag, char* pvalue, int* pvallen );
int Winks_httpgeturl( int http, char* pbuf, int* len );
const char* Winks_httpStatus( int statuscode );
int Winks_httpclose( int http );

#ifdef __cplusplus
}
#endif

#endif
