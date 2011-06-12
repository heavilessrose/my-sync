//
//  WBImageRecord.h
//  WBNetwork
//
//  Created by luke on 11-2-23.
//  Copyright 2011 58.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface LKImageRecord : NSObject {
	
	NSURL   *url;
	UIImage *img;
}

@property (nonatomic, retain) UIImage	*img;
@property (nonatomic, retain) NSURL     *url;

- (id)initWithUrl:(NSString *)urlStr;

@end
