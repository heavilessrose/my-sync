//
//  SLRegController.h
//  SLVod
//
//  Created by luke on 11-8-14.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKViewController.h"
#import "ASIHTTPRequest.h"

@interface SLRegController : LKViewController <ASIHTTPRequestDelegate> {
    UITableView *table;
    NSString *username;
    NSString *pass;
    NSString *pass2;
    NSString *email;
    
    ASIHTTPRequest *loginReq;
}

@property (nonatomic, assign) ASIHTTPRequest *loginReq;
@property (nonatomic, retain) NSString *pass2;
@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) NSString *pass;
@property (nonatomic, retain) NSString *email;

@property (nonatomic, retain) IBOutlet UITableView *table;

- (void)reg;

@end
