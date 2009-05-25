//
//  MainViewController.h
//  AppSettings
//
//  Created by Jeff LaMarche on 7/24/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#define	kUsernameKey		@"username"
#define kPasswordKey		@"password"
#define kProtocolKey		@"protocol"
#define kWarpDriveKey		@"warp"
#define kWarpFactorKey		@"warpfactor"
#define kFavoriteTeaKey		@"favoriteTea"
#define kFavoriteCandyKey	@"favoriteCandy"
#define kFavoriteGameKey	@"favoriteGame"
#define kFavoriteExcuseKey	@"favoriteExcuse"
#define kFavoriteSinKey		@"favoriteSin"

#import <UIKit/UIKit.h>

@interface MainViewController : UIViewController {
	IBOutlet	UILabel *usernameLabel;
	IBOutlet	UILabel *passwordLabel;
	IBOutlet	UILabel *protocolLabel;
	IBOutlet	UILabel *warpDriveLabel;
	IBOutlet	UILabel *warpFactorLabel;
	
	IBOutlet	UILabel *favoriteTeaLabel;
	IBOutlet	UILabel *favoriteCandyLabel;
	IBOutlet	UILabel *favoriteGameLabel;
	IBOutlet	UILabel	*favoriteExcuseLabel;
	IBOutlet	UILabel *favoriteSinLabel;
}
@property (nonatomic, retain) UILabel *usernameLabel;
@property (nonatomic, retain) UILabel *passwordLabel;
@property (nonatomic, retain) UILabel *protocolLabel;
@property (nonatomic, retain) UILabel *warpDriveLabel;
@property (nonatomic, retain) UILabel *warpFactorLabel;

@property (nonatomic, retain) UILabel *favoriteTeaLabel;
@property (nonatomic, retain) UILabel *favoriteCandyLabel;
@property (nonatomic, retain) UILabel *favoriteGameLabel;
@property (nonatomic, retain) UILabel *favoriteExcuseLabel;
@property (nonatomic, retain) UILabel *favoriteSinLabel;

-(void)refreshFields;
@end