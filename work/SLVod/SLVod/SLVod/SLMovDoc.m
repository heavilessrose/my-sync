//
//  SLMovDoc.m
//  SLVod
//
//  Created by luke on 11-8-13.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLMovDoc.h"
#import "SLDownloadManController.h"

#define kDataKey        @"Data"
#define kDataFile       @"data.plist"
#define kThumbImageFile @"thumbImage.png"
#define kFullImageFile  @"fullImage.png"

@implementation SLMovDoc
@synthesize data = _data;
@synthesize thumbImage = _thumbImage;
@synthesize fullImage = _fullImage;
@synthesize docPath = _docPath;

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (id)initWithDocPath:(NSString *)docPath {
    if ((self = [super init])) {
        _docPath = [docPath copy];
    }
    return self;
}

//- (id)initWithTitle:(NSString*)title rating:(float)rating thumbImage:(UIImage *)thumbImage fullImage:(UIImage *)fullImage {   
//    if ((self = [super init])) {
//        _data = [[ScaryBugData alloc] initWithTitle:title rating:rating];
//        _thumbImage = [thumbImage retain];
//        _fullImage = [fullImage retain];
//    }
//    return self;
//}
@end
