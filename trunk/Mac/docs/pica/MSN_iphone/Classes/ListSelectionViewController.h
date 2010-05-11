//
//  Created by wangwei on 8/16/08.
//  Copyright Beijing Mobile Security Technology Co., Ltd 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ListSelectionData : NSObject{
    NSMutableArray *data;
    NSString* title;
    int       selected;
}
@property (nonatomic, retain)NSMutableArray *data;
@property (nonatomic, retain)NSString* title;
@property (nonatomic, assign)int selected;

@end


@class MyUITextAndValueTableCell;

@interface ListSelectionViewController : UIViewController <UIScrollViewDelegate,UINavigationBarDelegate,
													  UITableViewDelegate, UITableViewDataSource>
{
	IBOutlet UITableView	*myTableView;	
    ListSelectionData* data;
}

@property (nonatomic, retain) UITableView *myTableView;
@property (nonatomic, retain) ListSelectionData  *data;

@end
