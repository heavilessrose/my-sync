//
//  CellsAppDelegate.h
//  Cells
//
//  Created by Jeff LaMarche on 7/12/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CellsViewController;

@interface CellsAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet CellsViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) CellsViewController *viewController;

@end

