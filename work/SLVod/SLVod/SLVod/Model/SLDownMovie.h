//
//  SLDownMovie.h
//  SLVod
//
//  Created by luke on 11-7-30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//


#define SLDownloading_Key          (@"SLDownloading")
#define SLDownloading_Path         ([docPath() stringByAppendingPathComponent:SLDownloading_Key])

#define SLDownloaded_Key          (@"SLDownloaded")
#define SLDownloaded_Path         ([docPath() stringByAppendingPathComponent:SLDownloaded_Key])

#define Make_SLDownMovie_Key        (self.hash)

@class SLMovie, ASIHTTPRequest;
@interface SLDownMovie : NSObject <NSCoding>
{
    int         downloadPercent;
    NSString    *hash;
    long long   *size;
    NSString    *path;
    SLMovie     *movie;
    ASIHTTPRequest *movieReq;
    
    NSIndexPath *indexInCell;
}


@property (nonatomic, retain) NSIndexPath *indexInCell;
@property (nonatomic, assign) ASIHTTPRequest *movieReq;
@property (nonatomic, retain) SLMovie     *movie;
@property (nonatomic, assign) int         downloadPercent;
@property (nonatomic, retain) NSString    *hash;
@property (nonatomic, assign) long long   *size;
@property (nonatomic, retain) NSString    *path;

- (id)initWithMovie:(SLMovie *)mov req:(ASIHTTPRequest *)req;

- (BOOL)archive:(NSString *)type;
- (NSString *)archivedPath:(NSString *)type;
+ (SLDownMovie *)unarchive:(NSString *)file type:(NSString *)type;
- (void)removeArchived:(NSString *)type;

@end
