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
	
	NSString *imageURLString;
	UIImage *picImage;
}

@property (nonatomic, retain) UIImage	*picImage;
@property (nonatomic, retain) NSString	*imageURLString;

@end
