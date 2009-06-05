#if 1
#include "wk_public.h"
#include "wk_osal.h"
//#include "../xml/wk_xml.h"
//#include "wk_xmlgb.h"
#include "wk_enfnc.h"
#else/* for demo debug */
#include <stdio.h>
#include "../xml/wk_xml.h"

#define Winks_printf    printf
#endif

//#include "wk_prot_if.h"
#include "wk_prot.h"


#define WINKS_PROT_MEM_BLOCK_LEN     1024
#define WINKS_ALIGN(len, Align)     (((len) + Align - 1) & (unsigned long)(0-Align))
#define WINKS_TAGCMP(s1,s2)         strcmp(s1,s2)
#define WK_ARRSIZE(x)    (sizeof(x)/sizeof(x[0]))     /*取数组元素个数*/

typedef struct tag_WinksTagName_s
{
    WinksTag_e      TagId;
#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
    char  * txt;
#else
    char    txt[16];
#endif
    
}WinksTagName_s;

static const WinksTagName_s TagName[] =
{
    {WINKS_TAG_NONE               , "123456789012345"},
/* WinksProt_result_s */          
    {WINKS_TAG_result             , "result"         },
    {WINKS_TAG_code               , "code"           },
    {WINKS_TAG_text               , "text"           },
    {WINKS_TAG_greeting           , "greeting"       },
    {WINKS_TAG_debug              , "debug"          },
/* WinksProt_serviceConfig_s */   
    {WINKS_TAG_serviceConfig      , "serviceConfig"  },
    {WINKS_TAG_address            , "address"        },
    {WINKS_TAG_connection         , "connection"     },
    {WINKS_TAG_config             , "config"         },
    {WINKS_TAG_client             , "client"         },
    {WINKS_TAG_winks              , "winks"          },
    //{WINKS_TAG_media              , "media"          },
    {WINKS_TAG_wap                , "wap"            },
    {WINKS_TAG_sms                , "sms"            },
    {WINKS_TAG_number             , "number"         },
    {WINKS_TAG_prefix             , "prefix"         },
    {WINKS_TAG_push               , "push"           },
    {WINKS_TAG_proxy              , "proxy"          },
    {WINKS_TAG_proxyPort          , "proxyPort"      },
    {WINKS_TAG_apn                , "apn"            },
    {WINKS_TAG_subscription       , "subscription"   },
    {WINKS_TAG_diy                , "diy"            },
    {WINKS_TAG_vcard              , "vcard"          },
    {WINKS_TAG_activity           , "activity"       },
    {WINKS_TAG_vcash              , "vcash"          },
    {WINKS_TAG_limited            , "limited"        },
    {WINKS_TAG_trial              , "trial"          },
    {WINKS_TAG_adware             , "vcash"          },
    {WINKS_TAG_subscriber         , "subscriber"     },
    {WINKS_TAG_autoSubscribe      , "autoSubscribe"  },
/* WinksProt_userServiceStatus_s */
    {WINKS_TAG_userServiceStatus  , "#0"/*"userServiceStatus"*/},
    {WINKS_TAG_unregistered       , "needAuth"       },
    {WINKS_TAG_uid                , "uid"            },
    {WINKS_TAG_services           , "services"       },
    {WINKS_TAG_registerMethod     , "authMethod"     },
    //{WINKS_TAG_subscription       , "subscription"   },
    //{WINKS_TAG_diy                , "diy"            },
    //{WINKS_TAG_vcard              , "vcard"          },
    //{WINKS_TAG_activity           , "activity"       },
    //{WINKS_TAG_vcash              , "vcash"          },
/* ServicesType_s */
    //{WINKS_TAG_services           , "services"       },
    //{WINKS_TAG_subscription       , "subscription"   },
    //{WINKS_TAG_diy                , "diy"            },
    //{WINKS_TAG_vcard              , "vcard"          },
    //{WINKS_TAG_activity           , "activity"       },
    //{WINKS_TAG_vcash              , "vcash"          },
/* WinksProt_userConfig_s */
    {WINKS_TAG_userConfig         , "userConfig"     },
    //{WINKS_TAG_proxy              , "proxy"          },
    //{WINKS_TAG_proxyPort          , "proxyPort"      },
    //{WINKS_TAG_apn                , "apn"            },
    {WINKS_TAG_durationAfterHangup, "#1"/*"durationAfterHangup"*/},
    {WINKS_TAG_showWinks          , "showWinks"      },
    {WINKS_TAG_showMode           , "showMode"       },
    {WINKS_TAG_roaming            , "roaming"        },
    {WINKS_TAG_autorun            , "autorun"        },
    {WINKS_TAG_updateAfterHangup  , "#2"/*"updateAfterHangup"*/},
    {WINKS_TAG_upgradeMode        , "upgradeMode"    },
    {WINKS_TAG_vcardOverwrite     , "vcardOverwrite" },
    {WINKS_TAG_connectInCall      , "connectInCall"  },
    {WINKS_TAG_support3G          , "support3G"      },
    {WINKS_TAG_networkTimeout     , "networkTimeout" },
    {WINKS_TAG_splashDuration     , "splashDuration" },
    {WINKS_TAG_updateOptionMinFee , "#3"/*"updateOptionMinFee"*/},
    {WINKS_TAG_updateOptionMostRecent, "#4"/*"updateOptionMostRecent"*/},
    {WINKS_TAG_updateConfig       , "updateConfig"   },
    {WINKS_TAG_option             , "option"         },
    {WINKS_TAG_period             , "period"         },
/* WinksProt_winksShow_s */
    {WINKS_TAG_winksShow          , "winksShow"      },
    {WINKS_TAG_wid                , "wid"            },
    {WINKS_TAG_media              , "media"          },
    {WINKS_TAG_thumbnail          , "thumbnail"      },
    //{WINKS_TAG_greeting           , "greeting"       },
    {WINKS_TAG_status             , "presence"       },
    {WINKS_TAG_statusText         , "presenceText"   },
    {WINKS_TAG_message            , "message"        },
    //{WINKS_TAG_vcard              , "vcard"          },
    {WINKS_TAG_url                , "url"            },
    {WINKS_TAG_w                  , "w"              },
    {WINKS_TAG_h                  , "h"              },
    {WINKS_TAG_name               , "name"           },
    {WINKS_TAG_mobile             , "mobile"         },
    {WINKS_TAG_corporation        , "corporation"    },
    {WINKS_TAG_title              , "title"          },
    {WINKS_TAG_phone              , "phone"          },
    {WINKS_TAG_email              , "email"          },
    {WINKS_TAG_fax                , "fax"            },
    //{WINKS_TAG_address            , "address"        },
/* WinksProt_phoneBook_s */       
    {WINKS_TAG_phoneBook          , "phoneBook"      },
    {WINKS_TAG_pbi                , "pbi"            },
    {WINKS_TAG_n                  , "n"              },
    {WINKS_TAG_p                  , "p"              },
/* WinksProt_UserWinksType_s */   
    {WINKS_TAG_userWinks          , "userWinks"      },
    //{WINKS_TAG_wid                , "wid"            },
    //{WINKS_TAG_name               , "name"           },
    //{WINKS_TAG_diy                , "diy"            },
    {WINKS_TAG_censor             , "censor"         },
    {WINKS_TAG_duration           , "duration"       },
    {WINKS_TAG_price              , "price"          },
    {WINKS_TAG_state              , "state"          },
    {WINKS_TAG_expire             , "expire"         },
/* WinksProt_winksScript_s */     
    {WINKS_TAG_winksScript        , "winksScript"    },
    //{WINKS_TAG_wid                , "wid"            },
    //{WINKS_TAG_p                  , "p"              },
    {WINKS_TAG_all                , "all"            },
    //{WINKS_TAG_greeting           , "greeting"       },
/* WinksProt_shared_s */          
    {WINKS_TAG_shared             , "shared"         },
    //{WINKS_TAG_status             , "presence"        },
    //{WINKS_TAG_statusText         , "presenceText"    },
    //{WINKS_TAG_activity           , "activity"       },
    //{WINKS_TAG_name               , "name"           },
    //{WINKS_TAG_message            , "message"        },
    //{WINKS_TAG_showWinks          , "showWinks"      },
    {WINKS_TAG_showVcard          , "showVcard"      },
/* WinksProt_messageList_s */     
    {WINKS_TAG_messageList        , "messageList"    },
    {WINKS_TAG_mi                 , "mi"             },
    {WINKS_TAG_mid                , "mid"            },
    //{WINKS_TAG_serviceConfig      , "serviceConfig"  },
    //{WINKS_TAG_address            , "address"        },
    //{WINKS_TAG_config             , "config"         },
    //{WINKS_TAG_winks              , "winks"          },
    //{WINKS_TAG_media              , "media"          },
    //{WINKS_TAG_wap                , "wap"            },
    //{WINKS_TAG_sms                , "sms"            },
    //{WINKS_TAG_push               , "push"           },
    //{WINKS_TAG_subscription       , "subscription"   },
    //{WINKS_TAG_diy                , "diy"            },
    //{WINKS_TAG_vcard              , "vcard"          },
    //{WINKS_TAG_activity           , "activity"       },
    //{WINKS_TAG_vcash              , "vcash"          },
    //{WINKS_TAG_limited            , "limited"        },
    //{WINKS_TAG_trial              , "trial"          },
    //{WINKS_TAG_adware             , "vcash"          },
    //{WINKS_TAG_subscriber         , "subscriber"     },
/* WinksProt_softwarePackage_s */
    {WINKS_TAG_softwarePackage    , "softwarePackage"},
    //{WINKS_TAG_name               , "name"           },
    {WINKS_TAG_version            , "version"        },
    {WINKS_TAG_file               , "file"           },
    {WINKS_TAG_optional           , "optional"       },
    {WINKS_TAG_description        , "description"    },
};

static const WinksTagName_s registerMethodVal[] =
{
    {WINKS_registerMethod_sms  , "sms"  },
    {WINKS_registerMethod_push , "push" },
    {WINKS_registerMethod_auto , "auto" },
};

static const WinksTagName_s SubscriptionModeTypeVal[] =
{
    {WINKS_SubscriptionModeType_none       , "none"},
    {WINKS_SubscriptionModeType_limited    , "limited"  },
    {WINKS_SubscriptionModeType_trial      , "trial" },
    {WINKS_SubscriptionModeType_adware     , "adware" },
    {WINKS_SubscriptionModeType_subscriber , "subscriber" },
};

static const WinksTagName_s BoolTypeVal[] =
{
    {WINKS_BoolType_false , "false" },
    {WINKS_BoolType_true  , "true"  },
};

static const WinksTagName_s SwitchVal[] =
{
    {WINKS_BoolType_false , "off" },
    {WINKS_BoolType_true  , "on"  },
};


static const WinksTagName_s upgradeModeVal[] =
{
    {WINKS_upgradeMode_auto   , "auto" },
    {WINKS_upgradeMode_manual , "manual"  },
};

static const WinksTagName_s statusVal[] =
{
    {WINKS_status_normal  , "normal" },
    {WINKS_status_metting , "metting" },
};

static const WinksTagName_s censorVal[] =
{
    {WINKS_censor_passed   , "passed" },
    {WINKS_censor_waiting  , "waiting" },
    {WINKS_censor_rejected , "rejected" },
};

static const WinksTagName_s stateVal[] =
{
    {WINKS_state_unrelated , "unrelated" },
    {WINKS_state_usable    , "usable"    },
    {WINKS_state_expired   , "expired"   },
};

static const WinksTagName_s updateConfigVal[] =
{
    {WINKS_updateConfig_minFee      , "minFee" },
    {WINKS_updateConfig_mostRecent  , "mostRecent"    },
    {WINKS_updateConfig_custom      , "custom"   },
};

typedef enum tag_WinksTagType_e
{
    WINKS_TAGTYPE_tree = 0, /* 子结构体节点 */
    WINKS_TAGTYPE_str,      /* ascii字符串 */
    WINKS_TAGTYPE_utf8,     /* utf-8字符串 */
    WINKS_TAGTYPE_atoi,     /* 字符串直接用atoi转换为数字 */
    WINKS_TAGTYPE_pt2i,     /* 根据字符串含义转换为数字 */
    WINKS_TAGTYPE_null,     /* 没有内容，仅靠Tag区分 */
    WINKS_TAGTYPE_attr      /* 可以看作子结构体，多个属性组成 */
}WinksTagType_e;

typedef struct tag_WinksTagContent_s
{
    WinksTag_e      TagId;
    WinksTagType_e  TagType; /* 说明:
                                WINKS_TAGTYPE_tree 需指定子结构体大小，需申请内存;
                                WINKS_TAGTYPE_str  不需指定大小，需申请conlen大小内存;
                                WINKS_TAGTYPE_utf8 不需指定大小，需申请conlen大小内存;
                                WINKS_TAGTYPE_atoi 不需指定大小，已分配内存;
                                WINKS_TAGTYPE_pt2i 不需指定大小，已分配内存;
                                WINKS_TAGTYPE_null 不需指定大小，已分配内存;
                                WINKS_TAGTYPE_attr 需指定子结构体大小，需申请内存; */
#if 0
    int         Off;        /* 成员变量的偏移地址 */
    int         Size;       /* 结构体大小 */
#else
    unsigned char   Off;        /* 成员变量的偏移地址 */
    unsigned char   Size;       /* 结构体大小 */
#endif
}WinksTagContent_s;
///////////////////////////////////////////////////////////////////////////////
//            I N T E R N A L    F U N C T I O N 
///////////////////////////////////////////////////////////////////////////////
static int winks_Prot_pt2i(WinksTag_e TagId, char* pcontent, int conlen)
{
    const WinksTagName_s * pArr;
    int     i, ArrSize;

    switch(TagId)
    {
    case WINKS_TAG_registerMethod:
        pArr = registerMethodVal;
        ArrSize = WK_ARRSIZE(registerMethodVal);
        break;
    case WINKS_TAG_subscription:
        pArr = SubscriptionModeTypeVal;
        ArrSize = WK_ARRSIZE(SubscriptionModeTypeVal);
        break;
    case WINKS_TAG_diy:
    case WINKS_TAG_vcard:
    case WINKS_TAG_activity:
    case WINKS_TAG_vcash:
    case WINKS_TAG_autoSubscribe:
    case WINKS_TAG_showWinks:
    case WINKS_TAG_showVcard:
    case WINKS_TAG_roaming:
    case WINKS_TAG_autorun:
    case WINKS_TAG_updateAfterHangup:
    case WINKS_TAG_connectInCall:
    case WINKS_TAG_support3G:
    case WINKS_TAG_optional:
        pArr = BoolTypeVal;
        ArrSize = WK_ARRSIZE(BoolTypeVal);
        break;
    case WINKS_TAG_upgradeMode:
    case WINKS_TAG_vcardOverwrite:
        pArr = upgradeModeVal;
        ArrSize = WK_ARRSIZE(upgradeModeVal);
        break;
    case WINKS_TAG_status:
        pArr = statusVal;
        ArrSize = WK_ARRSIZE(statusVal);
        break;
    case WINKS_TAG_censor:
        pArr = censorVal;
        ArrSize = WK_ARRSIZE(censorVal);
        break;
    case WINKS_TAG_state:
        pArr = stateVal;
        ArrSize = WK_ARRSIZE(stateVal);
        break;
    case WINKS_TAG_option:
        pArr = updateConfigVal;
        ArrSize = WK_ARRSIZE(updateConfigVal);
        break;
    default:
        Winks_fail("winks_Prot_null2i");
        return -1;
    }
    
    for(i=0; i<ArrSize; i++)
    {
        if( pcontent && conlen &&
            WINKS_TAGCMP(pArr[i].txt, pcontent) == 0 )
            break;
    }

    if(i == ArrSize)
    {
        switch(TagId)
        {
        case WINKS_TAG_diy:
        case WINKS_TAG_vcard:
        case WINKS_TAG_activity:
            pArr = SwitchVal;
            ArrSize = WK_ARRSIZE(SwitchVal);
            break;
        default:
            return -1;
        }
        
        for(i=0; i<ArrSize; i++)
        {
            if( pcontent && conlen &&
                WINKS_TAGCMP(pArr[i].txt, pcontent) == 0 )
                break;
        }
        
        if(i == ArrSize)
            return -1;
    }

    return pArr[i].TagId;
}

static int winks_Prot_null2i(WinksTag_e TagId)
{
    switch(TagId)
    {
    case WINKS_TAG_all:
        return 1;
    case WINKS_TAG_limited:
        return WINKS_SubscriptionModeType_limited;
    case WINKS_TAG_trial:
        return WINKS_SubscriptionModeType_trial;
    case WINKS_TAG_adware:
        return WINKS_SubscriptionModeType_adware;
    case WINKS_TAG_subscriber:
        return WINKS_SubscriptionModeType_subscriber;
    case WINKS_TAG_vcash:
        return 1;
    default:
        Winks_printf("Warn: winks_Prot_null2i(%d) \"%s\"\r\n", TagId, TagName[TagId].txt);
		return 0;
    }
    return -1;
}

static int winks_ProtSetTagVal( Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree, 
                                      WinksTagContent_s  * pTagCont, 
                                      void * pData,
                                      int  * retLen)
{
    int     iRet = -1;
	char  * pBuf = NULL;
    
    switch(pTagCont->TagType)
    {
    case WINKS_TAGTYPE_tree :   /* 子结构体节点 */
                                /* 需指定子结构体大小，需申请内存 */
        if(pData)
        {
            Winks_assert(pTagCont->Size > 0);
            pBuf = Winks_EngMemPoolAlloc(pMemPool, pTagCont->Size, 4 );
            if(pBuf != NULL)
			{
				*(char **)pData = pBuf;
                iRet = 0;
			}
        }
        break;
    case WINKS_TAGTYPE_str  :   /* ascii字符串 */
                                /* 不需指定大小，需申请conlen大小内存 */
        if(pData && pTree->pcontent && pTree->conlen)
        {
            pBuf = Winks_EngMemPoolAlloc(pMemPool, pTree->conlen+1, 0 );
            if(pBuf != NULL)
			{
                strncpy(pBuf, pTree->pcontent, pTree->conlen);
				*(char **)pData = pBuf;
                iRet = 0;
			}
        }
        break;
    case WINKS_TAGTYPE_utf8 :   /* utf-8字符串 */
                                /* 不需指定大小，需申请conlen大小内存 */
        if(pData && pTree->pcontent && pTree->conlen)
        {
            pBuf = Winks_EngMemPoolAlloc(pMemPool, pTree->conlen+1, 0 );
            if(pBuf != NULL)
			{
                Winks_mem_cpy(pBuf, pTree->pcontent, pTree->conlen);
				*(char **)pData = pBuf;
                iRet = 0;
			}
            if(retLen)
                *retLen = pTree->conlen;
        }
        break;
    case WINKS_TAGTYPE_atoi :   /* 字符串直接用atoi转换为数字 */
                                /* 不需指定，已分配内存 */
        if(pData && pTree->pcontent && pTree->conlen)
        {
            *(int *)pData = atoi(pTree->pcontent);
            iRet = 0;
        }
        break;
    case WINKS_TAGTYPE_pt2i :   /* 根据字符串含义转换为数字 */
                                /* 不需指定，已分配内存 */
        if(pData && pTree->pcontent && pTree->conlen)
        {
            int Val = winks_Prot_pt2i(pTagCont->TagId, pTree->pcontent, pTree->conlen);
            if(Val >= 0)
            {
                *(int *)pData = Val;
                iRet = 0;
            }
        }
        break;
    case WINKS_TAGTYPE_null:    /* 没有内容 */
                                /* 不需指定大小，已分配内存 */
        if(pData)
        {
            int Val = winks_Prot_null2i(pTagCont->TagId);
            if(Val >= 0)
            {
                *(int *)pData = Val;
                iRet = 0;
            }
        }
        break;
    #if 0
    case WINKS_TAGTYPE_attr :   /* 可以看作子结构体，多个属性组成 */
                                /* 需指定子结构体大小，需申请内存 */
        if(pData && pTree->pattribute)
        {
            Winks_XmlAttribute_s* pattribute = pTree->pattribute;
            while(pattribute)
            {
                
                pattribute = pattribute->pnext;
            }
            iRet = 0;
        }
        break;
    #endif
    default:
        Winks_fail("winks_Prot_null2i");
        break;
    }

    return iRet;
}

static int winks_ProtSetTagVal_result( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont)
{
/* 参考 TagCont_result */
    WinksProt_result_s * presult = NULL;
    void * pData = NULL;
    int  * pLen = NULL;

    if(pTagCont->TagId == WINKS_TAG_result)
        pData = (void *)&presult;
    else
        pData = (void *)(pMemPool->pdata + pTagCont->Off);

    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    return  winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen);
}


static int winks_ProtSetTagVal_serviceConfig( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_serviceConfig */
    WinksProt_serviceConfig_s * pserviceConfig = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

	if(pTagCont->TagId == WINKS_TAG_serviceConfig)
        pData = (void *)&pserviceConfig;
    else
    {
        struct Prot_sms_s * sms = NULL;
        pserviceConfig = (WinksProt_serviceConfig_s * )pMemPool->pdata;
        
        if(pserviceConfig == NULL)
            return -1;
      #if 1
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_sms          :
        case WINKS_TAG_number       :
        case WINKS_TAG_prefix       :
            sms = pserviceConfig->address->sms;
            while(sms && sms->pnext)
                sms = sms->pnext;
            break;
        }
      #endif
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_address      :
        case WINKS_TAG_connection   :
        case WINKS_TAG_config       :
        case WINKS_TAG_client       :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_winks        :
      #if 0
        case WINKS_TAG_media        :
      #endif
        case WINKS_TAG_wap          :
      #if 0
        case WINKS_TAG_sms          :
      #endif
        case WINKS_TAG_push         :
            if(pserviceConfig->address)
                pData = (void *)((char *)pserviceConfig->address + pTagCont->Off);
            break;
      #if 1
        case WINKS_TAG_sms          :
            if(pserviceConfig->address)
            {
                if(pserviceConfig->address->sms == NULL)
                    pData = (void *)&pserviceConfig->address->sms;
                else
                    pData = (void *)&sms->pnext;
                pserviceConfig->address->sms_num++;
            }
            break;
        case WINKS_TAG_number       :
        case WINKS_TAG_prefix       :
            if(sms)
                pData = (void *)((char *)sms + pTagCont->Off);
            break;
      #endif
        case WINKS_TAG_proxy        :
        case WINKS_TAG_apn          :
            if(pserviceConfig->connection)
                pData = (void *)((char *)pserviceConfig->connection + pTagCont->Off);
            break;
        case WINKS_TAG_subscription :
            if(pserviceConfig->config)
                pData = (void *)&pserviceConfig->config->subscription;
            break;
        case WINKS_TAG_proxyPort    :
        case WINKS_TAG_diy          :
        case WINKS_TAG_vcard        :
        case WINKS_TAG_activity     :
        case WINKS_TAG_vcash        :
        case WINKS_TAG_limited      :
        case WINKS_TAG_trial        :
        case WINKS_TAG_adware       :
        case WINKS_TAG_subscriber   :
        case WINKS_TAG_autoSubscribe:
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;

    if(pTagCont->TagId == WINKS_TAG_proxyPort)
    {
        if(pserviceConfig->connection)
            pserviceConfig->connection->proxyPort = (unsigned short)((Val!=-1) ? Val : 0);
    }
    else if(pTagCont->TagId == WINKS_TAG_autoSubscribe)
    {
        if(pserviceConfig->client)
            pserviceConfig->client->autoSubscribe = (Val==WINKS_BoolType_true);
    }
    else if(pserviceConfig->config)
    {
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_diy          :
            if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_limited].txt) == 0 )
                pserviceConfig->config->limited_diy = (Val==WINKS_BoolType_true);
            else if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_trial].txt) == 0 )
                pserviceConfig->config->trial_diy = (Val==WINKS_BoolType_true);
            else if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_adware].txt) == 0 )
                pserviceConfig->config->adware_diy = (Val==WINKS_BoolType_true);
            else /*if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_subscriber].txt) == 0 )*/
                pserviceConfig->config->subscriber_diy = (Val==WINKS_BoolType_true);
            break;
        case WINKS_TAG_vcard        :
            if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_limited].txt) == 0 )
                pserviceConfig->config->limited_vcard= (Val==WINKS_BoolType_true);
            else if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_trial].txt) == 0 )
                pserviceConfig->config->trial_vcard = (Val==WINKS_BoolType_true);
            else if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_adware].txt) == 0 )
                pserviceConfig->config->adware_vcard = (Val==WINKS_BoolType_true);
            else /*if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_subscriber].txt) == 0 )*/
                pserviceConfig->config->subscriber_vcard = (Val==WINKS_BoolType_true);
            break;
        case WINKS_TAG_activity     :
            if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_limited].txt) == 0 )
                pserviceConfig->config->limited_activity = (Val==WINKS_BoolType_true);
            else if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_trial].txt) == 0 )
                pserviceConfig->config->trial_activity = (Val==WINKS_BoolType_true);
            else if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_adware].txt) == 0 )
                pserviceConfig->config->adware_activity = (Val==WINKS_BoolType_true);
            else /*if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_subscriber].txt) == 0 )*/
                pserviceConfig->config->subscriber_activity = (Val==WINKS_BoolType_true);
            break;
        case WINKS_TAG_vcash        :
            pserviceConfig->config->vcash = (Val>0)?Val:0;
            break;
        case WINKS_TAG_limited      :
            pserviceConfig->config->subscription.limited = (Val>0)?1:0;
            break;
        case WINKS_TAG_trial        :
            pserviceConfig->config->subscription.trial = (Val>0)?1:0;
            break;
        case WINKS_TAG_adware       :
            pserviceConfig->config->subscription.adware = (Val>0)?1:0;
            break;
        case WINKS_TAG_subscriber   :
            pserviceConfig->config->subscription.subscriber = (Val>0)?1:0;
            break;
        default:
            break;
        }
    }

    return 0;
}

#if 1 /* "mask WINKS_PROTOCOL_GETSERVICEMODE" */
static int winks_ProtSetTagVal_ServicesType( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_serviceMode */
    ServicesType_s * pserviceMode = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_services)
        pData = (void *)&pserviceMode;
    else
    {
        pserviceMode = (ServicesType_s * )pMemPool->pdata;
        if(pserviceMode == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_subscription     :
        case WINKS_TAG_vcash            :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_diy              :
        case WINKS_TAG_vcard            :
        case WINKS_TAG_activity         :
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }

    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;
    
    switch(pTagCont->TagId)
    {
    case WINKS_TAG_diy          :
        pserviceMode->diy = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_vcard        :
        pserviceMode->vcard = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_activity     :
        pserviceMode->activity = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_vcash        :
        pserviceMode->vcash = (Val==WINKS_BoolType_true);
        break;
    default:
        break;
    }

    return 0;
}
#endif

static int winks_ProtSetTagVal_userServiceStatus( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_userServiceStatus */
    WinksProt_userServiceStatus_s * puserServiceStatus = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_userServiceStatus)
        pData = (void *)&puserServiceStatus;
    else
    {
        puserServiceStatus = (WinksProt_userServiceStatus_s * )pMemPool->pdata;
        if(puserServiceStatus == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_unregistered     :
            puserServiceStatus->IsRegistered = 0;
            return 0;
        case WINKS_TAG_uid              :
            pData = (void *)&puserServiceStatus->RegStat.reg.uid;
            puserServiceStatus->IsRegistered = 1;
            break;
        case WINKS_TAG_services         :
            pData = (void *)&puserServiceStatus->RegStat.reg.service;
            break;
        case WINKS_TAG_registerMethod   :
            pData = (void *)&puserServiceStatus->RegStat.unregistered.registerMethod;
            break;
        case WINKS_TAG_subscription     :
        case WINKS_TAG_vcash            :
            pData = (void *)((char *)&puserServiceStatus->RegStat.reg.service + pTagCont->Off);
            break;
        case WINKS_TAG_diy              :
        case WINKS_TAG_vcard            :
        case WINKS_TAG_activity         :
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }

    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;
    
    switch(pTagCont->TagId)
    {
    case WINKS_TAG_diy          :
        puserServiceStatus->RegStat.reg.service.diy = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_vcard        :
        puserServiceStatus->RegStat.reg.service.vcard = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_activity     :
        puserServiceStatus->RegStat.reg.service.activity = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_vcash        :
        puserServiceStatus->RegStat.reg.service.vcash = (Val==WINKS_BoolType_true);
        break;
    default:
        break;
    }

    return 0;
}

static int winks_ProtSetTagVal_winksShow(
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_winksShow */
    WinksProt_winksShow_s * pwinksShow = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;

    if(pTagCont->TagId == WINKS_TAG_winksShow)
        pData = (void *)&pwinksShow;
    else
    {
        WinksFileType_s * media = NULL;
        WinksFileType_s * thumbnail = NULL;
        
        pwinksShow = (WinksProt_winksShow_s *)pMemPool->pdata;
        if(pwinksShow == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_url        :
        case WINKS_TAG_w          :
        case WINKS_TAG_h          :
            if( WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_media].txt) == 0 &&
                pwinksShow->media )
            {
                media = pwinksShow->media;
                while(media && media->pnext)
                    media = media->pnext;
            }
            else if(WINKS_TAGCMP(pTree->pparent->ptag, TagName[WINKS_TAG_thumbnail].txt) == 0 &&
                pwinksShow->thumbnail )
            {
                thumbnail = pwinksShow->thumbnail;
                while(thumbnail && thumbnail->pnext)
                    thumbnail = thumbnail->pnext;
            }
            break;
        }
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_media      :
            if(pwinksShow->media == NULL)
                pData = (void *)&pwinksShow->media;
            else
                pData = (void *)&media->pnext;
            pwinksShow->media_num++;
            break;
        case WINKS_TAG_thumbnail  :
            if(pwinksShow->thumbnail == NULL)
                pData = (void *)&pwinksShow->thumbnail;
            else
                pData = (void *)&thumbnail->pnext;
            pwinksShow->thumbnail_num++;
            break;
        case WINKS_TAG_greeting   :
            pwinksShow->greetValid = 1;
        case WINKS_TAG_wid        :
        case WINKS_TAG_status     :
        case WINKS_TAG_statusText :
        case WINKS_TAG_message    :
        case WINKS_TAG_vcard      :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_url        :
        case WINKS_TAG_w          :
        case WINKS_TAG_h          :
            if( media )
                pData = (void *)((char *)media + pTagCont->Off);
            else if(thumbnail )
                pData = (void *)((char *)thumbnail + pTagCont->Off);
            break;
        case WINKS_TAG_name       :
        case WINKS_TAG_mobile     :
        case WINKS_TAG_corporation:
        case WINKS_TAG_title      :
        case WINKS_TAG_phone      :
        case WINKS_TAG_email      :
        case WINKS_TAG_fax        :
        case WINKS_TAG_address    :
            if(pwinksShow->vcard)
            {
                pData = (void *)((char *)pwinksShow->vcard + pTagCont->Off);
            }
            break;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    return winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen);

}

#if 0 /* "mask WINKS_PROTOCOL_GETPHONEBOOK" */
static int winks_ProtSetTagVal_phoneBook( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_phoneBook */
    WinksProt_phoneBook_s * pphoneBook = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;

    if(pTagCont->TagId == WINKS_TAG_phoneBook)
        pData = (void *)&pphoneBook;
    else
    {
        pphoneBook = (WinksProt_phoneBook_s *)pMemPool->pdata;
        if(pphoneBook == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_pbi      :
        case WINKS_TAG_n        :
        case WINKS_TAG_p        :
        {
            struct Prot_pbi_s * pbi = pphoneBook->pbi;

            if(pTagCont->TagId == WINKS_TAG_pbi)
            {
                if(pphoneBook->pbi == NULL)
                    pData = (void *)&pphoneBook->pbi;
                else
                {
                    while(pbi && pbi->pnext)
                        pbi = pbi->pnext;
                    pData = (void *)&pbi->pnext;
                }
                pphoneBook->pbi_num++;
            }
            else if(pbi)
            {
                while(pbi && pbi->pnext)
                    pbi = pbi->pnext;

                 if(pTagCont->TagId == WINKS_TAG_n)
                    pData = (void *)&pbi->n;
                 else if(pTagCont->TagId == WINKS_TAG_p)
                    pData = (void *)&pbi->p;
            }
        }
            break;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    return winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen);
}
#endif

static int winks_ProtSetTagVal_UserWinksType( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_UserWinksType */
    WinksProt_UserWinksType_s * pUserWinksType = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_userWinks)
        pData = (void *)&pUserWinksType;
    else
    {
        pUserWinksType = (WinksProt_UserWinksType_s * )pMemPool->pdata;
        if(pUserWinksType == NULL)
            return -1;
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_wid          :
        case WINKS_TAG_name         :
        case WINKS_TAG_duration     :
        case WINKS_TAG_price        :
        case WINKS_TAG_state        :
        case WINKS_TAG_expire       :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_diy          :
        case WINKS_TAG_censor       :
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;
    
    if(pUserWinksType)
    {
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_diy          :
            pUserWinksType->diy = Val;
            break;
        case WINKS_TAG_censor       :
            pUserWinksType->censor = Val;
            break;
        default:
            break;
        }
    }

    return 0;
}

#if 0 /* "mask WINKS_PROTOCOL_GETWINKSSCRIPT" */
static int winks_ProtSetTagVal_winksScript( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_winksScript */
    WinksProt_winksScript_s * pwinksScript = NULL;
    void  * pData = NULL;
    int   * pLen = NULL;

    if(pTagCont->TagId == WINKS_TAG_winksScript)
        pData = (void *)&pwinksScript;
    else
    {
        pwinksScript = (WinksProt_winksScript_s * )pMemPool->pdata;
        if(pwinksScript == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_wid        :
        case WINKS_TAG_greeting   :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_p          :
        {
            struct Prot_pno_s * pno = 
                (struct Prot_pno_s *)Winks_EngMemPoolAlloc(pMemPool, sizeof(struct Prot_pno_s), 4);
            if(pno == NULL)
                return -1;

            if(pwinksScript->pno == NULL)
                pwinksScript->pno = pno;
            else
            {
                struct Prot_pno_s * pnox = pwinksScript->pno;
                while(pnox && pnox->pnext)
                    pnox = pnox->pnext;
                pnox->pnext = pno;
            }
            pData = (void *)&pno->p;
            pwinksScript->IsForAll = 0;
        }
            break;
        case WINKS_TAG_all        :
            pwinksScript->IsForAll = 1;
            return 0;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    return winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen);
}
#endif

#if 0 /* "mask WINKS_PROTOCOL_SETSHARED" */
static int winks_ProtSetTagVal_shared( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_shared */
    WinksProt_shared_s * pshared;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_shared)
        pData = (void *)&pshared;
    else
    {
        pshared = (WinksProt_shared_s * )pMemPool->pdata;
        if(pshared == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_status         :
        case WINKS_TAG_statusText     :
        case WINKS_TAG_activity       :
        case WINKS_TAG_name           :
        case WINKS_TAG_message        :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_showWinks      :
        case WINKS_TAG_showVcard      :
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;
    
    switch(pTagCont->TagId)
    {
    case WINKS_TAG_showWinks      :
        pshared->showWinks_Mask = 1;
        pshared->showWinks = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_showVcard      :
        pshared->showVcard_Mask = 1;
        pshared->showVcard = (Val==WINKS_BoolType_true);
        break;
    }
    
    return 0;
}
#endif
static int winks_ProtSetTagVal_softwarePackage( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_softwarePackage */
    WinksProt_softwarePackage_s * psoftwarePackage;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_softwarePackage)
        pData = (void *)&psoftwarePackage;
    else
    {
        psoftwarePackage = (WinksProt_softwarePackage_s * )pMemPool->pdata;
        if(psoftwarePackage == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_name           :
        case WINKS_TAG_version        :
        case WINKS_TAG_file           :
        case WINKS_TAG_description    :
        case WINKS_TAG_optional       :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        default:
            break;
        }
    }
    
    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }
    
    return winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen);
}

#if 0 /* "mask WINKS_PROTOCOL_GETMESSAGE" */
static int winks_ProtSetTagVal_messageList( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_messageList */
    WinksProt_messageList_s * pmessageList;
    void  * pData = NULL;
    int   * pLen = NULL;
    struct Prot_mi_s * mi = NULL;
    WinksProt_serviceConfig_s * pserviceConfig = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_messageList)
        pData = (void *)&pmessageList;
    else
    {
        pmessageList = (WinksProt_messageList_s * )pMemPool->pdata;
        if(pmessageList == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_mid          :
        case WINKS_TAG_serviceConfig:
        case WINKS_TAG_userConfig   :
            mi = pmessageList->mi;
            while(mi && mi->pnext)
                mi = mi->pnext;
            break;
        case WINKS_TAG_config       :
        case WINKS_TAG_winks        :
        case WINKS_TAG_media        :
        case WINKS_TAG_wap          :
        case WINKS_TAG_sms          :
        case WINKS_TAG_push         :
        case WINKS_TAG_subscription :
            mi = pmessageList->mi;
            while(mi && mi->pnext)
                mi = mi->pnext;
            pserviceConfig = mi->serviceConfig;
            break;
        }
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_mi           :
            pData = (void *)&pmessageList->mi;
            break;
        case WINKS_TAG_mid          :
            if(mi)
                pData = (void *)&mi->mid;
            break;
        case WINKS_TAG_serviceConfig:
            if(mi)
                pData = (void *)&mi->serviceConfig;
            break;
        case WINKS_TAG_userConfig   :
            if(mi)
                pData = (void *)&mi->clientConfig;
            break;
        case WINKS_TAG_address      :
            if(pserviceConfig)
                pData = (void *)&pserviceConfig->address;
            break;
        case WINKS_TAG_config       :
            if(pserviceConfig)
                pData = (void *)&pserviceConfig->config;
            break;
        case WINKS_TAG_winks        :
            if(pserviceConfig && pserviceConfig->address)
                pData = (void *)&pserviceConfig->address->winks;
            break;
        case WINKS_TAG_media        :
            if(pserviceConfig && pserviceConfig->address)
                pData = (void *)&pserviceConfig->address->media;
            break;
        case WINKS_TAG_wap          :
            if(pserviceConfig && pserviceConfig->address)
                pData = (void *)&pserviceConfig->address->store;
            break;
        case WINKS_TAG_sms          :
            if(pserviceConfig && pserviceConfig->address)
                pData = (void *)&pserviceConfig->address->sms;
            break;
        case WINKS_TAG_push         :
            if(pserviceConfig && pserviceConfig->address)
                pData = (void *)&pserviceConfig->address->push;
            break;
        case WINKS_TAG_subscription :
            if(pserviceConfig && pserviceConfig->config)
                pData = (void *)&pserviceConfig->config->subscription;
            break;
        case WINKS_TAG_diy          :
        case WINKS_TAG_vcard        :
        case WINKS_TAG_activity     :
        case WINKS_TAG_vcash        :
        case WINKS_TAG_limited      :
        case WINKS_TAG_trial        :
        case WINKS_TAG_adware       :
        case WINKS_TAG_subscriber   :
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }

    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }

    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;
    
    switch(pTagCont->TagId)
    {
    case WINKS_TAG_diy          :
        if(pserviceConfig && pserviceConfig->config)
            pserviceConfig->config->diy = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_vcard        :
        if(pserviceConfig && pserviceConfig->config)
            pserviceConfig->config->vcard = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_activity     :
        if(pserviceConfig && pserviceConfig->config)
            pserviceConfig->config->activity = (Val==WINKS_BoolType_true);
        break;
    case WINKS_TAG_vcash        :
        if(pserviceConfig && pserviceConfig->config)
            pserviceConfig->config->vcash = (Val==WINKS_BoolType_true);
        break;
        
    case WINKS_TAG_limited      :
        if(pserviceConfig->config)
            pserviceConfig->config->subscription.limited = Val?1:0;
        break;
    case WINKS_TAG_trial        :
        if(pserviceConfig->config)
            pserviceConfig->config->subscription.trial = Val?1:0;
        break;
    case WINKS_TAG_adware       :
        if(pserviceConfig->config)
            pserviceConfig->config->subscription.adware = Val?1:0;
        break;
    case WINKS_TAG_subscriber   :
        if(pserviceConfig->config)
            pserviceConfig->config->subscription.subscriber = Val?1:0;
        break;
    default:
        break;
    }
    
    return 0;
}
#endif

static int winks_ProtSetTagVal_userConfig( 
                                      Winks_MemPool_s * pMemPool,
                                      Winks_XmlElement_s * pTree,
                                      WinksTagContent_s  * pTagCont )
{
/* 参考 TagCont_userConfig */
    WinksProt_userConfig_s * puserConfig;
    void  * pData = NULL;
    int   * pLen = NULL;
    int     Val = -1;

    if(pTagCont->TagId == WINKS_TAG_userConfig)
        pData = (void *)&puserConfig;
    else
    {
        puserConfig = (WinksProt_userConfig_s * )pMemPool->pdata;
        if(puserConfig == NULL)
            return -1;
        
        switch(pTagCont->TagId)
        {
        case WINKS_TAG_proxy              :
        case WINKS_TAG_apn                :
        case WINKS_TAG_durationAfterHangup:
        case WINKS_TAG_networkTimeout     :
        case WINKS_TAG_splashDuration     :
        case WINKS_TAG_updateOptionMinFee :
        case WINKS_TAG_updateOptionMostRecent:
        case WINKS_TAG_updateConfig       :
            pData = (void *)(pMemPool->pdata + pTagCont->Off);
            break;
        case WINKS_TAG_option             :
        case WINKS_TAG_period             :
            if(puserConfig->updateConfig)
                pData = (void *)((char *)puserConfig->updateConfig + pTagCont->Off);
            break;
        case WINKS_TAG_proxyPort          :
            pData = (void *)&Val;
            break;
        case WINKS_TAG_showWinks          :
        case WINKS_TAG_showMode           :
        case WINKS_TAG_roaming            :
        case WINKS_TAG_autorun            :
        case WINKS_TAG_updateAfterHangup  :
        case WINKS_TAG_upgradeMode        :
        case WINKS_TAG_vcardOverwrite     :
        case WINKS_TAG_connectInCall      :
        case WINKS_TAG_support3G          :
            pData = (void *)&Val;
            break;
        default:
            break;
        }
    }

    if(pData == NULL)
    {
        Winks_assert(pData);
        return -1;
    }

    if(winks_ProtSetTagVal(pMemPool, pTree, pTagCont, pData, pLen) != 0)
        return -1;
    
    switch(pTagCont->TagId)
    {
    case WINKS_TAG_proxyPort          :
        puserConfig->proxyPort = (short)Val;
        break;
    case WINKS_TAG_showWinks          :
        puserConfig->showWinks = (Val==WINKS_BoolType_true);
        puserConfig->showWinks_Mask = 1;
        break;
    case WINKS_TAG_showMode           :
        puserConfig->showMode = Val;
        puserConfig->showMode_Mask = 7;
        break;
    case WINKS_TAG_roaming            :
        puserConfig->roaming = (Val==WINKS_BoolType_true);
        puserConfig->roaming_Mask = 1;
        break;
    case WINKS_TAG_autorun            :
        puserConfig->autorun = (Val==WINKS_BoolType_true);
        puserConfig->autorun_Mask = 1;
        break;
    case WINKS_TAG_updateAfterHangup  :
        puserConfig->updateAfterHangup = (Val==WINKS_BoolType_true);
        puserConfig->updateAfterHangup_Mask = 1;
        break;
    case WINKS_TAG_upgradeMode        :
        puserConfig->upgradeMode = (Val==WINKS_upgradeMode_manual);
        puserConfig->upgradeMode_Mask = 1;
        break;
    case WINKS_TAG_vcardOverwrite     :
        puserConfig->vcardOverwrite = (Val==WINKS_upgradeMode_manual);
        puserConfig->vcardOverwrite_Mask = 1;
        break;
    case WINKS_TAG_connectInCall      :
        puserConfig->connectInCall = (Val==WINKS_BoolType_true);
        puserConfig->connectInCall_Mask = 1;
        break;
    case WINKS_TAG_support3G          :
        puserConfig->support3G = (Val==WINKS_BoolType_true);
        puserConfig->support3G_Mask = 1;
        break;
    default:
        break;
    }
    
    return 0;
}

static int winks_ProtGetRetType(char * ptag)
{
    int i;
    
    int TagArr[] = {
        WINKS_TAG_NONE,
        WINKS_TAG_result,
        WINKS_TAG_serviceConfig,
        WINKS_TAG_userServiceStatus,
        WINKS_TAG_services,
        WINKS_TAG_userConfig,
        WINKS_TAG_winksShow,
        WINKS_TAG_phoneBook,
        WINKS_TAG_userWinks,
        WINKS_TAG_winksScript,
        WINKS_TAG_shared,
        WINKS_TAG_softwarePackage,
        WINKS_TAG_messageList,
    };

    if(WINKS_TAGCMP("userServiceStatus", ptag) == 0)
        return WINKS_PROT_RET_TYPE_userServiceStatus;
    
    for(i=1; i<WK_ARRSIZE(TagArr); i++)
    {
        if(WINKS_TAGCMP(ptag, TagName[TagArr[i]].txt) == 0)
            break;
    }

    return i;
}

static WinksProtRetType_e winks_ProtGetRetTypeX(char * ptag)
{
    int i, max;
    
    int TagArr_str[] = {
        WINKS_TAG_wid,
    };

    max = WK_ARRSIZE(TagArr_str);
    for(i=0; i<max; i++)
    {
        if(WINKS_TAGCMP(ptag, TagName[TagArr_str[i]].txt) == 0)
            break;
    }

    if(i<max)
        return WINKS_PROT_RET_TYPE_str;
    
    return WINKS_PROT_RET_TYPE_MAX;
}

static int winks_ProtFindSubTag(char *ptag, int taglen, 
                                        const WinksTagContent_s * pArr, int ArrSize)
{
    int     i;
    
    for(i=0; i<ArrSize; i++)
    {
        char *pTagName;
        
        Winks_assert( pArr[i].TagId>WINKS_TAG_NONE &&
                      pArr[i].TagId<WINKS_TAG_MAX );
        pTagName = (char *)TagName[pArr[i].TagId].txt;

        if(pTagName && ptag && taglen)
        {
            if(pTagName[0]=='#')
            {
                char * pSpecialTag = NULL;
                switch(pTagName[1])
                {
                case '0':
                    pSpecialTag = "userServiceStatus";
                    break;
                case '1':
                    pSpecialTag = "durationAfterHangup";
                    break;
                case '2':
                    pSpecialTag = "updateAfterHangup";
                    break;
                case '3':
                    pSpecialTag = "updateOptionMinFee";
                    break;
                case '4':
                    pSpecialTag = "updateOptionMostRecent";
                    break;
                default:
                    pSpecialTag = "????";
                    break;
                }
                if(WINKS_TAGCMP(pSpecialTag, ptag) == 0)
                    break;
            }
            else if( WINKS_TAGCMP(pTagName, ptag) == 0 )
                break;
        }
    }

    return i;
}

static const WinksTagContent_s TagCont_result[] = {
  /* TagId               TagType             Off, Size */
    {WINKS_TAG_result  , WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_result_s)}, 
    {WINKS_TAG_code    , WINKS_TAGTYPE_atoi, (int)&((WinksProt_result_s *)0)->code, 0},
    {WINKS_TAG_text    , WINKS_TAGTYPE_utf8, (int)&((WinksProt_result_s *)0)->rst_txt.text, 0},
    {WINKS_TAG_greeting, WINKS_TAGTYPE_utf8, (int)&(((WinksProt_result_s *)0)->rst_txt.greeting), 0},
    //{WINKS_TAG_debug   , WINKS_TAGTYPE_attr, sizeof(struct Prot_debug_attr_s)},
    {WINKS_TAG_debug   , WINKS_TAGTYPE_utf8, (int)&((WinksProt_result_s *)0)->debug, 0},
};

static const WinksTagContent_s TagCont_serviceConfig[] = {
    {WINKS_TAG_serviceConfig, WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_serviceConfig_s)}, 
    {WINKS_TAG_address      , WINKS_TAGTYPE_tree, (int)&((WinksProt_serviceConfig_s *)0)->address, sizeof(struct Prot_address_s)},
    {WINKS_TAG_connection   , WINKS_TAGTYPE_tree, (int)&((WinksProt_serviceConfig_s *)0)->connection, sizeof(struct Prot_connection_s)},
    {WINKS_TAG_config       , WINKS_TAGTYPE_tree, (int)&((WinksProt_serviceConfig_s *)0)->config, sizeof(struct Prot_config_s)},
    {WINKS_TAG_client       , WINKS_TAGTYPE_tree, (int)&((WinksProt_serviceConfig_s *)0)->client, sizeof(struct Prot_client_s)},
    {WINKS_TAG_winks        , WINKS_TAGTYPE_str , (int)&((struct Prot_address_s *)0)->winks, 0},
    //{WINKS_TAG_media        , WINKS_TAGTYPE_str , 0},
    {WINKS_TAG_wap          , WINKS_TAGTYPE_str , (int)&((struct Prot_address_s *)0)->store, 0},
  #if 0
    {WINKS_TAG_sms          , WINKS_TAGTYPE_str , (int)&((struct Prot_address_s *)0)->sms, 0},
  #else
    {WINKS_TAG_sms          , WINKS_TAGTYPE_tree, (int)&((struct Prot_address_s *)0)->sms, sizeof(struct Prot_sms_s)},
    {WINKS_TAG_number       , WINKS_TAGTYPE_str , (int)&((struct Prot_sms_s *)0)->number, 0},
    {WINKS_TAG_prefix       , WINKS_TAGTYPE_str , (int)&((struct Prot_sms_s *)0)->prefix, 0},
  #endif
    {WINKS_TAG_push         , WINKS_TAGTYPE_atoi, (int)&((struct Prot_address_s *)0)->push, 0},
    {WINKS_TAG_proxy        , WINKS_TAGTYPE_str , (int)&((struct Prot_connection_s *)0)->proxy, 0},
    {WINKS_TAG_proxyPort    , WINKS_TAGTYPE_atoi, (int)&((struct Prot_connection_s *)0)->proxyPort, 0},
    {WINKS_TAG_apn          , WINKS_TAGTYPE_str , (int)&((struct Prot_connection_s *)0)->apn, 0}, 
    {WINKS_TAG_subscription , WINKS_TAGTYPE_null, (int)&((struct Prot_config_s *)0)->subscription, sizeof(struct Prot_subscription_s)},
    {WINKS_TAG_diy          , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_vcard        , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_activity     , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_vcash        , WINKS_TAGTYPE_null, 0, 0},
    //{WINKS_TAG_limited      , WINKS_TAGTYPE_attr, 0, 0},
    //{WINKS_TAG_trial        , WINKS_TAGTYPE_attr, 0, 0},
    //{WINKS_TAG_adware       , WINKS_TAGTYPE_attr, 0, 0},
    //{WINKS_TAG_subscriber   , WINKS_TAGTYPE_attr, 0, 0},
    {WINKS_TAG_limited      , WINKS_TAGTYPE_null, 0, 0},
    {WINKS_TAG_trial        , WINKS_TAGTYPE_null, 0, 0},
    {WINKS_TAG_adware       , WINKS_TAGTYPE_null, 0, 0},
    {WINKS_TAG_subscriber   , WINKS_TAGTYPE_null, 0, 0},
    {WINKS_TAG_autoSubscribe, WINKS_TAGTYPE_pt2i, (int)&((struct Prot_client_s *)0)->autoSubscribe, 0},/* BoolTypeVal */
};

static const WinksTagContent_s TagCont_userServiceStatus[] = {
    {WINKS_TAG_userServiceStatus, WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_userServiceStatus_s)},
    {WINKS_TAG_unregistered     , WINKS_TAGTYPE_tree, (int)&(((WinksProt_userServiceStatus_s *)0)->RegStat.unregistered), sizeof(struct Prot_unregistered_s)},
    {WINKS_TAG_uid              , WINKS_TAGTYPE_str , (int)&(((WinksProt_userServiceStatus_s *)0)->RegStat.reg.uid), 0},
    {WINKS_TAG_services         , WINKS_TAGTYPE_tree, (int)&(((WinksProt_userServiceStatus_s *)0)->RegStat.reg.service), sizeof(ServicesType_s)},
    {WINKS_TAG_registerMethod   , WINKS_TAGTYPE_pt2i, 0, 0},/* registerMethodVal */
    {WINKS_TAG_subscription     , WINKS_TAGTYPE_pt2i, (int)&(((ServicesType_s *)0)->subscription), 0},/* SubscriptionModeTypeVal */
    {WINKS_TAG_diy              , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_vcard            , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_activity         , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_vcash            , WINKS_TAGTYPE_atoi, (int)&((ServicesType_s *)0)->vcash, 0}
};

static const WinksTagContent_s TagCont_serviceMode[] = {
    {WINKS_TAG_services         , WINKS_TAGTYPE_tree, 0, sizeof(ServicesType_s)},
    {WINKS_TAG_registerMethod   , WINKS_TAGTYPE_pt2i, 0, 0},/* registerMethodVal */
    {WINKS_TAG_subscription     , WINKS_TAGTYPE_pt2i, (int)&(((ServicesType_s *)0)->subscription), 0},/* SubscriptionModeTypeVal */
    {WINKS_TAG_diy              , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_vcard            , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_activity         , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_vcash            , WINKS_TAGTYPE_atoi, (int)&((ServicesType_s *)0)->vcash, 0},
};

static const WinksTagContent_s TagCont_userConfig[] = {
    {WINKS_TAG_userConfig   , WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_userConfig_s)},
    {WINKS_TAG_proxy        , WINKS_TAGTYPE_str , (int)&((WinksProt_userConfig_s *)0)->proxy, 0},
    {WINKS_TAG_proxyPort    , WINKS_TAGTYPE_atoi, (int)&((WinksProt_userConfig_s *)0)->proxyPort, 0},
    {WINKS_TAG_apn          , WINKS_TAGTYPE_str , (int)&((WinksProt_userConfig_s *)0)->apn, 0}, 
    {WINKS_TAG_durationAfterHangup,WINKS_TAGTYPE_atoi, (int)&((WinksProt_userConfig_s *)0)->durationAfterHangup, 0},
    {WINKS_TAG_showWinks         , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_showMode          , WINKS_TAGTYPE_atoi, 0, 0},
    {WINKS_TAG_roaming           , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_autorun           , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_updateAfterHangup , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_upgradeMode       , WINKS_TAGTYPE_pt2i, 0, 0},/* upgradeModeVal */
    {WINKS_TAG_vcardOverwrite    , WINKS_TAGTYPE_pt2i, 0, 0},/* upgradeModeVal */
    {WINKS_TAG_connectInCall     , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_support3G         , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_networkTimeout    , WINKS_TAGTYPE_atoi, (int)&((WinksProt_userConfig_s *)0)->networkTimeout, 0},
    {WINKS_TAG_splashDuration    , WINKS_TAGTYPE_atoi, (int)&((WinksProt_userConfig_s *)0)->splashDuration, 0},
    {WINKS_TAG_updateOptionMinFee, WINKS_TAGTYPE_atoi, (int)&((WinksProt_userConfig_s *)0)->updateOptionMinFee, 0},
    {WINKS_TAG_updateOptionMostRecent, WINKS_TAGTYPE_atoi, (int)&((WinksProt_userConfig_s *)0)->updateOptionMostRecent, 0},
    {WINKS_TAG_updateConfig      , WINKS_TAGTYPE_tree, (int)&((WinksProt_userConfig_s *)0)->updateConfig, sizeof(struct Prot_updateConfig_s)},
    {WINKS_TAG_option            , WINKS_TAGTYPE_pt2i, (int)&((struct Prot_updateConfig_s *)0)->option, 0},/* updateConfigVal */
    {WINKS_TAG_period            , WINKS_TAGTYPE_atoi, (int)&((struct Prot_updateConfig_s *)0)->period, 0},
};

static const WinksTagContent_s TagCont_winksShow[] = {
    {WINKS_TAG_winksShow  , WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_winksShow_s)},
    {WINKS_TAG_wid        , WINKS_TAGTYPE_str , (int)&((WinksProt_winksShow_s *)0)->wid, 0},
    {WINKS_TAG_media      , WINKS_TAGTYPE_tree, (int)&((WinksProt_winksShow_s *)0)->media, sizeof(WinksFileType_s)},
    {WINKS_TAG_thumbnail  , WINKS_TAGTYPE_tree, (int)&((WinksProt_winksShow_s *)0)->thumbnail, sizeof(WinksFileType_s)},
    {WINKS_TAG_greeting   , WINKS_TAGTYPE_utf8, (int)&((WinksProt_winksShow_s *)0)->greeting, 0},
    {WINKS_TAG_status     , WINKS_TAGTYPE_pt2i, (int)&((WinksProt_winksShow_s *)0)->status, 0},/* statusVal */
    {WINKS_TAG_statusText , WINKS_TAGTYPE_utf8, (int)&((WinksProt_winksShow_s *)0)->statusText, 0},
    {WINKS_TAG_message    , WINKS_TAGTYPE_utf8, (int)&((WinksProt_winksShow_s *)0)->message, 0},
    {WINKS_TAG_vcard      , WINKS_TAGTYPE_tree, (int)&((WinksProt_winksShow_s *)0)->vcard, sizeof(VcardType_s)},
    {WINKS_TAG_url        , WINKS_TAGTYPE_str , (int)&((WinksFileType_s *)0)->url, 0},
    {WINKS_TAG_w          , WINKS_TAGTYPE_atoi, (int)&((WinksFileType_s *)0)->w, 0},
    {WINKS_TAG_h          , WINKS_TAGTYPE_atoi, (int)&((WinksFileType_s *)0)->h, 0},
    {WINKS_TAG_name       , WINKS_TAGTYPE_utf8, (int)&((VcardType_s *)0)->name, 0},
    {WINKS_TAG_mobile     , WINKS_TAGTYPE_str , (int)&((VcardType_s *)0)->mobile, 0},
    {WINKS_TAG_corporation, WINKS_TAGTYPE_utf8, (int)&((VcardType_s *)0)->corporation, 0},
    {WINKS_TAG_title      , WINKS_TAGTYPE_utf8, (int)&((VcardType_s *)0)->title, 0},
    {WINKS_TAG_phone      , WINKS_TAGTYPE_str , (int)&((VcardType_s *)0)->phone, 0},
    {WINKS_TAG_email      , WINKS_TAGTYPE_str , (int)&((VcardType_s *)0)->email, 0},
    {WINKS_TAG_fax        , WINKS_TAGTYPE_str , (int)&((VcardType_s *)0)->fax, 0},
    {WINKS_TAG_address    , WINKS_TAGTYPE_utf8, (int)&((VcardType_s *)0)->address, 0},
};

static const WinksTagContent_s TagCont_phoneBook[] = {
    {WINKS_TAG_phoneBook, WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_phoneBook_s)},
    {WINKS_TAG_pbi      , WINKS_TAGTYPE_tree, (int)&((WinksProt_phoneBook_s *)0)->pbi, sizeof(struct Prot_pbi_s)},
    {WINKS_TAG_n        , WINKS_TAGTYPE_utf8, (int)&((struct Prot_pbi_s *)0)->n, 0},
    {WINKS_TAG_p        , WINKS_TAGTYPE_str , (int)&((struct Prot_pbi_s *)0)->p, 0},
};

static const WinksTagContent_s TagCont_UserWinksType[] = {
    {WINKS_TAG_userWinks    , WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_UserWinksType_s)},
    {WINKS_TAG_wid          , WINKS_TAGTYPE_str , (int)&((WinksProt_UserWinksType_s *)0)->wid, 0},
    {WINKS_TAG_name         , WINKS_TAGTYPE_utf8, (int)&((WinksProt_UserWinksType_s *)0)->name, 0},
    {WINKS_TAG_diy          , WINKS_TAGTYPE_pt2i, 0, 0},
    {WINKS_TAG_censor       , WINKS_TAGTYPE_pt2i, 0, 0},/* censorVal */
    {WINKS_TAG_duration     , WINKS_TAGTYPE_atoi, (int)&((WinksProt_UserWinksType_s *)0)->duration, 0},
    {WINKS_TAG_price        , WINKS_TAGTYPE_atoi, (int)&((WinksProt_UserWinksType_s *)0)->price, 0},
    {WINKS_TAG_state        , WINKS_TAGTYPE_pt2i, (int)&((WinksProt_UserWinksType_s *)0)->state, 0},/* stateVal */
    {WINKS_TAG_expire       , WINKS_TAGTYPE_str , (int)&((WinksProt_UserWinksType_s *)0)->expire, 0},
};

static const WinksTagContent_s TagCont_winksScript[] = {
    {WINKS_TAG_winksScript, WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_winksScript_s)},
    {WINKS_TAG_wid        , WINKS_TAGTYPE_str , (int)&((WinksProt_winksScript_s *)0)->wid, 0},
    {WINKS_TAG_p          , WINKS_TAGTYPE_str , 0, 0},
    {WINKS_TAG_all        , WINKS_TAGTYPE_null, 0, 0},
    {WINKS_TAG_greeting   , WINKS_TAGTYPE_utf8, (int)&((WinksProt_winksScript_s *)0)->greeting, 0},
};

const WinksTagContent_s TagCont_shared[] = {
    {WINKS_TAG_shared       , WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_shared_s)},
    {WINKS_TAG_status       , WINKS_TAGTYPE_pt2i, (int)&((WinksProt_shared_s *)0)->status, 0},/* statusVal */
    {WINKS_TAG_statusText   , WINKS_TAGTYPE_str , (int)&((WinksProt_shared_s *)0)->statusText, 0},
    {WINKS_TAG_activity     , WINKS_TAGTYPE_str , (int)&((WinksProt_shared_s *)0)->activity, 0},
    {WINKS_TAG_name         , WINKS_TAGTYPE_str , (int)&((WinksProt_shared_s *)0)->name, 0},
    {WINKS_TAG_message      , WINKS_TAGTYPE_str , (int)&((WinksProt_shared_s *)0)->message, 0},
    {WINKS_TAG_showWinks    , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
    {WINKS_TAG_showVcard    , WINKS_TAGTYPE_pt2i, 0, 0},/* BoolTypeVal */
};

static const WinksTagContent_s TagCont_softwarePackage[] = {
    {WINKS_TAG_softwarePackage, WINKS_TAGTYPE_tree, 0, sizeof(WinksProt_softwarePackage_s)},
    {WINKS_TAG_name           , WINKS_TAGTYPE_utf8, (int)&((WinksProt_softwarePackage_s *)0)->name, 0},
    {WINKS_TAG_version        , WINKS_TAGTYPE_str , (int)&((WinksProt_softwarePackage_s *)0)->version, 0},
    {WINKS_TAG_file           , WINKS_TAGTYPE_str , (int)&((WinksProt_softwarePackage_s *)0)->file, 0},
    {WINKS_TAG_optional       , WINKS_TAGTYPE_pt2i, (int)&((WinksProt_softwarePackage_s *)0)->optional, 0},/* BoolTypeVal */
    {WINKS_TAG_description    , WINKS_TAGTYPE_str , (int)&((WinksProt_softwarePackage_s *)0)->description, 0},
};

#if 0
static const WinksTagContent_s TagCont_messageList[] = {
    {WINKS_TAG_messageList  , WINKS_TAGTYPE_tree, sizeof(WinksProt_messageList_s)},
    {WINKS_TAG_mi           , WINKS_TAGTYPE_tree, sizeof(struct Prot_mi_s)},
    {WINKS_TAG_mid          , WINKS_TAGTYPE_str , 0},
    {WINKS_TAG_serviceConfig, WINKS_TAGTYPE_tree, sizeof(WinksProt_serviceConfig_s)}, 
    {WINKS_TAG_userConfig   , WINKS_TAGTYPE_tree, sizeof(WinksProt_clientConfig_s)},
    {WINKS_TAG_address      , WINKS_TAGTYPE_tree, sizeof(struct Prot_address_s)},
    {WINKS_TAG_config       , WINKS_TAGTYPE_tree, sizeof(struct Prot_config_s)},
    {WINKS_TAG_winks        , WINKS_TAGTYPE_str , 0},
    {WINKS_TAG_media        , WINKS_TAGTYPE_str , 0},
    {WINKS_TAG_wap          , WINKS_TAGTYPE_str , 0},
    {WINKS_TAG_sms          , WINKS_TAGTYPE_str , 0},
    {WINKS_TAG_push         , WINKS_TAGTYPE_atoi, 0},
    {WINKS_TAG_subscription , WINKS_TAGTYPE_tree, sizeof(struct Prot_subscription_s)},
    {WINKS_TAG_diy          , WINKS_TAGTYPE_pt2i, 0},/* BoolTypeVal */
    {WINKS_TAG_vcard        , WINKS_TAGTYPE_pt2i, 0},/* BoolTypeVal */
    {WINKS_TAG_activity     , WINKS_TAGTYPE_pt2i, 0},/* BoolTypeVal */
    {WINKS_TAG_vcash        , WINKS_TAGTYPE_pt2i, 0},/* BoolTypeVal */
    //{WINKS_TAG_limited      , WINKS_TAGTYPE_attr, 0},
    //{WINKS_TAG_trial        , WINKS_TAGTYPE_attr, 0},
    //{WINKS_TAG_adware       , WINKS_TAGTYPE_attr, 0},
    //{WINKS_TAG_subscriber   , WINKS_TAGTYPE_attr, 0},
    {WINKS_TAG_limited      , WINKS_TAGTYPE_null, 0},
    {WINKS_TAG_trial        , WINKS_TAGTYPE_null, 0},
    {WINKS_TAG_adware       , WINKS_TAGTYPE_null, 0},
    {WINKS_TAG_subscriber   , WINKS_TAGTYPE_null, 0},
};
#endif

static int winks_ProtGetTagInfo(int RetType, char *ptag, int taglen,
                                      WinksTagContent_s *retTagCont )
{
    int     i, ArrSize = 0;
    const WinksTagContent_s * TagContArr = NULL;

    switch(RetType)
    {
    case WINKS_PROT_RET_TYPE_result:
        ArrSize = WK_ARRSIZE(TagCont_result);
        TagContArr = TagCont_result;
        break;
    case WINKS_PROT_RET_TYPE_serviceConfig:
        ArrSize = WK_ARRSIZE(TagCont_serviceConfig);
        TagContArr = TagCont_serviceConfig;
        break;
    case WINKS_PROT_RET_TYPE_userServiceStatus:
        ArrSize = WK_ARRSIZE(TagCont_userServiceStatus);
        TagContArr = TagCont_userServiceStatus;
        break;
    case WINKS_PROT_RET_TYPE_serviceMode:
    #if 1 /* "mask WINKS_PROTOCOL_GETSERVICEMODE" */
        ArrSize = WK_ARRSIZE(TagCont_serviceMode);
        TagContArr = TagCont_serviceMode;
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETWINKSSCRIPT");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_userConfig:
        ArrSize = WK_ARRSIZE(TagCont_userConfig);
        TagContArr = TagCont_userConfig;
        break;
    case WINKS_PROT_RET_TYPE_winksShow:
        ArrSize = WK_ARRSIZE(TagCont_winksShow);
        TagContArr = TagCont_winksShow;
        break;
    case WINKS_PROT_RET_TYPE_phoneBook:
    #if 0 /* "mask WINKS_PROTOCOL_GETPHONEBOOK" */
        ArrSize = WK_ARRSIZE(TagCont_phoneBook);
        TagContArr = TagCont_phoneBook;
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETPHONEBOOK");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_UserWinksType:
        ArrSize = WK_ARRSIZE(TagCont_UserWinksType);
        TagContArr = TagCont_UserWinksType;
        break;
    case WINKS_PROT_RET_TYPE_winksScript:
    #if 0 /* "mask WINKS_PROTOCOL_GETWINKSSCRIPT" */
        ArrSize = WK_ARRSIZE(TagCont_winksScript);
        TagContArr = TagCont_winksScript;
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETWINKSSCRIPT");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_shared:
    #if 0 /* "mask WINKS_PROTOCOL_SETSHARED" */
        ArrSize = WK_ARRSIZE(TagCont_shared);
        TagContArr = TagCont_shared;
    #else
        Winks_fail("mask WINKS_PROTOCOL_SETSHARED");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_softwarePackage:
        ArrSize = WK_ARRSIZE(TagCont_softwarePackage);
        TagContArr = TagCont_softwarePackage;
        break;
    case WINKS_PROT_RET_TYPE_messageList:
    #if 0 /* "mask WINKS_PROTOCOL_GETMESSAGE" */
        ArrSize = WK_ARRSIZE(TagCont_messageList);
        TagContArr = TagCont_messageList;
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETMESSAGE");
    #endif
        break;
	default:
		Winks_fail("winks_ProtGetTagInfo");
		return -1;
    }
    
    i = winks_ProtFindSubTag(ptag, taglen, TagContArr, ArrSize);
    if(i >= ArrSize)
    {
        Winks_printf("winks_ProtFindSubTag(\"%s\") not found!\r\n", ptag);
        return 1;
    }
    if(retTagCont)
        Winks_mem_cpy(retTagCont, &TagContArr[i], sizeof(WinksTagContent_s));
    
    return 0;
}

static int winks_ProtDealTag( Winks_MemPool_s * pMemPool,
                                    Winks_XmlElement_s * pTree,
                                    int RetType )
{
    int iRet = -1;
    
    WinksTagContent_s   TagCont;

    Winks_assert(pMemPool);
    Winks_assert(pTree);

    iRet = winks_ProtGetTagInfo(RetType, pTree->ptag, pTree->taglen, &TagCont);
    if( iRet < 0 )
        return -1;
    else if( iRet > 0 )
        return 0;

    switch(RetType)
    {
    case WINKS_PROT_RET_TYPE_result:
        iRet = winks_ProtSetTagVal_result(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_serviceConfig:
        iRet = winks_ProtSetTagVal_serviceConfig(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_userServiceStatus:
        iRet = winks_ProtSetTagVal_userServiceStatus(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_serviceMode:
    #if 1 /* "mask WINKS_PROTOCOL_GETSERVICEMODE" */
        iRet = winks_ProtSetTagVal_ServicesType(pMemPool, pTree, &TagCont);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETSERVICEMODE");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_userConfig:
        iRet = winks_ProtSetTagVal_userConfig(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_winksShow:
        iRet = winks_ProtSetTagVal_winksShow(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_phoneBook:
    #if 0 /* "mask WINKS_PROTOCOL_GETPHONEBOOK" */
        iRet = winks_ProtSetTagVal_phoneBook(pMemPool, pTree, &TagCont);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETPHONEBOOK");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_UserWinksType:
        iRet = winks_ProtSetTagVal_UserWinksType(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_winksScript:
    #if 0 /* "mask WINKS_PROTOCOL_GETWINKSSCRIPT" */
        iRet = winks_ProtSetTagVal_winksScript(pMemPool, pTree, &TagCont);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETWINKSSCRIPT");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_shared:
    #if 0 /* "mask WINKS_PROTOCOL_SETSHARED" */
        iRet = winks_ProtSetTagVal_shared(pMemPool, pTree, &TagCont);
    #else
        Winks_fail("mask WINKS_PROTOCOL_SETSHARED");
    #endif
        break;
    case WINKS_PROT_RET_TYPE_softwarePackage:
        iRet = winks_ProtSetTagVal_softwarePackage(pMemPool, pTree, &TagCont);
        break;
    case WINKS_PROT_RET_TYPE_messageList:
    #if 0 /* "mask WINKS_PROTOCOL_GETMESSAGE" */
        iRet = winks_ProtSetTagVal_messageList(pMemPool, pTree, &TagCont);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETMESSAGE");
    #endif
        break;
    default:
        Winks_fail("winks_ProtDealTag");
        break;
    }

    return iRet;
}

static int winks_ProtSpreadTree( Winks_MemPool_s * pMemPool,
                                        Winks_XmlElement_s* pTree,
                                        int RetType)
{
#ifdef WINKS_DEBUG
	static Winks_XmlElement_s* pparent = NULL;
    static int TagNum = 0;
    static int Level = 0;
    char   space[64];
#endif
	if( pTree == NULL ||pTree->ptag == NULL || pTree->ptag[0] == 0 )
	{
		return -1;
	}
#ifdef WINKS_DEBUG
    TagNum++;
    Winks_mem_set(space, ' ', Level*4);
    space[Level*4] = '\0';
    if(pTree->pcontent && pTree->conlen)
    {
        Winks_printf("%sTag(%4d): \"%s\" Content: \"%s\"\r\n", 
                    space, TagNum, pTree->ptag, pTree->pcontent);
    }
    else
    {
        Winks_printf("%sTag(%4d): \"%s\"\r\n", space, TagNum, pTree->ptag);
    }
#endif
    
    if( winks_ProtDealTag(pMemPool, pTree, RetType) < 0 )
		return -1;
    
    if(pTree->pattribute)
    {
        Winks_XmlAttribute_s * pAttr = pTree->pattribute;
    #ifdef WINKS_DEBUG
        Winks_printf("    Attr: ");
    #endif
        while(pAttr)
        {
        #ifdef WINKS_DEBUG
            Winks_printf("%s=\"%s\" ", pAttr->pname, pAttr->pvalue);
        #endif
            pAttr = pAttr->pnext;
        }
    #ifdef WINKS_DEBUG
        Winks_printf("\r\n");
    #endif
    }
	if( pTree->pchild )
	{
    #ifdef WINKS_DEBUG
        Level++;
    #endif
		if( winks_ProtSpreadTree(pMemPool, pTree->pchild, RetType) < 0 )
			return -1;
	}

	if( pTree->pbrother )
	{
		if( winks_ProtSpreadTree(pMemPool, pTree->pbrother, RetType) < 0 )
			return -1;
	}

#ifdef WINKS_DEBUG
    if(Level>0 && pparent != pTree->pparent)
    {
	    pparent = pTree->pparent;
	    Level--;
    }
#endif
	return 0;
}

#ifndef WINKS_UNSUPPORTED_GLOBAL_VAR
static const char * Wk_Key[] = 
#else
static const char   Wk_Key[][13] = 
#endif
{
    "not key",          /* WK_KEY_NONE         */
    "service",          /* WK_KEY_service      */
    "config",           /* WK_KEY_config       */
    "user",             /* WK_KEY_user         */
    "activate",         /* WK_KEY_activate     */
    "status",           /* WK_KEY_status       */
    "winks",            /* WK_KEY_winks        */
    "upload",           /* WK_KEY_upload       */
    "show",             /* WK_KEY_show         */
    "specialwinks",     /* WK_KEY_specialwinks */
    "recommend",        /* WK_KEY_recommend    */
    "list",             /* WK_KEY_list         */
    "purchase",         /* WK_KEY_purchase     */
    "script",           /* WK_KEY_script       */
    "merge",            /* WK_KEY_merge        */
    "software",         /* WK_KEY_software     */
    "imsi",             /* WK_KEY_imsi         */
    "p",                /* WK_KEY_p            */
    "uid",              /* WK_KEY_uid          */
    "cl",               /* WK_KEY_cl           */
    "mt",               /* WK_KEY_mt           */
    "wt",               /* WK_KEY_wt           */
    "splash",           /* WK_KEY_splash       */
    "carrier",          /* WK_KEY_carrier      */
    "wid",              /* WK_KEY_wid          */
    "name",             /* WK_KEY_name         */
    "ext",              /* WK_KEY_ext          */
    "attachName",       /* WK_KEY_attachName   */
    "v",                /* WK_KEY_v            */
    "an",               /* WK_KEY_an           */
    "av",               /* WK_KEY_av           */
    "os",               /* WK_KEY_os           */
    "hw",               /* WK_KEY_hw           */
    "bua",              /* WK_KEY_bua          */
    "latest",           /* WK_KEY_latest       */
    "message",          /* WK_KEY_message      */
    "confirm",          /* WK_KEY_confirm      */
    "phonebook",        /* WK_KEY_phonebook    */
    "winksPO",          /* WK_KEY_winksPO      */
    "count",            /* WK_KEY_count        */
    "amount",           /* WK_KEY_amount       */
    "auth",             /* WK_KEY_auth         */
    "token",            /* WK_KEY_token        */
    "adc",              /* WK_KEY_adc          */
    "imei",             /* WK_KEY_imei         */
    "file",             /* WK_KEY_file         */
    "stat",             /* WK_KEY_stat         */
    "log",              /* WK_KEY_log          */
};

/* url中用到的关键字 */
#define WKPACK_service      Wk_Key[WK_KEY_service]
#define WKPACK_config       Wk_Key[WK_KEY_config]
#define WKPACK_user         Wk_Key[WK_KEY_user]
#define WKPACK_activate     Wk_Key[WK_KEY_activate]
#define WKPACK_status       Wk_Key[WK_KEY_status]
#define WKPACK_winks        Wk_Key[WK_KEY_winks]
#define WKPACK_upload       Wk_Key[WK_KEY_upload]
#define WKPACK_show         Wk_Key[WK_KEY_show]
#define WKPACK_specialwinks Wk_Key[WK_KEY_specialwinks]
#define WKPACK_recommend    Wk_Key[WK_KEY_recommend]
#define WKPACK_list         Wk_Key[WK_KEY_list]
#define WKPACK_purchase     Wk_Key[WK_KEY_purchase]
#define WKPACK_script       Wk_Key[WK_KEY_script]
#define WKPACK_merge        Wk_Key[WK_KEY_merge]
#define WKPACK_software     Wk_Key[WK_KEY_software]
#define WKPACK_imsi         Wk_Key[WK_KEY_imsi]
#define WKPACK_p            Wk_Key[WK_KEY_p]
#define WKPACK_uid          Wk_Key[WK_KEY_uid]
#define WKPACK_cl           Wk_Key[WK_KEY_cl]
#define WKPACK_mt           Wk_Key[WK_KEY_mt]
#define WKPACK_wt           Wk_Key[WK_KEY_wt]
#define WKPACK_splash       Wk_Key[WK_KEY_splash]
#define WKPACK_carrier      Wk_Key[WK_KEY_carrier]
#define WKPACK_wid          Wk_Key[WK_KEY_wid]
#define WKPACK_name         Wk_Key[WK_KEY_name]
#define WKPACK_ext          Wk_Key[WK_KEY_ext]
#define WKPACK_attachName   Wk_Key[WK_KEY_attachName]
#define WKPACK_v            Wk_Key[WK_KEY_v]
#define WKPACK_an           Wk_Key[WK_KEY_an]
#define WKPACK_av           Wk_Key[WK_KEY_av]
#define WKPACK_os           Wk_Key[WK_KEY_os]
#define WKPACK_hw           Wk_Key[WK_KEY_hw]
#define WKPACK_bua          Wk_Key[WK_KEY_bua]
#define WKPACK_latest       Wk_Key[WK_KEY_latest]
#define WKPACK_message      Wk_Key[WK_KEY_message]
#define WKPACK_confirm      Wk_Key[WK_KEY_confirm]
#define WKPACK_phonebook    Wk_Key[WK_KEY_phonebook]
#define WKPACK_winksPO      Wk_Key[WK_KEY_winksPO]
#define WKPACK_count        Wk_Key[WK_KEY_count]
#define WKPACK_amount       Wk_Key[WK_KEY_amount]
#define WKPACK_auth         Wk_Key[WK_KEY_auth]
#define WKPACK_token        Wk_Key[WK_KEY_token]
#define WKPACK_adc          Wk_Key[WK_KEY_adc]
#define WKPACK_imei         Wk_Key[WK_KEY_imei]
#define WKPACK_file         Wk_Key[WK_KEY_file]
#define WKPACK_stat         Wk_Key[WK_KEY_stat]
#define WKPACK_log          Wk_Key[WK_KEY_log]

/* post内容用到的关键字 */
#define WKPACK_phoneBook        TagName[WINKS_TAG_phoneBook].txt
#define WKPACK_pbi              TagName[WINKS_TAG_pbi].txt
#define WKPACK_n                TagName[WINKS_TAG_n].txt
//#define WKPACK_p                TagName[WINKS_TAG_p].txt
#define WKPACK_diy              TagName[WINKS_TAG_diy].txt
#define WKPACK_greeting         TagName[WINKS_TAG_greeting].txt
#define WKPACK_all              TagName[WINKS_TAG_all].txt
#define WKPACK_duration         TagName[WINKS_TAG_duration].txt
#define WKPACK_messageList      TagName[WINKS_TAG_messageList].txt
#define WKPACK_mi               TagName[WINKS_TAG_mi].txt
#define WKPACK_mid              TagName[WINKS_TAG_mid].txt
#define WKPACK_serviceConfig    TagName[WINKS_TAG_serviceConfig].txt
#define WKPACK_userConfig       TagName[WINKS_TAG_userConfig].txt
#define WKPACK_address          TagName[WINKS_TAG_address].txt
//#define WKPACK_config           TagName[WINKS_TAG_config].txt
//#define WKPACK_winks            TagName[WINKS_TAG_winks].txt
#define WKPACK_media            TagName[WINKS_TAG_media].txt
//#define WKPACK_store            TagName[WINKS_TAG_wap].txt
#define WKPACK_sms              TagName[WINKS_TAG_sms].txt
#define WKPACK_push             TagName[WINKS_TAG_push].txt
#define WKPACK_subscription     TagName[WINKS_TAG_subscription].txt
#define WKPACK_vcard            TagName[WINKS_TAG_vcard].txt
#define WKPACK_activity         TagName[WINKS_TAG_activity].txt
#define WKPACK_vcash            TagName[WINKS_TAG_vcash].txt
#define WKPACK_winksScript      TagName[WINKS_TAG_winksScript].txt
#define WKPACK_price            TagName[WINKS_TAG_price].txt
#define WKPACK_proxy                TagName[WINKS_TAG_proxy].txt
#define WKPACK_proxyPort            TagName[WINKS_TAG_proxyPort].txt
#define WKPACK_apn                  TagName[WINKS_TAG_apn].txt
#define WKPACK_durationAfterHangup  TagName[WINKS_TAG_durationAfterHangup].txt
#define WKPACK_showWinks            TagName[WINKS_TAG_showWinks].txt
#define WKPACK_showMode             TagName[WINKS_TAG_showMode].txt
#define WKPACK_roaming              TagName[WINKS_TAG_roaming].txt
#define WKPACK_autorun              TagName[WINKS_TAG_autorun].txt
#define WKPACK_updateAfterHangup    TagName[WINKS_TAG_updateAfterHangup].txt
#define WKPACK_upgradeMode          TagName[WINKS_TAG_upgradeMode].txt
#define WKPACK_vcardOverwrite       TagName[WINKS_TAG_vcardOverwrite].txt
#define WKPACK_connectInCall        TagName[WINKS_TAG_connectInCall].txt
#define WKPACK_support3G            TagName[WINKS_TAG_support3G].txt
#define WKPACK_networkTimeout       TagName[WINKS_TAG_networkTimeout].txt
#define WKPACK_splashDuration       TagName[WINKS_TAG_splashDuration].txt
#define WKPACK_updateConfig         TagName[WINKS_TAG_updateConfig].txt
#define WKPACK_option               TagName[WINKS_TAG_option].txt
#define WKPACK_period               TagName[WINKS_TAG_period].txt
#define WKPACK_shared               TagName[WINKS_TAG_shared].txt
#define WKPACK_statusText           TagName[WINKS_TAG_statusText].txt
#define WKPACK_showVcard            TagName[WINKS_TAG_showVcard].txt
#define WKPACK_services             TagName[WINKS_TAG_services].txt
/*
4.2.	协议版本
Winks-Protocol-Version: 1.0
;v=1.0
4.3.	传输内容编码
Content-Type: text/xml; charset=UTF-8
4.7.	分页查询
ps={pageSize};pn={pageNumber}
pageSize	每页返回条目数
pageNumber	当前查询页号，从1开始
4.10.	终端软件版本和硬件信息
Winks- Agent: winks-wm5/1.0 (WindowsMobile/6.1; 87AE9800)
an=winks-wm5;av=1.0;os=WM-6.1;hw=87AE9800
4.12.	加密
*/

#if 0/* for demo debug */
int Winks_GetImsi(int chn, char *buff, unsigned long buff_size)
{
    strncpy(buff, "460023105604051", buff_size);
    return 0;
}
#endif

static Winks_XmlElement_s * winks_ProtTreeCreate( Winks_MemPool_s * pMemPool,
                                                        const char * ptag,
                                                        Winks_XmlAttribute_s* pattribute,
                                                        char  * pcontent,
                                                        int     conlen )
{
    Winks_XmlElement_s * pTree = NULL;
    
    pTree = (Winks_XmlElement_s *) Winks_EngMemPoolAlloc(pMemPool, sizeof(Winks_XmlElement_s), 4);
    
    if(pTree)
    {
        //pTree->pchild = NULL;
        //pTree->pbrother = NULL;
        //pTree->pparent = NULL;
        pTree->taglen = strlen(ptag);
        pTree->ptag = (char *) Winks_EngMemPoolAlloc(pMemPool, pTree->taglen, 0);
        if(pTree->ptag == NULL)
            return NULL;
        strcpy(pTree->ptag, ptag);
        pTree->pattribute = pattribute;
        if(pcontent && conlen)
        {
            pTree->pcontent = (char *) Winks_EngMemPoolAlloc(pMemPool, conlen, 0);
            if(pTree->pcontent)
            {
                Winks_mem_cpy(pTree->pcontent, pcontent, conlen);
                pTree->conlen = conlen;
            }
            else
                return NULL;
        }
    }
    
	return pTree;
}

static int winks_ProtTreeAddChild( Winks_XmlElement_s * pparent, 
                                          Winks_XmlElement_s * pchild )
{
    Winks_assert(pparent);

    pchild->pparent = pparent;
    if(pparent->pchild == NULL)
        pparent->pchild = pchild;
    else
    {
        Winks_XmlElement_s * pbrother = pparent->pchild;

        while(pbrother->pbrother)
            pbrother = pbrother->pbrother;
    	pbrother->pbrother = pchild;
    }
    
    return 0;
}

static Winks_XmlElement_s * winks_ProtCreateTree_phoneBook(
                                        WinksProt_phoneBook_s * phoneBook, 
                                        Winks_MemPool_s * pMemPool)
{
    //int     iRet;
    Winks_XmlElement_s * pRoot = NULL;
    Winks_XmlElement_s * pTree = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_phoneBook, NULL,
                                        NULL, 0 )) == NULL )
        return NULL;

#if 0
    iRet = 0;
#endif
    while(phoneBook->pbi)
    {
        struct Prot_pbi_s * pbi = phoneBook->pbi;
        Winks_XmlElement_s * pChild = NULL;
    #if 0
        Winks_assert(iRet<phoneBook->pbi_num);
    #endif
        if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                            WKPACK_pbi, NULL,
                                            NULL, 0  )) == NULL )
            return NULL;

        winks_ProtTreeAddChild(pRoot, pTree);

        if(pbi->n)
        {
            if( (pChild = winks_ProtTreeCreate( pMemPool, 
                                                WKPACK_n, NULL,
                                                (char *)pbi->n, strlen((char *)pbi->n)/*pbi->nlen*/ )) == NULL )
                return NULL;

            winks_ProtTreeAddChild(pTree, pChild);
        }

        if(pbi->p == NULL)
        {
            Winks_assert(pbi->p);
            break;
        }
            
        if( (pChild = winks_ProtTreeCreate( pMemPool, 
                                            WKPACK_p, NULL,
                                            pbi->p, strlen(pbi->p) )) == NULL )
            return NULL;

        winks_ProtTreeAddChild(pTree, pChild);

        phoneBook->pbi = phoneBook->pbi->pnext;
    #if 0
        iRet++;
        if(iRet >= phoneBook->pbi_num)
            break;
    #endif
    }

    return pRoot;
}

#if 0
static int winks_str2upper(char *str1, int len1, char *str2, int len2)
{
    int cnt, len;
    int pos = 0;
    const char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    cnt = 0;
    len = MIN(len1, len2);
    while(cnt<len && str1)
    {
        if(*str1>='A' && *str1<='Z')
        {
        }
        else if(*str1>='a' && *str1<='z')
        {
            pos = *str1-'a';
            *str2 = alpha[pos];
            str2++;
        }
        else
            break;
        str1++;
        cnt++;
    }
    *str2 = '\0';
    return cnt;
}
#endif

#if 0//def WINKS_SIMULATOR
#define TEST_HTTP "http://192.168.10.91:8080/winksWS"
//#define TEST_HTTP "http://192.168.10.246:8080/winksWS"
//#define TEST_HTTP "http://192.168.10.92:8080/winksWS"
#endif

static int winks_ProtGetBaseUrl(int srv, char * pBase, int BaseSize)
{
    if(pBase == NULL || BaseSize==0)
        return -1;
    
    if(srv == WINKS_SRV_SETUP)
    {
    #ifdef TEST_HTTP /* 测试Http协议 */
        Winks_assert(BaseSize>(int)strlen(TEST_HTTP));
        strncpy(pBase, TEST_HTTP, BaseSize);
    #else
        Winks_assert(BaseSize>strlen(WINKS_HOST_SERVER_URL));
        strncpy(pBase, WINKS_HOST_SERVER_URL, BaseSize);
    #endif
    }
    else
    {
    #ifdef TEST_HTTP /* 测试Http协议 */
        Winks_assert(BaseSize>(int)strlen(TEST_HTTP));
        strncpy(pBase, TEST_HTTP, BaseSize);
    #else
        if(srv == WINKS_SRV_DATA)
        {
            if(Winks_GetSettingString(WINKS_SETTING_OPR_SERVER_URL/*WINKS_SETTING_MEDIA_SERVER_URL*/,
                        pBase, BaseSize) != 0  || pBase[0]==0)
                return -1;
        }
        else
        {
            if(Winks_GetSettingString(WINKS_SETTING_OPR_SERVER_URL,
                        pBase, BaseSize) != 0  || pBase[0]==0)
                return -1;
        }
    #endif
    }

    return 0;
}

static char * winks_ProtUrlAlloc( Winks_MemPool_s * pMemPool, char *pUrl, 
                                      int srv, int Check )
{
    char  * pUrlAlloc = NULL;
    int     AddBaseUrl = 1;

    Winks_assert(pMemPool);
    Winks_assert(pUrl);
    Winks_assert(srv>=0 && srv<WINKS_SRV_MAX);
    
    if(Check)
    {
    #if 0
        char    Tmp[8];
        Winks_mem_set(Tmp, 0, sizeof(Tmp));
        winks_str2upper(pUrl, strlen(pUrl), Tmp, 5);
        if(strncmp(Tmp, "HTTP:", 5) == 0)
            AddBaseUrl = 0;
    #else
        if(strnicmp(pUrl, "http:", 5) == 0)
            AddBaseUrl = 0;
    #endif
    }

    if(AddBaseUrl)
    {
        int  len;
        char Tmp[256];
        
        if( winks_ProtGetBaseUrl(srv, Tmp, sizeof(Tmp)) != 0 ||
            (len = strlen(Tmp)) <= 0 )
            return NULL;
        if(Tmp[len-1] != '/' && pUrl[0] != '/')
            Tmp[len++] = '/';
        Winks_assert((int)(pMemPool->offset+len+strlen(pUrl)+1) < pMemPool->len);
    #if 0
        pUrlAlloc = (char *)Winks_EngMemPoolAlloc(pMemPool, len+strlen(pUrl)+1, 0);
        if(pUrlAlloc == NULL)
            return NULL;
    #else
        pUrlAlloc = pMemPool->pdata + pMemPool->offset;
        pMemPool->offset += len+strlen(pUrl)+1;
    #endif
        strcpy(pUrlAlloc, Tmp);
        strcat(pUrlAlloc, pUrl);
    }
    else
    {
        Winks_assert((int)(pMemPool->offset+strlen(pUrl)) < pMemPool->len);
    #if 0
        pUrlAlloc = (char *)Winks_EngMemPoolAlloc(pMemPool, 
                                            strlen(pUrl)+1, 0);
        if(pUrlAlloc == NULL)
            return NULL;
    #else
        pUrlAlloc = pMemPool->pdata + pMemPool->offset;
        pMemPool->offset += strlen(pUrl)+1;
    #endif
        strcpy(pUrlAlloc, pUrl);
        
    }

    return pUrlAlloc;
}

static char * winks_ProtUrlStrcat( Winks_MemPool_s * pMemPool, 
                                        char *pUrl, char *pStr )
{
    char * pret = NULL;
    
    Winks_assert(pMemPool);
    Winks_assert(pUrl);
    Winks_assert(pStr);
    
    Winks_assert((int)(pMemPool->offset+strlen(pStr)) < pMemPool->len);
#if 0
    if((pret = Winks_EngMemPoolAlloc(pMemPool, strlen(pStr), 0)) == NULL)
        return NULL;
#else
    pret = pMemPool->pdata + pMemPool->offset;
    pMemPool->offset += strlen(pStr);
#endif
    strcat(pUrl, pStr);

    return pret;
}

static char * winks_ProtUrlAddVer( Winks_MemPool_s * pMemPool, char *pUrl, int IsQueryMark )
{
    char   Tmp[128];
    char * v;
    char * pRet = NULL;
    int     i;
    char  * Key = NULL, * Val = NULL;
    
    v  = WINKS_V;
    if(v == NULL || v[0] == 0)
        return NULL;

    /* 协议版本 */
    if(IsQueryMark)
        Tmp[0] = '?';
    else
        Tmp[0] = '&';
    sprintf(&Tmp[1], "%s=%s", WKPACK_v, v);
    if((pRet = winks_ProtUrlStrcat(pMemPool, pUrl, Tmp)) == NULL)
        return NULL;

    for(i=0; i<6; i++)
    {
        Val = NULL;
        
        switch(i)
        {
        case 0:/* 终端软件包名称 */
        #ifdef WINKS_AN
            Key = (char*)WKPACK_an;
            Val = WINKS_AN;
        #endif
            break;
        case 1:/* 终端软件包版本 */
        #ifdef WINKS_AV
            Key = (char*)WKPACK_av;
            Val = WINKS_AV;
        #endif
            break;
        case 2:/* 软件分发渠道标识（十进制整数），支持更新的平台必须填写 */
        #ifdef WINKS_ADC
            Key = (char *)WKPACK_adc;
            Val = WINKS_ADC;
        #endif
            break;
        case 3:/* 终端软件操作系统信息 */
        #ifdef WINKS_OS
            Key = (char *)WKPACK_os;
            Val = WINKS_OS;
        #endif
            break;
        case 4:/* 终端硬件标识 */
        #ifdef WINKS_HW
            Key = (char *)WKPACK_hw;
            Val = WINKS_HW;
        #endif
            break;
        case 5:/* base64url编码的终端默认浏览器的用户代理串 */
        #ifdef WINKS_BUA
            Key = WKPACK_bua;
            Val = WINKS_BUA;
        #endif
            break;
        }

        if(Key && Key[0] && Val && Val[0])
        {
            sprintf(Tmp, "&%s=%s", Key, Val);
            if((pRet = winks_ProtUrlStrcat(pMemPool, pUrl, Tmp)) == NULL)
                return NULL;
        }
    }

    return pRet;
}

/*
6.1.1.	查询彩像服务信息(GetServiceConfig)
请求：
GET /service/config?imsi={imsi}
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_serviceConfig -- WinksProt_serviceConfig_s
*/

/*
6.2.1.	鉴权(Auth)
请求：
GET /user/auth?token={token}&mode={mode}
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_userServiceStatus -- WinksProt_userServiceStatus_s
*/

/*
6.2.2.	设置服务订阅模式(SetServiceMode)
请求：
POST /user/{uid}/service?[imei={imei}]
services --- subscription
         | - diy
         | - vcard
         | - activity
         | - vcash
<services>
    <subscription>"limited"/"trial"/"adware"/"subscriber"</subscription>
    <diy>"true"/"false"</diy>
    <vcard>"true"/"false"</vcard>
    <activity>"true"/"false"</activity>
    <vcash>"true"/"false"</vcash>
</services>
输入参数:
    ServicesType_s
        WinksTag_SubscriptionModeType_e -> SubscriptionModeTypeVal
        WinksTag_ServiceStatusType_e    -> BoolTypeVal
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
#if 1 /* "mask WINKS_PROTOCOL_SETSERVICEMODE" */
static int winks_ProtSetServiceMode( ServicesType_s *pServicesType,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX)
{
    char    Buf[16];
    Winks_XmlElement_s *pRoot = NULL;
    Winks_XmlElement_s *pTree = NULL;
    int     i;
    char  * pTag = NULL;
    char  * pCont = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_services, NULL,
                                        NULL, 0 )) == NULL )
        return -1;

    pPackDataX->pRoot = pRoot;

    for(i=0; i<1/*5*/; i++)
    {
        switch(i)
        {
        case 0:
            pTag = (char *)WKPACK_subscription;
            if( pServicesType->subscription > WINKS_SUBSCRIPTION_NONE &&
                pServicesType->subscription < WINKS_SUBSCRIPTION_MAX )
                pCont = (char *)SubscriptionModeTypeVal[pServicesType->subscription].txt;
            else
                pCont = NULL;
            break;
        case 1:
            pTag = (char *)WKPACK_diy;
            pCont = (char *)SwitchVal[pServicesType->diy].txt;
            break;
        case 2:
            pTag = (char *)WKPACK_vcard;
            pCont = (char *)SwitchVal[pServicesType->vcard].txt;
            break;
        case 3:
            pTag = (char *)WKPACK_activity;
            pCont = (char *)SwitchVal[pServicesType->activity].txt;
            break;
        case 4:
            pTag = (char *)WKPACK_vcash;
            sprintf(Buf, "%d", pServicesType->vcash);
            pCont = Buf;
            break;
        }

        if(pTag && pCont)
        {
            if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                pTag, NULL,
                                                pCont, strlen(pCont) )) == NULL )
                return -1;
            winks_ProtTreeAddChild(pRoot, pTree);
        }
    }

    return 0;
}
#endif

/*
6.2.3.	查询服务订阅模式(GetServiceMode)
请求：
GET /user/{uid}/service
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_serviceMode -- ServicesType_s
*/

/*
6.2.4.	查询用户配置 (GetUserConfig)
请求：
GET /user/{uid}/config
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_userConfig -- WinksProt_userConfig_s
*/

/*
6.2.5.	设置用户配置 (SetUserConfig)
请求：
POST /user/{uid}/config
userConfig --- proxy
           | - proxyPort
           | - ...
           | - connectInCall
           | - support3G
<userConfig>
    <proxy>proxy</proxy>
    <proxyPort>proxyPort</proxyPort>
    ...
    <connectInCall>connectInCall</connectInCall>
    <support3G>support3G</support3G>
</userConfig>
输入参数:
    WinksProt_userConfig_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
#if 1 /* "mask WINKS_PROTOCOL_SETUSERCONFIG" */
static int winks_ProtSetUserConfig( WinksProt_userConfig_s * puserConfig,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    char    Buf[128];

    Winks_XmlElement_s *pRoot = NULL;
    Winks_XmlElement_s *pTree = NULL;
    int     i;
    char  * pTag = NULL;
    char  * pCont = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_userConfig, NULL,
                                        NULL, 0 )) == NULL )
        return -1;

    pPackDataX->pRoot = pRoot;

    for(i=0; i<15; i++)
    {
        pTag = NULL;
        pCont = NULL;
        
        switch(i)
        {
        case 0:
            if( puserConfig->proxy )
            {
            pTag = (char *)WKPACK_proxy;
            pCont = (char *)puserConfig->proxy;
            }
            break;
        case 1:
            if( puserConfig->proxyPort)
            {
            pTag = (char *)WKPACK_proxyPort;
            sprintf(Buf, "%d", puserConfig->proxyPort);
            pCont = (char *)Buf;
            }
            break;
        case 2:
            if( puserConfig->apn)
            {
            pTag = (char *)WKPACK_apn;
            pCont = (char *)puserConfig->apn;
            }
            break;
        case 3:
            if( puserConfig->durationAfterHangup)
            {
            pTag = (char *)WKPACK_durationAfterHangup;
            sprintf(Buf, "%d", puserConfig->durationAfterHangup);
            pCont = (char *)Buf;
            }
            break;
        case 4:
            if( puserConfig->showWinks_Mask )
            {
            pTag = (char *)WKPACK_showWinks;
            pCont = (char *)BoolTypeVal[puserConfig->showWinks].txt;
            }
            break;
        case 5:
            if( puserConfig->showMode_Mask )
            {
            pTag = (char *)WKPACK_showMode;
            sprintf(Buf, "%d", puserConfig->showMode);
            pCont = (char *)Buf;
            }
            break;
        case 6:
            if( puserConfig->roaming_Mask )
            {
            pTag = (char *)WKPACK_roaming;
            pCont = (char *)BoolTypeVal[puserConfig->roaming].txt;
            }
            break;
        case 7:
            if( puserConfig->autorun_Mask )
            {
            pTag = (char *)WKPACK_autorun;
            pCont = (char *)BoolTypeVal[puserConfig->autorun].txt;
            }
            break;
        case 8:
            if( puserConfig->updateAfterHangup_Mask )
            {
            pTag = (char *)WKPACK_updateAfterHangup;
            pCont = (char *)BoolTypeVal[puserConfig->updateAfterHangup].txt;
            }
            break;
        case 9:
            if( puserConfig->upgradeMode_Mask )
            {
            pTag = (char *)WKPACK_upgradeMode;
            pCont = (char *)upgradeModeVal[puserConfig->upgradeMode].txt;
            }
            break;
        case 10:
            if( puserConfig->vcardOverwrite_Mask )
            {
            pTag = (char *)WKPACK_vcardOverwrite;
            pCont = (char *)upgradeModeVal[puserConfig->vcardOverwrite].txt;
            }
            break;
        case 11:
            if( puserConfig->connectInCall_Mask )
            {
            pTag = (char *)WKPACK_connectInCall;
            pCont = (char *)BoolTypeVal[puserConfig->connectInCall].txt;
            }
            break;
        case 12:
            if( puserConfig->support3G_Mask )
            {
            pTag = (char *)WKPACK_support3G;
            pCont = (char *)BoolTypeVal[puserConfig->support3G].txt;
            }
            break;
        case 13:
            if( puserConfig->networkTimeout)
            {
            pTag = (char *)WKPACK_networkTimeout;
            sprintf(Buf, "%d", puserConfig->networkTimeout);
            pCont = (char *)Buf;
            }
            break;
        case 14:
            if( puserConfig->splashDuration)
            {
            pTag = (char *)WKPACK_splashDuration;
            sprintf(Buf, "%d", puserConfig->splashDuration);
            pCont = (char *)Buf;
            }
            break;
        default:
            break;
        }

        if(pTag && pCont)
        {
            if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                pTag, NULL,
                                                pCont, strlen(pCont) )) == NULL )
                return -1;
            winks_ProtTreeAddChild(pRoot, pTree);
        }
        
        if( puserConfig->updateConfig && 
            puserConfig->updateConfig->option > WINKS_updateConfig_none &&
            puserConfig->updateConfig->option < WINKS_updateConfig_max )
        {
            if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                WKPACK_updateConfig, NULL,
                                                NULL, 0 )) == NULL )
                return -1;
            winks_ProtTreeAddChild(pRoot, pTree);

            pRoot = pTree;
            
            pCont = (char *)updateConfigVal[puserConfig->updateConfig->option-1].txt;
            if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                WKPACK_option, NULL,
                                                pCont, strlen(pCont) )) == NULL )
                return -1;
            winks_ProtTreeAddChild(pRoot, pTree);

            if(puserConfig->updateConfig->option == WINKS_updateConfig_custom)
            {
                sprintf(Buf, "%d", puserConfig->updateConfig->period);
                if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                    WKPACK_period, NULL,
                                                    Buf, strlen(Buf) )) == NULL )
                    return -1;
                winks_ProtTreeAddChild(pRoot, pTree);
            }
        }
    }

    return 0;
}
#endif
/*
6.2.6.	上传本地电话本(MergePhoneBook)
请求：
POST /user/{uid}/phonebook/merge
phoneBook---pbi---n
                |-p
          |-pbi---n
                |-p
<phoneBook>
    <pbi>
        <n>name1</n>
        <p>13112345678<p>
    </pbi>
    <pbi>
        <n>name2</n>
        <p>13212345678<p>
    </pbi>
    <pbi>
        <n>name3</n>
        <p>13312345678<p>
    </pbi>
    <pbi>
        <n>name4</n>
        <p>13412345678<p>
    </pbi>
</phoneBook>
输入参数:
    WinksProt_phoneBook_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
static int winks_ProtMergePhoneBook(WinksProt_phoneBook_s * phoneBook,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    pPackDataX->pRoot = winks_ProtCreateTree_phoneBook(phoneBook, pMemPool);

    return 0;
}

/*
6.2.7.	查询服务器电话本(GetPhoneBook)
请求：
GET /user/{uid}/phonebook
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_phoneBook -- WinksProt_phoneBook_s
*/

/*
6.2.8.	上传DIY彩像(UploadWinks)
请求：
POST /user/{uid}/winks/upload?fn={fileName}
输入参数:
    WinksProt_UploadWinks_s
应答：
    WINKS_PROT_RET_TYPE_UserWinksType -- WinksProt_UserWinksType_s
*/
static int winks_ProtUploadWinks( WinksProt_UploadWinks_s * pUploadWinks, 
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    pPackDataX->ContentLen = pUploadWinks->FileSize;

    if(pUploadWinks->pContenttype)
    {
        pPackDataX->pContenttype = (char *)Winks_EngMemPoolAlloc(pMemPool, 
                                                strlen(pUploadWinks->pContenttype)+1, 0);
        if(pPackDataX->pContenttype == NULL)
            return -1;
        
        strcpy(pPackDataX->pContenttype, pUploadWinks->pContenttype);
    }

    if(pUploadWinks->attachName && pUploadWinks->attachNameLen)
    {
        pPackDataX->pPath = (unsigned char *)Winks_EngMemPoolAlloc(pMemPool, 
                                                pUploadWinks->attachNameLen, 0);
        if(pPackDataX->pPath == NULL)
            return -1;
        
        Winks_mem_cpy(pPackDataX->pPath, pUploadWinks->attachName, pUploadWinks->attachNameLen);
		pPackDataX->PathLen = pUploadWinks->attachNameLen;
    }

    return 0;
}

/*
6.2.9.	查询定制(GetWinksScript)
请求：
//GET /user/script?uid={uid};p={phoneNumber}
GET  /user/{uid}/script?p={phoneNumber}
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_winksScript -- WinksProt_winksScript_s
*/

/*
6.2.10.	创建定制(CreateWinksScript)
请求：
POST /user/{uid}/script 
winksScript---wid
            |-greeting
            |-p-p-p
            |-all
<winksScript>
    <wid>wid</wid>
    <greeting>greeting</greeting>
    <all></all>
</winksScript>
或
<winksScript>
    <wid>wid</wid>
    <greeting>greeting</greeting>
    <p>13100001111</p>
    <p>13100001112</p>
    <p>13100001113</p>
    <p>13100001114</p>
</winksScript>
输入参数:
    WinksProt_winksScript_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
static int winks_ProtCreateWinksScript(WinksProt_winksScript_s * winksScript,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    Winks_XmlElement_s * pRoot = NULL;
    Winks_XmlElement_s * pTree = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_winksScript, NULL,
                                        NULL, 0 )) == NULL )
        return -1;

    pPackDataX->pRoot = pRoot;
    
    if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_wid, NULL,
                                        winksScript->wid, strlen(winksScript->wid) ))
                                        == NULL )
        return -1;

    winks_ProtTreeAddChild(pRoot, pTree);

    if(winksScript->IsForAll)
    {
        if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                            WKPACK_all, NULL,
                                            (char *)WKPACK_all, strlen(WKPACK_all) )) == NULL )
            return -1;

        winks_ProtTreeAddChild(pRoot, pTree);
    }
    else
    {
        struct Prot_pno_s * pno = winksScript->pno;
        while(pno)
        {
            
            Winks_assert(pno->p);
            if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                WKPACK_p, NULL,
                                                pno->p, strlen(pno->p) )) == NULL )
                return -1;

            winks_ProtTreeAddChild(pRoot, pTree);
            
            pno = pno->pnext;
        }
    }

    if(winksScript->greeting)
    {
        if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                            WKPACK_greeting, NULL,
                                            (char *)winksScript->greeting, strlen((char *)winksScript->greeting)/*winksScript->greetlen*/ ))
                                            == NULL )
            return -1;

        winks_ProtTreeAddChild(pRoot, pTree);
    }

    
    return 0;
}

/*
6.2.11.	设置共享信息 (SetShared)
请求：
POST /user/{uid}/shared
shared---status
       |-statusText
       |-activity
       |-...
       |-showVcard
<shared>
    <status>state</status>
    <statusText>statusText</statusText>
    <activity>activity</activity>
    ...
    <showVcard>showVcard</showVcard>
</shared>
输入参数:
    WinksProt_winksScript_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
#if 0 /* "mask WINKS_PROTOCOL_SETSHARED" */
static int winks_ProtSetShared(WinksProt_shared_s * pshared,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    Winks_XmlElement_s * pRoot = NULL;
    Winks_XmlElement_s * pTree = NULL;
    int     i;
    char  * pTag = NULL;
    char  * pCont = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_shared, NULL,
                                        NULL, 0 )) == NULL )
        return -1;

    pPackDataX->pRoot = pRoot;

    for(i=0; i<7; i++)
    {
        pCont = NULL;
        switch(i)
        {
        case 0:
            pTag = (char *)WKPACK_status;
            if( pshared->status )
                pCont = (char *)statusVal[pshared->status-1].txt;
            break;
        case 1:
            pTag = (char *)WKPACK_statusText;
            if(pshared->statusText)
                pCont = (char *)pshared->statusText;
            break;
        case 2:
            pTag = (char *)WKPACK_activity;
            if(pshared->activity)
                pCont = (char *)pshared->activity;
            break;
        case 3:
            pTag = (char *)WKPACK_name;
            if(pshared->name)
                pCont = (char *)pshared->name;
            break;
        case 4:
            pTag = (char *)WKPACK_message;
            if(pshared->message)
                pCont = (char *)pshared->message;
            break;
        case 5:
            pTag = (char *)WKPACK_showWinks;
            if(pshared->showWinks_Mask)
                pCont = (char *)BoolTypeVal[pshared->showWinks].txt;
            break;
        case 6:
            pTag = (char *)WKPACK_showVcard;
            if(pshared->showVcard_Mask)
                pCont = (char *)BoolTypeVal[pshared->showVcard].txt;
            break;
        default:
            break;
        }

        if(pTag && pCont)
        {
            if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                                pTag, NULL,
                                                pCont, strlen(pCont) )) == NULL )
                return -1;
            winks_ProtTreeAddChild(pRoot, pTree);
        }
    }
    
    return 0;
}
#endif

/*
6.2.12.	查询共享信息(GetShared)
请求：
GET /user/{uid}/shared
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_shared -- WinksProt_shared_s
*/

/*
6.2.13.	推荐彩像服务(RecommendService)
请求：
POST /user/{uid}/service/recommend 
phoneBook---pbi---n
                |-p
          |-pbi---n
                |-p
<phoneBook>
    <pbi>
        <n>name1</n>
        <p>13112345678<p>
    </pbi>
    <pbi>
        <n>name2</n>
        <p>13212345678<p>
    </pbi>
    <pbi>
        <n>name3</n>
        <p>13312345678<p>
    </pbi>
    <pbi>
        <n>name4</n>
        <p>13412345678<p>
    </pbi>
</phoneBook>
输入参数:
    WinksProt_phoneBook_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
static int winks_ProtRecommendService( WinksProt_phoneBook_s * phoneBook, 
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    pPackDataX->pRoot = winks_ProtCreateTree_phoneBook(phoneBook, pMemPool);

    return 0;
}

/*
6.2.14.	购买彩像(PurchaseWinks)
请求：
POST /user/{uid}/winks/purchase
winksPO --- count
        & - price
        & - amount
        & - duration
        & - wid
<winksPO>
    <count>2</count>
    <price>10</price>
    <amount>20</amount>
    <duration>duration</duration>
    <wid>wid</wid>
</winksPO>
输入参数:
    WinksProt_winksPO_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
#if 0 /* "mask WINKS_PROTOCOL_PURCHASEWINKS" */
static int winks_ProtPurchaseWinks(WinksProt_winksPO_s * pwinksPO,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    char    Buf[256];
    int     i;
    char  * pTag = NULL;
    Winks_XmlElement_s * pRoot = NULL;
    Winks_XmlElement_s * pTree = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_winksPO, NULL,
                                        NULL, 0 )) == NULL )
        return -1;

    pPackDataX->pRoot = pRoot;


    for(i=0; i<5; i++)
    {
        switch(i)
        {
        case 0:
            pTag = (char *)WKPACK_count;
            sprintf(Buf, "%d", pwinksPO->count);
            break;
        case 1:
            pTag = (char *)WKPACK_price;
            sprintf(Buf, "%d", pwinksPO->price);
            break;
        case 2:
            pTag = (char *)WKPACK_amount;
            sprintf(Buf, "%d", pwinksPO->amount);
            break;
        case 3:
            pTag = (char *)WKPACK_duration;
            sprintf(Buf, "%d", pwinksPO->duration);
            break;
        case 4:
            pTag = (char *)WKPACK_wid;
            strcpy(Buf, pwinksPO->wid);
            break;
        default:
            return -1;
        }
        if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                            pTag, NULL,
                                            Buf, strlen(Buf) )) == NULL )
            return -1;
        winks_ProtTreeAddChild(pRoot, pTree);
    }

    return 0;
}
#endif

/*
6.2.15.	获取用户彩像信息(GetUserWinks)
请求：GET /user/{uid}/winks/{wid}
输入参数:
    wid   : 彩像id
应答：
    WINKS_PROT_RET_TYPE_UserWinksType -- WinksProt_UserWinksType_s
*/

/*
6.3.1.	查询彩像展示信息(ShowWinks)
请求：
GET /winks/show?p={phoneNumber}[&uid={uid}][&cl={canvasList}][&mt={mediaTypes}]
canvasList = 1#( {canvasWidth} "x" {canvasHeight} )
mediaTypes = 1#( media-range )
输入参数:
    phoneNumber : 电话号码
应答：
    WINKS_PROT_RET_TYPE_winksShow -- WinksProt_winksShow_s
*/

/*
6.3.2.	查询特殊彩像展示信息(ShowSpecialWinks)
请求：
GET /user/{uid}/specialwinks/{winksType}[?cl={canvasList}][&mt={mediaTypes}]
对于未鉴权用户，可以使用
GET /specialwinks/{winksType}[?cl={canvasList}][&mt={mediaTypes}]
winksType	特殊彩像的标识，取值范围是如下字符串枚举值：
            splash	片头彩像
            carrier	运营商彩像
canvasList = 1#( {canvasWidth} "x" {canvasHeight} )
mediaTypes = 1#( media-range )
输入参数:
    wt : WINKS_WT_SPLASH / WINKS_WT_CARRIER
应答：
    WINKS_PROT_RET_TYPE_winksShow -- WinksProt_winksShow_s
*/

/*
请求：
Get url 
输入参数:
    url   : 
应答：
    WINKS_PROT_RET_TYPE_file -- 彩像文件
*/

/*
6.4.1.	上传文件(UploadFile)
POST /file/upload?ft={fileType}[&fn={fileNumber}][&uid={uid}]
输入参数:
    WinksProt_UploadFile_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
static int winks_ProtUploadFile( WinksProt_UploadFile_s * pUploadFile, 
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    unsigned int   FileSize;   /* input 文件大小 */

    if(Winks_fgetsize((char *)pUploadFile->attachName, 0, &FileSize) != 0)
        return -1;
    
    pPackDataX->ContentLen = FileSize;
    
    if(pUploadFile->pContenttype)
    {
        pPackDataX->pContenttype = (char *)Winks_EngMemPoolAlloc(pMemPool, 
                                                strlen(pUploadFile->pContenttype)+1, 0);
        if(pPackDataX->pContenttype == NULL)
            return -1;
        
        strcpy(pPackDataX->pContenttype, pUploadFile->pContenttype);
    }

    if(pUploadFile->attachName && pUploadFile->attachNameLen)
    {
        pPackDataX->pPath = (unsigned char *)Winks_EngMemPoolAlloc(pMemPool, 
                                                pUploadFile->attachNameLen, 0);
        if(pPackDataX->pPath == NULL)
            return -1;
        
        Winks_mem_cpy(pPackDataX->pPath, pUploadFile->attachName, pUploadFile->attachNameLen);
		pPackDataX->PathLen = pUploadFile->attachNameLen;
    }

    return 0;
}

/*
6.5.1.	取消息(GetMessage)
请求：
GET /user/{uid}/message
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_messageList -- WinksProt_messageList_s
*/

/*
6.5.2.	确认通知消息 (ConfirmUserMessage)
请求：
POST /user/{uid}/message/confirm
confimMessage --- mid
<confimMessage>
    <mid>mid1</mid>
    <mid>mid2</mid>
</confimMessage>
输入参数:
    WinksProt_confimMessage_s
应答：
    WINKS_PROT_RET_TYPE_result -- WinksProt_result_s
*/
static int winks_ProtConfirmMessage(WinksProt_confimMessage_s * pconfimMessage,
                                        Winks_MemPool_s * pMemPool, 
                                        WinksProt_PackDataX_s * pPackDataX )
{
    char    Buf[128];
    Winks_XmlElement_s * pRoot = NULL;
    Winks_XmlElement_s * pTree = NULL;

    if( (pRoot = winks_ProtTreeCreate( pMemPool, 
                                        "confimMessage", NULL,
                                        NULL, 0 )) == NULL )
        return -1;

    pPackDataX->pRoot = pRoot;


    sprintf(Buf, "%d", pconfimMessage->mid);
    if( (pTree = winks_ProtTreeCreate( pMemPool, 
                                        WKPACK_mid, NULL,
                                        Buf, strlen(Buf) )) == NULL )
        return -1;

    winks_ProtTreeAddChild(pRoot, pTree);

    return 0;
}

/*
6.6.1.	查询本终端最新的软件包信息(GetLatestSoftwarePackage)
请求：
GET /software/winks/latest?an={package-name}&av={version} [&os={client-os}] [&hw={hardware-id}] [(&bua={base64-user-agent}] [&adc={dist-channel}]
输入参数:
    
应答：
    WINKS_PROT_RET_TYPE_softwarePackage -- WinksProt_softwarePackage_s
*/

/* 协议打包: 将xml树转换为xml文件 
输入:
    pTree   : 
    pPackDataX   :
输出:
    
返回:
    拷贝的输入参数及生成的xml文件数据，该数据为动态申请的内存，使用后需要释放
*/
static WinksProt_PackData_s * winks_ProtGenXml(WinksProt_PackDataX_s * pPackDataX)
{
	Winks_Xml_s Xml;
    
	WinksProt_PackData_s * pPackData = NULL;
    char  * pBuf;
	int     BufSize = 1024;
    int     StuctSize = 0;
    int     iRet = 0;

    Winks_assert(pPackDataX);
    
	Xml.mainver = 1;
	Xml.slavever = 0;
	Xml.encoding = WINKS_WX_CUTF8;
	Xml.ifsd = 1;
    Xml.pelement = pPackDataX->pRoot;

    pBuf = (char *)Winks_salAlloc(BufSize);
    if(pBuf == NULL)
    {
        Winks_printf("Err: winks_ProtGenXml alloc(%d)\r\n", BufSize);
        return NULL;
    }

    while(pBuf)
    {
        StuctSize = sizeof(WinksProt_PackData_s);
        
    	Winks_mem_set(pBuf, 0, BufSize);
        pPackData = (WinksProt_PackData_s *)pBuf;
        pPackData->method = pPackDataX->method;
        pPackData->server = pPackDataX->server;
        pPackData->IsOnlyPath = pPackDataX->IsOnlyPath;
        pPackData->ContentLen = pPackDataX->ContentLen;
        pPackData->PathLen =  pPackDataX->PathLen;

        if(pPackDataX->pPath)
        {
            pPackData->pPath = (unsigned char *)&pBuf[StuctSize];
            Winks_mem_cpy(pPackData->pPath, pPackDataX->pPath, pPackData->PathLen);
            StuctSize += pPackData->PathLen+2;
        }
        if(pPackDataX->url)
        {
            pPackData->url = &pBuf[StuctSize];
            strcpy(pPackData->url, pPackDataX->url);
            StuctSize += strlen(pPackData->url)+1;
        }
        if(pPackDataX->pContenttype)
        {
            pPackData->pContenttype = &pBuf[StuctSize];
            strcpy(pPackData->pContenttype, pPackDataX->pContenttype);
            StuctSize += strlen(pPackData->pContenttype)+1;
        }
        StuctSize = WINKS_ALIGN(StuctSize, 4);
        BufSize -= StuctSize;
        
        pPackData->pData = &pBuf[StuctSize];
        iRet = Winks_XML_Generate( &Xml, &pBuf[StuctSize], &BufSize );
        if(iRet > 0)
		{
			pPackData->DataLen = BufSize;
            break;
		}
        if(iRet == WINKS_WX_EBUFFSHORT)
        {
            Winks_printf("WARN: winks_ProtGenXml need mem %d+%d\r\n", BufSize, StuctSize);
            Winks_salFree(pBuf);
            BufSize += StuctSize;
            pBuf = (char *)Winks_salAlloc(BufSize);
            if(pBuf == NULL)
                return NULL;
        }
        else if(iRet <= 0)
        {
            Winks_salFree(pBuf);
            return NULL;
        }
        
    }

	return pPackData;
}

///////////////////////////////////////////////////////////////////////////////
//            E X T E R N A L    F U N C T I O N 
///////////////////////////////////////////////////////////////////////////////
#if 0  /* for demo debug */
static const char * xmlfile[] =
{
/*WINKS_PROTOCOL_NONE                    */ "protsim/null.xml",
/*WINKS_PROTOCOL_GETSERVICECONFIG        */ "protsim/serviceConfig.xml",
/*WINKS_PROTOCOL_ACTIVATE                */ "protsim/result_active.xml",
/*WINKS_PROTOCOL_AUTH                    */ "protsim/userServiceStatus.xml",
/*WINKS_PROTOCOL_UPLOADWINKS             */ "protsim/wid.xml",
/*WINKS_PROTOCOL_SHOWWINKS               */ "protsim/winksShow.xml",
/*WINKS_PROTOCOL_SHOWSPECIALWINKS        */ "protsim/winksShow_Special.xml",
/*WINKS_PROTOCOL_GETURLFILE              */ "protsim/Img0001.jpg",
/*WINKS_PROTOCOL_RECOMMENDSERVICE        */ "protsim/result_recommend.xml",
/*WINKS_PROTOCOL_GETUSERWINKS            */ "protsim/UserWinksType.xml",
/*WINKS_PROTOCOL_PURCHASEWINKS           */ "protsim/result_purchase.xml",
/*WINKS_PROTOCOL_CREATEWINKSSCRIPT       */ "protsim/winksScript.xml",
/*WINKS_PROTOCOL_MERGEPHONEBOOK          */ "protsim/result_merge.xml",
/*WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE*/ "protsim/softwarePackage.xml",
};

int SimuHttpRet(int ProtocolNo, Winks_MemPool_s ** ppMem)
{
    FILE * fp = NULL;
    char * file = NULL;
    Winks_MemPool_s * pMemPool = NULL;
    char * pBuf = NULL;
    int    ret = 0;

    if( ProtocolNo>WINKS_PROTOCOL_NONE &&
        ProtocolNo<WINKS_PROTOCOL_MAX )
        file = xmlfile[ProtocolNo];

    if(file == NULL)
        return -1;
    
    if(Winks_EngMemPoolCreate(&pMemPool) == NULL)
        return -1;
    pBuf = Winks_EngMemPoolAlloc(pMemPool, WINKS_PROT_MEM_BLOCK_LEN, 4);
    if(pBuf == NULL)
    {
        Winks_EngMemPoolDestory(pMemPool);
        return -1;
    }
    
    fp = Winks_fopen(file, "rb");
    while(pBuf)
    {
        ret = Winks_fread(pBuf, WINKS_PROT_MEM_BLOCK_LEN, 1, fp);
        if(ret == 1)
            pBuf = Winks_EngMemPoolAlloc(pMemPool, WINKS_PROT_MEM_BLOCK_LEN, 0);
        else
            break;
    }

    if(ppMem)
        *ppMem = pMemPool;
    
    return 0;
}

int SimuHttpRetFree(Winks_MemPool_s * pMemPool)
{
    return Winks_EngMemPoolDestory(pMemPool);
}
#endif

#if 0//def WINKS_DEBUG
static void winks_PrintProtParse(WinksProt_ParseData_s *pParseData)
{
    Winks_assert(pParseData);

    Winks_printf("\r\nwinks_PrintProtParse: %d\r\n", pParseData->RetType);
    switch(pParseData->RetType)
    {
    case WINKS_PROT_RET_TYPE_result:
		{
        WinksProt_result_s * presult = (WinksProt_result_s *)pParseData->pData;
        Winks_printf("WinksProt_result_s:\r\n");
        Winks_printf("code: %d\r\n", presult->code);
		}
        break;
    case WINKS_PROT_RET_TYPE_serviceConfig:
		{
        WinksProt_serviceConfig_s * pserviceConfig = (WinksProt_serviceConfig_s *)pParseData->pData;
        Winks_printf("WinksProt_serviceConfig_s:\r\n");
        Winks_printf("address: 0x%08x\r\n", pserviceConfig->address);
        Winks_printf("config: 0x%08x\r\n", pserviceConfig->config);
		}
        break;
    case WINKS_PROT_RET_TYPE_userServiceStatus:
		{
        WinksProt_userServiceStatus_s * puserServiceStatus = (WinksProt_userServiceStatus_s *)pParseData->pData;
        Winks_printf("WinksProt_userServiceStatus_s:\r\n");
        Winks_printf("IsRegistered: %d\r\n", puserServiceStatus->IsRegistered);
		}
        break;
    case WINKS_PROT_RET_TYPE_winksShow:
		{
        WinksProt_winksShow_s * pwinksShow = (WinksProt_winksShow_s *)pParseData->pData;
        Winks_printf("WinksProt_winksShow_s:\r\n");
        Winks_printf("wid: %s\r\n", pwinksShow->wid);
		}
        break;
    case WINKS_PROT_RET_TYPE_phoneBook:
		{
        WinksProt_phoneBook_s * pphoneBook = (WinksProt_phoneBook_s *)pParseData->pData;
        Winks_printf("WinksProt_phoneBook_s:\r\n");
        Winks_printf("pbi_num: %d\r\n", pphoneBook->pbi_num);
		}
        break;
    case WINKS_PROT_RET_TYPE_UserWinksType:
		{
        WinksProt_UserWinksType_s * pUserWinksType = (WinksProt_UserWinksType_s *)pParseData->pData;
        Winks_printf("WinksProt_UserWinksType_s:\r\n");
        Winks_printf("wid: %s\r\n", pUserWinksType->wid);
		}
        break;
    case WINKS_PROT_RET_TYPE_winksScript:
		{
        WinksProt_winksScript_s * pwinksScript = (WinksProt_winksScript_s *)pParseData->pData;
        Winks_printf("WinksProt_winksScript_s:\r\n");
        Winks_printf("wid: %s\r\n", pwinksScript->wid);
		}
        break;
    case WINKS_PROT_RET_TYPE_softwarePackage:
		{
        WinksProt_softwarePackage_s * psoftwarePackage = (WinksProt_softwarePackage_s *)pParseData->pData;
        Winks_printf("WinksProt_softwarePackage_s:\r\n");
        Winks_printf("namelen: %d\r\n", psoftwarePackage->namelen);
		}
        break;
    case WINKS_PROT_RET_TYPE_str:
		{
        char * pStr = (char *)pParseData->pData;
        Winks_printf("WINKS_PROT_RET_TYPE_str:\r\n");
        Winks_printf("str: %s\r\n", pStr);
		}
        break;
    default:
        Winks_fail("winks_ProtPrintStruct");
        break;
    }

}
#endif
#ifdef WINKS_DEBUG
extern void Winks_PrintfXml(char *pbuf, int BufSize);
static void winks_PrintProtPack(WinksProt_PackData_s * pProtPack, int prot)
{
	Winks_assert(pProtPack);

    Winks_printf("\r\nwinks_PrintProtPack: %d \r\n", prot);
    if(pProtPack->url)
        Winks_printf("    url: %s\r\n", pProtPack->url);
    if(pProtPack->pContenttype)
        Winks_printf("    Contenttype: %s\r\n", pProtPack->pContenttype);
    Winks_printf("    method=%d server=%d IsOnlyPath=%d\r\n", pProtPack->method, pProtPack->server, pProtPack->IsOnlyPath);

    if(pProtPack->pData && pProtPack->DataLen)
    {
        Winks_PrintfXml(pProtPack->pData, pProtPack->DataLen);
        //Winks_printf(pProtPack->pData);
        //Winks_printf("\r\n");
        Winks_printHex((unsigned char*)pProtPack->pData, (unsigned short)pProtPack->DataLen, (unsigned char *)"[ProtPack]:\r\n");
    }
}

#endif

/* 协议解析: 将Http返回的xml文档数据解析为引擎可识别的结构体数据
输入:
    pXmlTree    : xml树
输出:
    ppParseData : 解析出来的协议相关结构体数据
返回:
     0 -- 成功
    -1 -- 解析失败
    -2 -- 申请内存失败
*/
int Winks_ProtParse( Winks_XmlElement_s * pXmlTree, 
                          WinksProt_ParseData_s ** ppParseData )
{
    Winks_MemPool_s * pMemPool = NULL;

    WinksProt_ParseDataX_s * pParseDataX = NULL;
    int RetType;

    Winks_assert(pXmlTree);
    Winks_assert(ppParseData);
    
    if(Winks_EngMemPoolCreate(&pMemPool) == NULL || pMemPool == NULL)
        return -3;

    RetType = winks_ProtGetRetType(pXmlTree->ptag);
    
    if(RetType >= WINKS_PROT_RET_TYPE_str)
    {
        RetType = winks_ProtGetRetTypeX(pXmlTree->ptag);
        if(RetType == WINKS_PROT_RET_TYPE_MAX)
            return -1;

        if(RetType == WINKS_PROT_RET_TYPE_str)
        {
            if(pXmlTree->pcontent && pXmlTree->conlen)
            {
                char *pStr = (char *)Winks_EngMemPoolAlloc(pMemPool, pXmlTree->conlen, 0);
                if(pStr == NULL)
                    return -3;
                strncpy(pStr, pXmlTree->pcontent, pXmlTree->conlen);
            }
        }
    }
    else
    {
        if(winks_ProtSpreadTree(pMemPool, pXmlTree, RetType) != 0)
            return -1;
    }
    
    pParseDataX = Winks_EngMemPoolAlloc(pMemPool, sizeof(WinksProt_ParseDataX_s), 4);
    if(pParseDataX == NULL)
        return -3;
    
    pParseDataX->RetType = RetType;
    pParseDataX->pData = (void *)pMemPool->pdata;
    pParseDataX->pMem = pMemPool;
    
    if(ppParseData)
        *ppParseData = (WinksProt_ParseData_s *)pParseDataX;
    
#if 0//def WINKS_DEBUG
    winks_PrintProtParse(*ppParseData);
#endif
    return 0;
}

int Winks_ProtParseFree(WinksProt_ParseData_s * pParseData)
{
    WinksProt_ParseDataX_s * pParseDataX;
    
    if(pParseData)
    {
        pParseDataX = (WinksProt_ParseDataX_s *)pParseData;
        if(pParseDataX->pMem)
            Winks_EngMemPoolDestory(pParseDataX->pMem);
    }
    return 0;
}

/* 协议打包: 将模块输入的结构体数据打包成xml文档 
输入:
    ProtocolNo  : 需要打包的协议号
    indata      : 需要输入的参数(无参数可以为空)
    inLen       : 输入参数的长度(无参数可以为0)
输出:
    ppProtPack  : 打包出来提供给http发送的结构体数据，
                  pProtPack->pData为实际发送的xml数据
返回:
    成功或失败。
*/
int Winks_ProtPack( WinksProtocolEnum_e ProtocolNo, void  * inData, int inLen,
                         WinksProt_PackData_s ** ppProtPack )
{
    int     iRet = 0;
    Winks_MemPool_s * pMemPool = NULL;
    WinksProt_PackData_s * pProtPack = NULL;
    WinksProt_PackDataX_s * pPackDataX = NULL;
    WinksSetServiceMode_s * pSetServiceMode = NULL;
    char    imsi[16];
    char    uid[WINKS_PROT_UID_MAX];        /* input */
    char    Buf[256];
    int     IsQueryMark = 1;
    int     NeedCheckUrl = 0;

    
    Winks_assert( ProtocolNo > WINKS_PROTOCOL_NONE && 
                  ProtocolNo < WINKS_PROTOCOL_MAX );

    Winks_mem_set(imsi, 0, sizeof(imsi));
    Winks_mem_set(uid, 0, sizeof(uid));
    
	switch(ProtocolNo)
	{
    case WINKS_PROTOCOL_GETSERVICECONFIG:
    case WINKS_PROTOCOL_AUTH:
        if(Winks_GetImsi(0, imsi, sizeof(imsi)) != 0)
        {
            Winks_fail("Winks_GetImsi");
            return -1;
        }
        break;
    case WINKS_PROTOCOL_SETSERVICEMODE:
    {
        if(inData == NULL)
        {
            Winks_assert(inData);
            return -1;
        }
        pSetServiceMode = (WinksSetServiceMode_s *)inData;
        strcpy(uid, pSetServiceMode->uid);
        break;
    }
    case WINKS_PROTOCOL_GETSERVICEMODE:
    case WINKS_PROTOCOL_GETUSERCONFIG:
    case WINKS_PROTOCOL_SETUSERCONFIG:
    case WINKS_PROTOCOL_MERGEPHONEBOOK:
    case WINKS_PROTOCOL_GETPHONEBOOK:
    case WINKS_PROTOCOL_UPLOADWINKS:
    case WINKS_PROTOCOL_GETWINKSSCRIPT:
    case WINKS_PROTOCOL_CREATEWINKSSCRIPT:
    case WINKS_PROTOCOL_SETSHARED:
    case WINKS_PROTOCOL_GETSHARED:
    case WINKS_PROTOCOL_RECOMMENDSERVICE:
    case WINKS_PROTOCOL_PURCHASEWINKS:
    case WINKS_PROTOCOL_GETUSERWINKS:
    case WINKS_PROTOCOL_GETURLFILE:
    case WINKS_PROTOCOL_UPLOADFILE:
    case WINKS_PROTOCOL_GETMESSAGE:
    case WINKS_PROTOCOL_CONFIRMMESSAGE:
        if(Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid)) != 0 || uid[0]==0)
        {
            Winks_fail("Winks_GetSettingString uid");
            return -1;
        }
        break;
    case WINKS_PROTOCOL_SHOWWINKS:
    case WINKS_PROTOCOL_SHOWSPECIALWINKS:
    case WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE:
        break;
    default:
        Winks_fail("Winks_ProtPack error case");
        return -1;
    }
    
    if(Winks_EngMemPoolCreate(&pMemPool) == NULL || pMemPool == NULL)
        return -1;
    
    pPackDataX = (WinksProt_PackDataX_s *)Winks_EngMemPoolAlloc(pMemPool, 
                                            sizeof(WinksProt_PackDataX_s),
                                            4);
    if(pPackDataX == NULL)
    {
        Winks_EngMemPoolDestory(pMemPool);
        return -1;
    }
    
    pPackDataX->method = WINKS_METHOD_GET;
    pPackDataX->server = WINKS_SRV_COMM;
    pPackDataX->IsOnlyPath = 0;

    Winks_mem_set(Buf, 0, sizeof(Buf));

	switch(ProtocolNo)
	{
    case WINKS_PROTOCOL_GETSERVICECONFIG:
        pPackDataX->server = WINKS_SRV_SETUP;
        sprintf(Buf, "/%s/%s?%s=%s", WKPACK_service, 
                                     WKPACK_config, 
                                     WKPACK_imsi, 
                                     imsi);
        IsQueryMark = 0;
        break;
    case WINKS_PROTOCOL_AUTH:
        {
        char    token[128];     /* input */

        Winks_mem_set(token, 0, sizeof(token));
        
    #if 0   // 0--normal; 1--debug
        sprintf(token, "%s&mode=debug", imsi);
    #else
        Winks_printf("Call Winks_Encode!");
        if( Winks_Encode(imsi, strlen(imsi), token, sizeof(token)) != 0 )
        {
            Winks_EngMemPoolDestory(pMemPool);
            return -1;
        }
    #endif
        
        sprintf(Buf, "/%s/%s?%s=%s", WKPACK_user, 
                                     WKPACK_auth, 
                                     WKPACK_token, 
                                     token);
        IsQueryMark = 0;
        }
        break;
    case WINKS_PROTOCOL_SETSERVICEMODE:
    #if 1 /* "mask WINKS_PROTOCOL_SETSERVICEMODE" */
        {
        char imei[WINKS_IMEI_LEN+1];
        pPackDataX->method = WINKS_METHOD_POST;
      #if 1
        Winks_GetImei(WINKS_CHN_NULL, imei, sizeof(imei));
        sprintf(Buf, "/%s/%s/%s?%s=%s", WKPACK_user, 
                                        uid, 
                                        WKPACK_service,
                                        WKPACK_imei, 
                                        imei);
      #endif
        IsQueryMark = 0;
        }
    #else
        Winks_fail("mask WINKS_PROTOCOL_SETSERVICEMODE");
    #endif
        break;
    case WINKS_PROTOCOL_GETSERVICEMODE:
    #if 1 /* "mask WINKS_PROTOCOL_GETSERVICEMODE" */
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid, 
                                  WKPACK_service);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETSERVICEMODE");
    #endif
        break;
    case WINKS_PROTOCOL_GETUSERCONFIG:
        {
        int AllOrDiff = 0;
      #if 1
        Winks_assert(inData);
        AllOrDiff = *((int *)inData);
        if(AllOrDiff)
            sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user, 
                                         uid, 
                                         WKPACK_config,
                                         WKPACK_all);
        else
            sprintf(Buf, "/%s/%s/%s/diff", WKPACK_user, 
                                           uid, 
                                           WKPACK_config);
      #else
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid, 
                                  WKPACK_config);
      #endif
        }
        break;
    case WINKS_PROTOCOL_SETUSERCONFIG:
    #if 1 /* "mask WINKS_PROTOCOL_SETUSERCONFIG" */
    {
        WinksProt_userConfigEx_s * puserConfigEx = (WinksProt_userConfigEx_s *)inData;
        Winks_assert(puserConfigEx);
        pPackDataX->method = WINKS_METHOD_POST;
        if(puserConfigEx->AllOrDiff)
            sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user, 
                                         uid, 
                                         WKPACK_config,
                                         WKPACK_all);
        else
            sprintf(Buf, "/%s/%s/%s/diff", WKPACK_user, 
                                           uid, 
                                           WKPACK_config);
    }
    #else
        Winks_fail("mask WINKS_PROTOCOL_SETUSERCONFIG");
    #endif
       break;
    case WINKS_PROTOCOL_MERGEPHONEBOOK:
        pPackDataX->method = WINKS_METHOD_POST;
        sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user, 
                                     uid,
                                     WKPACK_phonebook,
                                     WKPACK_merge);
        break;
    case WINKS_PROTOCOL_GETPHONEBOOK:
    #if 0 /* "mask WINKS_PROTOCOL_GETPHONEBOOK" */
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid,
                                  WKPACK_phonebook);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETPHONEBOOK");
    #endif
        break;
    case WINKS_PROTOCOL_UPLOADWINKS:
        {
        WinksProt_UploadWinks_s * pUploadWinks = NULL;
        
        pPackDataX->method = WINKS_METHOD_POST;
        Winks_assert(inData);
        pUploadWinks = (WinksProt_UploadWinks_s *)inData;
        pPackDataX->IsOnlyPath = 1;
        sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user, 
                                     uid,
                                     WKPACK_winks,
                                     WKPACK_upload);

        if(pUploadWinks->name)
        {
            iRet = strlen(Buf);
            //TODO: name base64 encode
            sprintf(&Buf[iRet], "?fn=%s", pUploadWinks->name);
            IsQueryMark = 0;
        }
        }
        break;
    case WINKS_PROTOCOL_GETWINKSSCRIPT:
    #if 0 /* "mask WINKS_PROTOCOL_GETWINKSSCRIPT" */
        {
        char * phoneNumber = NULL;
        
        Winks_assert(inData);
        phoneNumber = (char *)inData;
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid,
                                  WKPACK_script);

        iRet = strlen(Buf);
        sprintf(&Buf[iRet], "?%s=%s", WKPACK_p, phoneNumber);
        IsQueryMark = 0;
        }
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETWINKSSCRIPT");
    #endif
        break;
    case WINKS_PROTOCOL_CREATEWINKSSCRIPT:
        pPackDataX->method = WINKS_METHOD_POST;
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid,
                                  WKPACK_script);
        break;
    case WINKS_PROTOCOL_SETSHARED:
    #if 0 /* "mask WINKS_PROTOCOL_SETSHARED" */
        pPackDataX->method = WINKS_METHOD_POST;
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid,
                                  WKPACK_shared);
    #else
        Winks_fail("mask WINKS_PROTOCOL_SETSHARED");
    #endif
        break;
    case WINKS_PROTOCOL_GETSHARED:
    #if 0 /* "mask WINKS_PROTOCOL_GETSHARED" */
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid,
                                  WKPACK_shared);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETSHARED");
    #endif
        break;
    case WINKS_PROTOCOL_RECOMMENDSERVICE:
        pPackDataX->method = WINKS_METHOD_POST;
        sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user,
                                     uid,
                                     WKPACK_service,
                                     WKPACK_recommend);
        break;
    case WINKS_PROTOCOL_PURCHASEWINKS:
    #if 0 /* "mask WINKS_PROTOCOL_PURCHASEWINKS" */
        pPackDataX->method = WINKS_METHOD_POST;
        sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user,
                                     uid,
                                     WKPACK_winks, 
                                     WKPACK_purchase);
    #else
        Winks_fail("mask WINKS_PROTOCOL_PURCHASEWINKS");
    #endif
        break;
    case WINKS_PROTOCOL_GETUSERWINKS:
    #if 0 /* "mask WINKS_PROTOCOL_GETUSERWINKS" */
        {
        char * wid = NULL;

        Winks_assert(inData);
        wid = (char *)inData;
        sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user,
                                     uid,
                                     WKPACK_winks, 
                                     wid);
        }
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETUSERWINKS");
    #endif
        break;
    case WINKS_PROTOCOL_SHOWWINKS:
    case WINKS_PROTOCOL_SHOWSPECIALWINKS:
        {
        char * cl, * mt;
        
        Winks_assert(inData);
        if(ProtocolNo == WINKS_PROTOCOL_SHOWWINKS)
        {
            char * phoneNumber = NULL;
            phoneNumber = (char *)inData;
            sprintf(Buf, "/%s/%s?%s=%s", WKPACK_winks, 
                                         WKPACK_show, 
                                         WKPACK_p, 
                                         phoneNumber );
            if(Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid)) == 0 && uid[0])
            {
                iRet = strlen(Buf);
                sprintf(&Buf[iRet], "&%s=%s", WKPACK_uid, uid);
            }
            IsQueryMark = 0;
        }
        else
        {
            int wt = *(int *)inData;
            char * pWt;
            
            if(wt == WINKS_SPECIAL_CARRIER)
                pWt = (char *)WKPACK_carrier;
            else
                pWt = (char *)WKPACK_splash;
            if(Winks_GetSettingString(WINKS_SETTING_UID, uid, sizeof(uid)) != 0 || uid[0]==0)
                sprintf(Buf, "/%s/%s", WKPACK_specialwinks,
                                       pWt);
            else
                sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user, 
                                             uid,
                                             WKPACK_specialwinks,
                                             pWt);
        }
        
        cl = WINKS_CL;
        mt = WINKS_MT;
        if(cl != NULL && mt != NULL)
        {
            iRet = strlen(Buf);
            if(IsQueryMark)
            {
                sprintf(&Buf[iRet], "?cl=%s&mt=%s", cl, mt);
                IsQueryMark = 0;
            }
            else
                sprintf(&Buf[iRet], "&cl=%s&mt=%s", cl, mt);
        }
        }
        break;
    case WINKS_PROTOCOL_GETURLFILE:
        {
        char * url = NULL;
        
        pPackDataX->server = WINKS_SRV_DATA;
        NeedCheckUrl = 1;
        
        Winks_assert(inData);
        url = (char *)inData;
        strcpy(Buf, url);
        }
        break;
    case WINKS_PROTOCOL_UPLOADFILE:
        {
        WinksProt_UploadFile_s * pUploadFile = NULL;
        
        pPackDataX->method = WINKS_METHOD_POST;
        pPackDataX->IsOnlyPath = 1;
        
        Winks_assert(inData);
        pUploadFile = (WinksProt_UploadFile_s *)inData;
        
        sprintf(Buf, "/%s/%s", WKPACK_file, 
                               WKPACK_upload);
        iRet = strlen(Buf);
        if(pUploadFile->fileType == WINKS_UPLD_FILETYPE_stat)
            sprintf(&Buf[iRet], "?ft=%s", WKPACK_stat);
        else
            sprintf(&Buf[iRet], "?ft=%s", WKPACK_log);
        
        iRet = strlen(Buf);
        sprintf(&Buf[iRet], "&fn=%d", pUploadFile->fileNumber);

        iRet = strlen(Buf);
        sprintf(&Buf[iRet], "&%s=%s", WKPACK_uid, uid);

        IsQueryMark = 0;
        }
        break;
    case WINKS_PROTOCOL_GETMESSAGE:
    #if 0 /* "mask WINKS_PROTOCOL_GETMESSAGE" */
        sprintf(Buf, "/%s/%s/%s", WKPACK_user, 
                                  uid,
                                  WKPACK_message);
    #else
        Winks_fail("mask WINKS_PROTOCOL_GETMESSAGE");
    #endif
        break;
    case WINKS_PROTOCOL_CONFIRMMESSAGE:
        pPackDataX->method = WINKS_METHOD_POST;
        sprintf(Buf, "/%s/%s/%s/%s", WKPACK_user, 
                                     uid,
                                     WKPACK_message,
                                     WKPACK_confirm);
        break;
    case WINKS_PROTOCOL_GETLATESTSOFTWAREPACKAGE:
        sprintf(Buf, "/%s/%s/%s", WKPACK_software,
                                  WKPACK_winks,
                                  WKPACK_latest);
        break;
    default:
        Winks_fail("Winks_ProtPack error case");
        break;
    }   

    pPackDataX->url = (char *)winks_ProtUrlAlloc(pMemPool, Buf, 
                                            pPackDataX->server, NeedCheckUrl);
    if( pPackDataX->url == NULL ||
        winks_ProtUrlAddVer(pMemPool, pPackDataX->url, IsQueryMark) == NULL )
    {
        Winks_EngMemPoolDestory(pMemPool);
        return -1;
    }

    iRet = 0;
    if(pPackDataX->method == WINKS_METHOD_POST)
    {
        Winks_assert(inData);
        Winks_assert(inLen);
        
        switch(ProtocolNo)
    	{
        case WINKS_PROTOCOL_SETSERVICEMODE:
        #if 1 /* "mask WINKS_PROTOCOL_SETSERVICEMODE" */
            {
                if(winks_ProtSetServiceMode(&(pSetServiceMode->serviceMode), pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
        #else
            Winks_fail("mask WINKS_PROTOCOL_SETSERVICEMODE");
        #endif
            break;
        case WINKS_PROTOCOL_SETUSERCONFIG:
        #if 1 /* "mask WINKS_PROTOCOL_SETUSERCONFIG" */
            {
            if(winks_ProtSetUserConfig(((WinksProt_userConfigEx_s *)inData)->userConfig, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
        #else
            Winks_fail("mask WINKS_PROTOCOL_SETUSERCONFIG");
        #endif
           break;
        case WINKS_PROTOCOL_MERGEPHONEBOOK:
            {
            if(winks_ProtMergePhoneBook((WinksProt_phoneBook_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
            break;
        case WINKS_PROTOCOL_UPLOADWINKS:
            {
            if(winks_ProtUploadWinks((WinksProt_UploadWinks_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
            break;
        case WINKS_PROTOCOL_CREATEWINKSSCRIPT:
            {
            if(winks_ProtCreateWinksScript((WinksProt_winksScript_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
            break;
        case WINKS_PROTOCOL_SETSHARED:
        #if 0 /* "mask WINKS_PROTOCOL_SETSHARED" */
            {
            if(winks_ProtSetShared((WinksProt_shared_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
        #else
            Winks_fail("mask WINKS_PROTOCOL_SETSHARED");
        #endif
            break;
        case WINKS_PROTOCOL_RECOMMENDSERVICE:
            {
            if(winks_ProtRecommendService((WinksProt_phoneBook_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
            break;
        case WINKS_PROTOCOL_PURCHASEWINKS:
        #if 0 /* "mask WINKS_PROTOCOL_PURCHASEWINKS" */
            {
            if(winks_ProtPurchaseWinks((WinksProt_winksPO_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
        #else
            Winks_fail("mask WINKS_PROTOCOL_PURCHASEWINKS");
        #endif
            break;
        case WINKS_PROTOCOL_UPLOADFILE:
            {
            if(winks_ProtUploadFile((WinksProt_UploadFile_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
            break;
        case WINKS_PROTOCOL_CONFIRMMESSAGE:
            {
            if(winks_ProtConfirmMessage((WinksProt_confimMessage_s *)inData, pMemPool, pPackDataX) != 0)
                iRet = -1;
            }
            break;
        default:
            Winks_fail("Winks_ProtPack error case");
            iRet = -1;
            break;
        }
    }
    
    if(iRet == 0)
    {
        pProtPack = winks_ProtGenXml(pPackDataX);
    #ifdef WINKS_DEBUG
        winks_PrintProtPack(pProtPack, ProtocolNo);
    #endif
    }
    Winks_EngMemPoolDestory(pMemPool);
    
    if(pProtPack == NULL)
        iRet = -1;
    
    if(ppProtPack)
        *ppProtPack = pProtPack;

    return iRet;
}

int Winks_ProtPackFree(WinksProt_PackData_s * pProtPack)
{
    if(pProtPack)
        Winks_salFree(pProtPack);

	return 0;
}

/*
激活流程:
    Winks_EngGetServiceConfig
    Winks_EngActivate
    Winks_EngGetUserStatus

上传DIY:
    Winks_EngUploadWinks
    Winks_EngCreateWinksScript

下载用户彩像:
    Winks_EngShowWinks
    Winks_EngGetUrlFile
    
下载片头运营商彩像:
    Winks_EngShowSpecialWinks
    Winks_EngGetUrlFile

推荐彩像:
    Winks_EngRecommendService

加为我的收藏:
    Winks_EngGetUserWinks
    Winks_EngPurchaseWinks
    Winks_EngCreateWinksScript

上传电话簿:
    Winks_EngMergePhoneBook

软件升级:
    Winks_EngSoftwareVersionDetect(1)
    Winks_EngGetLatestSoftwarePackage
*/

