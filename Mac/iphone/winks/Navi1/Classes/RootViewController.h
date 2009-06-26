//
//  RootViewController.h
//  Navi1
//
//  Created by wang luke on 6/25/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


/*!
    @class       RootViewController 
    @superclass  UIViewController {
    @abstract    navigationController的根视图控制器
    @discussion  <#(comprehensive description)#>
*/
@interface RootViewController : UIViewController {
	IBOutlet UIButton *pressMe;
}

@property (nonatomic, retain) IBOutlet UIButton *pressMe;

- (IBAction)pressMe:(id)sender;
@end
