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

//#import "tinyxml.h"
#import "rapidxml.hpp"
//#import "rapidxml_print.hpp"
//#import "rapidxml_iterators.hpp"
//#import "rapidxml_utils.hpp"

#import "SlimXml.h"

@interface FeaturedViewController : UITableViewController {
	NSArray					*entries; // the main data model for our UITableView
	RecommendCell	*tmpRecomCell;
	FeatureCell	*tmpFeatureCell;
}

@property (nonatomic, retain) NSArray			*entries;
@end
