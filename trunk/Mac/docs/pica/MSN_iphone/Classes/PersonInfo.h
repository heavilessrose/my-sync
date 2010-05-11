//
//  PersonInfo.h
//  msn_for_mac
//
//  Created by  apple on 08-9-19.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioServices.h>

//@class ListViewCell;
@class HttpClient;

@interface PersonInfo :NSObject
{
	NSString *jid;
	NSString *imid;
	NSString *nickname;	
	NSString *gbcode;
	NSString *localname;
	NSString *localgbcode;
	NSString *sync;
	NSString *block;
	NSString *group;
	NSString *impresa;
	NSString *portraithash;
	NSString *spaces;
	NSString *phone;
	NSString *state;
	NSString *portraitType; 
	NSString *portraitData;
	NSString *portraitURL;
	UIImage *image;
	UIImage *originImage;
	UIImage *spacesImage;
	BOOL spacesGleam;
	BOOL needPortraitData;
	BOOL needMakeImage;
//	ListViewCell *cell;
	NSTimer	*soundTimer;
	HttpClient *httpClient;
	SystemSoundID onlineSound;
}

+ (UIImage*)borgImage;
+ (UIImage*)onLineImage;
+ (UIImage*)offLineImage;
+ (void)releaseImages;

- (void)setPlaysoundFlag:(NSTimer*)theTimer;

- (id)initWithDictionary:(NSDictionary*)dict;

- (NSDictionary*)dictionaryRepresentation;

- (void)updateInfo:(PersonInfo*)thePersonInfo;
- (void)getImageByUrl;
- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict;

- (UIImage*)makeStatusImage:(UIImage*)theImage;
- (UIImage*)originImage;
- (UIImage*)smallImage;
- (UIImage*)spacesImage;
- (UIImage*)currentStatusImage;
- (NSString*)ex_nickname;

@property (nonatomic, retain) NSString *jid;
@property (nonatomic, retain) NSString *imid;
@property (nonatomic, retain) NSString *nickname;
@property (nonatomic, retain) NSString *gbcode;
@property (nonatomic, retain) NSString *localname;
@property (nonatomic, retain) NSString *localgbcode;
@property (nonatomic, retain) NSString *sync;
@property (nonatomic, retain) NSString *block;
@property (nonatomic, retain) NSString *impresa;
@property (nonatomic, retain) NSString *group;
@property (nonatomic, retain) NSString *portraithash;
@property (nonatomic, retain) NSString *spaces;
@property (nonatomic, retain) NSString *phone;
@property (nonatomic, retain) NSString *state;
@property (nonatomic, retain) NSString *portraitType;
@property (nonatomic, retain) NSString *portraitData;
@property (nonatomic, retain) NSString *portraitURL;
@property (nonatomic, retain) UIImage *image;
@property (nonatomic, retain) UIImage *originImage;
@property (nonatomic, retain) UIImage *spacesImage;
//@property (nonatomic, retain) ListViewCell *cell;
@property (nonatomic) BOOL spacesGleam;
@property (nonatomic, readonly) BOOL needPortraitData;
@property (nonatomic,retain) NSTimer *soundTimer;
@end
