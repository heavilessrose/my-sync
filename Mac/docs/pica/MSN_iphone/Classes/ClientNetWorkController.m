//
//  ClientNetWorkController.m
//  msn_for_mac
//
//  Created by  apple on 08-9-19.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ClientNetWorkController.h"
#import "MSNAppDelegate.h"
#import "HttpClient.h"
#import "TcpClient.h"
#import "NetWorkReachability.h"
#import "ClientResponseParser.h"
#import "AccountResponseParser.h"
#import "PresenceResponseParser.h"
#import "LoginResponseParser.h"
#import "MessageParser.h"
#import "AdvtResponseParser.h"
#import "ZwpResponseParser.h"
#import "PersonInfo.h"
#import "define.h"
#import "API.h"
#import "DDXML.h"
#import "ContactList.h"

@interface ClientNetWorkController (PrivateMethods)

- (void)sendLoginRequest;
- (void)sendDisPatchRequest;
- (void)sendAccountRequest;

- (void)praseClientResponseData:(NSData*)theData;
- (void)praseAccountResponseData:(NSData*)theData;
- (void)prasePresenceResponseData:(NSData*)theData;
- (void)praseLoginResponseData:(NSData*)theData;
- (void)praseMessageResponseData:(NSData*)theData;
- (void)praseAdvertiseResponseData:(NSData*)theData;
- (void)praseZwpResponseData:(NSData*)theData headers:(NSDictionary *)theHeaders type:(int)theType;
- (void)praseZwpData:(NSData*)theData;
- (void)praseZwpHeaders:(NSDictionary*)theHeaders;
- (void)praseH3GClientResponseData:(NSData*)theData;
- (void)praseH3GWIFIResponseData:(NSData*)theData;

//- (void)praseOfflineMsg:(NSData*)theData;

- (NSString*)numericString:(NSString*) aString;

@end

NSString *NetWorkH3GNotification = @"H3gNotification";
NSString *LoginFaildNotification = @"LoginFaild";
NSString *H3GLoginFaildNotification = @"H3GLoginFaild";
NSString *H3GWIFITimeNotification = @"H3GWIFITime";
NSString *H3GWIFIFaildNotification = @"H3GWIFIFaild";
NSString *NeedUpdateNotification = @"NeedUpdate";
NSString *ReceivedErrorNotification = @"ReceivedError";
NSString *FindDispatchNotification = @"FindDispatch";
NSString *ConnectingServerNotification = @"ConnectingServer";
NSString *LoggingInNotification = @"LoggingIn";

NSString *UserFeedArrivalNotification = @"UserFeedArrivalNotification";
NSString *SayHiFailedNotification = @"SayHiFailedNotification";
NSString *HiMessageFromSomebodayNotification = @"HiMessageFromSomebodayNotification";


static ClientNetWorkController *clientNetWorkController = nil;

@implementation ClientNetWorkController
@synthesize tcpClient, httpClient, wifiStatus;

- (id) init
{
	self = [super init];
	if (self != nil) 
	{	
		tcpClient = nil;
		httpClient = nil;
		wifiStatus = [[NetWorkReachability alloc] init];
        
        lock = [[NSLock alloc] init];
        exitThread = YES;
        
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector(notifyExitThread:)
                                                     name: @"ExitThreadNotification"
                                                   object: nil];
	}
	return self;
}

- (void)notifyExitThread: (NSNotification*)aNotify{
    [lock lock];
    exitThread = YES;
    [lock unlock];
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver: self
                                                    name: @"ExitThreadNotification"
                                                  object: nil];
    [lock release];
    
	tcpClient.delegate = nil;
	tcpClient.callbackFun = nil;
	[tcpClient release];
	httpClient.delegate = nil;
	httpClient.callbackFun = nil;
	[httpClient release];
	[zwpDataQueue cancelAllOperations];
	[zwpDataQueue release];
	[zwpParser release];
	if (clientNetWorkController == self)
		clientNetWorkController = nil;
//	MYLog(@"-----------------%@ dealloc----------------", self);
	[super dealloc];
}


+ (id)getInstance
{
	if (clientNetWorkController == nil)
	{
		clientNetWorkController = [[[ClientNetWorkController alloc] init] autorelease];
	}
	return clientNetWorkController;
}


+ (void)postNotification:(NSString*)note info:(NSDictionary*)theInformation
{
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	if (note != nil)
		[dict setObject:note forKey:@"note"];
	if (theInformation != nil)
		[dict setObject:theInformation forKey:@"information"];
	
	[clientNetWorkController performSelectorOnMainThread:@selector(postNotification:) withObject:dict waitUntilDone:YES];
}

- (void)postNotification:(NSDictionary*)dict
{
	NSString *note = [dict objectForKey:@"note"];
	NSDictionary *info = [dict objectForKey:@"information"];
	[[NSNotificationCenter defaultCenter] postNotificationName:note object:clientNetWorkController userInfo:info];
}


- (NSString*)convertString:(NSString*)theString
{
	theString = [theString stringByReplacingOccurrencesOfString:@"&" withString:@"&amp;"];
	theString = [theString stringByReplacingOccurrencesOfString:@"'" withString:@"&apos;"];
	theString = [theString stringByReplacingOccurrencesOfString:@"\"" withString:@"&quot;"];
	theString = [theString stringByReplacingOccurrencesOfString:@"<" withString:@"&lt;"];
	theString = [theString stringByReplacingOccurrencesOfString:@">" withString:@"&gt;"];
	return theString;
}

- (void)callBackStream:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	NSNumber *theType = [theDict objectForKey:@"messageType"];
	NSDictionary *headers = nil;
	
	switch ([theType intValue]) {
		case CLIENT_REQUEST:
			[self praseClientResponseData:theData];
			break;
		case ADVERTISE_REQUEST:
			[self praseAdvertiseResponseData:theData];
			break;
		case ZWP_REQUEST:
			headers = [theDict objectForKey:@"headers"];
			[self praseZwpResponseData:theData headers:headers type:ZWP_REQUEST];
			break;
		case STANDBY_ZWP_REQUEST:
			headers = [theDict objectForKey:@"headers"];
			[self praseZwpResponseData:theData headers:headers type:STANDBY_ZWP_REQUEST];
			break;
		case X_H3G_MSISDN:
			[self praseH3GClientResponseData:theData];
			break;
		case H3G_WIFI:
			[self praseH3GWIFIResponseData:theData];
			break;
		default:
			MYLog(@">>>[unknown type]:%d",theType);
			break;
	}
}

- (void)callBackStream:(NSData*)theData messageType:(NSNumber*)theType;
{
//	[self performSelectorOnMainThread:@selector(setTextfieldText:) withObject:theData waitUntilDone:YES];
//	NSString *theString = [[[NSString alloc] initWithData:theData encoding:NSUTF8StringEncoding] autorelease];
	
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
	switch ([theType intValue]) {
//		case CLIENT_REQUEST:
//			[self praseClientResponseData:theData];
//			MYLog(@"CLIENT_REQUEST:%@",theString);
//			break;
		case DISPATCH_REQUEST:
			[self performSelectorOnMainThread:@selector(sendAccountRequest) withObject:nil waitUntilDone:YES];
			break;
		case ACCOUNT_REQUEST:
			[self praseAccountResponseData:theData];
			break;
		case ROSTER_REQUEST:
			[self performSelectorOnMainThread:@selector(sendPresenceRequest) withObject:nil waitUntilDone:YES];
			break;
		case PRESENCE_REQUEST:
			[self prasePresenceResponseData:theData];
			break;
		case LOGIN_REQUEST:
			[self praseLoginResponseData:theData];
			break;
		case MESSAGE_REQUEST:
			[self praseMessageResponseData:theData];
			break;
		case MCLUB_REQUEST:
			[self localDispatcher:theData];
			break;
		default:
			break;
	}
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];
}

- (void)praseClientResponseData:(NSData*)theData
{
    NSLog(@"response: \n %@", [NSString stringWithUTF8String:(const char *)[theData bytes]]);
	NSError *error = nil;
	NSNumber* aNumber = nil;
	ClientResponseParser *parser = [[ClientResponseParser alloc] initWithData:theData];
	NSInteger result = [parser praseResponseData:&error];

	if (error == nil)
	{
		aNumber = [NSNumber numberWithInt: result];
		if (result == 0)
		{
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
								  parser.systemProperties, @"system", aNumber, @"myErrCode", nil];
			[ClientNetWorkController postNotification:GetSystemInfoNotification info: dict];
		}
		else if (result == 1)
		{
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
								  parser.systemProperties, @"system", aNumber, @"myErrCode", nil];
			if (parser.systemProperties != nil){
				[parser.systemProperties addObject: parser.upgradeProperties];
			}
			else {
				parser.systemProperties = [[NSMutableArray alloc] init];
				[parser.systemProperties addObject: parser.upgradeProperties];
			}
			dict = [NSDictionary dictionaryWithObjectsAndKeys: parser.systemProperties, @"message", aNumber, @"myErrCode", nil];
			[ClientNetWorkController postNotification:NeedUpdateNotification info:dict];
		}
		else if (result == 2)
		{
			if (parser.systemProperties != nil) {
				if (parser.upgradeProperties != nil){
					[parser.systemProperties addObject: parser.upgradeProperties];
				}
				NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys: parser.systemProperties, @"system", aNumber, @"myErrCode", nil];
				[ClientNetWorkController postNotification:GetSystemInfoNotification info: dict];
			}
			else if (parser.upgradeProperties != nil){
				NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys: parser.upgradeProperties, @"upgrade", aNumber, @"myErrCode", nil];
				[ClientNetWorkController postNotification:GetUpgradeInfoNotification info:dict];
			}
			else {
				[self performSelectorOnMainThread:@selector(sendDisPatchRequest) withObject:nil waitUntilDone:YES];
			}
		}
		else if (result == 4){
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys: parser.systemProperties, @"system", aNumber, @"myErrCode", nil];
			[ClientNetWorkController postNotification: kLoginFaildOfUnregisterUserNotify info: dict];
		}
		else {
			MYLog(@"unknown result type(result : %@)", aNumber);
            [ClientNetWorkController postNotification:LogoutNotification info:nil];
        }
	}
	else
		MYLog(@"theType = 0, parser error:%@",[error description]);
	[parser release];
}

- (void)praseH3GClientResponseData:(NSData*)theData
{
	if(theData == nil)
	{
		[self sendClientRequest: nil];
	}
	else
	{		
		NSString* aStr = [[NSString alloc] initWithData: theData
											   encoding: NSASCIIStringEncoding];
		[self sendClientRequest: [self numericString: aStr]];
		[aStr release];
	}
}

- (void)praseH3GWIFIResponseData:(NSData*)theData
{
	if(theData == nil)
	{
		[self sendH3GClientRequest: X_H3G_MSISDN];
	}
	else
	{		
		NSString* aStr = [[NSString alloc] initWithData: theData
											   encoding: NSASCIIStringEncoding];
		[[NSNotificationCenter defaultCenter] postNotificationName:H3GWIFITimeNotification
															object:aStr
															];
		[aStr release];
	}
}


#pragma mark locaDispatcher

- (void) localDispatcher:(NSData *) theData
{
	NSString *theString = [[[NSString alloc] initWithData:theData encoding:NSUTF8StringEncoding] autorelease];
	NSError *error = nil;

	DDXMLElement *theElement = [[[DDXMLElement alloc] initWithXMLString:theString error:&error] autorelease];
	DDXMLElement *theQuery = [[theElement elementsForName:@"query"] objectAtIndex:0];
	NSString *theNameSpace = [[theQuery elementsForName:@"xmlns"] objectAtIndex:0];
	
	if ([theNameSpace compare:@"iq"] == NSOrderedSame)
	{
		if ([[[theElement attributeForName:@"from"] stringValue] compare:@"mclub.pica"] == NSOrderedSame)
		{
			// mclub messages.
		}else {
			[self praseMessageResponseData:theData];
		}
	}
	return;
}

#pragma mark mClub
- (void)praseMClubResponseData:(NSString *)theString
{
	NSError *error = nil;
	NSArray *items = nil;

	DDXMLElement *iq = [[[DDXMLElement alloc] initWithXMLString:theString error:&error] autorelease];
	NSString *theType = [[iq attributeForName:@"type"] stringValue];
	DDXMLElement *theQuery = nil;
	theQuery = [[iq elementsForName:@"query"] objectAtIndex:0];
	NSString* theNS = [[[theQuery namespaces] objectAtIndex:0] stringValue];
	
	if ([theType compare:@"error"] == NSOrderedSame)
	{
		MYLog(@">>>Feed error");
		MYLog(@">>>iq content:%@",[iq stringValue]);
		NSMutableDictionary * dict = [NSMutableDictionary dictionaryWithCapacity:0];
		[dict setObject:[NSNumber numberWithInt:400] forKey:@"errorcode"];		
		[ClientNetWorkController postNotification:ReceivedErrorNotification info:dict];
		return;
	}else if([theType compare:@"set"] == NSOrderedSame) {
		if([theNS compare:@"mclub:invite"] == NSOrderedSame)
		{
			DDXMLElement *theItem = [[theQuery elementsForName:@"item"] objectAtIndex:0];
			NSString *fromIMID	= [[theItem attributeForName:@"imid"] stringValue];
			NSString *theString = [NSString stringWithFormat:NSLocalizedString(@"somebodyInviteYou",nil),fromIMID];
			
			MYLog(@">>>%@ invited you",theString);
			
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" 
															message:theString
														   delegate:self 
												  cancelButtonTitle:NSLocalizedString(@"Dismiss",nil)
												  otherButtonTitles:NSLocalizedString(@"qtn_ok",nil), nil];
			[alert show];	
			[alert release];
			return;
		}

		if([theNS compare:@"mclub:notify"] == NSOrderedSame)
		{
			// Hi from somebody.
			MYLog(@"hi msg:%@",theString);
			theQuery = [[iq elementsForName:@"query"] objectAtIndex:0];
			DDXMLElement *theItem = [[theQuery elementsForName:@"item"] objectAtIndex:0];
			
			NSString *fromIMID	= [[theItem attributeForName:@"imid"] stringValue];
			NSString *theType	= [[theItem attributeForName:@"type"] stringValue];
			theType = [[theType lowercaseString] stringByAppendingString:@".png"];
			NSString *isPrivate = [[theItem attributeForName:@"isprivate"] stringValue];
			NSString *theBody   = [[[theItem elementsForName:@"body"] objectAtIndex:0] stringValue];
			
			NSDictionary *theDict = [NSDictionary dictionaryWithObjectsAndKeys:fromIMID,@"kFromIMID",theType,@"kHiImageID",\
									 isPrivate,@"kHiFromIsPrivate",theBody,@"kHiFromBody",\
									 @"chat",@"type",\
									 fromIMID,@"imid",\
									 [NSString stringWithFormat:@"j-%d",random()],@"sid",\
									 theBody,@"message",\
									 nil];
			
			[[NSNotificationCenter defaultCenter] postNotificationName:ReceiveMessageNotification
																object:nil
															  userInfo:theDict];
			return;		
		}
	}


	if (theNS == nil)
	{
		LogMethod();
		MYLog(@">>>FATAL.");
		return;
	}
		
	if ([theNS compare:@"mclub:feed"] == NSOrderedSame)
	{
		items = [[[theQuery elementsForName:@"result"] objectAtIndex:0]\
				 elementsForName:@"item"];		
		if (items == nil)
			return;
		[[NSNotificationCenter defaultCenter] postNotificationName:UserFeedArrivalNotification
															object:nil
														  userInfo:(id)items];
		return;
	}else if ([theNS compare:@"mclub:notify"] == NSOrderedSame){
		MYLog(@">>>notify:%@",theString);
		[[NSNotificationCenter defaultCenter] postNotificationName:SayHiFailedNotification object:nil];

		 NSArray *hiArray = [NSArray arrayWithObjects:@"HI, dear~~~\\*^o^*//",
							 @"代表月亮惩罚你,忏悔,赶紧的! ＜( ￣＾￣)",
							 @"我要抱抱我要抱抱\(￣)￣)/",
							 @"抛个媚眼,秋天的菠菜三毛一斤,要嘛? ~w_w~",
							 @"奉送香吻一枚,小心轻放╮（╯◇╰）╭",
							 @"捅你一下╰（￣▽￣）╭",
							 @"苏格兰调情，调戏下~(￣▽￣)~ ",
							 @"严肃点，我们这儿打劫呢（＞﹏＜）",
							 @"请叫我内裤超人,wakaka *@_@*",
							 @"人生就一个字，囧/(⊙＿⊙)\\’’ ",
							 @"雷倒人不犯法(@口@)",
							 @"就摸你，我高兴(^ω^)",
							 @"别摸我，我害羞~*.*~",
							 @"五指搧到，是长白搧（＞﹏＜）",
							 @"小妞，和大爷啵一个(*^?^*)",
							 @"请问是如花MM嘛（*>.<*）",nil];
		
		DDXMLElement *theItem = [[theQuery elementsForName:@"item"] objectAtIndex:0];
		DDXMLNode    *theIMID = [theItem attributeForName:@"imid"];
		DDXMLNode    *theNo = [theItem attributeForName:@"type"]; // sth like m03.png.
		
		NSString *sNo = [[theNo stringValue] substringWithRange:NSMakeRange(1, 2)];
		NSInteger theIndex = [sNo integerValue] - 1;
		
		NSString *stringIMID = [theIMID stringValue];
		MSNAppDelegate *theApp = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
		NSString *theMessage = [NSString stringWithFormat:@"(G)%@正通过手机MSN向你打了个招呼：\n	%@\n 请通过http://mobile.msn.com.cn下载手机MSN",\
										theApp.myAccount,[hiArray objectAtIndex:theIndex]];
		
		if (stringIMID != nil)
		{
			[self sendMessageTo:stringIMID message:theMessage sid:stringIMID contact:NO];
		}

	}else if ([theNS compare:@"mclub:invite"] == NSOrderedSame){
		// Invite failed.
		DDXMLElement *theItem = [[theQuery elementsForName:@"item"] objectAtIndex:0];
		NSString *imid = [[theItem attributeForName:@"imid"] stringValue];
		if (imid != nil)
		{
			[self sendMessageTo:imid message:@"请求加为mClub好友." sid:imid contact:NO];
		}
	}else{
		MYLog(@">>>theString:%@",theString);
		MYLog(@">>>theNS:%@",theNS);
	}
}

#pragma mark -
- (void)praseAccountResponseData:(NSData*)theData
{
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
	
	AccountResponseParser *parser = [[AccountResponseParser alloc] initWithData:theData];
	NSError *error = nil;
	NSString *result = [parser praseResponseData:&error];
    
	if (error == nil)
	{
		if (result != nil)
		{
			if ([result compare:@"error"] == NSOrderedSame)
			{
				NSMutableDictionary * dict = [NSMutableDictionary dictionaryWithCapacity:0];
				if (parser.errorString != nil)
					[dict setObject:parser.errorString forKey:@"message"];
				[dict setObject:[NSNumber numberWithInt:parser.errorCode] forKey:@"errorcode"];		
				[ClientNetWorkController postNotification:ReceivedErrorNotification info:dict];
			}
			else if ([result compare:@"result"] == NSOrderedSame)
			{
                [self performSelectorOnMainThread:@selector(sendPresenceRequest) withObject:nil waitUntilDone:YES];
			}
			else{
				MYLog(@"unknown result type : %@", result);	
                [ClientNetWorkController postNotification:LogoutNotification info:nil];
            }
		}
		else{
			MYLog(@"result = nil");
            [ClientNetWorkController postNotification:LogoutNotification info:nil];
        }
	}
	else{
		MYLog(@"parser error:%@",[error description]);
        [ClientNetWorkController postNotification:LogoutNotification info:nil];
    }
	[parser release];
	
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];
}


- (void)prasePresenceResponseData:(NSData*)theData
{
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
	
	PresenceResponseParser *parser = [[PresenceResponseParser alloc] initWithData: theData];
	NSError *error = nil;
    NSString *type = [parser praseResponseData:&error];
//    NSString* aString = [[NSString alloc] initWithData: theData encoding: NSASCIIStringEncoding];
//    NSLog(@"type : %@ error : %@ theData : %@", type, [error localizedDescription], aString);
//    [aString release];
    
    if (error != nil){
        [parser release];
        NSDictionary *info = nil;
        info = [NSDictionary dictionaryWithObject: NSLocalizedString(@"Connection timeout.",nil) forKey:@"message"];
        [ClientNetWorkController postNotification: H3GLoginFaildNotification info: info];
        [[UIApplication sharedApplication] endIgnoringInteractionEvents];
        return;
    }
    if ([type isEqualToString: @"available"]){
        [self performSelectorOnMainThread: @selector(sendLoginRequest) withObject:nil waitUntilDone:YES];
    }
    else {
        [ClientNetWorkController postNotification: LogoutNotification info:nil];
    }
	[parser release];
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];
}

- (void)praseLoginResponseData:(NSData*)theData
{
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
	
	if (theData != nil)
	{
		if ([theData length] > 1)
		{
			NSMutableData *data = [NSMutableData data];
			[data appendData:[@"<queue>" dataUsingEncoding:NSUTF8StringEncoding]];
			[data appendData:theData];
			[data appendData:[@"</queue>" dataUsingEncoding:NSUTF8StringEncoding]];
			LoginResponseParser *parser = [[LoginResponseParser alloc] initWithData:data];
			NSError *error = nil;
			NSString *result = [parser praseResponseData:&error];
			
			if (error == nil)
			{
				if (result != nil)
				{
					if ([result compare:@"error"] == NSOrderedSame)
					{
						NSMutableDictionary * dict = [NSMutableDictionary dictionaryWithCapacity:0];
						if (parser.errorString != nil)
							[dict setObject:parser.errorString forKey:@"message"];
						[dict setObject:[NSNumber numberWithInt:parser.errorCode] forKey:@"errorcode"];		
						[ClientNetWorkController postNotification:ReceivedErrorNotification info:dict];
					}
				}
			}
			else
				MYLog(@"parser error:%@",[error description]);	
			
			[parser release];
		}	
	}
	
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];

}

- (void)praseMessageResponseData:(NSData*)theData
{
	[[UIApplication sharedApplication] beginIgnoringInteractionEvents];
	
	if (theData != nil)
	{
		if ([theData length] > 1)
		{
			NSMutableData *data = [NSMutableData data];
			[data appendData:[@"<queue>" dataUsingEncoding:NSUTF8StringEncoding]];
			[data appendData:theData];
			[data appendData:[@"</queue>" dataUsingEncoding:NSUTF8StringEncoding]];
            
			MessageParser *parser = [[MessageParser alloc] initWithData:data];
			NSError *error = nil;
			NSString *result = [parser praseResponseData:&error];
			if (error == nil)
			{
				if (result != nil)
				{
					if ([result compare:@"error"] == NSOrderedSame)
					{
						NSMutableDictionary * dict = [NSMutableDictionary dictionaryWithCapacity:0];
						if (parser.errorString != nil)
							[dict setObject:parser.errorString forKey:@"message"];
						[dict setObject:[NSNumber numberWithInt:parser.errorCode] forKey:@"errorcode"];		
						[ClientNetWorkController postNotification:ReceivedErrorNotification info:dict];
					}	
				}
			}
			else
				MYLog(@"parser error:%@",[error description]);	
			[parser release];
		}
	}
	[[UIApplication sharedApplication] endIgnoringInteractionEvents];
}

- (void)praseAdvertiseResponseData:(NSData*)theData
{
	if (theData != nil && [theData length] > 0)
	{
		AdvtResponseParser *parser = [[AdvtResponseParser alloc] initWithData:theData];
		NSError *error = nil;
		NSString *result = [parser praseResponseData:&error];
		if (error == nil)
		{
			if (result != nil)
			{
				MYLog(@"parser result:%@",result);	
			}
		}
		else
			MYLog(@"parser error:%@",[error description]);	
		[parser release];
	}
}

- (void)praseZwpResponseData:(NSData*)theData headers:(NSDictionary *)theHeaders type:(int)theType
{
	if (zwpParser == nil)
	{
		zwpParser = [[ZwpResponseParser alloc] initWithType:theType];
	}else {
		zwpParser.requestFrom = theType;
	}

	
	NSInvocationOperation *option = nil;
	
	if (theHeaders == nil)
		option = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(praseZwpData:) object:theData];
	else
		option = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(praseZwpHeaders:) object:theHeaders];
	
	if (zwpDataQueue == nil)
	{
		zwpDataQueue = [[NSOperationQueue alloc] init];
		[zwpDataQueue setMaxConcurrentOperationCount:1];
	}
	[zwpDataQueue addOperation:option];
	[option release];
}

- (void)praseZwpData:(NSData*)theData
{
	if (theData != nil)
	{
		if ([theData length] > 0)
		{
			[zwpParser setData:theData];
			NSError *error = nil;
			NSString *result = [zwpParser praseResponseData:&error];
			if (error == nil)
			{
				if (result != nil)
				{
					MYLog(@"parser result:%@",result);	
				}
			}
			else
				MYLog(@"parser error:%@",[error description]);		
		}
	}
}

- (void)praseZwpHeaders:(NSDictionary*)theHeaders
{
	if (theHeaders != nil)
	{
		[zwpParser setZwpHeaders:theHeaders];
	}
}


- (void)setTextfieldText:(NSData*)theData{
}

- (void)setMessageType:(int)type
{
	if (tcpClient != nil)
	{
		tcpClient.messageType = type;
	}
}

#pragma mark -
#pragma mark Requests.

#pragma mark mClub actions.
- (void) getFeedForUser:(NSString *)liveID count:(int)theCount
{
	NSString *command = [NSString stringWithFormat:@"<iq type='get' to='mclub.pica'>\
						 <query xmlns='mclub:feed' action='getfeed'>\
						 <item imid=\"%@\" count=\"%d\"/>\
						 </query>\
						 </iq>",liveID,theCount];
	
	[tcpClient writeString:command messageType:tcpClient.messageType];
}

- (void) sendMClubUserPresence
{
	NSString *command = @"<presence type='available' show='online' to='mclub.pica'/>";
	[tcpClient writeString:command messageType:PRESENCE_REQUEST];
}

- (void) sendMClubInvitePackage:(NSDictionary *)infoDict
{
	LogMethod();
	NSString *sTheMan = [infoDict objectForKey:@"liveID"];
	NSString *command = [NSString stringWithFormat:@"<iq type='set' to='mclub.pica'><query xmlns='mclub:invite' action='invite'><item imid=\"%@\"/></query></iq>",sTheMan];
	[tcpClient writeString:command messageType:tcpClient.messageType];
}

- (void) sendHiPackage:(NSDictionary *)infoDict
{
	NSString *liveID = [infoDict objectForKey:@"liveID"];
	NSInteger theID  = [[infoDict objectForKey:@"tag"] intValue];
	NSString *imageID = nil;
//	NSString *fromLiveID = [infoDict objectForKey:@"fromLiveID"];
	
	if (theID <10){
		imageID = [NSString stringWithFormat:@"M0%d",theID];
	}else {
		imageID = [NSString stringWithFormat:@"M%d",theID];
	}

	NSArray *hiArray = [NSArray arrayWithObjects:@"HI, dear~~~",
												@"代表月亮惩罚你,忏悔,赶紧的! ",
												@"我要抱抱我要抱抱",
												@"抛个媚眼,秋天的菠菜三毛一斤,要嘛?",
												@"奉送香吻一枚,小心轻放",
												@"捅你一下",
												@"苏格兰调情，调戏下",
												@"严肃点，我们这儿打劫呢",
												@"请叫我内裤超人,wakaka",
												@"人生就一个字，囧",
												@"雷倒人不犯法",
												@"就摸你，我高兴",
												@"别摸我，我害羞",
												@"五指搧到，是长白搧",
												@"小妞，和大爷啵一个",
												@"请问是如花MM嘛",nil];

	NSString *isPrivate = @"0";  // Default value.
	NSString *body = [hiArray objectAtIndex:theID-1];
	NSString *command = [NSString stringWithFormat:\
						 @"<iq type='set' to='mclub.pica'><query xmlns='mclub:notify' action='notify'><item imid=\"%@\" type =\"%@\" isprivate=\"%@\"><body>%@</body></item></query></iq>",
						 liveID,imageID,isPrivate,body];
	MYLog(@">>>command:%@",command);
	[tcpClient writeString:command messageType:tcpClient.messageType];
	
	[[NSNotificationCenter defaultCenter] postNotificationName:SayHiPackageSentNotification object:nil];
	
	imageID = [[imageID lowercaseString] stringByAppendingString:@".png"];
	NSDictionary *theDict = [NSDictionary dictionaryWithObjectsAndKeys:body,@"kMessage",@"send",@"kStatus",imageID,@"kHiImageID",\
																		liveID,@"kToLiveID",nil];
	[[NSNotificationCenter defaultCenter] postNotificationName:AddMessageToChatViewNotification
														object:nil
													  userInfo:theDict];
}

#pragma mark -
//20100310 香港版本加入WIFI查询功能
- (void)sendH3GWIFIRequest:(int)theType
{
	if (httpClient == nil)
	{
		self.httpClient = [HttpClient getInstance];
		httpClient.callbackFun = @selector(callBackStream:dictionary:);
		httpClient.delegate = self;	
	}
	NSDictionary *headers = [NSDictionary dictionaryWithObject:@"text/plain" forKey:@"Accept"];
	MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	NSString *account = appdelegate.myAccount;
	if (account != nil)
	{
		NSURL * url = [NSURL URLWithString :@"http://211.99.191.23:8800/Time"];
		[httpClient addHttpRequest:url
							method:@"GET"
						   headers:headers
						bodyString:nil
							  type:theType];
	}
}


- (void)sendH3GClientRequest:(int)theType
{
	if (httpClient == nil)
	{
		self.httpClient = [HttpClient getInstance];
		httpClient.callbackFun = @selector(callBackStream:dictionary:);
		httpClient.delegate = self;	
	}
	NSDictionary *headers = [NSDictionary dictionaryWithObject:@"text/plain" forKey:@"Accept"];
	MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	NSString *account = appdelegate.myAccount;
	if (account != nil)
	{
		// Just for debuging purpose.
		//get iphone number url
		NSURL * url = [NSURL URLWithString: @"http://m1.three.com.hk/iphone_chk_msisdn"];
		//test url
//		NSURL * url = [NSURL URLWithString: @"http://211.99.191.239:8030/EncodeServlet"];

		[httpClient addHttpRequest: url
							method: @"GET"
						   headers: headers
						bodyString: nil
							  type: theType];
	}
}
 
- (void)sendClientRequest:(NSString*)data
{
	if (httpClient == nil)
	{
		self.httpClient = [HttpClient getInstance];
		httpClient.callbackFun = @selector(callBackStream:dictionary:);
		httpClient.delegate = self;	
	}
	NSDictionary *headers;
	if(data == nil)
	{
		headers = [NSDictionary dictionaryWithObject:@"text/plain" forKey:@"Accept"];
	}
	else
	{
		headers = [NSDictionary dictionaryWithObjectsAndKeys: @"text/plain", @"Accept", data, @"X-H3G-MSISDN", data, @"pica-threehk-msisdn", nil];		
	}
	MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	NSString *account = appdelegate.myAccount;
	if(data != nil)
	{
		appdelegate.h3g_phone_number = data;
	}
	if (account != nil)
	{
		// Just for debuging purpose.
		//for test
//		NSURL * url = [[NSURL alloc] initWithString:@"http://211.99.191.239:32246/dispatch.xml"];//disp1.msn.pica.com:22246
		//for dispatch
		NSURL * url = [[NSURL alloc] initWithString:@"http://disp1.msn.pica.com:22246/dispatch.xml"];
		NSString *bodyString = [NSString stringWithFormat: @"imid=%@&version=%@&source=msn&type=s",
															account, NSLocalizedString(APPLICATION_VERSION,nil)];
		NSData *bodyData = [NSData dataWithBytes:[bodyString UTF8String] length:[bodyString length]];
		[httpClient addHttpRequest:url
							method:@"POST"
						   headers:headers
						bodyString:bodyData];
		[url release];
	}
}


- (void)sendDisPatchRequest
{
	if (tcpClient == nil)
	{
		self.tcpClient = [TCPClient getInstance];
		[tcpClient reset];
		
		tcpClient.delegate = self;
		tcpClient.callbackFun = @selector(callBackStream:messageType:);
		if (![tcpClient connectSocket])
		{
			NSDictionary *info = [NSDictionary dictionaryWithObject:NSLocalizedString(@"pica_str_tip_disconnected_unknown_reason",nil) forKey:@"message"];
			[ClientNetWorkController postNotification:NetWorkDisabledNotification info:info];
		}
	}
	
	NSString *command =@"<?xml version='1.0' encoding='UTF-8'?>\
<stream:stream  to='pica' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'>";
    
    
    
	
	[tcpClient writeString:command messageType:1];
	[ClientNetWorkController postNotification:FindDispatchNotification info:nil];	
}

- (void)sendAccountRequest
{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"login"];
	if (dict != nil && [dict count] > 0)
	{	
		NSString *userName = [dict valueForKey:@"id"];
		int ix = [userName rangeOfString:@"@"].location;
		if (ix != NSNotFound)
		{
			userName = [userName substringToIndex:ix];
			NSString *password = [dict valueForKey:kPasswordKey];
			NSString *command = [NSString stringWithFormat:
								 @"<iq type='set'>\
								 <query xmlns='jabber:iq:auth'>\
								 <username>%@</username>\
								 <resource>pica</resource>\
								 <password>%@</password>\
								 <version>%@</version>\
								 </query>\
								 </iq>",userName,password,NSLocalizedString(APPLICATION_VERSION,nil)];
			
			[tcpClient writeString:command messageType:2];
			[ClientNetWorkController postNotification:ConnectingServerNotification info:nil];	
		}
	}	
}

- (void)sendRosterRequest 
{
	NSString *command = @"<iq type='get'><query xmlns='jabber:iq:roster'/></iq>";
	
	[tcpClient writeString:command messageType:3];
}

- (void)sendPresenceRequest
{
	NSString *command =  @"<presence type='available'/>";
	
	[tcpClient writeString:command messageType:PRESENCE_REQUEST];
	[self sendMClubUserPresence];
}

- (void)sendLoginRequest
{
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSMutableDictionary *infoDictionary = appdelegate.infoDict;
	
	NSString *profileVersion = [infoDictionary objectForKey:kProfileVersionKey];
	NSString *rosterVersion = [infoDictionary objectForKey:kRosterVersionKey];
	
	NSString *deviceid = [infoDictionary objectForKey:@"deviceid"];
	NSString *profileResourceid = [infoDictionary objectForKey:@"resourceid"];
	
	NSString *account = appdelegate.myAccount;
	NSString *password = appdelegate.myPassword;
	NSString *status = appdelegate.myStatus;
	if (account == nil || password == nil || status == nil)
		return;
	
	status = [[status stringByReplacingOccurrencesOfString:@" " withString:@""] lowercaseString];
	if ([status compare:@"appearsoffline"] == NSOrderedSame)
		status = @"offline";
	
	NSString *command =[NSString stringWithFormat:
@"<iq type='set' to='msn.pica' id='jcl-3'>\
<query xmlns='jabber:iq:register'>\
<username>%@</username><password>%@</password>\
<uastring>WVGateway_MMIMGatewayNOK.N3230_0100-0.800.523.cn</uastring>\
<version>%@</version><image_accept>image/jpeg,image/png,image/bmp</image_accept><audio_accept>audio/wav</audio_accept>\
<source>msn</source>\
<xroster>all</xroster>",account,[self convertString:password],NSLocalizedString(APPLICATION_VERSION,nil)];	
	
	if (deviceid != nil)
	{
		command = [command stringByAppendingFormat:@"<deviceid>%@</deviceid>",deviceid];
	}
	
//	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
//	BOOL avatarEnabled  = [defaults boolForKey:@"kAvatarChangedKey"];
//	BOOL emotionEnabled = [defaults boolForKey:@"kEmotionChangedKey"];
	NSString *capabilityString = nil;
	
//	MYLog(@"avatar:%d emotion:%d",avatarEnabled, emotionEnabled);
	//modify by liubing 20091227
	/*if (avatarEnabled == YES && emotionEnabled == YES)
	{
		capabilityString = @"000003ff";
	}else if (avatarEnabled == YES && emotionEnabled != YES){
		capabilityString = @"000003bf";
	}else if (avatarEnabled != YES && emotionEnabled == YES){
		capabilityString = @"0000037f";
	}else if (avatarEnabled != YES && emotionEnabled != YES){
		capabilityString = @"0000033f";
	}*/
	
#ifdef VERSION_FOR_SIMPLIFIED
	capabilityString = @"000003f3";
#else
	capabilityString = @"000003ff";
#endif
	
	command = [command stringByAppendingFormat:
@"<show>%@</show><capability>%@</capability><profile_portaitsize>32</profile_portaitsize>\
<contact_portraitsize>32</contact_portraitsize>",status,capabilityString];
//	MYLog(@">>>[capability]:%@",command);
	
	if (profileResourceid != nil)
	{
		command = [command stringByAppendingFormat:@"<profile_resourceid>%@</profile_resourceid>",profileResourceid];//modify by liubing20091130
	}
	if (profileVersion != nil){
		command = [command stringByAppendingFormat:@"<sync_profile>%@</sync_profile>",profileVersion];
	}
	if (rosterVersion != nil){
		command = [command stringByAppendingFormat:@"<sync_contacts>%@</sync_contacts>",rosterVersion];
	}
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if (dict != nil)
	{
		NSString *dataMethod = [dict objectForKey:@"data_method"];
		if (dataMethod != nil)
		{
			command = [command stringByAppendingFormat:@"<data_method>%@</data_method>",dataMethod];
			//command = [command stringByAppendingFormat:@"<data_method>000000</data_method>",dataMethod];
		}
	}
	command = [command stringByAppendingString:@"</query></iq><iq type='get' source='msn' to='cluster.pica' id='jcl-4'><query xmlns='pica:cluster:category'/></iq>"];
	
	[tcpClient writeString:command messageType:LOGIN_REQUEST];
	[ClientNetWorkController postNotification:LoggingInNotification info:nil];
}

- (void)sendPortraitRequest:(NSString*)jid
{
	NSString *command = [NSString stringWithFormat:
@"<iq type='get' to='msn.pica'>\
<query xmlns='jabber:iq:roster' action='getportrait'>\
<item jid='%@'/>\
</query>\
</iq>",jid];
	
//	MYLog(@"sendPortraitRequest:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}


- (void)sendLogOutRequest
{
	NSString *command =
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:iq:register'>\
<remove/>\
</query>\
</iq></stream:stream>";
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendPresenceRequest:(NSString*)state
{
	NSString *command = [NSString stringWithFormat:
@"<presence type='available' to='msn.pica'>\
<show>%@</show>\
</presence>",state];
	MYLog(@"sendPresenceRequest:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendLocalNameRequest:(NSDictionary *)infoDict
{
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSDictionary *infoDictionary = appdelegate.infoDict;
	
	NSString *version = [infoDictionary objectForKey:kProfileVersionKey];
	NSString *command = [NSString stringWithFormat:
						 @"<iq type='set' to='msn.pica'><query xmlns='jabber:iq:roster' action='modify' version='%@'>",version];
	command = [command stringByAppendingFormat:@"<item jid='%@'>",[[infoDict objectForKey:@"kPersonInfo"] jid]];
    command = [command stringByAppendingFormat:@"<localname>%@</localname></item></query></iq>",[infoDict objectForKey:@"kLocalName"]];

	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendVcardRequest:(PersonInfo*)info
{
	if ([info isKindOfClass:[PersonInfo class]])
	{
		if (info.nickname != nil || info.impresa != nil || info.portraitURL != nil || info.portraitData != nil)
		{
			MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
			NSDictionary *infoDictionary = appdelegate.infoDict;
			
			NSString *version = [infoDictionary objectForKey:kProfileVersionKey];
			NSString *command = [NSString stringWithFormat:
								 @"<iq type='set' to='msn.pica'><query xmlns='vcard-temp' version='%@'>",version];
			if (info.nickname != nil)
			{
				command = [command stringByAppendingFormat:@"<nickname>%@</nickname>",[self convertString:info.nickname]];
			}
			if (info.impresa != nil)
			{
				command = [command stringByAppendingFormat:@"<impresa>%@</impresa>",[self convertString:info.impresa]];
			}
			if (info.portraitURL != nil)
			{
				command = [command stringByAppendingFormat:@"<portrait mime='%@'><url>%@</url></portrait>",info.portraitType,info.portraitURL];
			}
			else
			{
				if (info.portraitType == @"" || info.portraitData == @"")
				{
					command = [command stringByAppendingFormat:@"<portrait/>"];
				}
				else if (info.portraitType != nil && info.portraitData != nil)
				{
					command = [command stringByAppendingFormat:@"<portrait mime='%@'><data length='%d'>%@</data></portrait>",
							   info.portraitType,[info.portraitData length],info.portraitData];
				}
			}
			
			command = [command stringByAppendingString: @"</query></iq>"]; 
			
			MYLog(@">>>[sendVcardRequest]: %@",command);
			[tcpClient writeString:command messageType:MESSAGE_REQUEST];
		}
	}	
}

- (void)sendAddContactRequest:(NSString *)theContact
{
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSDictionary *infoDictionary = appdelegate.infoDict;
	
	NSString *version = [infoDictionary objectForKey:kRosterVersionKey];
	NSString *command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:iq:roster' action='add' version='%@'>\
<item imid='%@'/>\
</query>\
</iq>",version,theContact];
	
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendBlockContactRequest:(NSString*)theContact block: (BOOL)aBlock
{
	NSString *type = aBlock ? @"block" : @"unblock";
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSDictionary *infoDictionary = appdelegate.infoDict;
	NSString *version = [infoDictionary objectForKey:kRosterVersionKey];
	
	NSString *command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:iq:roster' action='%@' version='%@'>\
<item jid='%@'/>\
</query></iq>",type,version,theContact];
	
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendDeleteContactRequest:(NSString*)theContact block: (BOOL)aBlock
{
	NSString *type = aBlock ? @"rmblk" : @"delete";
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSDictionary *infoDictionary = appdelegate.infoDict;
	NSString *version = [infoDictionary objectForKey:kRosterVersionKey];
	NSString *command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:iq:roster' action='%@' version='%@'>\
<item jid='%@'/>\
</query></iq>",type,version,theContact];
	
	MYLog(@"delete command : %@", command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendAnswerStrangerRequest:(NSString *)theContact add: (BOOL)anAdd
{
	NSString *answer = anAdd ? @"accept" : @"deny";
	
	NSString *command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:iq:roster' action='subscriptionanswer'>\
<item imid='%@' subscription='%@'/>\
</query></iq>",theContact, answer];
	
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendMessageTo:(NSString*)theJid message:(NSString *)theMessage sid:(NSString*)theSid contact:(BOOL)known
{
	NSString *string = nil;
	if (known)
		string = @"jid";
	else
		string = @"imid";
	
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' %@='%@' type='chat' sid='%@'>\
<body>%@</body>\
</message>",string, theJid, theSid, [self convertString:theMessage]];

	MYLog(@"sendMessage:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendMessage:(NSString *)theMessage sid:(NSString*)theSid
{
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' type='groupchat' sid='%@'>\
<body>%@</body>\
</message>",theSid, [self convertString:theMessage]];
	
	MYLog(@"sendMessage:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];	
}

- (void)sendAddParticipantRequest:(NSString*)theJid jids:(NSArray*)theJids sid:(NSString*)theSid contact:(BOOL)known;
{
	NSString *string = nil;
	if (known)
		string = @"jid";
	else
		string = @"imid";
	
	NSString *command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:x:dialog' sid='%@' action='add'>\
<control>\
<firstparticipant %@='%@'/>\
</control>",theSid, string, theJid];
	
	for (string in theJids) 
	{
		command = [command stringByAppendingFormat:@"<participant jid='%@'/>",string];
	}
	command = [command stringByAppendingString: @"</query></iq>"];
	//MYLog(@"sendAddParticipantRequest:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendAddParticipantRequest:(NSArray*)theJids sid:(NSString*)theSid
{
	NSString *string = nil;
	NSString *command = command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica'>\
<query xmlns='jabber:x:dialog' sid='%@' action='add'>",theSid];					 
	for (string in theJids) 
	{
		command = [command stringByAppendingFormat:@"<participant jid='%@'/>",string];
	}
	command = [command stringByAppendingString: @"</query></iq>"];
	
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendEndDialogRequest:(NSString*)theSid
{
	NSString *command = [NSString stringWithFormat:
@"<iq type='set' to='msn.pica' >\
<query xmlns='jabber:x:dialog' sid='%@' action='end'/>\
</iq>", theSid];					 
	
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendTransferInviteRequest:(NSString*)theJid sid:(NSString*)theSid 
							 ftid:(NSString*)theFtid name:(NSString*)theName size:(int)theSize
{
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' jid='%@' type='ft' sid='%@'>\
<control ftid='%@' action='push'>\
<name>%@</name>\
<size>%d</size>\
</control>\
</message>", theJid,theSid,theFtid,theName,theSize];					 
	
	MYLog(@"--sendTransferInviteRequest:%@------------",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendTransferInviteResponse:(NSString*)theTransferid sid:(NSString*)theSid 
							  ftid:(NSString*)theFtid accept:(BOOL)accept
{
	NSString *string = nil;
	if (accept)
		string = @"accept";
	else
		string = @"reject";
	
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' type='ft' transferid='%@' sid='%@'>\
<control ftid='%@' action='%@'>\
</control>\
</message>", theTransferid, theSid, theFtid, string];					 
	
	MYLog(@"--sendTransferInviteResponse:%@------------",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}


- (void)sendTransferData:(NSString*)theTransferid sid:(NSString*)theSid ftid:(NSString*)theFtid
				   seqid:(NSString*)theSeqid totalLength:(int)theTotalLength startPos:(int)theStartPos 
				  endPos:(int)theEndPos currentLength:(int)theCurrentLength data:(NSString*)theData
{
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' type='ft' transferid='%@' sid='%@'>\
<control ftid='%@' seqid='%@' action='data'>\
<data bytes='%d' start='%d' end='%d' length='%d'>%@</data>\
</control>\
</message>", theTransferid, theSid, theFtid, theSeqid,theTotalLength,theStartPos,theEndPos,theCurrentLength,theData];					 
	
	MYLog(@"sendTransferData:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendVoiceClip:(NSString*)theJid sid:(NSString*)theSid totalLength:(int)theTotalLength 
		 base64Length:(int)theBase64Length data:(NSString*)theData url:(NSString*)theURL
{
	if (theURL != nil || theData != nil)
	{
		NSString *command = [NSString stringWithFormat:@"<message to='msn.pica' jid='%@' type='voiceclip' sid='%@' id='j-1'>", theJid, theSid];
		if (theURL != nil)
		{
			command = [command stringByAppendingFormat: @"<url mime='audio/wav'>%@</url>",theURL	];
		}
		else
		{
			command = [command stringByAppendingFormat: @"<data mime='audio/wav' bytes='%d' length='%d'>%@</data>",theTotalLength,theBase64Length,theData];
		}
		command = [command stringByAppendingString:@"</message>"];					 
		MYLog(@"sendVoiceClip:%@",command);
		[tcpClient writeString:command messageType:MESSAGE_REQUEST];	
	}
}

- (void)sendContactsNickNameToServer{
	NSString* anItems = @"";
	NSString* aNickName = nil;
	PersonInfo* aPerson = nil;
	ContactList* aContactList = [ContactList getInstance];
	if ([aContactList.contactList count] == 0)
		return ;
	for (aPerson in aContactList.contactList){
		aNickName = convertString([aPerson ex_nickname]);
		anItems = [anItems stringByAppendingFormat: @"<item jid='%@' nickname='%@'/>", aPerson.jid, aNickName];
	}

	[tcpClient writeString: [NSString stringWithFormat: @"<iq type='set' to='msn.pica'><query xmlns='jabber:iq:apncontact'>%@</query></iq>", anItems] messageType: MESSAGE_REQUEST];	
}

- (void)sendOfflineMsgRequest{
	[tcpClient writeString: [NSString stringWithFormat: @"<iq type='get' to='apn.pica'><query xmlns='jabber:iq:apnmsg'><msn_id>%@</msn_id></query></iq>", [MSNAppDelegate globalAppDelegate].myAccount] messageType: MESSAGE_REQUEST];	
}

- (void)sendAPSInfoToServer{
	MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
	unsigned char* aDeviceToken = [anAppDelegate deviceToken];
	UIApplication* anApp = [UIApplication sharedApplication];
	if ([anApp respondsToSelector: @selector(enabledRemoteNotificationTypes)] &&
		[anApp enabledRemoteNotificationTypes]){
		NSInteger aValue = [[NSUserDefaults standardUserDefaults] integerForKey: kAPSDurationKey];
		aValue = durationFrom(aValue);
		[tcpClient writeString: [NSString stringWithFormat: @"<iq type='set' to='msn.pica'><query xmlns='jabber:iq:apn'><devicetoken>%s</devicetoken><interval>%d</interval></query></iq>", aDeviceToken, aValue] messageType: MESSAGE_REQUEST];	
	}
}

- (void)updatePhotoThread: (id)aParam{
    exitThread = NO;
    PersonInfo* aPerson = nil;
    NSMutableArray* aPersons = nil;
    NSAutoreleasePool* aPool = [[NSAutoreleasePool alloc] init];
    if (![aParam isKindOfClass: [NSMutableArray class]]) return;
    sleep(1);
    aPersons = (NSMutableArray*)aParam;
	while ([aPersons count] > 0){
        [lock lock];
        if (exitThread){
            [lock unlock];
            break;
        }
        [lock unlock];
        aPerson = [aPersons lastObject];
        if (aPerson.state == nil || [aPerson.state isEqualToString: @"offline"]){
            [aPersons removeObject: aPerson];
            continue;
        }
        [[ClientNetWorkController getInstance] sendPortraitRequest: aPerson.jid];
        [aPersons removeObject: aPerson];
        sleep(1);
	}
    [aPool drain];
}

- (void)sendUpdatePortrait{
	PersonInfo* aPerson = nil;
	ContactList* aContactList = [ContactList getInstance];
	if ([aContactList.contactList count] == 0)
		return ;
    NSMutableArray* aPersons = [[NSMutableArray alloc] init];
	for (aPerson in aContactList.contactList){
        if ([aPerson.portraitData length] == 0){
            [aPersons addObject: aPerson];
        }
	}
    if ([aPersons count] > 0){
        [NSThread detachNewThreadSelector: @selector(updatePhotoThread:)
                                 toTarget: self
                               withObject: aPersons];
    }
    [aPersons release];
}

- (void)sendDataCompleteRequest:(NSString*)theTransferid sid:(NSString*)theSid 
						   ftid:(NSString*)theFtid url:theURL
{
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' type='ft' transferid='%@' sid='%@'>\
<control ftid='%@' action='complete'/>", theTransferid,theSid,theFtid];					 
	if (theURL != nil)
			command = [command stringByAppendingString: [NSString stringWithFormat:@"<url>%@</url>", theURL]];
	
	command = [command stringByAppendingString: @"</message>"];
	MYLog(@"sendDataCompleteRequest:%@",command);
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];	
}

- (void)sendDataReceiveAck:(NSString*)theTransferid
                       sid:(NSString*)theSid
                     seqid: (NSString*)aSeqid
                      ftid:(NSString*)theFtid{
	if (theTransferid == nil)
		theTransferid = @"";
	NSString *command = [NSString stringWithFormat:
                         @"<message to='msn.pica' type='ft' transferid='%@' sid='%@'>"\
                         "<control ftid='%@' seqid='%@' action='ack' status='ok'>"\
                         "</control>"\
                         "</message>", theTransferid, theSid, theFtid, aSeqid];
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}

- (void)sendCancelTransferRequest:(NSString*)theTransferid sid:(NSString*)theSid ftid:(NSString*)theFtid
{
	if (theTransferid == nil)
		theTransferid = @"";
	NSString *command = [NSString stringWithFormat:
@"<message to='msn.pica' type='ft' transferid='%@' sid='%@'>\
<control ftid='%@' action='cancel'>\
</control>\
</message>", theTransferid,theSid,theFtid];					 
	
	[tcpClient writeString:command messageType:MESSAGE_REQUEST];
}


//advt
- (void)sendAdviceRequest:(NSString*)theUrl
{
	if (httpClient == nil)
	{
		self.httpClient = [HttpClient getInstance];
		httpClient.callbackFun = @selector(callBackStream:dictionary:);
		httpClient.delegate = self;	
	}
	MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	NSString *account = appdelegate.myAccount;
	if (account != nil)
	{
		NSURL * url = [[NSURL alloc] initWithString:theUrl];//211.99.191.239:32246
		NSString *bodyString = [NSString stringWithFormat:@"<adreq from='%@' source='msn' screen='320*480' media='jpg/gif/png'>\
								<item type='login-prog-ad' id='1' count='1'/>\
								<item type='roster-footer-ad' id='1' count='1'/>\
								<item type='chat-header-ad' id='1' count='1'/>\
								</adreq>",account];
		
		NSData *bodyData = [NSData dataWithBytes:[bodyString UTF8String] length:[bodyString length]];
		[httpClient addHttpRequest:url
							method:@"POST"
						   headers:nil
						bodyString:bodyData
							  type:ADVERTISE_REQUEST];
		[url release];
	}
}

- (void)sendAdviceClickedRequest:(NSString*)content
{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if (dict != nil)
	{
		if (httpClient == nil)
		{
			self.httpClient = [HttpClient getInstance];
			httpClient.callbackFun = @selector(callBackStream:dictionary:);
			httpClient.delegate = self;	
		}
		NSString *url = [dict objectForKey:@"adurl"];
		if (url != nil)
		{
			NSRange range = [url rangeOfString:@"/" options:NSBackwardsSearch range:NSMakeRange(0, [url length])];
			if (range.location != NSNotFound)
			{
				url = [url substringToIndex:range.location];
				url = [url stringByAppendingString:@"/advisit"];
			}
			NSURL * urlPath = [[NSURL alloc] initWithString:url];
			if (urlPath != nil)
			{
				MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
				NSString *account = appdelegate.myAccount;
				if (account != nil)
				{
					NSString *body = [NSString stringWithFormat:@"<advisit from='%@' source='msn' %@/>",
									  account,content];
					[httpClient addHttpRequest:urlPath
										method:@"POST"
									   headers:nil
									bodyString:[body dataUsingEncoding:NSUTF8StringEncoding]
										  type:ADCLICKED_REQUEST];
				}
				[urlPath release];			
			}	
		}
	}
}

///////////////zwp

- (void)sendZwpRequest:(NSString*)theUrl 
			   headers:(NSDictionary*)theHeaders
				  body:(NSData*)theBody 
				method:(NSString*)theMethod 
			   session:(NSString*)theSession
				  type:(int)theType
{
	if (httpClient == nil)
	{
		self.httpClient = [HttpClient getInstance];
		httpClient.callbackFun = @selector(callBackStream:dictionary:);
		httpClient.delegate = self;	
	}

	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	if (dict != nil)
	{
		NSMutableData *bodyData = nil;
		UInt32 length = 0;
		NSString *string = nil;
		NSMutableData *userData = [NSMutableData dataWithCapacity:0];
		NSMutableDictionary *headers = [NSMutableDictionary dictionaryWithCapacity:0];
		if (theSession != nil)
		{
			[headers setObject: theSession forKey:@"Pica-Session"];
		}
		else
		{
			[headers setObject:@"application/octet-stream" forKey:@"Content-Type"];
			
//			string = [dict objectForKey:@"ua"];
//			if (string != nil)
//				[headers setObject:string forKey:@"User-Agent"];
			
			NSDictionary *loginDict = [[NSUserDefaults standardUserDefaults] objectForKey:@"login"];
			if (loginDict != nil)
			{
				string = [loginDict objectForKey:@"id"];
				int index = [string rangeOfString:@"@"].location;
				if (index != NSNotFound)
				{
					string = [string substringToIndex:index];
					NSString *password = [loginDict valueForKey:kPasswordKey];
					if (password != nil)
					{
						string = [string stringByAppendingFormat:@"/%@",password];
						if (string != nil)
							[headers setObject:string forKey:@"Pica-Auth"];
					}
				}
			}
			
			string = [dict objectForKey:@"zwp"];
			if (string != nil)
			{
				[headers setObject:string forKey:@"X-Online-Host"];
			}
			
			MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
			if (appdelegate.myAccount != nil && appdelegate.myPassword != nil)
			{
				string = [appdelegate.myAccount stringByAppendingFormat:@"/%@",appdelegate.myPassword];
				[headers setObject:string forKey:@"ZWP_IM_ID"];
			}
			
			[headers setObject:NSLocalizedString(APPLICATION_VERSION,nil) forKey:@"ZWP_IM_VERSION"];
			NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
			
			if ([dict objectForKey:@"iphone"] != nil)
			{
				[headers setObject:[dict objectForKey:@"iphone"] forKey:@"ZWP_IM_MDN"];
			}
			
			CGSize size = [@"."  sizeWithFont:[UIFont systemFontOfSize:20] constrainedToSize:CGSizeMake(100, 100)];
			
			UInt16 ZWP_UserAgent = char16(1);
			UInt8 platform_id = 1;//smartphone
			UInt16 zwp_version = char16(5);
			NSData *agent_version = [@"j2meV0.1" dataUsingEncoding:NSUTF8StringEncoding];
			UInt16 agent_version_length = char16([agent_version length]); 
			UInt16 screen_width = char16(320);
			UInt16 screen_height = char16(480);
			UInt16 client_width = char16(320);
			UInt16 client_height = char16(460);
			UInt8 font_with = size.width;
			UInt8 font_height = size.height;
			UInt8 screen_color_bits = 255;
			UInt8 support_cookie = 0;
			UInt8 support_upload = 0;
			UInt8 support_record = 1;
			UInt8 support_camera = 1;
			UInt8 support_sys_browser = 1;
			UInt8 support_telepone = 1;
			UInt8 support_sms = 1;
			UInt8 support_pica_msg = 1;
			
			NSData *images = [@"png;jpg;bmp;gif" dataUsingEncoding:NSUTF8StringEncoding];
			UInt16 images_length = char16([images length]);
			NSData *audios = [@"wav" dataUsingEncoding:NSUTF8StringEncoding];
			UInt16 audios_length = char16([audios length]);
			NSData *user_nickname = [[appdelegate.infoDict objectForKey:@"nickname"] dataUsingEncoding:NSUTF8StringEncoding];
			UInt16 user_nickname_length = char16([user_nickname length]);
			
			NSMutableData *bodyData = [NSMutableData data];
			[bodyData appendBytes:&platform_id length:1];
			[bodyData appendBytes:&zwp_version length:2];
			[bodyData appendBytes:&agent_version_length length:2];
			[bodyData appendData:agent_version];
			[bodyData appendBytes:&screen_width length:2];
			[bodyData appendBytes:&screen_height length:2];
			[bodyData appendBytes:&client_width length:2];
			[bodyData appendBytes:&client_height length:2];
			[bodyData appendBytes:&font_with length:1];
			[bodyData appendBytes:&font_height length:1];
			[bodyData appendBytes:&screen_color_bits length:1];
			[bodyData appendBytes:&images_length length:2];
			[bodyData appendData:images];
			[bodyData appendBytes:&audios_length length:2];
			[bodyData appendData:audios];
			[bodyData appendBytes:&support_cookie length:1];
			[bodyData appendBytes:&support_upload length:1];		
			[bodyData appendBytes:&support_record length:1];
			[bodyData appendBytes:&support_camera length:1];
			[bodyData appendBytes:&support_sys_browser length:1];
			[bodyData appendBytes:&support_telepone length:1];		
			[bodyData appendBytes:&support_sms length:1];
			[bodyData appendBytes:&support_pica_msg length:1];
			[bodyData appendBytes:&user_nickname_length length:2];
			[bodyData appendData:user_nickname];
			
			[userData appendBytes:&ZWP_UserAgent length:2];
			length = char32([bodyData length]);
			[userData appendBytes:&length length:4];
			[userData appendData:bodyData];
			
			
		}

		UInt8 method = 0;
		if (theMethod == nil || [theMethod compare:@"GET"] == NSOrderedSame)
		{
			method = 0;
		}
		else if ([theMethod compare:@"POST"] == NSOrderedSame)
		{
			method = 1;			
		}
		else if ([theMethod compare:@"PUT"] == NSOrderedSame)
		{
			method = 2;	
		}
		
		UInt16 ZWP_RequestRes = char16(2);
		NSData *urlString = [theUrl dataUsingEncoding:NSUTF8StringEncoding];
		UInt16 url_length = char16([urlString length]);
		UInt32 max_page_size = char32(1024);
		UInt8 req_segment_index = 0;
		UInt8 headers_count = [theHeaders count];
		
		bodyData = [NSMutableData data];
		[bodyData appendBytes:&method length:1];
		[bodyData appendBytes:&url_length length:2];
		[bodyData appendData:urlString];
		
//		MYLog(@">>> [max_page_size]:%ld",max_page_size);
		[bodyData appendBytes:&max_page_size length:4];
		[bodyData appendBytes:&req_segment_index length:1];
		
		[bodyData appendBytes:&headers_count length:1];
		if (theHeaders != nil)
		{
			NSString *headerKey = nil;
			NSString *headerValue = nil;
			for (headerKey in [theHeaders allKeys])
			{
				UInt16 header_len = char16([headerKey length]);
				headerValue = [theHeaders objectForKey:headerKey];
				if ([headerValue isKindOfClass:[NSString class]])
				{
					[bodyData appendBytes:&header_len length:2];
					[bodyData appendData:[headerKey dataUsingEncoding:NSUTF8StringEncoding]];
					UInt8 type = 0;
					[bodyData appendBytes:&type length:1];
					
					header_len = char16([headerValue length]);
					[bodyData appendBytes:&header_len length:2];
					[bodyData appendData:[headerValue dataUsingEncoding:NSUTF8StringEncoding]];
				}
			}
			
		}
		
		if (theBody != nil)
		{
			NSString *bodyString = [[NSString alloc] initWithData:theBody encoding:NSUTF8StringEncoding];
			if (bodyString != nil)
			{
				NSArray *array = [bodyString componentsSeparatedByString:@"&"];
				UInt8 form_params_count = [array count];
				[bodyData appendBytes:&form_params_count length:1];
				for (string in array)
				{
					NSArray *strings = [string componentsSeparatedByString:@"="];
					NSData *stringData = nil;
					NSString *bodyKey = nil;
					NSString *bodyValue = nil;
					UInt16 header_len = 0;
					if ([strings count] == 2)
					{
						bodyKey = [strings objectAtIndex:0];
						stringData = [bodyKey dataUsingEncoding:NSUTF8StringEncoding];
						header_len = char16([stringData length]);
						[bodyData appendBytes:&header_len length:2];
						[bodyData appendData:stringData];

						UInt8 type = 0;
						[bodyData appendBytes:&type length:1];
						
						bodyValue = [[strings objectAtIndex:1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
						stringData = [bodyValue dataUsingEncoding:NSUTF8StringEncoding];
						header_len = char16([stringData length]);
						[bodyData appendBytes:&header_len length:2];
						[bodyData appendData:stringData];
					}
				}
				[bodyString release];				
			}
		}
		else
		{
			UInt8 form_params_count = 0;
			[bodyData appendBytes:&form_params_count length:1];
		}
		
//		MYLog(@">>> [req_segment_index] %ld",ZWP_RequestRes);
//		ZWP_RequestRes = 0;
		
		[userData appendBytes:&ZWP_RequestRes length:2];
		length = char32([bodyData length]);
		[userData appendBytes:&length length:4];
		[userData appendData:bodyData];
		
		string = [dict objectForKey:@"zwp"];
		if (string != nil)
		{
			string = [@"http://" stringByAppendingString:string];
		}
		
		NSURL * url = [[NSURL alloc] initWithString:string];	
		[httpClient addHttpRequest:url
							method:@"POST"
						   headers:headers
						bodyString:userData 
							  type:theType];
		[url release];
	}
}

- (NSString*)numericString:(NSString*) aString
{
	if ([aString length] == 0) return aString;
	NSInteger i = 0;
	NSInteger anIndex = 0;
	const char* p = [aString UTF8String];
	NSInteger aLen = strlen(p);
	char* buf = malloc(aLen + 1);
	memset(buf, 0, aLen + 1);
	for (; i < aLen; i++)
	{
		if (p[i] >= '0' && p[i] <= '9')
		{
			buf[anIndex++] = p[i];
		}
	}
	NSString * theString = [NSString stringWithFormat: @"%s", buf];
	free(buf);
	return theString;
}

- (NSString*) toHexString:(NSString*) aString
{
	if ([aString length] == 0) return @"0x00";
	NSInteger i = 0;
	NSString * theString = nil;
	NSString *aContents = @"";
	const char* p = [aString UTF8String];
	NSInteger aLen = strlen(p);
	NSInteger aBufLen = aLen * 4;
	char* buf = malloc(aLen + 1);
	memset(buf, 0, aBufLen + 1);
	char* ptr = buf;
	for (; i < aLen; i++){
		unsigned short c = p[i];
		theString = [NSString stringWithFormat: @"0x%0x", c];
		aContents = [aContents stringByAppendingString: theString];
		ptr += 4;
	}
	free(buf);
	return aContents;
}

#pragma mark -
#pragma mark alertview
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 1)
	{
		// Jump to live-wap
		MSNAppDelegate *theApp = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
		theApp.tabController.selectedIndex = [theApp getTheIndex:kStandbyLiveWapVC];
		theApp.tabController.segmentedControl.selectedSegmentIndex = [theApp getTheIndex:kStandbyLiveWapVC];
	}
}

@end
