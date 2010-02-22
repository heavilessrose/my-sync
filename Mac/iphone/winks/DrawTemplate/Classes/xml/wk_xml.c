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
*Id: wk_xml.c,v 1.00 
**************************************************************************    
* File Description
* ----------------
* Color call XML Parser function. 
**************************************************************************/


#ifndef WK_XML_SINGLE
#include "wk_public.h"
//#include "wk_osal.h"
#endif
#include "wk_xml.h"
#include "wk_iphoneccshw.h"
#include <stdlib.h>
#include <string.h>

#define WINKS_WX_CAMP    (char* )"&amp;"
#define WINKS_WX_CLT    (char* )"&lt;"
#define WINKS_WX_CGT    (char* )"&gt;"
#define WINKS_WX_CAPOS    (char* )"&apos;"
#define WINKS_WX_CQUOT    (char* )"&quot;"

//�ַ�״̬����������
#define WINKS_WX_TAGBG        1
#define WINKS_WX_TAGEND        2
#define WINKS_WX_TAGENDBG    3
#define WINKS_WX_NULLELEEND    4
#define WINKS_WX_PIBG        5
#define WINKS_WX_PROBG        6
#define WINKS_WX_PIEND        7
#define WINKS_WX_COMBG        8
#define WINKS_WX_COMEND        9
#define WINKS_WX_CDATABG    10
#define WINKS_WX_CDATAEND    11
#define WINKS_WX_NA            20

//�ַ�״̬��״̬��
#define WINKS_WX_CSIDLE        0
#define WINKS_WX_CSCDATA    1

//�ĵ�����״̬��״̬��
#define WINKS_WX_DSINIT        0
#define WINKS_WX_DSPROLOG    1
#define WINKS_WX_DSELEMENT    2
#define WINKS_WX_DSELETAG    3
#define WINKS_WX_DSELEATTR    4
#define WINKS_WX_DSELECONTENT    5
#define WINKS_WX_DSELETAGEND    6

//�ڴ�����볤��
#define WINKS_WX_MEMLEN        2048

#define WINKS_WX_DFPROLOG    ((char* )"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>")

/*
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
#define WINKS_XMLGETGLOBALVAR get_module_glob(WK_MODULE_XML)
#else
static Winks_XmlGlobal_s XMLGlobalCB;
#define WINKS_HTTPGETGLOBALVAR (&XMLGlobalCB)
#endif
*/

static int Winks_xml_freeele( Winks_XmlEleInt_s* pele )
{
    if( pele->pchild )
        Winks_xml_freeele( pele->pchild );

    if( pele->pbrother )
        Winks_xml_freeele( pele->pbrother );

    if( pele->tagbflen != 0 )
        Winks_salFree( pele->ptag );
    if( pele->conbflen != 0 )
        Winks_salFree( pele->pcontent );

    return 0;
}


int Winks_xml_free( Winks_XmlInt_s* pxml )
{
    Winks_XmlMem_s* pmem = NULL;

    if( pxml->pelement )
        Winks_xml_freeele( pxml->pelement );

    while( pxml->pmem->pnext )
    {
        pmem = pxml->pmem->pnext;
        pxml->pmem->pnext = pmem->pnext;

        Winks_salFree( pmem );
    }

    Winks_salFree( pxml );

    return 0;
}

static int Winks_xml_ifnullchar( char c )
{
    if( (c == ' ') || (c == 0x0d) || (c == 0x0a) || (c == 0x09) )
        return 1;

    return 0;
}

static int Winks_xml_pargetbackoff( Winks_XmlMem_s* pdata, int len )
{
    Winks_XmlMem_s* plast = NULL;

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
        if( len > pdata->offset + plast->offset )
        {
            /* ��֧�ֻ��˳���һ�����ݿ鳤�ȵ�λ�� */
            Winks_assert(0);
            return -1;
        }
        len -= pdata->offset;
        pdata->offset = 0;
        
        plast->offset -= len;
        return 0;
    }

    return 0;
}

static int Winks_xml_pargetnextchar( Winks_XmlMem_s* pdata, char* pret )
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
            Winks_assert( 0 );
        }
    }

    return -1;
}

/*
static char* Winks_xml_pargetcurpoint( Winks_XmlMem_s* pdata )
{
    while( pdata->pnext )
        pdata = pdata->pnext;

    return pdata->pdata + pdata->offset;
}

static int Winks_xml_pargetdata( Winks_XmlMem_s* pdata, char* pdest, char* psource, int len )
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
                Winks_mem_cpy( pdest, ps, cpylen );
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

static int Winks_xml_checkfollow( Winks_XmlMem_s* pdata, char* follow )
{
    int i = 0;
    char ca = 0;

    while( *follow )
    {
        i++;
        if( Winks_xml_pargetnextchar( pdata, &ca ) < 0 )
        {
            if( i - 1 > 0 )
                Winks_xml_pargetbackoff( pdata, i - 1 );
            return -1;
        }
        if( ca != *follow )
        {
            Winks_xml_pargetbackoff( pdata, i );
            return -1;
        }

        follow ++;
    }

    return 0;
}

static int Winks_xml_deallt( Winks_XmlMem_s* pdata, char* ptoken )
{
    char ca = 0;

    if( Winks_xml_pargetnextchar( pdata, &ca ) < 0 )
        return WINKS_WX_EXMLINVALID;

    ptoken[0] = 1;
    switch( ca )
    {
    case '/':
        ptoken[1] = WINKS_WX_TAGENDBG;
        break;

    case '?':
        if( Winks_xml_checkfollow( pdata, "xml") >= 0 )
            ptoken[1] = WINKS_WX_PROBG;
        else
            ptoken[1] = WINKS_WX_PIBG;

        break;
        
    case '!':
        if( Winks_xml_checkfollow( pdata, "[CDATA[") >= 0 )
            ptoken[1] = WINKS_WX_CDATABG;
        else if( Winks_xml_checkfollow( pdata, "--") >= 0 )
            ptoken[1] = WINKS_WX_COMBG;
        else 
            return WINKS_WX_EXMLINVALID;
        break;

    default:
        Winks_xml_pargetbackoff( pdata, 1 );
        ptoken[1] = WINKS_WX_TAGBG;
        break;
    }

    return 0;

}
//char fsm
static int Winks_xml_getnexttoken( Winks_XmlInt_s* pxml, Winks_XmlMem_s* pdata, char* ptoken )
{
    char ca = 0; 
    int ret = 0;

    if( Winks_xml_pargetnextchar( pdata, &ca ) < 0 )
        return -1;
    
    if( pxml->charstate == WINKS_WX_CSIDLE )
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
            if( (ret = Winks_xml_deallt(pdata, ptoken)) < 0 )
            {
                pxml->parerr = ret;
                return -1;
            }
            if( (ptoken[0] == 1) && (ptoken[1] == WINKS_WX_CDATABG) )
            {
                pxml->charstate = WINKS_WX_CDATABG;
                ptoken[1] = WINKS_WX_NA;
            }
            break;
            
        case '/':
            if( Winks_xml_checkfollow( pdata, ">") >= 0 )
            {
                ptoken[0] = 1;
                ptoken[1] = WINKS_WX_NULLELEEND;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = '/';
            }
            break;

        case '-':
            if( Winks_xml_checkfollow( pdata, "->") >= 0 )
            {
                ptoken[0] = 1;
                ptoken[1] = WINKS_WX_COMEND;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = '-';
            }
            break;
        case '>':
            ptoken[0] = 1;
            ptoken[1] = WINKS_WX_TAGEND;
            break;

        case '?':
            if( Winks_xml_checkfollow( pdata, ">") >= 0 )
            {
                ptoken[0] = 1;
                ptoken[1] = WINKS_WX_PIEND;
            }
            else
            {
                ptoken[0] = 0;
                ptoken[1] = '?';
            }
            break;
            
        case '&':
            if( Winks_xml_checkfollow( pdata, WINKS_WX_CAMP + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '&';
            }
            else if( Winks_xml_checkfollow( pdata, WINKS_WX_CLT + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '<';
            }
            else if( Winks_xml_checkfollow( pdata, WINKS_WX_CGT + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '>';
            }
            else if( Winks_xml_checkfollow( pdata, WINKS_WX_CAPOS + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '\'';
            }
            else if( Winks_xml_checkfollow( pdata, WINKS_WX_CQUOT + 1) >= 0 )
            {
                ptoken[0] = 0;
                ptoken[1] = '\"';
            }
            else
            {
                /* �ַ���ת�壬�ݲ�֧�� */
                Winks_assert(0);
            }
            break;

        default:
            ptoken[0] = 0;
            ptoken[1] = ca;
            break;
        }
    }
    else if( pxml->charstate == WINKS_WX_CDATABG )
    {
        if( ca == ']' )
        {
            if( Winks_xml_checkfollow( pdata, "]>") >= 0 )
            {
                pxml->charstate = WINKS_WX_CSIDLE;
                ptoken[0] = 1;
                ptoken[1] = WINKS_WX_NA;
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
        Winks_assert( 0 );
    }

    return 0;
}

static Winks_XmlInt_s* Winks_xml_allocxml()
{
    Winks_XmlInt_s* pret = NULL;

    if( (pret = Winks_salAlloc( WINKS_WX_MEMLEN )) == NULL )
        return NULL;

    memset( pret, 0, WINKS_WX_MEMLEN );
    pret->ifsd = 1;
    pret->pmem = (Winks_XmlMem_s* )(pret + 1);
    pret->pmem->pdata = (char* )(pret->pmem + 1);
    pret->pmem->len = WINKS_WX_MEMLEN - (sizeof(Winks_XmlInt_s) + sizeof(Winks_XmlMem_s));

    /*
    pret->pelement = (Winks_XmlEleInt_s* )(pret->pmem + 1);
    
    pret->pparele = pret->pelement;
    pret->pelement->ptag = pret->pelement->tagbuf;
    pret->pelement->tagbflen = WINKS_WX_DFTAGLEN;
    pret->pelement->pcontent = pret->pelement->conbuf;
    pret->pelement->conbflen = WINKS_WX_DFCONLEN;
    
    pret->pmem->offset = sizeof(Winks_XmlEleInt_s);
    */

    return pret;
}

static Winks_XmlEleInt_s* Winks_xml_pargetnewele( Winks_XmlInt_s* pxml )
{
    Winks_XmlMem_s* pmem = NULL;
    Winks_XmlMem_s* pnew = NULL;
    Winks_XmlEleInt_s* pret = NULL;

    pmem = pxml->pmem;

    while( pmem->pnext )
        pmem = pmem->pnext;

    if( (pmem->len - pmem->offset) > sizeof(Winks_XmlEleInt_s) )
    {
        /*pmem->len���ı�����sizeof(Winks_XmlEleInt_s)Ҳ���ı�����������жϳ�����Բ���Ժ�Ҳ���ᳬ��*/
        pmem->offset = (pmem->offset + 3) & 0xfffffffc;
        pmem->offset += sizeof(Winks_XmlEleInt_s);

        pret = (Winks_XmlEleInt_s* )( pmem->pdata + pmem->offset - sizeof(Winks_XmlEleInt_s) );
    }
    else
    {
        if( (pnew = Winks_salAlloc( WINKS_WX_MEMLEN )) == NULL )
            return NULL;
        memset( pnew, 0, WINKS_WX_MEMLEN );
        
        pnew->len = WINKS_WX_MEMLEN - sizeof(Winks_XmlMem_s);
        pnew->offset = sizeof(Winks_XmlEleInt_s);
        pnew->pdata = (char* )(pnew + 1);
        pmem->pnext = pnew;
        pret = (Winks_XmlEleInt_s* )(pnew->pdata);
    }

    /*
    pret->ptag = pret->tagbuf;
    pret->tagbflen = WINKS_WX_DFTAGLEN;
    pret->pcontent = pret->conbuf;
    pret->conbflen = WINKS_WX_DFCONLEN;
    */

    return pret;
}

static int Winks_xml_parputnewchar( Winks_XmlInt_s* pxml, char** ppdata, int* datalen, int* buflen, char c )
{
    Winks_XmlMem_s* pmem = NULL;
    Winks_XmlMem_s* pnew = NULL;
    char* pbuff = NULL;

    pmem = pxml->pmem;

    while( pmem->pnext )
        pmem = pmem->pnext;

    if( *datalen + 1 >= WINKS_WX_MEMLEN - sizeof(Winks_XmlMem_s) )
    {
        /* ������ȳ���һ���ڴ�飬���ǵ��������ڴ������� */
        if( *ppdata == NULL )
        {
            /* Unlikely */
            Winks_assert(0);
            return -1;
        }
        if( *datalen + 1 + 1 > *buflen )    //datalen + this char + zero terminal
        {
            if( (pbuff = Winks_salAlloc( *datalen + 1 + WINKS_WX_MEMLEN )) == NULL )
                return -1;
            memset( pbuff, 0, *datalen + 1 + WINKS_WX_MEMLEN );
            memcpy( pbuff, *ppdata, *datalen );
            if( *buflen )
            {
                Winks_salFree( *ppdata );
            }
            else
            {
                pmem->offset -= *datalen + 1;
                memset( (pmem->pdata + pmem->offset), 0, (pmem->len - pmem->offset) );
            }
            *ppdata = pbuff;
            *buflen = *datalen + 1 + WINKS_WX_MEMLEN;
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
            Winks_assert(0);
            return -1;
        }
    }

    if( pmem->offset + 1 >= pmem->len ) //remain zero terminal
    {
        if( (pnew = Winks_salAlloc( WINKS_WX_MEMLEN )) == NULL )
            return -1;
        memset( pnew, 0, WINKS_WX_MEMLEN );
        
        pnew->len = WINKS_WX_MEMLEN - sizeof(Winks_XmlMem_s);
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

static int Winks_xml_docfsminit( char* ptoken, Winks_XmlInt_s* pxml )
{
    if( (ptoken[0] == 0) && Winks_xml_ifnullchar(ptoken[1]) )
        //jump space, \r, \n or tab
        return 0;

    if( (ptoken[0] == 1) && (ptoken[1] == WINKS_WX_PROBG) )
    {
        pxml->docstate = WINKS_WX_DSPROLOG;
        return 0;
    }

    pxml->parerr = WINKS_WX_EXMLINVALID;
    return -1;
}

static int Winks_xml_parversion( Winks_XmlMem_s* pdata, Winks_XmlInt_s* pxml )
{
    char ptoken[2];

    while( Winks_xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
    {
        if( ptoken[0] == 1 )
            return -1;
        if( ptoken[1] == '.' )
            break;
        if( (ptoken[1] < '0') || (ptoken[1] > '9') )
            return -1;
        pxml->mainver = (unsigned short)(pxml->mainver * 10 + ptoken[1] - '0');
    }

    while( Winks_xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
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
    if( Winks_xml_checkfollow( pdata, " " ) >= 0 )
        return 0;

    return -1;
*/
    return 0;
}

static int Winks_xml_parsd( Winks_XmlMem_s* pdata, Winks_XmlInt_s* pxml )
{
    if( Winks_xml_checkfollow( pdata, "yes" ) >= 0 )
        pxml->ifsd = 1;
    else if( Winks_xml_checkfollow( pdata, "no" ) >= 0 )
        pxml->ifsd = 0;
    else
        return -1;
/*
    if( (Winks_xml_checkfollow( pdata, "\' " ) >= 0) || (Winks_xml_checkfollow( pdata, "\" " ) >= 0) )
        return 0;
*/
    if( (Winks_xml_checkfollow( pdata, "\'" ) >= 0) || (Winks_xml_checkfollow( pdata, "\"" ) >= 0) )
        return 0;
    return -1;
}

static int Winks_xml_parencoding( Winks_XmlMem_s* pdata, Winks_XmlInt_s* pxml )
{
    char ptoken[2];

    if( (Winks_xml_checkfollow( pdata, "UTF-8" ) >= 0)
        && ((Winks_xml_checkfollow( pdata, "\'" ) >= 0) || (Winks_xml_checkfollow( pdata, "\"" ) >= 0)) )
    {
        pxml->encoding = WINKS_WX_CUTF8;
    }
    else 
    {
        pxml->encoding = WINKS_WX_CUNKNOW;
        while( Winks_xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
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


static int Winks_xml_parunknow( Winks_XmlMem_s* pdata, Winks_XmlInt_s* pxml )
{
    char ptoken[2];
    int ret = 0;

    while( (ret = Winks_xml_getnexttoken( pxml, pdata, ptoken )) >= 0 )
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

static int Winks_xml_docfsmprolog( char* ptoken, Winks_XmlMem_s* pdata, Winks_XmlInt_s* pxml )
{
    /* XML prolog parse fsm */
#define WINKS_WX_PROSINIT        0
#define WINKS_WX_PROSVERSION    1
#define WINKS_WX_PROSVERSIONEQ    2
#define WINKS_WX_PROSSD            3
#define WINKS_WX_PROSSDEQ        4
#define WINKS_WX_PROENCODING    5
#define WINKS_WX_PROENCODINGEQ    6

#define WINKS_WX_PROSUNKNOW        7
#define WINKS_WX_PROSUNKNOWEQ    8
    
    int prostate = WINKS_WX_PROSINIT, ret = 0;

    /* �ĵ�����ͷ����ض����пո���˱��������������token�ܿ����ǿո����ٲ������κ���������֮һ */
    while( Winks_xml_getnexttoken( pxml, pdata, ptoken ) >= 0 )
    {
        if( (ptoken[0] == 1) )
        {
            if( ptoken[1] == WINKS_WX_PIEND )
            {
                if( (pxml->ifsd == 0) || (pxml->mainver != 1) || (pxml->slavever != 0) || (pxml->encoding != WINKS_WX_CUTF8) )
                {
                    /* Unsupported document attribute */
                    pxml->parerr = WINKS_WX_EXMLINVALID;
                    return -1;
                }
                pxml->docstate = WINKS_WX_DSELEMENT;
                return 0;
            }
            else
            {
                /* Unsupported document prolog */
                pxml->parerr = WINKS_WX_EXMLINVALID;
                return -1;
            }
        }

        switch( prostate )
        {
        case WINKS_WX_PROSINIT:
            if( Winks_xml_ifnullchar(ptoken[1]) )
                break;
            if( (ptoken[1] == 'v') && (Winks_xml_checkfollow( pdata, "ersion") >= 0) )
                prostate = WINKS_WX_PROSVERSION;
            else if( (ptoken[1] == 's') && (Winks_xml_checkfollow( pdata, "tandalone") >= 0) )
                prostate = WINKS_WX_PROSSD;
            else if( (ptoken[1] == 'e') && (Winks_xml_checkfollow( pdata, "ncoding") >= 0) )
                prostate = WINKS_WX_PROENCODING;
            else
                prostate = WINKS_WX_PROSUNKNOW;
            break;

        case WINKS_WX_PROSVERSION:
        case WINKS_WX_PROSSD:
        case WINKS_WX_PROENCODING:
        case WINKS_WX_PROSUNKNOW:
            if( Winks_xml_ifnullchar(ptoken[1]) )
                break;
            if( ptoken[1] == '=' )
            {
                if( prostate == WINKS_WX_PROSVERSION )
                    prostate = WINKS_WX_PROSVERSIONEQ;
                else if( prostate == WINKS_WX_PROSSD )
                    prostate = WINKS_WX_PROSSDEQ;
                else if( prostate == WINKS_WX_PROENCODING )
                    prostate = WINKS_WX_PROENCODINGEQ;
                else
                    prostate = WINKS_WX_PROSUNKNOWEQ;
            }
            else
            {
                if( prostate != WINKS_WX_PROSUNKNOW )
                {
                    pxml->parerr = WINKS_WX_EXMLINVALID;
                    return -1;
                }
            }
            break;
        case WINKS_WX_PROSVERSIONEQ:
        case WINKS_WX_PROSSDEQ:
        case WINKS_WX_PROENCODINGEQ:
        case WINKS_WX_PROSUNKNOWEQ:
            if( Winks_xml_ifnullchar(ptoken[1]) )
                break;
            if( (ptoken[1] == '\'') || (ptoken[1] == '\"') )
            {
                if( prostate == WINKS_WX_PROSVERSIONEQ )
                    ret = Winks_xml_parversion(pdata, pxml);
                else if( prostate == WINKS_WX_PROSSDEQ )
                    ret = Winks_xml_parsd(pdata, pxml);
                else if( prostate == WINKS_WX_PROENCODINGEQ )
                    ret = Winks_xml_parencoding(pdata, pxml);
                else
                    ret = Winks_xml_parunknow(pdata, pxml);

                if( ret < 0 )
                {
                    pxml->parerr = WINKS_WX_EXMLINVALID;
                    return -1;
                }
                prostate = WINKS_WX_PROSINIT;
            }
            else
            {
                pxml->parerr = WINKS_WX_EXMLINVALID;
                return -1;
            }
            break;
        default:
            Winks_assert( 0 );
            break;
        }
    }

    /* �ĵ��������ȡ���󣬲���δ����PIEND */
    pxml->parerr = WINKS_WX_EXMLINVALID;
    return -1;
}

static int Winks_xml_docfsmelement( char* ptoken, Winks_XmlInt_s* pxml )
{
    if( (ptoken[0] == 1) )
    {
        if( ptoken[1] == WINKS_WX_TAGBG )
        {
            /* create root element */
            pxml->pelement = Winks_xml_pargetnewele( pxml );
            pxml->pelement->pparent = NULL;
            pxml->pparele = pxml->pelement;

            pxml->docstate = WINKS_WX_DSELETAG;
            return 0;
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( (ptoken[1] != ' ') && (ptoken[1] != '\r') && (ptoken[1] != '\n') && (ptoken[1] != 0x09) )
        {
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }

        return 0;
    }

    return -1;
}

static int Winks_xml_docfsmeletag( char* ptoken, Winks_XmlInt_s* pxml )
{
    if( (ptoken[0] == 1) )
    {
        if( (ptoken[1] == WINKS_WX_TAGEND) || (ptoken[1] == WINKS_WX_NULLELEEND) )
        {
            if( pxml->pparele->taglen == 0 )
            {
                /* no tag, error */
                pxml->parerr = WINKS_WX_EXMLINVALID;
                return -1;
            }
            if( ptoken[1] == WINKS_WX_TAGEND )
            {
                pxml->docstate = WINKS_WX_DSELECONTENT;
                return 0;
            }
            else
            {
                pxml->pparele = pxml->pparele->pparent;
                if( pxml->pparele == NULL )
                {
                    pxml->docstate = WINKS_WX_DSINIT;
                    return 1;
                }
                else
                {
                    pxml->docstate = WINKS_WX_DSELECONTENT;
                    return 0;
                }
            }
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( pxml->pparele->ptag && (ptoken[1] == ' ') )
        {
            pxml->docstate = WINKS_WX_DSELEATTR;
            return 0;
        }

        if( Winks_xml_parputnewchar( pxml, &(pxml->pparele->ptag), &(pxml->pparele->taglen), &(pxml->pparele->tagbflen), ptoken[1] ) < 0 )
        {
            pxml->parerr = WINKS_WX_ERESOURCE;
            return -1;
        }
        /*
        if( pxml->pparele->taglen + 1 + 1 >= pxml->pparele->tagbflen )
        {
            char* pbuff = NULL;
            if( (pbuff = Winks_salAlloc( pxml->pparele->tagbflen + WINKS_WX_DFNEWLEN )) == NULL )
            {
                pxml->parerr = WINKS_WX_ERESOURCE;
                return -1;
            }
            Winks_mem_set( pbuff, 0, pxml->pparele->tagbflen + WINKS_WX_DFNEWLEN );
            Winks_mem_cpy( pbuff, pxml->pparele->ptag, pxml->pparele->taglen );
            if( pxml->pparele->tagbflen != WINKS_WX_DFTAGLEN )
                Winks_salFree( pxml->pparele->ptag );
            pxml->pparele->ptag = pbuff;
            pxml->pparele->tagbflen += WINKS_WX_DFNEWLEN;
        }
        pxml->pparele->ptag[pxml->pparele->taglen ++] = ptoken[1];
        */
        return 0;
    }

    return -1;
}

static int Winks_xml_docfsmeleattr( char* ptoken, Winks_XmlInt_s* pxml )
{
    if( (ptoken[0] == 1) )
    {
        if( (ptoken[1] == WINKS_WX_TAGEND) || (ptoken[1] == WINKS_WX_NULLELEEND) )
        {
            if( ptoken[1] == WINKS_WX_TAGEND )
            {
                pxml->docstate = WINKS_WX_DSELECONTENT;
                return 0;
            }
            else
            {
                pxml->pparele = pxml->pparele->pparent;
                if( pxml->pparele == NULL )
                {
                    pxml->docstate = WINKS_WX_DSINIT;
                    return 1;
                }
                else
                {
                    pxml->docstate = WINKS_WX_DSELECONTENT;
                    return 0;
                }
            }
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }

    }

    /* Jump all char */
    return 0;
}

static int Winks_xml_docfsmelecontent( char* ptoken, Winks_XmlMem_s* pdata, Winks_XmlInt_s* pxml )
{
    Winks_XmlEleInt_s* pele = NULL;
    Winks_XmlEleInt_s* ptmp = NULL;
    static int slen = 0;    //space len

    if( (ptoken[0] == 1) )
    {
        slen = 0;
        if( ptoken[1] == WINKS_WX_TAGBG )
        {
            if( (pele = Winks_xml_pargetnewele( pxml )) == NULL )
            {
                pxml->parerr = WINKS_WX_ERESOURCE;
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
            pxml->docstate = WINKS_WX_DSELETAG;

            return 0;
        }
        else if( ptoken[1] == WINKS_WX_TAGENDBG )
        {
            pxml->docstate = WINKS_WX_DSELETAGEND;

            return 0;
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( (pxml->pparele->conlen == 0) && Winks_xml_ifnullchar(ptoken[1]) )
        {
            slen ++;
            return 0;
        }
        if( slen )
        {
            slen += 1; //add this char offset
            Winks_xml_pargetbackoff( pdata, slen );
            while( (slen --) )
            {
                char pt[2];
                pt[0] = 0;
                pt[1] = 0;
                Winks_xml_getnexttoken( pxml, pdata, pt );
                if( slen )
                {
#if WINKS_DEBUG
                    if( (pt[0] != 0) || (Winks_xml_ifnullchar(pt[1]) == 0) )
                    {
                        Winks_fail( "WK XML get data error" );
                    }
#endif
                if( Winks_xml_parputnewchar( pxml, &(pxml->pparele->pcontent), &(pxml->pparele->conlen), &(pxml->pparele->conbflen), pt[1] ) < 0 )
                {
                    pxml->parerr = WINKS_WX_ERESOURCE;
                    return -1;
                }
            }
        }
        }

        if( Winks_xml_parputnewchar( pxml, &(pxml->pparele->pcontent), &(pxml->pparele->conlen), &(pxml->pparele->conbflen), ptoken[1] ) < 0 )
        {
            pxml->parerr = WINKS_WX_ERESOURCE;
            return -1;
        }
/*
        if( pxml->pparele->conlen + 1 + 1 >= pxml->pparele->conbflen )
        {
            char* pbuff = NULL;
            if( (pbuff = Winks_salAlloc( pxml->pparele->conbflen + WINKS_WX_DFNEWLEN )) == NULL )
            {
                pxml->parerr = WINKS_WX_ERESOURCE;
                return -1;
            }
            Winks_mem_set( pbuff, 0, pxml->pparele->conbflen + WINKS_WX_DFNEWLEN );
            Winks_mem_cpy( pbuff, pxml->pparele->pcontent, pxml->pparele->conlen );
            if( pxml->pparele->conbflen != WINKS_WX_DFCONLEN )
                Winks_salFree( pxml->pparele->pcontent );
            pxml->pparele->pcontent = pbuff;
            pxml->pparele->conbflen += WINKS_WX_DFNEWLEN;
        }
        pxml->pparele->pcontent[pxml->pparele->conlen ++] = ptoken[1];
*/
    }

    return 0;
}

static int Winks_xml_docfsmeletagend( char* ptoken, Winks_XmlInt_s* pxml )
{
    if( ptoken[0] == 1 )
    {
        if( ptoken[1] == WINKS_WX_TAGEND )
        {
            
            if( pxml->pparele->checklen == pxml->pparele->taglen )
            {
                if( (pxml->pparele = pxml->pparele->pparent) == NULL )
                {
                    pxml->docstate = WINKS_WX_DSINIT;
                    return 1;
                }
                else
                {
                    pxml->docstate = WINKS_WX_DSELECONTENT;
                    return 0;
                }
            }
        }
        else
        {
            /* Unsupported document */
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }
    }
    else
    {
        if( pxml->pparele->ptag[pxml->pparele->checklen++] != ptoken[1] )
        {
            /* Tag not same */
            pxml->parerr = WINKS_WX_EXMLINVALID;
            return -1;
        }
    }

    return 0;
}

//xml parse fsm interface
int Winks_xml_parse( Winks_XmlMem_s* pdata, Winks_XmlInt_s** ppxml )
{
    char token[2];
    int ret = 0;
    Winks_XmlInt_s* pxml = NULL;
    Winks_XmlEleInt_s* pele = NULL;

    if( (pxml = Winks_xml_allocxml()) == NULL )
        return WINKS_WX_ERESOURCE;

    pele = pxml->pelement;

    while( Winks_xml_getnexttoken( pxml, pdata, token ) >= 0 )
    {
        if( (token[0] == 1) && (token[1] == WINKS_WX_NA) )
            continue;
        
        if( !(pxml->ifcommand) )
        {
            if( (token[0] == 1) && (token[1] == WINKS_WX_COMBG) )
                pxml->ifcommand = 1;
        }

        if( pxml->ifcommand )
        {
            if( (token[0] == 1) && (token[1] == WINKS_WX_COMEND) )
                pxml->ifcommand = 0;
            continue;
        }

        switch( pxml->docstate )
        {
        case WINKS_WX_DSINIT:
            ret = Winks_xml_docfsminit( token, pxml );
            break;
        case WINKS_WX_DSPROLOG:    
            ret = Winks_xml_docfsmprolog( token, pdata, pxml );
            break;
        case WINKS_WX_DSELEMENT:    
            ret = Winks_xml_docfsmelement( token, pxml );
            break;
        case WINKS_WX_DSELETAG:
            ret = Winks_xml_docfsmeletag( token, pxml );
            break;
        case WINKS_WX_DSELEATTR:    
            ret = Winks_xml_docfsmeleattr( token, pxml );
            break;
        case WINKS_WX_DSELECONTENT:    
            ret = Winks_xml_docfsmelecontent( token, pdata, pxml );
            break;
        case WINKS_WX_DSELETAGEND:    
            ret = Winks_xml_docfsmeletagend( token, pxml );
            break;
        default:
            /* FSM error, unknow */
            Winks_assert( 0 );
            pxml->parerr = WINKS_WX_EERROR;
            ret = -1;
        }

        if( ret == -1 )
        {
            ret = pxml->parerr;
            Winks_xml_free( pxml );
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

    Winks_xml_free( pxml );
    return WINKS_WX_EXMLINVALID;
}

/******************************************************************************************\
XML generate function group:
static int Winks_xml_genprolog()
    Write fixed string into document ------ <?xml version="1.0" standalone="no" ?>\n
static int Winks_xml_genbgtag( Winks_XmlElement_s* pele )
    Write begin tag into document ----- <tagname>\n
static int Winks_xml_gencontent( Winks_XmlElement_s* pele )
    Write content into document ----- content\n
static int Winks_xml_genendtag( Winks_XmlElement_s* pele )
    Write end tag into document ----- </tagname>\n
static int Winks_xml_generate( Winks_XmlElement_s* pele )
    XML generate internal interface
\******************************************************************************************/
static int Winks_xml_genpredfstr( char* pstr, Winks_XmlGen_s* pgen )
{
    int slen = 0;
    
    slen = strlen(pstr);
    if( pgen->pgoff + slen <= pgen->pglen )
        memcpy( pgen->pgbuff + pgen->pgoff, pstr, slen );
    pgen->pgoff += slen;

    return slen;
}

static int Winks_xml_geninputdata( char* pdata, int len, Winks_XmlGen_s* pgen )
{
    int oldoff = pgen->pgoff, i = 0;

    while( i < len )
    {
        switch( pdata[i] )
        {
        case '&':
            if( Winks_xml_genpredfstr(WINKS_WX_CAMP, pgen) < 0 )
                return -1;
            break;

        case '<':
            if( Winks_xml_genpredfstr(WINKS_WX_CLT, pgen) < 0 )
                return -1;
            break;

        case '>':
            if( Winks_xml_genpredfstr(WINKS_WX_CGT, pgen) < 0 )
                return -1;
            break;

        case '\'':
            if( Winks_xml_genpredfstr(WINKS_WX_CAPOS, pgen) < 0 )
                return -1;
            break;

        case '\"':
            if( Winks_xml_genpredfstr(WINKS_WX_CQUOT, pgen) < 0 )
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

static int Winks_xml_geninputrawchar( char raw, Winks_XmlGen_s* pgen )
{
    if( pgen->pgoff + 1 <= pgen->pglen )
        pgen->pgbuff[pgen->pgoff] = raw;
    pgen->pgoff++;
    
    return 0;
}

static int Winks_xml_genprolog( Winks_XmlGen_s* pgen )
{
    if( pgen->pgoff + (int)strlen(WINKS_WX_DFPROLOG) + 1 > pgen->pglen )
        pgen->pgoff += strlen(WINKS_WX_DFPROLOG) + 1;
    else
    {
        memcpy( pgen->pgbuff, WINKS_WX_DFPROLOG, strlen(WINKS_WX_DFPROLOG) );
        pgen->pgoff += strlen(WINKS_WX_DFPROLOG);
        pgen->pgbuff[pgen->pgoff] = 0x0a;
        pgen->pgoff ++;
    }

    return (strlen(WINKS_WX_DFPROLOG) + 1);
}

static int Winks_xml_genbgtag( Winks_XmlElement_s* pele, Winks_XmlGen_s* pgen )
{
    Winks_xml_geninputrawchar( '<', pgen );

    if( Winks_xml_geninputdata(pele->ptag, pele->taglen, pgen) < 0 )
        return -1;

    Winks_xml_geninputrawchar( '>', pgen );
    
    return 0;
}

static int Winks_xml_gencontent( Winks_XmlElement_s* pele, Winks_XmlGen_s* pgen )
{
    if( Winks_xml_geninputdata(pele->pcontent, pele->conlen, pgen) < 0 )
        return -1;

    return 0;
}

static int Winks_xml_genendtag( Winks_XmlElement_s* pele, Winks_XmlGen_s* pgen )
{

    Winks_xml_geninputrawchar( '<', pgen );
    Winks_xml_geninputrawchar( '/', pgen );

    if( Winks_xml_geninputdata(pele->ptag, pele->taglen, pgen) < 0 )
        return -1;

    Winks_xml_geninputrawchar( '>', pgen );
    
    return 0;
}

//XML Generate internal function
static int Winks_xml_generate( Winks_XmlElement_s* pele, Winks_XmlGen_s* pgen )
{
    if( (pele->ptag == NULL) || (strlen(pele->ptag) == 0) )
    {
        pgen->generr = WINKS_WX_ESTRINVALID;
        return -1;
    }

    if( Winks_xml_genbgtag( pele, pgen ) < 0 )
        return -1;

    if( pele->pchild )
    {
        if( Winks_xml_geninputdata( "\n", 1, pgen ) < 0 )
            return -1;
        if( Winks_xml_generate( pele->pchild, pgen ) < 0 )
            return -1;
    }
    
    if( pele->conlen > 0) 
    {
        if( Winks_xml_gencontent( pele, pgen ) < 0 )
            return -1;
    }
    
    if( Winks_xml_genendtag( pele, pgen ) < 0 )
        return -1;
    if( Winks_xml_geninputdata( "\n", 1, pgen ) < 0 )
        return -1;

    if( pele->pbrother )
    {
        if( Winks_xml_generate( pele->pbrother, pgen ) < 0 )
            return -1;
    }
    
    
    return 0;
}

/******************************************************************************************\
XML generate function group ended
\******************************************************************************************/

/**********************************************************************************\
����ԭ�ͣ�int Winks_XML_Generate(Winks_Xml_s* pXml, char* pSource, int* pSlen )
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
int Winks_XML_Generate( Winks_Xml_s* pXml, char* pSource, int* pSlen )
{
    Winks_XmlGen_s XmlGen;

    if( (pXml == NULL) || (pSlen == NULL) )
        return WINKS_WX_EPARA;

    if( (pXml->encoding != WINKS_WX_CUTF8) || !(pXml->ifsd) || (pXml->mainver != 1) || (pXml->slavever != 0) )
        return WINKS_WX_ESTRINVALID;

    XmlGen.pgbuff = pSource;
    XmlGen.pglen = *pSlen;
    XmlGen.pgoff = 0;
    
    /* Gen prolog first */
    Winks_xml_genprolog( &XmlGen );

    if( pXml->pelement )
        Winks_xml_generate( pXml->pelement, &XmlGen );
        
    *pSlen = XmlGen.pgoff;
    if( XmlGen.pgoff > XmlGen.pglen )
    {
        return WINKS_WX_EBUFFSHORT;
    }

    return XmlGen.pgoff;
}

/**********************************************************************************\
����ԭ�ͣ�int Winks_XML_Parse( Winks_XmlMem_s* pData, Winks_Xml_s** ppXml )
�������ܣ�
        ��������������һ��XML�ļ��Ľ�����
����˵����
        ��������    ��������            ��������    ��ע
        pData        Winks_XmlMem_s *    ����XML����    
        ppXml        Winks_Xml_s**        XML�����ش����    ���ṹ�ڴ��ɽ��������ɣ�Ϊֻ���ṹ��
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
int Winks_XML_Parse( Winks_XmlMem_s* pData, Winks_Xml_s** ppXml )
{
    Winks_XmlMem_s* pdata = pData;
    int ret = 0;

    if( pData == NULL )
        return WINKS_WX_EPARA;
    
    /* Set all memblock offset as zero */
    while( pdata )
    {
        pdata->offset = 0;
        pdata = pdata->pnext;
    }

    ret = Winks_xml_parse( pData, (Winks_XmlInt_s**)ppXml );

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
����ԭ�ͣ�int Winks_XML_Free(Winks_Xml_s* pXml )
�������ܣ�
        ��������������XML���ݽṹ���ڴ��ͷŹ�����
����˵����
        ��������    ��������        ��������        ��ע
        pXml        Winks_Xml_s*    XML�����ش����    

��������ֵ��
        �ɹ�����0��ʧ�ܷ��ظ�ֵ��
        ʧ�ܷ���ֵ��һ�������롣
ʵ��������
        �����û�ͨ��Winks_XML_Parse������õĽṹ���ڴ�ΪXMLģ������ģ���˱���ͨ���������ͷŸ����ݽṹ���Ա�����ڴ档
\**********************************************************************************/
int Winks_XML_Free(Winks_Xml_s* pXml )
{
    if( pXml == NULL )
        return WINKS_WX_EPARA;

    Winks_xml_free( (Winks_XmlInt_s*)pXml );

    return 0;
}

