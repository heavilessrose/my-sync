//
//  pcTwoLineAlertView.h
//  Picoffee
//
//  Created by luke on 10-7-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface pcTwoLineAlertView : UIAlertView {
	UITextField *nameField;
	UITextField *passwordField;
	UITextField *passwordConfirmField;
	
	UILabel *nameLabel;
	UILabel *passwordLabel;
	UILabel *passwordConfirmLabel;
}

@end
