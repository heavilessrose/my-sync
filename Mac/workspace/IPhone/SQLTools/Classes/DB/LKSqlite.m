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
#import "/usr/include/sqlite3.h"


#define kDbName "/var/mobile/Library/AddressBook/AddressBook.sqlitedb"

@implementation LKSqlite

- (BOOL)openDbFile:(NSString *)path
{
    sqlite3 *database;
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		NSLog(@"[contact]Failed to open DB--");
		return NO;
	}
    return YES;
}

@end
