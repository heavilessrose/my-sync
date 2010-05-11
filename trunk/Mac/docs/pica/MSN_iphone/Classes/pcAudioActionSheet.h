//
//  pcAudioActionSheet.h
//  textfieldAlertView
//
//  Created by  apple on 09-12-31.pcAudioActionSheet
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface pcAudioActionSheet : UIActionSheet {
	UILabel*			label;
	UIProgressView*		progress;
	UIButton*			leftButton;
	UIButton*			rightButton;
}

- (id)initWithTitle: (NSString*) aTitle
		   delegate: (id< UIActionSheetDelegate >) aDelegate
  cancelButtonTitle: (NSString*) aCancelButtonTitle
destructiveButtonTitle: (NSString*) aDestructiveButtonTitle
  otherButtonTitles: (NSString*)anOtherButtonTitles, ...;

- (void)setLabelText: (NSString*)aText;

- (IBAction)leftButtonClick: (id)sender;
- (IBAction)rightButtonClick: (id)sender;

@property (nonatomic, retain) UIProgressView* progress;
@property (nonatomic, retain) UIButton* leftButton;
@property (nonatomic, retain) UIButton* rightButton;

@end
