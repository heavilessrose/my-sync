//
//  LKStyle.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKStyle.h"
#import "LKPartStyle.h"

@implementation LKStyle

@synthesize next;

- (id)initWithNext:(LKStyle *)aNext {
	
	if (self = [super init]) {
		self.next = aNext;
	}
	
	return self;
}

- (id)init {
	if (self = [self initWithNext:nil]) {
	}
	
	return self;
}

- (void)dealloc {
	
	self.next = nil;
	[super dealloc];
}


#pragma mark -

- (LKStyle *)next:(LKStyle *)aNext {
	
	self.next = aNext;
	return self;
}

- (void)draw:(LKStyleContext *)aContext {
	[self.next draw:aContext];
}

- (UIEdgeInsets)addToInsets:(UIEdgeInsets)insets forSize:(CGSize)size {
	
	if (self.next) {
		return [self.next addToInsets:insets forSize:size];
		
	} else {
		return insets;
	}
}

- (CGSize)addToSize:(CGSize)size context:(LKStyleContext *)context {
	
	if (self.next) {
		return [self.next addToSize:size context:context];
		
	} else {
		return size;
	}
}

- (void)addStyle:(LKStyle *)style {
	
	if (self.next) {
		[self.next addStyle:style];
		
	} else {
		self.next = style;
	}
}

- (id)firstStyleOfClass:(Class)cls {
	
	if ([self isKindOfClass:cls]) {
		return self;
		
	} else {
		return [self.next firstStyleOfClass:cls];
	}
}

- (id)styleForPart:(NSString *)name {
	
	LKStyle *style = self;
	while (style) {
		if ([style isKindOfClass:[LKPartStyle class]]) {
			LKPartStyle *partStyle = (LKPartStyle *)style;
			if ([partStyle.name isEqualToString:name]) {
				return partStyle;
			}
		}
		style = style.next;
	}
	return nil;
}


@end
