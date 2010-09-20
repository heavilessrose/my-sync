//
//  DataSource.h
//  Searcher
//
//  Created by luke on 10-9-20.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SearcherViewController;


@interface DataSource : NSObject <UITableViewDataSource, UISearchDisplayDelegate, UISearchBarDelegate> {
	NSArray *listContent;
	SearcherViewController *tableDelegate;
	
	
	
	NSMutableArray	*filteredListContent;	// The content filtered as a result of a search.
	
	// The saved state of the search UI if a memory warning removed the view.
    NSString		*savedSearchTerm;
    NSInteger		savedScopeButtonIndex;
    BOOL			searchWasActive;
}

@property (nonatomic, retain) NSArray *listContent;
@property (nonatomic, assign) SearcherViewController *tableDelegate;




@property (nonatomic, retain) NSMutableArray *filteredListContent;

@property (nonatomic, copy) NSString *savedSearchTerm;
@property (nonatomic) NSInteger savedScopeButtonIndex;
@property (nonatomic) BOOL searchWasActive;

@end
