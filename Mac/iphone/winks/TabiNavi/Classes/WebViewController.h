//
//  WebViewController.h
//  TabiNavi
//
//  Created by luke on 7/5/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface WebViewController : UIViewController  <UITextFieldDelegate, UIWebViewDelegate>{
	UIWebView	*myWebView;
	UITextField *urlField;
}

@property (nonatomic, retain) IBOutlet UIWebView *myWebView;
@end

