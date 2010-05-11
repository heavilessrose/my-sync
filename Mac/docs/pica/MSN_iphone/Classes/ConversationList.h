//
//  ConversationList.h
//  MSN
//
//  Created by  apple on 09-1-12.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ConversationList : UITableViewController
{
	NSString *imid;
	NSMutableArray *datasource;
}

@property(nonatomic,retain) NSMutableArray *datasource;
@property(nonatomic,retain) NSString *imid;

- (BOOL)removeConversation: (NSString*)aFileName imid: (NSString*)anImid;

@end
