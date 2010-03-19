//
//  CountryController.h
//  Expo
//
//  Created by luke on 10-3-19.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Three20/Three20.h>

typedef enum {
	ContentTypeNone,
	ContentTypeCountry,
	ContentTypeCountryInfo,
	ContentTypeAbout,
	ContentTypeOrder,
} ContentType;

@interface CountryController : TTViewController {
	ContentType _contentType;
	NSString* _content;
	NSString* _text;
}

@property(nonatomic,copy) NSString* content;
@property(nonatomic,copy) NSString* text;

@end
