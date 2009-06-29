//
//  FirstViewController.h
//  TabiNavi
//
//  Created by wang luke on 6/29/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface FirstViewController : UIViewController {
	IBOutlet UIButton *next;
}

@property (nonatomic, retain) IBOutlet UIButton *next;

- (IBAction)nextPressed:(id)sender;
@end
