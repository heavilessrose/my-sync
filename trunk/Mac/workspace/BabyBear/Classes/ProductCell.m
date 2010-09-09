//
//  ProductCell.m
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ProductCell.h"


@implementation ProductCell

@synthesize product, delegate;

- (id)initWithStyle:(UITableViewCellStyle)style 
	reuseIdentifier:(NSString *)reuseIdentifier 
			product:(Product *)aProduct cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
		self.product = aProduct;
		self.textLabel.text = product.pname;
		self.detailTextLabel.text = product.pdesc;
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)dealloc
{
	[product release];
	
    [super dealloc];
}

- (void)refresh
{
	self.textLabel.text = product.pname;
	self.detailTextLabel.text = product.pdesc;
}

@end
