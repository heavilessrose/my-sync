//
//  BlocksTestViewController.m
//  BlocksTest
//
//  Created by luke on 10-9-14.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "BlocksTestViewController.h"
#import <unistd.h>

@implementation BlocksTestViewController

@synthesize imgv;

#pragma mark // 在方法执行完毕后completion block会被执行
- (void)animView
{
	CGRect cacheFrame = self.imgv.frame;
	CGFloat duration = 1.5f;
	[UIView animateWithDuration:duration animations:^ {
        CGRect newFrame = self.imgv.frame;
        newFrame.origin.y = newFrame.origin.y + 150.0;
        self.imgv.frame = newFrame;
        self.imgv.alpha = 0.2;
	} 
					 completion:^ (BOOL finished) {
                         if (finished) {
                             // Revert image view to original.
                             self.imgv.frame = cacheFrame;
                             self.imgv.alpha = 1.0;
                         }
						 [[NSNotificationCenter defaultCenter] postNotificationName:@"AnimViewOperationCompleted" 
																			 object:nil 
																		   userInfo:[NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:duration] forKey:@"AnimDuration"]];
                     }];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	self.imgv = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"test.png"]];
	[self.view addSubview:imgv];
	[imgv release];
	
	UIButton *animButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	animButton.frame = CGRectMake(150, 400, 50, 50);
	[animButton addTarget:self action:@selector(animView) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:animButton];
	
#pragma mark // implement the handler for a notification at the point you set up the observation
	[[NSNotificationCenter defaultCenter] addObserverForName:@"AnimViewOperationCompleted"
													  object:nil queue:[NSOperationQueue mainQueue]
												  usingBlock:^ (NSNotification *notif) {
													  NSNumber *theNum = [notif.userInfo objectForKey:@"AnimDuration"];
													  NSLog(@"AnimDuration: %f", [theNum floatValue]);
												  }];
	
#pragma mark -
#pragma mark // Processing enumerated arrays using two blocks
	NSString *area = @"Europe";
	NSArray *timeZoneNames = [NSTimeZone knownTimeZoneNames];
	NSMutableArray *areaArray = [NSMutableArray arrayWithCapacity:1];
	NSIndexSet *areaIndexes = [timeZoneNames indexesOfObjectsWithOptions:NSEnumerationConcurrent
															 passingTest:^ (id obj, NSUInteger idx, BOOL *stop) {
																 NSString *tmpStr = (NSString *)obj;
																 return [tmpStr hasPrefix:area];
															 }];
	
	NSArray *tmpArray = [timeZoneNames objectsAtIndexes:areaIndexes];
	[tmpArray enumerateObjectsWithOptions:NSEnumerationConcurrent|NSEnumerationReverse
							   usingBlock:^ (id obj, NSUInteger idx, BOOL *stop) {
								   [areaArray addObject:[obj substringFromIndex:[area length] + 1]];
							   }];
	NSLog(@"Cities in %@ time zone:%@", area, areaArray);
	
#pragma mark // Using a block to find matching substrings in a string
	NSString *musician = @"Beatles";
	NSString *musicDates = [NSString stringWithContentsOfFile:
							@"/usr/share/calendar/calendar.music"
													 encoding:NSASCIIStringEncoding error:NULL];
	//NSLog(@"%@", musicDates);
	[musicDates enumerateSubstringsInRange:NSMakeRange(0, [musicDates length]-1)
								   options:NSStringEnumerationByLines
								usingBlock:^(NSString *substring, NSRange substringRange, NSRange enclosingRange, BOOL *stop) {
									NSRange found = [substring rangeOfString:musician];
									if (found.location != NSNotFound) {
										NSLog(@"%@", substring);
									}
								}];
	
	
}

- (void)dealloc {
	[imgv release];
	
    [super dealloc];
}

@end
