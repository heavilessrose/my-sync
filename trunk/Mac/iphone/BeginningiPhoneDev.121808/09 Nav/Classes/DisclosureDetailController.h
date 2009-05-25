//
//  DisclosureDetailController.h
//  Nav
//
//  Created by Jeff LaMarche on 7/22/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DisclosureDetailController : UIViewController {
	IBOutlet	UILabel *label;
	NSString *message;
}
@property (nonatomic, retain) UILabel *label;
@property (nonatomic, retain) NSString *message;
@end
