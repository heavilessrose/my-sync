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
	PhotoTypeCountry,
	PhotoTypeStadium, // 场馆
} PhotoType;

/*
 * 单个图片的展示, 及动作响应
 */
@interface PhotoInfoController : TTViewController {
	PhotoType _contentType;
	NSInteger _index; // index in result set
	NSString* _summary;
	//PhotoItem *_photo;
}

@property(nonatomic,copy) NSString* summary;
//@property(nonatomic,copy) PhotoItem *_photo;

@end
