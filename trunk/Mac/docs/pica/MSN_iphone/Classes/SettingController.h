//
//  SettingController.h
//  msn_for_mac
//
//  Created by  apple on 08-10-6.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

enum GroupIndex
{
	StateIndex = 0,
	DisPlayNameIndex,
	ActivityOfFriends,
};

@class PersonInfo;
@class HttpClient;
@class MyUITextSwitchTableCell;

@interface SettingController : UITableViewController <UITextFieldDelegate,UIImagePickerControllerDelegate, UINavigationControllerDelegate> 
{
	UITextField *nickNameTextfield;
	UITextField *impresaTextfield;
	UIImageView *imageView;
	UIImagePickerController* picker;
	NSMutableDictionary *infoDictionary;
	PersonInfo *setting;
	HttpClient *httpClient;
	BOOL isEditing;
	
	NSArray *elementsItems;
	
	MyUITextSwitchTableCell*	conversationsCell;
}

- (UITableViewCell*)tableView:(UITableView *)tableView StateCellAtIndexPath:(NSIndexPath*)indexPath;
- (UITableViewCell*)tableView:(UITableView *)tableView DisPlayNameCellAtIndexPath:(NSIndexPath*)indexPath;
- (UITableViewCell*)tableView:(UITableView *)tableView FlowControlCellAtIndexPath:(NSIndexPath*)indexPath;
- (UITableViewCell*)tableView:(UITableView *)tableView PictureCellAtIndexPath:(NSIndexPath*)indexPath;
- (UITableViewCell*)tableView:(UITableView *)tableView ActivityCellAtIndexPath:(NSIndexPath*)indexPath;

- (BOOL)settingChanged;
- (void)setAuthInfo:(id)sender;
- (void)showPhotoLibrary:(id)sender;
- (void)capturePhoto:(id)sender;

- (void)sendportraitData:(NSString*)theString;
- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict;
- (void)setViewMovedUp:(BOOL)movedUp;
- (void)notifyKeyboardWillShow: (NSNotification*)note;
- (void)notifyKeyboardWillhide: (NSNotification*)note;

- (IBAction)switchAction: (id)sender;

@property (nonatomic, retain) UITextField *nickNameTextfield;
@property (nonatomic, retain) UITextField *impresaTextfield;
@property (nonatomic, retain) UIImagePickerController* picker;
@property (nonatomic, retain) UIImageView *imageView;	
@property (nonatomic, retain) PersonInfo *setting;
@property (nonatomic, retain) NSArray *elementsItems;
@end
