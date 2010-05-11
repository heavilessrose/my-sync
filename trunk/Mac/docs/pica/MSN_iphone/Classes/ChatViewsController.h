//
//  ChatViewsController.h
//  MSN
//
//  Created by  apple on 08-10-8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TouchScrollView;
@class ChatViewData;

@interface ChatViewsController : UIViewController <UIScrollViewDelegate, UIAlertViewDelegate>
{
	UIPageControl *pageControl;
	TouchScrollView *picturesView; 
	UILabel *label; 
	UIButton *closeButton;
	NSMutableDictionary * chatViewDatas;
	int currentPage;
	BOOL sysDismiss;
    BOOL pageControlUsed;
	UIAlertView *myAlertView;
}	

- (void)refreshView;
- (IBAction)changePage:(id)sender; 
- (void)closeCurrentPage:(id)sender;
- (void)closeChatAtIndex:(int)index;
- (void)scrollViewToPage:(int)page;
- (void)setLabelText:(ChatViewData*)theController;
- (ChatViewData*)getControllerByTouchPoint:(NSNumber*)theXLocation;

@property (nonatomic, assign) int currentPage;
@property (nonatomic, assign) NSMutableDictionary *chatViewDatas;
@property (nonatomic, retain) UIAlertView *myAlertView;
@end
