//
//  LKStyleSheet.m
//  LaunchCube
//
//  Created by luke on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LKStyleSheet.h"
#import "LKDefaultStyleSheet.h"


static LKStyleSheet *gStyleSheet = nil;

@implementation LKStyleSheet


- (id)init {
	if (self = [super init]) {
		[[NSNotificationCenter defaultCenter] addObserver: self
												 selector: @selector(didReceiveMemoryWarning:)
													 name: UIApplicationDidReceiveMemoryWarningNotification
												   object: nil];
	}
	
	return self;
}

- (void)dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:UIApplicationDidReceiveMemoryWarningNotification
												  object: nil];
	[styles release];
	styles = nil;
	
	[super dealloc];
}

#pragma mark -

+ (LKStyleSheet *)globalStyleSheet {
	if (!gStyleSheet) {
		gStyleSheet = [[LKDefaultStyleSheet alloc] init];
	}
	return gStyleSheet;
}

+ (void)setGlobalStyleSheet:(LKStyleSheet *)styleSheet {
	[gStyleSheet release];
	gStyleSheet = [styleSheet retain];
}


#pragma mark -
#pragma mark NSNotifications

- (void)didReceiveMemoryWarning:(void *)object {
	[self freeMemory];
}

#pragma mark -
#pragma mark Public

- (LKStyle *)styleWithSelector:(NSString *)selector {
	return [self styleWithSelector:selector forState:UIControlStateNormal];
}

- (LKStyle *)styleWithSelector:(NSString *)selector forState:(UIControlState)state {
	
	NSString *key = state == UIControlStateNormal ? selector : [NSString stringWithFormat:@"%@%d", selector, state];
	LKStyle *style = [styles objectForKey:key];
	if (!style) {
		SEL sel = NSSelectorFromString(selector);
		if ([self respondsToSelector:sel]) {
			style = [self performSelector:sel withObject:(id)state];
			if (style) {
				if (!styles) {
					styles = [[NSMutableDictionary alloc] init];
				}
				[styles setObject:style forKey:key];
			}
		}
	}
	return style;
}

- (void)freeMemory {
	
	[styles release];
	styles = nil;
}

@end
