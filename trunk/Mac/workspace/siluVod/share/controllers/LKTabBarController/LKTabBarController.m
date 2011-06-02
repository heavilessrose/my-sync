//
//  LKTabBarController.m
//  siluVod
//
//  Created by luke on 11-5-15.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "LKTabBarController.h"

//#define LKTabBar_SELECTED_VIEW_CONTROLLER_TAG 98456345

#define SELECTED_VIEW_CONTROLLER_TAG 98456345

static NSMutableArray *tabBarItems = nil;

@implementation LKTabBarController
@synthesize tabBar, delegate;


- (void)dealloc
{
    [tabBar release];
    [super dealloc];
    MLog(@"");
}

- (void)awakeFromNib
{
    [super awakeFromNib];
    
#if 0
    // test
    // Set up some fake view controllers each with a different background color so we can visually see the controllers getting swapped around
    UIViewController *detailController1 = [[[UIViewController alloc] init] autorelease];
    detailController1.view.backgroundColor = [UIColor redColor];
    UINavigationController *nav = [[[UINavigationController alloc] initWithRootViewController:detailController1] autorelease];
    
    UIViewController *detailController2 = [[[UIViewController alloc] init] autorelease];
    detailController2.view.backgroundColor = [UIColor greenColor];
    
    UIViewController *detailController3 = [[[UIViewController alloc] init] autorelease];
    detailController3.view.backgroundColor = [UIColor blueColor];
    
    UIViewController *detailController4 = [[[UIViewController alloc] init] autorelease];
    detailController4.view.backgroundColor = [UIColor cyanColor];
    
    UIViewController *detailController5 = [[[UIViewController alloc] init] autorelease];
    detailController5.view.backgroundColor = [UIColor purpleColor];
    
    tabBarItems = [[NSArray arrayWithObjects:
                    [NSDictionary dictionaryWithObjectsAndKeys:@"chat.png", @"image", detailController1, @"viewController", nav, @"nav", nil],
                    [NSDictionary dictionaryWithObjectsAndKeys:@"compose-at.png", @"image", detailController2, @"viewController", nil],
                    [NSDictionary dictionaryWithObjectsAndKeys:@"messages.png", @"image", detailController3, @"viewController", nil],
                    [NSDictionary dictionaryWithObjectsAndKeys:@"magnifying-glass.png", @"image", detailController4, @"viewController", nil],
                    [NSDictionary dictionaryWithObjectsAndKeys:@"more.png", @"image", detailController5, @"viewController", nil], nil] retain];
    
#else
    self.delegate = globalApp;
    NSInteger tabCount = 0;
    if (delegate && [delegate respondsToSelector:@selector(numberOfTabsInTabBar:)]) {
        tabCount = [delegate numberOfTabsInTabBar:self];
        tabBarItems = [[NSMutableArray arrayWithCapacity:tabCount] retain];
        if (delegate && [delegate respondsToSelector:@selector(tabBar:ctrlDicForIndexOfTab:)]) {
            for (int i = 0; i < tabCount; i++) {
                NSDictionary *aCtrlDic = [delegate tabBar:self ctrlDicForIndexOfTab:i];
                [tabBarItems addObject:aCtrlDic];
            }
        }
    } else {
        DLog(@"LKTabBarControllerDelegate not implemented");
    }
#endif
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Use the TabBarGradient image to figure out the tab bar's height (22x2=44)
    UIImage *tabBarGradient = [UIImage imageNamed:@"TabBarGradient.png"];
    
    // Create a custom tab bar passing in the number of items, the size of each item and setting ourself as the delegate
    CGSize aItemSize = CGSizeMake(self.view.frame.size.width/tabBarItems.count, tabBarGradient.size.height*2);
    self.tabBar = [[LKTabBar alloc] initWithItemCount:tabBarItems.count 
                                             itemSize:aItemSize 
                                                  tag:0 
                                             delegate:self];
    
    // Place the tab bar at the bottom of our view
    NSLog(@"%f", self.view.frame.size.height);
    CGRect tabBarFrame = CGRectMake(0,self.view.frame.size.height-(tabBarGradient.size.height*2),self.view.frame.size.width, tabBarGradient.size.height*2);
    [tabBar setFrame:tabBarFrame];
    [self.view addSubview:tabBar];
    [tabBar release];
    
    // test
    // Select the first tab
    [tabBar selectItemAtIndex:0];
    [self touchDownAtItemAtIndex:0];
}


#pragma mark -
#pragma mark LKTabBarDelegate

- (UIImage *)imageFor:(LKTabBar *)tabBar atIndex:(NSUInteger)itemIndex
{
    // Get the right data
    NSDictionary *data = [tabBarItems objectAtIndex:itemIndex];
    // Return the image for this tab bar item
    return [UIImage imageNamed:[data objectForKey:@"image"]];
}

- (UIImage *)backgroundImage
{
    // The tab bar's width is the same as our width
    CGFloat width = self.view.frame.size.width;
    // Get the image that will form the top of the background
    UIImage *topImage = [UIImage imageNamed:@"TabBarGradient.png"];
    
    // Create a new image context
    UIGraphicsBeginImageContextWithOptions(CGSizeMake(width, topImage.size.height*2), NO, 0.0);
    
    // Create a stretchable image for the top of the background and draw it
    UIImage *stretchedTopImage = [topImage stretchableImageWithLeftCapWidth:0 topCapHeight:0];
    [stretchedTopImage drawInRect:CGRectMake(0, 0, width, topImage.size.height)];
    
    // Draw a solid black color for the bottom of the background
    [[UIColor blackColor] set];
    CGContextFillRect(UIGraphicsGetCurrentContext(), CGRectMake(0, topImage.size.height, width, topImage.size.height));
    
    // Generate a new image
    UIImage *resultImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return resultImage;
}

// This is the blue background shown for selected tab bar items
- (UIImage *)selectedItemBackgroundImage
{
    return [UIImage imageNamed:@"TabBarItemSelectedBackground.png"];
}

// This is the glow image shown at the bottom of a tab bar to indicate there are new items
- (UIImage *)glowImage
{
    UIImage *tabBarGlow = [UIImage imageNamed:@"TabBarGlow.png"];
    
    // Create a new image using the TabBarGlow image but offset 4 pixels down
    UIGraphicsBeginImageContextWithOptions(CGSizeMake(tabBarGlow.size.width, tabBarGlow.size.height-4.0), NO, 0.0);
    
    // Draw the image
    [tabBarGlow drawAtPoint:CGPointZero];
    
    // Generate a new image
    UIImage *resultImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return resultImage;
}

// This is the embossed-like image shown around a selected tab bar item
- (UIImage *)selectedItemImage
{
    // Use the TabBarGradient image to figure out the tab bar's height (22x2=44)
    UIImage *tabBarGradient = [UIImage imageNamed:@"TabBarGradient.png"];
    CGSize tabBarItemSize = CGSizeMake(self.view.frame.size.width/tabBarItems.count, tabBarGradient.size.height*2);
    UIGraphicsBeginImageContextWithOptions(tabBarItemSize, NO, 0.0);
    
    // Create a stretchable image using the TabBarSelection image but offset 4 pixels down
    [[[UIImage imageNamed:@"TabBarSelection.png"] stretchableImageWithLeftCapWidth:4.0 topCapHeight:0] drawInRect:CGRectMake(0, 4.0, tabBarItemSize.width, tabBarItemSize.height-4.0)];  
    
    // Generate a new image
    UIImage *selectedItemImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return selectedItemImage;
}

- (UIImage *)tabBarArrowImage
{
    return [UIImage imageNamed:@"TabBarNipple.png"];
}

- (void)touchDownAtItemAtIndex:(NSUInteger)itemIndex
{
    // Remove the current view controller's view
    UIView *currentView = [self.view viewWithTag:SELECTED_VIEW_CONTROLLER_TAG];
    [currentView removeFromSuperview];
    
    // Get the right view controller
    NSDictionary *data = [tabBarItems objectAtIndex:itemIndex];
    UIViewController* viewController = [data objectForKey:@"viewController"];
    
    // Use the TabBarGradient image to figure out the tab bar's height (22x2=44)
    UIImage *tabBarGradient = [UIImage imageNamed:@"TabBarGradient.png"];
    
    // Set the view controller's frame to account for the tab bar
    viewController.view.frame = CGRectMake(0,0,self.view.frame.size.width, self.view.frame.size.height-(tabBarGradient.size.height*2));
    
    // Se the tag so we can find it later
    viewController.view.tag = SELECTED_VIEW_CONTROLLER_TAG;
    
    // Add the new view controller's view
    [self.view insertSubview:viewController.view belowSubview:tabBar];
    
    // test
    // In 1 second glow the selected tab
    [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(addGlowTimerFireMethod:) userInfo:[NSNumber numberWithInteger:itemIndex] repeats:NO];
}

// test
- (void)addGlowTimerFireMethod:(NSTimer *)theTimer
{
    // Remove the glow from all tab bar items
    for (NSUInteger i = 0; i < tabBarItems.count; i++)
    {
        [tabBar removeGlowAtIndex:i];
    }
    
    // Then add it to this tab bar item
    [tabBar glowItemAtIndex:[[theTimer userInfo] integerValue]];
}


#pragma mark - View lifecycle

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
