//
//  RootViewController.h
//  SQLTools
//
//  Created by luke on 5/17/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "LKSqlite.h"

@interface RootViewController : UITableViewController <LKSqliteDelegate>{
    LKSqlite *sqlite;
}

@property(nonatomic, retain) LKSqlite *sqlite;
@end
