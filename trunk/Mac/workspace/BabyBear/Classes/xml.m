#include "xml.h"
#include "stddef.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

#define WX_CAMP	(char* )"&amp;"
#define WX_CLT    (char* )"&lt;"
#define WX_CGT    (char* )"&gt;"
#define WX_CAPOS	(char* )"&apos;"
#define WX_CQUOT	(char* )"&quot;"

//�ַ�״̬����������
#define WX_TAGBG			1
#define WX_TAGEND			2
#define WX_TAGENDBG		3
#define WX_NULLELEEND		4
#define WX_PIBG			5
#define WX_PROBG			6
#define WX_PIEND			7
#define WX_COMBG			8
#define WX_COMEND			9
#define WX_CDATABG		10
#define WX_CDATAEND		11
#define WX_NA				20

//�ַ�״̬��״̬��
#define WX_CSIDLE		0
#define WX_CSCDATA	1

//�ĵ�����״̬��״̬��
#define WX_DSINIT			0
#define WX_DSPROLOG		1
#define WX_DSELEMENT		2
#define WX_DSELETAG		3
#define WX_DSELEATTR		4
#define WX_DSELECONTENT	5
#define WX_DSELETAGEND    6

//�ڴ�����볤��
#define WX_MEMLEN        2048

#define WX_DFPROLOG    ((char* )"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>")

static int xml_freeele( XmlEleInt_s* pele )
{
    if( pele->pchild )
        xml_freeele( pele->pchild );

    if( pele->pbrother )
        xml_freeele( pele->pbrother );

    if( pele->tagbflen != 0 )
        malloc( pele->ptag );
    if( pele->conbflen != 0 )
        malloc( pele->pcontent );

    return 0;
}


static int xml_free( XmlInt_s* pxml )
{
    XmlMem_s* pmem = NULL;

    if( pxml->pelement )
        xml_freeele( pxml->pelement );

    while( pxml->pmem->pnext )
    {
        pmem = pxml->pmem->pnext;
        pxml->pmem->pnext = pmem->pnext;

        malloc( pmem );
    }

    malloc( pxml );

    return 0;
}

static int xml_ifnullchar( char c )
{
    if( (c == ' ') || (c == 0x0d) || (c == 0x0a) || (c == 0x09) )
        return 1;

    return 0;
}

static int xml_pargetbackoff( XmlMem_s* pdata, int len )
{
    XmlMem_s* plast = NULL;

    while( pdata->pnext )
    {
        if( pdata->offset < pdata->len )
            break;
        plast = pdata;
        pdata = pdata->pnext;
    }

    if( pdata->offset >= len )
    {
        pdata->offset -= len;
        return 0;
    }
    else
    {
		if (!plast) {
			assert(0);
			return -1;
		}
		if( len > pdata->offset + plast->offset )
		{
			/* ��֧�ֻ��˳���һ�����ݿ鳤�ȵ�λ�� */
			assert(0);
			return -1;
		}
		len -= pdata->offset;
		pdata->offset = 0;
		
		plast->offset -= len;

        return 0;
    }

    return 0;
}

static int xml_pargetnextchar( XmlMem_s* pdata, char* pret )
{
    while( pdata )
    {
        if( pdata->offset < pdata->len )
        {
            *pret = pdata->pdata[pdata->offset];
            pdata->offset ++;

            return 0;
        }
        else if( pdata->offset == pdata->len )
        {
            pdata = pdata->pnext;
        }
        else
        {
            assert( 0 );
        }
    }

    return -1;
}

/*
static char* xml_pargetcurpoint( XmlMem_s* pdata )
{
    while( pdata->pnext )
        pdata = pdata->pnext;

    return pdata->pdata + pdata->offset;
}

static int xml_pargetdata( XmlMem_s* pdata, char* pdest, char* psource, int len )
{
    int cpylen = 0;
    char* ps = psource;

    while( pdata )
    {
        if( (pdata->pdata <= psource) && (pdata->pdata + pdata->offset > psource) )
        {
            cpylen = (pdata->offset - (psource - pdata->pdata)) > len ? len : (pdata->offset - (psource - pdata->pdata));
            while( len )
            {
                memcpy( pdest, ps, cpylen );
                if( (len -= cpylen) <= 0 )
                    break;

                pdata = pdata->pnext;
                ps = pdata->pdata;
                cpylen = pdata->offset > len ? len : pdata->offset;
            }
            return 0;
        }
        
        pdata = pdata->pnext;
    }

    return -1;

}
*/

static int xml_checkfollow( XmlMem_s* pdata, char* follow )
{
    int i = 0;
    char ca = 0;

    while( *follow )
    {
        i++;
        if( xml_pargetnextchar( pdata, &ca ) < 0 )
        {
            if( i - 1 > 0 )
                xml_pargetbackoff( pdata, i - 1 );
            return -1;
        }
        if( ca != *follow )
        {
            xml_pargetbackoff( pdata, i );
            return -1;
        }

        follow ++;
    }

    return 0;
}

static int xml_deallt( XmlMem_s* pdata, char* ptoken )
{
    char ca = 0;

    if( xml_pargetnextchar( pdata, &ca ) < 0 )
        return WX_EXMLINVALID;

    ptoken[0] = 1;
    switch( ca )
    {
    case '/':
        ptoken[1] = WX_TAGENDBG;
        break;

    case '?':
        if( xml_checkfollow( pdata, "xml") >= 0 )
            ptoken[1] = WX_PROBG;
        else
            ptoken[1] = WX_PIBG;

        break;
        
    case '!':
        if( xml_checkfollow( pdata, "[CDATA[") >= 0 )
            ptoken[1] = WX_CDATABG;
        else if( xml_checkfollow( pdata, "--") >= 0 )
            ptoken[1] = WX_COMBG;
        else 
            return WX_EXMLINVALID;
        break;

    default:
        xml_pargetbackoff( pdata, 1 );
        ptoken[1] = WX_TAGBG;
        break;
    }

    return 0;

}
//char fsm
static int xml_getnexttoken( XmlInt_s* pxml, XmlMem_s* pdata, char* ptoken )
{
    char ca = 0; 
    int ret = 0;

    if( xml_pargetnextchar( pdata, &ca ) < 0 )
        return -1;
    
    if( pxml->charstate == WX_CSIDLE )
    {
        /* ����ע��״̬�£����ǽ���ʶ��-���� */
        if( pxml->ifcommand && ca != '-' )
        {
            ptoken[0] = 0;
            ptoken[1] = ca;

            return 0;
        }
        switch( ca )
        {
        case '<':
            if( (ret = xml_deallt(pdata, ptoken)) < 0 )
            {
                pxml->parerr = ret;
                return -1;
            }
            if( (ptoken[0] == 1) && (ptoken[1] == WX_CDATABG) )
            {
                pxml->charstate = WX_CDATABG;
                ptoken[1] = WX_NA;
            }
            break;
            
        case '/':
            if( xml_checkfollow( pdata, ">") >= 0 )
            {
                ptoken[0] = 1;
                ptoken[1] = WX_NULLELEEND;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = '/';
            }
            break;

        case '-':
            if( xml_checkfollow( pdata, "->") >= 0 )
            {
                ptoken[0] = 1;
                ptoken[1] = WX_COMEND;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = '-';
            }
            break;
        case '>':
            ptoken[0] = 1;
            ptoken[1] = WX_TAGEND;
            break;

        case '?':
            if( xml_checkfollow( pdata, ">") >= 0 )
            {
                ptoken[0] = 1;
                ptoken[1] = WX_PIEND;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = '?';
            }
            break;
            
        case '&':
            if( xml_checkfollow( pdata, WX_CAMP + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '&';
            }
            else if( xml_checkfollow( pdata, WX_CLT + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '<';
            }
            else if( xml_checkfollow( pdata, WX_CGT + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '>';
            }
            else if( xml_checkfollow( pdata, WX_CAPOS + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '\'';
            }
            else if( xml_checkfollow( pdata, WX_CQUOT + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '\"';
            }
            else
            {
                /* �ַ���ת�壬�ݲ�֧�� */
                assert(0);
            }
            break;

        default:
            ptoken[0] = 0;
            ptoken[1] = ca;
            break;
        }
    }
    else if( pxml->charstate == WX_CDATABG )
    {
        if( ca == ']' )
        {
            if( xml_checkfollow( pdata, "]>") >= 0 )
            {
                pxml->charstate = WX_CSIDLE;
                ptoken[0] = 1;
                ptoken[1] = WX_NA;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = ']';
            }
        }
        else
        {
            ptoken[0] = 0;
            ptoken[1] = ca;
        }
    }
    else
    {
        assert( 0 );
    }

    return 0;
}

static XmlInt_s* xml_allocxml()
{
    XmlInt_s* pret = NULL;

    if( (pret = malloc( WX_MEMLEN )) == NULL )
        return NULL;

    memset( pret, 0, WX_MEMLEN );
    pret->ifsd = 1;
    pret->pmem = (XmlMem_s* )(pret + 1);
    pret->pmem->pdata = (char* )(pret->pmem + 1);
    pret->pmem->len = WX_MEMLEN - (sizeof(XmlInt_s) + sizeof(XmlMem_s));

    /*
    pret->pelement = (XmlEleInt_s* )(pret->pmem + 1);
    
    pret->pparele = pret->pelement;
    pret->pelement->ptag = pret->pelement->tagbuf;
    pret->pelement->tagbflen = WX_DFTAGLEN;
    pret->pelement->pcontent = pret->pelement->conbuf;
    pret->pelement->conbflen = WX_DFCONLEN;
    
    pret->pmem->offset = sizeof(XmlEleInt_s);
    */

    return pret;
}

static XmlEleInt_s* xml_pargetnewele( XmlInt_s* pxml )
{
    XmlMem_s* pmem = NULL;
    XmlMem_s* pnew = NULL;
    XmlEleInt_s* pret = NULL;

    pmem = pxml->pmem;

    while( pmem->pnext )
        pmem = pmem->pnext;

    if( (pmem->len - pmem->offset) > sizeof(XmlEleInt_s) )
    {
        /*pmem->len���ı�����sizeof(XmlEleInt_s)Ҳ���ı�����������жϳ�����Բ���Ժ�Ҳ���ᳬ��*/
        pmem->offset = (pmem->offset + 3) & 0xfffffffc;
        pmem->offset += sizeof(XmlEleInt_s);

        pret = (XmlEleInt_s* )( pmem->pdata + pmem->offset - sizeof(XmlEleInt_s) );
    }
    else
    {
        if( (pnew = malloc( WX_MEMLEN )) == NULL )
            return NULL;
        memset( pnew, 0, WX_MEMLEN );
        
        pnew->len = WX_MEMLEN - sizeof(XmlMem_s);
        pnew->offset = sizeof(XmlEleInt_s);
        pnew->pdata = (char* )(pnew + 1);
        pmem->pnext = pnew;
        pret = (XmlEleInt_s* )(pnew->pdata);
    }

    /*
    pret->ptag = pret->tagbuf;
    pret->tagbflen = WX_DFTAGLEN;
    pret->pcontent = pret->conbuf;
    pret->conbflen = WX_DFCONLEN;
    */

    return pret;
}

static int xml_parputnewchar( XmlInt_s* pxml, char** ppdata, int* datalen, int* buflen, char c )
{
    XmlMem_s* pmem = NULL;
    XmlMem_s* pnew = NULL;
    char* pbuff = NULL;

    pmem = pxml->pmem;

    while( pmem->pnext )
        pmem = pmem->pnext;

    if( *datalen + 1 >= WX_MEMLEN - sizeof(XmlMem_s) )
    {
        /* ������ȳ���һ���ڴ�飬���ǵ��������ڴ������� */
        if( *ppdata == NULL )
        {
            /* Unlikely */
            assert(0);
            return -1;
        }
        if( *datalen + 1 + 1 > *buflen )    //datalen + this char + zero terminal
        {
            if( (pbuff = malloc( *datalen + 1 + WX_MEMLEN )) == NULL )
                return -1;
            memset( pbuff, 0, *datalen + 1 + WX_MEMLEN );
            memcpy( pbuff, *ppdata, *datalen );
            if( *buflen )
            {
                malloc( *ppdata );
            }
            else
            {
                pmem->offset -= *datalen + 1;
                memset( (pmem->pdata + pmem->offset), 0, (pmem->len - pmem->offset) );
            }
            *ppdata = pbuff;
            *buflen = *datalen + 1 + WX_MEMLEN;
        }
        (*ppdata)[*datalen] = c;
        *datalen = *datalen + 1;

        return 0;
    }
    
    /* һ����ʼд�����ݣ�һ��������д�룬�����ڿ�ʼԤ����0������ */
    if( *ppdata == NULL )
    {
        *ppdata = pmem->pdata + pmem->offset;
        pmem->offset ++;
    }
    else
    {
        /* ���ָ����Ч�ԣ����ǲ�֧�ֶϿ���content�ĵ�������������ĵ����ڴ˴�����ʧ�� 
          <tagone>
             contentone
             <tagtwo></tagtwo>
             contenttwo
          </tagone>
        */
        if( *ppdata != pmem->pdata + pmem->offset - *datalen - 1 )
        {
            assert(0);
            return -1;
        }
    }

    if( pmem->offset + 1 >= pmem->len ) //remain zero terminal
    {
        if( (pnew = malloc( WX_MEMLEN )) == NULL )
            return -1;
        memset( pnew, 0, WX_MEMLEN );
        
        pnew->len = WX_MEMLEN - sizeof(XmlMem_s);
        pnew->offset = 1;
        pnew->pdata = (char* )(pnew + 1);
        pmem->pnext = pnew;
        if( *datalen > 0 )
        {
            memcpy( pnew->pdata, *ppdata, *datalen );
            pnew->offset += *datalen;
        }
        *ppdata = pnew->pdata;
        pmem = pnew;
    }

    pmem->pdata[pmem->offset - 1] = c;
    *datalen = *datalen + 1;
    pmem->offset ++;

    return 0;
}

static int xml_docfsminit( char* ptoken, XmlInt_s* pxml )
{
    if( (ptoken[0] == 0) && xml_ifnullchar(ptoken[1]) )
        //jump space, \r, \n or tab
        return 0;

    if( (ptoken[0] == 1) && (ptoken[1] == WX_PROBG) )
    {
        pxml->docstate = WX_DSPROLOG;
        return 0;
    }

    pxml->parerr = WX_EXMLINVALID;
    return -1;
}

static int xml_parversion( XmlMem_s* pdata, XmlInt_s* pxml )
{
    char ptoken[2];

    while( xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
    {
        if( ptoken[0] == 1 )
            return -1;
        if( ptoken[1] == '.' )
            break;
        if( (ptoken[1] < '0') || (ptoken[1] > '9') )
            return -1;
        pxml->mainver = (unsigned short)(pxml->mainver * 10 + ptoken[1] - '0');
    }

    while( xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
    {
        if( ptoken[0] == 1 )
            return -1;
        if( (ptoken[1] == '\'') || (ptoken[1] == '\"') )
            break;
        if( (ptoken[1] < '0') || (ptoken[1] > '9') )
            return -1;
        pxml->slavever = (unsigned short)(pxml->slavever * 10 + ptoken[1] - '0');
    }
/*
    if( xml_checkfollow( pdata, " " ) >= 0 )
        return 0;

    return -1;
*/
    return 0;
}

static int xml_parsd( XmlMem_s* pdata, XmlInt_s* pxml )
{
    if( xml_checkfollow( pdata, "yes" ) >= 0 )
        pxml->ifsd = 1;
    else if( xml_checkfollow( pdata, "no" ) >= 0 )
        pxml->ifsd = 0;
    else
        return -1;
/*
    if( (xml_checkfollow( pdata, "\' " ) >= 0) || (xml_checkfollow( pdata, "\" " ) >= 0) )
        return 0;
*/
    if( (xml_checkfollow( pdata, "\'" ) >= 0) || (xml_checkfollow( pdata, "\"" ) >= 0) )
        return 0;
    return -1;
}

static int xml_parencoding( XmlMem_s* pdata, XmlInt_s* pxml )
{
    char ptoken[2];

    if( (xml_checkfollow( pdata, "UTF-8" ) >= 0)
        && ((xml_checkfollow( pdata, "\'" ) >= 0) || (xml_checkfollow( pdata, "\"" ) >= 0)) )
    {
        pxml->encoding = WX_CUTF8;
    }
    else 
    {
        pxml->encoding = WX_CUNKNOW;
        while( xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
        {
            /* Unknow attribute, jump upto space */
            if( ptoken[0] == 1 )
                return -1;
            if( (ptoken[1] == ' ') )
                break;
        }
    }

    return 0;
}


static int xml_parunknow( XmlMem_s* pdata, XmlInt_s* pxml )
{
    char ptoken[2];
    int ret = 0;

    while( (ret = xml_getnexttoken( pxml, pdata, ptoken )) >= 0 )
    {
        /* Unknow attribute, jump upto space */
        if( ptoken[0] == 1 )
            return -1;
        if( (ptoken[1] == ' ') )
            break;
    }
    
    if( ret >= 0 )
        return 0;

    return -1;
}

static int xml_docfsmprolog( char* ptoken, XmlMem_s* pdata, XmlInt_s* pxml )
{
    /* XML prolog parse fsm */
#define WX_PROSINIT        0
#define WX_PROSVERSION    1
#define WX_PROSVERSIONEQ    2
#define WX_PROSSD            3
#define WX_PROSSDEQ        4
#define WX_PROENCODING    5
#define WX_PROENCODINGEQ    6

#define WX_PROSUNKNOW        7
#define WX_PROSUNKNOWEQ    8
    
    int prostate = WX_PROSINIT, ret = 0;

    /* �ĵ�����ͷ����ض����пո���˱��������������token�ܿ����ǿո����ٲ������κ���������֮һ */
    while( xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
    {
        if( (ptoken[0] == 1) )
        {
            if( ptoken[1] == WX_PIEND )
            {
                if( (pxml->ifsd == 0) || (pxml->mainver != 1) || (pxml->slavever != 0) || (pxml->encoding != WX_CUTF8) )
                {
                    /* Unsupported document attribute */
                    pxml->parerr = WX_EXMLINVALID;
                    return -1;
                }
                pxml->docstate = WX_DSELEMENT;
                return 0;
            }
            else
            {
                /* Unsupported document prolog */
                pxml->parerr = WX_EXMLINVALID;
                return -1;
            }
        }

        switch( prostate )
        {
        case WX_PROSINIT:
            if( xml_ifnullchar(ptoken[1]) )
                break;
            if( (ptoken[1] == 'v') && (xml_checkfollow( pdata, "ersion") >= 0) )
                prostate = WX_PROSVERSION;
            else if( (ptoken[1] == 's') && (xml_checkfollow( pdata, "tandalone") >= 0) )
                prostate = WX_PROSSD;
            else if( (ptoken[1] == 'e') && (xml_checkfollow( pdata, "ncoding") >= 0) )
                prostate = WX_PROENCODING;
            else
                prostate = WX_PROSUNKNOW;
            break;

        case WX_PROSVERSION:
        case WX_PROSSD:
        case WX_PROENCODING:
        case WX_PROSUNKNOW:
            if( xml_ifnullchar(ptoken[1]) )
                break;
            if( ptoken[1] == '=' )
            {
                if( prostate == WX_PROSVERSION )
                    prostate = WX_PROSVERSIONEQ;
                else if( prostate == WX_PROSSD )
                    prostate = WX_PROSSDEQ;
                else if( prostate == WX_PROENCODING )
                    prostate = WX_PROENCODINGEQ;
                else
                    prostate = WX_PROSUNKNOWEQ;
            }
            else
            {
                if( prostate != WX_PROSUNKNOW )
                {
                    pxml->parerr = WX_EXMLINVALID;
                    return -1;
                }
            }
            break;
        case WX_PROSVERSIONEQ:
        case WX_PROSSDEQ:
        case WX_PROENCODINGEQ:
        case WX_PROSUNKNOWEQ:
            if( xml_ifnullchar(ptoken[1]) )
                break;
            if( (ptoken[1] == '\'') || (ptoken[1] == '\"') )
            {
                if( prostate == WX_PROSVERSIONEQ )
                    ret = xml_parversion(pdata, pxml);
                else if( prostate == WX_PROSSDEQ )
                    ret = xml_parsd(pdata, pxml);
                else if( prostate == WX_PROENCODINGEQ )
                    ret = xml_parencoding(pdata, pxml);
                else
                    ret = xml_parunknow(pdata, pxml);

                if( ret < 0 )
                {
                    pxml->parerr = WX_EXMLINVALID;
                    return -1;
                }
                prostate = WX_PROSINIT;
            }
            else
            {
                pxml->parerr = WX_EXMLINVALID;
                return -1;
            }
            break;
        default:
            assert( 0 );
            break;
        }
    }

    /* �ĵ��������ȡ���󣬲���δ����PIEND */
    pxml->parerr = WX_EXMLINVALID;
    return -1;
}

static int xml_docfsmelement( char* ptoken, XmlInt_s* pxml )
{
    if( (ptoken[0] == 1) )
    {
        if( ptoken[1] == WX_TAGBG )
        {
            /* create root element */
            pxml->pelement = xml_pargetnewele( pxml );
            pxml->pelement->pparent = NULL;
            pxml->pparele = pxml->pelement;

            pxml->docstate = WX_DSELETAG;
            return 0;
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( (ptoken[1] != ' ') && (ptoken[1] != '\r') && (ptoken[1] != '\n') && (ptoken[1] != 0x09) )
        {
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }

        return 0;
    }

    return -1;
}

static int xml_docfsmeletag( char* ptoken, XmlInt_s* pxml )
{
    if( (ptoken[0] == 1) )
    {
        if( (ptoken[1] == WX_TAGEND) || (ptoken[1] == WX_NULLELEEND) )
        {
            if( pxml->pparele->taglen == 0 )
            {
                /* no tag, error */
                pxml->parerr = WX_EXMLINVALID;
                return -1;
            }
            if( ptoken[1] == WX_TAGEND )
            {
                pxml->docstate = WX_DSELECONTENT;
                return 0;
            }
            else
            {
                pxml->pparele = pxml->pparele->pparent;
                if( pxml->pparele == NULL )
                {
                    pxml->docstate = WX_DSINIT;
                    return 1;
                }
                else
                {
                    pxml->docstate = WX_DSELECONTENT;
                    return 0;
                }
            }
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( pxml->pparele->ptag && (ptoken[1] == ' ') )
        {
            pxml->docstate = WX_DSELEATTR;
            return 0;
        }

        if( xml_parputnewchar( pxml, &(pxml->pparele->ptag), &(pxml->pparele->taglen), &(pxml->pparele->tagbflen), ptoken[1] ) < 0 )
        {
            pxml->parerr = WX_ERESOURCE;
            return -1;
        }
        /*
        if( pxml->pparele->taglen + 1 + 1 >= pxml->pparele->tagbflen )
        {
            char* pbuff = NULL;
            if( (pbuff = malloc( pxml->pparele->tagbflen + WX_DFNEWLEN )) == NULL )
            {
                pxml->parerr = WX_ERESOURCE;
                return -1;
            }
            memset( pbuff, 0, pxml->pparele->tagbflen + WX_DFNEWLEN );
            memcpy( pbuff, pxml->pparele->ptag, pxml->pparele->taglen );
            if( pxml->pparele->tagbflen != WX_DFTAGLEN )
                malloc( pxml->pparele->ptag );
            pxml->pparele->ptag = pbuff;
            pxml->pparele->tagbflen += WX_DFNEWLEN;
        }
        pxml->pparele->ptag[pxml->pparele->taglen ++] = ptoken[1];
        */
        return 0;
    }

    return -1;
}

static int xml_docfsmeleattr( char* ptoken, XmlInt_s* pxml )
{
    if( (ptoken[0] == 1) )
    {
        if( (ptoken[1] == WX_TAGEND) || (ptoken[1] == WX_NULLELEEND) )
        {
            if( ptoken[1] == WX_TAGEND )
            {
                pxml->docstate = WX_DSELECONTENT;
                return 0;
            }
            else
            {
                pxml->pparele = pxml->pparele->pparent;
                if( pxml->pparele == NULL )
                {
                    pxml->docstate = WX_DSINIT;
                    return 1;
                }
                else
                {
                    pxml->docstate = WX_DSELECONTENT;
                    return 0;
                }
            }
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }

    }

    /* Jump all char */
    return 0;
}

static int xml_docfsmelecontent( char* ptoken, XmlMem_s* pdata, XmlInt_s* pxml )
{
    XmlEleInt_s* pele = NULL;
    XmlEleInt_s* ptmp = NULL;
    static int slen = 0;    //space len

    if( (ptoken[0] == 1) )
    {
        slen = 0;
        if( ptoken[1] == WX_TAGBG )
        {
            if( (pele = xml_pargetnewele( pxml )) == NULL )
            {
                pxml->parerr = WX_ERESOURCE;
                return -1;
            }
            pele->pparent = pxml->pparele;
            if( pxml->pparele->pchild == NULL )
                pxml->pparele->pchild = pele;
            else
            {
                ptmp = pxml->pparele->pchild;
                while( ptmp->pbrother != NULL )
                    ptmp = ptmp->pbrother;
                ptmp->pbrother = pele;
            }
            pxml->pparele = pele;
            pxml->docstate = WX_DSELETAG;

            return 0;
        }
        else if( ptoken[1] == WX_TAGENDBG )
        {
            pxml->docstate = WX_DSELETAGEND;

            return 0;
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( (pxml->pparele->conlen == 0) && xml_ifnullchar(ptoken[1]) )
        {
            slen ++;
            return 0;
        }
        if( slen )
        {
            slen += 1; //add this char offset
            xml_pargetbackoff( pdata, slen );
            while( (slen --) )
            {
                char pt[2];
                pt[0] = 0;
                pt[1] = 0;
                xml_getnexttoken( pxml, pdata, pt );
                if( slen )
                {
#if DEBUG
                    if( (pt[0] != 0) || (xml_ifnullchar(pt[1]) == 0) )
                    {
                        fail( "WK XML get data error" );
                    }
#endif
                    if( xml_parputnewchar( pxml, &(pxml->pparele->pcontent), &(pxml->pparele->conlen), &(pxml->pparele->conbflen), pt[1] ) < 0 )
                    {
                        pxml->parerr = WX_ERESOURCE;
                        return -1;
                    }
                }
            }
        }

        if( xml_parputnewchar( pxml, &(pxml->pparele->pcontent), &(pxml->pparele->conlen), &(pxml->pparele->conbflen), ptoken[1] ) < 0 )
        {
            pxml->parerr = WX_ERESOURCE;
            return -1;
        }
/*
        if( pxml->pparele->conlen + 1 + 1 >= pxml->pparele->conbflen )
        {
            char* pbuff = NULL;
            if( (pbuff = malloc( pxml->pparele->conbflen + WX_DFNEWLEN )) == NULL )
            {
                pxml->parerr = WX_ERESOURCE;
                return -1;
            }
            memset( pbuff, 0, pxml->pparele->conbflen + WX_DFNEWLEN );
            memcpy( pbuff, pxml->pparele->pcontent, pxml->pparele->conlen );
            if( pxml->pparele->conbflen != WX_DFCONLEN )
                malloc( pxml->pparele->pcontent );
            pxml->pparele->pcontent = pbuff;
            pxml->pparele->conbflen += WX_DFNEWLEN;
        }
        pxml->pparele->pcontent[pxml->pparele->conlen ++] = ptoken[1];
*/
    }

    return 0;
}

static int xml_docfsmeletagend( char* ptoken, XmlInt_s* pxml )
{
    if( ptoken[0] == 1 )
    {
        if( ptoken[1] == WX_TAGEND )
        {
            
            if( pxml->pparele->checklen == pxml->pparele->taglen )
            {
                if( (pxml->pparele = pxml->pparele->pparent) == NULL )
                {
                    pxml->docstate = WX_DSINIT;
                    return 1;
                }
                else
                {
                    pxml->docstate = WX_DSELECONTENT;
                    return 0;
                }
            }
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( pxml->pparele->ptag[pxml->pparele->checklen++] != ptoken[1] )
        {
            /* Tag not same */
            pxml->parerr = WX_EXMLINVALID;
            return -1;
        }
    }

    return 0;
}

//xml parse fsm interface
static int xml_parse( XmlMem_s* pdata, XmlInt_s** ppxml )
{
    char token[2];
    int ret = 0;
    XmlInt_s* pxml = NULL;
    //XmlEleInt_s* pele = NULL;

    if( (pxml = xml_allocxml()) == NULL )
        return WX_ERESOURCE;

    //pele = pxml->pelement;

    while( xml_getnexttoken( pxml, pdata, token ) >= 0 )
    {
        if( (token[0] == 1) && (token[1] == WX_NA) )
            continue;
        
        if( !(pxml->ifcommand) )
        {
            if( (token[0] == 1) && (token[1] == WX_COMBG) )
                pxml->ifcommand = 1;
        }

        if( pxml->ifcommand )
        {
            if( (token[0] == 1) && (token[1] == WX_COMEND) )
                pxml->ifcommand = 0;
            continue;
        }

        switch( pxml->docstate )
        {
			case WX_DSINIT:
				ret = xml_docfsminit( token, pxml );
				break;
			case WX_DSPROLOG:    
				ret = xml_docfsmprolog( token, pdata, pxml );
				break;
			case WX_DSELEMENT:    
				ret = xml_docfsmelement( token, pxml );
				break;
			case WX_DSELETAG:
				ret = xml_docfsmeletag( token, pxml );
				break;
			case WX_DSELEATTR:    
				ret = xml_docfsmeleattr( token, pxml );
				break;
			case WX_DSELECONTENT:    
				ret = xml_docfsmelecontent( token, pdata, pxml );
				break;
			case WX_DSELETAGEND:    
				ret = xml_docfsmeletagend( token, pxml );
				break;
			default:
				/* FSM error, unknow */
				assert( 0 );
				pxml->parerr = WX_EERROR;
				ret = -1;
        }

        if( ret == -1 )
        {
            ret = pxml->parerr;
            xml_free( pxml );
            return ret;
        }
        if( ret == 1 )
            break;
    }

    if( pxml->pparele == NULL )
    {
        *ppxml = pxml;
        return 0;
    }

    xml_free( pxml );
    return WX_EXMLINVALID;
}

/******************************************************************************************\
XML generate function group:
static int xml_genprolog()
    Write fixed string into document ------ <?xml version="1.0" standalone="no" ?>\n
static int xml_genbgtag( XmlElement_s* pele )
    Write begin tag into document ----- <tagname>\n
static int xml_gencontent( XmlElement_s* pele )
    Write content into document ----- content\n
static int xml_genendtag( XmlElement_s* pele )
    Write end tag into document ----- </tagname>\n
static int xml_generate( XmlElement_s* pele )
    XML generate internal interface
\******************************************************************************************/
static int xml_genpredfstr( char* pstr, XmlGen_s* pgen )
{
    int slen = 0;
    
    slen = strlen(pstr);
    if( pgen->pgoff + slen <= pgen->pglen )
        memcpy( pgen->pgbuff + pgen->pgoff, pstr, slen );
    pgen->pgoff += slen;

    return slen;
}

static int xml_geninputdata( char* pdata, int len, XmlGen_s* pgen )
{
    int oldoff = pgen->pgoff, i = 0;

    while( i < len )
    {
        switch( pdata[i] )
        {
			case '&':
				if( xml_genpredfstr(WX_CAMP, pgen) < 0 )
					return -1;
				break;
				
			case '<':
				if( xml_genpredfstr(WX_CLT, pgen) < 0 )
					return -1;
				break;
				
			case '>':
				if( xml_genpredfstr(WX_CGT, pgen) < 0 )
					return -1;
				break;
				
			case '\'':
				if( xml_genpredfstr(WX_CAPOS, pgen) < 0 )
					return -1;
				break;
				
			case '\"':
				if( xml_genpredfstr(WX_CQUOT, pgen) < 0 )
					return -1;
				break;
				
			default:
				if( pgen->pgoff + 1 <= pgen->pglen )
					pgen->pgbuff[pgen->pgoff] = pdata[i];
				pgen->pgoff++;
				break;
        }
        i++;
    }

    return (pgen->pgoff - oldoff);
}

static int xml_geninputrawchar( char raw, XmlGen_s* pgen )
{
    if( pgen->pgoff + 1 <= pgen->pglen )
        pgen->pgbuff[pgen->pgoff] = raw;
    pgen->pgoff++;
    
    return 0;
}

static int xml_genprolog( XmlGen_s* pgen )
{
    if( pgen->pgoff + (int)strlen(WX_DFPROLOG) + 1 > pgen->pglen )
        pgen->pgoff += strlen(WX_DFPROLOG) + 1;
    else
    {
        memcpy( pgen->pgbuff, WX_DFPROLOG, strlen(WX_DFPROLOG) );
        pgen->pgoff += strlen(WX_DFPROLOG);
        pgen->pgbuff[pgen->pgoff] = 0x0a;
        pgen->pgoff ++;
    }

    return (strlen(WX_DFPROLOG) + 1);
}

static int xml_genbgtag( XmlElement_s* pele, XmlGen_s* pgen )
{
    xml_geninputrawchar( '<', pgen );

    if( xml_geninputdata(pele->ptag, pele->taglen, pgen) < 0 )
        return -1;

    xml_geninputrawchar( '>', pgen );
    
    return 0;
}

static int xml_gencontent( XmlElement_s* pele, XmlGen_s* pgen )
{
    if( xml_geninputdata(pele->pcontent, pele->conlen, pgen) < 0 )
        return -1;

    return 0;
}

static int xml_genendtag( XmlElement_s* pele, XmlGen_s* pgen )
{

    xml_geninputrawchar( '<', pgen );
    xml_geninputrawchar( '/', pgen );

    if( xml_geninputdata(pele->ptag, pele->taglen, pgen) < 0 )
        return -1;

    xml_geninputrawchar( '>', pgen );
    
    return 0;
}

//XML Generate internal function
static int xml_generate( XmlElement_s* pele, XmlGen_s* pgen )
{
    while( pele )
    {
        if( (pele->ptag == NULL) || (strlen(pele->ptag) == 0) )
        {
            pgen->generr = WX_ESTRINVALID;
            return -1;
        }
        
        if( xml_genbgtag( pele, pgen ) < 0 )
            return -1;
        
        if( pele->pchild )
        {
            if( xml_geninputdata( "\n", 1, pgen ) < 0 )
                return -1;
            if( xml_generate( pele->pchild, pgen ) < 0 )
                return -1;
        }
        
        if( pele->conlen > 0) 
        {
            if( xml_gencontent( pele, pgen ) < 0 )
                return -1;
        }
        
        if( xml_genendtag( pele, pgen ) < 0 )
            return -1;
        if( xml_geninputdata( "\n", 1, pgen ) < 0 )
            return -1;
        /*
        if( pele->pbrother )
        {
            if( xml_generate( pele->pbrother, pgen ) < 0 )
                return -1;
        }
        */
        pele = pele->pbrother;
    }
    
    
    return 0;
}

/******************************************************************************************\
XML generate function group ended
\******************************************************************************************/

/**********************************************************************************\
����ԭ�ͣ�int XML_Generate(Xml_s* pXml, char* pSource, int* pSlen )
�������ܣ�
        ��������������һ��XML�ļ��Ľ�����
����˵����
        ��������    ��������    ��������    ��ע
        Xml            int            XML������    
        pSource        char*        �������    
        pSlen        int*        �����������    
��������ֵ��
        �ɹ�����д���ļ����ȣ�ʧ�ܷ��ظ�ֵ��
        ʧ�ܷ���ֵ��һ�������롣
ʵ��������
        XML���ɾ���������ã�һ�������������أ����ɽ�������ɿ����û���Ҫ���ٱ���������´���������д���
        ����������ڴ治�㷵�أ���Ҫ���ڴ泤�Ȼᱣ���ڲ���pSlen�з��ظ�Ӧ�ã�Ӧ�ÿ��԰��պ���������Ϣ������
        ���ڴ���������xml�ĵ���
\**********************************************************************************/
int XML_Generate( Xml_s* pXml, char* pSource, int* pSlen )
{
    XmlGen_s XmlGen;

    if( (pXml == NULL) || (pSlen == NULL) )
        return WX_EPARA;

    if( (pXml->encoding != WX_CUTF8) || !(pXml->ifsd) || (pXml->mainver != 1) || (pXml->slavever != 0) )
        return WX_ESTRINVALID;

    XmlGen.pgbuff = pSource;
    XmlGen.pglen = *pSlen;
    XmlGen.pgoff = 0;
    
    /* Gen prolog first */
    xml_genprolog( &XmlGen );

    if( pXml->pelement )
        xml_generate( pXml->pelement, &XmlGen );
        
    *pSlen = XmlGen.pgoff;
    if( XmlGen.pgoff > XmlGen.pglen )
    {
        return WX_EBUFFSHORT;
    }

    return XmlGen.pgoff;
}

/**********************************************************************************\
����ԭ�ͣ�int XML_Parse( XmlMem_s* pData, Xml_s** ppXml )
�������ܣ�
        ��������������һ��XML�ļ��Ľ�����
����˵����
        ��������    ��������            ��������    ��ע
        pData        XmlMem_s *    ����XML����    
        ppXml        Xml_s**        XML�����ش����    ���ṹ�ڴ��ɽ��������ɣ�Ϊֻ���ṹ��
                                        Ӧ�ó��򲻿��Լ���ͼ�޸ĺ��ͷű��ṹ��
��������ֵ��
        �ɹ����ؽ����ļ����ȣ�ʧ�ܷ��ظ�ֵ��
        ʧ�ܷ���ֵ��һ�������롣
ʵ��������
    ������Ϊͬ���������û�һ�ν�������XML�ĵ����룬��������ʱ��XML������������������ظ��û���
        ������ֱ�ӵ��ý���״̬�������ļ�������
    XML��������������ã�һ�������������أ�������������ɿ����û���Ҫ���ٱ���������´���������н�����
    ��������õĽ��ΪXML����ģ���Զ����ɵģ��û���Ҫ���ý������ֻ�����ݶԴ������ɶ�֮����д�룬�޸ĺ�ɾ��������
\**********************************************************************************/
int XML_Parse( XmlMem_s* pData, Xml_s** ppXml )
{
    XmlMem_s* pdata = pData;
    int ret = 0;

    if( pData == NULL )
        return WX_EPARA;
    
    /* Set all memblock offset as zero */
    while( pdata )
    {
        pdata->offset = 0;
        pdata = pdata->pnext;
    }

    ret = xml_parse( pData, (XmlInt_s**)ppXml );

    if( ret >= 0 )
    {
        return 0;
    }
    else
    {
        *ppXml = NULL;
        return ret;
    }

    return 0;
}

/**********************************************************************************\
����ԭ�ͣ�int XML_Free(Xml_s* pXml )
�������ܣ�
        ��������������XML���ݽṹ���ڴ��ͷŹ�����
����˵����
        ��������    ��������        ��������        ��ע
        pXml        Xml_s*    XML�����ش����    

��������ֵ��
        �ɹ�����0��ʧ�ܷ��ظ�ֵ��
        ʧ�ܷ���ֵ��һ�������롣
ʵ��������
        �����û�ͨ��XML_Parse������õĽṹ���ڴ�ΪXMLģ������ģ���˱���ͨ���������ͷŸ����ݽṹ���Ա�����ڴ档
\**********************************************************************************/
int XML_Free(Xml_s* pXml )
{
    if( pXml == NULL )
        return WX_EPARA;

    xml_free( (XmlInt_s*)pXml );

    return 0;
}

