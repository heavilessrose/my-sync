//
//  FeaturedViewController.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RecommendCell.h"
#import "FeatureCell.h"

@interface FeaturedViewController : UITableViewController {
	NSArray					*entries; // the main data model for our UITableView
	IBOutlet RecommendCell	*tmpRecomCell;
	IBOutlet FeatureCell	*tmpFeatureCell;
}

@property (nonatomic, retain) NSArray			*entries;
@end
