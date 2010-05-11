//
//  ImageLabel.h
//  MSN
//
//  Created by  apple on 08-10-31.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ImageLabel : UILabel
{
	BOOL findPressionString;
	int pressionIndex;
	NSRange findedRange;
	UIImage *pressionImage;
	NSArray *pressionStrings;
	NSString *currentString;
}

- (void)setLabel;
- (int)indexOfString:(NSString*)theString forWidth:(float)theWidth lineHeight:(float)theLineHeight;

@property (nonatomic,retain)UIImage *pressionImage;
@property (nonatomic,retain)NSArray *pressionStrings;
@property (nonatomic,retain)NSString *currentString;
@end
