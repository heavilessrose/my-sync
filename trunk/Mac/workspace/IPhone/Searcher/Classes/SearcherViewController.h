//
//  SearcherViewController.h
//  Searcher
//
//  Created by luke on 10-9-20.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DataSource.h"


@interface SearcherViewController : UITableViewController {
	DataSource *datas;
}

@property (nonatomic, retain) IBOutlet DataSource *datas;
@end
