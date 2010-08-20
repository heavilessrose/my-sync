//
//  FeatureCell.h
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "defines.h"
#import "Product.h"


@interface FeatureCell : UITableViewCell {
	NSString	*name;
	NSString	*briefInfo;
	UIImage		*img;
	Product		*item;
}

@property (nonatomic, retain) IBOutlet NSString		*name;
@property (nonatomic, retain) IBOutlet NSString		*briefInfo;
@property (nonatomic, retain) IBOutlet UIImage		*img;
@property (nonatomic, assign) Product				*item;
@end
