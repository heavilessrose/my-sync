//
//  define.h
//  msn_for_mac
//
//  Created by  apple on 08-9-16.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#ifdef _DEBUG_
#define MYLog(...) NSLog(__VA_ARGS__)
#else
#define MYLog(...) do{}while(0)
#endif

#define kAddBuddyKey			@"kAddBuddyKey"
#define kMSGFromWap				@"kMSGFromWap"

#define VOICE_CLIP_FOLDER		(@"voice")
#define VOICE_CLIP_NAME_FORMAT	(@"%04d%02d%02d%02d%02d%02d.wav")

#define CHECK_MESSAGE_KEY		(@"kMessageKey")
#define CHECK_MESSAGE_CONTENTS	(@"kMessageContets")

#define MIN_FREE_MEMORY_SIZE	(3 * 1024 * 1024)

enum MessageType 
{
	CLIENT_REQUEST = 0,
	DISPATCH_REQUEST,
	ACCOUNT_REQUEST,
	ROSTER_REQUEST,
	PRESENCE_REQUEST,
	LOGIN_REQUEST,
	MESSAGE_REQUEST,
	ADVERTISE_REQUEST,
	ADCLICKED_REQUEST,
	ZWP_REQUEST,
	STANDBY_ZWP_REQUEST,
	MCLUB_REQUEST,
	X_H3G_MSISDN,
	H3G_WIFI,
};

typedef enum _SendReceiveStatus
{
		NONE = 0,
		SEND,
		RECEIVE,
} SendReceiveStatus;

#define kStandbyLiveWapVC   @"StandbyLiveWapVC"
#define kSettingsVC			@"SettingsVC"
#define kListChatVC			@"ListChatVC"
#define kChatVC				@"ChatViewController"
#define kLiveWapVC			@"LiveWapViewController"


#define kAccountsKey			(@"accounts")
#define kLoginStatusKey			(@"loginstatus")
#define kLastAccountKey			(@"lastaccount")
#define kAutoLoginKey			(@"autologin")
#define kSavePwdFlagKey			(@"SavePwdFlag")
#define kPasswordKey			(@"password")
#define kAPSDurationKey			(@"APSDuration")
#define kAPSPasswordKey			(@"myKey1")

#define kProfileVersionKey		(@"profile_version")
#define kRosterVersionKey		(@"roster_version")

#define kH3GPhoneNumber     (@"h3g_phone_number")
#define kH3GpHomeTime		(@"h3g_time")

#define kLoginFaildOfUnregisterUserNotify		(@"LoginFaildOfUnregisterUser")

extern	SendReceiveStatus isSendReceive;

extern NSString *kMessageAlertKey;
extern NSString *kOnLineAlertKey;
extern NSString *kNotifyNudgeKey;
extern NSString *kShowBigImageKey;
extern NSString *kDisplayOnLinesKey;
extern NSString *kSaveConversationKey;

extern NSString		*NetWorkH3GNotification;
extern NSString		*NetworkReachabilityChangedNotification;
extern NSString		*FindDispatchNotification;
extern NSString		*ConnectingServerNotification;
extern NSString		*LoggingInNotification;
extern NSString		*GroupHerderClickedNotification;
extern NSString		*GetSystemInfoNotification;
extern NSString		*GetUpgradeInfoNotification;

extern NSString		*ForceUnfoldGroupNotification;

extern NSString		*NetWorkDisabledNotification;

extern NSString		*DeleteAccountNotification;

extern NSString		*LoginActionNotification;
extern NSString		*LoginNotification;
extern NSString		*LogoutNotification;
extern NSString		*LoginFaildNotification;
extern NSString		*H3GLoginFaildNotification;
extern NSString     *H3GWIFITimeNotification;
extern NSString     *H3GWIFIFaildNotification;
extern NSString		*NeedUpdateNotification;
extern NSString		*DismissAlertViewNotification;
extern NSString		*ReloadListNotification;

extern NSString		*TouchInReceivedMessageViewNotification;
extern NSString		*ReceivedErrorNotification;

extern NSString		*DeleteGroupNotification;
extern NSString		*DeletePersonNotification;
extern NSString		*UpdateGroupNotification;
extern NSString		*UpdatePersonNotification;
extern NSString		*PersonStatusChangedNotification;
extern NSString		*PersonAddedNotification;
extern NSString		*ReceiveMessageNotification;
extern NSString		*LocalNameResultNotification;
extern NSString		*ReceiveVoiceNotification;
extern NSString		*ReceiveFileNotification;
extern NSString		*ReceiveActionNotification;
extern NSString		*ReceiveStatusNotification;
extern NSString		*ReceiveDataNotification;

extern NSString		*TransferBeginNotification;
extern NSString		*TransferEndNotification;

extern NSString		*UpdateAuthInfoNotification;
extern NSString		*BeAddedRequestNotification;

extern NSString		*ParticipantChangedNotification;
extern NSString		*ChangeChatviewContactNotification;
extern NSString		*RecorderDataChangedNotification;
extern NSString		*ShowConversationListNotification;
extern NSString		*ShowConversationNotification;
extern NSString		*ShowContactListNotification;
extern NSString		*ShowChatViewNotification;
extern NSString     *ShowUpdateMessageNotification;

extern NSString		*ShowChatViewsNotification;
extern NSString		*ShowZwpViewNotification;
extern NSString		*ShowStandbyZwpViewNotification;
extern NSString		*ZwpDataChangedNotification;
extern NSString		*ZwpTitleChangedNotification;
extern NSString		*StandbyZwpDataChangedNotification;
extern NSString		*StandbyZwpTitleChangedNotification;

extern NSString		*LoginProgAdNotification;
extern NSString		*RosterFooterAdNotification;
extern NSString		*ChatHeaderAdNotification;
extern NSString		*AdImageChangedNotification;

extern NSString		*AddBuddyNotification;
extern NSString		*IMFromWapNotification;

extern NSString		*GroupInfoKey;
extern NSString		*PersonInfoKey;

// mClub
extern NSString		*SayHiViewNotification;
//extern NSString		*mClubInviateNotification;
extern NSString		*SayHiPackageSentNotification;
extern NSString		*MClubOnlineNotification;
extern NSString		*UserFeedArrivalNotification;
extern NSString		*SayHiFailedNotification;
extern NSString		*AddMessageToChatViewNotification;
extern NSString		*HiMessageFromSomebodayNotification;

// Send file object
extern NSString		*CancelSendingNotification;


#define PICA_BUDDY_IMID			{\
"msn.help@live.cn",\
"NULL",\
}

#ifdef VERSION_FOR_SIMPLIFIED
	#define APPLICATION_VERSION		(@"qtn_msn_version_simplify")
#elif defined FOR_PAYMENT_APP
    #define APPLICATION_VERSION     (@"qtn_msn_version_for_sale")
#else
	#define APPLICATION_VERSION		(@"qtn_msn_version")
#endif

enum {
	APS_DURATION_NONE				= 0,
	APS_DURATION_5_MINUTES,
	APS_DURATION_10_MINUTES,
	APS_DURATION_30_MINUTES,
    APS_DURATION_2_HOURS,
    APS_DURATION_6_HOURS,
	APS_DURATION_12_HOURS,
};
