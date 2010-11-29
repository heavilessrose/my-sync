//
//  LKStyledNode.h
//  LKStyledText
//
//  Created by luke on 10-11-29.
//  Copyright 2010 pica.com. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface LKStyledNode : NSObject {

	LKStyledNode *nextNode;
}

@property (nonatomic, retain) LKStyledNode *nextNode;
@end
