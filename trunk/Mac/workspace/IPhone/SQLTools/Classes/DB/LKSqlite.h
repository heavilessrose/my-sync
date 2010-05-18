//
//  LKSqlite.h
//  SQLTools
//
//  Created by luke on 5/18/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
#import "/usr/include/sqlite3.h"

@protocol LKSqliteDelegate <NSObject>

- (void)dbInitCallback;

@end


@interface LKSqlite : NSObject {
    sqlite3 *database;
}
- (BOOL)createTable:(const char *)sql;
@end
