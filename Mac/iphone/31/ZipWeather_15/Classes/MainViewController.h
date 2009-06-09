//
//  MainViewController.h
//  ZipWeather
//
//  Created by AppsAmuck on 10/15/08.
//  Copyright AppsAmuck LLC 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainViewController : UIViewController {
    IBOutlet UIWebView *webView;
    IBOutlet UITextField *textField;

}
- (IBAction)goClick;
@end
