//
//  ListViewCell.m
//  msn_for_mac
//
//  Created by  apple on 08-9-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "ListViewCell.h"
#import "ImageLabel.h"
#import "PersonInfo.h"
#import "ChatViewController.h"
#import "define.h"
#import "MSNAppDelegate.h"

@implementation ListViewCell
@synthesize personinfo, isSelected;

- (id)initWithFrame:(CGRect)frame personInfo:(PersonInfo *)info
{
	if (self = [super initWithFrame:frame reuseIdentifier:info.jid])
	{
		isSelected = NO;
		self.personinfo = info;
//		if (personinfo.group == nil || [personinfo.group rangeOfString: @","].location == NSNotFound)
//			personinfo.cell = self;
		
		showBigImage = [[[MSNAppDelegate globalAppDelegate].infoDict objectForKey:kShowBigImageKey] boolValue];
		if (showBigImage)
		{
			CGRect rc = CGRectMake(62, 8, 20, 20);
			imageView = [[UIImageView alloc] initWithImage:[personinfo currentStatusImage]];
			imageView.frame = rc;
			[self.contentView addSubview:imageView];
			
			rc = CGRectMake(84, 1, 190, 20);
			nameLabel = [[ImageLabel alloc] initWithFrame:rc];
			nameLabel.font = [UIFont boldSystemFontOfSize: 16];
			nameLabel.backgroundColor = [UIColor clearColor];
			[self.contentView addSubview:nameLabel];
		
			rc = CGRectMake(41, 10, 20, 20);
			spacesView = [[UIImageView alloc] initWithImage:[personinfo spacesImage]];
			spacesView.frame = rc;
			[self.contentView addSubview:spacesView];
			
			rc = CGRectMake(84, 21, 190, 20);
			infoLabel = [[ImageLabel alloc] initWithFrame:rc];
			infoLabel.font = [UIFont systemFontOfSize: 14.0];
			infoLabel.textColor = [UIColor grayColor];
			//infoLabel.textColor = [UIColor colorWithRed:59/255 green:59/255 blue:59/255 alpha:1.0];
			infoLabel.backgroundColor = [UIColor clearColor];
			[self.contentView addSubview:infoLabel];	
		}
		else 
		{
			CGRect rc = CGRectMake(0, 1, 20, 20);
			spacesView = [[UIImageView alloc] initWithImage:[personinfo spacesImage]];
			spacesView.frame = rc;
			[self.contentView addSubview:spacesView];	
			
			rc = CGRectMake(25, 1, 20, 20);
			imageView = [[UIImageView alloc] initWithImage:[personinfo currentStatusImage]];
			imageView.frame = rc;
			[self.contentView addSubview:imageView];
			
			rc = CGRectMake(50, 1, 230, 20);
			nameLabel = [[ImageLabel alloc] initWithFrame:rc];
			nameLabel.font = [UIFont systemFontOfSize:16.0];
			nameLabel.textColor = [UIColor blackColor];
			nameLabel.backgroundColor = [UIColor clearColor];
			nameLabel.lineBreakMode = UILineBreakModeTailTruncation;
			[self.contentView addSubview:nameLabel];
			infoLabel = nil;
		}
		[self reloadText];
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(notifyGroupHerderClicked:)
				   name:GroupHerderClickedNotification 
				 object:nil];
		// Initialization code
	}
	return self;
}

- (void)reloadText
{
	if (personinfo.localname != nil)
	{
		if ([personinfo.localname compare:@""]==NSOrderedSame)
		{
			nameLabel.text = personinfo.nickname;
		}else {
			nameLabel.text = personinfo.localname;
		}		
	}
	else{
		if (personinfo.nickname != nil)
			nameLabel.text = personinfo.nickname;
		else
			nameLabel.text = personinfo.imid;	
	}
	
	if (showBigImage)
	{
		imageView.image = [personinfo currentStatusImage];
		if(([personinfo.state compare:@"offline" options:NSCaseInsensitiveSearch] == NSOrderedSame) || (personinfo.state == nil))
		{
			
		}
		else
		{
			spacesView.image = [personinfo spacesImage];
		}
		self.image = [personinfo originImage];
		infoLabel.text = personinfo.impresa;
	}
	else
	{
		//self.image = [personinfo currentStatusImage];
		if(([personinfo.state compare:@"offline" options:NSCaseInsensitiveSearch] == NSOrderedSame) || (personinfo.state == nil))
		{
			
		}
		else
		{
			spacesView.image = [personinfo spacesImage];
		}
		imageView.image = [personinfo currentStatusImage];
	}
}


- (void)dealloc 
{
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver:self name:GroupHerderClickedNotification object:nil];
	personinfo = nil;
	[nameLabel release];
	[infoLabel release];
	[super dealloc];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
	[super setSelected:selected animated:animated];
	if (selected)
	{
		[[NSNotificationCenter defaultCenter] postNotificationName: GroupHerderClickedNotification object:self];
	}
}

- (void)notifyGroupHerderClicked: (NSNotification*)note
{
	UIColor *color = nil;
	id view = [note object];
	if (view == self)
	{
		color = [UIColor colorWithRed: 210.0/255
								green: 234.0/255
								 blue: 246.0/255
								alpha: 1.0];
		self.contentView.backgroundColor = color;
	}
	else
	{
		color = [UIColor whiteColor];
		self.contentView.backgroundColor = color;
	}
}

@end
