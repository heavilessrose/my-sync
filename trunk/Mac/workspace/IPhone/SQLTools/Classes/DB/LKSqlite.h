//
//  LKSqlite.h
//  SQLTools
//
//  Created by luke on 5/18/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
#import "/usr/include/sqlite3.h"

#define LKSqlite_TEXT  @"1"
#define LKSqlite_INT  @"2"
#define LKSqlite_BLOB  @"3"

@protocol LKSqliteDelegate <NSObject>

- (void)dbInitCallback;

@end


@interface LKSqlite : NSObject {
    NSString *table;
    sqlite3 *database;
}

@property(nonatomic, retain) NSString *table;

// Finalize
+ (void)finalizeStatements;
- (BOOL)closeDB;
// 建表
- (BOOL)createTable:(const char *)sql;
// 增
- (BOOL)addRecord:(NSDictionary *)recorditems;
// 删
- (BOOL)deleteRecord:(NSString *)where;
// 改
- (BOOL)updateRecord:(NSDictionary *)recorditems where:(NSString *)where;
// 查
- (BOOL)selectRecords:(NSMutableArray *)outArry wantColumns:(NSArray *)columns types:(NSArray *)colTypes wheres:(NSArray *)wheres;
@end
