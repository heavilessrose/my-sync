//
//  advertisement.h
//  MSN
//
//  Created by  apple on 09-3-10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HttpClient;
@class AdvertisementLabel;
@class AdvertisementView;
@interface Advertisement : NSObject <UIAlertViewDelegate>
{
	BOOL sale;
	NSInteger adId;
	NSInteger dbid;
	NSString *type;
	NSString *imageUrl;
	NSString *target;
	NSString *action;
	NSString *text;
	NSString *param;
	NSString *altText;
	BOOL sysDismiss;
	BOOL getImageData;
	HttpClient *httpClient;
	AdvertisementLabel *textLabel;
	AdvertisementView *imageView;
	UIAlertView *myAlertView;
}

- (id)initWithDictionary:(NSDictionary*)dict;
- (NSDictionary*)dictionaryRepresentation;

+ (void)saveImage;
- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict;
- (void)buttonClicked:(id)sender;
- (void)sendClickNote;

@property (nonatomic) BOOL sale;
@property (nonatomic) NSInteger adId;
@property (nonatomic) NSInteger dbid;
@property (nonatomic, retain) NSString *type;
@property (nonatomic, retain) NSString *imageUrl;
@property (nonatomic, retain) NSString *target;
@property (nonatomic, retain) NSString *action;
@property (nonatomic, retain) NSString *text;
@property (nonatomic, retain) NSString *param;
@property (nonatomic, retain) NSString *altText;
@property (nonatomic, retain) AdvertisementLabel *textLabel;
@property (nonatomic, retain) AdvertisementView *imageView;
@property (nonatomic, retain) UIAlertView *myAlertView;
@end
