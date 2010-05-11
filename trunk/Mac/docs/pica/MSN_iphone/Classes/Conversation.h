//
//  conversation.h
//  MSN
//
//  Created by  apple on 09-1-12.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface Conversation : UITableViewController
{
	NSArray *datasource;
}

- (void)setDatasourceFile:(NSString*)theFile;
@property(nonatomic,retain) NSArray *datasource;
@end
