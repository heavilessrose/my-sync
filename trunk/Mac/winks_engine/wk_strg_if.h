#ifndef _WK_STRG_IF_H_
#define _WK_STRG_IF_H_

#ifdef __cplusplus
extern "C" {
#endif


#define WINKS_STRG_NOBCD_LEN 6      /* 存储的电话号码长度 BCD-8421码 */
#define WINKS_STRG_CC_ID_LEN 40     /* 存储的彩像ID长度 */
#define WINKS_STRG_GREET_LEN 202    /* 存储的问候语长度 */

#define WINKS_STRG_PHONE_LEN 16  /* 存储的好友电话字符串 */
#define WINKS_STRG_NICK_LEN  32  /* 存储的好友昵称字符串 */

#define WINKS_MIN_USER      64      /* 最少cache的用户数 */
#define WINKS_MAX_USER      1024    /* 最多cache的用户数 上限2048(问候语限制) */

#define WINKS_INVALID_CALL_TIME     10  /* 小于该时间长度的电话事件为无效,可不关心彩像 */
#define WINKS_IGNORE_CALL_TIME      30  /* 小于该时间长度的电话事件在空间不足时可不关心彩像 */

/* 片头运营商按特殊好友处理 */
#define WINKS_DFT_CC_NO     "1"     /* 默认彩像ID */

#define WINKS_PRI_CC_NO     "11"    /* 片头彩像起始ID */
#define WINKS_PRI_CC_MAX    1       /* 片头彩像最大个数 <10 */
#define WINKS_PRI_BCD_MIN   0x11
#define WINKS_PRI_BCD_MAX   (WINKS_PRI_BCD_MIN+WINKS_PRI_CC_MAX)

#define WINKS_BUS_CC_NO     "21"    /* 运营商彩像起始ID */
#define WINKS_BUS_CC_MAX    1       /* 运营商彩像最大数 <10 */
#define WINKS_BUS_BCD_MIN   0x21
#define WINKS_BUS_BCD_MAX   (WINKS_BUS_BCD_MIN+WINKS_PRI_CC_MAX)

#define WINKS_PRIBUS_NUM    (WINKS_PRI_CC_MAX+WINKS_BUS_CC_MAX) /* 片头运营商预留数 */

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
/* <user.dat> 用户电话号码与彩像ID、问候语的映射文件 */
/* <user.dat> 文件信息，只记录彩像用户*/
typedef struct tag_WinksStrgFile_UserInfo_s
{
    char            NoBcd[WINKS_STRG_NOBCD_LEN];      /* 电话号码 BCD-8421码 */
    unsigned short  CCIdx;      /* 存储在<ccinfo.dat>中的彩像ID序号 */
    unsigned short  GrtOff;     /* 存储在<greet.dat>中的问候语偏移
                                   以16字节对齐，乘16为实际偏移 */
}WinksStrgFile_UserInfo_s;

/* <call.dat> 用户电话事件统计的映射文件 */
/* <call.dat> 文件信息*/
typedef struct tag_WinksStrgFile_CallStat_s
{
    char            NoBcd[WINKS_STRG_NOBCD_LEN];      /* 电话号码 BCD-8421码 */
    unsigned short  CallCnt;        /* 电话事件次数 */
    unsigned short  CallTime;       /* 最近一次电话通话时间(按秒计算) */
    unsigned short  Flag;           /* 是否彩向用户(可扩展别的标志) */
    unsigned long   LatestCallClock;  /* 最近一次电话事件的时刻 
                                         自2000-1-1 0:0:0起分钟数 */
}WinksStrgFile_CallStat_s;

/* <ccinfo.dat> 彩像信息文件 */
/* <ccinfo.dat> 文件信息*/
typedef struct tag_WinksStrgFile_CCInfo_s
{
    char            ID[WINKS_STRG_CC_ID_LEN];      /* 彩像ID */
    unsigned char   img_type;   /* 彩像类型 WinksFileExt_e */
    unsigned char   img_cnt;    /* 图片数 */
    unsigned short  interval;   /* 图片播放间隔 */
    unsigned short  UseCnt;     /* 电话事件数 */
    unsigned long   LatestUse;  /* 最近一次电话事件的时间 
                                   自2000-1-1 0:0:0起分钟数 */
}WinksStrgFile_CCInfo_s;

/* <greet.dat> 问候语信息文件 */
/* <greet.dat> 文件信息*/
/* 问候语记录为变长格式，每条记录以16字节对齐，用WINKS_ALIGN(len,16)计算 
   同一号码问候语更新时追加到最后(不一定是文件尾)，
   当无效问候语超过一定数量或开机初始化时整理文件 */
typedef struct tag_WinksStrgFile_GreetInfo_s
{
    unsigned long   Magic;          /* XX 问候语标识[由存储添加] */
    char            NoBcd[WINKS_STRG_NOBCD_LEN];           /* 电话号码 BCD-8421码 */
    unsigned char   GreetLen;       /* 问候语长度 */
    unsigned char   Occupy;         /* XX 整个问候语记录占用长度[由存储计算] */
    short           GreetStr[WINKS_STRG_GREET_LEN/2];  /* 问候语，根据实际长度保存 */
}WinksStrgFile_GreetInfo_s;

/* <stat.dat> 文件信息*/
typedef struct tag_WinksStrgFile_Stat_s
{
    char            ID[WINKS_STRG_CC_ID_LEN];      /* 彩像ID */
    unsigned short  show_times; /* 彩像展示次数 */
    unsigned short  int_times;  /* 彩像展示中断次数 */
    unsigned long   time;       /* 彩像播放时间 */
}WinksStrgFile_Stat_s;

/* <friend.dat> 好友文件 */
typedef struct tag_WinksStrgFile_FriendInfo_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* 电话号码 字符串 */
    unsigned char   Nick[WINKS_STRG_NICK_LEN];   /* 好友昵称 */
}WinksStrgFile_FriendInfo_s;

///////////////////////////////////////////////////////////////////////////////
//            F U N C T I O N     I N T E R F A C E
///////////////////////////////////////////////////////////////////////////////
typedef struct tag_WinksStrgFile_CCImage_s
{
    char          * NO;         /* 电话号码字符串 */
    char          * ID;         /* 彩像ID */
    unsigned char   img_type;   /* 彩像类型 WinksFileExt_e */
    unsigned char   img_idx;    /* 第几张图片 */
    unsigned char   img_cnt;    /* 图片数 */
    unsigned char   _pad;
    unsigned long   FileSize;   /* 文件大小 */
    int             file_handle;/* 文件句柄(FILE *),在Begin创建,在Do使用,在End关闭 */
    Winks_MemPool_s* pMem;      /* 存放下载的文件数据 */
    unsigned long   blocklen;   /* 当前下载块总大小(应该与内存列表中pMem->offset累加值相等) */
}WinksStrgFile_CCImage_s;

typedef struct tag_WinksStrgFile_CCShow_s
{
    int             RetStatus;                   /* 彩像是否与该号码对应
                                                    0 -- ok
                                                    1 -- 默认彩像
                                                    2 -- 片头 
                                                    3 -- 运营商 */
    char            NO[WINKS_STRG_PHONE_LEN];    /* 电话号码字符串 */
    char            Name[WINKS_STRG_NICK_LEN];   /* 好友昵称 */
    char            ID[WINKS_STRG_CC_ID_LEN];    /* 彩像ID */
    unsigned short  CC_Idx;     /* 彩像Idx */
    unsigned short  interval;   /* 图片播放间隔 */
    unsigned char   img_type;   /* 彩像类型 WinksFileExt_e */
    unsigned char   img_cnt;    /* 图片数 */
    unsigned char   GreetLen;   /* 问候语长度 */
    unsigned char   SaveType;   /* 彩像存储类型 0--文件 1--内存 */
    unsigned char * pCCImage;   /* 存储的彩像内存数据 */
    unsigned long   CCImageSize;/* 存储的彩像内存数据大小 */
    char            CCImgPath[4][256];      /* 存储的彩像路径 */
    short           GreetStr[WINKS_STRG_GREET_LEN/2];     /* 存储的彩像问候语 */
}WinksStrgFile_CCShow_s;

typedef struct tag_WinksStrgFile_CallInfo_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* 电话号码字符串 */
    char            ID[WINKS_STRG_CC_ID_LEN];    /* 彩像ID */
    unsigned long   CallClock;  /* 通话时刻 */
    unsigned short  CallTime;   /* 通话时长(以秒为单位，超过65535秒按65535算) */
    unsigned char   CallType;   /* 来电0或去电1 */
    unsigned char   _pad;
}WinksStrgFile_CallInfo_s;

/* 根据电话事件重建Cache时，因为已存有许多彩像，不宜全删，所以先将号码对应的
   彩像信息下载，问候语直接保存，其余保存到 */
typedef struct tag_WinksStrgFile_NeedDown_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* 电话号码字符串 */
    char            ID[WINKS_STRG_CC_ID_LEN];    /* 彩像ID */
    unsigned char   cc_type;    /* 彩像类型:1--片头 2--运营商 3--用户 */
    unsigned char   img_cnt;    /* 彩像ID对应的图片张数 */
    unsigned short  interval;   /* 图片播放间隔 */
}WinksStrgFile_NeedDown_s;

typedef enum tag_WinksStrgSearchResult_e
{
    WINKS_STRG_SEARCH_RESULT_NOT_FOUND = -20,     /* 没有找到 */
    WINKS_STRG_SEARCH_RESULT_FAIL ,    /* 查找失败 */
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
	Disk	: phone [1] 、 memory card[2] 、phone and card[3]
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

/* 记录此次彩像展示情况。返回负值表示失败 */
int Winks_log_cc_show(char * CC_ID, unsigned long time, unsigned char interrupted);
/*
            彩像事件处理流程

[下载彩像]
    查找空位[Winks_StrgSearchUser][Winks_StrgSearchCC],
    更新<greet.dat>[Winks_StrgSetGreet],
    更新<ccinfo.dat>[Winks_StrgSetCCInfo],
    更新<user.dat>[Winks_StrgSetUserInfo]
    保存彩像[Winks_StrgSaveCCImageBegin]
            [Winks_StrgSaveCCImageDo]
            [Winks_StrgSaveCCImageEnd]

[淘汰彩像]
    磁盘空间是否充足[Winks_StrgDiskIsFull]
    释放制定空间[Winks_StrgFreeDiskSpace]->[Winks_StrgCCIsFriendUsed][Winks_StrgDeleteCCImage]

[查找彩像]
    根据彩像ID查找[Winks_StrgSearchCC]
    根据电话号码查找[Winks_StrgSearchUser]

一、激活(初次激活或换Sim卡激活)时选择亲密好友流程:
 1、删除上述所有文件[Winks_StrgDeleteDiskFiles]
 2、UI提供接口选择亲密好友，保存。
        保存好友到内存FriendMap[]和文件<friend.dat>[Winks_StrgSaveFriend]
 3、依次联网获取好友基本信息及彩像
        [下载彩像]
                
二、更新好友信息
 1、修改好友姓名:更新文件<friend.dat>[Winks_StrgSaveFriend]
 2、删除好友[Winks_StrgDelFriend]
 3、添加好友[Winks_StrgAddFriend]
            [下载彩像]

三、初始化Cache
 1、上传电话簿,获取好友列表
        根据好友列表和电话簿整理并保存一份其他用户号码列表<fl.txt>[Winks_StrgFLSave]
 2、依次更新好友列表彩像:
        从好友列表中获取需要下载的号码[Winks_StrgFLGetNext]
        [下载彩像]
        下载完毕从好友列表删除删除号码，更新<fl.txt>[Winks_StrgFLDelete]

四、开机为已激活状态，读文件及映射内存:
    [Winks_StrgInitMapFile]

五、卡不对应:开机\存储卡插入\电话事件时检查:
 1、检查是否对应[Winks_StrgGetCardStatus]
 2、删除卡上彩像数据[Winks_StrgDeleteDiskFiles]
 3、初始化Cache
        读取<call.dat>按电话事件情况整理一份好友列表<fl.txt>[Winks_StrgFLSave]
        依次更新好友列表彩像:
            从好友列表中获取需要下载的号码[Winks_StrgFLGetNext]
            [下载彩像]
            下载完毕从好友列表删除删除号码，更新<fl.txt>[Winks_StrgFLDelete]

六、定时更新
    依次联网获取好友基本信息及彩像
       [下载彩像]

七、电话事件
 1、联网获取号码的彩像信息
 2、本地查找彩像
        [查找彩像]
 3、如果找到从本地播放[Winks_StrgGetCCShow]，播放问候语[Winks_StrgGetGreet]
    否则继续联网下载[下载彩像]
 4、挂断电话
        释放Cache[Winks_StrgFreeCCImageCache]
        如果彩像已下载，记录电话事件[Winks_StrgCallEvtAdd]
 5、未下载完毕号码计入<ul.txt>[Winks_StrgULAdd]

八、电话后补充下载
 1、从<ul.txt>获取需要更新彩像的号码[Winks_StrgULGet]
 2、[下载彩像]，记录电话事件[Winks_StrgCallEvtAdd]
 3、从<ul.txt>删除号码[Winks_StrgULDelete]

*/
#ifdef __cplusplus
}
#endif

#endif/*_WK_STRG_IF_H_*/


