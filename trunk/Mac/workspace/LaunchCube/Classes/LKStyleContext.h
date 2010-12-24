//
//  LKStyleContext.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@class LKShape, LKStyle, LKStyleContext;

@protocol LKStyleDelegate <NSObject>
@optional

- (NSString *)textForLayerWithStyle:(LKStyle *)style;

- (UIImage *)imageForLayerWithStyle:(LKStyle *)style;

- (void)drawLayer:(LKStyleContext *)context withStyle:(LKStyle *)style;

@end

@interface LKStyleContext : NSObject {
	
	CGRect	frame;
	CGRect	contentFrame;
	
	LKShape	*shape;
	
	UIFont	*font;
	
	BOOL	didDrawContent;
	
	id<LKStyleDelegate> delegate;
}

@property (nonatomic)         CGRect    frame;
@property (nonatomic)         CGRect    contentFrame;
@property (nonatomic, retain) LKShape	*shape;
@property (nonatomic, retain) UIFont	*font;
@property (nonatomic)         BOOL      didDrawContent;

@property (nonatomic, assign) id<LKStyleDelegate> delegate;

@end
