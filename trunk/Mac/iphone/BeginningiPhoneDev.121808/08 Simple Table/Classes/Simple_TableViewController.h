//
//  Simple_TableViewController.h
//  Simple Table
//
//  Created by markd on 8/2/08.
//  Copyright Length-O-Words.com 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface Simple_TableViewController : UIViewController
	<UITableViewDelegate, UITableViewDataSource>
{
	NSArray *listData;
}
@property (nonatomic, retain) NSArray *listData;
@end


