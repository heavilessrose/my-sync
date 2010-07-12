//
//  pcGroupsManagerTableCell.m
//  MSN
//
//  Created by  apple on 10-6-23.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "pcGroupsManagerTableCell.h"


@implementation pcGroupsManagerTableCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
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

#pragma mark override 
+ (NSString *)reuseIdentifier
{
    return (NSString *)TABLE_CELL_IDENTIFIER;
}
- (NSString *)reuseIdentifier
{
    return [[self class] reuseIdentifier];
}
@end
