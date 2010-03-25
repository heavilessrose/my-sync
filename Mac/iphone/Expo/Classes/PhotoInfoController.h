//
//  PhotoInfoController.h
//  Expo
//
//  Created by luke on 10-3-25.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Three20/Three20.h>


typedef enum {
	PhotoTypeNone,
	PhotoTypeCountry, // 国家
	PhotoTypeStadium, // 场馆
} PhotoType;

@interface PhotoInfoController : TTViewController {
	PhotoType _contentType;
	NSInteger _index;
	NSString* _summary;
}

@property(nonatomic,copy) NSString* summary;

@end
