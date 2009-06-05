
#define WINKS_INVALID_GREET_TOLERANCE   50  /* <greet.dat>�����������Ч��¼�� */

#define WINKS_CC_SPACE_RSV  100     /* Ϊ����Ԥ���ڴ�ӳ��ռ� */

#define WINKS_CALL_EVT_VER  0x0001

#define WINKS_GREET_MAGIC   0xabcd1234

#define WINKS_DFT_GREET     L"Hello winks"

#define WINKS_PNO_BCD_CMP_LEN   4
#define WINKS_PNO_STR_CMP_LEN   8

#define WINKS_MAX_UL        3       /* ���ɻ��������δ���ز����� */

#define WINKS_CALL_STAT_PRETAKE_SIZE    100

/* ��ȡlen����Align����ĳ��ȣ�����16�ֽڶ���WINKS_ALIGN(len,16) */
#define WINKS_ALIGN(len, Align)     (((len) + Align - 1) & (unsigned long)(0-Align))
#define WINKS_ARRSIZE(a)    (sizeof(a)/sizeof(a[0]))
///////////////////////////////////////////////////////////////////////////////
//           S T O R A G E    F I L E
///////////////////////////////////////////////////////////////////////////////
/* <user.dat> �û��绰���������ID���ʺ����ӳ���ļ� */
/* <user.dat> �ļ�ͷ*/
typedef struct tag_WinksStrgFile_UserHead_s
{
/* ����������Ա����У��洢���Ƿ�ƥ�� */
    unsigned long   CacheTime;      /* Cache������ʱ�� ��2000-1-1 0:0:0�������
                                       ��洢����<ccinfo.dat>�Ա�,
                                       ����Ǻ�,����Ҫ�ؽ�����Cache */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* �������� (����IMSI��¼,���ܻ���������)
                                       ��洢����<ccinfo.dat>�Ա�,
                                       ����Ǻ�,�洢���Ǹ��û�ʹ�ù� */
    char            _pad[6];
}WinksStrgFile_UserHead_s;

/* <call.dat> �û��绰�¼�ͳ�Ƶ�ӳ���ļ� */
/* <call.dat> �ļ�ͷ*/
#define WINKS_CALL_VER  "1000"  // 1.0.0.0
typedef struct tag_WinksStrgFile_CallHead_s
{
/* �����Ա���ڰ汾�����ݿ��� */
    char            Version[4];     /* �绰��Ϣ��¼�ṹ�İ汾�� */
/* ����������Ա����У��洢���Ƿ�ƥ�� */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* �������� (����IMSI��¼,���ܻ���������)
                                       ��洢����<ccinfo.dat>�Ա�,
                                       ����Ǻ�,�洢���Ǹ��û�ʹ�ù� */
    char            _pad[6];
}WinksStrgFile_CallHead_s;

/* <ccinfo.dat> ������Ϣ�ļ� */
/* <ccinfo.dat> �ļ�ͷ*/
typedef struct tag_WinksStrgFile_CCHead_s
{
/* ����������Ա����У��洢���Ƿ�ƥ�� */
    unsigned long   CacheTime;      /* Cache������ʱ�� ��2000-1-1 0:0:0�������
                                       ���ֻ���<call.dat>�Ա�,
                                       ����Ǻ�,����Ҫ�ؽ�����Cache */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* �������� (����IMSI��¼,���ܻ���������)
                                       ���ֻ���<call.dat>�Ա�,
                                       ����Ǻ�,�洢���Ǹ��û�ʹ�ù� */
    char            _pad[6];
}WinksStrgFile_CCHead_s;

/* <greet.dat> �ʺ�����Ϣ�ļ� */
/* <greet.dat> �ļ�ͷ*/
typedef struct tag_WinksStrgFile_GreetHead_s
{
    unsigned short  elem_cnt;
    unsigned short  valid_cnt;
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
    unsigned char   _pad[6];   /* ����16�ֽ� */
}WinksStrgFile_GreetHead_s;

/* <stat.dat> �ļ�ͷ*/
#define WINKS_STAT_VER  "1000"  // 1.0.0.0
typedef struct tag_WinksStrgFile_StatHead_s
{
/* �����Ա���ڰ汾�����ݿ��� */
    char            Version[4];        /* ͳ����Ϣ��¼�ṹ�İ汾�� */
    char            SelfNoBcd[WINKS_STRG_NOBCD_LEN];
                                    /* �������� (����IMSI��¼,���ܻ���������)
                                       ��<stat.dat>�Ա��ж��ļ��Ƿ���� */
    unsigned char   _pad[6];   /* ����16�ֽ� */
}WinksStrgFile_StatHead_s;

/* <friend.dat> �����ļ� */

/* <fl.txt> �����б���� ��ʱ��ź����б����,��һ�ν���Cache�� */

/* <ul.txt> �绰�¼����貹�����صĺ����б� */

/* <nd.dat> ���ݵ绰��¼�ؽ�Cacheʱ���Ƚ�����Ĳ�����Ϣ�����Ա�Ա� */
///////////////////////////////////////////////////////////////////////////////
//            F I L E    M E M M A P
///////////////////////////////////////////////////////////////////////////////
/* �û�������<user.dat>��ӳ�� */
#if 0
typedef struct tag_WinksMapFile_UserInfo_s
{
    char            NoBcd[WINKS_STRG_NOBCD_LEN];         /* �绰���� BCD-8421�� */
    unsigned short  NoStrgIdx;  /* �洢���ļ�<user.dat>�е���� */

///* Ϊ��ɾ�������ļ�����ٲ���ɾ��<user.dat>��Ӧ��Ϣ���Ӻ����ֶ� */
//    unsigned short  ID_Idx;     /* �洢��<ccinfo.dat>�еĲ���ID��� */
}WinksMapFile_UserInfo_s;
#endif
//WinksMapFile_UserInfo_s UserMap[WINKS_MAX_USER];

/* friend info map */
typedef struct tag_WinksMapFile_FriendInfo_s
{
    char            NO[WINKS_STRG_PHONE_LEN];    /* �绰���� */
    unsigned short  NoStrgIdx;  /* �洢���ļ�<user.dat>�е���� */
}WinksMapFile_FriendInfo_s;
//WinksMapFile_FriendInfo_s FriendMap[WINKS_FRIENDS_MAX_NUMBER];

/* ������<ccinfo.dat>��ӳ�� */
typedef struct tag_WinksMapFile_CCInfo_s
{
    unsigned short  CCStrgIdx;  /* �洢���ļ�<ccinfo.dat>�е���� */

/* Ϊ�˼�����̭�㷨���Ӻ����ֶ� */
    unsigned short  UseCnt;     /* �绰�¼��� */
    unsigned long   LatestUse;  /* ���һ�ε绰�¼���ʱ�� */
}WinksMapFile_CCInfo_s;
//WinksMapFile_CCInfo_s CCMap[WINKS_MAX_USER];

typedef struct tag_WinksFileMap_s
{
    unsigned long   CacheTime;
    char    SelfNoBcd[WINKS_STRG_NOBCD_LEN];
    //WinksMapFile_UserInfo_s * pUserMap;
    WinksStrgFile_UserInfo_s* pUserMap;     /* �û�ռ�� */
    WinksMapFile_FriendInfo_s* pFriendMap;  /* ����ռ�� */
    unsigned short UserNum;         /* �û��� */
    unsigned short FriendNum;       /* ������ */
    unsigned short UserOccupy;      /* �û�ռ���� */
    unsigned short LockCCIdx;       /* �����Ĳ���(���粥�Ų���) */
    unsigned char * pCCCache;       /* ��̬����ͼƬCache */
    unsigned long   CCCacheOff;     /* ͼƬCache���� */
    char CCCacheNoBcd[WINKS_STRG_NOBCD_LEN];
}WinksFileMap_s;

#define DIR_STRG        "strg"          //�洢��Ŀ¼

#define FILE_SETTING    "settings.dat" //ϵͳ�����ļ�
#define FILE_CALL       "call.dat"     //�û��绰�¼�ͳ�Ƶ�ӳ���ļ�
#define FILE_FRIEND     "friend.dat"   //�����ļ� 
#define FILE_USER       "user.dat"     //�û��绰���������ID���ʺ����ӳ���ļ� 
#define FILE_CCINFO     "ccinfo.dat"   //������Ϣ�ļ� 
#define FILE_GREET      "greet.dat"    //�ʺ�����Ϣ�ļ� 
#define FILE_STAT       "stat.dat"    //ͳ���ļ� 

#define TMP_FL          "fl.dat"       //��ʱ�ļ�--�����б���� ��ʱ��ź����б����,��һ�ν���Cache�� 
#define TMP_UL          "ul.dat"       //��ʱ�ļ�--�������صĺ����б�������3������ ��ʱ��� 
#define TMP_ND          "nd.dat"       //��ʱ�ļ�--���뼰������Ϣ

#define DIR_IMG         "img"           //�����ļ�Ŀ¼
#define PRE_IMG         "Img"           //�����ļ���ǰ׺
#define SUF_TMP         ".tmp"          //������ʱ�ļ���չ��
#define SUF_PTF         ".ptf"          //����Ԥռ�ļ���չ��

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
            �����ļ��洢���ڴ�ӳ��

�ļ�����:
 <settings.dat> ϵͳ�����ļ�
 <user.dat>     �û��绰���������ID���ʺ����ӳ���ļ� 
 <call.dat>     �û��绰�¼�ͳ�Ƶ�ӳ���ļ�
 <ccinfo.dat>  ������Ϣ�ļ� 
 <greet.dat>    �ʺ�����Ϣ�ļ� 
 <friend.dat>   �����ļ� 
 <fl.txt>       ��ʱ�ļ�--�����б���� ��ʱ��ź����б����,��һ�ν���Cache�� 
 <ul.txt>       ��ʱ�ļ�--�������صĺ����б�������3������ ��ʱ��� 
 <nd.dat>       ��ʱ�ļ�--���뼰������Ϣ

�ٶ����֧�ֵ��û�/������ΪWINKS_MAX_USER=1024 �ļ����ڴ�ӳ��ռ������:
 <settings.dat>                                 [�ֻ�Ԥռ]
 <user.dat>      4+ 10*1024  = (10k+4)�ֽ�      [�ֻ�Ԥռ]
 <call.dat>     12+ 12*1024  = (12k+12)�ֽ�     [�ֻ�Ԥռ]
 <ccinfo.dat>  24+ 50*1024  = (50k+24)�ֽ�     [�ֻ�Ԥռ]
 <greet.dat>     4+ 214*1024 = (214k+4)�ֽ�     [��Ԥռ]
 <friend.dat>    36*10 = 360�ֽ�                [�ֻ�Ԥռ]
 <fl.txt>        ��ʱ�ļ�
 <ul.txt>        ��ʱ�ļ�
 <nd.dat>        ��ʱ�ļ�
           �ܼ� ============= 280K + 396
�ڴ�ӳ��:
 UserMap       10*1024      = 10K�ֽ�
 CCMap          8*1024      = 8K�ֽ�
 FriendMap     10*10        = 100�ֽ�
           �ܼ� ============= 18K + 100
*/


