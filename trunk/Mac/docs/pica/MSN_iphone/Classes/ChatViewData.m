//
//  ChatViewData.m
//  MSN
//
//  Created by  apple on 09-3-18.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ChatViewData.h"
#import "ChatViewController.h"
#import "ImageLabel.h"
#import "SendFileObject.h"
#import "ReceivedFileObject.h"
#import "PersonInfo.h"
#import "ContactList.h"
#import "ClientNetWorkController.h"
#import "MSNAppDelegate.h"
#import "NSData+base64.h"
#import "define.h"
#import "public.h"
#import "API.h"
#import "OnlineListController.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define kRecvMsgTimeKey             (@"RecvMsgTime")

NSString *ChangeChatviewContactNotification = @"ChangeChatViewContact";
NSString *AddMessageToChatViewNotification = @"AddMessageToChatViewNotification";

enum {
	CELL_TAG_FOR_BUTTON		= 0x10,
	CELL_TAG_FOR_LABEL		= 0x20,
	CELL_TAG_FOR_IMAGE		= 0x40,
	CELL_TAG_FOR_PROGRESS	= 0x80,
};

@implementation ChatViewData

@synthesize  persons, sid, hasMessage, image, controller,sendingText;
@synthesize messageArray;
@synthesize isSound;

- (NSInteger)indexOfMsgTime: (NSNumber*)aCurrentMsgTime{
    NSInteger anIndex = 0;
    NSMutableDictionary* aDict = nil;
    NSNumber* aDictMsgTime = nil;
    NSComparisonResult aResult;
    for (;anIndex < [messageArray count]; anIndex++){
        aDict = [messageArray objectAtIndex: anIndex];
        aDictMsgTime = [aDict objectForKey: kRecvMsgTimeKey];
        if (aDictMsgTime != nil){
            aResult = [aDictMsgTime compare: aCurrentMsgTime];
            if (aResult == NSOrderedDescending){
                break;
            }
        }
    }
    return anIndex;
}

- (NSInteger)stringTimeToSeconds: (NSString*)aStringTime{
    if ([aStringTime length] == 0) return 0;
    
    NSInteger aSeconds = 0;
    NSArray* aComponents = [aStringTime componentsSeparatedByString: @" "];
    if ([aComponents count] == 2){
        NSArray* aDateComponents = [[aComponents objectAtIndex: 0] componentsSeparatedByString: @"-"];
        NSArray* aTimeComponents = [[aComponents objectAtIndex: 1] componentsSeparatedByString: @":"];
        if ([aDateComponents count] == 3 && [aTimeComponents count] == 3){
            NSDateComponents *theDateComponents = [[NSDateComponents alloc] init];
            [theDateComponents setYear: [[aDateComponents objectAtIndex: 0] intValue]];
            [theDateComponents setMonth: [[aDateComponents objectAtIndex: 1] intValue]];
            [theDateComponents setDay: [[aDateComponents objectAtIndex: 2] intValue]];
            
            [theDateComponents setHour: [[aTimeComponents objectAtIndex: 0] intValue]];
            [theDateComponents setMinute: [[aTimeComponents objectAtIndex: 1] intValue]];
            [theDateComponents setSecond: [[aTimeComponents objectAtIndex: 2] intValue]];
            
            NSCalendar *aCalender = [[NSCalendar alloc] initWithCalendarIdentifier: NSGregorianCalendar];
            NSDate *aDate = [aCalender dateFromComponents: theDateComponents];
            [theDateComponents release];
            [aCalender release];
            
            aSeconds = [aDate timeIntervalSince1970];
        }
    }
    return aSeconds;
}

- (void) clearViewsWithTag: (NSInteger)aTag inSuperView: (UIView*)aSuperView{
	UIView* aSubview = nil;
	NSInteger theTag = 0;
	NSArray* aSubviews = [aSuperView subviews];
	for (aSubview in aSubviews){
		theTag = aSubview.tag;
		if (theTag != aTag){
			[self clearViewsWithTag: aTag inSuperView: aSubview];
		}
		else {
			if ([aSubview isKindOfClass: [ImageLabel class]] ||
				[aSubview isKindOfClass: [UILabel class]] ||
				[aSubview isKindOfClass: [UIButton class]] ||
				[aSubview isKindOfClass: [UIProgressView class]]){
				[aSubview removeFromSuperview];
			}
			else {
				[self clearViewsWithTag: aTag inSuperView: aSubview];
			}
		}
	}
}

- (id) init
{
	self = [super init];
	if (self != nil)
	{
		messageSound = 0;
		nudgeSound = 0;
		clearCount = 0;
		image = nil;
		controller = nil;
		
		sendingFileDicts = nil;
		receivingFileDicts = nil;
		messageArray = [[NSMutableArray alloc] init];
		persons = [[NSMutableArray alloc] initWithCapacity:0];
		srandom(time(NULL));
		self.sid = [NSString stringWithFormat:@"j-%d",random()];
		myId = [sid retain];
		
		isSound = [[[MSNAppDelegate globalAppDelegate].infoDict objectForKey:kMessageAlertKey] boolValue];
		isConversationSaved = NO;

		NSString *sndpath = [[NSBundle mainBundle] pathForResource:@"message" ofType:@"wav" inDirectory:@"/"];
		if (sndpath != nil)
		{
			CFURLRef baseURL = (CFURLRef)[[NSURL alloc] initFileURLWithPath:sndpath];
			AudioServicesCreateSystemSoundID (baseURL, &messageSound);	
			CFRelease(baseURL);
		}
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		[nc addObserver:self 
			   selector:@selector(notifyPersonAdded:)
				   name:PersonAddedNotification 
				 object:nil];
		
		[nc addObserver:self 
			   selector:@selector(addMsgFromNotification:)
				   name:AddMessageToChatViewNotification 
				 object:nil];

		voiceClipPath = nil;
	}
	return self;
}

/*!
    @method     
    @abstract   save transcripts
    @discussion save chat transcripts.
*/
- (void)saveTranscripts
{
	MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
	BOOL saveConversation = [[anAppDelegate.infoDict objectForKey: kSaveConversationKey] boolValue];
	if (saveConversation && !isConversationSaved){
		LogMethod();
		NSDateComponents *timeComponents = [[NSCalendar currentCalendar] components:( kCFCalendarUnitMonth|kCFCalendarUnitDay|NSHourCalendarUnit|NSMinuteCalendarUnit|NSSecondCalendarUnit ) 
																		   fromDate:[NSDate date]];
		NSString *stringTime = [NSString stringWithFormat:@"%02d-%02d(%02d:%02d:%02d)",[timeComponents month],[timeComponents day],[timeComponents hour],[timeComponents minute],[timeComponents second]];
		[self saveConversation:stringTime];
		isConversationSaved = YES;
	}

	// Clean send file object
	SendFileObject *sendObj = nil;
	for (sendObj in [sendingFileDicts allValues])
	{
		[sendObj buttonClicked:sendObj.cancelButton];
	}
	
	// Clean received file object
	ReceivedFileObject *receiveObj = nil;
	for (receiveObj in [receivingFileDicts allValues])
	{
		[receiveObj buttonClicked:receiveObj.cancelButton];
	}
	
	if (isSendReceive) isSendReceive = NONE;
	
	// Turn off sound call.
	if (messageSound){
		AudioServicesDisposeSystemSoundID(messageSound);
		messageSound = 0;
	}
	if (nudgeSound){
		AudioServicesDisposeSystemSoundID(nudgeSound);
		nudgeSound = 0;
	}
	[[ClientNetWorkController getInstance] sendEndDialogRequest:sid];
}

/*!
    @method     
    @abstract   Save Conversation sessions
    @discussion Save sessions for global app dict, used for loading conversation lists from disk when app starting up.
*/

-(void)saveTranscriptsSessons
{
	LogMethod();
	NSArray *filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *directory = [filePaths objectAtIndex: 0];
	NSString *filePath = [directory stringByAppendingPathComponent:@"Sessions.plist"];
//	MYLog(@">>> filePath:%@",filePath);
	
	MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
	NSDictionary *dataDict = [appdelegate.infoDict objectForKey:@"conversation"];
	if (dataDict != nil)
	{
		BOOL res = [dataDict writeToFile:filePath atomically:NO];
		if (res == NO)
		{
//			NSLog(@">>> Write convsersation sesseion error!");
		}				
	}else{
		MYLog(@">>> data dict is null.");
	}
}

- (void) dealloc
{
	LogMethod();
	[self garbage];
	[super dealloc];
}

- (UITableViewCell*)findFileObjectCell: (UIView*)aSubView{
	if (aSubView == nil) return nil;
	UIView* aSuperView = [aSubView superview];
	if (aSuperView == nil) return nil;
	if ([aSuperView isKindOfClass: [UITableViewCell class]]) return (UITableViewCell*)aSuperView;
	return [self findFileObjectCell: aSuperView];
}

#pragma mark --
#pragma mark utilities

- (void)didSaveVoiceClip: (NSString*)aDstFilePath{
	if ([aDstFilePath length] <= 0) return;
	NSString* aSrcFilePath = NSTemporaryDirectory();
	NSString* aFileName = [aDstFilePath lastPathComponent];
	aSrcFilePath = [aSrcFilePath stringByAppendingPathComponent: aFileName];
	if ([[NSFileManager defaultManager] fileExistsAtPath: aDstFilePath]){
		NSString *aTmpFilePath = [aDstFilePath stringByDeletingLastPathComponent];
		aTmpFilePath = [aTmpFilePath stringByAppendingPathComponent: @"temp.wav"];
		[[NSFileManager defaultManager] moveItemAtPath: aDstFilePath
												toPath: aTmpFilePath
												 error: nil];
		if (![[NSFileManager defaultManager] copyItemAtPath: aSrcFilePath toPath: aDstFilePath error: nil]){
			[[NSFileManager defaultManager] moveItemAtPath: aTmpFilePath
													toPath: aDstFilePath
													 error: nil];
			UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"voice clip save fail", nil)
															  message: [NSString stringWithFormat: NSLocalizedString(@"Save voice clip failure", nil), aFileName]
															 delegate: nil
													cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)
													otherButtonTitles: nil];
			[anAlert show];
			[anAlert release];
		}
		else {
			[[NSFileManager defaultManager] removeItemAtPath: aTmpFilePath error: nil];
			UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"voice clip saved", nil)
															  message: [NSString stringWithFormat: NSLocalizedString(@"Save voice clip successfull", nil), aFileName]
															 delegate: nil
													cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)
													otherButtonTitles: nil];
			[anAlert show];
			[anAlert release];
		}
	}
	else {
		if (![[NSFileManager defaultManager] copyItemAtPath: aSrcFilePath toPath: aDstFilePath error: nil]){
			UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"voice clip save fail", nil)
															  message: [NSString stringWithFormat: NSLocalizedString(@"Save voice clip failure", nil), aFileName]
															 delegate: nil
													cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)
													otherButtonTitles: nil];
			[anAlert show];
			[anAlert release];
		}
		else {
			UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"voice clip saved", nil)
															  message: [NSString stringWithFormat: NSLocalizedString(@"Save voice clip successfull", nil), aFileName]
															 delegate: nil
													cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)
													otherButtonTitles: nil];
			[anAlert show];
			[anAlert release];
		}
	}
}

- (void)fakeSaveButtonCallback:(id)sender
{
	LogMethod();
	struct stat aPathInfo;
	NSString* aFilePath = nil;
	NSIndexPath* anIndexPath = [controller findIndexPathByCell: [self findFileObjectCell: sender]];
	if (anIndexPath == nil) return;
	NSMutableDictionary* aDict = [messageArray objectAtIndex: anIndexPath.row];
	if (aDict == nil) return;
	id anObject = [aDict objectForKey: @"progress"];
	if (anObject == nil){
		anObject = [aDict objectForKey: @"receivedVoice"];
	}
	if (anObject == nil || ![anObject respondsToSelector: @selector(fileName)]) return;
	aFilePath = audioClipPath([anObject fileName]);

	
	if ([[NSFileManager defaultManager] fileExistsAtPath: aFilePath]){
		memset(&aPathInfo, 0, sizeof(aPathInfo));
		NSString *aTmpPath = [NSTemporaryDirectory() stringByAppendingPathComponent: [anObject fileName]];
		NSInteger ret = stat([aTmpPath UTF8String], &aPathInfo);
		if (ret == 0 && aPathInfo.st_nlink > 1){
			//only link, the file is existent
			UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"voice clip saved", nil)
															  message: [NSString stringWithFormat: NSLocalizedString(@"Save voice clip successfull", nil), [anObject fileName]]
															 delegate: nil
													cancelButtonTitle: NSLocalizedString(@"qtn_ok",nil)
													otherButtonTitles: nil];
			[anAlert show];
			[anAlert release];
			return;
		}
		
		voiceClipPath = [aFilePath copy];
		UIAlertView *anAlert = [[UIAlertView alloc] initWithTitle: NSLocalizedString(@"voice clip save", nil)
														  message: [NSString stringWithFormat: NSLocalizedString(@"Save is existent", nil), [anObject fileName]]
														 delegate: self
												cancelButtonTitle: NSLocalizedString(@"qtn_no",nil)
												otherButtonTitles: NSLocalizedString(@"qtn_yes",nil), nil];
		[anAlert show];
		[anAlert release];
	}
	else{
		[self didSaveVoiceClip: aFilePath];
	}
}

- (void)clearVoiceMsg: (id)sender{
	UITableViewCell* aCell = [self findFileObjectCell: sender];
	NSIndexPath* anIndexPath = [controller findIndexPathByCell: aCell];
	if (anIndexPath == nil) return;
	NSMutableDictionary* aDict = [messageArray objectAtIndex: anIndexPath.row];
	if (aDict == nil) return;
	id anObject = [aDict objectForKey: @"progress"];
	if (anObject == nil){
		anObject = [aDict objectForKey: @"receivedVoice"];
	}
	if (anObject == nil) return;

	[messageArray removeObject: aDict];
	
	NSArray* anIndexPaths = [NSArray arrayWithObject: anIndexPath];
	[controller.receivedMessageView deleteRowsAtIndexPaths: anIndexPaths
	 withRowAnimation: UITableViewRowAnimationFade];
}

- (void)alertView: (UIAlertView *)alertView clickedButtonAtIndex: (NSInteger)buttonIndex{
	if (buttonIndex == 1){
		[self didSaveVoiceClip: voiceClipPath];
	}
	if (voiceClipPath != nil)
		[voiceClipPath release];
	voiceClipPath = nil;
}

#pragma mark --


- (PersonInfo*)contact
{
	return contact;
}

- (void)setContact:(PersonInfo*)theContact
{
	[theContact retain];
	[contact release];
	contact = theContact;
	if (contact == nil) return;
	
	if ([persons count] == 0){
		[persons addObject:contact];
    }
	else if ([persons count] == 1)
	{
		if ([persons objectAtIndex:0] != theContact)
		{
			[persons removeAllObjects];
			[persons addObject:theContact];
			[controller updatePressionButtonState];

		}
	}
}

- (int)messageCount
{
	return [messageArray count];
}


- (BOOL)receivingFile
{
	return [[receivingFileDicts allValues] count] > 0;
}

- (BOOL)sendingFile
{
	return [[sendingFileDicts allValues] count] > 0;
}


- (void)clear
{
	clearCount++;
	[messageArray removeAllObjects];
	[controller reloadMessageData];
}

- (void)addMsgToChatView:(NSString *)theMessage hiImageID:(NSString*)hiImageID
{
	NSDateComponents *timeComponents = [[NSCalendar currentCalendar] components:(NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit ) 
																	   fromDate:[NSDate date]];
	NSString *stringTime = [NSString stringWithFormat:@"%04d-%02d-%02d %02d:%02d:%02d",
							[timeComponents year], [timeComponents month], [timeComponents day],
							[timeComponents hour],[timeComponents minute],[timeComponents second]];
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	[dict setObject:[NSString stringWithFormat:@"%@ %@ (%@) ",controller.myName,NSLocalizedString(@"qtn_msn_say_name",nil),stringTime] forKey:@"name"];
	[dict setObject: theMessage forKey:@"message"];

	if (hiImageID != nil)
	{
		[dict setObject:hiImageID forKey:@"kHiImageID"];
	}
	
	[messageArray addObject:dict];
	
	[controller reloadMessageData];
}

-(void)sendMessageQuiet:(NSString *)theMessage
{
	if (contact != nil)
	{
		if ([persons count] > 1)
		{
			[[ClientNetWorkController getInstance] sendMessage:theMessage sid:sid];
		}
		else
		{
			if (contact.jid != nil)
				[[ClientNetWorkController getInstance] sendMessageTo:contact.jid message:theMessage sid:sid contact:YES];
			else if (contact.imid != nil)
			{
				[[ClientNetWorkController getInstance] sendMessageTo:contact.imid message:theMessage sid:sid contact:NO];
			}
		}
	}
}

-(void)addMsgFromNotification:(NSNotification *)note
{
	NSDictionary *userDict = [note userInfo];
	NSString *theStatus = [userDict objectForKey:@"kStatus"];
	NSString *theMessage = [userDict objectForKey:@"kMessage"];
	NSString *hiImageID = [userDict objectForKey:@"kHiImageID"];
	NSString *toLiveID = [userDict objectForKey:@"kToLiveID"];
	
	NSString *currentLiveID = contact.imid;
	if(([currentLiveID length] > 0) && ([currentLiveID compare:toLiveID] == NSOrderedSame)){
		if (([theStatus length] > 0) && ([theStatus compare:@"send"] == NSOrderedSame)){
			[self addMsgToChatView:theMessage hiImageID:hiImageID];
		}
		else if (([theStatus length] > 0) && ([theStatus compare:@"sayHiFailed"] == NSOrderedSame)){
			[self sendMessageQuiet:theMessage];
		}		
	}
}

- (void)send:(NSString*)theMessage;
{
	[self addMsgToChatView:theMessage hiImageID:nil];
	[self sendMessageQuiet:theMessage];
}

- (void)sendVoiceClip:(NSString*)theFile;
{
	NSData *data = [NSData dataWithContentsOfFile:theFile];
	if (data != nil && [data length] > 0)
	{
		SendFileObject *obj = [[SendFileObject alloc] initWithImageData:data voiceClip:YES];
		obj.chatViewData = self;
		obj.isVoiceClip = YES;
		obj.fileName = [theFile lastPathComponent];
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
		
		NSString *name = contact.nickname;
		if (name == nil)
			name = contact.imid;
		[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"send_voice_clip",nil), obj.fileSize / 1024 + 1, name] 
				 forKey:@"name"];
		
		if (obj.sendByServer)
		{
			if (sendingFileDicts == nil)
				sendingFileDicts = [[NSMutableDictionary alloc] initWithCapacity:0];	
			
			[sendingFileDicts setObject:obj forKey:obj.ftid];
		}
		[dict setObject:obj forKey:@"progress"];
		[messageArray addObject:dict];
		[controller reloadMessageData];
		[obj sendNextFile];
		[obj release];	
	}
}

- (void)sendImage:(UIImage*)theImage
{
	NSData *imageData = UIImageJPEGRepresentation (theImage, 0.95);
	SendFileObject *obj = [[SendFileObject alloc] initWithImageData:imageData voiceClip:NO];
	if (sendingFileDicts == nil)
	{
		sendingFileDicts = [[NSMutableDictionary alloc] initWithCapacity:0];	
		[sendingFileDicts setObject:obj forKey:obj.ftid];
	}
	else
	{
		while ([sendingFileDicts objectForKey:obj.ftid]) 
		{
			[obj release];
			obj = [[SendFileObject alloc] initWithImageData:imageData voiceClip:NO];
		}
		[sendingFileDicts setObject:obj forKey:obj.ftid];
	}
	obj.chatViewData = self;
	
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	//[dict setObject:@"client" forKey:@"from"];
	
	NSString *name = contact.nickname;
	if (name == nil)
		name = contact.imid;
	[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"wait_to_accept",nil), name, obj.fileName, obj.fileSize / 1024 + 1] 
			 forKey:@"name"];
	
	[dict setObject:obj forKey:@"sendfile"];
	[messageArray addObject:dict];
	[controller reloadMessageData];
	
	[[ClientNetWorkController getInstance] sendTransferInviteRequest:contact.jid sid:sid 
	 															ftid:obj.ftid name:obj.fileName size:[obj.imageData length]];
	[obj release];
}

- (void)sendAuioFile: (NSString*)aFilepath{
	if ([aFilepath length] == 0) return;
	NSData *aData = [NSData dataWithContentsOfFile: aFilepath];
	if (aData == nil) return;
	SendFileObject *aSendObject = [[SendFileObject alloc] initWithImageData: aData voiceClip: NO];
	aSendObject.chatViewData = self;
	aSendObject.fileName = [aFilepath lastPathComponent];
	if (sendingFileDicts == nil)
	{
		sendingFileDicts = [[NSMutableDictionary alloc] initWithCapacity:0];	
		[sendingFileDicts setObject: aSendObject forKey: aSendObject.ftid];
	}
	else
	{
		while ([sendingFileDicts objectForKey: aSendObject.ftid]) 
		{
			[aSendObject release];
			aSendObject = [[SendFileObject alloc] initWithImageData: aData voiceClip: NO];
			aSendObject.chatViewData = self;
			aSendObject.fileName = [aFilepath lastPathComponent];
		}
		[sendingFileDicts setObject: aSendObject forKey: aSendObject.ftid];
	}
	aSendObject.chatViewData = self;
	
	NSMutableDictionary *aDict = [NSMutableDictionary dictionaryWithCapacity:0];
	NSString *name = contact.nickname;
	if (name == nil)
		name = contact.imid;
	[aDict setObject:[NSString stringWithFormat:NSLocalizedString(@"wait_to_accept",nil), name, aSendObject.fileName, aSendObject.fileSize / 1024 + 1] 
			  forKey:@"name"];
	
	[aDict setObject:aSendObject forKey:@"sendfile"];
	[messageArray addObject: aDict];
	[controller reloadMessageData];
	
	[[ClientNetWorkController getInstance] sendTransferInviteRequest: contact.jid
																 sid: sid 
	 															ftid: aSendObject.ftid
																name: aSendObject.fileName
																size: [aSendObject.imageData length]];
	[aSendObject release];
}

/*!
    @method     
    @abstract   Save Conversation
    @discussion save chat transtrips to local file.
*/

- (void)saveConversation:(NSString *)theName
{
	NSDictionary *dict = nil;
	NSString *string = nil;
	NSString *messages = @"";
	FILE *file = NULL;
	if ([messageArray count] > 0){
		LogMethod();
		NSString * fileName = nil;
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;

		if (contact.imid != nil){
			fileName = [NSString stringWithFormat:@"%@-%@",contact.imid,theName];
		}else {
//			NSLog(@">>> myAccount is null,");
			return;
		}
		
		NSArray *filePaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES); 
		NSString *directory = [filePaths objectAtIndex: 0];
		NSString *fileString = [directory stringByAppendingPathComponent:fileName];
		file  = fopen([fileString UTF8String], "a+");
		NSData *data;
		if (file != NULL);
		{
			messages = @"";
			for (dict in messageArray)
			{
				string = [dict objectForKey:@"name"];
				
				if (string != nil)
					messages = [messages stringByAppendingString:string];
				
				string = [dict objectForKey:@"message"];
				if (string != nil)
				{
					messages = [messages stringByAppendingString:@"[#####]"];
					messages = [messages stringByAppendingString:string];
				}
				
				string = [dict objectForKey:@"reason"];
				if (string != nil)
				{
					messages = [messages stringByAppendingString:@"[*****]"];
					messages = [messages stringByAppendingString:string];
				}
				
				data = [messages dataUsingEncoding:NSUTF8StringEncoding];
				fseek(file,0, SEEK_END);
				fwrite([data bytes], 1, [data length], file);
				
				messages = @"[***##]";
			}
			
			fclose(file);
			NSMutableDictionary *conversations = [NSMutableDictionary dictionaryWithDictionary:[appdelegate.infoDict objectForKey:@"conversation"]];
			NSArray *oldConversations = [conversations objectForKey:appdelegate.myAccount];
			
			NSMutableArray *newConversations = nil;
			if (oldConversations != nil)
				newConversations = [NSMutableArray arrayWithArray:oldConversations];
			else
				newConversations = [NSMutableArray arrayWithCapacity:0];
			
			if ([newConversations indexOfObject:fileName] == NSNotFound){
				[newConversations addObject:fileName];	
            }
			
			[conversations setObject:newConversations forKey:appdelegate.myAccount];
			[appdelegate.infoDict setObject:conversations forKey:@"conversation"];
			[self saveTranscriptsSessons];
		}	
	}
}

- (void)receivedMessage:(NSDictionary*)theInfo{
    
	NSDateComponents *timeComponents = nil;
	NSString *theMessage = [theInfo objectForKey:@"message"];
	NSString *theReason = [theInfo objectForKey:@"reason"];
	BOOL nudge = [[theInfo objectForKey:@"nudge"] boolValue];
	NSString *stringTime = [theInfo objectForKey: @"recvtime"];
    NSInteger anIndex = [messageArray count];
    NSInteger aSeconds = 0;
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	if (stringTime == nil){
		timeComponents = [[NSCalendar currentCalendar] components:(NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit )
														 fromDate:[NSDate date]];
		stringTime = [NSString stringWithFormat:@"%04d-%02d-%02d %02d:%02d:%02d",
					  [timeComponents year], [timeComponents month], [timeComponents day],
					  [timeComponents hour],[timeComponents minute],[timeComponents second]];
	}
    else {
        aSeconds = [self stringTimeToSeconds: stringTime];
        if (aSeconds > 0){
            NSNumber* aSecondsObject = [NSNumber numberWithInt: aSeconds];
            anIndex = [self indexOfMsgTime: aSecondsObject];
            [dict setObject: aSecondsObject forKey: kRecvMsgTimeKey];
        }
    }
	
	if (theReason == nil){
        
		NSString *name = contact.nickname;
		if ([name length] == 0){
			name = contact.imid;
		}
		
        if ([name length] > 15){
            name = [name substringToIndex:15];
            name = [name stringByAppendingString:@"..."];
        }
        if (theMessage != nil){
            [dict setObject:[NSString stringWithFormat:@"%@ %@ (%@)",name,NSLocalizedString(@"qtn_msn_say_name",nil),stringTime] forKey:@"name"];
        }
        else{
            if ([[theInfo objectForKey: @"recvtime"] length] > 0){
                NSString* aString = [NSString stringWithFormat:NSLocalizedString(@"nudge",nil),name];
                aString = [NSString stringWithFormat: @"%@\n%@", [theInfo objectForKey: @"recvtime"], aString];
                [dict setObject: aString forKey:@"name"];
            }
            else {
                [dict setObject:[NSString stringWithFormat:NSLocalizedString(@"nudge",nil),name] forKey:@"name"];
            }
        }
        
		NSDictionary *info = [NSDictionary dictionaryWithObject:[self contact] forKey:@"key"];
		[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
	}
	else{
		[dict setObject:NSLocalizedString(@"not_delivered",nil) forKey:@"name"];
	}
	
	if (nudge && theMessage == nil)
	{
		nudge = [[[MSNAppDelegate globalAppDelegate].infoDict objectForKey:kNotifyNudgeKey] boolValue];
		if (nudge)
		{
			if (isSound)
			{
				NSString *sndpath = [[NSBundle mainBundle] pathForResource:@"nudge" ofType:@"wav" inDirectory:@"/"];
				if (sndpath != nil)
				{
					if (nudgeSound == 0)
					{
						CFURLRef baseURL = (CFURLRef)[[NSURL alloc] initFileURLWithPath:sndpath];
						AudioServicesCreateSystemSoundID (baseURL, &nudgeSound);
						CFRelease(baseURL);	
					}
					if (nudgeSound) 
					{
						AudioServicesPlaySystemSound (nudgeSound);
					}
				}
			}
			AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
		}
	}
	else	if (messageSound && isSound) 
		AudioServicesPlaySystemSound (messageSound);
	
	if (theMessage != nil)
		[dict setObject:theMessage forKey:@"message"];
	
	if (theReason != nil)
		[dict setObject:theReason forKey:@"reason"];

	
	NSString *sImageId = [theInfo objectForKey:@"kHiImageID"];
	if (sImageId != nil)
		[dict setObject:[theInfo objectForKey:@"kHiImageID"] forKey:@"kHiImageID"];

    [messageArray insertObject: dict atIndex: anIndex];
	
	[controller reloadMessageData];
}


- (void)receivedVoiceClip:(NSDictionary*)theInfo
{
	NSString *fileName = nil;
	
	NSString *string = [theInfo objectForKey:@"type"];
	if (string != nil)
	{
		NSRange range = [string rangeOfString:@"/" options:NSBackwardsSearch];
		if (range.location != NSNotFound && range.location + 1 < [string length])
		{
			string = [string substringFromIndex:range.location + 1];
		}
	}
	
	if (string == nil)
		string = @"wav";
	
	NSDateComponents *timeComponents = [[NSCalendar currentCalendar] components:( NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit ) 
																	   fromDate:[NSDate date]];
	fileName = [NSString stringWithFormat:@"%02d-%02d-%02d.%@",[timeComponents hour],[timeComponents minute],[timeComponents second],string];
	
	if (fileName != nil)
	{
		ReceivedFileObject *obj = [[ReceivedFileObject alloc] init];
		
		obj.fileName = fileName;
		obj.fileSize = [theInfo objectForKey:@"size"];
		/*if (obj.fileSize == nil)
		 {
		 int size = [[theInfo objectForKey:@"size"] intValue];
		 obj.fileSize = [NSString stringWithFormat:@"%d",size*3/4];//base64 about 3:4
		 }*/
		obj.transferid = fileName;
		obj.isVoiceClip = YES;
		obj.chatViewData = self;
		
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
		//[dict setObject:@"server" forKey:@"from"];
		
		NSString *name = contact.nickname;
		if (name == nil)
			name = contact.imid;
		
		if ([obj.fileSize intValue] > 0)
		{
			[dict setObject:[NSString stringWithFormat:
							 NSLocalizedString(@"receive_voice_clip_kb",nil),name,[obj.fileSize intValue] / 1024 + 1] forKey:@"name"];			
		}
		else
		{
			[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"receive_voice_clip",nil),name] forKey:@"name"];
		}

		[dict setObject:obj forKey:@"receivedVoice"];
		[messageArray addObject:dict];
		
		if (messageSound && isSound) 
			AudioServicesPlaySystemSound (messageSound);
		
		[controller reloadMessageData];
		
		if (receivingFileDicts == nil)
			receivingFileDicts = [[NSMutableDictionary alloc] initWithCapacity:0];	
		if (obj.transferid != nil)
			[receivingFileDicts setObject:obj forKey:obj.transferid];
		//add by liubing for bug 1371
		NSDictionary *info = [NSDictionary dictionaryWithObject:[self contact] forKey:@"key"];
		[[NSNotificationCenter defaultCenter] postNotificationName:ShowChatViewNotification object:self userInfo:info];
		//
		NSString *url = [theInfo objectForKey:@"url"];
		if (url != nil)
			[obj receivedUrl:url];
		else
			[obj receivedData:theInfo];
		[obj release];
		

	}
}


- (void)receivedFile:(NSDictionary*)theInfo
{	
	ReceivedFileObject *obj = [[ReceivedFileObject alloc] init];
	
	obj.fileName = [theInfo objectForKey:@"name"];
	obj.fileSize = [theInfo objectForKey:@"size"];
	obj.transferid = [theInfo objectForKey:@"transferid"];
	obj.ftid = [theInfo objectForKey:@"ftid"];
	obj.chatViewData = self;
	
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
    if (dict == nil) return;
	
	NSString *name = contact.nickname;
	if (name == nil)
		name = contact.imid;
	
	[dict setObject:[NSString stringWithFormat:
					 NSLocalizedString(@"receive_file",@""),name,obj.fileName,[obj.fileSize intValue] / 1024 + 1] forKey:@"name"];
	
	[dict setObject:obj forKey:@"receivedfile"];
	if (receivingFileDicts == nil)
		receivingFileDicts = [[NSMutableDictionary alloc] initWithCapacity:0];	
	if (obj.transferid != nil)
		[receivingFileDicts setObject:obj forKey:obj.transferid];
	[obj release];
	[messageArray addObject:dict];
	
	if (messageSound && isSound) 
		AudioServicesPlaySystemSound (messageSound);
	
	[controller reloadMessageData];
}


- (void)userAction:(BOOL)accept from:(id)sender
{
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	//[dict setObject:@"server" forKey:@"from"];
	
	NSString* actionStr = NSLocalizedString(@"qtn_accept",nil);
	if (!accept)
		actionStr = NSLocalizedString(@"qtn_reject",nil);
	
	NSString *name = contact.nickname;
	if (name == nil)
		name = contact.imid;
	if (name != nil)
	{
		if ([name length] > 15)
		{
			name = [name substringToIndex:15];
			name = [name stringByAppendingString:@"..."];
		}
		if (accept)
		{
			isSendReceive = RECEIVE;
			[dict setObject:[NSString stringWithFormat:
							 NSLocalizedString(@"accept_file",nil),actionStr, ((ReceivedFileObject*)sender).fileName, name] forKey:@"name"];
		}
		else
		{
			//isSendReceive = NONE;
			[dict setObject:[NSString stringWithFormat:
							 NSLocalizedString(@"reject_file",nil),actionStr,((ReceivedFileObject*)sender).fileName,name] forKey:@"name"];
		}
	}
	if (accept)
	{
		[dict setObject:sender forKey:@"progress"];
	}
	else
	{
		if ([receivingFileDicts objectForKey:((ReceivedFileObject*)sender).transferid] != nil){
			[receivingFileDicts removeObjectForKey:((ReceivedFileObject*)sender).transferid];
		}
	}
	[messageArray addObject:dict];

	[controller reloadMessageData];
}


- (void)receivedAction:(NSDictionary*)theInfo
{
	NSString *theAction = [theInfo objectForKey:@"action"];
	if (theAction != nil)
	{
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
		
		NSString *name = contact.nickname;
		if (name == nil)
			name = contact.imid;
		
		if ([theAction compare:@"cancel"] == NSOrderedSame)
		{
			NSString *transferid = [theInfo objectForKey:@"transferid"];
			ReceivedFileObject *receiveObj = [receivingFileDicts objectForKey:transferid];
			if (receiveObj != nil)
			{
				[receiveObj receivedAction:theAction];
				[dict setObject:[NSString stringWithFormat:
								 NSLocalizedString(@"cancel_action",nil),name,receiveObj.fileName] forKey:@"name"];
				[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:receiveObj];
				[receivingFileDicts removeObjectForKey:transferid];
			}
			else
			{
				SendFileObject *sendObj = nil;
				NSString *key = nil;
				for (key in [sendingFileDicts allKeys])
				{
					sendObj = [sendingFileDicts objectForKey:key];
					if ([sendObj.transferid compare:transferid] == NSOrderedSame)
					{
						[sendObj receivedAction:theAction];
						[dict setObject:[NSString stringWithFormat:
										 NSLocalizedString(@"cancel_action",nil),name,sendObj.fileName] forKey:@"name"];
						[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:sendObj];
						[sendingFileDicts removeObjectForKey:key];
						break;
					}
				}
			}
		}
		if ([theAction compare:@"complete_ack"] == NSOrderedSame)
		{
			if (isSendReceive == RECEIVE)
			{
				NSString *transferid = [theInfo objectForKey:@"transferid"];
				ReceivedFileObject *obj = [receivingFileDicts objectForKey:transferid];
				if (obj != nil)
				{
					NSString *url = [theInfo objectForKey:@"url"];
					if (url != nil)
						[obj receivedUrl:url];
				}	
			}
			else if (isSendReceive == SEND)
			{
				NSString *ftid = [theInfo objectForKey:@"ftid"];
				SendFileObject *obj = [sendingFileDicts objectForKey:ftid];
				if (obj != nil)
				{
					[obj receivedAction:theAction];
					//[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"send_file_complete",nil),obj.fileName,obj.fileSize/1024 + 1] forKey:@"name"];
				}
			}
		}
		else
		{
			NSString *theFtid = [theInfo objectForKey:@"ftid"];
			SendFileObject *obj = [sendingFileDicts objectForKey:theFtid];
			if ([theAction compare:@"accept"] == NSOrderedSame)
			{
				if (obj != nil)
				{
					obj.transferid = [theInfo objectForKey:@"transferid"];
					[obj receivedAction:theAction];
					[dict setObject:[NSString stringWithFormat:
									 NSLocalizedString(@"accept_action",nil),name,NSLocalizedString(@"qtn_accept",nil),obj.fileName] forKey:@"name"];
					[dict setObject:obj forKey:@"progress"];
				}
			}
			else if ([theAction compare:@"reject"] == NSOrderedSame)
			{
				if (obj != nil)
				{
					[obj receivedAction:theAction];
					[dict setObject:[NSString stringWithFormat:
									 NSLocalizedString(@"reject_action",nil),name,NSLocalizedString(@"qtn_reject",nil),obj.fileName] forKey:@"name"];
					[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:obj];
					[sendingFileDicts removeObjectForKey:theFtid];
				}
			}
		}
		[messageArray addObject:dict];
		[controller reloadMessageData];
	}
	
}

- (void)receivedStatus:(NSDictionary*)theInfo
{
	NSString *theFtid = [theInfo objectForKey:@"ftid"];
	NSString *status = [theInfo objectForKey:@"status"];
	SendFileObject *obj = [sendingFileDicts objectForKey:theFtid];
	if (obj != nil)
	{
		if ([status compare:@"ok"] == NSOrderedSame)
		{
			[obj sendNextFile];
		}
		else if ([status compare:@"fail"] == NSOrderedSame)
		{
			//[obj sendFile];
		}
		else if ([status compare:@"canceld"] == NSOrderedSame)
		{
			
		}
	}
}

- (void)receivedData:(NSDictionary*)theInfo
{
	NSString *transferid = [theInfo objectForKey:@"transferid"];
	if (transferid != nil)
	{
		ReceivedFileObject *obj = [receivingFileDicts objectForKey:transferid];
		if (obj != nil)
			[obj receivedData:theInfo];	
	}
}

- (void)sendFinished:(SendFileObject*)sender
{
	BOOL remove = YES;
	if (sender.isVoiceClip)
	{
		if (!sender.sendByServer)
			remove = NO;
	}
	else
	{
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];

		[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"send_file_complete",nil), sender.fileName,sender.fileSize / 1024 + 1] 
				 forKey:@"name"];
		
		[messageArray addObject:dict];
		
		[controller reloadMessageData];
	}
	if (remove)
	{
		SendFileObject* aSendObject = [sendingFileDicts objectForKey:sender.ftid];
		if (aSendObject != nil){
			aSendObject = [aSendObject retain];
			[sendingFileDicts removeObjectForKey:sender.ftid];
		}
		if (!sender.isVoiceClip)
			[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:sender];
		[aSendObject release];
	}
}

- (void)receiveFinished:(ReceivedFileObject*)sender saved:(BOOL)flag
{
	if (sender.isVoiceClip)
	{
		if ([receivingFileDicts objectForKey:sender.transferid] != nil)
			[receivingFileDicts removeObjectForKey:sender.transferid];
	}
	else
	{
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
		//[dict setObject:@"server" forKey:@"from"];
		NSString *string = [NSString stringWithFormat:NSLocalizedString(@"receive_file_complete",nil),sender.fileName, [sender.fileSize intValue] / 1024 + 1];
		if (flag)
			string = [string stringByAppendingString:NSLocalizedString(@"save_photo",nil)];
		else
			string = [string stringByAppendingString:NSLocalizedString(@"save_failed",nil)];
		
		[dict setObject:string
				 forKey:@"name"];
		
		[messageArray addObject:dict];
		
		ReceivedFileObject* aRecvFileObject = [receivingFileDicts objectForKey:sender.transferid];
		if (aRecvFileObject != nil){
			aRecvFileObject = [aRecvFileObject retain];
			[receivingFileDicts removeObjectForKey:sender.transferid];
		}
		[[NSNotificationCenter defaultCenter] postNotificationName:TransferEndNotification object:sender];
		[aRecvFileObject release];
		[controller reloadMessageData];
	}
}

- (void)receiveCanceled:(id)sender
{
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	//[dict setObject:@"server" forKey:@"from"];
	if ([sender isKindOfClass:[ReceivedFileObject class]])
	{
		ReceivedFileObject* obj = (ReceivedFileObject*)sender;
		[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"cancel_file",nil),obj.fileName, [obj.fileSize intValue] / 1024 + 1] 
				 forKey:@"name"];
		if ([receivingFileDicts objectForKey:obj.transferid] != nil)
			[receivingFileDicts removeObjectForKey:obj.transferid];
	}
	else if ([sender isKindOfClass:[SendFileObject class]])
	{
		SendFileObject *obj = (SendFileObject*)sender;
		[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"cancel_file",nil),obj.fileName, obj.fileSize / 1024 + 1] 
				 forKey:@"name"];
		if ([sendingFileDicts objectForKey:obj.ftid] != nil)
			[sendingFileDicts removeObjectForKey:obj.ftid];
	}
	
	[messageArray addObject:dict];
	[controller reloadMessageData];
}

- (void)receivedGroupMessage:(NSDictionary*)theInfo
{
	BOOL find = NO;
	NSDateComponents *timeComponents = nil;
	BOOL nudge = [[theInfo objectForKey:@"nudge"] boolValue];
	NSString *theMessage = [theInfo objectForKey:@"message"];
	NSString *theJid = [theInfo objectForKey:@"jid"];
	NSString *theImid = [theInfo objectForKey:@"imid"];
	NSString *theReason = [theInfo objectForKey:@"reason"];
	NSString *stringTime = [theInfo objectForKey: @"recvtime"];
    NSInteger aSeconds = 0;
    NSInteger anIndex = [messageArray count];
	NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
	
	if (stringTime == nil){
		timeComponents = [[NSCalendar currentCalendar] components:(NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit )
														 fromDate:[NSDate date]];
		stringTime = [NSString stringWithFormat:@"%04d-%02d-%02d %02d:%02d:%02d",
					  [timeComponents year], [timeComponents month], [timeComponents day],
					  [timeComponents hour],[timeComponents minute],[timeComponents second]];
	}
    else {
        aSeconds = [self stringTimeToSeconds: stringTime];
        if (aSeconds > 0){
            NSNumber* aSecondsObject = [NSNumber numberWithInt: aSeconds];
            anIndex = [self indexOfMsgTime: aSecondsObject];
            [dict setObject: aSecondsObject forKey: kRecvMsgTimeKey];
        }
    }
	
	if (theReason == nil)
	{
		PersonInfo *info = nil;
		if (theJid != nil)
		{
			for (info in persons)
			{
				if ([info.jid isEqualToString:theJid])
				{
					find = YES;
					break;
				}	
			}
		}
		else if (theImid != nil)
		{
			for (info in persons)
			{
				if ([info.imid isEqualToString:theImid])
				{
					find = YES;
					break;
				}	
			}
		}
		if (find)
		{
			NSString *name = info.nickname;
			if (name  == nil)
				name = info.imid;
			if (name != nil)
			{
				if ([name length] > 15)
				{
					name = [name substringToIndex:15];
					name = [name stringByAppendingString:@"..."];
				}
				if (theMessage != nil)
				{
					[dict setObject:[NSString stringWithFormat:@"%@ %@ (%@)",name,NSLocalizedString(@"qtn_msn_say_name",nil),stringTime] forKey:@"name"];
				}
				else if (nudge)
					[dict setObject:[NSString stringWithFormat:NSLocalizedString(@"nudge",nil),name] forKey:@"name"];
			}
		}
	}
	else
	{
		[dict setObject:NSLocalizedString(@"not_delivered",nil) forKey:@"name"];
	}
	
	if (nudge && theMessage == nil)
	{
		nudge = [[[MSNAppDelegate globalAppDelegate].infoDict objectForKey:kNotifyNudgeKey] boolValue];
		if (nudge)
		{
			if (isSound)
			{
				NSString *sndpath = [[NSBundle mainBundle] pathForResource:@"nudge" ofType:@"wav" inDirectory:@"/"];
				if (sndpath != nil)
				{
					if (nudgeSound == 0)
					{
						CFURLRef baseURL = (CFURLRef)[[NSURL alloc] initFileURLWithPath:sndpath];
						AudioServicesCreateSystemSoundID (baseURL, &nudgeSound);
						CFRelease(baseURL);	
					}
					if (nudgeSound) 
					{
						AudioServicesPlaySystemSound (nudgeSound);
					}
				}
			}
			AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
		}
	}
	else if (messageSound && isSound) 
		AudioServicesPlaySystemSound (messageSound);
	
	if (theMessage != nil)
		[dict setObject:theMessage forKey:@"message"];
	
	if (theReason != nil)
		[dict setObject:theReason forKey:@"reason"];
	
    [messageArray insertObject: dict atIndex: anIndex];
	[controller reloadMessageData];
}

- (void)participantChanged:(PersonInfo*)info type:(NSString*)type
{
	BOOL showMessage = NO;
	NSString *theName = nil;
	NSString *message = nil;
	BOOL aDidUpdateOnlineContacts = NO;
	if ([type compare:@"enter"] == NSOrderedSame)
	{
		message = NSLocalizedString(@"qtn_chat_session_chng_enter",nil);
		if (info.jid != nil){
			info = [[ContactList getInstance] personWithJid:info.jid];
			if (info != nil && [persons indexOfObject:info] == NSNotFound){
				showMessage = YES;
				theName = (info.nickname == nil) ? [info.imid copy] : [info.nickname copy];
				controller.title = [controller.title stringByAppendingString:theName];
				[persons addObject:info];
				[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:self];
				aDidUpdateOnlineContacts = YES;
			}
		}
		else if (info.imid != nil){
			BOOL found = NO;
			PersonInfo *person = nil;
			for (person in persons){
				if ([person.imid isEqualToString: info.imid]){
					found = YES;
					break;
				}
			}
			if (!found){
				showMessage = YES;
				theName = (info.nickname == nil) ? [info.imid copy] : [info.nickname copy];
				controller.title = [controller.title stringByAppendingString:theName];
				[persons addObject:info];
				[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:self];
				aDidUpdateOnlineContacts = YES;
			}
		}
	}
	else if ([type compare:@"leave"] == NSOrderedSame)
	{
		message = NSLocalizedString(@"qtn_chat_session_chng_leave",nil);
		if (info.jid != nil){
			info = [[ContactList getInstance] personWithJid:info.jid];
			if (info != nil && ([persons indexOfObject:info] != NSNotFound)){
				showMessage = YES;
				theName = (info.nickname == nil) ? [info.imid copy] : [info.nickname copy];
				[persons removeObject:info];
				
				aDidUpdateOnlineContacts = YES;
				
				if (info == contact){
					if ([persons count] > 0){
						self.contact = [persons objectAtIndex:0];
						[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:self];
					}
				}
			}
		}
		else if (info.imid != nil){
			PersonInfo *person = nil;
			for (person in persons){
				if ([person.imid isEqualToString: info.imid]){
					showMessage = YES;
					theName = (person.nickname == nil) ? [person.imid copy] : [person.nickname copy];
					
					aDidUpdateOnlineContacts = YES;
					
					[persons removeObject:person];
					
					if (person == contact){
						if ([persons count] > 0){
							self.contact = [persons objectAtIndex:0];
							[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:self];
						}
					}
					break;
				}
			}
		}
	}
	
	[controller setTitleText];
	
	if ([persons count] == 0)
		[controller setSendBUttonState:NO];
	else //if ([persons count] == 1 && contact.jid != nil)
		[controller updatePressionButtonState];
	
	if (aDidUpdateOnlineContacts){
		UINavigationController *aNavigation = [[MSNAppDelegate globalAppDelegate] chatViewNavigation];
		UIViewController* aController = [aNavigation topViewController];
		if ([aController isKindOfClass: [OnlineListController class]] &&
			[aController respondsToSelector: @selector(participantChanged)]){
			[aController participantChanged];
		}
	}
	
	if (showMessage)
	{
		NSDateComponents *timeComponents = [[NSCalendar currentCalendar] components:(NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit ) 
																		   fromDate:[NSDate date]];
		NSString *stringTime = [NSString stringWithFormat:@"%04d-%02d-%02d %02d:%02d:%02d",
								[timeComponents year], [timeComponents month], [timeComponents day],
								[timeComponents hour],[timeComponents minute],[timeComponents second]];
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:0];
		//[dict setObject:@"server" forKey:@"from"];
		[dict setObject:[NSString stringWithFormat:@"(%@) ",stringTime] forKey:@"name"];
		[dict setObject:[NSString stringWithFormat:message,theName] forKey:@"message"];
		[messageArray addObject:dict];
		
		[controller reloadMessageData];
	}
	[theName release];
}

- (void)notifyPersonAdded: (NSNotification*)note
{
	RETURN_WHEN_LESS_MEMORY;
	if ([persons count] <= 0) return;
	
	if ([note object] != nil
		&& [[note object] isKindOfClass:[PersonInfo class]])
	{
		PersonInfo *person = [note object];	
		if (contact.jid != nil)
		{
			if ([person.jid isEqualToString: contact.jid]){
				self.contact.imid = person.imid;
				self.contact.sync = person.sync;
			}
			else if ([person.imid isEqualToString: contact.imid]){
				self.contact.jid = person.jid;
				self.contact.sync = person.sync;
				[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:self];
			}
		}
		else if (contact.imid != nil 
				 && [person.imid compare:contact.imid] == NSOrderedSame)
		{
			self.contact.jid = person.jid;
			self.contact.sync = person.sync;
			[[NSNotificationCenter defaultCenter] postNotificationName:ChangeChatviewContactNotification object:self];
		}
	}
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [messageArray count];
}


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	int height = 5;
	NSDictionary *dict = [messageArray objectAtIndex:indexPath.row];
	UILabel *label = [[UILabel alloc] initWithFrame:CGRectZero];
	label.lineBreakMode = UILineBreakModeWordWrap;
	label.font = [UIFont systemFontOfSize:16];
	label.numberOfLines = 100;
	if ([dict objectForKey:@"name"] != nil)
	{
		label.text = [dict objectForKey:@"name"];
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 305, 500) limitedToNumberOfLines:0];
		height += rc.size.height;	
	}
	if ([dict objectForKey:@"message"] != nil)
	{
		label.text = [dict objectForKey:@"message"];
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 305, 500) limitedToNumberOfLines:0];
		height += rc.size.height + 20;	
	}
	if ([dict objectForKey:@"receivedfile"] != nil)
	{
		height += 40;
	}
	if ([dict objectForKey:@"receivedVoice"] != nil)
	{
		height += 20;
	}
	if ([dict objectForKey:@"sendfile"] != nil)
	{
		height += 20;
	}
	if ([dict objectForKey:@"progress"] != nil)
	{
		height += 20;
	}
	if ([dict objectForKey:@"reason"] != nil)
	{
		label.text = [dict objectForKey:@"reason"];
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 305, 500) limitedToNumberOfLines:0];
		height += rc.size.height;
	}
	[label release];
	return height;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	UITableViewCell *cell = nil;
	
	NSString* anId = [NSString stringWithFormat: @"%@%d%d", myId, indexPath.row, clearCount];
	cell = [tableView dequeueReusableCellWithIdentifier: anId];
	
	BOOL aNewCell = NO;
	if (cell == nil){
		cell = [[[UITableViewCell alloc] initWithFrame: CGRectMake(0, 0, 320, 5)
									   reuseIdentifier: anId] autorelease];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		aNewCell = YES;
	}
	if (!aNewCell){
		[self clearViewsWithTag: CELL_TAG_FOR_BUTTON
					inSuperView: cell];
		[self clearViewsWithTag: CELL_TAG_FOR_LABEL
					inSuperView: cell];
		[self clearViewsWithTag: CELL_TAG_FOR_IMAGE
					inSuperView: cell];
	}
	
	UILabel *label = nil;
	int height = 0;
	NSDictionary *dict = [messageArray objectAtIndex:indexPath.row];
	NSString * str = [dict objectForKey: @"name"];
	if (str != nil)
	{
		
		label = [[ImageLabel alloc] initWithFrame:CGRectMake(0, 0, 305, 500)];
		label.tag = CELL_TAG_FOR_LABEL;
		label.lineBreakMode = UILineBreakModeWordWrap;
		label.font = [UIFont systemFontOfSize:16];
		label.text = str;
		
		CGRect rc = [label textRectForBounds:CGRectMake(0, 0, 305, 500) limitedToNumberOfLines:0];
		rc.origin.y += height;
		rc.size.width = 305;
		label.frame = rc;
		height += rc.size.height;
		label.numberOfLines = 100;
		label.textColor = [UIColor grayColor];	
		if ([dict objectForKey:@"reason"] != nil)
			label.font = [UIFont systemFontOfSize:12];
		
		[cell.contentView addSubview:label];
		[label release];
	}
	
	str = [dict objectForKey:@"kHiImageID"];
	
	if(str != nil)
	{
		UIImage *theImage = [UIImage imageNamed:str];
		if (theImage != nil)
		{
			UIImageView *hiView = [[UIImageView alloc] initWithImage:theImage];
			hiView.tag = CELL_TAG_FOR_IMAGE;
			hiView.frame = CGRectMake(60, -100, 200, 200);
			[cell addSubview:hiView];
			
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:1.0];
			[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
			[UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft forView:hiView cache:YES];
			hiView.frame = CGRectMake(0, 20, 40, 40);
			[UIView commitAnimations];
			
			[hiView release];
		}
		str = nil;
	}
	
	str = [dict objectForKey:@"message"];
	if (str != nil)
	{
		label = [[ImageLabel alloc] initWithFrame:CGRectZero];
		label.tag = CELL_TAG_FOR_LABEL;
		label.backgroundColor = [UIColor clearColor];
		label.lineBreakMode = UILineBreakModeWordWrap;
		if ([dict objectForKey:@"reason"] == nil)
		{
			label.font = [UIFont systemFontOfSize:16];
		}
		else
		{
			label.textColor = [UIColor grayColor];
		}
		label.numberOfLines = 100;
		label.text = str;
		CGRect rc;
		
		if ([dict objectForKey:@"kHiImageID"] != nil)
		{
			rc = [label textRectForBounds:CGRectMake(50, 25, 305, 500) limitedToNumberOfLines:0];				
		}else{
			rc = [label textRectForBounds:CGRectMake(10, 0, 305, 500) limitedToNumberOfLines:0];
		}
		
		rc.origin.y += height;
		rc.size.width = 305;
		rc.size.height += 20;
		label.frame = rc;
		height += rc.size.height;
		[cell.contentView addSubview:label];
		[label release];
	}
	
	SendFileObject *sendObj = [dict objectForKey:@"sendfile"];
	if (sendObj != nil)
	{
		if (sendObj.status == 0)
		{
			sendObj.cancelButton.frame = CGRectMake(20, height, 60, 20);
			sendObj.cancelButton.tag = CELL_TAG_FOR_BUTTON;
			[cell.contentView addSubview:sendObj.cancelButton];	
		}
	}
	
	ReceivedFileObject *receivedObj = [dict objectForKey:@"receivedfile"];
	if (receivedObj != nil)
	{
		if (receivedObj.status == 0)
		{
			
			CGRect rc = CGRectMake(10, height, 305, 20);
			label = [[ImageLabel alloc] initWithFrame:rc];
			label.tag = CELL_TAG_FOR_LABEL;
			label.text = NSLocalizedString(@"string_ask",nil);
			[cell.contentView addSubview:label];
			height += 20;
			
			receivedObj.acceptButton.frame = CGRectMake(20, height, 60, 20);
			receivedObj.acceptButton.tag = CELL_TAG_FOR_BUTTON;
			[cell.contentView addSubview:receivedObj.acceptButton];
			
			rc = CGRectMake(80, height, 30, 20);
			label = [[ImageLabel alloc] initWithFrame:rc];
			label.tag = CELL_TAG_FOR_LABEL;
			label.text = NSLocalizedString(@"string_or",nil);
			[cell.contentView addSubview:label];
			
			[label release];
			receivedObj.rejectButton.frame = CGRectMake(110, height, 60, 20);
			receivedObj.rejectButton.tag = CELL_TAG_FOR_BUTTON;
			[cell.contentView addSubview:receivedObj.rejectButton];
			
			rc = CGRectMake(170, height, 130, 20);
			label = [[ImageLabel alloc] initWithFrame:rc];
			label.tag = CELL_TAG_FOR_LABEL;
			label.text = NSLocalizedString(@"string_invitation",nil);
			[cell.contentView addSubview:label];
			[label release];
			
			height += 20;	
		}
	}
	receivedObj = [dict objectForKey:@"receivedVoice"];
	if (receivedObj != nil)
	{
		receivedObj.progressBar.frame = CGRectMake(20, height + 5, 200, 10);
		receivedObj.progressBar.tag = CELL_TAG_FOR_PROGRESS;
		[cell.contentView addSubview:receivedObj.progressBar];
		receivedObj.playButton.frame = CGRectMake(20, height, 60, 20);
		receivedObj.playButton.tag = CELL_TAG_FOR_BUTTON;
		[cell.contentView addSubview:receivedObj.playButton];
		
		// Fake save, used to confirm the test case.
		UIButton *fakeSaveButton = [UIButton buttonWithType: UIButtonTypeRoundedRect];
		fakeSaveButton.tag = CELL_TAG_FOR_BUTTON;
		fakeSaveButton.frame = CGRectMake(100, height, 60, 20);
		[fakeSaveButton setTitle:NSLocalizedString(@"Save",nil) forState:UIControlStateNormal];
		[fakeSaveButton addTarget:self action:@selector(fakeSaveButtonCallback:) forControlEvents:UIControlEventTouchDown];
		[cell.contentView addSubview:fakeSaveButton];
		
		UIButton *aButton = [[UIButton buttonWithType: UIButtonTypeRoundedRect] retain];
		aButton.tag = CELL_TAG_FOR_BUTTON;
		aButton.frame = CGRectMake(180, height, 60, 20);
		[aButton setTitle:NSLocalizedString(@"qtn_delete_attache",nil) forState:UIControlStateNormal];
		[aButton addTarget: self action: @selector(clearVoiceMsg:) forControlEvents: UIControlEventTouchDown];
		[cell.contentView addSubview: aButton];
		[aButton release];
		height += 20;
	}
	id obj = [dict objectForKey:@"progress"];
	if (obj != nil)
	{
		if ([obj isKindOfClass:[SendFileObject class]])
		{
			sendObj = (SendFileObject*)obj;
			if (sendObj.isVoiceClip)
			{
				sendObj.progressBar.frame = CGRectMake(20, height + 5, 200, 10);
				sendObj.progressBar.tag = CELL_TAG_FOR_PROGRESS;
				[cell.contentView addSubview:sendObj.progressBar];
				sendObj.playButton.frame = CGRectMake(20, height, 60, 20);
				sendObj.playButton.tag = CELL_TAG_FOR_BUTTON;
				[cell.contentView addSubview:sendObj.playButton];
				
				// Fake save, used to confirm the test case.
				UIButton *fakeSaveButton = [UIButton buttonWithType: UIButtonTypeRoundedRect];
				fakeSaveButton.tag = CELL_TAG_FOR_BUTTON;
				fakeSaveButton.frame = CGRectMake(100, height, 60, 20);
				[fakeSaveButton setTitle:NSLocalizedString(@"Save",nil) forState:UIControlStateNormal];
				[fakeSaveButton addTarget:self action:@selector(fakeSaveButtonCallback:) forControlEvents:UIControlEventTouchDown];
				[cell.contentView addSubview:fakeSaveButton];
				
				UIButton *aButton = [UIButton buttonWithType: UIButtonTypeRoundedRect];
				aButton.tag = CELL_TAG_FOR_BUTTON;
				aButton.frame = CGRectMake(180, height, 60, 20);
				[aButton setTitle:NSLocalizedString(@"qtn_delete_attache",nil) forState:UIControlStateNormal];
				[aButton addTarget: self action: @selector(clearVoiceMsg:) forControlEvents: UIControlEventTouchDown];
				[cell.contentView addSubview: aButton];
			}
			else if (sendObj.status == 1)
			{
				sendObj.progressBar.frame = CGRectMake(20, height + 5, 200, 10);
				sendObj.progressBar.tag = CELL_TAG_FOR_PROGRESS;
				[cell.contentView addSubview:sendObj.progressBar];
				sendObj.cancelButton.frame = CGRectMake(230, height, 60, 20);
				sendObj.cancelButton.tag = CELL_TAG_FOR_BUTTON;
				[cell.contentView addSubview:sendObj.cancelButton];
				height += 20;	
			}
		}
		else if ([obj isKindOfClass:[ReceivedFileObject class]])
		{
			receivedObj = (ReceivedFileObject*)obj;	
			if (receivedObj.status == 1)
			{
				receivedObj.progressBar.frame = CGRectMake(20, height + 5, 200, 10);
				receivedObj.progressBar.tag = CELL_TAG_FOR_PROGRESS;
				[cell.contentView addSubview:receivedObj.progressBar];
				receivedObj.cancelButton.frame = CGRectMake(230, height, 60, 20);
				receivedObj.cancelButton.tag = CELL_TAG_FOR_BUTTON;
				[cell.contentView addSubview:receivedObj.cancelButton];
				height += 20;	
			}
		}
	}
	str = [dict objectForKey:@"reason"];
	if (str != nil)
	{
		label = [[UILabel alloc] initWithFrame:CGRectZero];
		label.tag = CELL_TAG_FOR_LABEL;
		label.lineBreakMode = UILineBreakModeWordWrap;
		label.font = [UIFont systemFontOfSize:16];
		label.numberOfLines = 100;
		label.text = str;
		CGRect rc = [label textRectForBounds:CGRectMake(10, 0, 305, 500) limitedToNumberOfLines:0];
		rc.origin.y += height;
		rc.size.width = 305;
		label.frame = rc;
		[cell.contentView addSubview:label];
		[label release];
	}
	return cell;
}

- (void)garbage{
	[self saveTranscripts];
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver: self
				  name: PersonAddedNotification
				object: nil];
	
	[nc removeObserver: self
				  name: AddMessageToChatViewNotification
				object: nil];
	
	if (messageSound > 0){
		AudioServicesDisposeSystemSoundID (messageSound);
		messageSound = 0;
	}
	if (nudgeSound > 0){
		AudioServicesDisposeSystemSoundID (nudgeSound);
		nudgeSound = 0;
	}
	
	[sendingFileDicts release];
	sendingFileDicts = nil;
	[receivingFileDicts release];
	receivingFileDicts = nil;

	self.contact = nil;
	self.persons = nil;
	controller = nil;
	self.messageArray = nil;
	
	self.sendingText = nil;
	[myId release];
	myId = nil;
	self.sid = nil;
	self.image = nil;
	[voiceClipPath release];
	voiceClipPath = nil;
}

- (void)cancelTransferingFile{
	SendFileObject *aSendObj = nil;
	NSArray* aValues = [sendingFileDicts allValues];
	for (aSendObj in aValues){
		[aSendObj buttonClicked: aSendObj.cancelButton];
	}
	[sendingFileDicts removeAllObjects];
	isSendReceive = NONE;
}

- (void)rejectReveicingFile{
	ReceivedFileObject *aReceiveObj = nil;
	NSArray* aValues = [receivingFileDicts allValues];
	for (aReceiveObj in aValues){
		[aReceiveObj buttonClicked: aReceiveObj.cancelButton];
	}
	[receivingFileDicts removeAllObjects];
	isSendReceive = NONE;
}

@end
