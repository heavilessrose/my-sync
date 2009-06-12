//
//  DisclosureDetailController.h
//  FileBrowser
//
//  Created by wang luke on 6/12/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DisclosureDetailController : UIViewController {
	IBOutlet UILabel *label;
	NSString *message;
}
@property (nonatomic, retain) UILabel *label;
@property (nonatomic, retain) NSString *message;

@end
