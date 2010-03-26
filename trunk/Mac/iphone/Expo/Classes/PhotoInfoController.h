//
//  PhotoInfoController.h
//  Expo
//
//  Created by luke on 10-3-25.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Three20/Three20.h>
#import "SearchResultsPhotoSource.h"

typedef enum {
	PhotoTypeNone,
	PhotoTypeCountry,
	PhotoTypeStadium, // 场馆
} PhotoType;

/*
 * 单个缩略图的展示, 及动作响应.
 *	- 根据index取得图片(PhotoItem *), 此缩略图直接从缓存中得到(创建新的localPhoto对象)
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
