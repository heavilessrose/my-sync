/*

File: WebViewController.h
Abstract: The view controller for hosting the UIWebView feature of this sample.

Version: 1.7
*/

#import <UIKit/UIKit.h>

@interface WebViewController : UIViewController <UITextFieldDelegate, UIWebViewDelegate>
{
	UIWebView	*myWebView;
	UITextField *urlField;
}

@end
