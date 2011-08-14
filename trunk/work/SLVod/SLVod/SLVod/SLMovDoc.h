//
//  SLMovDoc.h
//  SLVod
//
//  Created by luke on 11-8-13.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SLDownMovie;
@interface SLMovDoc : NSObject {

    SLDownMovie *_data;
    UIImage *_thumbImage;
    UIImage *_fullImage;
    NSString *_docPath;    
}

@property (retain) SLDownMovie *data;
@property (retain) UIImage *thumbImage;
@property (retain) UIImage *fullImage;
@property (copy) NSString *docPath;

- (id)init;
- (id)initWithDocPath:(NSString *)docPath;
- (id)initWithTitle:(NSString*)title rating:(float)rating thumbImage:(UIImage *)thumbImage fullImage:(UIImage *)fullImage;
- (void)saveData;
- (void)saveImages;
- (void)deleteDoc;
@end
