//
//  SettingController.m
//  msn_for_mac
//
//  Created by  apple on 08-10-6.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SettingController.h"
#import "ClientNetWorkController.h"
#import "HttpClient.h"
#import "MSNAppDelegate.h"
#import "PersonInfo.h"
#import "NSData+base64.h"
#import "define.h"
#import "API.h"
#import "public.h"
#import "DDXML.h"
#import "AdvertisementLabel.h"
#import "ImageLabel.h"
#import "ListViewController.h"
#import "ImageButton.h"
#import "MyUITextSwitchTableCell.h"

#define kOFFSET_FOR_KEYBOARD					150.0

enum  {
	TYPE_FRIEND = 0,
	TYPE_UPDATE_ICON,
	TYPE_UPDATE_PROFILE,
	TYPE_UPDATE_STATUS,
	TYPE_BLOG,
	TYPE_SAY_HI,
	TYPE_VOTE,
	TYPE_SEND_GIFT,
	TYPE_COMPONENT,
};

@implementation SettingController

@synthesize nickNameTextfield, impresaTextfield, imageView, setting, picker;
@synthesize elementsItems;

#pragma mark -
#pragma mark utilities

- (void)linkButtonClicked:(id)sender
{
	AdvertisementLabel *theLabel = (AdvertisementLabel *) sender;
	NSString *theURL = theLabel.sLink;
	MYLog(@">>>the URL :%@",theURL);
	
	NSDictionary *dict = [NSDictionary dictionaryWithObject:theURL forKey:@"url"];
	[[NSNotificationCenter defaultCenter] postNotificationName:ShowStandbyZwpViewNotification object:self userInfo:dict];
}

- (void) processMClubFeedData:(NSNotification *)note
{
	elementsItems = [(NSArray *)[note userInfo] retain];
}

- (void) avatarChangeSetting:(id)sender
{

	UISwitch *theSwitch = sender;
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setBool:theSwitch.on forKey:@"kAvatarChangedKey"];
	[defaults synchronize];
//	MYLog(@">>>[defaults.on]:%d",[defaults boolForKey:@"kAvatarChangedKey"]);
	[Public alertMessage:@"" addMsg:NSLocalizedString(@"changeWillBeEffectiveNextTime",nil)];
}

- (void) emotionChangeSetting:(id)sender
{
	UISwitch *theSwitch = sender;
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setBool:theSwitch.on forKey:@"kEmotionChangedKey"];
	[defaults synchronize];
//	MYLog(@">>>[defaults.on]:%d",[defaults boolForKey:@"kEmotionChangedKey"]);
	[Public alertMessage:@"" addMsg:NSLocalizedString(@"changeWillBeEffectiveNextTime",nil)];
}

#pragma mark -

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) 
	{
		isEditing = NO;
		nickNameTextfield = nil;
		impresaTextfield = nil;
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
		infoDictionary = appdelegate.infoDict;
		self.setting = [[[PersonInfo alloc] init] autorelease];
		setting.nickname = [infoDictionary objectForKey:@"nickname"];
		setting.impresa = [infoDictionary objectForKey:@"impresa"];
		setting.state = [infoDictionary objectForKey:@"show"];
		setting.portraitType = [infoDictionary objectForKey:@"portraitType"];
		setting.portraitData = [infoDictionary objectForKey:@"portraithashData"];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notifyKeyboardWillShow:) 
													 name:UIKeyboardWillShowNotification object:self.view.window];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notifyKeyboardWillhide:) 
													 name:UIKeyboardWillHideNotification object:self.view.window];
		
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(processMClubFeedData:)
													 name:UserFeedArrivalNotification
												   object:nil];
	}
	return self;
}

// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
}


// If you need to do additional setup after loading the view, override viewDidLoad.
- (void)viewDidLoad 
{
	UIBarButtonItem *button = [[UIBarButtonItem alloc]
							   initWithTitle:NSLocalizedString(@"setChange",nil)
							   style:UIBarButtonItemStyleBordered
							   target:self
							   action:@selector(setAuthInfo:)];
	button.enabled = NO;
	self.navigationItem.rightBarButtonItem = button;
	self.navigationItem.title = NSLocalizedString(@"qtn_title_my_msn",nil);
	[super viewDidLoad];
	
	MSNAppDelegate *appdelegate = (MSNAppDelegate *)[UIApplication sharedApplication].delegate;
	NSString *account = appdelegate.myAccount;
	[[ClientNetWorkController getInstance] getFeedForUser:account count:3];
}

- (void)viewDidAppear: (BOOL)animated{
	[super viewDidAppear: animated];
	CGSize size = self.tableView.contentSize;
	size.height += 44;
	[self.tableView setContentSize: size];
	[self.navigationController setNavigationBarHidden: NO animated: NO];
}

- (void)didReceiveMemoryWarning 
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	if (self.picker != nil)
	{
		[self dismissModalViewControllerAnimated:YES];
		self.picker = nil;
	}
	// Release anything that's not essential, such as cached data
}


- (void)dealloc 
{
	LogMethod();
	[setting release];
	httpClient.delegate = nil;
	[httpClient release];
	nickNameTextfield.delegate = nil;
	impresaTextfield.delegate = nil;
	[picker release];
	[imageView release];
	[impresaTextfield release];
	[nickNameTextfield release];
	[elementsItems release];
	[conversationsCell release];
	[super dealloc];
}

- (void)setAuthInfo:(id)sender
{
	BOOL changed = NO;
	BOOL sendToServer = NO;
	UIAlertView* anAlertView = nil;
	NSString * string = [infoDictionary objectForKey:@"show"];
	
	if ([string compare:setting.state] != NSOrderedSame)
	{
		[[ClientNetWorkController getInstance] sendPresenceRequest:setting.state];
		[[MSNAppDelegate globalAppDelegate].infoDict setObject: setting.state forKey: kLoginStatusKey];
		[[MSNAppDelegate globalAppDelegate] saveAccountInfo];
	}

	string = [infoDictionary objectForKey:@"nickname"];
	if (compareStrings(string,nickNameTextfield.text) != NSOrderedSame)
	{
		if (setting.nickname == nil)
		{
			MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;
			infoDictionary = appdelegate.infoDict;
			setting.nickname = appdelegate.myAccount;
			//setting.nickname = string;
		}

		changed = YES;
	}
	else
		setting.nickname = nil;
	
	string = [infoDictionary objectForKey:@"impresa"];
	if (compareStrings(string,impresaTextfield.text) != NSOrderedSame)
	{
		if (setting.impresa == nil)
			setting.impresa = string;
		changed = YES;
	}
	else
		setting.impresa = nil;
	
	string = [infoDictionary objectForKey:@"portraithashData"];
	if (compareStrings(string,setting.portraitData) != NSOrderedSame)
	{
		NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
		if (dict != nil)
		{
			NSString *string = [dict objectForKey:@"data_method"];
			if (string != nil)
			{
				sendToServer = [string characterAtIndex:4] == '0';
			}
		}
		
		if (sendToServer)
			[self sendportraitData:setting.portraitData];
		else
			changed = YES;
	}
	else
		setting.portraitData = nil;
	
	if (!sendToServer && changed)
		[[ClientNetWorkController getInstance] sendVcardRequest:setting];
	
	BOOL aSaveConversations = [conversationsCell getValue];
	MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
	BOOL anAppSaveConversations = [[anAppDelegate.infoDict objectForKey: kSaveConversationKey] boolValue];
	if (aSaveConversations != anAppSaveConversations){
		if (aSaveConversations){
			anAlertView = [[UIAlertView alloc] initWithTitle: nil
													 message: NSLocalizedString(@"You have chosen to save your conversations. This may not be permitted under local law. You should confirm that this is permitted before turning it on.",nil)
													delegate: self
										   cancelButtonTitle: NSLocalizedString(@"qtn_ok", nil)
										   otherButtonTitles: nil];
			[anAlertView show];
			[anAlertView release];
		}
		else {
			[anAppDelegate.infoDict setObject: [NSNumber numberWithBool: NO] forKey: kSaveConversationKey];
			[anAppDelegate saveAccountInfo];
		}
	}

	self.navigationItem.rightBarButtonItem.enabled = NO;
}

- (void)sendportraitData:(NSString*)theString
{
	if (theString != nil)
	{
		NSData *imageData = [NSData dataWithBase64EncodedString:theString];
		if (imageData != nil)
		{
			if (httpClient == nil)
				httpClient = [[HttpClient alloc] init];
			if (httpClient != nil)
			{
				httpClient.callbackFun = @selector(callBackData:dictionary:);
				httpClient.delegate = self;	

				NSMutableDictionary *headers = [NSMutableDictionary dictionaryWithCapacity:0];
				[headers setObject:@"*/*" forKey:@"Accept"];
				
				NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
				if (dict != nil)
				{
					NSString *string = [dict objectForKey:@"data"];
					NSURL *serverUrl = nil;
					if (string != nil)
					{
						string = [string stringByAppendingString:@"/Photo.jpg"];
						serverUrl = [[NSURL alloc] initWithString:string];
					}
					
					if (serverUrl != nil)
					{
						dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"login"];
						if (dict != nil)
						{	
							NSString *userName = [dict valueForKey:@"id"];
							if (userName != nil)
							{
								NSRange range = [userName rangeOfString:@"@"];
								if (range.location != NSNotFound)
								{
									userName = [userName substringToIndex:range.location];
									NSString *password = [dict valueForKey:kPasswordKey];	
									if (password != nil)
									{
										[headers setObject:[NSString stringWithFormat:@"%@;%@",userName,password] forKey:@"Pica-Auth"];
									}
								}
							}
						}
						[headers setObject:NSLocalizedString(APPLICATION_VERSION,nil) forKey:@"Pica-Clientver"];
						
						[httpClient addHttpRequest:serverUrl
											 method:@"POST"
											headers:headers
										 bodyString:imageData];
						
						[serverUrl release];
					}	
				}
			}
		}
	}
}

- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict
{
	if (theData != nil && [theData length] > 0)
	{
		NSString *fileUrl = [[NSString alloc] initWithData:theData encoding:NSUTF8StringEncoding];
		setting.portraitURL = fileUrl;
		[fileUrl release];
		[[ClientNetWorkController getInstance] sendVcardRequest:setting];
	}
}


- (void)showPhotoLibrary:(id)sender
{
	if ( [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
	{
		self.picker = [[[UIImagePickerController alloc] init] autorelease];
		picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
		picker.delegate = self;
		picker.allowsImageEditing = YES;
		
		// Picker is displayed asynchronously.
		[self presentModalViewController:picker animated:YES];
	}
	
}

- (void)capturePhoto:(id)sender
{
	if ( [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
	{
		self.picker = [[[UIImagePickerController alloc] init] autorelease];
		picker.sourceType = UIImagePickerControllerSourceTypeCamera;
		picker.delegate = self;
		picker.allowsImageEditing = YES;
		
		[self presentModalViewController:picker animated:YES];	
	}
}


- (void)imagePickerController:(UIImagePickerController *)picker
		didFinishPickingImage:(UIImage *)image
				  editingInfo:(NSDictionary *)editingInfo
{
	self.picker = nil;
	[self dismissModalViewControllerAnimated:YES];
	UIImage *squreImage = makeHeadPhotoImage(image);
    imageView.image = squreImage;
	NSData *imageData = UIImageJPEGRepresentation (squreImage,1.0);
	setting.portraitType = @"jpeg";
	setting.portraitData = [imageData base64Encoding];
	if ([self settingChanged])
		self.navigationItem.rightBarButtonItem.enabled = YES;
	else
		self.navigationItem.rightBarButtonItem.enabled = NO;	
}

- (BOOL)settingChanged
{
	BOOL changed = NO;
	NSString * string = [infoDictionary objectForKey:@"show"];
	if ([string compare:setting.state] != NSOrderedSame)
	{
		changed = YES;			
	}
	else
	{
		string = [infoDictionary objectForKey:@"nickname"];
		if (compareStrings(string,setting.nickname) != NSOrderedSame)
		{
			changed = YES;			
		}
		else
		{
			string = [infoDictionary objectForKey:@"impresa"];
			if (compareStrings(string,setting.impresa) != NSOrderedSame)
			{
				changed = YES;			
			}
			else
			{
				string = [infoDictionary objectForKey:@"portraithashData"];
				if (compareStrings(string,setting.portraitData) != NSOrderedSame)
					changed = YES;	
			}
		}
	}
	
	return changed;
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	self.picker = nil;
    [self dismissModalViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark tableView delegate methods

- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	//[tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
	return indexPath;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	if (indexPath.section == 0)
	{
		[nickNameTextfield resignFirstResponder];
		[impresaTextfield resignFirstResponder];
		UITableViewCell *currCell = [tableView cellForRowAtIndexPath:indexPath];
		NSString *string = getStatus(currCell.text);

		if ([string compare:setting.state] != NSOrderedSame)
		{
			setting.state = string;
			if ([self settingChanged]){
				self.navigationItem.rightBarButtonItem.enabled = YES;
			}
			else
				self.navigationItem.rightBarButtonItem.enabled = NO;	
			[(UITableView*)self.view reloadData];
		}
	}
	else if (indexPath.section == 1)
	{
		if (!isEditing)
		{
			if (indexPath.row == 0)
			{
				[nickNameTextfield becomeFirstResponder];
			}
			else
			{
				[impresaTextfield becomeFirstResponder];
			}
			[tableView scrollToRowAtIndexPath:indexPath atScrollPosition:UITableViewScrollPositionBottom animated:NO];
			[tableView deselectRowAtIndexPath:indexPath animated:NO];	
		}
	}
	else
	{
		[nickNameTextfield resignFirstResponder];
		[impresaTextfield resignFirstResponder];
	}
	//		
}


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	int height = 46;
	switch (indexPath.section)
	{
		case StateIndex:
			height = 30;
			break;
			
		case DisPlayNameIndex:
			height = 80;
			break;
	}
	return height;
}

/////datasource method
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	return ([dict objectForKey:@"second_zwp"] != nil) ? 3 : 2;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	// Number of rows is the number of names in the region dictionary for the specified section
	int rows = 0;
	switch (section)
	{
		case StateIndex:
			rows = 4;
			break;
			
		case DisPlayNameIndex:
			rows = 3;
			break;
		case ActivityOfFriends:
			rows = 3;
			break;
	}
	return rows;
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	// The header for the section is the region name -- get this from the dictionary at the section index
	NSString *groupName = nil;
	switch (section)
	{
		case StateIndex:
			groupName = NSLocalizedString(@"qtn_myzone_mystate",nil);
			break;
			
		case DisPlayNameIndex:
			groupName = NSLocalizedString(@"qtn_personal_setting",nil);
			break;
		case ActivityOfFriends:
			groupName = NSLocalizedString(@"qtn_setting_friends_activities",nil);
			break;			
	}
	return groupName;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	UITableViewCell * cell = nil;
//	NSString* anId = nil;
//	MSNAppDelegate* anAppDelegate = nil;
	NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
	switch (indexPath.section)
	{
		case StateIndex:
			cell = [self tableView:tableView StateCellAtIndexPath:indexPath];
			break;
			
		case DisPlayNameIndex:
			if (indexPath.row < 2)
				cell = [self tableView:tableView DisPlayNameCellAtIndexPath:indexPath];
			else
				cell = [self tableView:tableView PictureCellAtIndexPath:indexPath];
			break;
		case ActivityOfFriends:
			if ([dict objectForKey:@"second_zwp"] != nil)
			{
				cell = [self tableView:tableView ActivityCellAtIndexPath:indexPath];				
			}
 			break;
		default:
			break;
	}
	return cell;
}

- (UITableViewCell*)tableView:(UITableView *)tableView ActivityCellAtIndexPath:(NSIndexPath*)indexPath
{
	CGRect rc = CGRectZero;
	rc.size.height = 70;
	NSString *identifier = [NSString stringWithFormat:@"%d%d",indexPath.section,indexPath.row];
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
	if (cell == nil)
	{
		cell = [[UITableViewCell alloc] initWithFrame:rc reuseIdentifier:identifier];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	}
	
	if (elementsItems == nil)
	{
		[Public alertMessage:@"" addMsg:NSLocalizedString(@"data parse error",nil)];
		return cell;
	}

	DDXMLElement *theElement = [elementsItems objectAtIndex:indexPath.row];
	NSInteger iType = [[[theElement attributeForName:@"type"] stringValue] intValue];
	if (iType >8)
		iType = 10;
	
	NSString *sType = [NSString stringWithFormat:@"a%d.png", iType];
	UIImage *typeImage = [UIImage imageNamed:sType];
	cell.image = typeImage;
	
	AdvertisementLabel *textLabel = nil;
	UILabel *theText = nil;

	NSArray *children = [theElement children];
	NSInteger xIndex = 50;
	NSString *theString = nil;
	NSInteger theLength = 0;
	const NSInteger eLength = 12;
	const NSInteger cLength = 10;
	
//	MYLog(@">>>children:%@",children);
	for(DDXMLNode *theNode in children)
	{
		if ([[theNode name] compare:@"link"] == NSOrderedSame)
		{
			// For link node.
			theString = [theNode stringValue];
			if ([Public isEnglishNumberAlphabet:theString])
				theLength = [theString length] * eLength * 0.5; // 0.5 -- reduce length of english characters to half.
			else
				theLength = [theString length] * cLength;

			textLabel = [[AdvertisementLabel alloc] initWithFrame:CGRectZero];
			[textLabel setTextAlignment:UITextAlignmentLeft];
			textLabel.label.font = [UIFont systemFontOfSize:13];
			textLabel.sLink = [[(DDXMLElement *)theNode attributeForName:@"url"] stringValue];			
			[textLabel addTarget:self action:@selector(linkButtonClicked:)];					
			[textLabel setTextColor:[UIColor blueColor]];
			textLabel.label.adjustsFontSizeToFitWidth = YES;
			[textLabel setText:theString];
			textLabel.frame = [textLabel textRectForBounds:CGRectMake(xIndex, 10, theLength, 25) limitedToNumberOfLines:1];
			[cell addSubview:textLabel];
			[textLabel release];
			textLabel = nil;
			xIndex += theLength;
		}else if ([[theNode name] compare:@"text"] == NSOrderedSame){
			// For text node.
			theString = [theNode stringValue];
			if ([Public isEnglishNumberAlphabet:theString])
				theLength = [theString length] * eLength;
			else
				theLength = [theString length] * cLength;

			theText = [[UILabel alloc] initWithFrame:CGRectMake(xIndex, 5, theLength, 25)];
			theText.font = [UIFont systemFontOfSize:13];
			theText.adjustsFontSizeToFitWidth = YES;
			theText.text = theString;
			[cell addSubview:theText];
			[theText release];
			theText = nil;
			xIndex += theLength;
		}
	}
	
	return cell;
}

- (UITableViewCell*)tableView:(UITableView *)tableView PictureCellAtIndexPath:(NSIndexPath*)indexPath
{
	CGRect rc = CGRectZero;
	rc.size.height = 70;
	NSString *identifier = [NSString stringWithFormat:@"%d%d",indexPath.section,indexPath.row];
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
	if (cell == nil)
	{
		cell = [[UITableViewCell alloc] initWithFrame:rc reuseIdentifier:identifier];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	}

	MSNAppDelegate* anApp = [MSNAppDelegate globalAppDelegate];
	NSData *data = [NSData dataWithBase64EncodedString:setting.portraitData];
	imageView = [[UIImageView alloc] initWithImage:[UIImage imageWithData:data]];
	rc = CGRectMake(20, 10, 60, 60);
	imageView.frame = rc;
	
	[[cell contentView] addSubview:imageView];

	rc = CGRectMake(192, 18, 100, 44);
	UIButton *browserButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	browserButton.frame = rc;
	browserButton.backgroundColor = [UIColor clearColor];
	[browserButton setTitle:NSLocalizedString(@"qtn_camera_select",nil) forState:UIControlStateNormal];
	[browserButton addTarget:self action:@selector(showPhotoLibrary:) forControlEvents:UIControlEventTouchUpInside];
	[[cell contentView] addSubview:browserButton];

	UIImage *squreImage = anApp.listViewController.imageButton.image;
    imageView.image = squreImage;
	if (imageView.image == nil)
		imageView.image = [PersonInfo borgImage];
	
	return cell;
}


- (UITableViewCell*)tableView:(UITableView *)tableView StateCellAtIndexPath:(NSIndexPath*)indexPath
{
	CGRect rc = CGRectZero;
	rc.size.height = 30;
	
	NSString *identifier = [NSString stringWithFormat:@"%d%d",indexPath.section,indexPath.row];
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];

	if (cell == nil)
		cell = [[UITableViewCell alloc] initWithFrame:rc reuseIdentifier:identifier];
	
	switch (indexPath.row)
	{
		case 0:
			cell.text = NSLocalizedString(@"qtn_status_online",nil);
			break;
		case 1:
			cell.text = NSLocalizedString(@"qtn_status_busy",nil);
			break;
//		case 2:
//			cell.text = NSLocalizedString(@"qtn_status_berightback",nil);
//			break;
		case 2:
			cell.text = NSLocalizedString(@"qtn_status_away",nil);
			break;
//		case 4:
//			cell.text = NSLocalizedString(@"qtn_status_onthephone",nil);
//			break;
//		case 5:
//			cell.text = NSLocalizedString(@"qtn_status_outtolunch",nil);
//			break;
		case 3:
			cell.text = NSLocalizedString(@"qtn_status_appearoffline",nil);
			break;
	}
	NSString *string = getStatus(cell.text);
	if ([string isEqualToString: setting.state])
		cell.accessoryType = UITableViewCellAccessoryCheckmark;
	else
		cell.accessoryType = UITableViewCellAccessoryNone;
	
	return cell;
}

- (UITableViewCell*)tableView:(UITableView *)tableView FlowControlCellAtIndexPath:(NSIndexPath*)indexPath
{
	CGRect 	rc = CGRectMake(0, 0, 300, 45);
	NSString *identifier = [NSString stringWithFormat:@"%d%d",indexPath.section,indexPath.row];
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
	
	if (cell == nil)
	{
		cell = [[UITableViewCell alloc] initWithFrame:rc reuseIdentifier:identifier];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		rc = rc = CGRectMake(10, 0, 150, 40);
		
		UILabel *label = [[UILabel alloc] initWithFrame:rc];
		label.font = [UIFont systemFontOfSize:12.0];
		label.textAlignment = UITextAlignmentLeft;
		label.textColor = [UIColor blueColor];
		
		rc = CGRectMake(160, 10, 80, 45);
		UISwitch *theSwitch = [[UISwitch alloc] initWithFrame:rc];
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		
		switch (indexPath.row)
		{
			case 0:
				label.text = NSLocalizedString(@"supportContactIconChange",nil);
				[theSwitch addTarget:self action:@selector(avatarChangeSetting:) forControlEvents:UIControlEventValueChanged];
				theSwitch.on = [defaults boolForKey:@"kAvatarChangedKey"];
				break;
				
			case 1:
				label.text = NSLocalizedString(@"enableContactEmotionChange",nil);
				[theSwitch addTarget:self action:@selector(emotionChangeSetting:) forControlEvents:UIControlEventValueChanged];
				theSwitch.on = [defaults boolForKey:@"kEmotionChangedKey"];
				break;
			default:
				break;
		}

		[cell.contentView addSubview:label];
		[cell.contentView addSubview:theSwitch];
		[label release];
		[theSwitch release];	
		
	}
	
	return cell;
}
- (UITableViewCell*)tableView:(UITableView *)tableView DisPlayNameCellAtIndexPath:(NSIndexPath*)indexPath
{
	CGRect 	rc = CGRectMake(0, 0, 300, 80);
	NSString *identifier = [NSString stringWithFormat:@"%d%d",indexPath.section,indexPath.row];
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
	
	if (cell == nil)
	{
		cell = [[UITableViewCell alloc] initWithFrame:rc reuseIdentifier:identifier];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		rc = CGRectMake(10, 0, 270, 0);
		UILabel *label = [[UILabel alloc] initWithFrame:rc];
		label.font = [UIFont boldSystemFontOfSize:17];
		label.textAlignment = UITextAlignmentLeft;
		//label.textColor = [UIColor blueColor];
		label.autoresizingMask = UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleHeight;
		
		
		rc = CGRectMake(10, 40, 270, 0);
		UITextField *textField = [[UITextField alloc] initWithFrame:rc];
		textField.textAlignment = UITextAlignmentLeft;
		textField.borderStyle = UITextBorderStyleBezel;
		textField.textColor = [UIColor blackColor];
		textField.autoresizingMask = UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleHeight;
		MSNAppDelegate *appdelegate = (MSNAppDelegate*)[UIApplication sharedApplication].delegate;;
		
		switch (indexPath.row)
		{
			case 0:
				label.text = NSLocalizedString(@"qtn_settings_my_nickname",nil);
				textField.placeholder = appdelegate.myAccount;
				textField.text = appdelegate.listViewController.nickNameLabel.text;
				textField.keyboardType =  UIKeyboardTypeDefault;
				textField.font = [UIFont systemFontOfSize:16];
				textField.returnKeyType = UIReturnKeyDone;
				textField.clearButtonMode = UITextFieldViewModeWhileEditing;
				self.nickNameTextfield = textField;
				nickNameTextfield.delegate = self;
				break;
				
			case 1:
				label.text = NSLocalizedString(@"qtn_settings_my_personal_msg",nil);
				textField.text = appdelegate.listViewController.impresaLabel.text;
				textField.keyboardType =  UIKeyboardTypeDefault;
				textField.font = [UIFont systemFontOfSize:16];
				textField.returnKeyType = UIReturnKeyDone;
				textField.clearButtonMode = UITextFieldViewModeWhileEditing;
				self.impresaTextfield = textField;
				impresaTextfield.delegate = self;
			default:
				break;
		}
		[cell.contentView addSubview:label];
		[cell.contentView addSubview:textField];
		[label release];
		[textField release];
	}
	return cell;
}

#pragma mark -

- (void)setViewMovedUp:(BOOL)movedUp
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:0.3];
    CGRect rect = self.view.frame;
    if (movedUp)
	{
        rect.size.height -= kOFFSET_FOR_KEYBOARD;
    }
	else
	{
        rect.size.height += kOFFSET_FOR_KEYBOARD;
    }
    self.view.frame = rect;
    [UIView commitAnimations];
}


- (BOOL)textFieldShouldClear:(UITextField *)textField
// called when clear button pressed. return NO to ignore (no notifications)
{
	return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
// called when 'return' key pressed. return NO to ignore.
{
	if (textField == nickNameTextfield)
	{
	//	[impresaTextfield becomeFirstResponder];
	}
	else if (textField == impresaTextfield)
	{
	}
	[textField resignFirstResponder];	
	return YES;	
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	isEditing = YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	BOOL ret = YES;
	int length = [textField.text length] - range.length + [string length];
	if (textField == nickNameTextfield)
	{
		if (length > 129)
			ret = NO;
	}
	else if (textField == impresaTextfield)
	{
		if (length > 129)
			ret = NO;
	}
	return ret;
}


- (void)textFieldDidEndEditing:(UITextField *)textField
{
	if (textField == nickNameTextfield)
	{
		if ([[nickNameTextfield.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] length] == 0)
		{
			setting.nickname = nil;
			nickNameTextfield.text = nil;
		}
		else
			setting.nickname = nickNameTextfield.text;
	}
	else if (textField == impresaTextfield)
	{
		setting.impresa = impresaTextfield.text;
	}
	
	if ([self settingChanged])
		self.navigationItem.rightBarButtonItem.enabled = YES;
	else
		self.navigationItem.rightBarButtonItem.enabled = NO;
	isEditing = NO;
}

- (void)notifyKeyboardWillShow: (NSNotification*)note
{
	self.navigationItem.rightBarButtonItem.enabled = NO;
	[self setViewMovedUp:YES];
}

- (void)notifyKeyboardWillhide: (NSNotification*)note
{
	[self setViewMovedUp:NO];
}

- (IBAction)switchAction: (id)sender{
	if (!self.navigationItem.rightBarButtonItem.enabled)
		self.navigationItem.rightBarButtonItem.enabled = YES;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
	MSNAppDelegate* anAppDelegate = [MSNAppDelegate globalAppDelegate];
	[anAppDelegate.infoDict setObject: [NSNumber numberWithBool: [conversationsCell getValue]] forKey: kSaveConversationKey];
	[anAppDelegate saveAccountInfo];
}

@end
