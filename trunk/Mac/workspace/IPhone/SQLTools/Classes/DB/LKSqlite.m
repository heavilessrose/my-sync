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

//#define kdbName "/var/mobile/Library/AddressBook/AddressBook.sqlitedb"
#define kdb_phonebook_table_schema 
#define kdb_facebook_table_schema 
#define kdb_group_table_schema 
#define kdb_relation_table_schema 

static sqlite3_stmt *insert_statement = nil;
static sqlite3_stmt *select_statement = nil;
static sqlite3_stmt *delete_statement = nil;
static sqlite3_stmt *update_statement = nil;

@interface LKSqlite (privateMethods)
- (BOOL)openDbFile:(NSString *)dbfileName ifexist:(BOOL *)isexisted;
@end


@implementation LKSqlite
@synthesize table;

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

- (void)dealloc
{
    NSLog(@"LKSqlite dealloc =====");
    [table release];
    [self closeDB];
    [super dealloc];
    NSLog(@"LKSqlite dealloc over =====");
}

// Finalize (delete) all of the SQLite compiled queries.
+ (void)finalizeStatements
{
    if (insert_statement) {
        sqlite3_finalize(insert_statement);
        insert_statement = nil;
    }
    if (select_statement) {
        sqlite3_finalize(select_statement);
        select_statement = nil;
    }
    if (delete_statement) {
        sqlite3_finalize(delete_statement);
        delete_statement = nil;
    }
    if (update_statement) {
        sqlite3_finalize(update_statement);
        update_statement = nil;
    }
}

- (BOOL)prepareWithDbfile:(NSString *)dbfileName table:(NSString *)tablename delegate:(id)delegate
{
    BOOL isexisted = NO;
    if([self openDbFile:dbfileName ifexist:&isexisted]) {
        self.table = tablename;
        if(!isexisted){
            [delegate dbInitCallback];
        }
        return YES;
    } else {
        NSLog(@"openDbFile: %@ fail", dbfileName);
        return NO;
    }
}

- (BOOL)closeDB
{
    [LKSqlite finalizeStatements];
    if(sqlite3_close(database) != SQLITE_OK){
        NSLog(@"close database err");
        return NO;
    }
    return YES;
}

- (BOOL)createTable:(const char *)sql
{
    char *err = NULL;
    if(sqlite3_exec(database, sql, NULL, NULL, &err) != SQLITE_OK) {
        if(err){
            NSLog(@"create table err: %s", err);
            sqlite3_free(err);
            sqlite3_close(database);
        }
        return NO;
    }
	return YES;
}

- (BOOL)addRecord:(NSDictionary *)recorditems
{
    NSString *keys = @"";
    NSString *values = @"";
    int i = 0;
    
    if(recorditems == nil){
        return NO;
    }
    for (NSString *key in recorditems) {
        NSLog(@"key: %@, value: %@", key, [recorditems objectForKey:key]);
        if([recorditems count] == i+1) {
            keys = [keys stringByAppendingString: [NSString stringWithFormat:@"%@", key]];
            values = [values stringByAppendingString:[NSString stringWithFormat:@"%@", [recorditems objectForKey:key]]];
        } else {
            keys = [keys stringByAppendingString: [NSString stringWithFormat:@"%@, ", key]];
            values = [values stringByAppendingString:[NSString stringWithFormat:@"%@, ", [recorditems objectForKey:key]]];
        }
        i++;
    }
    NSLog(@" keys: %@\r\n values: %@", keys, values);
    NSString *nssql = [NSString stringWithFormat:
                       @"INSERT INTO %@ (%@) VALUES (%@)", table, keys, values];
    const char *sql = [nssql UTF8String];
    NSLog(@"%s", sql);
    if(insert_statement == nil){
        if(sqlite3_prepare_v2(database, sql, -1, &insert_statement, NULL) != SQLITE_OK)
            NSAssert1(0, @"[contact]Failed to prepare statement: %s", sqlite3_errmsg(database));
    }
    int success = sqlite3_step(insert_statement);
    // Because we want to reuse the statement, we "reset" it instead of "finalizing" it.
    sqlite3_reset(insert_statement);
    if (success == SQLITE_ERROR) {
        NSAssert1(0, @"Error: failed to insert: %s", sqlite3_errmsg(database));
    }
    return YES;
}

- (BOOL)updateRecord:(NSDictionary *)recorditems where:(NSString *)where
{
    NSString *sets = @"";
    NSString *wheres = @"";
    int i = 0;
    if (update_statement == nil) {
        for(NSString *key in recorditems){
            if([key compare:where] == NSOrderedSame){
                wheres = [wheres stringByAppendingString:[NSString stringWithFormat:@"%@=%@", key, [recorditems objectForKey:key]]];
            } else {
                if([recorditems count] == i+1) {
                    sets = [sets stringByAppendingString:[NSString stringWithFormat:@"%@=%@", key, [recorditems objectForKey:key]]];
                } else {
                    sets = [sets stringByAppendingString:[NSString stringWithFormat:@"%@=%@, ", key, [recorditems objectForKey:key]]];
                }
            }

            i++;
        }
        NSLog(@"sets: %@, where: %@", sets, wheres);
        NSString *nssql = [NSString stringWithFormat:@"UPDATE %@ SET %@ WHERE %@", table, sets, wheres];
        const char *sql = [nssql UTF8String];
        NSLog(@"%s", sql);
        if (sqlite3_prepare_v2(database, sql, -1, &update_statement, NULL) != SQLITE_OK) {
            NSAssert1(0, @"Error: failed to prepare statement: %s", sqlite3_errmsg(database));
        }
    }
    // Execute the query.
    int success = sqlite3_step(update_statement);
    // Reset the query for the next use.
    sqlite3_reset(update_statement);
    if (success != SQLITE_DONE) {
        NSAssert1(0, @"Error: failed to update: %s", sqlite3_errmsg(database));
    }
    return YES;
}

- (BOOL)deleteRecord:(NSString *)where
{
    if (delete_statement == nil) {
        NSString *nssql = [NSString stringWithFormat:@"DELETE FROM %@ WHERE %@", table, where];
        const char *sql = [nssql UTF8String];
        NSLog(@"%s", sql);
        if (sqlite3_prepare_v2(database, sql, -1, &delete_statement, NULL) != SQLITE_OK) {
            NSAssert1(0, @"Error: failed to prepare statement: %s", sqlite3_errmsg(database));
        }
    }
    int success = sqlite3_step(delete_statement);
    sqlite3_reset(delete_statement);
    if (success != SQLITE_DONE) {
        NSAssert1(0, @"Error: failed to delete: %s", sqlite3_errmsg(database));
    }
    return YES;
}

- (BOOL)selectRecords:(NSMutableArray *)outArry wantColumns:(NSArray *)columns types:(NSArray *)colTypes wheres:(NSArray *)wheres
{
    if(outArry == nil){
        return NO;
    }
    
    NSString *result_columns = @"";
    NSString *nswheres = @"";
    int i = 0;
    if (select_statement == nil) {
        for (NSString *column in columns) {
            if([columns count] == i+1){
                result_columns = [result_columns stringByAppendingString:[NSString stringWithFormat:@"%@", column]];
            } else {
                result_columns = [result_columns stringByAppendingString:[NSString stringWithFormat:@"%@, ", column]];
            }
            i++;
        }
        
        NSString *nssql = nil;
        if(wheres){
            i = 0;
            for (NSString *where in wheres) {
                if([wheres count] == i+1){
                    nswheres = [nswheres stringByAppendingString:[NSString stringWithFormat:@"%@", where]];
                } else {
                    nswheres = [nswheres stringByAppendingString:[NSString stringWithFormat:@"%@ AND ", where]];
                }
                i++;
            }
            nssql = [NSString stringWithFormat:@"SELECT %@ FROM %@ WHERE %@", result_columns, table, nswheres];
        } else {
            nssql = [NSString stringWithFormat:@"SELECT %@ FROM %@", result_columns, table];
        }

        const char *sql = [nssql UTF8String];
        NSLog(@"%s", sql);
        if (sqlite3_prepare_v2(database, sql, -1, &select_statement, NULL) != SQLITE_OK) {
            NSAssert1(0, @"Error: failed to prepare statement: %s", sqlite3_errmsg(database));
        }
        int colCount = [columns count];
        while(sqlite3_step(select_statement) == SQLITE_ROW) {
            NSMutableDictionary *aResult = [[NSMutableDictionary alloc] init];
            for(int j = 0; j < colCount; j++){
                NSString *coltype = [colTypes objectAtIndex:j];
                if([coltype compare:LKSqlite_TEXT] == NSOrderedSame){
                    const unsigned char *txt = sqlite3_column_text(select_statement, j);
                    [aResult setObject:[NSString stringWithCString:(const char*)txt] forKey:[columns objectAtIndex:j]];
                } else if([coltype compare:LKSqlite_INT] == NSOrderedSame) {
                    int integer = sqlite3_column_int(select_statement, j);
                    [aResult setObject:[NSString stringWithFormat:@"%d", integer] forKey:[columns objectAtIndex:j]];
                } else {
                    // BLOB
                    const void *data = sqlite3_column_blob(select_statement, j);
                    int len = sqlite3_column_bytes(select_statement, j);
                    [aResult setObject:[NSData dataWithBytes:data length:len] forKey:[columns objectAtIndex:j]];
                }
            }
            [outArry addObject:aResult];
            [aResult release];
        }
        sqlite3_reset(select_statement);
    }
    
    return YES;
}

@end
