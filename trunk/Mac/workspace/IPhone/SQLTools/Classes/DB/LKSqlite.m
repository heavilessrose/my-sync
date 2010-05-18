//
//  LKSqlite.m
//  SQLTools
//
//  Created by luke on 5/18/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
#import "LKSqlite.h"

#import <stdio.h>
#import <string.h>
#import <stdlib.h>


#define kDbName "/var/mobile/Library/AddressBook/AddressBook.sqlitedb"

@interface LKSqlite (privateMethods)
- (BOOL)openDbFile:(NSString *)dbfileName ifexist:(BOOL *)isexisted;
@end


@implementation LKSqlite

#pragma mark --- privateMethods ---
// 不存在会自动创建
- (BOOL)openDbFile:(NSString *)dbfileName ifexist:(BOOL *)isexisted
{
	NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [documentPaths objectAtIndex:0];
    NSString *nsDbfilepath = [documentsDir stringByAppendingPathComponent:dbfileName];
    const char *dbfilePath = [nsDbfilepath UTF8String];
    printf("%s\r\n", dbfilePath);
    
    if([[NSFileManager defaultManager] fileExistsAtPath:nsDbfilepath]) {
        *isexisted = YES;
        NSLog(@"dbfile existed");
    } else {
        *isexisted = NO;
        NSLog(@"dbfile not existed");
    }

    
	if(sqlite3_open(dbfilePath, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		NSLog(@"[contact]Failed to open DB--");
		return NO;
	}
    return YES;
}

#pragma mark -
- (BOOL)prepareWithDbfile:(NSString *)dbfileName delegate:(id)delegate
{
    BOOL isexisted = NO;
    if([self openDbFile:dbfileName ifexist:&isexisted]) {
        if(!isexisted){
            [delegate dbInitCallback];
        }
    } else {
        NSLog(@"openDbFile: %@ fail", dbfileName);
        return NO;
    }
}

// 建表
- (BOOL)createTable:(const char *)sql
{
    NSLog(@"create table");
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(database, sql, strlen(sql), &statement, NULL) == SQLITE_OK){
		/*while(sqlite3_step(statement) == SQLITE_ROW) {
			contactCountsAll = sqlite3_column_int(statement, 0);
			if (contactCountsAll >= 0) {
				*phone_cnt = contactCountsAll;
			} else {
				*phone_cnt = 0;
			}
		}*/
        NSLog(@"create table ok");
		sqlite3_finalize(statement);
        sqlite3_close(database);
		return 0;
	} else {
        NSLog(@"create table fail");
    }

	return -1;
}

// 增
- (BOOL)addRecord:(NSDictionary *)recorditems
{
    
}

@end
