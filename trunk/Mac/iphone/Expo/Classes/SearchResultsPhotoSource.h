//
//  SearchResultsPhotoSource.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"

@protocol SearchResultsModel;

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
