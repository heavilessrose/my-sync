
#include "wk_public.h"
#include "wk_osal.h"
#include "wk_httpi.h"
#include "wk_cache.h"
#include "wk_cookie.h"


static int Winks_httpParse200( Winks_HttpCB* phttp );
static int Winks_httpParse301( Winks_HttpCB* phttp );
static int Winks_httpParse302( Winks_HttpCB* phttp );
static int Winks_httpParse304( Winks_HttpCB* phttp );
static int Winks_httpParse401( Winks_HttpCB* phttp );
static int Winks_httpParse407( Winks_HttpCB* phttp );

#define WINKS_HTTP_WAYMAXLEN	16
const static char HttpWay[][WINKS_HTTP_WAYMAXLEN] = 
{
    "",
    "http://",
    "https://",
    "file://",
    ""
};

#define WINKS_HTTP_METHODMAXLEN	16
const static char HttpMethod[][WINKS_HTTP_METHODMAXLEN] = 
{
    "",
    "GET ",
    "POST ",
    "HEAD ",
    "CONNECT ",
    ""
};

static int Winks_HttpAcceptGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpHostGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpContentlengthGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpContenttypeGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpAuthoriGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpProxyAuthoriGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpUserAgentGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpUAProfileGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpIfModifiedGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpCookieGen( Winks_HttpCB* phttp, char* pbuf, int* len );
#if 0 
static int Winks_HttpAcCharsetGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpAcLanguageGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpAcEncodingGen( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpConnection( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpPragmaGen( Winks_HttpCB* phttp, char* pbuf, int* len );
#endif
//static int Winks_HttpParseContentLength( Winks_HttpCB* phttp, char* pbuf, int* len );
//static int Winks_HttpParseTransEncoding( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpParseConnection( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpParseSetCookie( Winks_HttpCB* phttp, char* pbuf, int* len );
static int Winks_HttpParseSetCookie2( Winks_HttpCB* phttp, char* pbuf, int* len );

const static char HttpVersion[] = "HTTP/1.1";
const static char HttpLine[] = "\r\n";
const static char HttpLineT[] = "\n";
const static char HttpColon[] = ":";

const static char HttpBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
								    "abcdefghijklmnopqrstuvwxyz"
								    "0123456789+/=";


static int Winks_httpfsmIdle(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );
static int Winks_httpfsmGethost(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );
static int Winks_httpfsmConnect(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );
static int Winks_httpfsmWaitheader(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );
static int Winks_httpfsmWaitdata(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );
static int Winks_httpfsmFinish(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );
static int Winks_httpfsmError(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );

static int Winks_httpfsm(Winks_HttpCB* phttp, int event );
static int Winks_httpSocketNotify(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg );

static int Winks_HttpSockSend( Winks_HttpSocket* psocket );
static int Winks_HttpSockRenew( Winks_HttpSocket* psocket, int ifrecover );
static int Winks_HttpParseURL( Winks_HttpCB* phttp );
static int Winks_FreeSocket( Winks_HttpSocket* psocket );
static int Winks_httpGetDatalen( Winks_HttpCB* phttp );
static int Winks_HttpSockOnClose( Winks_HttpSocket* psocket, Winks_Socketmsg_s* pmsg );

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_HTTPGETGLOBALVAR get_module_glob(WK_MODULE_HTTP)
#else
static Winks_HttpGlobalCB  HttpGlobalCB;
#define WINKS_HTTPGETGLOBALVAR (&HttpGlobalCB)
#endif
/*****************************************************/

/*****************************************************/
static char* Winks_StriStr( char *str, char *substr )
{
	int i = 0, j = 0;
	int mainstrlen = strlen( str );
	int substrlen = strlen( substr );

	while( i < mainstrlen && j < substrlen )
    {
        if( toupper( str[i] ) == toupper( substr[j] ) )
			i++, j++;
		else
			i = i-j+1, j = 0;
    }
	if( j >= substrlen )
		return &str[i-substrlen];
	else
		return NULL;
}

static char* Winks_StrhStr( char *str, char *substr )
{
	int i = 0;

    while( str[i] && substr[i] )
    {
        if( toupper( str[i] ) == toupper( substr[i] ) )
            i++;
        else
            return NULL;
    }

    if( substr[i] == 0 )
        return str;
    else
        return NULL;
}

/********************************************************************
 * 函数功能:	HTTP缓存添加函数
 * 参数意义:	ppBuf		需要添加的缓存
				pData		数据
				Datalen		数据长度
				ifhead		是否是头部数据（第一块）
                ifskipspace 是否跳过空格（URL内容）
                ifeven      是否要求偶字节对齐（Unicode内容）
 * 返回数值:	成功返回填入数据的头指针，失败返回NULL。只要是成功就一定是把所有数据都添加了。
 ********************************************************************/
static char* Winks_HttpBufAppend( Winks_HttpBuff** ppBuf, char* pData, int Datalen, int ifhead, int ifskipspace, int ifeven )
{
	Winks_HttpBuff* pBuf = *ppBuf;
	Winks_HttpBuff* pTemp = NULL;
	int llen = 0, copylen = 0;
    unsigned char* pret = NULL;

	if( ifhead )
	{
		/* 头部数据，保证是一块,而且我们在最后保留一个0结束符，便于字符串查找 */
		if( pBuf != NULL )
		{
			if( pBuf->pnext != NULL  )
			{
				/* Unlikely */
				Winks_printf( "winks http buf add head has pnext\r\n" );
				return NULL;
			}
            if( ifeven )
                /* 因为可能返回Unicode的指针，我们确保起始地址为偶字节对齐 */
                pBuf->offset = (pBuf->offset + 1) & 0xfffffffe;
			if( pBuf->len - pBuf->offset < Datalen + 1 )
			{
				pTemp = Winks_salAlloc( sizeof(Winks_HttpBuff) + pBuf->offset + Datalen + 1 );
                Winks_mem_set( pTemp, 0, sizeof(Winks_HttpBuff) + pBuf->offset + Datalen + 1 );
				pTemp->len = pBuf->offset + Datalen + 1;
				pTemp->pnext = pBuf->pnext;
				pTemp->pdata = (unsigned char* )(pTemp + 1);
				Winks_mem_cpy( pTemp->pdata, pBuf->pdata, pBuf->offset );
                pret = pTemp->pdata + pBuf->offset;

                if( ifskipspace )
                {
                    unsigned char* ptemp1 = NULL;
                    unsigned char* ptemp2 = NULL;
                    int datalen = Datalen;

                    ptemp2 = pret;
                    ptemp1 = (unsigned char* )pData;
                    /* Copy data, skip space char */
                    while( datalen -- )
                    {
                        if( *ptemp1 != ' ' )
                        {
                            if( *ptemp1 == '\\' )
                                *ptemp2 = '/';
                            else
                                *ptemp2 = *ptemp1;
                            ptemp2 ++;
                            pTemp->offset ++;
                        }
                        ptemp1 ++;
                    }
                }
                else
                {
				    Winks_mem_cpy( pTemp->pdata + pBuf->offset, pData, Datalen );
                    pTemp->offset = pBuf->offset + Datalen;
                }
				Winks_salFree( pBuf );
				*ppBuf = pTemp;
			}
			else
			{
                pret = pBuf->pdata + pBuf->offset;
                if( ifskipspace )
                {
                    unsigned char* ptemp1 = NULL;
                    unsigned char* ptemp2 = NULL;
                    int datalen = Datalen;

                    ptemp2 = pret;
                    ptemp1 = (unsigned char* )pData;
                    /* Copy URL data, skip space char */
                    while( datalen -- )
                    {
                        if( *ptemp1 != ' ' )
                        {
                            if( *ptemp1 == '\\' )
                                *ptemp2 = '/';
                            else
                                *ptemp2 = *ptemp1;
                            ptemp2 ++;
                            pBuf->offset ++;
                        }
                        ptemp1 ++;
                    }
                }
                else
                {
                    Winks_mem_cpy( pBuf->pdata + pBuf->offset, pData, Datalen );
                    pBuf->offset += Datalen;
                }
			}
		}
		else
		{
			llen = Datalen + 1 > WINKS_HTTP_BUFLEN ? Datalen + 1 : WINKS_HTTP_BUFLEN;
			pBuf = Winks_salAlloc( sizeof(Winks_HttpBuff) + llen );
            Winks_mem_set( pBuf, 0, sizeof(Winks_HttpBuff) + llen );
			pBuf->len = llen;
			pBuf->pnext = NULL;
			pBuf->pdata = (unsigned char* )(pBuf + 1);
			Winks_mem_cpy( pBuf->pdata, pData, Datalen );
            pret = pBuf->pdata;
			pBuf->offset = Datalen;
			*ppBuf = pBuf;
		}
        
        return (char* )pret;
	}
	
	if( pBuf == NULL )
	{
		/* Unlikely */
		Winks_fail( "winks http buf add data have not head data\r\n" );
		return NULL;
	}

    if( pBuf->pnext == NULL )
    {
        /* 我们还需保证头部仅在第一块，数据会从第二块开始保存 */
        pTemp = Winks_salAlloc( sizeof(Winks_HttpBuff) + WINKS_HTTP_BUFLEN );
        pTemp->len = WINKS_HTTP_BUFLEN;
        pTemp->pnext = NULL;
        pTemp->pdata = (unsigned char* )(pTemp + 1);
        pTemp->offset = 0;
        pBuf->pnext = pTemp;
        pBuf = pTemp;
    }
	else 
    {
        while( pBuf->pnext != NULL )
		    pBuf = pBuf->pnext;
    }

	if( pBuf->len - pBuf->offset > Datalen )
	{
		Winks_mem_cpy( pBuf->pdata + pBuf->offset, pData, Datalen );
		pret = pBuf->pdata + pBuf->offset;
		pBuf->offset += Datalen;
	}
	else
	{
		Winks_mem_cpy( pBuf->pdata + pBuf->offset, pData, pBuf->len - pBuf->offset );
		pret = pBuf->pdata + pBuf->offset;
		pData += pBuf->len - pBuf->offset;
		Datalen -= pBuf->len - pBuf->offset;
		pBuf->offset += pBuf->len - pBuf->offset;
		while( Datalen > 0 )
		{
			pTemp = Winks_salAlloc( sizeof(Winks_HttpBuff) + WINKS_HTTP_BUFLEN );
			pTemp->len = WINKS_HTTP_BUFLEN;
			pTemp->pnext = NULL;
			pTemp->pdata = (unsigned char* )(pTemp + 1);
			copylen = Datalen > WINKS_HTTP_BUFLEN ? WINKS_HTTP_BUFLEN : Datalen;
			Winks_mem_cpy( pTemp->pdata, pData, copylen );
			pTemp->offset = copylen;
			pData += copylen;
			Datalen -= copylen;
			pBuf->pnext = pTemp;
			pBuf = pTemp;
		}
	}

	return (char* )pret;
}

/********************************************************************
 * 函数功能:	HTTP缓存长度计算函数
 * 参数意义:	pBuf	需要计算的缓存
 * 返回数值:	缓存长度，无缓存则返回0。
 ********************************************************************/
static int Winks_HttpBufLen( Winks_HttpBuff* pBuf )
{
	int ret = 0;

	while( pBuf != NULL )
	{
		ret += pBuf->offset;
		pBuf = pBuf->pnext;
	}

	return ret;
}

/********************************************************************
 * 函数功能:	HTTP缓存拷贝函数
 * 参数意义:	pBuf		需要拷贝的缓存
				pData		拷贝数据区
				offset		拷贝位移
				len			拷贝长度
 * 返回数值:	成功返回拷贝的长度，不考虑失败情况
 ********************************************************************/
static int Winks_HttpBufCopy( Winks_HttpBuff* pBuf, char* pData, int offset, int len )
{
	int copylen = 0, copy = 0;

	while( pBuf )
	{
		if( pBuf->offset > offset )
			break;
		offset -= pBuf->offset;
		pBuf = pBuf->pnext;
	}

	while( pBuf && (len > 0) )
	{
		if( offset > 0 )
			copylen = (pBuf->offset - offset) > len ? len : (pBuf->offset - offset);
		else
			copylen = pBuf->offset > len ? len : pBuf->offset;

		Winks_mem_cpy( &pData[copy], &pBuf->pdata[offset], copylen );
		if( offset )
			offset = 0;
		copy += copylen;
		len -= copylen;
		pBuf = pBuf->pnext;
	}

	return copy;
}

/********************************************************************
 * 函数功能:	HTTP缓存抽取函数
 * 参数意义:	phttp		调试状态下可保留删除的数据
				ppBuf		需要抽取的缓存
				pData		抽取数据区
				len			拷贝长度
 * 返回数值:	成功返回抽取的长度，不考虑失败情况
 ********************************************************************/
static int Winks_HttpBufPullup( Winks_HttpCB* phttp, Winks_HttpBuff** ppBuf, char* pData, int len )
{
	int copylen = 0, copy = 0;
	Winks_HttpBuff* pBuf = NULL;
#ifdef WINKS_DEBUG
	Winks_HttpBuff* pTemp = NULL;
#endif
	
	while( *ppBuf && (len > 0) )
	{
		copylen = (*ppBuf)->offset > len ? len : (*ppBuf)->offset;

		Winks_mem_cpy( &pData[copy], (*ppBuf)->pdata, copylen );
		copy += copylen;
		(*ppBuf)->offset -= copylen;
		if( (*ppBuf)->offset <= 0 )
		{
			pBuf = *ppBuf;
			*ppBuf = (*ppBuf)->pnext;
#ifdef WINKS_DEBUG
			pBuf->pnext = NULL;
			if( phttp->ptrecv == NULL )
				phttp->ptrecv = pBuf;
			else
			{
				pTemp = phttp->ptrecv;
				while( pTemp->pnext != NULL )
					pTemp = pTemp->pnext;
				pTemp->pnext = pBuf;
			}
#else
			Winks_salFree( pBuf );
#endif
		}
		else
			(*ppBuf)->pdata += copylen;
		
		len -= copylen;
	}

	return copy;
}

/********************************************************************
 * 函数功能:	HTTP头部结尾删除函数。本函数用来删除已经接收到的HTTP头部信息结尾字符（\r\n\r\n），
                以便可以添加一些头部内容，目前仅仅为了完善304头部内容。
 * 参数意义:	pBuf		保留头部信息的内存区
 * 返回数值:	成功返回删除的长度，失败会返回零。
 ********************************************************************/
static int Winks_HttpBufDelTail( Winks_HttpBuff* pBuf )
{

    if( pBuf->pdata[pBuf->offset - 1] != 0x0a )
        return 0;
    
    if( pBuf->pdata[pBuf->offset - 2] == 0x0d )
    {
        if( pBuf->pdata[pBuf->offset - 3] != 0x0a )
            return 0;
        pBuf->offset -= 2;
        return 2;
    }
    else if( pBuf->pdata[pBuf->offset - 2] == 0x0a )
    {
        pBuf->offset -= 1;
        return 1;
    }
    
    return 0;

}

/********************************************************************
 * 函数功能:	HTTP缓存释放函数
 * 参数意义:	ppBuf	需要释放的缓存
 * 返回数值:	成功——0
 ********************************************************************/
static int Winks_HttpBufFree( Winks_HttpBuff** ppBuf )
{
	Winks_HttpBuff* pBuf = *ppBuf;
	Winks_HttpBuff* pTemp = NULL;

	while( pBuf != NULL )
	{
		pTemp = pBuf;
		pBuf = pBuf->pnext;
		Winks_salFree( pTemp );
	}

	*ppBuf = NULL;

	return 0;
}

static int Winks_HttpBufAppendChar( Winks_HttpBuff** ppBuf, char c, int ifhead )
{
    Winks_HttpBuff* pBuf = *ppBuf;
    Winks_HttpBuff* pTemp = NULL;

    if( pBuf == NULL )
    {
        pBuf = Winks_salAlloc( sizeof(Winks_HttpBuff) + WINKS_HTTP_BUFLEN );
        pBuf->len = WINKS_HTTP_BUFLEN;
        pBuf->pnext = NULL;
        pBuf->pdata = (unsigned char* )(pBuf + 1);
        pBuf->offset = 0;
        pBuf->pdata[0] = c;
        pBuf->offset ++;
        *ppBuf = pBuf;

        return 1;
    }

    if( ifhead )
    {
        if( pBuf->pnext != NULL  )
        {
            /* Unlikely */
            Winks_fail( "winks http buf add head has pnext\r\n" );
            return -1;
        }
        if( pBuf->len == pBuf->offset + 1 )
        {
            pTemp = Winks_salAlloc( sizeof(Winks_HttpBuff) + pBuf->offset + 1 + 1 );
            Winks_mem_set( pTemp, 0, sizeof(Winks_HttpBuff) + pBuf->offset + 1 + 1 );
            pTemp->len = pBuf->offset + 1 + 1;
            pTemp->pnext = NULL;
            pTemp->pdata = (unsigned char* )(pTemp + 1);
            Winks_mem_cpy( pTemp->pdata, pBuf->pdata, pBuf->offset );
            pTemp->pdata[pBuf->offset] = c;
            pTemp->offset = pBuf->offset + 1;
            Winks_salFree( pBuf );
            *ppBuf = pTemp;
        }
        else
        {
            pBuf->pdata[pBuf->offset] = c;
            pBuf->offset ++;
        }
        
        return 1;
    }

    while( pBuf->pnext )
        pBuf = pBuf->pnext;

    if( pBuf->offset <= pBuf->len - 1 )
    {
        pBuf->pdata[pBuf->offset] = c;
        pBuf->offset ++;
    }
    else
    {
        pTemp = Winks_salAlloc( sizeof(Winks_HttpBuff) + WINKS_HTTP_BUFLEN );
        pTemp->len = WINKS_HTTP_BUFLEN;
        pTemp->pnext = NULL;
        pTemp->pdata = (unsigned char* )(pTemp + 1);
        pTemp->offset = 0;
        pBuf->pnext = pTemp;
        pBuf = pTemp;
        pBuf->pdata[0] = c;
        pBuf->offset ++;
    }

    return 1;
}

static int Winks_HttpSetSocketState( Winks_HttpSocket* psocket, int state )
{
    Winks_printf( "Winks http psocket %x s %d change state from %d to %d\r\n", psocket, psocket->s, psocket->state, state );
    psocket->state = state;

    return 0;
}

static int Winks_HttpAppendChunk( Winks_HttpCB* phttp, char* pdata, int len )
{
    int off = 0;
    char c = 0;

    while( off < len )
    {
        switch( phttp->Chunkstate )
        {
        case WINKS_HTTP_CUIDLE:
            if( pdata[off] == '\r' )
                off ++;
            if( pdata[off] == '\n' )
            {
                off++;
                phttp->Chunkstate = WINKS_HTTP_CULENGTH;
            }
            else
                phttp->Chunkstate = WINKS_HTTP_CULENGTH;
            break;

        case WINKS_HTTP_CULENGTH:
            c = pdata[off++];
            if( c == '\r' )
                phttp->Chunkstate = WINKS_HTTP_CULENGTHRT;
            else if( c == '\n' )
            {
                if( phttp->Chunklen == 0 )
                    phttp->Chunkstate = WINKS_HTTP_CUTRAILER;
                else
                    phttp->Chunkstate = WINKS_HTTP_CUDATA;
            }
            else if( c == ';' )
                phttp->Chunkstate = WINKS_HTTP_CUEXTEN;
            else
            {
                if( c >= '0' && c <= '9' )
                    phttp->Chunklen = (phttp->Chunklen << 4) | (c - '0');
                else if( c >= 'a' && c <= 'f' )
                    phttp->Chunklen = (phttp->Chunklen << 4) | (c - 'a' + 0x0a);
                else if( c >= 'A' && c <= 'F' )
                    phttp->Chunklen = (phttp->Chunklen << 4) | (c - 'A' + 0x0a);
                else
                {
                    Winks_printf( "WINKS http chunk data length error\r\n" );
                    continue;
                }
            }
                
            break;
        
        case WINKS_HTTP_CUEXTEN:
            c = pdata[off++];
            if( c == '\r' )
                phttp->Chunkstate = WINKS_HTTP_CUEXTENRT;
            else if( c == '\n' )
            {
                if( phttp->Chunklen == 0 )
                    phttp->Chunkstate = WINKS_HTTP_CUTRAILER;
                else
                    phttp->Chunkstate = WINKS_HTTP_CUDATA;
            }
            break;


        case WINKS_HTTP_CULENGTHRT:
        case WINKS_HTTP_CUEXTENRT:
            if( pdata[off++] == '\n' )
            {
                if( phttp->Chunklen == 0 )
                    phttp->Chunkstate = WINKS_HTTP_CUTRAILER;
                else
                    phttp->Chunkstate = WINKS_HTTP_CUDATA;
            }
            else
            {
                Winks_printf( "WINKS http chunk data error\r\n" );
                return -1;
            }
            break;

        case WINKS_HTTP_CUDATA:
            if( len - off < phttp->Chunklen )
            {
                Winks_HttpBufAppend( &(phttp->precv), &(pdata[off]), len - off, 0, 0, 0 );
                phttp->Chunklen -= len - off;
                off += len - off;
            }
            else
            {
                Winks_HttpBufAppend( &(phttp->precv), &(pdata[off]), phttp->Chunklen, 0, 0, 0 );
                off += phttp->Chunklen;
                phttp->Chunklen = 0;
                phttp->Chunkstate = WINKS_HTTP_CUIDLE;
                phttp->Chunkblock ++;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAIN, NULL );
            }
            break;
/*        case WINKS_HTTP_CULASTDATA:
            if( pdata[off++] == '\n' )
                phttp->Chunkstate = WINKS_HTTP_CUTRAILER;
            break;
*/
        case WINKS_HTTP_CUTRAILER:
            if( pdata[off++] == '\n' )
            {
                phttp->Chunkstate = WINKS_HTTP_CUEND;
                if( phttp->pSocket->phttp != phttp )
                {
                    Winks_fail( "Winks http data end link error\r\n" );
                    return -1;
                }
                phttp->pSocket->phttp = phttp->pnext;
                phttp->pnext = NULL;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, NULL );

                return 1;
            }

            break;

        default:
            Winks_printf( "WINKS http chunk data in error state\r\n" );
            return -1;
        }
    }

    return 0;
}

static int Winks_HttpChunkUpdate( Winks_HttpCB* phttp )
{
    Winks_HttpBuff* pBuf = phttp->precv->pnext;

    phttp->precv->pnext = NULL;

    while( pBuf )
    {
        Winks_HttpAppendChunk( phttp, (char* )(pBuf->pdata), pBuf->offset );
        pBuf = pBuf->pnext;
    }

    return 0;
}

/* return 1 means send dataend msg to upper, return 0 means normal success */
static int Winks_HttpAppendData( Winks_HttpCB* phttp, char* pdata, int rlen )
{
    int ret = 0;

    if( phttp->Encoding == WINKS_HTTP_CHUNK )
    {
        ret = Winks_HttpAppendChunk( phttp, pdata, rlen );
        return ret;
    }
    else 
    {
        Winks_HttpBufAppend( &(phttp->precv), pdata, rlen, 0, 0, 0 );
        Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAIN, NULL );
        if( phttp->Encoding == WINKS_HTTP_PLANE )
        {
            phttp->RemainRcv -= rlen;
            
            if( phttp->RemainRcv <= 0 )
            {
                if( phttp->RemainRcv < 0 )
                    Winks_fail( "Winks http get a data parse error\r\n" );
                if( phttp->pSocket )
                {
                    if( phttp->pSocket->phttp != phttp )
                    {
                        Winks_fail( "Winks http data end link error\r\n" );
                        return -1;
                    }
                    phttp->pSocket->phttp = phttp->pnext;
                    phttp->pnext = NULL;
                    Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, NULL );
                }
                return 1;
            }
        }
    }

    return 0;
}

static int Winks_HttpSockOnConnect( Winks_HttpSocket* psocket, Winks_Socketmsg_s* pmsg )
{
    Winks_HttpCB* phttp = NULL;

    if( (psocket->state != WINKS_HTTP_SOSCONNECT) && (psocket->state != WINKS_HTTP_SOSRCCONNECT) )
    {
        Winks_printf( "WINKS http socket get connect msg in error state %x %d\r\n", psocket, psocket->state );
        return -1;
    }

    Winks_StopTimer( psocket->timer );

    if( pmsg->lParam >> 16 )
    {
        /* connect failure */
        while( (phttp = psocket->phttp) != NULL )
        {
            /* note list should notify disconnect message */
            psocket->phttp = psocket->phttp->pnext;
            phttp->pnext = NULL;
            Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDISCONNECT, pmsg );
        }
        Winks_printf( "WINKS http socket connect failure, freesocket\r\n" );
        Winks_FreeSocket( psocket );
    }
    else
    {
        /* connect success */
        phttp = psocket->phttp;
        if( phttp != NULL )
        {
            if( phttp->state == WINKS_HTTP_SOSCONNECT )
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVCONNECT, pmsg );
        }
        Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSTRANS );
        //psocket->state = WINKS_HTTP_SOSTRANS;

        Winks_HttpSockSend( psocket );
    }

    return 0;
}

/* return 0 means recv data normal, 1 means we send dataend to upper, 2 means we send dataend and renew socket, 
   lower then 0 means some error occured */
static int Winks_HttpDatain( Winks_HttpCB* phttp, char* pdata, int rlen, Winks_Socketmsg_s* pmsg )
{
    char* precv = pdata;
    int ret = 0;
    Winks_HttpBuff* pBuf = NULL;

    while( /*strlen(pdata)*/ rlen > 0 )
    {
        if( phttp->state == WINKS_HTTP_STWAITHEADER )
        {
            if( (pdata = Winks_StriStr(precv, "\r\n\r\n")) == NULL )
            {
                pdata = Winks_StriStr(precv, "\n\n");
            }
            if( pdata == NULL )
            {
                Winks_HttpBufAppend( &(phttp->precv), precv, rlen, 1, 0, 0 );
                if( (pdata = Winks_StriStr((char* )(phttp->precv->pdata), (char* )"\r\n\r\n")) == NULL )
                {
                    pdata = Winks_StriStr((char* )(phttp->precv->pdata), (char* )"\n\n");
                }
                if( pdata != NULL )
                {
                    pBuf = phttp->precv;
                    phttp->precv = NULL;
                    Winks_HttpBufAppend( &(phttp->precv), (char* )(pBuf->pdata), 
                        ((*pdata == '\r') ? (pdata + 4 - (char* )(pBuf->pdata)) : (pdata + 2 - (char* )(pBuf->pdata))), 1, 0, 0 );
                    pdata += (*pdata == '\r') ? 4 : 2;
                    rlen -= pdata - (char* )(pBuf->pdata);
                }
                else
                    break;
            }
            else
            {
                Winks_HttpBufAppend( &(phttp->precv), precv, 
                    ((*pdata == '\r') ? (pdata + 4 - precv) : (pdata + 2 - precv)), 1, 0, 0 ); 
                pdata += (*pdata == '\r') ? 4 : 2;
                rlen -= pdata - precv;
            }
            Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAIN, pmsg );
        }
        else if( phttp->state == WINKS_HTTP_STWAITDATA )
        {
            /* notify in append function to do */
            ret = Winks_HttpAppendData( phttp, pdata, rlen );
            break;                
        }
        else
        {
            Winks_fail( "Winks http get a data in error http state\r\n" );
            if( pBuf )
            {
                Winks_salFree( pBuf );
                pBuf = NULL;
            }
            return -1;
        }
        
    } 
    if( pBuf )
    {
        Winks_salFree( pBuf );
        pBuf = NULL;
    }

    return ret;
}

static int Winks_HttpSockOnRead( Winks_HttpSocket* psocket, Winks_Socketmsg_s* pmsg )
{
#define WINKS_HTTP_RCVNUM   1028
    Winks_HttpCB* phttp = NULL;
    char precv[WINKS_HTTP_RCVNUM];
    int rlen = 1, ret = 0;
#ifdef WINKS_DEBUG
    int tlen = 0;
    int flags = 0;
#endif

    if( psocket->state != WINKS_HTTP_SOSTRANS )
    {
        while( (rlen = Winks_recv( psocket->s, precv, WINKS_HTTP_RCVNUM - 4, 0 )) > 0 )
            ;
        if( (rlen == 0) 
            || ((rlen < 0) && (Winks_getlasterror() && (Winks_getlasterror() != WINKS_SO_EWOULDBLOCK))) )
        {
            Winks_HttpSockOnClose( psocket, pmsg );
            Winks_printf( "WINKS http socket datain in for close message %d\r\n", psocket->state );
        }
        else
            Winks_printf( "WINKS http socket datain in error state %d\r\n", psocket->state );
        return -1;
    }

    if( (phttp = psocket->phttp) == NULL )
    {
        Winks_fail( "Winks http get a data in no http cb\r\n" );
        return -1; 
    }

    while( (rlen = Winks_recv( psocket->s, precv, WINKS_HTTP_RCVNUM - 4, 0 )) > 0 )
    {
#if 0
#ifdef WINKS_DEBUG
        char pMsg[1024];
        
        sprintf( pMsg, "Winks Http %s receive raw data", phttp->pURL );
        /*Winks_printHex( (const unsigned char* )(precv), (unsigned short )rlen, 
            (const unsigned char* )pMsg );*/
        if(rlen < 1024)
        {
            Winks_printHex( (const unsigned char* )(precv), (unsigned short )(rlen>1024?32:rlen), 
            (const unsigned char* )pMsg );
        }
        Winks_assert( flags == 0 );
        tlen += rlen;
#endif
#endif
        if( phttp->precv == NULL )
		{
			Winks_DateTime_s tm;
			
			Winks_GetSysDateTime( &tm );
			phttp->Recvtick = Winks_mktime( &tm );
		}

        precv[rlen] = 0;
        if( (ret = Winks_HttpDatain( phttp, precv, rlen, pmsg )) < 0 )
        {
            Winks_printf( "Winks http %s Winks_recv get a data in parse error\r\n", phttp->pURL );
            return -1;
        }
        if( (phttp->Encoding == WINKS_HTTP_PLANE && phttp->info.contentlength == 0)
            || (phttp->info.statuscode == HTTP_Notmodified)
            || (phttp->info.statuscode > HTTP_Notmodified 
                && phttp->Encoding == WINKS_HTTP_UNKNOW 
                && phttp->info.contentlength == 0) )
        {
            /* 如果头部状态码或编码方式被置位则说明头部被解析了 */
            /* only header */
            if( phttp->info.statuscode == HTTP_Notmodified )
            {
                psocket->phttp = psocket->phttp->pnext;
                phttp->pnext = NULL;
                phttp->pSocket = NULL;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, pmsg );
                ret = 1;
#ifdef WINKS_DEBUG
                flags = 1;
#endif
            }
            else if( (phttp->info.statuscode > HTTP_Notmodified 
                    && phttp->Encoding == WINKS_HTTP_UNKNOW 
                    && phttp->info.contentlength == 0) )
            {
                /* 服务器发送的头部信息中没有数据长度信息同时头部信息足够我们进行处理，服务器可能不会在发送后续数据了
                我们为了避免万一服务器发送后续数据导致处理出错，更新socket */
                psocket->phttp = psocket->phttp->pnext;
                phttp->pnext = NULL;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, pmsg );
                if( psocket->phttp )
                    Winks_HttpSockRenew( psocket, 0 );
                else
                {
                    Winks_printf( "WINKS http socket recv failure and no http, freesocket\r\n" );
                    Winks_FreeSocket( psocket );
                }
                ret = 2;
                break;
            }
            else
            {
                psocket->phttp = psocket->phttp->pnext;
                phttp->pnext = NULL;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, pmsg );
                ret = 1;
#ifdef WINKS_DEBUG
                flags = 1;
#endif
            }
        }
        
    }
#ifdef WINKS_DEBUG
    Winks_printf( "WINKS http data recv end, total len %d\r\n", tlen );
#endif

    if( (rlen == 0) 
        || ((rlen < 0) && (Winks_getlasterror() && (Winks_getlasterror() != WINKS_SO_EWOULDBLOCK))) )
    {
        if( ret != 1 )
        {
            phttp = psocket->phttp;
            psocket->phttp = psocket->phttp->pnext;
            phttp->pnext = NULL;
            if( phttp->precv )
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, pmsg );
            else
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDISCONNECT, pmsg );
        }
        if( psocket->phttp )
            Winks_HttpSockRenew( psocket, 0 );
        else
        {
            Winks_printf( "WINKS http socket recv failure and no http, freesocket\r\n" );
            Winks_FreeSocket( psocket );
        }
        
        return 0;
    }


    if( ret == 1 )
    {
        if( psocket->phttp == NULL )
        {
            if( psocket->cway )
            {
                Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSSILENCE );
                //psocket->state = WINKS_HTTP_SOSSILENCE;
                Winks_printf( "WINKS http socket recv success, presistent connect, starttimer\r\n" );
                Winks_StartTimer( psocket->timer );
            }
            else
            {
                Winks_printf( "WINKS http socket recv success, not presistent connect, freesocket\r\n" );
                Winks_FreeSocket( psocket );
            }
        }
        else
        {
            if( psocket->cway )
            {
                Winks_httpSocketNotify( psocket->phttp, WINKS_HTTP_EVCONNECT, NULL );
                Winks_HttpSockSend( psocket );
            }
            else
                Winks_HttpSockRenew( psocket, 0 );
        }
    }

    return 0;
}

static int Winks_HttpSockOnWrite( Winks_HttpSocket* psocket, Winks_Socketmsg_s* pmsg )
{
    Winks_HttpSockSend( psocket );

    return 0;
}

static int Winks_HttpSockOnClose( Winks_HttpSocket* psocket, Winks_Socketmsg_s* pmsg )
{
    Winks_HttpCB* phttp = NULL;

    if( psocket->state == WINKS_HTTP_SOSCONNECT || psocket->state == WINKS_HTTP_SOSRCCONNECT )
    {
        while( (phttp = psocket->phttp) != NULL )
        {
            psocket->phttp = psocket->phttp->pnext;
            phttp->pnext = NULL;
            Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDISCONNECT, pmsg );
        }
        Winks_printf( "WINKS http socket close msg received in connect state, freesocket\r\n" );
        Winks_FreeSocket( psocket );
    }
    else if( psocket->state == WINKS_HTTP_SOSTRANS )
    {
        phttp = psocket->phttp;
        psocket->phttp = psocket->phttp->pnext;
        phttp->pnext = NULL;
        Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAEND, pmsg );
        if( psocket->phttp )
            Winks_HttpSockRenew( psocket, 0 );
        else
        {
            Winks_printf( "WINKS http socket close msg received in trans state and no other http, freesocket\r\n" );
            Winks_FreeSocket( psocket );
        }
    }
    else if( psocket->state == WINKS_HTTP_SOSSILENCE )
    {
        Winks_printf( "WINKS http socket close msg received in silence state, freesocket\r\n" );
        Winks_FreeSocket( psocket );
    }
    else
    {
        Winks_printf( "WINKS http socket get close msg in error state\r\n" );
        return 0;
    }
    
    return 0;
}

static int Winks_Http_MsgProc( unsigned long msg, void *data, unsigned long size )
{
    Winks_Socketmsg_s* pmsg = data;
    Winks_HttpSocket* psocket = NULL;
    Winks_HttpCB* phttp = NULL;
    WINKS_CHN_ID timerid = 0;
    int i = 0;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    switch( msg )
    {
    case WINKS_HTTP_GHMSG:
        Winks_printf( "WINKS http msg function get gh msg\r\n" );
        if( pmsg == NULL || size < sizeof(Winks_Socketmsg_s) )
        {
            Winks_printf( "WINKS http msg function gh msg no data\r\n" );
            return -1;
        }
        for( i = 0; i < WINKS_HTTP_MAXHTTPHD; i++ )
        {
            if( pHttpGlobalCB->HttpHD[i].GHHandle == (int)(pmsg->wParam) )
            {
                Winks_httpSocketNotify( &(pHttpGlobalCB->HttpHD[i]), WINKS_HTTP_EVGETHOST, data );
                return 0;
            }
        }
        break;

    case WINKS_HTTP_SOCKETMSG:
        if( pmsg == NULL || size < sizeof(Winks_Socketmsg_s) )
        {
            Winks_printf( "WINKS http msg function socket msg no data\r\n" );
            return -1;
        }
        for( i = 0; i < WINKS_HTTP_MAXHTTPHD; i++ )
        {
            if( pHttpGlobalCB->SocketHD[i].s == (int)(pmsg->wParam) )
            {
                psocket = &(pHttpGlobalCB->SocketHD[i]);
                break;
            }
        }
        if( i == WINKS_HTTP_MAXHTTPHD )
        {
            Winks_printf( "WINKS http msg function socket msg no cb\r\n" );
            return -1;
        }
        
        Winks_printf( "WINKS http msg function socket msg get socket msg, s %d, event %d\r\n", psocket->s, pmsg->lParam & 0xffff );
        switch( pmsg->lParam & 0xffff )
        {
        case WINKS_SO_CONNECT:
            Winks_HttpSockOnConnect( psocket, pmsg );
            break;

        case WINKS_SO_READ:
            Winks_HttpSockOnRead( psocket, pmsg );
            break;

        case WINKS_SO_WRITE:
            Winks_HttpSockOnWrite( psocket, pmsg );
            break;

        case WINKS_SO_CLOSE:
            Winks_HttpSockOnClose( psocket, pmsg );
            break;

        default:
            Winks_printf( "WINKS http msg function socket unknow event\r\n" );
            break;
        }
        
        break;
    case WINKS_MSG_TIMER_EXPIRY:
        if( data == NULL )
        {
            Winks_printf( "WINKS http timer msg no data\r\n" );
            return -1;
        }
        timerid = *((WINKS_CHN_ID* )data);
        for( i = 0; i < WINKS_HTTP_MAXSOCKETHD; i++ )
        {
            if(pHttpGlobalCB->SocketHD[i].timer == timerid)
            {
                if( pHttpGlobalCB->SocketHD[i].state == WINKS_HTTP_SOSSILENCE )
                {
                    Winks_printf( "WINKS http timer msg idle time out\r\n" );
                    Winks_FreeSocket( &(pHttpGlobalCB->SocketHD[i]) );
                    break;
                }
                else if( pHttpGlobalCB->SocketHD[i].state == WINKS_HTTP_SOSCONNECT 
                    || pHttpGlobalCB->SocketHD[i].state == WINKS_HTTP_SOSRCCONNECT )
                {
                    while( pHttpGlobalCB->SocketHD[i].phttp != NULL )
                    {
                        phttp = pHttpGlobalCB->SocketHD[i].phttp;
                        pHttpGlobalCB->SocketHD[i].phttp = phttp->pnext;
                        phttp->pnext = NULL;
                        Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDISCONNECT, NULL );
                    }
                    Winks_printf( "WINKS http socket connect timeout, freesocket\r\n" );
                    Winks_FreeSocket( &(pHttpGlobalCB->SocketHD[i]) );
                    break;
                }
#ifdef WINKS_DEBUG
                else
                    Winks_fail( "winks http timeout but state not correct\r\n" );
#endif
            }
        }

        break;
    default:
        Winks_printf( "WINKS http msg function unknow msg\r\n" );
        break;
    }
    return -1;
}

static int Winks_FreeSocket( Winks_HttpSocket* psocket )
{
    Winks_printf( "WINKS http socket freesocket %x, socket num %d\r\n", psocket, psocket->s );
    if( psocket->s > 0 )
        Winks_closesocket( psocket->s );
    if( psocket->timer != 0 )
        Winks_DestroyTimer( psocket->timer );

    Winks_mem_set( psocket, 0, sizeof(Winks_HttpSocket) );

    return 0;
}
/**********************************************************************************\
函数原型：static int Winks_FindSocket( struct winks_sockaddr_in* paddr )
函数介绍：
		HTTP数据传输控制块查找函数。
参数说明：
        参数名称	参数类型	        参数介绍	        备注
        psocket	    Winks_HttpSocket*	HTTP数据传输控制块	
        phttp	    Winks_HttpCB*	    HTTP传输控制块	

函数返回值：
        成功返回0，失败返回-1。
实现说明：	
        本函数按照指定地址查找可用的socket控制块。如果查找失败，http层会将该控制块搁置，等到
        有了空闲的socket控制块以后，数据传输模块需要自己去http数据解析模块查找有没有处于搁置
        状态的控制块，以便进行传输工作。
\*********************************************************************************/
static Winks_HttpSocket* Winks_FindSocket( struct winks_sockaddr_in* paddr )
{
    int i = 0;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    for( i = 0; i < WINKS_HTTP_MAXSOCKETHD; i++ )
    {
        if( pHttpGlobalCB->SocketHD[i].state != WINKS_HTTP_SOSFREE
            //&& pHttpGlobalCB->SocketHD[i].cway
            && pHttpGlobalCB->SocketHD[i].addr.sin_family == paddr->sin_family
            && pHttpGlobalCB->SocketHD[i].addr.sin_port == paddr->sin_port
            && pHttpGlobalCB->SocketHD[i].addr.sin_addr.wk_addr == paddr->sin_addr.wk_addr )
        {
            Winks_printf( "WINKS http socket find a alive socket %x socket num %d, cway %d\r\n", &(pHttpGlobalCB->SocketHD[i]),
                pHttpGlobalCB->SocketHD[i].s, pHttpGlobalCB->SocketHD[i].cway );
            return &(pHttpGlobalCB->SocketHD[i]);
        }
    }

    for( i = 0; i < WINKS_HTTP_MAXSOCKETHD; i++ )
    {
        if( pHttpGlobalCB->SocketHD[i].state == WINKS_HTTP_SOSFREE )
        {
            if( (pHttpGlobalCB->SocketHD[i].s = Winks_socket( AF_INET, SOCK_STREAM, 0 )) > 0 )
            {
				unsigned long timeout = 0;

//				if( (Winks_GetConnectTimeout(&timeout) != 0) || (timeout == 0) )
					timeout = WINKS_HTTP_SOTIMEOUT;
				
                if( (pHttpGlobalCB->SocketHD[i].timer = Winks_CreateTimer( pHttpGlobalCB->channel, WINKS_SECONDS2TICKS(timeout), WINKS_TIMER_DEFAULT )) == 0 )
                {
                    Winks_closesocket( pHttpGlobalCB->SocketHD[i].s );
                    Winks_printf( "WINKS http find socket handle create timer failure\r\n" );
                    return NULL;
                }
                Winks_printf( "WINKS http socket find a new socket %x, socket num %d\r\n", &(pHttpGlobalCB->SocketHD[i]),
                    pHttpGlobalCB->SocketHD[i].s );
                Winks_asyncselect( pHttpGlobalCB->SocketHD[i].s, WINKS_SO_CONNECT | WINKS_SO_READ | WINKS_SO_WRITE | WINKS_SO_CLOSE,
                    pHttpGlobalCB->channel, WINKS_HTTP_SOCKETMSG );
                
                Winks_mem_cpy( &(pHttpGlobalCB->SocketHD[i].addr), paddr, sizeof(struct winks_sockaddr_in) );
                pHttpGlobalCB->SocketHD[i].cway = pHttpGlobalCB->Setting.ifpipe;
                Winks_HttpSetSocketState( &(pHttpGlobalCB->SocketHD[i]), WINKS_HTTP_SOSIDLE );
                //pHttpGlobalCB->SocketHD[i].state = WINKS_HTTP_SOSIDLE;
                return &(pHttpGlobalCB->SocketHD[i]);
            }
            else
            {
                Winks_printf( "WINKS http find socket handle create socket failure\r\n" );
                return NULL;
            }
        }
    }

    Winks_printf( "WINKS http find socket handle cb lacked\r\n" );
    return NULL;
}

static int Winks_HttpSockRenew( Winks_HttpSocket* psocket, int ifrecover )
{
    Winks_HttpCB* ptemp = NULL;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;


    Winks_printf( "WINKS http socket renew a socket %x\r\n", psocket );
    if( psocket->phttp == NULL )
    {
        Winks_printf( "WINKS http socket renew a socket no httphd %x\r\n", psocket );
        return 0;
    }

    if( ifrecover && (psocket->retnum ++ > WINKS_HTTP_RETNUM) )
    {
        while( psocket->phttp != NULL )
        {
            ptemp = psocket->phttp;
            psocket->phttp = ptemp->pnext;
            ptemp->pnext = NULL;
            Winks_httpSocketNotify( ptemp, WINKS_HTTP_EVDISCONNECT, NULL );
        }
        Winks_printf( "WINKS http socket renew retry time out, freesocket\r\n" );
        Winks_FreeSocket( psocket );

        return 0;
    }

    Winks_closesocket( psocket->s );
    psocket->s = Winks_socket( AF_INET, SOCK_STREAM, 0 );
    if( psocket->s <= 0 )
    {
        while( psocket->phttp != NULL )
        {
            ptemp = psocket->phttp;
            psocket->phttp = ptemp->pnext;
            ptemp->pnext = NULL;
            Winks_httpSocketNotify( ptemp, WINKS_HTTP_EVDISCONNECT, NULL );
        }
        Winks_printf( "WINKS http socket renew socket create error, freesocket\r\n" );
        Winks_FreeSocket( psocket );
    }
    Winks_asyncselect( psocket->s, WINKS_SO_CONNECT | WINKS_SO_READ | WINKS_SO_WRITE | WINKS_SO_CLOSE,
        pHttpGlobalCB->channel, WINKS_HTTP_SOCKETMSG );
    Winks_printf( "Winks http socket renew goto connect %d.%d.%d.%d:%x\r\n", psocket->addr.sin_addr.wk_addr & 0x00ff,
        (psocket->addr.sin_addr.wk_addr >> 8) & 0x00ff, (psocket->addr.sin_addr.wk_addr >> 16) & 0x00ff, 
        (psocket->addr.sin_addr.wk_addr >> 24) & 0x00ff, psocket->addr.sin_port );
    if( Winks_connect( psocket->s, (struct winks_sockaddr* )(&(psocket->addr)), sizeof(struct winks_sockaddr) ) == 0 )
    {
        /* 连接直接成功，恢复原有状态 */
        Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSTRANS );
        //psocket->state = WINKS_HTTP_SOSTRANS;
        if( !ifrecover )
            Winks_httpSocketNotify( ptemp, WINKS_HTTP_EVCONNECT, NULL );
        Winks_HttpSockSend( psocket );
    }
    else if( Winks_getlasterror() && (Winks_getlasterror() != WINKS_SO_EWOULDBLOCK) )
    {
        while( psocket->phttp != NULL )
        {
            ptemp = psocket->phttp;
            psocket->phttp = ptemp->pnext;
            ptemp->pnext = NULL;
            Winks_httpSocketNotify( ptemp, WINKS_HTTP_EVDISCONNECT, NULL );
        }
        Winks_printf( "WINKS http socket renew connect error, freesocket\r\n" );
        Winks_FreeSocket( psocket );
    }
    else
    {
        if( !ifrecover )
            Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSCONNECT );
        //psocket->state = WINKS_HTTP_SOSCONNECT;
        else
            Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSRCCONNECT );
        //psocket->state = WINKS_HTTP_SOSRCCONNECT;
        Winks_StartTimer( psocket->timer );
   }

    return 0;
    
}

#if 0
static int Winks_HttpSockSend( Winks_HttpSocket* psocket )
{
    Winks_HttpCB* phttp = psocket->phttp;
    Winks_HttpBuff* pBuf = NULL;
#ifdef WINKS_DEBUG
    Winks_HttpBuff* pTemp = NULL;
#endif
    int slen = 0;

    if( (phttp == NULL) || (phttp->psend == NULL) )
    {
        Winks_printf( "WINKS http socket send data no cb or no data\r\n" );
        return 0;
    }

    pBuf = phttp->psend;
    while( pBuf != NULL )
    {
        slen = Winks_send( psocket->s, pBuf->pdata, pBuf->offset, 0 );
        
        if( slen < 0 )
            break;
        else if( slen == 0 )
        {
            /* 平稳断连 */
            Winks_HttpSockRenew( psocket, 1 );
            break;
        }
        else
        {
#ifdef WINKS_DEBUG
        #ifdef WINKS_SIMULATOR
            char pMsg[1024];

            sprintf( pMsg, "Winks Http %s request send", phttp->pURL );
            Winks_printHex( (const unsigned char* )(pBuf->pdata), (unsigned short )slen, 
                (const unsigned char* )pMsg );
        #else
            Winks_printf("Winks Http %s request send %d bytes\n", phttp->pURL, slen);
        #endif
#endif
            if( slen < pBuf->offset )
            {
                pBuf->pdata += slen;
                pBuf->offset -= slen;
            }
            else
            {
                Winks_assert( slen == pBuf->offset );
                phttp->psend = phttp->psend->pnext;
#ifdef WINKS_DEBUG
                pBuf->pnext = NULL;
                if( phttp->ptsend == NULL )
                    phttp->ptsend = pBuf;
                else
                {
                    pTemp = phttp->ptsend;
                    while( pTemp->pnext != NULL )
                        pTemp = pTemp->pnext;
                    pTemp->pnext = pBuf;
                }
#else
                Winks_salFree( pBuf );
#endif
                pBuf = phttp->psend;
            }
        }
    }

    if( phttp->psend == NULL )
	{
		Winks_DateTime_s tm;

		Winks_GetSysDateTime( &tm );
		phttp->Sendtick = Winks_mktime( &tm );
	}
    return 0;
}
#endif

static char* Winks_HttpGetSendData( Winks_HttpCB* phttp, char* pbuff, int* buflen )
{
	char* pret = NULL;
	int filelen = 0;
	
	if( phttp->psend )
	{
		pret = (char* )(phttp->psend->pdata);
		*buflen = phttp->psend->offset;

		return pret;
	}

	if( phttp->pfilename )
	{
		if( phttp->pfile == NULL )
			phttp->pfile = Winks_fopenUni( (const unsigned short* )(phttp->pfilename), "rb+" );
		if( (phttp->pfile == NULL) || (Winks_fGetFileSize(phttp->pfile, (unsigned long* )(&filelen)) < 0) )
		{
			*buflen = -1;
			return NULL;
		}
		if( filelen <= phttp->fileoff )
		{
			/* read to file end */
			Winks_fclose( phttp->pfile );
			phttp->pfile = NULL;
			*buflen = 0;
			return NULL;
		}
			
		pret = pbuff;
		Winks_fseek( phttp->pfile, phttp->fileoff, WINKS_SEEK_SET );
		*buflen = Winks_fread( pret, 1, *buflen, phttp->pfile );
		if( *buflen <= 0 )
			return NULL;
		else
			return pret;
	}

	*buflen = 0;

	return NULL;
}

static int Winks_HttpAddSendDataOff( Winks_HttpCB* phttp, int offset )
{
	Winks_HttpBuff* pBuf = NULL;
#ifdef WINKS_DEBUG
    Winks_HttpBuff* pTemp = NULL;
#endif

	while( phttp->psend )
	{
		if( offset <= 0 )
		{
			if( offset < 0 )
				Winks_fail( "WINKS HTTP AddSendDataOff offset compute error\r\n" );
			return 0;
		}
		if( phttp->psend->offset > offset )
		{
			phttp->psend->pdata += offset;
			phttp->psend->offset -= offset;
			return 0;
		}
		pBuf = phttp->psend;
		offset -= pBuf->offset;
		phttp->psend = phttp->psend->pnext;
#ifdef WINKS_DEBUG
		pBuf->pnext = NULL;
		if( phttp->ptsend == NULL )
			phttp->ptsend = pBuf;
		else
		{
			pTemp = phttp->ptsend;
			while( pTemp->pnext != NULL )
				pTemp = pTemp->pnext;
			pTemp->pnext = pBuf;
		}
#else
		Winks_salFree( pBuf );
#endif
	}

	if( offset == 0 )
		return 0;
	
	/* Come to here, should have file data */
	if( phttp->pfilename == NULL )
	{
		Winks_fail( "WINKS HTTP AddSendDataOff should have file\r\n" );
		return -1;
	}

	phttp->fileoff += offset;

	return 0;
}

static int Winks_HttpSockSend( Winks_HttpSocket* psocket )
{
    Winks_HttpCB* phttp = psocket->phttp;
	char pbuff[WINKS_HTTP_BUFLEN];
	char* psend = NULL;
	int buflen = WINKS_HTTP_BUFLEN, slen = 0;
    Winks_Socketmsg_s msg;

    if( (phttp == NULL) || (phttp->psend == NULL) )
    {
        Winks_printf( "WINKS http socket send data no cb or no data\r\n" );
        return 0;
    }

    while( (psend = Winks_HttpGetSendData(phttp, pbuff, &buflen)) != NULL )
    {
        slen = Winks_send( psocket->s, psend, buflen, 0 );
        
        if( slen < 0 )
            break;
        else if( slen == 0 )
        {
            /* 平稳断连 */
            Winks_HttpSockRenew( psocket, 1 );
            break;
        }
        else
        {
#ifdef WINKS_DEBUG
        #ifdef WINKS_SIMULATOR
            char pMsg[1024];

            sprintf( pMsg, "Winks Http %s request send", phttp->pURL );
            Winks_printHex( (const unsigned char* )psend, (unsigned short )slen, 
                (const unsigned char* )pMsg );
        #else
            Winks_printf("Winks Http %s request send %d bytes\n", phttp->pURL, slen);
        #endif
#endif
			Winks_HttpAddSendDataOff( phttp, slen );
            phttp->totalsend += slen;
            msg.wParam = 0;
            msg.lParam = phttp->totalsend;
            Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDATAOUT, &msg );
        }
		buflen = WINKS_HTTP_BUFLEN;
    }

    if( psend == NULL )
	{
		if( buflen < 0 )
		{
			/* 获取数据失败，该http控制块失败处理 */
            msg.wParam = 0;
			msg.lParam = (unsigned long)(WINKS_HTTP_EDATA << 16);
            
            psocket->phttp = psocket->phttp->pnext;
            phttp->pnext = NULL;
            Winks_httpSocketNotify( phttp, WINKS_HTTP_EVDISCONNECT, &msg );
			if( psocket->phttp )
				Winks_HttpSockRenew( psocket, 0 );
			else
			{
				Winks_printf( "WINKS http socket recv failure and no http, freesocket\r\n" );
				Winks_FreeSocket( psocket );
			}
			
			return 0;

		}
		else
		{
			Winks_DateTime_s tm;
			
			Winks_GetSysDateTime( &tm );
			phttp->Sendtick = Winks_mktime( &tm );
		}
	}
    return 0;
}

/**********************************************************************************\
函数原型：static int Winks_httpSocketTrans(Winks_HttpSocket* psocket, Winks_HttpCB* phttp )
函数介绍：
		HTTP数据传输启动函数。
参数说明：
        参数名称	参数类型	        参数介绍	        备注
        psocket	    Winks_HttpSocket*	HTTP数据传输控制块	
        phttp	    Winks_HttpCB*	    HTTP传输控制块	

函数返回值：
        成功返回0，失败返回-1。
实现说明：	
        本函数处理HTTP请求的数据传输事件。
        数据解析模块整理好需要发送的请求数据以后需要首先按照发送目的服务器寻找适合的传输控制块
        或创建新的传输控制块，然后通过本函数的调用要求启动数据传输。
        本函数成功返回并不代表数据传输已经成功，只是代表传输模块接受了数据传输的请求，随后传输
        模块会自行按照HTTP控制块内部信息进行数据传输，直到传输成功或者失败。
        默认情况下传输模块会采用持久连接方式传输数据。
        本函数被调用以后，数据解析状态机就应该准备接收随后的数据传输成功或其他错误发生的事件。
\*********************************************************************************/
static int Winks_httpSocketTrans(Winks_HttpSocket* psocket, Winks_HttpCB* phttp )
{
    Winks_Socketmsg_s msg;

    if( psocket->phttp == NULL )
    {
        /* 放弃流水线功能后，只有第一个http控制块才会引发网络动作，否则会由前面的控制块发起本控制块的网络行为 */
        Winks_printf( "WINKS http %s enter trans first note\r\n", phttp->pURL );
        psocket->phttp = phttp;
        switch( psocket->state )
        {
        case WINKS_HTTP_SOSIDLE:
            Winks_printf( "Winks http socket trans goto connect %d.%d.%d.%d:%x\r\n", psocket->addr.sin_addr.wk_addr & 0x00ff,
                ((psocket->addr.sin_addr.wk_addr) >> 8) & 0x00ff, ((psocket->addr.sin_addr.wk_addr) >> 16) & 0x00ff, 
                ((psocket->addr.sin_addr.wk_addr) >> 24) & 0x00ff, (psocket->addr.sin_port) );
            
            if( Winks_connect( psocket->s, (struct winks_sockaddr* )(&(psocket->addr)), sizeof(struct winks_sockaddr) ) == 0 )
            {
                Winks_printf( "WINKS http %s enter trans first note in idle, connect success\r\n", phttp->pURL );
                Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSTRANS );
                //psocket->state = WINKS_HTTP_SOSTRANS;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVCONNECT, NULL );
            }
            else if( Winks_getlasterror() && (Winks_getlasterror() != WINKS_SO_EWOULDBLOCK) )
            {
                Winks_printf( "WINKS http %s enter trans first note in idle, connect failure\r\n", phttp->pURL );
                msg.wParam = psocket->s;
                msg.lParam = (unsigned long)WINKS_HTTP_FAILURE;
                Winks_httpSocketNotify( phttp, WINKS_HTTP_EVCONNECT, &msg );
                Winks_printf( "WINKS http %s enter trans first note in idle, connect failure, freesocket\r\n", phttp->pURL );
                return Winks_FreeSocket( psocket );

            }
            else
            {
                Winks_printf( "WINKS http %s enter trans first note in idle goto connect state\r\n", phttp->pURL );
                Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSCONNECT );
                Winks_StartTimer( psocket->timer );
                //psocket->state = WINKS_HTTP_SOSCONNECT;
            }
            break;
            
        case WINKS_HTTP_SOSSILENCE:
            Winks_printf( "WINKS http %s enter trans first note in silence, goto send\r\n", phttp->pURL );
            Winks_StopTimer( psocket->timer );
            Winks_HttpSetSocketState( psocket, WINKS_HTTP_SOSTRANS );
            //psocket->state = WINKS_HTTP_SOSTRANS;
            Winks_httpSocketNotify( phttp, WINKS_HTTP_EVCONNECT, NULL );
            Winks_HttpSockSend( psocket );
            
            break;
        case WINKS_HTTP_SOSTRANS:
            /* When we find a redirected http and it is the last one in here, this request will come here. */
            /* Do nothing */
            break;
        default:
            Winks_printf( "WINKS http socket trans in error state %d\r\n", psocket->state );
            return -1;
        }
    }
    else
    {
        Winks_HttpCB* ptemp = psocket->phttp;
        while( ptemp->pnext != NULL )
            ptemp = ptemp->pnext;
        ptemp->pnext = phttp;
        Winks_printf( "WINKS http %s enter trans not first note, add in list\r\n", phttp->pURL );
    }

    return 0;
}

/**********************************************************************************\
函数原型：static int Winks_httpSocketClose(Winks_HttpSocket* psocket, Winks_HttpCB* phttp )
函数介绍：
		HTTP数据传输关闭函数。
参数说明：
        参数名称	参数类型	        参数介绍	        备注
        psocket	    Winks_HttpSocket*	HTTP数据传输控制块	
        phttp	    Winks_HttpCB*	    HTTP传输控制块	

函数返回值：
        成功返回0，失败返回-1。
实现说明：	
        本函数处理HTTP请求的关闭连接。
        通常在用户或者系统要求关闭连接或请求的情况下本函数会被调用，随后传输模块会自行按照HTTP
        控制块内部信息关闭连接，直到连接成功或者失败。
        默认情况下传输模块会采用持久连接连接请求。
        本函数会将该http控制块从连接等待或者数据等待链表上拆除下来，需要注意两点：
        1.拆除成功以后还需将http控制块内部指向socket控制块的位置清零。
        2.即使该控制块被拆除了，如果该控制块已经进入传输序列，即请求已经发出，我们需要特殊处理以免
        影响后续控制块的传输。
        本函数被调用以后，数据解析状态机就应该准备接收随后的连接关闭或其他错误发生的事件。
\*********************************************************************************/
static int Winks_httpSocketClose( Winks_HttpSocket* psocket, Winks_HttpCB* phttp )
{
    Winks_HttpCB* ptemp = psocket->phttp;

    Winks_assert( phttp->pSocket == psocket );

    if( ptemp == NULL )
    {
        Winks_fail( "Winks Http Socket Close in a null table\r\n" )
        return 0;
    }
    if( ptemp->pnext == NULL )
    {
        /* 只有这么一个控制块，释放控制块资源 */
        Winks_printf( "WINKS http socket httpspcketclose, no other http, freesocket\r\n" );
        Winks_FreeSocket( psocket );
    }
    else if( (psocket->state != WINKS_HTTP_SOSTRANS) || (ptemp != phttp) )
    {
        /* 我们采用顺序传输，因此只要不是在传输状态下或者不是第一个，就可以直接拆除 */
        if( ptemp == phttp )
        {
            psocket->phttp = phttp->pnext;
            phttp->pnext = NULL;
        }
        else
        {
            while( ptemp != NULL )
            {
                if( ptemp->pnext == phttp )
                {
                    ptemp->pnext = phttp->pnext;
                    phttp->pnext = NULL;
                    break;
                }
                ptemp = ptemp->pnext;
            }
        }
    }
    else
    {
        psocket->phttp = psocket->phttp->pnext;
        phttp->pnext = NULL;
        if( psocket->phttp )
            Winks_HttpSockRenew( psocket, 0 );
        else
        {
            Winks_printf( "WINKS http socket httpspcketclose, no other http, not presistent connection, freesocket\r\n" );
            Winks_FreeSocket( psocket );
        }
    }

    phttp->pSocket = NULL;

    return 0;
}


/* pTag should begin with '\n' */
static int Winks_HttpGetHeadValue( unsigned char* phead, char* pTag, char** pDest )
{
    char* pStr = NULL;
    char* pEnd = NULL;

    if( (pStr = Winks_StriStr( (char* )phead, pTag )) == NULL )
    {
        *pDest = NULL;
        return -1;
    }
    pStr += strlen(pTag);
    while( *pStr == ' ' || *pStr == ':' )
        pStr++;
    if( (pEnd = Winks_StriStr( pStr, (char* )HttpLine )) == NULL )
    {
        if( (pEnd = Winks_StriStr( pStr, (char* )HttpLineT )) == NULL )
        {
            *pDest = NULL;
            return -1;
        }
    }
    if( *(pEnd - 1) == ' ' )
        pEnd --;

    *pDest = pStr; 
    return pEnd - (*pDest);

}
/********************************************************************
 * 函数功能: 得到源码pSrc经过Base64编码后的最大长度.
 * 参数意义: pSrc 需要编码的源文本
 * 返回数值: 经过Base64编码后的最大长度.
 ********************************************************************/
static unsigned long Winks_HttpBase64EncodeLen(char *pSrc)
{
	unsigned long	EncodeLen = 0;
	unsigned long	Length = 0;
	
	Length = strlen(pSrc);
	EncodeLen = (((Length)/3 + 1 ) * 4) + 1;
	return	EncodeLen;
}

/********************************************************************
 * 函数功能: 将源码编码成为Base64编码.
 * 参数意义: pSrc 需要编码的源文本;
 *			 pDest 存放Base64的编码;
 *			 DestLen pDest的长度.
 * 返回数值: void.
 ********************************************************************/
static int Winks_HttpBase64Encode(char *pSrc, char *pDest, unsigned long DestLen)
{
	unsigned int	Quad = 0;
	unsigned int	Trip = 0;
	int	index, i, Value, Length = strlen(pSrc);
	
    if( DestLen < (((strlen(pSrc))/3 + 1 ) * 4) )
        return -1;

    for (i=0, index=0; i < Length; i+=3, index+=4) 
	{
		Quad = 0;
		Trip = 0;
        Value = (0xFF & (int) pSrc[i]);

        Value <<= 8;
        if ((i+1) < Length) 
		{
            Value |= (0xFF & (int) pSrc[i+1]);
            Trip = 1;
        }
		
        Value <<= 8;
        if ((i+2) < Length) 
		{
            Value |= (0xFF & (int) pSrc[i+2]);
            Quad = 1;
        }
		
        pDest[index+3] = HttpBase64Table[(Quad ? (Value & 0x3F) : 64)];
        Value >>= 6;
        pDest[index+2] = HttpBase64Table[(Trip ? (Value & 0x3F) : 64)];
        Value >>= 6;
        pDest[index+1] = HttpBase64Table[Value & 0x3F];
        Value >>= 6;
        pDest[index+0] = HttpBase64Table[Value & 0x3F];
    }
	
	return 0;
}

/********************************************************************
 * 函数功能:	把用户名和密码编码成Base64编码.
 * 参数意义:	pUser	用户名;
 *				pPass	用户密码;
 *				pBuf	存放编码的数据缓冲区;
 *				BufLen	pBuf数据缓冲区的长度.
 * 返回数值:	成功填写返回填写的长度，失败返回值小于-1。如果数据
 *				区长度不够则将需要的长度填写进数据区长度指针返回并且返回0。
 ********************************************************************/
static int Winks_HttpEncodeUserPass(char *pUser, char *pPass, char *pBuf, int *BufLen)
{
	char Buffer[201];
	char *pBasic = "Basic ", *pSrc = NULL;
	int UserLen = strlen(pUser), PassLen = strlen(pPass),
		Codelen = 0, SrcLen = UserLen + 1 + PassLen + 1,	/* User:Pass\0 */
		BaseLen = strlen(pBasic);

	if (SrcLen > 200)	
		pSrc = (char *)Winks_salAlloc(SrcLen);
	else
		pSrc = Buffer;

	Winks_mem_set(pSrc, 0, SrcLen);
	sprintf(pSrc, "%s:%s", pUser, pPass);

	Codelen = Winks_HttpBase64EncodeLen(pSrc);
	
	if(pBuf == NULL || *BufLen < (int)(Codelen + BaseLen))
	{
		*BufLen = Codelen + BaseLen;
        if (pSrc != Buffer)
            Winks_salFree(pSrc);

		return 0;
	}

    Winks_mem_set( pBuf, 0, *BufLen );
	Winks_mem_cpy(pBuf, pBasic, BaseLen);
	Winks_HttpBase64Encode(pSrc, &pBuf[BaseLen], *BufLen - BaseLen);

	*BufLen = Codelen + BaseLen;
	
	if (pSrc != Buffer)
		Winks_salFree(pSrc);

	return *BufLen;
}

static int Winks_httpCopyString( char* pData, char* pbuf, int* len )
{
    int slen = strlen( pData ) + 1;

    if( *len < slen )
    {
        *len = slen;
        return 0;
    }

    Winks_mem_cpy( pbuf, pData, slen );

    return slen;
}

static int Winks_HttpAcceptGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
#if 0
	    char* pAccept = "text/vnd.wap.wml, text/vnd.wap.wmlscript, image/vnd.wap.wbmp, application/vnd.wap.wmlc, " \
					"image/jpg, image/bmp, image/png, audio/arm, audio/imelody, audio/midi, " \
					"image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*";
#endif
	char* pAccept = "image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*";

    return Winks_httpCopyString( pAccept, pbuf, len );    
}

static int Winks_HttpHostGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    return Winks_httpCopyString( phttp->phost, pbuf, len );    
}

static int Winks_HttpContentlengthGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    int slen = 0;

#define WINKS_HTTP_MAXNUMLEN    16

    if( *len < WINKS_HTTP_MAXNUMLEN )
    {
        *len = WINKS_HTTP_MAXNUMLEN;
        return 0;
    }

	if( phttp->pfilename )
	{
		if( Winks_fgetsize( phttp->pfilename, 0, (unsigned int* )(&slen) ) < 0 )
			return -1;
        sprintf( pbuf, "%d", slen );
		slen = strlen( pbuf ) + 1;
		return slen;
	}
    else if( phttp->pdata->pnext )
    {
        sprintf( pbuf, "%d", Winks_HttpBufLen( phttp->pdata->pnext ) );
        slen = strlen( pbuf ) + 1;
        return slen;
    }
    else
        return -1;
}

static int Winks_HttpContenttypeGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    if( phttp->pContenttype == NULL )
        return -1;

    return Winks_httpCopyString( phttp->pContenttype, pbuf, len );
}

static int Winks_HttpAuthoriGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    if( phttp->wwwau.pusername )
    {
        return Winks_HttpEncodeUserPass( phttp->wwwau.pusername, phttp->wwwau.ppassword, pbuf, len );
    }
    else
        return -1;
}

static int Winks_HttpProxyAuthoriGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    if( phttp->proxy.pusername )
        return Winks_HttpEncodeUserPass( phttp->proxy.pusername, phttp->proxy.ppassword, pbuf, len );
    else
        return -1;
}

static int Winks_HttpUserAgentGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( pHttpGlobalCB->Setting.pUseragent[0] == 0 )
        return -1;

    return Winks_httpCopyString( pHttpGlobalCB->Setting.pUseragent, pbuf, len );
}

static int Winks_HttpUAProfileGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( pHttpGlobalCB->Setting.pUAprofile[0] == 0 )
        return -1;
    return Winks_httpCopyString( pHttpGlobalCB->Setting.pUAprofile, pbuf, len );
}

static int Winks_HttpIfModifiedGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    char* pModify = NULL;
    int clen = 0, mlen = 0;

    if( (winks_getlenth_cache( phttp->pURL, strlen(phttp->pURL), phttp->backmethod, (unsigned int* )&clen, &pModify, &mlen ) >= 0)
        && (clen > 0)
        && (mlen > 0) )
    {
        if( *len < mlen + 1 )
        {
            *len = mlen + 1;
            return 0;
        }
        Winks_mem_cpy( pbuf, pModify, mlen );
        pbuf[mlen] = 0;
        return mlen + 1;
    }

    return -1;
}

static int Winks_HttpCookieGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    int ret = 0, nlen = 0;

    winks_getlenth_cookie( phttp->pURL, &nlen, phttp->port );
    if( nlen == 0 )
        return -1;

    if( nlen + 1 > *len )
    {
        *len = nlen + 1;
        return 0;
    }
    if( (ret = winks_get_cookie( phttp->pURL, pbuf, len, phttp->port )) == 0 )
        return -1;
    pbuf[*len] = 0;
    *len = *len + 1;

    return *len;
}
#if 0
static int Winks_HttpAcCharsetGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
	char* pCharset = "UTF-8";

    return Winks_httpCopyString( pCharset, pbuf, len );    
}

static int Winks_HttpAcLanguageGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
	char* pLanguage = "en";

    return Winks_httpCopyString( pLanguage, pbuf, len );    
}

static int Winks_HttpAcEncodingGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
	char* pEncoding = "gzip";

    return Winks_httpCopyString( pEncoding, pbuf, len );    
}

static int Winks_HttpConnection( Winks_HttpCB* phttp, char* pbuf, int* len )
{
	char* pConnection = "keep-alive";

    return Winks_httpCopyString( pConnection, pbuf, len );    
}

static int Winks_HttpPragmaGen( Winks_HttpCB* phttp, char* pbuf, int* len )
{
	char* pPragma = "no-cache";

    return Winks_httpCopyString( pPragma, pbuf, len );
}
#endif
static int Winks_IfUserDefineHead( Winks_HttpCB* phttp, char* pTag )
{
    int i = 0;
    
    for( i = 0; i < WINKS_HTTP_MAXTAG; i ++ )
    {
        if( phttp->tag[i].tag )
        {
            /* skip \n char */
            if( stricmp(phttp->tag[i].tag, pTag) == 0 )
                return 1;
        }
    }

    return 0;
}

static int Winks_httpRedirect( Winks_HttpCB* phttp )
{
    char* pLocation = NULL;
    int len = 0;
    Winks_Socketmsg_s msg;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( phttp->renum ++ >= WINKS_HTTP_REDMAXNUM )
    {
        Winks_printf( "WINKS http redirect head time to large\r\n" );
        msg.wParam = (WINKS_HTTP_CLOSE << 24) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STERROR;

        return -1;
    }

    len = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTLOCATION, &pLocation );

    if( pLocation == NULL )
    {
        /* error occur */
        Winks_printf( "WINKS http redirect head location not present\r\n" );
        msg.wParam = (WINKS_HTTP_CLOSE << 24) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STERROR;

        return -1;
    }

    phttp->pURL = Winks_HttpBufAppend( &(phttp->pdata), pLocation, len, 1, 1, 0 );
    Winks_HttpBufAppendChar( &(phttp->pdata), (char )0, 1 );
    if( Winks_HttpParseURL( phttp ) < 0 )
    {
        /* error occur */
        Winks_printf( "WINKS http redirect head location parse error\r\n" );
        msg.wParam = (WINKS_HTTP_CLOSE << 24) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STERROR;

        return -1;
    }

    Winks_HttpBufFree( &(phttp->psend) );
    Winks_HttpBufFree( &(phttp->precv) );
#ifdef WINKS_DEBUG
    Winks_HttpBufFree( &(phttp->ptsend) );
#endif
    phttp->GHHandle = 0;
    phttp->Encoding = 0;
    phttp->info.contentlength = 0;
    phttp->ifredirect = 1;

    phttp->state = WINKS_HTTP_STIDLE;
    if( phttp->method == WINKS_HTTP_MEGET || phttp->method == WINKS_HTTP_MEHEAD )
    {
        /* Get or Head methos, we redirect automatically */
        
        Winks_mem_set( &(phttp->info), 0, sizeof(Winks_HttpInfo) );
        
        if( Winks_httpfsm(phttp, WINKS_HTTP_EVSTART) != 0 )
        {
            Winks_printf( "WINKS http redirect head rerun fsm error\r\n" );
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            
            return -1;
        }
    }
    else
    {
        msg.wParam = (WINKS_HTTP_REDIRECT << 24) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );

        /* Wait user's confirm */
    }

    return 0;

}

static int Winks_httpParse200( Winks_HttpCB* phttp )
{
    char* pdata = NULL;
    int datalen = 0, i = 0, ch = 0;
    /* save cache */
    Winks_CacheTimeItem_s Item;
    
    if( phttp->precv->pnext == NULL )
    {
        Winks_printf( "winks http %s 200 OK but no data\r\n", phttp->pURL );
        return 1;
    }

    Winks_mem_set( &Item, 0, sizeof(Winks_CacheTimeItem_s) );
//////////////////////////////////////////////////
    if( (datalen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTCONTTYPE, &pdata )) > 0 )
    {
        Item.content_type = pdata;
        Item.content_type_lenth = datalen;
    }

    if( (datalen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTCACHECON, &pdata )) > 0 )
    {
        Item.cache_control = pdata;
        Item.cache_control_lenth = datalen;
    }

    if( (datalen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTDATE, &pdata )) > 0 )
    {
        Item.date_value = pdata;
        Item.date_value_lenth = datalen;
    }

    if( (datalen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTEXPIRES, &pdata )) > 0 )
    {
        Item.expire_time = pdata;
        Item.expire_time_lenth = datalen;
    }

    if( (datalen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTLASTMOD, &pdata )) > 0 )
    {
        Item.last_modified_time = pdata;
        Item.last_modified_time_lenth = datalen;
    }
    else
    {
        /* @@ we suppose we need this tag */
        return 1;
    }

    if( (datalen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTAGE, &pdata )) > 0 )
    {
        while( i < datalen )
        {
            if( pdata[i] < '9' && pdata[i] > '0' )
                Item.age_value = Item.age_value * 10 + pdata[i] - 0x30;
            else
                break;
            i++;
        }
    }

    Item.request_time = phttp->Sendtick;
    Item.respose_time = phttp->Recvtick;

    ch = winks_create_cache( phttp->pURL, strlen(phttp->pURL), phttp->backmethod, 
        Winks_HttpBufLen(phttp->precv->pnext),
        &Item );
    if( ch )
    {
        Winks_HttpBuff* pBuf = phttp->precv->pnext;
        while( pBuf )
        {
            winks_put_cache_append( ch, pBuf->offset , (char* )(pBuf->pdata) );
            pBuf = pBuf->pnext;
        }
    }

    return 1;

}

static int Winks_httpParse301( Winks_HttpCB* phttp )
{
    /* remove this http from socket handle first */
    return Winks_httpRedirect( phttp );
}

static int Winks_httpParse302( Winks_HttpCB* phttp )
{
    /* remove this http from socket handle first */
    return Winks_httpRedirect( phttp );
}

static int Winks_httpParse304( Winks_HttpCB* phttp )
{
    Winks_Socketmsg_s msg;
    char precv[WINKS_HTTP_RCVNUM];
#define WINKS_HTTP_HDLEN    128
    char phead[WINKS_HTTP_HDLEN];
    Winks_HttpBuff* pTemp = NULL;
    int clen = 0, mlen = 0, off = 0, datalen = WINKS_HTTP_RCVNUM - 4, rlen = 0, ch = 0, headlen = 0;
    char* pModify = NULL;
    char* pdata = NULL;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    /* remove this http from socket handle first */
    Winks_assert( phttp->method == WINKS_HTTP_MEHEAD );

    if( phttp->precv->pnext )
    {
        Winks_printf( "Winks http 304 head has data\r\n" );
        Winks_HttpBufFree( &(phttp->precv->pnext) );
    }

    if( (ch = winks_find_cache( phttp->pURL, strlen(phttp->pURL), phttp->backmethod )) != 0 )
    {
        winks_getlenth_cache( phttp->pURL, strlen(phttp->pURL), phttp->backmethod, (unsigned int* )&clen, &pModify, &mlen );
        /* Cache 里面的全是明文 */
        phttp->Encoding = WINKS_HTTP_PLANE;
        phttp->info.contentlength = clen;
        phttp->RemainRcv = clen;
        
        if( phttp->precv->pnext )
        {
            pTemp = phttp->precv->pnext;
            phttp->precv->pnext = NULL;
        }
        Winks_HttpBufDelTail( phttp->precv );
        if( Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTCONTLEN, &pdata ) < 0 )
        {
            sprintf( phead, "%s: %d\r\n", WINKS_HTTP_HTCONTLEN + 1, clen );
            Winks_HttpBufAppend( &(phttp->precv), phead, strlen(phead), 1, 0, 0 );
        }
        if( Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTCONTTYPE, &pdata ) < 0 )
        {
            sprintf( phead, "%s: ", WINKS_HTTP_HTCONTTYPE + 1 );
            headlen = WINKS_HTTP_HDLEN - strlen(phead);
            if( winks_get_cache_centent_type( ch, phead + strlen(phead), &headlen ) >= 0 && headlen > 0 )
                Winks_HttpBufAppend( &(phttp->precv), phead, strlen(phead), 1, 0, 0 );
        }
        Winks_HttpBufAppend( &(phttp->precv), "\r\n", 2, 1, 0, 0 );
        if( pTemp )
            phttp->precv->pnext = pTemp;

        while( off < clen )
        {
            if( (rlen = winks_get_cache_append( ch, off, precv, &datalen )) == 0 )
            {
                Winks_printf( "Winks http get cache error" );
                break;
            }
            else
            {
                precv[rlen] = 0;
                if( Winks_HttpDatain( phttp, precv, rlen, NULL ) < 0 )
                    return -1;
                off += rlen;
            }
        }
    }
    else
    {
        /* Cache has been deleted, we change to normal request */
        phttp->method = phttp->backmethod;
        Winks_HttpBufFree( &(phttp->psend) );
        Winks_HttpBufFree( &(phttp->precv) );
#ifdef WINKS_DEBUG
        Winks_HttpBufFree( &(phttp->ptsend) );
#endif
        phttp->GHHandle = 0;
        phttp->Encoding = 0;
        phttp->ifredirect = 1;
        Winks_mem_set( &(phttp->info), 0, sizeof(Winks_HttpInfo) );
        
        phttp->state = WINKS_HTTP_STIDLE;
        if( Winks_httpfsm(phttp, WINKS_HTTP_EVSTART) != 0 )
        {
            Winks_printf( "WINKS http redirect head rerun fsm error\r\n" );
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            
            return -1;
        }

		return 0;
    }

    return 1;
}

static int Winks_httpParse401( Winks_HttpCB* phttp )
{
    return 1;
}

static int Winks_httpParse407( Winks_HttpCB* phttp )
{
    return 1;
}

/**********************************************************************************\
函数原型：static int Winks_httpGetDatalen( Winks_HttpCB* phttp )
函数介绍：
		HTTP数据长度读取函数。
参数说明：
        参数名称	参数类型	    参数介绍	备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数用来读取获得的数据长度。
\*********************************************************************************/
static int Winks_httpGetDatalen( Winks_HttpCB* phttp )
{
    
    if( phttp->precv == NULL )
        return 0;
    
    return Winks_HttpBufLen( phttp->precv->pnext );

}
/**********************************************************************************\
函数原型：static int Winks_HttpPackReq( Winks_HttpCB* phttp )
函数介绍：
		HTTP请求打包函数。
参数说明：
        参数名称	参数类型	    参数介绍	备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数用来打包请求数据。
\*********************************************************************************/
static int Winks_HttpPackReq( Winks_HttpCB* phttp )
{
    int i = 0, clen = 0, mlen = 0;
    //unsigned short method = 0;
    char* LastModify = NULL;
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    //method = phttp->method;

    Winks_assert( HttpMethod[phttp->method][0] != 0 );

    if( winks_getlenth_cache(phttp->pURL, strlen(phttp->pURL), phttp->method, (unsigned int* )&clen, &LastModify, &mlen) >= 0
        && clen > 0 )
    {
        phttp->method = WINKS_HTTP_MEHEAD;
    }

    Winks_HttpBufAppend( &(phttp->psend), (char* )HttpMethod[phttp->method], strlen(HttpMethod[phttp->method]), 1, 0, 0 );

    if( phttp->proxy.phostname )
    {
        Winks_HttpBufAppend( &(phttp->psend), phttp->pURL, strlen(phttp->pURL), 1, 0, 0 );
        Winks_HttpBufAppendChar( &(phttp->psend), ' ', 1 );
    }
    else
    {
        Winks_HttpBufAppend( &(phttp->psend), phttp->ppath, strlen(phttp->ppath), 1, 0, 0 );
        Winks_HttpBufAppendChar( &(phttp->psend), ' ', 1 );
    }
    Winks_HttpBufAppend( &(phttp->psend), (char* )HttpVersion, strlen(HttpVersion), 1, 0, 0 );
    Winks_HttpBufAppendChar( &(phttp->psend), '\r', 1 );
    Winks_HttpBufAppendChar( &(phttp->psend), '\n', 1 );

    while( (pHttpGlobalCB->HttpSendHead[i].HeaderTag[0] != 0) && pHttpGlobalCB->HttpSendHead[i].HttpHeadDeal )
    {
        if( !(Winks_IfUserDefineHead(phttp, (char* )(pHttpGlobalCB->HttpSendHead[i].HeaderTag))) )
        {
            char pvalue[WINKS_HTTP_HEADVALLEN];
            char* plvalue = NULL;
            int valen = WINKS_HTTP_HEADVALLEN;
            int ret = 0;
            
            plvalue = pvalue;
            if( (ret = pHttpGlobalCB->HttpSendHead[i].HttpHeadDeal( phttp, plvalue, &valen )) == 0 )
            {
                plvalue = Winks_salAlloc( valen );
                ret = pHttpGlobalCB->HttpSendHead[i].HttpHeadDeal( phttp, plvalue, &valen );
            }

            if( ret > 0 )
            {
                Winks_HttpBufAppend( &(phttp->psend), (char* )(pHttpGlobalCB->HttpSendHead[i].HeaderTag), strlen(pHttpGlobalCB->HttpSendHead[i].HeaderTag), 1, 0, 0 );
                Winks_HttpBufAppendChar( &(phttp->psend), ':', 1 );
                Winks_HttpBufAppendChar( &(phttp->psend), ' ', 1 );
                Winks_HttpBufAppend( &(phttp->psend), plvalue, strlen(plvalue), 1, 0, 0 );
                Winks_HttpBufAppendChar( &(phttp->psend), '\r', 1 );
                Winks_HttpBufAppendChar( &(phttp->psend), '\n', 1 );
            }
            if( plvalue != pvalue )
                Winks_salFree( plvalue );
        }

        i++;
    }

    for( i = 0; i < WINKS_HTTP_MAXTAG; i ++ )
    {
        if( phttp->tag[i].tag )
        {
            Winks_HttpBufAppend( &(phttp->psend), phttp->tag[i].tag, strlen(phttp->tag[i].tag), 1, 0, 0 );
            Winks_HttpBufAppendChar( &(phttp->psend), ':', 1 );
            Winks_HttpBufAppendChar( &(phttp->psend), ' ', 1 );
            Winks_HttpBufAppend( &(phttp->psend), phttp->tag[i].pvalue, strlen(phttp->tag[i].pvalue), 1, 0, 0 );
            Winks_HttpBufAppendChar( &(phttp->psend), '\r', 1 );
            Winks_HttpBufAppendChar( &(phttp->psend), '\n', 1 );
        }
    }

    Winks_HttpBufAppend( &(phttp->psend), (char* )HttpLine, strlen(HttpLine), 1, 0, 0 );

    if( phttp->pdata->pnext )
    {
        /* move post data to send buffer */
        phttp->psend->pnext = phttp->pdata->pnext;
        phttp->pdata->pnext = NULL;
    }
    /* Head end tag */

    return 0;
}


static int Winks_httpGetDataEncoding( unsigned char* phead, int* coding, int* length )
{
    int len = 0;
    char* pTemp = NULL;

    len = Winks_HttpGetHeadValue( phead, WINKS_HTTP_HTCONTLEN, &pTemp );
    if( pTemp == NULL )
    {
        len = Winks_HttpGetHeadValue( phead, WINKS_HTTP_HTTRANSENCODING, &pTemp );
        if( pTemp == NULL )
            *coding = WINKS_HTTP_UNKNOW;
        else
            *coding = WINKS_HTTP_CHUNK;
    }
    else
    {
        *coding = WINKS_HTTP_PLANE;
        while( len )
        {
            if( *pTemp >= '0' && *pTemp <= '9' )
                *length = *length * 10 + (*pTemp - 0x30);
            len --;
            pTemp ++;
        }
    }

    return 0;
}

/*
static int Winks_HttpParseContentLength( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    phttp->Encoding = WINKS_HTTP_PLANE;
    
    while( *len )
    {
        if( *pbuf < '9' && *pbuf > '0' )
        {
            phttp->info.contentlength = phttp->info.contentlength * 10 + *pbuf - 0x30;
            pbuf ++;
            *len --;
        }
        else
            break;
    }

    return 0;
}

static int Winks_HttpParseTransEncoding( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    phttp->Encoding = WINKS_HTTP_CHUNK;
    phttp->info.contentlength = 0;

    return 0;
}
*/

static int Winks_HttpParseConnection( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    if( Winks_StrhStr(pbuf, WINKS_HTTP_HTCONNECTCLOSE) != NULL )
        phttp->info.connection = WINKS_HTTP_CONCLOSE;

    return 0;
}

static int Winks_HttpParseSetCookie( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    char* pdata = NULL;
    int alen = 0, age = 0, i = 0;

    Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTSETCOOKIE2, &pdata );
    
    if( pdata )
        return -1;
    else
    {
        if( (alen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTAGE, &pdata )) > 0 )
        {
            while( i < alen )
            {
                if( pdata[i] < '9' && pdata[i] > '0' )
                    age = age * 10 + pdata[i] - 0x30;
                else
                    break;
                i++;
            }
        }

        Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTDATE, &pdata );
        winks_put_cookie( phttp->pURL, pbuf, *len, pdata, age, phttp->Sendtick, 
            phttp->Recvtick, phttp->port );
    }

    return 0;
}

static int Winks_HttpParseSetCookie2( Winks_HttpCB* phttp, char* pbuf, int* len )
{
    char* pdata = NULL;
    int alen = 0, age = 0, i = 0;

    if( (alen = Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTAGE, &pdata )) > 0 )
    {
        while( i < alen )
        {
            if( pdata[i] < '9' && pdata[i] > '0' )
                age = age * 10 + pdata[i] - 0x30;
            else
                break;
            i++;
        }
    }
    Winks_HttpGetHeadValue( phttp->precv->pdata, WINKS_HTTP_HTDATE, &pdata );
    winks_put_cookie( phttp->pURL, pbuf, *len, pdata, age, phttp->Sendtick, 
            phttp->Recvtick, phttp->port );
    
    return 0;
}

static int Winks_HttpIfHeadEnd( Winks_HttpCB* phttp )
{
    //Winks_assert( phttp->precv != NULL );
	if( phttp->precv == NULL )
		return 0;

    if( (phttp->precv->pnext == NULL )
        && Winks_StriStr((char* )phttp->precv->pdata, (char* )"\r\n\r\n") == NULL
        && Winks_StriStr((char* )phttp->precv->pdata, (char* )"\n\n") == NULL )
    {
        /* head data uncompleted, return failure */
        Winks_printf( "WINKS http head data uncompleted\r\n" );
        return 0;
    }

    return 1;
}
/**********************************************************************************\
函数原型：static int Winks_HttpParseHeader( Winks_HttpCB* phttp )
函数介绍：
		HTTP头部解析函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        ifend       int             是否传输结束

函数返回值：
        成功返回1，失败返回-1，返回零代表需要转向。
实现说明：
        本函数用来解析接收到的HTTP头部数据，如果头部数据未完全到达，本函数将返回失败，一旦函数返回成功
        则代表头部数据完全接收并解析成功。如果参数ifend为真，则代表连接结束，本函数需要强制解析头部数据
        返回解析结果。
\*********************************************************************************/
static int Winks_HttpParseHeader( Winks_HttpCB* phttp, int ifend )
{
    unsigned char* pStatus = NULL;
    unsigned char* pStatusE = NULL;
    unsigned char* pTag = NULL;
    unsigned char* pTemp = NULL;
    unsigned char* pTemp1 = NULL;
    int slstate = 0, i = 0, dlen = 0;
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( !Winks_HttpIfHeadEnd(phttp) && !ifend )
    {
        /* head data uncompleted, return failure */
        Winks_printf( "WINKS http head parse data uncompleted\r\n" );
        return -1;
    }

	if( phttp->precv == NULL )
	{
        Winks_printf( "WINKS http head parse no data at all\r\n" );
        return -1;
	}
	pStatus = phttp->precv->pdata;
	
    if( (pTag = (unsigned char* )Winks_StriStr( (char* )pStatus, (char* )HttpLine )) == NULL )
    {
        pTag = (unsigned char* )Winks_StriStr( (char* )pStatus, (char* )HttpLineT );
        pStatusE = pTag + strlen(HttpLine);
    }
    if( pTag )
        pStatusE = pTag;
    else
    {
        Winks_printf( "WINKS http head parse status line uncompleted\r\n" );
        return -1;
    }
    /* smile fsm to parse status line */
#define WINKS_HTTP_SUIDLE       0
#define WINKS_HTTP_SUMAINVER    1
#define WINKS_HTTP_SUSLAVEVER   2
#define WINKS_HTTP_SUSTATUSCODE 3
#define WINKS_HTTP_SUREASON     4

    pTemp = pStatus;
    while( pTemp < pStatusE )
    {
        switch( slstate )
        {
        case WINKS_HTTP_SUIDLE:
            if( *pTemp == '/' )
                slstate = WINKS_HTTP_SUMAINVER;
            break;

        case WINKS_HTTP_SUMAINVER:
            if( *pTemp == '.' )
                slstate = WINKS_HTTP_SUSLAVEVER;
            else
                phttp->info.mainver = (unsigned short)(phttp->info.mainver * 10 + (*pTemp - 0x30));
            break;

        case WINKS_HTTP_SUSLAVEVER:
            if( *pTemp == ' ' )
                slstate = WINKS_HTTP_SUSTATUSCODE;
            else
                phttp->info.slavever = (unsigned short)(phttp->info.slavever * 10 + (*pTemp - 0x30));
            break;

        case WINKS_HTTP_SUSTATUSCODE:
            if( *pTemp == ' ' )
                slstate = WINKS_HTTP_SUREASON;
            else
                phttp->info.statuscode = phttp->info.statuscode * 10 + (*pTemp - 0x30);
            break;

        default:
            Winks_printf( "WINKS http head parse status line get unknow state\r\n" );
            break;

        }

        if( slstate == WINKS_HTTP_SUREASON )
            break;

        pTemp ++;
    }

    /* set default connection first */
    phttp->info.connection = WINKS_HTTP_CONDEFAULT;
    pTemp = pTag;
    while( pTemp && *pTemp != 0 )
    {
        pTemp1 = (unsigned char* )Winks_StriStr( (char* )pTemp, (char* )HttpLineT );
        if( pTemp1 == NULL )
        {
            Winks_printf( "WINKS http head parse head data not ended by 0x0a\r\n" );
            break;
        }
        while( pHttpGlobalCB->HttpRecvHead[i].HeaderTag[0] != 0 )
        {
            if( pHttpGlobalCB->HttpRecvHead[i].HttpHeadDeal )
            {
                if( Winks_StrhStr( (char* )pTemp, (char* )(pHttpGlobalCB->HttpRecvHead[i].HeaderTag) ) != NULL )
                {
                    pTemp += strlen(pHttpGlobalCB->HttpRecvHead[i].HeaderTag);
                    while( *pTemp == ':' || *pTemp == ' ' )
                        pTemp ++;
                    if( *(pTemp1 - 1) == '\r' )
                        dlen = pTemp1 - pTemp - 1;
                    else
                        dlen = pTemp1 - pTemp;
                    pHttpGlobalCB->HttpRecvHead[i].HttpHeadDeal( phttp, (char* )pTemp, &dlen );
                }
            }
            i++;
        }
        i = 0;
        pTemp = pTemp1 + 1;
    }

    return 0;
}

static int Winks_HttpDealHeader( Winks_HttpCB* phttp )
{
    int i = 0;
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( (phttp->info.mainver * 10 + phttp->info.slavever < 11) || (phttp->info.connection == WINKS_HTTP_CONCLOSE) )
    {
        if( phttp->pSocket )
            phttp->pSocket->cway = WINKS_HTTP_CONCLOSE;
    }

    while( pHttpGlobalCB->HttpStatus[i].pStr[0] != 0 )
    {
        if( pHttpGlobalCB->HttpStatus[i].code == phttp->info.statuscode )
        {
            if( pHttpGlobalCB->HttpStatus[i].parse != NULL )
                return pHttpGlobalCB->HttpStatus[i].parse( phttp );
            else
                return 1;
        }

        i++;
    }

    Winks_printf( "WINKS http head parse get unknow status code(%d.%d, %d, %d, %d)\r\n", 
                    phttp->info.mainver, phttp->info.slavever, phttp->info.statuscode,
                    phttp->info.contentlength, phttp->info.connection);
#ifdef WINKS_DEBUG
    i = 0;
    while( pHttpGlobalCB->HttpStatus[i].pStr[0] != 0 )
    {
        Winks_printf( "HttpStatus: code(%d) parse(%08x) pStr(%s)\r\n", 
                        pHttpGlobalCB->HttpStatus[i].code, 
                        pHttpGlobalCB->HttpStatus[i].parse,
                        pHttpGlobalCB->HttpStatus[i].pStr );

        i++;
    }
    Winks_printHex( (const unsigned char* )(phttp->precv->pdata), (unsigned short )(phttp->precv->offset), 
        (const unsigned char* )"DBG RECV HTTP MSG:" );
#endif
    //unlikely
    return -1;
}
/**********************************************************************************\
函数原型：int Winks_httpfsm(Winks_HttpCB* phttp, int event )
函数介绍：
		HTTP状态机启动函数。
参数说明：
        参数名称	参数类型	    参数介绍	备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数用来启动状态机处理事件，所有相关信息会保留在控制块内部，状态机通过传入的
        控制块数据和事件码进行相应的状态处理，并进行相关动作。
\*********************************************************************************/
static int Winks_httpfsm(Winks_HttpCB* phttp, int event )
{
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( phttp == NULL || pHttpGlobalCB->HttpFsm[phttp->state] == NULL )
    {
        Winks_fail( "Winks http fsm hd error\r\n" );
        return -1;
    }
    return pHttpGlobalCB->HttpFsm[phttp->state]( phttp, event, NULL );
}

/**********************************************************************************\
函数原型：int Winks_httpSocketNotify(Winks_HttpCB* phttp, int event )
函数介绍：
		HTTP SOCKET事件通知函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数用来接收SOCKET数据时间通知，函数通过检查事件类型和控制块状态将该事件传入
        状态机进行处理。
\*********************************************************************************/
static int Winks_httpSocketNotify(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( phttp == NULL || pHttpGlobalCB->HttpFsm[phttp->state] == NULL )
    {
        Winks_fail( "Winks http fsm hd error\r\n" );
        return -1;
    }
    return pHttpGlobalCB->HttpFsm[phttp->state]( phttp, event, pmsg );
}

/**********************************************************************************\
函数原型：static int Winks_httpfsmIdle(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机空闲状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理空闲状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmIdle(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    Winks_Socketmsg_s msg;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( event != WINKS_HTTP_EVSTART )
    {
        Winks_printf( "WINKS http fsm idle get error event\r\n" );
        return -1;
    }
    if( phttp->phost == NULL )
    {
        Winks_printf( "WINKS http fsm idle use a free block\r\n" );
        return -1;
    }

    if( phttp->proxy.phostname )
    {
        Winks_printf( "WINKS http %s fsm idle gethost %s\r\n", phttp->pURL, phttp->proxy.phostname );
        phttp->GHHandle = Winks_AsyncGetHostByName( phttp->proxy.phostname, phttp->pGH, WINKS_SO_GHBFLEN, pHttpGlobalCB->channel, WINKS_HTTP_GHMSG );
    }
    else
    {
        Winks_printf( "WINKS http %s fsm idle gethost %s\r\n", phttp->pURL, phttp->phost );
        phttp->GHHandle = Winks_AsyncGetHostByName( phttp->phost, phttp->pGH, WINKS_SO_GHBFLEN, pHttpGlobalCB->channel, WINKS_HTTP_GHMSG );
    }
    phttp->state = WINKS_HTTP_STGETHOST;

    if( phttp->GHHandle == 0 )
    {
        msg.wParam = (WINKS_HTTP_GH << 24) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        if( !(phttp->ifredirect) )
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STCONNECT;
        /* return success sync, get result */
        phttp->addr.sin_family = AF_INET;
        phttp->addr.sin_port = Winks_htons( phttp->port );
        phttp->addr.sin_addr.wk_addr = *((unsigned int*)(((Winks_hostent*)(phttp->pGH))->h_addr_list[0]));
        phttp->pSocket = Winks_FindSocket( &(phttp->addr) );
        Winks_assert( phttp->pSocket != NULL );
        if( Winks_HttpPackReq(phttp) < 0 )
        {
            /* packet request data failure */
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | (WINKS_HTTP_EDATA << 16) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = WINKS_HTTP_EDATA;
            return 0;
        }
        Winks_printf( "WINKS http %s fsm idle call socket trans\r\n", phttp->pURL );
        Winks_httpSocketTrans( phttp->pSocket, phttp );
    }

    return 0;
}

/**********************************************************************************\
函数原型：static int Winks_httpfsmGethost(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机域名查询状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理域名查询状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmGethost(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    Winks_Socketmsg_s msg;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    switch( event )
    {
    case WINKS_HTTP_EVGETHOST:
        Winks_assert( pmsg->wParam == (unsigned long)(phttp->GHHandle) );
		if( pmsg->lParam )
		{
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EDATA << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
			Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = pmsg->lParam >> 16;
            break;
		}
		else
		{
			msg.wParam = (WINKS_HTTP_GH << 24) | (phttp - pHttpGlobalCB->HttpHD);
			msg.lParam = 0;
			if( !(phttp->ifredirect) )
				Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
		}
        phttp->addr.sin_family = AF_INET;
        phttp->addr.sin_port = Winks_htons( phttp->port );
        phttp->addr.sin_addr.wk_addr = *((unsigned long*)(((Winks_hostent*)(phttp->pGH))->h_addr_list[0]));
        phttp->pSocket = Winks_FindSocket( &(phttp->addr) );    //Must success
        Winks_assert( phttp->pSocket != NULL );

        if( Winks_HttpPackReq(phttp) < 0 )
        {
            /* packet request data failure */
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EDATA << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = WINKS_HTTP_EDATA;
            break;
        }
        
        phttp->state = WINKS_HTTP_STCONNECT;
        Winks_printf( "WINKS http %s fsm gethost call socket trans\r\n", phttp->pURL );
        Winks_httpSocketTrans( phttp->pSocket, phttp );

        break;

    case WINKS_HTTP_EVCLOSE:
        Winks_CancelGetHostByName( phttp->GHHandle );
        phttp->GHHandle = 0;
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EUSER;

        break;
    default:
        Winks_printf( "WINKS http fsm gethost get a unknow event %d\r\n", event );
        return -1;
    }

    return 0;
}

/**********************************************************************************\
函数原型：static int Winks_httpfsmConnect(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机连接状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理连接状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmConnect(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    Winks_Socketmsg_s msg;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    switch( event )
    {
    case WINKS_HTTP_EVCONNECT:
        if( pmsg )
        {
            /*Winks_printf("******pmsg=[%d]********",pmsg);*/
			if( pmsg->lParam & 0x00ff0000 )
				msg.wParam = (WINKS_HTTP_CLOSE << 24) | (pmsg->lParam & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
			else
                msg.wParam = (WINKS_HTTP_CONNECT << 24) | (pmsg->lParam & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
        }
        else
            msg.wParam = (WINKS_HTTP_CONNECT << 24) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        if( (pmsg && (pmsg->lParam >> 16)) || (!(phttp->ifredirect)) )
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        if( pmsg && (pmsg->lParam >> 16) )
        {
            /* Connect failure */
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = pmsg->lParam >> 16;
            break;
        }
        phttp->state = WINKS_HTTP_STWAITHEADER;
        break;

    case WINKS_HTTP_EVDISCONNECT:
        //msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
		if( pmsg && (pmsg->lParam >> 16) )
			msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((pmsg->lParam) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
		else
			msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);

        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EBROKE;
        break;

    case WINKS_HTTP_EVCLOSE:
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EUSER;
        
        return Winks_httpSocketClose( phttp->pSocket, phttp );
    default:
        Winks_printf( "WINKS http fsm connect get a unknow event %d\r\n", event );
        return -1;
    }

    return 0;
}

/**********************************************************************************\
函数原型：static int Winks_httpfsmWaitheader(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机头部数据等待状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理头部数据等待状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmWaitheader(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    Winks_Socketmsg_s msg;
    int ret = 0;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    switch( event )
    {
    case WINKS_HTTP_EVDATAOUT:
        if( pmsg )
        {
            msg.wParam = (WINKS_HTTP_DATAOUT << 24) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = pmsg->lParam;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            Winks_printf( "Winks http send data %d\r\n", pmsg->lParam );
        }
        break;
        
    case WINKS_HTTP_EVDATAIN:
        if( pmsg && (pmsg->lParam >> 16) )
        {
            /* Connect failure */
            Winks_fail( "winks in waitheader state get a error datain msg\r\n" ); //unlikely
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = pmsg->lParam >> 16;
            break;
        }
        
        if( Winks_HttpIfHeadEnd( phttp ) )
        {
#ifdef WINKS_DEBUG
        #ifdef WINKS_SIMULATOR
            char pMsg[1024];

            sprintf( pMsg, "Winks Http %s get datain head in waithead state", phttp->pURL );

            Winks_printHex( (const unsigned char* )(phttp->precv->pdata), (unsigned short )(phttp->precv->offset), 
                (const unsigned char* )pMsg );
        #else
            Winks_printf("Winks Http %s get datain head in waithead state %d bytes\r\n", phttp->pURL,(unsigned short )(phttp->precv->offset) );
        #endif
#endif

            Winks_httpGetDataEncoding( phttp->precv->pdata, &(phttp->Encoding), &(phttp->info.contentlength) );
            
            if( phttp->Encoding == WINKS_HTTP_CHUNK )
            {
                phttp->Chunkstate = WINKS_HTTP_CUIDLE;
                phttp->Chunkblock = 0;
                Winks_HttpChunkUpdate( phttp );
            }
            else
            {
                phttp->RemainRcv = phttp->info.contentlength - Winks_HttpBufLen( phttp->precv->pnext );
            }
            
            if( (ret = Winks_HttpParseHeader(phttp, 0)) < 0 )
            {
                msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
                msg.lParam = 0;
                Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
                phttp->state = WINKS_HTTP_STERROR;
                phttp->ereason = WINKS_HTTP_EDATA;

                break;
            }
            phttp->state = WINKS_HTTP_STWAITDATA;
        }

        break;

    case WINKS_HTTP_EVDATAEND:
        if( (ret = Winks_HttpParseHeader(phttp, 1)) < 0 )
        {
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = WINKS_HTTP_EDATA;
        }
        else
        {
#ifdef WINKS_DEBUG
        #ifdef WINKS_SIMULATOR
            char pMsg[1024];

            sprintf( pMsg, "Winks Http %s get dataend head in waithead state", phttp->pURL );
            Winks_printHex( (const unsigned char* )(phttp->precv->pdata), (unsigned short )(phttp->precv->offset), 
                (const unsigned char* )pMsg );
        #else
            Winks_printf("Winks Http %s get dataend head in waithead state %d bytes\r\n", phttp->pURL, (unsigned short )(phttp->precv->offset) );
        #endif
#endif
            if( (ret = Winks_HttpDealHeader(phttp)) > 0 )
            {
                /* Header data come complete and parse successfully */
                msg.wParam = (WINKS_HTTP_DATAEND << 24) | (phttp - pHttpGlobalCB->HttpHD);
                msg.lParam = Winks_HttpBufLen( phttp->precv->pnext );
                Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
                phttp->state = WINKS_HTTP_STFINISH;
            }
            else if( ret < 0 )
            {
                msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
                msg.lParam = 0;
                Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
                phttp->state = WINKS_HTTP_STERROR;
                phttp->ereason = WINKS_HTTP_EDATA;
            }
        }
        break;

    case WINKS_HTTP_EVDISCONNECT:
		if( pmsg && (pmsg->lParam >> 16) )
			msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((pmsg->lParam) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
		else
			msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EBROKE;
        break;

    case WINKS_HTTP_EVCLOSE:
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EUSER;
        
        return Winks_httpSocketClose( phttp->pSocket, phttp );
        
    default:
        Winks_printf( "WINKS http fsm waitheader get a unknow event %d\r\n", event );
        return -1;
    }

    return 0;
}
/**********************************************************************************\
函数原型：static int Winks_httpfsmWaitdata(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机数据等待状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理数据等待状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmWaitdata(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    Winks_Socketmsg_s msg;
    int ret = 0;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    switch( event )
    {
    case WINKS_HTTP_EVDATAIN:
        if( (phttp->info.statuscode >= HTTP_BadRequest) || (phttp->info.statuscode < HTTP_MultipleChoices) )
        {
            if( pmsg )
                msg.wParam = (WINKS_HTTP_DATAIN << 24) | (pmsg->lParam & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
            else
                msg.wParam = (WINKS_HTTP_DATAIN << 24) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = Winks_httpGetDatalen( phttp );
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        }
        if( pmsg && (pmsg->lParam >> 16) )
        {
            /* data failure, unlikely */
            Winks_fail( "winks http wait data state get a error datain msg\r\n" );
            Winks_httpSocketClose( phttp->pSocket, phttp );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = pmsg->lParam >> 16;
            break;
        }
        break;

    case WINKS_HTTP_EVDATAEND:
        if( (ret = Winks_HttpDealHeader(phttp)) > 0 )
        {
            /* Header data come complete and parse successfully */
            msg.wParam = (WINKS_HTTP_DATAEND << 24) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = Winks_HttpBufLen( phttp->precv->pnext );
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STFINISH;
        }
        else if( ret < 0 )
        {
            msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
            msg.lParam = 0;
            Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
            phttp->state = WINKS_HTTP_STERROR;
            phttp->ereason = WINKS_HTTP_EDATA;
        }

        break;
    
    case WINKS_HTTP_EVDISCONNECT:
        //msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
		if( pmsg && (pmsg->lParam >> 16) )
			msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((pmsg->lParam) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);
		else
			msg.wParam = (WINKS_HTTP_CLOSE << 24) | ((WINKS_HTTP_EBROKE << 16) & 0x00ff0000) | (phttp - pHttpGlobalCB->HttpHD);

        msg.lParam = 0;
        Winks_PostMsg( phttp->channel, phttp->msg, &msg, sizeof(Winks_Socketmsg_s) );
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EBROKE;
        break;

    case WINKS_HTTP_EVCLOSE:
        phttp->state = WINKS_HTTP_STERROR;
        phttp->ereason = WINKS_HTTP_EUSER;
        
        return Winks_httpSocketClose( phttp->pSocket, phttp );
    default:
        Winks_printf( "WINKS http fsm waitdata get a unknow event %d\r\n", event );
        return -1;
    }

    return 0;
}

/**********************************************************************************\
函数原型：static int Winks_httpfsmFinish(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机完成状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理完成状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmFinish(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    if( event == WINKS_HTTP_EVCLOSE )
        return 0;
    
    Winks_printf( "WINKS http fsm finish get a unknow event %d\r\n", event );

    return -1;
}
/**********************************************************************************\
函数原型：static int Winks_httpfsmError(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
函数介绍：
		HTTP状态机错误状态处理函数。
参数说明：
        参数名称	参数类型	    参数介绍	    备注
        phttp	    Winks_HttpCB*	HTTP传输控制块	
        event	    int	            事件码	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数处理错误状态下HTTP控制块的事件。
\*********************************************************************************/
static int Winks_httpfsmError(Winks_HttpCB* phttp, int event, Winks_Socketmsg_s* pmsg )
{
    if( event == WINKS_HTTP_EVCLOSE )
    {
        if( phttp->ereason != WINKS_HTTP_EUSER )
            return 0;
    }
    
    Winks_printf( "WINKS http fsm finish get a unknow event %d\r\n", event );

    return -1;
}

/**********************************************************************************\
函数原型：static Winks_HttpCB* Winks_httphandlealloc()
函数介绍：
		HTTP控制块句柄申请函数。
参数说明：
函数返回值：
        成功返回申请到的控制块，失败返回NULL。
实现说明：
        本函数用来申请一个空闲的HTTP句柄控制块。
\*********************************************************************************/
static Winks_HttpCB* Winks_httphandlealloc()
{
    int i = 0;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    while( i++ < WINKS_HTTP_MAXHTTPHD - 1 )
    {
        /* We keep handle 0 as a debug handle, will not alloc 0 to user */
        if( (++pHttpGlobalCB->hdindex) >= WINKS_HTTP_MAXHTTPHD )
            pHttpGlobalCB->hdindex = 1;
        if( !(pHttpGlobalCB->HttpHD[pHttpGlobalCB->hdindex].ifused) )
        {
            /* Get a vaild http CB */
            pHttpGlobalCB->HttpHD[pHttpGlobalCB->hdindex].ifused = 1;
            return &(pHttpGlobalCB->HttpHD[pHttpGlobalCB->hdindex]);
        }
    }

    Winks_printf( "WINKS http find handle failure\r\n" );
    return NULL;

}

/**********************************************************************************\
函数原型：static int Winks_httphandlefree(Winks_HttpCB* phttp )
函数介绍：
		HTTP控制块句柄释放函数。
参数说明：
        参数名称	参数类型	    参数介绍	备注
        phttp	    Winks_HttpCB*	释放的句柄	
			

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数用来释放一个空闲的HTTP句柄控制块。
\*********************************************************************************/
static int Winks_httphandlefree(Winks_HttpCB* phttp )
{
    if( !(phttp->ifused) )
    {
        Winks_printf( "WINKS http free handle unused\r\n" );
        return -1;
    }

    Winks_HttpBufFree( &(phttp->precv) );
    Winks_HttpBufFree( &(phttp->psend) );
    Winks_HttpBufFree( &(phttp->pdata) );
#ifdef WINKS_DEBUG
    Winks_HttpBufFree( &(phttp->ptsend) );
	Winks_HttpBufFree( &(phttp->ptrecv) );
#endif
    if( phttp->pfile )
    {
        Winks_fclose( phttp->pfile );
    }
    
    Winks_mem_set( phttp, 0, sizeof(Winks_HttpCB) );

    return 0;
}

static unsigned short Winks_HttpGetWay( char* pURL )
{
    unsigned short i = 1;
    char* plocal = NULL;

    while( HttpWay[i][0] != 0 )
    {
        if( ((plocal = Winks_StriStr(pURL, (char* )(HttpWay[i]))) != NULL) && (plocal == pURL) )
            return i;
        i++;
    }

    return 0;
}

static int Winks_HttpParseURL( Winks_HttpCB* phttp )
{
    char* pDHead = "://";
    char* pDfpath = "/";
    char* pHead = NULL;
    //char* pEnd = NULL;
    char* pTemp = NULL;
    int hostlen = 0, i = 0;

    if( (pHead = Winks_StriStr(phttp->pURL, pDHead)) == NULL )
    {
        /* Unlikely */
        Winks_fail( "winks http parse URL can not find ://\r\n" );
        return -1;
    }
    pHead += 3; //skip ://

    pTemp = pHead;
    while( *pTemp )
    {
        if( *pTemp == '/' )
        {
            /* path head */
            break;
        }
        pTemp ++;
        hostlen ++;
    }

    if( *pTemp == 0 )
    {
        /* Only host name in this URL */
        phttp->phost = Winks_HttpBufAppend( &(phttp->pdata), pHead, strlen(pHead) + 1, 1, 0, 0 );
        phttp->ppath = Winks_HttpBufAppend( &(phttp->pdata), pDfpath, strlen(pDfpath) + 1, 1, 0, 0 );
    }
    else
    {
        //Winks_assert( pEnd == NULL );
        phttp->phost = Winks_HttpBufAppend( &(phttp->pdata), pHead, hostlen, 1, 0, 0 );
        Winks_HttpBufAppend( &(phttp->pdata), (char* )&i, 1, 1, 0, 0 );
        phttp->ppath = Winks_HttpBufAppend( &(phttp->pdata), pTemp, strlen(pTemp) + 1, 1, 0, 0 );
    }
    
    pTemp = phttp->phost;
    while( *pTemp )
    {
        if( *pTemp == ':' )
        {
            /* cut off port from url */
            *pTemp = 0;
            pTemp ++;
            while( *pTemp )
            {
                Winks_assert( (*pTemp >= '0' && *pTemp <= '9') );
                phttp->port = (unsigned short)(phttp->port * 10 + (*pTemp - 0x30));
                pTemp ++;
            }
            break;
        }
        pTemp ++;
    }

    if( phttp->port == 0 )
        phttp->port = WINKS_HTTP_DEFPORT;

    return 0;
}

static Winks_HttpCB* Winks_HttpLockHD( int http )
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( http < 0 || http >= WINKS_HTTP_MAXHTTPHD )
    {
        Winks_printf( "winks http handle error\r\n" );
        return NULL;
    }

    if( !(pHttpGlobalCB->HttpHD[http].ifused) )
    {
        //Winks_fail( "winks http handle lock unused\r\n" );
		Winks_printf( "winks http handle lock unused\r\n" );
        return NULL;
    }

    return &(pHttpGlobalCB->HttpHD[http]);

}
/**********************************************************************************\
函数原型：int Winks_httpinit()
函数介绍：
		HTTP协议初始化函数。
参数说明：
			
函数返回值：
		成功返回0，失败返回-1。
实现说明：
        本函数用来初始化HTTP协议栈，任何HTTP接口函数被调用以前都要确保本函数已经被调用。
        本函数初始化HTTP协议栈全局资源。
\*********************************************************************************/
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
int Winks_httpinit(void)
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;
	int i = 0;
	Winks_HttpStatus HttpStatus[] = 
	{
   		"HTTP/1.1 100 Continue",					100,    NULL,
		"HTTP/1.1 101 Switching Protocols",			101,    NULL,
		"HTTP/1.1 200 OK, Success",					200,    NULL,
		"HTTP/1.1 201 Created",						201,    NULL,
		"HTTP/1.1 202 Accepted",					202,    NULL,
		"HTTP/1.1 203 Non-Authoritative Information",	203,    NULL,
		"HTTP/1.1 204 No Content",					204,    NULL,
		"HTTP/1.1 205 Reset Content",				205,    NULL,
		"HTTP/1.1 206 Partial Content",				206,    NULL,
		"HTTP/1.1 300 Multiple Choices",			300,    NULL,
		"HTTP/1.1 301 Moved Permanently",			301,    NULL,
		"HTTP/1.1 302 Moved temporarily",		    302,    NULL,
		"HTTP/1.1 303 See Other",					303,    NULL,
		"HTTP/1.1 304 Not modified",				304,    NULL,
		"HTTP/1.1 305 Use Proxy",					305,    NULL,
		"HTTP/1.1 400 Bad Request - server could not understand request",	400,    NULL,
		"HTTP/1.1 401 Unauthorized",				401,    NULL,
		"HTTP/1.1 402 Payment required",			402,    NULL,
		"HTTP/1.1 403 Forbidden – operation is understood but refused",	403,    NULL,
		"HTTP/1.1 404 Not Found",					404,    NULL,
		"HTTP/1.1 405 Method not allowed",			405,    NULL,
		"HTTP/1.1 406 Not Acceptable",				406,    NULL,
		"HTTP/1.1 407 Proxy Authentication required",	407,    NULL,
		"HTTP/1.1 408 Request Timeout",				408,    NULL,
		"HTTP/1.1 409 Conflict",					409,    NULL,
		"HTTP/1.1 410 Gone",						410,    NULL,
		"HTTP/1.1 411 Length Required",				411,    NULL,
		"HTTP/1.1 412 Precondition failed",			412,    NULL,    
		"HTTP/1.1 413 Requested entity too large",	413,    NULL,
		"HTTP/1.1 414 Request-URI too large",		414,    NULL,
		"HTTP/1.1 415 Unsupported media type",		415,    NULL,
		"HTTP/1.1 500 Internal Server Error",		500,    NULL,
		"HTTP/1.1 501 Not Implemented",				501,    NULL,
		"HTTP/1.1 502 Bad Gateway",					502,    NULL,
		"HTTP/1.1 503 Service Unavailable",			503,    NULL,
		"HTTP/1.1 504 Gateway Timeout",				504,    NULL,
		"HTTP/1.1 505 HTTP version not supported",	505,    NULL,
		"",										0xff,    NULL
	};

	Winks_HttpHead_s HttpSendHead[] = 
	{
		"Accept", NULL,
		"Host", NULL,
		"Content-Length", NULL,
		"Content-Type", NULL,
		"Authorization", NULL,
		"Proxy-Authorization", NULL,
		"User-Agent", NULL,
		"x-wap-profile", NULL,
		"If-Modified-Since", NULL,
		"Cookie", NULL,
		"", NULL
	};

	Winks_HttpHead_s HttpRecvHead[] = 
	{
	//    "Content-Length", Winks_HttpParseContentLength,
	//    "Transfer-Encoding", Winks_HttpParseTransEncoding,
		"Connection", NULL,
		"Set-Cookie", NULL,
		"Set-Cookie2", NULL,
		"", NULL
	};

    if( pHttpGlobalCB != NULL )
    {
        Winks_printf( "WINKS http init also\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    pHttpGlobalCB = Winks_salAlloc( sizeof(Winks_HttpGlobalCB) );
    set_module_glob( WK_MODULE_HTTP, pHttpGlobalCB );
    
    Winks_mem_set( pHttpGlobalCB, 0, sizeof(Winks_HttpGlobalCB) );

	/*
	Winks_mem_cpy( pHttpGlobalCB->HttpStatus, HttpStatus, (sizeof(Winks_HttpStatus) * WINKS_HTTP_MAXSTATUSNUM) );
	Winks_mem_cpy( pHttpGlobalCB->HttpSendHead, HttpSendHead, (sizeof(Winks_HttpHead_s) * WINKS_HTTP_MAXSENDHEAD) );
	Winks_mem_cpy( pHttpGlobalCB->HttpRecvHead, HttpRecvHead, (sizeof(Winks_HttpHead_s) * WINKS_HTTP_MAXRECVHEAD) );
	Winks_mem_cpy( pHttpGlobalCB->HttpFsm, HttpFsm, (sizeof(Winks_HttpFsm) * WINKS_HTTP_MAXSTATE) );
	*/
	Winks_mem_cpy( pHttpGlobalCB->HttpStatus, HttpStatus, sizeof(HttpStatus) );
	while( i < WINKS_HTTP_MAXSTATUSNUM )
	{
		switch( pHttpGlobalCB->HttpStatus[i].code )
		{
		case 200:
			pHttpGlobalCB->HttpStatus[i].parse = Winks_httpParse200;
			break;
		case 301:
			pHttpGlobalCB->HttpStatus[i].parse = Winks_httpParse301;
			break;
		case 302:
			pHttpGlobalCB->HttpStatus[i].parse = Winks_httpParse302;
			break;
		case 304:
			pHttpGlobalCB->HttpStatus[i].parse = Winks_httpParse304;
			break;
		case 401:
			pHttpGlobalCB->HttpStatus[i].parse = Winks_httpParse401;
			break;
		case 407:
			pHttpGlobalCB->HttpStatus[i].parse = Winks_httpParse407;
			break;
		default:
			break;

		}
		i++;
	}
	Winks_mem_cpy( pHttpGlobalCB->HttpSendHead, HttpSendHead, sizeof(HttpSendHead) );

	pHttpGlobalCB->HttpSendHead[0].HttpHeadDeal = Winks_HttpAcceptGen;
	pHttpGlobalCB->HttpSendHead[1].HttpHeadDeal = Winks_HttpHostGen;
	pHttpGlobalCB->HttpSendHead[2].HttpHeadDeal = Winks_HttpContentlengthGen;
	pHttpGlobalCB->HttpSendHead[3].HttpHeadDeal = Winks_HttpContenttypeGen;
	pHttpGlobalCB->HttpSendHead[4].HttpHeadDeal = Winks_HttpAuthoriGen;
	pHttpGlobalCB->HttpSendHead[5].HttpHeadDeal = Winks_HttpProxyAuthoriGen;
	pHttpGlobalCB->HttpSendHead[6].HttpHeadDeal = Winks_HttpUserAgentGen;
	pHttpGlobalCB->HttpSendHead[7].HttpHeadDeal = Winks_HttpUAProfileGen;
	pHttpGlobalCB->HttpSendHead[8].HttpHeadDeal = Winks_HttpIfModifiedGen;
	pHttpGlobalCB->HttpSendHead[9].HttpHeadDeal = Winks_HttpCookieGen;

	Winks_mem_cpy( pHttpGlobalCB->HttpRecvHead, HttpRecvHead, sizeof(HttpRecvHead) );

	pHttpGlobalCB->HttpRecvHead[0].HttpHeadDeal = Winks_HttpParseConnection;
	pHttpGlobalCB->HttpRecvHead[1].HttpHeadDeal = Winks_HttpParseSetCookie;
	pHttpGlobalCB->HttpRecvHead[2].HttpHeadDeal = Winks_HttpParseSetCookie2;

	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STIDLE] = Winks_httpfsmIdle;
	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STGETHOST] = Winks_httpfsmGethost;
	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STCONNECT] = Winks_httpfsmConnect;
	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STWAITHEADER] = Winks_httpfsmWaitheader;
	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STWAITDATA] = Winks_httpfsmWaitdata;
	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STFINISH] = Winks_httpfsmFinish;
	pHttpGlobalCB->HttpFsm[WINKS_HTTP_STERROR] = Winks_httpfsmError;

    
	if( (pHttpGlobalCB->channel = Winks_CreateChn( Winks_Http_MsgProc )) == 0 )
    {
        Winks_printf( "WINKS http init create channel error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    pHttpGlobalCB->Setting.cachesize = WINKS_HTTP_DEFAULTCACHESIZE;
    pHttpGlobalCB->Setting.cookiesize = WINKS_HTTP_DEFAULTCOOKIESIZE;
    pHttpGlobalCB->Setting.ifpipe = WINKS_HTTP_CONDEFAULT;
    
    pHttpGlobalCB->ifinit = 1;

    Winks_SoStartup();
    winks_cache_init();
    winks_cookie_init();


    return WINKS_HTTP_SUCCESS;
}

#else	//WINKS_UNSUPPORTED_GLOBAL_VAR

int Winks_httpinit(void)
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;
  #ifdef WINKS_DEBUG
	Winks_HttpStatus HttpStatus[] = 
	{
   		"HTTP/1.1 100 Continue",					100,    NULL,
		"HTTP/1.1 101 Switching Protocols",			101,    NULL,
		"HTTP/1.1 200 OK, Success",					200,    Winks_httpParse200,
		"HTTP/1.1 201 Created",						201,    NULL,
		"HTTP/1.1 202 Accepted",					202,    NULL,
		"HTTP/1.1 203 Non-Authoritative Information",	203,    NULL,
		"HTTP/1.1 204 No Content",					204,    NULL,
		"HTTP/1.1 205 Reset Content",				205,    NULL,
		"HTTP/1.1 206 Partial Content",				206,    NULL,
		"HTTP/1.1 300 Multiple Choices",			300,    NULL,
		"HTTP/1.1 301 Moved Permanently",			301,    Winks_httpParse301,
		"HTTP/1.1 302 Moved temporarily",		    302,    Winks_httpParse302,
		"HTTP/1.1 303 See Other",					303,    NULL,
		"HTTP/1.1 304 Not modified",				304,    Winks_httpParse304,
		"HTTP/1.1 305 Use Proxy",					305,    NULL,
		"HTTP/1.1 400 Bad Request - server could not understand request",	400,    NULL,
		"HTTP/1.1 401 Unauthorized",				401,    Winks_httpParse401,
		"HTTP/1.1 402 Payment required",			402,    NULL,
		"HTTP/1.1 403 Forbidden – operation is understood but refused",	403,    NULL,
		"HTTP/1.1 404 Not Found",					404,    NULL,
		"HTTP/1.1 405 Method not allowed",			405,    NULL,
		"HTTP/1.1 406 Not Acceptable",				406,    NULL,
		"HTTP/1.1 407 Proxy Authentication required",	407,    Winks_httpParse407,
		"HTTP/1.1 408 Request Timeout",				408,    NULL,
		"HTTP/1.1 409 Conflict",					409,    NULL,
		"HTTP/1.1 410 Gone",						410,    NULL,
		"HTTP/1.1 411 Length Required",				411,    NULL,
		"HTTP/1.1 412 Precondition failed",			412,    NULL,    
		"HTTP/1.1 413 Requested entity too large",	413,    NULL,
		"HTTP/1.1 414 Request-URI too large",		414,    NULL,
		"HTTP/1.1 415 Unsupported media type",		415,    NULL,
		"HTTP/1.1 500 Internal Server Error",		500,    NULL,
		"HTTP/1.1 501 Not Implemented",				501,    NULL,
		"HTTP/1.1 502 Bad Gateway",					502,    NULL,
		"HTTP/1.1 503 Service Unavailable",			503,    NULL,
		"HTTP/1.1 504 Gateway Timeout",				504,    NULL,
		"HTTP/1.1 505 HTTP version not supported",	505,    NULL,
		"",										0xff,    NULL
	};
  #else
	Winks_HttpStatus HttpStatus[] = 
	{
 		"100",  100,    NULL,                              
		"101",	101,    NULL,                          
		"200",	200,    Winks_httpParse200,                
		"201",	201,    NULL,                                
		"202",	202,    NULL,                                
		"203",	203,    NULL,                    
		"204",	204,    NULL,                              
		"205",	205,    NULL,                              
		"206",	206,    NULL,                            
		"300",	300,    NULL,                            
		"301",	301,    Winks_httpParse301,              
		"302",	302,    Winks_httpParse302,            
		"303",	303,    NULL,                                
		"304",	304,    Winks_httpParse304,                
		"305",	305,    NULL,                                
		"400",	400,    NULL,
		"401",	401,    Winks_httpParse401,                
		"402",	402,    NULL,                            
		"403",	403,    NULL,
		"404",	404,    NULL,                                
		"405",	405,    NULL,                          
		"406",	406,    NULL,                            
		"407",	407,    Winks_httpParse407,      
		"408",	408,    NULL,                            
		"409",	409,    NULL,                                
		"410",	410,    NULL,                                  
		"411",	411,    NULL,                            
		"412",	412,    NULL,                          
		"413",	413,    NULL,                      
		"414",	414,    NULL,                          
		"415",	415,    NULL,                        
		"500",	500,    NULL,                          
		"501",	501,    NULL,                            
		"502",	502,    NULL,                              
		"503",	503,    NULL,                          
		"504",	504,    NULL,                            
		"505",	505,    NULL,
		"",		0xff,   NULL
	};
  #endif

	Winks_HttpHead_s HttpSendHead[] = 
	{
		"Accept", Winks_HttpAcceptGen,
		"Host", Winks_HttpHostGen,
		"Content-Length", Winks_HttpContentlengthGen,
		"Content-Type", Winks_HttpContenttypeGen,
		"Authorization", Winks_HttpAuthoriGen,
		"Proxy-Authorization", Winks_HttpProxyAuthoriGen,
		"User-Agent", Winks_HttpUserAgentGen,
		"x-wap-profile", Winks_HttpUAProfileGen,
		"If-Modified-Since", Winks_HttpIfModifiedGen,
		"Cookie", Winks_HttpCookieGen,
#if 0
		"Accept-Charset", Winks_HttpAcCharsetGen,
		"Accept-Language", Winks_HttpAcLanguageGen,
		"Connection", Winks_HttpConnection,
		"Accept-Encoding", Winks_HttpAcEncodingGen,
		"Pragma", Winks_HttpPragmaGen,
#endif
		"", NULL
	};

	Winks_HttpHead_s HttpRecvHead[] = 
	{
	//    "Content-Length", Winks_HttpParseContentLength,
	//    "Transfer-Encoding", Winks_HttpParseTransEncoding,
		"Connection", Winks_HttpParseConnection,
		"Set-Cookie", Winks_HttpParseSetCookie,
		"Set-Cookie2", Winks_HttpParseSetCookie2,
		"", NULL
	};

	Winks_HttpFsm HttpFsm[] = 
	{
		Winks_httpfsmIdle,
		Winks_httpfsmGethost,
		Winks_httpfsmConnect,
		Winks_httpfsmWaitheader,
		Winks_httpfsmWaitdata,
		Winks_httpfsmFinish,
		Winks_httpfsmError
	};

    if( pHttpGlobalCB->ifinit )
    {
        Winks_printf( "WINKS http init also\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    Winks_mem_set( pHttpGlobalCB, 0, sizeof(Winks_HttpGlobalCB) );

	/*
	Winks_mem_cpy( pHttpGlobalCB->HttpStatus, HttpStatus, (sizeof(Winks_HttpStatus) * WINKS_HTTP_MAXSTATUSNUM) );
	Winks_mem_cpy( pHttpGlobalCB->HttpSendHead, HttpSendHead, (sizeof(Winks_HttpHead_s) * WINKS_HTTP_MAXSENDHEAD) );
	Winks_mem_cpy( pHttpGlobalCB->HttpRecvHead, HttpRecvHead, (sizeof(Winks_HttpHead_s) * WINKS_HTTP_MAXRECVHEAD) );
	Winks_mem_cpy( pHttpGlobalCB->HttpFsm, HttpFsm, (sizeof(Winks_HttpFsm) * WINKS_HTTP_MAXSTATE) );
	*/
	Winks_mem_cpy( pHttpGlobalCB->HttpStatus, HttpStatus, sizeof(HttpStatus) );
	Winks_mem_cpy( pHttpGlobalCB->HttpSendHead, HttpSendHead, sizeof(HttpSendHead) );
	Winks_mem_cpy( pHttpGlobalCB->HttpRecvHead, HttpRecvHead, sizeof(HttpRecvHead) );
	Winks_mem_cpy( pHttpGlobalCB->HttpFsm, HttpFsm, sizeof(HttpFsm) );
    
	if( (pHttpGlobalCB->channel = Winks_CreateChn( Winks_Http_MsgProc )) == 0 )
    {
        Winks_printf( "WINKS http init create channel error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    pHttpGlobalCB->Setting.cachesize = WINKS_HTTP_DEFAULTCACHESIZE;
    pHttpGlobalCB->Setting.cookiesize = WINKS_HTTP_DEFAULTCOOKIESIZE;
    pHttpGlobalCB->Setting.ifpipe = WINKS_HTTP_CONDEFAULT;
    
    pHttpGlobalCB->ifinit = 1;

    Winks_SoStartup();
    winks_cache_init();
    winks_cookie_init();


    return WINKS_HTTP_SUCCESS;
}

#endif	//WINKS_UNSUPPORTED_GLOBAL_VAR
/**********************************************************************************\
函数原型：int Winks_httpdeinit()
函数介绍：
		HTTP协议去初始化函数。
参数说明：
参数名称	参数类型	参数介绍	备注
			
函数返回值：
		成功返回0，失败返回-1。
实现说明：
        本函数用来去初始化HTTP协议栈，释放HTTP协议栈全局资源。
        第一版本函数不实现，以存根函数存在。
		第二版：清空所有活动域名查询句柄和socket句柄，以便系统回收资源。
\*********************************************************************************/
int Winks_httpdeinit(void)
{
	int i = 0;
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    winks_cache_close();
    winks_cookie_close();

	for( i = 0; i < WINKS_HTTP_MAXHTTPHD; i++ )
	{
		if( pHttpGlobalCB->HttpHD[i].ifused )
		{
			if( pHttpGlobalCB->HttpHD[i].state == WINKS_HTTP_STGETHOST )
				Winks_CancelGetHostByName( pHttpGlobalCB->HttpHD[i].GHHandle );
			if( pHttpGlobalCB->HttpHD[i].channel )
				Winks_DestroyChn( pHttpGlobalCB->HttpHD[i].channel );
			Winks_httphandlefree( &(pHttpGlobalCB->HttpHD[i]) );
		}
	}

	for( i = 0; i < WINKS_HTTP_MAXSOCKETHD; i++ )
	{
		if( pHttpGlobalCB->SocketHD[i].state != WINKS_HTTP_SOSFREE )
		{
			Winks_DestroyTimer( pHttpGlobalCB->SocketHD[i].timer );
			Winks_closesocket( pHttpGlobalCB->SocketHD[i].s );
		}
	}

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
	Winks_salFree( pHttpGlobalCB );
#endif
	
    return WINKS_HTTP_FAILURE;
}
/**********************************************************************************\
函数原型：int Winks_httpopen( Winks_Http* phttp )
函数介绍：
		HTTP句柄创建函数。
参数说明：
        参数名称	参数类型	参数介绍	        备注
        phttp	    Winks_Http	http控制块设置选项	

函数返回值：
		成功返回创建出来的HTTP传输句柄，失败返回-1。
实现说明：
        本函数用来创建一个HTTP数据传输句柄，用户通过传进来的参数指定需要请求的URL，传输方式
        （安全与否，传输方法，代理服务器设置等）,消息接收方式（消息通道，消息数值等）以及一
        些配合的设置（网页密码等）来指定一次HTTP数据传输，如果是POST方法，还需在本结构中指明
        需要传输的数据内容和长度。协议栈通过用户提供的选项创建HTTP传输句柄并返回句柄数值。
        HTTP传输句柄不可复用。
        API模块内部会保留一定数量的控制块空间，用户调用本函数以后通过分配该空间并记录用户参数
        实现句柄的创建和分配。
\*********************************************************************************/
int Winks_httpopen( Winks_Http* phttp )
{
    Winks_HttpCB* phd = NULL;
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;
	char* pCHeadh = "://";
	char* pCHeadt = "/";
	char* pTHead = NULL;

    if( (phd = Winks_httphandlealloc()) == NULL )
    {
        Winks_printf( "WINKS http find handle failure\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    /* 用户数据保存在控制块附加数据里面，作为头部数据存在 */
    if( (phd->way = Winks_HttpGetWay(phttp->pURL)) == 0 )
    {
        /* Can not find right way, we add http:// in url head as default */
        phd->pURL = Winks_HttpBufAppend( &(phd->pdata), (char* )HttpWay[1], strlen(HttpWay[1]), 1, 0, 0 );
        Winks_HttpBufAppend( &(phd->pdata), phttp->pURL, strlen(phttp->pURL), 1, 1, 0 );
        phd->way = 1;
    }
    else
        phd->pURL = Winks_HttpBufAppend( &(phd->pdata), phttp->pURL, strlen(phttp->pURL), 1, 1, 0 );
    
	/* Check if end with / */
	pTHead = Winks_StriStr( phd->pURL, pCHeadh );
	pTHead += 3;
	if( (pTHead = Winks_StriStr(pTHead, pCHeadt)) == NULL )
	{
		Winks_HttpBufAppendChar( &(phd->pdata), '/', 1 );
	}
	/* add null terminal */
	Winks_HttpBufAppendChar( &(phd->pdata), (char)0, 1 );
    phd->method = phttp->method;
    phd->backmethod = phttp->method;
    if( phttp->pproxy )
    {
        phd->proxy.port = phttp->pproxy->port;
        if(phttp->pproxy->phostname)
            phd->proxy.phostname = Winks_HttpBufAppend( &(phd->pdata), phttp->pproxy->phostname, 
                strlen(phttp->pproxy->phostname) + 1, 1, 1, 0 );
        if( phttp->pproxy->ppassword )
            phd->proxy.ppassword = Winks_HttpBufAppend( &(phd->pdata), phttp->pproxy->ppassword, 
                strlen(phttp->pproxy->ppassword) + 1, 1, 0, 0 );
        if( phttp->pproxy->pusername )
            phd->proxy.pusername = Winks_HttpBufAppend( &(phd->pdata), phttp->pproxy->pusername, 
                strlen(phttp->pproxy->pusername) + 1, 1, 0, 0 );
    }
    if( phttp->pwwwau )
    {
        if( phttp->pwwwau->ppassword )
            phd->wwwau.ppassword = Winks_HttpBufAppend( &(phd->pdata), phttp->pwwwau->ppassword,
                strlen(phttp->pwwwau->ppassword) + 1, 1, 0, 0 );
        if( phttp->pwwwau->pusername )
            phd->wwwau.pusername = Winks_HttpBufAppend( &(phd->pdata), phttp->pwwwau->pusername,
                strlen(phttp->pwwwau->pusername) + 1, 1, 0, 0 );
    }
    phd->channel = phttp->channel;
    phd->msg = phttp->msg;
    /* 用户数据保存完毕，解析URL */
    if( Winks_HttpParseURL( phd ) < 0 )
    {
        Winks_printf( "WINKS http parse URL failure\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->pContenttype )
        phd->pContenttype = Winks_HttpBufAppend( &(phd->pdata), phttp->pContenttype, strlen(phttp->pContenttype) + 1, 1, 0, 0 );
    else if( phttp->method == WINKS_HTTP_MEPOST )
        phd->pContenttype = Winks_HttpBufAppend( &(phd->pdata), (WINKS_HTTP_HTTEXT),
        strlen(WINKS_HTTP_HTTEXT) + 1, 1, 0, 0 );

    if( phd->proxy.phostname && (phttp->pproxy->port != 0) )
    {
        phd->port = phttp->pproxy->port;
    }

    return (phd - pHttpGlobalCB->HttpHD);
}
/**********************************************************************************\
函数原型：int Winks_httpsetopt( int opcode, void* pvalue, int vallen )
函数介绍：
		HTTP参数设置函数。
参数说明：
        参数名称	参数类型	参数介绍	    备注
        opcode	    int	        设置参数选项	具体数值参见常量定义
        pvalue	    void*	    参数指针	
        vallen	    int	        参数指针长度	具体长度由选项确定

函数返回值：
		成功返回0，失败返回-1。
实现说明：
        本函数用来进行HTTP协议栈全局参数设置，目前可设置选项有如下几个：
            是否支持持久连接；
            User-Agent数值；
            UA-Profile数值；
            可保存Cache大小；
            可保存Cookie大小。
        协议栈会将用户提供的全局参数保存并应用到相应的位置。
\*********************************************************************************/
int Winks_httpsetopt( int opcode, void* pvalue, int vallen )
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( pvalue == NULL || vallen <= 0 )
    {
        Winks_printf( "WINKS http set option para error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    switch( opcode )
    {
    case WINKS_HTTP_OPPIPE:
        if( vallen < sizeof(int) )
        {
            Winks_printf( "WINKS http set option para error\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        pHttpGlobalCB->Setting.ifpipe = *((int* )pvalue);
        break;

    case WINKS_HTTP_OPUAGENG:
        if( vallen > WINKS_HTTP_MAXUALEN - 1 )
        {
            Winks_printf( "WINKS http set option para user agent too long\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        Winks_mem_cpy( pHttpGlobalCB->Setting.pUseragent, pvalue, vallen );
        pHttpGlobalCB->Setting.pUseragent[vallen] = 0;
        break;

    case WINKS_HTTP_OPUPROFILE:
        if( vallen > WINKS_HTTP_MAXUPLEN - 1 )
        {
            Winks_printf( "WINKS http set option para UAProfile too long\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        Winks_mem_cpy( pHttpGlobalCB->Setting.pUAprofile, pvalue, vallen );
        pHttpGlobalCB->Setting.pUAprofile[vallen] = 0;
        break;

    case WINKS_HTTP_OPCACHE:
        if( vallen < sizeof(int) )
        {
            Winks_printf( "WINKS http set option para error\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        pHttpGlobalCB->Setting.cachesize = *((int* )pvalue);
        winks_setupcapability_cache( 0, pHttpGlobalCB->Setting.cachesize );
        //add this num in cache module;

        break;

    case WINKS_HTTP_OPCOOKIE:
        if( vallen < sizeof(int) )
        {
            Winks_printf( "WINKS http set option para error\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        pHttpGlobalCB->Setting.cookiesize = *((int* )pvalue);
        winks_setupcapability_cookie( 0, pHttpGlobalCB->Setting.cookiesize );
        //add this num in cookie module;

        break;
    default:
        Winks_printf( "WINKS http set option unknow option\r\n" );
        break;

    }

    return 0;

}
/**********************************************************************************\
函数原型：int Winks_httpgetopt( int opcode, void* pvalue, int* pvallen )
函数介绍：
		HTTP参数读取函数。
参数说明：
        参数名称	参数类型	参数介绍	    备注
        opcode	    int	        设置参数选项	具体数值参见常量定义
        pvalue	    void*	    参数指针	
        pvallen	    int*	    参数指针长度	具体长度由选项确定

函数返回值：
		成功返回0，失败返回
        -1						一般错误；
        WINKS_HTTP_EBUFF		缓冲区长度不够，用户可以在返回的pvallen里面读取到需要的长度并重新申请内存读取数据。
实现说明：
        本函数用来进行HTTP协议栈全局参数读取，目前可读取选项有如下几个：
            是否支持持久连接；
            User-Agent数值；
            UA-Profile数值；
            可保存Cache大小；
            可保存Cookie大小。
        协议栈会将内部保存的全局参数回填入用户提供的缓冲区。
\*********************************************************************************/
int Winks_httpgetopt( int opcode, void* pvalue, int* pvallen )
{
    Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;

    if( pvalue == NULL || pvallen == NULL || *pvallen <= 0 )
    {
        Winks_printf( "WINKS http get option para error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    switch( opcode )
    {
    case WINKS_HTTP_OPPIPE:
        if( *pvallen < sizeof(int) )
        {
            Winks_printf( "WINKS http get option para error\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        *(int* )pvallen = pHttpGlobalCB->Setting.ifpipe;
        *pvallen = sizeof(int);
        break;

    case WINKS_HTTP_OPUAGENG:
        if( *pvallen < (int)strlen(pHttpGlobalCB->Setting.pUseragent) + 1 )
        {
            Winks_printf( "WINKS http get option para user agent too long\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        Winks_mem_cpy( pvalue, pHttpGlobalCB->Setting.pUseragent, strlen(pHttpGlobalCB->Setting.pUseragent) + 1 );
        *pvallen = strlen(pHttpGlobalCB->Setting.pUseragent) + 1;
        break;

    case WINKS_HTTP_OPUPROFILE:
        if( *pvallen < (int)strlen(pHttpGlobalCB->Setting.pUAprofile) + 1 )
        {
            Winks_printf( "WINKS http get option para user agent too long\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        Winks_mem_cpy( pvalue, pHttpGlobalCB->Setting.pUAprofile, strlen(pHttpGlobalCB->Setting.pUAprofile) + 1 );
        *pvallen = strlen(pHttpGlobalCB->Setting.pUAprofile) + 1;
        break;

    case WINKS_HTTP_OPCACHE:
        if( *pvallen < sizeof(int) )
        {
            Winks_printf( "WINKS http get option para error\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        *((int* )pvalue) = pHttpGlobalCB->Setting.cachesize;
        *pvallen = sizeof(int);
        break;

    case WINKS_HTTP_OPCOOKIE:
        if( *pvallen < sizeof(int) )
        {
            Winks_printf( "WINKS http get option para error\r\n" );
            return WINKS_HTTP_FAILURE;
        }
        *((int* )pvalue) = pHttpGlobalCB->Setting.cookiesize;
        *pvallen = sizeof(int);
        break;

    default:
        Winks_printf( "WINKS http get option unknow option\r\n" );
        break;

    }

    return 0;

}

/**********************************************************************************\
函数原型：int Winks_httpsethead( int http, char* ptag, char* pvalue )
函数介绍：
		HTTP头部数据设置函数。
参数说明：
        参数名称	参数类型	参数介绍	备注
        http	    int	        HTTP传输句柄	
        ptag	    char*	    头部数据tag值	
        pvalue	    chat*	    头部数据内容	

函数返回值：
		成功返回0，失败返回-1。
实现说明：
        本函数用来进行某一HTTP传输句柄的头部数据设置。一旦用户创建一个HTTP传输句柄，
        协议栈会为该句柄添加一些必要的默认头部数据，例如Accept, Host,和content-length。
        用户可以通过本函数添加其他头部数据或修改HTTP协议栈填入的默认的头部数据。协议栈
        会无条件的应用用户提供的设置来进行请求数据的填写或修改内部默认数据，所以对于用
        户的设置数值的正确性需要由用户自己判断和维护，如果用户不能确定设置数据的正确性，
        本模块建议用户使用系统的默认值（即不进行该字段的设置）。
        本函数会在内部使用HTTP buffer来保存用户提供的头部数据，并在数据发送的时候应用
        该数据进行请求。
\*********************************************************************************/
int Winks_httpsethead( int http, char* ptag, char* pvalue )
{
    Winks_HttpCB* phttp = NULL;
    int i = 0;

    if( ptag == NULL || pvalue == NULL )
    {
        Winks_printf( "WINKS http set header para error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http set header hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->state != WINKS_HTTP_STIDLE )
    {
        Winks_printf( "WINKS http set header in error state %d\r\n", phttp->state );
        return WINKS_HTTP_FAILURE;
    }

    for( i = 0; i < WINKS_HTTP_MAXTAG; i++ )
    {
        if( phttp->tag[i].tag == NULL )
        {
            phttp->tag[i].tag = Winks_HttpBufAppend( &(phttp->pdata), ptag, strlen(ptag) + 1, 1, 1, 0 );
            phttp->tag[i].pvalue = Winks_HttpBufAppend( &(phttp->pdata), pvalue, strlen(pvalue) + 1, 1, 0, 0 );
            break;
        }
    }

    if( i == WINKS_HTTP_MAXTAG )
    {
        Winks_printf( "WINKS http set header no tag slot\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    return WINKS_HTTP_SUCCESS;
}

/**********************************************************************************\
函数原型：int Winks_httpappenddata( int http, char* pdata, int len, int dtype )
函数介绍：
		HTTP头部发送数据添加函数。
参数说明：
        参数名称	    参数类型	参数介绍	备注
        http	        int	        HTTP传输句柄	
        pdata           char*	    头部发送数据
        len             int         数据长度
		dtype			int			数据类型，目前有如下几种：
									WINKS_HTTP_DTDATA	传入的是一段数据
									WINKS_HTTP_DTFILE	传入的是一个文件名（UNICODE编码）
									二者只能有一个，不可以既传入文件名又传入数据，否则发送数据会出错返回。
函数返回值：
		成功返回添加的数据长度，失败返回-1。
实现说明：
        本函数用来进行某一HTTP传输句柄的头部发送数据的添加。如果用户准备通过POST方法发送
        一段数据，就需要通过本函数添加发送的数据。如果用户在调用传输启动函数时尚未设置
        发送的数据类型，协议会默认按照"text/ *"类型发送数据。
        本函数提供用户一种分片添加发送数据的接口，以增加内存的使用效率，用户可以反复调用本函数
        以便分片将发送数据填写入传输句柄，每次调用传入的数据会顺序添加在发送数据的尾部。
        本函数会在内部使用HTTP buffer来保存用户提供的头部发送数据，并在数据发送的时候应用
        该数据进行请求。
\*********************************************************************************/
int Winks_httpappenddata( int http, char* pdata, int len, int dtype )
{
    Winks_HttpCB* phttp = NULL;

    if( pdata == NULL || len == 0 )
    {
        Winks_printf( "WINKS http append data para error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http append data hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->state != WINKS_HTTP_STIDLE )
    {
        Winks_printf( "WINKS http append data in error state %d\r\n", phttp->state );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->method != WINKS_HTTP_MEPOST )
    {
        Winks_printf( "WINKS http append data not post method\r\n" );
        return WINKS_HTTP_FAILURE;
    }

	if( dtype == WINKS_HTTP_DTDATA )
	{
		/* 用户POST数据保存在控制块附加数据中，作为消息体存在 */ 
		if( phttp->pfilename )
		{
			Winks_printf( "WINKS http append data have file add data\r\n" );
			return WINKS_HTTP_FAILURE;
		}
		Winks_HttpBufAppend( &(phttp->pdata), pdata, len, 0, 0, 0 );
	}
	else if( dtype == WINKS_HTTP_DTFILE )
	{
		if( phttp->pdata->pnext )
		{
			Winks_printf( "WINKS http append data have data add file\r\n" );
			return WINKS_HTTP_FAILURE;
		}
		phttp->pfilename = Winks_HttpBufAppend( &(phttp->pdata), pdata, len + 2, 1, 0, 1 );
		phttp->fileoff = 0;
		phttp->pfile = NULL;
	}
	else
    {
        Winks_printf( "WINKS http append data data type unknow\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    return len;
}

/**********************************************************************************\
函数原型：int Winks_httpstart( int http )
函数介绍：
		HTTP传输启动函数。
参数说明：
        参数名称	参数类型	参数介绍	备注
        http	    int	        HTTP传输句柄	

函数返回值：
		成功返回0，失败返回-1。
实现说明：
        本函数用来启动HTTP数据传输。用户通过一系列函数创建好HTTP传输句柄并将相关参数设置
        完成以后可以通过调用本函数进行HTTP数据传输。一旦请求数据到达，用户设置的消息通道
        会接受到用户设置的消息数值，其中消息参数为两个长整型数字，其中wParam低16位代表HTTP
        传输句柄，第三个8位代表一个错误码，最高8位代表消息类型，目前可能的消息类型为如下几种：
            域名查询成功；
            服务器连接成功；
            数据到来；
            重定向
            数据传输结束；
            HTTP传输句柄关闭。
        
        lParam代表接收到的数据长度。由于HTTP的数据可能是分片到来，协议栈每接收到一片数据
        以后都会通知用户数据到来并告知用户相应的数据长度，用户可以使用本消息信息绘制诸如
        滑动条之类的图案提供给使用者。
        本函数会在内部使用HTTP buffer来保存用户提供的头部数据，并在数据发送的时候应用该
        数据进行请求。
\*********************************************************************************/
int Winks_httpstart( int http )
{
    Winks_HttpCB* phttp = NULL;

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http start hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }
    
    if( phttp->state != WINKS_HTTP_STIDLE )
    {
        Winks_printf( "WINKS http start in error state %d\r\n", phttp->state );
        return WINKS_HTTP_FAILURE;
    }
    if( phttp->pfilename )
	{
		FILE* pfile = NULL;
		/* Check if the file exist */
		if( (pfile = Winks_fopenUni((const unsigned short* )phttp->pfilename, "rb")) == NULL )
		{
			Winks_printf( "WINKS http start file open error\r\n" );
			return WINKS_HTTP_FAILURE;
		}
		Winks_fclose( pfile );
	}
    Winks_printf( "WINKS http %s start goto fsm\r\n", phttp->pURL );
    if( Winks_httpfsm(phttp, WINKS_HTTP_EVSTART) != 0 )
    {
        Winks_printf( "WINKS http start fsm error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    return WINKS_HTTP_SUCCESS;
}
/**********************************************************************************\
函数原型：int Winks_httprecv( int http, Winks_HttpInfo* pInfo, int offset, char* pbuf, int buflen )
函数介绍：
		HTTP数据接收函数。
参数说明：
        参数名称	参数类型	    参数介绍	        备注
        http	    int	            HTTP传输句柄	
        pInfo   	Winks_HttpInfo*	HTTP头部信息	    保存一些关键的HTTP头部信息，如需要了解头部具体内容
                                                        可通过头部数据获取函数获得。为空代表不关心。
        offset	    int	            需要读取数据的位移	
        pbuf	    char*	        数据存储缓冲区	    为空代表不关心。
        buflen	    int	            数据存储缓冲区长度	

函数返回值：
		成功返回读取的数据长度，失败返回
        -1						一般错误；
        WINKS_HTTP_ESTATUS	    状态错误，尚无数据可读；
        WINKS_HTTP_EBLOCK	    数据尚未完全到达，需等待。
实现说明：
        本函数用来接收HTTP服务器返回的数据，其中如果用户需要，同时也可返回本次HTTP请求
        的相关简单信息，包括状态码，连接方式，数据长度等。本函数不会返回具体的HTTP响应
        头信息，用户如果需要可通过头部数据获取函数获得具体的头部响应信息。本函数提供了
        一个流式数据接收接口，即用户可以通过提供要求的接收数据位移和接收缓冲区长度分片
        读取数据，以便应用更有效率的使用缓冲区内存。除非用户关闭本传输句柄，否则本传输
        句柄内部接收到的数据会一直有效，用户可以反复读取数据。
        本函数会在内部使用HTTP buffer来保存接收到的数据，并在用户请求的时候按需提供数据，
        同时还需要按照用户的要求提供本次传输信息。
\*********************************************************************************/
int Winks_httprecv( int http, Winks_HttpInfo* pInfo, int offset, char* pbuf, int buflen )
{
    Winks_HttpCB* phttp = NULL;
    Winks_HttpBuff* pBuff = NULL;

#ifdef DBG_UNSUPPORT_PROTOCOL   /* renzhan add for debug*/
    extern char *DbgHttpBuf;

if(DbgHttpBuf != NULL)
{
    Winks_mem_cpy(pbuf, &DbgHttpBuf[offset],buflen);

    return buflen;
}
#endif

	if( pbuf == NULL || buflen <= 0 )
	{
        Winks_printf( "WINKS http recv para error\r\n" );
        return WINKS_HTTP_FAILURE;
	}

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http recv hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->state < WINKS_HTTP_STWAITDATA )
    {
        Winks_printf( "WINKS http recv status error\r\n" );
        return WINKS_HTTP_ESTATUS;
    }

    if( phttp->state == WINKS_HTTP_STERROR )
    {
        Winks_printf( "WINKS http recv in WINKS_HTTP_STERROR\r\n" );
    }

    Winks_assert( phttp->precv != NULL );

    if( pInfo != NULL )
    {
        Winks_mem_cpy( pInfo, &(phttp->info), sizeof(Winks_HttpInfo) );
    }

    if( (pBuff = phttp->precv->pnext) == NULL )
    {
        Winks_printf( "WINKS http recv no data error\r\n" );
        return WINKS_HTTP_EBLOCK;
    }

    return Winks_HttpBufCopy( pBuff, pbuf, offset, buflen );
}

/**********************************************************************************\
函数原型：int Winks_httpgetinfo( int http, Winks_HttpInfo* pInfo )
函数介绍：
		获取HTTP头信息函数。
参数说明：
        参数名称	参数类型	    参数介绍	        备注
        http	    int	            HTTP传输句柄	
        pInfo   	Winks_HttpInfo*	HTTP头部信息	    保存一些关键的HTTP头部信息，如需要了解头部具体内容
                                                        可通过头部数据获取函数获得。为空代表不关心。

函数返回值：
		成功返回0，失败返回
        -1						一般错误；
        WINKS_HTTP_ESTATUS	    状态错误，尚无数据可读；
        WINKS_HTTP_EBLOCK	    数据尚未完全到达，需等待。
实现说明：
        本函数用来读取本次HTTP请求返回的相关简单信息，包括状态码，连接方式，数据长度等。
        本函数不会返回具体的HTTP响应头信息，用户如果需要可通过头部数据获取函数获得具体
        的头部响应信息。
\*********************************************************************************/
int Winks_httpgetinfo( int http, Winks_HttpInfo* pInfo )
{
    Winks_HttpCB* phttp = NULL;
//    Winks_HttpBuff* pBuff = NULL;

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http recv hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->state < WINKS_HTTP_STWAITDATA )
    {
        Winks_printf( "WINKS http recv status error\r\n" );
        return WINKS_HTTP_ESTATUS;
    }

    if( phttp->state == WINKS_HTTP_STERROR )
    {
        Winks_printf( "WINKS http recv in WINKS_HTTP_STERROR\r\n" );
    }

    Winks_assert( phttp->precv != NULL );

    if( pInfo != NULL )
    {
        Winks_mem_cpy( pInfo, &(phttp->info), sizeof(Winks_HttpInfo) );
        return 0;
    }

    return WINKS_HTTP_FAILURE;
}
/**********************************************************************************\
函数原型：int Winks_httppulldata( int http, char* pbuf, int buflen )
函数介绍：
		HTTP数据接收函数。
参数说明：
        参数名称	参数类型	    参数介绍	        备注
        http	    int	            HTTP传输句柄	
        pbuf	    char*	        数据存储缓冲区	    为空代表不关心。
        buflen	    int	            数据存储缓冲区长度	

函数返回值：
		成功返回读取的数据长度，失败返回
        -1						一般错误；
        WINKS_HTTP_ESTATUS	    状态错误，尚无数据可读；
        WINKS_HTTP_EBLOCK	    数据尚未完全到达，需等待。
实现说明：
        本函数用来接收HTTP服务器返回的数据，本函数可以较节省内存的获取HTTP数据，用户调用本函数获得
		数据以后，该数据即会从HTTP数据缓冲区中删除。用户可以反复调用本函数，直到读空整个HTTP的数据
		缓冲区。
		特别的，本函数一旦被调用，Winks_httprecv函数既不可用，如果调用，返回数据将会出现顺序错误。不过
		即使Winks_httprecv被调用过，本函数还是可以生效。
\*********************************************************************************/
int Winks_httppulldata( int http, char* pbuf, int buflen )
{
    Winks_HttpCB* phttp = NULL;

#ifdef DBG_UNSUPPORT_PROTOCOL   /* renzhan add for debug*/
    extern char *DbgHttpBuf;

if(DbgHttpBuf != NULL)
{
    Winks_mem_cpy(pbuf, &DbgHttpBuf[offset],buflen);

    return buflen;
}
#endif

	if( pbuf == NULL || buflen <= 0 )
	{
        Winks_printf( "WINKS http recv para error\r\n" );
        return WINKS_HTTP_FAILURE;
	}

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http recv hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( phttp->state < WINKS_HTTP_STWAITDATA )
    {
        Winks_printf( "WINKS http recv status error\r\n" );
        return WINKS_HTTP_ESTATUS;
    }

    if( phttp->state == WINKS_HTTP_STERROR )
    {
        Winks_printf( "WINKS http recv in WINKS_HTTP_STERROR\r\n" );
    }

    Winks_assert( phttp->precv != NULL );

    if( phttp->precv->pnext == NULL )
    {
        Winks_printf( "WINKS http recv no data error\r\n" );
        return WINKS_HTTP_EBLOCK;
    }

    return Winks_HttpBufPullup( phttp, &(phttp->precv->pnext), pbuf, buflen );
}

/**********************************************************************************\
函数原型：int Winks_httpgethead( int http, char* ptag, char* pvalue, int* pvallen )
函数介绍：
		HTTP头部数据读取函数。
参数说明：
        参数名称	参数类型	参数介绍	备注
        http	    int	        HTTP传输句柄	
        ptag	    char*	    头部数据tag值	
        pvalue	    chat*	    头部数据缓冲区	
        pvallen	    int*	    头部数据缓冲区长度	

函数返回值：
		成功返回0，失败返回
        -1						一般错误；
        WINKS_HTTP_EBUFF		缓冲区长度不够，用户可以在返回的pvallen里面读取到需要的长度并重新申请内存读取数据。
实现说明：
        本函数用来读取本次HTTP传输的头部数据，协议栈会按照用户提供的头部tag将相应的内容填入
        用户提供的缓冲区返回给用户。填写的内容是一个零字符结尾的字符串。
        本函数会在内部使用HTTP buffer来保存服务器传送过来的头部数据，并在需要的时候填写给用户。
\*********************************************************************************/
int Winks_httpgethead( int http, char* ptag, char* pvalue, int* pvallen )
{
    Winks_HttpCB* phttp = NULL;
    int len = 0;
    char* pHead = NULL;
    char* pNTag = NULL;
    char pBuf[WINKS_HTTP_HEADVALLEN];

    if( ptag == NULL || pvalue == NULL || pvallen == NULL || *pvallen <= 0 )
    {
        Winks_printf( "WINKS http gethead param error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http gethead hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }
    
    if( phttp->state < WINKS_HTTP_STWAITDATA )
    {
        Winks_printf( "WINKS http gethead status error\r\n" );
        return WINKS_HTTP_ESTATUS;
    }

    if( phttp->state == WINKS_HTTP_STERROR )
    {
        Winks_printf( "WINKS http gethead in WINKS_HTTP_STERROR\r\n" );
    }

    if( strlen(ptag) > WINKS_HTTP_HEADVALLEN - 2 )
    {
        pNTag = Winks_salAlloc(strlen(ptag) + 2);
    }
    else
        pNTag = pBuf;

    sprintf( pNTag, "\n%s", ptag );

    len = Winks_HttpGetHeadValue( phttp->precv->pdata, pNTag, &pHead );
    
    if( pHead == NULL )
    {
        Winks_printf( "WINKS http gethead no data\r\n" );
        return WINKS_HTTP_FAILURE;
    }
    if( len > *pvallen - 1 )
    {
        Winks_printf( "WINKS http gethead buflen to short\r\n" );
        *pvallen = len + 1;
        return WINKS_HTTP_EBUFF;
    }
    Winks_mem_cpy( pvalue, pHead, len );
    pvalue[len] = 0;

    return WINKS_HTTP_SUCCESS;

}
/**********************************************************************************\
函数原型：const char* Winks_httpStatus( int statuscode )
函数介绍：
		HTTP状态码解释函数。
参数说明：
        参数名称	参数类型	参数介绍	备注
        statuscode	int	        HTTP状态码	

函数返回值：
        成功返回对应的字符串，失败返回NULL。
实现说明：
        本函数用来返回一个HTTP头部状态码对应的字符串内容。
        本函数会在内部保存状态码字符串，按照用户要求返回。
\*********************************************************************************/
const char* Winks_httpStatus( int statuscode )
{
    int i = 0;
	Winks_HttpGlobalCB* pHttpGlobalCB = WINKS_HTTPGETGLOBALVAR;
    
    while( pHttpGlobalCB->HttpStatus[i].pStr[0] != 0 )
    {
        if( pHttpGlobalCB->HttpStatus[i].code == statuscode )
            return (const char* )(pHttpGlobalCB->HttpStatus[i].pStr);
        i++;
    }

    return NULL;
}
/**********************************************************************************\
函数原型：int Winks_httpclose( int http )
函数介绍：
		HTTP句柄关闭函数。
参数说明：
        参数名称	参数类型	参数介绍	备注
        http	    int	        HTTP传输句柄	

函数返回值：
        成功返回0，失败返回-1。
实现说明：
        本函数用来关闭一个HTTP传输句柄。本函数成功返回以后，句柄相关资源会被全部释放，
        用户无法使用该句柄进行任何操作。但是由于系统不能确保在被关闭的时候是否还有句
        柄相关的消息在消息通道里面，所以用户调用了本函数以后还需准备处理可能的与该句
        柄相关的消息。
        本函数会在内部销毁与句柄相关的所有资源。但是仍然会保留相关的Cache和Cookie数
        据以便后续使用。
\*********************************************************************************/
int Winks_httpclose( int http )
{
    Winks_HttpCB* phttp = NULL;

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http start hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }
    
    if( Winks_httpfsm(phttp, WINKS_HTTP_EVCLOSE) != 0 )
    {
        Winks_printf( "WINKS http start fsm error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    Winks_httphandlefree( phttp );

    return WINKS_HTTP_SUCCESS;
}

int Winks_httpgeturl( int http, char* pbuf, int* len )
{
    Winks_HttpCB* phttp = NULL;

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http start hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( *len < (int)(strlen(phttp->pURL) + 1) )
    {
        *len = strlen(phttp->pURL) + 1;
        return WINKS_HTTP_EBUFF;
    }

    Winks_mem_cpy( pbuf, phttp->pURL, strlen(phttp->pURL) + 1 );

    return strlen(phttp->pURL) + 1;
}

/* 内部调试用，不公开给应用 */
int Winks_httpgetheaddata( int http, char* pbuf, int* len )
{
    Winks_HttpCB* phttp = NULL;

    if( (phttp = Winks_HttpLockHD(http)) == NULL )
    {
        Winks_printf( "WINKS http start hd error\r\n" );
        return WINKS_HTTP_FAILURE;
    }

    if( *len < (int)(phttp->precv->offset) )
    {
        *len = phttp->precv->offset;
        return WINKS_HTTP_EBUFF;
    }

    Winks_mem_cpy( pbuf, phttp->precv->pdata, phttp->precv->offset );

    return phttp->precv->offset;
}
