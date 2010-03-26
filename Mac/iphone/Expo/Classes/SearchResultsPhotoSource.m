//
//  SearchResultsPhotoSource.m
//  Expo
//
//  Created by luke on 10-3-21.
//  Copyright 2010 Luke. All rights reserved.
//

#import "SearchResultsPhotoSource.h"
#import "SearchResultsModel.h"
#import "SearchResult.h"


#pragma mark -
@implementation SearchResultsPhotoSource

@synthesize title = albumTitle;

- (id)initWithModel:(id <SearchResultsModel>)theModel
{
    if ((self = [super init])) {
        albumTitle = @"Photos";
        model = [theModel retain];
    }
    return self;
}

- (id<TTModel>)underlyingModel
{
    return model;
}

#pragma mark Forwarding

// Forward unknown messages to the underlying TTModel object.
- (void)forwardInvocation:(NSInvocation *)invocation
{
    if ([model respondsToSelector:[invocation selector]])
        [invocation invokeWithTarget:model];
    else
        [super forwardInvocation:invocation];
}

- (BOOL)respondsToSelector:(SEL)selector
{
    if ([super respondsToSelector:selector])
        return YES;
    else
        return [model respondsToSelector:selector];
}

- (BOOL)conformsToProtocol:(Protocol *)protocol
{
    if ([super conformsToProtocol:protocol])
        return YES;
    else
        return [model conformsToProtocol:protocol];
}

- (NSMethodSignature *)methodSignatureForSelector:(SEL)selector
{
    NSMethodSignature *signature = [super methodSignatureForSelector:selector];
    if (!signature)
        signature = [model methodSignatureForSelector:selector];
    return signature;
}

#pragma mark TTPhotoSource

- (NSInteger)numberOfPhotos 
{
    return [model totalResultsAvailableOnServer];
}

- (NSInteger)maxPhotoIndex
{
    return [[model results] count] - 1;
}

- (id<TTPhoto>)photoAtIndex:(NSInteger)index 
{
    if (index < 0 || index > [self maxPhotoIndex])
        return nil;
    
    // Construct an object (PhotoItem) that is suitable for Three20's
    // photo browsing system from the domain object (SearchResult)
    // at the specified index in the TTModel.
    SearchResult *result = [[model results] objectAtIndex:index];
    id<TTPhoto> photo = [PhotoItem itemWithImageURL:result.bigImageURL thumbImageURL:result.thumbnailURL caption:result.title summary:result.summary size:result.bigImageSize];
    photo.index = index;
    photo.photoSource = self;
    return photo;
}

#pragma mark -

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ delegates: %@", [super description], [self delegates]];
}

- (void)dealloc
{
    [model release];
    [albumTitle release];
    [super dealloc];
}

@end

#pragma mark -

@implementation PhotoItem

@synthesize caption, photoSource, size, index; // properties declared in the TTPhoto protocol
@synthesize imageURL, thumbnailURL, summary; // PhotoItem's own properties

+ (id)itemWithImageURL:(NSString*)theImageURL thumbImageURL:(NSString*)theThumbImageURL caption:(NSString*)theCaption summary:(NSString *)theSummary size:(CGSize)theSize
{
    PhotoItem *item = [[[[self class] alloc] init] autorelease];
    item.caption = theCaption;
	if (theSummary) {
		TTDINFO(@"add summary");
		item.summary = theSummary;
		//TTDINFO(@"SearchResultsPhotoSource::PhotoItem summary: %@", item.summary);
	}
    item.imageURL = theImageURL;
    item.thumbnailURL = theThumbImageURL;
    item.size = theSize;
    return item;
}

// ----------------------------------------------------------
#pragma mark TTPhoto protocol

- (NSString*)URLForVersion:(TTPhotoVersion)version
{
    return (version == TTPhotoVersionThumbnail && thumbnailURL) 
    ? thumbnailURL
    : imageURL;
}

#pragma mark -

- (void)dealloc
{
    [caption release];
	[summary release];
    [imageURL release];
    [thumbnailURL release];
    [super dealloc];
}

@end

