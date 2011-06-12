//
//  LKViewController.h
//  siluVod
//
//  Created by luke on 11-6-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIViewController+iconImage.h"
#import "SBJson.h"

@interface LKViewController : UIViewController {
    
    NSMutableData   *jsonData;
    NSMutableArray  *movies;
}

@property (nonatomic, retain) NSMutableArray    *movies;
@property (nonatomic, retain) NSMutableData     *jsonData;

- (id)parse:(NSData *)theData;

@end
