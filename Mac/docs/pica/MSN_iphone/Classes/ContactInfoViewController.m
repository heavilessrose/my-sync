//
//  ContactInfoViewController.m
//  MSN
//
//  Created by  apple on 08-10-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ContactInfoViewController.h"
#import "ImageLabel.h"
#import "PersonInfo.h"
#import "ChatViewController.h"
#import "API.h"
#import "ClientNetWorkController.h"
#import "define.h"
#import "public.h"

@implementation ContactInfoViewController
@synthesize personInfo;
@synthesize nickNameField;
@synthesize spinner;

#pragma mark -
#pragma mark utilities

- (void)setLocalNameCallback:(NSNotification *)note
{
	[spinner stopAnimating];
	NSDictionary *userInfo = [note userInfo];
	NSString* aString = [userInfo objectForKey:@"updateLocalNameResult"];
	if ([aString isEqualToString: @"success"]){
		if (nickNameField != nil && [nickNameField.text length] == 0){
			nickNameField.text = personInfo.nickname;
		}
	}else {
		[Public alertMessage:@"" addMsg:NSLocalizedString(@"update local name error",nil)];
	}

}

- (void)attachNickNameLabel: (NSString*)aNickName{
	CGRect rc = CGRectMake(20, 110, 285, 46);
	CGSize size = rc.size;
	size.height = 10000;
	if (scrollViewNickName == nil){
		scrollViewNickName = [[UIScrollView alloc] initWithFrame: rc];
		[self.view addSubview: scrollViewNickName];
	}
	
	NSArray* aSubViews = [scrollViewNickName subviews];
	for (UIView* aSubView in aSubViews){
		if ([aSubView isKindOfClass: [ImageLabel class]]){
			[aSubView removeFromSuperview];
			break;
		}
	}
	
	size = [aNickName sizeWithFont: [UIFont systemFontOfSize: 20]
				 constrainedToSize: size
					 lineBreakMode: UILineBreakModeWordWrap];
	ImageLabel *label = [[ImageLabel alloc] initWithFrame: CGRectMake(0, 0, rc.size.width, size.height)];
	label.lineBreakMode = UILineBreakModeWordWrap;
	label.backgroundColor = [UIColor clearColor];
	label.font = [UIFont systemFontOfSize: 20];
	label.text = aNickName;
	rc = [label textRectForBounds: CGRectMake(0, 0, 281, 1000)
		   limitedToNumberOfLines: 0];
	label.frame = CGRectMake(0, 0, rc.size.width, rc.size.height + 24);
	scrollViewNickName.contentSize = rc.size;
	[scrollViewNickName addSubview:label];
	[label release];
}

- (void)attachEmailAddress: (NSString*)anEmailAddress{
	CGRect rc = CGRectMake(20, 240, 285, 46);
	CGSize size = rc.size;
	size.height = 10000;
	if (scrollViewEmailAddress == nil){
		scrollViewEmailAddress = [[UIScrollView alloc] initWithFrame: rc];
		[self.view addSubview: scrollViewEmailAddress];
	}
	
	NSArray* aSubViews = [scrollViewEmailAddress subviews];
	for (UIView* aSubView in aSubViews){
		if ([aSubView isKindOfClass: [ImageLabel class]]){
			[aSubView removeFromSuperview];
			break;
		}
	}
	
	size = [anEmailAddress sizeWithFont: [UIFont systemFontOfSize: 20]
					  constrainedToSize: size
						  lineBreakMode: UILineBreakModeWordWrap];
	ImageLabel *label = [[ImageLabel alloc] initWithFrame: CGRectMake(2, 0, rc.size.width, size.height)];
	label.lineBreakMode = UILineBreakModeWordWrap;
	label.backgroundColor = [UIColor clearColor];
	label.font = [UIFont systemFontOfSize: 20];
	label.text = anEmailAddress;
	rc = [label textRectForBounds: CGRectMake(0, 0, 285, 1000)
		   limitedToNumberOfLines: 0];
	label.frame = CGRectMake(0, 0, rc.size.width, rc.size.height + 24);
	scrollViewEmailAddress.contentSize = rc.size;
	[scrollViewEmailAddress addSubview:label];
	[label release];
}

#pragma mark -
- (id) init
{
	self = [super init];
	if (self != nil) 
	{
		[[NSNotificationCenter defaultCenter] addObserver:self 
												 selector:@selector(setLocalNameCallback:)
													 name:LocalNameResultNotification 
												   object:nil];
	}
	return self;
}


// Implement loadView if you want to create a view hierarchy programmatically
- (void)loadView 
{
	[super loadView];
	
	CGRect rect = CGRectMake(135, 20, 50, 50);
	imageView = [[UIImageView alloc] initWithFrame:rect];
	[self.view addSubview:imageView];
	[imageView release];
	
	rect = CGRectMake(10, 78, 200, 20);
	UILabel *label = [[UILabel alloc] initWithFrame:rect];
	NSString *string = NSLocalizedString(@"qtn_nickname",nil);
	label.text = [string stringByAppendingString:@":"];
	label.font = [UIFont boldSystemFontOfSize:20];
	[self.view addSubview:label];
	[label release];
	
	rect = CGRectMake(20, 100, 285, 50);
	nickNameView = [[UIScrollView alloc] initWithFrame:rect];
	nickNameView.showsHorizontalScrollIndicator = NO;
	nickNameView.showsVerticalScrollIndicator = YES;
	nickNameView.scrollsToTop = YES;
	nickNameView.clipsToBounds = YES;
	[self.view addSubview:nickNameView];
	[nickNameView release];
	
	rect = CGRectMake(10, 155, 200, 20);
	label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"qtn_status",nil);
	label.text = [string stringByAppendingString:@":"];
	label.font = [UIFont boldSystemFontOfSize:20];
	[self.view addSubview:label];
	[label release];

	rect = CGRectMake(20, 177, 20, 20);
	statusView = [[UIImageView alloc] initWithFrame:rect];
	[self.view addSubview:statusView];
	[statusView release];
	
	rect = CGRectMake(50, 177, 20, 20);
	spaceView = [[UIImageView alloc] initWithFrame:rect];
	[self.view addSubview:spaceView];
	[spaceView release];
	
	rect = CGRectMake(10, 212, 200, 20);
	label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"qtn_username",nil);
	label.text = [string stringByAppendingString:@":"];
	label.font = [UIFont boldSystemFontOfSize:20];
	[self.view addSubview:label];
	[label release];

	rect = CGRectMake(10, 285, 200, 20);
	label = [[UILabel alloc] initWithFrame:rect];
	string = NSLocalizedString(@"qtn_impresa",nil);
	label.text = [string stringByAppendingString:@":"];
	label.font = [UIFont boldSystemFontOfSize:20];
	[self.view addSubview:label];
	[label release];
	
	rect = CGRectMake(20, 309, 285, 95);
	impresaView = [[UIScrollView alloc] initWithFrame:rect];
	impresaView.showsHorizontalScrollIndicator = NO;
	impresaView.showsVerticalScrollIndicator = YES;
	impresaView.scrollsToTop = YES;
	impresaView.clipsToBounds = YES;
	[self.view addSubview:impresaView];
	[impresaView release];
	
	CGRect frame = CGRectMake(140, 220, 40, 40);
	spinner = [[UIActivityIndicatorView alloc] initWithFrame:frame];
	spinner.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
	[spinner sizeToFit];
	spinner.autoresizingMask = (UIViewAutoresizingFlexibleLeftMargin |
									UIViewAutoresizingFlexibleRightMargin |
									UIViewAutoresizingFlexibleTopMargin |
									UIViewAutoresizingFlexibleBottomMargin);
	[self.view addSubview:spinner];
}

- (void)viewDidAppear:(BOOL)animated
{
	self.navigationController.navigationBar.tintColor = [UIColor darkGrayColor];
	[self.navigationController setNavigationBarHidden:NO animated:YES];
	[super viewDidAppear:animated];
}

- (void)viewDidLoad 
{
	[super viewDidLoad];
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(notifynetworkDisabled:) 
                                                 name: NetWorkDisabledNotification 
                                               object: nil];
	self.navigationItem.title = NSLocalizedString(@"qtn_msn_member_detial",nil);

	if (personInfo != nil)
	{
		CGRect rect = CGRectZero;
		imageView.image = [personInfo originImage];
		statusView.image = [personInfo currentStatusImage];
		spaceView.image = [personInfo spacesImage];
		
		[self attachEmailAddress: personInfo.imid];
		
		if (personInfo.nickname == nil 
			|| [[personInfo.nickname stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] length] == 0)
		{
			personInfo.nickname = personInfo.imid;			
		}
		NSDictionary *dict = [[NSUserDefaults standardUserDefaults] objectForKey:@"config"];
		if ([dict objectForKey:@"second_zwp"] != nil){
			nickNameField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 285, 25)];
			nickNameField.borderStyle = UITextBorderStyleLine;
			if ([personInfo.localname length] > 0){
				nickNameField.text = personInfo.localname;				
			}
			else {
				nickNameField.text = personInfo.nickname;
			}
			nickNameField.delegate = self;
			[nickNameView addSubview: nickNameField];
		}
		else{
			[self attachNickNameLabel: ([personInfo.localname length] > 0) ? personInfo.localname : personInfo.nickname];
		}
		
				
		if (personInfo.impresa != nil){
			CGSize size = [nickNameView frame].size;
			size.height = 10000;
			
			size = [personInfo.impresa sizeWithFont:[UIFont systemFontOfSize:20] constrainedToSize:size lineBreakMode:UILineBreakModeWordWrap];
			ImageLabel *label = [[ImageLabel alloc] initWithFrame:CGRectMake(0, 0, impresaView.frame.size.width, size.height)];
			label.lineBreakMode = UILineBreakModeWordWrap;
			label.backgroundColor = [UIColor clearColor];
			label.font = [UIFont systemFontOfSize:20];
			label.text = personInfo.impresa;
			rect = [label textRectForBounds:CGRectMake(0, 0, 285, 1000) limitedToNumberOfLines:0];
			label.frame = CGRectMake(0, 0, 285, rect.size.height + 24);
			impresaView.contentSize = [label frame].size;
			[impresaView addSubview:label];
			[label release];
		}
	}
}


- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver: self
                                                    name: NetWorkDisabledNotification
                                                  object: nil];
	[spinner release];
	nickNameField.delegate = nil;
	self.nickNameField = nil;
	[personInfo release];
	[scrollViewNickName release];
	[scrollViewEmailAddress release];
	[super dealloc];
}

#pragma mark -
#pragma mark textField delegate methods
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
//	LogMethod();
	[spinner startAnimating];
	[textField resignFirstResponder];
	NSString *theText = textField.text;

	NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys:theText,@"kLocalName",personInfo,@"kPersonInfo",nil];
	[[ClientNetWorkController getInstance] sendLocalNameRequest:infoDict];
	
	return YES;
}

- (void)updateNickName: (NSString*)aNickName{
	[self attachNickNameLabel: aNickName];
}

- (void)notifynetworkDisabled: (NSNotification*)note{
    [self.navigationController popViewControllerAnimated: YES];
}

@end
