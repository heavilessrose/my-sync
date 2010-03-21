//
//  ForwardingAdapters.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"


@class SearchResultsPhotoSource;

@interface MyPhotoViewController : TTPhotoViewController
{
	id <TTModel> realModel;
}
@property (nonatomic,retain) id <TTModel> realModel;
@end


@interface MyThumbsViewController : TTThumbsViewController
{
	id <TTModel> realModel;
}
- (id)initForPhotoSource:(SearchResultsPhotoSource *)source;
@end


@interface MyThumbsDataSource : TTThumbsDataSource
{}
@end
