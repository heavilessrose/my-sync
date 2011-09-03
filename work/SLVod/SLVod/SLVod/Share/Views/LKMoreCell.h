//
//  LKMoreCell.h
//  SLVod
//
//  Created by luke on 11-7-24.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
    LKMoreCellState_Loading = 0,
    LKMoreCellState_Failed,
    LKMoreCellState_Loaded,
    LKMoreCellState_NoConn,
    LKMoreCellState_NoMore,
} LKMoreCellState;

@class LKMoreCell;
@protocol LKMoreCellDelegate <NSObject>

- (void)retryLoad:(LKMoreCell *)aMoreCell;

@end

@interface LKMoreCell : UITableViewCell {
    UIActivityIndicatorView *anim;
    UILabel                 *titleLabel;
    UIButton                *retryButton;
    
    LKMoreCellState         state;
    id <LKMoreCellDelegate> delegate;
    
    UITableView             *moreInTable;
}

@property (nonatomic, assign) UITableView             *moreInTable;
@property (nonatomic, assign) id <LKMoreCellDelegate> delegate;
@property (nonatomic, assign) LKMoreCellState         state;
@property (nonatomic, retain) IBOutlet UIActivityIndicatorView *anim;
@property (nonatomic, retain) IBOutlet UILabel                 *titleLabel;
@property (nonatomic, retain) IBOutlet UIButton                *retryButton;

- (void)startLoadMore;
- (void)loadFailed;
- (void)loadSuccessed;
- (void)nomore;
@end
