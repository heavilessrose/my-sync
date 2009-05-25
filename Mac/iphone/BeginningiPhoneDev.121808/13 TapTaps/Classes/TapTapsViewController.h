//
//  TapTapsViewController.h
//  TapTaps
//
//  Created by Jeff LaMarche on 8/3/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TapTapsViewController : UIViewController {
	IBOutlet	UILabel *singleLabel;
	IBOutlet	UILabel *doubleLabel;
	IBOutlet	UILabel *tripleLabel;
	IBOutlet	UILabel *quadrupleLabel;	
}
@property (nonatomic, retain) UILabel *singleLabel;
@property (nonatomic, retain) UILabel *doubleLabel;
@property (nonatomic, retain) UILabel *tripleLabel;
@property (nonatomic, retain) UILabel *quadrupleLabel;
- (void)singleTap;
- (void)doubleTap;
- (void)tripleTap;
- (void)quadrupleTap;
- (void)eraseMe:(UITextField *)textField ;
@end

