//
//  LKShape.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface LKShape : NSObject {

}

- (void)openPath:(CGRect)rect;
- (void)closePath:(CGRect)rect;

- (void)addTopEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource;
- (void)addRightEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource;
- (void)addBottomEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource;
- (void)addLeftEdgeToPath:(CGRect)rect lightSource:(NSInteger)lightSource;

/**
 * Opens the path, adds all edges, and closes the path.
 */
- (void)addToPath:(CGRect)rect;

- (void)addInverseToPath:(CGRect)rect;

- (UIEdgeInsets)insetsForSize:(CGSize)size;

@end
