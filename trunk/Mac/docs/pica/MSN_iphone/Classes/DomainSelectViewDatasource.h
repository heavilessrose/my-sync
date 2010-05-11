//
//  DomainSelectViewDatasource.h
//  MSN
//
//  Created by Jun Du on 7/30/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface DomainSelectViewDatasource : NSObject <UITableViewDataSource>{
	NSString *selectedDomain;
}
@property(nonatomic,retain) NSString *selectedDomain;
@end
