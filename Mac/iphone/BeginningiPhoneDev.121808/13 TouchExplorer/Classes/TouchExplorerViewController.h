//
//  TouchExplorerViewController.h
//  TouchExplorer
//
//  Created by Jeff LaMarche on 8/2/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TouchExplorerViewController : UIViewController {
	IBOutlet	UILabel	*messageLabel;
	IBOutlet	UILabel	*tapsLabel;
	IBOutlet	UILabel	*touchesLabel;
}
@property (nonatomic, retain) UILabel *messageLabel;
@property (nonatomic, retain) UILabel *tapsLabel;
@property (nonatomic, retain) UILabel *touchesLabel;
- (void)updateLabelsFromTouches:(NSSet *)touches;
@end

