#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <memory.h>
#include "wk_xml.h"

int Winks_assert( int a )
{
    if( !a )
        _asm int 3

    return 0;
}

void* Winks_salAlloc( int size )
{
    return malloc(size);
}

int Winks_salFree( void* p )
{
    free(p);
    return 0;
}

int Winks_mem_cpy( void* pd, void* ps, int len )
{
    return (int)Winks_mem_cpy( pd, ps, len );
}

int Winks_mem_set( void* pd, int data, int len )
{
    return (int)Winks_mem_set( pd, data, len );
}

static Winks_XmlElement_s* getelement( char* ptag, char* pcontent )
{
    Winks_XmlElement_s* pret = NULL; 

    /* test function, tag len < 8, content len < 16 */
    pret = malloc( sizeof(Winks_XmlElement_s) + 8 + 16 );
    Winks_mem_set( pret, 0, sizeof(Winks_XmlElement_s) + 8 + 16 );

    pret->pcontent = (char* )(pret + 1);
    pret->ptag = pret->pcontent + 16;

    if( pcontent )
    {
        Winks_mem_cpy( pret->pcontent, pcontent, strlen(pcontent) );
        pret->conlen = strlen(pcontent);
    }
    Winks_mem_cpy( pret->ptag, ptag, strlen(ptag) );
    pret->taglen = strlen(ptag);

    return pret;

}

int mainbak()
{
    Winks_Xml_s Xml;
    Winks_XmlElement_s* pele = NULL;
    char pbuff[1024];
    int buflen = 1024, ret = 0;
    FILE* pfile = NULL;

    //Winks_XML_Init();

    Xml.encoding = WINKS_WX_CUTF8;
    Xml.ifsd = 1;
    Xml.mainver = 1;
    Xml.slavever = 0;

    pele = getelement( "root", NULL );
    Xml.pelement = pele;
    pele = getelement( "bro01", NULL );
    Xml.pelement->pchild = pele;
    pele = getelement( "bro02", NULL );
    Xml.pelement->pchild->pbrother = pele;
    pele = getelement( "ele01", "first element" );
    //pele->ptag[0] = 0;
    Xml.pelement->pchild->pchild = pele;
    pele = getelement( "ele02", "second element" );
    Xml.pelement->pchild->pchild->pbrother = pele;
    pele = getelement( "ele03", "thired element" );
    Xml.pelement->pchild->pchild->pbrother->pbrother = pele;

    ret = Winks_XML_Generate( &Xml, pbuff, &buflen );

    pfile = fopen( "genxml.xml", "wb" );
    fwrite( pbuff, 1, ret, pfile );
    fclose( pfile );

    //Winks_XML_Dest();
    
    return 0;
}

int main()
{
    Winks_Xml_s* pXml = NULL;
    FILE* pfile = NULL;
    //char pbuff[1024];
    char* pbuff = NULL;
    int rlen = 0, ret = 0, buflen = 0;
    Winks_XmlMem_s* phead = NULL;
    Winks_XmlMem_s* pmem = NULL;
    Winks_XmlMem_s* pnew = NULL;

    pmem = malloc( sizeof(Winks_XmlMem_s) + 1024 );
    pmem->len = 1024;
    pmem->offset = 0;
    pmem->pnext = NULL;
    pmem->pdata = (char* )(pmem + 1);

    phead = pmem;

    pfile = fopen( "test.xml", "rb" );
    ret = fread( pmem->pdata, 1, pmem->len, pfile );
    pmem->len = ret;

    rlen += ret;

    while( !feof(pfile) )
    {
        pnew = malloc( sizeof(Winks_XmlMem_s) + 1024 );
        pnew->len = 1024;
        pnew->offset = 0;
        pnew->pnext = NULL;
        pnew->pdata = (char* )(pnew + 1);
        ret = fread( pnew->pdata, 1, pnew->len, pfile );
        pnew->len = ret;
        rlen += ret;
        pmem->pnext = pnew;
        pmem = pnew;
    }
    //rlen = fread( pbuff, 1, 1024, pfile );
    fclose( pfile );

    ret = Winks_XML_Parse( NULL, NULL );
    ret = Winks_XML_Parse( NULL, &pXml );
    if( Winks_XML_Parse( phead, &pXml ) < 0 )
    {
        printf( "Xml Parse return error %d\r\n" );
        return 0;
    }

    //pbuff = malloc( rlen + 1 );

    ret = Winks_XML_Generate( NULL, pbuff, &buflen );
    ret = Winks_XML_Generate( pXml, pbuff, NULL );
    if( (ret = Winks_XML_Generate( pXml, pbuff, &buflen )) < 0 )
    {
        printf( "Xml Generate return error %d\r\n" );
        pbuff = malloc( buflen + 1 );
        Winks_mem_set( pbuff, 0, buflen + 1 );
        Winks_XML_Generate( pXml, pbuff, &buflen );
    }

    pfile = fopen( "genxml.xml", "wb" );
    fwrite( pbuff, 1, buflen, pfile );
    fclose( pfile );

    Winks_XML_Free( pXml );
    
    //Winks_XML_Dest();

    return 0;
}