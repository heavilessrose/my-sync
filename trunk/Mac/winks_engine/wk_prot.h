
///////////////////////////////////////////////////////////////////////////////
//           M A C R O   D E C L A R E
///////////////////////////////////////////////////////////////////////////////
/* 解析Tag汇总 */
typedef enum tag_WinksTag_e
{
    WINKS_TAG_NONE          = 0 ,
/* WinksProt_result_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_result            ,
      WINKS_TAG_code              ,
      WINKS_TAG_text              ,
      WINKS_TAG_greeting          ,
      WINKS_TAG_debug             ,
/* WinksProt_serviceConfig_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_serviceConfig     ,
      WINKS_TAG_address           ,
      WINKS_TAG_connection        ,
      WINKS_TAG_config            ,
      WINKS_TAG_client            ,
        WINKS_TAG_winks             ,
        //WINKS_TAG_media             ,
        WINKS_TAG_wap               ,
        WINKS_TAG_sms               ,
          WINKS_TAG_number            ,
          WINKS_TAG_prefix            ,
        WINKS_TAG_push              ,
        WINKS_TAG_proxy             ,
        WINKS_TAG_proxyPort         ,
        WINKS_TAG_apn               ,
        WINKS_TAG_subscription      ,
        WINKS_TAG_diy               ,
        WINKS_TAG_vcard             ,
        WINKS_TAG_activity          ,
        WINKS_TAG_vcash             ,
          WINKS_TAG_limited           ,
          WINKS_TAG_trial             ,
          WINKS_TAG_adware            ,
          WINKS_TAG_subscriber        ,
        WINKS_TAG_autoSubscribe     ,
/* WinksProt_userServiceStatus_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_userServiceStatus ,
      WINKS_TAG_unregistered      ,
      WINKS_TAG_uid               ,
      WINKS_TAG_services          ,
        WINKS_TAG_registerMethod    ,
        //WINKS_TAG_subscription      ,
        //WINKS_TAG_diy               ,
        //WINKS_TAG_vcard             ,
        //WINKS_TAG_activity          ,
        //WINKS_TAG_vcash             ,
/* ServicesType_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    //WINKS_TAG_services          ,
      //WINKS_TAG_subscription      ,
      //WINKS_TAG_diy               ,
      //WINKS_TAG_vcard             ,
      //WINKS_TAG_activity          ,
      //WINKS_TAG_vcash             ,
/* WinksProt_userConfig_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_userConfig        ,
      //WINKS_TAG_proxy             ,
      //WINKS_TAG_proxyPort         ,
      //WINKS_TAG_apn               ,
      WINKS_TAG_durationAfterHangup,
      WINKS_TAG_showWinks         ,
      WINKS_TAG_showMode          ,
      WINKS_TAG_roaming           ,
      WINKS_TAG_autorun           ,
      WINKS_TAG_updateAfterHangup ,
      WINKS_TAG_upgradeMode       ,
      WINKS_TAG_vcardOverwrite    ,
      WINKS_TAG_connectInCall     ,
      WINKS_TAG_support3G         ,
      WINKS_TAG_networkTimeout    ,
      WINKS_TAG_splashDuration    ,
      WINKS_TAG_updateOptionMinFee,
      WINKS_TAG_updateOptionMostRecent,
      WINKS_TAG_updateConfig      ,
        WINKS_TAG_option            ,
        WINKS_TAG_period            ,
/* WinksProt_winksShow_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_winksShow         ,
      WINKS_TAG_wid               ,
      WINKS_TAG_media             ,
      WINKS_TAG_thumbnail         ,
      //WINKS_TAG_greeting          ,
      WINKS_TAG_status            ,
      WINKS_TAG_statusText        ,
      WINKS_TAG_message           ,
      //WINKS_TAG_vcard             ,
        WINKS_TAG_url               ,
        WINKS_TAG_w                 ,
        WINKS_TAG_h                 ,
        WINKS_TAG_name              ,
        WINKS_TAG_mobile            ,
        WINKS_TAG_corporation       ,
        WINKS_TAG_title             ,
        WINKS_TAG_phone             ,
        WINKS_TAG_email             ,
        WINKS_TAG_fax               ,
        //WINKS_TAG_address           ,
/* WinksProt_phoneBook_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_phoneBook         ,
      WINKS_TAG_pbi               ,
        WINKS_TAG_n                 ,
        WINKS_TAG_p                 ,
/* WinksProt_UserWinksType_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_userWinks         ,
      //WINKS_TAG_wid               ,
      //WINKS_TAG_name              ,
      //WINKS_TAG_diy               ,
      WINKS_TAG_censor            ,
      WINKS_TAG_duration          ,
      WINKS_TAG_price             ,
      WINKS_TAG_state             ,
      WINKS_TAG_expire            ,
/* WinksProt_winksScript_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_winksScript       ,
      //WINKS_TAG_wid               ,
      //WINKS_TAG_p                 ,
      WINKS_TAG_all               ,
      //WINKS_TAG_greeting          ,
/* WinksProt_shared_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_shared            ,
      //WINKS_TAG_status          ,
      //WINKS_TAG_statusText      ,
      //WINKS_TAG_activity          ,
      //WINKS_TAG_name              ,
      //WINKS_TAG_message           ,
      //WINKS_TAG_showWinks         ,
      WINKS_TAG_showVcard         ,
/* WinksProt_messageList_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_messageList       ,
      WINKS_TAG_mi                ,
        WINKS_TAG_mid               ,
        //WINKS_TAG_serviceConfig     ,
        //WINKS_TAG_userConfig      ,
/* WinksProt_softwarePackage_s */
/*--1-2-3-4-5-6-7-8-9-----------1-2-3-4-5-6-7-8-9--*/
    WINKS_TAG_softwarePackage   ,
      //WINKS_TAG_name              ,
      WINKS_TAG_version           ,
      WINKS_TAG_file              ,
      WINKS_TAG_optional          ,
      WINKS_TAG_description       ,
    WINKS_TAG_MAX
}WinksTag_e;

typedef enum tag_WinksKey_e
{
    WK_KEY_NONE          = 0 ,
    WK_KEY_service      ,
    WK_KEY_config       ,
    WK_KEY_user         ,
    WK_KEY_activate     ,
    WK_KEY_status       ,
    WK_KEY_winks        ,
    WK_KEY_upload       ,
    WK_KEY_show         ,
    WK_KEY_specialwinks ,
    WK_KEY_recommend    ,
    WK_KEY_list         ,
    WK_KEY_purchase     ,
    WK_KEY_script       ,
    WK_KEY_merge        ,
    WK_KEY_software     ,
    WK_KEY_imsi         ,
    WK_KEY_p            ,
    WK_KEY_uid          ,
    WK_KEY_cl           ,
    WK_KEY_mt           ,
    WK_KEY_wt           ,
    WK_KEY_splash       ,
    WK_KEY_carrier      ,
    WK_KEY_wid          ,
    WK_KEY_name         ,
    WK_KEY_ext          ,
    WK_KEY_attachName   ,
    WK_KEY_v            ,
    WK_KEY_an           ,
    WK_KEY_av           ,
    WK_KEY_os           ,
    WK_KEY_hw           ,
    WK_KEY_bua          ,
    WK_KEY_latest       ,
    WK_KEY_message      ,
    WK_KEY_confirm      ,
    WK_KEY_phonebook    ,
    WK_KEY_winksPO      ,
    WK_KEY_count        ,
    WK_KEY_amount       ,
    WK_KEY_auth         ,
    WK_KEY_token        ,
    WK_KEY_adc          ,
    WK_KEY_imei         ,
    WK_KEY_file         ,
    WK_KEY_stat         ,
    WK_KEY_log          ,
    WK_KEY_MAX
}WinksKey_e;
///////////////////////////////////////////////////////////////////////////////
//           S T R U C T   D E C L A R E
///////////////////////////////////////////////////////////////////////////////
typedef struct tag_WinksProt_ParseDataX_s
{
    int     RetType;
    void *  pData;
    Winks_MemPool_s * pMem;
}WinksProt_ParseDataX_s;

typedef struct tag_WinksProt_PackDataX_s
{
    unsigned char   method;         /* WinksHttpMethod_e WINKS_METHOD_xxx*/
    unsigned char   server;         /* WinksServerEnum_e */
    unsigned char   IsOnlyPath;     /* 0--数据,1--路径(diy上传传给http路径) */
    unsigned char   _pad;           
    unsigned long   ContentLen;     /* diy上传时需要填写 */
    unsigned char * pPath;          /* diy上传时需要填写(unicode编码) */
    int             PathLen;        /* diy上传时需要填写(所占字节数) */
    char  * url;
    char  * pContenttype;           /* diy上传时需要填写，其他为text/xml */
    char    imsi[WINKS_IMSI_LEN+1];
    char    uid[WINKS_SERVER_UID_MAX_LEN];
    Winks_XmlElement_s * pRoot;
}WinksProt_PackDataX_s;

///////////////////////////////////////////////////////////////////////////////
//            F U N C T I O N     I N T E R F A C E
///////////////////////////////////////////////////////////////////////////////


