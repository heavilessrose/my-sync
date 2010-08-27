//
//  Utils.h
//  BabyBear
//
//  Created by luke on 10-8-17.
//  Copyright 2010 Luke. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kPath_img @"images"

typedef enum _file_type {
	FILE_IMG = 0,
	FILE_OTHER,
}FileType;

@interface Utils : NSObject {

}

+ (BOOL)isMultitasking;
+ (NSString *)writeData:(NSData *)data toFile:(NSString *)fName type:(FileType)type replaceExists:(BOOL)replace;

@end
