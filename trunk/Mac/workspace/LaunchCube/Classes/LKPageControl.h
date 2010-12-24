//
//  LKPageControl.h
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface LKPageControl : UIControl {
	
	NSInteger	numberOfPages;
	NSInteger	currentPage;
	
	NSString	*dotStyle;
//	LKStyle		*normalDotStyle;
//	LKStyle		*currentDotStyle;
	
	BOOL		hidesForSinglePage;
}

@property (nonatomic)       NSInteger numberOfPages;
@property (nonatomic)       NSInteger currentPage;
@property (nonatomic, copy) NSString  *dotStyle;

/**
 * Set to YES to hide the pagecontrol if only one page is present
 *
 * @default NO
 */
@property (nonatomic)       BOOL      hidesForSinglePage;

@end
