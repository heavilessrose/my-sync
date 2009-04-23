//
//  controlFunViewController.h
//  controlFun
//
//  Created by wang luke on 4/23/09.
//  Copyright luke 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface controlFunViewController : UIViewController {
	IBOutlet UITextField *nameField;
	IBOutlet UITextField *numberField;
}

@property (nonatomic, retain) UITextField *nameField;
@property (nonatomic, retain) UITextField *numberField;

@end

