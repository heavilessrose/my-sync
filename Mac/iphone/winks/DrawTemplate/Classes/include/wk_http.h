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

//�����붨�壺
#define WINKS_HTTP_SUCCESS    0    //�������ɹ�����    0    
#define WINKS_HTTP_FAILURE    -1   //������ʧ�ܷ���    -1    
#define WINKS_HTTP_EBUFF    -2   //�û��ṩ�Ļ���������    -2    
#define WINKS_HTTP_ESTATUS    -3   //״̬�����������ݿɶ�    -3    
#define WINKS_HTTP_EBLOCK    -4   //������δ��ȫ�����ȴ�    -4    
#define WINKS_HTTP_EBROKE   -5   //�����쳣�ж� -5   
#define WINKS_HTTP_EDATA    -6   //�����쳣
#define WINKS_HTTP_EUSER    -50  //�û�Ҫ��
//ѡ������붨��
#define WINKS_HTTP_OPPIPE    1    //�־����Ӳ���ѡ��    1    
#define WINKS_HTTP_OPUAGENG    2   //User-Agent����ѡ��    2    
#define WINKS_HTTP_OPUPROFILE    3   //UA profile����ѡ��    3    
#define WINKS_HTTP_OPCACHE    4   //Cache��С����ѡ��    4    
#define WINKS_HTTP_OPCOOKIE    5   //Cookie��С����ѡ��    5    

//���䷽������
#define WINKS_HTTP_MEGET    1   //GET        
#define WINKS_HTTP_MEPOST    2   //POST        
#define WINKS_HTTP_MEHEAD    3   //HEAD        
#define WINKS_HTTP_MECONNECT    4   //CONNECT        

//����;������
#define WINKS_HTTP_WAYHTTP    0   //ͨ��HTTPЭ��        
#define WINKS_HTTP_WAYHTTPS    1   //ͨ����ȫ��HTTPЭ��        ���治֧��
#define WINKS_HTTP_WAYFILE    2   //ͨ���ļ�        

#define WINKS_HTTP_WAYNUM   4

//�¼�֪ͨ�붨��
#define WINKS_HTTP_GH        1   //������ѯ�¼�    1    
#define WINKS_HTTP_CONNECT    2   //�����¼�    2    
#define WINKS_HTTP_REDIRECT    3    //�ض���    3
#define WINKS_HTTP_DATAIN    4   //���ݵ����¼�    4
#define WINKS_HTTP_DATAOUT    5   //���ݷ����¼�  5
#define WINKS_HTTP_DATAEND    6   //���ݽ����¼�    6    
#define WINKS_HTTP_CLOSE    7   //����ر��¼�    7    

//HTTP�����붨��
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

//Connect״̬��
#define WINKS_HTTP_CONCLOSE     0
#define WINKS_HTTP_CONDEFAULT   1

//HTTP APPENDDATA API�����������Ͷ���
#define WINKS_HTTP_DTDATA        0
#define WINKS_HTTP_DTFILE        1


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
