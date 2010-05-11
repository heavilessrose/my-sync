//
//  AccountViewDatasource.h
//  MSN
//
//  Created by  apple on 08-11-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface AccountViewDatasource : NSObject <UITableViewDataSource>
{
	NSMutableDictionary *source;
	NSString *selectedAccount;
}

@property (nonatomic, retain) NSMutableDictionary *source;
@property (nonatomic, retain) NSString *selectedAccount;
@end
