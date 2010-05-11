//
//  pcAlertView.h
//  textfieldAlertView
//
//  Created by  apple on 09-12-31.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface pcAlertView : UIAlertView {
	id				textDelegate;
	UITextField*	textField;
}
@property (nonatomic, retain) UITextField* textField;

- (void)setKeyboardType: (NSInteger)aType;

- (id)initWithTitle: (NSString *)aTitle
			message: (NSString *)aMessage
		   delegate: (id)aDelegate
  cancelButtonTitle: (NSString*)aCancelButtonTitle
  otherButtonTitles: (NSString*)anOtherButtonTitles, ...;
@end
