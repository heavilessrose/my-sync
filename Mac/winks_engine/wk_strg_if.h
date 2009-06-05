#ifndef _WK_STRG_IF_H_
#define _WK_STRG_IF_H_

#ifdef __cplusplus
extern "C" {
#endif


#define WINKS_STRG_NOBCD_LEN 6      /* �洢�ĵ绰���볤�� BCD-8421�� */
#define WINKS_STRG_CC_ID_LEN 40     /* �洢�Ĳ���ID���� */
#define WINKS_STRG_GREET_LEN 202    /* �洢���ʺ��ﳤ�� */

#define WINKS_STRG_PHONE_LEN 16  /* �洢�ĺ��ѵ绰�ַ��� */
#define WINKS_STRG_NICK_LEN  32  /* �洢�ĺ����ǳ��ַ��� */

#define WINKS_MIN_USER      64      /* ����cache���û��� */
#define WINKS_MAX_USER      1024    /* ���cache���û��� ����2048(�ʺ�������) */

#define WINKS_INVALID_CALL_TIME     10  /* С�ڸ�ʱ�䳤�ȵĵ绰�¼�Ϊ��Ч,�ɲ����Ĳ��� */
#define WINKS_IGNORE_CALL_TIME      30  /* С�ڸ�ʱ�䳤�ȵĵ绰�¼��ڿռ䲻��ʱ�ɲ����Ĳ��� */

/* Ƭͷ��Ӫ�̰�������Ѵ��� */
#define WINKS_DFT_CC_NO     "1"     /* Ĭ�ϲ���ID */

#define WINKS_PRI_CC_NO     "11"    /* Ƭͷ������ʼID */
#define WINKS_PRI_CC_MAX    1       /* Ƭͷ���������� <10 */
#define WINKS_PRI_BCD_MIN   0x11
#define WINKS_PRI_BCD_MAX   (WINKS_PRI_BCD_MIN+WINKS_PRI_CC_MAX)

#define WINKS_BUS_CC_NO     "21"    /* ��Ӫ�̲�����ʼID */
#define WINKS_BUS_CC_MAX    1       /* ��Ӫ�̲�������� <10 */
#define WINKS_BUS_BCD_MIN   0x21
#define WINKS_BUS_BCD_MAX   (WINKS_BUS_BCD_MIN+WINKS_PRI_CC_MAX)

#define WINKS_PRIBUS_NUM    (WINKS_PRI_CC_MAX+WINKS_BUS_CC_MAX) /* Ƭͷ��Ӫ��Ԥ���� */

#define WINKS_MAX_FIXED     (WINKS_FRIENDS_MAX_NUMBER + WINKS_PRIBUS_NUM)

//#define WINKS_CCSHOW_USE_CCCACHE

typedef enum tag_WinksCCType_e
{
    CC_TYPE_NONE = 0,
    CC_TYPE_JPG,
    CC_TYPE_PNG,
    CC_TYPE_TIFF,
    CC_TYPE_SPL,
    CC_TYPE_SWF,
    CC_TYPE_GIF,
    CC_TYPE_MAX
}WinksCCType_e;

///////////////////////////////////////////////////////////////////////////////
//           S T O R A G E    F I L E
///////////////////////////////////////////////////////////////////////////////
/* <user.dat> �û��绰���������ID���ʺ����ӳ���ļ� */
/* <user.dat> �ļ���Ϣ��ֻ��¼�����û�*/
typedef struct tag_WinksStrgFile_UserInfo_s
{
    char            NoBcd[WINKS_STRG_NOBCD_LEN];      /* �绰���� BCD-8421�� */
    unsigned short  CCIdx;      /* �洢��<ccinfo.dat>�еĲ���ID��� */
    unsigned short  GrtOff;     /* �洢��<greet.dat>�е��ʺ���ƫ��
                                   ��16�ֽڶ��룬��16Ϊʵ��ƫ�� */
}WinksStrgFile_UserInfo_s;

/* <call.dat> �û��绰�¼�ͳ�Ƶ�ӳ���ļ� */
/* <call.dat> �ļ���Ϣ*/
typedef struct tag_WinksStrgFile_CallStat_s
{
    char            NoBcd[WINKS_STRG_NOBCD_LEN];      /* �绰���� BCD-8421�� */
    unsigned short  CallCnt;        /* �绰�¼����� */
    unsigned short  CallTime;       /* ���һ�ε绰ͨ��ʱ��(�������) */
    unsigned short  Flag;           /* �Ƿ�����û�(����չ��ı�־) */
    unsigned long   LatestCallClock;  /* ���һ�ε绰�¼���ʱ�� 
                                         ��2000-1-1 0:0:0������� */
}WinksStrgFile_CallStat_s;

/* <ccinfo.dat> ������Ϣ�ļ� */
/* <ccinfo.dat> �ļ���Ϣ*/
typedef struct tag_WinksStrgFile_CCInfo_s
{
    char            ID[WINKS_STRG_CC_ID_LEN];      /* ����ID */
    unsigned char   img_type;   /* �������� WinksFileExt_e */
    unsigned char   img_cnt;    /* ͼƬ�� */
    unsigned short  interval;   /* ͼƬ���ż�� */
    unsigned short  UseCnt;     /* �绰�¼��� */
    unsigned long   LatestUse;  /* ���һ�ε绰�¼���ʱ�� 
                                   ��2000-1-1 0:0:0������� */
}WinksStrgFile_CCInfo_s;

/* <greet.dat> �ʺ�����Ϣ�ļ� */
/* <greet.dat> �ļ���Ϣ*/
/* �ʺ����¼Ϊ�䳤��ʽ��ÿ����¼��16�ֽڶ��룬��WINKS_ALIGN(len,16)���� 
   ͬһ�����ʺ������ʱ׷�ӵ����(��һ�����ļ�β)��
   ����Ч�ʺ��ﳬ��һ�������򿪻���ʼ��ʱ�����ļ� */
typedef struct tag_WinksStrgFile_GreetInfo_s
{
    unsigned long   Magic;          /* XX �ʺ����ʶ[�ɴ洢���] */
    char            NoBcd[WINKS_STRG_NOBCD_LEN];           /* �绰���� BCD-8421�� */
    unsigned char   GreetLen;       /* �ʺ��ﳤ�� */
    unsigned char   Occupy;         /* XX �����ʺ����¼ռ�ó���[�ɴ洢����] */
    short           GreetStr[WINKS_STRG_GREET_LEN/2];  /* �ʺ������ʵ�ʳ��ȱ��� */
}WinksStrgFile_GreetInfo_s;

/* <stat.dat> �ļ���Ϣ*/
typedef struct tag_WinksStrgFile_Stat_s
{
    char            ID[WINKS_STRG_CC_ID_LEN];      /* ����ID */
    unsigned short  show_times; /* ����չʾ���� */
    unsigned short  int_times;  /* ����չʾ�жϴ��� */
    unsigned long   time;       /* ���񲥷�ʱ�� */
}WinksStrgFile_Stat_s;

/* <friend.dat> �����ļ� */
typedef struct tag_WinksStrgFile_FriendInfo_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* �绰���� �ַ��� */
    unsigned char   Nick[WINKS_STRG_NICK_LEN];   /* �����ǳ� */
}WinksStrgFile_FriendInfo_s;

///////////////////////////////////////////////////////////////////////////////
//            F U N C T I O N     I N T E R F A C E
///////////////////////////////////////////////////////////////////////////////
typedef struct tag_WinksStrgFile_CCImage_s
{
    char          * NO;         /* �绰�����ַ��� */
    char          * ID;         /* ����ID */
    unsigned char   img_type;   /* �������� WinksFileExt_e */
    unsigned char   img_idx;    /* �ڼ���ͼƬ */
    unsigned char   img_cnt;    /* ͼƬ�� */
    unsigned char   _pad;
    unsigned long   FileSize;   /* �ļ���С */
    int             file_handle;/* �ļ����(FILE *),��Begin����,��Doʹ��,��End�ر� */
    Winks_MemPool_s* pMem;      /* ������ص��ļ����� */
    unsigned long   blocklen;   /* ��ǰ���ؿ��ܴ�С(Ӧ�����ڴ��б���pMem->offset�ۼ�ֵ���) */
}WinksStrgFile_CCImage_s;

typedef struct tag_WinksStrgFile_CCShow_s
{
    int             RetStatus;                   /* �����Ƿ���ú����Ӧ
                                                    0 -- ok
                                                    1 -- Ĭ�ϲ���
                                                    2 -- Ƭͷ 
                                                    3 -- ��Ӫ�� */
    char            NO[WINKS_STRG_PHONE_LEN];    /* �绰�����ַ��� */
    char            Name[WINKS_STRG_NICK_LEN];   /* �����ǳ� */
    char            ID[WINKS_STRG_CC_ID_LEN];    /* ����ID */
    unsigned short  CC_Idx;     /* ����Idx */
    unsigned short  interval;   /* ͼƬ���ż�� */
    unsigned char   img_type;   /* �������� WinksFileExt_e */
    unsigned char   img_cnt;    /* ͼƬ�� */
    unsigned char   GreetLen;   /* �ʺ��ﳤ�� */
    unsigned char   SaveType;   /* ����洢���� 0--�ļ� 1--�ڴ� */
    unsigned char * pCCImage;   /* �洢�Ĳ����ڴ����� */
    unsigned long   CCImageSize;/* �洢�Ĳ����ڴ����ݴ�С */
    char            CCImgPath[4][256];      /* �洢�Ĳ���·�� */
    short           GreetStr[WINKS_STRG_GREET_LEN/2];     /* �洢�Ĳ����ʺ��� */
}WinksStrgFile_CCShow_s;

typedef struct tag_WinksStrgFile_CallInfo_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* �绰�����ַ��� */
    char            ID[WINKS_STRG_CC_ID_LEN];    /* ����ID */
    unsigned long   CallClock;  /* ͨ��ʱ�� */
    unsigned short  CallTime;   /* ͨ��ʱ��(����Ϊ��λ������65535�밴65535��) */
    unsigned char   CallType;   /* ����0��ȥ��1 */
    unsigned char   _pad;
}WinksStrgFile_CallInfo_s;

/* ���ݵ绰�¼��ؽ�Cacheʱ����Ϊ�Ѵ��������񣬲���ȫɾ�������Ƚ������Ӧ��
   ������Ϣ���أ��ʺ���ֱ�ӱ��棬���ౣ�浽 */
typedef struct tag_WinksStrgFile_NeedDown_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* �绰�����ַ��� */
    char            ID[WINKS_STRG_CC_ID_LEN];    /* ����ID */
    unsigned char   cc_type;    /* ��������:1--Ƭͷ 2--��Ӫ�� 3--�û� */
    unsigned char   img_cnt;    /* ����ID��Ӧ��ͼƬ���� */
    unsigned short  interval;   /* ͼƬ���ż�� */
}WinksStrgFile_NeedDown_s;

typedef enum tag_WinksStrgSearchResult_e
{
    WINKS_STRG_SEARCH_RESULT_NOT_FOUND = -20,     /* û���ҵ� */
    WINKS_STRG_SEARCH_RESULT_FAIL ,    /* ����ʧ�� */
    WINKS_STRG_SEARCH_RESULT_FOUND = 0,
    WINKS_STRG_SEARCH_RESULT_FOUND_EMPTY
}WinksStrgSearchResult_e;

typedef enum tag_WinksStrgCardStatus_e
{
    WINKS_STRG_CARD_STATUS_NOT_EXIST = -20,
    WINKS_STRG_CARD_STATUS_NO_CCCACHE,
    WINKS_STRG_CARD_STATUS_NEW_CARD,
    WINKS_STRG_CARD_STATUS_OLD_CARD,
    WINKS_STRG_CARD_STATUS_OK_CARD = 0
}WinksStrgCardStatus_e;

int Winks_WriteBuf2File( char *pPath, unsigned long Off, 
                            unsigned char *pBuf, unsigned long BufSize,
                            int CreateFlag);
int Winks_ReadFile2Buf( char *pPath, unsigned long Off, 
                            unsigned char *pBuf, unsigned long BufSize);
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
char * Winks_SkipPlus(char *pNo);

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
unsigned long Winks_CalculatePower(WinksStrgFile_CallStat_s *pCallStat);

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
                                  unsigned char *retBcd);

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
                                  char *retPhoneStr);

/*****************************************************************************
DESCRIPTION:
	get the app path of handset or memory card
INPUT:
	StrgLoc : handset[0] or memory card[1]
	PathSize: path buffer size
OUTPUT:
	retAppPath	: the app path string
RETURN:
	Success or failure
******************************************************************************/
int Winks_GetAppPath(int StrgLoc, char *retAppPath, int PathSize);

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
int Winks_SysTime2CacheTime(Winks_DateTime_s *pDateTime, unsigned long *retCacheTime);

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
int Winks_StrgGetSpace(Winks_FlashSpace_s *PhoneSpace, Winks_FlashSpace_s *CardSpace);
/*****************************************************************************
DESCRIPTION:
    power on, if ENABLE status Map file UserMap[],CCMap[],FriendMap[]
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgInitMapFile(void);

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
int Winks_StrgDeinitMapFile(void);

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
int Winks_StrgSetSelfNo(char *pNo);

/*****************************************************************************
DESCRIPTION:
    set cache time to <call.dat> and <ccinfo.dat>
INPUT:
    NULL
OUTPUT:
    NULL
RETURN:
    True or false
******************************************************************************/
int Winks_StrgSetCacheTime(unsigned long NewCacheTime);

/*****************************************************************************
DESCRIPTION:
	delete all disk file
INPUT:
	Disk	: phone [1] �� memory card[2] ��phone and card[3]
OUTPUT:
	NULL
RETURN:
	True or false
******************************************************************************/
int Winks_StrgDeleteDiskFiles(int Disk);

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
int Winks_StrgUserIsFriend(char *pNo, unsigned short *retFriendIdx);

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
int Winks_StrgFriendAdd(char *pNo, unsigned char *pNick);

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
int Winks_StrgFriendCnt(void);

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
                               unsigned char *pNick, int NickLen);

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
int Winks_StrgFriendDel(char *pNo);

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
int Winks_StrgFriendDelAll(void);

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
int Winks_StrgSearchUser(char *pNoBcd, unsigned short *retUsrIdx, WinksStrgFile_UserInfo_s *pUsrInfo);

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
int Winks_StrgSearchCC(char *pCC_ID, unsigned short *retCCIdx);

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
int Winks_StrgSearchUserEpt(unsigned short * retUsrIdx);

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
int Winks_StrgSearchCCEpt(unsigned long FileSize, unsigned short *retCCIdx);

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
int Winks_StrgSearchGreetEpt(unsigned char GrtLen, unsigned short * retGrtOff);

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
int Winks_StrgGetUserCnt(void);

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
int Winks_StrgGetUserInfo(unsigned short Idx, WinksStrgFile_UserInfo_s *pUsrInfo);

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
int Winks_StrgSetUserInfo(unsigned short Idx, WinksStrgFile_UserInfo_s *pUsrInfo);

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
int Winks_StrgGetCCInfo(unsigned short Idx, WinksStrgFile_CCInfo_s *pCCInfo);

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
int Winks_StrgSetCCInfo(unsigned short Idx, WinksStrgFile_CCInfo_s *pCCInfo);

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
int Winks_StrgCCIsExist(unsigned short Idx, WinksStrgFile_CCInfo_s * retCCInfo);

/*****************************************************************************
DESCRIPTION:
    get greet string from <greet.dat> file at index pos, 
INPUT:
    Off         : greet info offset in file <greet.dat> (Off= fp_off/16)
    pGreetInfo  : greet info struct, 
                  if (pGreetInfo->NoBcd) is empty, delete record
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgGetGreet(unsigned short Off, WinksStrgFile_GreetInfo_s *pGreetInfo);

/*****************************************************************************
DESCRIPTION:
    save greet string to <greet.dat> file at Off*16 pos,
INPUT:
    Off         : greet info offset in file <greet.dat> (Off= fp_off/16)
    pGreetInfo  : greet info struct
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSetGreet(unsigned short Off, WinksStrgFile_GreetInfo_s *pGreetInfo);

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
int Winks_StrgTrimGreet(void);

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
                                unsigned long *retTakeSize );

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
int Winks_StrgPreTakeupDelete(unsigned short Idx);

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
                                unsigned long *retTakeSize );

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
int Winks_StrgTmpTakeupDelete(unsigned short Idx);
#endif

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
int Winks_StrgSaveCCImageBegin(unsigned short Idx, WinksStrgFile_CCImage_s *pCCImage, int IfCached);

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
int Winks_StrgSaveCCImageDo(unsigned short Idx, WinksStrgFile_CCImage_s *pCCImage);

/*****************************************************************************
DESCRIPTION:
    save CC image to file end: close file and rename '.tmp'
    if friend image change prearrange 
INPUT:
    Idx     : index pos
    pCCImage: CC Image info struct
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgSaveCCImageEnd(unsigned short Idx, WinksStrgFile_CCImage_s *pCCImage);
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
int Winks_StrgFreeCCImageCache(void);
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
int Winks_StrgGetDftCCShow(int dir, WinksStrgFile_CCShow_s *pCCShow);

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
int Winks_StrgGetCCShow(char *pNo, int dir, WinksStrgFile_CCShow_s *pCCShow);

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
int Winks_StrgCCIsFriendUsed(unsigned short CC_Idx);

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
int Winks_StrgCCUserNum(unsigned short CC_Idx, int *retFriendUsedNum);

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
int Winks_StrgGetCCImageSize(unsigned short CC_Idx, unsigned int *retImgSize);

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
int Winks_StrgDeleteCCImage(unsigned short CC_Idx);
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
int Winks_StrgTrimCCCache();
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
int Winks_StrgFreeDiskSpace(unsigned long FreeSize);

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
int Winks_StrgDiskIsFull(void);

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
int Winks_StrgLockCC(unsigned short CC_Idx);

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
int Winks_StrgUnlockCC(void);

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
int Winks_StrgFLAdd(char *pNo);

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
int Winks_StrgFLCnt(void);

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
int Winks_StrgFLGet(int BeginIdx, char *pNo);

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
int Winks_StrgFLDelete(char *pNo);

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
int Winks_StrgFLDeleteAll();
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
int Winks_StrgULAdd(char *pNo);

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
int Winks_StrgULCnt(void);

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
int Winks_StrgULGet(int BeginIdx, char *pNo);

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
int Winks_StrgULDelete(char *pNo);

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
int Winks_StrgULDeleteAll(void);

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
int Winks_StrgNDAdd(WinksStrgFile_NeedDown_s *pNeedDown);

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
int Winks_StrgNDCnt();

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
int Winks_StrgNDGet(int BeginIdx, WinksStrgFile_NeedDown_s *pNeedDown);

/*****************************************************************************
DESCRIPTION:
    delete record from file <nd.dat>
    just need set deleted flag
INPUT:
    pNO : phone number string
OUTPUT:
    NULL
RETURN:
    success or failure
******************************************************************************/
int Winks_StrgNDDelete(char *pNo);

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
int Winks_StrgNDDeleteAll();
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
    success or failure
******************************************************************************/
int Winks_StrgGetStatus(void);

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
int Winks_StrgCallEvtAdd(WinksStrgFile_CallInfo_s *pCallInfo);

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
int Winks_StrgCallEvtCnt(void);

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
int Winks_StrgCallEvtGet(unsigned short Idx, WinksStrgFile_CallStat_s *pCallInfo);

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
int Winks_StrgCallEvtGetWithNo(char *pNoBcd, WinksStrgFile_CallStat_s *pCallInfo);

#ifdef WINKS_DEBUG
/*
  0 -- ok card
  1 -- new card
  2 -- old card
*/
int Winks_StrgSetCardStatus(int Flag);
#endif

/* ��¼�˴β���չʾ��������ظ�ֵ��ʾʧ�� */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);
/*
            �����¼���������

[���ز���]
    ���ҿ�λ[Winks_StrgSearchUser][Winks_StrgSearchCC],
    ����<greet.dat>[Winks_StrgSetGreet],
    ����<ccinfo.dat>[Winks_StrgSetCCInfo],
    ����<user.dat>[Winks_StrgSetUserInfo]
    �������[Winks_StrgSaveCCImageBegin]
            [Winks_StrgSaveCCImageDo]
            [Winks_StrgSaveCCImageEnd]

[��̭����]
    ���̿ռ��Ƿ����[Winks_StrgDiskIsFull]
    �ͷ��ƶ��ռ�[Winks_StrgFreeDiskSpace]->[Winks_StrgCCIsFriendUsed][Winks_StrgDeleteCCImage]

[���Ҳ���]
    ���ݲ���ID����[Winks_StrgSearchCC]
    ���ݵ绰�������[Winks_StrgSearchUser]

һ������(���μ����Sim������)ʱѡ�����ܺ�������:
 1��ɾ�����������ļ�[Winks_StrgDeleteDiskFiles]
 2��UI�ṩ�ӿ�ѡ�����ܺ��ѣ����档
        ������ѵ��ڴ�FriendMap[]���ļ�<friend.dat>[Winks_StrgSaveFriend]
 3������������ȡ���ѻ�����Ϣ������
        [���ز���]
                
�������º�����Ϣ
 1���޸ĺ�������:�����ļ�<friend.dat>[Winks_StrgSaveFriend]
 2��ɾ������[Winks_StrgDelFriend]
 3����Ӻ���[Winks_StrgAddFriend]
            [���ز���]

������ʼ��Cache
 1���ϴ��绰��,��ȡ�����б�
        ���ݺ����б�͵绰����������һ�������û������б�<fl.txt>[Winks_StrgFLSave]
 2�����θ��º����б����:
        �Ӻ����б��л�ȡ��Ҫ���صĺ���[Winks_StrgFLGetNext]
        [���ز���]
        ������ϴӺ����б�ɾ��ɾ�����룬����<fl.txt>[Winks_StrgFLDelete]

�ġ�����Ϊ�Ѽ���״̬�����ļ���ӳ���ڴ�:
    [Winks_StrgInitMapFile]

�塢������Ӧ:����\�洢������\�绰�¼�ʱ���:
 1������Ƿ��Ӧ[Winks_StrgGetCardStatus]
 2��ɾ�����ϲ�������[Winks_StrgDeleteDiskFiles]
 3����ʼ��Cache
        ��ȡ<call.dat>���绰�¼��������һ�ݺ����б�<fl.txt>[Winks_StrgFLSave]
        ���θ��º����б����:
            �Ӻ����б��л�ȡ��Ҫ���صĺ���[Winks_StrgFLGetNext]
            [���ز���]
            ������ϴӺ����б�ɾ��ɾ�����룬����<fl.txt>[Winks_StrgFLDelete]

������ʱ����
    ����������ȡ���ѻ�����Ϣ������
       [���ز���]

�ߡ��绰�¼�
 1��������ȡ����Ĳ�����Ϣ
 2�����ز��Ҳ���
        [���Ҳ���]
 3������ҵ��ӱ��ز���[Winks_StrgGetCCShow]�������ʺ���[Winks_StrgGetGreet]
    ���������������[���ز���]
 4���Ҷϵ绰
        �ͷ�Cache[Winks_StrgFreeCCImageCache]
        ������������أ���¼�绰�¼�[Winks_StrgCallEvtAdd]
 5��δ������Ϻ������<ul.txt>[Winks_StrgULAdd]

�ˡ��绰�󲹳�����
 1����<ul.txt>��ȡ��Ҫ���²���ĺ���[Winks_StrgULGet]
 2��[���ز���]����¼�绰�¼�[Winks_StrgCallEvtAdd]
 3����<ul.txt>ɾ������[Winks_StrgULDelete]

*/
#ifdef __cplusplus
}
#endif

#endif/*_WK_STRG_IF_H_*/


