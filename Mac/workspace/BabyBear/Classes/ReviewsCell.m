//
//  ReviewsCell.m
//  BabyBear
//
//  Created by luke on 10-9-10.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ReviewsCell.h"


@implementation ReviewsCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)dealloc {
    [super dealloc];
}


@end
