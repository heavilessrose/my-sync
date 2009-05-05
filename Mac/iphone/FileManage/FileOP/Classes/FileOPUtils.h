//
//  FileOPUtils.h
//  FileOP
//
//  Created by wang luke on 5/5/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FileOPUtils : NSObject {

}

#pragma mark -
#pragma mark 实用方法
// 得到保存文件的目录绝对路径
+ (NSString *)dataFileDirectory;
// 得到要保存文件的完整路径
+ (NSString *)dataFileFullPath;
// 写文件
+ (BOOL)write:(NSData *)data asFile:(NSString *)fileName toPath:(NSString *)directory;
// 读文件
+ (BOOL)read:(NSString *)fileName inDirectory:(NSString *)directory to:(NSData *)data;
// 文件是否存在
+ (BOOL)exists:(NSString *)fileName inDirectory:(NSString *)directory;
// 文件的最后修改时间
+ (id)modifyDate:(NSString *)fileName inDirectory:(NSString *)directory;
// 文件的类型
+ (NSString *)fileType:(NSString *)fileName inDirectory:(NSString *)directory;
@end
