/*
 *  MyLog.h
 *  
 *
 *  Created by michael.wu on 7/2/09
 *  Copyright 2009 Winks. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import "engine/wk_engin.h"

//#define SHOW_WINKS_LOG

FOUNDATION_EXPORT void Winks_printfErr(const char* fmt, ... );
FOUNDATION_EXPORT void Winks_clear_log();
FOUNDATION_EXPORT void CallLog(const char* str);

#ifndef SHOW_WINKS_LOG

#define Winks_printf(...)
//#define Winks_printfErr(...)
//#define CallLog(str)
#define Winks_HEX_DataOut(fragment,length,name)
//#define Winks_clear_log()

#else 


//FOUNDATION_EXPORT void CallLogInt(int str);
FOUNDATION_EXPORT void Winks_printf(const char* fmt, ... );

FOUNDATION_EXPORT void Winks_HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);

//#endif

#if 0
//通过notify 来监听电话事件
FOUNDATION_EXPORT int Winks_AddCoreTelephonyNotify(void);


// header of CoreTelephony
extern NSString* const kCTSMSMessageReceivedNotification;
extern NSString* const kCTSMSMessageReplaceReceivedNotification;
extern NSString* const kCTSIMSupportSIMStatusNotInserted;
extern NSString* const kCTSIMSupportSIMStatusReady;
extern NSString* const kCTCallStatusChangeNotification;

id CTTelephonyCenterGetDefault(void);
void CTTelephonyCenterAddObserver(id,id,CFNotificationCallback,NSString*,void*,int);
void CTTelephonyCenterRemoveObserver(id,id,NSString*,void*);
int CTSMSMessageGetUnreadCount(void);

int CTSMSMessageGetRecordIdentifier(void * msg);
NSString * CTSIMSupportGetSIMStatus();   //获取sim卡状态，kCTSIMSupportSIMStatusNotInserted表示没有sim卡
NSString * CTSIMSupportCopyMobileSubscriberIdentity();  //获取imsi号码，泪牛满面啊，我原来都是用AT+CCID来获取的iccid识别用户

id  CTSMSMessageCreate(void* unknow/*always 0*/,NSString* number,NSString* text);
void * CTSMSMessageCreateReply(void* unknow/*always 0*/,void * forwardTo,NSString* text);

void* CTSMSMessageSend(id server,id msg);

NSString *CTSMSMessageCopyAddress(void *, void *);
NSString *CTSMSMessageCopyText(void *, void *);
#endif
#endif


