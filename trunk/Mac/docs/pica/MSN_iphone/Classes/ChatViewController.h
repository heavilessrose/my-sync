//
//  ChatViewController.h
//  msn_for_mac
//
//  Created by  apple on 08-9-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PersonInfo;
@class AudioRecorder;
@class AudioPlayer;
@class ReceivedMessageView;
@class ChatViewData;
@class pcAudioClipsViewController;
@class pcAudioActionSheet;

@interface ChatViewController : UIViewController<UITextViewDelegate,  UIAlertViewDelegate, UIActionSheetDelegate,
													UIImagePickerControllerDelegate, UINavigationControllerDelegate>
{
	ReceivedMessageView *receivedMessageView;
	UITextView *sendingMessageView;
	UIBarButtonItem *sendButton;
	UIBarButtonItem *addContactButton;
	UISegmentedControl *pressionButton;
	UIAlertView *pressionPanel;
	pcAudioActionSheet *recorderView;
	UIImagePickerController* picker;
	AudioRecorder *recorder;
	AudioPlayer *player;
	BOOL sended;
	BOOL restore;
	NSString *myName;
	NSArray *pressionStrings;
	ChatViewData *chatViewData;
	NSInteger actionReason;
	pcAudioClipsViewController*		audioClipsController;
	BOOL forceScrollChatView;
	
	UIButton*		blockButton;
}


- (UIImage*)getImage;
- (void)reloadMessageData;

- (void)setName;
- (void)setTitleText;
- (ChatViewData*)chatViewData;
- (void)setChatViewData:(ChatViewData*)theData;

- (void)setClearButtonState:(BOOL)flag;
- (void)setSendBUttonState:(BOOL)flag;

- (void)updatePressionButtonState;

- (void)showPlayView:(id)sender;
- (void)record:(id)sender;
- (void)cancelRecord:(id)sender;
- (void)play:(id)sender;
- (void)closePlay:(id)sender;
- (void)audioCallBack:(NSNumber*)theBytes;
- (void)audioStateChanged;

- (IBAction)showListView:(id)sender;
- (IBAction)showPressionPanel:(id)sender;
- (IBAction)pressionSelected:(id)sender;
- (IBAction)close:(id)sender;
- (IBAction)clear:(id)sender;
- (IBAction)send:(id)sender;
- (IBAction)managerContact:(id)sender;

- (IBAction)block: (id)sender;

- (void)showPhotoLibrary:(id)sender;
- (void)capturePhoto:(id)sender;

- (NSIndexPath*)findIndexPathByCell: (UITableViewCell*)aCell;

- (void)notifyTransferBegin: (NSNotification*)note;
- (void)notifyTransferEnd: (NSNotification*)note;

- (void)notifyKeyboardWillShow: (NSNotification*)note;
- (void)notifyKeyboardWillHide: (NSNotification*)note;

- (void)notifyAuthInfoChanged: (NSNotification*)note;
- (void)notifyPersonStateChanged: (NSNotification*)note;

- (void)notifyTouchInReceivedMessageView: (NSNotification*)note;
- (void)notifyDismissAlertView: (NSNotification*)note;
- (void)notifyStateChanged: (NSNotification*)note;


@property (nonatomic, retain) UIAlertView *pressionPanel;
@property (nonatomic, retain) UIImagePickerController* picker;
@property (nonatomic, retain) NSString *myName;
@property (nonatomic, retain) ChatViewData *chatViewData;
@property (nonatomic, retain) ReceivedMessageView* receivedMessageView;
@property (nonatomic, retain) pcAudioActionSheet* recorderView;
@property (nonatomic, retain) UIButton*	blockButton;

//when logout, we need clear all resource, because the object is always existent after app finished startup.
- (void)garbage;

@end
