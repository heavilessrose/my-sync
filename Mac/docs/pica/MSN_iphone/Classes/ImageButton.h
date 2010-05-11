//
//  ImageButton.h
//  MSN
//
//  Created by  apple on 08-10-23.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HttpClient;

@interface ImageButton : UIImageView
{
	HttpClient *httpClient;
}

- (void)updateButtonImage:(BOOL)urlChanged;

- (void)getImageByUrl:(NSString*)theUrl;
- (void)setButtonImage:(UIImage*)theImage;
- (void)callBackData:(NSData*)theData dictionary:(NSDictionary*)theDict;
@end
