//
//  SLLoginController.h
//  SLVod
//
//  Created by luke on 11-8-14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKViewController.h"
#import "LKShadowTableView.h"
#import "ASIHTTPRequest.h"

@interface SLLoginController : LKViewController <ASIHTTPRequestDelegate> {
    UITableView *table;
    NSString *username;
    NSString *pass;
}

@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) NSString *pass;
@property (nonatomic, retain) IBOutlet UITableView *table;

@end
