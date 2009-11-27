//
//  GifWebPlayer.h
//  WebGif
//
//  Created by luke on 09-11-27.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface GifWebPlayer : UIViewController <UIWebViewDelegate> {
	
	NSString *localHtml;
	UIWebView *webview;
}

@property (nonatomic, retain) UIWebView *webview;
@property (nonatomic, retain) NSString *localHtml;
@end
