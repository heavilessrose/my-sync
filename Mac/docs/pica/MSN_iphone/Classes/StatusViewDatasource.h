//
//  StatusViewDatasource.h
//  MSN
//
//  Created by  apple on 08-11-27.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface StatusViewDatasource : NSObject <UITableViewDataSource>
{
	NSString *selectedStatus;
}
@property(nonatomic,retain) NSString *selectedStatus;
@end
