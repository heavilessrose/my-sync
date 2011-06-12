//
//  SLMovInfoCell.h
//  SLVod
//
//  Created by luke on 11-6-12.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SLMovie.h"

@interface SLMovInfoCell : UITableViewCell {
    UILabel     *label;
    UITextView  *contentTextView;
    
    SLMovie     *movie;
}

@property (nonatomic, retain) SLMovie     *movie;
@property (nonatomic, retain) IBOutlet UILabel     *label;
@property (nonatomic, retain) IBOutlet UITextView  *contentTextView;

@end
