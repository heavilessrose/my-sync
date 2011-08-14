//
//  SLDownMovie.m
//  SLVod
//
//  Created by luke on 11-7-30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "SLDownMovie.h"
#import "SLMovie.h"
#import "LKTools.h"
#import "ASIHTTPRequest.h"

@implementation SLDownMovie

@synthesize downloadPercent, hash, size, path, movie, movieReq;
@synthesize indexInCell;

- (void)dealloc
{
    MLog(@"");
    [indexInCell release];
    [movie release];
    [hash release];
    [path release];
    [super dealloc];
}

- (id)initWithMovie:(SLMovie *)mov req:(ASIHTTPRequest *)req
{
    self = [super init];
    if (self) {
        self.movie = mov;
        self.movieReq = req;
        self.hash = keyForURL(mov.url);
    }
    
    return self;
}

#pragma mark - NSCoding

- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeObject:indexInCell forKey:@"indexInCell"];
    [aCoder encodeObject:hash forKey:@"hash"];
    [aCoder encodeInt64:size forKey:@"size"];
    [aCoder encodeObject:path forKey:@"path"];
    [aCoder encodeObject:movie forKey:@"movie"];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if ((self = [super init])) {
        indexInCell = [aDecoder decodeObjectForKey:@"indexInCell"];
        size = [aDecoder decodeInt64ForKey:@"size"];
        hash = [[aDecoder decodeObjectForKey:@"hash"] retain];
        path = [[aDecoder decodeObjectForKey:@"path"] retain];
        movie = [[aDecoder decodeObjectForKey:@"movie"] retain];
    }
    return self;
}

#pragma mark - 

- (NSString *)archivedPath:(NSString *)type
{
    NSString *wpath = nil;
    if ([type isEqualToString:SLDownloading_Key]) {
        NSError *err;
        [[NSFileManager defaultManager] createDirectoryAtPath:SLDownloading_Path withIntermediateDirectories:YES attributes:nil error:&err];
        wpath = [SLDownloading_Path stringByAppendingPathComponent:self.hash];
    }
    if ([type isEqualToString:SLDownloaded_Key]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:SLDownloaded_Path withIntermediateDirectories:YES attributes:nil error:nil];
        wpath = [SLDownloaded_Path stringByAppendingPathComponent:self.hash];
    }
    return wpath;
}

- (void)removeArchived:(NSString *)type
{
    NSString *wpath = [self archivedPath:type];
    
    NSError *err = nil;
    if (![[NSFileManager defaultManager] removeItemAtPath:wpath error:&err]) {
        DLog(@"%@", [err description]);
    }
}

- (BOOL)archive:(NSString *)type
{
    NSString *wpath = [self archivedPath:type];
    
    NSMutableData *aData = [[NSMutableData alloc] init];
    NSKeyedArchiver *archiver = [[NSKeyedArchiver alloc] initForWritingWithMutableData:aData];          
    [archiver encodeObject:self forKey:type];
    [archiver finishEncoding];
    
    
    if (![aData writeToFile:wpath atomically:YES]) {
		DLog(@"write file %@ fail", path);
		[archiver release];
		[aData release];
		return NO;
	}
    [archiver release];
    [aData release];
	DLog(@"write ok");
    return YES;
}

+ (SLDownMovie *)unarchive:(NSString *)file type:(NSString *)type
{
#if 1
    NSData *codedData = [[[NSData alloc] initWithContentsOfFile:file] autorelease];
    if (codedData == nil)
        return nil;
    
    NSKeyedUnarchiver *unarchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:codedData];
    SLDownMovie *aMov = [unarchiver decodeObjectForKey:type];
    [unarchiver finishDecoding];
    [unarchiver release];
#else 
    SLDownMovie *aMov = [NSKeyedUnarchiver unarchiveObjectWithFile:file];
#endif
	return aMov;
}



@end
