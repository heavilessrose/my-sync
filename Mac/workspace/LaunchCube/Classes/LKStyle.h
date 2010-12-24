//
//  LKStyle.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@class LKStyleContext;
@interface LKStyle : NSObject {

	LKStyle *next;
}

@property (nonatomic, retain) LKStyle *next;

- (id)initWithNext:(LKStyle *)next;

- (LKStyle *)next:(LKStyle *)next;

- (void)draw:(LKStyleContext *)context;

- (UIEdgeInsets)addToInsets:(UIEdgeInsets)insets forSize:(CGSize)size;
- (CGSize)addToSize:(CGSize)size context:(LKStyleContext *)context;

- (void)addStyle:(LKStyle *)style;

- (id)firstStyleOfClass:(Class)cls;
- (id)styleForPart:(NSString *)name;

@end
