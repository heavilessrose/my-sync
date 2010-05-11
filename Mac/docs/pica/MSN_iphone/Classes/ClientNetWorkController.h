//
//  ClientNetWorkController.h
//  msn_for_mac
//
//  Created by  apple on 08-9-19.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TCPClient;
@class HttpClient;

@class PersonInfo;
@class ZwpResponseParser;
@class NetWorkReachability;

@interface ClientNetWorkController : NSObject 
{
	NetWorkReachability *wifiStatus;
	TCPClient *tcpClient;
	HttpClient *httpClient;
	ZwpResponseParser *zwpParser;
	NSOperationQueue *zwpDataQueue;
    
    NSLock* lock;
    BOOL exitThread;
}
- (void)notifyExitThread: (NSNotification*)aNotify;

+ (id)getInstance;

+ (void)postNotification:(NSString*)note info:(NSDictionary*)theInformation;
- (void)postNotification:(NSDictionary*)dict;
- (NSString*)convertString:(NSString*)theString;
- (void)setMessageType:(int)type;

- (void) localDispatcher:(NSData *) theData;

- (void)callBackStream:(NSData*)theData dictionary:(NSDictionary*)theDict;
- (void)callBackStream:(NSData*)theData messageType:(NSNumber*)theType;
- (void)setTextfieldText:(NSData*)theData;

- (void)sendClientRequest:(NSString*)data;
- (void)sendPortraitRequest:(NSString*)jid;
- (void)sendLocalNameRequest:(NSDictionary *)infoDict;
- (void)sendPresenceRequest:(NSString*)state;
- (void)sendVcardRequest:(PersonInfo*)info;
- (void)sendAddContactRequest:(NSString *)theContact;
- (void)sendAnswerStrangerRequest:(NSString *)theContact add: (BOOL)anAdd;
- (void)sendDeleteContactRequest:(NSString*)theContact block: (BOOL)aBlock;
- (void)sendBlockContactRequest:(NSString*)theContact block: (BOOL)aBlock;
- (void)sendAddParticipantRequest:(NSString*)theJid jids:(NSArray*)theJids sid:(NSString*)theSid contact:(BOOL)known;
- (void)sendAddParticipantRequest:(NSArray*)theJids sid:(NSString*)theSid;
- (void)sendMessageTo:(NSString*)theJid message:(NSString *)theMessage sid:(NSString*)theSid contact:(BOOL)known;
- (void)sendMessage:(NSString *)theMessage sid:(NSString*)theSid;
- (void)sendTransferInviteRequest:(NSString*)theJid sid:(NSString*)theSid 
							 ftid:(NSString*)theFtid name:(NSString*)theName size:(int)theSize;

- (void)sendTransferInviteResponse:(NSString*)transferid sid:(NSString*)theSid 
							  ftid:(NSString*)theFtid accept:(BOOL)accept;

- (void)sendTransferData:(NSString*)transferid sid:(NSString*)theSid ftid:(NSString*)theFtid
				   seqid:(NSString*)theSeqid totalLength:(int)theTotalLength startPos:(int)theStartPos 
				  endPos:(int)theEndPos currentLength:(int)theCurrentLength data:(NSString*)theData;

- (void)sendVoiceClip:(NSString*)theJid sid:(NSString*)theSid totalLength:(int)theTotalLength 
		 base64Length:(int)theBase64Length data:(NSString*)theData url:(NSString*)theURL;

- (void)sendContactsNickNameToServer;
- (void)sendOfflineMsgRequest;
- (void)sendAPSInfoToServer;
- (void)sendUpdatePortrait;

- (void)sendDataCompleteRequest:(NSString*)theTransferid sid:(NSString*)theSid 
						   ftid:(NSString*)theFtid url:theURL;
- (void)sendDataReceiveAck:(NSString*)theTransferid sid:(NSString*)theSid seqid: (NSString*)aSeqid ftid:(NSString*)theFtid;
- (void)sendCancelTransferRequest:(NSString*)theTransferid sid:(NSString*)theSid ftid:(NSString*)theFtid;

- (void)sendEndDialogRequest:(NSString*)theSid;
- (void)sendLogOutRequest;

- (void)sendAdviceRequest:(NSString*)theUrl;
- (void)sendAdviceClickedRequest:(NSString*)content;

- (void)sendZwpRequest:(NSString*)theUrl 
			   headers:(NSDictionary*)theHeaders 
				  body:(NSData*)theBody 
				method:(NSString*)theMethod 
			   session:(NSString*)theSession
				  type:(int)theType;
-(void)sendH3GClientRequest:(int)theType;
-(void)sendH3GWIFIRequest:(int)theType;

#pragma mark mClub related.
//- (void) sendMClubUserPresence;
- (void) getFeedForUser:(NSString *)liveID count:(int)theCount;
- (void)praseMClubResponseData:(NSString *)theString;
- (void) sendHiPackage:(NSDictionary *)infoDict;
- (void) sendMClubInvitePackage:(NSDictionary *)infoDict;

@property (nonatomic, readonly) NetWorkReachability *wifiStatus;
@property (nonatomic, retain) TCPClient *tcpClient;
@property (nonatomic, retain) HttpClient *httpClient;
@end
