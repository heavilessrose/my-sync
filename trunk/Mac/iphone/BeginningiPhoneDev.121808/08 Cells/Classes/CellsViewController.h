//
//  CellsViewController.h
//  Cells
//
//  Created by Jeff LaMarche on 7/12/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#define kNameValueTag	1
#define kColorValueTag	2
#import <UIKit/UIKit.h>

@interface CellsViewController : UIViewController 
	<UITableViewDataSource, UITableViewDelegate>
{
	NSArray	*computers;
}
@property (nonatomic, retain) NSArray *computers;
@end

