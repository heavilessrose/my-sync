//
//  SearchResultsPhotoSource.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"

@protocol SearchResultsModel;

// 单个图片
@interface PhotoItem : NSObject <TTPhoto>
{
    NSString *caption;
	NSString *summary;
    NSString *imageURL;
    NSString *thumbnailURL;
    id <TTPhotoSource> photoSource;
    CGSize size;
    NSInteger index;
}
@property (nonatomic, retain) NSString *imageURL;
@property (nonatomic, retain) NSString *thumbnailURL;
@property (nonatomic, retain) NSString *summary;
+ (id)itemWithImageURL:(NSString*)imageURL thumbImageURL:(NSString*)thumbImageURL caption:(NSString*)caption summary:(NSString *)summary size:(CGSize)size;
@end

/*
	- 从服务器载入图片s (通过Objective-C forwarding委托给SearchResultsModel实现类).
	- 产生TTPhoto实例给photo browsing system.
	- 告知photo browsing system服务器有多少可用的图片.
 */
@interface SearchResultsPhotoSource : NSObject <TTPhotoSource> {
	
    id<SearchResultsModel> *model;
    
    // Backing storage for TTPhotoSource properties.
    NSString *albumTitle;
    int totalNumberOfPhotos;    
}

- (id)initWithModel:(id<SearchResultsModel>)theModel;    // Designated initializer.

- (id<SearchResultsModel>)underlyingModel;     // The model to which this PhotoSource forwards in order to conform to the TTModel protocol.

@end
