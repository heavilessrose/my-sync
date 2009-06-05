#include "wk_public.h"
#include "wk_osal.h"

#include "wk_strg_if.h"
#include "wk_strg.h"

//#include "wk_storage_pub.h"

#define SORTED_LIST_FOR_BSEARCH
#define DELETE_UNUSED_IMAGE

extern void * get_strg_filemap(void);

#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    #define WINKS_STRG_FILEMAP_DEF WinksFileMap_s * pFileMap = NULL;
    
    #define WINKS_STRG_FILEMAP_INIT pFileMap = (WinksFileMap_s *)get_strg_filemap();
#else
    #define WINKS_STRG_FILEMAP_DEF
    #define WINKS_STRG_FILEMAP_INIT

    static WinksFileMap_s * pFileMap = NULL;
#endif

#define _SelfNoBcd      (pFileMap->SelfNoBcd)
#define _CacheTime      (pFileMap->CacheTime)

#define _UserMap        (pFileMap->pUserMap)
#define _UserNum        (pFileMap->UserNum)
#define _UserOccupy     (pFileMap->UserOccupy)

#define _FriendMap      (pFileMap->pFriendMap)
#define _FriendNum      (pFileMap->FriendNum)

#define _CCCache        (pFileMap->pCCCache)
#define _CCCacheOff     (pFileMap->CCCacheOff)
#define _CCCacheNoBcd   (pFileMap->CCCacheNoBcd)

#define _LockCCIdx      (pFileMap->LockCCIdx)

#ifdef xxxxxxxx
//WinksMapFile_UserInfo_s * _UserMap;
WinksStrgFile_UserInfo_s * _UserMap;
WinksMapFile_FriendInfo_s* _FriendMap;
#endif

#ifdef WINKS_SUPPORT_MEMORY_CARD
#define DRIVE_NUM       2
#else
#define DRIVE_NUM       1
#endif

#ifdef WINKS_DEBUG
static void winks_StrgPrintFsErrLine(int line, char *pPath)
{
    int FsErr;
    
    Winks_GetFileSysErrorCode((unsigned long *)&FsErr);
    if(FsErr)
        Winks_printf("ERR_FS: L%04d (%d) in %s\r\n", line, FsErr, pPath);
}

#define  winks_StrgPrintFsErr(pPath) \
    winks_StrgPrintFsErrLine(__LINE__, pPath)

#else /* WINKS_DEBUG */
#define winks_StrgPrintFsErr(pPath)
#endif /* WINKS_DEBUG */

static int winks_GetFilePath(int StrgLoc, char *retAppPath, int PathSize, char *pFileName)
{
    int iRet = Winks_GetAppPath(StrgLoc, retAppPath, PathSize);
    if(iRet != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    strcat(retAppPath, DIR_STRG);
    if(pFileName != NULL)
    {
        strcat(retAppPath, WINKS_PATH_SPACE);
        strcat(retAppPath, pFileName);
    }
    
    return iRet;
}

#define winks_GetDirPath_Strg(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, NULL)

#define winks_GetFilePath_Setting(StrgLoc, retAppPath, PathSize)    \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_SETTING)

#define winks_GetFilePath_User(StrgLoc, retAppPath, PathSize)   \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_USER)

#define winks_GetFilePath_Call(StrgLoc, retAppPath, PathSize)   \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_CALL)

#define winks_GetFilePath_CCInfo(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_CCINFO)

#define winks_GetFilePath_Greet(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_GREET)
    
#define winks_GetFilePath_Stat(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_STAT)
    
#define winks_GetFilePath_Friend(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, FILE_FRIEND)
    
#define winks_GetFilePath_FL(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, TMP_FL)
    
#define winks_GetFilePath_UL(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, TMP_UL)
    
#define winks_GetFilePath_ND(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, TMP_ND)

#define winks_GetDirPath_IMG(StrgLoc, retAppPath, PathSize) \
    winks_GetFilePath(StrgLoc, retAppPath, PathSize, DIR_IMG)

static int winks_GetImgPath(int StrgLoc, char *retAppPath, int PathSize, 
                unsigned short CC_Idx, unsigned char img_type, char *Ext)
{
    char ImageName[16];
    int iRet = winks_GetDirPath_IMG(StrgLoc, retAppPath, PathSize);
    Winks_assert(iRet == WINKS_RETURN_SUCCESS)

    Winks_assert(CC_Idx < WINKS_MAX_USER);
    switch(img_type)
    {
    case WINKS_FILE_EXT_gif:
        sprintf(ImageName, "%s%04d.%s", PRE_IMG, CC_Idx, "gif");
        break;
    case WINKS_FILE_EXT_jpg:
        sprintf(ImageName, "%s%04d.%s", PRE_IMG, CC_Idx, "jpg");
        break;
    case WINKS_FILE_EXT_png:
        sprintf(ImageName, "%s%04d.%s", PRE_IMG, CC_Idx, "png");
        break;
    case WINKS_FILE_EXT_tiff:
        sprintf(ImageName, "%s%04d.%s", PRE_IMG, CC_Idx, "tiff");
        break;
    case WINKS_FILE_EXT_spl:
        sprintf(ImageName, "%s%04d.%s", PRE_IMG, CC_Idx, "spl");
        break;
    case WINKS_FILE_EXT_swf:
        sprintf(ImageName, "%s%04d.%s", PRE_IMG, CC_Idx, "swf");
        break;
    default:
        sprintf(ImageName, "%s%04d", PRE_IMG, CC_Idx);
        break;
    }
    if(Ext)
        strcat(ImageName, Ext);
    
    strcat(retAppPath, WINKS_PATH_SPACE);
    strcat(retAppPath, ImageName);

    return iRet;
}

#define winks_GetImgFilePath(StrgLoc, retAppPath, PathSize, CC_Idx, img_type)    \
    winks_GetImgPath(StrgLoc, retAppPath, PathSize, CC_Idx, img_type, NULL)

#define winks_GetImgTmpPath(StrgLoc, retAppPath, PathSize, CC_Idx, img_type)    \
    winks_GetImgPath(StrgLoc, retAppPath, PathSize, CC_Idx, img_type, SUF_TMP)

#define winks_GetImgPtfPath(StrgLoc, retAppPath, PathSize, CC_Idx, img_type)    \
    winks_GetImgPath(StrgLoc, retAppPath, PathSize, CC_Idx, img_type, SUF_PTF)

static int winks_GetImgDftPath(char *retAppPath, int PathSize)
{
    Winks_mem_set(retAppPath, 0, PathSize);

    strcpy(retAppPath, WINKS_IMG_PATH);
    strcat(retAppPath, WINKS_DEF_IMG);

    return WINKS_RETURN_SUCCESS;
}


static int winks_PhoneNoCompare( unsigned char *pNo, unsigned int Len, 
                                unsigned char *pKeyNo, unsigned int KeyLen,
                                unsigned int Limit)
{
    int Cmp;
    unsigned int Cnt, CmpLen;
    
    Winks_assert(pNo != NULL);
    Winks_assert(pKeyNo != NULL);

    CmpLen = MIN(KeyLen, Limit);
    if(Len<CmpLen)return -1;
    
    Cnt = 1;
    while(Cnt <= CmpLen)
    {
        Cmp = (int)pNo[Len-Cnt] - (int)pKeyNo[KeyLen-Cnt];
        if(Cmp)
            return Cmp;
        Cnt++;
    }

    return 0;
}

#define winks_NoBcdCompare( pNoBcd, pKeyNoBcd )  \
    winks_PhoneNoCompare( pNoBcd, WINKS_STRG_NOBCD_LEN,    \
                          pKeyNoBcd, WINKS_STRG_NOBCD_LEN,     \
                          WINKS_PNO_BCD_CMP_LEN)

/* 注意:
    该函数是从后往前对比，所以Len,KeyLen要用strlen计算
    比对长度取 MIN(KeyLen, WINKS_PNO_STR_CMP_LEN)
*/
static int winks_NoStrCompare( char *pNoStr, char *pKeyNoStr)
{
    return winks_PhoneNoCompare( (unsigned char *)pNoStr, strlen(pNoStr), 
                                 (unsigned char *)pKeyNoStr, strlen(pKeyNoStr), 
                                 WINKS_PNO_STR_CMP_LEN );
}

unsigned char* Winks_UnicodeStrncpy(unsigned char *dst, const unsigned char *src, int size)
{
    unsigned short * pDst, *pSrc;

    if( dst == NULL || src == 0 || size < 4 )
        return NULL;

    pSrc = (unsigned short *)src;
    if(*pSrc == 0)
        return NULL;
    
    pDst = (unsigned short *)dst;
    while (pSrc && *pSrc && size>=4)
    {
        *pDst++ = *pSrc++;
        size -= 2;
    }
    *pDst = 0;
    
    return dst;
}

int Winks_WriteBuf2File( char *pPath, unsigned long Off, 
                            unsigned char *pBuf, unsigned long BufSize,
                            int CreateFlag)
{
    FILE *  fp;
    int     FsErr = 0;
    
    Winks_assert(pPath != NULL);

    if(CreateFlag)
        fp = Winks_fopen(pPath, "wb");
    else
        fp = Winks_fopen(pPath, "r+b");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }
    
    if(Off)
        FsErr = Winks_fseek(fp, Off, WINKS_SEEK_SET);
    if(FsErr>=0 && BufSize && pBuf)
        FsErr = Winks_fwrite(pBuf, BufSize, 1, fp);
    
    Winks_fclose(fp);

    if(FsErr<0)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }
    
    if(BufSize && FsErr != 1)
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}

int Winks_ReadFile2Buf( char *pPath, unsigned long Off, 
                            unsigned char *pBuf, unsigned long BufSize)
{
    FILE *  fp;
    int     FsErr = 0;
    
    Winks_assert(pPath != NULL);
    Winks_assert(pBuf != NULL);
    Winks_assert(BufSize > 0);

    fp = Winks_fopen(pPath, "rb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }
    
    FsErr = Winks_fseek(fp, Off, WINKS_SEEK_SET);
    if(FsErr >= 0)
        FsErr = Winks_fread(pBuf, BufSize, 1, fp);
    
    Winks_fclose(fp);

    if(FsErr<0)
        winks_StrgPrintFsErr(pPath);
    
    if(FsErr != 1)
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}
#if 0
#define WINKS_COPY_BLOCK_SIZE       4096
#define WINKS_COPY_BLOCK_LIMIT      512
static int winks_StrgCopyFile(char *pDstPath, char *pSrcPath)
{
    FILE  * fpIn;
    FILE  * fpOut;
    int     FsErr = 0;
    unsigned char * pBuf = NULL;
    unsigned long   BlkSize = WINKS_COPY_BLOCK_SIZE;
    size_t  ReadLen;     

    Winks_assert(pDstPath);
    Winks_assert(pSrcPath);

    while(pBuf == NULL)
    {
        pBuf = (unsigned char *)Winks_salAlloc(BlkSize);
        if(pBuf == NULL)
        {
            BlkSize >>= 1;
            if(BlkSize<WINKS_COPY_BLOCK_LIMIT)
                return WINKS_RETURN_FAILURE;
        }
    }
    
    fpIn = Winks_fopen(pSrcPath, "rb");
    if(fpIn == NULL)
    {
        Winks_salFree(pBuf);
        winks_StrgPrintFsErr(pSrcPath);
        return WINKS_RETURN_FAILURE;
    }
    
    fpOut = Winks_fopen(pDstPath, "wb");
    if(fpOut == NULL)
    {
        Winks_salFree(pBuf);
        Winks_fclose(fpIn);
        winks_StrgPrintFsErr(pDstPath);
    }

    while((FsErr = Winks_fread(pBuf, 1, BlkSize, fpIn)) > 0)
    {
        ReadLen = FsErr;
        if((FsErr = Winks_fwrite(pBuf, 1, ReadLen, fpOut)) != ReadLen)
            break;
    }

    Winks_salFree(pBuf);
    Winks_fclose(fpIn);
    Winks_fclose(fpOut);

    if(FsErr<0)
        winks_StrgPrintFsErr(pDstPath);
    
    if(FsErr != 1)
        return WINKS_RETURN_FAILURE;

    return WINKS_RETURN_SUCCESS;
}
#endif
#if 0
static winks_PreTakeup(char *pPath, unsigned long Size)
{
    FILE  * fp;
    int     FsErr = 0;
    unsigned int  FileSize;

    Winks_assert(pPath);
    
 	Winks_printf("winks_PreTakeup(%s, %lu)\r\n", pPath, Size);
   if(Size == 0)
        return WINKS_RETURN_SUCCESS;
    
    if( Winks_fgetsize(pPath, 1, &FileSize) >= 0 &&
        FileSize == Size )
        return WINKS_RETURN_SUCCESS;

    fp = Winks_fopen(pPath, "wb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }

    if(Size >= 4)
    {
        FsErr = Winks_fseek(fp, Size-4, WINKS_SEEK_SET);
        if(FsErr >= 0)
            FsErr = Winks_fwrite(&Size, 4, 1, fp);
    }
    else
        FsErr = Winks_fwrite(&Size, Size, 1, fp);
    
    Winks_fclose(fp);

    if(FsErr <= 0)
    {
        winks_StrgPrintFsErr(pPath);
        Winks_fdelete(pPath);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}
#endif
static int winks_StrgRecordAdd(char *pPath, 
                                unsigned int HeadLen, 
                                int Idx, 
                                void *pRecord, 
                                unsigned int RecordLen)
{
    FILE *  fp;
    int     FsErr = 0;

    int     Off;
    
    Winks_assert(pPath != NULL);
    Winks_assert(pRecord != NULL);
    Winks_assert(RecordLen > 0);

    fp = Winks_fopen(pPath, "r+b");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }

    if(Idx>=0)
    {
        Off = Idx*RecordLen + HeadLen;
        FsErr = Winks_fseek(fp, Off, WINKS_SEEK_CUR);
    }
    else
        FsErr = Winks_fseek(fp, 0, WINKS_SEEK_END);
    if(FsErr>=0)
        FsErr = Winks_fwrite(pRecord, RecordLen, 1, fp);
    
    Winks_fclose(fp);

    if(FsErr<0)
        winks_StrgPrintFsErr(pPath);
    
    if(FsErr != 1)
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_StrgRecordSearch(char *pPath, unsigned int HeadLen,
                                    void *Key,
                                    unsigned int RecordLen,
                                    int (*Compare) (void *, void *),
                                    int *retIdx,
                                    void * retRecord)
{
    FILE *  fp;
    int     FsErr = 0;
    int     Idx;
    void  * pRecord;
    
    Winks_assert(pPath != NULL);
    Winks_assert(Key != NULL);
    Winks_assert(RecordLen > 0);
    Winks_assert(Compare);
    Winks_assert(retIdx != NULL);

    *retIdx = -1;
    pRecord = Winks_salAlloc(RecordLen);
    if(pRecord == NULL)
        return WINKS_STRG_SEARCH_RESULT_FAIL;
    
    fp = Winks_fopen(pPath, "rb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        Winks_salFree(pRecord);
        return WINKS_STRG_SEARCH_RESULT_FAIL;
    }

    if(HeadLen)
    {
        FsErr = Winks_fseek(fp, HeadLen, WINKS_SEEK_SET);
        if(FsErr < 0)
        {
            winks_StrgPrintFsErr(pPath);
            Winks_salFree(pRecord);
            Winks_fclose(fp);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }
    }
    
    Idx = 0;
    do
    {
        FsErr = Winks_fread(pRecord, RecordLen, 1, fp);
        if(FsErr == 1)
        {
            if(Compare(pRecord, Key) == 0)
            {
                *retIdx = Idx;
                break;
            }
        }
        Idx++;
    }while(FsErr == 1);
    
    Winks_salFree(pRecord);
    
    Winks_fclose(fp);

    if(FsErr<0)
        winks_StrgPrintFsErr(pPath);

    if(*retIdx != -1)
    {
        if(retRecord)
            Winks_mem_cpy(retRecord, pRecord, RecordLen);
        return WINKS_STRG_SEARCH_RESULT_FOUND;
    }
    
    return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
}

#if 0
static int winks_StrgRecordCnt(char *pPath, unsigned int HeadLen, unsigned int RecordLen)
{
    unsigned int FileSize;
    
    Winks_assert(pPath != NULL);
    Winks_assert(RecordLen > 0);

    if(Winks_fgetsize(pPath, 1, &FileSize) < 0)
        return -1;

    if(FileSize <= HeadLen)
        return 0;

    FileSize -= HeadLen;
    Winks_assert((FileSize%RecordLen) == 0);
    return FileSize/RecordLen;
}
#endif

static int winks_StrgRecordValidCnt(char *pPath, 
                                            unsigned int  HeadLen, 
                                            unsigned int RecordLen,
                                            int (*IsValid) (void *))
{
    FILE *  fp;
    int     FsErr = 0;
    int     ValidCnt;
    void  * pRecord;
    
    Winks_assert(pPath != NULL);
    Winks_assert(RecordLen > 0);

    pRecord = Winks_salAlloc(RecordLen);
    if(pRecord == NULL)
        return WINKS_RETURN_FAILURE;
    
    fp = Winks_fopen(pPath, "rb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        Winks_salFree(pRecord);
        return WINKS_RETURN_FAILURE;
    }
    
    if(HeadLen)
    {
        FsErr = Winks_fseek(fp, HeadLen, WINKS_SEEK_SET);
        if(FsErr < 0)
        {
            Winks_fclose(fp);
            winks_StrgPrintFsErr(pPath);
            Winks_salFree(pRecord);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }
    }
    
    ValidCnt = 0;
    do
    {
        FsErr = Winks_fread(pRecord, RecordLen, 1, fp);
        if(FsErr == 1)
        {
            if(IsValid(pRecord))
            {
                ValidCnt++;
            }
        }
    }while(FsErr == 1);
    
    Winks_salFree(pRecord);
    
    Winks_fclose(fp);

    if(FsErr<0)
        winks_StrgPrintFsErr(pPath);

    return ValidCnt;
}

static int winks_StrgRecordGet( int BeginIdx, 
                                        char *pPath, 
                                        unsigned int  HeadLen, 
                                        void *pRecord, 
                                        unsigned int RecordLen,
                                        int (*IsValid) (void *))
{
    FILE *  fp;
    int     FsErr = 0;
    int     Off;
    
    Winks_assert(pPath != NULL);
    Winks_assert(RecordLen > 0);
    Winks_assert(BeginIdx >= 0);

    fp = Winks_fopen(pPath, "rb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
    }

    Off = BeginIdx*RecordLen + HeadLen;
    FsErr = Winks_fseek(fp, Off, WINKS_SEEK_SET);
    if(FsErr<0)
    {
        winks_StrgPrintFsErr(pPath);
        Winks_fclose(fp);
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
    }
    
    do
    {
        FsErr = Winks_fread(pRecord, RecordLen, 1, fp);
        if(FsErr == 1)
        {
            if(IsValid(pRecord))
            {
                break;
            }
        }
        BeginIdx++;
    }while(FsErr == 1);
    
    Winks_fclose(fp);

    if(FsErr<0)
        winks_StrgPrintFsErr(pPath);

    if(FsErr == 1)
        return BeginIdx+1;
    
    return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
}

#if 0
static int winks_StrgRecordDelete(char *pPath, unsigned int  HeadLen, int Idx, int RecordLen)
{
    FILE *  fp;
    int     FsErr = 0;
    int     Off;
    void  * pRecord;
    
    Winks_assert(pPath != NULL);
    Winks_assert(RecordLen > 0);
    Winks_assert(Idx >= 0);

    pRecord = Winks_salAlloc(RecordLen);
    if(pRecord == NULL)
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(pRecord, 0, RecordLen);
    
    fp = Winks_fopen(pPath, "r+b");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        Winks_salFree(pRecord);
        return WINKS_RETURN_FAILURE;
    }

    Off = Idx*RecordLen + HeadLen;
    FsErr = Winks_fseek(fp, Off, WINKS_SEEK_SET);
    if(FsErr>=0)
        FsErr = Winks_fwrite(pRecord, RecordLen, 1, fp);

    Winks_salFree(pRecord);
    
    Winks_fclose(fp);

    if(FsErr<0)
        winks_StrgPrintFsErr(pPath);

    if(FsErr == 1)
        return WINKS_RETURN_SUCCESS;
    
    return WINKS_RETURN_FAILURE;
}
#endif
static int winks_StrgRecordDeleteAll(char *pPath)
{
    Winks_assert(pPath != NULL);

    return Winks_fdelete(pPath);
}

static int winks_IsValidString(void *pStr)
{
    Winks_assert(pStr);
    
    return (*((char *)pStr) != 0);
}

#if 0
static int winks_IsValid_ND(void *pNeedDown)
{
    Winks_assert(pNeedDown);
    
    return (((WinksStrgFile_NeedDown_s *)pNeedDown)->NO[0] != 0);
}
#endif

static int winks_IsValid_CS(void *pCallStat)
{
    int Cnt;
    
    Winks_assert(pCallStat);

    for(Cnt=WINKS_STRG_NOBCD_LEN-1; Cnt>=0; Cnt--)
    {
        if(((WinksStrgFile_CallStat_s *)pCallStat)->NoBcd[Cnt])
            return 1;
    }
    return 0;
}

#if 0
static int winks_NDCompare(void *pNeedDown, void *pKeyNo)
{
    Winks_assert(pNeedDown);
    Winks_assert(pKeyNo);
    
    return winks_NoStrCompare(((WinksStrgFile_NeedDown_s *)pNeedDown)->NO, (char *)pKeyNo);
}
#endif

static int winks_CSCompare(void *pCallStat, void *pKeyNoBcd)
{
    Winks_assert(pCallStat);
    Winks_assert(pKeyNoBcd);
    
    return winks_NoBcdCompare((unsigned char *)((WinksStrgFile_CallStat_s *)pCallStat)->NoBcd, 
                    (unsigned char *)pKeyNoBcd);
}

static int winks_CCIDCompare(void *pCCInfo, void *pKeyCCID)
{
    Winks_assert(pCCInfo);
    Winks_assert(pKeyCCID);
    
    return strcmp(((WinksStrgFile_CCInfo_s *)pCCInfo)->ID, (char *)pKeyCCID);
}

static int winks_StatCCIDCompare(void *pStat, void *pKeyCCId)
{
    Winks_assert(pStat);
    Winks_assert(pKeyCCId);
    
    return strcmp((const char *)((WinksStrgFile_Stat_s *)pStat)->ID, 
                    (char *)pKeyCCId);
}

static int winks_StrgCreateFile_Friend(char *pPath)
{   
    int     iRet;
    unsigned char * pTmpBuf;
    
    WinksStrgFile_FriendInfo_s FriendInfo;

    Winks_assert(pPath != NULL);
    pTmpBuf = (unsigned char *)Winks_salAlloc(sizeof(FriendInfo)*WINKS_FRIENDS_MAX_NUMBER);

    if(pTmpBuf)
    {
        Winks_mem_set(pTmpBuf, 0, sizeof(FriendInfo)*WINKS_FRIENDS_MAX_NUMBER);
        iRet = Winks_WriteBuf2File(pPath, 0, pTmpBuf, sizeof(FriendInfo)*WINKS_FRIENDS_MAX_NUMBER, 1);
        Winks_salFree(pTmpBuf);
    }
    else
    {
        int Cnt;
        Winks_mem_set(&FriendInfo, 0, sizeof(FriendInfo));

        iRet = WINKS_RETURN_SUCCESS;
        for(Cnt=0; iRet==WINKS_RETURN_SUCCESS && Cnt<WINKS_FRIENDS_MAX_NUMBER; Cnt++)
        iRet = Winks_WriteBuf2File(pPath, 0, (unsigned char *)&FriendInfo, sizeof(FriendInfo), 1);
    }

    return iRet;
}

static int winks_StrgCreateFile_User(char *pPath)
{   
    FILE  * fp;
    int     FsErr = 0;

    int     Cnt;
    char    SelfNo[32];
    WinksStrgFile_UserHead_s Head;
    WinksStrgFile_UserInfo_s UserInfo;
    
    Winks_mem_set(SelfNo, 0, sizeof(SelfNo));
    Winks_mem_set(&UserInfo, 0, sizeof(UserInfo));

    fp = Winks_fopen(pPath, "wb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }
    
    Winks_mem_set(&Head, 0, sizeof(Head));
    FsErr = Winks_fwrite(&Head, sizeof(Head), 1, fp);
    if(FsErr == 1)/* 预占用户信息 */
    {
        Cnt = 0;
        while(Cnt < WINKS_MAX_USER)
        {
            FsErr = Winks_fwrite(&UserInfo, sizeof(UserInfo), 1, fp);
            if(FsErr != 1)
                break;
            
            Cnt++;
        }
    }

    Winks_fclose(fp);
    
    if(FsErr < 0)
    {
        winks_StrgPrintFsErr(pPath);
        return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_StrgCreateFile_CCInfo(char *pPath)
{   
    int     iRet;
    unsigned char  * pTmpBuf;
    
    WinksStrgFile_CCHead_s Head; 
    WinksStrgFile_CCInfo_s CCInfo;
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    pTmpBuf = (unsigned char *)Winks_salAlloc(sizeof(Head)+sizeof(CCInfo));

    Winks_mem_set(&Head, 0, sizeof(Head));
    Winks_mem_cpy(Head.SelfNoBcd, _SelfNoBcd, WINKS_STRG_NOBCD_LEN);
    Head.CacheTime = _CacheTime;
    
    if(pTmpBuf)
    {
        Winks_mem_set(pTmpBuf, 0, sizeof(Head)+sizeof(CCInfo));
        Winks_mem_cpy(pTmpBuf, &Head, sizeof(Head));
        iRet = Winks_WriteBuf2File(pPath, 0, pTmpBuf, sizeof(Head)+sizeof(CCInfo), 1);
        Winks_salFree(pTmpBuf);
    }
    else
    {
        iRet = Winks_WriteBuf2File(pPath, 0, (unsigned char *)&Head, sizeof(Head), 1);
        Winks_mem_set(&CCInfo, 0, sizeof(CCInfo));
        if(iRet == WINKS_RETURN_SUCCESS)
            iRet = Winks_WriteBuf2File(pPath, sizeof(Head), (unsigned char *)&CCInfo, sizeof(CCInfo), 0);
    }

    return iRet;
}

static int winks_StrgCheckFile(int FileEnum, int CreateFlag)
{
    int     iRet = WINKS_RETURN_SUCCESS;
    char    Path[WINKS_PATH_MAX];
    int     FsErr;
    
    unsigned int    FileSize;
    unsigned int    MinFileSize;

    switch(FileEnum)
    {
    case WINKS_STRG_FILE_ENUM_CALL:
        iRet = winks_GetFilePath_Call(0, Path, sizeof(Path));
        MinFileSize = sizeof(WinksStrgFile_CallHead_s);
        break;
    case WINKS_STRG_FILE_ENUM_FRIEND:
        iRet = winks_GetFilePath_Friend(0, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_FriendInfo_s) * WINKS_FRIENDS_MAX_NUMBER);
        break;
    case WINKS_STRG_FILE_ENUM_UL:
        iRet = winks_GetFilePath_UL(0, Path, sizeof(Path));
        MinFileSize = WINKS_MAX_UL*WINKS_STRG_PHONE_LEN;
        break;
    case WINKS_STRG_FILE_ENUM_USER:
        iRet = winks_GetFilePath_User(0, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_UserHead_s)+ sizeof(WinksStrgFile_UserInfo_s)*WINKS_MAX_USER);
        break;
    case WINKS_STRG_FILE_ENUM_STAT:
        iRet = winks_GetFilePath_Stat(0, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_StatHead_s));
        break;
    case WINKS_STRG_FILE_ENUM_CCINFO:
        iRet = winks_GetFilePath_CCInfo(0, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_CCHead_s)+sizeof(WinksStrgFile_CCInfo_s));
        break;
    case WINKS_STRG_FILE_ENUM_GREET:
        iRet = winks_GetFilePath_Greet(0, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_GreetHead_s));
        break;
#ifdef WINKS_SUPPORT_MEMORY_CARD
    case WINKS_STRG_FILE_ENUM_CCINFO_1:
        iRet = winks_GetFilePath_CCInfo(1, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_CCHead_s)+sizeof(WinksStrgFile_CCInfo_s));
        break;
    case WINKS_STRG_FILE_ENUM_GREET_1:
        iRet = winks_GetFilePath_Greet(1, Path, sizeof(Path));
        MinFileSize = (sizeof(WinksStrgFile_GreetHead_s));
        break;
#endif
    default:
        return WINKS_RETURN_FAILURE;
    }

    if(iRet != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_SUCCESS;/* 认为没有存储卡 */
    
    FsErr = Winks_fgetsize(Path, 1, &FileSize);
    
    if( (FsErr<0) ||
        (FileSize <  MinFileSize) )
    {
        if(!CreateFlag)
            return WINKS_RETURN_FAILURE;
        
        switch(FileEnum)
        {
        case WINKS_STRG_FILE_ENUM_CALL:
            {
            WinksStrgFile_CallHead_s Head;

            Winks_mem_set(&Head, 0, sizeof(Head));
            Winks_mem_cpy(Head.Version, WINKS_CALL_VER, sizeof(Head.Version));
            iRet = Winks_WriteBuf2File(Path, 0, (unsigned char *)&Head, sizeof(Head), 1);
            if(iRet == WINKS_RETURN_SUCCESS)
            {
                WinksStrgFile_CallStat_s CallStatArr[WINKS_CALL_STAT_PRETAKE_SIZE];
                Winks_mem_set(&CallStatArr, 0, sizeof(WinksStrgFile_CallStat_s)*WINKS_CALL_STAT_PRETAKE_SIZE);
                iRet = Winks_WriteBuf2File(Path, sizeof(WinksStrgFile_CallHead_s), (unsigned char *)CallStatArr, sizeof(WinksStrgFile_CallStat_s)*WINKS_CALL_STAT_PRETAKE_SIZE, 0);
            }
            }
            break;
        case WINKS_STRG_FILE_ENUM_FRIEND:
            iRet = winks_StrgCreateFile_Friend(Path);
            break;
        case WINKS_STRG_FILE_ENUM_UL:
            {
            char    Buf[WINKS_MAX_UL][WINKS_STRG_PHONE_LEN];
            Winks_mem_set(Buf, 0, MinFileSize);
            iRet = Winks_WriteBuf2File(Path, 0, (unsigned char *)Buf, MinFileSize, 1);
            }
            break;
        case WINKS_STRG_FILE_ENUM_USER:
            iRet = winks_StrgCreateFile_User(Path);
            break;
        case WINKS_STRG_FILE_ENUM_STAT:
            {
            WinksStrgFile_StatHead_s Head; 
            Winks_mem_set(&Head, 0, sizeof(Head));
            Winks_mem_cpy(Head.Version, WINKS_STAT_VER, sizeof(Head.Version));
            iRet = Winks_WriteBuf2File(Path, 0, (unsigned char *)&Head, sizeof(Head), 1);
            }
            break;
        case WINKS_STRG_FILE_ENUM_CCINFO:
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        case WINKS_STRG_FILE_ENUM_CCINFO_1:
    #endif
            iRet = winks_StrgCreateFile_CCInfo(Path);
            break;
        case WINKS_STRG_FILE_ENUM_GREET:
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        case WINKS_STRG_FILE_ENUM_GREET_1:
    #endif
            {
            WinksStrgFile_GreetHead_s Head; 
            Winks_mem_set(&Head, 0, sizeof(Head));
            iRet = Winks_WriteBuf2File(Path, 0, (unsigned char *)&Head, sizeof(Head), 1);
            }
            break;
        default:
            return WINKS_RETURN_FAILURE;
        }
    }

    return iRet;
}

static int winks_StrgCheckDir(int StrgLoc)
{   
    int     iRet = 0;
    char    Path[WINKS_PATH_MAX];

    iRet = winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path));
    if(iRet != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_SUCCESS;/* 认为没有存储卡 */
    
    if (Winks_fexist(Path) < 0)
    {
        Winks_GetAppPath(StrgLoc, Path, sizeof(Path));
        if(iRet == WINKS_RETURN_SUCCESS)
        {
            Winks_mkdir(Path);
            iRet = winks_GetDirPath_Strg(StrgLoc, Path, sizeof(Path));
            if(iRet == WINKS_RETURN_SUCCESS)
                Winks_mkdir(Path);
        }

    }
    iRet = winks_GetDirPath_IMG(StrgLoc, Path, sizeof(Path));
    if(iRet == WINKS_RETURN_SUCCESS)
        Winks_mkdir(Path);

    return WINKS_RETURN_SUCCESS;
}
    
/*****************************************************************************
DESCRIPTION:
    set cache time and self phone no to <call.dat> and <ccinfo.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
static int winks_StrgSetCacheFlag(char *pNo, unsigned long NewCacheTime)
{
    int     Cnt = 0, Max;
    FILE  * fp;
    char    Path[WINKS_PATH_MAX];
    int     FsErr = 0;
    char    NoBcd[WINKS_STRG_NOBCD_LEN];
            
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(pNo == NULL && NewCacheTime == 0)
        return WINKS_RETURN_FAILURE;
    
    if(pNo)
    {
        if(Winks_PhoneNoStr2Bcd(pNo, strlen(pNo), WINKS_STRG_NOBCD_LEN, (unsigned char *)NoBcd) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

#ifdef WINKS_SUPPORT_MEMORY_CARD
    Max = 3;
#else
    Max = 2;
#endif
    while(Cnt < Max)
    {
        int iRet = WINKS_RETURN_FAILURE;
        WinksStrgFile_UserHead_s Head;

        switch(Cnt)
        {
        case 0:
            iRet = winks_GetFilePath_User(0, Path, sizeof(Path));
            break;
        case 1:
            iRet = winks_GetFilePath_CCInfo(0, Path, sizeof(Path));
            break;
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        case 2:
            iRet = winks_GetFilePath_CCInfo(1, Path, sizeof(Path));
            break;
    #endif
        default:
            break;
        }
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            Cnt++;
            continue;/* 认为没有存储卡 */
        }
        
        fp = Winks_fopen(Path, "r+b");
        if(fp == NULL)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }

        if(pNo==NULL || NewCacheTime==0)
            FsErr = Winks_fread(&Head, sizeof(Head), 1, fp);
            if(FsErr == 1)
                FsErr = Winks_fseek(fp, 0, WINKS_SEEK_SET);
        
        if( FsErr >= 0)
        {
            if(pNo)
                Winks_mem_cpy(Head.SelfNoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
            
            if(NewCacheTime)
                Head.CacheTime = NewCacheTime;
            
            FsErr = Winks_fwrite(&Head, sizeof(Head), 1, fp);
        }
        
        Winks_fclose(fp);

        if(FsErr<0)
            winks_StrgPrintFsErr(Path);

        if(FsErr<=0)
            return WINKS_RETURN_FAILURE;
        
        Cnt++;
    }
    
    if(pNo)
        Winks_mem_cpy(_SelfNoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
    
    if(NewCacheTime)
        _CacheTime = NewCacheTime;
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    search save pos in file <friend.dat> 
INPUT:
    NULL
OUTPUT:
    retFriendIdx    : save position
RETURN:
    Success or failure
******************************************************************************/
static int winks_StrgSearchFriendEpt(unsigned short *retFriendIdx)
{
    unsigned short i;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    *retFriendIdx = WINKS_MAX_FIXED;
    Winks_assert(retFriendIdx != NULL);
    for(i=0; i< WINKS_FRIENDS_MAX_NUMBER; i++)
    {
        if( _FriendMap[i].NO[0] == 0)
        {
                *retFriendIdx = i;
                break;
        }
    }

    if(i == WINKS_FRIENDS_MAX_NUMBER)
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_GetCCInfo( unsigned short CC_Idx, 
                                  WinksStrgFile_CCInfo_s * retCCInfo,
                                  char *retPath, int PathSize)
{
    WinksStrgFile_CCInfo_s CCInfo;
    
    if( Winks_StrgGetCCInfo(CC_Idx, &CCInfo) == WINKS_RETURN_SUCCESS &&
        CCInfo.ID[0] )
    {
        if(retCCInfo)
            Winks_mem_cpy(retCCInfo, &CCInfo, sizeof(WinksStrgFile_CCInfo_s));
        if(retPath && PathSize)
            return winks_GetImgFilePath((int)(CC_Idx>>15), retPath, PathSize, (unsigned short)(CC_Idx&0x7fff), CCInfo.img_type);

        return WINKS_RETURN_SUCCESS;
    }

    return WINKS_RETURN_FAILURE;
}

typedef struct tag_WinksStrg_CCStat_s
{
    unsigned short  CCIdx;      /* 彩像保存index */
    unsigned short  UseCnt;     /* 电话事件数 */
    unsigned long   LatestUse;  /* 最近一次电话事件的时间 
                                   自2000-1-1 0:0:0起分钟数 */
}WinksStrg_CCStat_s;

/* 按彩像使用时间由远及近(即LatestUse由小到大顺序)排列 
   可用2分法等排序办法加速
*/
static int winks_StrgSearchPosInCcStatList(
    WinksStrg_CCStat_s *pCSList,
    int Count,
    WinksStrg_CCStat_s *pCSFind)
{
    int pos;
    int min, max, mid=0;

    if(!Count)
        return 0;
    min = 0;
    max = Count-1;

    if(pCSFind->LatestUse < pCSList[min].LatestUse)
        return min;
    else if(pCSFind->LatestUse >= pCSList[max].LatestUse)
        return max+1;

#ifdef SORTED_LIST_FOR_BSEARCH
    while(min<max)
    {
        mid = (min+max)>>1;

        if(pCSFind->LatestUse == pCSList[mid].LatestUse)
            return mid;
        else if(pCSFind->LatestUse > pCSList[mid].LatestUse)
            min = mid+1;
        else
            max = mid-1;
    }
    
	/* 从min,mid,max中找出最小值和最大值 */
	pos = min;
    
	if(min > mid)
		min = mid;
	if(min > max)
		min = max;
    
	if(max < pos)
		max = pos;
	if(max < mid)
		max = mid;
#endif /* SORTED_LIST_FOR_BSEARCH */

    for(pos=min; pos<=max; pos++)
    {
        if(pCSList[pos].LatestUse > pCSFind->LatestUse)
            break;
    }
	return pos;
}

static int winks_StrgCheckCCStat(
    WinksStrg_CCStat_s   * pCSList,
    int MaxCnt,
    unsigned int *retFreeSize)
{
    int     StrgLoc;

    int     ValidCCCnt;

    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT
    
    ValidCCCnt = 0;
    *retFreeSize = 0;
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        FILE  * fp;
        char    Path[WINKS_PATH_MAX];
        int     FsErr = 0;
        
        unsigned short  Idx;
        WinksStrgFile_CCInfo_s CCInfo;
        
        int     Pos, i;
        WinksStrg_CCStat_s     CS_New;

        
        if(winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
            continue;/* 认为没有存储卡 */
        
        fp = Winks_fopen(Path, "rb");
        if(fp == NULL)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }

        /* 跳过文件头和第0条记录 */
        FsErr = Winks_fseek(fp, (sizeof(WinksStrgFile_CCHead_s)+sizeof(WinksStrgFile_CCInfo_s)), WINKS_SEEK_SET);
        if(FsErr<0)
        {
            Winks_fclose(fp);
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }

        Idx=1;
        do
        {
            FsErr = Winks_fread(&CCInfo, sizeof(CCInfo), 1, fp);

            if((FsErr==1) && (CCInfo.ID[0]) )
            {
                char    Path1[WINKS_PATH_MAX];
                int     UsedCCFriendNum = 0;
                int     FsErr1;
                
                CS_New.CCIdx = (unsigned short)(StrgLoc? (Idx|0x8000) : Idx);/* 最高位置1表示在存储卡上 */
                if(_LockCCIdx && CS_New.CCIdx == _LockCCIdx)
                {
                    /* 锁定的彩像不能淘汰 */
                    Idx++;
                    continue;
                }
                else if(Winks_StrgCCUserNum(CS_New.CCIdx, &UsedCCFriendNum) == 0)
                {
                    /* 无人使用的彩像提前淘汰 */
                    unsigned int ImageSize;
                    
                    winks_GetImgFilePath(StrgLoc, Path1, sizeof(Path1), Idx, CCInfo.img_type);
                    if(Winks_fexist(Path1) == 0)
                    {
                        if( (FsErr1 = Winks_fgetsize(Path1, 1, &ImageSize)) < 0 ||
                            (FsErr1 = Winks_fdelete(Path1)) < 0 )
                        {
                            winks_StrgPrintFsErr(Path);
                        }
                        else
                        {
                            Winks_printf("Delete1 CC[0x%04x] size=%d %s\r\n", CS_New.CCIdx, ImageSize, Path1);
                            *retFreeSize += ImageSize;
                        }
                    }
                }
                else if(UsedCCFriendNum)
                {
                    /* 好友使用的彩像不能淘汰 */
                    Idx++;
                    continue;
                }
                CS_New.UseCnt = CCInfo.UseCnt;
                CS_New.LatestUse = CCInfo.LatestUse;
                
                Pos = winks_StrgSearchPosInCcStatList(pCSList,ValidCCCnt,&CS_New);
                if(Pos >= MaxCnt)/* 该号码最近通过电话，不考虑删除 */
                {
                    Idx++;
                    continue;
                }

                /* 移动后面数据，插入新号码 */
                for(i=ValidCCCnt-1; i>=Pos; i--)
                {
                    if(i+1<MaxCnt)
                    {
                        Winks_mem_cpy(&pCSList[i+1], &pCSList[i], sizeof(WinksStrg_CCStat_s));
                    }
                }
                Winks_mem_cpy(&pCSList[Pos], &CS_New, sizeof(WinksStrg_CCStat_s));
                
                if(ValidCCCnt<MaxCnt)
                    ValidCCCnt++;
            }
            Idx++;
        }while(FsErr==1);

        Winks_fclose(fp);
        
        if(FsErr<0)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }
    }

#ifdef WINKS_DEBUG
{
    int Cnt;
    Winks_printf("winks_StrgCheckCCStat MaxCnt=%d ValidCCCnt=%d\r\n", MaxCnt, ValidCCCnt);
    Winks_printf("CNT : [CC    ] USE , LATEST_USE\r\n");
    for(Cnt=0; Cnt<ValidCCCnt; Cnt++)
    {
        int loc = (pCSList[Cnt].CCIdx&0x8000) ? 1 : 0;
        unsigned short CCIdx = (unsigned short)(pCSList[Cnt].CCIdx &0x7fff);
        
        Winks_printf("%4d: [%d,%-4d] %-4d, %lu\r\n", Cnt, loc, CCIdx, pCSList[Cnt].UseCnt, pCSList[Cnt].LatestUse);
    }
}
#endif

    return ValidCCCnt;
}
#if 0
typedef struct tag_WinksStrg_CallStat_s
{
    unsigned long   LatestCallClock;  /* 最近一次电话事件的时刻 
                                         自2000-1-1 0:0:0起分钟数 */
    char            NoBcd[WINKS_STRG_NOBCD_LEN];      /* 电话号码 BCD-8421码 */
}WinksStrg_CallStat_s;

/* 按电话时间由近及远(即LatestCallClock由大到小顺序)排列 
   可用2分法等排序办法加速
*/
static int winks_StrgSearchPosInCallStatList(
    WinksStrg_CallStat_s *pCSList,
    int Count,
    WinksStrg_CallStat_s *pCSFind)
{
    int pos;
    int min, max, mid=0;

    if(!Count)
        return 0;
    min = 0;
    max = Count-1;

#ifdef SORTED_LIST_FOR_BSEARCH
    if(pCSFind->LatestCallClock > pCSList[min].LatestCallClock)
        return min;
    else if(pCSFind->LatestCallClock <= pCSList[max].LatestCallClock)
        return max+1;
    while(min<max)
    {
        mid = (min+max)>>1;

        if(pCSFind->LatestCallClock == pCSList[mid].LatestCallClock)
            return mid;
        else if(pCSFind->LatestCallClock < pCSList[mid].LatestCallClock)
            min = mid+1;
        else
            max = mid-1;
    }

	/* 从min,mid,max中找出最小值和最大值 */
	pos = min;
    
	if(min > mid)
		min = mid;
	if(min > max)
		min = max;
    
	if(max < pos)
		max = pos;
	if(max < mid)
		max = mid;

#endif /* SORTED_LIST_FOR_BSEARCH */
    for(pos=min; pos<=max; pos++)
    {
        if(pCSList[pos].LatestCallClock < pCSFind->LatestCallClock)
            break;
    }
	return pos;
}
#endif
int winks_StrgDeleteUnUsedCC(unsigned short *CCIdxLeft, int CCLeftCnt)
{
    int StrgLoc;
    int Cnt;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)
    {
        int             iRet;
        WinksFindAttrib_s FsAttrib;
        char    Path[WINKS_PATH_MAX];

        winks_GetDirPath_IMG(StrgLoc, Path, sizeof(Path));
        
        iRet = Winks_FindDirFirst(Path, 1, &FsAttrib);
        if(iRet < 0)
            continue;
        
        do{
            #if 0
            if((FsAttrib.Attrib & WINKS_FS_ATTR_IS_ASCII) == 0)
            {
                char cFileName[64];
                UnicodeToAnsii((char *)cFileName, (char *)FsAttrib.FileName);
                Winks_printf("FileName: %s\r\n", cFileName);
            }
            else
            {
                Winks_printf("FileName: %s\r\n", FsAttrib.FileName);
            }
            #endif
            if((FsAttrib.Attrib & WINKS_FS_ATTR_IS_DIR) == 0)
            {
                int     DeleteFlag = 0;
                int     Len;
                unsigned short CC_Idx;
                char    FileName[64];

                if((FsAttrib.Attrib & WINKS_FS_ATTR_IS_ASCII) == 0)
                    Winks_Unicode2AsciiString((char *)FsAttrib.FileName, FileName, sizeof(FileName));
                else
                    strcpy(FileName, FsAttrib.FileName);

                Len = strlen(PRE_IMG);
                if( memcmp(FileName, PRE_IMG, Len) ||
                    (FileName[Len+4] != '.') )/* 不是彩像文件，删除 */
                    DeleteFlag = 1;
                else
                {
                    CC_Idx = 0;
                    Cnt = 0;
                    while(FileName[Len])
                    {
                        if(FileName[Len] < '0' || FileName[Len] > '9')
                            break;
                        CC_Idx += (unsigned short)(FileName[Len]-'0');
                        
                        Cnt++;
                        if(Cnt == 4)
                            break;
                        Len++;
                        
                        CC_Idx *= 10;
                    }
                    if(Cnt != 4)
                        DeleteFlag = 1;
                    else
                    {
                        if(StrgLoc)
                            CC_Idx |= 0x8000;
                        for(Cnt=0; Cnt<CCLeftCnt; Cnt++)
                        {
                            if(CCIdxLeft[Cnt] == CC_Idx)
                                break;
                        }
                        if(Cnt == CCLeftCnt)
                            DeleteFlag = 1;
                    }
                }

                if(DeleteFlag)
                {
                    char    Path1[WINKS_PATH_MAX];
                    
                    strcpy(Path1, Path);
                    strcat(Path1, WINKS_PATH_SPACE);
                    strcat(Path1, FileName);

                    Winks_fdelete(Path1);
                }
            }
        }while((iRet = Winks_FindDirNext(&FsAttrib)) == WINKS_RETURN_SUCCESS);
        
        Winks_FindDirClose(&FsAttrib);
    }

    return WINKS_RETURN_SUCCESS;
}

#if 0
static int winks_StrgCheckCallEvt(
    WinksStrg_CallStat_s   * pCS,
    int MaxCnt)
{
    unsigned short Cnt;
    int CallEvtCnt,ValidCallCnt,Pos,i;
    WinksStrgFile_CallStat_s CallStat;
    WinksStrg_CallStat_s     CS_New;
#ifdef DELETE_UNUSED_IMAGE
    int     CCLeftCnt;
    unsigned short CCIdxLeft[WINKS_MAX_USER];
    CCLeftCnt = 0;
    Winks_mem_set(CCIdxLeft, 0, sizeof(unsigned short)*WINKS_MAX_USER);
#else
    /* 存储卡上彩像全部删除 */
    Winks_StrgDeleteDiskFiles(2);
#endif /* DELETE_UNUSED_IMAGE */
    
    Cnt = 0;
    CallEvtCnt = Winks_StrgCallEvtCnt();
    ValidCallCnt = 0;
    while(Cnt < CallEvtCnt)
    {
        int UserFound = 0;
        unsigned short UserIdx;
        WinksStrgFile_UserInfo_s UserInfo;
        WinksStrgFile_GreetInfo_s GreetInfo;
        
        Winks_StrgCallEvtGet(Cnt, &CallStat);
        if(!winks_IsValid_CS(&CallStat))
        {
            Cnt++;
            continue;
        }
        //if(CallStat.Flag & 0x01)
        {
            if(CallStat.CallCnt < 1)/* 没有通过电话的(不会有此种情况) */
                CS_New.LatestCallClock = 0;
            else if(CallStat.CallCnt == 1)/* 通过1次电话的 */
            {
                if(CallStat.CallTime < WINKS_INVALID_CALL_TIME)/* 通话时长小于10秒 */
                    CS_New.LatestCallClock = 1;
                else if(CallStat.CallTime < WINKS_IGNORE_CALL_TIME)/* 通话时长小于30秒 */
                    CS_New.LatestCallClock = 2;
                else
                    CS_New.LatestCallClock = 3;/* 通话时长大于30秒 */
            }
            else
                CS_New.LatestCallClock = CallStat.LatestCallClock + 10;
            Winks_mem_cpy(CS_New.NoBcd, CallStat.NoBcd, WINKS_STRG_NOBCD_LEN);
            
            if(Winks_StrgSearchUser(CallStat.NoBcd, &UserIdx, &UserInfo) == WINKS_STRG_SEARCH_RESULT_FOUND)
                UserFound = 1;
            
            if( UserFound && CS_New.LatestCallClock > 10)
            {
                /* 号码对应的彩像及问候语信息存在，可不考虑 */
                if( UserInfo.GrtOff && Winks_StrgCCIsExist(UserInfo.CCIdx, NULL) 
                #ifdef DELETE_UNUSED_IMAGE
                    && CCLeftCnt<WINKS_MAX_USER
                #endif
                    )
                {
                #ifdef DELETE_UNUSED_IMAGE
                    CCIdxLeft[CCLeftCnt++] = UserInfo.CCIdx;
                #endif
                    Cnt++;
                    continue;
                }
            }

            if(UserFound)
            {
                if(UserInfo.GrtOff)
                {
                    /* 删除问候语 */
                    Winks_mem_set(&GreetInfo.NoBcd, 0, WINKS_STRG_NOBCD_LEN);
                    Winks_StrgSetGreet(UserInfo.GrtOff, &GreetInfo);
                }
                /* 删除用户信息，与彩像问候语断链 */
                Winks_mem_set(&UserInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
                Winks_StrgSetUserInfo(UserIdx, &UserInfo);
            }
                
            Pos = winks_StrgSearchPosInCallStatList(pCS,ValidCallCnt,&CS_New);
            if(Pos >= MaxCnt)/* 该号码很久没有通电话，可不考虑 */
            {
                Cnt++;
                continue;
            }

            /* 移动后面数据，插入新号码 */
            for(i=ValidCallCnt-1; i>=Pos; i--)
            {
                if(i+1<MaxCnt)
                {
                    Winks_mem_cpy(&pCS[i+1], &pCS[i], sizeof(WinksStrg_CallStat_s));
                }
            }
            Winks_mem_cpy(&pCS[Pos], &CS_New, sizeof(WinksStrg_CallStat_s));
            
            if(ValidCallCnt<MaxCnt)
                ValidCallCnt++;
        }
        Cnt++;
    }
#ifdef WINKS_DEBUG
{
    int Cnt;
    Winks_printf("winks_StrgCheckCallEvt MaxCnt=%d\r\n", MaxCnt);
    Winks_printf("CNT : [NOBCD       ], LATEST_CALL\r\n");
    for(Cnt=0; Cnt<ValidCallCnt; Cnt++)
    {
        char *pBcd = pCS[Cnt].NoBcd;
        
        Winks_printf("%4d: [%02x%02x%02x%02x%02x%02x], %lu\r\n", Cnt, pBcd[0],pBcd[1],pBcd[2],
                                pBcd[3],pBcd[4],pBcd[5],pCS[Cnt].LatestCallClock);
    }
}
#endif

#ifdef DELETE_UNUSED_IMAGE
/* 删除没用的彩像 */
    if(CCLeftCnt)
        winks_StrgDeleteUnUsedCC(CCIdxLeft, CCLeftCnt);
#endif /* DELETE_UNUSED_IMAGE */
    return ValidCallCnt;
}
#endif
static int winks_StrgDeleteAssociate(unsigned short CC_Idx)
{
    int iRet;
    unsigned short  Idx;
    WinksStrgFile_UserInfo_s UserInfo;
    WinksStrgFile_CCInfo_s   CCInfo;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_mem_set(&CCInfo, 0, sizeof(CCInfo));
    if(Winks_StrgSetCCInfo(CC_Idx, &CCInfo) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    Winks_mem_set(&UserInfo, 0, sizeof(UserInfo));
    Idx = 1;
    while(Idx < _UserOccupy/*WINKS_MAX_USER*/)
    {
        /* 号码是否为空 */
        if(_UserMap[Idx].CCIdx == CC_Idx)
        {
            if(_UserMap[Idx].GrtOff)
            {
                _UserMap[Idx].CCIdx = 0;/* 保留记录但删除彩像Idx */
                iRet = Winks_StrgSetUserInfo(Idx, &_UserMap[Idx]);
            }
            else
                iRet = Winks_StrgSetUserInfo(Idx, &UserInfo);/* 删除记录 */
            if(iRet != WINKS_RETURN_SUCCESS)
            {
                return WINKS_RETURN_FAILURE;
            }
        }
        Idx++;
    }

    return WINKS_RETURN_SUCCESS;
}

static int winks_StrgCallEvtAddCCStatus(WinksStrgFile_CallInfo_s *pCallInfo)
{
    //int iRet;
    unsigned short  Idx;
    WinksStrgFile_UserInfo_s UsrInfo;
    WinksStrgFile_CCInfo_s  CCInfo;
    int     Found = 0;
    char    NoBcd[WINKS_STRG_NOBCD_LEN];      /* 电话号码 BCD-8421码 */

    Winks_assert(pCallInfo != NULL);
    Winks_assert(pCallInfo->NO[0]);
    
    Winks_PhoneNoStr2Bcd(pCallInfo->NO, strlen(pCallInfo->NO), WINKS_STRG_NOBCD_LEN, (unsigned char *)NoBcd);
    Winks_mem_set(&UsrInfo, 0, sizeof(UsrInfo));
    Winks_mem_set(&CCInfo, 0, sizeof(CCInfo));
    if( Winks_StrgSearchUser(NoBcd, &Idx, &UsrInfo) == WINKS_STRG_SEARCH_RESULT_FOUND &&
        Winks_StrgGetCCInfo(UsrInfo.CCIdx, &CCInfo) == WINKS_RETURN_SUCCESS )
    {
        if(pCallInfo->ID[0] == 0)
        {
            /* 用户对应，彩像ID为空不用比对 */
            Found = 1;
        }
        else if( strcmp(CCInfo.ID, pCallInfo->ID) == 0 )
        {
            /* 用户及彩像ID都对应 */
            Found = 1;
        }
    }
    
    if(!Found && pCallInfo->ID[0])
    {
        if( Winks_StrgSearchCC(pCallInfo->ID, &Idx) == WINKS_STRG_SEARCH_RESULT_FOUND &&
            Winks_StrgGetCCInfo(UsrInfo.CCIdx, &CCInfo) == WINKS_RETURN_SUCCESS )
        {
            /* 彩像ID都对应 */
            Found = 1;
        }
    }

    if(Found)
    {
        CCInfo.UseCnt++;
        if(pCallInfo->CallClock > CCInfo.LatestUse)
            CCInfo.LatestUse = pCallInfo->CallClock;

        Winks_StrgSetCCInfo(UsrInfo.CCIdx, &CCInfo);
    }
    
    return WINKS_RETURN_SUCCESS;
}

static int winks_IsValidGreetFile(int StrgLoc)
{
    int     Cnt;
    char    Path[WINKS_PATH_MAX];
    
    WinksStrgFile_GreetHead_s   GreetHead;

    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    for(Cnt=0; Cnt<WINKS_STRG_NOBCD_LEN; Cnt++)
    {
        if(_SelfNoBcd[Cnt])
            break;
    }
    if(Cnt == WINKS_STRG_NOBCD_LEN)
        return 0;
    
    if(winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
        return 0;/* 认为没有存储卡 */
    
    /* 保存删除后结果 */
    if(Winks_ReadFile2Buf(Path, 0,
                (unsigned char *)&GreetHead, sizeof(WinksStrgFile_GreetHead_s)) 
                != WINKS_RETURN_SUCCESS)
         return 0;

    if(memcmp(GreetHead.SelfNoBcd, _SelfNoBcd, WINKS_STRG_NOBCD_LEN))
        return 0;

    return 1;
}

static int winks_StrgFriendGetInfo(unsigned short Idx, WinksStrgFile_FriendInfo_s *pFriendInfo)
{
    char    Path[WINKS_PATH_MAX];

    Winks_assert(/*Idx>=0 && */Idx<WINKS_FRIENDS_MAX_NUMBER);
    Winks_assert(pFriendInfo);

    winks_GetFilePath_Friend(0, Path, sizeof(Path));
    return Winks_ReadFile2Buf(Path, Idx*sizeof(WinksStrgFile_FriendInfo_s),
                            (unsigned char *)pFriendInfo, sizeof(WinksStrgFile_FriendInfo_s));
}

static int winks_UserIsFriend(unsigned short UserIdx)
{
    unsigned short Idx;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    for(Idx=0; Idx< WINKS_MAX_FIXED; Idx++)
    {
        if(_FriendMap[Idx].NoStrgIdx == UserIdx)
            break;
    }

    if(Idx == WINKS_MAX_FIXED)
        return 0;

    return 1;
}

int winks_StrgClusterSize(int StrgLoc)
{
//    if(!StrgLoc)
//        return FS_SYS_CLUSTER_SIZE;
//    else
//        return FS_EXT_CLUSTER_SIZE;
	return 4096;
}
#if 0
static int winks_StrgAddFriendPreTake(char *pFriendNo, unsigned short *retUserIdx)
{
    int     iRet = WINKS_RETURN_SUCCESS;
    char    Path[WINKS_PATH_MAX];
    char    NoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned short UserIdx;
    WinksStrgFile_UserInfo_s UserInfo;
    WinksStrgFile_CCInfo_s CCInfo;
    unsigned int CCTakeup;

    Winks_assert(pFriendNo);
    Winks_assert(retUserIdx);
    
    if(Winks_PhoneNoStr2Bcd(pFriendNo, strlen(pFriendNo), WINKS_STRG_NOBCD_LEN, (unsigned char *)NoBcd) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    if( Winks_StrgSearchUser(NoBcd, &UserIdx, &UserInfo) != WINKS_STRG_SEARCH_RESULT_FOUND ||
        winks_GetCCInfo(UserInfo.CCIdx, &CCInfo, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS ||
        Winks_fexist(Path) < 0 )
    {
        if(Winks_StrgDiskIsFull())
            if(Winks_StrgFreeDiskSpace(WINKS_FLASHSPACE_RESERVED+EACH_CC_TAKEUP_SIZE) 
                                        != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        
        Winks_mem_set(&UserInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
        Winks_mem_set(&CCInfo, 0, sizeof(WinksStrgFile_CCInfo_s));
        strncpy(CCInfo.ID, pFriendNo, sizeof(CCInfo.ID));
        Winks_mem_cpy(UserInfo.NoBcd, NoBcd, WINKS_STRG_NOBCD_LEN);
        if( Winks_StrgSearchUserEpt(&UserIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY ||
            Winks_StrgSearchCCEpt(EACH_CC_TAKEUP_SIZE, &UserInfo.CCIdx) != WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY ||
            Winks_StrgSetUserInfo(UserIdx, &UserInfo) != WINKS_RETURN_SUCCESS ||
            Winks_StrgSetCCInfo(UserInfo.CCIdx, &CCInfo) != WINKS_RETURN_SUCCESS )
            return WINKS_RETURN_FAILURE;
        
        CCTakeup = WINKS_ALIGN(EACH_CC_TAKEUP_SIZE, winks_StrgClusterSize(UserInfo.CCIdx>>15));
        iRet = Winks_StrgPreTakeup(UserInfo.CCIdx, CCTakeup, NULL);
    }
    else
    {
        unsigned int FileSize;
        int FsErr;
        
        FsErr = Winks_fgetsize(Path, 1, &FileSize);

        if(FsErr <= 0 )
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }
        
        if(Winks_StrgDiskIsFull())
            if(Winks_StrgFreeDiskSpace(WINKS_FLASHSPACE_RESERVED+EACH_CC_TAKEUP_SIZE) 
                                        != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;

        FileSize = WINKS_ALIGN(FileSize, winks_StrgClusterSize(UserInfo.CCIdx>>15));
        CCTakeup = WINKS_ALIGN(EACH_CC_TAKEUP_SIZE, winks_StrgClusterSize(UserInfo.CCIdx>>15));
        if(CCTakeup > FileSize)
            iRet = Winks_StrgPreTakeup(UserInfo.CCIdx, CCTakeup-FileSize, NULL);
    }

    *retUserIdx = UserIdx;

    return iRet;
}

static int winks_StrgDelFriendPreTake(unsigned short UserIdx)
{
    WinksStrgFile_UserInfo_s UserInfo;
    int UsedCCFriendNum;

    Winks_assert(UserIdx > 0 && UserIdx < WINKS_MAX_USER);
    if(Winks_StrgGetUserInfo(UserIdx, &UserInfo) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    if(Winks_StrgCCUserNum(UserInfo.CCIdx, &UsedCCFriendNum) > 1)
        return WINKS_RETURN_SUCCESS;

    if(Winks_StrgPreTakeupDelete(UserInfo.CCIdx) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    return Winks_StrgDeleteCCImage(UserInfo.CCIdx);
}
#endif
///////////////////////////////////////////////////////////////////////////////
//           G L O B A L   F U N C T I O N  
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
DESCRIPTION:
    skip "+86" or '+' at begining of phone number string
INPUT:
    pNo     : phone number string
OUTPUT:
    NULL
RETURN:
    phone number string without '+'
******************************************************************************/
char * Winks_SkipPlus(char *pNo)
{
    if(pNo[0]=='+')
    {
        if((pNo[1] == '8') &&(pNo[2] == '6'))
            return &pNo[3];
        else
            return &pNo[1];
    }

    return pNo;
}

/*****************************************************************************
DESCRIPTION:
    calcute the power with call parameter,such as CallCnt\CallTime\LatestCallClock
INPUT:
    pCallStat   : call parameter
OUTPUT:
    NULL
RETURN:
    the power value
******************************************************************************/
unsigned long Winks_CalculatePower(WinksStrgFile_CallStat_s *pCallStat)
{
    return pCallStat->LatestCallClock;
}

/*****************************************************************************
DESCRIPTION:
    covert phone number to bcd encoded string
INPUT:
    PhoneNo : phone number string ['0','9']
    BcdSize : save bcd string buffer size
OUTPUT:
    retBcd  : save bcd string returned
RETURN:
    Success or failure
******************************************************************************/
int Winks_PhoneNoStr2Bcd(char *PhoneNo, int PhoneNoSize, int BcdSize, 
                                  unsigned char *retBcd)
{
    int     Len;
    unsigned char * pPNo;
    
    Winks_assert(PhoneNo != NULL);
    Winks_assert(PhoneNoSize>0);
    Winks_assert(retBcd != NULL);
    Winks_assert(BcdSize>0);

    Len = strlen(PhoneNo);
    if(PhoneNoSize>Len)
        PhoneNoSize = Len;
    pPNo = (unsigned char *)PhoneNo;
    Winks_mem_set(retBcd, 0, BcdSize);
    while(BcdSize && PhoneNoSize)
    {
        if((pPNo[PhoneNoSize-1] >= '0') && (pPNo[PhoneNoSize-1] <= '9'))
        {
            retBcd[BcdSize-1] |= pPNo[PhoneNoSize-1]-'0';
            PhoneNoSize--;
        }
        else
        {
            Winks_printf("PhoneNo2Bcd:%s %d %d\r\n", PhoneNo, PhoneNoSize, BcdSize);
            break;
        }

        if(PhoneNoSize == 0)
            break;
        
        if((pPNo[PhoneNoSize-1] >= '0') && (pPNo[PhoneNoSize-1] <= '9'))
        {
            retBcd[BcdSize-1] |= (pPNo[PhoneNoSize-1]-'0')<<4;
            PhoneNoSize--;
        }
        else
        {
            Winks_printf("PhoneNo2Bcd:%s %d %d\r\n", PhoneNo, PhoneNoSize, BcdSize);
            break;;
        }
        
        BcdSize--;
    }
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    covert bcd encoded string to phone number
INPUT:
    pNoBcd  : phone number bcd encoded
    PhoneNoSize : save phone number string buffer size
OUTPUT:
    retPhoneStr  : phone number string ['0','9'] returned
RETURN:
    Success or failure
******************************************************************************/
int Winks_PhoneNoBcd2Str(unsigned char *pNoBcd, int NoBcdSize, int PhoneNoSize,
                                  char *retPhoneStr)
{
    int     Cnt, retCnt, Begin;
    unsigned char NoBcd;

    Winks_assert(pNoBcd != NULL);
    Winks_assert(NoBcdSize>0);
    Winks_assert(PhoneNoSize>0);
    Winks_assert(retPhoneStr != NULL);
    
    Winks_mem_set(retPhoneStr, 0, PhoneNoSize);
    Begin = 0;
    Cnt = 0;
    retCnt=0;
    while(Cnt < NoBcdSize)
    {
        if(retCnt+1 >= PhoneNoSize)
            break;
        NoBcd = (unsigned char)(pNoBcd[Cnt]>>4);
        if(/*NoBcd>=0 && */NoBcd<=9)
        {
            if((Begin == 0) && (NoBcd > 0))/* 忽略前面的0 */
                Begin = 1;
            if(Begin)
                retPhoneStr[retCnt++] = (char)('0'+NoBcd);
        }
        else
            return WINKS_RETURN_FAILURE;

        if(retCnt+1 >= PhoneNoSize)
            break;
        NoBcd = (unsigned char)(pNoBcd[Cnt]&0x0f);
        if(/*NoBcd>=0 && */NoBcd<=9)
        {
            if((Begin == 0) && (NoBcd > 0))/* 忽略前面的0 */
                Begin = 1;
            if(Begin)
                retPhoneStr[retCnt++] = (char)('0'+NoBcd);
        }
        else
            return WINKS_RETURN_FAILURE;
        
        Cnt++;
    }
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    get the app path of handset or memory card
INPUT:
    StrgLoc : handset[0] or memory card[1]
    PathSize: path buffer size
OUTPUT:
    retAppPath  : the app path string
RETURN:
    Success or failure
******************************************************************************/
//不容许返回失败
int Winks_GetAppPath(int StrgLoc, char *retAppPath, int PathSize)
{
    int     iRet = 0;
    char    drv[8];

    Winks_assert(StrgLoc>=0 && StrgLoc<DRIVE_NUM);
    Winks_assert(retAppPath != NULL);
    Winks_assert(PathSize>=sizeof(WINKS_APP_PATH));
    
    Winks_mem_set(retAppPath, 0, PathSize);

    if(StrgLoc == 0)
        strcpy(retAppPath, WINKS_APP_PATH);
    else
    {
#ifdef WINKS_SUPPORT_MEMORY_CARD
    iRet = Winks_GetMemoryCardDriver(drv, sizeof(drv));
    if (iRet < 0)
    {
        //Winks_fail("GetAppPath");
        return WINKS_RETURN_FAILURE;
    }

    sprintf(retAppPath, "%s"WINKS_PATH_SPACE"Winks"WINKS_PATH_SPACE, drv);
#else
    Winks_fail("GetAppPath");
    return WINKS_RETURN_FAILURE;
#endif
    }
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    Convert system date time to cache time 
    that is the minutes since 2000-1-1 0:0
INPUT:
    pDateTime   : the system date time
OUTPUT:
    retCacheTime: the cache time
RETURN:
    Success or failure
******************************************************************************/
int Winks_SysTime2CacheTime(Winks_DateTime_s *pDateTime, unsigned long *retCacheTime)
{
    unsigned char MonthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned long   CacheTime;
    unsigned int    i;
    
    Winks_assert( pDateTime != NULL );
    Winks_assert( retCacheTime != NULL );

    *retCacheTime = CacheTime = 0;
    
    if( (pDateTime->year < 2000) ||
        (pDateTime->month < 1 || pDateTime->month > 12) ||
        (pDateTime->day<1 || pDateTime->day>31) )
    {
        return WINKS_RETURN_FAILURE;
    }
    
    /* 年累加天数 */
    for(i=2000; i<pDateTime->year; i++)
    {
        if( (((pDateTime->year%4) == 0) && ((pDateTime->year%100) != 0)) || 
             ((pDateTime->year%400) == 0) )/* 闰年: 四年一闰；百年不闰,四百年再闰 */
            CacheTime += 366; /* 天数 */
        else
            CacheTime += 365; /* 天数 */
    }
    
    /* 月累加天数 */
    for(i=1; i<pDateTime->month; i++)
        CacheTime += MonthDays[i];  /* 天数 */
    if( (((pDateTime->year%4) == 0) && ((pDateTime->year%100) != 0)) || 
         ((pDateTime->year%400) == 0) )/* 闰年: 四年一闰；百年不闰,四百年再闰 */
    {
        if(pDateTime->month >2)
            CacheTime += 1; /* 天数 */
    }

    /* 日累加天数 */
    CacheTime += pDateTime->day-1;    /* 天数 */

    /* 天数转化为分钟数 */
    CacheTime *= 24*60;/* 分钟数 */

    /* 时累加分钟数 */
    CacheTime += pDateTime->hour*60;
    CacheTime += pDateTime->sec;

    *retCacheTime = CacheTime;
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    get phone and card space
INPUT:
    NULL
OUTPUT:
    PhoneSpace  : phone space
    CardSpace   : card space
RETURN:
    Success or failure
******************************************************************************/
int Winks_StrgGetSpace(Winks_FlashSpace_s *PhoneSpace, Winks_FlashSpace_s *CardSpace)
{
    int iRet = WINKS_RETURN_SUCCESS;
    
    if(PhoneSpace)
    {
        Winks_mem_set(PhoneSpace, 0, sizeof(Winks_FlashSpace_s));
        iRet = Winks_GetFlashSpace(PhoneSpace);
        Winks_printf("Winks_StrgGetSpace Flash[%lu/%lu]\r\n", PhoneSpace->free, PhoneSpace->total);
    }
    if(iRet != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
#ifdef WINKS_SUPPORT_MEMORY_CARD
    if(CardSpace)
    {
        Winks_FlashSpace_MB_s flashInfoMB;

        Winks_mem_set(CardSpace, 0, sizeof(Winks_FlashSpace_s));
        Winks_mem_set(&flashInfoMB, 0, sizeof(Winks_FlashSpace_MB_s));
        iRet = Winks_GetMemoryCardSpace(&flashInfoMB);
        if(iRet != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        
        if(flashInfoMB.total > 4095)/* 超过4G */
            flashInfoMB.total = 4095;
        if(flashInfoMB.free > 4095)/* 超过4G */
            flashInfoMB.free = 4095;
        
        CardSpace->total = flashInfoMB.total<<20;/* MB*1024*1024 == xxB */
        CardSpace->free = flashInfoMB.free<<20;/* MB*1024*1024 == xxB */
        Winks_printf("Winks_StrgGetSpace MCard[%lu/%lu]\r\n", CardSpace->free, CardSpace->total);
    }
#endif /* WINKS_SUPPORT_MEMORY_CARD */
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    power on, if ENABLE status, Map file UserMap[],CCMap[],FriendMap[]
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgInitMapFile(void)
{
    int     iRet = WINKS_RETURN_SUCCESS;
    int     StrgLoc;
    int     Idx;
    FILE  * fp;
    char    Path[WINKS_PATH_MAX];
    int     FsErr = 0;
    
    int     CardStatus;
    WinksStrgFile_UserHead_s Head;
    WinksStrgFile_UserInfo_s UserInfo;
    WinksStrgFile_FriendInfo_s FriendInfo;
    WinksStrgFile_CCInfo_s   CCInfo;
    
#ifdef WINKS_UNSUPPORTED_GLOBAL_VAR
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT
#else
    pFileMap = (WinksFileMap_s *)get_strg_filemap();
#endif

    Winks_assert(_FriendMap == NULL);
    Winks_assert(_UserMap == NULL);

    Winks_printf("Winks_StrgInitMapFile start ...\r\n");
/* 检查及创建目录和文件 */
    for(StrgLoc = 0; StrgLoc < DRIVE_NUM; StrgLoc++)
    {
        if(winks_StrgCheckDir(StrgLoc) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }
    
    for(Idx = WINKS_STRG_FILE_ENUM_CALL; Idx < WINKS_STRG_FILE_ENUM_MAX; Idx++)
    {
        if(winks_StrgCheckFile(Idx, 1) != WINKS_RETURN_SUCCESS)
        {
            Winks_printf("winks_StrgCheckFile(%d) error!\r\n", Idx);
            return WINKS_RETURN_FAILURE;
        }
    }
    Winks_printf("Winks_StrgInitMapFile [1] dir and file ok!\r\n");

/* 申请文件映射的内存 */
    _FriendMap = (WinksMapFile_FriendInfo_s *)Winks_salAlloc(sizeof(WinksMapFile_FriendInfo_s)*WINKS_MAX_FIXED);
    if(_FriendMap == NULL)
        return WINKS_RETURN_FAILURE;
    Winks_mem_set(_FriendMap, 0, sizeof(WinksMapFile_FriendInfo_s)*WINKS_MAX_FIXED);
    for(Idx=0; Idx<WINKS_PRI_CC_MAX; Idx++)
    {
        strcpy(_FriendMap[WINKS_FRIENDS_MAX_NUMBER+Idx].NO, WINKS_PRI_CC_NO);
        _FriendMap[WINKS_FRIENDS_MAX_NUMBER+Idx].NO[1] += (char)Idx;
    }
    for(Idx=0; Idx<WINKS_BUS_CC_MAX; Idx++)
    {
        strcpy(_FriendMap[WINKS_FRIENDS_MAX_NUMBER+WINKS_PRI_CC_MAX+Idx].NO, WINKS_BUS_CC_NO);
        _FriendMap[WINKS_FRIENDS_MAX_NUMBER+WINKS_PRI_CC_MAX+Idx].NO[1] += (char)Idx;
    }
    _FriendNum = 0;

    _UserMap = (WinksStrgFile_UserInfo_s *)Winks_salAlloc(sizeof(WinksStrgFile_UserInfo_s)*WINKS_MAX_USER);
    if(_UserMap == NULL)
    {
        Winks_StrgDeinitMapFile();
        return WINKS_RETURN_FAILURE;
    }
    Winks_mem_set(_UserMap, 0, sizeof(WinksStrgFile_UserInfo_s)*WINKS_MAX_USER);
    _UserNum = 0;
    _UserOccupy = 0;

    Winks_mem_set(_SelfNoBcd, 0, WINKS_STRG_NOBCD_LEN);
    _CacheTime = 0;

 #ifdef WINKS_CCSHOW_USE_CCCACHE
    _CCCache = NULL;
    _CCCacheOff = 0;
    Winks_mem_set(_CCCacheNoBcd, 0, WINKS_STRG_NOBCD_LEN);
#endif
    _LockCCIdx = 0;
    Winks_printf("Winks_StrgInitMapFile [2] mapfile memory ok!\r\n");
    
/* 映射文件<user.dat>到内存UserMap */
    winks_GetFilePath_User(0, Path, sizeof(Path));
    fp = Winks_fopen(Path, "rb");
    if(fp == NULL)
    {
        Winks_StrgDeinitMapFile();
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    
    FsErr = Winks_fread(&Head, sizeof(Head), 1, fp);
    if(FsErr == 1)
    {
        _CacheTime = Head.CacheTime;
        Winks_mem_cpy(_SelfNoBcd, Head.SelfNoBcd, WINKS_STRG_NOBCD_LEN);

    }
    else/* if(FsErr<0)*/
    {
        Winks_fclose(fp);
        Winks_StrgDeinitMapFile();
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    
    FsErr = Winks_fread(_UserMap, sizeof(WinksStrgFile_UserInfo_s), WINKS_MAX_USER, fp);
    Winks_fclose(fp);
    if(FsErr < 0)
    {
        Winks_StrgDeinitMapFile();
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    Winks_assert(FsErr == WINKS_MAX_USER);
    Winks_printf("Winks_StrgInitMapFile [3] map user ok!\r\n");

/* 映射文件<friend.dat>到内存FriendMap */
    winks_GetFilePath_Friend(0, Path, sizeof(Path));
    fp = Winks_fopen(Path, "rb");
    if(fp == NULL)
    {
        Winks_StrgDeinitMapFile();
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }

    do
    {
        FsErr = Winks_fread(&FriendInfo, sizeof(WinksStrgFile_FriendInfo_s), 1, fp);
        if(FsErr == 1 && FriendInfo.NO[0])
        {
            unsigned short UserIdx;
            char    NoBcd[WINKS_STRG_NOBCD_LEN];
            
            if( (Winks_PhoneNoStr2Bcd(FriendInfo.NO, strlen(FriendInfo.NO), WINKS_STRG_NOBCD_LEN, (unsigned char *)NoBcd) == WINKS_RETURN_SUCCESS) &&
                (Winks_StrgSearchUser(NoBcd, &UserIdx, &UserInfo) == WINKS_STRG_SEARCH_RESULT_FOUND) )
                _FriendMap[_FriendNum].NoStrgIdx = UserIdx;
            
            strncpy(_FriendMap[_FriendNum].NO, FriendInfo.NO, WINKS_STRG_PHONE_LEN);
            _FriendNum++;
            
        }
    }while(FsErr == 1);
    
    Winks_fclose(fp);
    
    if(FsErr<0)
    {
        Winks_StrgDeinitMapFile();
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    Winks_printf("Winks_StrgInitMapFile [4] map friend ok!\r\n");

/* 检查存储卡状态 */
    CardStatus = Winks_StrgGetStatus();
    
/* 检查<user.dat>有无断链记录，无用记录删除 */
    Winks_mem_set(&UserInfo, 0, sizeof(UserInfo));
    Idx = 1;
    while(Idx < WINKS_MAX_USER)
    {
        int DeleteFlag = 0;
        /* 号码是否为空 */
        if(memcmp(_UserMap[Idx].NoBcd, UserInfo.NoBcd, WINKS_STRG_NOBCD_LEN-1) == 0)/* 前5字节为空 */
        {
            int     FriendIdx;
            unsigned char LastByte = _UserMap[Idx].NoBcd[WINKS_STRG_NOBCD_LEN-1];
            if((LastByte < WINKS_PRI_BCD_MIN) || (LastByte >= WINKS_BUS_BCD_MAX))/* 第6字节为空或非法 */
            {
                Idx++;
                continue;
            }
            else if(LastByte < WINKS_PRI_BCD_MAX)/* 片头 */
            {
                FriendIdx = WINKS_FRIENDS_MAX_NUMBER+LastByte-WINKS_PRI_BCD_MIN;
                _FriendMap[FriendIdx].NoStrgIdx = (unsigned short)Idx;
            }
            else if(LastByte >= WINKS_BUS_BCD_MIN)/* 运营商 */
            {
                FriendIdx = WINKS_FRIENDS_MAX_NUMBER+WINKS_PRI_CC_MAX+LastByte-WINKS_BUS_BCD_MIN;
                _FriendMap[FriendIdx].NoStrgIdx = (unsigned short)Idx;
            }
        }
        _UserOccupy = (unsigned short)(Idx+1);/* NoBcd非空则认为是有效的记录 */
        /* 彩像和问候语是否为空，断链记录，删除 */
        if( _UserMap[Idx].CCIdx == 0 &&
            _UserMap[Idx].GrtOff == 0 )
        {
            DeleteFlag = 1;
            iRet = Winks_StrgSetUserInfo((unsigned short)Idx, &UserInfo);/* 删除记录 */
        }
        else if(_UserMap[Idx].CCIdx)
        {
            if( (_UserMap[Idx].CCIdx & 0x8000 == 0) || /* 在手机上 */
                (CardStatus == WINKS_STRG_CARD_STATUS_OLD_CARD) || /* 在存储卡上 */
                (CardStatus == WINKS_STRG_CARD_STATUS_OK_CARD) )
            {
                if(!Winks_StrgCCIsExist(_UserMap[Idx].CCIdx, NULL))
                {
                    if(_UserMap[Idx].GrtOff)
                    {
                        _UserMap[Idx].CCIdx = 0;/* 保留记录但删除彩像Idx */
                        iRet = Winks_StrgSetUserInfo((unsigned short)Idx, &_UserMap[Idx]);
                    }
                    else
                    {
                        DeleteFlag = 1;
                        iRet = Winks_StrgSetUserInfo((unsigned short)Idx, &UserInfo);/* 删除记录 */
                    }
                }
            }
        }
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            Winks_StrgDeinitMapFile();
            return WINKS_RETURN_FAILURE;
        }

        if(!DeleteFlag)
            _UserNum++;

        Idx++;
    }
    Winks_printf("Winks_StrgInitMapFile [5] check user complete!\r\n");

/* 检查<ccinfo.dat>有无断链记录，无用记录删除 */
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        
        unsigned short  Idx;
        //unsigned short  CC_Idx;

        Idx = winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path));
        if(Idx != WINKS_RETURN_SUCCESS)
            continue;/* 认为没有存储卡 */

        fp = Winks_fopen(Path, "r+b");
        if(fp == NULL)
        {
            winks_StrgPrintFsErr(Path);
            continue;
        }
        
        FsErr = Winks_fseek(fp, (sizeof(WinksStrgFile_CCHead_s)+sizeof(WinksStrgFile_CCInfo_s)), WINKS_SEEK_SET);
        if(FsErr<0)
        {
            Winks_fclose(fp);
            Winks_StrgDeinitMapFile();
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }

        Idx = 1;
        do
        {
            FsErr = Winks_fread(&CCInfo, sizeof(CCInfo), 1, fp);
            if(FsErr==1 && CCInfo.ID[0])
            {
                winks_GetImgFilePath(StrgLoc, Path, sizeof(Path), Idx, CCInfo.img_type);
                if(Winks_fexist(Path) < 0)
                {
                    FsErr = Winks_fseek(fp, -sizeof(CCInfo), WINKS_SEEK_CUR);
                    if(FsErr>=0)
                    {
                        Winks_mem_set(&CCInfo, 0, sizeof(CCInfo));
                        FsErr = Winks_fwrite(&CCInfo, sizeof(CCInfo), 1, fp);
                    }
                }
            }
            Idx++;
        }while(FsErr == 1);
        
        Winks_fclose(fp);
        
        if(FsErr<0)
        {
            Winks_StrgDeinitMapFile();
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }
        
    } 
    Winks_printf("Winks_StrgInitMapFile [6] check ccinfo complete!\r\n");

/* 整理文件<greet.dat> */
    if(Winks_StrgTrimGreet() != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    Winks_printf("Winks_StrgInitMapFile complete!!!\r\n");
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    power off, free Map file UserMap[],CCMap[],FriendMap[]
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgDeinitMapFile(void)
{
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_FriendMap)
    {
        Winks_salFree(_FriendMap);
        _FriendMap = NULL;
        _FriendNum = 0;
    }
    if(_UserMap)
    {
        Winks_salFree(_UserMap);
        _UserMap = NULL;
        _UserNum = 0;
        _UserOccupy = 0;
    }

    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    set cache self phone number to <user.dat> and <ccinfo.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgSetSelfNo(char *pNo)
{
    int     StrgLoc;
    char    Path[WINKS_PATH_MAX];
    int     Cnt, Max;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        WinksStrgFile_GreetHead_s   GreetHead;
        
        if(Winks_PhoneNoStr2Bcd(pNo, strlen(pNo), WINKS_STRG_NOBCD_LEN, (unsigned char *)GreetHead.SelfNoBcd) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        
        if(winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
            continue;/* 认为没有存储卡 */

        /* 保存删除后结果 */
        if(Winks_WriteBuf2File(Path, sizeof(GreetHead.elem_cnt)+sizeof(GreetHead.valid_cnt),
                    (unsigned char *)GreetHead.SelfNoBcd, WINKS_STRG_NOBCD_LEN, 0) 
                    != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    Cnt = 0;
#ifdef WINKS_SUPPORT_MEMORY_CARD
    Max = 4;
#else
    Max = 3;
#endif
    while(Cnt < Max)
    {
        int iRet = WINKS_RETURN_FAILURE;
        WinksStrgFile_CallHead_s Head;

        if(Winks_PhoneNoStr2Bcd(pNo, strlen(pNo), WINKS_STRG_NOBCD_LEN, (unsigned char *)Head.SelfNoBcd) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        
        switch(Cnt)
        {
        case 0:
            iRet = winks_GetFilePath_Call(0, Path, sizeof(Path));
            break;
        case 1:
            iRet = winks_GetFilePath_Stat(0, Path, sizeof(Path));
            break;
        case 2:
            iRet = winks_GetFilePath_Greet(0, Path, sizeof(Path));
            break;
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        case 3:
            iRet = winks_GetFilePath_Greet(1, Path, sizeof(Path));
            break;
    #endif
        default:
            break;
        }
        Cnt++;
        if(iRet != WINKS_RETURN_SUCCESS)
        {
            continue;/* 认为没有存储卡 */
        }
        
        /* 保存删除后结果 */
        if(Winks_WriteBuf2File(Path, sizeof(Head.Version),
                    (unsigned char *)Head.SelfNoBcd, WINKS_STRG_NOBCD_LEN, 0) 
                    != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    return winks_StrgSetCacheFlag(pNo, 0);;
}

/*****************************************************************************
DESCRIPTION:
    set cache time to <user.dat> and <ccinfo.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgSetCacheTime(unsigned long NewCacheTime)
{
    return winks_StrgSetCacheFlag(NULL, NewCacheTime);
}

/*****************************************************************************
DESCRIPTION:
    check file
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgCheckDiskFiles(void)
{
    int StrgLoc, Idx;
    
    for(StrgLoc = 0; StrgLoc < DRIVE_NUM; StrgLoc++)
    {
        if(winks_StrgCheckDir(StrgLoc) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }
    
    for(Idx = WINKS_STRG_FILE_ENUM_CALL; Idx <= WINKS_STRG_FILE_ENUM_GREET; Idx++)
    {
        if(winks_StrgCheckFile(Idx, 0) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    delete all disk file
INPUT:
    Disk    : phone [1] 、 memory card[2] 、phone and card[3]
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgDeleteDiskFiles(int Disk)
{
    int     iRet = WINKS_RETURN_FAILURE;
    int     StrgLoc;
    int     MaxLoc;
    char    Path[WINKS_PATH_MAX];
    int     Idx;

    if((Disk&0x03) == 0x03)/* phone and card */
    {
        StrgLoc = 0;
        MaxLoc = DRIVE_NUM;
    }
    else if((Disk&0x01) == 0x01)/* phone */
    {
        StrgLoc = 0;
        MaxLoc = 1;
    }
    else if((Disk&0x02) == 0x02)/* card */
    {
        StrgLoc = 1;
        MaxLoc = DRIVE_NUM;
    }
    else
    {
        Winks_fail("DeleteDiskCC");
        return WINKS_RETURN_FAILURE;
    }

#ifdef WINKS_SUPPORT_MEMORY_CARD
    if((MaxLoc == DRIVE_NUM) && (Winks_IsMemoryCardExist() != 1))
        MaxLoc = 1;
#endif /* WINKS_SUPPORT_MEMORY_CARD */

    while(StrgLoc < MaxLoc)
    {
        iRet = winks_GetDirPath_IMG(StrgLoc, Path, sizeof(Path));
        if(!StrgLoc)
            return WINKS_RETURN_FAILURE;
        else
        {
            StrgLoc++;
            continue;/* 认为没有存储卡 */
        }
        Winks_XdeleteDir(Path);

        iRet = winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path));
        if(iRet == WINKS_RETURN_SUCCESS)
            Winks_fdelete(Path);

        if( ((Disk&0x03) == 0x03) || 
            !winks_IsValidGreetFile(StrgLoc) )/* phone and card */
        {
            iRet = winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path));
            if(iRet == WINKS_RETURN_SUCCESS)
                /*iRet = */Winks_fdelete(Path);
        }
        
        //if(iRet != WINKS_RETURN_SUCCESS)
        //    break;
        
        StrgLoc++;
    }
    
    if((Disk&0x03) == 0x03)/* phone and card */
    {
        Winks_StrgULDeleteAll();
        winks_GetFilePath_User(0, Path, sizeof(Path));
        iRet = Winks_fdelete(Path);
        winks_GetFilePath_Call(0, Path, sizeof(Path));
        iRet = Winks_fdelete(Path);
        winks_GetFilePath_Stat(0, Path, sizeof(Path));
        iRet = Winks_fdelete(Path);
    }
    
    if(Disk&0x01)/* phone */
    {
        for(Idx = WINKS_STRG_FILE_ENUM_CALL; Idx <= WINKS_STRG_FILE_ENUM_GREET; Idx++)
        {
            if(winks_StrgCheckFile(Idx, 1) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
    }

    if(MaxLoc == DRIVE_NUM)/* card */
    {
#ifdef WINKS_SUPPORT_MEMORY_CARD
        for(Idx = WINKS_STRG_FILE_ENUM_CCINFO_1; Idx < WINKS_STRG_FILE_ENUM_MAX; Idx++)
        {
            if(winks_StrgCheckFile(Idx, 1) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
#else
        return WINKS_RETURN_FAILURE;
#endif
    }
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    judge user is friend or not, 
    You can search in file <friend.dat> or FriendMap[]
INPUT:
    pNoBcd  : phone number string 
OUTPUT:
    retFriendIdx: friend index
RETURN:
    True or false
******************************************************************************/
int Winks_StrgUserIsFriend(char *pNo, unsigned short *retFriendIdx)
{
    unsigned short Idx;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pNo != NULL);
    for(Idx=0; Idx< WINKS_MAX_FIXED; Idx++)
    {
        if( _FriendMap[Idx].NO[0] &&
            winks_NoStrCompare( pNo, _FriendMap[Idx].NO ) == 0)
        {
            if(retFriendIdx)
                *retFriendIdx = Idx;
            
            if(Idx < WINKS_FRIENDS_MAX_NUMBER)
                return 1;
            else if(Idx < WINKS_FRIENDS_MAX_NUMBER + WINKS_PRI_CC_MAX)
                return 2;
            else
                return 3;
        }
    }
    return 0;
}

/*****************************************************************************
DESCRIPTION:
    Save friend info to file <friend.dat> 
INPUT:
    pNo     : friend phone number
    pNick   : friend nickname (Unicode encoded)
OUTPUT:
    NULL
RETURN:
    Success or failure
******************************************************************************/
int Winks_StrgFriendAdd(char *pNo, unsigned char *pNick)
{
    int     iRet;
    unsigned short FriendIdx;
    WinksStrgFile_FriendInfo_s FriendInfo;
    char    Path[WINKS_PATH_MAX];

    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pNo != NULL);

    winks_GetFilePath_Friend(0, Path, sizeof(Path));
    Winks_mem_set(&FriendInfo, 0, sizeof(WinksStrgFile_FriendInfo_s));
    strncpy(FriendInfo.NO, pNo, sizeof(FriendInfo.NO));
    if(pNick)
    {
        Winks_UnicodeStrncpy(FriendInfo.Nick, pNick, sizeof(FriendInfo.Nick));
    }
    /* 是否已在好友录中 */
    if(Winks_StrgUserIsFriend(FriendInfo.NO, &FriendIdx))
    {
        Winks_assert(FriendIdx<WINKS_FRIENDS_MAX_NUMBER);
        iRet = Winks_WriteBuf2File(Path, FriendIdx*sizeof(WinksStrgFile_FriendInfo_s),
                            (unsigned char *)&FriendInfo, sizeof(WinksStrgFile_FriendInfo_s), 0);
        return WINKS_RETURN_SUCCESS;
    }

    if(_FriendNum >= WINKS_FRIENDS_MAX_NUMBER)
        return WINKS_RETURN_FAILURE;
    
    /* 在好友文件中添加 */
    if(winks_StrgSearchFriendEpt(&FriendIdx) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    if( /*winks_StrgAddFriendPreTake(FriendInfo.NO, &UserIdx) == WINKS_RETURN_SUCCESS &&*/
        Winks_WriteBuf2File(Path, FriendIdx*sizeof(WinksStrgFile_FriendInfo_s),
                            (unsigned char *)&FriendInfo, sizeof(WinksStrgFile_FriendInfo_s), 0)
                            == WINKS_RETURN_SUCCESS )
    {
        //_FriendMap[_FriendNum].NoStrgIdx = UserIdx;
        strncpy(_FriendMap[FriendIdx].NO, FriendInfo.NO, WINKS_STRG_PHONE_LEN);
        _FriendNum++;

        return WINKS_RETURN_SUCCESS;
    }
    
    return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    Get friend count in <friend.dat> 
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    Friend count or error code
******************************************************************************/
int Winks_StrgFriendCnt(void)
{    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    return _FriendNum;
}

/*****************************************************************************
DESCRIPTION:
    Get friend info from file <friend.dat> 
INPUT:
    BeginIdx: friend index begin,valid range [0,WINKS_FRIENDS_MAX_NUMBER)
    NoLen   : max friend number length
    NickLen : max friend nick name length
OUTPUT:
    pNo     : friend phone number 
    pNick   : friend nickname
RETURN:
    next friend info index in file <friend.dat> 
    if <0 search end.
******************************************************************************/
int Winks_StrgFriendGet( int BeginIdx, 
                               char *pNo, int NoLen,
                               unsigned char *pNick, int NickLen)
{
    int iRet;
    char    Path[WINKS_PATH_MAX];

    WinksStrgFile_FriendInfo_s FriendInfo;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(BeginIdx<0 || BeginIdx >= WINKS_FRIENDS_MAX_NUMBER)
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
    
    if(_FriendNum == 0)
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
    
    Winks_assert(pNo);
    Winks_assert(NoLen);
    
    while(BeginIdx < WINKS_FRIENDS_MAX_NUMBER)
    {
        if(_FriendMap[BeginIdx].NO[0])
            break;
        BeginIdx++;
    }
    if(BeginIdx == WINKS_FRIENDS_MAX_NUMBER)
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
    
    winks_GetFilePath_Friend(0, Path, sizeof(Path));
    iRet = Winks_ReadFile2Buf(Path, BeginIdx*sizeof(WinksStrgFile_FriendInfo_s),
                            (unsigned char *)&FriendInfo, sizeof(WinksStrgFile_FriendInfo_s));

    if(iRet != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    
    if(BeginIdx+1 < WINKS_FRIENDS_MAX_NUMBER)
    {
        strncpy(pNo, FriendInfo.NO, NoLen);
        if(pNick && NickLen && FriendInfo.Nick[0])
            Winks_UnicodeStrncpy(pNick, FriendInfo.Nick, NickLen);
        
        return BeginIdx+1;
    }
    else
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
}

/*****************************************************************************
DESCRIPTION:
    delete friend info in file <friend.dat> with phone number,
INPUT:
    pNo : phone number 
OUTPUT:
    NULL
RETURN:
    Success or failure
******************************************************************************/
int Winks_StrgFriendDel(char *pNo)
{
    int     iRet;
    unsigned short  FriendIdx;
    char    Path[WINKS_PATH_MAX];

    unsigned short  UserIdx;
    WinksStrgFile_FriendInfo_s  FriendInfo;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_FriendNum == 0)
        return WINKS_RETURN_FAILURE;
    
    Winks_assert(pNo != NULL);
    /* 是否已在好友录中 */
    if(Winks_StrgUserIsFriend(pNo, &FriendIdx) != 1)
        return WINKS_RETURN_FAILURE;

    UserIdx = _FriendMap[FriendIdx].NoStrgIdx;
#if 0
    if(winks_StrgDelFriendPreTake(UserIdx) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
#endif
    /* 在好友文件中删除 */
    Winks_mem_set(&FriendInfo, 0, sizeof(WinksStrgFile_FriendInfo_s));
    winks_GetFilePath_Friend(0, Path, sizeof(Path));
    iRet = Winks_WriteBuf2File(Path, FriendIdx*sizeof(WinksStrgFile_FriendInfo_s),
                            (unsigned char *)&FriendInfo, sizeof(WinksStrgFile_FriendInfo_s), 0);

    if(iRet == WINKS_RETURN_SUCCESS)
    {
        Winks_mem_set(&_FriendMap[FriendIdx], 0, sizeof(WinksMapFile_FriendInfo_s));
        _FriendNum--;
    }
    
    return iRet;
}

/*****************************************************************************
DESCRIPTION:
    delete all friend info in file <friend.dat>,
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    Success or failure
******************************************************************************/
int Winks_StrgFriendDelAll(void)
{
    int     iRet;
//    unsigned short  FriendIdx;
    char    Path[WINKS_PATH_MAX];

    WinksStrgFile_FriendInfo_s FriendInfoArr[WINKS_FRIENDS_MAX_NUMBER];

    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_FriendNum == 0)
        return WINKS_RETURN_FAILURE;
#if 0
    for(FriendIdx=0; FriendIdx< WINKS_FRIENDS_MAX_NUMBER; FriendIdx++)
    {
        if(_FriendMap[FriendIdx].NO[0])
        {
            if(winks_StrgDelFriendPreTake(_FriendMap[FriendIdx].NoStrgIdx) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
    }
#endif
    /* 在好友文件中添加 */
    Winks_mem_set(_FriendMap, 0, sizeof(WinksMapFile_FriendInfo_s)*WINKS_FRIENDS_MAX_NUMBER);
    Winks_mem_set(&FriendInfoArr, 0, sizeof(WinksStrgFile_FriendInfo_s)*WINKS_FRIENDS_MAX_NUMBER);
    winks_GetFilePath_Friend(0, Path, sizeof(Path));
    iRet = Winks_WriteBuf2File( Path, 0, (unsigned char *)&FriendInfoArr, 
                                sizeof(WinksStrgFile_FriendInfo_s)*WINKS_FRIENDS_MAX_NUMBER, 0 );
    _FriendNum = 0;
    
    return iRet;
}

/*****************************************************************************
DESCRIPTION:
    search user info in file <user.dat> with phone number
    You can search in UserMap[] faster
INPUT:
    pNoBcd  : phone number string (bcd encoded)
OUTPUT:
    retUsrIdx:save pos in <user.dat>
    pUsrInfo: user info struct
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSearchUser(char *pNoBcd, unsigned short *retUsrIdx, WinksStrgFile_UserInfo_s *pUsrInfo)
{
    unsigned short  Idx;
        
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pNoBcd != NULL);
    Winks_assert(pUsrInfo != NULL);
    
    for(Idx=1; Idx<_UserOccupy/*WINKS_MAX_USER*/; Idx++)
    {
        if(!winks_NoBcdCompare((unsigned char *)_UserMap[Idx].NoBcd, (unsigned char *)pNoBcd))
        {
            Winks_mem_cpy(pUsrInfo, &_UserMap[Idx], sizeof(WinksStrgFile_UserInfo_s));
            break;
        }
    }

    if(Idx >= _UserOccupy/*WINKS_MAX_USER*/)/* 未找到 */
        return WINKS_STRG_SEARCH_RESULT_NOT_FOUND;
    else
    {
        *retUsrIdx = Idx;
        return WINKS_STRG_SEARCH_RESULT_FOUND;
    }
}

/*****************************************************************************
DESCRIPTION:
    search for CC save index pos in <ccinfo.dat> file with CC ID, 
INPUT:
    pCC_ID  : CC ID string
OUTPUT:
    retCCIdx  : if found output the index , 
    
RETURN:
    success return pos ,fail return error code
******************************************************************************/
int Winks_StrgSearchCC(char *pCC_ID, unsigned short *retCCIdx)
{
    int     StrgLoc;
        
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pCC_ID != NULL);
    Winks_assert(retCCIdx != NULL);
    
    *retCCIdx = 0;
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        int     iRet;
        char    Path[WINKS_PATH_MAX];
        
        unsigned short  retIdx;

        if(winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
            return WINKS_STRG_SEARCH_RESULT_FAIL;/* 认为没有存储卡 */

        if(StrgLoc)
        {
            WinksStrgFile_CCHead_s Head;
            
            if(Winks_ReadFile2Buf(Path, 0, (unsigned char *)&Head, sizeof(WinksStrgFile_CCHead_s)) 
                    != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
            if(memcmp(Head.SelfNoBcd, _SelfNoBcd, WINKS_STRG_NOBCD_LEN))
                return WINKS_STRG_SEARCH_RESULT_FAIL;
            
            if(Head.CacheTime != _CacheTime)
                return WINKS_STRG_SEARCH_RESULT_FAIL;
        }
        
        iRet = winks_StrgRecordSearch( Path, (sizeof(WinksStrgFile_CCHead_s)+sizeof(WinksStrgFile_CCInfo_s)),
                                    (void *)pCC_ID,
                                    sizeof(WinksStrgFile_CCInfo_s),
                                    winks_CCIDCompare,
                                    (int *)&retIdx,
                                    NULL );
        if(iRet ==  WINKS_STRG_SEARCH_RESULT_FOUND)
        {
            retIdx += 1;
            *retCCIdx = (unsigned short)(StrgLoc ? (retIdx|0x8000) : retIdx);/* 最高位置1表示在存储卡上 */
             return WINKS_STRG_SEARCH_RESULT_FOUND;
        }
        else if(iRet ==  WINKS_STRG_SEARCH_RESULT_FAIL)
            return iRet;
    }

    return WINKS_STRG_SEARCH_RESULT_FAIL;
}


/*****************************************************************************
DESCRIPTION:
    search save index pos in file <user.dat>
    You can search in UserMap[] faster
INPUT:
    NULL
OUTPUT:
    retUsrIdx  : output first empty position for save
RETURN:
    success return pos in <user.dat> ,fail return error code
******************************************************************************/
int Winks_StrgSearchUserEpt(unsigned short * retUsrIdx)
{
    unsigned short  Idx, FirstCCEpt = 0;
    char            EmptyNoBcd[WINKS_STRG_NOBCD_LEN];
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(retUsrIdx != NULL);
    
    Winks_mem_set(EmptyNoBcd, 0, WINKS_STRG_NOBCD_LEN);
    *retUsrIdx = 0;
    for(Idx=1; Idx<WINKS_MAX_USER; Idx++)
    {
        if(!memcmp(_UserMap[Idx].NoBcd, EmptyNoBcd, sizeof(_UserMap[Idx].NoBcd)))
        {
            *retUsrIdx = Idx;/* 记录第一个空位 */
            break;
        }
        if(!FirstCCEpt && (_UserMap[Idx].CCIdx == 0))
            FirstCCEpt = Idx;
    }

    if(*retUsrIdx == 0 && FirstCCEpt != 0)
        *retUsrIdx = FirstCCEpt;
    
    if(*retUsrIdx == 0)/* 未找到 */
        return WINKS_STRG_SEARCH_RESULT_FAIL;
    else
        return WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY;
}

/*****************************************************************************
DESCRIPTION:
    search first empty position for save CC index in <ccinfo.dat> file 
INPUT:
    FileSize  : cc file size
OUTPUT:
    retCCIdx  : output first empty position for save
RETURN:
    success return pos ,fail return error code
******************************************************************************/
int Winks_StrgSearchCCEpt(unsigned long FileSize, unsigned short *retCCIdx)
{
    int     iRet;
    int     StrgLoc;

    Winks_assert(retCCIdx != NULL);
    
    *retCCIdx = 0;
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        Winks_FlashSpace_s FlashSpace;
        FILE  * fp;
        char    Path[WINKS_PATH_MAX];
        int     FsErr = 0;
        
        unsigned short  Idx;
        WinksStrgFile_CCInfo_s CCInfo;

        if(StrgLoc == 0)
            iRet = Winks_StrgGetSpace(&FlashSpace, NULL);
        else
            iRet = Winks_StrgGetSpace(NULL, &FlashSpace);
        
        if( iRet != WINKS_RETURN_SUCCESS || 
            FlashSpace.free < WINKS_FLASHSPACE_RESERVED+FileSize )/* 手机空间不足 */
            continue;
        
        if(winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
            continue;/* 认为没有存储卡 */
        
        if(StrgLoc)
        {
            if(winks_StrgCheckDir(StrgLoc) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
            
            if(Winks_fexist(Path) < 0)
            {
#ifdef WINKS_SUPPORT_MEMORY_CARD
                for(Idx = WINKS_STRG_FILE_ENUM_CCINFO_1; Idx < WINKS_STRG_FILE_ENUM_MAX; Idx++)
                {
                    if(winks_StrgCheckFile(Idx, 1) != WINKS_RETURN_SUCCESS)
                        return WINKS_RETURN_FAILURE;
                }
#else
                return WINKS_RETURN_FAILURE;
#endif
            }
        }
        
        fp = Winks_fopen(Path, "r+b");
        if(fp == NULL)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }

        Idx=1;
        /* 跳过文件头和第0条记录 */
        FsErr = Winks_fseek(fp, (sizeof(WinksStrgFile_CCHead_s) + sizeof(WinksStrgFile_CCInfo_s)*Idx), WINKS_SEEK_SET);
        if(FsErr<0)
        {
            Winks_fclose(fp);
            winks_StrgPrintFsErr(Path);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }

        do
        {
            FsErr = Winks_fread(&CCInfo, sizeof(CCInfo), 1, fp);
            if(FsErr < 1)
                break;
            if((CCInfo.ID[0] == 0))
            {
                *retCCIdx = (unsigned short)(StrgLoc ? (Idx|0x8000) : Idx);/* 最高位置1表示在存储卡上 */
                break;
            }
            Idx++;
        }while(FsErr==1);

        if( (*retCCIdx == 0) && (FsErr >= 0) )/* 文件系统无错误 */
        {   
			/* 尝试在<ccinf.dat>预占CC Idx记录 */
			Winks_mem_set(&CCInfo, 0, sizeof(CCInfo));
			FsErr = Winks_fwrite(&CCInfo, sizeof(CCInfo), 1, fp);
			if(FsErr == 1)
				*retCCIdx = (unsigned short)(StrgLoc? (Idx|0x8000) : Idx);/* 最高位置1表示在存储卡上 */
        }

        Winks_fclose(fp);
        
        if(FsErr<0)
            winks_StrgPrintFsErr(Path);
        
        if(*retCCIdx)
            return WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY;
            
    }
    return WINKS_STRG_SEARCH_RESULT_FAIL;
}

/*****************************************************************************
DESCRIPTION:
    search for empty pos for save Greet string in file <greet.dat>
INPUT:
    GrtLen  : greet length
OUTPUT:
    retGrtOff  : output first empty position for save
RETURN:
    success return pos in <user.dat> ,fail return error code
******************************************************************************/
int Winks_StrgSearchGreetEpt(unsigned char GrtLen, unsigned short * retGrtOff)
{
    int     StrgLoc;
    
    Winks_assert(retGrtOff != NULL);
    
    *retGrtOff = 0;
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        FILE  * fp;
        char    Path[WINKS_PATH_MAX];
        int     FsErr = 0;
        
        unsigned long  Off;
        WinksStrgFile_GreetHead_s GreetHead;

        if(winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
            continue;/* 认为没有存储卡 */
        
        fp = Winks_fopen(Path, "r+b");
        if(fp == NULL)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }

        FsErr = Winks_fread(&GreetHead, sizeof(WinksStrgFile_GreetHead_s), 1, fp);
        if(FsErr != 1)
        {
            Winks_fclose(fp);
            winks_StrgPrintFsErr(Path);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }
        
        FsErr = Winks_fseek(fp, 0, WINKS_SEEK_END);
        if(FsErr == 0)
        {
            int     Occupy;
            char    Buf[256];
            
             FsErr = Winks_ftell(fp);
             if(FsErr<0)
            {
                Winks_fclose(fp);
                winks_StrgPrintFsErr(Path);
                return WINKS_STRG_SEARCH_RESULT_FAIL;
            }
             Off = (unsigned long)FsErr;
            Winks_assert((Off%16)==0);
            
            Winks_mem_set(Buf, 0, sizeof(Buf));
            Occupy = GrtLen + sizeof(WinksStrgFile_GreetInfo_s) - WINKS_STRG_GREET_LEN;
            Occupy = WINKS_ALIGN(Occupy, 16);
            Winks_assert(Occupy < 256);
            
            FsErr = Winks_fwrite(Buf, Occupy, 1, fp);
            if(FsErr == 1)
            {
                GreetHead.elem_cnt++;
                FsErr = Winks_fseek(fp, 0, WINKS_SEEK_SET);
                if(FsErr>=0)
                    FsErr = Winks_fwrite(&GreetHead, sizeof(WinksStrgFile_GreetHead_s), 1, fp);

                *retGrtOff = (unsigned short)(Off >> 4);
                if(StrgLoc)
                    *retGrtOff |= 0x8000;
            }
        }
        
        Winks_fclose(fp);

        if(FsErr<0)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_STRG_SEARCH_RESULT_FAIL;
        }

        if(*retGrtOff)
            return WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY;
    }

    return WINKS_STRG_SEARCH_RESULT_FAIL;
}

/*****************************************************************************
DESCRIPTION:
    get user num
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    user num 
******************************************************************************/
int Winks_StrgGetUserCnt(void)
{    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    return _UserNum;
}

/*****************************************************************************
DESCRIPTION:
    get user info from <user.dat> file at index pos
    You can search in UserMap[] faster, also got ID_Idx and Grt_Off
INPUT:
    Idx     : index pos
    pUsrInfo: user info struct
OUTPUT:
    NULL
RETURN:
    success or failure 
******************************************************************************/
int Winks_StrgGetUserInfo(unsigned short Idx, WinksStrgFile_UserInfo_s *pUsrInfo)
{    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pUsrInfo != NULL);
    
    Winks_mem_set(pUsrInfo, 0, sizeof(WinksStrgFile_UserInfo_s));
    
    if(Idx ==0 || Idx > WINKS_MAX_USER)
        return WINKS_RETURN_FAILURE;
    Winks_mem_cpy(pUsrInfo, &_UserMap[Idx], sizeof(WinksStrgFile_UserInfo_s));

    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    save user info to <user.dat> file at index pos ,
    also change UserMap[] 
INPUT:
    Idx     : index pos
    pUsrInfo: user info struct
OUTPUT:
    NULL
RETURN:
    success or failure 
******************************************************************************/
int Winks_StrgSetUserInfo(unsigned short Idx, WinksStrgFile_UserInfo_s *pUsrInfo)
{
    int     iRet; 
    char    Path[WINKS_PATH_MAX];
    
    unsigned long  Off;   
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(Idx ==0 || Idx > WINKS_MAX_USER)/* 超出预占范围 */
        return WINKS_RETURN_FAILURE;
    
    Winks_assert(pUsrInfo != NULL);
    
    winks_GetFilePath_User(0, Path, sizeof(Path));

    Off = sizeof(WinksStrgFile_UserHead_s) + Idx * sizeof(WinksStrgFile_UserInfo_s);
    iRet = Winks_WriteBuf2File(Path, Off, (unsigned char *)pUsrInfo, sizeof(WinksStrgFile_UserInfo_s), 0);
    
    if(iRet == WINKS_RETURN_SUCCESS)
    {
        int     Cmp1,Cmp2,Cmp3;
        char    EmptyNoBcd[WINKS_STRG_NOBCD_LEN];
        
        Winks_mem_set(EmptyNoBcd, 0, WINKS_STRG_NOBCD_LEN);

        Cmp1 = memcmp(_UserMap[Idx].NoBcd, EmptyNoBcd, WINKS_STRG_NOBCD_LEN);
        Cmp2 = memcmp(pUsrInfo->NoBcd, EmptyNoBcd, WINKS_STRG_NOBCD_LEN);
        Cmp3 = memcmp(pUsrInfo->NoBcd, _UserMap[Idx].NoBcd, WINKS_STRG_NOBCD_LEN);
        if(!Cmp3 && Cmp1)/* 修改问候语位置 */
        {
            Winks_assert(Cmp2);
            //Winks_assert(pUsrInfo->CCIdx == _UserMap[Idx].CCIdx);
        }
        else if(!Cmp1 && Cmp2)/* 当前为空，输入不为空，添加 */
        {
            if(_UserNum < WINKS_MAX_USER)
                _UserNum++;
        }
        else if(Cmp1 && !Cmp2)/* 当前不为空，输入为空，删除 */
        {
            if(_UserNum > 0)
                _UserNum--;
        }

        if(Cmp2 && (Idx+1)>_UserOccupy)
            _UserOccupy = (unsigned short)(Idx+1);
        
        /* 处理好友 */
    {
        unsigned short FriendIdx;
        char    PhoneNo[WINKS_STRG_PHONE_LEN];
        
        if(Winks_PhoneNoBcd2Str((unsigned char *)pUsrInfo->NoBcd, WINKS_STRG_NOBCD_LEN, 
                                 WINKS_STRG_PHONE_LEN, PhoneNo) == WINKS_RETURN_SUCCESS)
        {
            FriendIdx = 0;
            if( Winks_StrgUserIsFriend(PhoneNo, &FriendIdx) && 
                FriendIdx<WINKS_MAX_FIXED )
                _FriendMap[FriendIdx].NoStrgIdx = Idx;
        }
    }
        Winks_mem_cpy(&_UserMap[Idx], pUsrInfo, sizeof(WinksStrgFile_UserInfo_s));
    }
    return iRet;
}

/*****************************************************************************
DESCRIPTION:
    get CC info from <ccinfo.dat> file at index pos , 
INPUT:
    Idx     : index pos
    pCCInfo : CC info struct
OUTPUT:
    NULL
RETURN:
    success or failure 
******************************************************************************/
int Winks_StrgGetCCInfo(unsigned short Idx, WinksStrgFile_CCInfo_s *pCCInfo)
{
    int     StrgLoc = 0;

    char    Path[WINKS_PATH_MAX];
    
    unsigned long  Off;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pCCInfo != NULL);
    
    if((Idx&0x7fff) == 0)
        return WINKS_RETURN_FAILURE;
    
    if(Idx & 0x8000)
        StrgLoc = 1;
    Idx &= 0x7fff;

    if(winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
        return WINKS_STRG_CARD_STATUS_NOT_EXIST;/* 认为没有存储卡 */

    if(StrgLoc)
    {
        WinksStrgFile_CCHead_s Head;
        
        if(Winks_ReadFile2Buf(Path, 0, (unsigned char *)&Head, sizeof(WinksStrgFile_CCHead_s)) 
                != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
        if(memcmp(Head.SelfNoBcd, _SelfNoBcd, WINKS_STRG_NOBCD_LEN))
            return WINKS_STRG_CARD_STATUS_NEW_CARD;
        
        if(Head.CacheTime != _CacheTime)
            return WINKS_STRG_CARD_STATUS_OLD_CARD;
    }

    Off = sizeof(WinksStrgFile_CCHead_s)+ Idx * sizeof(WinksStrgFile_CCInfo_s);
    return Winks_ReadFile2Buf(Path, Off, (unsigned char *)pCCInfo, sizeof(WinksStrgFile_CCInfo_s));
}

/*****************************************************************************
DESCRIPTION:
    save CC info to <ccinfo.dat> file at index pos , 
INPUT:
    Idx     : index pos
    pCCInfo : CC info struct
OUTPUT:
    NULL
RETURN:
    success or failure 
******************************************************************************/
int Winks_StrgSetCCInfo(unsigned short Idx, WinksStrgFile_CCInfo_s *pCCInfo)
{
    int     StrgLoc = 0;

    char    Path[WINKS_PATH_MAX];
    
    unsigned long  Off;

    Winks_assert(pCCInfo != NULL);
    
    if(Idx & 0x8000)
        StrgLoc = 1;
    Idx &= 0x7fff;

    if(winks_GetFilePath_CCInfo(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;/* 认为没有存储卡 */
    
    Off = sizeof(WinksStrgFile_CCHead_s) + Idx * sizeof(WinksStrgFile_CCInfo_s);
    return Winks_WriteBuf2File(Path, Off, (unsigned char *)pCCInfo, sizeof(WinksStrgFile_CCInfo_s), 0);
}

/*****************************************************************************
DESCRIPTION:
    judge CC is exist , 
INPUT:
    Idx     : index pos
OUTPUT:
    retCCInfo   : cc info
RETURN:
    exist return 1, else 0
******************************************************************************/
int Winks_StrgCCIsExist(unsigned short Idx, WinksStrgFile_CCInfo_s * retCCInfo)
{
    char    Path[WINKS_PATH_MAX];
    
    if(winks_GetCCInfo(Idx, retCCInfo, Path, sizeof(Path)) == WINKS_RETURN_SUCCESS)
        return (Winks_fexist(Path) >= 0);
    
    return 0;
}

/*****************************************************************************
DESCRIPTION:
    get greet string from <greet.dat> file at index pos, 
INPUT:
    Off         : greet info offset in file <greet.dat> (Off= fp_off/16)
    pGreetInfo  : greet info struct
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgGetGreet(unsigned short Off, WinksStrgFile_GreetInfo_s *pGreetInfo)
{
    int     StrgLoc = 0;

    FILE  * fp;
    char    Path[WINKS_PATH_MAX];
    int     FsErr = 0;

    unsigned long   ulOff;

    Winks_assert(pGreetInfo != NULL);
    
    if(Off & 0x8000)
        StrgLoc = 1;
    Off &= 0x7fff;
    
    if(Off == 0)
        return WINKS_RETURN_FAILURE;

    if(winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;/* 认为没有存储卡 */
    
    fp = Winks_fopen(Path, "rb");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    
    Winks_mem_set(pGreetInfo, 0, sizeof(WinksStrgFile_GreetInfo_s));

    ulOff = Off;
    ulOff <<= 4;
    
    FsErr = Winks_fseek(fp, ulOff, WINKS_SEEK_SET);
    if(FsErr>=0)
        FsErr = Winks_fread(pGreetInfo, sizeof(WinksStrgFile_GreetInfo_s), 1, fp); 

    Winks_fclose(fp);
    
    if(FsErr<0)
         winks_StrgPrintFsErr(Path);
    
    if( FsErr<0 ||
        (pGreetInfo->Magic != WINKS_GREET_MAGIC) )
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    save greet string to <greet.dat> file at Off*16 pos,
INPUT:
    Off         : greet info offset in file <greet.dat> (Off= fp_off/16)
    pGreetInfo  : greet info struct, 
                  if (pGreetInfo->NoBcd) is empty, delete record
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSetGreet(unsigned short Off, WinksStrgFile_GreetInfo_s *pGreetInfo)
{
    int     StrgLoc = 0;

    FILE  * fp;
    char    Path[WINKS_PATH_MAX];
    int     FsErr = 0;

    WinksStrgFile_GreetHead_s   GreetHead;
    unsigned long   ulOff;
    char            EmptyNoBcd[WINKS_STRG_NOBCD_LEN];

    Winks_assert(pGreetInfo != NULL);
    Winks_assert(pGreetInfo->GreetLen <= WINKS_STRG_GREET_LEN);
    
    if(Off & 0x8000)
        StrgLoc = 1;
    Off &= 0x7fff;

    if(Off == 0)
        return WINKS_RETURN_FAILURE;
    
    if(winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;/* 认为没有存储卡 */

    
    fp = Winks_fopen(Path, "r+b");
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    
    FsErr = Winks_fread(&GreetHead, sizeof(WinksStrgFile_GreetHead_s), 1, fp);
    if(FsErr <= 0)
    {
        Winks_fclose(fp);
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    
    Winks_mem_set(EmptyNoBcd, 0, WINKS_STRG_NOBCD_LEN);
    if(memcmp(pGreetInfo->NoBcd, EmptyNoBcd, WINKS_STRG_NOBCD_LEN) == 0)
    {
        if(GreetHead.valid_cnt)
            GreetHead.valid_cnt--;
    }
    else
    {
        GreetHead.valid_cnt++;
        if(pGreetInfo->GreetLen == 0)
        {
            Winks_fclose(fp);
            return WINKS_RETURN_SUCCESS;
        }
    }

    ulOff = Off;
    ulOff <<= 4;
    
    FsErr = Winks_fseek(fp, ulOff, WINKS_SEEK_SET);
    if(FsErr>=0)
    {
        int     Occupy;
        
        pGreetInfo->Magic = WINKS_GREET_MAGIC;
        Occupy = pGreetInfo->GreetLen + sizeof(WinksStrgFile_GreetInfo_s) - WINKS_STRG_GREET_LEN;
        Occupy = WINKS_ALIGN(Occupy, 16);
        Winks_assert(Occupy < 256);
        pGreetInfo->Occupy = (unsigned char)Occupy;
        FsErr = Winks_fwrite(pGreetInfo, Occupy, 1, fp);
    }

    if(FsErr>0)
    {
        FsErr = Winks_fseek(fp, 0, WINKS_SEEK_SET);
        if(FsErr>=0)
            FsErr = Winks_fwrite(&GreetHead, sizeof(WinksStrgFile_GreetHead_s), 1, fp);
    }

    Winks_fclose(fp);
    
    if(FsErr<0)
         winks_StrgPrintFsErr(Path);
    
    if(FsErr<=0)
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    Trim file <greet.dat> ,
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgTrimGreet(void)
{
    int     StrgLoc;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        FILE  * fp;
        char    Path[WINKS_PATH_MAX];
        int     FsErr = 0;
        
        unsigned long  rOff = 0, wOff = 0;
        WinksStrgFile_GreetInfo_s GreetInfo;

        if(winks_GetFilePath_Greet(StrgLoc, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
            continue;/* 认为没有存储卡 */
        
        fp = Winks_fopen(Path, "r+b");
        if(fp == NULL)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }

        wOff = rOff = WINKS_ALIGN(sizeof(WinksStrgFile_GreetHead_s), 16);
        FsErr = Winks_fseek(fp, rOff, WINKS_SEEK_SET);
        if(FsErr < 0)
        {
            Winks_fclose(fp);
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }

        do
        {
            int     Occupy;
            unsigned short UserIdx;
            WinksStrgFile_UserInfo_s UserInfo;
            
            FsErr = Winks_fread(&GreetInfo, sizeof(GreetInfo)-sizeof(GreetInfo.GreetStr), 1, fp);
            if(FsErr <= 0)
                break;
            
            if(GreetInfo.Magic != WINKS_GREET_MAGIC)
                break;
            
            if( (Winks_StrgSearchUser(GreetInfo.NoBcd, &UserIdx, &UserInfo) == WINKS_STRG_SEARCH_RESULT_FOUND) &&
                (UserInfo.GrtOff == (rOff>>4)) &&/* 正确的纪录 */
                (rOff > wOff) )/* 前面有错误的纪录，将当前纪录移动到前面 */
            {
                /* 完全读取当前纪录 */
                FsErr = Winks_fread(GreetInfo.GreetStr, GreetInfo.GreetLen, 1, fp);
                if(FsErr <= 0)
                    break;
                FsErr = Winks_ftell(fp);/* 纪录读指针 */
                if(FsErr < 0)
                    break;
                rOff = (unsigned long)FsErr;
                FsErr = Winks_fseek(fp, wOff, WINKS_SEEK_SET);/* 移动写指针 */
                if(FsErr<0)
                    break;
                /* 写纪录 */
                Occupy = GreetInfo.GreetLen + sizeof(WinksStrgFile_GreetInfo_s) - WINKS_STRG_GREET_LEN;
                Occupy = WINKS_ALIGN(Occupy, 16);
                Winks_assert(Occupy < 256);
                GreetInfo.Occupy = (unsigned char)Occupy;
                FsErr = Winks_fwrite(&GreetInfo, Occupy, 1, fp);
                if(FsErr <= 0)
                    break;
                FsErr = Winks_ftell(fp);/* 纪录写指针 */
                if(FsErr < 0)
                    break;
                wOff = (unsigned long)FsErr;
                FsErr = Winks_fseek(fp, rOff, WINKS_SEEK_SET);/* 恢复读指针 */
                if(FsErr < 0)
                    break;
            }
            else
            {
                Occupy = GreetInfo.Occupy;
                Occupy -= sizeof(WinksStrgFile_GreetInfo_s) - WINKS_STRG_GREET_LEN;
                FsErr = Winks_fseek(fp, Occupy, WINKS_SEEK_CUR);/* 移动读指针 */
                if(FsErr <= 0)
                    break;
            }

        }while(FsErr>=0);
        
        Winks_fclose(fp);

        if(FsErr<0)
        {
            winks_StrgPrintFsErr(Path);
            return WINKS_RETURN_FAILURE;
        }
    }

    return WINKS_RETURN_SUCCESS;
}
#if 0
/*****************************************************************************
DESCRIPTION:
    PreTakeup image file
INPUT:
    Idx     : cc image index
    Size    : takeup size
    retTakeSize : return takeup size
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgPreTakeup( unsigned short Idx, unsigned long Size, 
                                unsigned long *retTakeSize )
{
    unsigned long AlignSize;
    char    Path[WINKS_PATH_MAX];

    if(Size == 0)
        return WINKS_RETURN_SUCCESS;
    
    AlignSize = WINKS_ALIGN(Size, winks_StrgClusterSize(Idx>>15));
    if(retTakeSize)
        *retTakeSize = AlignSize;
    winks_GetImgPtfPath((int)(Idx>>15), Path, sizeof(Path), (unsigned short)(Idx&0x7fff), 0/*img_type*/);
    return winks_PreTakeup(Path, AlignSize);
}

/*****************************************************************************
DESCRIPTION:
    Delete PreTakeup image file
INPUT:
    Idx     : cc image index
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgPreTakeupDelete(unsigned short Idx)
{
    char    Path[WINKS_PATH_MAX];

    winks_GetImgPtfPath((int)(Idx>>15), Path, sizeof(Path), (unsigned short)(Idx&0x7fff), 0/*img_type*/);
    return Winks_fdelete(Path);
}
#endif
#if 0
/*****************************************************************************
DESCRIPTION:
    Takeup .tmp image file
INPUT:
    Idx     : cc image index
    Size    : takeup size
    retTakeSize : return takeup size
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgTmpTakeup(unsigned short Idx, unsigned long Size,
                                unsigned long *retTakeSize )
{
    unsigned long AlignSize;
    char    Path[WINKS_PATH_MAX];

    if(Size == 0)
        return WINKS_RETURN_SUCCESS;
    
    AlignSize = WINKS_ALIGN(Size, winks_StrgClusterSize(Idx>>15));
    if(retTakeSize)
        *retTakeSize = AlignSize;
    winks_GetImgTmpPath((int)(Idx>>15), Path, sizeof(Path), (unsigned short)(Idx&0x7fff), 0/*img_type*/);
    return winks_PreTakeup(Path, AlignSize);
}
#endif
/*****************************************************************************
DESCRIPTION:
    Delete .tmp image file
INPUT:
    Idx     : cc image index
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgTmpTakeupDelete(unsigned short Idx)
{
    char    Path[WINKS_PATH_MAX];

    winks_GetImgTmpPath((int)(Idx>>15), Path, sizeof(Path), (unsigned short)(Idx&0x7fff), 0/*img_type*/);
    return Winks_fdelete(Path);
}

/*****************************************************************************
DESCRIPTION:
    begin save CC image: create file and init CCImage struct
INPUT:
    Idx     : index pos
    pCCImage: CC Image info struct
    IfCached: memory cache CC Image Data
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSaveCCImageBegin(unsigned short Idx, WinksStrgFile_CCImage_s *pCCImage, int IfCached)
{
    int     StrgLoc = 0;
//    unsigned short FriendIdx;
    
    FILE  * fp;
    char    Path[WINKS_PATH_MAX];
    int     FsErr = 0;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pCCImage != NULL);
    
    if(Idx & 0x8000)
        StrgLoc = 1;
    Idx &= 0x7fff;

#ifdef WINKS_CCSHOW_USE_CCCACHE
    if(IfCached && pCCImage->FileSize && pCCImage->img_cnt==1)
    {
        if(_CCCache != NULL)Winks_salFree(_CCCache);
        _CCCache = (unsigned char *)Winks_salAlloc(pCCImage->FileSize);
        _CCCacheOff = 0;
    }
#endif
#if 0
    if(Winks_StrgUserIsFriend(pCCImage->NO, &FriendIdx))
    {
        unsigned long AlignSize = WINKS_ALIGN(pCCImage->FileSize, winks_StrgClusterSize(Idx>>15));
        unsigned long CCTakeup = WINKS_ALIGN(EACH_CC_TAKEUP_SIZE, winks_StrgClusterSize(Idx>>15));

        if(CCTakeup > AlignSize)
        {
            if(Winks_StrgPreTakeup(Idx, CCTakeup-AlignSize, NULL) != WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_FAILURE;
        }
    }
#endif
    winks_GetImgTmpPath(StrgLoc, Path, sizeof(Path), Idx, 0/*pCCImage->img_type*/);

    fp = Winks_fopen(Path, "wb"/*"r+b"*/);
    if(fp == NULL)
    {
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }

    if(FsErr < 0)
    {
        Winks_fclose(fp);
        pCCImage->file_handle = 0;
        winks_StrgPrintFsErr(Path);
        return WINKS_RETURN_FAILURE;
    }
    
    pCCImage->file_handle = (int)fp;
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    save CC image data to file
INPUT:
    Idx     : index pos
    pCCImage: CC Image info struct
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSaveCCImageDo(unsigned short Idx, WinksStrgFile_CCImage_s *pCCImage)
{
    FILE  * fp;
    int     FsErr = 0;
    unsigned long   wsize = 0;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pCCImage != NULL);
    
    if(pCCImage->file_handle == 0 || pCCImage->pMem == NULL)
        return WINKS_RETURN_FAILURE;
    
    fp = (FILE *)pCCImage->file_handle;

    while( pCCImage->pMem && 
           pCCImage->pMem->pdata && 
           pCCImage->pMem->offset && 
           wsize < pCCImage->blocklen )
    {
    #ifdef WINKS_CCSHOW_USE_CCCACHE
        if(_CCCache)
            Winks_mem_cpy(&_CCCache[_CCCacheOff], pCCImage->pMem->pdata, pCCImage->pMem->offset);
    #endif
        FsErr = Winks_fwrite(pCCImage->pMem->pdata, pCCImage->pMem->offset, 1, fp);
        if(FsErr < 0)
        {
            winks_StrgPrintFsErr("");
            return WINKS_RETURN_FAILURE;
        }
        
        wsize += pCCImage->pMem->offset;
        pCCImage->pMem = pCCImage->pMem->pnext;
    }
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    save CC image to file end: close file and rename '.tmp'
INPUT:
    Idx     : index pos
    pCCImage: CC Image info struct
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSaveCCImageEnd(unsigned short Idx, WinksStrgFile_CCImage_s *pCCImage)
{
    int     iRet;
    
    FILE  * fp;
    char    Path[WINKS_PATH_MAX];
    char    PathRN[WINKS_PATH_MAX];
    //int     FsErr = 0;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pCCImage != NULL);
#ifdef WINKS_CCSHOW_USE_CCCACHE
    if(_CCCache && pCCImage->NO)/* 下载结束纪录电话号码 */
        Winks_PhoneNoStr2Bcd(pCCImage->NO, strlen(pCCImage->NO), WINKS_STRG_NOBCD_LEN, (unsigned char *)_CCCacheNoBcd);
#endif
    if(pCCImage->file_handle == 0)
        return WINKS_RETURN_FAILURE;
    
    fp = (FILE *)pCCImage->file_handle;

    Winks_fclose(fp);

    winks_GetImgTmpPath(Idx>>15, Path, sizeof(Path), (unsigned short)(Idx&0x7fff), 0/*pCCImage->img_type*/);
    winks_GetImgFilePath(Idx>>15, PathRN, sizeof(PathRN), (unsigned short)(Idx&0x7fff), pCCImage->img_type);
    iRet = Winks_frename(Path, PathRN);

    return iRet;
}
#ifdef WINKS_CCSHOW_USE_CCCACHE
/*****************************************************************************
DESCRIPTION:
    free CC image cache
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgFreeCCImageCache(void)
{    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_CCCache)
        Winks_salFree(_CCCache);
    _CCCacheOff = 0;
    Winks_mem_set(_CCCacheNoBcd, 0, WINKS_STRG_NOBCD_LEN);
    
    return WINKS_RETURN_SUCCESS;
}
#endif
/*****************************************************************************
DESCRIPTION:
    get default show cc info
INPUT:
    dir     : direction -- call in/out
OUTPUT:
    pCCShow : show cc info
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgGetDftCCShow(int dir, WinksStrgFile_CCShow_s *pCCShow)
{
    strncpy(pCCShow->NO, WINKS_DFT_CC_NO, sizeof(pCCShow->NO));
    pCCShow->SaveType = 0;
    pCCShow->img_cnt = 1;
    winks_GetImgDftPath(pCCShow->CCImgPath[0], sizeof(pCCShow->CCImgPath[0]));
    Winks_UnicodeStrncpy((unsigned char *)pCCShow->GreetStr, (unsigned char *)WINKS_DFT_GREET, sizeof(pCCShow->GreetStr));
    pCCShow->GreetLen = (WK_STRLEN((char *)pCCShow->GreetStr)+1)<<1;
    return WINKS_RETURN_SUCCESS;
}
/*****************************************************************************
DESCRIPTION:
    get call number show cc info
INPUT:
    pNo     : phone number string
    dir     : direction -- call in/out
OUTPUT:
    pCCShow : show cc info
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow)
{
    unsigned char   NoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned short  UserIdx;
    WinksStrgFile_UserInfo_s UsrInfo;
    unsigned short  FriendIdx;
    WinksStrgFile_FriendInfo_s FriendInfo;
    int CCImageFound = 0;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    Winks_assert(pNo);
    Winks_assert(dir>=0 && dir<=1);
    Winks_assert(pCCShow);

    Winks_mem_set(pCCShow, 0, sizeof(WinksStrgFile_CCShow_s));
    strncpy(pCCShow->NO, pNo, sizeof(pCCShow->NO));
    pCCShow->SaveType = 0;

    if( Winks_StrgUserIsFriend(pCCShow->NO, &FriendIdx) == 1 &&
        winks_StrgFriendGetInfo(FriendIdx, &FriendInfo) == WINKS_RETURN_SUCCESS &&
        FriendInfo.Nick[0] /*&&
        winks_NoStrCompare(pCCShow->NO, FriendInfo.NO) == 0*/ )
    {
        strncpy(pCCShow->Name, (char *)FriendInfo.Nick, sizeof(pCCShow->Name));
    }
    else
    {
        /* TODO: 从电话簿读取 */
        //strncpy(pCCShow->Name, FriendInfo.Nick, sizeof(pCCShow->Name));
    }
    
    Winks_PhoneNoStr2Bcd(pCCShow->NO, strlen(pCCShow->NO), WINKS_STRG_NOBCD_LEN, NoBcd);
#ifdef WINKS_CCSHOW_USE_CCCACHE
    if(_CCCache)
    {
        if(!winks_NoBcdCompare((unsigned char *)NoBcd, (unsigned char *)_CCCacheNoBcd))
        {
            Winks_printf("Winks_StrgGetCCShow found in _CCCache %s size=%d\r\n", pCCShow->NO, _CCCacheOff);
            pCCShow->SaveType = 1;
            pCCShow->pCCImage = _CCCache;
            pCCShow->CCImageSize = _CCCacheOff;
            CCImageFound = 1;
        }
    }
#endif
    if(Winks_StrgSearchUser((char *)NoBcd, &UserIdx, &UsrInfo) == WINKS_RETURN_SUCCESS)
    {
        WinksStrgFile_GreetInfo_s GreetInfo;
        if(Winks_StrgGetGreet(UsrInfo.GrtOff, &GreetInfo) == WINKS_RETURN_SUCCESS)
        {
            Winks_mem_set(pCCShow->GreetStr, 0, sizeof(pCCShow->GreetStr));
            Winks_mem_cpy(pCCShow->GreetStr, GreetInfo.GreetStr, GreetInfo.GreetLen);
            pCCShow->GreetLen = GreetInfo.GreetLen;
        }

        if(!CCImageFound)
        {
            WinksStrgFile_CCInfo_s  CCInfo;
            
            if( Winks_StrgGetCCInfo(UsrInfo.CCIdx, &CCInfo) == WINKS_RETURN_SUCCESS &&
                CCInfo.ID[0] )
            {
                int StrgLoc = 0;
                unsigned short Idx = UsrInfo.CCIdx;
                
                if(Idx & 0x8000)
                    StrgLoc = 1;
                Idx &= 0x7fff;

                winks_GetImgFilePath(StrgLoc, pCCShow->CCImgPath[0], sizeof(pCCShow->CCImgPath[0]), Idx, CCInfo.img_type);
                if(Winks_fexist(pCCShow->CCImgPath[0]) == 0)
                    CCImageFound = 1;
                strncpy(pCCShow->ID, CCInfo.ID, sizeof(pCCShow->ID));
                pCCShow->img_type = CCInfo.img_type;
                pCCShow->img_cnt = CCInfo.img_cnt;
                pCCShow->interval = CCInfo.interval;
                pCCShow->CC_Idx = UsrInfo.CCIdx;
            }
        }
    }

    if(CCImageFound)
        return WINKS_RETURN_SUCCESS;
    else
        return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    judge friend use the cc or not, 
    You can search in FriendMap[] then <user.dat>
INPUT:
    CC_Idx  : cc index in <ccinfo.dat>
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgCCIsFriendUsed(unsigned short CC_Idx)
{
    unsigned short Idx;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    for(Idx=0; Idx< WINKS_MAX_FIXED; Idx++)
    {
        if( _FriendMap[Idx].NO[0] && (_FriendMap[Idx].NoStrgIdx < WINKS_MAX_USER) )
        {
            if(_UserMap[_FriendMap[Idx].NoStrgIdx].CCIdx == CC_Idx)
            {
                if(Idx < WINKS_FRIENDS_MAX_NUMBER)
                    return 1;
                else if(Idx < WINKS_FRIENDS_MAX_NUMBER+WINKS_PRI_CC_MAX)
                    return 2;
                else
                    return 3;
            }
        }
    }
    return 0;
}

/*****************************************************************************
DESCRIPTION:
    Get cc user number
INPUT:
    CC_Idx  : cc index in <ccinfo.dat>
OUTPUT:
    retFriendUsedNum    : Friend number of use the cc 
RETURN:
    cc user number
******************************************************************************/
int Winks_StrgCCUserNum(unsigned short CC_Idx, int *retFriendUsedNum)
{
    int UserNum = 0;
    unsigned short Idx;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if((CC_Idx & 0x7fff) == 0)
        return 0;

    if(retFriendUsedNum)
        *retFriendUsedNum = 0;
    for(Idx=1; Idx< _UserOccupy/*WINKS_MAX_USER*/; Idx++)
    {
        if(_UserMap[Idx].CCIdx == CC_Idx)
        {
            if(retFriendUsedNum && winks_UserIsFriend(Idx))
                *retFriendUsedNum += 1;
            UserNum++;
        }
    }
    
    return UserNum;
}

/*****************************************************************************
DESCRIPTION:
    get CC image size,
INPUT:
    Idx     : cc index in <ccinfo.dat>
OUTPUT:
    retImgSize  : cc image size
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgGetCCImageSize(unsigned short CC_Idx, unsigned int *retImgSize)
{
    WinksStrgFile_CCInfo_s  CCInfo;
    char    Path[WINKS_PATH_MAX];
    //unsigned int    FileSize;
    int     FsErr;
    
    if( winks_GetCCInfo(CC_Idx, &CCInfo, Path, sizeof(Path)) == WINKS_RETURN_SUCCESS )
    {
        FsErr = Winks_fgetsize(Path, 1, retImgSize);

        if(FsErr < 0)
            winks_StrgPrintFsErr(Path);
        else
            return WINKS_RETURN_SUCCESS;
    }

    return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    delete CC image ,
    also need update <ccinfo.dat>,<greet.dat> and <user.dat>
INPUT:
    Idx     : cc index in <ccinfo.dat>
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgDeleteCCImage(unsigned short CC_Idx)
{
    WinksStrgFile_CCInfo_s  CCInfo;
    char    Path[WINKS_PATH_MAX];
    int     FsErr = 0;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_LockCCIdx && _LockCCIdx == CC_Idx)
        return WINKS_RETURN_SUCCESS;
    
    if(winks_GetCCInfo(CC_Idx, &CCInfo, Path, sizeof(Path)) == WINKS_RETURN_SUCCESS)
    {
        if(Winks_fexist(Path)>=0)
            FsErr = Winks_fdelete(Path);
        
        if(FsErr<0)
            winks_StrgPrintFsErr(Path);
        else
            return winks_StrgDeleteAssociate(CC_Idx);
    }

    return WINKS_RETURN_FAILURE;
}

#if 0
/*****************************************************************************
DESCRIPTION:
    Trim the cc cache ,
        delete unused cc and trim a need download cc list.
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    need download cc count or failure
******************************************************************************/
int Winks_StrgTrimCCCache()
{
    WinksStrg_CallStat_s   * pCS = NULL;
    int MaxUser;
    int ValidCallCnt, Cnt;
    char    PhoneNumber[WINKS_STRG_PHONE_LEN];

    /* 尝试申请足够内存保存需要更新的号码 */
    MaxUser = WINKS_MAX_USER;
    while(pCS == NULL)
    {
        pCS = (WinksStrg_CallStat_s *)Winks_salAlloc(MaxUser*sizeof(WinksStrg_CallStat_s));
        if(pCS == NULL)
        {
            MaxUser >>= 1;
            if(MaxUser<WINKS_MIN_USER)
                return WINKS_RETURN_FAILURE;
        }
    }
    
    Winks_mem_set(pCS, 0, MaxUser*sizeof(WinksStrg_CallStat_s));

    ValidCallCnt = winks_StrgCheckCallEvt(pCS, MaxUser);

    Winks_StrgFLDeleteAll();
    for(Cnt=0; Cnt<ValidCallCnt; Cnt++)
    {
        Winks_PhoneNoBcd2Str( (unsigned char *)pCS[Cnt].NoBcd, WINKS_STRG_NOBCD_LEN, 
                              WINKS_STRG_PHONE_LEN, PhoneNumber);
        if(Winks_StrgFLAdd(PhoneNumber) != WINKS_RETURN_SUCCESS)
        {
            break;
        }
    }
    
    Winks_salFree(pCS);
    return Cnt;
}
#endif
/*****************************************************************************
DESCRIPTION:
    delete CC image to free disk size given.
INPUT:
    FreeSize: need to free size
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgFreeDiskSpace(unsigned long FreeSize)
{
    //int     iRet = WINKS_RETURN_FAILURE;
    WinksStrg_CCStat_s *pCSList = NULL;
    int ValidCCCnt, Cnt;
    int MaxCC;
    int MinCC = (FreeSize *2)/EACH_CC_TAKEUP_SIZE;
    unsigned int    ImageSize;

    char    Path[WINKS_PATH_MAX];
    WinksStrgFile_CCInfo_s  CCInfo;
    int     FsErr;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(MinCC<=0)
        MinCC = 2;
    
    MaxCC = WINKS_MAX_USER;
    while(pCSList == NULL)
    {
        pCSList = (WinksStrg_CCStat_s *)Winks_salAlloc(MaxCC*sizeof(WinksStrg_CCStat_s));
        if(pCSList == NULL)
        {
            MaxCC >>= 1;
            if(MaxCC<MinCC)
                return WINKS_RETURN_FAILURE;
        }
    }

    Winks_mem_set(pCSList, 0, MaxCC*sizeof(WinksStrg_CCStat_s));

    ImageSize = 0;
    ValidCCCnt = winks_StrgCheckCCStat(pCSList, MaxCC, &ImageSize);

    /* 释放无用的彩像就足够了 */
    if(FreeSize < ImageSize)
        return WINKS_RETURN_SUCCESS;

    FreeSize -= ImageSize;
    for(Cnt=0; Cnt<ValidCCCnt; Cnt++)
    {
        if(_LockCCIdx && _LockCCIdx == pCSList[Cnt].CCIdx)
            continue;
        if( winks_GetCCInfo(pCSList[Cnt].CCIdx, &CCInfo, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS ||
            Winks_fexist(Path) < 0 )
        {
            if( winks_StrgDeleteAssociate(pCSList[Cnt].CCIdx) != WINKS_RETURN_SUCCESS )
                break;
            
            continue;
        }
        if( (FsErr = Winks_fgetsize(Path, 1, &ImageSize)) < 0 ||
            (FsErr = Winks_fdelete(Path)) < 0 ||
            winks_StrgDeleteAssociate(pCSList[Cnt].CCIdx) != WINKS_RETURN_SUCCESS )
        {
            if(FsErr < 0)
                winks_StrgPrintFsErr(Path);
            break;
        }

        Winks_printf("Delete CC[0x%04x] size=%d %s\r\n", pCSList[Cnt].CCIdx, ImageSize, Path);
        if(FreeSize < ImageSize)
        {
            FreeSize = 0;
            break;
        }
        FreeSize -= ImageSize;
    }
    
    Winks_salFree(pCSList);

    if(FreeSize)
        return WINKS_RETURN_FAILURE;
    
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    judge disk space is full
    user count over WINKS_MAX_USER of disk not enough
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    1 full, 0 not full, -1 don't known
******************************************************************************/
int Winks_StrgDiskIsFull(void)
{
    Winks_FlashSpace_s PhoneSpace, CardSpace;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_UserNum >= WINKS_MAX_USER)return 1;

    if(Winks_StrgGetSpace(&PhoneSpace, &CardSpace) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    if( PhoneSpace.free < (WINKS_FLASHSPACE_RESERVED + EACH_CC_TAKEUP_SIZE) &&
        CardSpace.free < (WINKS_FLASHSPACE_RESERVED + EACH_CC_TAKEUP_SIZE) )
        return 1;
    
    return 0;
}

/*****************************************************************************
DESCRIPTION:
    Lock cc for call use
INPUT:
    CC_Idx  : cc index
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgLockCC(unsigned short CC_Idx)
{    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_LockCCIdx)
        return WINKS_RETURN_FAILURE;
    
    _LockCCIdx = CC_Idx;
    return WINKS_RETURN_SUCCESS;
}

/*****************************************************************************
DESCRIPTION:
    unlock the cc
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgUnlockCC(void)
{    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(_LockCCIdx)
        _LockCCIdx = 0;
    return WINKS_RETURN_SUCCESS;
}
#if 0
/*****************************************************************************
DESCRIPTION:
    save friend list: Trim a download friend list in order initiative\passive ...
    append to file <fl.txt>
INPUT:
    pNo     : phone number string in friend list
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgFLAdd(char *pNo)
{
    int     StrgLoc;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        char    Path[WINKS_PATH_MAX];

        int     iRet;
        int     retIdx;
        char    Buf[WINKS_STRG_PHONE_LEN];
        
        winks_GetFilePath_FL(StrgLoc, Path, sizeof(Path));
        if(Winks_fexist(Path) < 0)
        {
            /* 创建文件<fl.dat> */
            if(Winks_WriteBuf2File(Path, 0, NULL, 0, 1) != WINKS_RETURN_SUCCESS)
                continue;
        }

        Winks_mem_set(Buf, 0, WINKS_STRG_PHONE_LEN);
        strncpy(Buf, pNo, WINKS_STRG_PHONE_LEN);
        iRet = winks_StrgRecordSearch( Path, 0,
                                       (void *)pNo,
                                       WINKS_STRG_PHONE_LEN,
                                       strcmp,
                                       &retIdx );
        if(iRet == WINKS_STRG_SEARCH_RESULT_FOUND)
            return WINKS_RETURN_SUCCESS;
        else if(iRet ==  WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
            if(winks_StrgRecordAdd(Path, 0, -1, Buf, WINKS_STRG_PHONE_LEN) == WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_SUCCESS;
    }
    
    return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    get a phone number count in file <fl.txt>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    phone number count or error code
******************************************************************************/
int Winks_StrgFLCnt()
{
    int     StrgLoc;
    int     ValidCnt = 0;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        char    Path[WINKS_PATH_MAX];
        
        int     iRet;
        
        winks_GetFilePath_FL(StrgLoc, Path, sizeof(Path));
        if((iRet = winks_StrgRecordValidCnt(Path, 0,
                                            WINKS_STRG_PHONE_LEN,
                                            winks_IsValidString)) > 0)
            ValidCnt += iRet;
    }

    return ValidCnt;
}

/*****************************************************************************
DESCRIPTION:
    get a phone number from file <fl.txt>
INPUT:
    BeginIdx: index begin , >=0 
OUTPUT:
    pNo     : phone number string in friend list
RETURN:
    next fl index or error code
******************************************************************************/
int Winks_StrgFLGet(int BeginIdx, char *pNo)
{
    int     StrgLoc;
    int     iRet = WINKS_STRG_SEARCH_RESULT_FAIL;

    if(BeginIdx<0)
        return iRet;

    StrgLoc = (BeginIdx & 0x8000) ? 1: 0;
    BeginIdx &= 0x7fff; 

    while(StrgLoc < DRIVE_NUM)
    {
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_FL(StrgLoc, Path, sizeof(Path));
        iRet = winks_StrgRecordGet( BeginIdx, 
                                    Path, 0,
                                    pNo, 
                                    WINKS_STRG_PHONE_LEN,
                                    winks_IsValidString);
        if(iRet > 0)
            return StrgLoc ? (iRet|0x8000) : iRet;
        else
        {
            BeginIdx = 0;
            StrgLoc++;
            continue;
        }

        StrgLoc++;
    }

    return iRet;
}

/*****************************************************************************
DESCRIPTION:
    delete phone number from file <fl.txt>
INPUT:
    pNo     : phone number string in friend list
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgFLDelete(char *pNo)
{
    int     StrgLoc;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        int     retIdx;
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_FL(StrgLoc, Path, sizeof(Path));

        if(winks_StrgRecordSearch( Path, 0,
                                   (void *)pNo,
                                   WINKS_STRG_PHONE_LEN,
                                   strcmp,
                                   &retIdx ) ==  WINKS_STRG_SEARCH_RESULT_FOUND)
            return winks_StrgRecordDelete(Path, 0, retIdx, WINKS_STRG_PHONE_LEN);
    }
    return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    delete file <fl.txt>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgFLDeleteAll()
{
    int     StrgLoc;
    int     iRet = WINKS_RETURN_SUCCESS;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        int     ret;
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_FL(StrgLoc, Path, sizeof(Path));
        ret = winks_StrgRecordDeleteAll(Path);
        if(iRet != WINKS_RETURN_SUCCESS)
            iRet = ret;
    }

    return iRet;
}
#endif
/*****************************************************************************
DESCRIPTION:
    add phone number to  file <ul.txt>
INPUT:
    pNo     : phone number string
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgULAdd(char *pNo)
{
    char    Path[WINKS_PATH_MAX];

    int     cnt, r, w;
    char    Buf[WINKS_MAX_UL][WINKS_STRG_PHONE_LEN];
    
    winks_GetFilePath_UL(0, Path, sizeof(Path));

    Winks_mem_set(Buf, 0, WINKS_MAX_UL*WINKS_STRG_PHONE_LEN);
    if(Winks_fexist(Path) < 0)
    {   /* 创建文件<ul.dat> */
        if(Winks_WriteBuf2File(Path, 0, (unsigned char *)Buf, WINKS_MAX_UL*WINKS_STRG_PHONE_LEN, 1) != WINKS_RETURN_SUCCESS)
            return WINKS_RETURN_FAILURE;
    }

    if(Winks_ReadFile2Buf(Path, 0,
                    (unsigned char *)Buf, WINKS_STRG_PHONE_LEN*WINKS_MAX_UL)!= WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    /* 查找第一个空位 */
    for(cnt=0; cnt<WINKS_MAX_UL; cnt++)
    {
        if(Buf[cnt][0] == 0)
            break;
        if(winks_NoStrCompare(pNo, Buf[cnt]) == 0)
            break;
    }
    if(cnt == WINKS_MAX_UL)
    {
        /* 没有空位，所有记录前移1个(挤掉第1个)，留出最后1个存储当前号码 */
        for(cnt=0; cnt<WINKS_MAX_UL-1; cnt++)
            Winks_mem_cpy(Buf[cnt], Buf[cnt+1], WINKS_STRG_PHONE_LEN);
        Winks_mem_set(Buf[cnt], 0, WINKS_STRG_PHONE_LEN);
        strncpy(Buf[cnt], pNo, WINKS_STRG_PHONE_LEN);
    }
    else
    {
        /* 有空位或记录中找到这个号码，将当前号码移到最后，其他号码前移，跳过空号码 */
        for(r=0,w=0; r<WINKS_MAX_UL; r++)
        {
            if(r == cnt)
                continue;
            if(Buf[r][0])
            {
                if(r != w)
                    Winks_mem_cpy(Buf[w], Buf[r], WINKS_STRG_PHONE_LEN);
                w++;
            }
        }
        strncpy(Buf[w++], pNo, WINKS_STRG_PHONE_LEN);
        for(; w<WINKS_MAX_UL; w++)
            Winks_mem_set(Buf[w], 0, WINKS_STRG_PHONE_LEN);
    }

    /* 存储当前号码 */
    return Winks_WriteBuf2File(Path, 0,
                    (unsigned char *)Buf, WINKS_STRG_PHONE_LEN*WINKS_MAX_UL, 0);
}    

/*****************************************************************************
DESCRIPTION:
    get a phone number count in file <ul.txt>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    phone number count or error code
******************************************************************************/
int Winks_StrgULCnt(void)
{
    char    Path[WINKS_PATH_MAX];
    
    winks_GetFilePath_UL(0, Path, sizeof(Path));

    return winks_StrgRecordValidCnt(Path, 0, 
                                    WINKS_STRG_PHONE_LEN,
                                    winks_IsValidString);
}

/*****************************************************************************
DESCRIPTION:
    get a phone number from  file <ul.txt>
INPUT:
    BeginIdx: index begin , >=0 
OUTPUT:
    pNo     : phone number string
RETURN:
    next ul index or error code
******************************************************************************/
int Winks_StrgULGet(int BeginIdx, char *pNo)
{
    char    Path[WINKS_PATH_MAX];
    
    winks_GetFilePath_UL(0, Path, sizeof(Path));
    return winks_StrgRecordGet( BeginIdx, 
                                Path, 0,
                                pNo, 
                                WINKS_STRG_PHONE_LEN,
                                winks_IsValidString);
}

/*****************************************************************************
DESCRIPTION:
    delete phone number from file <ul.txt>
INPUT:
    pNo     : phone number string
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgULDelete(char *pNo)
{
    char    Path[WINKS_PATH_MAX];
    
    int     cnt;
    char    Buf[WINKS_MAX_UL][WINKS_STRG_PHONE_LEN];
    
    winks_GetFilePath_UL(0, Path, sizeof(Path));

    if(Winks_ReadFile2Buf(Path, 0,
                    (unsigned char *)Buf, WINKS_STRG_PHONE_LEN*WINKS_MAX_UL) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;

    /* 查找要删除的电话号码 */
    for(cnt=0; cnt<WINKS_MAX_UL; cnt++)
    {
        if(Buf[cnt][0] && (winks_NoStrCompare(Buf[cnt], pNo)==0))
            break;
    }
    /* 没有找到 */
    if(cnt == WINKS_MAX_UL)
        return WINKS_RETURN_FAILURE;

    /* 移动后面记录 */
    for(;cnt<WINKS_MAX_UL-1; cnt++)
    {
        Winks_mem_cpy(Buf[cnt], Buf[cnt+1], WINKS_STRG_PHONE_LEN);
    }
    Winks_mem_set(Buf[cnt], 0, WINKS_STRG_PHONE_LEN);

    /* 保存删除后结果 */
    return Winks_WriteBuf2File(Path, 0,
                    (unsigned char *)Buf, WINKS_STRG_PHONE_LEN*WINKS_MAX_UL, 0);
}

/*****************************************************************************
DESCRIPTION:
    delete file <ul.txt>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgULDeleteAll(void)
{
    char    Path[WINKS_PATH_MAX];
    
    winks_GetFilePath_UL(0, Path, sizeof(Path));

    return winks_StrgRecordDeleteAll(Path);
}
#if 0
/*****************************************************************************
DESCRIPTION:
    add to file <nd.dat> 
INPUT:
    pNeedDown   : phone number string and/or ID info
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgNDAdd(WinksStrgFile_NeedDown_s *pNeedDown)
{
    int     StrgLoc;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        int     iRet;
        int     retIdx;
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_ND(StrgLoc, Path, sizeof(Path));

        if(Winks_fexist(Path) < 0)
        {
            /* 创建文件<nd.dat> */
            if(Winks_WriteBuf2File(Path, 0, NULL, 0, 1) != WINKS_RETURN_SUCCESS)
                continue;
        }
        
        iRet = winks_StrgRecordSearch( Path, 0,
                                       (void *)pNeedDown->NO,
                                       sizeof(WinksStrgFile_NeedDown_s),
                                       winks_NDCompare,
                                       &retIdx);
        if(iRet ==  WINKS_STRG_SEARCH_RESULT_FOUND)
            return winks_StrgRecordAdd(Path, 0, retIdx, pNeedDown, sizeof(WinksStrgFile_NeedDown_s));
        else if(iRet ==  WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
        {
            if(winks_StrgRecordAdd(Path, 0, -1, pNeedDown, sizeof(WinksStrgFile_NeedDown_s)) == WINKS_RETURN_SUCCESS)
                return WINKS_RETURN_SUCCESS;
        }
    }
    
    return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    get a record count in file <nd.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    need download count or error code
******************************************************************************/
int Winks_StrgNDCnt()
{
    int     StrgLoc;
    int     ValidCnt = 0;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        char    Path[WINKS_PATH_MAX];

        int     iRet;
        
        winks_GetFilePath_ND(StrgLoc, Path, sizeof(Path));

        if((iRet = winks_StrgRecordValidCnt(Path, 0,
                                        sizeof(WinksStrgFile_NeedDown_s),
                                        winks_IsValid_ND)) > 0)
            ValidCnt += iRet;
    }

    return ValidCnt;
}

/*****************************************************************************
DESCRIPTION:
    get a record from file <nd.dat>
INPUT:
    BeginIdx: index begin , >=0 
OUTPUT:
    pNeedDown   : phone number string and/or ID info
RETURN:
    next nd index or error code
******************************************************************************/
int Winks_StrgNDGet(int BeginIdx, WinksStrgFile_NeedDown_s *pNeedDown)
{
    int     StrgLoc;
    int     iRet = WINKS_STRG_SEARCH_RESULT_FAIL;

    if(BeginIdx<0)
        return iRet;

    StrgLoc = (BeginIdx & 0x8000) ? 1: 0;
    BeginIdx &= 0x7fff;

    while(StrgLoc < DRIVE_NUM)
    {
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_ND(StrgLoc, Path, sizeof(Path));
        iRet = winks_StrgRecordGet( BeginIdx, 
                                    Path, 0,
                                    pNeedDown, 
                                    sizeof(WinksStrgFile_NeedDown_s),
                                    winks_IsValid_ND);
        if(iRet > 0)
            return StrgLoc ? (iRet|0x8000) : iRet;
        else
        {
            BeginIdx = 0;
            StrgLoc++;
            continue;
        }

        StrgLoc++;
    }

    return iRet;
}

/*****************************************************************************
DESCRIPTION:
    delete record from file <nd.dat>
    just need set deleted flag
INPUT:
    pNo : phone number string
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgNDDelete(char *pNo)
{
    int     StrgLoc;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        //int   iRet;
        int     retIdx;
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_ND(StrgLoc, Path, sizeof(Path));

        if(winks_StrgRecordSearch( Path, 0,
                                       (void *)pNo,
                                       sizeof(WinksStrgFile_NeedDown_s),
                                       winks_NDCompare,
                                       &retIdx ) ==  WINKS_STRG_SEARCH_RESULT_FOUND)
            return winks_StrgRecordDelete(Path, 0, retIdx, sizeof(WinksStrgFile_NeedDown_s));
    }
    
    return WINKS_RETURN_FAILURE;
}

/*****************************************************************************
DESCRIPTION:
    delete file <nd.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgNDDeleteAll()
{
    int     StrgLoc;
    int     iRet = WINKS_RETURN_SUCCESS;
    
    for(StrgLoc=0; StrgLoc<DRIVE_NUM; StrgLoc++)/* 依次搜索手机和存储卡 */
    {
        int     ret;
        char    Path[WINKS_PATH_MAX];
        
        winks_GetFilePath_ND(StrgLoc, Path, sizeof(Path));
        ret = winks_StrgRecordDeleteAll(Path);
        if(iRet != WINKS_RETURN_SUCCESS)
            iRet = ret;
    }

    return iRet;
}
#endif
/*****************************************************************************
DESCRIPTION:
    judge card data is correct or not
    use SelfNoBcd and CacheTime in <call.dat> and <ccinfo.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    memory card marched or not
******************************************************************************/
int Winks_StrgGetStatus(void)
{
    int     Cnt = 0, Max;
    unsigned long   CacheTimeArr[3];
    char            SelfNoBcdArr[3][WINKS_STRG_NOBCD_LEN];

    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

/* 彩像Cache建立前无法判断是否存储卡是否对应 */
    if(_CacheTime == 0)
        return WINKS_STRG_CARD_STATUS_NO_CCCACHE;
    
#ifdef WINKS_SUPPORT_MEMORY_CARD
    if(Winks_IsMemoryCardExist() != 1)
        return WINKS_STRG_CARD_STATUS_NOT_EXIST;
#endif /* WINKS_SUPPORT_MEMORY_CARD */

/* 插上USB作为外设,不能读取,返回WINKS_RETURN_FAILURE */
#if 1   /* 不考虑手机上存储文件是否正确 */
{
    unsigned long UsbState;
    if(Winks_GetUsbState(&UsbState))
        return WINKS_RETURN_FAILURE;
}
    Cnt = 2;    
#endif
#ifdef WINKS_SUPPORT_MEMORY_CARD
    Max = 3;
#else
    Max = 2;
#endif
    while(Cnt < Max)
    {
        int iRet = WINKS_RETURN_FAILURE;
        FILE  * fp;
        char    Path[WINKS_PATH_MAX];
        WinksStrgFile_UserHead_s Head;

        switch(Cnt)
        {
        case 0:
            iRet = winks_GetFilePath_User(0, Path, sizeof(Path));
            break;
        case 1:
            iRet = winks_GetFilePath_CCInfo(0, Path, sizeof(Path));
            break;
    #ifdef WINKS_SUPPORT_MEMORY_CARD
        case 2:
            iRet = winks_GetFilePath_CCInfo(1, Path, sizeof(Path));
            break;
    #endif
        default:
            break;
        }
        if(iRet != WINKS_RETURN_SUCCESS)
            return WINKS_STRG_CARD_STATUS_NOT_EXIST;/* 认为没有存储卡 */
        
        fp = Winks_fopen(Path, "rb");
        if( (fp != NULL) &&
            (Winks_fread(&Head, sizeof(Head), 1, fp) == 1))
        {
            CacheTimeArr[Cnt] = Head.CacheTime;
            Winks_mem_cpy(SelfNoBcdArr[Cnt], Head.SelfNoBcd, WINKS_STRG_NOBCD_LEN);
        }
        else
        {
            if(Cnt == 2)
                return WINKS_STRG_CARD_STATUS_NOT_EXIST;
            else
                return WINKS_RETURN_FAILURE;
        }
        if(fp != NULL)
             Winks_fclose(fp);

        Cnt++;
    }   

#if 0   /* 不考虑手机上存储文件是否正确 */
    if(strcmp(SelfNoBcdArr[0], SelfNoBcdArr[1]))
        return WINKS_RETURN_FAILURE;
    if(strcmp(SelfNoBcdArr[0], _SelfNoBcd))
        return WINKS_RETURN_FAILURE;
    if(CacheTimeArr[0] != CacheTimeArr[1])
        return WINKS_RETURN_FAILURE;
    if(CacheTimeArr[0] != _CacheTime)
        return WINKS_RETURN_FAILURE;
#endif
#ifdef WINKS_SUPPORT_MEMORY_CARD
    if(memcmp(SelfNoBcdArr[2], _SelfNoBcd, WINKS_STRG_NOBCD_LEN))
        return WINKS_STRG_CARD_STATUS_NEW_CARD;
    
    if(CacheTimeArr[2] != _CacheTime)
        return WINKS_STRG_CARD_STATUS_OLD_CARD;
#endif
    return WINKS_STRG_CARD_STATUS_OK_CARD;
}

/*****************************************************************************
DESCRIPTION:
    record call event to <call.dat> and/or <ccinfo.dat>
INPUT:
    pCallInfo   : call info
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgCallEvtAdd(WinksStrgFile_CallInfo_s *pCallInfo)
{
    int     iRet;
    int     retIdx;
    char    Path[WINKS_PATH_MAX];
    WinksStrgFile_CallStat_s CallStat;

    Winks_assert(pCallInfo != NULL);
    Winks_assert(pCallInfo->NO[0]);
    
    winks_GetFilePath_Call(0, Path, sizeof(Path));

    Winks_mem_set(&CallStat, 0, sizeof(WinksStrgFile_CallStat_s));
    Winks_PhoneNoStr2Bcd(pCallInfo->NO, strlen(pCallInfo->NO), WINKS_STRG_NOBCD_LEN, (unsigned char *)CallStat.NoBcd);
    
    iRet = winks_StrgRecordSearch( Path, sizeof(WinksStrgFile_CallHead_s),
                                   (void *)CallStat.NoBcd,
                                   sizeof(WinksStrgFile_CallStat_s),
                                   winks_CSCompare,
                                   &retIdx,
                                   (void *)&CallStat );
    if(iRet ==  WINKS_STRG_SEARCH_RESULT_FOUND)
    {
        //if(Winks_StrgCallEvtGet((unsigned short)retIdx, &CallStat) != WINKS_RETURN_SUCCESS)
        //    return WINKS_RETURN_FAILURE;
    }
    else if(iRet ==  WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
    {
        /* 查找第1个空位 */
        char    EmptyNoBcd[WINKS_STRG_NOBCD_LEN];

        Winks_mem_set(EmptyNoBcd, 0, WINKS_STRG_NOBCD_LEN);
        iRet = winks_StrgRecordSearch( Path, sizeof(WinksStrgFile_CallHead_s),
                                       (void *)EmptyNoBcd,
                                       sizeof(WinksStrgFile_CallStat_s),
                                       winks_CSCompare,
                                       &retIdx,
                                       NULL );
    }
    
    CallStat.CallTime = pCallInfo->CallTime;

    if(iRet ==  WINKS_STRG_SEARCH_RESULT_FOUND)
    {
        CallStat.CallCnt++;
        if(pCallInfo->CallClock > CallStat.LatestCallClock)
            CallStat.LatestCallClock = pCallInfo->CallClock;
        
        iRet = winks_StrgRecordAdd(Path, sizeof(WinksStrgFile_CallHead_s), retIdx, &CallStat, sizeof(WinksStrgFile_CallStat_s));
    }
    else if(iRet ==  WINKS_STRG_SEARCH_RESULT_NOT_FOUND)
    {
        WinksStrgFile_CallStat_s CallStatArr[WINKS_CALL_STAT_PRETAKE_SIZE];

        Winks_mem_set(CallStatArr, 0, sizeof(WinksStrgFile_CallStat_s)*WINKS_CALL_STAT_PRETAKE_SIZE);
        Winks_mem_cpy(&CallStatArr[0], &CallStat, sizeof(WinksStrgFile_CallStat_s));
        if(winks_StrgRecordAdd(Path, sizeof(WinksStrgFile_CallHead_s), -1, CallStatArr, sizeof(WinksStrgFile_CallStat_s)*WINKS_CALL_STAT_PRETAKE_SIZE)
                                != WINKS_RETURN_SUCCESS)
        {
            Winks_printf("Warning: Winks_StrgCallEvtAdd found not enough space!!!\r\n");
            //TODO: 
            return WINKS_RETURN_FAILURE;
        }
    }
    else
        return WINKS_RETURN_FAILURE;

    winks_StrgCallEvtAddCCStatus(pCallInfo);
    
    return iRet;
}

/*****************************************************************************
DESCRIPTION:
    get call event log count in file <call.dat> 
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    call event log count or error code
******************************************************************************/
int Winks_StrgCallEvtCnt(void)
{
    char    Path[WINKS_PATH_MAX];
    
    winks_GetFilePath_Call(0, Path, sizeof(Path));

    return winks_StrgRecordValidCnt( Path, sizeof(WinksStrgFile_CallHead_s),
                                sizeof(WinksStrgFile_CallStat_s),
                                winks_IsValid_CS);
}

/*****************************************************************************
DESCRIPTION:
    get call event from <call.dat> with Idx
INPUT:
    Idx     : call event index
OUTPUT:
    pCallInfo   : call info
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgCallEvtGet(unsigned short Idx, WinksStrgFile_CallStat_s *pCallInfo)
{
    char    Path[WINKS_PATH_MAX];

    unsigned long Off;
    
    winks_GetFilePath_Call(0, Path, sizeof(Path));

    Off = sizeof(WinksStrgFile_CCHead_s)+ Idx * sizeof(WinksStrgFile_CallStat_s);
    return Winks_ReadFile2Buf(Path, Off, (unsigned char *)pCallInfo, sizeof(WinksStrgFile_CallStat_s));
}

/*****************************************************************************
DESCRIPTION:
    get call event from <call.dat> with phone number
INPUT:
    pNoBcd  : phone number string (bcd encoded)
OUTPUT:
    pCallInfo   : call info
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgCallEvtGetWithNo(char *pNoBcd, WinksStrgFile_CallStat_s *pCallInfo)
{
    int     iRet;
    int     retIdx;
    char    Path[WINKS_PATH_MAX];
    //WinksStrgFile_CallStat_s CallStat;

    Winks_assert(pNoBcd != NULL);
    Winks_assert(pCallInfo != NULL);
    
    winks_GetFilePath_Call(0, Path, sizeof(Path));

    iRet = winks_StrgRecordSearch( Path, sizeof(WinksStrgFile_CallHead_s),
                                   (void *)pNoBcd,
                                   sizeof(WinksStrgFile_CallStat_s),
                                   winks_CSCompare,
                                   &retIdx,
                                   NULL );
    if(iRet ==  WINKS_STRG_SEARCH_RESULT_FOUND)
        return retIdx;

    return WINKS_RETURN_FAILURE;
}

#ifdef WINKS_DEBUG
/*
  0 -- ok card
  1 -- new card
  2 -- old card
*/
int Winks_StrgSetCardStatus(int Flag)
{
    //int     iRet;
    char    Path[WINKS_PATH_MAX];
    WinksStrgFile_CCHead_s  CCHead;
    
    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

    if(winks_GetFilePath_CCInfo(1, Path, sizeof(Path)) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;/* 认为没有存储卡 */
    
    Winks_mem_set(&CCHead, 0, sizeof(WinksStrgFile_CCHead_s));

    switch(Flag)
    {
        case 1:
            Winks_mem_set(CCHead.SelfNoBcd, 0, sizeof(WINKS_STRG_NOBCD_LEN));
            CCHead.CacheTime = 0;
            break;
        case 2:
            Winks_mem_cpy(CCHead.SelfNoBcd, _SelfNoBcd, sizeof(WINKS_STRG_NOBCD_LEN));
            if(_CacheTime)
                CCHead.CacheTime = _CacheTime-1;
            else
                CCHead.CacheTime = 0;
            break;
        case 0:
        default:
            Winks_mem_cpy(CCHead.SelfNoBcd, _SelfNoBcd, sizeof(WINKS_STRG_NOBCD_LEN));
            CCHead.CacheTime = _CacheTime;
            break;
    }
        
    return Winks_WriteBuf2File(Path, 0, (unsigned char *)&CCHead, sizeof(WinksStrgFile_CCHead_s), 0);
}
#endif

/* 记录此次彩像展示情况。返回负值表示失败 */
//int Winks_log_cc_show(unsigned short CC_Idx, unsigned long time, unsigned char interrupted)
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted)
{
    int     iRet;
    int     retIdx;
    WinksStrgFile_StatHead_s header;
    //WinksStrgFile_CCInfo_s CCInfo;
    WinksStrgFile_Stat_s   Stat;
    char    Path[WINKS_PATH_MAX];

    WINKS_STRG_FILEMAP_DEF
    WINKS_STRG_FILEMAP_INIT

#if 1
    Winks_assert(CC_ID);
#else
    Winks_mem_set(&CCInfo, 0, sizeof(CCInfo));
    if( Winks_StrgGetCCInfo(CC_Idx, &CCInfo) != WINKS_RETURN_SUCCESS ||
        CCInfo.ID[0] == 0 )
        return WINKS_RETURN_FAILURE;
#endif
    winks_GetFilePath_Stat(0, Path, sizeof(Path));
    if(Winks_ReadFile2Buf(Path, 0, (unsigned char *)&header, sizeof(header)) != WINKS_RETURN_SUCCESS)
        return WINKS_RETURN_FAILURE;
    if(memcmp(header.SelfNoBcd, _SelfNoBcd, WINKS_STRG_NOBCD_LEN))
    {
        
    }
    
    Winks_mem_set(&Stat, 0, sizeof(Stat));
    iRet = winks_StrgRecordSearch( Path, sizeof(WinksStrgFile_StatHead_s),
                                   (void *)CC_ID/*CCInfo.ID*/,
                                   sizeof(WinksStrgFile_Stat_s),
                                   winks_StatCCIDCompare,
                                   &retIdx,
                                   (void *)&Stat);
    if(iRet ==  WINKS_STRG_SEARCH_RESULT_FOUND)
    {
        Stat.int_times += interrupted;
        Stat.show_times ++;
        Stat.time += time;
        iRet = winks_StrgRecordAdd( Path, sizeof(WinksStrgFile_StatHead_s), 
                                    retIdx, 
                                    (void *)&Stat,
                                    sizeof(WinksStrgFile_Stat_s) );
    }
    else
    {
        strncpy(Stat.ID, CC_ID/*CCInfo.ID*/, sizeof(Stat.ID));
        Stat.int_times = interrupted;
        Stat.show_times = 1;
        Stat.time = time;
        iRet = winks_StrgRecordAdd( Path, sizeof(WinksStrgFile_StatHead_s), 
                                    -1, 
                                    (void *)&Stat,
                                    sizeof(WinksStrgFile_Stat_s) );
    }

    return iRet;
}

