//
//  LKTip.h
//  SK
//
//  Created by luke on 10-11-17.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface LKTip : NSObject {

	UIImage *image;
	NSString *message;
	NSTimeInterval time;
}

@property (nonatomic, retain) UIImage *image;
@property (nonatomic, retain) NSString *message;
@property (nonatomic, assign) NSTimeInterval time;

- (id)initWithMsg:(NSString *)msg img:(UIImage *)img durationTime:(NSTimeInterval)dTime;

@end
