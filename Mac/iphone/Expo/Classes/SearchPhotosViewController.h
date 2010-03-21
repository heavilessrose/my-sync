//
//  SearchPhotosViewController.h
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "Three20/Three20.h"

@class SearchResultsPhotoSource;


@interface SearchPhotosViewController : TTViewController {
	
    UITextField *queryField;
    SearchResultsPhotoSource *photoSource;
}

@end
