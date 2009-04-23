//
//  practiceViewController.h
//  practice
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface practiceViewController : UIViewController {
	IBOutlet UILabel *statusText;
}

@property (nonatomic, retain) UILabel *statusText;

- (IBAction)buttonPressed:(id)sender;
@end

