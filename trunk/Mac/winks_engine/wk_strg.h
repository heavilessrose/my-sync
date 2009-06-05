
#define WINKS_INVALID_GREET_TOLERANCE   50  /* <greet.dat>中容许最多无效记录数 */

#define WINKS_CC_SPACE_RSV  100     /* 为彩像预留内存映射空间 */

#define WINKS_CALL_EVT_VER  0x0001

#define WINKS_GREET_MAGIC   0xabcd1234

#define WINKS_DFT_GREET     L"Hello winks"

#define WINKS_PNO_BCD_CMP_LEN   4
#define WINKS_PNO_STR_CMP_LEN   8

#define WINKS_MAX_UL        3       /* 最多可缓冲的来电未下载彩像数 */

#define WINKS_CALL_STAT_PRETAKE_SIZE    100

/* 获取len的以Align对齐的长度，如以16字节对齐WINKS_ALIGN(len,16) */
#define WINKS_ALIGN(len, Align)     (((len) + Align - 1) & (unsigned long)(0-Align))
#define WINKS_ARRSIZE(a)    (sizeof(a)/sizeof(a[0]))
///////////////////////////////////////////////////////////////////////////////
//           S T O R A G E    F I L E
///////////////////////////////////////////////////////////////////////////////
/* <user.dat> 用户电话号码与彩像ID、问候语的映射文件 */
/* <user.dat> 文件头*/
typedef struct tag_WinksStrgFile_UserHead_s
{
/* 下面两个成员用于校验存储卡是否匹配 */
    unsigned long   CacheTime;      /* Cache创建的时间 自2000-1-1 0:0:0起分钟数
                                       与存储卡上<ccinfo.dat>对比,
                                       如果吻合,不需要重建彩像Cache */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* 本机号码 (不以IMSI记录,可能换卡不还号)
                                       与存储卡上<ccinfo.dat>对比,
                                       如果吻合,存储卡是该用户使用过 */
    char            _pad[6];
}WinksStrgFile_UserHead_s;

/* <call.dat> 用户电话事件统计的映射文件 */
/* <call.dat> 文件头*/
#define WINKS_CALL_VER  "1000"  // 1.0.0.0
typedef struct tag_WinksStrgFile_CallHead_s
{
/* 下面成员用于版本及兼容控制 */
    char            Version[4];     /* 电话信息记录结构的版本号 */
/* 下面两个成员用于校验存储卡是否匹配 */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* 本机号码 (不以IMSI记录,可能换卡不还号)
                                       与存储卡上<ccinfo.dat>对比,
                                       如果吻合,存储卡是该用户使用过 */
    char            _pad[6];
}WinksStrgFile_CallHead_s;

/* <ccinfo.dat> 彩像信息文件 */
/* <ccinfo.dat> 文件头*/
typedef struct tag_WinksStrgFile_CCHead_s
{
/* 下面两个成员用于校验存储卡是否匹配 */
    unsigned long   CacheTime;      /* Cache创建的时间 自2000-1-1 0:0:0起分钟数
                                       与手机上<call.dat>对比,
                                       如果吻合,不需要重建彩像Cache */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* 本机号码 (不以IMSI记录,可能换卡不还号)
                                       与手机上<call.dat>对比,
                                       如果吻合,存储卡是该用户使用过 */
    char            _pad[6];
}WinksStrgFile_CCHead_s;

/* <greet.dat> 问候语信息文件 */
/* <greet.dat> 文件头*/
typedef struct tag_WinksStrgFile_GreetHead_s
{
    unsigned short  elem_cnt;
    unsigned short  valid_cnt;
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned char   _pad[6];   /* 补齐16字节 */
}WinksStrgFile_GreetHead_s;

/* <stat.dat> 文件头*/
#define WINKS_STAT_VER  "1000"  // 1.0.0.0
typedef struct tag_WinksStrgFile_StatHead_s
{
/* 下面成员用于版本及兼容控制 */
    char            Version[4];        /* 统计信息记录结构的版本号 */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* 本机号码 (不以IMSI记录,可能换卡不还号)
                                       与<stat.dat>对比判断文件是否可用 */
    unsigned char   _pad[6];   /* 补齐16字节 */
}WinksStrgFile_StatHead_s;

/* <friend.dat> 好友文件 */

/* <fl.txt> 好友列表号码 临时存放好友列表号码,第一次建立Cache用 */

/* <ul.txt> 电话事件后需补充下载的号码列表 */

/* <nd.dat> 根据电话记录重建Cache时，先将号码的彩像信息保存以便对比 */
///////////////////////////////////////////////////////////////////////////////
//            F I L E    M E M M A P
///////////////////////////////////////////////////////////////////////////////
/* 用户号码与<user.dat>的映射 */
#if 0
typedef struct tag_WinksMapFile_UserInfo_s
{
    char            NoBcd[WINKS_STRG_NOBCD_LEN];         /* 电话号码 BCD-8421码 */
    unsigned short  NoStrgIdx;  /* 存储在文件<user.dat>中的序号 */

///* 为了删除彩像文件后加速查找删除<user.dat>对应信息，加后面字段 */
//    unsigned short  ID_Idx;     /* 存储在<ccinfo.dat>中的彩像ID序号 */
}WinksMapFile_UserInfo_s;
#endif
//WinksMapFile_UserInfo_s UserMap[WINKS_MAX_USER];

/* friend info map */
typedef struct tag_WinksMapFile_FriendInfo_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* 电话号码 */
    unsigned short  NoStrgIdx;  /* 存储在文件<user.dat>中的序号 */
}WinksMapFile_FriendInfo_s;
//WinksMapFile_FriendInfo_s FriendMap[WINKS_FRIENDS_MAX_NUMBER];

/* 彩像与<ccinfo.dat>的映射 */
typedef struct tag_WinksMapFile_CCInfo_s
{
    unsigned short  CCStrgIdx;  /* 存储在文件<ccinfo.dat>中的序号 */

/* 为了加速淘汰算法，加后面字段 */
    unsigned short  UseCnt;     /* 电话事件数 */
    unsigned long   LatestUse;  /* 最近一次电话事件的时间 */
}WinksMapFile_CCInfo_s;
//WinksMapFile_CCInfo_s CCMap[WINKS_MAX_USER];

typedef struct tag_WinksFileMap_s
{
    unsigned long   CacheTime;
    char    SelfNoBcd[WINKS_STRG_NOBCD_LEN];
    //WinksMapFile_UserInfo_s * pUserMap;
    WinksStrgFile_UserInfo_s* pUserMap;     /* 用户占用 */
    WinksMapFile_FriendInfo_s* pFriendMap;  /* 好友占用 */
    unsigned short UserNum;         /* 用户数 */
    unsigned short FriendNum;       /* 好友数 */
    unsigned short UserOccupy;      /* 用户占用数 */
    unsigned short LockCCIdx;       /* 锁定的彩像(来电播放彩像) */
    unsigned char * pCCCache;       /* 动态下载图片Cache */
    unsigned long   CCCacheOff;     /* 图片Cache长度 */
    char CCCacheNoBcd[WINKS_STRG_NOBCD_LEN];
}WinksFileMap_s;

#define DIR_STRG        "strg"          //存储主目录

#define FILE_SETTING    "settings.dat" //系统配置文件
#define FILE_CALL       "call.dat"     //用户电话事件统计的映射文件
#define FILE_FRIEND     "friend.dat"   //好友文件 
#define FILE_USER       "user.dat"     //用户电话号码与彩像ID、问候语的映射文件 
#define FILE_CCINFO     "ccinfo.dat"   //彩像信息文件 
#define FILE_GREET      "greet.dat"    //问候语信息文件 
#define FILE_STAT       "stat.dat"    //统计文件 

#define TMP_FL          "fl.dat"       //临时文件--好友列表号码 临时存放好友列表号码,第一次建立Cache用 
#define TMP_UL          "ul.dat"       //临时文件--补充下载的号码列表，存放最近3个号码 临时存放 
#define TMP_ND          "nd.dat"       //临时文件--号码及彩像信息

#define DIR_IMG         "img"           //彩像文件目录
#define PRE_IMG         "Img"           //彩像文件名前缀
#define SUF_TMP         ".tmp"          //彩像临时文件扩展名
#define SUF_PTF         ".ptf"          //彩像预占文件扩展名

#define WINKS_DEF_IMG       "defwinks.gif"

typedef enum tag_WinksStrgFileEnum_e
{
    WINKS_STRG_FILE_ENUM_CALL ,
    WINKS_STRG_FILE_ENUM_FRIEND ,
    WINKS_STRG_FILE_ENUM_UL ,
    WINKS_STRG_FILE_ENUM_USER ,
    WINKS_STRG_FILE_ENUM_CCINFO ,
    WINKS_STRG_FILE_ENUM_STAT ,
    WINKS_STRG_FILE_ENUM_GREET ,
#ifdef WINKS_SUPPORT_MEMORY_CARD
    WINKS_STRG_FILE_ENUM_CCINFO_1 ,
    WINKS_STRG_FILE_ENUM_GREET_1 ,
#endif
    WINKS_STRG_FILE_ENUM_MAX
}WinksStrgFileEnum_e;


/*
            彩像文件存储及内存映射

文件汇总:
 <settings.dat> 系统配置文件
 <user.dat>     用户电话号码与彩像ID、问候语的映射文件 
 <call.dat>     用户电话事件统计的映射文件
 <ccinfo.dat>  彩像信息文件 
 <greet.dat>    问候语信息文件 
 <friend.dat>   好友文件 
 <fl.txt>       临时文件--好友列表号码 临时存放好友列表号码,第一次建立Cache用 
 <ul.txt>       临时文件--补充下载的号码列表，存放最近3个号码 临时存放 
 <nd.dat>       临时文件--号码及彩像信息

假定最多支持的用户/彩像数为WINKS_MAX_USER=1024 文件及内存映射占用如下:
 <settings.dat>                                 [手机预占]
 <user.dat>      4+ 10*1024  = (10k+4)字节      [手机预占]
 <call.dat>     12+ 12*1024  = (12k+12)字节     [手机预占]
 <ccinfo.dat>  24+ 50*1024  = (50k+24)字节     [手机预占]
 <greet.dat>     4+ 214*1024 = (214k+4)字节     [不预占]
 <friend.dat>    36*10 = 360字节                [手机预占]
 <fl.txt>        临时文件
 <ul.txt>        临时文件
 <nd.dat>        临时文件
           总计 ============= 280K + 396
内存映射:
 UserMap       10*1024      = 10K字节
 CCMap          8*1024      = 8K字节
 FriendMap     10*10        = 100字节
           总计 ============= 18K + 100
*/


