//
//  RecommendCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "defines.h"


@interface RecommendCell : UITableViewCell {
	Product			item;
	UILabel			*title;
	UIImageView		*imgView;
}

@property (nonatomic, retain) IBOutlet UILabel			*title;
@property (nonatomic, retain) IBOutlet UIImageView		*imgView;
@property (nonatomic, assign) Product					item;
@end
