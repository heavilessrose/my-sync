//
//  FirstViewController.h
//  Navi1
//
//  Created by wang luke on 6/25/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface FirstViewController : UIViewController {
	IBOutlet UIButton *pressMe;
}


@property (nonatomic, retain) IBOutlet UIButton *pressMe;

- (IBAction)pressMe:(id)sender;
@end
