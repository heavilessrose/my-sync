#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <memory.h>

//#include "wk_xmlgb.h"
#include "../xml/wk_xml.h"
#include "wk_prot_if.h"

int Winks_assert( int a )
{
	if( !a )
		_asm int 3

	return 0;
}

int Winks_Assert( int a )
{
	if( !a )
		_asm int 3

	return 0;
}


int Winks_fail(char *p)
{
    printf(p);
    
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

int Winks_strncpy(char *s1, char *s2, int len)
{
    return (int)strncpy(s1, s2, len);
}

int Winks_strcpy(char *s1, char *s2)
{
    return (int)strcpy(s1, s2);
}

FILE* Winks_fopen(const char *filename,const char *type)
{
    return fopen(filename, type);
}
size_t Winks_fread(void *data, size_t size, size_t count, FILE *stream)
{
    return fread(data, size, count, stream);
}

size_t Winks_fwrite(const void *data, size_t size, size_t count, FILE *stream)
{
    return fwrite(data, size, count, stream);
}

int Winks_fclose(FILE *stream)
{
    return fclose(stream);
}

#define Winks_printf    printf

typedef struct tag_Winks_MemPool_s
{
	struct tag_Winks_MemPool_s* pnext;	//链表指针
	int len;							//data len
	int offset;							//data offset
	char* pdata;						//数据指针
}Winks_MemPool_s;

Winks_MemPool_s * Winks_EngMemPoolCreate( Winks_MemPool_s ** ppMemPool )
{
	Winks_MemPool_s* pmem = NULL;
	Winks_MemPool_s* pnew = NULL;

    Winks_assert(ppMemPool);
    
    if(*ppMemPool != NULL)
    {
    	pmem = *ppMemPool;

    	while( pmem->pnext )
    		pmem = pmem->pnext;
    }

	if( (pnew = (Winks_MemPool_s *)Winks_salAlloc( WINKS_ENG_MEM_BLOCK_LEN + sizeof( Winks_MemPool_s) )) == NULL )
		return NULL;
	Winks_mem_set( pnew, 0, WINKS_ENG_MEM_BLOCK_LEN + sizeof( Winks_MemPool_s) );
	
	pnew->len = WINKS_ENG_MEM_BLOCK_LEN;
	pnew->offset = 0;
	pnew->pdata = (char* )(pnew + 1);

    if(pmem)
	    pmem->pnext = pnew;
    else
        *ppMemPool = pnew;

	return pnew;
}

int Winks_EngMemPoolDestory( Winks_MemPool_s * pMemPool )
{
	Winks_MemPool_s* pmem = NULL;
    Winks_assert(pMemPool);
    
	while( pMemPool->pnext )
	{
		pmem = pMemPool->pnext;
		pMemPool->pnext = pmem->pnext;

		Winks_salFree( pmem );
	}

	return 0;
}

void * Winks_EngMemPoolAlloc( Winks_MemPool_s* pMemPool, 
                                             int AllocSize,
                                             int Align )
{
	Winks_MemPool_s* pmem = NULL;
	void * pret = NULL;

    Winks_assert(pMemPool);
    
	pmem = pMemPool;

	while( pmem->pnext )
		pmem = pmem->pnext;

    if(Align)
        pmem->offset = WINKS_ALIGN(pmem->offset, Align);
    
	if( (pmem->len - pmem->offset) < AllocSize )
	{
		if( (pmem = Winks_EngMemPoolCreate(&pMemPool)) == NULL)
            return NULL;
	}
    
	pret = (void * )(pmem->pdata + pmem->offset);
    Winks_mem_set( pret, 0, AllocSize );
	pmem->offset += AllocSize;
    
	return pret;
}

int main()
{
    int ProtocolNo;
    void  * inData = NULL;
    int     inLen = 0;
    WinksProt_PackData_s * pProtPack = NULL;
    Winks_XmlMem_s * pHttpRcvMem = NULL;
    WinksProt_ParseData_s * pParseData = NULL;

    for( ProtocolNo = WINKS_PROTOCOL_GETSERVICECONFIG; 
         ProtocolNo < WINKS_PROTOCOL_MAX; ProtocolNo++ )
    {
    	switch(ProtocolNo)
    	{
        case WINKS_PROTOCOL_GETSERVICECONFIG:
            
            break;
        case WINKS_PROTOCOL_ACTIVATE:
            
            break;
        case WINKS_PROTOCOL_GETUSERSTATUS:
            
            break;
        case WINKS_PROTOCOL_UPLOADWINKS:
            {
            WinksProt_UploadWinks_s UploadWinks;

            UploadWinks.name = "filename.jpg";
            UploadWinks.namelen = strlen(UploadWinks.name);
            UploadWinks.ext = "jpg";
            UploadWinks.attachName = "/winks/image/filename.jpg";
            UploadWinks.attachNameLen = strlen(UploadWinks.attachName);

            inData = (void *)&UploadWinks;
            inLen = sizeof(WinksProt_UploadWinks_s) + UploadWinks.namelen + UploadWinks.attachNameLen;
            }
            break;
        case WINKS_PROTOCOL_SHOWWINKS:
            {
            char * phoneNumber = "13166668888";
            inData = (void *)phoneNumber;
            inLen = strlen(phoneNumber);
            }
            break;
        case WINKS_PROTOCOL_SHOWSPECIALWINKS:
            {
            int wt = 1;
            inData = (void *)&wt;
            inLen = sizeof(wt);
            }
            break;
        case WINKS_PROTOCOL_GETURLFILE:
            continue;
            {
            char  * url = "/winks/image/img1234.jpg";
            inData = (void *)url;
            inLen = strlen(url);
            }
            break;
        case WINKS_PROTOCOL_RECOMMENDSERVICE:
            {
            int i;
            WinksProt_phoneBook_s phoneBook;
            struct Prot_pbi_s pbi[3];
            char   nameArr[3][8];
            char   phoneArr[3][16];

            phoneBook.pbi_num = 3;

            for(i=0;i<phoneBook.pbi_num;i++)
            {
                sprintf(nameArr[i], "name%d", i);
                sprintf(phoneArr[i], "1316666000%d%d", i);
                pbi[i].n = nameArr[i];
                pbi[i].nlen = strlen(pbi[i].n);
                pbi[i].p = phoneArr[i];
                if(i+1 < phoneBook.pbi_num)
                    pbi[i].pnext = &pbi[i+1];
                else
                    pbi[i].pnext = NULL;
            }
            phoneBook.pbi = &pbi[0];

            inData = (void *)&phoneBook;
            inLen = sizeof(phoneBook)+3*24;
            }
            break;
        case WINKS_PROTOCOL_GETUSERWINKS:
            {
            char * wid = "widwidwidwidwidwid";
            inData = (void *)wid;
            inLen = strlen(wid);
            }
            break;
        case WINKS_PROTOCOL_PURCHASEWINKS:
            {
            char * wid = "widwidwidwidwidwid";
            inData = (void *)wid;
            inLen = strlen(wid);
            }
            break;
        case WINKS_PROTOCOL_CREATEWINKSSCRIPT:
            {
            WinksProt_winksScript_s winksScript;

            winksScript.wid = "widwidwidwidwidwid";
            winksScript.greeting = "greeting";
            winksScript.greetlen = strlen(winksScript.greeting);
            winksScript.IsForAll = 1;
            if(winksScript.IsForAll)
            {
                winksScript.pno = NULL;
                inLen = sizeof(winksScript);
            }
            else
            {
                int i;
                struct Prot_pno_s * pno[3];
                char   phoneArr[3][16];
                
                for(i=0;i<3;i++)
                {
                    sprintf(phoneArr[i], "1316666000%d%d", i);
                    pno[i]->p = phoneArr[i];
                    if(i+1 < 3)
                        pno[i]->pnext = &pno[i+1];
                    else
                        pno[i]->pnext = NULL;
                }
                winksScript.pno = &pno[0];
                inLen = sizeof(winksScript)+3*16;
            }

            inData = (void *)&winksScript;
            }
            break;
        case WINKS_PROTOCOL_MERGEPHONEBOOK:
            {
            int i;
            WinksProt_phoneBook_s phoneBook;
            struct Prot_pbi_s pbi[3];
            char   nameArr[3][8];
            char   phoneArr[3][16];

            phoneBook.pbi_num = 3;

            for(i=0;i<phoneBook.pbi_num;i++)
            {
                sprintf(nameArr[i], "name%d", i);
                sprintf(phoneArr[i], "1316666000%d%d", i);
                pbi[i].n = nameArr[i];
                pbi[i].nlen = strlen(pbi[i].n);
                pbi[i].p = phoneArr[i];
                if(i+1 < phoneBook.pbi_num)
                    pbi[i].pnext = &pbi[i+1];
                else
                    pbi[i].pnext = NULL;
            }
            phoneBook.pbi = &pbi[0];

            inData = (void *)&phoneBook;
            inLen = sizeof(phoneBook)+3*24;
            }
            break;
        case WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE:
            
            break;
        default:
            return -1;
        }	
        /* 打包协议 */
        if(Winks_ProtPack(ProtocolNo, inData, inLen, &pProtPack))
        {
            Winks_printf( "Err: Winks_ProtPack(%d, 0x%08x, %d, 0x%08x)\r\n", 
                                ProtocolNo, inData, inLen, &pProtPack );
            Winks_assert(0);
        }
        Winks_ProtPackFree(pProtPack);
        /* 模拟协议返回文件 */
        SimuHttpRet(ProtocolNo, &pHttpRcvMem);
        
        /* 解析协议 */
        if(Winks_ProtParse( pHttpRcvMem, Winks_EngMemPoolDestory, &pParseData ))
        {
            Winks_printf( "Err: Winks_ProtParse(0x%08x, 0x%08x) prot=%d\r\n", 
                                pHttpRcvMem, &pParseData, ProtocolNo  );
            Winks_assert(0);
        }
        Winks_ProtParseFree(pParseData);
    }

    return 0;
}
