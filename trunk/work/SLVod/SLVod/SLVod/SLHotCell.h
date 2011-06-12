//
//  SLHotCell.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SLMovie.h"


@interface SLHotCell : UITableViewCell {
    UIImageView     *imageView;
    UILabel         *titleLabel;
    UILabel         *actorLabel;
    
    SLMovie         *movie;
}

@property (nonatomic, retain) SLMovie *movie;
@property (nonatomic, retain) IBOutlet UIImageView     *imageView;
@property (nonatomic, retain) IBOutlet UILabel         *titleLabel;
@property (nonatomic, retain) IBOutlet UILabel         *actorLabel;

@end
